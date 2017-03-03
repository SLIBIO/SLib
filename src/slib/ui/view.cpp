#include "../../../inc/slib/ui/view.h"

#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/scroll_bar.h"
#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/ui/animation.h"
#include "../../../inc/slib/ui/resource.h"

#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/math/transform2d.h"
#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/util.h"
#include "../../../inc/slib/render/canvas.h"

#include "ui_animation.h"

namespace slib
{

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
		m_flagInstanceLayer = sl_false;
		m_flagClipping = sl_false;
		m_flagDrawing = sl_true;
		m_flagSavingCanvasState = sl_true;
		
		m_flagFocused = sl_false;
		m_flagPressed = sl_false;
		m_flagHover = sl_false;
		
		m_flagCurrentDrawing = sl_false;

		m_actionMouseDown = UIAction::Unknown;
		m_flagTouchMultipleChildren = sl_false;
		m_flagPassEventToChildren = sl_true;
		m_flagProcessingOkCancel = sl_true;
		m_flagProcessingTabStop = sl_true;
		
		m_flagCapturingChildInstanceEvents = sl_false;
		
		m_flagOnAddChild = sl_false;
		m_flagOnRemoveChild = sl_false;
		
		m_flagInvalidLayout = sl_true;
		m_paddingLeft = 0;
		m_paddingTop = 0;
		m_paddingRight = 0;
		m_paddingBottom = 0;
		
	}

	View::~View()
	{
		SLIB_REFERABLE_DESTRUCTOR
		detach();
	}

