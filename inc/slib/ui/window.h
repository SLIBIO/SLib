#ifndef CHECKHEADER_SLIB_UI_WINDOW
#define CHECKHEADER_SLIB_UI_WINDOW

#include "definition.h"

#include "event.h"
#include "menu.h"

#include "../core/string.h"
#include "../core/object.h"
#include "../core/function.h"
#include "../core/ptr.h"
#include "../graphics/color.h"
#include "../math/rectangle.h"

SLIB_UI_NAMESPACE_BEGIN

class Screen;
class View;
class ViewInstance;
class Window;
class WindowInstance;

class SLIB_EXPORT WindowInstanceParam
{
public:
	Ref<Screen> screen;
	Ref<Menu> menu;
	sl_bool flagBorderless;
	sl_bool flagFullScreen;
	sl_bool flagCenterScreen;
	sl_bool flagDialog;
	UIPoint location;
	UISize size;

	String title;
	sl_bool flagShowTitleBar;

	// jobject
	void* activity;

public:
	WindowInstanceParam();
	
	~WindowInstanceParam();
	
public:
	UIRect calculateRegion(const UIRect& screenFrame) const;
	
};

class SLIB_EXPORT Window : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	Window();
	
	~Window();
	
public:
	virtual void close();
	
	sl_bool isClosed();
	
	sl_bool isOpened();
	
	
	Ref<Window> getParent();
	
	virtual void setParent(const Ref<Window>& parent);
	
	
	Ref<Screen> getScreen();
	
	void setScreen(const Ref<Screen>& screen);

	
	Ref<View> getContentView();
	
	virtual void setContentView(const Ref<View>& view);
	
	
	Ref<Menu> getMenu();
	
	virtual void setMenu(const Ref<Menu>& menu);
	
	
	virtual void setFocus();
	
	void runModal();
	
	
	UIRect getFrame();
	
	virtual void setFrame(const UIRect& frame);
	
	void setFrame(sl_ui_pos left, sl_ui_pos top, sl_ui_len width, sl_ui_len height);
	
	UIPoint getLocation();
	
	virtual void setLocation(const UIPoint& location);
	
	void setLocation(sl_ui_pos x, sl_ui_pos y);
	
	sl_ui_pos getLeft();
	
	void setLeft(sl_ui_pos x);
	
	sl_ui_pos getTop();
	
	void setTop(sl_ui_pos y);
	
	virtual UISize getSize();
	
	virtual void setSize(const UISize& size);
	
	void setSize(sl_ui_len width, sl_ui_len height);
	
	sl_ui_len getWidth();
	
	void setWidth(sl_ui_len width);
	
	sl_ui_len getHeight();
	
	void setHeight(sl_ui_len height);

	
	UIRect getClientFrame();
	
	UISize getClientSize();
	
	virtual void setClientSize(const UISize& size);

	void setClientSize(sl_ui_len width, sl_ui_len height);
	
	UIRect getClientBounds();

	
	String getTitle();
	
	virtual void setTitle(const String& title);
	
	
	Color getBackgroundColor();
	
	virtual void setBackgroundColor(const Color& color);
	
	
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

	
	virtual UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen);
	
	UIRectf convertCoordinateFromScreenToWindow(const UIRectf& rect);
	
	virtual UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow);

	UIRectf convertCoordinateFromWindowToScreen(const UIRectf& rect);
	
	virtual UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen);
	
	UIRectf convertCoordinateFromScreenToClient(const UIRectf& rect);
	
	virtual UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient);
	
	UIRectf convertCoordinateFromClientToScreen(const UIRectf& rect);
	
	virtual UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow);
	
	UIRectf convertCoordinateFromWindowToClient(const UIRectf& rect);
	
	virtual UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient);
	
	UIRectf convertCoordinateFromClientToWindow(const UIRectf& rect);
	
	virtual UISize getWindowSizeFromClientSize(const UISize& sizeClient);
	
	virtual UISize getClientSizeFromWindowSize(const UISize& sizeWindow);

	
	sl_bool isModal();
	
	void setModal(sl_bool flag);
	

	sl_bool isDialog();
	
	void setDialog(sl_bool flag);
	
	
	sl_bool isBorderless();
	
	void setBorderless(sl_bool flag);
	
	
	sl_bool isTitleBarVisible();
	
	void setTitleBarVisible(sl_bool flag);

	
	sl_bool isFullScreenOnCreate();
	
	void setFullScreenOnCreate(sl_bool flag);
	
	
	sl_bool isCenterScreenOnCreate();
	
	void setCenterScreenOnCreate(sl_bool flag);
	
	
	void* getActivity();
	
	void setActivity(void* activity);

	
public:
	Ref<WindowInstance> getWindowInstance();
	
	void attach(const Ref<WindowInstance>& instance);

	void detach();

	void create();
	
	void forceCreate();

	
	void addView(const Ref<View>& view);

	void removeView(const Ref<View>& view);

	List< Ref<View> > getViews();
	
	void removeAllViews();

public:
	SLIB_PROPERTY(AtomicPtr<IWindowListener>, EventListener)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnCreate)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnCreateFailed)
	
	SLIB_PROPERTY(AtomicFunction<void(UIEvent*)>, OnClose)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnActivate)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnDeactivate)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnMove)
	
	SLIB_PROPERTY(AtomicFunction<void(UISize&)>, OnResizing)
	
	SLIB_PROPERTY(AtomicFunction<void(sl_ui_len, sl_ui_len)>, OnResize)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnMinimize)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnDeminimize)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnMaximize)
	
	SLIB_PROPERTY(AtomicFunction<void()>, OnDemaximize)

