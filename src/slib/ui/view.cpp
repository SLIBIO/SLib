#include "../../../inc/slib/ui/view.h"

#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/scroll_bar.h"
#include "../../../inc/slib/graphics/bitmap.h"

#include "../../../inc/slib/core/scoped_pointer.h"
#include "../../../inc/slib/math/transform2d.h"

SLIB_UI_NAMESPACE_BEGIN

/**********************
	View
**********************/

SLIB_DEFINE_OBJECT(View, Object)

View::View()
{
	m_flagCreatingInstance = sl_false;
	m_flagCreatingChildInstances = sl_false;
	m_flagCreatingNativeWidget = sl_false;
	m_attachMode = UIAttachMode::NotAttachInNativeWidget;
	
	m_frame.left = 0;
	m_frame.top = 0;
	m_frame.right = 0;
	m_frame.bottom = 0;
	
	m_visibility = Visibility::Visible;
	m_flagEnabled = sl_true;
	m_flagOpaque = sl_false;
	
	m_flagHitTestable = sl_true;
	
	m_flagFocusable = sl_false;
	m_flagFocused = sl_false;
	
	m_flagDown = sl_false;
	m_flagHover = sl_false;
	m_flagOccurringClick = sl_true;
	
	m_actionMouseDown = UIAction::Unknown;
	m_flagMultiTouchMode = sl_false;
	m_flagPassEventToChildren = sl_true;
	m_flagProcessingTabStop = sl_true;
	
	m_flagOnAddChild = sl_false;
	m_flagOnRemoveChild = sl_false;

}

View::~View()
{
	detach();
}

Ref<ViewInstance> View::getViewInstance()
{
	return m_instance;
}

sl_bool View::isInstance()
{
	Ref<ViewInstance> instance = getViewInstance();
	if (instance.isNotNull()) {
		return sl_true;
	}
	return sl_false;
}

sl_bool View::isValidInstance()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->isValid();
	}
	return sl_false;
}

sl_bool View::isCreatingInstance()
{
	return m_flagCreatingInstance;
}

void View::setCreatingInstance(sl_bool flag)
{
	m_flagCreatingInstance = flag;
}

sl_bool View::isCreatingChildInstances()
{
	return m_flagCreatingChildInstances;
}

void View::setCreatingChildInstances(sl_bool flag)
{
	m_flagCreatingChildInstances = flag;
	if (flag) {
		m_flagCreatingInstance = sl_true;
	}
}

sl_bool View::isCreatingNativeWidget()
{
	return m_flagCreatingNativeWidget;
}

void View::setCreatingNativeWidget(sl_bool flag)
{
	m_flagCreatingNativeWidget = flag;
	if (flag) {
		m_flagCreatingInstance = sl_true;
	}
}

UIAttachMode View::getAttachMode()
{
	return m_attachMode;
}

void View::setAttachMode(UIAttachMode mode)
{
	m_attachMode = mode;
}

Ref<ViewInstance> View::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

Ref<ViewInstance> View::createInstance(ViewInstance* parent)
{
	if (m_flagCreatingInstance) {
		if (m_flagCreatingNativeWidget) {
			Ref<ViewInstance> ret = createNativeWidget(parent);
			if (ret.isNotNull()) {
				ret->setNativeWidget(sl_true);
				return ret;
			}
		}
		return createGenericInstance(parent);
	}
	return Ref<ViewInstance>::null();
}

sl_bool View::isNativeWidget()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->isNativeWidget();
	}
	return sl_false;
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

Ref<GraphicsContext> View::getGraphicsContext()
{
	Ref<GraphicsContext> context = m_graphicsContext;
	if (context.isNotNull()) {
		return context;
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		return parent->getGraphicsContext();
	}
	return UI::getGraphicsContext();
}

void View::setGraphicsContext(const Ref<GraphicsContext>& context)
{
	m_graphicsContext = context;
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
	if (_parent) {
		Ref<View> parent = getParent();
		if (parent == _parent) {
			m_parent.setNull();
		}
	} else {
		m_parent.setNull();
	}
}

void View::attach(const Ref<ViewInstance>& instance)
{
	ObjectLocker lock(this);
	detach();
	if (instance.isNotNull()) {
		m_instance = instance;
		instance->setView(this);
		if (UI::isUiThread()) {
			_processAttachOnUiThread();
		} else {
			UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _processAttachOnUiThread, this));
		}
	}
}

Ref<ViewInstance> View::attachToNewInstance(const Ref<ViewInstance>& parent)
{
	Ref<ViewInstance> instance = createInstance(parent.ptr);
	if (instance.isNotNull()) {
		attach(instance);
	}
	return instance;
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

void View::_processAttachOnUiThread()
{
	if (isInstance()) {
		if (isCreatingChildInstances()) {
			ListLocker< Ref<View> > children(m_children);
			for (sl_size i = 0; i < children.count; i++) {
				Ref<View>& child = children[i];
				if (child.isNotNull()) {
					UIAttachMode attachMode = child->getAttachMode();
					if (attachMode == UIAttachMode::AttachAlways) {
						attachChild(child);
					} else if (attachMode == UIAttachMode::NotAttachInNativeWidget) {
						if (!(isNativeWidget())) {
							attachChild(child);
						}
					}
				}
			}
		}
		onAttach();
	}
}

String View::getId()
{
	return m_id;
}

void View::setId(const String& _id)
{
	m_id = _id;
}

List< Ref<View> > View::getChildren()
{
	return m_children.duplicate();
}

CList< Ref<View> >& View::_getChildren()
{
	return m_children;
}

sl_size View::getChildrenCount()
{
	return m_children.getCount();
}

void View::addChild(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	if (m_children.addIfNotExist(view)) {
		_addChild(view, flagRedraw);
	}
}

void View::insertChild(sl_size index, const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	if (m_children.insert(index, view)) {
		_addChild(view, flagRedraw);
	}
}

void View::removeChild(sl_size index, sl_bool flagRedraw)
{
	Ref<View> view = m_children.getItemValue(index, Ref<View>::null());
	if (view.isNull()) {
		return;
	}
	_removeChild(view);
	m_children.remove(index);
	if (view == m_childMouseDown) {
		m_childMouseDown.setNull();
	}
	if (view == m_childMouseMove) {
		m_childMouseMove.setNull();
	}
	if (view == m_childFocused) {
		_setFocusedChild(sl_null, sl_false);
	}
	m_childrenMultiTouch.removeValue(view);
	requestLayout(flagRedraw);
}

void View::removeChild(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_removeChild(view);
	m_children.removeValue(view);
	if (view == m_childMouseDown) {
		m_childMouseDown.setNull();
	}
	if (view == m_childMouseMove) {
		m_childMouseMove.setNull();
	}
	if (view == m_childFocused) {
		_setFocusedChild(sl_null, sl_false);
	}
	m_childrenMultiTouch.removeValue(view);
	requestLayout(flagRedraw);
}

void View::removeAllChildren(sl_bool flagRedraw)
{
	if (isInstance()) {
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, removeAllChildren, this, flagRedraw));
			return;
		}
		ListLocker< Ref<View> > children(m_children);
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				_removeChild(child.ptr);
			}
		}
	} else {
		ListLocker< Ref<View> > children(m_children);
		for (sl_size i = 0; i < children.count; i++) {
			children[i]->removeParent(this);
		}
	}
	m_childMouseDown.setNull();
	m_childMouseMove.setNull();
	_setFocusedChild(sl_null, sl_false);
	m_childrenMultiTouch.setNull();
	m_children.removeAll();
	requestLayout(flagRedraw);
}

Ref<View> View::getChildAt(sl_ui_pos x, sl_ui_pos y)
{
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = children.count - 1, ii = 0; ii < children.count; i--, ii++) {
		Ref<View>& child = children[i];
		if (child.isNotNull() && child->isVisible() && child->isHitTestable()) {
			UIPoint pt = child->convertCoordinateFromParent(UIPointf((sl_ui_posf)x, (sl_ui_posf)y));
			if (child->hitTest(pt)) {
				return child;
			}
		}
	}
	return Ref<View>::null();
}

Ref<View> View::getChildAt(const UIPoint& point)
{
	return getChildAt(point.x, point.y);
}

Ref<View> View::getChildById(const String& _id)
{
	if (m_id == _id) {
		return this;
	}
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		const Ref<View>& child = children[i];
		if (child.isNotNull()) {
			Ref<View> _child = child->getChildById(_id);
			if (_child.isNotNull()) {
				return _child;
			}
		}
	}
	return Ref<View>::null();
}

Ref<View> View::getRootView()
{
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		return parent->getRootView();
	}
	return this;
}

sl_bool View::isRootView()
{
	return m_parent.isNull();
}

void View::removeFromParent()
{
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		parent->removeChild(this);
	}
}

void View::attachChild(const Ref<View>& child)
{
	if (m_flagCreatingChildInstances) {
		if (child.isNotNull()) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _attachChildOnUiThread, this, child));
				return;
			}
			Ref<ViewInstance> parentInstance = getViewInstance();
			if (parentInstance.isNotNull()) {
				child->attachToNewInstance(parentInstance);
			}
		}
	}
}

void View::addChildInstance(const Ref<ViewInstance>& child)
{
	if (child.isNotNull()) {
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (UI::isUiThread()) {
				instance->addChildInstance(child);
			} else {
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _addChildInstanceOnUiThread, this, child));
			}
		}
	}
}

void View::removeChildInstance(const Ref<ViewInstance>& child)
{
	if (child.isNotNull()) {
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (UI::isUiThread()) {
				instance->removeChildInstance(child);
			} else {
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _removeChildInstanceOnUiThread, this, child));
			}
		}
	}
}

sl_bool View::isOnAddChildEnabled()
{
	return m_flagOnAddChild;
}

void View::setOnAddChildEnabled(sl_bool flagEnabled)
{
	m_flagOnAddChild = flagEnabled;
}

sl_bool View::isOnRemoveChildEnabled()
{
	return m_flagOnRemoveChild;
}

void View::setOnRemoveChildEnabled(sl_bool flagEnabled)
{
	m_flagOnRemoveChild = flagEnabled;
}

void View::_addChild(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNotNull()) {
		view->setFocus(sl_false, sl_false);
		view->setParent(this);
		if (m_flagOnAddChild) {
			onAddChild(view.ptr);
		}
		requestLayout(sl_false);
		
		UIAttachMode attachMode = view->getAttachMode();
		if (attachMode == UIAttachMode::AttachAlways) {
			if (isInstance() && isCreatingChildInstances()) {
				attachChild(view);
			}
		} else if (attachMode == UIAttachMode::NotAttachInNativeWidget) {
			if (isInstance() && isCreatingChildInstances() && !(isNativeWidget())) {
				attachChild(view);
			}
		}
		if (flagRedraw) {
			invalidate();
		}
	}
}

void View::_removeChild(const Ref<View>& view)
{
	if (view.isNotNull()) {
		if (m_flagOnRemoveChild) {
			onRemoveChild(view.ptr);
		}
		Ref<ViewInstance> instanceChild = view->getViewInstance();
		if (instanceChild.isNotNull()) {
			removeChildInstance(instanceChild.ptr);
			view->detach();
		}
		view->removeParent(this);
	}
}

void View::_attachChildOnUiThread(Ref<slib::View> view)
{
	attachChild(view);
}

void View::_addChildInstanceOnUiThread(Ref<ViewInstance> child)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->addChildInstance(child);
	}
}

void View::_removeChildInstanceOnUiThread(Ref<ViewInstance> child)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->removeChildInstance(child);
	}
}

void View::invalidate()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->invalidate();
	} else {
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->invalidate(getBoundsInParent());
		}
	}
}

void View::invalidate(const UIRect &rect)
{
	UIRect rectIntersect;
	if (getBounds().intersectRectangle(rect, &rectIntersect)) {
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->invalidate(rectIntersect);
		} else {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				parent->invalidate(getBoundsInParent(rectIntersect));
			}
		}
	}
}

const UIRect& View::getFrame()
{
	return m_frame;
}

UIRect View::getInstanceFrame()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->getFrame();
	}
	return UIRect::zero();
}

void View::_setFrame(const UIRect& _frame, sl_bool flagRedraw, sl_bool flagLayouting)
{
	UIRect frameOld = m_frame;
	UIRect frame = _frame;
	frame.fixSizeError();
	
	sl_bool flagNotMoveX = Math::isAlmostZero(frameOld.left - frame.left);
	sl_bool flagNotMoveY = Math::isAlmostZero(frameOld.top - frame.top);
	sl_bool flagNotResizeWidth = Math::isAlmostZero(frameOld.getWidth() - frame.getWidth());
	sl_bool flagNotResizeHeight = Math::isAlmostZero(frameOld.getHeight() - frame.getHeight());
	
	m_frame = frame;

	if (flagNotMoveX && flagNotMoveY && flagNotResizeWidth && flagNotResizeHeight) {
		return;
	}

	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setFrame(frame);
	}
	
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		layout->flagInvalidMeasure = sl_true;
		if (!(flagNotResizeWidth && flagNotResizeHeight)) {
			layout->flagInvalidLayout = sl_true;
		}
		if (!flagLayouting) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				parent->requestLayout(sl_false);
			} else {
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _makeLayout, this, sl_false));
			}
		}
	} else {
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->requestLayout(sl_false);
		}
	}
	
	if (!(flagNotResizeWidth && flagNotResizeHeight)) {
		dispatchResize(frame.getWidth(), frame.getHeight());
	}
	
	if (instance.isNull()) {
		if (flagRedraw) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				UIRect bounds = getBoundsInParent(UIRect(0, 0, frameOld.getWidth(), frameOld.getHeight()));
				bounds.mergeRectangle(getBoundsInParent());
				parent->invalidate(bounds);
			}
		}
	}

}

void View::setFrame(const UIRect &frame, sl_bool flagRedraw)
{
	_setFrame(frame, flagRedraw, sl_false);
}

void View::setFrame(sl_ui_pos x, sl_ui_pos y, sl_ui_len width, sl_ui_len height, sl_bool flagRedraw)
{
	_setFrame(UIRect(x, y, x+width, y+height), flagRedraw, sl_false);
}

sl_ui_len View::getWidth()
{
	return m_frame.getWidth();
}

void View::setWidth(sl_ui_len width, sl_bool flagRedraw)
{
	setFrame(m_frame.left, m_frame.top, width, m_frame.getHeight(), flagRedraw);
}

sl_ui_len View::getHeight()
{
	return m_frame.getHeight();
}

void View::setHeight(sl_ui_len height, sl_bool flagRedraw)
{
	setFrame(m_frame.left, m_frame.top, m_frame.getWidth(), height, flagRedraw);
}

UISize View::getSize()
{
	return m_frame.getSize();
}

void View::setSize(const UISize& size, sl_bool flagRedraw)
{
	setFrame(m_frame.left, m_frame.top, size.x, size.y, flagRedraw);
}

void View::setSize(sl_ui_len width, sl_ui_len height, sl_bool flagRedraw)
{
	setFrame(m_frame.left, m_frame.top, width, height, flagRedraw);
}

sl_ui_pos View::getLeft()
{
	return m_frame.left;
}

sl_ui_pos View::getTop()
{
	return m_frame.top;
}

