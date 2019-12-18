/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/window.h"

#include "slib/ui/view.h"
#include "slib/ui/screen.h"
#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/ui/resource.h"

#include "view_ios.h"

namespace slib
{
	namespace priv
	{
		namespace window
		{
			class iOS_WindowInstance;
		}
	}
}

@interface SLIBWindowRootViewController : UIViewController
{
	@public slib::WeakRef<slib::priv::window::iOS_WindowInstance> m_window;
	
	@public slib::UISize m_sizeClient;
	@public slib::UISize m_sizeClientResizedByKeyboard;
}
@end

namespace slib
{

	namespace priv
	{
		
		namespace platform
		{
			extern UIInterfaceOrientation g_screenOrientation;
		}
		
		using namespace platform;
		
		namespace window
		{
			
			__weak SLIBWindowRootViewController* g_currentRootController = nil;
			sl_bool g_flagSetStatusBarStyle = sl_false;
			StatusBarStyle g_currentStatusBarStyle = StatusBarStyle::Dark;
			
			__weak UIScrollView* g_keyboardCurrentScrollView = nil;
			CGRect g_keyboardCurrentScrollViewOriginalFrame;

			class iOS_WindowInstance : public WindowInstance
			{
			public:
				UIView* m_window;
				AtomicRef<ViewInstance> m_viewContent;
				
			public:
				iOS_WindowInstance()
				{
				}
				
				~iOS_WindowInstance()
				{
					release();
				}
				
			public:
				static Ref<iOS_WindowInstance> create(UIView* window)
				{
					if (window != nil) {
						Ref<iOS_WindowInstance> ret = new iOS_WindowInstance();
						if (ret.isNotNull()) {
							ret->m_window = window;
							UIView* view;
							if ([window isKindOfClass:[UIWindow class]]) {
								view = ((UIWindow*)window).rootViewController.view;
							} else {
								view = window;
							}
							if (view != nil) {
								Ref<ViewInstance> content = UIPlatform::createViewInstance(view);
								if (content.isNotNull()) {
									content->setWindowContent(sl_true);
									ret->m_viewContent = content;
									if ([view isKindOfClass:[SLIBViewHandle class]]) {
										((SLIBViewHandle*)view)->m_viewInstance = Ref<iOS_ViewInstance>::from(content);
									}
								}
							}
							UIPlatform::registerWindowInstance(window, ret.get());
							return ret;
						}
					}
					return sl_null;
				}
				
				static Ref<WindowInstance> create(const WindowInstanceParam& param)
				{
					UIRect screenFrame;
					Ref<Screen> _screen = param.screen;
					if (_screen.isNotNull()) {
						screenFrame = _screen->getRegion();
					} else {
						_screen = UI::getPrimaryScreen();
						if (_screen.isNotNull()) {
							screenFrame = _screen->getRegion();
						} else {
							screenFrame = UIRect::zero();
						}
					}
					UIRect _rect = param.calculateRegion(screenFrame);
					CGRect rect;
					CGFloat f = UIPlatform::getGlobalScaleFactor();
					rect.origin.x = (CGFloat)(_rect.left) / f;
					rect.origin.y = (CGFloat)(_rect.top) / f;
					rect.size.width = (CGFloat)(_rect.getWidth()) / f;
					rect.size.height = (CGFloat)(_rect.getHeight()) / f;
					
					UIWindow* window;
					sl_bool flagMainWindow = sl_false;
					static sl_bool flagFirstWindow = sl_true;
					if (flagFirstWindow) {
						window = UIPlatform::getMainWindow();
						flagFirstWindow = sl_false;
						flagMainWindow = sl_true;
					} else {
						window = [[UIWindow alloc] initWithFrame:rect];
					}
					if (window != nil) {
#ifndef SLIB_PLATFORM_IS_IOS_CATALYST
						if (!flagMainWindow) {
							UIScreen* screen = UIPlatform::getScreenHandle(_screen.get());
							if (screen != nil) {
								window.screen = screen;
							}
							window.windowLevel = UIWindowLevelNormal + 1;
						}
#endif
						SLIBWindowRootViewController* controller = [[SLIBWindowRootViewController alloc] init];
						if (controller != nil) {
							controller->m_sizeClient = _rect.getSize();
							controller->m_sizeClientResizedByKeyboard = controller->m_sizeClient;
							SLIBViewHandle* view = [[SLIBViewHandle alloc] init];
							if (view != nil) {
								view.opaque = NO;
								controller.view = view;
								window.rootViewController = controller;
								Ref<iOS_WindowInstance> ret = create(window);
								if (ret.isNotNull()) {
									controller->m_window = ret;
									ret->activate();
									return ret;
								}
							}
						}
					}
					return sl_null;
				}
				
