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
	
	m_frame.left = 0;
	m_frame.top = 0;
	m_frame.right = 0;
	m_frame.bottom = 0;
	
	m_paddingLeft = 0;
	m_paddingTop = 0;
	m_paddingRight = 0;
	m_paddingBottom = 0;

	m_flagVisible = sl_true;
	m_flagEnabled = sl_true;
	m_flagOpaque = sl_false;
	
	m_flagHitTestable = sl_true;
	
	m_flagFocusable = sl_false;
	m_flagFocused = sl_false;
	
	m_flagDown = sl_false;
	m_flagHover = sl_false;
	m_flagOccurringClick = sl_true;
	
	m_flagPreDrawEnabled = sl_false;
	m_flagPostDrawEnabled = sl_false;
	m_flagClippingBounds = sl_true;
	m_flagDoubleBuffer = sl_false;
	
	m_actionMouseDown = UIAction::Unknown;
	m_flagMultiTouchMode = sl_false;
	m_flagPassEventToChildren = sl_true;
	
	m_flagProcessingTabStop = sl_true;
	
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
		if (isCreatingChildInstances() && !(isNativeWidget())) {
			ListLocker< Ref<View> > children(m_children);
			for (sl_size i = 0; i < children.count; i++) {
				Ref<View> child = children[i];
				if (child.isNotNull()) {
					attachChild(child);
				}
			}
		}
		onAttach();
	}
}

List< Ref<View> > View::getChildren()
{
	return m_children.duplicate();
}

void View::addChild(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNull()) {
		return;
	}
	view->setFocus(sl_false, sl_false);
	view->setParent(this);
	m_children.addIfNotExist(view);
	if (isInstance() && isCreatingChildInstances() && !(isNativeWidget())) {
		attachChild(view);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

void View::removeChild(const Ref<View>& view, sl_bool flagRedraw)
{
	if (view.isNotNull()) {
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
		if (flagRedraw) {
			invalidate();
		}
	}
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
			Ref<View> child = children[i];
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
	m_children.removeAll();
	if (flagRedraw) {
		invalidate();
	}
}

Ref<View> View::getChildAt(sl_real x, sl_real y)
{
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = children.count - 1, ii = 0; ii < children.count; i--, ii++) {
		Ref<View> child = children[i];
		if (child.isNotNull() && child->isVisible() && child->isHitTestable()) {
			Point pt = child->convertCoordinateFromParent(Point(x, y));
			if (child->hitTest(pt)) {
				return child;
			}
		}
	}
	return Ref<View>::null();
}

Ref<View> View::getChildAt(const Point& point)
{
	return getChildAt(point.x, point.y);
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

void View::_removeChild(const Ref<View>& view)
{
	if (view.isNotNull()) {
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

void View::invalidate(const Rectangle &rect)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->invalidate(rect);
	} else {
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->invalidate(getBoundsInParent(rect));
		}
	}
}

const Rectangle& View::getFrame()
{
	return m_frame;
}

Rectangle View::getInstanceFrame()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->getFrame();
	}
	return Rectangle::zero();
}

void View::setFrame(const Rectangle &_frame, sl_bool flagRedraw)
{
	Rectangle frameOld = m_frame;
	Size sizeOld = frameOld.getSize();
	Rectangle frame = _frame;
	if (frame.right < frame.left) {
		frame.right = frame.left;
	}
	if (frame.bottom < frame.top) {
		frame.bottom = frame.top;
	}
	Size sizeNew = frame.getSize();
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		m_frame = frame;
		instance->setFrame(frame);
	} else {
		if (Math::isNearZero(frameOld.left - frame.left) && Math::isNearZero(frameOld.top - frame.top) && Math::isNearZero(frameOld.right - frame.right) && Math::isNearZero(frameOld.bottom - frame.bottom)) {
			m_frame = frame;
			return;
		}
	}
	if (!(Math::isNearZero(sizeOld.x - sizeNew.x) && Math::isNearZero(sizeOld.y - sizeNew.y))) {
		dispatchResize(sizeNew.x, sizeNew.y);
	}
	if (instance.isNull()) {
		if (flagRedraw) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				Rectangle bounds = getBoundsInParent();
				m_frame = frame;
				bounds.mergeRectangle(getBoundsInParent());
				parent->invalidate(bounds);
			} else {
				m_frame = frame;
			}
		} else {
			m_frame = frame;
		}
	}
}