void View::setLeft(sl_ui_pos x, sl_bool flagRedraw)
{
	setFrame(x, m_frame.top, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

void View::setTop(sl_ui_pos y, sl_bool flagRedraw)
{
	setFrame(m_frame.left, y, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

UIPoint View::getPosition()
{
	return m_frame.getLeftTop();
}

void View::setPosition(sl_ui_pos x, sl_ui_pos y, sl_bool flagRedraw)
{
	setFrame(x, y, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

void View::setPosition(const UIPoint& point, sl_bool flagRedraw)
{
	setFrame(point.x, point.y, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

UIRect View::getBounds()
{
	return UIRect(0, 0, m_frame.getWidth(), m_frame.getHeight());
}

UIRect View::getBoundsInnerPadding()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNull()) {
		return getBounds();
	}
	UIRect ret(layout->paddingLeft, layout->paddingTop, m_frame.getWidth() - layout->paddingRight, m_frame.getHeight() - layout->paddingBottom);
	ret.fixSizeError();
	return ret;
}

UIRect View::getBoundsInParent()
{
	return getBoundsInParent(getBounds());
}

UIRect View::getBoundsInParent(const UIRect& boundsLocal)
{
	UIPoint pts[4];
	boundsLocal.getCornerPoints(pts);
	for (int i = 0; i < 4; i++) {
		pts[i] = convertCoordinateToParent(pts[i]);
	}
	UIRect rc;
	rc.setFromPoints(pts, 4);
	return rc;
}

Visibility View::getVisibility()
{
	return m_visibility;
}

void View::setVisibility(Visibility visibility, sl_bool flagRedraw)
{
	Visibility oldVisibility = m_visibility;
	m_visibility = visibility;
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setVisible(visibility == Visibility::Visible);
	}
	if (oldVisibility != visibility) {
		if (isLayoutEnabled()) {
			requestParentLayout(sl_false);
		}
		dispatchChangeVisibility(oldVisibility, visibility);
	}
	if (instance.isNull()) {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isVisible()
{
	return m_visibility == Visibility::Visible;
}

void View::setVisible(sl_bool flag, sl_bool flagRedraw)
{
	if (flag) {
		setVisibility(Visibility::Visible, flagRedraw);
	} else {
		setVisibility(Visibility::Gone, flagRedraw);
	}
}

sl_bool View::isEnabled()
{
	return m_flagEnabled;
}

void View::setEnabled(sl_bool flag, sl_bool flagRedraw)
{
	m_flagEnabled = flag;
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setEnabled(flag);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isOpaque()
{
	return m_flagOpaque;
}

void View::setOpaque(sl_bool flag, sl_bool flagRedraw)
{
	m_flagOpaque = flag;
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setOpaque(flag);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isHitTestable()
{
	return m_flagHitTestable;
}

void View::setHitTestable(sl_bool flag)
{
	m_flagHitTestable = flag;
}

sl_bool View::hitTest(sl_ui_pos x, sl_ui_pos y)
{
	UIRect rc(0, 0, getWidth(), getHeight());
	switch (getBoundShape()) {
		case BoundShape::RoundRect:
			return GraphicsPath::containsPointInRoundRect(Point((sl_real)x, (sl_real)y), rc, getRoundRectBoundShapeRadius());
		case BoundShape::Ellipse:
			return GraphicsPath::containsPointInEllipse(Point((sl_real)x, (sl_real)y), rc);
		default:
			break;
	}
	return rc.containsPoint(x, y);
}

sl_bool View::hitTest(const UIPoint& point)
{
	return hitTest(point.x, point.y);
}

sl_bool View::isFocusable()
{
	return m_flagFocusable;
}

void View::setFocusable(sl_bool flagFocusable)
{
	m_flagFocusable = flagFocusable;
}

sl_bool View::isFocused()
{
	return m_flagFocused;
}

void View::setFocus(sl_bool flagFocused, sl_bool flagRedraw)
{
	m_flagFocused = flagFocused;
	if (flagFocused) {
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->setFocus();
		}
	} else {
		_killFocusFromParent();
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		if (flagFocused) {
			parent->_setFocusedChild(this, flagRedraw);
			return;
		} else {
			if (m_childFocused == this) {
				m_childFocused.setNull();
			}
		}
	}
	if (flagRedraw) {
		invalidate();
	}
}

void View::_killFocusFromParent()
{
	m_flagFocused = sl_false;
	Ref<View> child = m_childFocused;
	if (child.isNotNull()) {
		child->_killFocusFromParent();
	}
	m_childFocused.setNull();
}

Ref<View> View::getFocusedChild()
{
	return m_childFocused;
}

void View::_setFocusedChild(View* child, sl_bool flagRedraw)
{
	Ref<View> old = m_childFocused;
	if (old != child) {
		if (old.isNotNull()) {
			old->_killFocusFromParent();
		}
		m_childFocused = child;
	}
	if (child) {
		m_flagFocused = sl_true;
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->_setFocusedChild(this, flagRedraw);
			return;
		}
	}
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool View::isDownState()
{
	return m_flagDown;
}

void View::setDownState(sl_bool flagState, sl_bool flagRedraw)
{
	if (m_flagDown != flagState) {
		m_flagDown = flagState;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isHoverState()
{
	return m_flagHover;
}

void View::setHoverState(sl_bool flagState, sl_bool flagRedraw)
{
	if (m_flagHover != flagState) {
		m_flagHover = flagState;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isOccurringClick()
{
	return m_flagOccurringClick;
}

void View::setOccurringClick(sl_bool flag)
{
	m_flagOccurringClick = flag;
}

Ref<Cursor> View::getCursor()
{
	return m_cursor;
}

void View::setCursor(const Ref<Cursor> &cursor)
{
	m_cursor = cursor;
}

void View::measureLayout()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNull()) {
		return;
	}
	if (!(layout->flagEnabled)) {
		return;
	}
	if (!(layout->flagInvalidMeasure)) {
		return;
	}
	
	SizeMode widthMode = layout->widthMode;
	SizeMode heightMode = layout->heightMode;
	
	sl_bool flagHorizontal = sl_false;
	sl_bool flagVertical = sl_false;
	
	sl_ui_len measuredWidth = 0;
	sl_ui_len measuredHeight = 0;
	
	if (widthMode == SizeMode::Wrapping) {
		flagHorizontal = sl_true;
	} else if (widthMode == SizeMode::Fixed) {
		measuredWidth = getWidth();
	}
	if (heightMode == SizeMode::Wrapping) {
		flagVertical = sl_true;
	} else if (heightMode == SizeMode::Fixed) {
		measuredHeight = getHeight();
	}
	layout->measuredWidth = measuredWidth;
	layout->measuredHeight = measuredHeight;
	
	if (flagHorizontal || flagVertical) {
		onMeasureLayout(flagHorizontal, flagVertical);
	}
	
	if (widthMode == SizeMode::Wrapping) {
		measuredWidth = layout->measuredWidth;
	}
	if (heightMode == SizeMode::Wrapping) {
		measuredHeight = layout->measuredHeight;
	}
	
	layout->flagInvalidMeasure = sl_false;

}

sl_ui_len View::getMeasuredWidth()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		if (layout->flagEnabled) {
			if (layout->widthMode == SizeMode::Wrapping) {
				return layout->measuredWidth;
			} else if (layout->widthMode == SizeMode::Filling) {
				return 0;
			} else if (layout->widthMode == SizeMode::Weight) {
				return 0;
			}
		}
	}
	return getWidth();
}

void View::setMeasuredWidth(sl_ui_len width)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		if (layout->flagEnabled) {
			layout->measuredWidth = width;
		}
	}
}

sl_ui_len View::getMeasuredHeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		if (layout->flagEnabled) {
			if (layout->heightMode == SizeMode::Wrapping) {
				return layout->measuredHeight;
			} else if (layout->heightMode == SizeMode::Filling) {
				return 0;
			} else if (layout->heightMode == SizeMode::Weight) {
				return 0;
			}
		}
	}
	return getHeight();
}

void View::setMeasuredHeight(sl_ui_len height)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		if (layout->flagEnabled) {
			layout->measuredHeight = height;
		}
	}
}

void View::_prepareLayout(ViewPrepareLayoutParam& param)
{
	if (!(isVisible())) {
		return;
	}
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNull()) {
		return;
	}
	if (!(layout->flagEnabled)) {
		return;
	}
	
	UIRect frame;
	if (param.flagUseLayoutFrame) {
		frame = layout->frame;
	} else {
		frame = m_frame;
	}
	
	SizeMode widthMode = layout->widthMode;
	SizeMode heightMode = layout->heightMode;
	
	if (widthMode == SizeMode::Wrapping || heightMode == SizeMode::Wrapping) {
		measureLayout();
	}
	
	PositionMode leftMode = layout->leftMode;
	PositionMode topMode = layout->topMode;
	PositionMode rightMode = layout->rightMode;
	PositionMode bottomMode = layout->bottomMode;
	
	if (widthMode == SizeMode::Filling) {
		if (leftMode == PositionMode::CenterInParent || leftMode == PositionMode::CenterInOther) {
			widthMode = SizeMode::Weight;
			rightMode = PositionMode::Fixed;
		} else {
			if (leftMode == PositionMode::Fixed) {
				leftMode = PositionMode::ParentEdge;
			}
			if (rightMode == PositionMode::Fixed) {
				rightMode = PositionMode::ParentEdge;
			}
		}
	} else {
		if (leftMode != PositionMode::Fixed) {
			rightMode = PositionMode::Fixed;
		}
	}
	if (heightMode == SizeMode::Filling) {
		if (topMode == PositionMode::CenterInParent || topMode == PositionMode::CenterInOther) {
			heightMode = SizeMode::Weight;
			bottomMode = PositionMode::Fixed;
		} else {
			if (topMode == PositionMode::Fixed) {
				topMode = PositionMode::ParentEdge;
			}
			if (bottomMode == PositionMode::Fixed) {
				bottomMode = PositionMode::ParentEdge;
			}
		}
	} else {
		if (topMode != PositionMode::Fixed) {
			bottomMode = PositionMode::Fixed;
		}
	}
	
	sl_ui_pos parentWidth = param.parentContentFrame.getWidth();
	sl_ui_pos parentHeight = param.parentContentFrame.getHeight();
	
	sl_ui_pos width, height;
	switch (widthMode) {
		case SizeMode::Wrapping:
			width = layout->measuredWidth;
			break;
		case SizeMode::Filling:
		case SizeMode::Weight:
			width = (sl_ui_pos)((sl_real)parentWidth * Math::abs(layout->widthWeight));
			break;
		default:
			width = frame.getWidth();
			break;
	}
	switch (heightMode) {
		case SizeMode::Wrapping:
			height = layout->measuredHeight;
			break;
		case SizeMode::Filling:
		case SizeMode::Weight:
			height = (sl_ui_pos)((sl_real)parentHeight * Math::abs(layout->heightWeight));
			break;
		default:
			height = frame.getHeight();
			break;
	}
	if (width < 0) {
		width = 0;
	}
	if (height < 0) {
		height = 0;
	}
	
	if (layout->flagRelativeMarginLeft) {
		layout->marginLeft = (sl_ui_pos)((sl_real)parentWidth * layout->relativeMarginLeftWeight);
	}
	if (layout->flagRelativeMarginTop) {
		layout->marginTop = (sl_ui_pos)((sl_real)parentHeight * layout->relativeMarginTopWeight);
	}
	if (layout->flagRelativeMarginRight) {
		layout->marginRight = (sl_ui_pos)((sl_real)parentWidth * layout->relativeMarginRightWeight);
	}
	if (layout->flagRelativeMarginBottom) {
		layout->marginBottom = (sl_ui_pos)((sl_real)parentHeight * layout->relativeMarginBottomWeight);
	}
	Ref<View> referView;
	switch (leftMode) {
		case PositionMode::ParentEdge:
			frame.left = param.parentContentFrame.left + layout->marginLeft;
			break;
		case PositionMode::OtherStart:
			referView = layout->leftReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.left = referFrame.left + layout->marginLeft;
			} else {
				frame.left = param.parentContentFrame.left + layout->marginLeft;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layout->leftReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.left = referFrame.right + referView->getMarginRight() + layout->marginLeft;
			} else {
				frame.left = param.parentContentFrame.left + layout->marginLeft;
			}
			break;
		case PositionMode::CenterInParent:
			frame.left = (param.parentContentFrame.left + layout->marginLeft + param.parentContentFrame.right - layout->marginRight - width) / 2;
			break;
		case PositionMode::CenterInOther:
			referView = layout->leftReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.left = (referFrame.left + layout->marginLeft + referFrame.right - layout->marginRight - width) / 2;
			} else {
				frame.left = (param.parentContentFrame.left + layout->marginLeft + param.parentContentFrame.right - layout->marginRight - width) / 2;
			}
		default:
			break;
	}
	switch (rightMode) {
		case PositionMode::ParentEdge:
			frame.right = param.parentContentFrame.right - layout->marginRight;
			break;
		case PositionMode::OtherStart:
			referView = layout->rightReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.right = referFrame.left - referView->getMarginLeft() - layout->marginRight;
			} else {
				frame.right = param.parentContentFrame.right - layout->marginRight;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layout->rightReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.right = referFrame.right - layout->marginRight;
			} else {
				frame.right = param.parentContentFrame.right - layout->marginRight;
			}
			break;
		default:
			frame.right = param.parentContentFrame.right;
			break;
	}
	switch (topMode) {
		case PositionMode::ParentEdge:
			frame.top = param.parentContentFrame.top + layout->marginTop;
			break;
		case PositionMode::OtherStart:
			referView = layout->topReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.top = referFrame.top + layout->marginTop;
			} else {
				frame.top = param.parentContentFrame.top + layout->marginTop;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layout->topReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.top = referFrame.bottom + referView->getMarginBottom() + layout->marginTop;
			} else {
				frame.top = param.parentContentFrame.top + layout->marginTop;
			}
			break;
		case PositionMode::CenterInParent:
			frame.top = (param.parentContentFrame.top + layout->marginTop + param.parentContentFrame.bottom - layout->marginBottom - height) / 2;
			break;
		case PositionMode::CenterInOther:
			referView = layout->topReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.top = (referFrame.top + layout->marginTop + referFrame.bottom - layout->marginBottom - height) / 2;
			} else {
				frame.top = (param.parentContentFrame.top + layout->marginTop + param.parentContentFrame.bottom - layout->marginBottom - height) / 2;
			}
			break;
		default:
			break;
	}
	switch (bottomMode)
	{
		case PositionMode::ParentEdge:
			frame.bottom = param.parentContentFrame.bottom - layout->marginBottom;
			break;
		case PositionMode::OtherStart:
			referView = layout->bottomReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.bottom = referFrame.top - referView->getMarginTop() - layout->marginBottom;
			} else {
				frame.bottom = param.parentContentFrame.bottom - layout->marginBottom;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layout->bottomReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.bottom = referFrame.bottom - layout->marginBottom;
			} else {
				frame.bottom = param.parentContentFrame.bottom - layout->marginBottom;
			}
			break;
		default:
			frame.bottom = param.parentContentFrame.bottom;
			break;
	}
	if (widthMode == SizeMode::Filling) {
		if (frame.right < frame.left) {
			frame.right = frame.left;
		}
		if (width > 0 && width < frame.right - frame.left) {
			if (layout->widthWeight > 0) {
				frame.right = frame.left + width;
			} else {
				frame.left = frame.right - width;
			}
		}
	} else {
		if (leftMode == PositionMode::Fixed) {
			if (rightMode != PositionMode::Fixed) {
				frame.left = frame.right - width;
			} else {
				frame.right = frame.left + width;
			}
		} else {
			frame.right = frame.left + width;
		}
	}
	if (heightMode == SizeMode::Filling) {
		if (frame.bottom < frame.top) {
			frame.bottom = frame.top;
		}
		if (height > 0 && height < frame.bottom - frame.top) {
			if (layout->heightWeight > 0) {
				frame.bottom = frame.top + height;
			} else {
				frame.top = frame.bottom - height;
			}
		}
	} else {
		if (topMode == PositionMode::Fixed) {
			if (bottomMode != PositionMode::Fixed) {
				frame.top = frame.bottom - height;
			} else {
				frame.bottom = frame.top + height;
			}
		} else {
			frame.bottom = frame.top + height;
		}
	}
	frame.fixSizeError();
	layout->frame = frame;
	layout->flagUpdatedLayoutFrame = sl_true;
}

