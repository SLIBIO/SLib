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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/window.h"

#include "slib/ui/screen.h"
#include "slib/ui/core.h"
#include "slib/ui/platform.h"

#include "view_macos.h"

namespace slib
{
	namespace priv
	{
		namespace window
		{
			class macOS_WindowInstance;
		}
	}
}

@interface SLIBWindowHandle : NSWindow<NSWindowDelegate> {
	
	@public slib::WeakRef<slib::priv::window::macOS_WindowInstance> m_window;
	
	@public sl_bool m_flagStateResizingWidth;
	
	@private NSView* m_handleLastHitMouse;
	
}
@end

namespace slib
{
	
	namespace priv
	{
		namespace window
		{
			
			class macOS_WindowInstance : public WindowInstance
			{
			public:
				NSWindow* m_window;
				__weak NSWindow* m_parent;
				
				sl_ui_len m_heightScreen;
				
				AtomicRef<ViewInstance> m_viewContent;
				
				sl_bool m_flagClosing;
				sl_bool m_flagSheet;
				sl_bool m_flagDoModal;
				
			public:
				macOS_WindowInstance()
				{
					m_flagClosing = sl_false;
					m_flagSheet = sl_false;
					m_flagDoModal = sl_false;
				}
				
				~macOS_WindowInstance()
				{
					release();
				}
				
			public:
				static Ref<macOS_WindowInstance> create(NSWindow* window)
				{
					if (window != nil) {
						sl_ui_pos heightScreen;
						NSScreen* screen = [window screen];
						if (screen != nil) {
							NSRect rc = [screen frame];
							heightScreen = (sl_ui_pos)(rc.size.height);
							if (heightScreen < 0) {
								heightScreen = 0;
							}
						} else {
							heightScreen = 0;
						}
						Ref<macOS_WindowInstance> ret = new macOS_WindowInstance();
						if (ret.isNotNull()) {
							ret->m_window = window;
							ret->m_heightScreen = heightScreen;
							NSView* view = [window contentView];
							if (view != nil) {
								Ref<ViewInstance> content = UIPlatform::createViewInstance(view);
								if (content.isNotNull()) {
									content->setWindowContent(sl_true);
									ret->m_viewContent = content;
									if ([view isKindOfClass:[SLIBViewHandle class]]) {
										((SLIBViewHandle*)view)->m_viewInstance = Ref<macOS_ViewInstance>::from(content);
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
					
					NSWindow* parent = nil;
					if (param.parent.isNotNull()) {
						macOS_WindowInstance* w = static_cast<macOS_WindowInstance*>(param.parent.get());
						parent = w->m_window;
					}
					
					sl_bool flagSheet = param.flagSheet && parent != nil;
					
					NSScreen* screen = nil;
					int styleMask = 0;
					NSRect rect;
					
					if (flagSheet) {
						
						rect.origin.x = 0;
						rect.origin.y = 0;
						rect.size.width = param.size.x;
						rect.size.height = param.size.y;
						
						styleMask = NSTitledWindowMask;
						
					} else {
						
						if (param.flagBorderless || param.flagFullScreen) {
							styleMask = NSBorderlessWindowMask;
						} else {
							if (param.flagShowTitleBar){
								styleMask = NSTitledWindowMask | NSClosableWindowMask;
							} else {
								styleMask = NSClosableWindowMask | NSMiniaturizableWindowMask;
							}
						}
						
						screen = UIPlatform::getScreenHandle(param.screen.get());
						
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
						
						_getNSRect(rect, param.calculateRegion(screenFrame), screenFrame.getHeight());
						
						rect = [NSWindow contentRectForFrameRect:rect styleMask:styleMask];
					}
					
					SLIBWindowHandle* window = [[SLIBWindowHandle alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:YES screen:screen];
					
					if (window != nil) {
						
						[window setDelegate: window];
						window.animationBehavior = NSWindowAnimationBehaviorDocumentWindow;

						window->m_flagStateResizingWidth = sl_false;
						[window setReleasedWhenClosed:NO];
						[window setContentView:[[SLIBViewHandle alloc] init]];
						
						Ref<macOS_WindowInstance> ret = create(window);
						
						if (ret.isNotNull()) {
							
							ret->m_parent = parent;
							
							window->m_window = ret;
							
							{
								NSString* title = Apple::getNSStringFromString(param.title);
								[window setTitle: title];
							}
							
							if (flagSheet) {
								ret->m_flagSheet = sl_true;
								WeakRef<macOS_WindowInstance> retWeak = ret;
								[parent beginSheet:window completionHandler:^(NSModalResponse returnCode) {
									Ref<macOS_WindowInstance> w = retWeak;
									if (w.isNotNull()) {
										w->m_flagSheet = sl_false;
									}
								}];
							}
							return ret;
						}
					}
					return sl_null;
				}
				
				void release()
				{
					close();
				}
				
				void close() override
				{
					m_viewContent.setNull();
					NSWindow* window = m_window;
					if (window != nil) {
						UIPlatform::removeWindowInstance(window);
						if (m_flagSheet) {
							m_flagSheet = sl_false;
							NSWindow* parent = m_parent;
							if (parent != nil) {
								[parent endSheet: window];
								return;
							}
						}
						if (m_flagDoModal) {
							m_flagDoModal = sl_false;
							[NSApp stopModal];
						}
						if (!m_flagClosing) {
							[window close];
						}
						m_window = nil;
					}
				}
				
				sl_bool isClosed() override
				{
					return m_window == nil;
				}
				
				void setParent(const Ref<WindowInstance>& windowInst) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						if (windowInst.isNotNull()) {
							macOS_WindowInstance* w = static_cast<macOS_WindowInstance*>(windowInst.get());
							NSWindow* p = w->m_window;
							m_parent = p;
							if (p != nil && window.parentWindow == nil) {
								[p addChildWindow:window ordered:NSWindowAbove];
							}
						} else {
							NSWindow* p = window.parentWindow;
							if (p != nil) {
								[p removeChildWindow:window];
							}
							m_parent = nil;
						}
					}
				}
				
				Ref<ViewInstance> getContentView() override
				{
					return m_viewContent;
				}
				
				sl_bool isActive() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						return [window isKeyWindow];
						return sl_true;
					}
					return sl_false;
				}
				
				void activate() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						[window makeKeyAndOrderFront:NSApp];
					}
				}
				
