#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN
class _iOS_Window;
SLIB_UI_NAMESPACE_END

@interface _slib_iOS_Window : UIWindow {
@public slib::WeakRef<slib::_iOS_Window> m_window;
}
@end

@interface _slib_iOS_Window_RootViewController : UIViewController
{
}
@end

@implementation _slib_iOS_Window_RootViewController
- (BOOL)prefersStatusBarHidden
{
	return NO;
}
@end

SLIB_UI_NAMESPACE_BEGIN
class _iOS_Window : public WindowInstance
{
public:
	UIView* m_window;
	Ref<ViewInstance> m_viewContent;
	
	_iOS_Window()
	{
	}
	
	static Ref<_iOS_Window> create(UIView* view)
	{
		Ref<_iOS_Window> ret;
		if (view != nil) {
			ret = new _iOS_Window();
			if (ret.isNotNull()) {
				ret->m_window = view;
				if ([view isKindOfClass:[UIWindow class]]) {
					UIWindow* window = (UIWindow*)view;
					view = window.rootViewController.view;
					if (view == nil) {
						window.rootViewController = [[_slib_iOS_Window_RootViewController alloc] init];
						view = [[UIView alloc] init];
						window.rootViewController.view = view;
					}
				}
				ret->m_viewContent = UIPlatform::createViewInstance(view);
			}
		}
		return ret;
	}
	
	static Ref<WindowInstance> create(const WindowInstanceParam& param)
	{
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
		Rectangle _rect = param.calculateRegion(screenFrame);
		CGRect rect;
		rect.origin.x = _rect.left;
		rect.origin.y = _rect.top;
		rect.size.width = _rect.getWidth();
		rect.size.height = _rect.getHeight();
		Ref<_iOS_Window> ret;
		_slib_iOS_Window* window = [[_slib_iOS_Window alloc] initWithFrame:rect];
		if (window != nil) {
			UIScreen* screen = UIPlatform::getScreenHandle(_screen.get());
			if (screen != nil) {
				window.screen = screen;
			}
			window.windowLevel = UIWindowLevelNormal + 1;
			ret = Ref<_iOS_Window>::from(UIPlatform::createWindowInstance(window));
			if (ret.isNotNull()) {
				window->m_window = ret;
				ret->setFocus();
			}
		}
		return Ref<WindowInstance>::from(ret);
	}
	
	~_iOS_Window()
	{
		release();
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
	
	const Ref<ViewInstance>& getContentView()
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
		return Ref<WindowInstance>::null();
	}
	
	sl_bool setParent(const Ref<WindowInstance>& window)
	{
		return sl_false;
	}
	
	sl_bool setFocus()
	{
		UIView* view = m_window;
		if (view != nil) {
			if ([view isKindOfClass:[UIWindow class]]) {
				UIWindow* window = (UIWindow*)view;
				[window makeKeyAndVisible];
				UIPlatform::setKeyWindow(window);
			} else {
				[view becomeFirstResponder];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	String getTitle()
	{
		return String::null();
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
			return UIPlatform::getColorFromUIColor(color);
		}
		return Color::transparent();
	}
	
	sl_bool setBackgroundColor(const Color& _color)
	{
		UIView* window = m_window;
		if (window != nil) {
			UIColor* color;
			if (_color.isZero()) {
				color = nil;
			} else {
				color = UIPlatform::getUIColorFromColor(_color);
			}
			[window setBackgroundColor:color];
			return sl_true;
		}
		return sl_false;
	}
	
	Rectangle getFrame()
	{
		UIView* window = m_window;
		if (window != nil) {
			CGRect rect = [window frame];
			Rectangle frame;
			frame.left = (sl_real)(rect.origin.x);
			frame.top = (sl_real)(rect.origin.y);
			frame.right = frame.left + (sl_real)(rect.size.width);
			frame.bottom = frame.top + (sl_real)(rect.size.height);
			return frame;
		} else {
			return Rectangle::zero();
		}
	}
	
	sl_bool setFrame(const Rectangle& _frame)
	{
		Rectangle frame = _frame;
		UIView* window = m_window;
		if (window != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				CGRect rect;
				rect.origin.x = frame.left;
				rect.origin.y = frame.top;
				rect.size.width = frame.getWidth();
				rect.size.height = frame.getHeight();
				[window setFrame:rect];
			});
			return sl_true;
		}
		return sl_false;
	}
	
	Rectangle getClientFrame()
	{
		return getFrame();
	}
	