void View::_makeLayout(sl_bool flagApplyLayout)
{
	if (!(UI::isUiThread())) {
		return;
	}
	if (isVisible()) {
		
		Ref<LayoutAttributes> layout = m_layout;
		
		if (layout.isNotNull() && layout->flagEnabled && !(layout->flagRecursiveMakeLayout)) {
			
			layout->flagRecursiveMakeLayout = sl_true;
			
			do {
				if (flagApplyLayout) {
					if (layout->frame.isAlmostEqual(m_frame)) {
						break;
					}
					UISize oldSize = m_frame.getSize();
					_setFrame(layout->frame, sl_false, sl_true);
					if (layout->frame.getSize().isAlmostEqual(oldSize)) {
						break;
					}
				} else {
					if (!(layout->flagInvalidLayout)) {
						break;
					}
					if (layout->widthMode == SizeMode::Wrapping || layout->heightMode == SizeMode::Wrapping) {
						measureLayout();
						UIRect frame = layout->frame;
						if (layout->widthMode == SizeMode::Wrapping) {
							frame.setWidth(layout->measuredWidth);
						}
						if (layout->heightMode == SizeMode::Wrapping) {
							frame.setHeight(layout->measuredHeight);
						}
						_setFrame(frame, sl_false, sl_true);
					}
				}
				
				ViewPrepareLayoutParam param;
				param.parentContentFrame.left = layout->paddingLeft;
				param.parentContentFrame.top = layout->paddingTop;
				param.parentContentFrame.right = getWidth() - layout->paddingRight;
				param.parentContentFrame.bottom = getHeight() - layout->paddingBottom;
				param.parentContentFrame.fixSizeError();
				param.flagUseLayoutFrame = sl_false;
				
				{
					ListLocker< Ref<View> > children(m_children);
					for (sl_size i = 0; i < children.count; i++) {
						Ref<View>& child = children[i];
						if (child.isNotNull() && child->isLayoutEnabled()) {
							child->_prepareLayout(param);
							child->setLayoutFrameUpdated(sl_false);
						}
					}
				}
				
				if (layout->flagOnMakeLayout) {
					onMakeLayout();
				}
				
				param.flagUseLayoutFrame = sl_true;

				{
					ListLocker< Ref<View> > children(m_children);
					for (sl_size i = 0; i < children.count; i++) {
						Ref<View>& child = children[i];
						if (child.isNotNull() && child->isLayoutEnabled()) {
							if (!(child->isLayoutFrameUpdated())) {
								child->_prepareLayout(param);
								if (child->isOnPrepareLayoutEnabled()) {
									child->onPrepareLayout(param);
								}
							}
							child->_makeLayout(sl_true);
						}
					}
				}
				
				layout->flagInvalidLayout = sl_false;
				
			} while (0);

			layout->flagRecursiveMakeLayout = sl_false;
			
		}
	}

}

void View::_measureRelativeBoundWidth()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNull()) {
		return;
	}
	if (!(layout->flagEnabled)) {
		return;
	}
	
	if (!(layout->flagInvalidRelativeBoundWidth)) {
		return;
	}
	layout->flagInvalidRelativeBoundWidth = sl_false;
	layout->measuredRelativeBoundWidth = 0;
	layout->flagBadRelativeBoundWidth = sl_false;
	
	measureLayout();
	
	PositionMode leftMode = layout->leftMode;
	PositionMode rightMode = layout->rightMode;
	
	SizeMode widthMode = layout->widthMode;
	if (widthMode == SizeMode::Filling) {
		if (leftMode == PositionMode::CenterInParent || leftMode == PositionMode::CenterInOther) {
			rightMode = PositionMode::Fixed;
		} else {
			if (leftMode == PositionMode::Fixed) {
				leftMode = PositionMode::ParentEdge;
			}
			if (rightMode == PositionMode::Fixed) {
				rightMode = PositionMode::ParentEdge;
			}
		}
	}
	if (leftMode != PositionMode::Fixed) {
		rightMode = PositionMode::Fixed;
	}
	
	
	sl_ui_pos marginLeft = 0;
	if (!(layout->flagRelativeMarginLeft)) {
		marginLeft = layout->marginLeft;
	}
	sl_ui_pos marginRight = 0;
	if (!(layout->flagRelativeMarginRight)) {
		marginRight = layout->marginRight;
	}
	
	sl_ui_pos parentPaddingLeft = 0;
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parentPaddingLeft = parent->getPaddingLeft();
	}

	if (leftMode == PositionMode::Fixed && rightMode == PositionMode::Fixed) {
		marginLeft = m_frame.left - parentPaddingLeft;
	}
	
	sl_ui_pos outerWidth = layout->measuredWidth + marginLeft + marginRight;
	
	if (leftMode == PositionMode::OtherStart || leftMode == PositionMode::OtherEnd) {
		Ref<View> referView = layout->leftReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayout = referView->m_layout;
			if (referLayout.isNull() || !(layout->flagEnabled)) {
				if (leftMode == PositionMode::OtherStart) {
					outerWidth += referView->m_frame.left - parentPaddingLeft;
				} else {
					outerWidth += referView->m_frame.right - parentPaddingLeft;
				}
			} else {
				if (referLayout->rightMode != PositionMode::Fixed || referLayout->leftMode == PositionMode::CenterInOther || referLayout->leftMode == PositionMode::CenterInParent) {
					layout->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				referView->_measureRelativeBoundWidth();
				if (referLayout->flagBadRelativeBoundWidth) {
					layout->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				if (leftMode == PositionMode::OtherStart) {
					sl_ui_pos w = referLayout->measuredRelativeBoundWidth;
					w -= referView->getMeasuredWidth();
					w -= referView->getAbsoluteMarginRight();
					if (w < 0) {
						w = 0;
					}
					outerWidth += w;
				} else {
					sl_ui_pos w = referLayout->measuredRelativeBoundWidth;
					w -= referView->getAbsoluteMarginRight();
					if (w < 0) {
						w = 0;
					}
					outerWidth += w;
				}
			}
		}
	}
	if (rightMode == PositionMode::OtherStart || rightMode == PositionMode::OtherEnd) {
		Ref<View> referView = layout->rightReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayout = referView->m_layout;
			if (referLayout.isNull() || !(layout->flagEnabled)) {
				layout->flagBadRelativeBoundWidth = sl_true;
				return;
			} else {
				if (referLayout->rightMode == PositionMode::Fixed || referLayout->leftMode != PositionMode::Fixed) {
					layout->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				referView->_measureRelativeBoundWidth();
				if (referLayout->flagBadRelativeBoundWidth) {
					layout->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				if (rightMode == PositionMode::OtherStart) {
					sl_ui_pos w = referLayout->measuredRelativeBoundWidth;
					w -= referView->getAbsoluteMarginLeft();
					if (w < 0) {
						w = 0;
					}
					outerWidth += w;
				} else {
					sl_ui_pos w = referLayout->measuredRelativeBoundWidth;
					w -= referView->getMeasuredWidth();
					w -= referView->getAbsoluteMarginLeft();
					if (w < 0) {
						w = 0;
					}
					outerWidth += w;
				}
			}
		}
	}
	
	if (outerWidth < 0) {
		outerWidth = 0;
	}
	
	layout->measuredRelativeBoundWidth = outerWidth;
	
}

void View::_measureRelativeBoundHeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNull()) {
		return;
	}
	if (!(layout->flagEnabled)) {
		return;
	}
	
	if (!(layout->flagInvalidRelativeBoundHeight)) {
		return;
	}
	layout->flagInvalidRelativeBoundHeight = sl_false;
	layout->measuredRelativeBoundHeight = 0;
	layout->flagBadRelativeBoundHeight = sl_false;
	
	measureLayout();
	
	PositionMode topMode = layout->topMode;
	PositionMode bottomMode = layout->bottomMode;
	
	SizeMode heightMode = layout->heightMode;
	if (heightMode == SizeMode::Filling) {
		if (topMode == PositionMode::CenterInParent || topMode == PositionMode::CenterInOther) {
			bottomMode = PositionMode::Fixed;
		} else {
			if (topMode == PositionMode::Fixed) {
				topMode = PositionMode::ParentEdge;
			}
			if (bottomMode == PositionMode::Fixed) {
				bottomMode = PositionMode::ParentEdge;
			}
		}
	}
	if (topMode != PositionMode::Fixed) {
		bottomMode = PositionMode::Fixed;
	}
	
	sl_ui_pos marginTop = 0;
	if (!(layout->flagRelativeMarginTop)) {
		marginTop = layout->marginTop;
	}
	sl_ui_pos marginBottom = 0;
	if (!(layout->flagRelativeMarginBottom)) {
		marginBottom = layout->marginBottom;
	}
	
	sl_ui_pos parentPaddingTop = 0;
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parentPaddingTop = parent->getPaddingTop();
	}
	if (topMode == PositionMode::Fixed && bottomMode == PositionMode::Fixed) {
		marginTop = m_frame.top - getPaddingBottom();
	}
	
	sl_ui_pos outerHeight = layout->measuredHeight + marginTop + marginBottom;
	
	if (topMode == PositionMode::OtherStart || topMode == PositionMode::OtherEnd) {
		Ref<View> referView = layout->topReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayout = referView->m_layout;
			if (referLayout.isNull() || !(layout->flagEnabled)) {
				if (topMode == PositionMode::OtherStart) {
					outerHeight += referView->m_frame.top - parentPaddingTop;
				} else {
					outerHeight += referView->m_frame.bottom - parentPaddingTop;
				}
			} else {
				if (referLayout->bottomMode != PositionMode::Fixed || referLayout->topMode == PositionMode::CenterInOther || referLayout->topMode == PositionMode::CenterInParent) {
					layout->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				referView->_measureRelativeBoundHeight();
				if (referLayout->flagBadRelativeBoundHeight) {
					layout->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				if (topMode == PositionMode::OtherStart) {
					sl_ui_pos h = referLayout->measuredRelativeBoundHeight;
					h -= referView->getMeasuredHeight();
					h -= referView->getAbsoluteMarginBottom();
					if (h < 0) {
						h = 0;
					}
					outerHeight += h;
				} else {
					sl_ui_pos h = referLayout->measuredRelativeBoundHeight;
					h -= referView->getAbsoluteMarginBottom();
					if (h < 0) {
						h = 0;
					}
					outerHeight += h;
				}
			}
		}
	}
	
	if (bottomMode == PositionMode::OtherStart || bottomMode == PositionMode::OtherEnd) {
		Ref<View> referView = layout->bottomReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayout = referView->m_layout;
			if (referLayout.isNull() || !(layout->flagEnabled)) {
				layout->flagBadRelativeBoundHeight = sl_true;
				return;
			} else {
				if (referLayout->bottomMode == PositionMode::Fixed || referLayout->topMode != PositionMode::Fixed) {
					layout->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				referView->_measureRelativeBoundHeight();
				if (referLayout->flagBadRelativeBoundHeight) {
					layout->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				if (bottomMode == PositionMode::OtherStart) {
					sl_ui_pos h = referLayout->measuredRelativeBoundHeight;
					h -= referView->getAbsoluteMarginTop();
					if (h < 0) {
						h = 0;
					}
					outerHeight += h;
				} else {
					sl_ui_pos h = referLayout->measuredRelativeBoundHeight;
					h -= referView->getMeasuredHeight();
					h -= referView->getAbsoluteMarginTop();
					if (h < 0) {
						h = 0;
					}
					outerHeight += h;
				}
			}
		}
	}
	
	if (outerHeight < 0) {
		outerHeight = 0;
	}
	
	layout->measuredRelativeBoundHeight = outerHeight;
	
}

void View::measureRelativeLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	sl_ui_pos measuredWidth = 0;
	sl_ui_pos measuredHeight = 0;
	sl_ui_pos paddingWidth = getPaddingLeft() + getPaddingRight();
	sl_ui_pos paddingHeight = getPaddingTop() + getPaddingBottom();
	
	sl_size i;
	ListLocker< Ref<View> > children(_getChildren());
	for (i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				Ref<LayoutAttributes> layout = child->m_layout;
				if (layout.isNotNull() && layout->flagEnabled) {
					layout->flagInvalidRelativeBoundWidth = sl_true;
					layout->flagInvalidRelativeBoundHeight = sl_true;
				}
			}
		}
	}
	for (i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				if (child->getVisibility() != Visibility::Gone) {
					Ref<LayoutAttributes> layout = child->m_layout;
					if (layout.isNotNull() && layout->flagEnabled) {
						if (flagHorizontal) {
							child->_measureRelativeBoundWidth();
							if (!(layout->flagBadRelativeBoundWidth)) {
								sl_ui_pos w = layout->measuredRelativeBoundWidth + paddingWidth;
								if (w > measuredWidth) {
									measuredWidth = w;
								}
							}
						}
						if (flagVertical) {
							child->_measureRelativeBoundHeight();
							if (!(layout->flagBadRelativeBoundHeight)) {
								sl_ui_pos h = layout->measuredRelativeBoundHeight + paddingHeight;
								if (h > measuredHeight) {
									measuredHeight = h;
								}
							}
						}
					} else {
						if (flagHorizontal) {
							sl_ui_pos w = child->m_frame.right;
							if (w > measuredWidth) {
								measuredWidth = w;
							}
						}
						if (flagVertical) {
							sl_ui_pos h = child->m_frame.bottom;
							if (h > measuredHeight) {
								measuredHeight = h;
							}
						}
					}
				}
			}
		}
	}
	if (flagHorizontal) {
		setMeasuredWidth(measuredWidth);
	}
	if (flagVertical) {
		setMeasuredHeight(measuredHeight);
	}
}

void View::_requestInvalidateLayout()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		layout->flagInvalidLayout = sl_true;
		layout->flagInvalidMeasure = sl_true;
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _makeLayout, this, sl_false));
	}
}

void View::_requestInvalidateMeasure(sl_bool flagWidth, sl_bool flagHeight)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		layout->flagInvalidLayout = sl_true;
		flagWidth = flagWidth && layout->widthMode == SizeMode::Wrapping;
		flagHeight = flagHeight && layout->heightMode == SizeMode::Wrapping;
		if (flagWidth || flagHeight) {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->_requestInvalidateMeasure(flagWidth, flagHeight);
			}
		}
		_requestInvalidateLayout();
	}
}

void View::requestLayout(sl_bool flagRedraw)
{
	_requestInvalidateMeasure(sl_true, sl_true);
	if (flagRedraw) {
		invalidate();
	}
}

void View::requestParentLayout(sl_bool flagRedraw)
{
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		parent->requestLayout(flagRedraw);
	}
}

void View::requestParentAndSelfLayout(sl_bool flagRedraw)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		layout->flagInvalidLayout = sl_true;
		layout->flagInvalidMeasure = sl_true;
		Ref<View> parent = m_parent;
		if (parent.isNotNull() && parent->isLayoutEnabled()) {
			parent->requestLayout(flagRedraw);
			_requestInvalidateLayout();
		} else {
			_requestInvalidateLayout();
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

sl_bool View::isLayoutEnabled()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagEnabled;
	}
	return sl_false;
}

void View::setLayoutEnabled(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = flagEnabled;
		if (flagRedraw) {
			invalidate();
		}
	}
}

UIRect View::getLayoutFrame()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->frame;
	}
	return getFrame();
}

void View::setLayoutFrame(const UIRect& rect)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		layout->frame = rect;
		layout->flagUpdatedLayoutFrame = sl_true;
	} else {
		_setFrame(rect, sl_false, sl_true);
	}
}

sl_bool View::isLayoutFrameUpdated()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->flagUpdatedLayoutFrame;
	}
	return sl_false;
}

void View::setLayoutFrameUpdated(sl_bool flag)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		layout->flagUpdatedLayoutFrame = flag;
	}
}

sl_bool View::isOnPrepareLayoutEnabled()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagOnPrepareLayout;
	}
	return sl_false;
}

void View::setOnPrepareLayoutEnabled(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagOnPrepareLayout = flagEnabled;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isOnMakeLayoutEnabled()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagOnMakeLayout;
	}
	return sl_false;
}

void View::setOnMakeLayoutEnabled(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagOnMakeLayout = flagEnabled;
		requestLayout(flagRedraw);
	}
}

SizeMode View::getWidthMode()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->widthMode;
	}
	return SizeMode::Fixed;
}

SizeMode View::getHeightMode()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->heightMode;
	}
	return SizeMode::Fixed;
}

sl_bool View::isWidthFixed()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->widthMode == SizeMode::Fixed;
	}
	return sl_true;
}

void View::setWidthFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Fixed;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_bool View::isHeightFixed()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->heightMode == SizeMode::Fixed;
	}
	return sl_true;
}

void View::setHeightFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->heightMode = SizeMode::Fixed;
		requestParentAndSelfLayout(flagRedraw);
	}
}

void View::setSizeFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Fixed;
		layout->heightMode = SizeMode::Fixed;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_real View::getWidthWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->widthWeight;
	}
	return 1;
}

sl_bool View::isHeightWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->heightMode == SizeMode::Weight;
	}
	return sl_false;
}

sl_bool View::isWidthFilling()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->widthMode == SizeMode::Filling;
	}
	return sl_false;
}

