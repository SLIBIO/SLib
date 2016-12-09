#include "../../../inc/slib/ui/view.h"

#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/scroll_bar.h"
#include "../../../inc/slib/ui/render_view.h"

#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/math/transform2d.h"
#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/util.h"

SLIB_UI_NAMESPACE_BEGIN

/**********************
	View
**********************/

SLIB_DEFINE_OBJECT(View, Object)

View::View()
{
	m_flagCreatingInstance = sl_true;
	m_flagCreatingChildInstances = sl_false;
	m_flagCreatingNativeWidget = sl_false;
	m_attachMode = UIAttachMode::NotAttachInNativeWidget;
	
	m_frame.left = 0;
	m_frame.top = 0;
	m_frame.right = 0;
	m_frame.bottom = 0;
	m_boundsInParent.left = 0;
	m_boundsInParent.top = 0;
	m_boundsInParent.right = 0;
	m_boundsInParent.bottom = 0;
	
	m_visibility = Visibility::Visible;
	m_flagEnabled = sl_true;	
	m_flagHitTestable = sl_true;
	m_flagFocusable = sl_false;
	
	m_flagFocused = sl_false;
	m_flagPressed = sl_false;
	m_flagHover = sl_false;
	m_flagOccurringClick = sl_false;
	
	m_flagCurrentDrawing = sl_false;

	m_actionMouseDown = UIAction::Unknown;
	m_flagMultiTouchMode = sl_false;
	m_flagPassEventToChildren = sl_true;
	m_flagProcessingTabStop = sl_true;
	
	m_flagCapturingChildInstanceEvents = sl_false;
	
	m_flagOnAddChild = sl_false;
	m_flagOnRemoveChild = sl_false;
	
	m_paddingLeft = 0;
	m_paddingTop = 0;
	m_paddingRight = 0;
	m_paddingBottom = 0;
	
}

View::~View()
{
	detach();
	detachAnimations();
}

Ref<View::LayoutAttributes> View::_initializeLayoutAttibutes()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs;
	}
	attrs = new LayoutAttributes;
	if (attrs.isNull()) {
		return Ref<LayoutAttributes>::null();
	}

	attrs->widthMode = SizeMode::Fixed;
	attrs->heightMode = SizeMode::Fixed;
	attrs->widthWeight = 1;
	attrs->heightWeight = 1;
	
	attrs->leftMode = PositionMode::Fixed;
	attrs->topMode = PositionMode::Fixed;
	attrs->rightMode = PositionMode::Fixed;
	attrs->bottomMode = PositionMode::Fixed;
	
	attrs->measuredWidth = 0;
	attrs->measuredHeight = 0;
	attrs->frame = m_frame;
	attrs->flagInvalidMeasure = sl_true;
	attrs->flagInvalidLayout = sl_true;
	attrs->flagMakeLayout = sl_false;
	
	attrs->flagInvalidRelativeBoundWidth = sl_true;
	attrs->measuredRelativeBoundWidth = 0;
	attrs->flagBadRelativeBoundWidth = sl_false;
	attrs->flagInvalidRelativeBoundHeight = sl_true;
	attrs->measuredRelativeBoundHeight = 0;
	attrs->flagBadRelativeBoundHeight = sl_false;
	
	attrs->flagOnPrepareLayout = sl_false;
	attrs->flagOnMakeLayout = sl_false;
	
	attrs->marginLeft = 0;
	attrs->marginTop = 0;
	attrs->marginRight = 0;
	attrs->marginBottom = 0;
	attrs->flagRelativeMarginLeft = sl_false;
	attrs->relativeMarginLeftWeight = 0;
	attrs->flagRelativeMarginTop = sl_false;
	attrs->relativeMarginTopWeight = 0;
	attrs->flagRelativeMarginRight = sl_false;
	attrs->relativeMarginRightWeight = 0;
	attrs->flagRelativeMarginBottom = sl_false;
	attrs->relativeMarginBottomWeight = 0;
	
	attrs->flagRecursiveMakeLayout = sl_false;
	attrs->flagUpdatedLayoutFrame = sl_false;
		
	m_layoutAttributes = attrs;
	
	return attrs;
	
}

Ref<View::TransformAttributes> View::_initializeTransformAttributes()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs;
	}
	attrs = new TransformAttributes;
	if (attrs.isNull()) {
		return Ref<TransformAttributes>::null();
	}

	attrs->flagTransformFinalInvalid = sl_false;
	attrs->flagTransformFinal = sl_false;
	
	attrs->flagInverseTransformFinalInvalid = sl_false;
	attrs->flagInverseTransformFinal = sl_false;
	
	attrs->flagTransformStatic = sl_false;
	attrs->flagTransformAnimation = sl_false;
	
	attrs->flagTransformCalcInvalid = sl_false;
	attrs->flagTransformCalc = sl_false;
	
	attrs->translationStatic.x = 0;
	attrs->translationStatic.y = 0;
	attrs->translationAnimation.x = 0;
	attrs->translationAnimation.y = 0;
	
	attrs->scaleStatic.x = 1;
	attrs->scaleStatic.y = 1;
	attrs->scaleAnimation.x = 1;
	attrs->scaleAnimation.y = 1;
	
	attrs->rotationAngleStatic = 0;
	attrs->rotationAngleAnimation = 0;
	
	attrs->anchorOffset.x = 0;
	attrs->anchorOffset.y = 0;

	m_transformAttributes = attrs;
	
	return attrs;
	
}

Ref<View::DrawAttributes> View::_initializeDrawAttributes()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs;
	}
	attrs = new DrawAttributes;
	if (attrs.isNull()) {
		return Ref<DrawAttributes>::null();
	}
	
	attrs->backgroundScaleMode = ScaleMode::Stretch;
	attrs->backgroundAlignment = Alignment::MiddleCenter;
	attrs->backgroundColor = Color::zero();
	
	attrs->boundShape = BoundShape::Rectangle;
	
	attrs->roundRectBoundShapeRadius.x = 5;
	attrs->roundRectBoundShapeRadius.y = 5;
	
	attrs->borderColor = Color::Black;
	attrs->borderStyle = PenStyle::Solid;
	attrs->borderWidth = 0;
	
	attrs->flagPreDrawEnabled = sl_false;
	attrs->flagPostDrawEnabled = sl_false;
	attrs->flagOnDrawBackgroundAlways = sl_false;
	attrs->flagOnDrawBorderAlways = sl_false;
	
	attrs->flagUsingFont = sl_false;
	
	attrs->flagOpaque = sl_false;
	attrs->alphaStatic = 1;
	attrs->alphaAnimation = 1;
	attrs->flagLayer = sl_false;
	attrs->flagInvalidatedLayer = sl_true;
	attrs->flagInvalidatedWholeLayer = sl_true;

	m_drawAttributes = attrs;
	
	return attrs;
	
}

Ref<View::AnimationAttributes> View::_initializeAnimationAttributes()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs;
	}
	attrs = new AnimationAttributes;
	if (attrs.isNull()) {
		return Ref<AnimationAttributes>::null();
	}
	
	m_animationAttributes = attrs;
	
	return attrs;
}

Ref<View::ScrollAttributes> View::_initializeScrollAttributes()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs;
	}
	attrs = new ScrollAttributes;
	if (attrs.isNull()) {
		return Ref<ScrollAttributes>::null();
	}
	
	attrs->flagValidHorz = sl_false;
	attrs->flagValidVert = sl_false;
	attrs->x = 0;
	attrs->y = 0;
	attrs->contentWidth = 0;
	attrs->contentHeight = 0;
	attrs->barWidth = UI::getDefaultScrollBarWidth();
	
	attrs->flagContentScrollingByMouse = sl_true;
	attrs->flagContentScrollingByTouch = sl_true;
	attrs->flagContentScrollingByMouseWheel = sl_true;
	attrs->flagContentScrollingByKeyboard = sl_true;
	attrs->flagDownContent = sl_false;
	
	m_scrollAttributes = attrs;
	
	return attrs;
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
		if (getParent().isNull() && !(RenderView::checkInstance(this))) {
			_makeLayout(sl_false);
		}
		if (m_flagCreatingChildInstances) {
			ListLocker< Ref<View> > children(m_children);
			for (sl_size i = 0; i < children.count; i++) {
#if defined(SLIB_PLATFORM_IS_WIN32)
				Ref<View>& child = children[children.count - 1 - i];
#else
				Ref<View>& child = children[i];
#endif
				if (child.isNotNull() && child->m_flagCreatingInstance) {
					switch(child->getAttachMode()) {
						case UIAttachMode::NotAttach:
							break;
						case UIAttachMode::AttachAlways:
							attachChild(child);
							break;
						case UIAttachMode::NotAttachInNativeWidget:
							if (!(isNativeWidget())) {
								attachChild(child);
							}
							break;
						case UIAttachMode::AttachInNativeWidget:
							if (isNativeWidget()) {
								attachChild(child);
							}
							break;
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

void View::addChild(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	if (m_children.addIfNotExist(view)) {
		_addChild(view, mode);
	}
}

void View::insertChild(sl_size index, const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	if (m_children.insert(index, view)) {
		_addChild(view, mode);
	}
}

void View::removeChild(sl_size index, UIUpdateMode mode)
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
		_setFocusedChild(sl_null, UIUpdateMode::NoRedraw);
	}
	m_childrenMultiTouch.removeValue(view);
	requestLayout(mode);
}

void View::removeChild(const Ref<View>& view, UIUpdateMode mode)
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
		_setFocusedChild(sl_null, UIUpdateMode::NoRedraw);
	}
	m_childrenMultiTouch.removeValue(view);
	requestLayout(mode);
}

void View::removeAllChildren(UIUpdateMode mode)
{
	if (isInstance()) {
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, removeAllChildren, this, mode));
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
	_setFocusedChild(sl_null, UIUpdateMode::NoRedraw);
	m_childrenMultiTouch.setNull();
	m_children.removeAll();
	requestLayout(mode);
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
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		return parent->getRootView();
	}
	return this;
}

sl_bool View::isRootView()
{
	return m_parent.isNull();
}

Ref<View> View::getInstanceView()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return this;
	}
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		return parent->getInstanceView();
	}
	return Ref<View>::null();
}

Ref<ViewInstance> View::getInstanceViewInstance()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance;
	}
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		return parent->getInstanceViewInstance();
	}
	return Ref<ViewInstance>::null();
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
		if (child.isNotNull() && child->m_flagCreatingInstance) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, attachChild, this, child));
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
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, addChildInstance, this, child));
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
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, removeChildInstance, this, child));
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

void View::bringToFront(UIUpdateMode mode)
{
	Ref<ViewInstance> instance = m_instance;
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		ObjectLocker lock(&(parent->m_children));
		sl_reg index = parent->m_children.indexOf_NoLock(this);
		if (index >= 0) {
			parent->m_children.remove_NoLock(index);
			parent->m_children.add_NoLock(this);
			if (instance.isNull()) {
				if (mode == UIUpdateMode::Redraw) {
					invalidateBoundsInParent();
				}
			}
		}
	}
	if (instance.isNotNull()) {
		instance->bringToFront();
	}
}

void View::_addChild(const Ref<View>& child, UIUpdateMode mode)
{
	if (child.isNotNull()) {
		if (mode != UIUpdateMode::Init) {
			child->setFocus(sl_false, UIUpdateMode::NoRedraw);
		}
		child->setParent(this);
		if (m_flagOnAddChild) {
			onAddChild(child.ptr);
		}
		if (mode != UIUpdateMode::Init) {
			requestLayout(UIUpdateMode::NoRedraw);
			if (isInstance() && m_flagCreatingChildInstances && child->m_flagCreatingInstance) {
				switch(child->getAttachMode()) {
					case UIAttachMode::NotAttach:
						break;
					case UIAttachMode::AttachAlways:
						attachChild(child);
						break;
					case UIAttachMode::NotAttachInNativeWidget:
						if (!(isNativeWidget())) {
							attachChild(child);
						}
						break;
					case UIAttachMode::AttachInNativeWidget:
						if (isNativeWidget()) {
							attachChild(child);
						}
						break;
				}
			}
			child->updateAndInvalidateBoundsInParent(UIUpdateMode::NoRedraw);
		}
		if (mode == UIUpdateMode::Redraw) {
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

void View::invalidate()
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull() && instance->isNativeWidget()) {
		return;
	}
	if (m_frame.getWidth() > 0 && m_frame.getHeight() > 0) {
		if (UI::isUiThread()) {
			if (m_flagCurrentDrawing) {
				if (m_rectCurrentDrawing.containsRectangle(getBounds())) {
					return;
				}
			}
		}
		invalidateLayer();
#if defined(SLIB_PLATFORM_IS_WIN32)
		if (checkSelfInvalidatable()) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				instance->invalidate();
			}
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->invalidate(m_boundsInParent);
			}
		}
#else
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->invalidate();
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->invalidate(m_boundsInParent);
			}
		}
#endif
	}
}

void View::invalidate(const UIRect &rect)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull() && instance->isNativeWidget()) {
		return;
	}
	UIRect rectIntersect;
	if (getBounds().intersectRectangle(rect, &rectIntersect)) {
		if (UI::isUiThread()) {
			if (m_flagCurrentDrawing) {
				if (m_rectCurrentDrawing.containsRectangle(rectIntersect)) {
					return;
				}
			}
		}
		invalidateLayer(rectIntersect);
#if defined(SLIB_PLATFORM_IS_WIN32)
		if (checkSelfInvalidatable()) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				instance->invalidate(rectIntersect);
			}
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->invalidate(convertCoordinateToParent(rectIntersect));
			}
		}
#else
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->invalidate(rectIntersect);
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->invalidate(convertCoordinateToParent(rectIntersect));
			}
		}
#endif
	}
}

void View::invalidateBoundsInParent()
{
	if (checkSelfInvalidatable()) {
		return;
	}
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		parent->invalidate(m_boundsInParent);
	}
}

void View::updateAndInvalidateBoundsInParent(UIUpdateMode mode)
{
	if (mode == UIUpdateMode::Init) {
		return;
	}
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		if (mode == UIUpdateMode::Redraw && !(checkSelfInvalidatable())) {
			UIRect bounds = m_boundsInParent;
			if (bounds.getWidth() > 0 && bounds.getHeight() > 0) {
				UIRect boundsNew = convertCoordinateToParent(getBounds());
				boundsNew.left -= 2;
				boundsNew.top -= 2;
				boundsNew.right += 2;
				boundsNew.bottom += 2;
				m_boundsInParent = boundsNew;
				if (bounds.intersectRectangle(boundsNew)) {
					bounds.mergeRectangle(boundsNew);
					parent->invalidate(bounds);
				} else {
					parent->invalidate(bounds);
					parent->invalidate(boundsNew);
				}
			} else {
				m_boundsInParent = convertCoordinateToParent(getBounds());
				parent->invalidate(m_boundsInParent);
			}
		} else {
			m_boundsInParent = convertCoordinateToParent(getBounds());
		}
	} else {
		m_boundsInParent.setZero();
	}
}

sl_bool View::checkSelfInvalidatable()
{
#if defined(SLIB_PLATFORM_IS_WIN32)
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		if (instance->isNativeWidget()) {
			return sl_true;
		}
		if (isOpaque()) {
			return sl_true;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			Ref<ViewInstance> parentInstance = parent->m_instance;
			if (parentInstance.isNull()) {
				return sl_true;
			}
			if (parentInstance->isNativeWidget()) {
				return sl_true;
			}
			return sl_false;
		}
		return sl_true;
	}
	return sl_false;
#else
	return isInstance();
#endif
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