protected:
	virtual void onCreate();
	
	virtual void onCreateFailed();
	
	virtual void onClose(UIEvent* ev);
	
	virtual void onActivate();
	
	virtual void onDeactivate();
	
	virtual void onMove();
	
	virtual void onResizing(UISize& size);
	
	virtual void onResize(sl_ui_len width, sl_ui_len height);

	virtual void onMinimize();
	
	virtual void onDeminimize();
	
	virtual void onMaximize();
	
	virtual void onDemaximize();
	
public:
	virtual void dispatchCreate();
	
	virtual void dispatchCreateFailed();
	
	virtual void dispatchClose(UIEvent* ev);
	
	virtual void dispatchActivate();
	
	virtual void dispatchDeactivate();
	
	virtual void dispatchMove();
	
	virtual void dispatchResizing(UISize& size);
	
	virtual void dispatchResize(sl_ui_len width, sl_ui_len height);
	
	virtual void dispatchMinimize();
	
	virtual void dispatchDeminimize();
	
	virtual void dispatchMaximize();
	
	virtual void dispatchDemaximize();

private:
	Ref<WindowInstance> createWindowInstance(const WindowInstanceParam& param);

	void _runModal();
	
	void _create();
	
	void _refreshSize();
	
private:
	AtomicRef<WindowInstance> m_instance;
	AtomicWeakRef<Window> m_parent;
	AtomicRef<Screen> m_screen;
	AtomicRef<View> m_viewContent;
	AtomicRef<Menu> m_menu;
	
	UIRect m_frame;
	AtomicString m_title;
	Color m_backgroundColor;
	
	sl_bool m_flagVisible;
	sl_bool m_flagMinimized;
	sl_bool m_flagMaximized;
	
	sl_bool m_flagAlwaysOnTop;
	sl_bool m_flagCloseButtonEnabled;
	sl_bool m_flagMinimizeButtonEnabled;
	sl_bool m_flagMaximizeButtonEnabled;
	sl_bool m_flagResizable;
	sl_real m_alpha;
	sl_bool m_flagTransparent;
	
	sl_bool m_flagModal;
	sl_bool m_flagDialog;
	sl_bool m_flagBorderless;
	sl_bool m_flagShowTitleBar;
	sl_bool m_flagFullScreenOnCreate;
	sl_bool m_flagCenterScreenOnCreate;

	// jobject
	void* m_activity;
	
};


class SLIB_EXPORT WindowInstance : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	Ref<Window> getWindow();
	
	void setWindow(const Ref<Window>& window);
	
public:
	virtual void close() = 0;
	
	virtual sl_bool isClosed() = 0;
	

	virtual sl_bool setParent(const Ref<WindowInstance>& parent) = 0;
	
	virtual Ref<ViewInstance> getContentView() = 0;
	
	virtual void setMenu(const Ref<Menu>& menu);
	
	
	virtual sl_bool setFocus() = 0;
	
	virtual void runModal() = 0;

	
	virtual UIRect getFrame() = 0;
	
	virtual sl_bool setFrame(const UIRect& frame) = 0;
	
	virtual UIRect getClientFrame() = 0;
	
	virtual UISize getClientSize() = 0;
	
	virtual sl_bool setClientSize(const UISize& size) = 0;
	

	virtual sl_bool setTitle(const String& title) = 0;
	
	virtual sl_bool setBackgroundColor(const Color& color) = 0;

	
	virtual sl_bool isMinimized() = 0;
	
	virtual sl_bool setMinimized(sl_bool flag) = 0;
	
	
	virtual sl_bool isMaximized() = 0;
	
	virtual sl_bool setMaximized(sl_bool flag) = 0;
	
	
	virtual sl_bool setVisible(sl_bool flag) = 0;

	virtual sl_bool setAlwaysOnTop(sl_bool flag) = 0;
	
	virtual sl_bool setCloseButtonEnabled(sl_bool flag) = 0;
	
	virtual sl_bool setMinimizeButtonEnabled(sl_bool flag) = 0;
	
	virtual sl_bool setMaximizeButtonEnabled(sl_bool flag) = 0;
	
	virtual sl_bool setResizable(sl_bool flag) = 0;
	
	virtual sl_bool setAlpha(sl_real alpha) = 0;
	
	virtual sl_bool setTransparent(sl_bool flag) = 0;

	
	virtual UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen) = 0;
	
	virtual UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow) = 0;
	
	virtual UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen) = 0;
	
	virtual UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient) = 0;
	
	virtual UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow) = 0;
	
	virtual UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient) = 0;
	
	virtual UISize getWindowSizeFromClientSize(const UISize& sizeClient) = 0;
	
	virtual UISize getClientSizeFromWindowSize(const UISize& sizeWindow) = 0;
	
public:
	sl_bool onClose();
	
	void onActivate();
	
	void onDeactivate();
	
	void onMove();
	
	void onResizing(UISize& size);
	
	void onResize(sl_ui_len width, sl_ui_len height);
	
	void onMinimize();
	
	void onDeminimize();
	
	void onMaximize();
	
	void onDemaximize();

private:
	AtomicWeakRef<Window> m_window;
	
};

SLIB_UI_NAMESPACE_END

#endif