				UIRect getFrame() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect rect = [window frame];
						UIRect frame;
						_getFrame(frame, rect, m_heightScreen);
						return frame;
					} else {
						return UIRect::zero();
					}
				}
				
				void setFrame(const UIRect& frame) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect rect;
						_getNSRect(rect, frame, m_heightScreen);
						[window setFrame:rect display:TRUE];
					}
				}
				
				UIRect getClientFrame() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect fr = [window frame];
						NSRect cr = [window contentRectForFrameRect:fr];
						UIRect frame;
						_getFrame(frame, cr, m_heightScreen);
						return frame;
					} else {
						return UIRect::zero();
					}
				}
				
				UISize getClientSize() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect rect = [window contentRectForFrameRect:[window frame]];
						return UISize((sl_ui_pos)(rect.size.width), (sl_ui_pos)(rect.size.height));
					} else {
						return UISize::zero();
					}
				}
				
				sl_bool setClientSize(const UISize& size) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSSize s;
						s.width = (CGFloat)(size.x);
						s.height = (CGFloat)(size.y);
						[window setContentSize:s];
						return sl_true;
					}
					return sl_false;
				}
				
				void setTitle(const String& title) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						[window setTitle: Apple::getNSStringFromString(title)];
					}
				}
				
				void setBackgroundColor(const Color& _color) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSColor* color = GraphicsPlatform::getNSColorFromColor(_color);
						[window setBackgroundColor:color];
					}
				}
				
				sl_bool isMinimized() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						BOOL flag = [window isMiniaturized];
						if (flag) {
							return sl_true;
						} else {
							return sl_false;
						}
					} else {
						return sl_false;
					}
				}
				
				void setMinimized(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						sl_bool f1 = [window isMiniaturized] ? sl_true : sl_false;
						sl_bool f2 = flag ? sl_true : sl_false;
						if (f1 != f2) {
							if (f2) {
								[window miniaturize:nil];
							} else {
								[window deminiaturize:nil];
							}
						}
					}
				}
				
				sl_bool isMaximized() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						BOOL flag = [window isZoomed];
						if (flag) {
							return sl_true;
						} else {
							return sl_false;
						}
					} else {
						return sl_false;
					}
				}
				
				void setMaximized(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						sl_bool f1 = [window isZoomed] ? sl_true : sl_false;
						sl_bool f2 = flag ? sl_true : sl_false;
						if (f1 != f2) {
							[window zoom:nil];
							UI::dispatchToUiThread([window]() {
								[window invalidateShadow];
							}, 100);
						}
					}
				}
				
				void setVisible(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						if (flag) {
							NSWindow* parent = m_parent;
							if (parent != nil) {
								if (window.parentWindow == nil) {
									[parent addChildWindow:window ordered:NSWindowAbove];
								}
							} else {
								[window orderFrontRegardless];
							}
						} else {
							[window orderOut:nil];
						}
					}
				}
				
				void setAlwaysOnTop(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						if (flag) {
							[window setLevel:NSFloatingWindowLevel];
						} else {
							[window setLevel:NSNormalWindowLevel];
						}
					}
				}
				
				void setCloseButtonEnabled(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSUInteger style = [window styleMask];
						if (style & NSBorderlessWindowMask) {
							return;
						}
						if (!(style & NSTitledWindowMask)) {
							return;
						}
						sl_bool f1 = (style & NSClosableWindowMask) ? sl_true : sl_false;
						sl_bool f2 = flag ? sl_true : sl_false;
						if (f1 != f2) {
							if (flag) {
								style = style | NSClosableWindowMask;
							} else {
								style = style & (~NSClosableWindowMask);
							}
							[window setStyleMask:style];
						}
					}
				}
				
				void setMinimizeButtonEnabled(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSUInteger style = [window styleMask];
						if (style & NSBorderlessWindowMask) {
							return;
						}
						if (!(style & NSTitledWindowMask)) {
							return;
						}
						sl_bool f1 = (style & NSMiniaturizableWindowMask) ? sl_true : sl_false;
						sl_bool f2 = flag ? sl_true : sl_false;
						if (f1 != f2) {
							if (flag) {
								style = style | NSMiniaturizableWindowMask;
							} else {
								style = style & (~NSMiniaturizableWindowMask);
							}
							[window setStyleMask:style];
						}
					}
				}
				
				void setMaximizeButtonEnabled(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSUInteger style = [window styleMask];
						if (style & NSBorderlessWindowMask) {
							return;
						}
						NSButton* buttonZoom = [window standardWindowButton:NSWindowZoomButton];
						sl_bool f1 = [buttonZoom isEnabled] ? sl_true : sl_false;
						sl_bool f2 = flag ? sl_true : sl_false;
						if (f1 != f2) {
							[buttonZoom setEnabled:flag];
						}
					}
				}
				
				void setResizable(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSUInteger style = [window styleMask];
						if (style & NSBorderlessWindowMask) {
							return;
						}
						sl_bool f1 = (style & NSResizableWindowMask) ? sl_true : sl_false;
						sl_bool f2 = flag ? sl_true : sl_false;
						if (f1 != f2) {
							if (flag) {
								style = style | NSResizableWindowMask;
							} else {
								style = style & (~NSResizableWindowMask);
								if (style == 0) {
									style = NSClosableWindowMask;
								}
							}
							[window setStyleMask:style];
						}
					}
				}
				
				void setAlpha(sl_real _alpha) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						sl_real alpha = _alpha;
						if (alpha < 0) {
							alpha = 0;
						}
						if (alpha > 1) {
							alpha = 1;
						}
						[window setAlphaValue:alpha];
					}
				}
				
				void setTransparent(sl_bool flag) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						[window setIgnoresMouseEvents:(flag?TRUE:FALSE)];
					}
				}
				
				
				UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect rect;
						rect.origin.x = (CGFloat)(ptScreen.x);
						rect.origin.y = (CGFloat)(m_heightScreen - ptScreen.y);
						rect.size.width = 0;
						rect.size.height = 0;
						rect = [window convertRectFromScreen:rect];
						NSRect frame = [window frame];
						UIPointf ret;
						ret.x = (sl_ui_pos)(rect.origin.x);
						ret.y = (sl_ui_pos)(frame.size.height - rect.origin.y);
						return ret;
					} else {
						return ptScreen;
					}
				}
				
				UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect frame = [window frame];
						NSRect rect;
						rect.origin.x = (CGFloat)(ptWindow.x);
						rect.origin.y = (CGFloat)(frame.size.height - ptWindow.y);
						rect.size.width = 0;
						rect.size.height = 0;
						rect = [window convertRectToScreen:rect];
						UIPointf ret;
						ret.x = (sl_ui_posf)(rect.origin.x);
						ret.y = (sl_ui_posf)(m_heightScreen - rect.origin.y);
						return ret;
					} else {
						return ptWindow;
					}
				}
				
				UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect rect;
						rect.origin.x = (CGFloat)(ptScreen.x);
						rect.origin.y = (CGFloat)(m_heightScreen - ptScreen.y);
						rect.size.width = 0;
						rect.size.height = 0;
						rect = [window convertRectFromScreen:rect];
						NSRect client = [window contentRectForFrameRect:[window frame]];
						UIPointf ret;
						ret.x = (sl_ui_posf)(rect.origin.x);
						ret.y = (sl_ui_posf)(client.size.height - rect.origin.y);
						return ret;
					} else {
						return ptScreen;
					}
				}
				
				UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect client = [window contentRectForFrameRect:[window frame]];
						NSRect rect;
						rect.origin.x = (CGFloat)(ptClient.x);
						rect.origin.y = (CGFloat)(client.size.height - ptClient.y);
						rect.size.width = 0;
						rect.size.height = 0;
						rect = [window convertRectToScreen:rect];
						UIPointf ret;
						ret.x = (sl_ui_posf)(rect.origin.x);
						ret.y = (sl_ui_posf)(m_heightScreen - rect.origin.y);
						return ret;
					} else {
						return ptClient;
					}
				}
				
				UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect frame = [window frame];
						NSRect client = [window contentRectForFrameRect:frame];
						UIPointf ret;
						ret.x = (sl_ui_posf)(ptWindow.x);
						ret.y = (sl_ui_posf)(client.size.height - (frame.size.height - ptWindow.y));
						return ret;
					} else {
						return ptWindow;
					}
				}
				
				UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect frame = [window frame];
						NSRect client = [window contentRectForFrameRect:frame];
						UIPointf ret;
						ret.x = (sl_ui_posf)(ptClient.x);
						ret.y = (sl_ui_posf)(frame.size.height - (client.size.height - ptClient.y));
						return ret;
					} else {
						return ptClient;
					}
				}
				
				UISize getWindowSizeFromClientSize(const UISize& sizeClient) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect frame = [window frame];
						NSRect client = [window contentRectForFrameRect:frame];
						UISize ret;
						ret.x = sizeClient.x + (sl_ui_pos)(frame.size.width - client.size.width);
						ret.y = sizeClient.y + (sl_ui_pos)(frame.size.height - client.size.height);
						if (ret.x < 0) {
							ret.x = 0;
						}
						if (ret.y < 0) {
							ret.y = 0;
						}
						return ret;
					} else {
						return sizeClient;
					}
				}
				
				UISize getClientSizeFromWindowSize(const UISize& sizeWindow) override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						NSRect frame = [window frame];
						NSRect client = [window contentRectForFrameRect:frame];
						UISize ret;
						ret.x = sizeWindow.x - (sl_ui_pos)(frame.size.width - client.size.width);
						ret.y = sizeWindow.y - (sl_ui_pos)(frame.size.height - client.size.height);
						if (ret.x < 0) {
							ret.x = 0;
						}
						if (ret.y < 0) {
							ret.y = 0;
						}
						return ret;
					} else {
						return sizeWindow;
					}
				}
				
				sl_bool doModal() override
				{
					NSWindow* window = m_window;
					if (window != nil) {
						if (m_flagSheet) {
							return sl_false;
						}
						m_flagDoModal = sl_true;
						[NSApp runModalForWindow:window];
						return sl_true;
					}
					return sl_false;
				}
				
			private:
				static void _applyRectLimit(NSRect& rect)
				{
					if (rect.origin.x < -16000.0f) {
						rect.origin.x = -16000.0f;
					}
					if (rect.origin.x > 16000.0f) {
						rect.origin.x = 16000.0f;
					}
					if (rect.origin.y < -16000.0f) {
						rect.origin.y = -16000.0f;
					}
					if (rect.origin.y > 16000.0f) {
						rect.origin.y = 16000.0f;
					}
					if (rect.size.width < 0.0f) {
						rect.size.width = 0.0f;
					}
					if (rect.size.width > 10000.0f) {
						rect.size.width = 10000.0f;
					}
					if (rect.size.height < 0.0f) {
						rect.size.height = 0.0f;
					}
					if (rect.size.height > 10000.0f) {
						rect.size.height = 10000.0f;
					}
				}
				
				static void _getNSRect(NSRect& rectOut, const UIRect& rectIn, sl_ui_len heightScreen)
				{
					rectOut.origin.x = (int)(rectIn.left);
					rectOut.origin.y = (int)(heightScreen - rectIn.bottom);
					rectOut.size.width = (int)(rectIn.getWidth());
					rectOut.size.height = (int)(rectIn.getHeight());
					_applyRectLimit(rectOut);
				}
				
				static void _getFrame(UIRect& rectOut, const NSRect& rectIn, sl_ui_len heightScreen)
				{
					rectOut.left = (sl_ui_pos)(rectIn.origin.x);
					rectOut.top = heightScreen - (sl_ui_pos)(rectIn.origin.y + rectIn.size.height);
					rectOut.setWidth((sl_ui_pos)(rectIn.size.width));
					rectOut.setHeight((sl_ui_pos)(rectIn.size.height));
					rectOut.fixSizeError();
				}
				
			};

		}
	}
	
	using namespace priv::window;
	
	Ref<WindowInstance> Window::createWindowInstance(const WindowInstanceParam& param)
	{
		return macOS_WindowInstance::create(param);
	}

	Ref<Window> Window::getActiveWindow()
	{
		NSWindow* handle = UIPlatform::getKeyWindow();
		if (handle != nil) {
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(handle);
			if (instance.isNotNull()) {
				return instance->getWindow();
			}
		}
		return sl_null;
	}
	
	
	Ref<WindowInstance> UIPlatform::createWindowInstance(NSWindow* window)
	{
		Ref<WindowInstance> ret = UIPlatform::_getWindowInstance((__bridge void*)window);
		if (ret.isNotNull()) {
			return ret;
		}
		return macOS_WindowInstance::create(window);
	}
	
	void UIPlatform::registerWindowInstance(NSWindow* window, WindowInstance* instance)
	{
		UIPlatform::_registerWindowInstance((__bridge void*)window, instance);
	}
	
	Ref<WindowInstance> UIPlatform::getWindowInstance(NSWindow* window)
	{
		return UIPlatform::_getWindowInstance((__bridge void*)window);
	}
	
	void UIPlatform::removeWindowInstance(NSWindow* window)
	{
		UIPlatform::_removeWindowInstance((__bridge void*)window);
	}
	
	NSWindow* UIPlatform::getWindowHandle(WindowInstance* instance)
	{
		macOS_WindowInstance* window = (macOS_WindowInstance*)instance;
		if (window) {
			return window->m_window;
		} else {
			return nil;
		}
	}
	
	NSWindow* UIPlatform::getWindowHandle(Window* window)
	{
		if (window) {
			Ref<WindowInstance> instance = window->getWindowInstance();
			if (instance.isNotNull()) {
				macOS_WindowInstance* _instance = (macOS_WindowInstance*)(instance.get());
				if (_instance) {
					return _instance->m_window;
				}
			}
		}
		return nil;
	}
	
	NSWindow* UIPlatform::getMainWindow()
	{
		return [NSApp mainWindow];
	}
	
	NSWindow* UIPlatform::getKeyWindow()
	{
		NSWindow* window = [NSApp keyWindow];
		if (window != nil) {
			return window;
		}
		return UIPlatform::getMainWindow();
	}
	
}

