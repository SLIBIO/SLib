#include "../../../inc/slib/ui/window.h"

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

WindowInstanceParam::WindowInstanceParam()
{
	flagBorderless = sl_false;
	flagShowTitleBar = sl_false;
	flagFullScreen = sl_true;
	flagCenterScreen = sl_true;
	flagDialog = sl_false;
	
#if defined(SLIB_PLATFORM_IS_ANDROID)
	activity = sl_null;
#endif
}

Rectangle WindowInstanceParam::calculateRegion(const Rectangle& screenFrame) const
{
	Rectangle frame;
	if (flagFullScreen) {
		frame.setLeftTop(0, 0);
		frame.setSize(screenFrame.getSize());
	} else {
		if (flagCenterScreen) {
			frame.setLeftTop(screenFrame.getWidth() / 2 - size.x / 2, screenFrame.getHeight() / 2 - size.y / 2);
		} else {
			frame.setLeftTop(location);
		}
		frame.setSize(size);
	}
	return frame;
}

#define CHECK_INSTANCE(instance) (instance.isNotNull() && !(instance->isClosed()))

SLIB_DEFINE_OBJECT(Window, Object)

Window::Window()
{
	SLIB_REFERABLE_CONSTRUCTOR;
	
	Ref<View> view = new ViewGroup;
	if (view.isNotNull()) {
		view->setWindow(this);
		m_viewContent = view;
	}
	
	m_frame.left = 100;
	m_frame.top = 100;
	m_frame.right = 500;
	m_frame.bottom = 400;
	
	m_backgroundColor = Color::zero();
	
	m_flagVisible = sl_true;
	m_flagMinimized = sl_false;
	m_flagMaximized = sl_false;
	
	m_flagAlwaysOnTop = sl_false;
	m_flagCloseButtonEnabled = sl_true;
	m_flagMinimizeButtonEnabled = sl_false;
	m_flagMaximizeButtonEnabled = sl_false;
	m_flagResizable = sl_false;
	
	m_alpha = 1.0;
	m_flagTransparent = sl_false;
	
	m_flagModal = sl_false;
	m_flagDialog = sl_false;
	m_flagBorderless = sl_false;
	m_flagShowTitleBar = sl_true;
	m_flagFullScreenOnCreate = SLIB_IF_PLATFORM_IS_MOBILE(sl_true, sl_false);
	m_flagCenterScreenOnCreate = sl_false;
	
	m_activity = sl_null;
	
}

void Window::close()
{
	ObjectLocker lock(this);
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->close();
	}
	detach();
}

sl_bool Window::isClosed()
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->isClosed();
	}
	return sl_true;
}

Ref<Window> Window::getParent()
{
	return m_parent;
}

void Window::setParent(const Ref<Window>& parent)
{
	m_parent = parent;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		if (parent.isNotNull()) {
			instance->setParent(parent->m_instance);
		} else {
			instance->setParent(Ref<WindowInstance>::null());
		}
	}
}

Ref<Screen> Window::getScreen()
{
	return m_screen;
}

void Window::setScreen(const Ref<Screen>& screen)
{
	m_screen = screen;
}

Ref<View> Window::getContentView()
{
	return m_viewContent;
}

void Window::setContentView(const Ref<View>& view)
{
	ObjectLocker lock(this);
	
	Ref<View> orig = m_viewContent;
	Ref<ViewInstance> instance;
	if (orig.isNotNull()) {
		instance = orig->getViewInstance();
		orig->detach();
	}
	if (view.isNotNull() && instance.isNotNull()) {
		view->attach(instance);
		view->setWindow(this);
	}
	m_viewContent = view;
}


Ref<Menu> Window::getMenu()
{
	return m_menu;
}

void Window::setMenu(const Ref<Menu>& menu)
{
	m_menu = menu;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMenu(menu);
	}
}


void Window::setFocus()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setFocus();
	}
}

void Window::runModal()
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(Window, _runModal, this));
	}
}

void Window::_runModal()
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->runModal();
	}
}

Rectangle Window::getFrame()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		m_frame = instance->getFrame();
	}
	return m_frame;
}

void Window::setFrame(const Rectangle& frame)
{
	m_frame = frame;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setFrame(frame);
	}
}

void Window::setFrame(sl_real left, sl_real top, sl_real width, sl_real height)
{
	Rectangle rect;
	rect.left = left;
	rect.top = top;
	rect.setSize(width, height);
	setFrame(rect);
}

Point Window::getLocation()
{
	return getFrame().getLocation();
}

void Window::setLocation(const Point& location)
{
	Rectangle frame = getFrame();
	frame.setLocation(location);
	setFrame(frame);
}

void Window::setLocation(sl_real x, sl_real y)
{
	setLocation(Point(x, y));
}