				void release()
				{
					UIView* window = m_window;
					if (window != nil) {
						UIPlatform::removeWindowInstance(window);
					}
					if (!([window isKindOfClass:[UIWindow class]])) {
						[window removeFromSuperview];
					}
					m_viewContent.setNull();
					m_window = nil;
				}
				
				void close() override
				{
					UIView* view = m_window;
					if (view != nil) {
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							window.hidden = TRUE;
						} else {
							[view removeFromSuperview];
						}
					}
					m_window = nil;
					m_viewContent.setNull();
				}
				
				sl_bool isClosed() override
				{
					return m_window == nil;
				}
				
				void setParent(const Ref<WindowInstance>& window) override
				{
				}
				
				Ref<ViewInstance> getContentView() override
				{
					return m_viewContent;
				}
				
				sl_bool isActive() override
				{
					UIView* view = m_window;
					if (view != nil) {
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							return [window isKeyWindow];
						} else {
							return [view isFirstResponder];
						}
					}
					return sl_false;
				}
				
				void activate() override
				{
					UIView* view = m_window;
					if (view != nil) {
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							[window makeKeyAndVisible];
						} else {
							[view becomeFirstResponder];
						}
					}
				}
				
				UIRect getFrame() override
				{
					UIView* window = m_window;
					if (window != nil) {
						CGRect rect = [window frame];
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						UIRect frame;
						frame.left = (sl_ui_pos)(rect.origin.x * f);
						frame.top = (sl_ui_pos)(rect.origin.y * f);
						frame.right = (sl_ui_pos)((rect.origin.x + rect.size.width) * f);
						frame.bottom = (sl_ui_pos)((rect.origin.y + rect.size.height) * f);
						return frame;
					} else {
						return UIRect::zero();
					}
				}
				