#define DEFAULT_MAX_SIZE 0x3fffffff

	View::LayoutAttributes::LayoutAttributes()
	{
		widthMode = SizeMode::Fixed;
		heightMode = SizeMode::Fixed;
		widthWeight = 1;
		heightWeight = 1;
		
		leftMode = PositionMode::Fixed;
		topMode = PositionMode::Fixed;
		rightMode = PositionMode::Fixed;
		bottomMode = PositionMode::Fixed;
		
		minWidth = 0;
		maxWidth = DEFAULT_MAX_SIZE;
		minHeight = 0;
		maxHeight = DEFAULT_MAX_SIZE;
		
		aspectRatioMode = AspectRatioMode::None;
		aspectRatio = 1;
		
		measuredWidth = 0;
		measuredHeight = 0;
		flagInvalidMeasure = sl_true;
		
		flagInvalidRelativeBoundWidth = sl_true;
		measuredRelativeBoundWidth = 0;
		flagBadRelativeBoundWidth = sl_false;
		flagInvalidRelativeBoundHeight = sl_true;
		measuredRelativeBoundHeight = 0;
		flagBadRelativeBoundHeight = sl_false;
		
		flagOnPrepareLayout = sl_false;
		flagOnMakeLayout = sl_false;
		
		marginLeft = 0;
		marginTop = 0;
		marginRight = 0;
		marginBottom = 0;
		flagRelativeMarginLeft = sl_false;
		relativeMarginLeftWeight = 0;
		flagRelativeMarginTop = sl_false;
		relativeMarginTopWeight = 0;
		flagRelativeMarginRight = sl_false;
		relativeMarginRightWeight = 0;
		flagRelativeMarginBottom = sl_false;
		relativeMarginBottomWeight = 0;
		
		flagUpdatedLayoutFrame = sl_false;
	}

	View::LayoutAttributes::~LayoutAttributes()
	{
	}

	void View::_initializeLayoutAttributes()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new LayoutAttributes;
		if (attrs.isNull()) {
			return;
		}
		attrs->frame = m_frame;
		attrs->requestedFrame = m_frame;
	}

	View::TransformAttributes::TransformAttributes()
	{
		flagTransformFinalInvalid = sl_false;
		flagTransformFinal = sl_false;
		
		flagInverseTransformFinalInvalid = sl_false;
		flagInverseTransformFinal = sl_false;
		
		flagTransform = sl_false;
		
		flagTransformCalcInvalid = sl_false;
		flagTransformCalc = sl_false;
		
		translation.x = 0;
		translation.y = 0;
		
		scale.x = 1;
		scale.y = 1;
		
		rotationAngle = 0;
		
		anchorOffset.x = 0;
		anchorOffset.y = 0;
	}

	View::TransformAttributes::~TransformAttributes()
	{
	}

	void View::_initializeTransformAttributes()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new TransformAttributes;
	}

	View::DrawAttributes::DrawAttributes()
	{
		backgroundScaleMode = ScaleMode::Stretch;
		backgroundAlignment = Alignment::MiddleCenter;
		backgroundColor = Color::zero();
		
		boundShape = BoundShape::Rectangle;
		
		roundRectBoundShapeRadius.x = 5;
		roundRectBoundShapeRadius.y = 5;
		
		borderColor = Color::Black;
		borderStyle = PenStyle::Solid;
		borderWidth = 0;
		
		flagPreDrawEnabled = sl_false;
		flagPostDrawEnabled = sl_false;
		flagOnDrawBackgroundAlways = sl_false;
		flagOnDrawBorderAlways = sl_false;
		
		flagUsingFont = sl_false;
		
		flagOpaque = sl_false;
		alpha = 1;
		flagLayer = sl_false;
		flagInvalidatedLayer = sl_true;
		flagInvalidatedWholeLayer = sl_true;
	}

	View::DrawAttributes::~DrawAttributes()
	{
	}

	void View::_initializeDrawAttributes()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new DrawAttributes;
	}

	View::ScrollAttributes::ScrollAttributes()
	{
		flagHorz = sl_false;
		flagVert = sl_false;
		
		flagValidHorz = sl_false;
		flagValidVert = sl_false;
		x = 0;
		y = 0;
		contentWidth = 0;
		contentHeight = 0;
		barWidth = UI::getDefaultScrollBarWidth();
		flagPaging = sl_false;
		pageWidth = 0;
		pageHeight = 0;

		flagContentScrollingByMouse = sl_true;
		flagContentScrollingByTouch = sl_true;
		flagContentScrollingByMouseWheel = sl_true;
		flagContentScrollingByKeyboard = sl_true;
		flagSmoothContentScrolling = sl_true;
		flagDownContent = sl_false;
		
		flagHorzScrollBarVisible = sl_true;
		flagVertScrollBarVisible = sl_true;
		flagInitHorzScrollBar = sl_false;
		flagInitVertScrollBar = sl_false;
	}

	View::ScrollAttributes::~ScrollAttributes()
	{
	}

	void View::_initializeScrollAttributes()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new ScrollAttributes;
	}

	View::EventAttributes::EventAttributes()
	{
	}

	View::EventAttributes::~EventAttributes()
	{
	}

	void View::_initializeEventAttributes()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new EventAttributes;
	}

	Ref<ViewInstance> View::getViewInstance()
	{
		return m_instance;
	}

	sl_bool View::isInstance()
	{
		return m_instance.isNotNull();
	}

	sl_bool View::isValidInstance()
	{
		if (m_instance.isNotNull()) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				return instance->isValid();
			}
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
		return sl_null;
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
		return sl_null;
	}

	sl_bool View::isNativeWidget()
	{
		if (m_instance.isNotNull()) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				return instance->isNativeWidget();
			}
		}
		return sl_false;
	}

	sl_bool View::isHardwareLayer()
	{
		return m_flagInstanceLayer;
	}

	void View::setHardwareLayer(sl_bool flagLayered)
	{
		m_flagInstanceLayer = flagLayered;
	}

	Ref<Window> View::getWindow()
	{
		if (m_window.isNotNull()) {
			Ref<Window> window = m_window;
			if (window.isNotNull()) {
				return window;
			}
		}
		if (m_parent.isNotNull()) {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				return parent->getWindow();
			}
		}
		return sl_null;
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
			Ref<View> parent = m_parent;
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
				UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(View, _processAttachOnUiThread, this));
			}
		}
	}

	Ref<ViewInstance> View::attachToNewInstance(const Ref<ViewInstance>& parent)
	{
		Ref<ViewInstance> instance = createInstance(parent.get());
		if (instance.isNotNull()) {
			attach(instance);
		}
		return instance;
	}

	void View::detach()
	{
		ObjectLocker lock(this);
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->setView(sl_null);
			m_instance.setNull();
			onDetach();
		}
	}

	void View::_processAttachOnUiThread()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (m_flagFocused) {
				instance->setFocus();
			}
			Ref<GestureDetector> gesture = m_gestureDetector;
			if (gesture.isNotNull()) {
				gesture->enableNative();
			}
			if (getParent().isNull()) {
				if (IsInstanceOf<RenderView>(this)) {
					dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), View, _makeLayout, this, sl_false));
				} else {
					_makeLayout(sl_false);
				}
			}
			if (m_flagCreatingChildInstances) {
				ListElements< Ref<View> > children(getChildren());
				for (sl_size i = 0; i < children.count; i++) {
#if defined(SLIB_PLATFORM_IS_WIN32)
					Ref<View>& child = children[children.count - 1 - i];
#else
					Ref<View>& child = children[i];
#endif
					if (child.isNotNull()) {
						if (child->m_flagCreatingInstance) {
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
						if (!m_flagDrawing) {
							if (!(child->isInstance()) && !(isNativeWidget())) {
								setDrawing(sl_true, UIUpdateMode::NoRedraw);
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
		List< Ref<View> > children = m_childrenCache;
		if (children.isNotNull()) {
			return children;
		}
		if (m_children.getCount() > 0) {
			children = m_children.duplicate();
			m_childrenCache = children;
			return children;
		} else {
			return sl_null;
		}
	}

	sl_size View::getChildrenCount()
	{
		return m_children.getCount();
	}

	Ref<View> View::getChild(sl_size index)
	{
		return m_children.getValueAt(index);
	}

	void View::addChild(const Ref<View>& view, UIUpdateMode mode)
	{
		if (view.isNull()) {
			return;
		}
		if (m_children.addIfNotExist(view)) {
			m_childrenCache.setNull();
			_addChild(view, mode);
		}
	}

	void View::insertChild(sl_size index, const Ref<View>& view, UIUpdateMode mode)
	{
		if (view.isNull()) {
			return;
		}
		if (m_children.insert(index, view)) {
			m_childrenCache.setNull();
			_addChild(view, mode);
		}
	}

	void View::removeChild(sl_size index, UIUpdateMode mode)
	{
		Ref<View> view = m_children.getValueAt(index);
		if (view.isNull()) {
			return;
		}
		_removeChild(view);
		m_children.removeAt(index);
		m_childrenCache.setNull();
		if (view == m_childMouseDown) {
			m_childMouseDown.setNull();
		}
		if (view == m_childMouseMove) {
			m_childMouseMove.setNull();
		}
		if (view == m_childFocused) {
			_setFocusedChild(sl_null, UIUpdateMode::NoRedraw);
		}
		invalidateLayoutFromResizeContent(mode);
	}

	void View::removeChild(const Ref<View>& view, UIUpdateMode mode)
	{
		if (view.isNull()) {
			return;
		}
		_removeChild(view);
		m_children.removeValue(view);
		m_childrenCache.setNull();
		if (view == m_childMouseDown) {
			m_childMouseDown.setNull();
		}
		if (view == m_childMouseMove) {
			m_childMouseMove.setNull();
		}
		if (view == m_childFocused) {
			_setFocusedChild(sl_null, UIUpdateMode::NoRedraw);
		}
		invalidateLayoutFromResizeContent(mode);
	}

	void View::removeAllChildren(UIUpdateMode mode)
	{
		if (isInstance()) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), View, removeAllChildren, this, mode));
				return;
			}
			ListLocker< Ref<View> > children(m_children);
			for (sl_size i = 0; i < children.count; i++) {
				Ref<View>& child = children[i];
				if (child.isNotNull()) {
					_removeChild(child.get());
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
		m_children.removeAll();
		m_childrenCache.setNull();
		invalidateLayoutFromResizeContent(mode);
	}

	Ref<View> View::getChildAt(sl_ui_pos x, sl_ui_pos y)
	{
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = children.count - 1, ii = 0; ii < children.count; i--, ii++) {
			Ref<View>& child = children[i];
			if (child.isNotNull() && child->isVisible() && child->isHitTestable()) {
				UIPoint pt = child->convertCoordinateFromParent(UIPointf((sl_ui_posf)x, (sl_ui_posf)y));
				if (child->hitTest(pt)) {
					return child;
				}
			}
		}
		return sl_null;
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
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			const Ref<View>& child = children[i];
			if (child.isNotNull()) {
				Ref<View> _child = child->getChildById(_id);
				if (_child.isNotNull()) {
					return _child;
				}
			}
		}
		return sl_null;
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
		if (m_instance.isNotNull()) {
			return this;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			return parent->getInstanceView();
		}
		return sl_null;
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
		return sl_null;
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
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), View, attachChild, this, child));
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
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), View, addChildInstance, this, child));
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
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), View, removeChildInstance, this, child));
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
			List< Ref<View> > children = parent->m_children;
			MutexLocker lock(children.getLocker());
			sl_reg index = children.indexOf_NoLock(this);
			if (index >= 0) {
				children.removeAt_NoLock(index);
				children.add_NoLock(this);
				m_childrenCache.setNull();
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
		SLIB_REFERABLE_MEMBER
		if (child.isNotNull()) {
			if (mode != UIUpdateMode::Init) {
				child->setFocus(sl_false, UIUpdateMode::NoRedraw);
			}
			child->m_parent = this;
			if (m_flagOnAddChild) {
				onAddChild(child.get());
			}
			
			if (mode != UIUpdateMode::Init) {
				invalidateLayoutFromResizeContent(UIUpdateMode::NoRedraw);
				if (isInstance()) {
					sl_bool flagAttach = sl_false;
					if (m_flagCreatingChildInstances && child->m_flagCreatingInstance) {
						switch(child->getAttachMode()) {
							case UIAttachMode::NotAttach:
								break;
							case UIAttachMode::AttachAlways:
								attachChild(child);
								flagAttach = sl_true;
								break;
							case UIAttachMode::NotAttachInNativeWidget:
								if (!(isNativeWidget())) {
									attachChild(child);
									flagAttach = sl_true;
								}
								break;
							case UIAttachMode::AttachInNativeWidget:
								if (isNativeWidget()) {
									attachChild(child);
									flagAttach = sl_true;
								}
								break;
						}
					}
					if (!m_flagDrawing) {
						if (!flagAttach && !(isNativeWidget())) {
							setDrawing(sl_true, UIUpdateMode::NoRedraw);
						}
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
				onRemoveChild(view.get());
			}
			Ref<ViewInstance> instanceChild = view->getViewInstance();
			if (instanceChild.isNotNull()) {
				removeChildInstance(instanceChild.get());
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
			if (isDrawingThread()) {
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
			if (isDrawingThread()) {
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
		
		if (mode != UIUpdateMode::Init) {
			_setFrame_NI(frame);
		}
		
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			layoutAttrs->frame = frame;
			layoutAttrs->requestedFrame = frame;
			layoutAttrs->flagInvalidMeasure = sl_true;
		}
		
		if (!(flagNotResizeWidth && flagNotResizeHeight)) {
			dispatchResize(frame.getWidth(), frame.getHeight());
			if (mode == UIUpdateMode::Redraw) {
				invalidateLayer();
			}
			if (flagLayouting) {
				m_flagInvalidLayout = sl_true;
			} else {
				invalidateLayoutFromResize(UIUpdateMode::NoRedraw);
			}
		} else {
			if (!flagLayouting) {
				invalidateParentLayout(UIUpdateMode::NoRedraw);
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

	void View::requestFrame(const UIRect& _frame, UIUpdateMode mode)
	{
		UIRect frame = _frame;
		frame.fixSizeError();

		if (mode != UIUpdateMode::Init) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull()) {
				Ref<View> parent = m_parent;
				if (parent.isNotNull()) {
					layoutAttrs->requestedFrame = frame;
					invalidateLayoutFromResize(UIUpdateMode::NoRedraw);
					return;
				}
			}
		}
		_setFrame(frame, mode, sl_false);
	}

	Visibility View::getVisibility()
	{
		return m_visibility;
	}

	void View::setVisibility(Visibility visibility, UIUpdateMode mode)
	{
		Visibility oldVisibility = m_visibility;
		m_visibility = visibility;
		if (oldVisibility != visibility) {
			if (mode != UIUpdateMode::Init) {
				Ref<ViewInstance> instance = m_instance;
				if (instance.isNotNull()) {
					instance->setVisible(visibility == Visibility::Visible);
				}
				switch (visibility) {
					case Visibility::Visible:
					case Visibility::Hidden:
						if (oldVisibility == Visibility::Gone) {
							invalidateParentLayout(UIUpdateMode::NoRedraw);
						}
						break;
					case Visibility::Gone:
						invalidateParentLayout(UIUpdateMode::NoRedraw);
						break;
				}
				dispatchChangeVisibility(oldVisibility, visibility);
				if (instance.isNull()) {
					if (mode == UIUpdateMode::Redraw) {
						invalidateBoundsInParent();
					}
				}
			} else {
				dispatchChangeVisibility(oldVisibility, visibility);
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
		if (mode != UIUpdateMode::Init) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				instance->setEnabled(flag);
			} else {
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
	}
	
	sl_bool View::isClipping()
	{
		return m_flagClipping;
	}
	
	void View::setClipping(sl_bool flagClipping, UIUpdateMode mode)
	{
		m_flagClipping = flagClipping;
		if (mode != UIUpdateMode::Init) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				instance->setClipping(flagClipping);
			} else {
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
	}
	
	sl_bool View::isDrawing()
	{
		return m_flagDrawing;
	}
	
	void View::setDrawing(sl_bool flagDrawing, UIUpdateMode mode)
	{
		m_flagDrawing = flagDrawing;
		if (mode != UIUpdateMode::Init) {
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				instance->setDrawing(flagDrawing);
			} else {
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
	}
	
	sl_bool View::isSavingCanvasState()
	{
		return m_flagSavingCanvasState;
	}
	
	void View::setSavingCanvasState(sl_bool flagDrawing)
	{
		m_flagSavingCanvasState = flagDrawing;
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
		return sl_null;
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
				Ref<DrawAttributes>& attrs = m_drawAttrs;
				if (attrs.isNotNull()) {
					if (attrs->backgroundHover.isNotNull() && attrs->background != attrs->backgroundHover) {
						invalidate();
					}
				}
			}
		}
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
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
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
		layoutAttrs->measuredWidth = Math::clamp(measuredWidth, layoutAttrs->minWidth, layoutAttrs->maxWidth);
		layoutAttrs->measuredHeight = Math::clamp(measuredHeight, layoutAttrs->minHeight, layoutAttrs->maxHeight);
		
		if (flagHorizontal || flagVertical) {
			onMeasureLayout(flagHorizontal, flagVertical);
		}
		
		layoutAttrs->measuredWidth = Math::clamp(layoutAttrs->measuredWidth, layoutAttrs->minWidth, layoutAttrs->maxWidth);
		layoutAttrs->measuredHeight = Math::clamp(layoutAttrs->measuredHeight, layoutAttrs->minHeight, layoutAttrs->maxHeight);

		layoutAttrs->flagInvalidMeasure = sl_false;

	}

	sl_ui_len View::getMeasuredWidth()
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			if (layoutAttrs->widthMode == SizeMode::Wrapping) {
				return layoutAttrs->measuredWidth;
			} else if (layoutAttrs->widthMode == SizeMode::Filling || layoutAttrs->widthMode == SizeMode::Weight) {
				return layoutAttrs->minWidth;
			} else {
				return Math::max(getWidth(), layoutAttrs->minWidth);
			}
		} else {
			return getWidth();
		}
	}

	void View::setMeasuredWidth(sl_ui_len width)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			layoutAttrs->measuredWidth = width;
		}
	}

	sl_ui_len View::getMeasuredHeight()
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			if (layoutAttrs->heightMode == SizeMode::Wrapping) {
				return layoutAttrs->measuredHeight;
			} else if (layoutAttrs->heightMode == SizeMode::Filling || layoutAttrs->heightMode == SizeMode::Weight) {
				return layoutAttrs->minHeight;
			} else {
				return Math::max(getHeight(), layoutAttrs->minHeight);
			}
		} else {
			return getHeight();
		}
	}

	void View::setMeasuredHeight(sl_ui_len height)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			layoutAttrs->measuredHeight = height;
		}
	}

	void View::_prepareLayout(ViewPrepareLayoutParam& param)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNull()) {
			return;
		}
		
		UIRect frame;
		if (param.flagUseLayoutFrame) {
			frame = layoutAttrs->frame;
		} else {
			frame = layoutAttrs->requestedFrame;
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
		
		if (layoutAttrs->aspectRatioMode == AspectRatioMode::AdjustWidth) {
			width = (sl_ui_pos)(height * layoutAttrs->aspectRatio);
		} else if (layoutAttrs->aspectRatioMode == AspectRatioMode::AdjustHeight) {
			if (layoutAttrs->aspectRatio > 0.0000001f) {
				height = (sl_ui_pos)(width / layoutAttrs->aspectRatio);
			}
		}
		
		width = Math::clamp(width, layoutAttrs->minWidth, layoutAttrs->maxWidth);
		height = Math::clamp(height, layoutAttrs->minHeight, layoutAttrs->maxHeight);
		
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
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			if (flagApplyLayout) {
				if (!(layoutAttrs->frame.isAlmostEqual(m_frame))) {
					_setFrame(layoutAttrs->frame, UIUpdateMode::NoRedraw, sl_true);
					m_flagInvalidLayout = sl_true;
				}
			} else {
				if (layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping) {
					measureLayout();
					UIRect frame = layoutAttrs->frame;
					sl_bool flagUpdate = sl_false;
					if (layoutAttrs->widthMode == SizeMode::Wrapping && !(Math::isAlmostZero(layoutAttrs->measuredWidth - m_frame.getWidth()))) {
						frame.setWidth(layoutAttrs->measuredWidth);
						flagUpdate = sl_true;
					}
					if (layoutAttrs->heightMode == SizeMode::Wrapping && !(Math::isAlmostZero(layoutAttrs->measuredHeight - m_frame.getHeight()))) {
						frame.setHeight(layoutAttrs->measuredHeight);
						flagUpdate = sl_true;
					}
					if (flagUpdate) {
						_setFrame(frame, UIUpdateMode::NoRedraw, sl_true);
						m_flagInvalidLayout = sl_true;
					}
				}
			}
		}
		
		if (m_flagInvalidLayout) {
			
			ListElements< Ref<View> > children(getChildren());
			
			if (children.count > 0) {
				
				ViewPrepareLayoutParam param;
				param.parentContentFrame.left = m_paddingLeft;
				param.parentContentFrame.top = m_paddingTop;
				param.parentContentFrame.right = getWidth() - m_paddingRight;
				param.parentContentFrame.bottom = getHeight() - m_paddingBottom;
				param.parentContentFrame.fixSizeError();
				param.flagUseLayoutFrame = sl_false;
				
				{
					for (sl_size i = 0; i < children.count; i++) {
						Ref<View>& child = children[i];
						if (child.isNotNull() && child->m_layoutAttrs.isNotNull()) {
							child->_prepareLayout(param);
							child->setLayoutFrameUpdated(sl_false);
						}
					}
				}
				
				if (layoutAttrs.isNotNull() && layoutAttrs->flagOnMakeLayout) {
					onMakeLayout();
				}
				
				param.flagUseLayoutFrame = sl_true;

				{
					for (sl_size i = 0; i < children.count; i++) {
						Ref<View>& child = children[i];
						if (child.isNotNull()) {
							Ref<LayoutAttributes>& childLayoutAttrs = child->m_layoutAttrs;
							if (childLayoutAttrs.isNotNull() && !(child->isLayoutFrameUpdated())) {
								child->_prepareLayout(param);
								if (child->isOnPrepareLayoutEnabled()) {
									child->onPrepareLayout(param);
								}
							}
							if (child->isNativeWidget() && IsInstanceOf<RenderView>(child)) {
								if (childLayoutAttrs.isNotNull()) {
									child->_setFrame(child->getLayoutFrame(), UIUpdateMode::NoRedraw, sl_true);
								}
								if (child->m_flagInvalidLayout) {
									child->_requestMakeLayout();
								}
							} else {
								child->_makeLayout(sl_true);
							}
						}
					}
				}
			}
			
			m_flagInvalidLayout = sl_false;
			
		};
		
	}

	void View::_measureRelativeBoundWidth()
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
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
				Ref<LayoutAttributes>& referLayoutAttrs = referView->m_layoutAttrs;
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
				Ref<LayoutAttributes>& referLayoutAttrs = referView->m_layoutAttrs;
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
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
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
				Ref<LayoutAttributes>& referLayoutAttrs = referView->m_layoutAttrs;
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
				Ref<LayoutAttributes>& referLayoutAttrs = referView->m_layoutAttrs;
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
		ListElements< Ref<View> > children(getChildren());
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				if (child->getVisibility() != Visibility::Gone) {
					Ref<LayoutAttributes>& layoutAttrs = child->m_layoutAttrs;
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
						Ref<LayoutAttributes>& layoutAttrs = child->m_layoutAttrs;
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
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), View, _makeLayout, this, sl_false));
	}

	void View::invalidateLayoutFromResize(UIUpdateMode mode)
	{
		if (mode == UIUpdateMode::Init) {
			return;
		}
		Ref<View> instanceView = getInstanceView();
		if (instanceView.isNull()) {
			return;
		}
		if (!(instanceView->isDrawingThread())) {
			instanceView->dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), View, invalidateLayoutFromResize, this, mode));
			return;
		}
		m_flagInvalidLayout = sl_true;
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->invalidateLayoutFromResizeContent(UIUpdateMode::NoRedraw);
		} else {
			if (m_children.getCount() > 0) {
				_requestMakeLayout();
			}
		}
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}

	void View::invalidateLayoutFromResizeContent(UIUpdateMode mode)
	{
		if (mode == UIUpdateMode::Init) {
			return;
		}
		Ref<View> instanceView = getInstanceView();
		if (instanceView.isNull()) {
			return;
		}
		if (!(instanceView->isDrawingThread())) {
			instanceView->dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), View, invalidateLayoutFromResizeContent, this, mode));
			return;
		}
		m_flagInvalidLayout = sl_true;
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			if (layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping) {
				layoutAttrs->flagInvalidMeasure = sl_true;
				Ref<View> parent = m_parent;
				if (parent.isNotNull()) {
					parent->invalidateLayoutFromResizeContent(UIUpdateMode::NoRedraw);
				} else {
					_requestMakeLayout();
				}
			} else {
				if (m_children.getCount() > 0) {
					_requestMakeLayout();
				}
			}
		} else {
			if (m_children.getCount() > 0) {
				_requestMakeLayout();
			}
		}
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}

	void View::invalidateParentLayout(UIUpdateMode mode)
	{
		if (mode == UIUpdateMode::Init) {
			return;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->invalidateLayoutFromResizeContent(mode);
		}
	}

	UIRect View::getLayoutFrame()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->frame;
		}
		return getFrame();
	}

	void View::setLayoutFrame(const UIRect& _rect)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			UIRect rect = _rect;
			rect.right = rect.left + Math::clamp(rect.right - rect.left, attrs->minWidth, attrs->maxWidth);
			rect.bottom = rect.top + Math::clamp(rect.bottom - rect.top, attrs->minHeight, attrs->maxHeight);
			attrs->frame = rect;
			attrs->flagUpdatedLayoutFrame = sl_true;
		} else {
			_setFrame(_rect, UIUpdateMode::NoRedraw, sl_true);
		}
	}

	sl_bool View::isLayoutFrameUpdated()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagUpdatedLayoutFrame;
		}
		return sl_false;
	}

	void View::setLayoutFrameUpdated(sl_bool flag)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagUpdatedLayoutFrame = flag;
		}
	}

	sl_bool View::isOnPrepareLayoutEnabled()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagOnPrepareLayout;
		}
		return sl_false;
	}

	void View::setOnPrepareLayoutEnabled(sl_bool flagEnabled, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagOnPrepareLayout = flagEnabled;
		}
	}

	sl_bool View::isOnMakeLayoutEnabled()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagOnMakeLayout;
		}
		return sl_false;
	}

	void View::setOnMakeLayoutEnabled(sl_bool flagEnabled, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagOnMakeLayout = flagEnabled;
		}
	}

	SizeMode View::getWidthMode()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->widthMode;
		}
		return SizeMode::Fixed;
	}

	SizeMode View::getHeightMode()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->heightMode;
		}
		return SizeMode::Fixed;
	}

	sl_bool View::isWidthFixed()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->widthMode == SizeMode::Fixed;
		}
		return sl_true;
	}

	void View::setWidthFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Fixed;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_bool View::isHeightFixed()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->heightMode == SizeMode::Fixed;
		}
		return sl_true;
	}

	void View::setHeightFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->heightMode = SizeMode::Fixed;
			invalidateLayoutFromResize(mode);
		}
	}

	void View::setSizeFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Fixed;
			attrs->heightMode = SizeMode::Fixed;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_real View::getWidthWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->widthWeight;
		}
		return 1;
	}

	sl_bool View::isHeightWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->heightMode == SizeMode::Weight;
		}
		return sl_false;
	}

	sl_bool View::isWidthFilling()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->widthMode == SizeMode::Filling;
		}
		return sl_false;
	}

	void View::setWidthFilling(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Filling;
			attrs->widthWeight = weight;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_bool View::isHeightFilling()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->heightMode == SizeMode::Filling;
		}
		return sl_false;
	}

	void View::setHeightFilling(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->heightMode = SizeMode::Filling;
			attrs->heightWeight = weight;
			invalidateLayoutFromResize(mode);
		}
	}

	void View::setSizeFilling(sl_real widthWeight, sl_real heightWeight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Filling;
			attrs->widthWeight = widthWeight;
			attrs->heightMode = SizeMode::Filling;
			attrs->heightWeight = heightWeight;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_bool View::isWidthWrapping()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->widthMode == SizeMode::Wrapping;
		}
		return sl_false;
	}

	void View::setWidthWrapping(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Wrapping;
			invalidateLayoutFromResizeContent(mode);
		}
	}

	sl_bool View::isHeightWrapping()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->heightMode == SizeMode::Wrapping;
		}
		return sl_false;
	}

	void View::setHeightWrapping(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->heightMode = SizeMode::Wrapping;
			invalidateLayoutFromResizeContent(mode);
		}
	}

	void View::setSizeWrapping(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Wrapping;
			attrs->heightMode = SizeMode::Wrapping;
			invalidateLayoutFromResizeContent(mode);
		}
	}

	sl_bool View::isWidthWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->widthMode == SizeMode::Weight;
		}
		return sl_false;
	}

	void View::setWidthWeight(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Weight;
			attrs->widthWeight = weight;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_real View::getHeightWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->heightWeight;
		}
		return 1;
	}

	void View::setHeightWeight(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->heightMode = SizeMode::Weight;
			attrs->heightWeight = weight;
			invalidateLayoutFromResize(mode);
		}
	}

	void View::setSizeWeight(sl_real widthWeight, sl_real heightWeight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->widthMode = SizeMode::Weight;
			attrs->widthWeight = widthWeight;
			attrs->heightMode = SizeMode::Weight;
			attrs->heightWeight = heightWeight;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_bool View::isLayoutLeftFixed()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->leftMode == PositionMode::Fixed;
		}
		return sl_true;
	}

	void View::setLayoutLeftFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::Fixed;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignParentLeft()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->leftMode == PositionMode::ParentEdge;
		}
		return sl_false;
	}

	void View::setAlignParentLeft(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::ParentEdge;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignLeft()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::OtherStart;
			attrs->leftReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isRightOf()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::OtherEnd;
			attrs->leftReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	Ref<View> View::getLayoutLeftReferingView()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->leftReferingView;
		}
		return sl_null;
	}

	sl_bool View::isLayoutRightFixed()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->rightMode == PositionMode::Fixed;
		}
		return sl_true;
	}

	void View::setLayoutRightFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->rightMode = PositionMode::Fixed;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignParentRight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->rightMode == PositionMode::ParentEdge;
		}
		return sl_false;
	}

	void View::setAlignParentRight(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->rightMode = PositionMode::ParentEdge;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignRight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->rightMode = PositionMode::OtherEnd;
			attrs->rightReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isLeftOf()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->rightMode = PositionMode::OtherStart;
			attrs->rightReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	Ref<View> View::getLayoutRightReferingView()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->rightReferingView;
		}
		return sl_null;
	}

	sl_bool View::isLayoutTopFixed()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->topMode == PositionMode::Fixed;
		}
		return sl_true;
	}

	void View::setLayoutTopFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::Fixed;
			invalidateParentLayout(mode);
		}

	}

	sl_bool View::isAlignParentTop()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->topMode == PositionMode::ParentEdge;
		}
		return sl_false;
	}

	void View::setAlignParentTop(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::ParentEdge;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignTop()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::OtherStart;
			attrs->topReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isBelow()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::OtherEnd;
			attrs->topReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	Ref<View> View::getLayoutTopReferingView()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->topReferingView;
		}
		return sl_null;
	}

	sl_bool View::isLayoutBottomFixed()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->bottomMode == PositionMode::Fixed;
		}
		return sl_true;
	}

	void View::setLayoutBottomFixed(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::Fixed;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignParentBottom()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->bottomMode == PositionMode::ParentEdge;
		}
		return sl_false;
	}

	void View::setAlignParentBottom(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::ParentEdge;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignBottom()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::OtherEnd;
			attrs->bottomReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAbove()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::OtherStart;
			attrs->bottomReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	Ref<View> View::getLayoutBottomReferingView()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->bottomReferingView;
		}
		return sl_null;
	}

	sl_bool View::isCenterHorizontal()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->leftMode == PositionMode::CenterInParent;
		}
		return sl_false;
	}

	void View::setCenterHorizontal(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::CenterInParent;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isCenterVertical()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->topMode == PositionMode::CenterInParent;
		}
		return sl_false;
	}

	void View::setCenterVertical(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::CenterInParent;
			invalidateParentLayout(mode);
		}
	}

	void View::setCenterInParent(UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::CenterInParent;
			attrs->topMode = PositionMode::CenterInParent;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignCenterHorizontal()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::CenterInOther;
			attrs->leftReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isAlignCenterVertical()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::CenterInOther;
			attrs->topReferingView = view;
			invalidateParentLayout(mode);
		}
	}

	sl_ui_len View::getMinimumWidth()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->minWidth;
		}
		return 0;
	}

	void View::setMinimumWidth(sl_ui_len width, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (width < 0) {
				width = 0;
			}
			attrs->minWidth = width;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_ui_len View::getMaximumWidth()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->maxWidth;
		}
		return DEFAULT_MAX_SIZE;
	}

	void View::setMaximumWidth(sl_ui_len width, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (width < 0) {
				width = DEFAULT_MAX_SIZE;
			}
			attrs->maxWidth = width;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_ui_len View::getMinimumHeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->minHeight;
		}
		return 0;
	}

	void View::setMinimumHeight(sl_ui_len height, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (height < 0) {
				height = 0;
			}
			attrs->minHeight = height;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_ui_len View::getMaximumHeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->maxHeight;
		}
		return 0;
	}

	void View::setMaximumHeight(sl_ui_len height, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (height < 0) {
				height = DEFAULT_MAX_SIZE;
			}
			attrs->maxHeight = height;
			invalidateLayoutFromResize(mode);
		}
	}

	AspectRatioMode View::getAspectRatioMode()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->aspectRatioMode;
		}
		return AspectRatioMode::None;
	}

	void View::setAspectRatioMode(AspectRatioMode aspectRatioMode, UIUpdateMode updateMode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->aspectRatioMode = aspectRatioMode;
			invalidateLayoutFromResize(updateMode);
		}
	}

	sl_real View::getAspectRatio()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->aspectRatio;
		}
		return 0;
	}

	void View::setAspectRatio(sl_real ratio, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (ratio < 0) {
				ratio = 1;
			}
			attrs->aspectRatio = ratio;
			invalidateLayoutFromResize(mode);
		}
	}

	sl_ui_pos View::getMarginLeft()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->marginLeft;
		}
		return 0;
	}

	void View::setMarginLeft(sl_ui_pos marginLeft, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->marginLeft = marginLeft;
			if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
				attrs->leftMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getMarginTop()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->marginTop;
		}
		return 0;
	}

	void View::setMarginTop(sl_ui_pos marginTop, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->marginTop = marginTop;
			if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
				attrs->topMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getMarginRight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->marginRight;
		}
		return 0;
	}

	void View::setMarginRight(sl_ui_pos marginRight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->marginRight = marginRight;
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getMarginBottom()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->marginBottom;
		}
		return 0;
	}

	void View::setMarginBottom(sl_ui_pos marginBottom, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->marginBottom = marginBottom;
			invalidateParentLayout(mode);
		}
	}

	void View::setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->marginLeft = left;
			attrs->marginTop = top;
			attrs->marginRight = right;
			attrs->marginBottom = bottom;
			invalidateParentLayout(mode);
		}
	}

	void View::setMargin(sl_ui_pos margin, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->marginLeft = margin;
			attrs->marginTop = margin;
			attrs->marginRight = margin;
			attrs->marginBottom = margin;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isRelativeMarginLeft()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagRelativeMarginLeft;
		}
		return sl_false;
	}

	sl_real View::getRelativeMarginLeftWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->relativeMarginLeftWeight;
		}
		return 0;
	}

	void View::setRelativeMarginLeft(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginLeft = sl_true;
			attrs->relativeMarginLeftWeight = weight;
			if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
				attrs->leftMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getAbsoluteMarginLeft()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginLeft = sl_false;
			attrs->marginLeft = margin;
			if (attrs->leftMode == PositionMode::Fixed && attrs->rightMode == PositionMode::Fixed) {
				attrs->leftMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isRelativeMarginTop()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagRelativeMarginTop;
		}
		return sl_false;
	}

	sl_real View::getRelativeMarginTopWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->relativeMarginTopWeight;
		}
		return 0;
	}

	void View::setRelativeMarginTop(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginTop = sl_true;
			attrs->relativeMarginTopWeight = weight;
			if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
				attrs->topMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getAbsoluteMarginTop()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginTop = sl_false;
			attrs->marginTop = margin;
			if (attrs->topMode == PositionMode::Fixed && attrs->bottomMode == PositionMode::Fixed) {
				attrs->topMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isRelativeMarginRight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagRelativeMarginRight;
		}
		return sl_false;
	}

	sl_real View::getRelativeMarginRightWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->relativeMarginRightWeight;
		}
		return 0;
	}

	void View::setRelativeMarginRight(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginRight = sl_true;
			attrs->relativeMarginRightWeight = weight;
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getAbsoluteMarginRight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginRight = sl_false;
			attrs->marginRight = margin;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isRelativeMarginBottom()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagRelativeMarginBottom;
		}
		return sl_false;
	}

	sl_real View::getRelativeMarginBottomWeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->relativeMarginBottomWeight;
		}
		return 0;
	}

	void View::setRelativeMarginBottom(sl_real weight, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginBottom = sl_true;
			attrs->relativeMarginBottomWeight = weight;
			invalidateParentLayout(mode);
		}
	}

	sl_ui_pos View::getAbsoluteMarginBottom()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagRelativeMarginBottom = sl_false;
			attrs->marginBottom = margin;
			invalidateParentLayout(mode);
		}
	}

	void View::applyRelativeMargins(sl_ui_len parentWidth, sl_ui_len parentHeight)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
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
		invalidateLayoutFromResizeContent(mode);
		onChangePadding();
	}

	sl_ui_pos View::getPaddingTop()
	{
		return m_paddingTop;
	}

	void View::setPaddingTop(sl_ui_pos paddingTop, UIUpdateMode mode)
	{
		m_paddingTop = paddingTop;
		invalidateLayoutFromResizeContent(mode);
		onChangePadding();
	}

	sl_ui_pos View::getPaddingRight()
	{
		return m_paddingRight;
	}

	void View::setPaddingRight(sl_ui_pos paddingRight, UIUpdateMode mode)
	{
		m_paddingRight = paddingRight;
		invalidateLayoutFromResizeContent(mode);
		onChangePadding();
	}

	sl_ui_pos View::getPaddingBottom()
	{
		return m_paddingBottom;
	}

	void View::setPaddingBottom(sl_ui_pos paddingBottom, UIUpdateMode mode)
	{
		m_paddingBottom = paddingBottom;
		invalidateLayoutFromResizeContent(mode);
		onChangePadding();
	}

	void View::setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		m_paddingLeft = left;
		m_paddingTop = top;
		m_paddingRight = right;
		m_paddingBottom = bottom;
		invalidateLayoutFromResizeContent(mode);
		onChangePadding();
	}

	void View::setPadding(sl_ui_pos padding, UIUpdateMode mode)
	{
		m_paddingLeft = padding;
		m_paddingTop = padding;
		m_paddingRight = padding;
		m_paddingBottom = padding;
		invalidateLayoutFromResizeContent(mode);
		onChangePadding();
	}

	sl_bool View::getFinalTransform(Matrix3* _out)
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNull()) {
			return sl_false;
		}
		
		if (attrs->flagTransformFinalInvalid) {
			attrs->flagTransformFinalInvalid = sl_false;
			attrs->flagInverseTransformFinalInvalid = sl_true;
			if (attrs->flagTransform) {
				attrs->flagTransformFinal = sl_true;
				attrs->transformFinal = attrs->transform;
			} else {
				if (attrs->flagTransformCalcInvalid) {
					attrs->flagTransformCalcInvalid = sl_false;
					Matrix3 mat;
					sl_bool flagInited = sl_false;
					sl_real tx = attrs->translation.x;
					sl_real ty = attrs->translation.y;
					sl_bool flagTranslate = !(Math::isAlmostZero(tx) && Math::isAlmostZero(ty));
					sl_real sx = attrs->scale.x;
					sl_real sy = attrs->scale.y;
					sl_bool flagScale = !(Math::isAlmostZero(sx - 1) && Math::isAlmostZero(sy - 1));
					sl_real r = attrs->rotationAngle;
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
					attrs->transformFinal = attrs->transformCalc;
				} else {
					attrs->flagTransformFinal = sl_false;
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
		Ref<TransformAttributes>& attrs = m_transformAttrs;
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
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull() && attrs->flagTransform) {
			return attrs->transform;
		}
		return Matrix3::identity();
	}

	void View::setTransform(const Matrix3& matrix, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->flagTransform = sl_true;
			attrs->transform = matrix;
			_applyFinalTransform(mode);
		}
	}

	void View::resetTransform(UIUpdateMode mode)
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull() && attrs->flagTransform) {
			attrs->flagTransform = sl_false;
			_applyFinalTransform(mode);
		}
	}

	sl_bool View::getFinalTranslationRotationScale(Vector2* translation, sl_real* rotation, Vector2* scale, Vector2* anchor)
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNull()) {
			return sl_false;
		}
		if (translation) {
			translation->x = attrs->translation.x;
			translation->y = attrs->translation.y;
		}
		if (scale) {
			scale->x = attrs->scale.x;
			scale->y = attrs->scale.y;
		}
		if (rotation) {
			*rotation = attrs->rotationAngle;
		}
		if (anchor) {
			anchor->x = attrs->anchorOffset.x;
			anchor->y = attrs->anchorOffset.y;
		}
		return sl_true;
	}

	sl_real View::getTranslationX()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->translation.x;
		}
		return 0;
	}

	sl_real View::getTranslationY()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->translation.y;
		}
		return 0;
	}

	const Vector2& View::getTranslation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->translation;
		}
		return Vector2::zero();
	}

	void View::setTranslationX(sl_real tx, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->translation.x = tx;
			_applyCalcTransform(mode);
		}
	}

	void View::setTranslationY(sl_real ty, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->translation.y = ty;
			_applyCalcTransform(mode);
		}
	}

	void View::setTranslation(sl_real tx, sl_real ty, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->translation.x = tx;
			attrs->translation.y = ty;
			_applyCalcTransform(mode);
		}
	}

	void View::setTranslation(const Vector2& t, UIUpdateMode mode)
	{
		setTranslation(t.x, t.y, mode);
	}

	sl_real View::getScaleX()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->scale.x;
		}
		return 1;
	}

	sl_real View::getScaleY()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->scale.y;
		}
		return 1;
	}

	const Vector2& View::getScale()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->scale;
		}
		static const sl_real t[2] = {1, 1};
		return Vector2::fromArray(t);
	}

	void View::setScaleX(sl_real sx, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->scale.x = sx;
			_applyCalcTransform(mode);
		}
	}

	void View::setScaleY(sl_real sy, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->scale.y = sy;
			_applyCalcTransform(mode);
		}
	}

	void View::setScale(sl_real sx, sl_real sy, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->scale.x = sx;
			attrs->scale.y = sy;
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

	sl_real View::getRotation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->rotationAngle;
		}
		return 0;
	}

	void View::setRotation(sl_real radian, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->rotationAngle = radian;
			_applyCalcTransform(mode);
		}
	}

	sl_real View::getAnchorOffsetX()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->anchorOffset.x;
		}
		return 0;
	}

	sl_real View::getAnchorOffsetY()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->anchorOffset.y;
		}
		return 0;
	}

	const Vector2& View::getAnchorOffset()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->anchorOffset;
		}
		return Vector2::zero();
	}

	void View::setAnchorOffsetX(sl_real x, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->anchorOffset.x = x;
			_applyCalcTransform(mode);
		}
	}

	void View::setAnchorOffsetY(sl_real y, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->anchorOffset.y = y;
			_applyCalcTransform(mode);
		}
	}

	void View::setAnchorOffset(sl_real x, sl_real y, UIUpdateMode mode)
	{
		_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
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
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			attrs->flagTransformCalcInvalid = sl_true;
			if (!(attrs->flagTransform)) {
				_applyFinalTransform(mode);
			}
		}
	}

	void View::_applyFinalTransform(UIUpdateMode mode)
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
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
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->background;
		}
		return sl_null;
	}

	void View::setBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->background = drawable;
			if (drawable.isNotNull()) {
				setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
			}
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Ref<Drawable> View::getPressedBackground()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->backgroundPressed;
		}
		return sl_null;
	}

	void View::setPressedBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->backgroundPressed = drawable;
			if (drawable.isNotNull()) {
				setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
			}
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Ref<Drawable> View::getHoverBackground()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->backgroundHover;
		}
		return sl_null;
	}

	void View::setHoverBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->backgroundHover = drawable;
			if (drawable.isNotNull()) {
				setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
			}
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	ScaleMode View::getBackgroundScaleMode()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->backgroundScaleMode;
		}
		return ScaleMode::Stretch;
	}

	void View::setBackgroundScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->backgroundScaleMode = scaleMode;
			if (updateMode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Alignment View::getBackgroundAlignment()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->backgroundAlignment;
		}
		return Alignment::MiddleCenter;
	}

	void View::setBackgroundAlignment(Alignment align, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->backgroundAlignment = align;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Color View::getBackgroundColor()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->backgroundColor;
		}
		return Color::zero();
	}

	void View::setBackgroundColor(const Color& color, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->backgroundColor = color;
			setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
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
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->penBorder;
		}
		return sl_null;
	}

	void View::setBorder(const Ref<Pen>& pen, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->penBorder = pen;
			if (pen.isNotNull()) {
				if (pen->getWidth() > 0) {
					setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
				}
			}
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Color View::getBorderColor()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->borderColor;
		}
		return Color::Black;
	}

	void View::setBorderColor(const Color& color, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
		if (mode != UIUpdateMode::Init) {
			if (isNativeWidget()) {
				_setBorder_NW(flagBorder);
			} else {
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
	}

	PenStyle View::getBorderStyle()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->borderStyle;
		}
		return PenStyle::Solid;
	}

	void View::setBorderStyle(PenStyle style, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->borderStyle = style;
			_refreshBorderPen(mode);
		}
	}

	sl_real View::getBorderWidth()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->borderWidth;
		}
		return 0;
	}

	void View::setBorderWidth(sl_real width, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->borderWidth = width;
			if (width > 0) {
				setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
			}
			_refreshBorderPen(mode);
		}
	}

	void View::_refreshBorderPen(UIUpdateMode mode)
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->boundShape;
		}
		return BoundShape::Rectangle;
	}

	void View::setBoundShape(BoundShape shape, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->boundShape = shape;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	const Size& View::getRoundRectBoundShapeRadius()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->roundRectBoundShapeRadius;
		}
		return Size::zero();
	}

	void View::setRoundRectBoundShapeRadius(const Size& radius, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->roundRectBoundShapeRadius.x = rx;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	void View::setRoundRectBoundShapeRadiusY(sl_real ry, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->boundShapePath;
		}
		return sl_null;
	}

	void View::setBoundShapePath(const Ref<GraphicsPath>& path, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->boundShapePath = path;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool View::isPreDrawEnabled()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagPreDrawEnabled;
		}
		return sl_false;
	}

	void View::setPreDrawEnabled(sl_bool flagEnabled, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagPreDrawEnabled = flagEnabled;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool View::isPostDrawEnabled()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagPostDrawEnabled;
		}
		return sl_false;
	}

	void View::setPostDrawEnabled(sl_bool flagEnabled, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagPostDrawEnabled = flagEnabled;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool View::isAlwaysOnDrawBackground()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagOnDrawBackgroundAlways;
		}
		return sl_false;
	}

	void View::setAlwaysOnDrawBackground(sl_bool flagEnabled, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagOnDrawBackgroundAlways = flagEnabled;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool View::isAlwaysOnDrawBorder()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagOnDrawBorderAlways;
		}
		return sl_false;
	}

	void View::setAlwaysOnDrawBorder(sl_bool flagEnabled, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagOnDrawBorderAlways = flagEnabled;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Ref<Font> View::getFont()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				child->_setFontInvalidateChildInstances();
				if (child->isUsingFont()) {
					child->invalidateLayoutFromResizeContent(UIUpdateMode::NoRedraw);
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
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
					invalidateLayoutFromResizeContent(mode);
					if (mode == UIUpdateMode::Redraw) {
						invalidate();
					}
				}
			}
			
		}
	}