void View::_setFrame(const UIRect& _frame, UIUpdateMode mode, sl_bool flagLayouting)
{
	
	UIRect frame = _frame;
	frame.fixSizeError();
	UIRect frameOld = m_frame;
	
	sl_bool flagNotMoveX = Math::isAlmostZero(frameOld.left - frame.left);
	sl_bool flagNotMoveY = Math::isAlmostZero(frameOld.top - frame.top);
	sl_bool flagNotResizeWidth = Math::isAlmostZero(frameOld.getWidth() - frame.getWidth());
	sl_bool flagNotResizeHeight = Math::isAlmostZero(frameOld.getHeight() - frame.getHeight());
	
	if (flagNotMoveX && flagNotMoveY && flagNotResizeWidth && flagNotResizeHeight) {
		return;
	}

	m_frame = frame;
	
	_setFrame_NI(frame);
	
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		layoutAttrs->frame = frame;
		layoutAttrs->flagInvalidMeasure = sl_true;
		if (!(flagNotResizeWidth && flagNotResizeHeight)) {
			layoutAttrs->flagInvalidLayout = sl_true;
		}
		if (mode != UIUpdateMode::Init) {
			if (!flagLayouting) {
				Ref<View> parent = getParent();
				if (parent.isNotNull()) {
					parent->requestLayout(UIUpdateMode::NoRedraw);
				} else {
					_requestMakeLayout();
				}
			}
		}
	} else {
		if (mode != UIUpdateMode::Init) {
			requestParentLayout(UIUpdateMode::NoRedraw);
		}
	}
	
	if (!(flagNotResizeWidth && flagNotResizeHeight)) {
		dispatchResize(frame.getWidth(), frame.getHeight());
		if (mode == UIUpdateMode::Redraw) {
			invalidateLayer();
		}
	}
	
	updateAndInvalidateBoundsInParent(mode);

}

void View::setFrame(const UIRect &frame, UIUpdateMode mode)
{
	_setFrame(frame, mode, sl_false);
}

void View::setFrame(sl_ui_pos x, sl_ui_pos y, sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
{
	_setFrame(UIRect(x, y, x+width, y+height), mode, sl_false);
}

sl_ui_len View::getWidth()
{
	return m_frame.getWidth();
}

void View::setWidth(sl_ui_len width, UIUpdateMode mode)
{
	setFrame(m_frame.left, m_frame.top, width, m_frame.getHeight(), mode);
}

sl_ui_len View::getHeight()
{
	return m_frame.getHeight();
}

void View::setHeight(sl_ui_len height, UIUpdateMode mode)
{
	setFrame(m_frame.left, m_frame.top, m_frame.getWidth(), height, mode);
}

UISize View::getSize()
{
	return m_frame.getSize();
}

void View::setSize(const UISize& size, UIUpdateMode mode)
{
	setFrame(m_frame.left, m_frame.top, size.x, size.y, mode);
}

void View::setSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
{
	setFrame(m_frame.left, m_frame.top, width, height, mode);
}

sl_ui_pos View::getLeft()
{
	return m_frame.left;
}

sl_ui_pos View::getTop()
{
	return m_frame.top;
}

void View::setLeft(sl_ui_pos x, UIUpdateMode mode)
{
	setFrame(x, m_frame.top, m_frame.getWidth(), m_frame.getHeight(), mode);
}

void View::setTop(sl_ui_pos y, UIUpdateMode mode)
{
	setFrame(m_frame.left, y, m_frame.getWidth(), m_frame.getHeight(), mode);
}

UIPoint View::getPosition()
{
	return m_frame.getLeftTop();
}

void View::setPosition(sl_ui_pos x, sl_ui_pos y, UIUpdateMode mode)
{
	setFrame(x, y, m_frame.getWidth(), m_frame.getHeight(), mode);
}

void View::setPosition(const UIPoint& point, UIUpdateMode mode)
{
	setFrame(point.x, point.y, m_frame.getWidth(), m_frame.getHeight(), mode);
}

UIRect View::getBounds()
{
	return UIRect(0, 0, m_frame.getWidth(), m_frame.getHeight());
}

UIRect View::getBoundsInnerPadding()
{
	UIRect ret(m_paddingLeft, m_paddingTop, m_frame.getWidth() - m_paddingRight, m_frame.getHeight() - m_paddingBottom);
	ret.fixSizeError();
	return ret;
}

UIRect View::getBoundsInParent()
{
	return m_boundsInParent;
}

Visibility View::getVisibility()
{
	return m_visibility;
}

void View::setVisibility(Visibility visibility, UIUpdateMode mode)
{
	Visibility oldVisibility = m_visibility;
	m_visibility = visibility;
	Ref<ViewInstance> instance = m_instance;
	if (oldVisibility != visibility) {
		if (instance.isNotNull()) {
			instance->setVisible(visibility == Visibility::Visible);
		}
		if (mode != UIUpdateMode::Init) {
			switch (visibility) {
				case Visibility::Visible:
				case Visibility::Hidden:
					if (oldVisibility == Visibility::Gone) {
						requestParentAndSelfLayout(UIUpdateMode::NoRedraw);
					}
					break;
				case Visibility::Gone:
					requestParentLayout(UIUpdateMode::NoRedraw);
					break;
			}
		}
		dispatchChangeVisibility(oldVisibility, visibility);
	}
	if (instance.isNull()) {
		if (mode == UIUpdateMode::Redraw) {
			invalidateBoundsInParent();
		}
	}
}

sl_bool View::isVisible()
{
	return m_visibility == Visibility::Visible;
}

void View::setVisible(sl_bool flag, UIUpdateMode mode)
{
	if (flag) {
		setVisibility(Visibility::Visible, mode);
	} else {
		setVisibility(Visibility::Gone, mode);
	}
}

sl_bool View::isEnabled()
{
	return m_flagEnabled;
}

void View::setEnabled(sl_bool flag, UIUpdateMode mode)
{
	m_flagEnabled = flag;
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setEnabled(flag);
	} else {
		if (mode == UIUpdateMode::Redraw) {
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
			return GraphicsUtil::containsPointInRoundRect(Point((sl_real)x, (sl_real)y), rc, getRoundRectBoundShapeRadius());
		case BoundShape::Ellipse:
			return GraphicsUtil::containsPointInEllipse(Point((sl_real)x, (sl_real)y), rc);
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

void View::setFocus(sl_bool flagFocused, UIUpdateMode mode)
{
	m_flagFocused = flagFocused;
	if (flagFocused) {
		Ref<ViewInstance> instance = getInstanceViewInstance();
		if (instance.isNotNull()) {
			instance->setFocus();
		}
	} else {
		_killFocusFromParent();
	}
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		if (flagFocused) {
			parent->_setFocusedChild(this, mode);
			return;
		} else {
			if (m_childFocused == this) {
				m_childFocused.setNull();
			}
		}
	}
	if (mode == UIUpdateMode::Redraw) {
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

Ref<View> View::getFocusedDescendant()
{
	Ref<View> focused = m_childFocused;
	if (focused.isNotNull()) {
		Ref<View> descendant = focused->getFocusedDescendant();
		if (descendant.isNotNull()) {
			return descendant;
		}
		return focused;
	}
	return Ref<View>::null();
}

void View::_setFocusedChild(View* child, UIUpdateMode mode)
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
			parent->_setFocusedChild(this, mode);
			return;
		}
	}
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

sl_bool View::isPressedState()
{
	return m_flagPressed;
}

void View::setPressedState(sl_bool flagState, UIUpdateMode mode)
{
	if (m_flagPressed != flagState) {
		m_flagPressed = flagState;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool View::isHoverState()
{
	return m_flagHover;
}

void View::setHoverState(sl_bool flagState, UIUpdateMode mode)
{
	if (m_flagHover != flagState) {
		m_flagHover = flagState;
		if (mode == UIUpdateMode::Redraw) {
			Ref<DrawAttributes> attrs = m_drawAttributes;
			if (attrs.isNotNull()) {
				if (attrs->backgroundHover.isNotNull() && attrs->background != attrs->backgroundHover) {
					invalidate();
				}
			}
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
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNull()) {
		return;
	}
	if (!(layoutAttrs->flagInvalidMeasure)) {
		return;
	}
	
	SizeMode widthMode = layoutAttrs->widthMode;
	SizeMode heightMode = layoutAttrs->heightMode;
	
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
	layoutAttrs->measuredWidth = measuredWidth;
	layoutAttrs->measuredHeight = measuredHeight;
	
	if (flagHorizontal || flagVertical) {
		onMeasureLayout(flagHorizontal, flagVertical);
	}
	
	if (widthMode == SizeMode::Wrapping) {
		measuredWidth = layoutAttrs->measuredWidth;
	}
	if (heightMode == SizeMode::Wrapping) {
		measuredHeight = layoutAttrs->measuredHeight;
	}
	
	layoutAttrs->flagInvalidMeasure = sl_false;

}

sl_ui_len View::getMeasuredWidth()
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		if (layoutAttrs->widthMode == SizeMode::Wrapping) {
			return layoutAttrs->measuredWidth;
		} else if (layoutAttrs->widthMode == SizeMode::Filling) {
			return 0;
		} else if (layoutAttrs->widthMode == SizeMode::Weight) {
			return 0;
		}
	}
	return getWidth();
}

void View::setMeasuredWidth(sl_ui_len width)
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		layoutAttrs->measuredWidth = width;
	}
}

sl_ui_len View::getMeasuredHeight()
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		if (layoutAttrs->heightMode == SizeMode::Wrapping) {
			return layoutAttrs->measuredHeight;
		} else if (layoutAttrs->heightMode == SizeMode::Filling) {
			return 0;
		} else if (layoutAttrs->heightMode == SizeMode::Weight) {
			return 0;
		}
	}
	return getHeight();
}

void View::setMeasuredHeight(sl_ui_len height)
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		layoutAttrs->measuredHeight = height;
	}
}