Size Window::getSize()
{
	return getFrame().getSize();
}

void Window::setSize(const Size& size)
{
	Rectangle frame = getFrame();
	frame.setSize(size);
	setFrame(frame);
}

void Window::setSize(sl_real width, sl_real height)
{
	setSize(Size(width, height));
}

Rectangle Window::getClientFrame()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getClientFrame();
	}
	return Rectangle::zero();
}

Size Window::getClientSize()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getClientSize();
	}
	return Size::zero();
}

void Window::setClientSize(const Size& size)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setClientSize(size);
	}
}

void Window::setClientSize(sl_real width, sl_real height)
{
	setClientSize(Size(width, height));
}

String Window::getTitle()
{
	return m_title;
}

void Window::setTitle(const String& title)
{
	m_title = title;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setTitle(title);
	}
}

Color Window::getBackgroundColor()
{
	return m_backgroundColor;
}

void Window::setBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setBackgroundColor(color);
	}
}

sl_bool Window::isMinimized()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		m_flagMinimized = instance->isMinimized();
	}
	return m_flagMinimized;
}

void Window::setMinimized(sl_bool flag)
{
	m_flagMinimized = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMinimized(flag);
	}
}

sl_bool Window::isMaximized()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		m_flagMaximized = instance->isMaximized();
	}
	return m_flagMaximized;
}

void Window::setMaximized(sl_bool flag)
{
	m_flagMaximized = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMaximized(flag);
	}
}

sl_bool Window::isVisible()
{
	return m_flagVisible;
}

void Window::setVisible(sl_bool flag)
{
	m_flagVisible = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setVisible(flag);
	}
}

sl_bool Window::isAlwaysOnTop()
{
	return m_flagAlwaysOnTop;
}

void Window::setAlwaysOnTop(sl_bool flag)
{
	m_flagAlwaysOnTop = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setAlwaysOnTop(flag);
	}
}

sl_bool Window::isCloseButtonEnabled()
{
	return m_flagCloseButtonEnabled;
}

void Window::setCloseButtonEnabled(sl_bool flag)
{
	m_flagCloseButtonEnabled = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setCloseButtonEnabled(flag);
	}
}

sl_bool Window::isMinimizeButtonEnabled()
{
	return m_flagMinimizeButtonEnabled;
}

void Window::setMinimizeButtonEnabled(sl_bool flag)
{
	m_flagMinimizeButtonEnabled = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMinimizeButtonEnabled(flag);
	}
}

sl_bool Window::isMaximizeButtonEnabled()
{
	return m_flagMaximizeButtonEnabled;
}

void Window::setMaximizeButtonEnabled(sl_bool flag)
{
	m_flagMaximizeButtonEnabled = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMaximizeButtonEnabled(flag);
	}
}

sl_bool Window::isResizable()
{
	return m_flagResizable;
}

void Window::setResizable(sl_bool flag)
{
	m_flagResizable = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setResizable(flag);
	}
}

sl_real Window::getAlpha()
{
	return m_alpha;
}

void Window::setAlpha(sl_real alpha)
{
	m_alpha = alpha;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setAlpha(alpha);
	}
}

sl_bool Window::isTransparent()
{
	return m_flagTransparent;
}

void Window::setTransparent(sl_bool flag)
{
	m_flagTransparent = flag;
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setTransparent(flag);
	}
}

Point Window::convertCoordinateFromScreenToWindow(const Point& ptScreen)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromScreenToWindow(ptScreen);
	} else {
		return Point::zero();
	}
}

Rectangle Window::convertCoordinateFromScreenToWindow(const Rectangle& rect)
{
	Rectangle ret;
	ret.setLeftTop(convertCoordinateFromScreenToWindow(rect.getLeftTop()));
	ret.setRightBottom(convertCoordinateFromScreenToWindow(rect.getRightBottom()));
	return ret;
}

Point Window::convertCoordinateFromWindowToScreen(const Point& ptWindow)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromWindowToScreen(ptWindow);
	} else {
		return Point::zero();
	}
}

Rectangle Window::convertCoordinateFromWindowToScreen(const Rectangle& rect)
{
	Rectangle ret;
	ret.setLeftTop(convertCoordinateFromWindowToScreen(rect.getLeftTop()));
	ret.setRightBottom(convertCoordinateFromWindowToScreen(rect.getRightBottom()));
	return ret;
}

Point Window::convertCoordinateFromScreenToClient(const Point& ptScreen)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromScreenToClient(ptScreen);
	} else {
		return Point::zero();
	}
}

Rectangle Window::convertCoordinateFromScreenToClient(const Rectangle& rect)
{
	Rectangle ret;
	ret.setLeftTop(convertCoordinateFromScreenToClient(rect.getLeftTop()));
	ret.setRightBottom(convertCoordinateFromScreenToClient(rect.getRightBottom()));
	return ret;
}

