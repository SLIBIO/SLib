#ifndef CHECKHEADER_SLIB_UI_WINDOW
#define CHECKHEADER_SLIB_UI_WINDOW

#include "definition.h"

#include "event.h"

#include "../core/string.h"
#include "../core/object.h"
#include "../core/callback.h"
#include "../graphics/color.h"

SLIB_UI_NAMESPACE_BEGIN

class Screen;
class View;
class ViewInstance;
class ViewGroup;
class Window;
class WindowInstance;

class SLIB_EXPORT WindowInstanceParam
{
public:
	Ref<Screen> screen;
	sl_bool flagBorderless;
	sl_bool flagFullScreen;
	sl_bool flagCenterScreen;
	sl_bool flagDialog;
	Point location;
	Size size;

	String title;
	sl_bool flagShowTitleBar;

#if defined(SLIB_PLATFORM_IS_ANDROID)
	// jobject
	void* activity;
#endif

	WindowInstanceParam();
	Rectangle calculateRegion(const Rectangle& screenFrame) const;
};

class SLIB_EXPORT WindowParam : public WindowInstanceParam
{
public:
	Ref<WindowInstance> parent;
	
	Color backgroundColor;
	
	sl_bool flagVisible;
	sl_bool flagMinimized;
	sl_bool flagMaximized;
	
	sl_bool flagAlwaysOnTop;
	sl_bool flagCloseButtonEnabled;
	sl_bool flagMinimizeButtonEnabled;
	sl_bool flagMaximizeButtonEnabled;
	sl_bool flagResizable;
	sl_real alpha;
	sl_bool flagTransparent;

	sl_bool flagModal;

	WindowParam();
	WindowParam(Rectangle _rect);
	WindowParam(sl_real width, sl_real height);
	WindowParam(sl_real x, sl_real y, sl_real width, sl_real height);
	WindowParam(String _title, Rectangle _rect);
	WindowParam(String _title, sl_real width, sl_real height);
	WindowParam(String _title, sl_real x, sl_real y, sl_real width, sl_real height);
	
	void setParent(const Ref<Window>& parent);
	
private:
	void init();
};

class SLIB_EXPORT Window : public Object
{
	SLIB_DECLARE_OBJECT(Window, Object)
public:
	Window();
	
	virtual void close();
	sl_bool isClosed();
	
	const Ref<ViewGroup>& getContentView();
	virtual void setContentView(const Ref<ViewGroup>& view);
	
	Ref<WindowInstance> getParentInstance();
	virtual void setParentInstance(const Ref<WindowInstance>& parent);
	
	Ref<Window> getParent();
	virtual void setParent(Window* parent);
	
	virtual void setFocus();

	String getTitle();
	virtual void setTitle(const String& title);
	
	Color getBackgroundColor();
	virtual void setBackgroundColor(const Color& color);
	
	Rectangle getFrame();
	virtual void setFrame(const Rectangle& frame);
	
	Rectangle getClientFrame();
	
	Size getClientSize();
	virtual void setClientSize(const Size& size);
	
	sl_bool isMinimized();
	virtual void setMinimized(sl_bool flag);
	
	sl_bool isMaximized();
	virtual void setMaximized(sl_bool flag);
	
	sl_bool isVisible();
	virtual void setVisible(sl_bool flag);
	
	sl_bool isAlwaysOnTop();
	virtual void setAlwaysOnTop(sl_bool flag);
	
	sl_bool isCloseButtonEnabled();
	virtual void setCloseButtonEnabled(sl_bool flag);
	
	sl_bool isMinimizeButtonEnabled();
	virtual void setMinimizeButtonEnabled(sl_bool flag);
	
	sl_bool isMaximizeButtonEnabled();
	virtual void setMaximizeButtonEnabled(sl_bool flag);
	
	sl_bool isResizable();
	virtual void setResizable(sl_bool flag);
	
	sl_real getAlpha();
	virtual void setAlpha(sl_real alpha);
	
	sl_bool isTransparent();
	virtual void setTransparent(sl_bool flag);

	virtual Point convertCoordinateFromScreenToWindow(const Point& ptScreen);
	virtual Point convertCoordinateFromWindowToScreen(const Point& ptWindow);
	virtual Point convertCoordinateFromScreenToClient(const Point& ptScreen);
	virtual Point convertCoordinateFromClientToScreen(const Point& ptClient);
	virtual Point convertCoordinateFromWindowToClient(const Point& ptWindow);
	virtual Point convertCoordinateFromClientToWindow(const Point& ptClient);
	virtual Size getWindowSizeFromClientSize(const Size& sizeClient);
	virtual Size getClientSizeFromWindowSize(const Size& sizeWindow);

public:
	SLIB_PROPERTY_INLINE(Ptr<IWindowListener>, EventListener)

public:
	SLIB_INLINE Ref<WindowInstance> getWindowInstance()
	{
		return m_instance;
	}
	void detach();

public:
	virtual sl_bool createWindow(const WindowParam& param);
	