void View::_prepareLayout(ViewPrepareLayoutParam& param)
{
	if (!(isVisible())) {
		return;
	}
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNull()) {
		return;
	}
	
	UIRect frame;
	if (param.flagUseLayoutFrame) {
		frame = layoutAttrs->frame;
	} else {
		frame = m_frame;
	}
	
	SizeMode widthMode = layoutAttrs->widthMode;
	SizeMode heightMode = layoutAttrs->heightMode;
	
	if (widthMode == SizeMode::Wrapping || heightMode == SizeMode::Wrapping) {
		measureLayout();
	}
	
	PositionMode leftMode = layoutAttrs->leftMode;
	PositionMode topMode = layoutAttrs->topMode;
	PositionMode rightMode = layoutAttrs->rightMode;
	PositionMode bottomMode = layoutAttrs->bottomMode;
	
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
			width = layoutAttrs->measuredWidth;
			break;
		case SizeMode::Filling:
		case SizeMode::Weight:
			width = (sl_ui_pos)((sl_real)parentWidth * Math::abs(layoutAttrs->widthWeight));
			break;
		default:
			width = frame.getWidth();
			break;
	}
	switch (heightMode) {
		case SizeMode::Wrapping:
			height = layoutAttrs->measuredHeight;
			break;
		case SizeMode::Filling:
		case SizeMode::Weight:
			height = (sl_ui_pos)((sl_real)parentHeight * Math::abs(layoutAttrs->heightWeight));
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
	
	if (layoutAttrs->flagRelativeMarginLeft) {
		layoutAttrs->marginLeft = (sl_ui_pos)((sl_real)parentWidth * layoutAttrs->relativeMarginLeftWeight);
	}
	if (layoutAttrs->flagRelativeMarginTop) {
		layoutAttrs->marginTop = (sl_ui_pos)((sl_real)parentHeight * layoutAttrs->relativeMarginTopWeight);
	}
	if (layoutAttrs->flagRelativeMarginRight) {
		layoutAttrs->marginRight = (sl_ui_pos)((sl_real)parentWidth * layoutAttrs->relativeMarginRightWeight);
	}
	if (layoutAttrs->flagRelativeMarginBottom) {
		layoutAttrs->marginBottom = (sl_ui_pos)((sl_real)parentHeight * layoutAttrs->relativeMarginBottomWeight);
	}
	Ref<View> referView;
	switch (leftMode) {
		case PositionMode::ParentEdge:
			frame.left = param.parentContentFrame.left + layoutAttrs->marginLeft;
			break;
		case PositionMode::OtherStart:
			referView = layoutAttrs->leftReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.left = referFrame.left + layoutAttrs->marginLeft;
			} else {
				frame.left = param.parentContentFrame.left + layoutAttrs->marginLeft;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layoutAttrs->leftReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.left = referFrame.right + referView->getMarginRight() + layoutAttrs->marginLeft;
			} else {
				frame.left = param.parentContentFrame.left + layoutAttrs->marginLeft;
			}
			break;
		case PositionMode::CenterInParent:
			frame.left = (param.parentContentFrame.left + layoutAttrs->marginLeft + param.parentContentFrame.right - layoutAttrs->marginRight - width) / 2;
			break;
		case PositionMode::CenterInOther:
			referView = layoutAttrs->leftReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.left = (referFrame.left + layoutAttrs->marginLeft + referFrame.right - layoutAttrs->marginRight - width) / 2;
			} else {
				frame.left = (param.parentContentFrame.left + layoutAttrs->marginLeft + param.parentContentFrame.right - layoutAttrs->marginRight - width) / 2;
			}
		default:
			break;
	}
	switch (rightMode) {
		case PositionMode::ParentEdge:
			frame.right = param.parentContentFrame.right - layoutAttrs->marginRight;
			break;
		case PositionMode::OtherStart:
			referView = layoutAttrs->rightReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.right = referFrame.left - referView->getMarginLeft() - layoutAttrs->marginRight;
			} else {
				frame.right = param.parentContentFrame.right - layoutAttrs->marginRight;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layoutAttrs->rightReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.right = referFrame.right - layoutAttrs->marginRight;
			} else {
				frame.right = param.parentContentFrame.right - layoutAttrs->marginRight;
			}
			break;
		default:
			frame.right = param.parentContentFrame.right;
			break;
	}
	switch (topMode) {
		case PositionMode::ParentEdge:
			frame.top = param.parentContentFrame.top + layoutAttrs->marginTop;
			break;
		case PositionMode::OtherStart:
			referView = layoutAttrs->topReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.top = referFrame.top + layoutAttrs->marginTop;
			} else {
				frame.top = param.parentContentFrame.top + layoutAttrs->marginTop;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layoutAttrs->topReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.top = referFrame.bottom + referView->getMarginBottom() + layoutAttrs->marginTop;
			} else {
				frame.top = param.parentContentFrame.top + layoutAttrs->marginTop;
			}
			break;
		case PositionMode::CenterInParent:
			frame.top = (param.parentContentFrame.top + layoutAttrs->marginTop + param.parentContentFrame.bottom - layoutAttrs->marginBottom - height) / 2;
			break;
		case PositionMode::CenterInOther:
			referView = layoutAttrs->topReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.top = (referFrame.top + layoutAttrs->marginTop + referFrame.bottom - layoutAttrs->marginBottom - height) / 2;
			} else {
				frame.top = (param.parentContentFrame.top + layoutAttrs->marginTop + param.parentContentFrame.bottom - layoutAttrs->marginBottom - height) / 2;
			}
			break;
		default:
			break;
	}
	switch (bottomMode)
	{
		case PositionMode::ParentEdge:
			frame.bottom = param.parentContentFrame.bottom - layoutAttrs->marginBottom;
			break;
		case PositionMode::OtherStart:
			referView = layoutAttrs->bottomReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.bottom = referFrame.top - referView->getMarginTop() - layoutAttrs->marginBottom;
			} else {
				frame.bottom = param.parentContentFrame.bottom - layoutAttrs->marginBottom;
			}
			break;
		case PositionMode::OtherEnd:
			referView = layoutAttrs->bottomReferingView;
			if (referView.isNotNull()) {
				UIRect referFrame = referView->getLayoutFrame();
				frame.bottom = referFrame.bottom - layoutAttrs->marginBottom;
			} else {
				frame.bottom = param.parentContentFrame.bottom - layoutAttrs->marginBottom;
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
			if (layoutAttrs->widthWeight > 0) {
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
			if (layoutAttrs->heightWeight > 0) {
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
	layoutAttrs->frame = frame;
	layoutAttrs->flagUpdatedLayoutFrame = sl_true;
}

void View::_makeLayout(sl_bool flagApplyLayout)
{
	if (isVisible()) {
		
		Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
		
		if (layoutAttrs.isNotNull() && !(layoutAttrs->flagRecursiveMakeLayout)) {
			
			layoutAttrs->flagRecursiveMakeLayout = sl_true;
			
			do {
				if (flagApplyLayout) {
					if (layoutAttrs->frame.isAlmostEqual(m_frame)) {
						if (!(layoutAttrs->flagInvalidLayout)) {
							break;
						}
					} else {
						_setFrame(layoutAttrs->frame, UIUpdateMode::NoRedraw, sl_true);
					}
				} else {
					if (!(layoutAttrs->flagInvalidLayout)) {
						break;
					}
					if (layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping) {
						measureLayout();
						UIRect frame = layoutAttrs->frame;
						if (layoutAttrs->widthMode == SizeMode::Wrapping) {
							frame.setWidth(layoutAttrs->measuredWidth);
						}
						if (layoutAttrs->heightMode == SizeMode::Wrapping) {
							frame.setHeight(layoutAttrs->measuredHeight);
						}
						_setFrame(frame, UIUpdateMode::NoRedraw, sl_true);
					}
				}
				
				if (layoutAttrs->flagMakeLayout) {
					
					ViewPrepareLayoutParam param;
					param.parentContentFrame.left = m_paddingLeft;
					param.parentContentFrame.top = m_paddingTop;
					param.parentContentFrame.right = getWidth() - m_paddingRight;
					param.parentContentFrame.bottom = getHeight() - m_paddingBottom;
					param.parentContentFrame.fixSizeError();
					param.flagUseLayoutFrame = sl_false;
					
					{
						ListLocker< Ref<View> > children(m_children);
						for (sl_size i = 0; i < children.count; i++) {
							Ref<View>& child = children[i];
							if (child.isNotNull() && child->isLayouting()) {
								child->_prepareLayout(param);
								child->setLayoutFrameUpdated(sl_false);
							}
						}
					}
					
					if (layoutAttrs->flagOnMakeLayout) {
						onMakeLayout();
					}
					
					param.flagUseLayoutFrame = sl_true;

					{
						ListLocker< Ref<View> > children(m_children);
						for (sl_size i = 0; i < children.count; i++) {
							Ref<View>& child = children[i];
							if (child.isNotNull() && child->isLayouting()) {
								if (!(child->isLayoutFrameUpdated())) {
									child->_prepareLayout(param);
									if (child->isOnPrepareLayoutEnabled()) {
										child->onPrepareLayout(param);
									}
								}
								if (child->isInstance() && RenderView::checkInstance(child.ptr)) {
									child->_setFrame(child->getLayoutFrame(), UIUpdateMode::NoRedraw, sl_true);
									child->_requestMakeLayout();
								} else {
									child->_makeLayout(sl_true);
								}
							}
						}
					}
				}
				
				layoutAttrs->flagInvalidLayout = sl_false;
				
			} while (0);

			layoutAttrs->flagRecursiveMakeLayout = sl_false;
			
		}
	}

}

void View::_measureRelativeBoundWidth()
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNull()) {
		return;
	}
	if (!(layoutAttrs->flagInvalidRelativeBoundWidth)) {
		return;
	}
	layoutAttrs->flagInvalidRelativeBoundWidth = sl_false;
	layoutAttrs->measuredRelativeBoundWidth = 0;
	layoutAttrs->flagBadRelativeBoundWidth = sl_false;
	
	measureLayout();
	
	PositionMode leftMode = layoutAttrs->leftMode;
	PositionMode rightMode = layoutAttrs->rightMode;
	
	SizeMode widthMode = layoutAttrs->widthMode;
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
	if (!(layoutAttrs->flagRelativeMarginLeft)) {
		marginLeft = layoutAttrs->marginLeft;
	}
	sl_ui_pos marginRight = 0;
	if (!(layoutAttrs->flagRelativeMarginRight)) {
		marginRight = layoutAttrs->marginRight;
	}
	
	sl_ui_pos parentPaddingLeft = 0;
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parentPaddingLeft = parent->getPaddingLeft();
	}

	if (leftMode == PositionMode::Fixed && rightMode == PositionMode::Fixed) {
		marginLeft = m_frame.left - parentPaddingLeft;
	}
	
	sl_ui_pos outerWidth = layoutAttrs->measuredWidth + marginLeft + marginRight;
	
	if (leftMode == PositionMode::OtherStart || leftMode == PositionMode::OtherEnd) {
		Ref<View> referView = layoutAttrs->leftReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayoutAttrs = referView->m_layoutAttributes;
			if (referLayoutAttrs.isNull()) {
				if (leftMode == PositionMode::OtherStart) {
					outerWidth += referView->m_frame.left - parentPaddingLeft;
				} else {
					outerWidth += referView->m_frame.right - parentPaddingLeft;
				}
			} else {
				if (referLayoutAttrs->rightMode != PositionMode::Fixed || referLayoutAttrs->leftMode == PositionMode::CenterInOther || referLayoutAttrs->leftMode == PositionMode::CenterInParent) {
					layoutAttrs->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				referView->_measureRelativeBoundWidth();
				if (referLayoutAttrs->flagBadRelativeBoundWidth) {
					layoutAttrs->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				if (leftMode == PositionMode::OtherStart) {
					sl_ui_pos w = referLayoutAttrs->measuredRelativeBoundWidth;
					w -= referView->getMeasuredWidth();
					w -= referView->getAbsoluteMarginRight();
					if (w < 0) {
						w = 0;
					}
					outerWidth += w;
				} else {
					sl_ui_pos w = referLayoutAttrs->measuredRelativeBoundWidth;
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
		Ref<View> referView = layoutAttrs->rightReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayoutAttrs = referView->m_layoutAttributes;
			if (referLayoutAttrs.isNull()) {
				layoutAttrs->flagBadRelativeBoundWidth = sl_true;
				return;
			} else {
				if (referLayoutAttrs->rightMode == PositionMode::Fixed || referLayoutAttrs->leftMode != PositionMode::Fixed) {
					layoutAttrs->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				referView->_measureRelativeBoundWidth();
				if (referLayoutAttrs->flagBadRelativeBoundWidth) {
					layoutAttrs->flagBadRelativeBoundWidth = sl_true;
					return;
				}
				if (rightMode == PositionMode::OtherStart) {
					sl_ui_pos w = referLayoutAttrs->measuredRelativeBoundWidth;
					w -= referView->getAbsoluteMarginLeft();
					if (w < 0) {
						w = 0;
					}
					outerWidth += w;
				} else {
					sl_ui_pos w = referLayoutAttrs->measuredRelativeBoundWidth;
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
	
	layoutAttrs->measuredRelativeBoundWidth = outerWidth;
	
}

void View::_measureRelativeBoundHeight()
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNull()) {
		return;
	}
	
	if (!(layoutAttrs->flagInvalidRelativeBoundHeight)) {
		return;
	}
	layoutAttrs->flagInvalidRelativeBoundHeight = sl_false;
	layoutAttrs->measuredRelativeBoundHeight = 0;
	layoutAttrs->flagBadRelativeBoundHeight = sl_false;
	
	measureLayout();
	
	PositionMode topMode = layoutAttrs->topMode;
	PositionMode bottomMode = layoutAttrs->bottomMode;
	
	SizeMode heightMode = layoutAttrs->heightMode;
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
	if (!(layoutAttrs->flagRelativeMarginTop)) {
		marginTop = layoutAttrs->marginTop;
	}
	sl_ui_pos marginBottom = 0;
	if (!(layoutAttrs->flagRelativeMarginBottom)) {
		marginBottom = layoutAttrs->marginBottom;
	}
	
	sl_ui_pos parentPaddingTop = 0;
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		parentPaddingTop = parent->getPaddingTop();
	}
	if (topMode == PositionMode::Fixed && bottomMode == PositionMode::Fixed) {
		marginTop = m_frame.top - getPaddingBottom();
	}
	
	sl_ui_pos outerHeight = layoutAttrs->measuredHeight + marginTop + marginBottom;
	
	if (topMode == PositionMode::OtherStart || topMode == PositionMode::OtherEnd) {
		Ref<View> referView = layoutAttrs->topReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayoutAttrs = referView->m_layoutAttributes;
			if (referLayoutAttrs.isNull()) {
				if (topMode == PositionMode::OtherStart) {
					outerHeight += referView->m_frame.top - parentPaddingTop;
				} else {
					outerHeight += referView->m_frame.bottom - parentPaddingTop;
				}
			} else {
				if (referLayoutAttrs->bottomMode != PositionMode::Fixed || referLayoutAttrs->topMode == PositionMode::CenterInOther || referLayoutAttrs->topMode == PositionMode::CenterInParent) {
					layoutAttrs->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				referView->_measureRelativeBoundHeight();
				if (referLayoutAttrs->flagBadRelativeBoundHeight) {
					layoutAttrs->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				if (topMode == PositionMode::OtherStart) {
					sl_ui_pos h = referLayoutAttrs->measuredRelativeBoundHeight;
					h -= referView->getMeasuredHeight();
					h -= referView->getAbsoluteMarginBottom();
					if (h < 0) {
						h = 0;
					}
					outerHeight += h;
				} else {
					sl_ui_pos h = referLayoutAttrs->measuredRelativeBoundHeight;
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
		Ref<View> referView = layoutAttrs->bottomReferingView;
		if (referView.isNotNull()) {
			Ref<LayoutAttributes> referLayoutAttrs = referView->m_layoutAttributes;
			if (referLayoutAttrs.isNull()) {
				layoutAttrs->flagBadRelativeBoundHeight = sl_true;
				return;
			} else {
				if (referLayoutAttrs->bottomMode == PositionMode::Fixed || referLayoutAttrs->topMode != PositionMode::Fixed) {
					layoutAttrs->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				referView->_measureRelativeBoundHeight();
				if (referLayoutAttrs->flagBadRelativeBoundHeight) {
					layoutAttrs->flagBadRelativeBoundHeight = sl_true;
					return;
				}
				if (bottomMode == PositionMode::OtherStart) {
					sl_ui_pos h = referLayoutAttrs->measuredRelativeBoundHeight;
					h -= referView->getAbsoluteMarginTop();
					if (h < 0) {
						h = 0;
					}
					outerHeight += h;
				} else {
					sl_ui_pos h = referLayoutAttrs->measuredRelativeBoundHeight;
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
	
	layoutAttrs->measuredRelativeBoundHeight = outerHeight;
	
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
				Ref<LayoutAttributes> layoutAttrs = child->m_layoutAttributes;
				if (layoutAttrs.isNotNull()) {
					layoutAttrs->flagInvalidRelativeBoundWidth = sl_true;
					layoutAttrs->flagInvalidRelativeBoundHeight = sl_true;
				}
			}
		}
	}
	for (i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				if (child->getVisibility() != Visibility::Gone) {
					Ref<LayoutAttributes> layoutAttrs = child->m_layoutAttributes;
					if (layoutAttrs.isNotNull()) {
						if (flagHorizontal) {
							child->_measureRelativeBoundWidth();
							if (!(layoutAttrs->flagBadRelativeBoundWidth)) {
								sl_ui_pos w = layoutAttrs->measuredRelativeBoundWidth + paddingWidth;
								if (w > measuredWidth) {
									measuredWidth = w;
								}
							}
						}
						if (flagVertical) {
							child->_measureRelativeBoundHeight();
							if (!(layoutAttrs->flagBadRelativeBoundHeight)) {
								sl_ui_pos h = layoutAttrs->measuredRelativeBoundHeight + paddingHeight;
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

void View::_requestMakeLayout()
{
	sl_bool flagRenderView = sl_false;
	Ref<View> view = this;
	while (view.isNotNull()) {
		if (view->isInstance()) {
			if (RenderView::checkInstance(view.ptr)) {
				flagRenderView = sl_true;
				break;
			}
		}
		view = view->getParent();
	}
	if (flagRenderView) {
		((RenderView*)(view.ptr))->requestRender();
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(View, _makeLayout, this, sl_false));
	}
}

void View::_requestInvalidateLayout(UIUpdateMode mode)
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		layoutAttrs->flagInvalidLayout = sl_true;
		layoutAttrs->flagInvalidMeasure = sl_true;
		if (mode != UIUpdateMode::Init) {
			_requestMakeLayout();
		}
	}
}

void View::_requestInvalidateMeasure(sl_bool flagWidth, sl_bool flagHeight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		layoutAttrs->flagInvalidLayout = sl_true;
		flagWidth = flagWidth && layoutAttrs->widthMode == SizeMode::Wrapping;
		flagHeight = flagHeight && layoutAttrs->heightMode == SizeMode::Wrapping;
		if (flagWidth || flagHeight) {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->_requestInvalidateMeasure(flagWidth, flagHeight, mode);
			}
		}
		_requestInvalidateLayout(mode);
	}
}

void View::requestLayout(UIUpdateMode mode)
{
	_requestInvalidateMeasure(sl_true, sl_true, mode);
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void View::requestParentLayout(UIUpdateMode mode)
{
	Ref<View> parent = m_parent;
	if (parent.isNotNull()) {
		parent->requestLayout(mode);
	}
}

void View::requestParentAndSelfLayout(UIUpdateMode mode)
{
	Ref<LayoutAttributes> layoutAttrs = m_layoutAttributes;
	if (layoutAttrs.isNotNull()) {
		layoutAttrs->flagInvalidLayout = sl_true;
		layoutAttrs->flagInvalidMeasure = sl_true;
		Ref<View> parent = m_parent;
		if (parent.isNotNull() && parent->isLayouting()) {
			parent->requestLayout(mode);
			_requestInvalidateLayout(mode);
		} else {
			_requestInvalidateLayout(mode);
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}
}

sl_bool View::isLayouting()
{
	return m_layoutAttributes.isNotNull();
}

void View::resetLayout(UIUpdateMode mode)
{
	m_layoutAttributes.setNull();
	requestParentLayout(mode);
}

sl_bool View::isMakingLayout()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagMakeLayout;
	}
	return sl_false;
}

void View::setMakingLayout(sl_bool flagMakeLayout, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagMakeLayout = flagMakeLayout;
		requestLayout(mode);
	}
}

UIRect View::getLayoutFrame()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->frame;
	}
	return getFrame();
}

void View::setLayoutFrame(const UIRect& rect)
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		attrs->frame = rect;
		attrs->flagUpdatedLayoutFrame = sl_true;
	} else {
		_setFrame(rect, UIUpdateMode::NoRedraw, sl_true);
	}
}

sl_bool View::isLayoutFrameUpdated()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagUpdatedLayoutFrame;
	}
	return sl_false;
}

void View::setLayoutFrameUpdated(sl_bool flag)
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		attrs->flagUpdatedLayoutFrame = flag;
	}
}

sl_bool View::isOnPrepareLayoutEnabled()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagOnPrepareLayout;
	}
	return sl_false;
}

void View::setOnPrepareLayoutEnabled(sl_bool flagEnabled, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagOnPrepareLayout = flagEnabled;
		requestParentLayout(mode);
	}
}

sl_bool View::isOnMakeLayoutEnabled()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagOnMakeLayout;
	}
	return sl_false;
}

void View::setOnMakeLayoutEnabled(sl_bool flagEnabled, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagOnMakeLayout = flagEnabled;
		requestLayout(mode);
	}
}

SizeMode View::getWidthMode()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->widthMode;
	}
	return SizeMode::Fixed;
}

SizeMode View::getHeightMode()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->heightMode;
	}
	return SizeMode::Fixed;
}

sl_bool View::isWidthFixed()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->widthMode == SizeMode::Fixed;
	}
	return sl_true;
}

void View::setWidthFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Fixed;
		requestParentAndSelfLayout(mode);
	}
}

sl_bool View::isHeightFixed()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->heightMode == SizeMode::Fixed;
	}
	return sl_true;
}

void View::setHeightFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->heightMode = SizeMode::Fixed;
		requestParentAndSelfLayout(mode);
	}
}

void View::setSizeFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Fixed;
		attrs->heightMode = SizeMode::Fixed;
		requestParentAndSelfLayout(mode);
	}
}

sl_real View::getWidthWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->widthWeight;
	}
	return 1;
}

sl_bool View::isHeightWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->heightMode == SizeMode::Weight;
	}
	return sl_false;
}

sl_bool View::isWidthFilling()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->widthMode == SizeMode::Filling;
	}
	return sl_false;
}

void View::setWidthFilling(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Filling;
		attrs->widthWeight = weight;
		requestParentAndSelfLayout(mode);
	}
}

sl_bool View::isHeightFilling()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->heightMode == SizeMode::Filling;
	}
	return sl_false;
}

void View::setHeightFilling(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->heightMode = SizeMode::Filling;
		attrs->heightWeight = weight;
		requestParentAndSelfLayout(mode);
	}
}

