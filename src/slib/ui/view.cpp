#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN
/**********************
	View
**********************/
View::View()
{
	m_flagGroup = sl_false;
	
	setFrame(Rectangle(0, 0, 1, 1));
	
	m_flagVisible = sl_true;
	m_flagEnabled = sl_true;
	m_flagOpaque = sl_false;
}

View::~View()
{
	detach();
}

sl_bool View::isGroup()
{
	return m_flagGroup;
}

void View::setGroup(sl_bool flag)
{
	m_flagGroup = flag;
}

const Ref<ViewInstance>& View::getViewInstance()
{
	return m_instance;
}

Ref<Window> View::getWindow()
{
	Ref<Window> window = m_window;
	if (window.isNotNull()) {
		return window;
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		return parent->getWindow();
	}
	return Ref<Window>::null();
}

void View::setWindow(const Ref<Window>& window)
{
	m_window = window;
}

Ref<View> View::getParent()
{
	return m_parent;
}

void View::setParent(const Ref<View>& parent)
{
	m_parent = parent;
}

void View::removeParent(View* _parent)
{
	Ref<View> parent = getParent();
	if (parent == _parent) {
		m_parent.setNull();
	}
}

sl_bool View::isValid()
{
	return _isValid();
}

void View::setFocus()
{
	_setFocus();
}

void View::invalidate()
{
	_invalidate();
}

void View::invalidate(const Rectangle &rect)
{
	_invalidate(rect);
}

const Rectangle& View::getFrame()
{
	return m_frame;
}

void View::setFrame(const Rectangle &frame)
{
	Size sizeOld = m_frame.getSize();
	Size sizeNew = frame.getSize();
	m_frame = frame;
	_setFrame(frame);
	if (!(Math::isNearZero(sizeOld.x - sizeNew.x) && Math::isNearZero(sizeOld.y - sizeNew.y))) {
		onResize();
	}
}

void View::setFrame(sl_real x, sl_real y, sl_real width, sl_real height)
{
	setFrame(Rectangle(x, y, x+width, y+height));
}

sl_bool View::isVisible()
{
	return m_flagVisible;
}

void View::setVisible(sl_bool flag)
{
	sl_bool flagOld = m_flagVisible;
	m_flagVisible = flag;
	_setVisible(flag);
	if (flagOld != flag) {
		if (flag) {
			onResize();
		}
	}
}

sl_bool View::isEnabled()
{
	return m_flagEnabled;
}

void View::setEnabled(sl_bool flag)
{
	m_flagEnabled = flag;
	_setEnabled(flag);
}

sl_bool View::isOpaque()
{
	return m_flagOpaque;
}

void View::setOpaque(sl_bool flag)
{
	m_flagOpaque = flag;
	_setOpaque(flag);
}

sl_real View::getWidth()
{
	return m_frame.getWidth();
}

sl_real View::getHeight()
{
	return m_frame.getHeight();
}

Size View::getSize()
{
	return m_frame.getSize();
}

void View::setSize(const Size& size)
{
	setFrame(m_frame.left, m_frame.top, size.x, size.y);
}

void View::setSize(sl_real width, sl_real height)
{
	setFrame(m_frame.left, m_frame.top, width, height);
}

sl_real View::getX()
{
	return m_frame.left;
}

sl_real View::getY()
{
	return m_frame.top;
}

Point View::getPosition()
{
	return m_frame.getLeftTop();
}

void View::setPosition(const Point& point)
{
	setFrame(point.x, point.y, m_frame.getWidth(), m_frame.getHeight());
}

void View::setPosition(sl_real x, sl_real y)
{
	setFrame(x, y, m_frame.getWidth(), m_frame.getHeight());
}

Rectangle View::getFrameBounds()
{
	return Rectangle(Point::zero(), m_frame.getSize());
}

Rectangle View::getContentBounds()
{
	return Rectangle(Point::zero(), m_frame.getSize());
}

Point View::convertCoordinateFromScreenToView(const Point& ptScreen)
{
	return _convertCoordinateFromScreenToView(ptScreen);
}

Point View::convertCoordinateFromViewToScreen(const Point& ptView)
{
	return _convertCoordinateFromViewToScreen(ptView);
}

void View::attach(const Ref<ViewInstance>& instance)
{
	ObjectLocker lock(this);
	detach();
	if (instance.isNotNull()) {
		m_instance = instance;
		instance->setView(this);
		if (UI::isUIThread()) {
			onAttach();
		} else {
			UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(View, onAttach, this));
		}
	}
}