void View::setWidthFilling(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Filling;
		layout->widthWeight = weight;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_bool View::isHeightFilling()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->heightMode == SizeMode::Filling;
	}
	return sl_false;
}

void View::setHeightFilling(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->heightMode = SizeMode::Filling;
		layout->heightWeight = weight;
		requestParentAndSelfLayout(flagRedraw);
	}
}

void View::setSizeFilling(sl_real widthWeight, sl_real heightWeight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Filling;
		layout->widthWeight = widthWeight;
		layout->heightMode = SizeMode::Filling;
		layout->heightWeight = heightWeight;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_bool View::isWidthWrapping()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->widthMode == SizeMode::Wrapping;
	}
	return sl_false;
}

void View::setWidthWrapping(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Wrapping;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_bool View::isHeightWrapping()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->heightMode == SizeMode::Wrapping;
	}
	return sl_false;
}

void View::setHeightWrapping(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->heightMode = SizeMode::Wrapping;
		requestParentAndSelfLayout(flagRedraw);
	}
}

void View::setSizeWrapping(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Wrapping;
		layout->heightMode = SizeMode::Wrapping;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_bool View::isWidthWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->widthMode == SizeMode::Weight;
	}
	return sl_false;
}

void View::setWidthWeight(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Weight;
		layout->widthWeight = weight;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_real View::getHeightWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->heightWeight;
	}
	return 1;
}

void View::setHeightWeight(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->heightMode = SizeMode::Weight;
		layout->heightWeight = weight;
		requestParentAndSelfLayout(flagRedraw);
	}
}

void View::setSizeWeight(sl_real widthWeight, sl_real heightWeight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->widthMode = SizeMode::Weight;
		layout->widthWeight = widthWeight;
		layout->heightMode = SizeMode::Weight;
		layout->heightWeight = heightWeight;
		requestParentAndSelfLayout(flagRedraw);
	}
}

sl_bool View::isLayoutLeftFixed()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->leftMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutLeftFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::Fixed;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignParentLeft()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->leftMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentLeft(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::ParentEdge;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignLeft()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->leftMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setAlignLeft(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::OtherStart;
		layout->leftReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isRightOf()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->leftMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setRightOf(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::OtherEnd;
		layout->leftReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

Ref<View> View::getLayoutLeftReferingView()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->leftReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isLayoutRightFixed()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->rightMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutRightFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->rightMode = PositionMode::Fixed;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignParentRight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->rightMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentRight(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->rightMode = PositionMode::ParentEdge;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignRight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->rightMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setAlignRight(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->rightMode = PositionMode::OtherEnd;
		layout->rightReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isLeftOf()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->rightMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setLeftOf(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->rightMode = PositionMode::OtherStart;
		layout->rightReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

Ref<View> View::getLayoutRightReferingView()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->rightReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isLayoutTopFixed()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->topMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutTopFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->topMode = PositionMode::Fixed;
		requestParentLayout(flagRedraw);
	}

}

sl_bool View::isAlignParentTop()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->topMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentTop(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->topMode = PositionMode::ParentEdge;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignTop()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->topMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setAlignTop(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->topMode = PositionMode::OtherStart;
		layout->topReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isBelow()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->topMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setBelow(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->topMode = PositionMode::OtherEnd;
		layout->topReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

Ref<View> View::getLayoutTopReferingView()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->topReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isLayoutBottomFixed()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->bottomMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutBottomFixed(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->bottomMode = PositionMode::Fixed;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignParentBottom()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->bottomMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentBottom(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->bottomMode = PositionMode::ParentEdge;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignBottom()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->bottomMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setAlignBottom(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNotNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->bottomMode = PositionMode::OtherEnd;
		layout->bottomReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAbove()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->bottomMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setAbove(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNotNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->bottomMode = PositionMode::OtherStart;
		layout->bottomReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

Ref<View> View::getLayoutBottomReferingView()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->bottomReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isCenterHorizontal()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->leftMode == PositionMode::CenterInParent;
	}
	return sl_false;
}

void View::setCenterHorizontal(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::CenterInParent;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isCenterVertical()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->topMode == PositionMode::CenterInParent;
	}
	return sl_false;
}

void View::setCenterVertical(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->topMode = PositionMode::CenterInParent;
		requestParentLayout(flagRedraw);
	}
}

void View::setCenterInParent(sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::CenterInParent;
		layout->topMode = PositionMode::CenterInParent;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignCenterHorizontal()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->leftMode == PositionMode::CenterInOther;
	}
	return sl_false;
}

void View::setAlignCenterHorizontal(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNotNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->leftMode = PositionMode::CenterInOther;
		layout->leftReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isAlignCenterVertical()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && layout->flagEnabled) {
		return layout->topMode == PositionMode::CenterInOther;
	}
	return sl_false;
}

void View::setAlignCenterVertical(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNotNull()) {
		return;
	}
	_initializeLayout();
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		layout->flagEnabled = sl_true;
		layout->topMode = PositionMode::CenterInOther;
		layout->topReferingView = view;
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getMarginLeft()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->marginLeft;
	}
	return 0;
}

void View::setMarginLeft(sl_ui_pos marginLeft, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->marginLeft = marginLeft;
		if (attr->leftMode == PositionMode::Fixed && attr->rightMode == PositionMode::Fixed) {
			attr->leftMode = PositionMode::ParentEdge;
		}
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getMarginTop()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->marginTop;
	}
	return 0;
}

void View::setMarginTop(sl_ui_pos marginTop, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->marginTop = marginTop;
		if (attr->topMode == PositionMode::Fixed && attr->bottomMode == PositionMode::Fixed) {
			attr->topMode = PositionMode::ParentEdge;
		}
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getMarginRight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->marginRight;
	}
	return 0;
}

void View::setMarginRight(sl_ui_pos marginRight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->marginRight = marginRight;
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getMarginBottom()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->marginBottom;
	}
	return 0;
}

void View::setMarginBottom(sl_ui_pos marginBottom, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->marginBottom = marginBottom;
		requestParentLayout(flagRedraw);
	}
}

void View::setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->marginLeft = left;
		attr->marginTop = top;
		attr->marginRight = right;
		attr->marginBottom = bottom;
		requestParentLayout(flagRedraw);
	}
}

void View::setMargin(sl_ui_pos margin, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->marginLeft = margin;
		attr->marginTop = margin;
		attr->marginRight = margin;
		attr->marginBottom = margin;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isRelativeMarginLeft()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagRelativeMarginLeft;
	}
	return sl_false;
}

sl_real View::getRelativeMarginLeftWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->relativeMarginLeftWeight;
	}
	return 0;
}

void View::setRelativeMarginLeft(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginLeft = sl_true;
		attr->relativeMarginLeftWeight = weight;
		if (attr->leftMode == PositionMode::Fixed && attr->rightMode == PositionMode::Fixed) {
			attr->leftMode = PositionMode::ParentEdge;
		}
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getAbsoluteMarginLeft()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && !(layout->flagRelativeMarginLeft)) {
		if (layout->leftMode == PositionMode::Fixed && layout->rightMode == PositionMode::Fixed) {
			return 0;
		}
		return layout->marginLeft;
	}
	return 0;
}

void View::setAbsoluteMarginLeft(sl_ui_pos margin, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginLeft = sl_false;
		attr->marginLeft = margin;
		if (attr->leftMode == PositionMode::Fixed && attr->rightMode == PositionMode::Fixed) {
			attr->leftMode = PositionMode::ParentEdge;
		}
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isRelativeMarginTop()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagRelativeMarginTop;
	}
	return sl_false;
}

sl_real View::getRelativeMarginTopWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->relativeMarginTopWeight;
	}
	return 0;
}

void View::setRelativeMarginTop(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginTop = sl_true;
		attr->relativeMarginTopWeight = weight;
		if (attr->topMode == PositionMode::Fixed && attr->bottomMode == PositionMode::Fixed) {
			attr->topMode = PositionMode::ParentEdge;
		}
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getAbsoluteMarginTop()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && !(layout->flagRelativeMarginTop)) {
		if (layout->topMode == PositionMode::Fixed && layout->bottomMode == PositionMode::Fixed) {
			return 0;
		}
		return layout->marginTop;
	}
	return 0;
}

void View::setAbsoluteMarginTop(sl_ui_pos margin, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginTop = sl_false;
		attr->marginTop = margin;
		if (attr->topMode == PositionMode::Fixed && attr->bottomMode == PositionMode::Fixed) {
			attr->topMode = PositionMode::ParentEdge;
		}
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isRelativeMarginRight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagRelativeMarginRight;
	}
	return sl_false;
}

sl_real View::getRelativeMarginRightWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->relativeMarginRightWeight;
	}
	return 0;
}

void View::setRelativeMarginRight(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginRight = sl_true;
		attr->relativeMarginRightWeight = weight;
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getAbsoluteMarginRight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && !(layout->flagRelativeMarginRight)) {
		if (layout->leftMode == PositionMode::Fixed && layout->rightMode == PositionMode::Fixed) {
			return 0;
		}
		return layout->marginRight;
	}
	return 0;
}

void View::setAbsoluteMarginRight(sl_ui_pos margin, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginRight = sl_false;
		attr->marginRight = margin;
		requestParentLayout(flagRedraw);
	}
}

sl_bool View::isRelativeMarginBottom()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->flagRelativeMarginBottom;
	}
	return sl_false;
}

sl_real View::getRelativeMarginBottomWeight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->relativeMarginBottomWeight;
	}
	return 0;
}

void View::setRelativeMarginBottom(sl_real weight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginBottom = sl_true;
		attr->relativeMarginBottomWeight = weight;
		requestParentLayout(flagRedraw);
	}
}

sl_ui_pos View::getAbsoluteMarginBottom()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull() && !(layout->flagRelativeMarginBottom)) {
		if (layout->topMode == PositionMode::Fixed && layout->bottomMode == PositionMode::Fixed) {
			return 0;
		}
		return layout->marginBottom;
	}
	return 0;
}

void View::setAbsoluteMarginBottom(sl_ui_pos margin, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_true;
		attr->flagRelativeMarginBottom = sl_false;
		attr->marginBottom = margin;
		requestParentLayout(flagRedraw);
	}
}

void View::applyRelativeMargins(sl_ui_len parentWidth, sl_ui_len parentHeight)
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		if (layout->flagRelativeMarginLeft) {
			layout->marginLeft = (sl_ui_pos)((sl_real)parentWidth * layout->relativeMarginLeftWeight);
		}
		if (layout->flagRelativeMarginTop) {
			layout->marginTop = (sl_ui_pos)((sl_real)parentHeight * layout->relativeMarginTopWeight);
		}
		if (layout->flagRelativeMarginRight) {
			layout->marginRight = (sl_ui_pos)((sl_real)parentWidth * layout->relativeMarginRightWeight);
		}
		if (layout->flagRelativeMarginBottom) {
			layout->marginBottom = (sl_ui_pos)((sl_real)parentHeight * layout->relativeMarginBottomWeight);
		}
	}
}

sl_ui_pos View::getPaddingLeft()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->paddingLeft;
	}
	return 0;
}

void View::setPaddingLeft(sl_ui_pos paddingLeft, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->paddingLeft = paddingLeft;
		requestLayout(flagRedraw);
		onChangePadding();
	}
}

sl_ui_pos View::getPaddingTop()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->paddingTop;
	}
	return 0;
}

void View::setPaddingTop(sl_ui_pos paddingTop, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->paddingTop = paddingTop;
		requestLayout(flagRedraw);
		onChangePadding();
	}
}

sl_ui_pos View::getPaddingRight()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->paddingRight;
	}
	return 0;
}

void View::setPaddingRight(sl_ui_pos paddingRight, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->paddingRight = paddingRight;
		requestLayout(flagRedraw);
		onChangePadding();
	}
}

sl_ui_pos View::getPaddingBottom()
{
	Ref<LayoutAttributes> layout = m_layout;
	if (layout.isNotNull()) {
		return layout->paddingBottom;
	}
	return 0;
}

void View::setPaddingBottom(sl_ui_pos paddingBottom, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->paddingBottom = paddingBottom;
		requestLayout(flagRedraw);
		onChangePadding();
	}
}

void View::setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->paddingLeft = left;
		attr->paddingTop = top;
		attr->paddingRight = right;
		attr->paddingBottom = bottom;
		requestLayout(flagRedraw);
		onChangePadding();
	}
}

void View::setPadding(sl_ui_pos padding, sl_bool flagRedraw)
{
	_initializeLayout();
	Ref<LayoutAttributes> attr = m_layout;
	if (attr.isNotNull()) {
		attr->paddingLeft = padding;
		attr->paddingTop = padding;
		attr->paddingRight = padding;
		attr->paddingBottom = padding;
		requestLayout(flagRedraw);
		onChangePadding();
	}
}

void View::_initializeLayout()
{
	if (m_layout.isNotNull()) {
		return;
	}
	Ref<LayoutAttributes> attr = new LayoutAttributes;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_false;
		
		attr->widthMode = SizeMode::Fixed;
		attr->heightMode = SizeMode::Fixed;
		attr->widthWeight = 1;
		attr->heightWeight = 1;
		
		attr->leftMode = PositionMode::Fixed;
		attr->topMode = PositionMode::Fixed;
		attr->rightMode = PositionMode::Fixed;
		attr->bottomMode = PositionMode::Fixed;
		
		attr->measuredWidth = 0;
		attr->measuredHeight = 0;
		attr->frame.left = 0;
		attr->frame.top = 0;
		attr->frame.right = 0;
		attr->frame.bottom = 0;
		attr->flagInvalidMeasure = sl_true;
		attr->flagInvalidLayout = sl_true;
		
		attr->flagInvalidRelativeBoundWidth = sl_true;
		attr->measuredRelativeBoundWidth = 0;
		attr->flagBadRelativeBoundWidth = sl_false;
		attr->flagInvalidRelativeBoundHeight = sl_true;
		attr->measuredRelativeBoundHeight = 0;
		attr->flagBadRelativeBoundHeight = sl_false;
		
		attr->flagOnPrepareLayout = sl_false;
		attr->flagOnMakeLayout = sl_false;
		
		attr->paddingLeft = 0;
		attr->paddingTop = 0;
		attr->paddingRight = 0;
		attr->paddingBottom = 0;
		
		attr->marginLeft = 0;
		attr->marginTop = 0;
		attr->marginRight = 0;
		attr->marginBottom = 0;
		attr->flagRelativeMarginLeft = sl_false;
		attr->relativeMarginLeftWeight = 0;
		attr->flagRelativeMarginTop = sl_false;
		attr->relativeMarginTopWeight = 0;
		attr->flagRelativeMarginRight = sl_false;
		attr->relativeMarginRightWeight = 0;
		attr->flagRelativeMarginBottom = sl_false;
		attr->relativeMarginBottomWeight = 0;
		
		attr->flagRecursiveMakeLayout = sl_false;
		attr->flagUpdatedLayoutFrame = sl_false;
		
		m_layout = attr;
	}
}

sl_bool View::isTransformEnabled()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->flagEnabled;
	}
	return sl_false;
}

void View::setTransformEnabled(sl_bool flag, sl_bool flagRedraw)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->flagEnabled = flag;
		if (flagRedraw) {
			invalidate();
		}
	}
}

const Matrix3& View::getTransform()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->transform;
	}
	return Matrix3::identity();
}

void View::setTransform(const Matrix3& matrix, sl_bool flagRedraw)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->transform = matrix;
		transform->transformInverse = matrix.inverse();
		if (transform->flagEnabled) {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

sl_real View::getScaleX()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->scaleX;
	}
	return 1;
}

sl_real View::getScaleY()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->scaleY;
	}
	return 1;
}

Vector2 View::getScale()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return Vector2(transform->scaleX, transform->scaleY);
	}
	return Vector2(1, 1);
}