void View::setSizeFilling(sl_real widthWeight, sl_real heightWeight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Filling;
		attrs->widthWeight = widthWeight;
		attrs->heightMode = SizeMode::Filling;
		attrs->heightWeight = heightWeight;
		requestParentAndSelfLayout(mode);
	}
}

sl_bool View::isWidthWrapping()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->widthMode == SizeMode::Wrapping;
	}
	return sl_false;
}

void View::setWidthWrapping(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Wrapping;
		requestParentAndSelfLayout(mode);
	}
}

sl_bool View::isHeightWrapping()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->heightMode == SizeMode::Wrapping;
	}
	return sl_false;
}

void View::setHeightWrapping(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->heightMode = SizeMode::Wrapping;
		requestParentAndSelfLayout(mode);
	}
}

void View::setSizeWrapping(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Wrapping;
		attrs->heightMode = SizeMode::Wrapping;
		requestParentAndSelfLayout(mode);
	}
}

sl_bool View::isWidthWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->widthMode == SizeMode::Weight;
	}
	return sl_false;
}

void View::setWidthWeight(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Weight;
		attrs->widthWeight = weight;
		requestParentAndSelfLayout(mode);
	}
}

sl_real View::getHeightWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->heightWeight;
	}
	return 1;
}

void View::setHeightWeight(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->heightMode = SizeMode::Weight;
		attrs->heightWeight = weight;
		requestParentAndSelfLayout(mode);
	}
}

void View::setSizeWeight(sl_real widthWeight, sl_real heightWeight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->widthMode = SizeMode::Weight;
		attrs->widthWeight = widthWeight;
		attrs->heightMode = SizeMode::Weight;
		attrs->heightWeight = heightWeight;
		requestParentAndSelfLayout(mode);
	}
}

sl_bool View::isLayoutLeftFixed()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutLeftFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::Fixed;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignParentLeft()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentLeft(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::ParentEdge;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignLeft()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setAlignLeft(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::OtherStart;
		attrs->leftReferingView = view;
		requestParentLayout(mode);
	}
}

sl_bool View::isRightOf()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setRightOf(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::OtherEnd;
		attrs->leftReferingView = view;
		requestParentLayout(mode);
	}
}

Ref<View> View::getLayoutLeftReferingView()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isLayoutRightFixed()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->rightMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutRightFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->rightMode = PositionMode::Fixed;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignParentRight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->rightMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentRight(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->rightMode = PositionMode::ParentEdge;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignRight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->rightMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setAlignRight(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->rightMode = PositionMode::OtherEnd;
		attrs->rightReferingView = view;
		requestParentLayout(mode);
	}
}

sl_bool View::isLeftOf()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->rightMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setLeftOf(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->rightMode = PositionMode::OtherStart;
		attrs->rightReferingView = view;
		requestParentLayout(mode);
	}
}

Ref<View> View::getLayoutRightReferingView()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->rightReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isLayoutTopFixed()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutTopFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->topMode = PositionMode::Fixed;
		requestParentLayout(mode);
	}

}

sl_bool View::isAlignParentTop()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentTop(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->topMode = PositionMode::ParentEdge;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignTop()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setAlignTop(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->topMode = PositionMode::OtherStart;
		attrs->topReferingView = view;
		requestParentLayout(mode);
	}
}

sl_bool View::isBelow()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setBelow(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->topMode = PositionMode::OtherEnd;
		attrs->topReferingView = view;
		requestParentLayout(mode);
	}
}

Ref<View> View::getLayoutTopReferingView()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isLayoutBottomFixed()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->bottomMode == PositionMode::Fixed;
	}
	return sl_true;
}

void View::setLayoutBottomFixed(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->bottomMode = PositionMode::Fixed;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignParentBottom()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->bottomMode == PositionMode::ParentEdge;
	}
	return sl_false;
}

void View::setAlignParentBottom(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->bottomMode = PositionMode::ParentEdge;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignBottom()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->bottomMode == PositionMode::OtherEnd;
	}
	return sl_false;
}

void View::setAlignBottom(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNotNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->bottomMode = PositionMode::OtherEnd;
		attrs->bottomReferingView = view;
		requestParentLayout(mode);
	}
}

sl_bool View::isAbove()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->bottomMode == PositionMode::OtherStart;
	}
	return sl_false;
}

void View::setAbove(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNotNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->bottomMode = PositionMode::OtherStart;
		attrs->bottomReferingView = view;
		requestParentLayout(mode);
	}
}

Ref<View> View::getLayoutBottomReferingView()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->bottomReferingView;
	}
	return Ref<View>::null();
}

sl_bool View::isCenterHorizontal()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftMode == PositionMode::CenterInParent;
	}
	return sl_false;
}

void View::setCenterHorizontal(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::CenterInParent;
		requestParentLayout(mode);
	}
}

sl_bool View::isCenterVertical()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topMode == PositionMode::CenterInParent;
	}
	return sl_false;
}

void View::setCenterVertical(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->topMode = PositionMode::CenterInParent;
		requestParentLayout(mode);
	}
}

void View::setCenterInParent(UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::CenterInParent;
		attrs->topMode = PositionMode::CenterInParent;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignCenterHorizontal()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->leftMode == PositionMode::CenterInOther;
	}
	return sl_false;
}

void View::setAlignCenterHorizontal(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNotNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->leftMode = PositionMode::CenterInOther;
		attrs->leftReferingView = view;
		requestParentLayout(mode);
	}
}

sl_bool View::isAlignCenterVertical()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->topMode == PositionMode::CenterInOther;
	}
	return sl_false;
}

void View::setAlignCenterVertical(const Ref<View>& view, UIUpdateMode mode)
{
	if (view.isNotNull()) {
		return;
	}
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->topMode = PositionMode::CenterInOther;
		attrs->topReferingView = view;
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getMarginLeft()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->marginLeft;
	}
	return 0;
}

void View::setMarginLeft(sl_ui_pos marginLeft, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->marginLeft = marginLeft;
		if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
			attrs->leftMode = PositionMode::ParentEdge;
		}
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getMarginTop()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->marginTop;
	}
	return 0;
}

void View::setMarginTop(sl_ui_pos marginTop, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->marginTop = marginTop;
		if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
			attrs->topMode = PositionMode::ParentEdge;
		}
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getMarginRight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->marginRight;
	}
	return 0;
}

void View::setMarginRight(sl_ui_pos marginRight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->marginRight = marginRight;
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getMarginBottom()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->marginBottom;
	}
	return 0;
}

void View::setMarginBottom(sl_ui_pos marginBottom, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->marginBottom = marginBottom;
		requestParentLayout(mode);
	}
}

void View::setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->marginLeft = left;
		attrs->marginTop = top;
		attrs->marginRight = right;
		attrs->marginBottom = bottom;
		requestParentLayout(mode);
	}
}

void View::setMargin(sl_ui_pos margin, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->marginLeft = margin;
		attrs->marginTop = margin;
		attrs->marginRight = margin;
		attrs->marginBottom = margin;
		requestParentLayout(mode);
	}
}

sl_bool View::isRelativeMarginLeft()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagRelativeMarginLeft;
	}
	return sl_false;
}

sl_real View::getRelativeMarginLeftWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->relativeMarginLeftWeight;
	}
	return 0;
}

void View::setRelativeMarginLeft(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginLeft = sl_true;
		attrs->relativeMarginLeftWeight = weight;
		if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
			attrs->leftMode = PositionMode::ParentEdge;
		}
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getAbsoluteMarginLeft()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull() && !(attrs->flagRelativeMarginLeft)) {
		if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
			return 0;
		}
		return attrs->marginLeft;
	}
	return 0;
}

void View::setAbsoluteMarginLeft(sl_ui_pos margin, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginLeft = sl_false;
		attrs->marginLeft = margin;
		if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
			attrs->leftMode = PositionMode::ParentEdge;
		}
		requestParentLayout(mode);
	}
}

sl_bool View::isRelativeMarginTop()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagRelativeMarginTop;
	}
	return sl_false;
}

sl_real View::getRelativeMarginTopWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->relativeMarginTopWeight;
	}
	return 0;
}

void View::setRelativeMarginTop(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginTop = sl_true;
		attrs->relativeMarginTopWeight = weight;
		if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
			attrs->topMode = PositionMode::ParentEdge;
		}
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getAbsoluteMarginTop()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull() && !(attrs->flagRelativeMarginTop)) {
		if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
			return 0;
		}
		return attrs->marginTop;
	}
	return 0;
}

void View::setAbsoluteMarginTop(sl_ui_pos margin, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginTop = sl_false;
		attrs->marginTop = margin;
		if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
			attrs->topMode = PositionMode::ParentEdge;
		}
		requestParentLayout(mode);
	}
}

sl_bool View::isRelativeMarginRight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagRelativeMarginRight;
	}
	return sl_false;
}

sl_real View::getRelativeMarginRightWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->relativeMarginRightWeight;
	}
	return 0;
}

void View::setRelativeMarginRight(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginRight = sl_true;
		attrs->relativeMarginRightWeight = weight;
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getAbsoluteMarginRight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull() && !(attrs->flagRelativeMarginRight)) {
		if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
			return 0;
		}
		return attrs->marginRight;
	}
	return 0;
}

void View::setAbsoluteMarginRight(sl_ui_pos margin, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginRight = sl_false;
		attrs->marginRight = margin;
		requestParentLayout(mode);
	}
}

sl_bool View::isRelativeMarginBottom()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagRelativeMarginBottom;
	}
	return sl_false;
}

sl_real View::getRelativeMarginBottomWeight()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		return attrs->relativeMarginBottomWeight;
	}
	return 0;
}

void View::setRelativeMarginBottom(sl_real weight, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginBottom = sl_true;
		attrs->relativeMarginBottomWeight = weight;
		requestParentLayout(mode);
	}
}

sl_ui_pos View::getAbsoluteMarginBottom()
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull() && !(attrs->flagRelativeMarginBottom)) {
		if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
			return 0;
		}
		return attrs->marginBottom;
	}
	return 0;
}

void View::setAbsoluteMarginBottom(sl_ui_pos margin, UIUpdateMode mode)
{
	Ref<LayoutAttributes> attrs = _initializeLayoutAttibutes();
	if (attrs.isNotNull()) {
		attrs->flagRelativeMarginBottom = sl_false;
		attrs->marginBottom = margin;
		requestParentLayout(mode);
	}
}

void View::applyRelativeMargins(sl_ui_len parentWidth, sl_ui_len parentHeight)
{
	Ref<LayoutAttributes> attrs = m_layoutAttributes;
	if (attrs.isNotNull()) {
		if (attrs->flagRelativeMarginLeft) {
			attrs->marginLeft = (sl_ui_pos)((sl_real)parentWidth * attrs->relativeMarginLeftWeight);
		}
		if (attrs->flagRelativeMarginTop) {
			attrs->marginTop = (sl_ui_pos)((sl_real)parentHeight * attrs->relativeMarginTopWeight);
		}
		if (attrs->flagRelativeMarginRight) {
			attrs->marginRight = (sl_ui_pos)((sl_real)parentWidth * attrs->relativeMarginRightWeight);
		}
		if (attrs->flagRelativeMarginBottom) {
			attrs->marginBottom = (sl_ui_pos)((sl_real)parentHeight * attrs->relativeMarginBottomWeight);
		}
	}
}

sl_ui_pos View::getPaddingLeft()
{
	return m_paddingLeft;
}

void View::setPaddingLeft(sl_ui_pos paddingLeft, UIUpdateMode mode)
{
	m_paddingLeft = paddingLeft;
	requestLayout(mode);
	onChangePadding();
}

sl_ui_pos View::getPaddingTop()
{
	return m_paddingTop;
}

void View::setPaddingTop(sl_ui_pos paddingTop, UIUpdateMode mode)
{
	m_paddingTop = paddingTop;
	requestLayout(mode);
	onChangePadding();
}

sl_ui_pos View::getPaddingRight()
{
	return m_paddingRight;
}

void View::setPaddingRight(sl_ui_pos paddingRight, UIUpdateMode mode)
{
	m_paddingRight = paddingRight;
	requestLayout(mode);
	onChangePadding();
}

sl_ui_pos View::getPaddingBottom()
{
	return m_paddingBottom;
}

void View::setPaddingBottom(sl_ui_pos paddingBottom, UIUpdateMode mode)
{
	m_paddingBottom = paddingBottom;
	requestLayout(mode);
	onChangePadding();
}

void View::setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
{
	m_paddingLeft = left;
	m_paddingTop = top;
	m_paddingRight = right;
	m_paddingBottom = bottom;
	requestLayout(mode);
	onChangePadding();
}

void View::setPadding(sl_ui_pos padding, UIUpdateMode mode)
{
	m_paddingLeft = padding;
	m_paddingTop = padding;
	m_paddingRight = padding;
	m_paddingBottom = padding;
	requestLayout(mode);
	onChangePadding();
}

sl_bool View::getFinalTransform(Matrix3* _out)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNull()) {
		return sl_false;
	}
	
	if (attrs->flagTransformFinalInvalid) {
		attrs->flagTransformFinalInvalid = sl_false;
		attrs->flagInverseTransformFinalInvalid = sl_true;
		if (attrs->flagTransformStatic) {
			attrs->flagTransformFinal = sl_true;
			if (attrs->flagTransformAnimation) {
				attrs->transformFinal = attrs->transformStatic * attrs->transformAnimation;
			} else {
				attrs->transformFinal = attrs->transformStatic;
			}
		} else {
			if (attrs->flagTransformCalcInvalid) {
				attrs->flagTransformCalcInvalid = sl_false;
				Matrix3 mat;
				sl_bool flagInited = sl_false;
				sl_real tx = attrs->translationStatic.x + attrs->translationAnimation.x;
				sl_real ty = attrs->translationStatic.y + attrs->translationAnimation.y;
				sl_bool flagTranslate = !(Math::isAlmostZero(tx) && Math::isAlmostZero(ty));
				sl_real sx = attrs->scaleStatic.x * attrs->scaleAnimation.x;
				sl_real sy = attrs->scaleStatic.y * attrs->scaleAnimation.y;
				sl_bool flagScale = !(Math::isAlmostZero(sx - 1) && Math::isAlmostZero(sy - 1));
				sl_real r = attrs->rotationAngleStatic + attrs->rotationAngleAnimation;
				sl_bool flagRotate = !(Math::isAlmostZero(r));
				if (flagScale || flagRotate) {
					sl_real ax = attrs->anchorOffset.x;
					sl_real ay = attrs->anchorOffset.y;
					sl_bool flagAnchor = !(Math::isAlmostZero(ax) && Math::isAlmostZero(ay));
					if (flagAnchor) {
						if (flagInited) {
							Transform2::translate(mat, -ax, -ay);
						} else {
							mat = Transform2::getTranslationMatrix(-ax, -ay);
							flagInited = sl_true;
						}
					}
					if (flagRotate) {
						if (flagInited) {
							Transform2::rotate(mat, r);
						} else {
							mat = Transform2::getRotationMatrix(r);
							flagInited = sl_true;
						}
					}
					if (flagScale) {
						if (flagInited) {
							Transform2::scale(mat, sx, sy);
						} else {
							mat = Transform2::getScalingMatrix(sx, sy);
							flagInited = sl_true;
						}
					}
					if (flagAnchor) {
						Transform2::translate(mat, ax, ay);
					}
				}
				if (flagTranslate) {
					if (flagInited) {
						Transform2::translate(mat, tx, ty);
					} else {
						mat = Transform2::getTranslationMatrix(tx, ty);
						flagInited = sl_true;
					}
				}
				if (flagInited) {
					attrs->flagTransformCalc = sl_true;
					attrs->transformCalc = mat;
				} else {
					attrs->flagTransformCalc = sl_false;
				}
			}

			if (attrs->flagTransformCalc) {
				attrs->flagTransformFinal = sl_true;
				if (attrs->flagTransformAnimation) {
					attrs->transformFinal = attrs->transformCalc * attrs->transformAnimation;
				} else {
					attrs->transformFinal = attrs->transformCalc;
				}
			} else {
				if (attrs->flagTransformAnimation) {
					attrs->flagTransformFinal = sl_true;
					attrs->transformFinal = attrs->transformAnimation;
				} else {
					attrs->flagTransformFinal = sl_false;
				}
			}
			
		}
	}
	
	if (attrs->flagTransformFinal) {
		if (_out) {
			*_out = attrs->transformFinal;
		}
		return sl_true;
	}
	
	return sl_false;
	
}