				void setFrame(const UIRect& frame) override
				{
					UIView* window = m_window;
					if (window != nil) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						CGRect rect;
						rect.origin.x = (CGFloat)(frame.left) / f;
						rect.origin.y = (CGFloat)(frame.top) / f;
						rect.size.width = (CGFloat)(frame.getWidth()) / f;
						rect.size.height = (CGFloat)(frame.getHeight()) / f;
						[window setFrame:rect];
						if ([window isKindOfClass:[UIWindow class]]) {
							UIViewController* controller = ((UIWindow*)window).rootViewController;
							if (controller != nil && [controller isKindOfClass:[SLIBWindowRootViewController class]]) {
								UISize size = frame.getSize();
								((SLIBWindowRootViewController*)controller)->m_sizeClient = size;
								((SLIBWindowRootViewController*)controller)->m_sizeClientResizedByKeyboard = size;
							}
						}
					}
				}
				
				UIRect getClientFrame() override
				{
					return getFrame();
				}
				
				UISize getClientSize() override
				{
					UIView* window = m_window;
					if (window != nil) {
						if ([window isKindOfClass:[UIWindow class]]) {
							UIViewController* controller = ((UIWindow*)window).rootViewController;
							if (controller != nil && [controller isKindOfClass:[SLIBWindowRootViewController class]]) {
								return ((SLIBWindowRootViewController*)controller)->m_sizeClientResizedByKeyboard;
							}
						}
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						CGRect rect = [window frame];
						UISize ret;
						ret.x = (sl_ui_pos)(rect.size.width * f);
						ret.y = (sl_ui_pos)(rect.size.height * f);
						return ret;
					} else {
						return UISize::zero();
					}
				}
				
				sl_bool setClientSize(const UISize& size) override
				{
					UIView* window = m_window;
					if (window != nil) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						CGRect frame = [window frame];
						frame.size.width = (CGFloat)(size.x) / f;
						frame.size.height = (CGFloat)(size.y) / f;
						[window setFrame:frame];
						if ([window isKindOfClass:[UIWindow class]]) {
							UIViewController* controller = ((UIWindow*)window).rootViewController;
							if (controller != nil && [controller isKindOfClass:[SLIBWindowRootViewController class]]) {
								((SLIBWindowRootViewController*)controller)->m_sizeClient = size;
								((SLIBWindowRootViewController*)controller)->m_sizeClientResizedByKeyboard = size;
							}
						}
						return sl_true;
					}
					return sl_false;
				}
				
				void setTitle(const String& title) override
				{
				}
				
				void setBackgroundColor(const Color& _color) override
				{
					UIView* window = m_window;
					if (window != nil) {
						UIColor* color;
						if (_color.isZero()) {
							color = nil;
						} else {
							color = GraphicsPlatform::getUIColorFromColor(_color);
						}
						[window setBackgroundColor:color];
					}
				}
				
				void setVisible(sl_bool flag) override
				{
					UIView* window = m_window;
					if (window != nil) {
						[window setHidden:(flag ? NO : YES)];
					}
				}
				
				void setAlwaysOnTop(sl_bool flag) override
				{
					UIView* view = m_window;
					if (view != nil) {
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							if (flag) {
								window.windowLevel = UIWindowLevelAlert + 1;
							} else {
								window.windowLevel = UIWindowLevelNormal + 1;
							}
						}
					}
				}
				
				void setAlpha(sl_real _alpha) override
				{
					UIView* window = m_window;
					if (window != nil) {
						sl_real alpha = _alpha;
						if (alpha < 0) {
							alpha = 0;
						}
						if (alpha > 1) {
							alpha = 1;
						}
						window.alpha = alpha;
					}
				}
				
				UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen) override
				{
					UIView* view = m_window;
					if (view != nil) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							CGPoint pt;
							pt.x = (CGFloat)(ptScreen.x) / f;
							pt.y = (CGFloat)(ptScreen.y) / f;
							pt = [window convertPoint:pt fromWindow:nil];
							UIPointf ret;
							ret.x = (sl_ui_posf)(pt.x * f);
							ret.y = (sl_ui_posf)(pt.y * f);
							return ret;
						} else {
							UIWindow* window = [view window];
							if (window != nil) {
								CGPoint pt;
								pt.x = (CGFloat)(ptScreen.x) / f;
								pt.y = (CGFloat)(ptScreen.y) / f;
								pt = [window convertPoint:pt fromWindow:nil];
								pt = [window convertPoint:pt toView:view];
								UIPointf ret;
								ret.x = (sl_ui_posf)(pt.x * f);
								ret.y = (sl_ui_posf)(pt.y * f);
								return ret;
							}
						}
					}
					return ptScreen;
				}
				
				UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow) override
				{
					UIView* view = m_window;
					if (view != nil) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							CGPoint pt;
							pt.x = (CGFloat)(ptWindow.x) / f;
							pt.y = (CGFloat)(ptWindow.y) / f;
							pt = [window convertPoint:pt toWindow:nil];
							UIPointf ret;
							ret.x = (sl_ui_posf)(pt.x * f);
							ret.y = (sl_ui_posf)(pt.y * f);
							return ret;
						} else {
							UIWindow* window = [view window];
							if (window != nil) {
								CGPoint pt;
								pt.x = (CGFloat)(ptWindow.x) / f;
								pt.y = (CGFloat)(ptWindow.y) / f;
								pt = [window convertPoint:pt fromView:view];
								pt = [window convertPoint:pt toWindow:nil];
								UIPointf ret;
								ret.x = (sl_ui_posf)(pt.x * f);
								ret.y = (sl_ui_posf)(pt.y * f);
								return ret;
							}
						}
					}
					return ptWindow;
				}
				
				UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen) override
				{
					return convertCoordinateFromScreenToWindow(ptScreen);
				}
				
				UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient) override
				{
					return convertCoordinateFromWindowToScreen(ptClient);
				}
				
				UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow) override
				{
					return ptWindow;
				}
				
				UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient) override
				{
					return ptClient;
				}
				
				UISize getWindowSizeFromClientSize(const UISize& sizeClient) override
				{
					return sizeClient;
				}
				
				UISize getClientSizeFromWindowSize(const UISize& sizeWindow) override
				{
					return sizeWindow;
				}
				
			};
			
			void ResetOrientation()
			{
#ifndef SLIB_PLATFORM_IS_IOS_CATALYST
				g_screenOrientation = [[UIApplication sharedApplication] statusBarOrientation];
#endif
			}
			
			ScreenOrientation ConvertScreenOrientation(UIInterfaceOrientation orientation)
			{
				switch (orientation) {
					case UIInterfaceOrientationPortraitUpsideDown:
						return ScreenOrientation::PortraitUpsideDown;
					case UIInterfaceOrientationLandscapeLeft:
						return ScreenOrientation::LandscapeLeft;
					case UIInterfaceOrientationLandscapeRight:
						return ScreenOrientation::LandscapeRight;
					default:
						return ScreenOrientation::Portrait;
				}
			}
			
			UIInterfaceOrientation ConvertScreenOrientation(ScreenOrientation orientation)
			{
				switch (orientation) {
					case ScreenOrientation::PortraitUpsideDown:
						return UIInterfaceOrientationPortraitUpsideDown;
					case ScreenOrientation::LandscapeLeft:
						return UIInterfaceOrientationLandscapeLeft;
					case ScreenOrientation::LandscapeRight:
						return UIInterfaceOrientationLandscapeRight;
					default:
						return UIInterfaceOrientationPortrait;
				}
			}
			
		}
	}

	using namespace priv::platform;
	using namespace priv::window;

	Ref<WindowInstance> Window::createWindowInstance(const WindowInstanceParam& param)
	{
		return iOS_WindowInstance::create(param);
	}
	
	Ref<Window> Window::getActiveWindow()
	{
		UIView* handle = UIPlatform::getKeyWindow();
		if (handle != nil) {
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(handle);
			if (instance.isNotNull()) {
				return instance->getWindow();
			}
		}
		return sl_null;
	}
	
	
	ScreenOrientation UI::getScreenOrientation()
	{
		switch (g_screenOrientation) {
			case UIInterfaceOrientationPortrait:
				return ScreenOrientation::Portrait;
			case UIInterfaceOrientationPortraitUpsideDown:
				return ScreenOrientation::PortraitUpsideDown;
			case UIInterfaceOrientationLandscapeLeft:
				return ScreenOrientation::LandscapeLeft;
			case UIInterfaceOrientationLandscapeRight:
				return ScreenOrientation::LandscapeRight;
			default:
				break;
		}
		return ConvertScreenOrientation(g_screenOrientation);
	}
	
	void UI::attemptRotateScreenOrientation()
	{
		[UIViewController attemptRotationToDeviceOrientation];
#ifndef SLIB_PLATFORM_IS_IOS_CATALYST
		List<ScreenOrientation> orientations(UI::getAvailableScreenOrientations());
		if (orientations.isEmpty()) {
			return;
		}
		if (orientations.contains(ConvertScreenOrientation([[UIApplication sharedApplication] statusBarOrientation]))) {
			return;
		}
		ScreenOrientation orientation = orientations.getValueAt(0, ScreenOrientation::Portrait);
		@try {
			[[UIDevice currentDevice] setValue:[NSNumber numberWithInteger:ConvertScreenOrientation(orientation)] forKey:@"orientation"];
		} @catch (NSError* error) {
			NSLog(@"[Error] %@", error.localizedDescription);
		}
#endif
	}

	void UI::setStatusBarStyle(StatusBarStyle style)
	{
		g_flagSetStatusBarStyle = sl_true;
		g_currentStatusBarStyle = style;
		SLIBWindowRootViewController* controller = g_currentRootController;
		if (controller != nil) {
			[controller setNeedsStatusBarAppearanceUpdate];
		}
		UIResource::updateDefaultScreenSize();
	}
	
	
	Ref<WindowInstance> UIPlatform::createWindowInstance(UIView* window)
	{
		Ref<WindowInstance> ret = UIPlatform::_getWindowInstance((__bridge void*)window);
		if (ret.isNotNull()) {
			return ret;
		}
		return iOS_WindowInstance::create(window);
	}
	
	void UIPlatform::registerWindowInstance(UIView* window, WindowInstance* instance)
	{
		UIPlatform::_registerWindowInstance((__bridge void*)window, instance);
	}
	
	Ref<WindowInstance> UIPlatform::getWindowInstance(UIView* window)
	{
		return UIPlatform::_getWindowInstance((__bridge void*)window);
	}
	
	void UIPlatform::removeWindowInstance(UIView* window)
	{
		UIPlatform::_removeWindowInstance((__bridge void*)window);
	}
	
	UIView* UIPlatform::getWindowHandle(WindowInstance* instance)
	{
		iOS_WindowInstance* window = (iOS_WindowInstance*)instance;
		if (window) {
			return window->m_window;
		} else {
			return nil;
		}
	}

	UIWindow* UIPlatform::getKeyWindow()
	{
#ifndef SLIB_PLATFORM_IS_IOS_CATALYST
		UIWindow* window = [[UIApplication sharedApplication] keyWindow];
		if (window != nil) {
			return window;
		}
#endif
		return UIPlatform::getMainWindow();
	}
	
}