void View::setScaleX(sl_real sx, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->scaleX = sx;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

void View::setScaleY(sl_real sy, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->scaleY = sy;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

void View::setScale(sl_real sx, sl_real sy, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->scaleX = sx;
		transform->scaleY = sy;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

void View::setScale(const Vector2& factor, sl_bool flagSetTransform)
{
	setScale(factor.x, factor.y, flagSetTransform);
}

void View::setScale(sl_real factor, sl_bool flagSetTransform)
{
	setScale(factor, factor, flagSetTransform);
}

sl_real View::getRotation()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->rotationAngle;
	}
	return 0;
}

void View::setRatation(sl_real radian, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->rotationAngle = radian;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

Point View::getAnchorOffset()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return Point(transform->anchorOffsetX, transform->anchorOffsetY);
	}
	return Point::zero();
}

void View::setAnchorOffset(sl_real x, sl_real y, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->anchorOffsetX = x;
		transform->anchorOffsetY = y;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

void View::setAnchorOffset(const Point& pt, sl_bool flagSetTransform)
{
	setAnchorOffset(pt.x, pt.y, flagSetTransform);
}

sl_real View::getAnchorOffsetX()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->anchorOffsetX;
	}
	return 0;
}

void View::setAnchorOffsetX(sl_real x, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->anchorOffsetX = x;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

sl_real View::getAnchorOffsetY()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		return transform->anchorOffsetY;
	}
	return 0;
}

void View::setAnchorOffsetY(sl_real y, sl_bool flagSetTransform)
{
	_initializeTransform();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		transform->anchorOffsetY = y;
		if (flagSetTransform) {
			_applyTransform();
		}
	}
}

void View::_initializeTransform()
{
	if (m_transform.isNotNull()) {
		return;
	}
	Ref<TransformAttributes> attr = new TransformAttributes;
	if (attr.isNotNull()) {
		attr->flagEnabled = sl_false;
		attr->transform = Matrix3::identity();
		attr->transformInverse = Matrix3::identity();
		attr->scaleX = 1;
		attr->scaleY = 1;
		attr->rotationAngle = 0;
		attr->anchorOffsetX = 0;
		attr->anchorOffsetY = 0;
		m_transform = attr;
	}
}

void View::_applyTransform()
{
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull()) {
		sl_real tx = 0.5f + transform->anchorOffsetX;
		sl_real ty = 0.5f + transform->anchorOffsetY;
		Matrix3 mat = Transform2::getTranslationMatrix(-tx, -ty);
		mat *= Transform2::getRotationMatrix(transform->rotationAngle);
		mat *= Transform2::getScalingMatrix(transform->scaleX, transform->scaleY);
		mat *= Transform2::getTranslationMatrix(tx, tx);
		setTransform(mat);
	}
}

UIPointf View::convertCoordinateFromScreen(const UIPointf& ptScreen)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromScreenToView(ptScreen);
	}
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return ptScreen;
	}
	UIPointf pt = parent->convertCoordinateFromScreen(ptScreen);
	return convertCoordinateFromParent(pt);
}

UIPointf View::convertCoordinateToScreen(const UIPointf& ptView)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromViewToScreen(ptView);
	}
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return ptView;
	}
	UIPointf pt = convertCoordinateToParent(ptView);
	return parent->convertCoordinateToScreen(pt);
}

UIPointf View::convertCoordinateFromParent(const UIPointf& ptParent)
{
	if (isInstance()) {
		return UIPointf(ptParent.x - (sl_ui_posf)(m_frame.left), ptParent.y - (sl_ui_posf)(m_frame.top));
	}
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return ptParent;
	}
	
	UIPointf pt = ptParent;
	pt.x -= (sl_ui_posf)(m_frame.left);
	pt.y -= (sl_ui_posf)(m_frame.top);
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull() && transform->flagEnabled) {
		pt = transform->transformInverse.transformPosition(pt);
	}
	return pt;
}

UIPointf View::convertCoordinateToParent(const UIPointf& ptView)
{
	if (isInstance()) {
		return UIPointf(ptView.x + (sl_ui_pos)(m_frame.left), ptView.y + (sl_ui_pos)(m_frame.top));
	}
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return ptView;
	}
	
	Point pt = ptView;
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull() && transform->flagEnabled) {
		pt = transform->transform.transformPosition(pt);
	}
	pt.x += (sl_ui_pos)(m_frame.left);
	pt.y += (sl_ui_pos)(m_frame.top);
	return pt;
}

Ref<Drawable> View::getBackground()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->background;
	}
	return Ref<Drawable>::null();
}

void View::setBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->background = drawable;
		if (flagRedraw) {
			invalidate();
		}
	}
}

ScaleMode View::getBackgroundScaleMode()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->backgroundScaleMode;
	}
	return ScaleMode::Stretch;
}

void View::setBackgroundScaleMode(ScaleMode scaleMode, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->backgroundScaleMode = scaleMode;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Alignment View::getBackgroundAlignment()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->backgroundAlignment;
	}
	return Alignment::MiddleCenter;
}

void View::setBackgroundAlignment(Alignment align, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->backgroundAlignment = align;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Color View::getBackgroundColor()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->backgroundColor;
	}
	return Color::zero();
}

void View::setBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->backgroundColor = color;
		if (isNativeWidget()) {
			_setBackgroundColor_NW(color);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

Ref<Pen> View::getBorder()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->penBorder;
	}
	return Ref<Pen>::null();
}

void View::setBorder(const Ref<Pen>& pen, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->penBorder = pen;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Color View::getBorderColor()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->borderColor;
	}
	return Color::Black;
}

void View::setBorderColor(const Color& color, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->borderColor = color;
		_refreshBorderPen(flagRedraw);
	}
}

sl_bool View::isBorder()
{
	return getBorder().isNotNull();
}

void View::setBorder(sl_bool flagBorder, sl_bool flagRedraw)
{
	if (flagBorder) {
		if (isBorder()) {
			return;
		}
		setBorder(Pen::getDefault(), sl_false);
	} else {
		if (isBorder()) {
			setBorder(Ref<Pen>::null(), sl_false);
		}
	}
	if (isNativeWidget()) {
		_setBorder_NW(flagBorder);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

PenStyle View::getBorderStyle()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->borderStyle;
	}
	return PenStyle::Solid;
}

void View::setBorderStyle(PenStyle style, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->borderStyle = style;
		_refreshBorderPen(flagRedraw);
	}
}

sl_real View::getBorderWidth()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->borderWidth;
	}
	return 0;
}

void View::setBorderWidth(sl_real width, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->borderWidth = width;
		_refreshBorderPen(flagRedraw);
	}
}

BoundShape View::getBoundShape()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->boundShape;
	}
	return BoundShape::Rectangle;
}

void View::setBoundShape(BoundShape shape, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->boundShape = shape;
		if (flagRedraw) {
			invalidate();
		}
	}
}

const Size& View::getRoundRectBoundShapeRadius()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->roundRectBoundShapeRadius;
	}
	return Size::zero();
}

void View::setRoundRectBoundShapeRadius(const Size& radius, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->roundRectBoundShapeRadius = radius;
		if (flagRedraw) {
			invalidate();
		}
	}
}

void View::setRoundRectBoundShapeRadius(sl_real rx, sl_real ry, sl_bool flagRedraw)
{
	setRoundRectBoundShapeRadius(Size(rx, ry), flagRedraw);
}

void View::setRoundRectBoundShapeRadius(sl_real radius, sl_bool flagRedraw)
{
	setRoundRectBoundShapeRadius(Size(radius, radius), flagRedraw);
}

Ref<GraphicsPath> View::getBoundShapePath()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->boundShapePath;
	}
	return Ref<GraphicsPath>::null();
}

void View::setBoundShapePath(const Ref<GraphicsPath>& path, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->boundShapePath = path;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isPreDrawEnabled()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagPreDrawEnabled;
	}
	return sl_false;
}

void View::setPreDrawEnabled(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagPreDrawEnabled = flagEnabled;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isPostDrawEnabled()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagPostDrawEnabled;
	}
	return sl_false;
}

void View::setPostDrawEnabled(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagPostDrawEnabled = flagEnabled;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isClippingBounds()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagClippingBounds;
	}
	return sl_true;
}

void View::setClippingBounds(sl_bool flag)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagClippingBounds = flag;
	}
}

sl_bool View::isDoubleBuffering()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagDoubleBuffer;
	}
	return sl_false;
}

void View::setDoubleBuffering(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagDoubleBuffer = flagEnabled;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isAlwaysOnDrawBackground()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagOnDrawBackgroundAlways;
	}
	return sl_false;
}

void View::setAlwaysOnDrawBackground(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagOnDrawBackgroundAlways = flagEnabled;
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool View::isAlwaysOnDrawBorder()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagOnDrawBorderAlways;
	}
	return sl_false;
}

void View::setAlwaysOnDrawBorder(sl_bool flagEnabled, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagOnDrawBorderAlways = flagEnabled;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Ref<Font> View::getFont()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		Ref<Font> font = draw->font;
		if (font.isNotNull()) {
			return font;
		}
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		return parent->getFont();
	}
	return UI::getDefaultFont();
}

void View::_setFontInvalidateChildren()
{
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			child->_setFontInvalidateChildren();
			if (child->isUsingFont()) {
				child->requestLayout(sl_false);
				if (child->isInstance()) {
					if (child->isNativeWidget()) {
						Ref<Font> font = child->getFont();
						if (font.isNotNull()) {
							child->_setFont_NW(font);
						}
					} else {
						invalidate();
					}
				}
			}
		}
	}
}

void View::setFont(const Ref<Font>& _font, sl_bool flagRedraw)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->font = _font;
		_setFontInvalidateChildren();
		if (isNativeWidget()) {
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				_setFont_NW(font);
			}
		} else {
			if (isUsingFont()) {
				requestLayout(sl_false);
				if (flagRedraw) {
					invalidate();
				}
			}
		}
	}
}

void View::setFont(const String& fontFamily, sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, sl_bool flagRedraw)
{
	setFont(Font::create(fontFamily, size, flagBold, flagItalic, flagUnderline), flagRedraw);
}

void View::setFontAttributes(sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, sl_bool flagRedraw)
{
	Ref<Font> font = getFont();
	if (font.isNull()) {
		setFont(Font::create(UI::getDefaultFontFamily(), size, flagBold, flagItalic, flagUnderline), flagRedraw);
	} else {
		setFont(Font::create(font->getFamilyName(), size, flagBold, flagItalic, flagUnderline), flagRedraw);
	}
}

sl_bool View::isUsingFont()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->flagUsingFont;
	}
	return sl_false;
}

void View::setUsingFont(sl_bool flag)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->flagUsingFont = flag;
	}
}

void View::_setFontInstance(const Ref<FontInstance>& instance)
{
	_initializeDraw();
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		draw->fontInstance = instance;
	}
}

void View::_initializeDraw()
{
	if (m_draw.isNotNull()) {
		return;
	}
	
	Ref<DrawAttributes> attr = new DrawAttributes;
	
	if (attr.isNotNull()) {
		
		attr->backgroundScaleMode = ScaleMode::Stretch;
		attr->backgroundAlignment = Alignment::MiddleCenter;
		attr->backgroundColor = Color::zero();
		
		attr->boundShape = BoundShape::Rectangle;
		
		attr->roundRectBoundShapeRadius.x = 5;
		attr->roundRectBoundShapeRadius.y = 5;
		
		attr->borderColor = Color::Black;
		attr->borderStyle = PenStyle::Solid;
		attr->borderWidth = 0;
		
		attr->flagPreDrawEnabled = sl_false;
		attr->flagPostDrawEnabled = sl_false;
		attr->flagClippingBounds = sl_true;
		attr->flagDoubleBuffer = sl_false;
		
		attr->flagUsingFont = sl_false;
		
		attr->flagOnDrawBackgroundAlways = sl_false;
		attr->flagOnDrawBorderAlways = sl_false;
		
		m_draw = attr;
		
	}
}

void View::_refreshBorderPen(sl_bool flagRedraw)
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		sl_real width = draw->borderWidth;
		Ref<Pen> pen;
		if (width > 0) {
			pen = Pen::create(draw->borderStyle, draw->borderWidth, draw->borderColor);
		}
		setBorder(pen, flagRedraw);
	}
}

Ref<ScrollBar> View::getHorizontalScrollBar()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->horz;
	}
	return Ref<ScrollBar>::null();
}

Ref<ScrollBar> View::getVerticalScrollBar()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->vert;
	}
	return Ref<ScrollBar>::null();
}

void View::setHorizontalScrollBar(const Ref<ScrollBar>& bar, sl_bool flagRefresh)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		removeChild(scroll->horz, sl_false);
		scroll->horz = bar;
		if (flagRefresh) {
			_refreshScroll(sl_true);
		}
	}
}

void View::setVerticalScrollBar(const Ref<ScrollBar>& bar, sl_bool flagRefresh)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		removeChild(scroll->vert, sl_false);
		scroll->vert = bar;
		if (flagRefresh) {
			_refreshScroll(sl_true);
		}
	}
}

void View::createHorizontalScrollBar(sl_bool flagRefresh)
{
	setHorizontalScrollBar(_createHorizontalScrollBar(), flagRefresh);
}

void View::createVerticalScrollBar(sl_bool flagRefresh)
{
	setVerticalScrollBar(_createVerticalScrollBar(), flagRefresh);
}

void View::removeHorizontalScrollBar(sl_bool flagRefresh)
{
	setHorizontalScrollBar(Ref<ScrollBar>::null(), flagRefresh);
}

void View::removeVerticalScrollBar(sl_bool flagRefresh)
{
	setVerticalScrollBar(Ref<ScrollBar>::null(), flagRefresh);
}

void View::createScrollBars(sl_bool flagRefresh)
{
	setHorizontalScrollBar(_createHorizontalScrollBar(), sl_false);
	setVerticalScrollBar(_createVerticalScrollBar(), sl_false);
	if (flagRefresh) {
		_refreshScroll(sl_true);
	}
}

void View::removeScrollBars(sl_bool flagRefresh)
{
	setHorizontalScrollBar(Ref<ScrollBar>::null(), sl_false);
	setVerticalScrollBar(Ref<ScrollBar>::null(), sl_false);
	if (flagRefresh) {
		_refreshScroll(sl_true);
	}
}

sl_bool View::isHorizontalScrollBarVisible()
{
	Ref<ScrollBar> bar = getHorizontalScrollBar();
	if (bar.isNotNull()) {
		return bar->isVisible() && bar->isValid();
	}
	return sl_false;
}

sl_bool View::isVerticalScrollBarVisible()
{
	Ref<ScrollBar> bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		return bar->isVisible() && bar->isValid();
	}
	return sl_false;
}

void View::setHorizontalScrollBarVisible(sl_bool flagVisible, sl_bool flagRefresh)
{
	Ref<ScrollBar> bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, sl_false);
	}
	if (flagRefresh) {
		_refreshScroll(sl_true);
	}
}

void View::setVerticalScrollBarVisible(sl_bool flagVisible, sl_bool flagRefresh)
{
	Ref<ScrollBar> bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, sl_false);
	}
	if (flagRefresh) {
		_refreshScroll(sl_true);
	}
}

void View::setScrollBarsVisible(sl_bool flagVisible, sl_bool flagRefresh)
{
	Ref<ScrollBar> bar;
	bar = getHorizontalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, sl_false);
	}
	bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, sl_false);
	}
	if (flagRefresh) {
		_refreshScroll(sl_true);
	}
}

sl_scroll_pos View::getScrollX()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->x;
	}
	return 0;
}

sl_scroll_pos View::getScrollY()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->y;
	}
	return 0;
}

ScrollPoint View::getScrollPosition()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return ScrollPoint(scroll->x, scroll->y);
	}
	return ScrollPoint::zero();
}

void View::scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagRedraw)
{
	_initializeScroll();
	
	Ref<ScrollAttributes> scroll = m_scroll;
	
	if (scroll.isNotNull()) {
		
		sl_scroll_pos rx = scroll->contentWidth - getWidth();
		if (x > rx) {
			x = rx;
		}
		if (x < 0) {
			x = 0;
		}
		sl_scroll_pos ry = scroll->contentHeight - getHeight();
		if (y > ry) {
			y = ry;
		}
		if (y < 0) {
			y = 0;
		}
		
		if (Math::isAlmostZero(scroll->x - x) && Math::isAlmostZero(scroll->y - y)) {
			scroll->x = x;
			scroll->y = y;
			return;
		}
		
		scroll->x = x;
		scroll->y = y;
		
		dispatchScroll(x, y);
		
		Ref<ScrollBar> bar = scroll->horz;
		if (bar.isNotNull()) {
			bar->setValueOfOutRange(x, sl_false);
		}
		bar = scroll->vert;
		if (bar.isNotNull()) {
			bar->setValueOfOutRange(y, sl_false);
		}
		
		if (flagRedraw) {
			invalidate();
		}
	}
}