sl_bool View::getFinalInverseTransform(Matrix3* _out)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNull()) {
		return sl_false;
	}
	if (attrs->flagTransformFinalInvalid) {
		getFinalTransform(sl_null);
	}
	if (attrs->flagInverseTransformFinalInvalid) {
		attrs->flagInverseTransformFinalInvalid = sl_false;
		if (attrs->flagTransformFinal) {
			attrs->flagInverseTransformFinal = sl_true;
			attrs->inverseTransformFinal = attrs->transformFinal.inverse();
		} else {
			attrs->flagInverseTransformFinal = sl_false;
		}
	}
	if (attrs->flagInverseTransformFinal) {
		if (_out) {
			*_out = attrs->inverseTransformFinal;
		}
		return sl_true;
	}
	return sl_false;
}

const Matrix3& View::getTransform()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull() && attrs->flagTransformStatic) {
		return attrs->transformStatic;
	}
	return Matrix3::identity();
}

void View::setTransform(const Matrix3& matrix, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->flagTransformStatic = sl_true;
		attrs->transformStatic = matrix;
		_applyFinalTransform(mode);
	}
}

void View::resetTransform(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull() && attrs->flagTransformStatic) {
		attrs->flagTransformStatic = sl_false;
		_applyFinalTransform(mode);
	}
}

void View::setTransformFromAnimation(const Matrix3& matrix, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->flagTransformAnimation = sl_true;
		attrs->transformAnimation = matrix;
		_applyFinalTransform(mode);
	}
}

void View::resetTransformFromAnimation(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull() && attrs->flagTransformAnimation) {
		attrs->flagTransformAnimation = sl_false;
		_applyFinalTransform(mode);
	}
}

sl_bool View::getFinalTranslationRotationScale(Vector2* translation, sl_real* rotation, Vector2* scale, Vector2* anchor)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNull()) {
		return sl_false;
	}
	if (translation) {
		translation->x = attrs->translationStatic.x + attrs->translationAnimation.x;
		translation->y = attrs->translationStatic.y + attrs->translationAnimation.y;
	}
	if (scale) {
		scale->x = attrs->scaleStatic.x * attrs->scaleAnimation.x;
		scale->y = attrs->scaleStatic.y * attrs->scaleAnimation.y;
	}
	if (rotation) {
		*rotation = attrs->rotationAngleStatic + attrs->rotationAngleAnimation;
	}
	if (anchor) {
		anchor->x = attrs->anchorOffset.x;
		anchor->y = attrs->anchorOffset.y;
	}
	return sl_true;
}

sl_real View::getTranslationX()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->translationStatic.x;
	}
	return 0;
}

sl_real View::getTranslationY()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->translationStatic.y;
	}
	return 0;
}

const Vector2& View::getTranslation()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->translationStatic;
	}
	return Vector2::zero();
}

void View::setTranslationX(sl_real tx, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->translationStatic.x = tx;
		_applyCalcTransform(mode);
	}
}

void View::setTranslationY(sl_real ty, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->translationStatic.y = ty;
		_applyCalcTransform(mode);
	}
}

void View::setTranslation(sl_real tx, sl_real ty, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->translationStatic.x = tx;
		attrs->translationStatic.y = ty;
		_applyCalcTransform(mode);
	}
}

void View::setTranslation(const Vector2& t, UIUpdateMode mode)
{
	setTranslation(t.x, t.y, mode);
}

void View::setTranslationFromAnimation(const Vector2& t, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->translationAnimation = t;
		_applyCalcTransform(mode);
	}
}

void View::resetTranslationFromAnimation(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		attrs->translationAnimation.x = 0;
		attrs->translationAnimation.y = 0;
		_applyCalcTransform(mode);
	}
}

sl_real View::getScaleX()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->scaleStatic.x;
	}
	return 1;
}

sl_real View::getScaleY()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->scaleStatic.y;
	}
	return 1;
}

const Vector2& View::getScale()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->scaleStatic;
	}
	static const sl_real t[2] = {1, 1};
	return Vector2::fromArray(t);
}

void View::setScaleX(sl_real sx, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->scaleStatic.x = sx;
		_applyCalcTransform(mode);
	}
}

void View::setScaleY(sl_real sy, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->scaleStatic.y = sy;
		_applyCalcTransform(mode);
	}
}

void View::setScale(sl_real sx, sl_real sy, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->scaleStatic.x = sx;
		attrs->scaleStatic.y = sy;
		_applyCalcTransform(mode);
	}
}

void View::setScale(sl_real factor, UIUpdateMode mode)
{
	setScale(factor, factor, mode);
}

void View::setScale(const Vector2& factor, UIUpdateMode mode)
{
	setScale(factor.x, factor.y, mode);
}

void View::setScaleFromAnimation(const Vector2& factor, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->scaleAnimation = factor;
		_applyCalcTransform(mode);
	}
}

void View::resetScaleFromAnimation(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		attrs->scaleAnimation.x = 1;
		attrs->scaleAnimation.y = 1;
		_applyCalcTransform(mode);
	}
}

sl_real View::getRotation()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->rotationAngleStatic;
	}
	return 0;
}

void View::setRotation(sl_real radian, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->rotationAngleStatic = radian;
		_applyCalcTransform(mode);
	}
}

void View::setRotationFromAnimation(sl_real radian, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->rotationAngleAnimation = radian;
		_applyCalcTransform(mode);
	}
}

void View::resetRotationFromAnimation(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		attrs->rotationAngleAnimation = 0;
		_applyCalcTransform(mode);
	}
}

sl_real View::getAnchorOffsetX()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->anchorOffset.x;
	}
	return 0;
}

sl_real View::getAnchorOffsetY()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->anchorOffset.y;
	}
	return 0;
}

const Vector2& View::getAnchorOffset()
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		return attrs->anchorOffset;
	}
	return Vector2::zero();
}

void View::setAnchorOffsetX(sl_real x, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->anchorOffset.x = x;
		_applyCalcTransform(mode);
	}
}

void View::setAnchorOffsetY(sl_real y, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->anchorOffset.y = y;
		_applyCalcTransform(mode);
	}
}

void View::setAnchorOffset(sl_real x, sl_real y, UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = _initializeTransformAttributes();
	if (attrs.isNotNull()) {
		attrs->anchorOffset.x = x;
		attrs->anchorOffset.y = y;
		_applyCalcTransform(mode);
	}
}

void View::setAnchorOffset(const Vector2& pt, UIUpdateMode mode)
{
	setAnchorOffset(pt.x, pt.y, mode);
}

void View::_applyCalcTransform(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		attrs->flagTransformCalcInvalid = sl_true;
		if (!(attrs->flagTransformStatic)) {
			_applyFinalTransform(mode);
		}
	}
}

void View::_applyFinalTransform(UIUpdateMode mode)
{
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		attrs->flagTransformFinalInvalid = sl_true;
		if (mode != UIUpdateMode::Init) {
			if (isInstance()) {
				_invalidateInstanceTransform();
			}
			updateAndInvalidateBoundsInParent(mode);
		}
	}
}

void View::_invalidateInstanceTransform()
{
	Matrix3 mat;
	if (getFinalTransform(&mat)) {
		_setTransform_NI(mat);
	} else {
		_setTransform_NI(Matrix3::identity());
	}
}

UIPointf View::convertCoordinateFromScreen(const UIPointf& ptScreen)
{
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return ptScreen;
	}
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromScreenToView(ptScreen);
	}
	UIPointf pt = parent->convertCoordinateFromScreen(ptScreen);
	return convertCoordinateFromParent(pt);
}

UIPointf View::convertCoordinateToScreen(const UIPointf& ptView)
{
	Ref<View> parent = getParent();
	if (parent.isNull()) {
		return ptView;
	}
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		return instance->convertCoordinateFromViewToScreen(ptView);
	}
	UIPointf pt = convertCoordinateToParent(ptView);
	return parent->convertCoordinateToScreen(pt);
}

UIPointf View::convertCoordinateFromParent(const UIPointf& ptParent)
{
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return ptParent;
	}
	
	sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
	sl_ui_posf offy = (sl_ui_posf)(m_frame.top);
	
#if defined(SLIB_PLATFORM_IS_WIN32)
	if (isInstance()) {
		Vector2 t;
		if (getFinalTranslationRotationScale(&t, sl_null, sl_null, sl_null)) {
			offx += (sl_ui_pos)(t.x);
			offy += (sl_ui_pos)(t.y);
		}
		return UIPointf(ptParent.x - offx, ptParent.y - offy);
	}
#endif
	
	UIPointf pt = ptParent;
	pt.x -= offx;
	pt.y -= offy;
	
	Matrix3 mat;
	if (getFinalInverseTransform(&mat)) {
		sl_real ax = (sl_real)(m_frame.getWidth()) / 2;
		sl_real ay = (sl_real)(m_frame.getHeight()) / 2;
		pt = mat.transformPosition(pt.x - ax, pt.y - ay);
		pt.x += (sl_ui_posf)(ax);
		pt.y += (sl_ui_posf)(ay);
	}
	
	return pt;
}

UIRectf View::convertCoordinateFromParent(const UIRectf& rcParent)
{
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return rcParent;
	}
	
	sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
	sl_ui_posf offy = (sl_ui_posf)(m_frame.top);
	
#if defined(SLIB_PLATFORM_IS_WIN32)
	if (isInstance()) {
		Vector2 t;
		if (getFinalTranslationRotationScale(&t, sl_null, sl_null, sl_null)) {
			offx += (sl_ui_pos)(t.x);
			offy += (sl_ui_pos)(t.y);
		}
		return UIRectf(rcParent.left - offx, rcParent.top - offy, rcParent.right - offx, rcParent.bottom - offy);
	}
#endif
	
	Matrix3 mat;
	if (getFinalInverseTransform(&mat)) {
		UIPointf pts[4];
		rcParent.getCornerPoints(pts);
		for (int i = 0; i < 4; i++) {
			sl_real ax = (sl_real)(m_frame.getWidth()) / 2;
			sl_real ay = (sl_real)(m_frame.getHeight()) / 2;
			pts[i] = mat.transformPosition(pts[i].x - (sl_real)offx - ax, pts[i].y - (sl_real)offy - ay);
			pts[i].x += (sl_ui_posf)(ax);
			pts[i].y += (sl_ui_posf)(ay);
		}
		UIRectf rc;
		rc.setFromPoints(pts, 4);
		return rc;
	} else {
		return UIRectf(rcParent.left - offx, rcParent.top - offy, rcParent.right - offx, rcParent.bottom - offy);
	}
}

UIPointf View::convertCoordinateToParent(const UIPointf& ptView)
{
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return ptView;
	}
	
	sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
	sl_ui_posf offy = (sl_ui_posf)(m_frame.top);

#if defined(SLIB_PLATFORM_IS_WIN32)
	if (isInstance()) {
		Vector2 t;
		if (getFinalTranslationRotationScale(&t, sl_null, sl_null, sl_null)) {
			offx += (sl_ui_pos)(t.x);
			offy += (sl_ui_pos)(t.y);
		}
		return UIPointf(ptView.x + offx, ptView.y + offy);
	}
#endif
	
	UIPointf pt = ptView;
	Matrix3 mat;
	if (getFinalTransform(&mat)) {
		sl_real ax = (sl_real)(m_frame.getWidth()) / 2;
		sl_real ay = (sl_real)(m_frame.getHeight()) / 2;
		pt = mat.transformPosition(pt.x - ax, pt.y - ay);
		pt.x += (sl_ui_posf)(ax);
		pt.y += (sl_ui_posf)(ay);
	}
	
	pt.x += offx;
	pt.y += offy;
	
	return pt;
}

UIRectf View::convertCoordinateToParent(const UIRectf& rcView)
{
	Ref<View> parent = m_parent;
	if (parent.isNull()) {
		return rcView;
	}
	
	sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
	sl_ui_posf offy = (sl_ui_posf)(m_frame.top);

#if defined(SLIB_PLATFORM_IS_WIN32)
	if (isInstance()) {
		Vector2 t;
		if (getFinalTranslationRotationScale(&t, sl_null, sl_null, sl_null)) {
			offx += (sl_ui_pos)(t.x);
			offy += (sl_ui_pos)(t.y);
		}
		return UIRectf(rcView.left + offx, rcView.top + offy, rcView.right + offx, rcView.bottom + offy);
	}
#endif
	
	Matrix3 mat;
	if (getFinalTransform(&mat)) {
		UIPointf pts[4];
		rcView.getCornerPoints(pts);
		for (int i = 0; i < 4; i++) {
			sl_real ax = (sl_real)(m_frame.getWidth()) / 2;
			sl_real ay = (sl_real)(m_frame.getHeight()) / 2;
			pts[i] = mat.transformPosition(pts[i].x - ax, pts[i].y - ay);
			pts[i].x += (sl_ui_posf)(ax) + offx;
			pts[i].y += (sl_ui_posf)(ay) + offy;
		}
		UIRectf rc;
		rc.setFromPoints(pts, 4);
		return rc;
	} else {
		return UIRectf(rcView.left + offx, rcView.top + offy, rcView.right + offx, rcView.bottom + offy);
	}
}

Ref<Drawable> View::getBackground()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->background;
	}
	return Ref<Drawable>::null();
}

void View::setBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->background = drawable;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Ref<Drawable> View::getPressedBackground()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->backgroundPressed;
	}
	return Ref<Drawable>::null();
}

void View::setPressedBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->backgroundPressed = drawable;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Ref<Drawable> View::getHoverBackground()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->backgroundHover;
	}
	return Ref<Drawable>::null();
}

void View::setHoverBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->backgroundHover = drawable;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

ScaleMode View::getBackgroundScaleMode()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->backgroundScaleMode;
	}
	return ScaleMode::Stretch;
}

void View::setBackgroundScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->backgroundScaleMode = scaleMode;
		if (updateMode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Alignment View::getBackgroundAlignment()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->backgroundAlignment;
	}
	return Alignment::MiddleCenter;
}

void View::setBackgroundAlignment(Alignment align, UIUpdateMode mode)
{
	Ref<DrawAttributes> draw = _initializeDrawAttributes();
	if (draw.isNotNull()) {
		draw->backgroundAlignment = align;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Color View::getBackgroundColor()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->backgroundColor;
	}
	return Color::zero();
}

void View::setBackgroundColor(const Color& color, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->backgroundColor = color;
		if (isNativeWidget()) {
			_setBackgroundColor_NW(color);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}
}

Ref<Pen> View::getBorder()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->penBorder;
	}
	return Ref<Pen>::null();
}

void View::setBorder(const Ref<Pen>& pen, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->penBorder = pen;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Color View::getBorderColor()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->borderColor;
	}
	return Color::Black;
}

void View::setBorderColor(const Color& color, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->borderColor = color;
		_refreshBorderPen(mode);
	}
}

sl_bool View::isBorder()
{
	return getBorder().isNotNull();
}

void View::setBorder(sl_bool flagBorder, UIUpdateMode mode)
{
	if (flagBorder) {
		if (isBorder()) {
			return;
		}
		setBorder(Pen::getDefault(), UIUpdateMode::NoRedraw);
	} else {
		if (isBorder()) {
			setBorder(Ref<Pen>::null(), UIUpdateMode::NoRedraw);
		}
	}
	if (isNativeWidget()) {
		_setBorder_NW(flagBorder);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

PenStyle View::getBorderStyle()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->borderStyle;
	}
	return PenStyle::Solid;
}

void View::setBorderStyle(PenStyle style, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->borderStyle = style;
		_refreshBorderPen(mode);
	}
}

sl_real View::getBorderWidth()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->borderWidth;
	}
	return 0;
}

void View::setBorderWidth(sl_real width, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->borderWidth = width;
		_refreshBorderPen(mode);
	}
}

void View::_refreshBorderPen(UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		sl_real width = attrs->borderWidth;
		Ref<Pen> pen;
		if (width > 0) {
			pen = Pen::create(attrs->borderStyle, attrs->borderWidth, attrs->borderColor);
		}
		setBorder(pen, mode);
	}
}

BoundShape View::getBoundShape()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->boundShape;
	}
	return BoundShape::Rectangle;
}

