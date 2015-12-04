#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/view_group.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN
WindowInstance::WindowInstance()
{
}

void WindowInstance::setWindow(Ref<Window> window)
{
	m_window = window;
}

Ref<Window> WindowInstance::getWindow()
{
	return m_window.lock();
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


#define CHECK_INSTANCE(instance) (instance.isNotNull() && !(instance->isClosed()))

// Window
SLIB_UI_NAMESPACE_BEGIN
Window::Window()
{
	SLIB_REFERABLE_CONSTRUCTOR;
	
	m_viewContent = new ViewGroup();
	m_viewContent->setWindow(this);
}

void Window::close()
{
	MutexLocker lock(getLocker());
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
	} else {
		return sl_true;
	}
}

const Ref<ViewGroup>& Window::getContentView()
{
	return m_viewContent;
}

void Window::setContentView(const Ref<ViewGroup>& _view)
{
	MutexLocker lock(getLocker());
	
	Ref<ViewGroup> orig = m_viewContent;
	Ref<ViewInstance> instance;
	if (orig.isNotNull()) {
		instance = orig->getViewInstance();
		orig->detach();
	}
	Ref<ViewGroup> view = _view;
	if (view.isNotNull() && instance.isNotNull()) {
		view->attach(instance);
		view->setWindow(this);
	}
	m_viewContent = view;
}

Ref<WindowInstance> Window::getParentInstance()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getParent();
	}
	return Ref<WindowInstance>::null();
}

void Window::setParentInstance(const Ref<WindowInstance>& parent)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setParent(parent);
	}
}

Ref<Window> Window::getParent()
{
	Ref<WindowInstance> instance = getParentInstance();
	if (instance.isNotNull()) {
		return instance->getWindow();
	}
	return Ref<Window>::null();
}

void Window::setParent(Window* parent)
{
	if (parent) {
		setParentInstance(parent->getWindowInstance());
	}
}

void Window::setFocus()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setFocus();
	}
}

String Window::getTitle()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getTitle();
	}
	return String::null();
}

void Window::setTitle(const String& title)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setTitle(title);
	}
}

Color Window::getBackgroundColor()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getBackgroundColor();
	}
	return Color::zero();
}

void Window::setBackgroundColor(const Color& color)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setBackgroundColor(color);
	}
}

Rectangle Window::getFrame()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getFrame();
	}
	return Rectangle::zero();
}

void Window::setFrame(const Rectangle& frame)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setFrame(frame);
	}
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

sl_bool Window::isMinimized()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isMinimized();
	}
	return sl_false;
}

void Window::setMinimized(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMinimized(flag);
	}
}

sl_bool Window::isMaximized()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isMaximized();
	}
	return sl_false;
}

void Window::setMaximized(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMaximized(flag);
	}
}

sl_bool Window::isVisible()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isVisible();
	}
	return sl_false;
}

void Window::setVisible(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setVisible(flag);
	}
}

sl_bool Window::isAlwaysOnTop()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isAlwaysOnTop();
	}
	return sl_false;
}

void Window::setAlwaysOnTop(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setAlwaysOnTop(flag);
	}
}

sl_bool Window::isCloseButtonEnabled()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isCloseButtonEnabled();
	}
	return sl_false;
}

void Window::setCloseButtonEnabled(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setCloseButtonEnabled(flag);
	}
}

sl_bool Window::isMinimizeButtonEnabled()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isMinimizeButtonEnabled();
	}
	return sl_false;
}

void Window::setMinimizeButtonEnabled(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMinimizeButtonEnabled(flag);
	}
}

sl_bool Window::isMaximizeButtonEnabled()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isMaximizeButtonEnabled();
	}
	return sl_false;
}

void Window::setMaximizeButtonEnabled(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setMaximizeButtonEnabled(flag);
	}
}

sl_bool Window::isResizable()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isResizable();
	}
	return sl_false;
}

void Window::setResizable(sl_bool flag)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setResizable(flag);
	}
}

sl_real Window::getAlpha()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->getAlpha();
	}
	return 1;
}