void View::scrollTo(const ScrollPoint& position, sl_bool flagRedraw)
{
	scrollTo(position.x, position.y, flagRedraw);
}

void View::setScrollX(sl_scroll_pos x, sl_bool flagRedraw)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		scrollTo(x, scroll->y, flagRedraw);
	}
}

void View::setScrollY(sl_scroll_pos y, sl_bool flagRedraw)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		scrollTo(scroll->x, y, flagRedraw);
	}
}

sl_scroll_pos View::getContentWidth()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->contentWidth;
	}
	return 0;
}

sl_scroll_pos View::getContentHeight()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->contentHeight;
	}
	return 0;
}

ScrollPoint View::getContentSize()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return ScrollPoint(scroll->contentWidth, scroll->contentHeight);
	}
	return ScrollPoint::zero();
}

void View::setContentSize(sl_scroll_pos width, sl_scroll_pos height, sl_bool flagRefresh)
{
	_initializeScroll();
	
	Ref<ScrollAttributes> scroll = m_scroll;
	
	if (scroll.isNotNull()) {
		if (Math::isAlmostZero(width - scroll->contentWidth) && Math::isAlmostZero(height - scroll->contentHeight)) {
			scroll->contentWidth = width;
			scroll->contentHeight = height;
			return;
		}
		scroll->contentWidth = width;
		scroll->contentHeight = height;
		onResizeContent(width, height);
		if (flagRefresh) {
			_refreshScroll(sl_true);
		}
	}
}

void View::setContentSize(const ScrollPoint& size, sl_bool flagRefresh)
{
	setContentSize(size.x, size.y, flagRefresh);
}

void View::setContentWidth(sl_scroll_pos width, sl_bool flagRefresh)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		setContentSize(width, scroll->contentHeight, flagRefresh);
	}
}

void View::setContentHeight(sl_scroll_pos height, sl_bool flagRefresh)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		setContentSize(scroll->contentWidth, height, flagRefresh);
	}
}

ScrollPoint View::getScrollRange()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		ScrollPoint ret(scroll->contentWidth - (sl_scroll_pos)(getWidth()), scroll->contentHeight - (sl_scroll_pos)(getHeight()));
		if (ret.x < 0) {
			ret.x = 0;
		}
		if (ret.y < 0) {
			ret.y = 0;
		}
		return ret;
	}
	return ScrollPoint::zero();
}

sl_ui_len View::getScrollBarWidth()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->barWidth;
	}
	return 0;
}

void View::setScrollBarWidth(sl_ui_len width, sl_bool flagRefresh)
{
	_initializeScroll();
	
	Ref<ScrollAttributes> scroll = m_scroll;
	
	if (scroll.isNotNull()) {
		scroll->barWidth = width;
		if (flagRefresh) {
			_refreshScroll(sl_true);
		}
	}
}

sl_bool View::isContentScrollingByMouse()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->flagContentScrollingByMouse;
	}
	return sl_true;
}

void View::setContentScrollingByMouse(sl_bool flag)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		scroll->flagContentScrollingByMouse = flag;
	}
}

sl_bool View::isContentScrollingByTouch()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->flagContentScrollingByTouch;
	}
	return sl_true;
}

void View::setContentScrollingByTouch(sl_bool flag)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		scroll->flagContentScrollingByTouch = flag;
	}
}

sl_bool View::isContentScrollingByMouseWheel()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->flagContentScrollingByMouseWheel;
	}
	return sl_true;
}

void View::setContentScrollingByMouseWheel(sl_bool flag)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		scroll->flagContentScrollingByMouseWheel = flag;
	}
}

sl_bool View::isContentScrollingByKeyboard()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->flagContentScrollingByKeyboard;
	}
	return sl_true;
}

void View::setContentScrollingByKeyboard(sl_bool flag)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		scroll->flagContentScrollingByKeyboard = flag;
	}
}

void View::_initializeScroll()
{
	if (m_scroll.isNotNull()) {
		return;
	}
	
	Ref<ScrollAttributes> scroll = new ScrollAttributes;
	
	if (scroll.isNotNull()) {
		scroll->flagValidHorz = sl_false;
		scroll->flagValidVert = sl_false;
		scroll->x = 0;
		scroll->y = 0;
		scroll->contentWidth = 0;
		scroll->contentHeight = 0;
		scroll->barWidth = 12;
		
		scroll->flagContentScrollingByMouse = sl_true;
		scroll->flagContentScrollingByTouch = sl_true;
		scroll->flagContentScrollingByMouseWheel = sl_true;
		scroll->flagContentScrollingByKeyboard = sl_true;
		scroll->flagDownContent = sl_false;
		
		m_scroll = scroll;
	}

}

Ref<ScrollBar> View::_createHorizontalScrollBar()
{
	Ref<ScrollBar> ret = new ScrollBar;
	if (ret.isNotNull()) {
		return ret;
	}
	return Ref<ScrollBar>::null();
}

Ref<ScrollBar> View::_createVerticalScrollBar()
{
	Ref<ScrollBar> ret = new ScrollBar;
	if (ret.isNotNull()) {
		return ret;
	}
	return Ref<ScrollBar>::null();
}

class _View_ScrollBarListener : public Referable, public IScrollBarListener
{
public:
	WeakRef<View> m_view;
	
public:
	_View_ScrollBarListener(View* view)
	{
		m_view = view;
	}
	
	// override
	void onScroll(ScrollBar* scrollBar, sl_scroll_pos value)
	{
		Ref<View> view = m_view;
		if (view.isNull()) {
			return;
		}
		sl_scroll_pos sx = 0;
		sl_scroll_pos sy = 0;
		Ref<ScrollBar> horz = view->getHorizontalScrollBar();
		if (horz.isNotNull()) {
			sx = horz->getValue();
		}
		Ref<ScrollBar> vert = view->getVerticalScrollBar();
		if (vert.isNotNull()) {
			sy = vert->getValue();
		}
		view->scrollTo(sx, sy);
	}
};

void View::_refreshScroll(sl_bool flagRedraw)
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		sl_ui_pos width = getWidth();
		sl_ui_pos height = getHeight();
		sl_bool flagVisibleHorz = sl_false;
		Ref<ScrollBar> barHorz = scroll->horz;
		if (barHorz.isNotNull()) {
			barHorz->setParent(this);
			barHorz->setMinimumValue(0, sl_false);
			barHorz->setMaximumValue(scroll->contentWidth, sl_false);
			barHorz->setPage(width, sl_false);
			barHorz->setValueOfOutRange(scroll->x, sl_false);
			barHorz->setFrame(UIRect(0, height - scroll->barWidth, width, height), sl_false);
			if (barHorz->getListener().isNull()) {
				barHorz->setListener(Ref<_View_ScrollBarListener>(new _View_ScrollBarListener(this)));
			}
			scroll->flagValidHorz = barHorz->isValid();
			if (barHorz->isVisible() && scroll->flagValidHorz) {
				flagVisibleHorz = sl_true;
			}
		}
		sl_bool flagVisibleVert = sl_false;
		Ref<ScrollBar> barVert = scroll->vert;
		if (barVert.isNotNull()) {
			barVert->setParent(this);
			barVert->setVertical(sl_true);
			barVert->setMinimumValue(0, sl_false);
			barVert->setMaximumValue(scroll->contentHeight, sl_false);
			barVert->setPage(height, sl_false);
			barVert->setValueOfOutRange(scroll->y, sl_false);
			barVert->setFrame(UIRect(width - scroll->barWidth, 0, width, height), sl_false);
			if (barVert->getListener().isNull()) {
				barVert->setListener(Ref<_View_ScrollBarListener>(new _View_ScrollBarListener(this)));
			}
			scroll->flagValidVert = barVert->isValid();
			if (barVert->isVisible() && scroll->flagValidVert) {
				flagVisibleVert = sl_true;
			}
		}
		if (flagVisibleHorz && flagVisibleVert) {
			//// avoid overlapping two scrollbars.
			////		2016/6/8 temporally cancel this function
			//barHorz->setFrame(UIRect(0, height - scroll->barWidth, width - scroll->barWidth, height), sl_false);
		}
		scrollTo(scroll->x, scroll->y, sl_false);
		if (flagRedraw) {
			invalidate();
		}
	}
}

void View::_getScrollBars(Ref<View> views[2])
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		if (scroll->flagValidHorz) {
			Ref<ScrollBar> bar = scroll->horz;
			if (bar.isNotNull()) {
				if (bar->isVisible()) {
					views[0] = bar;
				}
			}
		}
		if (scroll->flagValidVert) {
			Ref<ScrollBar> bar = scroll->vert;
			if (bar.isNotNull()) {
				if (bar->isVisible()) {
					views[1] = bar;
				}
			}
		}
	}
}

sl_bool View::isMultiTouchMode()
{
	return m_flagMultiTouchMode;
}

void View::setMultiTouchMode(sl_bool flag)
{
	m_flagMultiTouchMode = flag;
}

sl_bool View::isPassingEventsToChildren()
{
	return m_flagPassEventToChildren;
}

void View::setPassingEventsToChildren(sl_bool flag)
{
	m_flagPassEventToChildren = flag;
}

Ref<View> View::getNextFocusableView()
{
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return this;
	}
	{
		sl_size index = 0;
		ListLocker< Ref<View> > children(m_children);
		sl_size i;
		for (i = 0; i < children.count; i++) {
			if (children[i] == this) {
				index = i;
				break;
			}
		}
		for (i = index + 1; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				Ref<View> ret = child->getFirstFocusableDescendant();
				if (ret.isNotNull()) {
					return ret;
				}
			}
		}
	}
	Ref<View> ret = parent->getNextFocusableView();
	if (ret.isNotNull()) {
		return ret;
	}
	return this;
}

Ref<View> View::getPreviousFocusableView()
{
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return this;
	}
	{
		sl_size index = 0;
		ListLocker< Ref<View> > children(m_children);
		sl_size i;
		for (i = 0; i < children.count; i++) {
			if (children[i] == this) {
				index = i;
				break;
			}
		}
		for (i = index; i > 0; i--) {
			Ref<View> child = children[i-1];
			if (child.isNotNull()) {
				Ref<View> ret = child->getLastFocusableDescendant();
				if (ret.isNotNull()) {
					return ret;
				}
			}
		}
	}
	Ref<View> ret = parent->getPreviousFocusableView();
	if (ret.isNotNull()) {
		return ret;
	}
	return this;
}

Ref<View> View::getFirstFocusableDescendant()
{
	if (isFocusable()) {
		return this;
	}
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			Ref<View> v = child->getFirstFocusableDescendant();
			if (v.isNotNull()) {
				return v;
			}
		}
	}
	return Ref<View>::null();
}

Ref<View> View::getLastFocusableDescendant()
{
	if (isFocusable()) {
		return this;
	}
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = children.count; i > 0; i--) {
		Ref<View> child = children[i - 1];
		if (child.isNotNull()) {
			Ref<View> v = child->getLastFocusableDescendant();
			if (v.isNotNull()) {
				return v;
			}
		}
	}
	return Ref<View>::null();
}

sl_bool View::isProcessingTabStop()
{
	return m_flagProcessingTabStop;
}

void View::setProcessingTabStop(sl_bool flag)
{
	m_flagProcessingTabStop = flag;
}

Ref<View> View::getNextTabStop()
{
	Ref<View> view = m_viewNextTabStop;
	if (view.isNotNull()) {
		return view;
	}
	return getNextFocusableView();
}

void View::setNextTabStop(const Ref<View>& view)
{
	m_viewNextTabStop = view;
}

Ref<View> View::getPreviousTabStop()
{
	Ref<View> view = m_viewPrevTabStop;
	if (view.isNotNull()) {
		return view;
	}
	return getPreviousFocusableView();
}

void View::setPreviousTabStop(const Ref<View>& view)
{
	m_viewPrevTabStop = view;
}

void View::draw(Canvas *canvas)
{
	sl_ui_pos width = m_frame.getWidth();
	if (width <= 0) {
		return;
	}
	sl_ui_pos height = m_frame.getHeight();
	if (height <= 0) {
		return;
	}
	
	Ref<DrawAttributes> attr = m_draw;
	Ref<ScrollAttributes> scroll = m_scroll;
	
	_makeLayout(sl_false);

	if (attr.isNotNull()) {
		if (attr->flagPreDrawEnabled) {
			onPreDraw(canvas);
		}
		if (attr->flagOnDrawBackgroundAlways || attr->backgroundColor.isNotZero() || attr->background.isNotNull()) {
			onDrawBackground(canvas);
		}
	}
	
	do {
		CanvasStatusScope scopeClip;
		if (attr.isNull() || attr->flagClippingBounds) {
			Rectangle rcClip(0, 0, (sl_real)width, (sl_real)height);
			switch (getBoundShape()) {
				case BoundShape::RoundRect:
					rcClip.right -= 1;
					rcClip.bottom -= 1;
					rcClip.fixSizeError();
					scopeClip.save(canvas);
					canvas->clipToRoundRect(rcClip, getRoundRectBoundShapeRadius());
					break;
				case BoundShape::Ellipse:
					rcClip.right -= 1;
					rcClip.bottom -= 1;
					rcClip.fixSizeError();
					scopeClip.save(canvas);
					canvas->clipToEllipse(rcClip);
					break;
				case BoundShape::Path:
					scopeClip.save(canvas);
					canvas->clipToPath(getBoundShapePath());
					break;
				case BoundShape::Rectangle:
				default:
					if (!(isInstance())) {
						scopeClip.save(canvas);
						canvas->clipToRectangle(rcClip);
					}
					break;
			}
			rcClip = canvas->getClipBounds();
			rcClip.fixSizeError();
			if (Math::isAlmostZero(rcClip.getWidth()) || Math::isAlmostZero(rcClip.getHeight())) {
				break;
			}
		}
		
		Ref<View> scrollBars[2];
		if (scroll.isNotNull()) {
			_getScrollBars(scrollBars);
		}

		sl_bool flagSaveStatus = sl_false;
		if (m_children.getCount() > 0) {
			flagSaveStatus = sl_true;
		} else if (scrollBars[0].isNotNull() || scrollBars[1].isNotNull()) {
			flagSaveStatus = sl_true;
		} else if (attr.isNotNull() && attr->penBorder.isNotNull() && scopeClip.getCanvas().isNull()) {
			flagSaveStatus = sl_true;
		}
		
		{
			CanvasStatusScope scopeContent(flagSaveStatus ? canvas : sl_null);
			if (scroll.isNotNull()) {
				sl_real scrollX = (sl_real)(scroll->x);
				sl_real scrollY = (sl_real)(scroll->y);
				if(!(Math::isAlmostZero(scrollX)) || !(Math::isAlmostZero(scrollY))) {
					canvas->translate(-scrollX, -scrollY);
				}
			}
			onDraw(canvas);
		}
		if (m_children.getCount() > 0) {
			onDrawChildren(canvas);
		}
		if (scrollBars[0].isNotNull() || scrollBars[1].isNotNull()) {
			drawChildren(canvas, scrollBars, 2);
		}
		
	} while (0);
	
	
	if (attr.isNotNull()) {
		if (attr->flagOnDrawBorderAlways || attr->penBorder.isNotNull()) {
			onDrawBorder(canvas);
		}
		if (attr->flagPostDrawEnabled) {
			onPostDraw(canvas);
		}
	}

}

void View::drawBackground(Canvas* canvas, const Color& color, const Ref<Drawable>& background)
{
	Rectangle rc(0, 0, (sl_real)(m_frame.getWidth()), (sl_real)(m_frame.getHeight()));
	if (color.a > 0) {
		Ref<Brush> brush = Brush::createSolidBrush(color);
		canvas->fillRectangle(rc, brush);
	}
	if (background.isNotNull()) {
		sl_real width = background->getDrawableWidth();
		sl_real height = background->getDrawableHeight();
		Rectangle rcSrc(0, 0, width, height);
		canvas->draw(rc, background, getBackgroundScaleMode(), getBackgroundAlignment());
	}
}