void View::setBoundShape(BoundShape shape, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->boundShape = shape;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

const Size& View::getRoundRectBoundShapeRadius()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->roundRectBoundShapeRadius;
	}
	return Size::zero();
}

void View::setRoundRectBoundShapeRadius(const Size& radius, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->roundRectBoundShapeRadius = radius;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void View::setRoundRectBoundShapeRadius(sl_real rx, sl_real ry, UIUpdateMode mode)
{
	setRoundRectBoundShapeRadius(Size(rx, ry), mode);
}

void View::setRoundRectBoundShapeRadiusX(sl_real rx, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->roundRectBoundShapeRadius.x = rx;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void View::setRoundRectBoundShapeRadiusY(sl_real ry, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->roundRectBoundShapeRadius.y = ry;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void View::setRoundRectBoundShapeRadius(sl_real radius, UIUpdateMode mode)
{
	setRoundRectBoundShapeRadius(Size(radius, radius), mode);
}

Ref<GraphicsPath> View::getBoundShapePath()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->boundShapePath;
	}
	return Ref<GraphicsPath>::null();
}

void View::setBoundShapePath(const Ref<GraphicsPath>& path, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->boundShapePath = path;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool View::isPreDrawEnabled()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagPreDrawEnabled;
	}
	return sl_false;
}

void View::setPreDrawEnabled(sl_bool flagEnabled, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagPreDrawEnabled = flagEnabled;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool View::isPostDrawEnabled()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagPostDrawEnabled;
	}
	return sl_false;
}

void View::setPostDrawEnabled(sl_bool flagEnabled, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagPostDrawEnabled = flagEnabled;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool View::isAlwaysOnDrawBackground()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagOnDrawBackgroundAlways;
	}
	return sl_false;
}

void View::setAlwaysOnDrawBackground(sl_bool flagEnabled, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagOnDrawBackgroundAlways = flagEnabled;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool View::isAlwaysOnDrawBorder()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagOnDrawBorderAlways;
	}
	return sl_false;
}

void View::setAlwaysOnDrawBorder(sl_bool flagEnabled, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagOnDrawBorderAlways = flagEnabled;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Ref<Font> View::getFont()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		Ref<Font> font = attrs->font;
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

void View::_setFontInvalidateChildInstances()
{
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			child->_setFontInvalidateChildInstances();
			if (child->isUsingFont()) {
				child->requestLayout(UIUpdateMode::NoRedraw);
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

void View::setFont(const Ref<Font>& font, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		if (mode != UIUpdateMode::Init) {
			_setFontInvalidateChildInstances();
		}
		if (isNativeWidget()) {
			Ref<Font> _font = font;
			if (_font.isNull()) {
				Ref<View> parent = getParent();
				if (parent.isNotNull()) {
					_font = parent->getFont();
				}
				_font = UI::getDefaultFont();
			}
			if (_font.isNotNull()) {
				_setFont_NW(_font);
			}
			attrs->font = font;
		} else {
			attrs->font = font;
			if (isUsingFont()) {
				requestLayout(mode);
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
		
	}
}

void View::setFont(const String& fontFamily, sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, UIUpdateMode mode)
{
	setFont(Font::create(fontFamily, size, flagBold, flagItalic, flagUnderline), mode);
}

void View::setFontAttributes(sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, UIUpdateMode mode)
{
	Ref<Font> font = getFont();
	if (font.isNull()) {
		setFont(Font::create(UI::getDefaultFontFamily(), size, flagBold, flagItalic, flagUnderline), mode);
	} else {
		setFont(Font::create(font->getFamilyName(), size, flagBold, flagItalic, flagUnderline), mode);
	}
}

sl_bool View::isUsingFont()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagUsingFont;
	}
	return sl_false;
}

void View::setUsingFont(sl_bool flag)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagUsingFont = flag;
	}
}

sl_bool View::isOpaque()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagOpaque;
	}
	return sl_false;
}

void View::setOpaque(sl_bool flag, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagOpaque = flag;
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->setOpaque(flag);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidateBoundsInParent();
			}
		}
	}
}

sl_real View::getFinalAlpha()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->alphaStatic * attrs->alphaAnimation;
	}
	return 1;
}

sl_real View::getAlpha()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->alphaStatic;
	}
	return 1;
}

void View::setAlpha(sl_real alpha, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->alphaStatic = alpha;
		_applyFinalAlpha(mode);
	}
}

void View::setAlphaFromAnimation(sl_real alpha, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->alphaAnimation = alpha;
		_applyFinalAlpha(mode);
	}
}

void View::resetAlphaFromAnimation(UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		attrs->alphaAnimation = 1;
		_applyFinalAlpha(mode);
	}
}

void View::_applyFinalAlpha(UIUpdateMode mode)
{
#if defined(SLIB_PLATFORM_IS_WIN32)
	invalidateBoundsInParent();
#else
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setAlpha(getFinalAlpha());
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidateBoundsInParent();
		}
	}
#endif
}

sl_bool View::isLayer()
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagLayer;
	}
	return sl_false;
}

void View::setLayer(sl_bool flagLayer, UIUpdateMode mode)
{
	Ref<DrawAttributes> attrs = _initializeDrawAttributes();
	if (attrs.isNotNull()) {
		attrs->flagLayer = flagLayer;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void View::invalidateLayer()
{
	Ref<DrawAttributes> draw = m_drawAttributes;
	if (draw.isNotNull()) {
		draw->flagInvalidatedLayer = sl_true;
		draw->flagInvalidatedWholeLayer = sl_true;
	}
}

void View::invalidateLayer(const UIRect& rect)
{
	Ref<DrawAttributes> draw = m_drawAttributes;
	if (draw.isNotNull()) {
		if (draw->flagInvalidatedLayer) {
			if (!(draw->flagInvalidatedWholeLayer)) {
				UIRect r = draw->rectInvalidatedLayer;
				r.mergeRectangle(rect);
				draw->rectInvalidatedLayer = r;
			}
		} else {
			draw->rectInvalidatedLayer = rect;
			draw->flagInvalidatedWholeLayer = sl_false;
			draw->flagInvalidatedLayer = sl_true;
		}
	}
}

void View::detachAnimations()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		if (attrs->animationTransform.isNotNull()) {
			Ref<Animation> animation = attrs->animationTransform;
			Ref< AnimationTargetT<Matrix3> > target = attrs->targetTransform;
			if (animation.isNotNull() && target.isNotNull()) {
				animation->removeTarget(target);
			}
		}
		if (attrs->animationTranslation.isNotNull()) {
			Ref<Animation> animation = attrs->animationTranslation;
			Ref< AnimationTargetT<Vector2> > target = attrs->targetTranslation;
			if (animation.isNotNull() && target.isNotNull()) {
				animation->removeTarget(target);
			}
		}
		if (attrs->animationScale.isNotNull()) {
			Ref<Animation> animation = attrs->animationScale;
			Ref< AnimationTargetT<Vector2> > target = attrs->targetScale;
			if (animation.isNotNull() && target.isNotNull()) {
				animation->removeTarget(target);
			}
		}
		if (attrs->animationRotation.isNotNull()) {
			Ref<Animation> animation = attrs->animationRotation;
			Ref< AnimationTargetT<sl_real> > target = attrs->targetRotation;
			if (animation.isNotNull() && target.isNotNull()) {
				animation->removeTarget(target);
			}
		}
		if (attrs->animationAlpha.isNotNull()) {
			Ref<Animation> animation = attrs->animationAlpha;
			Ref< AnimationTargetT<sl_real> > target = attrs->targetAlpha;
			if (animation.isNotNull() && target.isNotNull()) {
				animation->removeTarget(target);
			}
		}
		if (attrs->animationBackgroundColor.isNotNull()) {
			Ref<Animation> animation = attrs->animationBackgroundColor;
			Ref< AnimationTargetT<Color4f> > target = attrs->targetBackgroundColor;
			if (animation.isNotNull() && target.isNotNull()) {
				animation->removeTarget(target);
			}
		}
		m_animationAttributes.setNull();
	}
}

void View::resetAnimations(UIUpdateMode mode)
{
	detachAnimations();
	Ref<TransformAttributes> attrs = m_transformAttributes;
	if (attrs.isNotNull()) {
		attrs->flagTransformAnimation = sl_false;
		attrs->translationAnimation.x = 0;
		attrs->translationAnimation.y = 0;
		attrs->scaleAnimation.x = 1;
		attrs->scaleAnimation.y = 1;
		attrs->rotationAngleAnimation = 0;
		attrs->flagTransformCalcInvalid = sl_true;
		_applyFinalTransform(mode);
	}
	resetAlphaFromAnimation(mode);
	if (mode == UIUpdateMode::Redraw) {
		invalidateBoundsInParent();
	}
}

class _ViewTransformAnimationTarget : public AnimationTargetT<Matrix3>
{
public:
	WeakRef<View> m_view;
	
	_ViewTransformAnimationTarget(const Ref<View>& view, const AnimationFrames<Matrix3>& frames) : AnimationTargetT<Matrix3>(frames), m_view(view)
	{
	}
	
	// override
	void update(float fraction, const Matrix3& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setTransformFromAnimation(value);
		}
	}
	
};

Ref<Animation> View::getTransformAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs->animationTransform;
	}
	return Ref<Animation>::null();
}

void View::setTransformAnimation(const Ref<Animation>& animation, const AnimationFrames<Matrix3>& frames, UIUpdateMode mode)
{
	_resetTransformAnimation();
	setTransformFromAnimation(frames.startValue, mode);
	if (animation.isNotNull()) {
		Ref<AnimationAttributes> attrs = _initializeAnimationAttributes();
		if (attrs.isNotNull()) {
			Ref<_ViewTransformAnimationTarget> target = new _ViewTransformAnimationTarget(this, frames);
			if (target.isNotNull()) {
				attrs->animationTransform = animation;
				attrs->targetTransform = target;
				animation->addTarget(target);
			}
		}
	}
}

void View::setTransformAnimation(const Ref<Animation>& animation, const Matrix3& startValue, const Matrix3& endValue, UIUpdateMode mode)
{
	AnimationFrames<Matrix3> frames(startValue, endValue);
	setTransformAnimation(animation, frames, mode);
}

Ref<Animation> View::startTransformAnimation(const AnimationFrames<Matrix3>& frames, float duration, AnimationCurve curve)
{
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		setTransformAnimation(animation, frames);
		animation->setAnimationCurve(curve);
		animation->start();
		return animation;
	}
	return Ref<Animation>::null();
}

Ref<Animation> View::startTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<Matrix3> frames(startValue, endValue);
	return startTransformAnimation(frames, duration, curve);
}

void View::resetTransformAnimation(UIUpdateMode mode)
{
	_resetTransformAnimation();
	resetTransformFromAnimation(mode);
}

void View::_resetTransformAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		Ref<Animation> animation = attrs->animationTransform;
		Ref< AnimationTargetT<Matrix3> > target = attrs->targetTransform;
		if (animation.isNotNull() && target.isNotNull()) {
			animation->removeTarget(target);
		}
		attrs->animationTransform.setNull();
		attrs->targetTransform.setNull();
	}
}

class _ViewTranslateAnimationTarget : public AnimationTargetT<Vector2>
{
public:
	WeakRef<View> m_view;
	
	_ViewTranslateAnimationTarget(const Ref<View>& view, const AnimationFrames<Vector2>& frames) : AnimationTargetT<Vector2>(frames), m_view(view)
	{
	}
	
	// override
	void update(float fraction, const Vector2& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setTranslationFromAnimation(value);
		}
	}
	
};

Ref<Animation> View::getTranslateAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs->animationTranslation;
	}
	return Ref<Animation>::null();
}

void View::setTranslateAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames, UIUpdateMode mode)
{
	_resetTranslateAnimation();
	setTranslationFromAnimation(frames.startValue, mode);
	if (animation.isNotNull()) {
		Ref<AnimationAttributes> attrs = _initializeAnimationAttributes();
		if (attrs.isNotNull()) {
			Ref<_ViewTranslateAnimationTarget> target = new _ViewTranslateAnimationTarget(this, frames);
			if (target.isNotNull()) {
				attrs->animationTranslation = animation;
				attrs->targetTranslation = target;
				animation->addTarget(target);
			}
		}
	}
}

void View::setTranslateAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue, UIUpdateMode mode)
{
	AnimationFrames<Vector2> frames(startValue, endValue);
	setTranslateAnimation(animation, frames, mode);
}

Ref<Animation> View::startTranslateAnimation(const AnimationFrames<Vector2>& frames, float duration, AnimationCurve curve)
{
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		setTranslateAnimation(animation, frames);
		animation->setAnimationCurve(curve);
		animation->start();
		return animation;
	}
	return Ref<Animation>::null();
}

Ref<Animation> View::startTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<Vector2> frames(startValue, endValue);
	return startTranslateAnimation(frames, duration, curve);
}

void View::resetTranslateAnimation(UIUpdateMode mode)
{
	_resetTranslateAnimation();
	resetTranslationFromAnimation(mode);
}

void View::_resetTranslateAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		Ref<Animation> animation = attrs->animationTranslation;
		Ref< AnimationTargetT<Vector2> > target = attrs->targetTranslation;
		if (animation.isNotNull() && target.isNotNull()) {
			animation->removeTarget(target);
		}
		attrs->animationTranslation.setNull();
		attrs->targetTranslation.setNull();
	}
}

class _ViewScaleAnimationTarget : public AnimationTargetT<Vector2>
{
public:
	WeakRef<View> m_view;
	
	_ViewScaleAnimationTarget(const Ref<View>& view, const AnimationFrames<Vector2>& frames) : AnimationTargetT<Vector2>(frames), m_view(view)
	{
	}
	
	// override
	void update(float fraction, const Vector2& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setScaleFromAnimation(value);
		}
	}
	
};

Ref<Animation> View::getScaleAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs->animationScale;
	}
	return Ref<Animation>::null();
}

void View::setScaleAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames, UIUpdateMode mode)
{
	_resetScaleAnimation();
	setScaleFromAnimation(frames.startValue, mode);
	if (animation.isNotNull()) {
		Ref<AnimationAttributes> attrs = _initializeAnimationAttributes();
		if (attrs.isNotNull()) {
			Ref<_ViewScaleAnimationTarget> target = new _ViewScaleAnimationTarget(this, frames);
			if (target.isNotNull()) {
				attrs->animationScale = animation;
				attrs->targetScale = target;
				animation->addTarget(target);
			}
		}
	}
}

void View::setScaleAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue, UIUpdateMode mode)
{
	AnimationFrames<Vector2> frames(startValue, endValue);
	setScaleAnimation(animation, frames, mode);
}

void View::setScaleAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue, UIUpdateMode mode)
{
	AnimationFrames<Vector2> frames(Vector2(startValue, startValue), Vector2(endValue, endValue));
	setScaleAnimation(animation, frames, mode);
}

Ref<Animation> View::startScaleAnimation(const AnimationFrames<Vector2>& frames, float duration, AnimationCurve curve)
{
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		setScaleAnimation(animation, frames);
		animation->setAnimationCurve(curve);
		animation->start();
		return animation;
	}
	return Ref<Animation>::null();
}

Ref<Animation> View::startScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<Vector2> frames(startValue, endValue);
	return startScaleAnimation(frames, duration, curve);
}

Ref<Animation> View::startScaleAnimation(sl_real startValue, sl_real endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<Vector2> frames(Vector2(startValue, startValue), Vector2(endValue, endValue));
	return startScaleAnimation(frames, duration, curve);
}

void View::resetScaleAnimation(UIUpdateMode mode)
{
	_resetScaleAnimation();
	resetScaleFromAnimation(mode);
}

void View::_resetScaleAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		Ref<Animation> animation = attrs->animationScale;
		Ref< AnimationTargetT<Vector2> > target = attrs->targetScale;
		if (animation.isNotNull() && target.isNotNull()) {
			animation->removeTarget(target);
		}
		attrs->animationScale.setNull();
		attrs->targetScale.setNull();
	}
}

class _ViewRotateAnimationTarget : public AnimationTargetT<sl_real>
{
public:
	WeakRef<View> m_view;
	
	_ViewRotateAnimationTarget(const Ref<View>& view, const AnimationFrames<sl_real>& frames) : AnimationTargetT<sl_real>(frames), m_view(view)
	{
	}
	
	// override
	void update(float fraction, const sl_real& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setRotationFromAnimation(value);
		}
	}
	
};

