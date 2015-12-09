#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#include "view_osx.h"

SLIB_UI_NAMESPACE_BEGIN
class _OSX_Window;
SLIB_UI_NAMESPACE_END

@interface _slib_OSX_Window : NSWindow<NSWindowDelegate> {
@public slib::WeakRef<slib::_OSX_Window> m_window;
	
@public sl_bool m_flagClosing;
@public sl_bool m_flagModal;

}
@end


SLIB_UI_NAMESPACE_BEGIN
class _OSX_Window : public WindowInstance
{
public:
	NSWindow* m_window;
	sl_real m_heightScreen;
	Ref<ViewInstance> m_viewContent;
	
	_OSX_Window()
	{
	}
	
	static Ref<_OSX_Window> create(NSWindow* window)
	{
		Ref<_OSX_Window> ret;
		if (window != nil) {
			sl_real heightScreen;
			NSScreen* screen = [window screen];
			if (screen != nil) {
				NSRect rc = [screen frame];
				heightScreen = rc.size.height;
			} else {
				heightScreen = 0;
			}
			ret = new _OSX_Window();
			if (ret.isNotNull()) {
				ret->m_window = window;
				ret->m_heightScreen = heightScreen;
				NSView* view = [window contentView];
				ret->m_viewContent = UIPlatform::createViewInstance(view, sl_false);
				if ([view isKindOfClass:[Slib_OSX_ViewHandle class]]) {
					((Slib_OSX_ViewHandle*)view)->m_viewInstance = Ref<OSX_ViewInstance>::from(ret->m_viewContent);
				}
			}
		}
		return ret;
	}
	
