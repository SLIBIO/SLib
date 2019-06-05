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
			class WindowInstanceImpl;
		}
	}
}

@interface SLIBWindowRootViewController : UIViewController
{
	@public slib::WeakRef<slib::priv::window::WindowInstanceImpl> m_window;
	
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

			class WindowInstanceImpl : public WindowInstance
			{
			public:
				UIView* m_window;
				AtomicRef<ViewInstance> m_viewContent;
				
			public:
				WindowInstanceImpl()
				{
				}
				
				~WindowInstanceImpl()
				{
					release();
				}
				
			public:
				static Ref<WindowInstanceImpl> create(UIView* window)
				{
					if (window != nil) {
						Ref<WindowInstanceImpl> ret = new WindowInstanceImpl();
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
						if (!flagMainWindow) {
							UIScreen* screen = UIPlatform::getScreenHandle(_screen.get());
							if (screen != nil) {
								window.screen = screen;
							}
							window.windowLevel = UIWindowLevelNormal + 1;
						}
						SLIBWindowRootViewController* controller = [[SLIBWindowRootViewController alloc] init];
						if (controller != nil) {
							controller->m_sizeClient = _rect.getSize();
							controller->m_sizeClientResizedByKeyboard = controller->m_sizeClient;
							SLIBViewHandle* view = [[SLIBViewHandle alloc] init];
							if (view != nil) {
								view.opaque = NO;
								controller.view = view;
								window.rootViewController = controller;
								Ref<WindowInstanceImpl> ret = create(window);
								if (ret.isNotNull()) {
									controller->m_window = ret;
									ret->setFocus();
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
				
				Ref<ViewInstance> getContentView()
				{
					return m_viewContent;
				}
				
				void close()
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
				
				sl_bool isClosed()
				{
					return m_window == nil;
				}
				
				Ref<WindowInstance> getParent()
				{
					return sl_null;
				}
				
				sl_bool setParent(const Ref<WindowInstance>& window)
				{
					return sl_false;
				}
				
				sl_bool setFocus()
				{
					UIView* view = m_window;
					if (view != nil) {
						if (!(UI::isUiThread())) {
							UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setFocus, this));
							return sl_true;
						}
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							[window makeKeyAndVisible];
						} else {
							[view becomeFirstResponder];
						}
						return sl_true;
					}
					return sl_false;
				}
				
				UIRect getFrame()
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
				
				sl_bool setFrame(const UIRect& frame)
				{
					UIView* window = m_window;
					if (window != nil) {
						if (!(UI::isUiThread())) {
							UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setFrame, this, frame));
							return sl_true;
						}
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
						return sl_true;
					}
					return sl_false;
				}
				
				UIRect getClientFrame()
				{
					return getFrame();
				}
				
				UISize getClientSize()
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
				
				sl_bool setClientSize(const UISize& size)
				{
					UIView* window = m_window;
					if (window != nil) {
						if (!(UI::isUiThread())) {
							UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setClientSize, this, size));
							return sl_true;
						}
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
				
				String getTitle()
				{
					return sl_null;
				}
				
				sl_bool setTitle(const String& title)
				{
					return sl_false;
				}
				
				Color getBackgroundColor()
				{
					UIView* window = m_window;
					if (window != nil) {
						UIColor* color = [window backgroundColor];
						if (color == nil) {
							return Color::zero();
						}
						return GraphicsPlatform::getColorFromUIColor(color);
					}
					return Color::Transparent;
				}
				