Ref<Animation> View::getRotateAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs->animationRotation;
	}
	return Ref<Animation>::null();
}

void View::setRotateAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames, UIUpdateMode mode)
{
	_resetRotateAnimation();
	setRotationFromAnimation(frames.startValue, mode);
	if (animation.isNotNull()) {
		Ref<AnimationAttributes> attrs = _initializeAnimationAttributes();
		if (attrs.isNotNull()) {
			Ref<_ViewRotateAnimationTarget> target = new _ViewRotateAnimationTarget(this, frames);
			if (target.isNotNull()) {
				attrs->animationRotation = animation;
				attrs->targetRotation = target;
				animation->addTarget(target);
			}
		}
	}
}

void View::setRotateAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue, UIUpdateMode mode)
{
	AnimationFrames<sl_real> frames(startValue, endValue);
	setRotateAnimation(animation, frames, mode);
}

Ref<Animation> View::startRotateAnimation(const AnimationFrames<sl_real>& frames, float duration, AnimationCurve curve)
{
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		setRotateAnimation(animation, frames);
		animation->setAnimationCurve(curve);
		animation->start();
		return animation;
	}
	return Ref<Animation>::null();
}

Ref<Animation> View::startRotateAnimation(sl_real startValue, sl_real endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<sl_real> frames(startValue, endValue);
	return startRotateAnimation(frames, duration, curve);
}

void View::resetRotateAnimation(UIUpdateMode mode)
{
	_resetRotateAnimation();
	resetRotationFromAnimation(mode);
}

void View::_resetRotateAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		Ref<Animation> animation = attrs->animationRotation;
		Ref< AnimationTargetT<sl_real> > target = attrs->targetRotation;
		if (animation.isNotNull() && target.isNotNull()) {
			animation->removeTarget(target);
		}
		attrs->animationRotation.setNull();
		attrs->targetRotation.setNull();
	}
}

class _ViewAlphaAnimationTarget : public AnimationTargetT<sl_real>
{
public:
	WeakRef<View> m_view;
	
	_ViewAlphaAnimationTarget(const Ref<View>& view, const AnimationFrames<sl_real>& frames) : AnimationTargetT<sl_real>(frames), m_view(view)
	{
	}
	
	// override
	void update(float fraction, const sl_real& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setAlphaFromAnimation(value);
		}
	}
	
};

Ref<Animation> View::getAlphaAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs->animationAlpha;
	}
	return Ref<Animation>::null();
}

void View::setAlphaAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames, UIUpdateMode mode)
{
	_resetAlphaAnimation();
	setAlphaFromAnimation(frames.startValue, mode);
	if (animation.isNotNull()) {
		Ref<AnimationAttributes> attrs = _initializeAnimationAttributes();
		if (attrs.isNotNull()) {
			Ref<_ViewAlphaAnimationTarget> target = new _ViewAlphaAnimationTarget(this, frames);
			if (target.isNotNull()) {
				attrs->animationAlpha = animation;
				attrs->targetAlpha = target;
				animation->addTarget(target);
			}
		}
	}
}

void View::setAlphaAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue, UIUpdateMode mode)
{
	AnimationFrames<sl_real> frames(startValue, endValue);
	setAlphaAnimation(animation, frames, mode);
}

Ref<Animation> View::startAlphaAnimation(const AnimationFrames<sl_real>& frames, float duration, AnimationCurve curve)
{
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		setAlphaAnimation(animation, frames);
		animation->setAnimationCurve(curve);
		animation->start();
		return animation;
	}
	return Ref<Animation>::null();
}

Ref<Animation> View::startAlphaAnimation(sl_real startValue, sl_real endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<sl_real> frames(startValue, endValue);
	return startAlphaAnimation(frames, duration, curve);
}

void View::resetAlphaAnimation(UIUpdateMode mode)
{
	_resetAlphaAnimation();
	resetAlphaFromAnimation(mode);
}

void View::_resetAlphaAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		Ref<Animation> animation = attrs->animationAlpha;
		Ref< AnimationTargetT<sl_real> > target = attrs->targetAlpha;
		if (animation.isNotNull() && target.isNotNull()) {
			animation->removeTarget(target);
		}
		attrs->animationAlpha.setNull();
		attrs->targetAlpha.setNull();
	}
}

class _ViewBackgroundColorAnimationTarget : public AnimationTargetT<Color4f>
{
public:
	WeakRef<View> m_view;
	
	_ViewBackgroundColorAnimationTarget(const Ref<View>& view, const AnimationFrames<Color4f>& frames) : AnimationTargetT<Color4f>(frames), m_view(view)
	{
	}
	
	// override
	void update(float fraction, const Color4f& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setBackgroundColor(value);
		}
	}
	
};

Ref<Animation> View::getBackgroundColorAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		return attrs->animationBackgroundColor;
	}
	return Ref<Animation>::null();
}

void View::setBackgroundColorAnimation(const Ref<Animation>& animation, const AnimationFrames<Color4f>& frames, UIUpdateMode mode)
{
	_resetBackgroundColorAnimation();
	setBackgroundColor(frames.startValue, mode);
	if (animation.isNotNull()) {
		Ref<AnimationAttributes> attrs = _initializeAnimationAttributes();
		if (attrs.isNotNull()) {
			Ref<_ViewBackgroundColorAnimationTarget> target = new _ViewBackgroundColorAnimationTarget(this, frames);
			if (target.isNotNull()) {
				attrs->animationBackgroundColor = animation;
				attrs->targetBackgroundColor = target;
				animation->addTarget(target);
			}
		}
	}
}

void View::setBackgroundColorAnimation(const Ref<Animation>& animation, const Color4f& startValue, const Color4f& endValue, UIUpdateMode mode)
{
	AnimationFrames<Color4f> frames(startValue, endValue);
	setBackgroundColorAnimation(animation, frames, mode);
}

Ref<Animation> View::startBackgroundColorAnimation(const AnimationFrames<Color4f>& frames, float duration, AnimationCurve curve)
{
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		setBackgroundColorAnimation(animation, frames);
		animation->setAnimationCurve(curve);
		animation->start();
		return animation;
	}
	return Ref<Animation>::null();
}

Ref<Animation> View::startBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, AnimationCurve curve)
{
	AnimationFrames<Color4f> frames(startValue, endValue);
	return startBackgroundColorAnimation(frames, duration, curve);
}

void View::resetBackgroundColorAnimation(UIUpdateMode mode)
{
	_resetBackgroundColorAnimation();
}

void View::_resetBackgroundColorAnimation()
{
	Ref<AnimationAttributes> attrs = m_animationAttributes;
	if (attrs.isNotNull()) {
		Ref<Animation> animation = attrs->animationBackgroundColor;
		Ref< AnimationTargetT<Color4f> > target = attrs->targetBackgroundColor;
		if (animation.isNotNull() && target.isNotNull()) {
			animation->removeTarget(target);
		}
		attrs->animationBackgroundColor.setNull();
		attrs->targetBackgroundColor.setNull();
	}
}

Ref<ScrollBar> View::getHorizontalScrollBar()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->horz;
	}
	return Ref<ScrollBar>::null();
}

Ref<ScrollBar> View::getVerticalScrollBar()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->vert;
	}
	return Ref<ScrollBar>::null();
}

void View::setHorizontalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		removeChild(attrs->horz, UIUpdateMode::NoRedraw);
		attrs->horz = bar;
		refreshScroll(mode);
	}
}

void View::setVerticalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		removeChild(attrs->vert, UIUpdateMode::NoRedraw);
		attrs->vert = bar;
		refreshScroll(mode);
	}
}

void View::createHorizontalScrollBar(UIUpdateMode mode)
{
	setHorizontalScrollBar(_createHorizontalScrollBar(), mode);
}

void View::createVerticalScrollBar(UIUpdateMode mode)
{
	setVerticalScrollBar(_createVerticalScrollBar(), mode);
}

void View::removeHorizontalScrollBar(UIUpdateMode mode)
{
	setHorizontalScrollBar(Ref<ScrollBar>::null(), mode);
}

void View::removeVerticalScrollBar(UIUpdateMode mode)
{
	setVerticalScrollBar(Ref<ScrollBar>::null(), mode);
}

void View::createScrollBars(UIUpdateMode mode)
{
	setHorizontalScrollBar(_createHorizontalScrollBar(), UIUpdateMode::Init);
	setVerticalScrollBar(_createVerticalScrollBar(), UIUpdateMode::Init);
	refreshScroll(mode);
}

void View::removeScrollBars(UIUpdateMode mode)
{
	setHorizontalScrollBar(Ref<ScrollBar>::null(), UIUpdateMode::Init);
	setVerticalScrollBar(Ref<ScrollBar>::null(), UIUpdateMode::Init);
	refreshScroll(mode);
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

void View::setHorizontalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode)
{
	Ref<ScrollBar> bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, UIUpdateMode::NoRedraw);
	}
	refreshScroll(mode);
}

void View::setVerticalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode)
{
	Ref<ScrollBar> bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, UIUpdateMode::NoRedraw);
	}
	refreshScroll(mode);
}

void View::setScrollBarsVisible(sl_bool flagVisible, UIUpdateMode mode)
{
	Ref<ScrollBar> bar;
	bar = getHorizontalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, UIUpdateMode::NoRedraw);
	}
	bar = getVerticalScrollBar();
	if (bar.isNotNull()) {
		bar->setVisible(flagVisible, UIUpdateMode::NoRedraw);
	}
	refreshScroll(mode);
}

sl_scroll_pos View::getScrollX()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->x;
	}
	return 0;
}

sl_scroll_pos View::getScrollY()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->y;
	}
	return 0;
}

ScrollPoint View::getScrollPosition()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return ScrollPoint(attrs->x, attrs->y);
	}
	return ScrollPoint::zero();
}

void View::scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	
	if (attrs.isNotNull()) {
		
		sl_scroll_pos rx = attrs->contentWidth - getWidth();
		if (x > rx) {
			x = rx;
		}
		if (x < 0) {
			x = 0;
		}
		sl_scroll_pos ry = attrs->contentHeight - getHeight();
		if (y > ry) {
			y = ry;
		}
		if (y < 0) {
			y = 0;
		}
		
		if (Math::isAlmostZero(attrs->x - x) && Math::isAlmostZero(attrs->y - y)) {
			attrs->x = x;
			attrs->y = y;
			return;
		}
		
		attrs->x = x;
		attrs->y = y;
		
		dispatchScroll(x, y);
		
		Ref<ScrollBar> bar = attrs->horz;
		if (bar.isNotNull()) {
			bar->setValueOfOutRange(x, UIUpdateMode::NoRedraw);
		}
		bar = attrs->vert;
		if (bar.isNotNull()) {
			bar->setValueOfOutRange(y, UIUpdateMode::NoRedraw);
		}
		
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void View::scrollTo(const ScrollPoint& position, UIUpdateMode mode)
{
	scrollTo(position.x, position.y, mode);
}

void View::setScrollX(sl_scroll_pos x, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		scrollTo(x, attrs->y, mode);
	}
}

void View::setScrollY(sl_scroll_pos y, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		scrollTo(attrs->x, y, mode);
	}
}

sl_scroll_pos View::getContentWidth()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->contentWidth;
	}
	return 0;
}

sl_scroll_pos View::getContentHeight()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->contentHeight;
	}
	return 0;
}

ScrollPoint View::getContentSize()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return ScrollPoint(attrs->contentWidth, attrs->contentHeight);
	}
	return ScrollPoint::zero();
}

void View::setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		if (Math::isAlmostZero(width - attrs->contentWidth) && Math::isAlmostZero(height - attrs->contentHeight)) {
			attrs->contentWidth = width;
			attrs->contentHeight = height;
			return;
		}
		attrs->contentWidth = width;
		attrs->contentHeight = height;
		onResizeContent(width, height);
		refreshScroll(mode);
	}
}

void View::setContentSize(const ScrollPoint& size, UIUpdateMode mode)
{
	setContentSize(size.x, size.y, mode);
}

void View::setContentWidth(sl_scroll_pos width, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		setContentSize(width, attrs->contentHeight, mode);
	}
}

void View::setContentHeight(sl_scroll_pos height, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		setContentSize(attrs->contentWidth, height, mode);
	}
}

ScrollPoint View::getScrollRange()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		ScrollPoint ret(attrs->contentWidth - (sl_scroll_pos)(getWidth()), attrs->contentHeight - (sl_scroll_pos)(getHeight()));
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
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->barWidth;
	}
	return 0;
}

void View::setScrollBarWidth(sl_ui_len width, UIUpdateMode mode)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		attrs->barWidth = width;
		refreshScroll(mode);
	}
}

sl_bool View::isContentScrollingByMouse()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagContentScrollingByMouse;
	}
	return sl_true;
}

void View::setContentScrollingByMouse(sl_bool flag)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		attrs->flagContentScrollingByMouse = flag;
	}
}

sl_bool View::isContentScrollingByTouch()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagContentScrollingByTouch;
	}
	return sl_true;
}

void View::setContentScrollingByTouch(sl_bool flag)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		attrs->flagContentScrollingByTouch = flag;
	}
}

sl_bool View::isContentScrollingByMouseWheel()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagContentScrollingByMouseWheel;
	}
	return sl_true;
}

void View::setContentScrollingByMouseWheel(sl_bool flag)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		attrs->flagContentScrollingByMouseWheel = flag;
	}
}

sl_bool View::isContentScrollingByKeyboard()
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		return attrs->flagContentScrollingByKeyboard;
	}
	return sl_true;
}