#define _APPLY_FONT_PRECISION(f) (sl_real)((sl_int32)((f) + 0.5f))

	void View::setFont(const String& fontFamily, sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, UIUpdateMode mode)
	{
		setFont(Font::create(fontFamily, _APPLY_FONT_PRECISION(size), flagBold, flagItalic, flagUnderline), mode);
	}

	void View::setFontAttributes(sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, UIUpdateMode mode)
	{
		Ref<Font> font = getFont();
		if (font.isNull()) {
			setFont(Font::create(UI::getDefaultFontFamily(), _APPLY_FONT_PRECISION(size), flagBold, flagItalic, flagUnderline), mode);
		} else {
			setFont(Font::create(font->getFamilyName(), _APPLY_FONT_PRECISION(size), flagBold, flagItalic, flagUnderline), mode);
		}
	}

	sl_bool View::isUsingFont()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagUsingFont;
		}
		return sl_false;
	}

	void View::setUsingFont(sl_bool flag)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagUsingFont = flag;
		}
	}

	sl_bool View::isOpaque()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagOpaque;
		}
		return sl_false;
	}

	void View::setOpaque(sl_bool flag, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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

	sl_real View::getAlpha()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->alpha;
		}
		return 1;
	}

	void View::setAlpha(sl_real alpha, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->alpha = alpha;
#if defined(SLIB_PLATFORM_IS_WIN32)
			if (mode == UIUpdateMode::Redraw) {
				invalidateBoundsInParent();
			}
#else
			if (mode != UIUpdateMode::Init) {
				Ref<ViewInstance> instance = m_instance;
				if (instance.isNotNull()) {
					instance->setAlpha(alpha);
				} else {
					if (mode == UIUpdateMode::Redraw) {
						invalidateBoundsInParent();
					}
				}
			}
#endif
		}
	}

	sl_bool View::isLayer()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagLayer;
		}
		return sl_false;
	}

	void View::setLayer(sl_bool flagLayer, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagLayer = flagLayer;
			setDrawing(sl_true, mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::NoRedraw);
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	void View::invalidateLayer()
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagInvalidatedLayer = sl_true;
			attrs->flagInvalidatedWholeLayer = sl_true;
		}
	}

	void View::invalidateLayer(const UIRect& rect)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			if (attrs->flagInvalidatedLayer) {
				if (!(attrs->flagInvalidatedWholeLayer)) {
					UIRect r = attrs->rectInvalidatedLayer;
					r.mergeRectangle(rect);
					attrs->rectInvalidatedLayer = r;
				}
			} else {
				attrs->rectInvalidatedLayer = rect;
				attrs->flagInvalidatedWholeLayer = sl_false;
				attrs->flagInvalidatedLayer = sl_true;
			}
		}
	}

	Ref<AnimationLoop> View::getAnimationLoop()
	{
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			return parent->getAnimationLoop();
		} else {
			return UIAnimationLoop::getInstance();
		}
	}

	Ref<Animation> View::createAnimation(float duration)
	{
		Ref<AnimationLoop> loop = getAnimationLoop();
		if (loop.isNotNull()) {
			return Animation::createWithLoop(loop, duration);
		}
		return sl_null;
	}

	Ref<Animation> View::createAnimation(const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<AnimationLoop> loop = getAnimationLoop();
		if (loop.isNotNull()) {
			return Animation::createWithLoop(loop, target, duration, onStop, curve, flags);
		}
		return sl_null;
	}

	Ref<Animation> View::getTransformAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationTransform;
		}
		return sl_null;
	}

	void View::setTransformAnimation(const Ref<Animation>& animation, const AnimationFrames<Matrix3>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewTransformAnimationTarget(this, frames));
				attrs->m_animationTransform = animation;
			}
		}
	}

	void View::setTransformAnimation(const Ref<Animation>& animation, const Matrix3& startValue, const Matrix3& endValue)
	{
		setTransformAnimation(animation, AnimationFrames<Matrix3>(startValue, endValue));
	}

	void View::setTransformAnimation(const Ref<Animation>& animation, const Matrix3& toValue)
	{
		setTransformAnimation(animation, AnimationFrames<Matrix3>(getTransform(), toValue));
	}

	Ref<Animation> View::createTransformAnimation(const AnimationFrames<Matrix3>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewTransformAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationTransform = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(AnimationFrames<Matrix3>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createTransformAnimationTo(const Matrix3& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(AnimationFrames<Matrix3>(getTransform(), toValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::getTranslateAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationTranslate;
		}
		return sl_null;
	}

	void View::setTranslateAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewTranslateAnimationTarget(this, frames));
				attrs->m_animationTranslate = animation;
			}
		}
	}

	void View::setTranslateAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue)
	{
		setTranslateAnimation(animation, AnimationFrames<Vector2>(startValue, endValue));
	}

	void View::setTranslateAnimation(const Ref<Animation>& animation, const Vector2& toValue)
	{
		setTranslateAnimation(animation, AnimationFrames<Vector2>(getTranslation(), toValue));
	}

	Ref<Animation> View::createTranslateAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewTranslateAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationTranslate = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(AnimationFrames<Vector2>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createTranslateAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(AnimationFrames<Vector2>(getTranslation(), toValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::getScaleAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationScale;
		}
		return sl_null;
	}

	void View::setScaleAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewScaleAnimationTarget(this, frames));
				attrs->m_animationScale = animation;
			}
		}
	}

	void View::setScaleAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue)
	{
		setScaleAnimation(animation, AnimationFrames<Vector2>(startValue, endValue));
	}

	void View::setScaleAnimation(const Ref<Animation>& animation, const Vector2& toValue)
	{
		setScaleAnimation(animation, AnimationFrames<Vector2>(getScale(), toValue));
	}

	void View::setScaleAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue)
	{
		setScaleAnimation(animation, AnimationFrames<Vector2>(Vector2(startValue, startValue), Vector2(endValue, endValue)));
	}

	void View::setScaleAnimation(const Ref<Animation>& animation, sl_real toValue)
	{
		setScaleAnimation(animation, AnimationFrames<Vector2>(getScale(), Vector2(toValue, toValue)));
	}

	Ref<Animation> View::createScaleAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewScaleAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationScale = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createScaleAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(getScale(), toValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createScaleAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(Vector2(startValue, startValue), Vector2(endValue, endValue)), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createScaleAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(getScale(), Vector2(toValue, toValue)), duration, onStop, curve, flags);
	}

	Ref<Animation> View::getRotateAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationRotate;
		}
		return sl_null;
	}

	void View::setRotateAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewRotateAnimationTarget(this, frames));
				attrs->m_animationRotate = animation;
			}
		}
	}

	void View::setRotateAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue)
	{
		setRotateAnimation(animation, AnimationFrames<sl_real>(startValue, endValue));
	}

	void View::setRotateAnimation(const Ref<Animation>& animation, sl_real toValue)
	{
		setRotateAnimation(animation, AnimationFrames<sl_real>(getRotation(), toValue));
	}

	Ref<Animation> View::createRotateAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewRotateAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationRotate = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createRotateAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(AnimationFrames<sl_real>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createRotateAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(AnimationFrames<sl_real>(getRotation(), toValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::getFrameAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationFrame;
		}
		return sl_null;
	}

	void View::setFrameAnimation(const Ref<Animation>& animation, const AnimationFrames<Rectangle>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewFrameAnimationTarget(this, frames));
				attrs->m_animationFrame = animation;
			}
		}
	}

	void View::setFrameAnimation(const Ref<Animation>& animation, const Rectangle& startValue, const Rectangle& endValue)
	{
		setFrameAnimation(animation, AnimationFrames<Rectangle>(startValue, endValue));
	}

	void View::setFrameAnimation(const Ref<Animation>& animation, const Rectangle& toValue)
	{
		setFrameAnimation(animation, AnimationFrames<Rectangle>(getFrame(), toValue));
	}

	Ref<Animation> View::createFrameAnimation(const AnimationFrames<Rectangle>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewFrameAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationFrame = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createFrameAnimation(const Rectangle& startValue, const Rectangle& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(AnimationFrames<Rectangle>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createFrameAnimationTo(const Rectangle& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(AnimationFrames<Rectangle>(getFrame(), toValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::getAlphaAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationAlpha;
		}
		return sl_null;
	}

	void View::setAlphaAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewAlphaAnimationTarget(this, frames));
				attrs->m_animationAlpha = animation;
			}
		}
	}

	void View::setAlphaAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue)
	{
		setAlphaAnimation(animation, AnimationFrames<sl_real>(startValue, endValue));
	}

	void View::setAlphaAnimation(const Ref<Animation>& animation, sl_real toValue)
	{
		setAlphaAnimation(animation, AnimationFrames<sl_real>(getAlpha(), toValue));
	}

	Ref<Animation> View::createAlphaAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewAlphaAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
			Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationAlpha = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createAlphaAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(AnimationFrames<sl_real>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createAlphaAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(AnimationFrames<sl_real>(getAlpha(), toValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::getBackgroundColorAnimation()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			return attrs->m_animationBackgroundColor;
		}
		return sl_null;
	}

	void View::setBackgroundColorAnimation(const Ref<Animation>& animation, const AnimationFrames<Color4f>& frames)
	{
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				animation->addTarget(new ViewBackgroundColorAnimationTarget(this, frames));
				attrs->m_animationBackgroundColor = animation;
			}
		}
	}

	void View::setBackgroundColorAnimation(const Ref<Animation>& animation, const Color4f& startValue, const Color4f& endValue)
	{
		setBackgroundColorAnimation(animation, AnimationFrames<Color4f>(startValue, endValue));
	}

	void View::setBackgroundColorAnimation(const Ref<Animation>& animation, const Color4f& toValue)
	{
		setBackgroundColorAnimation(animation, AnimationFrames<Color4f>(getBackgroundColor(), toValue));
	}

	Ref<Animation> View::createBackgroundColorAnimation(const AnimationFrames<Color4f>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> animation = createAnimation(new ViewBackgroundColorAnimationTarget(this, frames), duration, onStop, curve, flags);
		if (animation.isNotNull()) {
			_initializeTransformAttributes();
		Ref<TransformAttributes>& attrs = m_transformAttrs;
			if (attrs.isNotNull()) {
				attrs->m_animationBackgroundColor = animation;
			}
		}
		return animation;
	}

	Ref<Animation> View::createBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(AnimationFrames<Color4f>(startValue, endValue), duration, onStop, curve, flags);
	}

	Ref<Animation> View::createBackgroundColorAnimationTo(const Color4f& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(AnimationFrames<Color4f>(getBackgroundColor(), toValue), duration, onStop, curve, flags);
	}

	sl_bool View::isHorizontalScrolling()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagHorz;
		}
		return sl_false;
	}

	void View::setHorizontalScrolling(sl_bool flagHorizontal)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagHorz = flagHorizontal;
		}
	}

	sl_bool View::isVerticalScrolling()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagVert;
		}
		return sl_false;
	}

	void View::setVerticalScrolling(sl_bool flagVertical)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagVert = flagVertical;
		}
	}

	Ref<ScrollBar> View::getHorizontalScrollBar()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->horz;
		}
		return sl_null;
	}

	Ref<ScrollBar> View::getVerticalScrollBar()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->vert;
		}
		return sl_null;
	}

	void View::setHorizontalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			removeChild(attrs->horz, UIUpdateMode::NoRedraw);
			attrs->horz = bar;
			refreshScroll(mode);
		}
	}

	void View::setVerticalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
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
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagHorzScrollBarVisible;
		}
		return sl_true;
	}

	sl_bool View::isVerticalScrollBarVisible()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagVertScrollBarVisible;
		}
		return sl_true;
	}

	void View::setHorizontalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode)
	{
		setScrollBarsVisible(flagVisible, isVerticalScrollBarVisible(), mode);
	}

	void View::setVerticalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode)
	{
		setScrollBarsVisible(isHorizontalScrollBarVisible(), flagVisible, mode);
	}

	void View::setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagHorzScrollBarVisible = flagHorizontal;
			attrs->flagVertScrollBarVisible = flagVertical;
		}
		_initScrollBars(UIUpdateMode::NoRedraw);
		Ref<ScrollBar> bar;
		bar = getHorizontalScrollBar();
		if (bar.isNotNull()) {
			bar->setVisible(flagHorizontal, UIUpdateMode::NoRedraw);
		}
		bar = getVerticalScrollBar();
		if (bar.isNotNull()) {
			bar->setVisible(flagVertical, UIUpdateMode::NoRedraw);
		}
		refreshScroll(mode);
	}

	sl_bool View::isHorizontalScrollBarValid()
	{
		Ref<ScrollBar> bar = getHorizontalScrollBar();
		if (bar.isNotNull()) {
			return bar->isVisible() && bar->isValid();
		}
		return sl_false;
	}

	sl_bool View::isVerticalScrollBarValid()
	{
		Ref<ScrollBar> bar = getVerticalScrollBar();
		if (bar.isNotNull()) {
			return bar->isVisible() && bar->isValid();
		}
		return sl_false;
	}

	sl_scroll_pos View::getScrollX()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->x;
		}
		return 0;
	}

	sl_scroll_pos View::getScrollY()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->y;
		}
		return 0;
	}

	ScrollPoint View::getScrollPosition()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return ScrollPoint(attrs->x, attrs->y);
		}
		return ScrollPoint::zero();
	}

	void View::scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		
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
			
			if (_scrollTo(x, y, sl_true, sl_false)) {
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
	}

	void View::scrollTo(const ScrollPoint& position, UIUpdateMode mode)
	{
		scrollTo(position.x, position.y, mode);
	}

	void View::smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		
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
			
			_startContentScrollingFlow(sl_true, Pointlf(x, y));
			
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	void View::smoothScrollTo(const ScrollPoint& position, UIUpdateMode mode)
	{
		scrollTo(position.x, position.y, mode);
	}

	void View::setScrollX(sl_scroll_pos x, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			scrollTo(x, attrs->y, mode);
		}
	}

	void View::setScrollY(sl_scroll_pos y, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			scrollTo(attrs->x, y, mode);
		}
	}

	sl_scroll_pos View::getContentWidth()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->contentWidth;
		}
		return 0;
	}

	sl_scroll_pos View::getContentHeight()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->contentHeight;
		}
		return 0;
	}

	ScrollPoint View::getContentSize()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return ScrollPoint(attrs->contentWidth, attrs->contentHeight);
		}
		return ScrollPoint::zero();
	}

	void View::setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			if (Math::isAlmostZero(width - attrs->contentWidth) && Math::isAlmostZero(height - attrs->contentHeight)) {
				attrs->contentWidth = width;
				attrs->contentHeight = height;
				return;
			}
			attrs->contentWidth = width;
			attrs->contentHeight = height;
			_initScrollBars(UIUpdateMode::NoRedraw);
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
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			setContentSize(width, attrs->contentHeight, mode);
		}
	}

	void View::setContentHeight(sl_scroll_pos height, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			setContentSize(attrs->contentWidth, height, mode);
		}
	}

	ScrollPoint View::getScrollRange()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
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
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->barWidth;
		}
		return 0;
	}

	void View::setScrollBarWidth(sl_ui_len width, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->barWidth = width;
			refreshScroll(mode);
		}
	}

	sl_bool View::isContentScrollingByMouse()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagContentScrollingByMouse;
		}
		return sl_true;
	}

	void View::setContentScrollingByMouse(sl_bool flag)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagContentScrollingByMouse = flag;
		}
	}

	sl_bool View::isContentScrollingByTouch()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagContentScrollingByTouch;
		}
		return sl_true;
	}

	void View::setContentScrollingByTouch(sl_bool flag)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagContentScrollingByTouch = flag;
		}
	}

	sl_bool View::isContentScrollingByMouseWheel()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagContentScrollingByMouseWheel;
		}
		return sl_true;
	}

	void View::setContentScrollingByMouseWheel(sl_bool flag)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagContentScrollingByMouseWheel = flag;
		}
	}

	sl_bool View::isContentScrollingByKeyboard()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagContentScrollingByKeyboard;
		}
		return sl_true;
	}

	void View::setContentScrollingByKeyboard(sl_bool flag)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagContentScrollingByKeyboard = flag;
		}
	}

	sl_bool View::isSmoothContentScrolling()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagSmoothContentScrolling;
		}
		return sl_true;
	}

	void View::setSmoothContentScrolling(sl_bool flag)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagSmoothContentScrolling = flag;
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
		void onChange(ScrollBar* scrollBar, sl_scroll_pos value)
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
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			sl_ui_pos width = getWidth();
			sl_ui_pos height = getHeight();
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
			}
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
			}

			scrollTo(attrs->x, attrs->y, UIUpdateMode::NoRedraw);
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool View::isPaging()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagPaging;
		}
		return sl_false;
	}

	void View::setPaging(sl_bool flagPaging)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagPaging = flagPaging;
			onUpdatePaging();
		}
	}

	sl_ui_len View::getPageWidth()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->pageWidth;
		}
		return 0;
	}

	void View::setPageWidth(sl_ui_len width)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->pageWidth = width;
			onUpdatePaging();
		}
	}

	sl_ui_len View::getPageHeight()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->pageHeight;
		}
		return 0;
	}

	void View::setPageHeight(sl_ui_len height)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->pageHeight = height;
			onUpdatePaging();
		}
	}

	Ref<ScrollBar> View::_createHorizontalScrollBar()
	{
		Ref<ScrollBar> ret = new ScrollBar;
		if (ret.isNotNull()) {
			return ret;
		}
		return sl_null;
	}

	Ref<ScrollBar> View::_createVerticalScrollBar()
	{
		Ref<ScrollBar> ret = new ScrollBar;
		if (ret.isNotNull()) {
			return ret;
		}
		return sl_null;
	}


	void View::_getScrollBars(Ref<View> views[2])
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
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

	void View::_initScrollBars(UIUpdateMode mode)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			if (attrs->flagHorz && attrs->flagHorzScrollBarVisible) {
				if (!(attrs->flagInitHorzScrollBar)) {
					attrs->flagInitHorzScrollBar = sl_true;
					if (attrs->horz.isNull()) {
						createHorizontalScrollBar(mode);
					}
				}
			}
			if (attrs->flagVert && attrs->flagVertScrollBarVisible) {
				if (!(attrs->flagInitVertScrollBar)) {
					attrs->flagInitVertScrollBar = sl_true;
					if (attrs->vert.isNull()) {
						createVerticalScrollBar(mode);
					}
				}
			}
		}
	}