void View::detach()
{
	ObjectLocker lock(this);
	Ref<ViewInstance> instance = getViewInstance();
	if (instance.isNotNull()) {
		instance->setView(sl_null);
		m_instance.setNull();
	}
}

void View::attachChild(const Ref<View>& child)
{
	if (m_flagGroup) {
		if (child.isNotNull()) {
			if (!(UI::isUIThread())) {
				UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(View, _attachChild, this, child));
				return;
			}
			Ref<ViewInstance> parentInstance = getViewInstance();
			if (parentInstance.isNotNull()) {
				child->attachToNewInstance(parentInstance);
			}
		}
	}
}

void View::_attachChild(Ref<slib::View> view)
{
	attachChild(view);
}

Ref<ViewInstance> View::attachToNewInstance(const Ref<ViewInstance>& parent)
{
	Ref<ViewInstance> instance = createInstance(parent.get());
	if (instance.isNotNull()) {
		attach(instance);
	}
	return instance;
}

void View::addChildInstance(const Ref<ViewInstance>& instance)
{
	if (instance.isNotNull()) {
		if (UI::isUIThread()) {
			_addChildInstance(instance.get());
		} else {
			UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(View, _addChildInstance_, this, instance));
		}
	}
}

void View::_addChildInstance_(Ref<ViewInstance> instance)
{
	_addChildInstance(instance.get());
}

void View::removeChildInstance(const Ref<ViewInstance>& instance)
{
	if (instance.isNotNull()) {
		if (UI::isUIThread()) {
			_removeChildInstance(instance.get());
		} else {
			UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(View, _removeChildInstance_, this, instance));
		}
	}
}

void View::_removeChildInstance_(Ref<ViewInstance> instance)
{
	_removeChildInstance(instance.get());
}

void View::onDraw(Canvas* canvas)
{
}

void View::onClick()
{
}

void View::onKeyEvent(UIEvent* ev)
{
}

void View::onMouseEvent(UIEvent* ev)
{
}

void View::onTouchEvent(UIEvent* ev)
{
}

void View::onMouseWheelEvent(UIEvent* ev)
{
}

void View::onSetCursor(UIEvent* ev)
{
}

void View::onResize()
{
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parent->onResizeChild(this);
	}
}

void View::onResizeChild(View* child)
{
}

void View::onAttach()
{
}

void View::dispatchDraw(Canvas* canvas)
{
	onDraw(canvas);
}

void View::dispatchClick()
{
	onClick();
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onClick(this);
	}
}

void View::dispatchKeyEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	onKeyEvent(ev);
	if (ev->isStoppedPropagation()) {
		return;
	}
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onKeyEvent(this, ev);
	}
}

void View::dispatchMouseEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	onMouseEvent(ev);
	if (ev->isStoppedPropagation()) {
		return;
	}
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMouseEvent(this, ev);
	}
}

void View::dispatchTouchEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	onTouchEvent(ev);
	if (ev->isStoppedPropagation()) {
		return;
	}
	onMouseEvent(ev);
	if (ev->isStoppedPropagation()) {
		return;
	}
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onTouchEvent(this, ev);
		if (ev->isStoppedPropagation()) {
			return;
		}
		listener->onMouseEvent(this, ev);
	}
}

void View::dispatchMouseWheelEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	onMouseWheelEvent(ev);
	if (ev->isStoppedPropagation()) {
		return;
	}
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMouseWheelEvent(this, ev);
	}
}

void View::dispatchSetCursor(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	onSetCursor(ev);
	if (ev->isStoppedPropagation()) {
		return;
	}
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onSetCursor(this, ev);
	}
}

/**********************
	ViewInstance
 **********************/
ViewInstance::ViewInstance()
{
}

ViewInstance::~ViewInstance()
{
}

void ViewInstance::setView(View* view)
{
	m_view = view;
}

Ref<View> ViewInstance::getView()
{
	return m_view;
}

void ViewInstance::onDraw(Canvas* canvas)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchDraw(canvas);
	}
}

void ViewInstance::onClick()
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchClick();
	}
}

void ViewInstance::onKeyEvent(UIEvent* event)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchKeyEvent(event);
	}
}

void ViewInstance::onMouseEvent(UIEvent* event)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchMouseEvent(event);
	}
}

void ViewInstance::onTouchEvent(UIEvent* event)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchTouchEvent(event);
	}
}

void ViewInstance::onMouseWheelEvent(UIEvent* event)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchMouseWheelEvent(event);
	}
}

void ViewInstance::onSetCursor(UIEvent* event)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchSetCursor(event);
	}
}
SLIB_UI_NAMESPACE_END