using namespace slib;
using namespace slib::priv::window;

@implementation SLIBWindowRootViewController

- (id)init
{
	self = [super init];
	if (self != nil) {
		self->m_sizeClient = UISize::zero();
		self->m_sizeClientResizedByKeyboard = UISize::zero();
	}
	return self;
}

- (BOOL)prefersStatusBarHidden
{
	if (g_flagSetStatusBarStyle) {
		if (g_currentStatusBarStyle == StatusBarStyle::Hidden) {
			return YES;
		}
		return NO;
	}
	return [super prefersStatusBarHidden];
}

- (UIStatusBarStyle)preferredStatusBarStyle
{
	if (g_flagSetStatusBarStyle) {
		if (g_currentStatusBarStyle == StatusBarStyle::Light) {
			return UIStatusBarStyleLightContent;
		}
		return UIStatusBarStyleDefault;
	}
	return [super preferredStatusBarStyle];
}

- (BOOL)shouldAutorotate
{
	return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
	return UIInterfaceOrientationMaskAll;
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	g_currentRootController = self;
	if (g_flagSetStatusBarStyle) {
		[self setNeedsStatusBarAppearanceUpdate];
	}
	Ref<iOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		UIView* handle = window->m_window;
		if (handle != nil) {
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			CGSize _size = handle.frame.size;
			UISize size;
			size.x = (sl_ui_pos)(_size.width * f);
			size.y = (sl_ui_pos)(_size.height * f);
			self->m_sizeClient = size;
			self->m_sizeClientResizedByKeyboard = size;
			window->onResize(size.x, size.y);
		}
	}
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
	
	dispatch_async(dispatch_get_main_queue(), ^{
		ResetOrientation();
		UIWindow* handle = self.view.window;
		if (handle != nil) {
			CGRect r = handle.frame;
			r.size = size;
			handle.frame = r;
		}
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		UISize sizeContent;
		sizeContent.x = (sl_ui_pos)(size.width * f);
		sizeContent.y = (sl_ui_pos)(size.height * f);
		self->m_sizeClient = sizeContent;
		self->m_sizeClientResizedByKeyboard = sizeContent;
		Ref<iOS_WindowInstance> window = self->m_window;
		if (window.isNotNull()) {
			window->onResize(sizeContent.x, sizeContent.y);
		}
	});
}