#define BOUNCE_WEIGHT 0.07f

	sl_bool View::_scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagFinish, sl_bool flagAnimate)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNull()) {
			return sl_false;
		}
		
		sl_bool flagFinishX = flagFinish;
		sl_bool flagFinishY = flagFinish;
		
		sl_scroll_pos comp;
		sl_real w = 0;
		if (attrs->flagHorz) {
			w = (sl_real)(getWidth());
			if (attrs->contentWidth > w) {
				comp = -(w * BOUNCE_WEIGHT);
				if (x < comp) {
					x = comp;
					flagFinishX = sl_true;
				}
				comp = attrs->contentWidth - w + (w * BOUNCE_WEIGHT);
				if (x > comp) {
					x = comp;
					flagFinishX = sl_true;
				}
			} else {
				flagFinishX = sl_true;
			}
		} else {
			flagFinishX = sl_true;
		}
		sl_real h = 0;
		if (attrs->flagVert) {
			h = (sl_real)(getHeight());
			if (attrs->contentHeight > h) {
				comp = -(h * BOUNCE_WEIGHT);
				if (y < comp) {
					y = comp;
					flagFinishY = sl_true;
				}
				comp = attrs->contentHeight - h + (h * BOUNCE_WEIGHT);
				if (y > comp) {
					y = comp;
					flagFinishY = sl_true;
				}
			} else {
				flagFinishY = sl_true;
			}
		} else {
			flagFinishY = sl_true;
		}
		
		sl_bool flagUpdated = sl_false;
		if (Math::isAlmostZero(attrs->x - x) && Math::isAlmostZero(attrs->y - y)) {
			attrs->x = x;
			attrs->y = y;
		} else {
			
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

			flagUpdated = sl_true;
		}
		
		if (flagAnimate) {
			if (flagFinishX && flagFinishY) {
				sl_bool flagTarget = sl_false;
				if (attrs->flagHorz) {
					if (x < 0) {
						x = 0;
						flagTarget = sl_true;
					}
					if (attrs->contentWidth > w) {
						if (x > attrs->contentWidth - w) {
							x = attrs->contentWidth - w;
							flagTarget = sl_true;
						}
					}
				}
				if (attrs->flagVert) {
					if (y < 0) {
						y = 0;
						flagTarget = sl_true;
					}
					if (attrs->contentHeight > h) {
						if (y > attrs->contentHeight - h) {
							y = attrs->contentHeight - h;
							flagTarget = sl_true;
						}
					}
				}
				if (flagTarget) {
					_startContentScrollingFlow(sl_true, Pointlf(x, y));
				} else {
					_stopContentScrollingFlow();
				}
			}
		} else {
			_stopContentScrollingFlow();
		}

		return flagUpdated;
	}

	sl_bool View::isTouchMultipleChildren()
	{
		return m_flagTouchMultipleChildren;
	}

	void View::setTouchMultipleChildren(sl_bool flag)
	{
		m_flagTouchMultipleChildren = flag;
	}

	sl_bool View::isPassingEventsToChildren()
	{
		return m_flagPassEventToChildren;
	}

	void View::setPassingEventsToChildren(sl_bool flag)
	{
		m_flagPassEventToChildren = flag;
	}

	sl_bool View::isProcessingOkCancel()
	{
		return m_flagProcessingOkCancel;
	}

	void View::setProcessingOkCancel(sl_bool flag)
	{
		m_flagProcessingOkCancel = flag;
	}

	Ref<View> View::getNextFocusableView()
	{
		Ref<View> parent = getParent();
		if (parent.isNull()) {
			return getFirstFocusableDescendant();
		}
		{
			sl_size index = 0;
			ListElements< Ref<View> > children(parent->getChildren());
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
			return getLastFocusableDescendant();
		}
		{
			sl_size index = 0;
			ListElements< Ref<View> > children(parent->getChildren());
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
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				Ref<View> v = child->getFirstFocusableDescendant();
				if (v.isNotNull()) {
					return v;
				}
			}
		}
		return sl_null;
	}

	Ref<View> View::getLastFocusableDescendant()
	{
		if (isFocusable()) {
			return this;
		}
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = children.count; i > 0; i--) {
			Ref<View> child = children[i - 1];
			if (child.isNotNull()) {
				Ref<View> v = child->getLastFocusableDescendant();
				if (v.isNotNull()) {
					return v;
				}
			}
		}
		return sl_null;
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
		if (view.isNotNull()) {
			if (view->m_viewPrevTabStop.isNull()) {
				view->m_viewPrevTabStop = this;
			}
		}
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
		if (view.isNotNull()) {
			if (view->m_viewNextTabStop.isNull()) {
				view->m_viewNextTabStop = this;
			}
		}
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

	Ref<GestureDetector> View::createGestureDetector()
	{
		Ref<GestureDetector> gesture = m_gestureDetector;
		if (gesture.isNull()) {
			gesture = new GestureDetector(this);
			m_gestureDetector = gesture;
		}
		return gesture;
	}

	Ref<GestureDetector> View::getGestureDetector()
	{
		return m_gestureDetector;
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
					if (isInstance()) {
						rc.right -= 1;
						rc.bottom -= 1;
					}
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
		
		CanvasType canvasType = canvas->getType();
		
		if (canvasType == CanvasType::Render) {
			
			RenderCanvas* render = static_cast<RenderCanvas*>(canvas);
			RenderCanvasState* currentState = render->getCurrentState();
			RenderCanvasState savedState(currentState);
			
			sl_bool flagTransformed = sl_false;
			
			for (sl_size i = 0; i < count; i++) {
				
				View* child = children[i].get();
				
				if (child) {

					if (child->isVisible()) {
						
						sl_ui_pos offx = child->m_frame.left;
						sl_ui_pos offy = child->m_frame.top;
						Matrix3 mat;
						sl_bool flagTranslation = sl_true;
						if (child->getFinalTransform(&mat)) {
							if (Transform2::isTranslation(mat)) {
								offx += (sl_ui_pos)(mat.m20);
								offy += (sl_ui_pos)(mat.m21);
							} else {
								flagTranslation = sl_false;
							}
						}
						if (flagTranslation) {
							UIRect rcInvalidated(rcInvalidatedParent.left - offx, rcInvalidatedParent.top - offy, rcInvalidatedParent.right - offx, rcInvalidatedParent.bottom - offy);
							if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
								if (flagTransformed) {
									currentState->copyFrom(&savedState);
									flagTransformed = sl_false;
								}
								render->translateFromSavedState(&savedState, (sl_real)(offx), (sl_real)(offy));
								render->setAlpha(alphaParent * child->getAlpha());
								canvas->setInvalidatedRect(rcInvalidated);
								child->dispatchDraw(render);
							}
						} else {
							UIRect rcInvalidated = child->convertCoordinateFromParent(rcInvalidatedParent);
							rcInvalidated.left -= 1;
							rcInvalidated.top -= 1;
							rcInvalidated.right += 1;
							rcInvalidated.bottom += 1;
							if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
								sl_real ax = (sl_real)(child->getWidth()) / 2;
								sl_real ay = (sl_real)(child->getHeight()) / 2;
								mat.m20 = -ax * mat.m00 - ay * mat.m10 + mat.m20 + ax + (sl_real)(offx);
								mat.m21 = -ax * mat.m01 - ay * mat.m11 + mat.m21 + ay + (sl_real)(offy);
								if (i != 0) {
									currentState->copyFrom(&savedState);
								}
								render->concatMatrix(mat);
								render->setAlpha(alphaParent * child->getAlpha());
								canvas->setInvalidatedRect(rcInvalidated);
								child->dispatchDraw(render);
								flagTransformed = sl_true;
							}
						}
					}
				}
			}
			
			currentState->copyFrom(&savedState);
			
		} else {
			
			for (sl_size i = 0; i < count; i++) {
				
				View* child = children[i].get();
				
				if (child) {
					
					if (child->isVisible()) {
						
						if (child->isInstance()) {
#if defined(SLIB_PLATFORM_IS_WIN32)
							if (!(child->checkSelfInvalidatable())) {
								sl_ui_pos offx = child->m_frame.left;
								sl_ui_pos offy = child->m_frame.top;
								Vector2 t;
								if (child->getFinalTranslationRotationScale(&t, sl_null, sl_null, sl_null)) {
									offx += (sl_ui_pos)(t.x);
									offy += (sl_ui_pos)(t.y);
								}
								UIRect rcInvalidated(rcInvalidatedParent.left - offx, rcInvalidatedParent.top - offy, rcInvalidatedParent.right - offx, rcInvalidatedParent.bottom - offy);
								if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
									CanvasStateScope scope(canvas);
									canvas->translate((sl_real)(offx), (sl_real)(offy));
									canvas->setAlpha(alphaParent * child->getAlpha());
									canvas->setInvalidatedRect(rcInvalidated);
									child->dispatchDraw(canvas);
								}
							}
#endif
						} else {
							sl_ui_pos offx = child->m_frame.left;
							sl_ui_pos offy = child->m_frame.top;
							Matrix3 mat;
							sl_bool flagTranslation = sl_true;
							if (child->getFinalTransform(&mat)) {
								if (Transform2::isTranslation(mat)) {
									offx += (sl_ui_pos)(mat.m20);
									offy += (sl_ui_pos)(mat.m21);
								} else {
									flagTranslation = sl_false;
								}
							}
							if (flagTranslation) {
								UIRect rcInvalidated(rcInvalidatedParent.left - offx, rcInvalidatedParent.top - offy, rcInvalidatedParent.right - offx, rcInvalidatedParent.bottom - offy);
								if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
									CanvasStateScope scope(canvas);
									canvas->translate((sl_real)(offx), (sl_real)(offy));
									canvas->setAlpha(alphaParent * child->getAlpha());
									canvas->setInvalidatedRect(rcInvalidated);
									child->dispatchDraw(canvas);
								}
							} else {
								UIRect rcInvalidated = child->convertCoordinateFromParent(rcInvalidatedParent);
								rcInvalidated.left -= 1;
								rcInvalidated.top -= 1;
								rcInvalidated.right += 1;
								rcInvalidated.bottom += 1;
								if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated)) {
									CanvasStateScope scope(canvas);
									sl_real ax = (sl_real)(child->getWidth()) / 2;
									sl_real ay = (sl_real)(child->getHeight()) / 2;
									mat.m20 = -ax * mat.m00 - ay * mat.m10 + mat.m20 + ax + (sl_real)(offx);
									mat.m21 = -ax * mat.m01 - ay * mat.m11 + mat.m21 + ay + (sl_real)(offy);
									canvas->concatMatrix(mat);
									canvas->setAlpha(alphaParent * child->getAlpha());
									canvas->setInvalidatedRect(rcInvalidated);
									child->dispatchDraw(canvas);
								}
							}
						}
					}
				}
			}
		}
		canvas->setAlpha(alphaParent);
		canvas->setInvalidatedRect(rcInvalidatedParent);
	}

	void View::drawContent(Canvas* canvas)
	{

		Ref<DrawAttributes>& drawAttrs = m_drawAttrs;
		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
	
		if (m_flagSavingCanvasState || scrollAttrs.isNotNull()) {
			CanvasStateScope scope(canvas);
			if (drawAttrs.isNotNull()) {
				if (drawAttrs->flagOnDrawBackgroundAlways || drawAttrs->backgroundColor.isNotZero() || drawAttrs->background.isNotNull()) {
					onDrawBackground(canvas);
				}
			}
			if (scrollAttrs.isNotNull()) {
				sl_real scrollX = (sl_real)(scrollAttrs->x);
				sl_real scrollY = (sl_real)(scrollAttrs->y);
				if(!(Math::isAlmostZero(scrollX)) || !(Math::isAlmostZero(scrollY))) {
					canvas->translate(-scrollX, -scrollY);
				}
			}
			onDraw(canvas);
			Ref<EventAttributes>& eventAttrs = m_eventAttrs;
			if (eventAttrs.isNotNull()) {
				(eventAttrs->draw)(this, canvas);
			}
		} else {
			if (drawAttrs.isNotNull()) {
				if (drawAttrs->flagOnDrawBackgroundAlways || drawAttrs->backgroundColor.isNotZero() || drawAttrs->background.isNotNull()) {
					onDrawBackground(canvas);
				}
			}
			onDraw(canvas);
			Ref<EventAttributes>& eventAttrs = m_eventAttrs;
			if (eventAttrs.isNotNull()) {
				(eventAttrs->draw)(this, canvas);
			}
		}
		
		if (m_children.isNotNull()) {
			ListElements< Ref<View> > children(getChildren());
			drawChildren(canvas, children.data, children.count);
		}
		
	}