using namespace slib;
using namespace slib::priv::window;

@implementation SLIBWindowHandle

- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)windowShouldClose:(id)sender
{
	BOOL ret = YES;
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->m_flagClosing = sl_true;
		if (window->onClose()) {
			window->close();
			ret = YES;
		} else {
			ret = NO;
		}
		window->m_flagClosing = sl_false;
	}
	return ret;
}

- (void)windowWillClose:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		UI::dispatchToUiThread(SLIB_FUNCTION_REF(macOS_WindowInstance, release, window));
	}
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onActivate();
	}
}

- (void)windowDidResignKey:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onDeactivate();
	}
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		UISize size((sl_ui_pos)(frameSize.width), (sl_ui_pos)(frameSize.height));
		if (size.x < 0) {
			size.x = 0;
		}
		if (size.y < 0) {
			size.y = 0;
		}
		if (size.x != self.frame.size.width) {
			m_flagStateResizingWidth = sl_true;
		}
		window->onResizing(size, m_flagStateResizingWidth);
		frameSize.width = (CGFloat)(size.x);
		frameSize.height = (CGFloat)(size.y);
	}
	return frameSize;
}

- (void)windowWillStartLiveResize:(NSNotification *)notification
{
	m_flagStateResizingWidth = sl_false;
}