-(UIView*)findFirstResponderText
{
	UIView* view = UIPlatform::findFirstResponder(self.view);
	if (view != nil) {
		if ([view isKindOfClass:[UITextField class]] || [view isKindOfClass:[UITextView class]]) {
			return view;
		}
	}
	return nil;
}

-(void)restoreKeyboardScrollView
{
	if (g_keyboardCurrentScrollView != nil) {
		g_keyboardCurrentScrollView.frame = g_keyboardCurrentScrollViewOriginalFrame;
		g_keyboardCurrentScrollView = nil;
	}
}

-(void)keyboardWillShow : (NSNotification*)aNotification
{
	UIView* view = [self findFirstResponderText];
	if (view == nil) {
		[self restoreKeyboardScrollView];
		return;
	}
	
	UIScrollView* scroll = nil;
	UIView* parent = view;
	while (parent != nil) {
		if ([parent isKindOfClass:[UIScrollView class]] && ![parent isKindOfClass:[UITextView class]]) {
			scroll = (UIScrollView*)parent;
			break;
		}
		parent = parent.superview;
	}

	if (scroll != g_keyboardCurrentScrollView) {
		[self restoreKeyboardScrollView];
	}
	
	UIKeyboardAdjustMode adjustMode = UI::getKeyboardAdjustMode();
	Ref<iOS_WindowInstance> window = self->m_window;
	if (window.isNotNull()) {
		if (adjustMode == UIKeyboardAdjustMode::Resize) {
			NSDictionary* info = [aNotification userInfo];
			sl_ui_len heightKeyboard = (sl_ui_len)([[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size.height * UIPlatform::getGlobalScaleFactor());
			UISize size = self->m_sizeClient;
			size.y -= heightKeyboard;
			self->m_sizeClientResizedByKeyboard = size;
			window->onResize(size.x, size.y);
		} else {
			if (self->m_sizeClient.y != self->m_sizeClientResizedByKeyboard.y) {
				UISize size = self->m_sizeClient;
				self->m_sizeClientResizedByKeyboard = size;
				window->onResize(size.x, size.y);
			}
		}
	}
	
	if (adjustMode == UIKeyboardAdjustMode::Pan || scroll != nil) {
		NSDictionary* info = [aNotification userInfo];
		CGSize kbSize = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;
		CGRect rcScreen = [[UIScreen mainScreen] bounds];
		CGRect rcTextLocal = view.bounds;
		CGRect rcTextScreen = [view convertRect:rcTextLocal toView:self.view];
		CGFloat yText = rcTextScreen.origin.y + rcTextScreen.size.height + rcScreen.size.height / 100;
		CGFloat yBottomLimit = rcScreen.size.height - kbSize.height;

		if (scroll != nil) {
			CGRect rcScrollLocal = scroll.bounds;
			if (g_keyboardCurrentScrollView == scroll) {
				rcScrollLocal.size = g_keyboardCurrentScrollViewOriginalFrame.size;
			}
			CGRect rcScrollScreen = [scroll convertRect:rcScrollLocal toView:nil];
			CGFloat yScroll = rcScrollScreen.origin.y + rcScrollScreen.size.height;
			if (yScroll > rcScreen.size.height - kbSize.height) {
				CGRect rc = scroll.frame;
				if (g_keyboardCurrentScrollView == scroll) {
					rc = g_keyboardCurrentScrollViewOriginalFrame;
				}
				g_keyboardCurrentScrollViewOriginalFrame = rc;
				rc.size.height -= yScroll - (rcScreen.size.height - kbSize.height);
				scroll.frame = rc;
				g_keyboardCurrentScrollView = scroll;
			}
		}
		
		if (yText > yBottomLimit) {
			CGFloat offset = yBottomLimit - yText;
			if (scroll != nil) {
				if (!([scroll isKindOfClass:[UITextView class]])) {
					CGPoint pos = scroll.contentOffset;
					pos.y -= offset;
					scroll.contentOffset = pos;
				}
				self.view.transform = CGAffineTransformIdentity;
			} else {
				CGAffineTransform transform = CGAffineTransformTranslate(CGAffineTransformIdentity, 0, offset);
				[UIView animateWithDuration:0.3 animations:^(void) {
					self.view.transform = transform;
				}];
			}
		} else {
			self.view.transform = CGAffineTransformIdentity;
		}
	} else {
		self.view.transform = CGAffineTransformIdentity;
	}
	
}

-(void)keyboardWillHide {
	if (!(Math::isAlmostZero(self.view.transform.ty))) {
		[UIView animateWithDuration:0.3 animations:^(void) {
			self.view.transform = CGAffineTransformIdentity;
		}];
	}
	[self restoreKeyboardScrollView];
	
	Ref<iOS_WindowInstance> window = self->m_window;
	if (window.isNotNull()) {
		if (self->m_sizeClient.y != self->m_sizeClientResizedByKeyboard.y) {
			UISize size = self->m_sizeClient;
			self->m_sizeClientResizedByKeyboard = size;
			window->onResize(size.x, size.y);
		}
	}
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(keyboardWillShow:)
												 name:UIKeyboardWillShowNotification
											   object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(keyboardWillHide)
												 name:UIKeyboardWillHideNotification
											   object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
	
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:UIKeyboardWillShowNotification
												  object:nil];
	
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:UIKeyboardWillHideNotification
												  object:nil];
}

- (void)viewSafeAreaInsetsDidChange
{
	[super viewSafeAreaInsetsDidChange];
	UIResource::updateDefaultScreenSize();
}

@end

#endif