	void attach(const Ref<WindowInstance>& instance);
	void create(const WindowParam& param, const Ref<Runnable>& callbackSuccess, const Ref<Runnable>& callbackFail);
	
	SLIB_INLINE void create(const WindowParam& param)
	{
		create(param, Ref<Runnable>::null(), Ref<Runnable>::null());
	}
	SLIB_INLINE void create()
	{
		WindowParam param;
		create(param);
	}
	SLIB_INLINE void create(Rectangle rect)
	{
		WindowParam param(rect);
		return create(param);
	}
	SLIB_INLINE void create(sl_real width, sl_real height)
	{
		WindowParam param(width, height);
		return create(param);
	}
	SLIB_INLINE void create(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		WindowParam param(x, y, width, height);
		return create(param);
	}
	SLIB_INLINE void create(String title, Rectangle rect)
	{
		WindowParam param(title, rect);
		return create(param);
	}
	SLIB_INLINE void create(String title, sl_real width, sl_real height)
	{
		WindowParam param(title, width, height);
		return create(param);
	}
	SLIB_INLINE void create(String title, sl_real x, sl_real y, sl_real width, sl_real height)
	{
		WindowParam param(title, x, y, width, height);
		return create(param);
	}
	
	void runModal();

	virtual Point getLocation()
	{
		return getFrame().getLocation();
	}
	virtual void setLocation(const Point& location)
	{
		Rectangle frame = getFrame();
		frame.setLocation(location);
		setFrame(frame);
	}
	SLIB_INLINE void setLocation(sl_real x, sl_real y)
	{
		setLocation(Point(x, y));
	}

	virtual Size getSize()
	{
		return getFrame().getSize();
	}
	virtual void setSize(const Size& size)
	{
		Rectangle frame = getFrame();
		frame.setSize(size);
		setFrame(frame);
	}
	SLIB_INLINE void setSize(sl_real width, sl_real height)
	{
		setSize(Size(width, height));
	}
	SLIB_INLINE void setClientSize(sl_real width, sl_real height)
	{
		setClientSize(Size(width, height));
	}
	SLIB_INLINE void setFrame(sl_real left, sl_real top, sl_real width, sl_real height)
	{
		Rectangle rect;
		rect.left = left;
		rect.top = top;
		rect.setSize(width, height);
		setFrame(rect);
	}