				sl_bool setBackgroundColor(const Color& _color)
				{
					UIView* window = m_window;
					if (window != nil) {
						if (!(UI::isUiThread())) {
							UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setBackgroundColor, this, _color));
							return sl_true;
						}
						UIColor* color;
						if (_color.isZero()) {
							color = nil;
						} else {
							color = GraphicsPlatform::getUIColorFromColor(_color);
						}
						[window setBackgroundColor:color];
						return sl_true;
					}
					return sl_false;
				}
				
				sl_bool isMinimized()
				{
					return sl_false;
				}
				
				sl_bool setMinimized(sl_bool flag)
				{
					return sl_false;
				}
				
				sl_bool isMaximized()
				{
					return sl_false;
				}
				
				sl_bool setMaximized(sl_bool flag)
				{
					return sl_false;
				}
				
				sl_bool isVisible()
				{
					UIView* window = m_window;
					if (window != nil) {
						BOOL flag = [window isHidden];
						if (flag) {
							return sl_false;
						} else {
							return sl_true;
						}
					} else {
						return sl_false;
					}
				}
				
				sl_bool setVisible(sl_bool flag)
				{
					UIView* window = m_window;
					if (window != nil) {
						if (!(UI::isUiThread())) {
							UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setVisible, this, flag));
							return sl_true;
						}
						[window setHidden:(flag ? NO : YES)];
						return sl_true;
					} else {
						return sl_false;
					}
				}
				
				sl_bool isAlwaysOnTop()
				{
					UIView* view = m_window;
					if (view != nil) {
						if ([view isKindOfClass:[UIWindow class]]) {
							UIWindow* window = (UIWindow*)view;
							if (window.windowLevel >= UIWindowLevelAlert) {
								return sl_true;
							} else {
								return sl_false;
							}
						}
					}
					return sl_false;
				}
				
				sl_bool setAlwaysOnTop(sl_bool flag)
				{
					UIView* view = m_window;
					if (view != nil) {
						if ([view isKindOfClass:[UIWindow class]]) {
							if (!(UI::isUiThread())) {
								UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setAlwaysOnTop, this, flag));
								return sl_true;
							}
							UIWindow* window = (UIWindow*)view;
							if (flag) {
								window.windowLevel = UIWindowLevelAlert + 1;
							} else {
								window.windowLevel = UIWindowLevelNormal + 1;
							}
							return sl_true;
						}
					}
					return sl_false;
				}
				
				sl_bool isCloseButtonEnabled()
				{
					return sl_false;
				}
				
				sl_bool setCloseButtonEnabled(sl_bool flag)
				{
					return sl_false;
				}
				
				sl_bool isMinimizeButtonEnabled()
				{
					return sl_false;
				}
				
				sl_bool setMinimizeButtonEnabled(sl_bool flag)
				{
					return sl_false;
				}
				
				sl_bool isMaximizeButtonEnabled()
				{
					return sl_false;
				}
				
				sl_bool setMaximizeButtonEnabled(sl_bool flag)
				{
					return sl_false;
				}
				
				sl_bool isResizable()
				{
					return sl_false;
				}
				
				sl_bool setResizable(sl_bool flag)
				{
					return sl_false;
				}
				
				sl_real getAlpha()
				{
					UIView* window = m_window;
					if (window != nil) {
						sl_real alpha = (sl_real)(window.alpha);
						return alpha;
					}
					return 1;
				}
				
				sl_bool setAlpha(sl_real _alpha)
				{
					UIView* window = m_window;
					if (window != nil) {
						if (!(UI::isUiThread())) {
							UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WindowInstanceImpl, setAlpha, this, _alpha));
							return sl_true;
						}
						sl_real alpha = _alpha;
						if (alpha < 0) {
							alpha = 0;
						}
						if (alpha > 1) {
							alpha = 1;
						}
						window.alpha = alpha;
						return sl_true;
					} else {
						return sl_false;
					}
				}
				
				sl_bool isTransparent()
				{
					return sl_false;
				}
				
				sl_bool setTransparent(sl_bool flag)
				{
					return sl_false;
				}
				
				UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen)
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
				
				UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow)
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
				
				UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen)
				{
					return convertCoordinateFromScreenToWindow(ptScreen);
				}
				
				UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient)
				{
					return convertCoordinateFromWindowToScreen(ptClient);
				}
				
				UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow)
				{
					return ptWindow;
				}
				
				UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient)
				{
					return ptClient;
				}
				
				UISize getWindowSizeFromClientSize(const UISize& sizeClient)
				{
					return sizeClient;
				}
				
				UISize getClientSizeFromWindowSize(const UISize& sizeWindow)
				{
					return sizeWindow;
				}
				
			};
			
			void resetOrientation()
			{
				g_screenOrientation = [[UIApplication sharedApplication] statusBarOrientation];
			}
			
			ScreenOrientation convertScreenOrientation(UIInterfaceOrientation orientation)
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
			
			UIInterfaceOrientation convertScreenOrientation(ScreenOrientation orientation)
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
		return WindowInstanceImpl::create(param);
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
		return convertScreenOrientation(g_screenOrientation);
	}
	
	void UI::attemptRotateScreenOrientation()
	{
		[UIViewController attemptRotationToDeviceOrientation];
		List<ScreenOrientation> orientations(UI::getAvailableScreenOrientations());
		if (orientations.isEmpty()) {
			return;
		}
		if (orientations.contains(convertScreenOrientation([[UIApplication sharedApplication] statusBarOrientation]))) {
			return;
		}
		ScreenOrientation orientation = orientations.getValueAt(0, ScreenOrientation::Portrait);
		@try {
			[[UIDevice currentDevice] setValue:[NSNumber numberWithInteger:convertScreenOrientation(orientation)] forKey:@"orientation"];
		} @catch (NSError* error) {
			NSLog(@"[Error] %@", error.localizedDescription);
		}
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
	
}