Point Window::convertCoordinateFromClientToScreen(const Point& ptClient)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromClientToScreen(ptClient);
	} else {
		return Point::zero();
	}
}

Rectangle Window::convertCoordinateFromClientToScreen(const Rectangle& rect)
{
	Rectangle ret;
	ret.setLeftTop(convertCoordinateFromClientToScreen(rect.getLeftTop()));
	ret.setRightBottom(convertCoordinateFromClientToScreen(rect.getRightBottom()));
	return ret;
}

Point Window::convertCoordinateFromWindowToClient(const Point& ptWindow)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromWindowToClient(ptWindow);
	} else {
		return Point::zero();
	}
}

Rectangle Window::convertCoordinateFromWindowToClient(const Rectangle& rect)
{
	Rectangle ret;
	ret.setLeftTop(convertCoordinateFromWindowToClient(rect.getLeftTop()));
	ret.setRightBottom(convertCoordinateFromWindowToClient(rect.getRightBottom()));
	return ret;
}

Point Window::convertCoordinateFromClientToWindow(const Point& ptClient)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromClientToWindow(ptClient);
	} else {
		return Point::zero();
	}
}

Rectangle Window::convertCoordinateFromClientToWindow(const Rectangle& rect)
{
	Rectangle ret;
	ret.setLeftTop(convertCoordinateFromClientToWindow(rect.getLeftTop()));
	ret.setRightBottom(convertCoordinateFromClientToWindow(rect.getRightBottom()));
	return ret;
}

Size Window::getWindowSizeFromClientSize(const Size& sizeClient)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->getWindowSizeFromClientSize(sizeClient);
	} else {
		return Point::zero();
	}
}

Size Window::getClientSizeFromWindowSize(const Size& sizeWindow)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->getClientSizeFromWindowSize(sizeWindow);
	} else {
		return Point::zero();
	}
}

sl_bool Window::isModal()
{
	return m_flagModal;
}

void Window::setModal(sl_bool flag)
{
	m_flagModal = flag;
}

sl_bool Window::isDialog()
{
	return m_flagDialog;
}

void Window::setDialog(sl_bool flag)
{
	m_flagDialog = flag;
}

sl_bool Window::isBorderless()
{
	return m_flagBorderless;
}

void Window::setBorderless(sl_bool flag)
{
	m_flagBorderless = flag;
}

sl_bool Window::isTitleBarVisible()
{
	return m_flagShowTitleBar;
}

void Window::setTitleBarVisible(sl_bool flag)
{
	m_flagShowTitleBar = flag;
}

sl_bool Window::isFullScreenOnCreate()
{
	return m_flagFullScreenOnCreate;
}

void Window::setFullScreenOnCreate(sl_bool flag)
{
	m_flagFullScreenOnCreate = flag;
}

sl_bool Window::isCenterScreenOnCreate()
{
	return m_flagCenterScreenOnCreate;
}

void Window::setCenterScreenOnCreate(sl_bool flag)
{
	m_flagCenterScreenOnCreate = flag;
}

void* Window::getActivity()
{
	return m_activity;
}

void Window::setActivity(void* activity)
{
	m_activity = activity;
}

Ref<WindowInstance> Window::getWindowInstance()
{
	return m_instance;
}

void Window::attach(const Ref<WindowInstance>& instance)
{
	ObjectLocker lock(this);

	detach();
	
	if (instance.isNotNull()) {
		
		instance->setWindow(this);
		
		Ref<ViewInstance> contentViewInstance = instance->getContentView();
		if (contentViewInstance.isNotNull()) {
			Rectangle rect;
			rect.setLeftTop(Point::zero());
			rect.setSize(instance->getClientSize());
			Ref<View> view = m_viewContent;
			if (view.isNotNull()) {
				view->attach(contentViewInstance);
				view->m_frame = rect;
			}
		}
	}
	m_instance = instance;
}

void Window::detach()
{
	ObjectLocker lock(this);
	
	// save current window's properties
	getFrame();
	isMaximized();
	isMinimized();
	
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		view->detach();
	}
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setWindow(Ref<Window>::null());
	}
	m_instance.setNull();
}

void Window::create()
{
	if (UI::isUiThread()) {
		_create();
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_REF(Window, _create, this));
	}
}