void Window::setAlpha(sl_real alpha)
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		instance->setAlpha(alpha);
	}
}

sl_bool Window::isTransparent()
{
	Ref<WindowInstance> instance = m_instance;
	if (CHECK_INSTANCE(instance)) {
		return instance->isTransparent();
	}
	return sl_false;
}

void Window::setTransparent(sl_bool flag)
{
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

Point Window::convertCoordinateFromWindowToScreen(const Point& ptWindow)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromWindowToScreen(ptWindow);
	} else {
		return Point::zero();
	}
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

Point Window::convertCoordinateFromClientToScreen(const Point& ptClient)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromClientToScreen(ptClient);
	} else {
		return Point::zero();
	}
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

Point Window::convertCoordinateFromClientToWindow(const Point& ptClient)
{
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromClientToWindow(ptClient);
	} else {
		return Point::zero();
	}
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

void Window::attach(const Ref<WindowInstance>& _instance)
{
	MutexLocker lock(getLocker());

	detach();
	
	Ref<WindowInstance> instance = _instance;
	
	if (instance.isNotNull()) {
		
		instance->setWindow(this);
		
		Ref<ViewInstance> contentViewInstance = instance->getContentView();
		if (contentViewInstance.isNotNull()) {
			Rectangle rect;
			rect.setLeftTop(Point::zero());
			rect.setSize(instance->getClientSize());
			Ref<ViewGroup> view = m_viewContent;
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
	MutexLocker lock(getLocker());
	
	m_viewContent->detach();
	Ref<WindowInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setWindow(Ref<Window>::null());
	}
	m_instance.setNull();
}

sl_bool Window::createWindow(const WindowParam& param)
{
	if (! (UI::isUIThread())) {
		return sl_false;
	}
	
	Ref<WindowInstance> window = createWindowInstance(param);
	
	if (window.isNotNull()) {

		window->setParent(param.parent);
		
		window->setBackgroundColor(param.backgroundColor);
		
		window->setVisible(param.flagVisible);
		window->setMinimized(param.flagMinimized);
		window->setMaximized(param.flagMaximized);
		
		window->setAlwaysOnTop(param.flagAlwaysOnTop);
		window->setCloseButtonEnabled(param.flagCloseButtonEnabled);
		window->setMinimizeButtonEnabled(param.flagMinimizeButtonEnabled);
		window->setMaximizeButtonEnabled(param.flagMaximizeButtonEnabled);
		window->setResizable(param.flagResizable);
		window->setAlpha(param.alpha);
		window->setTransparent(param.flagTransparent);

		window->setVisible(sl_true);
		
		attach(window);
		
		onCreate();
		
		if (param.flagModal) {
			_runModal();
		}
		
		return sl_true;
	}
	return sl_false;
}

void Window::create(const WindowParam& param, const Ref<Runnable>& _callbackSuccess, const Ref<Runnable>& _callbackFail)
{
	Ref<Runnable> callbackSuccess = _callbackSuccess;
	Ref<Runnable> callbackFail = _callbackFail;
	if (UI::isUIThread()) {
		if (createWindow(param)) {
			if (callbackSuccess.isNotNull()) {
				callbackSuccess->run();
			}
		} else {
			if (callbackFail.isNotNull()) {
				callbackFail->run();
			}
		}
	} else {
		UI::runOnUIThread(SLIB_CALLBACK_REF(Window, _createCallback, this, param, callbackSuccess, callbackFail));
	}
}

void Window::_createCallback(WindowParam param, Ref<Runnable> callbackSuccess, Ref<Runnable> callbackFail)
{
	if (createWindow(param)) {
		if (callbackSuccess.isNotNull()) {
			callbackSuccess->run();
		}
	} else {
		if (callbackFail.isNotNull()) {
			callbackFail->run();
		}
	}
}

void Window::runModal()
{
	UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(Window, _runModal, this));
}

void Window::_runModal()
{
	Ref<WindowInstance> instance = getWindowInstance();
	if (instance.isNotNull()) {
		instance->runModal();
	}
}