	static Ref<WindowInstance> create(const WindowInstanceParam& param)
	{
		int styleMask = 0;
		if (param.flagBorderless || param.flagFullScreen) {
			styleMask = NSBorderlessWindowMask;
		} else {
			if (param.flagShowTitleBar){
				styleMask = NSTitledWindowMask | NSClosableWindowMask;
			} else {
				styleMask = NSClosableWindowMask;
			}
		}
		NSScreen* screen = UIPlatform::getScreenHandle(param.screen.get());
		Rectangle screenFrame;
		Ref<Screen> _screen = param.screen;
		if (_screen.isNotNull()) {
			screenFrame = _screen->getRegion();
		} else {
			_screen = UI::getPrimaryScreen();
			if (_screen.isNotNull()) {
				screenFrame = _screen->getRegion();
			} else {
				screenFrame = Rectangle::zero();
			}
		}
		NSRect rect;
		_getNSRect(rect, param.calculateRegion(screenFrame), screenFrame.getHeight());
		
		rect = [NSWindow contentRectForFrameRect:rect styleMask:styleMask];

		Ref<_OSX_Window> ret;
		_slib_OSX_Window* window = [[_slib_OSX_Window alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:YES screen:screen];
		if (window != nil) {
			window->m_flagClosing = sl_false;
			window->m_flagModal = sl_false;
			[window setReleasedWhenClosed:NO];
			[window setContentView:[[Slib_OSX_ViewHandle alloc] init]];
			ret = Ref<_OSX_Window>::from(UIPlatform::createWindowInstance(window));
			if (ret.isNotNull()) {
				window->m_window = ret;
				{
					NSString* title = Apple::getNSStringFromString(param.title);
					[window setTitle: title];
				}
				[window makeKeyAndOrderFront:NSApp];
				[window setDelegate: window];
				//[window setAcceptsMouseMovedEvents:TRUE];
			}
		}
		return Ref<WindowInstance>::from(ret);
	}
	
	~_OSX_Window()
	{
		release();
	}
	
	void release()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			UIPlatform::removeWindowInstance(window);
		}
		m_viewContent.setNull();
		m_window = nil;
	}
	
	const Ref<ViewInstance>& getContentView()
	{
		return m_viewContent;
	}
	
	void close()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			if ([window isKindOfClass:[_slib_OSX_Window class]]) {
				_slib_OSX_Window* w = (_slib_OSX_Window*)window;
				if (w->m_flagModal) {
					w->m_flagModal = sl_false;
					[NSApp stopModal];
				}
				if (!(w->m_flagClosing)) {
					[w close];
				}
			}
		}
	}
	
	sl_bool isClosed()
	{
		return m_window == nil;
	}
	
	Ref<WindowInstance> getParent()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSWindow* parent = [window parentWindow];
			if (parent != nil) {
				return UIPlatform::createWindowInstance(parent);
			}
		}
		return Ref<WindowInstance>::null();
	}
	
	sl_bool setParent(const Ref<WindowInstance>& _window)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			Ref<WindowInstance> windowInst = _window;
			if (windowInst.isNotNull()) {
				_OSX_Window* w = (_OSX_Window*)(windowInst.getObject());
				NSWindow* p = w->m_window;
				if (p != nil) {
					[p addChildWindow:window ordered:NSWindowAbove];
					return sl_true;
				}
			} else {
				NSWindow* p = [window parentWindow];
				if (p != nil) {
					[p removeChildWindow:window];
				}
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool setFocus()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			[window makeKeyAndOrderFront:NSApp];
			return sl_true;
		}
		return sl_false;
	}
	
	String getTitle()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSString* title = [window title];
			return Apple::getStringFromNSString(title);
		}
		return String::null();
	}
	
	sl_bool setTitle(const String& title)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSString* ntitle = Apple::getNSStringFromString(title);
			[window setTitle: ntitle];
			return sl_true;
		}
		return sl_false;
	}
	
	Color getBackgroundColor()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSColor* color = [window backgroundColor];
			if (color == [NSColor windowBackgroundColor]) {
				return Color::zero();
			}
			return UIPlatform::getColorFromNSColor(color);
		}
		return Color::transparent();
	}
	
	sl_bool setBackgroundColor(const Color& _color)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSColor* color;
			if (_color.isZero()) {
				color = [NSColor windowBackgroundColor];
			} else {
				color = UIPlatform::getNSColorFromColor(_color);
			}
			[window setBackgroundColor:color];
			return sl_true;
		}
		return sl_false;
	}
	
	Rectangle getFrame()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect rect = [window frame];
			Rectangle frame;
			_getFrame(frame, rect, m_heightScreen);
			return frame;
		} else {
			return Rectangle::zero();
		}
	}
	
	sl_bool setFrame(const Rectangle& _frame)
	{
		Rectangle frame = _frame;
		NSWindow* window = m_window;
		if (window != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				NSRect rect;
				_getNSRect(rect, frame, m_heightScreen);
				[window setFrame:rect display:TRUE];
			});
			return sl_true;
		}
		return sl_false;
	}
	
	Rectangle getClientFrame()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect fr = [window frame];
			NSRect cr = [window contentRectForFrameRect:fr];
			Rectangle ret;
			ret.left = (sl_real)(cr.origin.x - fr.origin.x);
			ret.top = (sl_real)((fr.origin.y + fr.size.height) - (cr.origin.y + cr.size.height));
			ret.setWidth((sl_real)(cr.size.width));
			ret.setHeight((sl_real)(cr.size.height));
			return ret;
		} else {
			return Rectangle::zero();
		}
	}
	
	Size getClientSize()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect rect = [window contentRectForFrameRect:[window frame]];
			return Size((sl_real)(rect.size.width), (sl_real)(rect.size.height));
		} else {
			return Size::zero();
		}
	}
	
	sl_bool setClientSize(const Size& _size)
	{
		Size size = _size;
		NSWindow* window = m_window;
		if (window != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				NSSize s;
				s.width = size.x;
				s.height = size.y;
				[window setContentSize:s];
			});
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isMinimized()
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
	
	sl_bool setMinimized(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				sl_bool f1 = [window isMiniaturized] ? sl_true : sl_false;
				sl_bool f2 = flag ? sl_true : sl_false;
				if (f1 != f2) {
					if (f2) {
						[window miniaturize:nil];
					} else {
						[window deminiaturize:nil];
					}
				}
			});
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isMaximized()
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
	
	sl_bool setMaximized(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				sl_bool f1 = [window isZoomed] ? sl_true : sl_false;
				sl_bool f2 = flag ? sl_true : sl_false;
				if (f1 != f2) {
					[window zoom:nil];
				}
			});
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isVisible()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			BOOL flag = [window isVisible];
			if (flag) {
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			return sl_false;
		}
	}
	
	sl_bool setVisible(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			if (flag) {
				[window orderFrontRegardless];
			} else {
				[window orderOut:nil];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isAlwaysOnTop()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSInteger level = [window level];
			if (level == NSNormalWindowLevel) {
				return sl_false;
			} else {
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool setAlwaysOnTop(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			if (flag) {
				[window setLevel:NSFloatingWindowLevel];
			} else {
				[window setLevel:NSNormalWindowLevel];
			}
		}
		return sl_false;
	}
	
	sl_bool isCloseButtonEnabled()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			if (!(style & NSTitledWindowMask)) {
				return sl_false;
			}
			sl_bool f = (style & NSClosableWindowMask) ? sl_true : sl_false;
			return f;
		}
		return sl_false;
	}
	
	sl_bool setCloseButtonEnabled(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			if (!(style & NSTitledWindowMask)) {
				return sl_false;
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
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isMinimizeButtonEnabled()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			if (!(style & NSTitledWindowMask)) {
				return sl_false;
			}
			sl_bool f = (style & NSMiniaturizableWindowMask) ? sl_true : sl_false;
			return f;
		}
		return sl_false;
	}
	
	sl_bool setMinimizeButtonEnabled(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			if (!(style & NSTitledWindowMask)) {
				return sl_false;
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
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isMaximizeButtonEnabled()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			NSButton* buttonZoom = [window standardWindowButton:NSWindowZoomButton];
			sl_bool f = [buttonZoom isEnabled] ? sl_true : sl_false;
			return f;
		}
		return sl_false;
	}
	
	sl_bool setMaximizeButtonEnabled(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			NSButton* buttonZoom = [window standardWindowButton:NSWindowZoomButton];
			sl_bool f1 = [buttonZoom isEnabled] ? sl_true : sl_false;
			sl_bool f2 = flag ? sl_true : sl_false;
			if (f1 != f2) {
				if (flag) {
					[buttonZoom setEnabled:TRUE];
				} else {
					[buttonZoom setEnabled:FALSE];
				}
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isResizable()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
			}
			sl_bool f = (style & NSResizableWindowMask) ? sl_true : sl_false;
			return f;
		}
		return sl_false;
	}

	sl_bool setResizable(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSUInteger style = [window styleMask];
			if (style & NSBorderlessWindowMask) {
				return sl_false;
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
			return sl_true;
		}
		return sl_false;
	}
	
	sl_real getAlpha()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			sl_real alpha = (sl_real)([window alphaValue]);
			return alpha;
		}
		return 1;
	}
	
	sl_bool setAlpha(sl_real _alpha)
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
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool isTransparent()
	{
		NSWindow* window = m_window;
		if (window != nil) {
			BOOL f = [window ignoresMouseEvents];
			return f ? sl_true : sl_false;
		}
		return sl_false;
	}
	
	sl_bool setTransparent(sl_bool flag)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			[window setIgnoresMouseEvents:(flag?TRUE:FALSE)];
			return sl_true;
		}
		return sl_false;
	}
	
	
	Point convertCoordinateFromScreenToWindow(const Point& ptScreen)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect rect;
			rect.origin.x = ptScreen.x;
			rect.origin.y = m_heightScreen - ptScreen.y;
			rect.size.width = 0;
			rect.size.height = 0;
			rect = [window convertRectFromScreen:rect];
			NSRect frame = [window frame];
			Point ret;
			ret.x = (sl_real)(rect.origin.x);
			ret.y = (sl_real)(frame.size.height - rect.origin.y);
			return ret;
		} else {
			return ptScreen;
		}
	}
	
	Point convertCoordinateFromWindowToScreen(const Point& ptWindow)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect frame = [window frame];
			NSRect rect;
			rect.origin.x = ptWindow.x;
			rect.origin.y = frame.size.height - ptWindow.y;
			rect.size.width = 0;
			rect.size.height = 0;
			rect = [window convertRectToScreen:rect];
			Point ret;
			ret.x = (sl_real)(rect.origin.x);
			ret.y = m_heightScreen - (sl_real)(rect.origin.y);
			return ret;
		} else {
			return ptWindow;
		}
	}
	
	Point convertCoordinateFromScreenToClient(const Point& ptScreen)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect rect;
			rect.origin.x = ptScreen.x;
			rect.origin.y = m_heightScreen - ptScreen.y;
			rect.size.width = 0;
			rect.size.height = 0;
			rect = [window convertRectFromScreen:rect];
			NSRect client = [window contentRectForFrameRect:[window frame]];
			Point ret;
			ret.x = (sl_real)(rect.origin.x);
			ret.y = (sl_real)(client.size.height - rect.origin.y);
			return ret;
		} else {
			return ptScreen;
		}
	}
	
	Point convertCoordinateFromClientToScreen(const Point& ptClient)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect client = [window contentRectForFrameRect:[window frame]];
			NSRect rect;
			rect.origin.x = ptClient.x;
			rect.origin.y = client.size.height - ptClient.y;
			rect.size.width = 0;
			rect.size.height = 0;
			rect = [window convertRectToScreen:rect];
			Point ret;
			ret.x = (sl_real)(rect.origin.x);
			ret.y = m_heightScreen - (sl_real)(rect.origin.y);
			return ret;
		} else {
			return ptClient;
		}
	}
	
	Point convertCoordinateFromWindowToClient(const Point& ptWindow)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect frame = [window frame];
			NSRect client = [window contentRectForFrameRect:frame];
			Point ret;
			ret.x = (sl_real)(ptWindow.x);
			ret.y = (sl_real)(client.size.height - (frame.size.height - ptWindow.y));
			return ret;
		} else {
			return ptWindow;
		}
	}
	
	Point convertCoordinateFromClientToWindow(const Point& ptClient)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect frame = [window frame];
			NSRect client = [window contentRectForFrameRect:frame];
			Point ret;
			ret.x = (sl_real)(ptClient.x);
			ret.y = (sl_real)(frame.size.height - (client.size.height - ptClient.y));
			return ret;
		} else {
			return ptClient;
		}
	}
	
	Size getWindowSizeFromClientSize(const Size& sizeClient)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect frame = [window frame];
			NSRect client = [window contentRectForFrameRect:frame];
			Size ret;
			ret.x = sizeClient.x + (sl_real)(frame.size.width - client.size.width);
			ret.y = sizeClient.y + (sl_real)(frame.size.height - client.size.height);
			return ret;
		} else {
			return sizeClient;
		}
	}
	
	Size getClientSizeFromWindowSize(const Size& sizeWindow)
	{
		NSWindow* window = m_window;
		if (window != nil) {
			NSRect frame = [window frame];
			NSRect client = [window contentRectForFrameRect:frame];
			Size ret;
			ret.x = sizeWindow.x - (sl_real)(frame.size.width - client.size.width);
			ret.y = sizeWindow.y - (sl_real)(frame.size.height - client.size.height);
			return ret;
		} else {
			return sizeWindow;
		}
	}
	
	void runModal()
	{
		NSWindow* _handle = m_window;
		if (_handle != nil && [_handle isKindOfClass:[_slib_OSX_Window class]]) {
			_slib_OSX_Window* handle = (_slib_OSX_Window*)_handle;
			if (handle->m_flagModal) {
				return;
			}
			handle->m_flagModal = sl_true;
			[NSApp runModalForWindow:handle];
		}
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
	
	static void _getNSRect(NSRect& rectOut, const Rectangle& rectIn, sl_real heightScreen)
	{
		rectOut.origin.x = rectIn.left;
		rectOut.origin.y = heightScreen - rectIn.bottom;
		rectOut.size.width = rectIn.getWidth();
		rectOut.size.height = rectIn.getHeight();
		_applyRectLimit(rectOut);
	}
	
	static void _getFrame(Rectangle& rectOut, const NSRect& rectIn, sl_real heightScreen)
	{
		rectOut.left = (sl_real)(rectIn.origin.x);
		rectOut.top = heightScreen - (sl_real)(rectIn.origin.y + rectIn.size.height);
		rectOut.setWidth((sl_real)(rectIn.size.width));
		rectOut.setHeight((sl_real)(rectIn.size.height));
	}
	
};