void Window::_create()
{
	if (m_instance.isNotNull()) {
		return;
	}
	
	WindowInstanceParam param;
	param.screen = m_screen;
	param.menu = m_menu;
	param.flagBorderless = m_flagBorderless;
	param.flagFullScreen = m_flagFullScreenOnCreate;
	param.flagCenterScreen = m_flagCenterScreenOnCreate;
	param.flagDialog = m_flagDialog;
	param.location = m_frame.getLocation();
	param.size = m_frame.getSize();
	param.title = m_title;
	param.flagShowTitleBar = m_flagShowTitleBar;
	param.activity = m_activity;
	
	Ref<WindowInstance> window = createWindowInstance(param);
	
	if (window.isNotNull()) {
		
		Ref<Window> parent = m_parent;
		if (parent.isNotNull()) {
			window->setParent(parent->m_instance);
		} else {
			window->setParent(Ref<WindowInstance>::null());
		}
		
		window->setBackgroundColor(m_backgroundColor);
		
		window->setVisible(m_flagVisible);
		window->setMinimized(m_flagMinimized);
		window->setMaximized(m_flagMaximized);
		
		window->setAlwaysOnTop(m_flagAlwaysOnTop);
		window->setCloseButtonEnabled(m_flagCloseButtonEnabled);
		window->setMinimizeButtonEnabled(m_flagMinimizeButtonEnabled);
		window->setMaximizeButtonEnabled(m_flagMaximizeButtonEnabled);
		window->setResizable(m_flagResizable);
		window->setAlpha(m_alpha);
		window->setTransparent(m_flagTransparent);
		
		window->setVisible(sl_true);
		
		attach(window);
		
		onCreate();
		
#if !defined(SLIB_PLATFORM_IS_OSX)
		Size sizeOld = getSize();
		if (sizeOld.x > 0 && sizeOld.y > 0) {
			Size size = sizeOld;
			onResize(size);
			if (size != sizeOld) {
				setSize(size);
			}
		}
#endif
		
		if (m_flagModal) {
			_runModal();
		}
		
	}
	
	onCreateFailed();
	
}

void Window::addView(const Ref<View>& child)
{
	if (child.isNotNull()) {
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			view->addChild(child);
		}
	}
}

void Window::removeView(const Ref<View>& child)
{
	if (child.isNotNull()) {
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			view->removeChild(child);
		}
	}
}

List< Ref<View> > Window::getViews()
{
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		return view->getChildren();
	}
	return List< Ref<View> >::null();
}

void Window::removeAllViews()
{
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		view->removeAllChildren();
	}
}

void Window::onCreate()
{
}

void Window::onCreateFailed()
{
}

sl_bool Window::onClose()
{
	PtrLocker<IWindowListener> listener(getEventListener());
	sl_bool flagClose = sl_true;
	if (listener.isNotNull()) {
		flagClose = listener->onClose(this);
	}
	if (flagClose) {
		detach();
	}
	return flagClose;
}

void Window::onActivate()
{
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onActivate(this);
	}
}

void Window::onDeactivate()
{
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onDeactivate(this);
	}
}

void Window::onMove()
{
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMove(this);
	}
}

void Window::onResize(Size& size)
{
	_refreshSize();
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onResize(this, size);
	}
}

void Window::onMinimize()
{
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMinimize(this);
	}
}

void Window::onDeminimize()
{
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onDeminimize(this);
	}
}

void Window::onMaximize()
{
	_refreshSize();
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMaximize(this);
	}
}

void Window::onDemaximize()
{
	_refreshSize();
	PtrLocker<IWindowListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onDemaximize(this);
	}
}

void Window::_refreshSize()
{
#if !defined(SLIB_PLATFORM_IS_WIN32)
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		Rectangle rect;
		rect.setLeftTop(Point::zero());
		rect.setSize(getClientSize());
		view->setFrame(rect);
	}
#endif
}


SLIB_DEFINE_OBJECT(WindowInstance, Object)

Ref<Window> WindowInstance::getWindow()
{
	return m_window;
}

void WindowInstance::setWindow(const Ref<Window>& window)
{
	m_window = window;
}

void WindowInstance::setMenu(const Ref<Menu>& menu)
{
}

void WindowInstance::onCreate()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onCreate();
	}
}

sl_bool WindowInstance::onClose()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		return window->onClose();
	} else {
		return sl_true;
	}
}

void WindowInstance::onActivate()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onActivate();
	}
}

void WindowInstance::onDeactivate()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onDeactivate();
	}
}

void WindowInstance::onMove()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onMove();
	}
}

void WindowInstance::onResize(Size& size)
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onResize(size);
	}
}

void WindowInstance::onMinimize()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onMinimize();
	}
}

void WindowInstance::onDeminimize()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onDeminimize();
	}
}

void WindowInstance::onMaximize()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onMaximize();
	}
}

void WindowInstance::onDemaximize()
{
	Ref<Window> window = getWindow();
	if (window.isNotNull()) {
		window->onDemaximize();
	}
}

SLIB_UI_NAMESPACE_END