using namespace slib::priv::window;

@implementation SLIBWindowRootViewController

- (id)init
{
	self = [super init];
	if (self != nil) {
		self->m_sizeClient = slib::UISize::zero();
		self->m_sizeClientResizedByKeyboard = slib::UISize::zero();
	}
	return self;
}

- (BOOL)prefersStatusBarHidden
{
	if (g_flagSetStatusBarStyle) {
		if (g_currentStatusBarStyle == slib::StatusBarStyle::Hidden) {
			return YES;
		}
		return NO;
	}
	return [super prefersStatusBarHidden];
}

- (UIStatusBarStyle)preferredStatusBarStyle
{
	if (g_flagSetStatusBarStyle) {
		if (g_currentStatusBarStyle == slib::StatusBarStyle::Light) {
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
	slib::Ref<slib::WindowInstanceImpl> window = m_window;
	if (window.isNotNull()) {
		UIView* handle = window->m_window;
		if (handle != nil) {
			CGFloat f = slib::UIPlatform::getGlobalScaleFactor();
			CGSize _size = handle.frame.size;
			slib::UISize size;
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
		resetOrientation();
		UIWindow* handle = self.view.window;
		if (handle != nil) {
			CGRect r = handle.frame;
			r.size = size;
			handle.frame = r;
		}
		CGFloat f = slib::UIPlatform::getGlobalScaleFactor();
		slib::UISize sizeContent;
		sizeContent.x = (sl_ui_pos)(size.width * f);
		sizeContent.y = (sl_ui_pos)(size.height * f);
		self->m_sizeClient = sizeContent;
		self->m_sizeClientResizedByKeyboard = sizeContent;
		slib::Ref<slib::WindowInstanceImpl> window = self->m_window;
		if (window.isNotNull()) {
			window->onResize(sizeContent.x, sizeContent.y);
		}
	});
}

-(UIView*)findFirstResponderText
{
	UIView* view = slib::UIPlatform::findFirstResponder(self.view);
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
	
	slib::UIKeyboardAdjustMode adjustMode = slib::UI::getKeyboardAdjustMode();
	slib::Ref<slib::WindowInstanceImpl> window = self->m_window;
	if (window.isNotNull()) {
		if (adjustMode == slib::UIKeyboardAdjustMode::Resize) {
			NSDictionary* info = [aNotification userInfo];
			sl_ui_len heightKeyboard = (sl_ui_len)([[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size.height * slib::UIPlatform::getGlobalScaleFactor());
			slib::UISize size = self->m_sizeClient;
			size.y -= heightKeyboard;
			self->m_sizeClientResizedByKeyboard = size;
			window->onResize(size.x, size.y);
		} else {
			if (self->m_sizeClient.y != self->m_sizeClientResizedByKeyboard.y) {
				slib::UISize size = self->m_sizeClient;
				self->m_sizeClientResizedByKeyboard = size;
				window->onResize(size.x, size.y);
			}
		}
	}
	
	if (adjustMode == slib::UIKeyboardAdjustMode::Pan || scroll != nil) {
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
	if (!(slib::Math::isAlmostZero(self.view.transform.ty))) {
		[UIView animateWithDuration:0.3 animations:^(void) {
			self.view.transform = CGAffineTransformIdentity;
		}];
	}
	[self restoreKeyboardScrollView];
	
	slib::Ref<slib::WindowInstanceImpl> window = self->m_window;
	if (window.isNotNull()) {
		if (self->m_sizeClient.y != self->m_sizeClientResizedByKeyboard.y) {
			slib::UISize size = self->m_sizeClient;
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
	slib::UIResource::updateDefaultScreenSize();
}

@end


namespace slib
{
	Ref<WindowInstance> UIPlatform::createWindowInstance(UIView* window)
	{
		Ref<WindowInstance> ret = UIPlatform::_getWindowInstance((__bridge void*)window);
		if (ret.isNotNull()) {
			return ret;
		}
		return WindowInstanceImpl::create(window);
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
		WindowInstanceImpl* window = (WindowInstanceImpl*)instance;
		if (window) {
			return window->m_window;
		} else {
			return nil;
		}
	}
	
}

#endif