void View::setFrame(sl_real x, sl_real y, sl_real width, sl_real height, sl_bool flagRedraw)
{
	setFrame(Rectangle(x, y, x+width, y+height), flagRedraw);
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

void View::setSize(const Size& size, sl_bool flagRedraw)
{
	setFrame(m_frame.left, m_frame.top, size.x, size.y, flagRedraw);
}

void View::setSize(sl_real width, sl_real height, sl_bool flagRedraw)
{
	setFrame(m_frame.left, m_frame.top, width, height, flagRedraw);
}

sl_real View::getX()
{
	return m_frame.left;
}

sl_real View::getY()
{
	return m_frame.top;
}

void View::setX(sl_real x, sl_bool flagRedraw)
{
	setFrame(x, m_frame.top, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

void View::setY(sl_real y, sl_bool flagRedraw)
{
	setFrame(m_frame.left, y, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

Point View::getPosition()
{
	return m_frame.getLeftTop();
}

void View::setPosition(sl_real x, sl_real y, sl_bool flagRedraw)
{
	setFrame(x, y, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

void View::setPosition(const Point& point, sl_bool flagRedraw)
{
	setFrame(point.x, point.y, m_frame.getWidth(), m_frame.getHeight(), flagRedraw);
}

Rectangle View::getBounds()
{
	return Rectangle(0, 0, m_frame.getWidth(), m_frame.getHeight());
}

Rectangle View::getBoundsInnerPadding()
{
	Rectangle ret(m_paddingLeft, m_paddingTop, m_frame.getWidth() - m_paddingRight, m_frame.getHeight() - m_paddingBottom);
	if (ret.right < ret.left) {
		ret.right = ret.left;
	}
	if (ret.bottom < ret.top) {
		ret.bottom = ret.top;
	}
	return ret;
}

Rectangle View::getBoundsInParent()
{
	return getBoundsInParent(getBounds());
}

Rectangle View::getBoundsInParent(const Rectangle& boundsLocal)
{
	Point pts[4];
	boundsLocal.getCornerPoints(pts);
	for (int i = 0; i < 4; i++) {
		pts[i] = convertCoordinateToParent(pts[i]);
	}
	Rectangle rc;
	rc.setFromPoints(pts, 4);
	return rc;
}

sl_real View::getPaddingLeft()
{
	return m_paddingLeft;
}

void View::setPaddingLeft(sl_real paddingLeft, sl_bool flagRedraw)
{
	setPadding(paddingLeft, m_paddingTop, m_paddingRight, m_paddingBottom, flagRedraw);
}

sl_real View::getPaddingTop()
{
	return m_paddingTop;
}

void View::setPaddingTop(sl_real paddingTop, sl_bool flagRedraw)
{
	setPadding(m_paddingLeft, paddingTop, m_paddingRight, m_paddingBottom, flagRedraw);
}

sl_real View::getPaddingRight()
{
	return m_paddingRight;
}

void View::setPaddingRight(sl_real paddingRight, sl_bool flagRedraw)
{
	setPadding(m_paddingLeft, m_paddingTop, paddingRight, m_paddingBottom, flagRedraw);
}

sl_real View::getPaddingBottom()
{
	return m_paddingBottom;
}

void View::setPaddingBottom(sl_real paddingBottom, sl_bool flagRedraw)
{
	setPadding(m_paddingLeft, m_paddingTop, m_paddingRight, paddingBottom, flagRedraw);
}

void View::setPadding(sl_real left, sl_real top, sl_real right, sl_real bottom, sl_bool flagRedraw)
{
	m_paddingLeft = left;
	m_paddingTop = top;
	m_paddingRight = right;
	m_paddingBottom = bottom;
	if (flagRedraw) {
		invalidate();
	}
}

void View::setPadding(sl_real padding, sl_bool flagRedraw)
{
	setPadding(padding, padding, padding, padding, flagRedraw);
}

sl_bool View::isVisible()
{
	return m_flagVisible;
}

void View::setVisible(sl_bool flag, sl_bool flagRedraw)
{
	sl_bool flagOld = m_flagVisible;
	m_flagVisible = flag;
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setVisible(flag);
	}
	if (flagOld != flag) {
		if (flag) {
			dispatchShow();
		} else {
			dispatchHide();
		}
	}
	if (instance.isNull()) {
		if (flagRedraw) {
			invalidate();
		}
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

sl_bool View::hitTest(sl_real x, sl_real y)
{
	Rectangle rc(0, 0, getWidth(), getHeight());
	switch (getBoundShape()) {
		case BoundShape::RoundRect:
			GraphicsPath::containsPointInRoundRect(Point(x, y), rc, getRoundRectBoundShapeRadius());
			break;
		case BoundShape::Ellipse:
			GraphicsPath::containsPointInEllipse(Point(x, y), rc);
			break;
	}
	return rc.containsPoint(x, y);
}

sl_bool View::hitTest(const Point& point)
{
	return hitTest(point.x, point.y);
}

sl_bool View::isFocusable()
{
	return m_flagFocusable;
}

void View::setFocusable(sl_bool flagFocusable, sl_bool flagRedraw)
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
			if (parent->getFocusedChild() == this) {
				parent->m_childFocused.setNull();
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
	m_flagDown = flagState;
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool View::isHoverState()
{
	return m_flagHover;
}

void View::setHoverState(sl_bool flagState, sl_bool flagRedraw)
{
	m_flagHover = flagState;
	if (flagRedraw) {
		invalidate();
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

Point View::convertCoordinateFromScreen(const Point& ptScreen)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromScreenToView(ptScreen);
	}
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return ptScreen;
	}
	Point pt = parent->convertCoordinateFromScreen(ptScreen);
	return convertCoordinateFromParent(pt);
}

Point View::convertCoordinateToScreen(const Point& ptView)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromViewToScreen(ptView);
	}
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return ptView;
	}
	Point pt = convertCoordinateToParent(ptView);
	return parent->convertCoordinateToScreen(pt);
}

Point View::convertCoordinateFromParent(const Point& ptParent)
{
	if (isInstance()) {
		return ptParent - getPosition();
	}
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return ptParent;
	}
	
	Point pt = ptParent;
	pt.x -= getX();
	pt.y -= getY();
	Ref<TransformAttributes> transform = m_transform;
	if (transform.isNotNull() && transform->flagEnabled) {
		pt = transform->transformInverse.transformPosition(pt);
	}
	return pt;
}

Point View::convertCoordinateToParent(const Point& ptView)
{
	if (isInstance()) {
		return ptView + getPosition();
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
	pt.x += getX();
	pt.y += getY();
	Ref<ScrollAttributes> scroll = parent->m_scroll;
	if (scroll.isNotNull()) {
		pt.x -= scroll->x;
		pt.y -= scroll->y;
	}
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

Ref<Pen> View::getBorderPen()
{
	Ref<DrawAttributes> draw = m_draw;
	if (draw.isNotNull()) {
		return draw->penBorder;
	}
	return Ref<Pen>::null();
}

void View::setBorderPen(const Ref<Pen>& pen, sl_bool flagRedraw)
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
	return getBorderPen().isNotNull();
}

void View::setBorder(sl_bool flagBorder, sl_bool flagRedraw)
{
	if (flagBorder) {
		if (isBorder()) {
			return;
		}
		setBorderPen(Pen::getDefault(), sl_false);
	} else {
		if (isBorder()) {
			setBorderPen(Ref<Pen>::null(), sl_false);
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

void View::_initializeDraw()
{
	if (m_draw.isNotNull()) {
		return;
	}
	
	Ref<DrawAttributes> attr = new DrawAttributes;
	
	if (attr.isNotNull()) {
		
		attr->backgroundColor = Color::zero();
		
		attr->boundShape = BoundShape::Rectangle;
		
		attr->roundRectBoundShapeRadius.x = 5;
		attr->roundRectBoundShapeRadius.y = 5;
		
		attr->borderColor = Color::Black;
		attr->borderStyle = PenStyle::Solid;
		attr->borderWidth = 0;
		
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
		setBorderPen(pen, flagRedraw);
	}
}

sl_bool View::isPreDrawEnabled()
{
	return m_flagPreDrawEnabled;
}

void View::setPreDrawEnabled(sl_bool flag, sl_bool flagRedraw)
{
	m_flagPreDrawEnabled = flag;
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool View::isPostDrawEnabled()
{
	return m_flagPostDrawEnabled;
}

void View::setPostDrawEnabled(sl_bool flag, sl_bool flagRedraw)
{
	m_flagPostDrawEnabled = flag;
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool View::isClippingBounds()
{
	return m_flagClippingBounds;
}

void View::setClippingBounds(sl_bool flag)
{
	m_flagClippingBounds = flag;
}

sl_bool View::isDoubleBuffering()
{
	return m_flagDoubleBuffer;
}

void View::setDoubleBuffering(sl_bool flag, sl_bool flagRedraw)
{
	m_flagDoubleBuffer = flag;
	if (flagRedraw) {
		invalidate();
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

sl_real View::getScrollX()
{
	return getScrollPosition().x;
}

sl_real View::getScrollY()
{
	return getScrollPosition().y;
}

Point View::getScrollPosition()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return Point(scroll->x, scroll->y);
	}
	return Point::zero();
}

void View::scrollTo(sl_real x, sl_real y, sl_bool flagRedraw)
{
	_initializeScroll();
	
	Ref<ScrollAttributes> scroll = m_scroll;
	
	if (scroll.isNotNull()) {
		
		sl_real rx = scroll->contentWidth - getWidth();
		if (x > rx) {
			x = rx;
		}
		if (x < 0) {
			x = 0;
		}
		sl_real ry = scroll->contentHeight - getHeight();
		if (y > ry) {
			y = ry;
		}
		if (y < 0) {
			y = 0;
		}
		
		if (Math::isNearZero(scroll->x - x) && Math::isNearZero(scroll->y - y)) {
			scroll->x = x;
			scroll->y = y;
			return;
		}
		
		scroll->x = x;
		scroll->y = y;
		
		dispatchScroll(x, y);
		
		Ref<ScrollBar> bar;
		bar = scroll->horz;
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

void View::scrollTo(const Point& position, sl_bool flagRedraw)
{
	scrollTo(position.x, position.y, flagRedraw);
}

sl_real View::getContentWidth()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->contentWidth;
	}
	return 0;
}

sl_real View::getContentHeight()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->contentHeight;
	}
	return 0;
}

Size View::getContentSize()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return Size(scroll->contentWidth, scroll->contentHeight);
	}
	return Size::zero();
}

void View::setContentSize(sl_real width, sl_real height, sl_bool flagRefresh)
{
	_initializeScroll();
	
	Ref<ScrollAttributes> scroll = m_scroll;
	
	if (scroll.isNotNull()) {
		if (Math::isNearZero(width - scroll->contentWidth) && Math::isNearZero(width - scroll->contentWidth)) {
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

void View::setContentSize(const Size& size, sl_bool flagRefresh)
{
	setContentSize(size.x, size.y, flagRefresh);
}

void View::setContentWidth(sl_real width, sl_bool flagRefresh)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		setContentSize(width, scroll->contentHeight, flagRefresh);
	}
}

void View::setContentHeight(sl_real height, sl_bool flagRefresh)
{
	_initializeScroll();
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		setContentSize(scroll->contentWidth, height, flagRefresh);
	}
}

Size View::getScrollRange()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		Size ret(scroll->contentWidth - getWidth(), scroll->contentHeight - getHeight());
		if (ret.x < 0) {
			ret.x = 0;
		}
		if (ret.y < 0) {
			ret.y = 0;
		}
		return ret;
	}
	return Size::zero();
}

sl_real View::getScrollBarWidth()
{
	Ref<ScrollAttributes> scroll = m_scroll;
	if (scroll.isNotNull()) {
		return scroll->barWidth;
	}
	return 0;
}

void View::setScrollBarWidth(sl_real width, sl_bool flagRefresh)
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
	return sl_false;
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
	return sl_false;
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
	return sl_false;
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
	return sl_false;
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
		scroll->x = 0;
		scroll->y = 0;
		scroll->contentWidth = 0;
		scroll->contentHeight = 0;
		scroll->barWidth = 10;
		
		scroll->flagContentScrollingByMouse = sl_true;
		scroll->flagContentScrollingByTouch = sl_true;
		scroll->flagContentScrollingByMouseWheel = sl_true;
		scroll->flagContentScrollingByKeyboard = sl_true;
		scroll->flagDownContent = sl_false;
	}
	
}

#define COLOR_SCROLLBAR_BORDER Color(80, 80, 80)
#define COLOR_SCROLLBAR_BACK Color(255, 255, 255, 210)

class _View_HorzScrollBarHoverBackground : public Drawable
{
public:
	Ref<Pen> m_pen;
	Ref<Brush> m_brush;
	
public:
	_View_HorzScrollBarHoverBackground()
	{
		m_pen = Pen::createSolidPen(1, COLOR_SCROLLBAR_BORDER);
		m_brush = Brush::createSolidBrush(COLOR_SCROLLBAR_BACK);
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst)
	{
		canvas->fillRectangle(rectDst, m_brush);
		canvas->drawLine(rectDst.left, rectDst.top, rectDst.right, rectDst.top, m_pen);
	}
};

class _View_VertScrollBarHoverBackground : public Drawable
{
public:
	Ref<Pen> m_pen;
	Ref<Brush> m_brush;
	
public:
	_View_VertScrollBarHoverBackground()
	{
		m_pen = Pen::createSolidPen(1, COLOR_SCROLLBAR_BORDER);
		m_brush = Brush::createSolidBrush(COLOR_SCROLLBAR_BACK);
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst)
	{
		canvas->fillRectangle(rectDst, m_brush);
		canvas->drawLine(rectDst.left, rectDst.top, rectDst.left, rectDst.bottom, m_pen);
	}
};

Ref<ScrollBar> View::_createHorizontalScrollBar()
{
	Ref<ScrollBar> ret = new ScrollBar;
	if (ret.isNotNull()) {
		ret->setHoverBackground(new _View_HorzScrollBarHoverBackground);
		return ret;
	}
	return Ref<ScrollBar>::null();
}

Ref<ScrollBar> View::_createVerticalScrollBar()
{
	Ref<ScrollBar> ret = new ScrollBar;
	if (ret.isNotNull()) {
		ret->setHoverBackground(new _View_VertScrollBarHoverBackground);
		return ret;
	}
	return Ref<ScrollBar>::null();
}

class _View_ScrollBarListener : public IScrollBarListener
{
public:
	WeakRef<View> m_view;
	
public:
	_View_ScrollBarListener(View* view)
	{
		m_view = view;
	}
	
	// override
	void onScroll(ScrollBar* scrollBar, sl_real value)
	{
		Ref<View> view = m_view;
		if (view.isNull()) {
			return;
		}
		sl_real sx = 0;
		sl_real sy = 0;
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
		sl_real width = getWidth();
		sl_real height = getHeight();
		sl_bool flagVisibleHorz = sl_false;
		Ref<ScrollBar> barHorz = scroll->horz;
		if (barHorz.isNotNull()) {
			barHorz->setParent(this);
			barHorz->setMinimumValue(0, sl_false);
			barHorz->setMaximumValue(scroll->contentWidth, sl_false);
			barHorz->setPage(width, sl_false);
			barHorz->setValueOfOutRange(scroll->x, sl_false);
			barHorz->setFrame(Rectangle(0, height - scroll->barWidth, width, height), sl_false);
			if (barHorz->getListener().isNull()) {
				barHorz->setListener(new _View_ScrollBarListener(this));
			}
			if (barHorz->isVisible() && barHorz->isValid()) {
				flagVisibleHorz = sl_true;
			}
		}
		sl_bool flagVisibleVert = sl_false;
		Ref<ScrollBar> barVert = scroll->vert;
		if (barVert.isNotNull()) {
			barVert->setParent(this);
			barVert->setMinimumValue(0, sl_false);
			barVert->setMaximumValue(scroll->contentHeight, sl_false);
			barVert->setPage(height, sl_false);
			barVert->setValueOfOutRange(scroll->y, sl_false);
			barVert->setFrame(Rectangle(width - scroll->barWidth, 0, width, height), sl_false);
			if (barVert->getListener().isNull()) {
				barVert->setListener(new _View_ScrollBarListener(this));
			}
			if (barVert->isVisible() && barVert->isValid()) {
				flagVisibleVert = sl_true;
			}
		}
		if (flagVisibleHorz && flagVisibleVert) {
			barHorz->setFrame(Rectangle(0, height - scroll->barWidth, width - scroll->barWidth, height), sl_false);
		}
		scrollTo(scroll->x, scroll->y, sl_false);
		if (flagRedraw) {
			invalidate();
		}
	}
}

Ref<Font> View::getFont()
{
	Ref<Font> font = m_font;
	if (font.isNotNull()) {
		return font;
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		return parent->getFont();
	}
	return UI::getDefaultFont();
}

void View::setFont(const Ref<Font>& _font, sl_bool flagRedraw)
{
	m_font = _font;
	if (isNativeWidget()) {
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			_setFont_NW(font);
		}
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

void View::_setFontInstance(const Ref<FontInstance>& instance)
{
	m_fontInstance = instance;
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

Ref<View> View::getRootView()
{
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		return parent->getRootView();;
	}
	return this;
}

Ref<View> View::getNextFocusableView()
{
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return this;
	}
	{
		sl_size index = 0;
		ListLocker< Ref<View> > children(parent->m_children);
		sl_size i;
		for (i = 0; i < children.count; i++) {
			if (children[i] == this) {
				index = i;
				break;
			}
		}
		for (i = index + 1; i < children.count; i++) {
			Ref<View> child = children[i];
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
		ListLocker< Ref<View> > children(parent->m_children);
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
		Ref<View> child = children[i];
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
	Ref<DrawAttributes> attr = m_draw;
	Ref<ScrollAttributes> scroll = m_scroll;
	
	if (m_flagPreDrawEnabled) {
		onPreDraw(canvas);
	}

	if (attr.isNotNull()) {
		if (attr->backgroundColor.isNotZero() || attr->background.isNotNull()) {
			onDrawBackground(canvas);
		}
	}
	
	{
		CanvasStatusScope scopeClip;
		if (m_flagClippingBounds) {
			Rectangle rcClip(Point::zero(), getSize());
			switch (getBoundShape()) {
				case BoundShape::RoundRect:
					rcClip.right -= 1;
					rcClip.bottom -= 1;
					scopeClip.save(canvas);
					canvas->clipToRoundRect(rcClip, getRoundRectBoundShapeRadius());
					break;
				case BoundShape::Ellipse:
					rcClip.right -= 1;
					rcClip.bottom -= 1;
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
		}
		
		if (m_children.getCount() > 0) {
			{
				CanvasStatusScope scopeContent;
				if (scroll.isNotNull()) {
					if(!(Math::isNearZero(scroll->x)) || !(Math::isNearZero(scroll->y))) {
						canvas->translate(-(scroll->x), -(scroll->y));
					}
				}
				onDraw(canvas);
			}
			onDrawChildren(canvas);
		} else {
			if (scroll.isNotNull()) {
				if(!(Math::isNearZero(scroll->x)) || !(Math::isNearZero(scroll->y))) {
					canvas->translate(-(scroll->x), -(scroll->y));
				}
			}
			onDraw(canvas);
		}
		
	}
	
	// draw scrollbars
	if (scroll.isNotNull()) {
		Ref<View> bar = scroll->horz;
		if (bar.isNotNull()) {
			if (bar->isVisible()) {
				drawChild(canvas, bar.ptr);
			}
		}
		bar = scroll->vert;
		if (bar.isNotNull()) {
			if (bar->isVisible()) {
				drawChild(canvas, bar.ptr);
			}
		}
	}
	
	if (attr.isNotNull()) {
		if (attr->penBorder.isNotNull()) {
			onDrawBorder(canvas);
		}
	}

	if (m_flagPostDrawEnabled) {
		onPostDraw(canvas);
	}
}

void View::drawBackground(Canvas *canvas, const Color &color, const Ref<Drawable>& background)
{
	Rectangle rc(Point::zero(), getSize());
	if (color.a > 0) {
		Ref<Brush> brush = Brush::createSolidBrush(color);
		canvas->fillRectangle(rc, brush);
	}
	if (background.isNotNull()) {
		sl_real width = background->getDrawableWidth();
		sl_real height = background->getDrawableHeight();
		Rectangle rcSrc(0, 0, width, height);
		canvas->draw(rc, background, rcSrc);
	}
}

void View::drawBorder(Canvas* canvas, const Ref<Pen>& pen)
{
	Rectangle rc(Point::zero(), getSize());
	if (pen.isNotNull()) {
		switch (getBoundShape()) {
			case BoundShape::RoundRect:
				rc.left += 1;
				rc.top += 1;
				rc.right -= 2;
				rc.bottom -= 2;
				canvas->drawRoundRect(rc, getRoundRectBoundShapeRadius(), pen);
				break;
			case BoundShape::Ellipse:
				rc.left += 1;
				rc.top += 1;
				rc.right -= 2;
				rc.bottom -= 2;
				canvas->drawEllipse(rc, pen);
				break;
			case BoundShape::Path:
				canvas->drawPath(getBoundShapePath(), pen);
				break;
			case BoundShape::Rectangle:
			default:
				rc.right -= 1;
				rc.bottom -= 1;
				canvas->setAntiAlias(sl_false);
				canvas->drawRectangle(rc, pen);
				canvas->setAntiAlias(sl_true);
				break;
		}
	}
}

void View::drawChildren(Canvas* canvas, const Ref<View>* children, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		View* child = children[i].ptr;
		if (child && !(child->isInstance()) && child->isVisible()) {
			drawChild(canvas, child);
		}
	}
}

void View::drawChild(Canvas* canvas, View* child)
{
	if (child) {
		CanvasStatusScope scope(canvas);
		canvas->translate(child->getX(), child->getY());
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

void View::onResize(sl_real width, sl_real height)
{
}

void View::onResizeChild(View* child, sl_real width, sl_real height)
{
}

void View::onShow()
{
}

void View::onHide()
{
}

void View::onScroll(sl_real x, sl_real y)
{
}

void View::onResizeContent(sl_real width, sl_real height)
{
}

void View::onAttach()
{
}

void View::dispatchDraw(Canvas* canvas)
{
	if (m_flagDoubleBuffer && !(canvas->isBuffer())) {
		Rectangle region = canvas->getClipBounds();
		if (!region.intersectRectangle(getBounds(), &region)) {
			return;
		}
		Ref<Bitmap> bitmapBuffer = m_bitmapDoubleBuffer;
		Ref<Canvas> canvasBuffer = m_canvasDoubleBuffer;
		sl_real width = getWidth();
		sl_real height = getHeight();
		if (bitmapBuffer.isNull() || canvasBuffer.isNull() || bitmapBuffer->getWidth() < width || bitmapBuffer->getHeight() < height) {
			Ref<GraphicsContext> context = canvas->getGraphicsContext();
			bitmapBuffer = context->createBitmap((sl_uint32)(Math::ceil(width / 256)) * 256, (sl_uint32)(Math::ceil(height / 256) * 256));
			if (bitmapBuffer.isNull()) {
				return;
			}
			canvasBuffer = bitmapBuffer->getCanvas();
			if (canvasBuffer.isNull()) {
				return;
			}
		}
		{
			CanvasStatusScope scope(canvasBuffer);
			canvasBuffer->clipToRectangle(region);
			draw(canvasBuffer.ptr);
		}
		canvas->draw(region, bitmapBuffer, region);
	} else {
		draw(canvas);
	}
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
		Ref<View> oldChildMouseMove = m_childMouseMove;
		Ref<View> scrollBars[2];
		scrollBars[0] = getHorizontalScrollBar();
		scrollBars[1] = getVerticalScrollBar();
		if (!(dispatchMouseEventToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				if (dispatchMouseEventToChildren(ev, children.data, children.count)) {
					oldChildMouseMove.setNull();
				}
			}
		} else {
			oldChildMouseMove.setNull();
		}
		if (oldChildMouseMove != m_childMouseMove) {
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
	Point ptMouse = ev->getPoint();
	
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
					Point pt = child->convertCoordinateFromParent(ptMouse);
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
					Point pt = child->convertCoordinateFromParent(ptMouse);
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
					Point pt = child->convertCoordinateFromParent(ptMouse);
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
			Point ptMouse = ev->getPoint();
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
		scrollBars[0] = getHorizontalScrollBar();
		scrollBars[1] = getVerticalScrollBar();
		if (!(dispatchTouchEventToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				if (m_flagMultiTouchMode) {
					dispatchMultiTouchEventToChildren(ev, children.data, children.count);
				} else {
					dispatchTouchEventToChildren(ev, children.data, children.count);
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
	Point ptMouse = ev->getPoint();
	
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
					Point pt = child->convertCoordinateFromParent(ptMouse);
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
							Point pt = child->convertCoordinateFromParent(ptsCheck[k].point);
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
			
			child->dispatchMouseEvent(ev);
			
			ev->setTouchPoints(arr);
			ev->setTouchPoint(ptTouch);
			
		} else {
			
			child->dispatchMouseEvent(ev);
			
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
		scrollBars[0] = getHorizontalScrollBar();
		scrollBars[1] = getVerticalScrollBar();
		if (!(dispatchMouseWheelEventToChildren(ev, scrollBars, 2))) {
			if (m_flagPassEventToChildren) {
				ListLocker< Ref<View> > children(m_children);
				dispatchMouseWheelEventToChildren(ev, children.data, children.count);
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
	Point ptMouse = ev->getPoint();
	for (sl_size i = 0; i < count; i++) {
		View* child = children[count - 1 - i].ptr;
		if (POINT_EVENT_CHECK_CHILD(child)) {
			Point pt = child->convertCoordinateFromParent(ptMouse);
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
			Point ptMouse = ev->getPoint();
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
	
	if (m_flagProcessingTabStop) {
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
		scrollBars[0] = getHorizontalScrollBar();
		scrollBars[1] = getVerticalScrollBar();
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
	Point ptMouse = ev->getPoint();
	for (sl_size i = 0; i < count; i++) {
		View* child = children[count - 1 - i].ptr;
		if (POINT_EVENT_CHECK_CHILD(child)) {
			Point pt = child->convertCoordinateFromParent(ptMouse);
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
			Point ptMouse = ev->getPoint();
			ev->setPoint(child->convertCoordinateFromParent(ptMouse));
			child->dispatchSetCursor(ev);
			ev->setPoint(ptMouse);
		} else {
			child->dispatchSetCursor(ev);
		}
	}
}

void View::dispatchResize(sl_real width, sl_real height)
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

void View::dispatchShow()
{
	onShow();
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onShow(this);
	}
}

void View::dispatchHide()
{
	onHide();
	PtrLocker<IViewListener> listener(getEventListener());
	if (listener.isNotNull()) {
		listener->onHide(this);
	}
}

void View::dispatchScroll(sl_real x, sl_real y)
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
	if (scroll->contentWidth < getWidth() && scroll->contentHeight < getHeight()) {
		return;
	}
	sl_real lineX = getWidth() / 20;
	sl_real lineY = getHeight() / 20;
	sl_real value = scroll->y;
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
				sl_real sx = scroll->scrollX_DownContent + ev->getX() - scroll->mouseX_DownContent;
				sl_real sy = scroll->scrollY_DownContent + ev->getY() - scroll->mouseY_DownContent;
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
				sl_real delta = ev->getDeltaY();
				if (delta > SLIB_EPSILON) {
					scrollTo(scroll->x, value + lineY);
					ev->stopPropagation();
				} else if (delta < -SLIB_EPSILON) {
					scrollTo(scroll->x, value - lineY);
					ev->stopPropagation();
				}
			}
			break;
		case UIAction::KeyDown:
			{
				sl_real sx = scroll->x;
				sl_real sy = scroll->y;
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
					scrollTo(sx, sy - getHeight());
					ev->stopPropagation();
				} else if (key == Keycode::Down) {
					scrollTo(sx, sy + getHeight());
					ev->stopPropagation();
				}
			}
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
}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))
Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}
#endif

SLIB_UI_NAMESPACE_END