- (void)windowDidResize:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		NSRect fr = [self frame];
		NSRect cr = [self contentRectForFrameRect:fr];
		NSSize size = cr.size;
		window->onResize((sl_ui_len)(size.width), (sl_ui_len)(size.height));
	}
}

- (void)windowDidMove:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onMove();
	}
}

- (void)windowWillMiniaturize:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onMinimize();
	}
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onDeminimize();
	}
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onMaximize();
	}
}

- (void)windowDidExitFullScreen:(NSNotification *)notification
{
	Ref<macOS_WindowInstance> window = m_window;
	if (window.isNotNull()) {
		window->onDemaximize();
	}
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)acceptsMouseMovedEvents
{
	return YES;
}

- (void)sendEvent:(NSEvent *)event
{
	NSEventType type = event.type;
	if (type == NSEventTypeKeyDown) {
		id view = [self firstResponder];
		if ([view isKindOfClass:[NSTextView class]]) {
			// Find NSTextField
			NSView* t = view;
			t = t.superview;
			if (!([t isKindOfClass:[NSTextField class]])) {
				t = t.superview;
			}
			if ([t isKindOfClass:[NSTextField class]]) {
				int c = event.keyCode;
				// Tab, Return, Escape
				if (c == 0x30 || c == 0x24 || c == 0x35) {
					// NSTextField can't get keyDown event, so we manually invoke this event
					[t keyDown:event];
					return;
				}
			}
		}
	} else if (type == NSEventTypeMouseMoved) {
		NSPoint pt = [event locationInWindow];
		NSView* hit = nil;
		NSView* content = [self contentView];
		if (content != nil) {
			hit = [content hitTest:pt];
		}
		if (m_handleLastHitMouse != hit) {
			if (m_handleLastHitMouse != nil) {
				if ([m_handleLastHitMouse isKindOfClass:[SLIBViewHandle class]]) {
					[m_handleLastHitMouse mouseExited:event];
				}
			}
			if (hit != nil) {
				if ([hit isKindOfClass:[SLIBViewHandle class]]) {
					[hit mouseEntered:event];
				}
			}
			m_handleLastHitMouse = hit;
		}
		if (hit != nil) {
			[hit mouseMoved:event];
		}
		return;
	}
	if (type == NSEventTypeKeyDown || type == NSEventTypeKeyUp) {
		id responder = [self firstResponder];
		if (responder == self) {
			responder = self.contentView;
			if (responder != nil) {
				[self makeFirstResponder:responder];
			}
		}
	}
	[super sendEvent:event];
}

@end

#endif