void View::setContentScrollingByKeyboard(sl_bool flag)
{
	Ref<ScrollAttributes> attrs = _initializeScrollAttributes();
	if (attrs.isNotNull()) {
		attrs->flagContentScrollingByKeyboard = flag;
	}
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

void View::refreshScroll(UIUpdateMode mode)
{
	if (mode == UIUpdateMode::Init) {
		return;
	}
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		sl_ui_pos width = getWidth();
		sl_ui_pos height = getHeight();
		sl_bool flagVisibleHorz = sl_false;
		Ref<ScrollBar> barHorz = attrs->horz;
		if (barHorz.isNotNull()) {
			barHorz->setParent(this);
			barHorz->setMinimumValue(0, UIUpdateMode::NoRedraw);
			barHorz->setMaximumValue(attrs->contentWidth, UIUpdateMode::NoRedraw);
			barHorz->setPage(width, UIUpdateMode::NoRedraw);
			barHorz->setValueOfOutRange(attrs->x, UIUpdateMode::NoRedraw);
			barHorz->setFrame(UIRect(0, height - attrs->barWidth, width, height), UIUpdateMode::NoRedraw);
			if (barHorz->getListener().isNull()) {
				barHorz->setListener(Ref<_View_ScrollBarListener>(new _View_ScrollBarListener(this)));
			}
			attrs->flagValidHorz = barHorz->isValid();
			if (barHorz->isVisible() && attrs->flagValidHorz) {
				flagVisibleHorz = sl_true;
			}
		}
		sl_bool flagVisibleVert = sl_false;
		Ref<ScrollBar> barVert = attrs->vert;
		if (barVert.isNotNull()) {
			barVert->setParent(this);
			barVert->setVertical(UIUpdateMode::NoRedraw);
			barVert->setMinimumValue(0, UIUpdateMode::NoRedraw);
			barVert->setMaximumValue(attrs->contentHeight, UIUpdateMode::NoRedraw);
			barVert->setPage(height, UIUpdateMode::NoRedraw);
			barVert->setValueOfOutRange(attrs->y, UIUpdateMode::NoRedraw);
			barVert->setFrame(UIRect(width - attrs->barWidth, 0, width, height), UIUpdateMode::NoRedraw);
			if (barVert->getListener().isNull()) {
				barVert->setListener(Ref<_View_ScrollBarListener>(new _View_ScrollBarListener(this)));
			}
			attrs->flagValidVert = barVert->isValid();
			if (barVert->isVisible() && attrs->flagValidVert) {
				flagVisibleVert = sl_true;
			}
		}
		if (flagVisibleHorz && flagVisibleVert) {
			//// avoid overlapping two scrollbars.
			////		2016/6/8 temporally cancel this function
			//barHorz->setFrame(UIRect(0, height - scroll->barWidth, width - scroll->barWidth, height), sl_false);
		}
		scrollTo(attrs->x, attrs->y, UIUpdateMode::NoRedraw);
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
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


void View::_getScrollBars(Ref<View> views[2])
{
	Ref<ScrollAttributes> attrs = m_scrollAttributes;
	if (attrs.isNotNull()) {
		if (attrs->flagValidHorz) {
			Ref<ScrollBar> bar = attrs->horz;
			if (bar.isNotNull()) {
				if (bar->isVisible()) {
					views[0] = bar;
				}
			}
		}
		if (attrs->flagValidVert) {
			Ref<ScrollBar> bar = attrs->vert;
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

sl_bool View::isCapturingChildInstanceEvents()
{
	return m_flagCapturingChildInstanceEvents;
}

void View::setCapturingChildInstanceEvents(sl_bool flag)
{
	m_flagCapturingChildInstanceEvents = flag;
}

sl_bool View::hitTestForCapturingChildInstanceEvents(const UIPoint& pt)
{
	return sl_true;
}

void View::drawBackground(Canvas* canvas, const Color& color, const Ref<Drawable>& background)
{
	Rectangle rc(0, 0, (sl_real)(m_frame.getWidth()), (sl_real)(m_frame.getHeight()));
	if (color.a > 0) {
		Ref<Brush> brush = Brush::createSolidBrush(color);
		canvas->fillRectangle(rc, brush);
	}
	if (background.isNotNull()) {
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
	if (count == 0) {
		return;
	}
	
	sl_real alphaParent = canvas->getAlpha();
	UIRect rcInvalidatedParent = canvas->getInvalidatedRect();
	
	for (sl_size i = 0; i < count; i++) {
		View* child = children[i].ptr;
		if (child) {
			if (child->isVisible()) {
				if (child->isInstance()) {
#if defined(SLIB_PLATFORM_IS_WIN32)
					if (!(child->checkSelfInvalidatable())) {
						CanvasStateScope scope(canvas);
						sl_ui_pos offx = child->m_frame.left;
						sl_ui_pos offy = child->m_frame.top;
						Vector2 t;
						if (child->getFinalTranslationRotationScale(&t, sl_null, sl_null, sl_null)) {
							offx += (sl_ui_pos)(t.x);
							offy += (sl_ui_pos)(t.y);
						}
						UIRect rcInvalidated(rcInvalidatedParent.left - offx, rcInvalidatedParent.top - offy, rcInvalidatedParent.right - offx, rcInvalidatedParent.bottom - offy);
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
							canvas->setInvalidatedRect(rcInvalidated);
							canvas->translate((sl_real)(offx), (sl_real)(offy));
							canvas->setAlpha(alphaParent * child->getFinalAlpha());
							child->dispatchDraw(canvas);
						}
					}
#endif
				} else {
					CanvasStateScope scope(canvas);
					sl_ui_pos offx = child->m_frame.left;
					sl_ui_pos offy = child->m_frame.top;
					Matrix3 mat;
					if (child->getFinalTransform(&mat)) {
						UIRect rcInvalidated = child->convertCoordinateFromParent(rcInvalidatedParent);
						rcInvalidated.left -= 1;
						rcInvalidated.top -= 1;
						rcInvalidated.right += 1;
						rcInvalidated.bottom += 1;
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
							canvas->clipToRectangle(rcInvalidatedParent);
							canvas->setInvalidatedRect(rcInvalidated);
							sl_real ax = (sl_real)(child->getWidth()) / 2;
							sl_real ay = (sl_real)(child->getHeight()) / 2;
							mat.m20 = -ax * mat.m00 - ay * mat.m10 + mat.m20 + ax + (sl_real)(offx);
							mat.m21 = -ax * mat.m01 - ay * mat.m11 + mat.m21 + ay + (sl_real)(offy);
							canvas->concatMatrix(mat);
							canvas->setAlpha(alphaParent * child->getFinalAlpha());
							child->dispatchDraw(canvas);
						}
					} else {
						UIRect rcInvalidated(rcInvalidatedParent.left - offx, rcInvalidatedParent.top - offy, rcInvalidatedParent.right - offx, rcInvalidatedParent.bottom - offy);
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
							canvas->setInvalidatedRect(rcInvalidated);
							canvas->translate((sl_real)(offx), (sl_real)(offy));
							canvas->setAlpha(alphaParent * child->getFinalAlpha());
							child->dispatchDraw(canvas);
						}
					}
				}
			}
		}
	}
	canvas->setAlpha(alphaParent);
	canvas->setInvalidatedRect(rcInvalidatedParent);
}

void View::drawContent(Canvas *canvas)
{
	sl_uint32 width = getWidth();
	sl_uint32 height = getHeight();
	
	Ref<DrawAttributes> drawAttrs = m_drawAttributes;

	if (drawAttrs.isNotNull()) {
		Rectangle rcClip(0, 0, (sl_real)width, (sl_real)height);
		switch (getBoundShape()) {
			case BoundShape::RoundRect:
				rcClip.right -= 1;
				rcClip.bottom -= 1;
				rcClip.fixSizeError();
				canvas->clipToRoundRect(rcClip, getRoundRectBoundShapeRadius());
				break;
			case BoundShape::Ellipse:
				rcClip.right -= 1;
				rcClip.bottom -= 1;
				rcClip.fixSizeError();
				canvas->clipToEllipse(rcClip);
				break;
			case BoundShape::Path:
				canvas->clipToPath(getBoundShapePath());
				break;
			default:
				break;
		}
	}
	
	do {
		
		{
			CanvasStateScope scope(canvas);
			canvas->clipToRectangle(canvas->getInvalidatedRect());
			if (drawAttrs.isNotNull()) {
				if (drawAttrs->flagOnDrawBackgroundAlways || drawAttrs->backgroundColor.isNotZero() || drawAttrs->background.isNotNull()) {
					onDrawBackground(canvas);
				}
			}
			Ref<ScrollAttributes> scrollAttrs = m_scrollAttributes;
			if (scrollAttrs.isNotNull()) {
				sl_real scrollX = (sl_real)(scrollAttrs->x);
				sl_real scrollY = (sl_real)(scrollAttrs->y);
				if(!(Math::isAlmostZero(scrollX)) || !(Math::isAlmostZero(scrollY))) {
					canvas->translate(-scrollX, -scrollY);
				}
			}
			onDraw(canvas);
		}
		
		if (m_children.getCount() > 0) {
			onDrawChildren(canvas);
		}

	} while (0);
	
}

#define MAX_LAYER_SIZE 8192

Ref<Bitmap> View::drawLayer()
{
	Ref<DrawAttributes> drawAttrs = _initializeDrawAttributes();
	if (drawAttrs.isNull()) {
		return Ref<Bitmap>::null();
	}
	
	sl_uint32 width = getWidth();
	sl_uint32 height = getHeight();
	
	if (width == 0 || height == 0 || width > MAX_LAYER_SIZE || height > MAX_LAYER_SIZE) {
		return Ref<Bitmap>::null();
	}

	sl_bool flagInvalidate = drawAttrs->flagInvalidatedLayer;
	sl_bool flagInvalidateWhole = drawAttrs->flagInvalidatedWholeLayer;
	
	Ref<Bitmap> bitmap = drawAttrs->bitmapLayer;
	Ref<Canvas> canvas = drawAttrs->canvasLayer;
	
	if (bitmap.isNull() || bitmap->getWidth() < width || bitmap->getHeight() < height) {
		bitmap = Bitmap::create((width + 255) & 0xFFFFFF00, (height + 255) & 0xFFFFFF00);
		if (bitmap.isNull()) {
			return Ref<Bitmap>::null();
		}
		canvas = bitmap->getCanvas();
		if (canvas.isNull()) {
			return Ref<Bitmap>::null();
		}
		drawAttrs->bitmapLayer = bitmap;
		drawAttrs->canvasLayer = canvas;
		flagInvalidate = sl_true;
		flagInvalidateWhole = sl_true;
	}
	
	if (!flagInvalidate) {
		return bitmap;
	}
	
	drawAttrs->flagInvalidatedLayer = sl_false;
	
	UIRect rc;
	if (flagInvalidateWhole) {
		flagInvalidateWhole = sl_false;
		rc.left = 0;
		rc.top = 0;
		rc.right = width;
		rc.bottom = height;
		canvas->setInvalidatedRect(Rectangle(0, 0, (sl_real)width, (sl_real)height));
	} else {
		rc = drawAttrs->rectInvalidatedLayer;
		canvas->setInvalidatedRect(rc);
	}
	
	do {
		if (isOpaque()) {
			break;
		}
		if (drawAttrs->backgroundColor.a == 255) {
			break;
		}
		bitmap->resetPixels((sl_uint32)(rc.left), (sl_uint32)(rc.top), (sl_uint32)(rc.getWidth()), (sl_uint32)(rc.getHeight()), Color::zero());
	} while (0);

	CanvasStateScope scope(canvas);
	drawContent(canvas.ptr);

	return bitmap;
	
}

void View::draw(Canvas* canvas)
{

	if (isLayer()) {
		Ref<Bitmap> bitmap = drawLayer();
		if (bitmap.isNotNull()) {
			Rectangle rcInvalidated = canvas->getInvalidatedRect();
			canvas->draw(rcInvalidated, bitmap, rcInvalidated);
			return;
		}
	}
	
	CanvasStateScope scope(canvas);
	drawContent(canvas);
	
}

void View::post(const Function<void()>& callback, sl_bool flagInvalidate)
{
	if (callback.isNotNull()) {
		m_postCallbacks.push(callback);
		if (flagInvalidate) {
			invalidate();
		}
	}
}

void View::dispatchDraw(Canvas* canvas)
{
	m_flagCurrentDrawing = sl_true;
	{
		Rectangle rcInvalidated = canvas->getInvalidatedRect();
		if (rcInvalidated.right < rcInvalidated.left + SLIB_EPSILON) {
			return;
		}
		if (rcInvalidated.bottom < rcInvalidated.top + SLIB_EPSILON) {
			return;
		}
		m_rectCurrentDrawing.left = (sl_ui_pos)(rcInvalidated.left - SLIB_EPSILON);
		m_rectCurrentDrawing.top = (sl_ui_pos)(rcInvalidated.top - SLIB_EPSILON);
		m_rectCurrentDrawing.right = (sl_ui_pos)(rcInvalidated.right + SLIB_EPSILON);
		m_rectCurrentDrawing.bottom = (sl_ui_pos)(rcInvalidated.bottom + SLIB_EPSILON);
	}
	
	_makeLayout(sl_false);
	
	Ref<DrawAttributes> drawAttrs = m_drawAttributes;
	if (drawAttrs.isNotNull() && drawAttrs->flagPreDrawEnabled) {
		onPreDraw(canvas);
	}
	
	draw(canvas);
	
	Ref<View> scrollBars[2];
	_getScrollBars(scrollBars);
	if (scrollBars[0].isNotNull() || scrollBars[1].isNotNull()) {
		drawChildren(canvas, scrollBars, 2);
	}

	if (drawAttrs.isNotNull()) {
		if (drawAttrs->flagOnDrawBorderAlways || drawAttrs->penBorder.isNotNull()) {
			onDrawBorder(canvas);
		}
		if (drawAttrs->flagPostDrawEnabled) {
			onPostDraw(canvas);
		}
	}

	m_flagCurrentDrawing = sl_false;
	
	Function<void()> callback;
	while (m_postCallbacks.pop(&callback)) {
		callback();
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
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		Ref<Drawable> background;
		if (isPressedState()) {
			background = attrs->backgroundPressed;
		} else if (isHoverState()) {
			background = attrs->backgroundHover;
		}
		if (background.isNull()) {
			background = attrs->background;
		}
		drawBackground(canvas, attrs->backgroundColor, background);
	}
}

void View::onDrawBorder(Canvas* canvas)
{
	Ref<DrawAttributes> attrs = m_drawAttributes;
	if (attrs.isNotNull()) {
		drawBorder(canvas, attrs->penBorder);
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
				m_childMouseMove.setNull();
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
	
	ev->resetStates();
	
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
		ev->resetStates();
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
	
	ev->resetStates();
	
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
		
		ev->resetStates();
		
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
	
	ev->resetStates();
	
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
		ev->resetStates();
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
	
	ev->resetStates();
	
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
	(getOnClick())();
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
	
	ev->resetStates();
	
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
		ev->resetStates();
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
	refreshScroll(UIUpdateMode::NoRedraw);
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
	switch (action) {
		case UIAction::LeftButtonDown:
		case UIAction::TouchBegin:
			setPressedState(sl_true);
			if (m_flagOccurringClick) {
				ev->stopPropagation();
			}
			break;
		case UIAction::LeftButtonUp:
		case UIAction::TouchEnd:
			if (isPressedState()) {
				setPressedState(sl_false);
				if (m_flagOccurringClick) {
					if (getBounds().containsPoint(ev->getPoint())) {
						dispatchClick(ev);
					}
				}
			}
			break;
		case UIAction::TouchCancel:
			setPressedState(sl_false);
			break;
		case UIAction::MouseEnter:
			setHoverState(sl_true);
			break;
		case UIAction::MouseLeave:
			setHoverState(sl_false);
			break;
		default:
			break;
	}
}

void View::_processContentScrollingEvents(UIEvent* ev)
{
	Ref<ScrollAttributes> scrollAttrs = m_scrollAttributes;
	if (scrollAttrs.isNull()) {
		return;
	}
	if (scrollAttrs->contentWidth < (sl_scroll_pos)(getWidth()) && scrollAttrs->contentHeight < (sl_scroll_pos)(getHeight())) {
		return;
	}
	sl_scroll_pos lineX = (sl_scroll_pos)(getWidth() / 20);
	sl_scroll_pos lineY = (sl_scroll_pos)(getHeight() / 20);
	UIAction action = ev->getAction();
	switch (action) {
		case UIAction::LeftButtonDown:
		case UIAction::TouchBegin:
			scrollAttrs->flagDownContent = sl_true;
			scrollAttrs->scrollX_DownContent = scrollAttrs->x;
			scrollAttrs->scrollY_DownContent = scrollAttrs->y;
			scrollAttrs->mouseX_DownContent = ev->getX();
			scrollAttrs->mouseY_DownContent = ev->getY();
			ev->stopPropagation();
			break;
		case UIAction::LeftButtonDrag:
		case UIAction::TouchMove:
			if (scrollAttrs->flagDownContent) {
				sl_scroll_pos sx = scrollAttrs->scrollX_DownContent - (sl_scroll_pos)(ev->getX() - scrollAttrs->mouseX_DownContent);
				sl_scroll_pos sy = scrollAttrs->scrollY_DownContent - (sl_scroll_pos)(ev->getY() - scrollAttrs->mouseY_DownContent);
				scrollTo(sx, sy);
				ev->stopPropagation();
			}
			break;
		case UIAction::LeftButtonUp:
		case UIAction::TouchEnd:
			if (scrollAttrs->flagDownContent) {
				scrollAttrs->flagDownContent = sl_false;
				ev->stopPropagation();
			}
			break;
		case UIAction::MouseWheel:
			{
				sl_bool flagChange = sl_false;
				sl_scroll_pos sx = scrollAttrs->x;
				sl_scroll_pos sy = scrollAttrs->y;
				
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
				sl_scroll_pos sx = scrollAttrs->x;
				sl_scroll_pos sy = scrollAttrs->y;
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


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))
Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}
#endif

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_WIN32))
void View::_setFrame_NI(const UIRect& frame)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setFrame(frame);
	}
}
#endif

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))
void View::_setTransform_NI(const Matrix3& matrix)
{
	Ref<ViewInstance> instance = m_instance;
	if (instance.isNotNull()) {
		instance->setTransform(matrix);
	}
}
#endif

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
#if defined(SLIB_PLATFORM_IS_OSX)
		sl_real alpha = view->getFinalAlpha();
		Ref<View> parent = view->getParent();
		while (parent.isNotNull()) {
			alpha *= parent->getFinalAlpha();
			parent = parent->getParent();
		}
		if (alpha < 0.005f) {
			return;
		}
		if (alpha < 0.995f) {
			canvas->setAlpha(alpha);
		}
#endif
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

ViewGroup::ViewGroup()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setCreatingChildInstances(sl_true);
	setMakingLayout(sl_true, UIUpdateMode::Init);
	
}

SLIB_UI_NAMESPACE_END