Ref<WindowInstance> Window::createWindowInstance(const WindowInstanceParam& param)
{
	return _OSX_Window::create(param);
}

SLIB_UI_NAMESPACE_END

@implementation _slib_OSX_Window

- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)windowShouldClose:(id)sender
{
	BOOL ret = YES;
	m_flagClosing = sl_true;
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		if (window->onClose()) {
			window->close();
			ret = YES;
		} else {
			ret = NO;
		}
	}
	m_flagClosing = sl_false;
	return ret;
}

- (void)windowWillClose:(NSNotification *)notification
{
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		slib::UI::runOnUIThread(SLIB_CALLBACK_REF(slib::_OSX_Window, release, window));
	}
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		slib::Size size(frameSize.width, frameSize.height);
		window->onResize(size);
		frameSize.width = size.x;
		frameSize.height = size.y;
	}
	return frameSize;
}
- (void)windowDidMove:(NSNotification *)notification
{
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		window->onMove();
	}
}

- (void)windowWillMiniaturize:(NSNotification *)notification
{
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		window->onMinimize();
	}
}
- (void)windowDidDeminiaturize:(NSNotification *)notification
{
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		window->onDeminimize();
	}
}
- (BOOL)windowShouldZoom:(NSWindow *)w toFrame:(NSRect)newFrame
{
	slib::Ref<slib::_OSX_Window> window = m_window.lock();
	if (window.isNotNull()) {
		if ([self isZoomed]) {
			window->onDemaximize();
		} else {
			window->onMaximize();
		}
	}
	return TRUE;
}

- (BOOL)acceptsFirstResponder
{
	return TRUE;
}
@end

SLIB_UI_NAMESPACE_BEGIN
Ref<WindowInstance> UIPlatform::createWindowInstance(NSWindow* window)
{
	Ref<WindowInstance> ret = UIPlatform::_getWindowInstance((__bridge void*)window);
	if (ret.isNotNull()) {
		return ret;
	}
	ret = Ref<WindowInstance>::from(_OSX_Window::create(window));
	if (ret.isNotNull()) {
		UIPlatform::_registerWindowInstance((__bridge void*)window, ret.get());
	}
	return Ref<WindowInstance>::from(ret);
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
	_OSX_Window* window = (_OSX_Window*)instance;
	if (window) {
		return window->m_window;
	} else {
		return nil;
	}
}
SLIB_UI_NAMESPACE_END

#endif