#define MAX_LAYER_SIZE 8192

	Ref<Bitmap> View::drawLayer()
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& drawAttrs = m_drawAttrs;
		if (drawAttrs.isNull()) {
			return sl_null;
		}
		
		sl_uint32 width = getWidth();
		sl_uint32 height = getHeight();
		
		if (width == 0 || height == 0 || width > MAX_LAYER_SIZE || height > MAX_LAYER_SIZE) {
			return sl_null;
		}

		sl_bool flagInvalidate = drawAttrs->flagInvalidatedLayer;
		sl_bool flagInvalidateWhole = drawAttrs->flagInvalidatedWholeLayer;
		
		Ref<Bitmap> bitmap = drawAttrs->bitmapLayer;
		Ref<Canvas> canvas = drawAttrs->canvasLayer;
		
		if (bitmap.isNull() || bitmap->getWidth() < width || bitmap->getHeight() < height) {
			bitmap = Bitmap::create((width + 255) & 0xFFFFFF00, (height + 255) & 0xFFFFFF00);
			if (bitmap.isNull()) {
				return sl_null;
			}
			canvas = bitmap->getCanvas();
			if (canvas.isNull()) {
				return sl_null;
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

		if (m_flagClipping && drawAttrs->boundShape != BoundShape::Rectangle) {
			CanvasStateScope scope(canvas);
			clipBounds(canvas.get());
			drawContent(canvas.get());
		} else {
			drawContent(canvas.get());
		}

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
		
		if (m_flagClipping) {
			CanvasStateScope scope(canvas);
			clipBounds(canvas);
			drawContent(canvas);
		} else {
			drawContent(canvas);
		}
		
	}
	
	void View::clipBounds(Canvas* canvas)
	{
		Rectangle rcClip(0, 0, (sl_real)(m_frame.getWidth()), (sl_real)(m_frame.getHeight()));
		switch (getBoundShape()) {
			case BoundShape::Rectangle:
				canvas->clipToRectangle(rcClip);
				break;
			case BoundShape::RoundRect:
				canvas->clipToRoundRect(rcClip, getRoundRectBoundShapeRadius());
				break;
			case BoundShape::Ellipse:
				canvas->clipToEllipse(rcClip);
				break;
			case BoundShape::Path:
				canvas->clipToPath(getBoundShapePath());
				break;
			default:
				break;
		}
	}

	Size View::measureText(const String& text, const Ref<Font>& _font, sl_bool flagMultiLine)
	{
		if (!(isInstance())) {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				return parent->measureText(text, _font, flagMultiLine);
			}
		}
		Ref<Font> font = _font;
		if (font.isNull()) {
			font = getFont();
			if (font.isNull()) {
				return Size::zero();
			}
		}
		return font->measureText(text, flagMultiLine);
	}

	void View::runAfterDraw(const Function<void()>& callback, sl_bool flagInvalidate)
	{
		if (callback.isNotNull()) {
			_initializeDrawAttributes();
			Ref<DrawAttributes>& attrs = m_drawAttrs;
			if (attrs.isNotNull()) {
				if (attrs->runAfterDrawCallbacks.isNull()) {
					ObjectLocker lock(this);
					if (attrs->runAfterDrawCallbacks.isNull()) {
						attrs->runAfterDrawCallbacks.pushBack(callback);
						setDrawing(sl_true, UIUpdateMode::NoRedraw);
						if (flagInvalidate) {
							invalidate();
						}
						return;
					}
				}
				setDrawing(sl_true, UIUpdateMode::NoRedraw);
				attrs->runAfterDrawCallbacks.pushBack(callback);
				if (flagInvalidate) {
					invalidate();
				}
			}
		}
	}

	sl_bool View::isDrawingThread()
	{
		if (isInstance()) {
			return UI::isUiThread();
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				return parent->isDrawingThread();
			} else {
				return UI::isUiThread();
			}
		}
	}

	void View::dispatchToDrawingThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (isInstance()) {
			UI::dispatchToUiThread(callback, delayMillis);
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->dispatchToDrawingThread(callback, delayMillis);
			} else {
				UI::dispatchToUiThread(callback, delayMillis);
			}
		}
	}

	void View::runOnDrawingThread(const Function<void()>& callback)
	{
		if (isInstance()) {
			UI::runOnUiThread(callback);
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->runOnDrawingThread(callback);
			} else {
				UI::runOnUiThread(callback);
			}
		}
	}

	Ref<Dispatcher> View::getDispatcher()
	{
		if (isInstance()) {
			return UI::getDispatcher();
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				return parent->getDispatcher();
			} else {
				return UI::getDispatcher();
			}
		}
	}

	Ref<Timer> View::createTimer(const Function<void()>& task, sl_uint32 interval_ms, sl_bool flagStart)
	{
		return Timer::createWithDispatcher(getDispatcher(), task, interval_ms, flagStart);
	}

	Ptr<IViewListener> View::getEventListener()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->listener;
		}
		return sl_null;
	}

	void View::setEventListener(const Ptr<IViewListener>& listener)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->listener = listener;
		}
	}

	Function<void(View*, Canvas*)> View::getOnDraw()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->draw;
		}
		return sl_null;
	}

	void View::setOnDraw(const Function<void(View*, Canvas*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->draw = callback;
			setDrawing(sl_true, UIUpdateMode::NoRedraw);
		}
	}

	Function<void(View*, Canvas*)> View::getOnPreDraw()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->preDraw;
		}
		return sl_null;
	}

	void View::setOnPreDraw(const Function<void(View*, Canvas*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->preDraw = callback;
		}
	}

	Function<void(View*, Canvas*)> View::getOnPostDraw()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->postDraw;
		}
		return sl_null;
	}

	void View::setOnPostDraw(const Function<void(View*, Canvas*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->postDraw = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnMouseEvent()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->mouse;
		}
		return sl_null;
	}

	void View::setOnMouseEvent(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->mouse = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnTouchEvent()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->touch;
		}
		return sl_null;
	}

	void View::setOnTouchEvent(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->touch = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnKeyEvent()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->key;
		}
		return sl_null;
	}

	void View::setOnKeyEvent(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->key = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnMouseWheelEvent()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->mouseWheel;
		}
		return sl_null;
	}

	void View::setOnMouseWheelEvent(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->mouseWheel = callback;
		}
	}

	Function<void(View*)> View::getOnClick()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->click;
		}
		return sl_null;
	}

	void View::setOnClick(const Function<void(View*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->click = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnSetCursor()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->setCursor;
		}
		return sl_null;
	}

	void View::setOnSetCursor(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->setCursor = callback;
		}
	}

	Function<void(View*, sl_ui_len, sl_ui_len)> View::getOnResize()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->resize;
		}
		return sl_null;
	}

	void View::setOnResize(const Function<void(View*, sl_ui_len, sl_ui_len)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->resize = callback;
		}
	}

	Function<void(View*, sl_scroll_pos, sl_scroll_pos)> View::getOnScroll()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->scroll;
		}
		return sl_null;
	}

	void View::setOnScroll(const Function<void(View*, sl_scroll_pos, sl_scroll_pos)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->scroll = callback;
		}
	}

	Function<void(View*, GestureEvent*)> View::getOnSwipe()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->swipe;
		}
		return sl_null;
	}

	void View::setOnSwipe(const Function<void(View*, GestureEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->swipe = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnOK()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->ok;
		}
		return sl_null;
	}

	void View::setOnOK(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->ok = callback;
		}
	}

	Function<void(View*, UIEvent*)> View::getOnCancel()
	{
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			return attrs->cancel;
		}
		return sl_null;
	}

	void View::setOnCancel(const Function<void(View*, UIEvent*)>& callback)
	{
		_initializeEventAttributes();
		Ref<EventAttributes>& attrs = m_eventAttrs;
		if (attrs.isNotNull()) {
			attrs->cancel = callback;
		}
	}

	void View::dispatchDraw(Canvas* canvas)
	{
		
		Ref<DrawAttributes>& drawAttrs = m_drawAttrs;
		
		m_flagCurrentDrawing = sl_true;
		
		do {
			
			Rectangle rcInvalidated = canvas->getInvalidatedRect();
			if (rcInvalidated.right < rcInvalidated.left + SLIB_EPSILON) {
				break;
			}
			if (rcInvalidated.bottom < rcInvalidated.top + SLIB_EPSILON) {
				break;
			}
			
			if (!(isInstance())) {
				_makeLayout(sl_false);
			}

			if (m_flagDrawing) {
				
				m_rectCurrentDrawing.left = (sl_ui_pos)(rcInvalidated.left - SLIB_EPSILON);
				m_rectCurrentDrawing.top = (sl_ui_pos)(rcInvalidated.top - SLIB_EPSILON);
				m_rectCurrentDrawing.right = (sl_ui_pos)(rcInvalidated.right + SLIB_EPSILON);
				m_rectCurrentDrawing.bottom = (sl_ui_pos)(rcInvalidated.bottom + SLIB_EPSILON);
				
				if (drawAttrs.isNotNull() && drawAttrs->flagPreDrawEnabled) {
					onPreDraw(canvas);
					getOnPreDraw()(this, canvas);
				}
				
				draw(canvas);
				
				if (drawAttrs.isNotNull()) {
					if (drawAttrs->flagOnDrawBorderAlways || drawAttrs->penBorder.isNotNull()) {
						onDrawBorder(canvas);
					}
					if (drawAttrs->flagPostDrawEnabled) {
						onPostDraw(canvas);
						getOnPostDraw()(this, canvas);
					}
				}

			} else {
				if (m_children.isNotNull()) {
					ListElements< Ref<View> > children(getChildren());
					if (children.count > 0) {
						if (m_flagClipping) {
							CanvasStateScope scope(canvas);
							clipBounds(canvas);
							drawChildren(canvas, children.data, children.count);
						} else {
							drawChildren(canvas, children.data, children.count);
						}
					}
				}
			}
			
			if (m_scrollAttrs.isNotNull()) {
				Ref<View> scrollBars[2];
				_getScrollBars(scrollBars);
				if (scrollBars[0].isNotNull() || scrollBars[1].isNotNull()) {
					drawChildren(canvas, scrollBars, 2);
				}
			}
			
		} while (0);
		
		m_flagCurrentDrawing = sl_false;
		
		if (drawAttrs.isNotNull()) {
			sl_size n = drawAttrs->runAfterDrawCallbacks.getCount();
			Function<void()> callback;
			while (n > 0 && drawAttrs->runAfterDrawCallbacks.popFront(&callback)) {
				callback();
				n--;
			}
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

	void View::onDrawBackground(Canvas* canvas)
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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
		Ref<DrawAttributes>& attrs = m_drawAttrs;
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

	void View::onSwipe(GestureEvent* ev)
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

	void View::onDetach()
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

	void View::onUpdatePaging()
	{	
	}

	void View::onOK(UIEvent* ev)
	{
	}

	void View::onCancel(UIEvent* ev)
	{
	}

	static UIAction _View_getActionUp(UIAction actionDown)
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
					ListElements< Ref<View> > children(getChildren());
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
					dispatchMouseEventToChild(ev, oldChildMouseMove.get());
					ev->setAction(action);
					ev->setStoppedPropagation(flagSP);
					m_childMouseMove.setNull();
				}
			}
		}
		
		Ref<GestureDetector> gesture = m_gestureDetector;
		if (gesture.isNotNull()) {
			gesture->processEvent(ev);
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
		
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->mouse)(this, ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onMouseEvent(this, ev);
				if (ev->isPreventedDefault()) {
					return;
				}
			}
		}

		onMouseEvent(ev);
		if (ev->isPreventedDefault()) {
			return;
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
					View* child = children[count - 1 - i].get();
					if (POINT_EVENT_CHECK_CHILD(child)) {
						UIPointf pt = child->convertCoordinateFromParent(ptMouse);
						if (child->hitTest(pt)) {
							ev->setPoint(pt);
							dispatchMouseEventToChild(ev, child, sl_false);
							ev->setPoint(ptMouse);
							if (!(ev->isPassedToNext())) {
								oldChild = m_childMouseDown;
								if (oldChild.isNotNull() && oldChild != child) {
									ev->setAction(UIAction::TouchCancel);
									dispatchTouchEventToChild(ev, oldChild.get());
									ev->setAction(action);
								}
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
					dispatchMouseEventToChild(ev, oldChild.get());
				}
				return sl_true;
			case UIAction::LeftButtonDoubleClick:
			case UIAction::RightButtonDoubleClick:
			case UIAction::MiddleButtonDoubleClick:
				for (sl_size i = 0; i < count; i++) {
					View* child = children[count - 1 - i].get();
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
					dispatchMouseEventToChild(ev, oldChild.get());
					if (action == _View_getActionUp(m_actionMouseDown)) {
						m_childMouseDown.setNull();
						m_actionMouseDown = UIAction::Unknown;
					}
				}
				return sl_true;
			case UIAction::MouseMove:
			case UIAction::MouseEnter:
				oldChild = m_childMouseMove;
				for (sl_size i = 0; i < count; i++) {
					View* child = children[count - 1 - i].get();
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
									dispatchMouseEventToChild(ev, oldChild.get());
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
					dispatchMouseEventToChild(ev, oldChild.get());
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
					ListElements< Ref<View> > children(getChildren());
					if (children.count > 0) {
						if (m_flagTouchMultipleChildren) {
							dispatchTouchEventToMultipleChildren(ev, children.data, children.count);
						} else {
							dispatchTouchEventToChildren(ev, children.data, children.count);
						}
					}
				}
			}
		}
		
		Ref<GestureDetector> gesture = m_gestureDetector;
		if (gesture.isNotNull()) {
			gesture->processEvent(ev);
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
		
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->touch)(this, ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			(eventAttrs->mouse)(this, ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			PtrLocker<IViewListener> listener(eventAttrs->listener);
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
		}

		onTouchEvent(ev);
		if (ev->isPreventedDefault()) {
			return;
		}
		onMouseEvent(ev);
		if (ev->isPreventedDefault()) {
			return;
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
					View* child = children[count - 1 - i].get();
					if (POINT_EVENT_CHECK_CHILD(child)) {
						UIPointf pt = child->convertCoordinateFromParent(ptMouse);
						if (child->hitTest(pt)) {
							dispatchTouchEventToChild(ev, child);
							if (!(ev->isPassedToNext())) {
								oldChild = m_childMouseDown;
								if (oldChild.isNotNull() && oldChild != child) {
									ev->setAction(UIAction::TouchCancel);
									dispatchTouchEventToChild(ev, oldChild.get());
									ev->setAction(action);
								}
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
					dispatchTouchEventToChild(ev, oldChild.get());
					return sl_true;
				}
			case UIAction::TouchEnd:
			case UIAction::TouchCancel:
				oldChild = m_childMouseDown;
				if (oldChild.isNotNull()) {
					dispatchTouchEventToChild(ev, oldChild.get());
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

	void View::dispatchTouchEventToMultipleChildren(UIEvent *ev, const Ref<View>* children, sl_size count)
	{
		UIAction action = ev->getAction();
		
		Array<TouchPoint> ptsOriginal = ev->getTouchPoints();
		TouchPoint ptOriginal = ev->getTouchPoint();
		
		List< Ref<View> > selectedChildren;
		
		if (action != UIAction::TouchCancel) {
			
			sl_size nTouch = ptsOriginal.getCount();
			
			if (nTouch >= 2) {
				
#define MAX_TOUCH 10
				
				if (nTouch > MAX_TOUCH) {
					nTouch = MAX_TOUCH;
				}
				
				TouchPoint ptsCheck[MAX_TOUCH];
				TouchPoint ptsOutside[MAX_TOUCH];
				
				ptsOriginal.read(0, nTouch, ptsCheck);
				
				Array<TouchPoint> arrInside = Array<TouchPoint>::create(nTouch);
				TouchPoint* ptsInside = arrInside.getData();
				if (!ptsInside) {
					return;
				}
				
				sl_size nCheck = nTouch;
				
				for (sl_size i = 0; i < count && nCheck > 0; i++) {
					
					View* child = children[count - 1 - i].get();
					
					if (POINT_EVENT_CHECK_CHILD(child)) {
						
						sl_size nInside = 0;
						sl_size nOutside = 0;
						
						sl_size k = 0;
						for (; k < nCheck; k++) {
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
							if (nInside >= 2) {
								ev->setTouchPoints(arrInside.sub(0, nInside));
							} else {
								ev->setTouchPoints(sl_null);
							}
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
				
			} else {
				
				for (sl_size i = 0; i < count; i++) {
					
					View* child = children[count - 1 - i].get();
					
					if (POINT_EVENT_CHECK_CHILD(child)) {
						
						UIPointf pt = child->convertCoordinateFromParent(ptOriginal.point);
						if (child->hitTest(pt)) {
							dispatchTouchEventToChild(ev, child, sl_false);
							if (!(ev->isPassedToNext())) {
								selectedChildren.add_NoLock(child);
								break;
							}
						}
						
					}
					
				}
				
			}
			
			
		}
		
		// dispatch cancel events
		List< Ref<View> >& old = m_childrenMultiTouch;
		if (old.isNotNull()){
			{
				ListElements< Ref<View> > list(selectedChildren);
				for (sl_size i = 0; i < list.count; i++) {
					old.removeValue_NoLock(list[i]);
				}
			}
			
			sl_bool flagSP = ev->isStoppedPropagation();
			UIAction action = ev->getAction();
			
			ev->setTouchPoint(ptOriginal);
			ev->setTouchPoints(sl_null);
			
			ListElements< Ref<View> > list(old);
			for (sl_size i = 0; i < list.count; i++) {
				if (list[i].isNotNull()) {
					ev->setAction(UIAction::TouchCancel);
					dispatchTouchEventToChild(ev, list[i].get());
				}
			}
			
			ev->setAction(action);
			ev->setStoppedPropagation(flagSP);

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
				
				TouchPoint ptTouch = ev->getTouchPoint();
				Array<TouchPoint> arr = ev->getTouchPoints();

				sl_size n = arr.getCount();
				
				if (n > 0) {
					Array<TouchPoint> arrConverted = arr.duplicate();
					if (arrConverted.isEmpty()) {
						return;
					}
					TouchPoint* pts = arr.getData();
					TouchPoint* ptsConverted = arrConverted.getData();
					for (sl_size i = 0; i < n; i++) {
						ptsConverted[i].point = child->convertCoordinateFromParent(pts[i].point);
					}
					ev->setTouchPoints(arrConverted);
				} else {
					ev->setTouchPoints(sl_null);
				}
				
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
					ListElements< Ref<View> > children(getChildren());
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
		
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->mouseWheel)(this, ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onMouseWheelEvent(this, ev);
				if (ev->isPreventedDefault()) {
					return;
				}
			}
		}
		
		onMouseWheelEvent(ev);
		if (ev->isPreventedDefault()) {
			return;
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
			View* child = children[count - 1 - i].get();
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
		
		Ref<View> viewFocusedChild = m_childFocused;
		if (viewFocusedChild.isNotNull() && !(viewFocusedChild->isInstance())) {
			viewFocusedChild->dispatchKeyEvent(ev);
		}
		
		if (ev->isStoppedPropagation()) {
			return;
		}
		
		ev->resetStates();
		
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->key)(this, ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onKeyEvent(this, ev);
				if (ev->isPreventedDefault()) {
					return;
				}
			}
		}
		
		onKeyEvent(ev);
		
		if (ev->isPreventedDefault()) {
			return;
		}
		
		if (isContentScrollingByKeyboard()) {
			_processContentScrollingEvents(ev);
		}
		
		if (m_flagProcessingTabStop) {
			if (ev->getAction() == UIAction::KeyUp) {
				if (ev->getKeycode() == Keycode::Tab) {
					if (ev->isShiftKey()) {
						Ref<View> v = getPreviousTabStop();
						if (v.isNotNull() && v != this) {
							v->setFocus();
							ev->stopPropagation();
							ev->preventDefault();
						}
					} else {
						Ref<View> v = getNextTabStop();
						if (v.isNotNull() && v != this) {
							v->setFocus();
							ev->stopPropagation();
							ev->preventDefault();
						}
					}
				}
			}
		}
		
		if (m_flagProcessingOkCancel) {
			if (ev->getAction() == UIAction::KeyDown) {
				Keycode keycode = ev->getKeycode();
				if (keycode == Keycode::Enter) {
					dispatchOK();
					ev->stopPropagation();
					ev->preventDefault();
				} else if (keycode == Keycode::Escape) {
					dispatchCancel();
					ev->stopPropagation();
					ev->preventDefault();
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

		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->click)(this);
			if (ev->isPreventedDefault()) {
				return;
			}
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onClick(this, ev);
				if (ev->isPreventedDefault()) {
					return;
				}
			}
		}
		
		onClick(ev);

	}

	void View::dispatchClick()
	{
		Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::Unknown, 0, 0, Time::zero());
		if (ev.isNotNull()) {
			dispatchClick(ev.get());		
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
					ListElements< Ref<View> > children(getChildren());
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
		
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->setCursor)(this, ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onSetCursor(this, ev);
				if (ev->isPreventedDefault()) {
					return;
				}
			}
		}
		
		onSetCursor(ev);
		if (ev->isPreventedDefault()) {
			return;
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
			View* child = children[count - 1 - i].get();
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
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onResize(this, width, height);
			}
			(eventAttrs->resize)(this, width, height);
		}
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->onResizeChild(this, width, height);
		}
	}

	void View::dispatchChangeVisibility(Visibility oldVisibility, Visibility newVisibility)
	{
		onChangeVisibility(oldVisibility, newVisibility);
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onChangeVisibility(this, oldVisibility, newVisibility);
			}
		}
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->onChangeVisibilityOfChild(this, oldVisibility, newVisibility);
		}
	}

	void View::dispatchScroll(sl_scroll_pos x, sl_scroll_pos y)
	{
		onScroll(x, y);
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onScroll(this, x, y);
			}
			(eventAttrs->scroll)(this, x, y);
		}
	}

	void View::dispatchSwipe(GestureEvent* ev)
	{
		onSwipe(ev);
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			PtrLocker<IViewListener> listener(eventAttrs->listener);
			if (listener.isNotNull()) {
				listener->onSwipe(this, ev);
			}
			(eventAttrs->swipe)(this, ev);
		}
	}

	void View::dispatchOK(UIEvent* ev)
	{
		onOK(ev);
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->ok)(this, ev);
		}
		if (ev->isStoppedPropagation()) {
			return;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->dispatchOK(ev);
		} else {
			Ref<Window> window = m_window;
			if (window.isNotNull()) {
				window->dispatchOK();
			}
		}
	}

	void View::dispatchOK()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchOK(ev.get());
		}
	}

	void View::dispatchCancel(UIEvent* ev)
	{
		onCancel(ev);
		Ref<EventAttributes>& eventAttrs = m_eventAttrs;
		if (eventAttrs.isNotNull()) {
			(eventAttrs->cancel)(this, ev);
		}
		if (ev->isStoppedPropagation()) {
			return;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->dispatchCancel(ev);
		} else {
			Ref<Window> window = m_window;
			if (window.isNotNull()) {
				window->dispatchCancel();
			}
		}
	}

	void View::dispatchCancel()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchCancel(ev.get());
		}
	}

	void View::_processEventForStateAndClick(UIEvent* ev)
	{
		UIAction action = ev->getAction();
		switch (action) {
			case UIAction::LeftButtonDown:
			case UIAction::TouchBegin:
				setPressedState(sl_true);
				break;
			case UIAction::LeftButtonUp:
			case UIAction::TouchEnd:
				if (isPressedState()) {
					setPressedState(sl_false);
					if (getBounds().containsPoint(ev->getPoint())) {
						dispatchClick(ev);
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

	static void _View_scrollPagingElement(sl_scroll_pos& value, sl_scroll_pos speed, sl_scroll_pos pageSize)
	{
		if (pageSize < 1) {
			return;
		}
		speed = -speed;
		if (speed > pageSize * 0.4) {
			speed = pageSize * 0.4;
		}
		if (speed < -pageSize * 0.4) {
			speed = -pageSize * 0.4;
		}
		sl_scroll_pos page = Math::round(value / pageSize);
		sl_scroll_pos offset = value - page * pageSize;
		if (offset + speed > pageSize / 2) {
			value = (page + 1) * pageSize;
		} else if (offset + speed < - pageSize / 2) {
			value = (page - 1) * pageSize;
		} else {
			value = page * pageSize;
		}
	}

	void View::_processContentScrollingEvents(UIEvent* ev)
	{
		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
		if (scrollAttrs.isNull()) {
			return;
		}
		sl_bool flagHorz = scrollAttrs->flagHorz;
		if (flagHorz && scrollAttrs->contentWidth <= (sl_scroll_pos)(getWidth())) {
			flagHorz = sl_false;
		}
		sl_bool flagVert = scrollAttrs->flagVert;
		if (flagVert && scrollAttrs->contentHeight <= (sl_scroll_pos)(getHeight())) {
			flagVert = sl_false;
		}
		if (!flagHorz && !flagVert) {
			return;
		}
		
		sl_scroll_pos lineX = (sl_scroll_pos)(getWidth() / 20);
		sl_scroll_pos lineY = (sl_scroll_pos)(getHeight() / 20);
		
		if (scrollAttrs->flagSmoothContentScrolling) {
			if (scrollAttrs->motionTracker.isNull()) {
				ObjectLocker lock(this);
				if (scrollAttrs->motionTracker.isNull()) {
					scrollAttrs->motionTracker = new MotionTracker;
				}
			}
		}
		
		UIAction action = ev->getAction();
		switch (action) {
			case UIAction::LeftButtonDown:
			case UIAction::TouchBegin:
				_stopContentScrollingFlow();
				scrollAttrs->flagDownContent = sl_true;
				scrollAttrs->mousePointBefore = ev->getPoint();
				if (scrollAttrs->flagSmoothContentScrolling) {
					scrollAttrs->motionTracker->clearMovements();
					scrollAttrs->motionTracker->addMovement(ev);
				}
				ev->stopPropagation();
				break;
			case UIAction::LeftButtonDrag:
			case UIAction::TouchMove:
				_stopContentScrollingFlow();
				if (scrollAttrs->flagDownContent) {
					Point offset = ev->getPoint() - scrollAttrs->mousePointBefore;
					scrollAttrs->mousePointBefore = ev->getPoint();
					sl_scroll_pos sx = scrollAttrs->x;
					sl_scroll_pos sy = scrollAttrs->y;
					if (flagHorz) {
						sx -= offset.x;
					}
					if (flagVert) {
						sy -= offset.y;
					}
					if (scrollAttrs->flagSmoothContentScrolling) {
						_scrollTo(sx, sy, sl_true, sl_false);
						scrollAttrs->motionTracker->addMovement(ev);
						invalidate();
					} else {
						scrollTo(sx, sy);
					}
#if defined(SLIB_PLATFORM_IS_MOBILE)
					sl_real T = (sl_real)(UIResource::getScreenMinimum() / 200);
#else
					sl_real T = 2;
#endif
					if (offset.getLength2p() > T * T) {
						ev->setAction(UIAction::TouchCancel);
						Ref<View> view = m_childMouseDown;
						if (view.isNotNull()) {
							dispatchTouchEventToChild(ev, view.get());
						}
						ev->setAction(action);
					}
					ev->stopPropagation();
				}
				break;
			case UIAction::LeftButtonUp:
			case UIAction::TouchEnd:
				if (scrollAttrs->flagDownContent) {
					scrollAttrs->flagDownContent = sl_false;
					if (scrollAttrs->flagPaging) {
						sl_scroll_pos x = scrollAttrs->x;
						sl_scroll_pos y = scrollAttrs->y;
						Point speed = Point::zero();
						if (scrollAttrs->flagSmoothContentScrolling) {
							scrollAttrs->motionTracker->addMovement(ev);
							scrollAttrs->motionTracker->getVelocity(&speed);
						}
						if (flagHorz) {
							sl_scroll_pos pageWidth = (sl_scroll_pos)(scrollAttrs->pageWidth == 0 ? getWidth() : scrollAttrs->pageWidth);
							_View_scrollPagingElement(x, speed.x, pageWidth);
						}
						if (flagVert) {
							sl_scroll_pos pageHeight = (sl_scroll_pos)(scrollAttrs->pageHeight == 0 ? getHeight() : scrollAttrs->pageHeight);
							_View_scrollPagingElement(y, speed.y, pageHeight);
						}
						smoothScrollTo(x, y);
					} else {
						if (scrollAttrs->flagSmoothContentScrolling) {
							scrollAttrs->motionTracker->addMovement(ev);
							Point speed;
							if (scrollAttrs->motionTracker->getVelocity(&speed)) {
								if (!flagHorz) {
									speed.x = 0;
								}
								if (!flagVert) {
									speed.y = 0;
								}
								_startContentScrollingFlow(sl_false, speed);
							} else {
								_startContentScrollingFlow(sl_false, Point::zero());
							}
						}
					}
					ev->stopPropagation();
				}
				break;
			case UIAction::MouseWheel:
				{
					sl_bool flagChange = sl_false;
					sl_scroll_pos sx = scrollAttrs->x;
					sl_scroll_pos sy = scrollAttrs->y;
					
					if (flagHorz) {
						sl_real deltaX = ev->getDeltaX();
						if (deltaX > SLIB_EPSILON) {
							sx -= lineX;
							flagChange = sl_true;
						} else if (deltaX < -SLIB_EPSILON) {
							sx += lineX;
							flagChange = sl_true;
						}
					}
					if (flagVert) {
						sl_real deltaY = ev->getDeltaY();
						if (deltaY > SLIB_EPSILON) {
							sy -= lineY;
							flagChange = sl_true;
						} else if (deltaY < -SLIB_EPSILON) {
							sy += lineY;
							flagChange = sl_true;
						}
					}
					
					if (flagChange) {
						scrollTo(sx, sy);
						ev->stopPropagation();
					}
				}
				break;
			case UIAction::KeyDown:
				{
					sl_bool flagChange = sl_false;
					sl_scroll_pos sx = scrollAttrs->x;
					sl_scroll_pos sy = scrollAttrs->y;
					Keycode key = ev->getKeycode();
					if (flagHorz) {
						if (key == Keycode::Left) {
							sx -= lineX;
							flagChange = sl_true;
						} else if (key == Keycode::Right) {
							sx += lineX;
							flagChange = sl_true;
						}
					}
					if (flagVert) {
						if (key == Keycode::Up) {
							sy -= lineY;
							flagChange = sl_true;
						} else if (key == Keycode::Down) {
							sy += lineY;
							flagChange = sl_true;
						} else if (key == Keycode::PageUp) {
							sy -= (sl_scroll_pos)(getHeight());
							flagChange = sl_true;
						} else if (key == Keycode::PageDown) {
							sy += (sl_scroll_pos)(getHeight());
							flagChange = sl_true;
						}
					}
					if (flagChange) {
						scrollTo(sx, sy);
						ev->stopPropagation();
					}
				}
				break;
			default:
				break;
		}
	}

#define SMOOTH_SCROLL_FRAME_MS 15

	void View::_startContentScrollingFlow(sl_bool flagSmoothTarget, const Pointlf& speedOrTarget)
	{
		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
		if (scrollAttrs.isNull()) {
			return;
		}
		if (!(isDrawingThread())) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), View, _startContentScrollingFlow, this, flagSmoothTarget, speedOrTarget));
			return;
		}
		scrollAttrs->flagSmoothTarget = flagSmoothTarget;
		if (flagSmoothTarget) {
			scrollAttrs->xSmoothTarget = speedOrTarget.x;
			scrollAttrs->ySmoothTarget = speedOrTarget.y;
		} else {
			scrollAttrs->speedFlow = speedOrTarget;
		}
		scrollAttrs->timeFlowFrameBefore = Time::now();
		scrollAttrs->timerFlow = createTimer(SLIB_FUNCTION_WEAKREF(View, _processContentScrollingFlow, this), SMOOTH_SCROLL_FRAME_MS);
	}

	void View::_stopContentScrollingFlow()
	{
		if (!(isDrawingThread())) {
			dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(View, _stopContentScrollingFlow, this));
			return;
		}
		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
		if (scrollAttrs.isNull()) {
			return;
		}
		scrollAttrs->timerFlow.setNull();
	}

	static void _View_smoothScrollElement(sl_scroll_pos& value, sl_scroll_pos& target, sl_scroll_pos dt, sl_scroll_pos T, sl_bool& flagAnimating)
	{
		flagAnimating = sl_false;
		sl_scroll_pos offset = target - value;
		sl_scroll_pos offsetAbs = Math::abs(offset);
		if (offsetAbs > 1) {
			sl_scroll_pos speed;
			if (offsetAbs > T) {
				speed = offset;
			} else {
				speed = T * Math::sign(offset);
			}
			sl_scroll_pos add = speed * (dt * 3.5f);
			if (Math::abs(add) < offsetAbs) {
				value += add;
				flagAnimating = sl_true;
			} else {
				value = target;
			}
		} else {
			value = target;
		}
	}

	void View::_processContentScrollingFlow()
	{
		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
		if (scrollAttrs.isNull()) {
			return;
		}
		
		Time time = Time::now();
		sl_real dt = (sl_real)((time - scrollAttrs->timeFlowFrameBefore).getSecondsCountf());
		scrollAttrs->timeFlowFrameBefore = time;
		
#ifdef SLIB_PLATFORM_IS_MOBILE
		sl_real T = (sl_real)(UIResource::getScreenMinimum() / 2);
#else
		sl_real T = (sl_real)(UIResource::getScreenMinimum() / 4);
#endif
		
		if (scrollAttrs->flagSmoothTarget) {

			sl_bool flagX = sl_false, flagY = sl_false;
			
			sl_scroll_pos x = scrollAttrs->x;
			sl_scroll_pos y = scrollAttrs->y;
			_View_smoothScrollElement(x, scrollAttrs->xSmoothTarget, dt, T, flagX);
			_View_smoothScrollElement(y, scrollAttrs->ySmoothTarget, dt, T, flagY);
			
			_scrollTo(x, y, sl_false, sl_true);
			
			if (!flagX && !flagY) {
				_stopContentScrollingFlow();
			}
			
		} else {
			
			sl_scroll_pos x = scrollAttrs->x;
			sl_scroll_pos y = scrollAttrs->y;
			
			sl_bool flagFinish = sl_false;
			if (scrollAttrs->speedFlow.getLength() <= T / 5) {
				flagFinish = sl_true;
			} else {
				x -= scrollAttrs->speedFlow.x * dt;
				y -= scrollAttrs->speedFlow.y * dt;
				scrollAttrs->speedFlow *= 0.95f;
			}

			_scrollTo(x, y, flagFinish, sl_true);
			
		}
		
		invalidate();
		
	}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID)) && !(defined(SLIB_PLATFORM_IS_TIZEN))
	Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
	{
		return sl_null;
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

	ViewInstance::~ViewInstance()
	{
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
			sl_real alpha = view->getAlpha();
			Ref<View> parent = view->getParent();
			while (parent.isNotNull()) {
				alpha *= parent->getAlpha();
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
			view->dispatchClick();
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

	void ViewInstance::onSwipe(GestureType type)
	{
		Ref<View> view = getView();
		if (view.isNotNull()) {
			if (view->isEnabled()) {
				Ref<GestureEvent> ev = new GestureEvent;
				if (ev.isNotNull()) {
					ev->type = type;
					view->dispatchSwipe(ev.get());
				}
			}
		}
	}

	SLIB_DEFINE_OBJECT(ViewGroup, View)

	ViewGroup::ViewGroup()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setCreatingChildInstances(sl_true);
		setDrawing(sl_false, UIUpdateMode::Init);
		
	}

	ViewGroup::~ViewGroup()
	{
	}

}