	SLIB_INLINE Rectangle convertCoordinateFromScreenToWindow(const Rectangle& rect)
	{
		Rectangle ret;
		ret.setLeftTop(convertCoordinateFromScreenToWindow(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromScreenToWindow(rect.getRightBottom()));
		return ret;
	}
	SLIB_INLINE Rectangle convertCoordinateFromWindowToScreen(const Rectangle& rect)
	{
		Rectangle ret;
		ret.setLeftTop(convertCoordinateFromWindowToScreen(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromWindowToScreen(rect.getRightBottom()));
		return ret;
	}
	SLIB_INLINE Rectangle convertCoordinateFromScreenToClient(const Rectangle& rect)
	{
		Rectangle ret;
		ret.setLeftTop(convertCoordinateFromScreenToClient(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromScreenToClient(rect.getRightBottom()));
		return ret;
	}
	SLIB_INLINE Rectangle convertCoordinateFromClientToScreen(const Rectangle& rect)
	{
		Rectangle ret;
		ret.setLeftTop(convertCoordinateFromClientToScreen(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromClientToScreen(rect.getRightBottom()));
		return ret;
	}
	SLIB_INLINE Rectangle convertCoordinateFromWindowToClient(const Rectangle& rect)
	{
		Rectangle ret;
		ret.setLeftTop(convertCoordinateFromWindowToClient(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromWindowToClient(rect.getRightBottom()));
		return ret;
	}
	SLIB_INLINE Rectangle convertCoordinateFromClientToWindow(const Rectangle& rect)
	{
		Rectangle ret;
		ret.setLeftTop(convertCoordinateFromClientToWindow(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromClientToWindow(rect.getRightBottom()));
		return ret;
	}
	
	void addView(const Ref<View>& view);

	void removeView(const Ref<View>& view);

	List< Ref<View> > getViews();
	void removeAllViews();
	
public:
	virtual void onCreate();
	virtual sl_bool onClose();
	virtual void onActivate();
	virtual void onDeactivate();
	virtual void onMove();
	virtual void onResize(Size& size);
	virtual void onMinimize();
	virtual void onDeminimize();
	virtual void onMaximize();
	virtual void onDemaximize();
	
private:
	static Ref<WindowInstance> createWindowInstance(const WindowInstanceParam& param);
	void _createCallback(WindowParam param, Ref<Runnable> callbackSuccess, Ref<Runnable> callbackFail);
	void _runModal();
	void _refreshSize();
	
private:
	friend class WindowInstance;
	
	Ref<WindowInstance> m_instance;
	
	Ref<ViewGroup> m_viewContent;
};

class SLIB_EXPORT WindowInstance : public Object
{
	SLIB_DECLARE_OBJECT(WindowInstance, Object)
protected:
	WindowInstance();

public:
	virtual void close() = 0;
	virtual sl_bool isClosed() = 0;
	
	virtual const Ref<ViewInstance>& getContentView() = 0;

	virtual Ref<WindowInstance> getParent() = 0;
	virtual sl_bool setParent(const Ref<WindowInstance>& parent) = 0;
	
	virtual sl_bool setFocus() = 0;
	
	virtual String getTitle() = 0;
	virtual sl_bool setTitle(const String& title) = 0;
	
	virtual Color getBackgroundColor() = 0;
	virtual sl_bool setBackgroundColor(const Color& color) = 0;

	virtual Rectangle getFrame() = 0;
	virtual sl_bool setFrame(const Rectangle& frame) = 0;
	
	virtual Rectangle getClientFrame() = 0;
	
	virtual Size getClientSize() = 0;
	virtual sl_bool setClientSize(const Size& size) = 0;
	
	virtual sl_bool isMinimized() = 0;
	virtual sl_bool setMinimized(sl_bool flag) = 0;
	
	virtual sl_bool isMaximized() = 0;
	virtual sl_bool setMaximized(sl_bool flag) = 0;
	
	virtual sl_bool isVisible() = 0;
	virtual sl_bool setVisible(sl_bool flag) = 0;
	
	virtual sl_bool isAlwaysOnTop() = 0;
	virtual sl_bool setAlwaysOnTop(sl_bool flag) = 0;
	
	virtual sl_bool isCloseButtonEnabled() = 0;
	virtual sl_bool setCloseButtonEnabled(sl_bool flag) = 0;
	
	virtual sl_bool isMinimizeButtonEnabled() = 0;
	virtual sl_bool setMinimizeButtonEnabled(sl_bool flag) = 0;
	
	virtual sl_bool isMaximizeButtonEnabled() = 0;
	virtual sl_bool setMaximizeButtonEnabled(sl_bool flag) = 0;
	
	virtual sl_bool isResizable() = 0;
	virtual sl_bool setResizable(sl_bool flag) = 0;
	
	virtual sl_real getAlpha() = 0;
	virtual sl_bool setAlpha(sl_real alpha) = 0;
	
	virtual sl_bool isTransparent() = 0;
	virtual sl_bool setTransparent(sl_bool flag) = 0;

	virtual Point convertCoordinateFromScreenToWindow(const Point& ptScreen) = 0;
	virtual Point convertCoordinateFromWindowToScreen(const Point& ptWindow) = 0;
	virtual Point convertCoordinateFromScreenToClient(const Point& ptScreen) = 0;
	virtual Point convertCoordinateFromClientToScreen(const Point& ptClient) = 0;
	virtual Point convertCoordinateFromWindowToClient(const Point& ptWindow) = 0;
	virtual Point convertCoordinateFromClientToWindow(const Point& ptClient) = 0;
	virtual Size getWindowSizeFromClientSize(const Size& sizeClient) = 0;
	virtual Size getClientSizeFromWindowSize(const Size& sizeWindow) = 0;
	
	virtual void runModal() = 0;

private:
	WeakRef<Window> m_window;

public:
	void setWindow(Ref<Window> window);
	Ref<Window> getWindow();

public:
	sl_bool onClose();
	void onActivate();
	void onDeactivate();
	void onMove();
	void onResize(Size& size);
	void onMinimize();
	void onDeminimize();
	void onMaximize();
	void onDemaximize();
};

SLIB_UI_NAMESPACE_END

#endif