void Window::addView(const Ref<View>& _child)
{
	Ref<View> child = _child;
	if (child.isNotNull()) {
		Ref<ViewGroup> view = m_viewContent;
		if (view.isNotNull()) {
			view->addChild(child);
		}
	}
}

void Window::removeView(const Ref<View>& _child)
{
	Ref<View> child = _child;
	if (child.isNotNull()) {
		Ref<ViewGroup> view = m_viewContent;
		if (view.isNotNull()) {
			view->removeChild(child);
		}
	}
}

List< Ref<View> > Window::getViews()
{
	Ref<ViewGroup> view = m_viewContent;
	if (view.isNotNull()) {
		return view->getChildren();
	}
	return List< Ref<View> >::null();
}

void Window::removeAllViews()
{
	Ref<ViewGroup> view = m_viewContent;
	if (view.isNotNull()) {
		view->removeAllChildren();
	}
}

void Window::onCreate()
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
	Ref<ViewGroup> view = m_viewContent;
	if (view.isNotNull()) {
		Rectangle rect;
		rect.setLeftTop(Point::zero());
		rect.setSize(getClientSize());
		view->setFrame(rect);
	}
}

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

WindowParam::WindowParam()
{
	init();
	flagShowTitleBar = sl_false;
	flagFullScreen = sl_true;
}

WindowParam::WindowParam(Rectangle _rect)
{
	init();
	flagShowTitleBar = sl_false;
	flagFullScreen = sl_false;
	flagCenterScreen = sl_false;
	location.x = _rect.left;
	location.y = _rect.top;
	size.x = _rect.getWidth();
	size.y = _rect.getHeight();
}

WindowParam::WindowParam(sl_real width, sl_real height)
{
	init();
	flagShowTitleBar = sl_false;
	flagFullScreen = sl_false;
	flagCenterScreen = sl_true;
	size.x = width;
	size.y = height;
}

WindowParam::WindowParam(sl_real x, sl_real y, sl_real width, sl_real height)
{
	init();
	flagShowTitleBar = sl_false;
	flagFullScreen = sl_false;
	flagCenterScreen = sl_false;
	location.x = x;
	location.y = y;
	size.x = width;
	size.y = height;
}

WindowParam::WindowParam(String _title, Rectangle _rect)
{
	init();
	flagShowTitleBar = sl_true;
	title = _title;
	flagFullScreen = sl_false;
	flagCenterScreen = sl_false;
	location.x = _rect.left;
	location.y = _rect.top;
	size.x = _rect.getWidth();
	size.y = _rect.getHeight();
}

WindowParam::WindowParam(String _title, sl_real width, sl_real height)
{
	init();
	flagShowTitleBar = sl_true;
	title = _title;
	flagFullScreen = sl_false;
	flagCenterScreen = sl_true;
	size.x = width;
	size.y = height;
}

WindowParam::WindowParam(String _title, sl_real x, sl_real y, sl_real width, sl_real height)
{
	init();
	flagShowTitleBar = sl_true;
	title = _title;
	flagFullScreen = sl_false;
	flagCenterScreen = sl_false;
	location.x = x;
	location.y = y;
	size.x = width;
	size.y = height;
}

void WindowParam::init()
{
	backgroundColor = Color::zero();
	
	flagVisible = sl_true;
	flagMinimized = sl_false;
	flagMaximized = sl_false;
	
	flagAlwaysOnTop = sl_false;
	flagCloseButtonEnabled = sl_true;
	flagMinimizeButtonEnabled = sl_false;
	flagMaximizeButtonEnabled = sl_false;
	flagResizable = sl_false;
	alpha = 1.0f;
	flagTransparent = sl_false;
	
	flagModal = sl_false;
}

void WindowParam::setParent(const Ref<Window>& _parent)
{
	Ref<Window> parent = _parent;
	if (parent.isNotNull()) {
		this->parent = parent->getWindowInstance();
	}
}
SLIB_UI_NAMESPACE_END