	Size getClientSize()
	{
		UIView* window = m_window;
		if (window != nil) {
			CGRect rect = [window frame];
			Size ret;
			ret.x = (sl_real)(rect.size.width);
			ret.y = (sl_real)(rect.size.height);
			return ret;
		} else {
			return Size::zero();
		}
	}
	
	sl_bool setClientSize(const Size& _size)
	{
		Size size = _size;
		UIView* window = m_window;
		if (window != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				CGRect frame = [window frame];
				frame.size.width = size.x;
				frame.size.height = size.y;
				[window setFrame:frame];
			});
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
			[window setHidden:(flag?NO:YES)];
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
	
	Point convertCoordinateFromScreenToWindow(const Point& ptScreen)
	{
		UIView* view = m_window;
		if (view != nil) {
			if ([view isKindOfClass:[UIWindow class]]) {
				UIWindow* window = (UIWindow*)view;
				CGPoint pt;
				pt.x = ptScreen.x;
				pt.y = ptScreen.y;
				pt = [window convertPoint:pt fromWindow:nil];
				Point ret;
				ret.x = (sl_real)(pt.x);
				ret.y = (sl_real)(pt.y);
				return ret;
			} else {
				UIWindow* window = [view window];
				if (window != nil) {
					CGPoint pt;
					pt.x = ptScreen.x;
					pt.y = ptScreen.y;
					pt = [window convertPoint:pt fromWindow:nil];
					pt = [window convertPoint:pt toView:view];
					Point ret;
					ret.x = (sl_real)(pt.x);
					ret.y = (sl_real)(pt.y);
					return ret;
				}
			}
		}
		return ptScreen;
	}
	
	Point convertCoordinateFromWindowToScreen(const Point& ptWindow)
	{
		UIView* view = m_window;
		if (view != nil) {
			if ([view isKindOfClass:[UIWindow class]]) {
				UIWindow* window = (UIWindow*)view;
				CGPoint pt;
				pt.x = ptWindow.x;
				pt.y = ptWindow.y;
				pt = [window convertPoint:pt toWindow:nil];
				Point ret;
				ret.x = (sl_real)(pt.x);
				ret.y = (sl_real)(pt.y);
				return ret;
			} else {
				UIWindow* window = [view window];
				if (window != nil) {
					CGPoint pt;
					pt.x = ptWindow.x;
					pt.y = ptWindow.y;
					pt = [window convertPoint:pt fromView:view];
					pt = [window convertPoint:pt toWindow:nil];
					Point ret;
					ret.x = (sl_real)(pt.x);
					ret.y = (sl_real)(pt.y);
					return ret;
				}
			}
		}
		return ptWindow;
	}
	
	Point convertCoordinateFromScreenToClient(const Point& ptScreen)
	{
		return convertCoordinateFromScreenToWindow(ptScreen);
	}
	
	Point convertCoordinateFromClientToScreen(const Point& ptClient)
	{
		return convertCoordinateFromWindowToScreen(ptClient);
	}
	
	Point convertCoordinateFromWindowToClient(const Point& ptWindow)
	{
		return ptWindow;
	}
	
	Point convertCoordinateFromClientToWindow(const Point& ptClient)
	{
		return ptClient;
	}
	
	Size getWindowSizeFromClientSize(const Size& sizeClient)
	{
		return sizeClient;
	}
	
	Size getClientSizeFromWindowSize(const Size& sizeWindow)
	{
		return sizeWindow;
	}
	
	void runModal()
	{
	}
};


Ref<WindowInstance> Window::createWindowInstance(const WindowInstanceParam& param)
{
	return _iOS_Window::create(param);
}

SLIB_UI_NAMESPACE_END

@implementation _slib_iOS_Window
@end

SLIB_UI_NAMESPACE_BEGIN
Ref<WindowInstance> UIPlatform::createWindowInstance(UIView* window)
{
	Ref<WindowInstance> ret = UIPlatform::_getWindowInstance((__bridge void*)window);
	if (ret.isNotNull()) {
		return ret;
	}
	ret = Ref<WindowInstance>::from(_iOS_Window::create(window));
	if (ret.isNotNull()) {
		UIPlatform::_registerWindowInstance((__bridge void*)window, ret.get());
	}
	return Ref<WindowInstance>::from(ret);
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
	_iOS_Window* window = (_iOS_Window*)instance;
	if (window) {
		return window->m_window;
	} else {
		return nil;
	}
}
SLIB_UI_NAMESPACE_END

#endif