void View::drawBorder(Canvas* canvas, const Ref<Pen>& pen)
{
	UIRect rc(0, 0, m_frame.getWidth(), m_frame.getHeight());
	if (pen.isNotNull()) {
		switch (getBoundShape()) {
			case BoundShape::RoundRect:
				rc.left += 1;
				rc.top += 1;
				rc.right -= 2;
				rc.bottom -= 2;
				rc.fixSizeError();
				canvas->drawRoundRect(rc, getRoundRectBoundShapeRadius(), pen);
				break;
			case BoundShape::Ellipse:
				rc.left += 1;
				rc.top += 1;
				rc.right -= 2;
				rc.bottom -= 2;
				rc.fixSizeError();
				canvas->drawEllipse(rc, pen);
				break;
			case BoundShape::Path:
				canvas->drawPath(getBoundShapePath(), pen);
				break;
			case BoundShape::Rectangle:
			default:
				rc.right -= 1;
				rc.bottom -= 1;
				rc.fixSizeError();
				sl_bool flagAntiAlias = canvas->isAntiAlias();
				canvas->setAntiAlias(sl_false);
				canvas->drawRectangle(rc, pen);
				canvas->setAntiAlias(flagAntiAlias);
				break;
		}
	}
}

void View::drawChildren(Canvas* canvas, const Ref<View>* children, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		View* child = children[i].ptr;
		if (child) {
			if (child->isVisible()) {
				if (!(child->isInstance())) {
					drawChild(canvas, child);
				}
			}
		}
	}
}

void View::drawChild(Canvas* canvas, View* child)
{
	if (child) {
		CanvasStatusScope scope(canvas);
		canvas->translate((sl_real)(child->m_frame.left), (sl_real)(child->m_frame.top));
		if (child->isTransformEnabled()) {
			canvas->concatMatrix(child->getTransform());
		}
		child->dispatchDraw(canvas);
	}
}

void View::onDraw(Canvas* canvas)
{
}

void View::onPreDraw(Canvas* canvas)
{
}

void View::onPostDraw(Canvas* canvas)
{
}

void View::onDrawChildren(Canvas* canvas)
{
	ListLocker< Ref<View> > children(m_children);
	drawChildren(canvas, children.data, children.count);
}

void View::onDrawBackground(Canvas* canvas)
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		drawBackground(canvas, draw->backgroundColor, draw->background);
	}
}

void View::onDrawBorder(Canvas* canvas)
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		drawBorder(canvas, draw->penBorder);
	}
}

void View::onClick(UIEvent* ev)
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

void View::onResize(sl_ui_len width, sl_ui_len height)
{
}

void View::onResizeChild(View* child, sl_ui_len width, sl_ui_len height)
{
}

void View::onChangeVisibility(Visibility oldVisibility, Visibility newVisibility)
{
}

void View::onChangeVisibilityOfChild(View* child, Visibility oldVisibility, Visibility newVisibility)
{
}

void View::onScroll(sl_scroll_pos x, sl_scroll_pos y)
{
}

void View::onResizeContent(sl_scroll_pos width, sl_scroll_pos height)
{
}

void View::onAddChild(View* child)
{
}

void View::onRemoveChild(View* child)
{
}

void View::onAttach()
{
}

void View::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	measureRelativeLayout(flagHorizontal, flagVertical);
}

void View::onPrepareLayout(ViewPrepareLayoutParam& param)
{
}

void View::onMakeLayout()
{
}

void View::onChangePadding()
{
}

void View::dispatchDraw(Canvas* canvas)
{
	sl_ui_pos width = getWidth();
	if (width <= 0) {
		return;
	}
	sl_ui_pos height = getHeight();
	if (height <= 0) {
		return;
	}

	Ref<GraphicsContext> context = canvas->getGraphicsContext();
	m_graphicsContext = context;
	
	if (!(canvas->isBuffer())) {
		if (context.isNull()) {
			return;
		}
		if (isDoubleBuffering()) {
			_initializeDraw();
			Ref<DrawAttributes> attr = m_draw;
			if (attr.isNotNull() && attr->flagDoubleBuffer) {
				UIRect region = canvas->getClipBounds();
				if (!(region.intersectRectangle(getBounds(), &region))) {
					return;
				}
				Ref<Bitmap> bitmapBuffer = attr->bitmapDoubleBuffer;
				Ref<Canvas> canvasBuffer = attr->canvasDoubleBuffer;
				sl_uint32 iwidth = (sl_uint32)width;
				sl_uint32 iheight = (sl_uint32)height;
				
				if (bitmapBuffer.isNull() || canvasBuffer.isNull() || bitmapBuffer->getWidth() < iwidth || bitmapBuffer->getHeight() < iheight) {
					bitmapBuffer = context->createBitmap((iwidth + 255) & 0xFFFFFF00, (iheight + 255) & 0xFFFFFF00);
					if (bitmapBuffer.isNull()) {
						return;
					}
					canvasBuffer = bitmapBuffer->getCanvas();
					if (canvasBuffer.isNull()) {
						return;
					}
					attr->bitmapDoubleBuffer = bitmapBuffer;
					attr->canvasDoubleBuffer = canvasBuffer;
				}
				{
					Color colorClear = Color::zero();
					do {
						if (m_flagOpaque) {
							break;
						}
						Color color = attr->backgroundColor;
						if (color.a == 255) {
							break;
						}
						Ref<Window> window = m_window;
						if (window.isNotNull() && window->getContentView() == this) {
							colorClear = window->getBackgroundColor();
							if (colorClear.a < 255) {
								Color c = Color::White;
								c.blend_PA_NPA(colorClear);
								colorClear = c;
							}
							break;
						}
						colorClear = Color::White;
					} while (0);
					if (colorClear.isNotZero()) {
						bitmapBuffer->resetPixels((sl_uint32)(region.left), (sl_uint32)(region.top), (sl_uint32)(region.getWidth()), (sl_uint32)(region.getHeight()), colorClear);
					}
					CanvasStatusScope scope(canvasBuffer);
					canvasBuffer->clipToRectangle(region);
					draw(canvasBuffer.ptr);
				}
				canvas->draw(region, bitmapBuffer, region);
				return;
			}
		}
	}
	draw(canvas);
}

static UIAction _SView_getActionUp(UIAction actionDown)
{
	if (actionDown == UIAction::LeftButtonDown) {
		return UIAction::LeftButtonUp;
	} else if (actionDown == UIAction::RightButtonDown) {
		return UIAction::RightButtonUp;
	} else if (actionDown == UIAction::MiddleButtonDown) {
		return UIAction::MiddleButtonUp;
	}
	return UIAction::Unknown;
}

#define POINT_EVENT_CHECK_CHILD(c) (c && !(c->isInstance()) && c->isVisible() && c->isHitTestable())

void View::dispatchMouseEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	if (! m_flagEnabled) {
		return;
	}
	
	UIAction action = ev->getAction();
	
	// pass event to children
	{
		Ref<View> oldChildMouseMove;
		if (action == UIAction::MouseMove || action == UIAction::MouseEnter) {
			oldChildMouseMove = m_childMouseMove;
		}
		Ref<View> scrollBars[2];
		_getScrollBars(scrollBars);
		if (!(dispatchMouseEventToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				if (children.count > 0) {
					if (dispatchMouseEventToChildren(ev, children.data, children.count)) {
						oldChildMouseMove.setNull();
					}
				}
			}
		} else {
			oldChildMouseMove.setNull();
		}
		if (action == UIAction::MouseMove || action == UIAction::MouseEnter) {
			if (oldChildMouseMove.isNotNull()) {
				sl_bool flagSP = ev->isStoppedPropagation();
				UIAction action = ev->getAction();
				ev->setAction(UIAction::MouseLeave);
				dispatchMouseEventToChild(ev, oldChildMouseMove.ptr);
				ev->setAction(action);
				ev->setStoppedPropagation(flagSP);
			}
		}
	}
	
	if (ev->isStoppedPropagation()) {
		return;
	}

	if (m_flagFocusable) {
		if (action == UIAction::LeftButtonDown || action == UIAction::RightButtonDown || action == UIAction::MiddleButtonDown) {
			setFocus();
		}
	}
	
	ev->resetStatus();
	
	onMouseEvent(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMouseEvent(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
	}
	
	if (isContentScrollingByMouse()) {
		_processContentScrollingEvents(ev);
	}
	
	_processEventForStateAndClick(ev);
	
}

sl_bool View::dispatchMouseEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count)
{
	UIAction action = ev->getAction();
	UIPointf ptMouse = ev->getPoint();
	
	Ref<View> oldChild;
	switch (action) {
		case UIAction::LeftButtonDown:
		case UIAction::RightButtonDown:
		case UIAction::MiddleButtonDown:
			oldChild = m_childMouseDown;
			if (oldChild.isNotNull()) {
				dispatchMouseEventToChild(ev, oldChild.ptr);
				return sl_true;
			}
			for (sl_size i = 0; i < count; i++) {
				View* child = children[count - 1 - i].ptr;
				if (POINT_EVENT_CHECK_CHILD(child)) {
					UIPointf pt = child->convertCoordinateFromParent(ptMouse);
					if (child->hitTest(pt)) {
						ev->setPoint(pt);
						dispatchMouseEventToChild(ev, child, sl_false);
						ev->setPoint(ptMouse);
						if (!(ev->isPassedToNext())) {
							m_childMouseDown = child;
							m_actionMouseDown = action;
							return sl_true;
						}
					}
				}
			}
			break;
		case UIAction::LeftButtonDrag:
		case UIAction::RightButtonDrag:
		case UIAction::MiddleButtonDrag:
			oldChild = m_childMouseDown;
			if (oldChild.isNotNull()) {
				dispatchMouseEventToChild(ev, oldChild.ptr);
			}
			return sl_true;
		case UIAction::LeftButtonDoubleClick:
		case UIAction::RightButtonDoubleClick:
		case UIAction::MiddleButtonDoubleClick:
			for (sl_size i = 0; i < count; i++) {
				View* child = children[count - 1 - i].ptr;
				if (POINT_EVENT_CHECK_CHILD(child)) {
					UIPointf pt = child->convertCoordinateFromParent(ptMouse);
					if (child->hitTest(pt)) {
						ev->setPoint(pt);
						dispatchMouseEventToChild(ev, child, sl_false);
						ev->setPoint(ptMouse);
						if (!(ev->isPassedToNext())) {
							return sl_true;
						}
					}
				}
			}
			break;
		case UIAction::LeftButtonUp:
		case UIAction::RightButtonUp:
		case UIAction::MiddleButtonUp:
			oldChild = m_childMouseDown;
			if (oldChild.isNotNull()) {
				dispatchMouseEventToChild(ev, oldChild.ptr);
				if (action == _SView_getActionUp(m_actionMouseDown)) {
					m_childMouseDown.setNull();
					m_actionMouseDown = UIAction::Unknown;
				}
			}
			return sl_true;
		case UIAction::MouseMove:
		case UIAction::MouseEnter:
			oldChild = m_childMouseMove;
			for (sl_size i = 0; i < count; i++) {
				View* child = children[count - 1 - i].ptr;
				if (POINT_EVENT_CHECK_CHILD(child)) {
					UIPointf pt = child->convertCoordinateFromParent(ptMouse);
					if (child->hitTest(pt)) {
						if (oldChild == child) {
							ev->setAction(UIAction::MouseMove);
						} else {
							ev->setAction(UIAction::MouseEnter);
						}
						ev->setPoint(pt);
						dispatchMouseEventToChild(ev, child, sl_false);
						ev->setPoint(ptMouse);
						ev->setAction(action);
						if (!(ev->isPassedToNext())) {
							m_childMouseMove = child;
							if (oldChild.isNotNull() && oldChild != child) {
								ev->setAction(UIAction::MouseLeave);
								dispatchMouseEventToChild(ev, oldChild.ptr);
								ev->setAction(action);
							}
							return sl_true;
						}
					}
				}
			}
			break;
		case UIAction::MouseLeave:
			oldChild = m_childMouseMove;
			if (oldChild.isNotNull()) {
				dispatchMouseEventToChild(ev, oldChild.ptr);
				m_childMouseMove.setNull();
			}
			return sl_true;
		default:
			return sl_true;
	}
	return sl_false;
}

void View::dispatchMouseEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints)
{
	if (child) {
		ev->resetStatus();
		if (flagTransformPoints) {
			UIPointf ptMouse = ev->getPoint();
			ev->setPoint(child->convertCoordinateFromParent(ptMouse));
			child->dispatchMouseEvent(ev);
			ev->setPoint(ptMouse);
		} else {
			child->dispatchMouseEvent(ev);
		}
	}
}

void View::dispatchTouchEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	if (! m_flagEnabled) {
		return;
	}
	
	UIAction action = ev->getAction();
	
	// pass event to children
	{
		Ref<View> scrollBars[2];
		_getScrollBars(scrollBars);
		if (!(dispatchTouchEventToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				if (children.count > 0) {
					if (m_flagMultiTouchMode) {
						dispatchMultiTouchEventToChildren(ev, children.data, children.count);
					} else {
						dispatchTouchEventToChildren(ev, children.data, children.count);
					}
				}
			}
		}
	}
	
	if (ev->isStoppedPropagation()) {
		return;
	}
	
	if (m_flagFocusable) {
		if (action == UIAction::TouchBegin) {
			setFocus();
		}
	}
	
	ev->resetStatus();
	
	onTouchEvent(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	
	onMouseEvent(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onTouchEvent(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
		
		listener->onMouseEvent(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
	}
	
	_processEventForStateAndClick(ev);
	
	if (isContentScrollingByTouch()) {
		_processContentScrollingEvents(ev);
	}

}

sl_bool View::dispatchTouchEventToChildren(UIEvent *ev, const Ref<View>* children, sl_size count)
{
	UIAction action = ev->getAction();
	UIPointf ptMouse = ev->getPoint();
	
	Ref<View> oldChild;
	switch (action) {
		case UIAction::TouchBegin:
			oldChild = m_childMouseDown;
			if (oldChild.isNotNull()) {
				dispatchTouchEventToChild(ev, oldChild.ptr);
				return sl_true;
			}
			for (sl_size i = 0; i < count; i++) {
				View* child = children[count - 1 - i].ptr;
				if (POINT_EVENT_CHECK_CHILD(child)) {
					UIPointf pt = child->convertCoordinateFromParent(ptMouse);
					if (child->hitTest(pt)) {
						dispatchTouchEventToChild(ev, child);
						if (!(ev->isPassedToNext())) {
							m_childMouseDown = child;
							m_actionMouseDown = action;
							break;
						}
					}
				}
			}
			break;
		case UIAction::TouchMove:
			oldChild = m_childMouseDown;
			if (oldChild.isNotNull()) {
				dispatchTouchEventToChild(ev, oldChild.ptr);
				return sl_true;
			}
		case UIAction::TouchEnd:
		case UIAction::TouchCancel:
			oldChild = m_childMouseDown;
			if (oldChild.isNotNull()) {
				dispatchTouchEventToChild(ev, oldChild.ptr);
				if (m_actionMouseDown == UIAction::TouchBegin) {
					m_childMouseDown.setNull();
					m_actionMouseDown = UIAction::Unknown;
				}
				return sl_true;
			}
		default:
			return sl_true;
	}
	return sl_false;
}

void View::dispatchMultiTouchEventToChildren(UIEvent *ev, const Ref<View>* children, sl_size count)
{
	UIAction action = ev->getAction();
	
	Array<TouchPoint> ptsOriginal = ev->getTouchPoints();
	sl_size nTouch = ptsOriginal.getCount();
	TouchPoint ptOriginal = ev->getTouchPoint();
	
	List< Ref<View> > selectedChildren;
	
	if (action != UIAction::TouchCancel) {
		
		if (nTouch > 0) {
			
			Array<TouchPoint> _ptsCheck = Array<TouchPoint>::create(nTouch);
			Array<TouchPoint> _ptsInside = Array<TouchPoint>::create(nTouch);
			Array<TouchPoint> _ptsOutside = Array<TouchPoint>::create(nTouch);
			
			if (_ptsCheck.isNotNull() && _ptsInside.isNotNull() && _ptsOutside.isNotNull()) {
				
				_ptsCheck.copy(ptsOriginal);
				
				TouchPoint* ptsCheck = _ptsCheck.getData();
				TouchPoint* ptsInside = _ptsInside.getData();
				TouchPoint* ptsOutside = _ptsOutside.getData();
				
				sl_size nCheck = nTouch;
				sl_size k;
				
				for (sl_size i = 0; i < count && nCheck > 0; i++) {
					View* child = children[count - 1 - i].ptr;
					if (POINT_EVENT_CHECK_CHILD(child)) {
						sl_size nInside = 0;
						sl_size nOutside = 0;
						for (k = 0; k < nCheck; k++) {
							UIPointf pt = child->convertCoordinateFromParent(ptsCheck[k].point);
							if (child->hitTest(pt)) {
								ptsInside[nInside] = ptsCheck[k];
								ptsInside[nInside].point = pt;
								nInside++;
							} else {
								ptsOutside[nOutside] = ptsCheck[k];
								nOutside++;
							}
						}
						if (nInside > 0) {
							if (nInside == 1) {
								if (ptsInside[0].phase == TouchPhase::Begin) {
									ev->setAction(UIAction::TouchBegin);
								} else if (ptsInside[0].phase == TouchPhase::End) {
									ev->setAction(UIAction::TouchEnd);
								} else if (ptsInside[0].phase == TouchPhase::Cancel) {
									ev->setAction(UIAction::TouchCancel);
								}
							}
							ev->setTouchPoints(_ptsInside.sub(0, nInside));
							ev->setTouchPoint(ptsInside[0]);
							dispatchTouchEventToChild(ev, child, sl_false);
							ev->setAction(action);
							if (!(ev->isPassedToNext())) {
								selectedChildren.add_NoLock(child);
								nCheck = nOutside;
								for (k = 0; k < nCheck; k++) {
									ptsCheck[k] = ptsOutside[k];
								}
							}
						}
					}
				}
			}
		}
		
	}
	
	// dispatch cancel events
	{
		List< Ref<View> > old = m_childrenMultiTouch;
		{
			ListLocker< Ref<View> > list(selectedChildren);
			for (sl_size i = 0; i < list.count; i++) {
				old.removeValue(list[i]);
			}
		}
		Array<TouchPoint> arr = ptsOriginal.sub(0, 1);
		if (arr.isNotNull()) {
			ListLocker< Ref<View> > list(old);
			for (sl_size i = 0; i < list.count; i++) {
				sl_bool flagSP = ev->isStoppedPropagation();
				UIAction action = ev->getAction();
				ev->setAction(UIAction::TouchCancel);
				ev->setTouchPoints(arr);
				dispatchTouchEventToChild(ev, list[i].ptr);
				ev->setAction(action);
				ev->setStoppedPropagation(flagSP);
			}
		}
	}
	
	ev->setTouchPoint(ptOriginal);
	ev->setTouchPoints(ptsOriginal);
	
	m_childrenMultiTouch = selectedChildren;
	
}

void View::dispatchTouchEventToChild(UIEvent* ev, View* child, sl_bool flagTranformPoints)
{
	if (child) {
		
		ev->resetStatus();
		
		if (flagTranformPoints) {
			
			Array<TouchPoint> arr = ev->getTouchPoints();
			Array<TouchPoint> arrConverted = arr.duplicate();
			sl_size n = arrConverted.getCount();
			if (n == 0) {
				return;
			}
			TouchPoint* pts = arr.getData();
			TouchPoint* ptsConverted = arrConverted.getData();
			for (sl_size i = 0; i < n; i++) {
				ptsConverted[i].point = child->convertCoordinateFromParent(pts[i].point);
			}
			ev->setTouchPoints(arrConverted);
			
			TouchPoint ptTouch = ev->getTouchPoint();
			TouchPoint ptTouchConverted = ptTouch;
			ptTouchConverted.point = child->convertCoordinateFromParent(ptTouch.point);
			ev->setTouchPoint(ptTouchConverted);
			
			child->dispatchTouchEvent(ev);
			
			ev->setTouchPoints(arr);
			ev->setTouchPoint(ptTouch);
			
		} else {
			
			child->dispatchTouchEvent(ev);
			
		}
		
	}
}

void View::dispatchMouseWheelEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	if (! m_flagEnabled) {
		return;
	}
	
	// pass event to children
	{
		Ref<View> scrollBars[2];
		_getScrollBars(scrollBars);
		if (!(dispatchMouseWheelEventToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				if (children.count > 0) {
					dispatchMouseWheelEventToChildren(ev, children.data, children.count);
				}
			}
		}
	}
	
	if (ev->isStoppedPropagation()) {
		return;
	}
	
	ev->resetStatus();
	
	onMouseWheelEvent(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onMouseWheelEvent(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
	}
	
	if (isContentScrollingByMouseWheel()) {
		_processContentScrollingEvents(ev);
	}
	
}

sl_bool View::dispatchMouseWheelEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count)
{
	UIAction action = ev->getAction();
	if (action != UIAction::MouseWheel) {
		return sl_true;
	}
	UIPointf ptMouse = ev->getPoint();
	for (sl_size i = 0; i < count; i++) {
		View* child = children[count - 1 - i].ptr;
		if (POINT_EVENT_CHECK_CHILD(child)) {
			UIPointf pt = child->convertCoordinateFromParent(ptMouse);
			if (child->hitTest(pt)) {
				ev->setPoint(pt);
				dispatchMouseWheelEventToChild(ev, child, sl_false);
				ev->setPoint(ptMouse);
				if (!(ev->isPassedToNext())) {
					return sl_true;
				}
			}
		}
	}
	return sl_false;
}

void View::dispatchMouseWheelEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints)
{
	if (child) {
		ev->resetStatus();
		if (flagTransformPoints) {
			UIPointf ptMouse = ev->getPoint();
			ev->setPoint(child->convertCoordinateFromParent(ptMouse));
			child->dispatchMouseWheelEvent(ev);
			ev->setPoint(ptMouse);
		} else {
			child->dispatchMouseWheelEvent(ev);
		}
	}
}

void View::dispatchKeyEvent(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	if (! m_flagEnabled) {
		return;
	}
	
	Ref<View> viewFocusedChild = getFocusedChild();
	if (viewFocusedChild.isNotNull()) {
		viewFocusedChild->dispatchKeyEvent(ev);
	}
	
	if (ev->isStoppedPropagation()) {
		return;
	}
	
	ev->resetStatus();
	
	onKeyEvent(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onKeyEvent(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
	}
	
	if (isContentScrollingByKeyboard()) {
		_processContentScrollingEvents(ev);
	}
	
	if (isProcessingTabStop()) {
		Keycode key = ev->getKeycode();
		if (key == Keycode::Tab) {
			if (ev->isShiftKey()) {
				Ref<View> v = getPreviousTabStop();
				if (v.isNotNull() && v != this) {
					v->setFocus();
					ev->stopPropagation();
				}
			} else {
				Ref<View> v = getNextTabStop();
				if (v.isNotNull() && v != this) {
					v->setFocus();
					ev->stopPropagation();
				}
			}
		}
	}
}

void View::dispatchClick(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	if (! m_flagEnabled) {
		return;
	}
	onClick(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onClick(this, ev);
	}
	Ref<Runnable> onClick = getOnClick();
	if (onClick.isNotNull()) {
		onClick->run();
	}
}

void View::dispatchClickWithNoEvent()
{
	Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::Unknown, 0, 0);
	if (ev.isNotNull()) {
		dispatchClick(ev.ptr);		
	}
}

void View::dispatchSetCursor(UIEvent* ev)
{
	if (!ev) {
		return;
	}
	if (! m_flagEnabled) {
		return;
	}
	
	// pass event to children
	{
		Ref<View> scrollBars[2];
		_getScrollBars(scrollBars);
		if (!(dispatchSetCursorToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				if (children.count > 0) {
					dispatchSetCursorToChildren(ev, children.data, children.count);
				}
			}
		}
	}
	
	if (ev->isStoppedPropagation()) {
		return;
	}
	if (ev->isPreventedDefault()) {
		return;
	}
	
	ev->resetStatus();
	
	onSetCursor(ev);
	if (ev->isPreventedDefault()) {
		return;
	}
	
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onSetCursor(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
	}
	
	Ref<Cursor> cursor = getCursor();
	if (cursor.isNotNull()) {
		Cursor::setCurrent(cursor);
		ev->preventDefault();
	}
	
}

sl_bool View::dispatchSetCursorToChildren(UIEvent* ev, const Ref<View>* children, sl_size count)
{
	UIAction action = ev->getAction();
	if (action != UIAction::SetCursor) {
		return sl_true;
	}
	UIPointf ptMouse = ev->getPoint();
	for (sl_size i = 0; i < count; i++) {
		View* child = children[count - 1 - i].ptr;
		if (POINT_EVENT_CHECK_CHILD(child)) {
			UIPointf pt = child->convertCoordinateFromParent(ptMouse);
			if (child->hitTest(pt)) {
				ev->setPoint(pt);
				dispatchSetCursorToChild(ev, child, sl_false);
				ev->setPoint(ptMouse);
				if (!(ev->isPassedToNext())) {
					return sl_true;
				}
			}
		}
	}
	return sl_false;
}

void View::dispatchSetCursorToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints)
{
	if (child) {
		ev->resetStatus();
		if (flagTransformPoints) {
			UIPointf ptMouse = ev->getPoint();
			ev->setPoint(child->convertCoordinateFromParent(ptMouse));
			child->dispatchSetCursor(ev);
			ev->setPoint(ptMouse);
		} else {
			child->dispatchSetCursor(ev);
		}
	}
}

void View::dispatchResize(sl_ui_len width, sl_ui_len height)
{
	_refreshScroll(sl_false);
	onResize(width, height);
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onResize(this, width, height);
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parent->onResizeChild(this, width, height);
	}
}

void View::dispatchChangeVisibility(Visibility oldVisibility, Visibility newVisibility)
{
	onChangeVisibility(oldVisibility, newVisibility);
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onChangeVisibility(this, oldVisibility, newVisibility);
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parent->onChangeVisibilityOfChild(this, oldVisibility, newVisibility);
	}
}

void View::dispatchScroll(sl_scroll_pos x, sl_scroll_pos y)
{
	onScroll(x, y);
}

void View::_processEventForStateAndClick(UIEvent* ev)
{
	UIAction action = ev->getAction();
	if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
		setDownState(sl_true);
		if (m_flagOccurringClick) {
			ev->preventDefault();
		}
	} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd) {
		if (m_flagOccurringClick) {
			if (isDownState()) {
				if (getBounds().containsPoint(ev->getPoint())) {
					dispatchClick(ev);
				}
			}
		}
		setDownState(sl_false);
	} else if (action == UIAction::TouchCancel) {
		setDownState(sl_false);
	} else if (action == UIAction::MouseEnter) {
		setHoverState(sl_true);
	} else if (action == UIAction::MouseLeave) {
		setHoverState(sl_false);
	}
}

void View::_processContentScrollingEvents(UIEvent* ev)
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNull()) {
		return;
	}
	if (scroll->contentWidth < (sl_scroll_pos)(getWidth()) && scroll->contentHeight < (sl_scroll_pos)(getHeight())) {
		return;
	}
	sl_scroll_pos lineX = (sl_scroll_pos)(getWidth() / 20);
	sl_scroll_pos lineY = (sl_scroll_pos)(getHeight() / 20);
	UIAction action = ev->getAction();
	switch (action) {
		case UIAction::LeftButtonDown:
		case UIAction::TouchBegin:
			scroll->flagDownContent = sl_true;
			scroll->scrollX_DownContent = scroll->x;
			scroll->scrollY_DownContent = scroll->y;
			scroll->mouseX_DownContent = ev->getX();
			scroll->mouseY_DownContent = ev->getY();
			ev->stopPropagation();
			break;
		case UIAction::LeftButtonDrag:
		case UIAction::TouchMove:
			if (scroll->flagDownContent) {
				sl_scroll_pos sx = scroll->scrollX_DownContent - (sl_scroll_pos)(ev->getX() - scroll->mouseX_DownContent);
				sl_scroll_pos sy = scroll->scrollY_DownContent - (sl_scroll_pos)(ev->getY() - scroll->mouseY_DownContent);
				scrollTo(sx, sy);
				ev->stopPropagation();
			}
			break;
		case UIAction::LeftButtonUp:
		case UIAction::TouchEnd:
			if (scroll->flagDownContent) {
				scroll->flagDownContent = sl_false;
				ev->stopPropagation();
			}
			break;
		case UIAction::MouseWheel:
			{
				sl_bool flagChange = sl_false;
				sl_scroll_pos sx = scroll->x;
				sl_scroll_pos sy = scroll->y;
				
				sl_real deltaX = ev->getDeltaX();
				if (deltaX > SLIB_EPSILON) {
					sx -= lineX;
					flagChange = sl_true;
				} else if (deltaX < -SLIB_EPSILON) {
					sx += lineX;
					flagChange = sl_true;
				}
				sl_real deltaY = ev->getDeltaY();
				if (deltaY > SLIB_EPSILON) {
					sy -= lineY;
					flagChange = sl_true;
				} else if (deltaY < -SLIB_EPSILON) {
					sy += lineY;
					flagChange = sl_true;
				}
				
				if (flagChange) {
					scrollTo(sx, sy);
					ev->stopPropagation();
				}
			}
			break;
		case UIAction::KeyDown:
			{
				sl_scroll_pos sx = scroll->x;
				sl_scroll_pos sy = scroll->y;
				Keycode key = ev->getKeycode();
				if (key == Keycode::Left) {
					scrollTo(sx - lineX, sy);
					ev->stopPropagation();
				} else if (key == Keycode::Right) {
					scrollTo(sx + lineX, sy);
					ev->stopPropagation();
				} else if (key == Keycode::Up) {
					scrollTo(sx, sy - lineY);
					ev->stopPropagation();
				} else if (key == Keycode::Down) {
					scrollTo(sx, sy + lineY);
					ev->stopPropagation();
				} else if (key == Keycode::PageUp) {
					scrollTo(sx, sy - (sl_scroll_pos)(getHeight()));
					ev->stopPropagation();
				} else if (key == Keycode::PageDown) {
					scrollTo(sx, sy + (sl_scroll_pos)(getHeight()));
					ev->stopPropagation();
				}
			}
			break;
		default:
			break;
	}
}

void View::_setBorder_NW(sl_bool flag)
{
}

void View::_setBackgroundColor_NW(const Color& color)
{
}

void View::_setFont_NW(const Ref<Font>& font)
{
}

/**********************
	ViewInstance
**********************/

SLIB_DEFINE_OBJECT(ViewInstance, Object)

ViewInstance::ViewInstance()
{
	m_flagNativeWidget = sl_false;
	m_flagWindowContent = sl_false;
}

Ref<View> ViewInstance::getView()
{
	return m_view;
}

void ViewInstance::setView(View* view)
{
	m_view = view;
}

sl_bool ViewInstance::isNativeWidget()
{
	return m_flagNativeWidget;
}

void ViewInstance::setNativeWidget(sl_bool flag)
{
	m_flagNativeWidget = flag;
}

sl_bool ViewInstance::isWindowContent()
{
	return m_flagWindowContent;
}

void ViewInstance::setWindowContent(sl_bool flag)
{
	m_flagWindowContent = flag;
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
		view->dispatchClickWithNoEvent();
	}
}

void ViewInstance::onKeyEvent(UIEvent* ev)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchKeyEvent(ev);
	}
}

void ViewInstance::onMouseEvent(UIEvent* ev)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchMouseEvent(ev);
	}
}

void ViewInstance::onTouchEvent(UIEvent* ev)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchTouchEvent(ev);
	}
}

void ViewInstance::onMouseWheelEvent(UIEvent* ev)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchMouseWheelEvent(ev);
	}
}

void ViewInstance::onSetCursor(UIEvent* ev)
{
	Ref<View> view = getView();
	if (view.isNotNull()) {
		view->dispatchSetCursor(ev);
	}
}

SLIB_DEFINE_OBJECT(ViewGroup, View)

ViewGroup::ViewGroup(sl_bool flagCreatingChildInstances)
{
	setCreatingChildInstances(flagCreatingChildInstances);
	setLayoutEnabled(sl_true, sl_false);
}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))
Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}
#endif

SLIB_UI_NAMESPACE_END
