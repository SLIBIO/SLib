/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/ui/view.h"

#include "slib/ui/core.h"
#include "slib/ui/window.h"
#include "slib/ui/scroll_bar.h"
#include "slib/ui/render_view.h"
#include "slib/ui/animation.h"
#include "slib/ui/resource.h"
#include "slib/ui/scroll_view.h"

#include "slib/core/scoped.h"
#include "slib/core/timer.h"
#include "slib/math/transform2d.h"
#include "slib/graphics/bitmap.h"
#include "slib/graphics/util.h"
#include "slib/render/canvas.h"

#include "ui_animation.h"

namespace slib
{

/**********************
	View
**********************/

	SLIB_DEFINE_OBJECT(View, Object)

	View::View():
		m_flagCreatingInstance(sl_false),
		m_flagCreatingChildInstances(sl_false),
		m_flagCreatingNativeWidget(sl_false),
		m_flagCreatingLargeContent(sl_false),
		m_flagCreatingEmptyContent(sl_false),
		m_flagUsingChildLayouts(sl_true),
		m_flagEnabled(sl_true),
		m_flagHitTestable(sl_true),
		m_flagFocusable(sl_false),
		m_flagInstanceLayer(sl_false),
		m_flagClipping(sl_false),
		m_flagDrawing(sl_true),
		m_flagSavingCanvasState(sl_true),
		m_flagOkCancelEnabled(sl_true),
		m_flagTabStopEnabled(sl_true),
		m_flagKeepKeyboard(sl_false),
	
		m_flagCurrentCreatingInstance(sl_false),
		m_flagInvalidLayout(sl_true),
		m_flagNeedApplyLayout(sl_false),
		m_flagFocused(sl_false),
		m_flagPressed(sl_false),
		m_flagHover(sl_false),
	
		m_attachMode(UIAttachMode::AttachAlways),
		m_visibility(Visibility::Visible),
	
		m_frame(0, 0, 0, 0),
		m_boundsInParent(0, 0, 0, 0),
		m_idUpdateInvalidateLayout(0),
	
		m_actionMouseDown(UIAction::Unknown)
	{
	}

	View::~View()
	{
	}

#define DEFAULT_MAX_SIZE 0x3fffffff

	View::LayoutAttributes::LayoutAttributes():
		flagMarginLeftWeight(sl_false),
		flagMarginTopWeight(sl_false),
		flagMarginRightWeight(sl_false),
		flagMarginBottomWeight(sl_false),
		flagCustomLayout(sl_false),

		flagInvalidLayoutInParent(sl_false),
		flagRequestedFrame(sl_false),
	
		widthMode(SizeMode::Fixed),
		heightMode(SizeMode::Fixed),
		widthWeight(1),
		heightWeight(1),
	
		leftMode(PositionMode::Free),
		topMode(PositionMode::Free),
		rightMode(PositionMode::Free),
		bottomMode(PositionMode::Free),
	
		minWidth(0),
		maxWidth(DEFAULT_MAX_SIZE),
		minHeight(0),
		maxHeight(DEFAULT_MAX_SIZE),
	
		aspectRatioMode(AspectRatioMode::None),
		aspectRatio(1),
	
		marginLeft(0),
		marginTop(0),
		marginRight(0),
		marginBottom(0),
		marginLeftWeight(0),
		marginTopWeight(0),
		marginRightWeight(0),
		marginBottomWeight(0)
	{}

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
		attrs->layoutFrame = m_frame;
		attrs->requestedFrame = m_frame;
	}
	
	void View::LayoutAttributes::applyMarginWeightsX(sl_ui_pos parentWidth)
	{
		if (flagMarginLeftWeight) {
			marginLeft = (sl_ui_pos)((sl_real)parentWidth * marginLeftWeight);
		}
		if (flagMarginRightWeight) {
			marginRight = (sl_ui_pos)((sl_real)parentWidth * marginRightWeight);
		}
	}
	
	void View::LayoutAttributes::applyMarginWeightsY(sl_ui_pos parentHeight)
	{
		if (flagMarginTopWeight) {
			marginTop = (sl_ui_pos)((sl_real)parentHeight * marginTopWeight);
		}
		if (flagMarginBottomWeight) {
			marginBottom = (sl_ui_pos)((sl_real)parentHeight * marginBottomWeight);
		}
	}
	
	void View::LayoutAttributes::applyMarginWeights(sl_ui_pos parentWidth, sl_ui_pos parentHeight)
	{
		applyMarginWeightsX(parentWidth);
		applyMarginWeightsY(parentHeight);
	}

	View::PaddingAttributes::PaddingAttributes():
		flagPaddingLeftWeight(sl_false),
		flagPaddingTopWeight(sl_false),
		flagPaddingRightWeight(sl_false),
		flagPaddingBottomWeight(sl_false),
	
		paddingLeft(0),
		paddingTop(0),
		paddingRight(0),
		paddingBottom(0),
	
		paddingLeftWeight(0),
		paddingTopWeight(0),
		paddingRightWeight(0),
		paddingBottomWeight(0)
	{}
	
	View::PaddingAttributes::~PaddingAttributes()
	{
	}
	
	void View::_initializePaddingAttributes()
	{
		Ref<PaddingAttributes>& attrs = m_paddingAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new PaddingAttributes;
	}

	void View::PaddingAttributes::applyPaddingWeightsX(sl_ui_pos width)
	{
		if (flagPaddingLeftWeight) {
			paddingLeft = (sl_ui_pos)((sl_real)width * paddingLeftWeight);
		}
		if (flagPaddingRightWeight) {
			paddingRight = (sl_ui_pos)((sl_real)width * paddingRightWeight);
		}
	}
	
	void View::PaddingAttributes::applyPaddingWeightsY(sl_ui_pos height)
	{
		if (flagPaddingTopWeight) {
			paddingTop = (sl_ui_pos)((sl_real)height * paddingTopWeight);
		}
		if (flagPaddingBottomWeight) {
			paddingBottom = (sl_ui_pos)((sl_real)height * paddingBottomWeight);
		}
	}
	
	void View::PaddingAttributes::applyPaddingWeights(sl_ui_pos width, sl_ui_pos height)
	{
		applyPaddingWeightsX(width);
		applyPaddingWeightsY(height);
	}

	View::TransformAttributes::TransformAttributes():
		flagTransformFinalInvalid(sl_false),
		flagTransformFinal(sl_false),
		flagInverseTransformFinalInvalid(sl_false),
		flagInverseTransformFinal(sl_false),
		flagTransform(sl_false),
		flagTransformCalcInvalid(sl_false),
		flagTransformCalc(sl_false),
	
		translation(0, 0),
		scale(1, 1),
		rotationAngle(0),
		anchorOffset(0, 0)
	{}

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

	View::DrawAttributes::DrawAttributes():
		flagUsingFont(sl_false),
		flagOpaque(sl_false),
		flagLayer(sl_false),

		flagForcedDraw(sl_false),
		flagInvalidatedLayer(sl_true),
		flagInvalidatedWholeLayer(sl_true),
	
		backgroundScaleMode(ScaleMode::Stretch),
		backgroundAlignment(Alignment::MiddleCenter),
	
		boundShape(BoundShape::Rectangle),
		boundRadius(5, 5),

		contentShape(BoundShape::None),
		contentRadius(5, 5),
	
		borderColor(Color::Black),
		borderStyle(PenStyle::Solid),
		borderWidth(0),
	
		alpha(1)
	{}

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

	View::ScrollAttributes::ScrollAttributes():
		flagHorz(sl_false),
		flagVert(sl_false),
		flagHorzScrollBarVisible(sl_true),
		flagVertScrollBarVisible(sl_true),
		flagPaging(sl_false),
		flagContentScrollingByMouse(sl_true),
		flagContentScrollingByTouch(sl_true),
		flagContentScrollingByMouseWheel(sl_true),
		flagContentScrollingByKeyboard(sl_true),
		flagSmoothContentScrolling(sl_true),

		flagValidHorz(sl_false),
		flagValidVert(sl_false),
		flagInitHorzScrollBar(sl_false),
		flagInitVertScrollBar(sl_false),
		flagDownContent(sl_false),

		x(0),
		y(0),
		contentWidth(0),
		contentHeight(0),
		barWidth(UI::getDefaultScrollBarWidth()),
		pageWidth(0),
		pageHeight(0)
	{}

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

	View::ChildAttributes::ChildAttributes():
		flagTouchMultipleChildren(sl_false),
		flagPassEventToChildren(sl_true),
		flagHasInstances(sl_false)
	{}
	
	View::ChildAttributes::~ChildAttributes()
	{
	}
	
	void View::_initializeChildAttributes()
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new ChildAttributes;
	}
	
	View::OtherAttributes::OtherAttributes()
	{
	}
	
	View::OtherAttributes::~OtherAttributes()
	{
	}
	
	void View::_initializeOtherAttributes()
	{
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (attrs.isNotNull()) {
			return;
		}
		attrs = new OtherAttributes;
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

	Ref<ViewInstance> View::getNativeWidget()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull() && instance->isNativeWidget()) {
			return instance;
		}
		return sl_null;
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
				return instance->isValid(this);
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
	}

	sl_bool View::isCreatingNativeWidget()
	{
		return m_flagCreatingInstance && m_flagCreatingNativeWidget;
	}

	void View::setCreatingNativeWidget(sl_bool flag)
	{
		m_flagCreatingNativeWidget = flag;
		if (flag) {
			m_flagCreatingInstance = sl_true;
		}
	}
	
	sl_bool View::isCreatingLargeContent()
	{
		return m_flagCreatingLargeContent;
	}
	
	void View::setCreatingLargeContent(sl_bool flag)
	{
		m_flagCreatingLargeContent = flag;
	}

	sl_bool View::isCreatingEmptyContent()
	{
		return m_flagCreatingEmptyContent;
	}
	
	void View::setCreatingEmptyContent(sl_bool flag)
	{
		m_flagCreatingEmptyContent = flag;
	}

	UIAttachMode View::getAttachMode()
	{
		return m_attachMode;
	}

	void View::setAttachMode(UIAttachMode mode)
	{
		m_attachMode = mode;
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
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			return parent->getWindow();
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
		Ref<View> old = m_parent;
		if (old != parent) {
			onChangeParent(old.get(), parent.get());
			m_parent = parent;
		}
	}

	void View::_removeParent(View* _parent)
	{
		Ref<View> parent = m_parent;
		if (_parent) {
			if (parent == _parent) {
				onChangeParent(_parent, sl_null);
				m_parent.setNull();
			}
		} else {
			if (parent.isNotNull()) {
				onChangeParent(parent.get(), sl_null);
				m_parent.setNull();
			}
		}
	}

#if !defined(SLIB_UI)
	// Run on UI thread
	Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
	{
		return sl_null;
	}
#endif
	
	// Run on UI thread
	Ref<ViewInstance> View::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	// Run on UI thread
	Ref<ViewInstance> View::attachToNewInstance(ViewInstance* parent)
	{
		_detach();
		Ref<ViewInstance> instance = _createInstance(parent);
		if (instance.isNotNull()) {
			m_instance = instance;
			instance->setView(this);
			_doAttach();
		}
		return instance;
	}

	// Run on UI thread
	void View::_attach(const Ref<ViewInstance>& instance)
	{
		_detach();
		if (instance.isNotNull()) {
			m_instance = instance;
			instance->setView(this);
			_doAttach();
		}
	}

	void View::_detach()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			dispatchDetach();
			instance->setView(sl_null);
			m_instance.setNull();
		}
	}
	
	void View::_detachAll()
	{
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			children[i]->_detachAll();
		}
		_detach();
	}

	// Run on UI thread
	void View::_doAttach()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (m_flagFocused && m_flagFocusable) {
				instance->setFocus(this, sl_true);
			}
			Ref<GestureDetector> gesture = getGestureDetector();
			if (gesture.isNotNull()) {
				gesture->enableNative();
			}
			Ref<View> parent = m_parent;
			if (parent.isNull()) {
				if (IsInstanceOf<RenderView>(this)) {
					dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(View, _updateAndApplyLayout, this));
				} else {
					_updateAndApplyLayout();
				}
			} else {
				for (;;) {
					if (parent->isInstance() || parent->m_flagCurrentCreatingInstance) {
						break;
					}
					Ref<ChildAttributes>& attrs = parent->m_childAttrs;
					if (attrs.isNotNull()) {
						attrs->flagHasInstances = sl_true;
					}
					parent = parent->m_parent;
					if (parent.isNull()) {
						break;
					}
				}
			}
			dispatchAttach();
		}
		Ref<View> viewCreatingChildInstances = getNearestViewCreatingChildInstances();
		if (viewCreatingChildInstances.isNotNull()) {
			sl_bool flagNativeWidget = viewCreatingChildInstances->isNativeWidget();
			ListElements< Ref<View> > children(getChildren());
			for (sl_size i = 0; i < children.count; i++) {
#if defined(SLIB_UI_IS_WIN32)
				View* child = children[children.count - 1 - i].get();
#else
				View* child = children[i].get();
#endif
				if (!(child->isInstance())) {
					if (child->m_flagCreatingInstance) {
						switch(child->m_attachMode) {
							case UIAttachMode::NotAttach:
								break;
							case UIAttachMode::AttachAlways:
								viewCreatingChildInstances->_attachChild(child);
								break;
							case UIAttachMode::NotAttachInNativeWidget:
								if (!flagNativeWidget) {
									viewCreatingChildInstances->_attachChild(child);
								}
								break;
							case UIAttachMode::AttachInNativeWidget:
								if (flagNativeWidget) {
									viewCreatingChildInstances->_attachChild(child);
								}
								break;
							case UIAttachMode::AttachInInstance:
								if (isInstance()) {
									viewCreatingChildInstances->_attachChild(child);
								}
								break;
						}
						if (!(child->isInstance())) {
							child->_doAttach();
						}
					} else {
						child->_doAttach();
					}
				}
			}
		}
		if (isNativeWidget() && (isWidthWrapping() || isHeightWrapping())) {
			invalidateLayout();
		}
	}

	// Run on UI thread
	Ref<ViewInstance> View::_createInstance(ViewInstance* parent)
	{
		m_flagCurrentCreatingInstance = sl_true;
		if (m_flagCreatingNativeWidget) {
			Ref<ViewInstance> ret = createNativeWidget(parent);
			if (ret.isNotNull()) {
				ret->setNativeWidget(sl_true);
				m_flagCurrentCreatingInstance = sl_false;
				return ret;
			}
		}
		Ref<ViewInstance> ret = createGenericInstance(parent);
		m_flagCurrentCreatingInstance = sl_false;
		return ret;
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
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			if (attrs->children.isNotNull()) {
				List< Ref<View> > children = attrs->childrenCache;
				if (children.isNotNull()) {
					return children;
				}
				children = attrs->children.duplicate();
				attrs->childrenCache = children;
				return children;
			}
		}
		return sl_null;
	}

	sl_size View::getChildrenCount()
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			return attrs->children.getCount();
		}
		return 0;
	}

	Ref<View> View::getChild(sl_size index)
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			return attrs->children.getValueAt(index);
		}
		return sl_null;
	}

	void View::addChild(const Ref<View>& view, UIUpdateMode mode)
	{
		if (view.isNull()) {
			return;
		}
		Ref<View> viewCreatingChildInstances = getNearestViewCreatingChildInstances();
		if (viewCreatingChildInstances.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::addChild, view, mode)
		}
		_initializeChildAttributes();
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNull()) {
			return;
		}
		if (attrs->children.addIfNotExist(view)) {
			attrs->childrenCache.setNull();
			_addChild(view.get(), viewCreatingChildInstances.get(), mode);
		}
	}

	void View::insertChild(sl_size index, const Ref<View>& view, UIUpdateMode mode)
	{
		if (view.isNull()) {
			return;
		}
		Ref<View> viewCreatingChildInstances = getNearestViewCreatingChildInstances();
		if (viewCreatingChildInstances.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::insertChild, index, view, mode)
		}
		_initializeChildAttributes();
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNull()) {
			return;
		}
		if (attrs->children.insert(index, view)) {
			attrs->childrenCache.setNull();
			_addChild(view.get(), viewCreatingChildInstances.get(), mode);
		}
	}

	void View::removeChild(sl_size index, UIUpdateMode mode)
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNull()) {
			return;
		}
		Ref<View> view = attrs->children.getValueAt(index);
		if (view.isNull()) {
			return;
		}
		_removeChild(view.get());
		attrs->children.removeAt(index);
		attrs->childrenCache.setNull();
		
		if (view == attrs->childMouseDown) {
			attrs->childMouseDown.setNull();
		}
		if (view == attrs->childMouseMove) {
			attrs->childMouseMove.setNull();
		}
		if (view == attrs->childFocused) {
			if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
				_setFocusedChild(sl_null, UIUpdateMode::Init);
			} else {
				_setFocusedChild(sl_null, UIUpdateMode::None);
			}
		}
		invalidateLayout(mode);
	}

	void View::removeChild(const Ref<View>& view, UIUpdateMode mode)
	{
		if (view.isNull()) {
			return;
		}
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNull()) {
			return;
		}
		_removeChild(view.get());
		attrs->children.remove(view);
		attrs->childrenCache.setNull();
		
		if (view == attrs->childMouseDown) {
			attrs->childMouseDown.setNull();
		}
		if (view == attrs->childMouseMove) {
			attrs->childMouseMove.setNull();
		}
		if (view == attrs->childFocused) {
			if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
				_setFocusedChild(sl_null, UIUpdateMode::Init);
			} else {
				_setFocusedChild(sl_null, UIUpdateMode::None);
			}
		}
		invalidateLayout(mode);
	}

	void View::removeAllChildren(UIUpdateMode mode)
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNull()) {
			return;
		}
		if (isInstance()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::removeAllChildren, mode)
			ListLocker< Ref<View> > children(attrs->children);
			for (sl_size i = 0; i < children.count; i++) {
				Ref<View>& child = children[i];
				_removeChild(child.get());
			}
		} else {
			ListLocker< Ref<View> > children(attrs->children);
			for (sl_size i = 0; i < children.count; i++) {
				children[i]->_removeParent(this);
			}
		}
		attrs->children.removeAll();
		attrs->childrenCache.setNull();
		
		attrs->childMouseDown.setNull();
		attrs->childMouseMove.setNull();
		if (attrs->childFocused.isNotNull()) {
			if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
				_setFocusedChild(sl_null, UIUpdateMode::Init);
			} else {
				_setFocusedChild(sl_null, UIUpdateMode::None);
			}
		}
		invalidateLayout(mode);
	}

	Ref<View> View::getChildAt(sl_ui_pos x, sl_ui_pos y)
	{
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = children.count - 1, ii = 0; ii < children.count; i--, ii++) {
			Ref<View>& child = children[i];
			if (child->isVisible() && child->isHitTestable()) {
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

	Ref<View> View::getTopmostViewAt(sl_ui_pos x, sl_ui_pos y)
	{
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = children.count - 1, ii = 0; ii < children.count; i--, ii++) {
			Ref<View>& child = children[i];
			if (child->isVisible() && child->isHitTestable()) {
				UIPoint pt = child->convertCoordinateFromParent(UIPointf((sl_ui_posf)x, (sl_ui_posf)y));
				if (child->hitTest(pt)) {
					return child->getTopmostViewAt(pt.x, pt.y);
				}
			}
		}
		return this;
	}

	Ref<View> View::getTopmostViewAt(const UIPoint& point)
	{
		return getTopmostViewAt(point.x, point.y);
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
		Ref<View> parent = m_parent;
		return parent.isNull();
	}

	Ref<View> View::getNearestViewWithInstance()
	{
		if (m_instance.isNotNull()) {
			return this;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			return parent->getNearestViewWithInstance();
		}
		return sl_null;
	}

	Ref<ViewInstance> View::getNearestViewInstance()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			return parent->getNearestViewInstance();
		}
		return sl_null;
	}
	
	Ref<View> View::getNearestViewCreatingChildInstances()
	{
		if (!m_flagCreatingChildInstances) {
			return sl_null;
		}
		if (m_instance.isNotNull()) {
			return this;
		}
		if (m_flagCreatingInstance) {
			return sl_null;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			return parent->getNearestViewCreatingChildInstances();
		}
		return sl_null;
	}
	
	Ref<ViewPage> View::getNearestViewPage()
	{
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			ViewPage* page = CastInstance<ViewPage>(parent.get());
			if (page) {
				return page;
			}
			return parent->getNearestViewPage();
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

	void View::bringToFront(UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::bringToFront, mode)
		}
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			Ref<ChildAttributes>& attrsParent = parent->m_childAttrs;
			if (attrsParent.isNotNull()) {
				List< Ref<View> > children = attrsParent->children;
				MutexLocker lock(children.getLocker());
				sl_reg index = children.indexOf_NoLock(this);
				if (index >= 0) {
					children.removeAt_NoLock(index);
					children.add_NoLock(this);
					attrsParent->childrenCache.setNull();
					if (instance.isNull()) {
						invalidateBoundsInParent(mode);
						return;
					}
				}
			}
		}
		if (instance.isNotNull()) {
			instance->bringToFront(this);
		}
	}

	// Run on UI thread
	void View::_addChild(View* child, View* viewCreatingChildInstances, UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			child->setFocus(sl_false, UIUpdateMode::None);
		}
		
		child->setParent(this);
		onAddChild(child);
		
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			if (!(isCustomLayout())) {
				if (child->isDrawingThread()) {
					_updateAndApplyChildLayout(child);
				}
			}
		}
		
		child->_removeAllViewInstances();
		
		if (viewCreatingChildInstances) {
			if (child->m_flagCreatingInstance) {
				switch(child->getAttachMode()) {
					case UIAttachMode::NotAttach:
						break;
					case UIAttachMode::AttachAlways:
						viewCreatingChildInstances->_attachChild(child);
						break;
					case UIAttachMode::NotAttachInNativeWidget:
						if (!(viewCreatingChildInstances->isNativeWidget())) {
							viewCreatingChildInstances->_attachChild(child);
						}
						break;
					case UIAttachMode::AttachInNativeWidget:
						if (viewCreatingChildInstances->isNativeWidget()) {
							viewCreatingChildInstances->_attachChild(child);
						}
						break;
					case UIAttachMode::AttachInInstance:
						if (viewCreatingChildInstances == this) {
							viewCreatingChildInstances->_attachChild(child);
						}
						break;
				}
				if (!(child->isInstance())) {
					child->_doAttach();
				}
			} else {
				child->_doAttach();
			}
		}
		invalidateLayout(mode);
	}
	
	void View::_removeChild(View* child)
	{
		child->_cancelPressState();

		onRemoveChild(child);
		
		child->_removeAllViewInstances();
		child->_removeParent(this);
	}
	
	void View::_removeChildInstances(View* child)
	{
		Ref<ViewInstance> instanceParent = m_instance;
		if (instanceParent.isNull()) {
			return;
		}
		Ref<ViewInstance> instanceChild = child->m_instance;
		if (instanceChild.isNotNull()) {
			if (UI::isUiThread()) {
				instanceParent->removeChildInstance(this, instanceChild);
			} else {
				UI::dispatchToUiThreadUrgently(Function<void()>::with(ToRef(this), SLIB_BIND_WEAKREF(void(), ViewInstance, removeChildInstance, instanceParent, this, instanceChild)));
			}
			child->_detach();
			ListElements< Ref<View> > children(child->getChildren());
			for (sl_size i = 0; i < children.count; i++) {
				children[i]->_detachAll();
			}
		} else {
			ListElements< Ref<View> > children(child->getChildren());
			for (sl_size i = 0; i < children.count; i++) {
				_removeChildInstances(children[i].get());
			}
		}
	}
	
	void View::_removeAllViewInstances()
	{
		Ref<View> viewWithInstance;
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			viewWithInstance = parent->getNearestViewWithInstance();
		}
		if (viewWithInstance.isNotNull()) {
			viewWithInstance->_removeChildInstances(this);
		} else {
			if (isInstance()) {
				ListElements< Ref<View> > children(getChildren());
				for (sl_size i = 0; i < children.count; i++) {
					_removeChildInstances(children[i].get());
				}
				_detach();
			} else {
				_detachAll();
			}
		}
	}
	
	void View::_attachChild(const Ref<View>& child)
	{
		if (m_flagCreatingChildInstances) {
			if (child.isNotNull() && child->m_flagCreatingInstance) {
				SLIB_VIEW_RUN_ON_UI_THREAD(&View::_attachChild, child)
				Ref<ViewInstance> parentInstance = getViewInstance();
				if (parentInstance.isNotNull()) {
					child->attachToNewInstance(parentInstance.get());
				}
			}
		}
	}

	void View::invalidate(UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			return;
		}
		
		if (!(isDrawingThread())) {
			void (View::*f)(UIUpdateMode) = &View::invalidate;
			dispatchToDrawingThread(Function<void()>::bindWeakRef(this, f, mode));
			return;
		}
		
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull() && instance->isNativeWidget()) {
#if defined(SLIB_UI_IS_WIN32)
			if (!(IsInstanceOf<ScrollView>(this))) {
				return;
			}
#else
			return;
#endif
		}
		if (m_frame.getWidth() > 0 && m_frame.getHeight() > 0) {

			invalidateLayer();
			
			if (instance.isNotNull()) {
				instance->invalidate(this);
				return;
			}

			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->invalidate(m_boundsInParent, mode);
			}
		}
	}

	void View::invalidate(const UIRect& rect, UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			return;
		}
		
		if (!(isDrawingThread())) {
			void (View::*f)(const UIRect&, UIUpdateMode) = &View::invalidate;
			dispatchToDrawingThread(Function<void()>::bindWeakRef(this, f, rect, mode));
			return;
		}

		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull() && instance->isNativeWidget()) {
#if defined(SLIB_UI_IS_WIN32)
			if (!(IsInstanceOf<ScrollView>(this))) {
				return;
			}
#else
			return;
#endif
		}
		UIRect rectIntersect;
		if (getBounds().intersectRectangle(rect, &rectIntersect)) {

			invalidateLayer(rectIntersect);
			
			if (instance.isNotNull()) {
				instance->invalidate(this, rectIntersect);
				return;
			}

			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->invalidate(convertCoordinateToParent(rectIntersect), mode);
			}
		}
	}

	void View::invalidateBoundsInParent(UIUpdateMode mode)
	{
		Ref<View> parent = m_parent;
		if (parent.isNull() || isInstance()) {
			return;
		}
		parent->invalidate(m_boundsInParent, mode);
	}

	void View::updateAndInvalidateBoundsInParent(UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			m_boundsInParent = convertCoordinateToParent(getBounds());
			return;
		}
		Ref<View> parent = m_parent;
		if (parent.isNull() || isInstance()) {
			m_boundsInParent = convertCoordinateToParent(getBounds());
			return;
		}
		UIRect boundsOld = m_boundsInParent;
		if (Math::isAlmostZero(boundsOld.getWidth()) || Math::isAlmostZero(boundsOld.getHeight())) {
			m_boundsInParent = convertCoordinateToParent(getBounds());
			parent->invalidate(m_boundsInParent, mode);
			return;
		}
		UIRect boundsNew = convertCoordinateToParent(getBounds());
		boundsNew.left -= 2;
		boundsNew.top -= 2;
		boundsNew.right += 2;
		boundsNew.bottom += 2;
		m_boundsInParent = boundsNew;
		if (boundsOld.intersectRectangle(boundsNew)) {
			boundsNew.mergeRectangle(boundsOld);
			parent->invalidate(boundsNew, mode);
		} else {
			parent->invalidate(boundsOld, mode);
			parent->invalidate(boundsNew, mode);
		}
	}
	
	void View::_updateInstanceFrames()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::_updateInstanceFrames)
			instance->setFrame(this, getFrameInInstance());
		} else {
			Ref<ChildAttributes>& attrs = m_childAttrs;
			if (attrs.isNotNull() && attrs->flagHasInstances) {
				ListElements< Ref<View> > children(getChildren());
				for (sl_size i = 0; i < children.count; i++) {
					children[i]->_updateInstanceFrames();
				}
			}
		}
	}

	const UIRect& View::getFrame()
	{
		return m_frame;
	}
	
	void View::setFrame(const UIRect& _frame, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			requestFrame(_frame, mode);
			return;
		}
		
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;

		UIRect frame = _frame;
		_restrictSize(frame);
		
		UIRect frameOld = m_frame;
		
		sl_bool flagNotMoveX = Math::isAlmostZero(frameOld.left - frame.left);
		sl_bool flagNotMoveY = Math::isAlmostZero(frameOld.top - frame.top);
		
		sl_ui_len newWidth = frame.getWidth();
		sl_ui_len newHeight = frame.getHeight();
		sl_bool flagNotResizeWidth = Math::isAlmostZero(frameOld.getWidth() - newWidth);
		sl_bool flagNotResizeHeight = Math::isAlmostZero(frameOld.getHeight() - newHeight);
		
		if (flagNotMoveX && flagNotMoveY && flagNotResizeWidth && flagNotResizeHeight) {
			m_frame = frame;
			if (layoutAttrs.isNotNull()) {
				layoutAttrs->requestedFrame = frame;
				layoutAttrs->layoutFrame = frame;
			}
			return;
		}
		
		m_frame = frame;
		if (layoutAttrs.isNotNull()) {
			layoutAttrs->requestedFrame = frame;
			layoutAttrs->layoutFrame = frame;
		}

		_updateInstanceFrames();
		
		if (!(flagNotMoveX && flagNotMoveY)) {
			dispatchMove(frame.left, frame.top);
		}
		if (!(flagNotResizeWidth && flagNotResizeHeight)) {
			Ref<PaddingAttributes>& paddingAttrs = m_paddingAttrs;
			if (paddingAttrs.isNotNull()) {
				paddingAttrs->applyPaddingWeights(newWidth, newHeight);
			}
			dispatchResize(newWidth, newHeight);
			invalidateLayer();
		}
		updateAndInvalidateBoundsInParent(mode);
	}
	
	void View::setFrame(sl_ui_pos x, sl_ui_pos y, sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
	{
		setFrame(UIRect(x, y, x+width, y+height), mode);
	}

	void View::requestFrame(const UIRect& frame, UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
			layoutAttrs->requestedFrame = frame;
			_restrictSize(layoutAttrs->requestedFrame);
			layoutAttrs->flagRequestedFrame = sl_true;
		} else {
			setFrame(frame, UIUpdateMode::None);
		}
		invalidateSelfAndParentLayout(mode);
	}

	sl_ui_len View::getWidth()
	{
		return m_frame.getWidth();
	}

	void View::setWidth(sl_ui_len width, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
				layoutAttrs->requestedFrame.setWidth(width);
				_restrictSize(layoutAttrs->requestedFrame);
				layoutAttrs->flagRequestedFrame = sl_true;
			} else {
				UIRect frame = m_frame;
				frame.setWidth(width);
				setFrame(frame, UIUpdateMode::None);
			}
			invalidateSelfAndParentLayout(mode);
			return;
		}
		UIRect frame = m_frame;
		frame.setWidth(width);
		setFrame(frame, mode);
	}
	
	sl_ui_len View::getHeight()
	{
		return m_frame.getHeight();
	}

	void View::setHeight(sl_ui_len height, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
				layoutAttrs->requestedFrame.setHeight(height);
				_restrictSize(layoutAttrs->requestedFrame);
				layoutAttrs->flagRequestedFrame = sl_true;
			} else {
				UIRect frame = m_frame;
				frame.setHeight(height);
				setFrame(frame, UIUpdateMode::None);
			}
			invalidateSelfAndParentLayout(mode);
			return;
		}
		UIRect frame = m_frame;
		frame.setHeight(height);
		setFrame(frame, mode);
	}

	UISize View::getSize()
	{
		return m_frame.getSize();
	}

	void View::setSize(const UISize& size, UIUpdateMode mode)
	{
		setSize(size.x, size.y, mode);
	}

	void View::setSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
				layoutAttrs->requestedFrame.setSize(width, height);
				_restrictSize(layoutAttrs->requestedFrame);
				layoutAttrs->flagRequestedFrame = sl_true;
			} else {
				UIRect frame = m_frame;
				frame.setSize(width, height);
				setFrame(frame, UIUpdateMode::None);
			}
			invalidateSelfAndParentLayout(mode);
			return;
		}
		UIRect frame = m_frame;
		frame.setSize(width, height);
		setFrame(frame, mode);
	}

	sl_ui_pos View::getLeft()
	{
		return m_frame.left;
	}

	void View::setLeft(sl_ui_pos x, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
				layoutAttrs->requestedFrame.setLocationLeft(x);
				_restrictSize(layoutAttrs->requestedFrame);
				layoutAttrs->flagRequestedFrame = sl_true;
			} else {
				UIRect frame = m_frame;
				frame.setLocationLeft(x);
				setFrame(frame, UIUpdateMode::None);
			}
			invalidateSelfAndParentLayout(mode);
			return;
		}
		UIRect frame = m_frame;
		frame.setLocationLeft(x);
		setFrame(frame, mode);
	}

	sl_ui_pos View::getTop()
	{
		return m_frame.top;
	}

	void View::setTop(sl_ui_pos y, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
				layoutAttrs->requestedFrame.setLocationTop(y);
				_restrictSize(layoutAttrs->requestedFrame);
				layoutAttrs->flagRequestedFrame = sl_true;
			} else {
				UIRect frame = m_frame;
				frame.setLocationTop(y);
				setFrame(frame, UIUpdateMode::None);
			}
			invalidateSelfAndParentLayout(mode);
			return;
		}
		UIRect frame = m_frame;
		frame.setLocationTop(y);
		setFrame(frame, mode);
	}

	UIPoint View::getLocation()
	{
		return m_frame.getLocation();
	}

	void View::setLocation(const UIPoint& point, UIUpdateMode mode)
	{
		setLocation(point.x, point.y, mode);
	}

	void View::setLocation(sl_ui_pos x, sl_ui_pos y, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
			if (layoutAttrs.isNotNull() && m_parent.isNotNull()) {
				layoutAttrs->requestedFrame.setLocation(x, y);
				_restrictSize(layoutAttrs->requestedFrame);
				layoutAttrs->flagRequestedFrame = sl_true;
			} else {
				UIRect frame = m_frame;
				frame.setLocation(x, y);
				setFrame(frame, UIUpdateMode::None);
			}
			invalidateSelfAndParentLayout(mode);
			return;
		}
		UIRect frame = m_frame;
		frame.setLocation(x, y);
		setFrame(frame, mode);
	}

	UIRect View::getBounds()
	{
		return UIRect(0, 0, m_frame.getWidth(), m_frame.getHeight());
	}
	
	UIRect View::getFrameInInstance()
	{
		UIRect ret = m_frame;
		Ref<View> parent = m_parent;
		while (parent.isNotNull()) {
			if (parent->isInstance() || parent->m_flagCurrentCreatingInstance) {
				break;
			}
			UIRect& frameParent = parent->m_frame;
			ret.left += frameParent.left;
			ret.top += frameParent.top;
			ret.right += frameParent.left;
			ret.bottom += frameParent.top;
			parent = parent->m_parent;
		}
		return ret;
	}

	UIRect View::getBoundsInnerPadding()
	{
		UIEdgeInsets padding = getPadding();
		UIRect ret(getBounds());
		ret.left += padding.left;
		ret.top += padding.top;
		ret.right -= padding.right;
		ret.bottom -= padding.bottom;
		ret.fixSizeError();
		return ret;
	}

	UIRect View::getBoundsInParent()
	{
		return m_boundsInParent;
	}

	sl_bool View::getVisibleBounds(UIRect* outBounds)
	{
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			UIRect boundsParent;
			if (!(parent->getVisibleBounds(&boundsParent))) {
				return sl_false;
			}
			boundsParent = convertCoordinateFromParent(boundsParent);
			return getBounds().intersectRectangle(boundsParent, outBounds);
		} else {
			if (outBounds) {
				*outBounds = getBounds();
			}
			return sl_true;
		}
	}
	
	Visibility View::getVisibility()
	{
		return m_visibility;
	}

	void View::setVisibility(Visibility visibility, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			m_visibility = visibility;
			return;
		}
		
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::setVisibility, visibility, mode)
		}

		Visibility oldVisibility = m_visibility;
		m_visibility = visibility;
		if (oldVisibility == visibility) {
			return;
		}
		if (visibility != Visibility::Visible) {
			_cancelPressState();
		}
		
		_setInstanceVisible(visibility == Visibility::Visible);
		
		dispatchChangeVisibility(oldVisibility, visibility);
		
		switch (visibility) {
			case Visibility::Visible:
			case Visibility::Hidden:
				if (oldVisibility == Visibility::Gone) {
					invalidateParentLayout(mode);
				} else {
					if (!(isInstance())) {
						invalidateBoundsInParent(mode);
					}
				}
				break;
			case Visibility::Gone:
				invalidateParentLayout(mode);
				break;
		}
	}

	sl_bool View::isVisible()
	{
		return m_visibility == Visibility::Visible;
	}
	
	sl_bool View::isVisibleInInstance()
	{
		if (m_visibility != Visibility::Visible) {
			return sl_false;
		}
		Ref<View> parent = m_parent;
		while (parent.isNotNull()) {
			if (parent->isInstance() || parent->m_flagCurrentCreatingInstance) {
				return sl_true;
			}
			if (parent->m_visibility != Visibility::Visible) {
				return sl_false;
			}
			parent = parent->m_parent;
		}
		return sl_true;
	}

	void View::setVisible(sl_bool flag, UIUpdateMode mode)
	{
		if (flag) {
			setVisibility(Visibility::Visible, mode);
		} else {
			setVisibility(Visibility::Hidden, mode);
		}
	}

	// Run on UI Thread
	void View::_setInstanceVisible(sl_bool flag)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->setVisible(this, flag && m_visibility == Visibility::Visible);
		} else {
			Ref<ChildAttributes>& attrs = m_childAttrs;
			if (attrs.isNotNull() && attrs->flagHasInstances) {
				ListElements< Ref<View> > children(getChildren());
				for (sl_size i = 0; i < children.count; i++) {
					children[i]->_setInstanceVisible(flag && m_visibility == Visibility::Visible);
				}
			}
		}
	}
	
	sl_bool View::isEnabled()
	{
		return m_flagEnabled;
	}

	void View::setEnabled(sl_bool flag, UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::setEnabled, flag, mode)
			m_flagEnabled = flag;
			instance->setEnabled(this, flag);
		} else {
			m_flagEnabled = flag;
			invalidate(mode);
		}
	}
	
	sl_bool View::isClipping()
	{
		return m_flagClipping;
	}
	
	void View::setClipping(sl_bool flagClipping, UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::setClipping, flagClipping, mode)
			m_flagClipping = flagClipping;
			instance->setClipping(this, flagClipping);
		} else {
			m_flagClipping = flagClipping;
			invalidate(mode);
		}
	}
	
	sl_bool View::isDrawing()
	{
		return m_flagDrawing;
	}
	
	void View::setDrawing(sl_bool flagDrawing, UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::setDrawing, flagDrawing, mode)
			m_flagDrawing = flagDrawing;
			instance->setDrawing(this, flagDrawing);
		} else {
			m_flagDrawing = flagDrawing;
		}
		invalidate(mode);
	}
	
	sl_bool View::isSavingCanvasState()
	{
		return m_flagSavingCanvasState;
	}
	
	void View::setSavingCanvasState(sl_bool flag)
	{
		m_flagSavingCanvasState = flag;
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
		UIRect rc(getBounds());
		switch (getBoundShape()) {
			case BoundShape::RoundRect:
				return GraphicsUtil::containsPointInRoundRect(Point((sl_real)x, (sl_real)y), rc, getBoundRadius());
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
		if (isUiThread()) {
			_setFocus(flagFocused, sl_true, mode);
		} else {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), View, _setFocus, this, flagFocused, sl_true, mode));
		}
	}
	
	void View::_setFocus(sl_bool flagFocused, sl_bool flagApplyInstance, UIUpdateMode mode)
	{
		_killChildFocus();
		_setFocusedFlag(flagFocused, flagApplyInstance);
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			if (flagFocused) {
				parent->_setFocusedChild(this, mode);
			} else {
				parent->_setFocusedChild(sl_null, mode);
			}
			return;
		}
		invalidate(mode);
	}
	
	void View::_setFocusedFlag(sl_bool flagFocused, sl_bool flagApplyInstance)
	{
		if (flagApplyInstance) {
			Ref<ViewInstance> instance = getNearestViewInstance();
			if (instance.isNotNull()) {
				SLIB_VIEW_RUN_ON_UI_THREAD(&View::_setFocusedFlag, flagFocused, flagApplyInstance)
				instance->setFocus(this, flagFocused);
			}
		}
		if (m_flagFocused != flagFocused) {
			m_flagFocused = flagFocused;
			dispatchChangeFocus(flagFocused);
		}
	}

	void View::_killChildFocus()
	{
		Ref<ChildAttributes>& childAttrs = m_childAttrs;
		if (childAttrs.isNull()) {
			return;
		}
		Ref<View> child = childAttrs->childFocused;
		if (child.isNotNull()) {
			child->_setFocusedFlag(sl_false, sl_false);
			child->_killChildFocus();
		}
		childAttrs->childFocused.setNull();
	}

	void View::_setFocusedChild(View* child, UIUpdateMode mode)
	{
		Ref<ChildAttributes>& childAttrs = m_childAttrs;
		if (childAttrs.isNull()) {
			return;
		}
		Ref<View> old = childAttrs->childFocused;
		if (old != child) {
			if (old.isNotNull()) {
				old->_setFocusedFlag(sl_false, sl_false);
				old->_killChildFocus();
			}
			childAttrs->childFocused = child;
		}
		if (child) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				parent->_setFocusedChild(this, mode);
				return;
			}
		}
		invalidate(mode);
	}

	Ref<View> View::getFocusedChild()
	{
		Ref<ChildAttributes>& childAttrs = m_childAttrs;
		if (childAttrs.isNotNull()) {
			return childAttrs->childFocused;
		}
		return sl_null;
	}
	
	Ref<View> View::getFocusedDescendant()
	{
		Ref<View> focused = getFocusedChild();
		if (focused.isNotNull()) {
			Ref<View> descendant = focused->getFocusedDescendant();
			if (descendant.isNotNull()) {
				return descendant;
			}
			return focused;
		}
		return sl_null;
	}

	sl_bool View::isPressedState()
	{
		return m_flagPressed;
	}

	void View::setPressedState(sl_bool flagState, UIUpdateMode mode)
	{
		if (m_flagPressed != flagState) {
			m_flagPressed = flagState;
			if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
				Ref<DrawAttributes>& attrs = m_drawAttrs;
				if (attrs.isNotNull()) {
					if (attrs->backgroundPressed.isNotNull() && attrs->background != attrs->backgroundPressed) {
						invalidate();
					}
				}
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
			if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
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
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			return attrs->cursor;
		}
		return sl_null;
	}

	void View::setCursor(const Ref<Cursor>& cursor)
	{
		_initializeOtherAttributes();
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			attrs->cursor = cursor;
		}
	}

	void View::_restrictSize(sl_ui_len& width, sl_ui_len& height)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNull()) {
			if (width < 0) {
				width = 0;
			}
			if (height < 0) {
				height = 0;
			}
			return;
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
	}
	
	void View::_restrictSize(UIRect& rect)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNull()) {
			if (rect.right < rect.left) {
				rect.right = rect.left;
			}
			if (rect.bottom < rect.top) {
				rect.bottom = rect.top;
			}
			return;
		}
		sl_ui_len width = rect.right - rect.left;
		sl_ui_len height = rect.bottom - rect.top;
		if (layoutAttrs->aspectRatioMode == AspectRatioMode::AdjustWidth) {
			width = (sl_ui_pos)(height * layoutAttrs->aspectRatio);
		} else if (layoutAttrs->aspectRatioMode == AspectRatioMode::AdjustHeight) {
			if (layoutAttrs->aspectRatio > 0.0000001f) {
				height = (sl_ui_pos)(width / layoutAttrs->aspectRatio);
			}
		}
		width = Math::clamp(width, layoutAttrs->minWidth, layoutAttrs->maxWidth);
		height = Math::clamp(height, layoutAttrs->minHeight, layoutAttrs->maxHeight);
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
	}
	
	UIRect View::_updateLayoutFrameInParent_getReferFrame(const UpdateLayoutFrameParam& param, View* refer)
	{
		if (refer->m_parent == m_parent) {
			refer->updateLayoutFrameInParent(param);
			return refer->getLayoutFrame();
		}
		Ref<View> parentRefer = refer->m_parent;
		Ref<View> parent = m_parent;
		if (parentRefer.isNull() || parent.isNull()) {
			return refer->getLayoutFrame();
		}
		UIRect frame = refer->getLayoutFrame();
		UIPoint d(0, 0);
		Ref<View> view = parent;
		while (view.isNotNull()) {
			UIPoint r(0, 0);
			Ref<View> viewRefer = parentRefer;
			while (viewRefer.isNotNull()) {
				if (viewRefer == view) {
					frame.translate(r - d);
					return frame;
				}
				r += viewRefer->getLayoutFrame().getLeftTop();
				viewRefer = viewRefer->m_parent;
			}
			d += view->getLayoutFrame().getLeftTop();
			view = view->m_parent;
		}
		return frame;
	}
	
	void View::updateLayoutFrameInParent(const UpdateLayoutFrameParam& param)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNull()) {
			_updateLayout();
			return;
		}
		
		if (!(layoutAttrs->flagInvalidLayoutInParent)) {
			return;
		}
		layoutAttrs->flagInvalidLayoutInParent = sl_false;

		UIRect oldFrame = layoutAttrs->layoutFrame;
		UIRect frame;
		if (layoutAttrs->flagRequestedFrame) {
			frame = layoutAttrs->requestedFrame;
			layoutAttrs->flagRequestedFrame = sl_false;
		} else {
			frame = oldFrame;
		}
		
		if (!(param.flagUseLayout)) {
			if (!(oldFrame.getSize().isAlmostEqual(frame.getSize()))) {
				_setInvalidateLayout();
			}
			layoutAttrs->layoutFrame = frame;
			_updateLayout();
			if (!m_flagNeedApplyLayout) {
				if (!(layoutAttrs->layoutFrame.isAlmostEqual(m_frame))) {
					m_flagNeedApplyLayout = sl_true;
				}
			}
			return;
		}

		SizeMode widthMode = layoutAttrs->widthMode;
		SizeMode heightMode = layoutAttrs->heightMode;
		
		PositionMode leftMode = layoutAttrs->leftMode;
		PositionMode topMode = layoutAttrs->topMode;
		PositionMode rightMode = layoutAttrs->rightMode;
		PositionMode bottomMode = layoutAttrs->bottomMode;
		
		if (widthMode == SizeMode::Filling) {
			if (leftMode == PositionMode::CenterInParent || leftMode == PositionMode::CenterInOther) {
				leftMode = PositionMode::ParentEdge;
				rightMode = PositionMode::ParentEdge;
			}
		} else {
			if (leftMode != PositionMode::Free) {
				rightMode = PositionMode::Free;
			}
		}
		if (heightMode == SizeMode::Filling) {
			if (topMode == PositionMode::CenterInParent || topMode == PositionMode::CenterInOther) {
				topMode = PositionMode::ParentEdge;
				bottomMode = PositionMode::ParentEdge;
			}
		} else {
			if (topMode != PositionMode::Free) {
				bottomMode = PositionMode::Free;
			}
		}
		
		sl_ui_pos parentWidth = param.parentContentFrame.getWidth();
		sl_ui_pos parentHeight = param.parentContentFrame.getHeight();
		
		layoutAttrs->applyMarginWeights(parentWidth, parentHeight);
		
		for (int step = 0; step < 2; step++) {
			
			sl_ui_pos width = frame.getWidth();
			sl_ui_pos height = frame.getHeight();
			
			switch (widthMode) {
				case SizeMode::Weight:
					width = (sl_ui_pos)((sl_real)parentWidth * layoutAttrs->widthWeight);
					break;
				default:
					break;
			}
			
			switch (heightMode) {
				case SizeMode::Weight:
					height = (sl_ui_pos)((sl_real)parentHeight * layoutAttrs->heightWeight);
					break;
				default:
					break;
			}
			
			_restrictSize(width, height);
			
			Ref<View> referView;
			if (param.flagHorizontal) {
				switch (leftMode) {
					case PositionMode::ParentEdge:
						frame.left = param.parentContentFrame.left + layoutAttrs->marginLeft;
						break;
					case PositionMode::OtherStart:
						referView = layoutAttrs->leftReferingView;
						if (referView.isNotNull()) {
							frame.left = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).left + layoutAttrs->marginLeft;
						} else {
							frame.left = param.parentContentFrame.left + layoutAttrs->marginLeft;
						}
						break;
					case PositionMode::OtherEnd:
						referView = layoutAttrs->leftReferingView;
						if (referView.isNotNull()) {
							frame.left = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).right + layoutAttrs->marginLeft;
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
							UIRect referFrame = _updateLayoutFrameInParent_getReferFrame(param, referView.get());
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
							frame.right = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).left - layoutAttrs->marginRight;
						} else {
							frame.right = param.parentContentFrame.right - layoutAttrs->marginRight;
						}
						break;
					case PositionMode::OtherEnd:
						referView = layoutAttrs->rightReferingView;
						if (referView.isNotNull()) {
							frame.right = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).right - layoutAttrs->marginRight;
						} else {
							frame.right = param.parentContentFrame.right - layoutAttrs->marginRight;
						}
						break;
					default:
						frame.right = param.parentContentFrame.right;
						break;
				}
				if (widthMode == SizeMode::Filling) {
					if (frame.right < frame.left) {
						frame.right = frame.left;
					}
				} else {
					if (leftMode == PositionMode::Free && rightMode != PositionMode::Free) {
						frame.left = frame.right - width;
					} else {
						frame.right = frame.left + width;
					}
				}
			} else {
				frame.right = frame.left + width;
			}
			if (param.flagVertical) {
				switch (topMode) {
					case PositionMode::ParentEdge:
						frame.top = param.parentContentFrame.top + layoutAttrs->marginTop;
						break;
					case PositionMode::OtherStart:
						referView = layoutAttrs->topReferingView;
						if (referView.isNotNull()) {
							frame.top = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).top + layoutAttrs->marginTop;
						} else {
							frame.top = param.parentContentFrame.top + layoutAttrs->marginTop;
						}
						break;
					case PositionMode::OtherEnd:
						referView = layoutAttrs->topReferingView;
						if (referView.isNotNull()) {
							frame.top = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).bottom + layoutAttrs->marginTop;
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
							UIRect referFrame = _updateLayoutFrameInParent_getReferFrame(param, referView.get());
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
							frame.bottom = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).top - layoutAttrs->marginBottom;
						} else {
							frame.bottom = param.parentContentFrame.bottom - layoutAttrs->marginBottom;
						}
						break;
					case PositionMode::OtherEnd:
						referView = layoutAttrs->bottomReferingView;
						if (referView.isNotNull()) {
							frame.bottom = _updateLayoutFrameInParent_getReferFrame(param, referView.get()).bottom - layoutAttrs->marginBottom;
						} else {
							frame.bottom = param.parentContentFrame.bottom - layoutAttrs->marginBottom;
						}
						break;
					default:
						frame.bottom = param.parentContentFrame.bottom;
						break;
				}
				if (heightMode == SizeMode::Filling) {
					if (frame.bottom < frame.top) {
						frame.bottom = frame.top;
					}
				} else {
					if (topMode == PositionMode::Free && bottomMode != PositionMode::Free) {
						frame.top = frame.bottom - height;
					} else {
						frame.bottom = frame.top + height;
					}
				}
			} else {
				frame.bottom = frame.top + height;
			}
			
			_restrictSize(frame);
			
			if (step != 0) {
				break;
			}
				
			if (!(oldFrame.getSize().isAlmostEqual(frame.getSize()))) {
				_setInvalidateLayout();
			}

			oldFrame = frame;
			layoutAttrs->layoutFrame = frame;
			
			_updateLayout();
			
			frame = layoutAttrs->layoutFrame;
			sl_bool flagRelayout = sl_false;
			if (param.flagHorizontal) {
				if (!(Math::isAlmostZero(oldFrame.getWidth() - frame.getWidth()))) {
					flagRelayout = sl_true;
				}
			}
			if (param.flagVertical) {
				if (!(Math::isAlmostZero(oldFrame.getHeight() - frame.getHeight()))) {
					flagRelayout = sl_true;
				}
			}
			if (!flagRelayout) {
				break;
			}
		}
		
		layoutAttrs->layoutFrame = frame;
		
		if (!m_flagNeedApplyLayout) {
			if (!(frame.isAlmostEqual(m_frame))) {
				m_flagNeedApplyLayout = sl_true;
			}
		}

	}
	
	void View::_updateLayout()
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		
		while (m_flagInvalidLayout) {
			
			sl_int32 updateId = m_idUpdateInvalidateLayout;

			UIRect frame = getLayoutFrame();
			
			sl_size i;
			ListElements< Ref<View> > children(getChildren());
			for (int step = 0; step < 2; step++) {
				sl_ui_len width = frame.getWidth();
				sl_ui_len height = frame.getHeight();
				Ref<PaddingAttributes>& paddingAttrs = m_paddingAttrs;
				if (paddingAttrs.isNotNull()) {
					paddingAttrs->applyPaddingWeights(width, height);
				}
				if (children.count > 0 && (layoutAttrs.isNull() || !(layoutAttrs->flagCustomLayout))) {
					UpdateLayoutFrameParam param;
					Ref<PaddingAttributes>& paddingAttrs = m_paddingAttrs;
					if (paddingAttrs.isNotNull()) {
						param.parentContentFrame.left = paddingAttrs->paddingLeft;
						param.parentContentFrame.top = paddingAttrs->paddingTop;
						param.parentContentFrame.right = width - paddingAttrs->paddingRight;
						param.parentContentFrame.bottom = height - paddingAttrs->paddingBottom;
					} else {
						param.parentContentFrame.left = 0;
						param.parentContentFrame.top = 0;
						param.parentContentFrame.right = width;
						param.parentContentFrame.bottom = height;
					}
					param.flagUseLayout = m_flagUsingChildLayouts;
					param.flagHorizontal = sl_true;
					param.flagVertical = sl_true;
					for (i = 0; i < children.count; i++) {
						Ref<View>& child = children[i];
						Ref<LayoutAttributes>& childLayoutAttrs = child->m_layoutAttrs;
						if (childLayoutAttrs.isNotNull()) {
							childLayoutAttrs->flagInvalidLayoutInParent = sl_true;
						}
					}
					for (i = 0; i < children.count; i++) {
						Ref<View>& child = children[i];
						child->updateLayoutFrameInParent(param);
						if (child->m_flagNeedApplyLayout) {
							m_flagNeedApplyLayout = sl_true;
						}
					}
				}
				if (layoutAttrs.isNull()) {
					break;
				}
				if (layoutAttrs->flagCustomLayout || layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping) {
					onUpdateLayout();
					if (!m_flagNeedApplyLayout) {
						for (i = 0; i < children.count; i++) {
							Ref<View>& child = children[i];
							if (child->m_flagNeedApplyLayout) {
								m_flagNeedApplyLayout = sl_true;
							}
						}
					}
					_restrictSize(layoutAttrs->layoutFrame);
					if (!m_flagUsingChildLayouts) {
						break;
					}
					if (step != 0) {
						break;
					}
					UIRect oldFrame = frame;
					frame = layoutAttrs->layoutFrame;
					if (frame.isAlmostEqual(oldFrame)) {
						break;
					}
				}
				if (children.count == 0) {
					break;
				}
			}
			
			if (Base::interlockedIncrement32(&m_idUpdateInvalidateLayout) == updateId + 1) {
				m_flagInvalidLayout = sl_false;
				break;
			} else {
				m_flagInvalidLayout = sl_true;
			}
		}
		
	}
	
	void View::_applyLayout(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;

		if (!m_flagNeedApplyLayout) {
			invalidate(mode);
			return;
		}
		m_flagNeedApplyLayout = sl_false;
		
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			child->_applyLayout(UIUpdateMode::None);
		}
		
		if (layoutAttrs.isNotNull()) {
			setFrame(layoutAttrs->layoutFrame, UIUpdateMode::None);
		}
		invalidate(mode);
	}

	void View::_updateAndApplyChildLayout(View* child)
	{
		Ref<LayoutAttributes>& childLayoutAttrs = child->m_layoutAttrs;
		if (childLayoutAttrs.isNotNull()) {
			childLayoutAttrs->flagInvalidLayoutInParent = sl_true;
			UpdateLayoutFrameParam param;
			param.parentContentFrame = getBoundsInnerPadding();
			param.flagUseLayout = m_flagUsingChildLayouts;
			param.flagHorizontal = sl_true;
			param.flagVertical = sl_true;
			child->updateLayoutFrameInParent(param);
		} else {
			child->_updateLayout();
		}
		child->_applyLayout(UIUpdateMode::Redraw);
	}
	
	void View::_updateAndApplyLayoutWithMode(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		
		if (!m_flagInvalidLayout) {
			invalidate(mode);
			return;
		}
		
		if (layoutAttrs.isNotNull()) {
			if (layoutAttrs->flagRequestedFrame) {
				layoutAttrs->layoutFrame = layoutAttrs->requestedFrame;
				layoutAttrs->flagRequestedFrame = sl_false;
			}
		}
		
		_updateLayout();
		
		if (!m_flagNeedApplyLayout && layoutAttrs.isNotNull()) {
			if (!(m_frame.isAlmostEqual(layoutAttrs->layoutFrame))) {
				m_flagNeedApplyLayout = sl_true;
			}
		}
		
		_applyLayout(mode);
	}
	
	void View::_updateAndApplyLayout()
	{
		_updateAndApplyLayoutWithMode(UIUpdateMode::Redraw);
	}
	
	void View::_setInvalidateLayout()
	{
		m_flagInvalidLayout = sl_true;
		Base::interlockedIncrement32(&m_idUpdateInvalidateLayout);
	}
	
	sl_ui_len View::_measureLayoutWrappingSize_Horz(View* view, Pair<sl_ui_len, sl_ui_len>& insets, HashMap< View*, Pair<sl_ui_len, sl_ui_len> >& map, sl_ui_pos paddingLeft, sl_ui_pos paddingRight)
	{
		LayoutAttributes* layoutAttrs = view->m_layoutAttrs.get();
		if (!layoutAttrs) {
			insets.first = view->m_frame.left;
			insets.second = 0;
			return view->m_frame.getWidth();
		}
		SizeMode widthMode = layoutAttrs->widthMode;
		PositionMode leftMode = layoutAttrs->leftMode;
		PositionMode rightMode = layoutAttrs->rightMode;
		if (widthMode == SizeMode::Filling) {
			if (leftMode == PositionMode::CenterInParent || leftMode == PositionMode::CenterInOther) {
				leftMode = PositionMode::ParentEdge;
				rightMode = PositionMode::ParentEdge;
			}
		} else {
			if (leftMode != PositionMode::Free) {
				rightMode = PositionMode::Free;
			}
		}
		if (leftMode != PositionMode::OtherStart && leftMode != PositionMode::OtherEnd && leftMode != PositionMode::CenterInOther && rightMode != PositionMode::OtherStart && rightMode != PositionMode::OtherEnd) {
			if (leftMode == PositionMode::Free && rightMode == PositionMode::Free) {
				insets.first = layoutAttrs->layoutFrame.left;
				insets.second = 0;
			} else {
				insets.first = paddingLeft + layoutAttrs->marginLeft;
				insets.second = paddingRight + layoutAttrs->marginRight;
			}
		} else {
			if (!(map.get_NoLock(view, &insets))) {
				insets.first = paddingLeft + layoutAttrs->marginLeft;
				insets.second = paddingRight + layoutAttrs->marginRight;
				if (leftMode == PositionMode::CenterInOther) {
					Ref<View> refer = layoutAttrs->leftReferingView;
					if (widthMode != SizeMode::Filling && refer.isNotNull()) {
						Pair<sl_ui_len, sl_ui_len> insetsRefer;
						sl_ui_len widthRefer = _measureLayoutWrappingSize_Horz(refer.get(), insetsRefer, map, paddingLeft, paddingRight);
						sl_ui_len diff = widthRefer / 2 - layoutAttrs->layoutFrame.getWidth() / 2;
						insets.first = Math::max(insets.first, insetsRefer.first + diff);
						insets.second = Math::max(insets.second, insetsRefer.second + diff);
					}
				} else {
					if (leftMode == PositionMode::OtherStart || leftMode == PositionMode::OtherEnd) {
						Ref<View> refer = layoutAttrs->leftReferingView;
						if (refer.isNotNull()) {
							Pair<sl_ui_len, sl_ui_len> insetsRefer;
							sl_ui_len widthRefer = _measureLayoutWrappingSize_Horz(refer.get(), insetsRefer, map, paddingLeft, paddingRight);
							insets.first = insetsRefer.first + layoutAttrs->marginLeft;
							if (leftMode == PositionMode::OtherEnd) {
								insets.first += widthRefer;
							}
							if (rightMode == PositionMode::Free) {
								sl_ui_len t = insetsRefer.second - layoutAttrs->layoutFrame.getWidth();
								if (leftMode == PositionMode::OtherStart) {
									t += widthRefer;
								}
								insets.second = Math::max(insets.second, t);
							}
						}
					}
					if (rightMode == PositionMode::OtherStart || rightMode == PositionMode::OtherEnd) {
						Ref<View> refer = layoutAttrs->rightReferingView;
						if (refer.isNotNull()) {
							Pair<sl_ui_len, sl_ui_len> insetsRefer;
							sl_ui_len widthRefer = _measureLayoutWrappingSize_Horz(refer.get(), insetsRefer, map, paddingLeft, paddingRight);
							insets.second = insetsRefer.second + layoutAttrs->marginRight;
							if (rightMode == PositionMode::OtherStart) {
								insets.second += widthRefer;
							}
							if (leftMode == PositionMode::Free) {
								sl_ui_len t = insetsRefer.first - layoutAttrs->layoutFrame.getWidth();
								if (rightMode == PositionMode::OtherEnd) {
									t += widthRefer;
								}
								insets.first = Math::max(insets.first, t);
							}
						}
					}
				}
				map.put_NoLock(view, insets);
			}
		}
		if (widthMode != SizeMode::Filling) {
			return layoutAttrs->layoutFrame.getWidth();
		} else {
			return 0;
		}
	}
	
	sl_ui_len View::_measureLayoutWrappingSize_Vert(View* view, Pair<sl_ui_len, sl_ui_len>& insets, HashMap< View*, Pair<sl_ui_len, sl_ui_len> >& map, sl_ui_pos paddingTop, sl_ui_pos paddingBottom)
	{
		LayoutAttributes* layoutAttrs = view->m_layoutAttrs.get();
		if (!layoutAttrs) {
			insets.first = view->m_frame.top;
			insets.second = 0;
			return view->m_frame.getHeight();
		}
		SizeMode heightMode = layoutAttrs->heightMode;
		PositionMode topMode = layoutAttrs->topMode;
		PositionMode bottomMode = layoutAttrs->bottomMode;
		if (heightMode == SizeMode::Filling) {
			if (topMode == PositionMode::CenterInParent || topMode == PositionMode::CenterInOther) {
				topMode = PositionMode::ParentEdge;
				bottomMode = PositionMode::ParentEdge;
			}
		} else {
			if (topMode != PositionMode::Free) {
				bottomMode = PositionMode::Free;
			}
		}
		if (topMode != PositionMode::OtherStart && topMode != PositionMode::OtherEnd && topMode != PositionMode::CenterInOther && bottomMode != PositionMode::OtherStart && bottomMode != PositionMode::OtherEnd) {
			if (topMode == PositionMode::Free && bottomMode == PositionMode::Free) {
				insets.first = layoutAttrs->layoutFrame.top;
				insets.second = 0;
			} else {
				insets.first = paddingTop + layoutAttrs->marginTop;
				insets.second = paddingBottom + layoutAttrs->marginBottom;
			}
		} else {
			if (!(map.get_NoLock(view, &insets))) {
				insets.first = paddingTop + layoutAttrs->marginTop;
				insets.second = paddingBottom + layoutAttrs->marginBottom;
				if (topMode == PositionMode::CenterInOther) {
					Ref<View> refer = layoutAttrs->topReferingView;
					if (heightMode != SizeMode::Filling && refer.isNotNull()) {
						Pair<sl_ui_len, sl_ui_len> insetsRefer;
						sl_ui_len heightRefer = _measureLayoutWrappingSize_Vert(refer.get(), insetsRefer, map, paddingTop, paddingBottom);
						sl_ui_len diff = heightRefer / 2 - layoutAttrs->layoutFrame.getHeight() / 2;
						insets.first = Math::max(insets.first, insetsRefer.first + diff);
						insets.second = Math::max(insets.second, insetsRefer.second + diff);
					}
				} else {
					if (topMode == PositionMode::OtherStart || topMode == PositionMode::OtherEnd) {
						Ref<View> refer = layoutAttrs->topReferingView;
						if (refer.isNotNull()) {
							Pair<sl_ui_len, sl_ui_len> insetsRefer;
							sl_ui_len heightRefer = _measureLayoutWrappingSize_Vert(refer.get(), insetsRefer, map, paddingTop, paddingBottom);
							insets.first = insetsRefer.first + layoutAttrs->marginTop;
							if (topMode == PositionMode::OtherEnd) {
								insets.first += heightRefer;
							}
							if (bottomMode == PositionMode::Free) {
								sl_ui_len t = insetsRefer.second - layoutAttrs->layoutFrame.getHeight();
								if (topMode == PositionMode::OtherStart) {
									t += heightRefer;
								}
								insets.second = Math::max(insets.second, t);
							}
						}
					}
					if (bottomMode == PositionMode::OtherStart || bottomMode == PositionMode::OtherEnd) {
						Ref<View> refer = layoutAttrs->bottomReferingView;
						if (refer.isNotNull()) {
							Pair<sl_ui_len, sl_ui_len> insetsRefer;
							sl_ui_len heightRefer = _measureLayoutWrappingSize_Vert(refer.get(), insetsRefer, map, paddingTop, paddingBottom);
							insets.second = insetsRefer.second + layoutAttrs->marginBottom;
							if (bottomMode == PositionMode::OtherStart) {
								insets.second += heightRefer;
							}
							if (topMode == PositionMode::Free) {
								sl_ui_len t = insetsRefer.first - layoutAttrs->layoutFrame.getHeight();
								if (bottomMode == PositionMode::OtherEnd) {
									t += heightRefer;
								}
								insets.first = Math::max(insets.first, t);
							}
						}
					}
				}
				map.put_NoLock(view, insets);
			}
		}
		if (heightMode != SizeMode::Filling) {
			return layoutAttrs->layoutFrame.getHeight();
		} else {
			return 0;
		}
	}
	
	UISize View::measureLayoutWrappingSize(sl_bool flagHorizontal, sl_bool flagVertical)
	{

		UISize ret = {0, 0};
		if (!flagVertical && !flagHorizontal) {
			return ret;
		}
		
		HashMap< View*, Pair<sl_ui_len, sl_ui_len> > mapHorzInsets;
		HashMap< View*, Pair<sl_ui_len, sl_ui_len> > mapVertInsets;
		
		sl_ui_pos paddingLeft = 0;
		sl_ui_pos paddingRight = 0;
		sl_ui_pos paddingTop = 0;
		sl_ui_pos paddingBottom = 0;
		Ref<PaddingAttributes>& paddingAttrs = m_paddingAttrs;
		if (paddingAttrs.isNotNull()) {
			if (!(paddingAttrs->flagPaddingLeftWeight)) {
				paddingLeft = paddingAttrs->paddingLeft;
			}
			if (!(paddingAttrs->flagPaddingTopWeight)) {
				paddingTop = paddingAttrs->paddingTop;
			}
			if (!(paddingAttrs->flagPaddingRightWeight)) {
				paddingRight = paddingAttrs->paddingRight;
			}
			if (!(paddingAttrs->flagPaddingBottomWeight)) {
				paddingBottom = paddingAttrs->paddingBottom;
			}
		}

		sl_ui_pos measuredWidth = paddingLeft + paddingRight;
		sl_ui_pos measuredHeight = paddingTop + paddingBottom;
		
		ListElements< Ref<View> > children(getChildren());
		
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child->getVisibility() != Visibility::Gone) {
				if (flagHorizontal) {
					Pair<sl_ui_len, sl_ui_len> insetsHorz;
					sl_ui_pos w = _measureLayoutWrappingSize_Horz(child.get(), insetsHorz, mapHorzInsets, paddingLeft, paddingRight);
					w += insetsHorz.first + insetsHorz.second;
					if (w > measuredWidth) {
						measuredWidth = w;
					}
				}
				if (flagVertical) {
					Pair<sl_ui_len, sl_ui_len> insetsVert;
					sl_ui_pos h = _measureLayoutWrappingSize_Vert(child.get(), insetsVert, mapVertInsets, paddingTop, paddingBottom);
					h += insetsVert.first + insetsVert.second;
					if (h > measuredHeight) {
						measuredHeight = h;
					}
				}
			}
		}
		if (flagHorizontal) {
			if (paddingAttrs.isNotNull()) {
				if (paddingAttrs->flagPaddingLeftWeight || paddingAttrs->flagPaddingRightWeight) {
					sl_real f = 1;
					if (paddingAttrs->flagPaddingLeftWeight) {
						f -= paddingAttrs->paddingLeftWeight;
					}
					if (paddingAttrs->flagPaddingRightWeight) {
						f -= paddingAttrs->paddingRightWeight;
					}
					if (f < 0.001f) {
						f = 0.001f;
					}
					measuredWidth = (sl_ui_len)(measuredWidth / f);
				}
			}
			ret.x = measuredWidth;
		}
		if (flagVertical) {
			if (paddingAttrs.isNotNull()) {
				if (paddingAttrs->flagPaddingTopWeight || paddingAttrs->flagPaddingBottomWeight) {
					sl_real f = 1;
					if (paddingAttrs->flagPaddingTopWeight) {
						f -= paddingAttrs->paddingTopWeight;
					}
					if (paddingAttrs->flagPaddingBottomWeight) {
						f -= paddingAttrs->paddingBottomWeight;
					}
					if (f < 0.001f) {
						f = 0.001f;
					}
					measuredHeight = (sl_ui_len)(measuredHeight / f);
				}
			}
			ret.y = measuredHeight;
		}
		return ret;
	}
	
	void View::measureAndSetLayoutWrappingSize(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (m_layoutAttrs.isNull()) {
			return;
		}		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		UISize size = measureLayoutWrappingSize(flagHorizontal, flagVertical);
		if (flagHorizontal) {
			setLayoutWidth(size.x);
		}
		if (flagVertical) {
			setLayoutHeight(size.y);
		}
	}

	sl_bool View::isCustomLayout()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagCustomLayout;
		}
		return sl_false;
	}
	
	void View::setCustomLayout(sl_bool flag)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagCustomLayout = flag;
		}
	}
	
	const UIRect& View::getRequestedFrame()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->requestedFrame;
		}
		return m_frame;
	}
	
	UISize View::getRequestedSize()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->requestedFrame.getSize();
		}
		return m_frame.getSize();
	}
	
	sl_ui_len View::getRequestedWidth()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->requestedFrame.getWidth();
		}
		return m_frame.getWidth();
	}
	
	sl_ui_len View::getRequestedHeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->requestedFrame.getHeight();
		}
		return m_frame.getHeight();
	}
	
	const UIRect& View::getLayoutFrame()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->layoutFrame;
		}
		return m_frame;
	}
	
	void View::setLayoutFrame(const UIRect& rect)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (rect.isAlmostEqual(attrs->layoutFrame)) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			attrs->layoutFrame = rect;
		} else {
			if (rect.isAlmostEqual(m_frame)) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			setFrame(rect, UIUpdateMode::None);
		}
	}
	
	UISize View::getLayoutSize()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->layoutFrame.getSize();
		}
		return m_frame.getSize();
	}
	
	void View::setLayoutSize(sl_ui_len width, sl_ui_len height)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (Math::isAlmostZero(width - attrs->layoutFrame.getWidth()) && Math::isAlmostZero(height - attrs->layoutFrame.getHeight())) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			attrs->layoutFrame.setSize(width, height);
		} else {
			if (Math::isAlmostZero(width - m_frame.getWidth()) && Math::isAlmostZero(height - m_frame.getHeight())) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			setSize(width, height, UIUpdateMode::None);
		}
	}
	
	void View::setLayoutSize(const UISize& size)
	{
		setLayoutSize(size.x, size.y);
	}
	
	sl_ui_len View::getLayoutWidth()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->layoutFrame.getWidth();
		}
		return m_frame.getWidth();
	}
	
	void View::setLayoutWidth(sl_ui_len width)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (Math::isAlmostZero(width - attrs->layoutFrame.getWidth())) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			attrs->layoutFrame.setWidth(width);
		} else {
			if (Math::isAlmostZero(width - m_frame.getWidth())) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			setWidth(width, UIUpdateMode::None);
		}
	}
	
	sl_ui_len View::getLayoutHeight()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->layoutFrame.getHeight();
		}
		return m_frame.getHeight();
	}
	
	void View::setLayoutHeight(sl_ui_len height)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			if (Math::isAlmostZero(height - attrs->layoutFrame.getHeight())) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			attrs->layoutFrame.setHeight(height);
		} else {
			if (Math::isAlmostZero(height - m_frame.getHeight())) {
				return;
			}
			m_flagNeedApplyLayout = sl_true;
			setHeight(height, UIUpdateMode::None);
		}
	}

	void View::invalidateLayout(UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		if (!(SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode))) {
			_setInvalidateLayout();
			invalidate(mode);
			return;
		}
		Ref<View> view = this;
		for (;;) {
			view->_setInvalidateLayout();
			Ref<LayoutAttributes>& layoutAttrs = view->m_layoutAttrs;
			if (layoutAttrs.isNull()) {
				break;
			}
			if (!(layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping)) {
				break;
			}
			Ref<View> parent = view->m_parent;
			if (parent.isNotNull()) {
				view = parent;
			} else {
				break;
			}
		}
		view->dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(View, _updateAndApplyLayout, view.get()));
		if (view != this) {
			void (View::*f)(UIUpdateMode) = &View::invalidate;
			view->dispatchToDrawingThread(Function<void()>::bindWeakRef(WeakRef<View>(this), f, mode));
		}
	}

	void View::invalidateParentLayout(UIUpdateMode mode)
	{
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->invalidateLayout(mode);
		}
	}
	
	void View::invalidateSelfAndParentLayout(UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		_setInvalidateLayout();
		Ref<View> parent = m_parent;
		if (!(SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode))) {
			if (parent.isNotNull()) {
				parent->_setInvalidateLayout();
			}
			invalidate(mode);
			return;
		}
		Ref<View> view = this;
		while (parent.isNotNull()) {
			view = parent;
			view->_setInvalidateLayout();
			Ref<LayoutAttributes>& layoutAttrs = view->m_layoutAttrs;
			if (layoutAttrs.isNull()) {
				break;
			}
			if (!(layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping)) {
				break;
			}
			parent = view->m_parent;
		}
		view->dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(View, _updateAndApplyLayout, view.get()));
	}
	
	void View::invalidateLayoutOfWrappingControl(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			if (layoutAttrs->widthMode == SizeMode::Wrapping || layoutAttrs->heightMode == SizeMode::Wrapping) {
				invalidateLayout(mode);
				return;
			}
		}
		invalidate(mode);
	}
	
	void View::forceUpdateLayout()
	{
		Ref<LayoutAttributes>& layoutAttrs = m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			if (layoutAttrs->flagRequestedFrame) {
				layoutAttrs->layoutFrame = layoutAttrs->requestedFrame;
				layoutAttrs->flagRequestedFrame = sl_false;
			}
		}
		_setInvalidateLayout();
		_updateLayout();
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
			invalidateParentLayout(mode);
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
			invalidateParentLayout(mode);
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
			if (weight < 0) {
				weight = 0;
			}
			attrs->widthWeight = weight;
			if (attrs->leftMode == PositionMode::Free) {
				attrs->leftMode = PositionMode::ParentEdge;
			}
			if (attrs->rightMode == PositionMode::Free) {
				attrs->rightMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
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
			if (weight < 0) {
				weight = 0;
			}
			attrs->heightWeight = weight;
			if (attrs->topMode == PositionMode::Free) {
				attrs->topMode = PositionMode::ParentEdge;
			}
			if (attrs->bottomMode == PositionMode::Free) {
				attrs->bottomMode = PositionMode::ParentEdge;
			}
			invalidateParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			if (weight < 0) {
				weight = 0;
			}
			attrs->widthWeight = weight;
			invalidateParentLayout(mode);
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
			if (weight < 0) {
				weight = 0;
			}
			attrs->heightWeight = weight;
			invalidateParentLayout(mode);
		}
	}

	sl_bool View::isLeftFree()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->leftMode == PositionMode::Free;
		}
		return sl_true;
	}

	void View::setLeftFree(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->leftMode = PositionMode::Free;
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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

	sl_bool View::isRightFree()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->rightMode == PositionMode::Free;
		}
		return sl_true;
	}

	void View::setRightFree(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->rightMode = PositionMode::Free;
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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

	sl_bool View::isTopFree()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->topMode == PositionMode::Free;
		}
		return sl_true;
	}

	void View::setTopFree(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->topMode = PositionMode::Free;
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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

	sl_bool View::isBottomFree()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->bottomMode == PositionMode::Free;
		}
		return sl_true;
	}

	void View::setBottomFree(UIUpdateMode mode)
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::Free;
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
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
		if (view.isNull()) {
			return;
		}
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::OtherEnd;
			attrs->bottomReferingView = view;
			invalidateSelfAndParentLayout(mode);
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
		if (view.isNull()) {
			return;
		}
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->bottomMode = PositionMode::OtherStart;
			attrs->bottomReferingView = view;
			invalidateSelfAndParentLayout(mode);
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
		if (view.isNull()) {
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
		if (view.isNull()) {
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
			invalidateSelfAndParentLayout(mode);
		}
	}
	
	sl_bool View::isMaximumWidthDefined()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->maxWidth != DEFAULT_MAX_SIZE;
		}
		return sl_false;
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
			invalidateSelfAndParentLayout(mode);
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
			invalidateSelfAndParentLayout(mode);
		}
	}

	sl_bool View::isMaximumHeightDefined()
	{
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			return attrs->maxHeight != DEFAULT_MAX_SIZE;
		}
		return sl_false;
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
			invalidateSelfAndParentLayout(mode);
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
			if (aspectRatioMode == AspectRatioMode::AdjustWidth) {
				attrs->widthMode = SizeMode::Fixed;
			} else if (aspectRatioMode == AspectRatioMode::AdjustHeight) {
				attrs->heightMode = SizeMode::Fixed;
			}
			invalidateSelfAndParentLayout(updateMode);
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
			invalidateSelfAndParentLayout(mode);
		}
	}
	
#define VIEW_MARGIN_FUNCTIONS(NAME) \
	sl_ui_pos View::getMargin##NAME() \
	{ \
		Ref<LayoutAttributes>& attrs = m_layoutAttrs; \
		if (attrs.isNotNull()) { \
			return attrs->margin##NAME; \
		} \
		return 0; \
	} \
	void View::setMargin##NAME(sl_ui_pos margin, UIUpdateMode mode) \
	{ \
		_initializeLayoutAttributes(); \
		Ref<LayoutAttributes>& attrs = m_layoutAttrs; \
		if (attrs.isNotNull()) { \
			attrs->flagMargin##NAME##Weight = sl_false; \
			attrs->margin##NAME = margin; \
			invalidateSelfAndParentLayout(mode); \
		} \
	} \
	sl_bool View::isMargin##NAME##Fixed() \
	{ \
		Ref<LayoutAttributes>& attrs = m_layoutAttrs; \
		if (attrs.isNotNull()) { \
			return !(attrs->flagMargin##NAME##Weight); \
		} \
		return sl_true; \
	} \
	sl_real View::getMargin##NAME##Weight() \
	{ \
		Ref<LayoutAttributes>& attrs = m_layoutAttrs; \
		if (attrs.isNotNull()) { \
			return attrs->margin##NAME##Weight; \
		} \
		return 0; \
	} \
	void View::setMargin##NAME##Weight(sl_real weight, UIUpdateMode mode) \
	{ \
		_initializeLayoutAttributes(); \
		Ref<LayoutAttributes>& attrs = m_layoutAttrs; \
		if (attrs.isNotNull()) { \
			attrs->flagMargin##NAME##Weight = sl_true; \
			attrs->margin##NAME##Weight = weight; \
			invalidateSelfAndParentLayout(mode); \
		} \
	}

	VIEW_MARGIN_FUNCTIONS(Left)
	VIEW_MARGIN_FUNCTIONS(Top)
	VIEW_MARGIN_FUNCTIONS(Right)
	VIEW_MARGIN_FUNCTIONS(Bottom)

	void View::setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		_initializeLayoutAttributes();
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			attrs->flagMarginLeftWeight = sl_false;
			attrs->flagMarginTopWeight = sl_false;
			attrs->flagMarginRightWeight = sl_false;
			attrs->flagMarginBottomWeight = sl_false;
			attrs->marginLeft = left;
			attrs->marginTop = top;
			attrs->marginRight = right;
			attrs->marginBottom = bottom;
			invalidateSelfAndParentLayout(mode);
		}
	}

	void View::setMargin(sl_ui_pos margin, UIUpdateMode mode)
	{
		setMargin(margin, margin, margin, margin, mode);
	}
	
	UIEdgeInsets View::getMargin()
	{
		UIEdgeInsets ret;
		Ref<LayoutAttributes>& attrs = m_layoutAttrs;
		if (attrs.isNotNull()) {
			ret.left = attrs->marginLeft;
			ret.top = attrs->marginTop;
			ret.right = attrs->marginRight;
			ret.bottom = attrs->marginBottom;
		} else {
			ret.left = 0;
			ret.top = 0;
			ret.right = 0;
			ret.bottom = 0;
		}
		return ret;
	}
	
	void View::setMargin(const UIEdgeInsets& margin, UIUpdateMode mode)
	{
		setMargin(margin.left, margin.top, margin.right, margin.bottom, mode);
	}

#define VIEW_PADDING_FUNCTIONS(NAME, PARENT_LEN) \
	sl_ui_pos View::getPadding##NAME() \
	{ \
		Ref<PaddingAttributes>& attrs = m_paddingAttrs; \
		if (attrs.isNotNull()) { \
			return attrs->padding##NAME; \
		} \
		return 0; \
	} \
	void View::setPadding##NAME(sl_ui_pos padding, UIUpdateMode mode) \
	{ \
		_initializePaddingAttributes(); \
		Ref<PaddingAttributes>& attrs = m_paddingAttrs; \
		if (attrs.isNotNull()) { \
			attrs->flagPadding##NAME##Weight = sl_false; \
			attrs->padding##NAME = padding; \
			invalidateLayout(mode); \
			if (!SLIB_UI_UPDATE_MODE_IS_INIT(mode)) { \
				onChangePadding(); \
			} \
		} \
	} \
	sl_bool View::isPadding##NAME##Fixed() \
	{ \
		Ref<PaddingAttributes>& attrs = m_paddingAttrs; \
		if (attrs.isNotNull()) { \
			return !(attrs->flagPadding##NAME##Weight); \
		} \
		return sl_true; \
	} \
	sl_real View::getPadding##NAME##Weight() \
	{ \
		Ref<PaddingAttributes>& attrs = m_paddingAttrs; \
		if (attrs.isNotNull()) { \
			return attrs->padding##NAME##Weight; \
		} \
		return 0; \
	} \
	void View::setPadding##NAME##Weight(sl_real weight, UIUpdateMode mode) \
	{ \
		_initializePaddingAttributes(); \
		Ref<PaddingAttributes>& attrs = m_paddingAttrs; \
		if (attrs.isNotNull()) { \
			attrs->flagPadding##NAME##Weight = sl_true; \
			attrs->padding##NAME##Weight = weight; \
			attrs->padding##NAME = (sl_ui_pos)(PARENT_LEN * weight); \
			invalidateLayout(mode); \
			if (!SLIB_UI_UPDATE_MODE_IS_INIT(mode)) { \
				onChangePadding(); \
			} \
		} \
	}

	VIEW_PADDING_FUNCTIONS(Left, getWidth())
	VIEW_PADDING_FUNCTIONS(Top, getHeight())
	VIEW_PADDING_FUNCTIONS(Right, getWidth())
	VIEW_PADDING_FUNCTIONS(Bottom, getHeight())

	void View::_setInstancePadding()
	{
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::_setInstancePadding)
			instance->setPadding(this, getPadding());
		}
	}
	
	void View::setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		_initializePaddingAttributes();
		Ref<PaddingAttributes>& attrs = m_paddingAttrs;
		if (attrs.isNotNull()) {
			attrs->flagPaddingLeftWeight = sl_false;
			attrs->paddingLeft = left;
			attrs->paddingTop = top;
			attrs->paddingRight = right;
			attrs->paddingBottom = bottom;
			invalidateLayout(mode);
			if (!SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
				onChangePadding();
			}
		}
	}

	void View::setPadding(sl_ui_pos padding, UIUpdateMode mode)
	{
		setPadding(padding, padding, padding, padding, mode);
	}

	UIEdgeInsets View::getPadding()
	{
		UIEdgeInsets ret;
		Ref<PaddingAttributes>& attrs = m_paddingAttrs;
		if (attrs.isNotNull()) {
			ret.left = attrs->paddingLeft;
			ret.top = attrs->paddingTop;
			ret.right = attrs->paddingRight;
			ret.bottom = attrs->paddingBottom;
		} else {
			ret.left = 0;
			ret.top = 0;
			ret.right = 0;
			ret.bottom = 0;
		}
		return ret;
	}
	
	void View::setPadding(const UIEdgeInsets& padding, UIUpdateMode mode)
	{
		setPadding(padding.left, padding.top, padding.right, padding.bottom, mode);
	}
	
	sl_bool View::isUsingChildLayouts()
	{
		return m_flagUsingChildLayouts;
	}
	
	void View::setUsingChildLayouts(sl_bool flag)
	{
		m_flagUsingChildLayouts = flag;
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
						if (flagScale) {
							if (flagInited) {
								Transform2::scale(mat, sx, sy);
							} else {
								mat = Transform2::getScalingMatrix(sx, sy);
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

	Matrix3 View::getFinalTransformInInstance()
	{
		Matrix3 ret;
		if (!(getFinalTransform(&ret))) {
			ret = Matrix3::identity();
		}
		Ref<View> parent = m_parent;
		while (parent.isNotNull()) {
			if (parent->isInstance() || parent->m_flagCurrentCreatingInstance) {
				break;
			}
			Matrix3 t;
			if (parent->getFinalTransform(&t)) {
				ret = t * ret;
			}
			parent = parent->m_parent;
		}
		return ret;
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

	void View::_updateInstanceTransforms()
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::_updateInstanceTransforms)
			instance->setTransform(this, getFinalTransformInInstance());
		} else {
			Ref<ChildAttributes>& attrs = m_childAttrs;
			if (attrs.isNotNull() && attrs->flagHasInstances) {
				ListElements< Ref<View> > children(getChildren());
				for (sl_size i = 0; i < children.count; i++) {
					children[i]->_updateInstanceTransforms();
				}
			}
		}
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
			_updateInstanceTransforms();
			updateAndInvalidateBoundsInParent(mode);
		}
	}

	UIPointf View::convertCoordinateFromScreen(const UIPointf& ptScreen)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromScreenToView(this, ptScreen);
		}
		UIPointf pt;
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			pt = parent->convertCoordinateFromScreen(ptScreen);
		} else {
			pt = ptScreen;
		}
		return convertCoordinateFromParent(pt);
	}

	UIPointf View::convertCoordinateToScreen(const UIPointf& ptView)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromViewToScreen(this, ptView);
		}
		UIPointf pt = convertCoordinateToParent(ptView);
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			return parent->convertCoordinateToScreen(pt);
		} else {
			return pt;
		}
	}

	UIPointf View::convertCoordinateFromParent(const UIPointf& ptParent)
	{
		if (m_instance.isNotNull() && m_parent.isNotNull()) {
			Ref<ViewInstance> instance = m_instance;
			Ref<View> parent = m_parent;
			if (instance.isNotNull() && parent.isNotNull() && parent->m_instance.isNotNull()) {
				Ref<ViewInstance> instanceParent = parent->m_instance;
				if (instanceParent.isNotNull()) {
					UIPointf pt = instanceParent->convertCoordinateFromViewToScreen(parent.get(), ptParent);
					return instance->convertCoordinateFromScreenToView(this, pt);
				}
			}
		}
		
		sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
		sl_ui_posf offy = (sl_ui_posf)(m_frame.top);
		
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
		if (m_instance.isNotNull() && m_parent.isNotNull()) {
			Ref<ViewInstance> instance = m_instance;
			Ref<View> parent = m_parent;
			if (instance.isNotNull() && parent.isNotNull() && parent->m_instance.isNotNull()) {
				Ref<ViewInstance> instanceParent = parent->m_instance;
				if (instanceParent.isNotNull()) {
					if (getFinalTransform(sl_null)) {
						UIPointf pts[4];
						rcParent.getCornerPoints(pts);
						for (int i = 0; i < 4; i++) {
							UIPointf pt = instanceParent->convertCoordinateFromViewToScreen(parent.get(), pts[i]);
							pts[i] = instance->convertCoordinateFromScreenToView(this, pt);
						}
						UIRectf rc;
						rc.setFromPoints(pts, 4);
						return rc;
					} else {
						UIPointf pt = instanceParent->convertCoordinateFromViewToScreen(parent.get(), rcParent.getLocation());
						pt = instance->convertCoordinateFromScreenToView(this, pt);
						UIRectf rc;
						rc.left = pt.x;
						rc.top = pt.y;
						rc.right = pt.x + rcParent.getWidth();
						rc.bottom = pt.y + rcParent.getHeight();
						return rc;
					}
				}
			}
		}
		
		sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
		sl_ui_posf offy = (sl_ui_posf)(m_frame.top);
		
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
		if (m_instance.isNotNull() && m_parent.isNotNull()) {
			Ref<ViewInstance> instance = m_instance;
			Ref<View> parent = m_parent;
			if (instance.isNotNull() && parent.isNotNull() && parent->m_instance.isNotNull()) {
				Ref<ViewInstance> instanceParent = parent->m_instance;
				if (instanceParent.isNotNull()) {
					UIPointf pt = instance->convertCoordinateFromViewToScreen(this, ptView);
					return instanceParent->convertCoordinateFromScreenToView(parent.get(), pt);
				}
			}
		}
		
		sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
		sl_ui_posf offy = (sl_ui_posf)(m_frame.top);

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
		if (m_instance.isNotNull() && m_parent.isNotNull()) {
			Ref<ViewInstance> instance = m_instance;
			Ref<View> parent = m_parent;
			if (instance.isNotNull() && parent.isNotNull() && parent->m_instance.isNotNull()) {
				Ref<ViewInstance> instanceParent = parent->m_instance;
				if (instanceParent.isNotNull()) {
					if (getFinalTransform(sl_null)) {
						UIPointf pts[4];
						rcView.getCornerPoints(pts);
						for (int i = 0; i < 4; i++) {
							UIPointf pt = instance->convertCoordinateFromViewToScreen(this, pts[i]);
							pts[i] = instanceParent->convertCoordinateFromScreenToView(parent.get(), pt);
						}
						UIRectf rc;
						rc.setFromPoints(pts, 4);
						return rc;
					} else {
						UIPointf pt = instance->convertCoordinateFromViewToScreen(this, rcView.getLocation());
						pt = instanceParent->convertCoordinateFromScreenToView(parent.get(), pt);
						UIRectf rc;
						rc.left = pt.x;
						rc.top = pt.y;
						rc.right = pt.x + rcView.getWidth();
						rc.bottom = pt.y + rcView.getHeight();
						return rc;
					}
				}
			}
		}
		
		sl_ui_posf offx = (sl_ui_posf)(m_frame.left);
		sl_ui_posf offy = (sl_ui_posf)(m_frame.top);

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
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::setBackground, drawable, mode)
		}
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->background = drawable;
			if (instance.isNotNull()) {
				Color color;
				if (ColorDrawable::check(drawable, &color)) {
					instance->setBackgroundColor(this, color);
				}
			} else {
				invalidate(mode);
			}
		}
	}
	
	Color View::getBackgroundColor()
	{
		Color color;
		if (ColorDrawable::check(getBackground(), &color)) {
			return color;
		}
		return Color::zero();
	}
	
	void View::setBackgroundColor(const Color& color, UIUpdateMode mode)
	{
		setBackground(ColorDrawable::createColorDrawable(color), mode);
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
			invalidate(mode);
		}
	}
	
	Color View::getPressedBackgroundColor()
	{
		Color color;
		if (ColorDrawable::check(getPressedBackground(), &color)) {
			return color;
		}
		return Color::zero();
	}
	
	void View::setPressedBackgroundColor(const Color& color, UIUpdateMode mode)
	{
		setPressedBackground(ColorDrawable::createColorDrawable(color), mode);
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
			invalidate(mode);
		}
	}
	
	Color View::getHoverBackgroundColor()
	{
		Color color;
		if (ColorDrawable::check(getPressedBackground(), &color)) {
			return color;
		}
		return Color::zero();
	}
	
	void View::setHoverBackgroundColor(const Color& color, UIUpdateMode mode)
	{
		setHoverBackground(ColorDrawable::createColorDrawable(color), mode);
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
			invalidate(updateMode);
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

	void View::setBackgroundAlignment(const Alignment& align, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->backgroundAlignment = align;
			invalidate(mode);
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
			invalidate(mode);
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
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			void (View::*func)(sl_bool, UIUpdateMode) = &View::setBorder;
			SLIB_VIEW_RUN_ON_UI_THREAD(func, flagBorder, mode)
		}
		if (flagBorder) {
			if (isBorder()) {
				return;
			}
			setBorder(Pen::getDefault(), UIUpdateMode::None);
		} else {
			if (isBorder()) {
				setBorder(Ref<Pen>::null(), UIUpdateMode::None);
			}
		}
		if (instance.isNotNull()) {
			instance->setBorder(this, flagBorder);
		} else {
			invalidate(mode);
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
			invalidate(mode);
		}
	}

	const Size& View::getBoundRadius()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->boundRadius;
		}
		return Size::zero();
	}

	void View::setBoundRadius(const Size& radius, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->boundRadius = radius;
			if (radius.x > SLIB_EPSILON && radius.y > SLIB_EPSILON) {
				attrs->boundShape = BoundShape::RoundRect;
			} else {
				attrs->boundShape = BoundShape::Rectangle;
			}
			invalidate(mode);
		}
	}

	void View::setBoundRadius(sl_real rx, sl_real ry, UIUpdateMode mode)
	{
		setBoundRadius(Size(rx, ry), mode);
	}

	void View::setBoundRadiusX(sl_real rx, UIUpdateMode mode)
	{
		Size size = getBoundRadius();
		size.x = rx;
		setBoundRadius(size, mode);
	}

	void View::setBoundRadiusY(sl_real ry, UIUpdateMode mode)
	{
		Size size = getBoundRadius();
		size.y = ry;
		setBoundRadius(size, mode);
	}

	void View::setBoundRadius(sl_real radius, UIUpdateMode mode)
	{
		setBoundRadius(Size(radius, radius), mode);
	}

	Ref<GraphicsPath> View::getBoundPath()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->boundPath;
		}
		return sl_null;
	}

	void View::setBoundPath(const Ref<GraphicsPath>& path, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->boundPath = path;
			if (path.isNotNull()) {
				attrs->boundShape = BoundShape::Path;
			}
			invalidate(mode);
		}
	}

	BoundShape View::getContentShape()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->contentShape;
		}
		return BoundShape::None;
	}
	
	void View::setContentShape(BoundShape shape, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->contentShape = shape;
			invalidate(mode);
		}
	}
	
	const Size& View::getContentRadius()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->contentRadius;
		}
		return Size::zero();
	}
	
	void View::setContentRadius(const Size& radius, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->contentRadius = radius;
			if (radius.x > SLIB_EPSILON && radius.y > SLIB_EPSILON) {
				attrs->contentShape = BoundShape::RoundRect;
			} else {
				attrs->contentShape = BoundShape::Rectangle;
			}
			invalidate(mode);
		}
	}
	
	void View::setContentRadius(sl_real rx, sl_real ry, UIUpdateMode mode)
	{
		setContentRadius(Size(rx, ry), mode);
	}
	
	void View::setContentRadiusX(sl_real rx, UIUpdateMode mode)
	{
		Size size = getContentRadius();
		size.x = rx;
		setContentRadius(size, mode);
	}
	
	void View::setContentRadiusY(sl_real ry, UIUpdateMode mode)
	{
		Size size = getContentRadius();
		size.y = ry;
		setContentRadius(size, mode);
	}
	
	void View::setContentRadius(sl_real radius, UIUpdateMode mode)
	{
		setContentRadius(Size(radius, radius), mode);
	}
	
	Ref<GraphicsPath> View::getContentBoundPath()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->contentBoundPath;
		}
		return sl_null;
	}
	
	void View::setContentBoundPath(const Ref<GraphicsPath>& path, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->contentBoundPath = path;
			if (path.isNotNull()) {
				attrs->contentShape = BoundShape::Path;
			}
			invalidate(mode);
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

	void View::_setFontInvalidateChildren()
	{
		ListElements< Ref<View> > children(getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			Ref<DrawAttributes>& childAttrs = child->m_drawAttrs;
			if (childAttrs.isNull() || childAttrs->font.isNull()) {
				if (child->isUsingFont()) {
					child->_setInstanceFont();
				} else {
					child->_setFontInvalidateChildren();
				}
			}
		}
	}
	
	void View::_setInstanceFont()
	{
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::_setInstanceFont)
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				instance->setFont(this, font);
			}
		}
		_setFontInvalidateChildren();
		invalidateLayoutOfWrappingControl();
	}

	void View::setFont(const Ref<Font>& font, UIUpdateMode mode)
	{
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			Ref<ViewInstance> instance = getNativeWidget();
			if (instance.isNotNull()) {
				SLIB_VIEW_RUN_ON_UI_THREAD(&View::setFont, font, mode)
				attrs->font = font;
				Ref<Font> _font = getFont();
				if (_font.isNotNull()) {
					instance->setFont(this, _font);
				}
			} else {
				attrs->font = font;
			}
			if (SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
				_setFontInvalidateChildren();
				if (isUsingFont()) {
					invalidateLayoutOfWrappingControl();
				}
			} else {
				if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
					if (isUsingFont()) {
						invalidate();
					}
				}
			}
		}
	}

	sl_real View::getFontSize()
	{
		Ref<Font> font = getFont();
		if (font.isNull()) {
			return UI::getDefaultFontSize();
		} else {
			return font->getSize();
		}
	}
	
	void View::setFontSize(sl_real size, UIUpdateMode mode)
	{
		Ref<Font> font = getFont();
		if (font.isNull()) {
			setFont(Font::create(UI::getDefaultFontFamily(), size), mode);
		} else {
			setFont(Font::create(font->getFamilyName(), size), mode);
		}
	}
	
	String View::getFontFamily()
	{
		
		Ref<Font> font = getFont();
		if (font.isNull()) {
			return UI::getDefaultFontFamily();
		} else {
			return font->getFamilyName();
		}
	}
	
	void View::setFontFamily(const String& fontFamily, UIUpdateMode mode)
	{
		Ref<Font> font = getFont();
		if (font.isNull()) {
			setFont(Font::create(fontFamily, UI::getDefaultFontSize()), mode);
		} else {
			setFont(Font::create(fontFamily, font->getSize()), mode);
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
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				SLIB_VIEW_RUN_ON_UI_THREAD(&View::setOpaque, flag, mode)
				attrs->flagOpaque = flag;
				instance->setOpaque(this, flag);
			} else {
				attrs->flagOpaque = flag;
				invalidateBoundsInParent(mode);
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
			Ref<ViewInstance> instance = m_instance;
			if (instance.isNotNull()) {
				SLIB_VIEW_RUN_ON_UI_THREAD(&View::setAlpha, alpha, mode)
				attrs->alpha = alpha;
				instance->setAlpha(this, alpha);
			} else {
				attrs->alpha = alpha;
				invalidateBoundsInParent(mode);
			}
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
			invalidate(mode);
		}
	}

	void View::invalidateLayer()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull() && attrs->flagLayer) {
			attrs->flagInvalidatedLayer = sl_true;
			attrs->flagInvalidatedWholeLayer = sl_true;
		}
	}

	void View::invalidateLayer(const UIRect& rect)
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull() && attrs->flagLayer) {
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
	
	sl_bool View::isForcedDraw()
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagForcedDraw;
		}
		return sl_false;
	}
	
	void View::forceDraw(sl_bool flagInvalidate)
	{
		if (m_instance.isNotNull()) {
			if (flagInvalidate) {
				invalidate();
			}
			return;
		}
		_initializeDrawAttributes();
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			attrs->flagForcedDraw = sl_true;
		}
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->forceDraw(sl_false);
		}
		if (flagInvalidate) {
			invalidate();
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
	
	Ref<Animation> View::startAnimation(const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<AnimationLoop> loop = getAnimationLoop();
		if (loop.isNotNull()) {
			return Animation::startWithLoop(loop, target, duration, onStop, curve, flags);
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
	
	Ref<Animation> View::startTransformAnimation(const AnimationFrames<Matrix3>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(AnimationFrames<Matrix3>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(AnimationFrames<Matrix3>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createTransformAnimationTo(const Matrix3& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(AnimationFrames<Matrix3>(getTransform(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startTransformAnimationTo(const Matrix3& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTransformAnimation(AnimationFrames<Matrix3>(getTransform(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
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
	
	Ref<Animation> View::startTranslateAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(AnimationFrames<Vector2>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(AnimationFrames<Vector2>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createTranslateAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(AnimationFrames<Vector2>(getTranslation(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startTranslateAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createTranslateAnimation(AnimationFrames<Vector2>(getTranslation(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
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
	
	Ref<Animation> View::startScaleAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createScaleAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(getScale(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startScaleAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(getScale(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createScaleAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(Vector2(startValue, startValue), Vector2(endValue, endValue)), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startScaleAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(Vector2(startValue, startValue), Vector2(endValue, endValue)), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createScaleAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(getScale(), Vector2(toValue, toValue)), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startScaleAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createScaleAnimation(AnimationFrames<Vector2>(getScale(), Vector2(toValue, toValue)), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
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
	
	Ref<Animation> View::startRotateAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createRotateAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(AnimationFrames<sl_real>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startRotateAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(AnimationFrames<sl_real>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createRotateAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(AnimationFrames<sl_real>(getRotation(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startRotateAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createRotateAnimation(AnimationFrames<sl_real>(getRotation(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
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
	
	Ref<Animation> View::startFrameAnimation(const AnimationFrames<Rectangle>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createFrameAnimation(const Rectangle& startValue, const Rectangle& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(AnimationFrames<Rectangle>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startFrameAnimation(const Rectangle& startValue, const Rectangle& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(AnimationFrames<Rectangle>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createFrameAnimationTo(const Rectangle& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(AnimationFrames<Rectangle>(getFrame(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startFrameAnimationTo(const Rectangle& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createFrameAnimation(AnimationFrames<Rectangle>(getFrame(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
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
	
	Ref<Animation> View::startAlphaAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createAlphaAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(AnimationFrames<sl_real>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startAlphaAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(AnimationFrames<sl_real>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createAlphaAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(AnimationFrames<sl_real>(getAlpha(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startAlphaAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createAlphaAnimation(AnimationFrames<sl_real>(getAlpha(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
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
	
	Ref<Animation> View::startBackgroundColorAnimation(const AnimationFrames<Color4f>& frames, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(frames, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> View::createBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(AnimationFrames<Color4f>(startValue, endValue), duration, onStop, curve, flags);
	}
	
	Ref<Animation> View::startBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(AnimationFrames<Color4f>(startValue, endValue), duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<Animation> View::createBackgroundColorAnimationTo(const Color4f& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(AnimationFrames<Color4f>(getBackgroundColor(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart);
	}
	
	Ref<Animation> View::startBackgroundColorAnimationTo(const Color4f& toValue, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createBackgroundColorAnimation(AnimationFrames<Color4f>(getBackgroundColor(), toValue), duration, onStop, curve, flags | AnimationFlags::NotUpdateWhenStart | AnimationFlags::AutoStart);
	}
	
	void View::_attachNativeAnimations()
	{
		Ref<TransformAttributes>& attrs = m_transformAttrs;
		if (attrs.isNotNull()) {
			_attachNativeAnimation(attrs->m_animationTransform);
			_attachNativeAnimation(attrs->m_animationTranslate);
			_attachNativeAnimation(attrs->m_animationScale);
			_attachNativeAnimation(attrs->m_animationRotate);
			_attachNativeAnimation(attrs->m_animationFrame);
			_attachNativeAnimation(attrs->m_animationAlpha);
			_attachNativeAnimation(attrs->m_animationBackgroundColor);
		}
	}
	
	void View::_attachNativeAnimation(const Ref<Animation>& animation)
	{
		if (animation.isNotNull()) {
			if (animation->isNativeEnabled() && animation->isRepeatForever()) {
				animation->stop();
				animation->start();
			}
		}
	}

	sl_bool View::isHorizontalScrolling()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagHorz;
		}
		return sl_false;
	}

	sl_bool View::isVerticalScrolling()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagVert;
		}
		return sl_false;
	}

	void View::setHorizontalScrolling(sl_bool flagHorizontal, UIUpdateMode mode)
	{
		setScrolling(flagHorizontal, isVerticalScrolling(), mode);
	}

	void View::setVerticalScrolling(sl_bool flagVertical, UIUpdateMode mode)
	{
		setScrolling(isHorizontalScrolling(), flagVertical, mode);
	}
	
	void View::setScrolling(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagHorz = flagHorizontal;
			attrs->flagVert = flagVertical;
			setScrollBarsVisible(attrs->flagHorzScrollBarVisible, attrs->flagVertScrollBarVisible, mode);
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
			removeChild(attrs->horz, UIUpdateMode::None);
			attrs->horz = bar;
			refreshScroll(mode);
		}
	}

	void View::setVerticalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode)
	{
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			removeChild(attrs->vert, UIUpdateMode::None);
			attrs->vert = bar;
			refreshScroll(mode);
		}
	}

	sl_bool View::isHorizontalScrollBarVisible()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagHorz && attrs->flagHorzScrollBarVisible;
		}
		return sl_false;
	}

	sl_bool View::isVerticalScrollBarVisible()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagVert && attrs->flagVertScrollBarVisible;
		}
		return sl_false;
	}

	void View::setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::setScrollBarsVisible, flagHorizontal, flagVertical, mode)
		}
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			attrs->flagHorzScrollBarVisible = flagHorizontal;
			attrs->flagVertScrollBarVisible = flagVertical;
			flagHorizontal = flagHorizontal && attrs->flagHorz;
			flagVertical = flagVertical && attrs->flagVert;
		}
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		if (flagHorizontal || flagVertical) {
			_initScrollBars(UIUpdateMode::None);
		}
		Ref<ScrollBar> bar;
		bar = getHorizontalScrollBar();
		if (bar.isNotNull()) {
			bar->setVisible(flagHorizontal, UIUpdateMode::None);
		}
		bar = getVerticalScrollBar();
		if (bar.isNotNull()) {
			bar->setVisible(flagVertical, UIUpdateMode::None);
		}
		refreshScroll(mode);
		if (instance.isNotNull()) {
			instance->setScrollBarsVisible(this, flagHorizontal, flagVertical);
		}
	}

	void View::setHorizontalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode)
	{
		sl_bool flagVert = sl_true;
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			flagVert = attrs->flagVertScrollBarVisible;
		}
		setScrollBarsVisible(flagVisible, flagVert, mode);
	}
	
	void View::setVerticalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode)
	{
		sl_bool flagHorz = sl_true;
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			flagHorz = attrs->flagHorzScrollBarVisible;
		}
		setScrollBarsVisible(flagHorz, flagVisible, mode);
	}
	
	ScrollPoint View::getScrollPosition()
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			return ScrollPoint(attrs->x, attrs->y);
		}
		return ScrollPoint::zero();
	}

	sl_scroll_pos View::getScrollX()
	{
		return getScrollPosition().x;
	}
	
	sl_scroll_pos View::getScrollY()
	{
		return getScrollPosition().y;
	}

	namespace priv
	{
		namespace view
		{
			SLIB_INLINE static sl_scroll_pos ClampScrollPos(sl_scroll_pos x, sl_scroll_pos max)
			{
				if (x > max) {
					x = max;
				}
				if (x < 0) {
					x = 0;
				}
				return x;
			}			
		}
	}

	void View::scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			void (View::*func)(sl_scroll_pos, sl_scroll_pos, UIUpdateMode) = &View::scrollTo;
			SLIB_VIEW_RUN_ON_UI_THREAD(func, x, y, mode)
		}
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			x = priv::view::ClampScrollPos(x, attrs->contentWidth - (sl_scroll_pos)(getWidth()));
			y = priv::view::ClampScrollPos(y, attrs->contentHeight - (sl_scroll_pos)(getHeight()));
			if (_scrollTo(x, y, sl_true, sl_true, sl_false)) {
				invalidate(mode);
			}
			if (instance.isNotNull()) {
				instance->scrollTo(this, x, y, sl_false);
			}
		}
	}

	void View::scrollTo(const ScrollPoint& position, UIUpdateMode mode)
	{
		scrollTo(position.x, position.y, mode);
	}

	void View::scrollToX(sl_scroll_pos x, UIUpdateMode mode)
	{
		scrollTo(x, getScrollY(), mode);
	}
	
	void View::scrollToY(sl_scroll_pos y, UIUpdateMode mode)
	{
		scrollTo(getScrollX(), y, mode);
	}

	void View::smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
	{
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			void (View::*func)(sl_scroll_pos, sl_scroll_pos, UIUpdateMode) = &View::smoothScrollTo;
			SLIB_VIEW_RUN_ON_UI_THREAD(func, x, y, mode)
		}
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			x = priv::view::ClampScrollPos(x, attrs->contentWidth - (sl_scroll_pos)(getWidth()));
			y = priv::view::ClampScrollPos(y, attrs->contentHeight - (sl_scroll_pos)(getHeight()));
			if (instance.isNotNull()) {
				instance->scrollTo(this, x, y, sl_true);
			} else {
				_startContentScrollingFlow(sl_true, Pointlf(x, y));
				invalidate(mode);
			}
		}
	}

	void View::smoothScrollTo(const ScrollPoint& position, UIUpdateMode mode)
	{
		smoothScrollTo(position.x, position.y, mode);
	}
	
	void View::smoothScrollToX(sl_scroll_pos x, UIUpdateMode mode)
	{
		smoothScrollTo(x, getScrollY(), mode);
	}

	void View::smoothScrollToY(sl_scroll_pos y, UIUpdateMode mode)
	{
		smoothScrollTo(getScrollX(), y, mode);
	}
	
	void View::scrollToEndX(UIUpdateMode mode)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			scrollToX(attrs->contentWidth - (sl_scroll_pos)(getWidth()), mode);
		}
	}
	
	void View::scrollToEndY(UIUpdateMode mode)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			scrollToY(attrs->contentHeight - (sl_scroll_pos)(getHeight()), mode);
		}
	}
	
	void View::smoothScrollToEndX(UIUpdateMode mode)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			smoothScrollToX(attrs->contentWidth - (sl_scroll_pos)(getWidth()), mode);
		}
	}
	
	void View::smoothScrollToEndY(UIUpdateMode mode)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			smoothScrollToY(attrs->contentHeight - (sl_scroll_pos)(getHeight()), mode);
		}
	}

	void View::setScrollX(sl_scroll_pos x, UIUpdateMode mode)
	{
		scrollToX(x, mode);
	}

	void View::setScrollY(sl_scroll_pos y, UIUpdateMode mode)
	{
		scrollToY(y, mode);
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
			_initScrollBars(UIUpdateMode::None);
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

	void View::_onScrollBarChangeValue(ScrollBar* scrollBar, sl_scroll_pos value)
	{
		sl_scroll_pos sx = 0;
		sl_scroll_pos sy = 0;
		Ref<ScrollBar> horz = getHorizontalScrollBar();
		if (horz.isNotNull()) {
			sx = horz->getValue();
		}
		Ref<ScrollBar> vert = getVerticalScrollBar();
		if (vert.isNotNull()) {
			sy = vert->getValue();
		}
		scrollTo(sx, sy);
	}

	void View::refreshScroll(UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			sl_ui_pos width = getWidth();
			sl_ui_pos height = getHeight();
			Ref<ScrollBar> barHorz = attrs->horz;
			if (barHorz.isNotNull()) {
				barHorz->setParent(this);
				barHorz->setMinimumValue(0, UIUpdateMode::None);
				barHorz->setMaximumValue(attrs->contentWidth, UIUpdateMode::None);
				barHorz->setPage(width, UIUpdateMode::None);
				barHorz->setValueOfOutRange(attrs->x, UIUpdateMode::None);
				barHorz->setFrame(UIRect(0, height - attrs->barWidth, width, height), UIUpdateMode::None);
				barHorz->setOnChange(SLIB_FUNCTION_WEAKREF(View, _onScrollBarChangeValue, this));
				attrs->flagValidHorz = barHorz->isValid();
			}
			Ref<ScrollBar> barVert = attrs->vert;
			if (barVert.isNotNull()) {
				barVert->setParent(this);
				barVert->setVertical(UIUpdateMode::None);
				barVert->setMinimumValue(0, UIUpdateMode::None);
				barVert->setMaximumValue(attrs->contentHeight, UIUpdateMode::None);
				barVert->setPage(height, UIUpdateMode::None);
				barVert->setValueOfOutRange(attrs->y, UIUpdateMode::None);
				barVert->setFrame(UIRect(width - attrs->barWidth, 0, width, height), UIUpdateMode::None);
				barVert->setOnChange(SLIB_FUNCTION_WEAKREF(View, _onScrollBarChangeValue, this));
				attrs->flagValidVert = barVert->isValid();
			}
			sl_scroll_pos x = priv::view::ClampScrollPos(attrs->x, attrs->contentWidth - (sl_scroll_pos)(getWidth()));
			sl_scroll_pos y = priv::view::ClampScrollPos(attrs->y, attrs->contentHeight - (sl_scroll_pos)(getHeight()));
			if (!(Math::isAlmostZero(x - attrs->x) && Math::isAlmostZero(y - attrs->y))) {
				if (_scrollTo(x, y, sl_true, sl_true, sl_false)) {
					invalidate(mode);
				}
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
			if (views[0].isNotNull() || views[1].isNotNull()) {
				_initializeChildAttributes();
			}
		}
	}

	void View::_initScrollBars(UIUpdateMode mode)
	{
		if (isNativeWidget()) {
			return;
		}
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNotNull()) {
			if (attrs->flagHorz && attrs->flagHorzScrollBarVisible) {
				if (!(attrs->flagInitHorzScrollBar)) {
					attrs->flagInitHorzScrollBar = sl_true;
					if (attrs->horz.isNull()) {
						setHorizontalScrollBar(new ScrollBar, mode);
					}
				}
			}
			if (attrs->flagVert && attrs->flagVertScrollBarVisible) {
				if (!(attrs->flagInitVertScrollBar)) {
					attrs->flagInitVertScrollBar = sl_true;
					if (attrs->vert.isNull()) {
						setVerticalScrollBar(new ScrollBar, mode);
					}
				}
			}
		}
	}

#define BOUNCE_WEIGHT 0

	sl_bool View::_scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagPreprocess, sl_bool flagFinish, sl_bool flagAnimate)
	{
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNull()) {
			return sl_false;
		}
		
		sl_real width = (sl_real)(getWidth());;
		sl_real height = (sl_real)(getHeight());

		sl_bool flagFinishX = flagFinish;
		sl_bool flagFinishY = flagFinish;
		
		if (flagPreprocess) {
			sl_scroll_pos comp;
			if (attrs->flagHorz) {
				sl_real w = width;
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
			if (attrs->flagVert) {
				sl_real h = height;
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
				bar->setValueOfOutRange(x, UIUpdateMode::None);
			}
			bar = attrs->vert;
			if (bar.isNotNull()) {
				bar->setValueOfOutRange(y, UIUpdateMode::None);
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
					if (attrs->contentWidth > width) {
						if (x > attrs->contentWidth - width) {
							x = attrs->contentWidth - width;
							flagTarget = sl_true;
						}
					}
				}
				if (attrs->flagVert) {
					if (y < 0) {
						y = 0;
						flagTarget = sl_true;
					}
					if (attrs->contentHeight > height) {
						if (y > attrs->contentHeight - height) {
							y = attrs->contentHeight - height;
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
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagTouchMultipleChildren;
		}
		return sl_false;
	}

	void View::setTouchMultipleChildren(sl_bool flag)
	{
		_initializeChildAttributes();
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			attrs->flagTouchMultipleChildren = flag;
		}
	}

	sl_bool View::isPassingEventsToChildren()
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			return attrs->flagPassEventToChildren;
		}
		return sl_false;
	}

	void View::setPassingEventsToChildren(sl_bool flag)
	{
		_initializeChildAttributes();
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			attrs->flagPassEventToChildren = flag;
		}
	}

	sl_bool View::isOkCancelEnabled()
	{
		return m_flagOkCancelEnabled;
	}

	void View::setOkCancelEnabled(sl_bool flag)
	{
		m_flagOkCancelEnabled = flag;
	}

	void View::setOkOnClick()
	{
		setOnClick([](View* view) {
			view->dispatchOK();
		});
	}
	
	void View::setCancelOnClick()
	{
		setOnClick([](View* view) {
			view->dispatchCancel();
		});
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

	sl_bool View::isTabStopEnabled()
	{
		return m_flagTabStopEnabled;
	}

	void View::setTabStopEnabled(sl_bool flag)
	{
		m_flagTabStopEnabled = flag;
	}

	Ref<View> View::getNextTabStop()
	{
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			Ref<View> view = attrs->viewNextTabStop;
			if (view.isNotNull()) {
				return view;
			}
		}
		return getNextFocusableView();
	}

	void View::setNextTabStop(const Ref<View>& view)
	{
		_initializeOtherAttributes();
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			attrs->viewNextTabStop = view;
		}
		if (view.isNotNull()) {
			if (view->getPreviousTabStop().isNull()) {
				view->setPreviousTabStop(this);
			}
		}
	}

	Ref<View> View::getPreviousTabStop()
	{
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			Ref<View> view = attrs->viewPrevTabStop;
			if (view.isNotNull()) {
				return view;
			}
		}
		return getPreviousFocusableView();
	}

	void View::setPreviousTabStop(const Ref<View>& view)
	{
		_initializeOtherAttributes();
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			attrs->viewPrevTabStop = view;
		}
		if (view.isNotNull()) {
			if (view->getNextTabStop().isNull()) {
				view->setNextTabStop(this);
			}
		}
	}
	
	sl_bool View::isKeepKeyboard()
	{
		return m_flagKeepKeyboard;
	}
	
	void View::setKeepKeyboard(sl_bool flag)
	{
		m_flagKeepKeyboard = flag;
	}

	Function<sl_bool(const UIPoint& pt)> View::getCapturingChildInstanceEvents()
	{
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			return attrs->hitTestCapturingChildInstanceEvents;
		}
		return sl_null;
	}

	void View::setCapturingChildInstanceEvents(const Function<sl_bool(const UIPoint& pt)>& hitTestCaturing)
	{
		_initializeChildAttributes();
		Ref<ChildAttributes>& attrs = m_childAttrs;
		if (attrs.isNotNull()) {
			attrs->hitTestCapturingChildInstanceEvents = hitTestCaturing;
		}
	}
	
	Ref<UIEvent> View::getCurrentEvent()
	{
		return m_currentEvent;
	}
	
	void View::setCurrentEvent(UIEvent* ev)
	{
		m_currentEvent = ev;
	}
	
	Ref<GestureDetector> View::createGestureDetector()
	{
		_initializeOtherAttributes();
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			Ref<GestureDetector> gesture = attrs->gestureDetector;
			if (gesture.isNull()) {
				gesture = new GestureDetector(this);
				attrs->gestureDetector = gesture;
			}
			return gesture;
		}
		return sl_null;
	}

	Ref<GestureDetector> View::getGestureDetector()
	{
		Ref<OtherAttributes>& attrs = m_otherAttrs;
		if (attrs.isNotNull()) {
			return attrs->gestureDetector;
		}
		return sl_null;
	}
	
	Ref<Drawable> View::getCurrentBackground()
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
			return background;
		}
		return sl_null;
	}

	void View::drawBackground(Canvas* canvas, const Ref<Drawable>& background)
	{
		if (background.isNotNull()) {
			Rectangle rc(0, 0, (sl_real)(m_frame.getWidth()), (sl_real)(m_frame.getHeight()));
			canvas->draw(rc, background, getBackgroundScaleMode(), getBackgroundAlignment());
		}
	}

	void View::drawBorder(Canvas* canvas, const Ref<Pen>& pen)
	{
		UIRect rc(0, 0, m_frame.getWidth(), m_frame.getHeight());
		if (pen.isNotNull()) {
			switch (getBoundShape()) {
				case BoundShape::Rectangle:
					{
						if (isInstance()) {
#ifndef SLIB_PLATFORM_IS_WIN32
							rc.right -= 1;
#endif
#ifdef SLIB_PLATFORM_IS_APPLE
							rc.top += 1;
#else
							rc.bottom -= 1;
#endif
						}
						rc.fixSizeError();
						sl_bool flagAntiAlias = canvas->isAntiAlias();
						canvas->setAntiAlias(sl_false);
						canvas->drawRectangle(rc, pen);
						canvas->setAntiAlias(flagAntiAlias);
						break;
					}
				case BoundShape::RoundRect:
					rc.left += 1;
					rc.top += 1;
					rc.right -= 2;
					rc.bottom -= 2;
					rc.fixSizeError();
					canvas->drawRoundRect(rc, getBoundRadius(), pen);
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
					canvas->drawPath(getBoundPath(), pen);
					break;
				default:
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
			RenderCanvasState savedState(*currentState);
			
			sl_bool flagTransformed = sl_false;
			
			for (sl_size i = 0; i < count; i++) {
				
				View* child = children[i].get();
			
				if (child && child->isVisible()) {
					
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
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated) || child->isForcedDraw()) {
							if (flagTransformed) {
								*currentState = savedState;
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
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated) || child->isForcedDraw()) {
							sl_real ax = (sl_real)(child->getWidth()) / 2;
							sl_real ay = (sl_real)(child->getHeight()) / 2;
							mat.m20 = -ax * mat.m00 - ay * mat.m10 + mat.m20 + ax + (sl_real)(offx);
							mat.m21 = -ax * mat.m01 - ay * mat.m11 + mat.m21 + ay + (sl_real)(offy);
							if (i != 0) {
								*currentState = savedState;
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
			
			*currentState = savedState;
			
		} else {
			
			for (sl_size i = 0; i < count; i++) {
				
				View* child = children[i].get();
			
				if (child && child->isVisible() && !(child->isInstance())) {
					
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
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated) || child->isForcedDraw()) {
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
						if (rcInvalidated.intersectRectangle(child->getBounds(), &rcInvalidated) || child->isForcedDraw()) {
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
		canvas->setAlpha(alphaParent);
		canvas->setInvalidatedRect(rcInvalidatedParent);
	}

	void View::drawContent(Canvas* canvas)
	{

		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
	
		if (m_flagSavingCanvasState || scrollAttrs.isNotNull() || getContentShape() != BoundShape::None) {
			CanvasStateScope scope(canvas);
			onDrawBackground(canvas);
			if (scrollAttrs.isNotNull()) {
				sl_real scrollX = (sl_real)(scrollAttrs->x);
				sl_real scrollY = (sl_real)(scrollAttrs->y);
				if(!(Math::isAlmostZero(scrollX)) || !(Math::isAlmostZero(scrollY))) {
					canvas->translate(-scrollX, -scrollY);
				}
			}
			clipContentBounds(canvas);
			SLIB_INVOKE_EVENT_HANDLER(Draw, canvas)
		} else {
			onDrawBackground(canvas);
			SLIB_INVOKE_EVENT_HANDLER(Draw, canvas)
		}
		
		{
			ListElements< Ref<View> > children(getChildren());
			if (children.count > 0) {
				drawChildren(canvas, children.data, children.count);
			}
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
			bitmap->resetPixels((sl_uint32)(rc.left), (sl_uint32)(rc.top), (sl_uint32)(rc.getWidth()), (sl_uint32)(rc.getHeight()), Color::zero());
		} while (0);

		if (m_flagClipping && (drawAttrs->boundShape != BoundShape::Rectangle && drawAttrs->boundShape != BoundShape::None)) {
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
		BoundShape boundShape = getBoundShape();
		if (m_flagClipping && boundShape != BoundShape::None) {
			CanvasStateScope scope(canvas);
			clipBounds(canvas);
			drawContent(canvas);
		} else {
			drawContent(canvas);
		}
	}
	
	void View::clipBounds(Canvas* canvas)
	{
		Rectangle rcClip = getBounds();
		switch (getBoundShape()) {
			case BoundShape::Rectangle:
				canvas->clipToRectangle(rcClip);
				break;
			case BoundShape::RoundRect:
				canvas->clipToRoundRect(rcClip, getBoundRadius());
				break;
			case BoundShape::Ellipse:
				canvas->clipToEllipse(rcClip);
				break;
			case BoundShape::Path:
				canvas->clipToPath(getBoundPath());
				break;
			default:
				break;
		}
	}

	void View::clipContentBounds(Canvas* canvas)
	{
		Rectangle rcClip = getBoundsInnerPadding();
		switch (getContentShape()) {
			case BoundShape::Rectangle:
				canvas->clipToRectangle(rcClip);
				break;
			case BoundShape::RoundRect:
				canvas->clipToRoundRect(rcClip, getContentRadius());
				break;
			case BoundShape::Ellipse:
				canvas->clipToEllipse(rcClip);
				break;
			case BoundShape::Path:
				canvas->clipToPath(getContentBoundPath());
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
		if (isNativeWidget()) {
			UI::dispatchToUiThread(callback);
			return;
		}
		if (callback.isNotNull()) {
			_initializeDrawAttributes();
			Ref<DrawAttributes>& attrs = m_drawAttrs;
			if (attrs.isNotNull()) {
				if (attrs->runAfterDrawCallbacks.isNull()) {
					ObjectLocker lock(this);
					if (attrs->runAfterDrawCallbacks.isNull()) {
						attrs->runAfterDrawCallbacks.pushBack(callback);
						forceDraw(flagInvalidate);
						return;
					}
				}
				attrs->runAfterDrawCallbacks.pushBack(callback);
				forceDraw(flagInvalidate);
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
		if (m_instance.isNotNull()) {
			UI::dispatchToUiThreadUrgently(callback, delayMillis);
		} else {
			Ref<View> parent = m_parent;
			if (parent.isNotNull()) {
				parent->dispatchToDrawingThread(callback, delayMillis);
			} else {
				UI::dispatchToUiThreadUrgently(callback, delayMillis);
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

	sl_bool View::isUiThread()
	{
		return UI::isUiThread();
	}
	
	void View::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		UI::dispatchToUiThread(callback, delayMillis);
	}
	
	void View::runOnUiThread(const Function<void()>& callback)
	{
		UI::runOnUiThread(callback);
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

	Ref<Timer> View::createTimer(const Function<void(Timer*)>& task, sl_uint32 interval_ms)
	{
		return Timer::createWithDispatcher(getDispatcher(), task, interval_ms);
	}
	
	Ref<Timer> View::startTimer(const Function<void(Timer*)>& task, sl_uint32 interval_ms)
	{
		return Timer::startWithDispatcher(getDispatcher(), task, interval_ms);
	}
	
	void View::onChangeParent(View* oldParent, View* newParent)
	{
	}
	
	void View::onAddChild(View* child)
	{
	}
	
	void View::onRemoveChild(View* child)
	{
	}
	
	void View::onAttachChild(View* child)
	{
	}
	
	void View::onDetachChild(View* child)
	{
	}
	
	void View::onUpdateLayout()
	{
		if (getChildrenCount() > 0) {
			measureAndSetLayoutWrappingSize(isWidthWrapping(), isHeightWrapping());
		} else {
#if defined(SLIB_PLATFORM_IS_MOBILE)
			if (isWidthWrapping()) {
				setLayoutWidth(UI::getScreenWidth() / 4);
			}
			if (isHeightWrapping()) {
				setLayoutHeight(UI::getScreenWidth() / 6);
			}
#else
			if (isWidthWrapping()) {
				setLayoutWidth(80);
			}
			if (isHeightWrapping()) {
				setLayoutHeight(60);
			}
#endif
		}
	}
	
	void View::onChangePadding()
	{
		_setInstancePadding();
	}
	
	void View::onUpdatePaging()
	{
		_setInstancePaging();
	}

	void View::onDrawBackground(Canvas* canvas)
	{
		drawBackground(canvas, getCurrentBackground());
	}
	
	void View::onDrawBorder(Canvas* canvas)
	{
		Ref<DrawAttributes>& attrs = m_drawAttrs;
		if (attrs.isNotNull()) {
			drawBorder(canvas, attrs->penBorder);
		}
	}
	
	void View::onResizeChild(View* child, sl_ui_len width, sl_ui_len height)
	{
	}
	
	void View::onChangeVisibilityOfChild(View* child, Visibility oldVisibility, Visibility newVisibility)
	{
	}
	
	void View::onResizeContent(sl_scroll_pos width, sl_scroll_pos height)
	{
	}
	
#define DEFINE_VIEW_EVENT_HANDLER_WITHOUT_ON(NAME, ...) \
	slib::Function<void(View* sender, ##__VA_ARGS__)> View::getOn##NAME() const { \
		const Ref<EventAttributes>& attrs = m_eventAttrs; \
		if (attrs.isNotNull()) return attrs->on##NAME; else return sl_null; \
	} \
	slib::Function<void(View* sender, ##__VA_ARGS__)> View::setOn##NAME(const slib::Function<void(View* sender, ##__VA_ARGS__)>& handler) { \
		_initializeEventAttributes(); \
		Ref<EventAttributes>& attrs = m_eventAttrs; \
		if (attrs.isNotNull()) attrs->on##NAME = handler; \
		return handler; \
	} \
	slib::Function<void(View* sender, ##__VA_ARGS__)> View::addOn##NAME(const slib::Function<void(View* sender, ##__VA_ARGS__)>& handler) { \
		_initializeEventAttributes(); \
		Ref<EventAttributes>& attrs = m_eventAttrs; \
		if (attrs.isNotNull()) attrs->on##NAME.add(handler); \
		return handler; \
	} \
	void View::removeOn##NAME(const slib::Function<void(View* sender, ##__VA_ARGS__)>& handler) { \
		_initializeEventAttributes(); \
		Ref<EventAttributes>& attrs = m_eventAttrs; \
		if (attrs.isNotNull()) attrs->on##NAME.remove(handler); \
	}

#define DEFINE_VIEW_EVENT_HANDLER(NAME, ...) \
	DEFINE_VIEW_EVENT_HANDLER_WITHOUT_ON(NAME, ##__VA_ARGS__) \
	void View::on##NAME(__VA_ARGS__) {}

	DEFINE_VIEW_EVENT_HANDLER(Attach)
	
	void View::dispatchAttach()
	{
		SLIB_INVOKE_EVENT_HANDLER(Attach)
		_attachNativeAnimations();
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->onAttachChild(this);
		}
	}

	DEFINE_VIEW_EVENT_HANDLER(Detach)

	void View::dispatchDetach()
	{
		SLIB_INVOKE_EVENT_HANDLER(Detach)
		Ref<View> parent = m_parent;
		if (parent.isNotNull()) {
			parent->onDetachChild(this);
		}
	}
	
	DEFINE_VIEW_EVENT_HANDLER(Draw, Canvas* canvas)
	DEFINE_VIEW_EVENT_HANDLER_WITHOUT_ON(PreDraw, Canvas* canvas)
	DEFINE_VIEW_EVENT_HANDLER_WITHOUT_ON(PostDraw, Canvas* canvas)
	
	void View::dispatchDraw(Canvas* canvas)
	{
		Ref<DrawAttributes>& drawAttrs = m_drawAttrs;
		
		if (drawAttrs.isNotNull()) {
			drawAttrs->flagForcedDraw = sl_false;
		}
		
		do {
			
			Rectangle rcInvalidated = canvas->getInvalidatedRect();
			if (rcInvalidated.right < rcInvalidated.left + SLIB_EPSILON) {
				break;
			}
			if (rcInvalidated.bottom < rcInvalidated.top + SLIB_EPSILON) {
				break;
			}
			
			if (!(isInstance())) {
				_updateAndApplyLayoutWithMode(UIUpdateMode::None);
			}

			if (m_flagDrawing) {
				
				getOnPreDraw()(this, canvas);

				draw(canvas);
				
				onDrawBorder(canvas);
				
				getOnPostDraw()(this, canvas);

			} else {
				ListElements< Ref<View> > children(getChildren());
				if (children.count > 0) {
					if (m_flagClipping && getBoundShape() != BoundShape::None) {
						CanvasStateScope scope(canvas);
						clipBounds(canvas);
						drawChildren(canvas, children.data, children.count);
					} else {
						drawChildren(canvas, children.data, children.count);
					}
				}
			}
			
			if (m_scrollAttrs.isNotNull() && !isNativeWidget()) {
				Ref<View> scrollBars[2];
				_getScrollBars(scrollBars);
				if (scrollBars[0].isNotNull() || scrollBars[1].isNotNull()) {
					drawChildren(canvas, scrollBars, 2);
				}
			}
			
		} while (0);
		
		if (drawAttrs.isNotNull()) {
			sl_size n = drawAttrs->runAfterDrawCallbacks.getCount();
			Function<void()> callback;
			while (n > 0 && drawAttrs->runAfterDrawCallbacks.popFront(&callback)) {
				callback();
				n--;
			}
		}
		
	}

	namespace priv
	{
		namespace view
		{

			SLIB_INLINE static UIAction GetActionUp(UIAction actionDown)
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
			
			class DuringEventScope
			{
			public:
				View* view;
				
			public:
				SLIB_INLINE DuringEventScope(View* view, UIEvent* ev)
				{
					this->view = view;
					view->setCurrentEvent(ev);
				}
				
				SLIB_INLINE ~DuringEventScope()
				{
					view->setCurrentEvent(sl_null);
				}
				
			};

		}
	}

#define POINT_EVENT_CHECK_CHILD(c) (c && !(c->isInstance()) && c->isVisible() && c->isHitTestable())

	DEFINE_VIEW_EVENT_HANDLER(MouseEvent, UIEvent* ev)
	
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
		if (!(ev->getFlags() & UIEventFlags::NotDispatchToChildren)) {
			Ref<View> scrollBars[2];
			_getScrollBars(scrollBars);
			Ref<ChildAttributes>& childAttrs = m_childAttrs;
			if (childAttrs.isNotNull()) {
				Ref<View> oldChildMouseMove;
				if (action == UIAction::MouseMove || action == UIAction::MouseEnter) {
					oldChildMouseMove = childAttrs->childMouseMove;
				}
				if (!(dispatchMouseEventToChildren(ev, scrollBars, 2))) {
					if (childAttrs->flagPassEventToChildren) {
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
						childAttrs->childMouseMove.setNull();
					}
				}
			}
		}
		
		Ref<GestureDetector> gesture = getGestureDetector();
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
		
		ev->resetFlags();
		
		priv::view::DuringEventScope scope(this, ev);
		
		SLIB_INVOKE_EVENT_HANDLER(MouseEvent, ev)

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
		Ref<ChildAttributes>& childAttrs = m_childAttrs;
		if (childAttrs.isNull()) {
			return sl_false;
		}

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
								oldChild = childAttrs->childMouseDown;
								if (oldChild.isNotNull() && oldChild != child) {
									ev->setAction(UIAction::TouchCancel);
									dispatchTouchEventToChild(ev, oldChild.get());
									ev->setAction(action);
								}
								childAttrs->childMouseDown = child;
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
				oldChild = childAttrs->childMouseDown;
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
				oldChild = childAttrs->childMouseDown;
				if (oldChild.isNotNull()) {
					dispatchMouseEventToChild(ev, oldChild.get());
					if (action == priv::view::GetActionUp(m_actionMouseDown)) {
						childAttrs->childMouseDown.setNull();
						m_actionMouseDown = UIAction::Unknown;
					}
				}
				return sl_true;
			case UIAction::MouseMove:
			case UIAction::MouseEnter:
				oldChild = childAttrs->childMouseMove;
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
								childAttrs->childMouseMove = child;
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
				oldChild = childAttrs->childMouseMove;
				if (oldChild.isNotNull()) {
					dispatchMouseEventToChild(ev, oldChild.get());
					childAttrs->childMouseMove.setNull();
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
			ev->resetFlags();
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

	DEFINE_VIEW_EVENT_HANDLER(TouchEvent, UIEvent* ev)
	
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
		if (!(ev->getFlags() & UIEventFlags::NotDispatchToChildren)) {
			Ref<View> scrollBars[2];
			_getScrollBars(scrollBars);
			Ref<ChildAttributes>& childAttrs = m_childAttrs;
			if (childAttrs.isNotNull()) {
				if (!(dispatchTouchEventToChildren(ev, scrollBars, 2))) {
					if (childAttrs->flagPassEventToChildren) {
						ListElements< Ref<View> > children(getChildren());
						if (children.count > 0) {
							if (childAttrs->flagTouchMultipleChildren) {
								dispatchTouchEventToMultipleChildren(ev, children.data, children.count);
							} else {
								dispatchTouchEventToChildren(ev, children.data, children.count);
							}
						}
					}
				}
			}
		}
		
		Ref<GestureDetector> gesture = getGestureDetector();
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
		
		{
			int flags = ev->getFlags() & UIEventFlags::KeepKeyboard;
			ev->resetFlags();
			if (flags || m_flagKeepKeyboard) {
				ev->addFlag(UIEventFlags::KeepKeyboard);
			}
		}
		
		priv::view::DuringEventScope scope(this, ev);

		SLIB_INVOKE_EVENT_HANDLER(TouchEvent, ev)
		SLIB_INVOKE_EVENT_HANDLER(MouseEvent, ev)
		
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
		Ref<ChildAttributes>& childAttrs = m_childAttrs;
		if (childAttrs.isNull()) {
			return sl_false;
		}

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
								oldChild = childAttrs->childMouseDown;
								if (oldChild.isNotNull() && oldChild != child) {
									ev->setAction(UIAction::TouchCancel);
									dispatchTouchEventToChild(ev, oldChild.get());
									ev->setAction(action);
								}
								childAttrs->childMouseDown = child;
								m_actionMouseDown = action;
								return sl_true;
							}
						}
					}
				}
				break;
			case UIAction::TouchMove:
				oldChild = childAttrs->childMouseDown;
				if (oldChild.isNotNull()) {
					dispatchTouchEventToChild(ev, oldChild.get());
					return sl_true;
				}
				break;
			case UIAction::TouchEnd:
			case UIAction::TouchCancel:
				oldChild = childAttrs->childMouseDown;
				if (oldChild.isNotNull()) {
					dispatchTouchEventToChild(ev, oldChild.get());
					if (m_actionMouseDown == UIAction::TouchBegin) {
						childAttrs->childMouseDown.setNull();
						m_actionMouseDown = UIAction::Unknown;
					}
					return sl_true;
				}
				break;
			default:
				return sl_true;
		}
		return sl_false;
	}

	void View::dispatchTouchEventToMultipleChildren(UIEvent *ev, const Ref<View>* children, sl_size count)
	{
		Ref<ChildAttributes>& childAttrs = m_childAttrs;
		if (childAttrs.isNull()) {
			return;
		}

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
		List< Ref<View> >& old = childAttrs->childrenMultiTouch;
		if (old.isNotNull()){
			{
				ListElements< Ref<View> > list(selectedChildren);
				for (sl_size i = 0; i < list.count; i++) {
					old.remove_NoLock(list[i]);
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
		
		childAttrs->childrenMultiTouch = selectedChildren;
		
	}

	void View::dispatchTouchEventToChild(UIEvent* ev, View* child, sl_bool flagTranformPoints)
	{
		if (child) {
			
			ev->resetFlags();
			
			if (flagTranformPoints) {
				
				TouchPoint ptTouch = ev->getTouchPoint();
				Array<TouchPoint> arr = ev->getTouchPoints();

				sl_size n = arr.getCount();
				
				if (n > 0) {
					Array<TouchPoint> arrConverted = arr.duplicate();
					if (arrConverted.isNull()) {
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
	
	DEFINE_VIEW_EVENT_HANDLER(MouseWheelEvent, UIEvent* ev)
	
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
			Ref<ChildAttributes>& childAttrs = m_childAttrs;
			if (childAttrs.isNotNull()) {
				if (!(dispatchMouseWheelEventToChildren(ev, scrollBars, 2))) {
					if (childAttrs->flagPassEventToChildren) {
						ListElements< Ref<View> > children(getChildren());
						if (children.count > 0) {
							dispatchMouseWheelEventToChildren(ev, children.data, children.count);
						}
					}
				}
			}
		}
		
		if (ev->isStoppedPropagation()) {
			return;
		}
		
		ev->resetFlags();
		
		priv::view::DuringEventScope scope(this, ev);

		SLIB_INVOKE_EVENT_HANDLER(MouseWheelEvent, ev)
		
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
			ev->resetFlags();
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

	DEFINE_VIEW_EVENT_HANDLER(KeyEvent, UIEvent* ev)
	
	void View::dispatchKeyEvent(UIEvent* ev)
	{
		if (!ev) {
			return;
		}
		if (! m_flagEnabled) {
			return;
		}
		
		if (!(ev->getFlags() & UIEventFlags::NotDispatchToChildren)) {
			Ref<ChildAttributes>& childAttrs = m_childAttrs;
			if (childAttrs.isNotNull()) {
				Ref<View> viewFocusedChild = childAttrs->childFocused;
				if (viewFocusedChild.isNotNull() && !(viewFocusedChild->isInstance())) {
					viewFocusedChild->dispatchKeyEvent(ev);
				}
			}
		}
		
		if (ev->isStoppedPropagation()) {
			return;
		}
		
		ev->resetFlags();
		
		priv::view::DuringEventScope scope(this, ev);

		SLIB_INVOKE_EVENT_HANDLER(KeyEvent, ev)
		
		if (ev->isPreventedDefault()) {
			return;
		}
		
		if (isContentScrollingByKeyboard()) {
			_processContentScrollingEvents(ev);
		}
		
		if (isTabStopEnabled() && getFocusedChild().isNull()) {
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
		
		if (m_flagOkCancelEnabled) {
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
	
	DEFINE_VIEW_EVENT_HANDLER_WITHOUT_ON(Click)

	void View::dispatchClick()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchClickEvent(ev.get());
		}
	}

	DEFINE_VIEW_EVENT_HANDLER(ClickEvent, UIEvent* ev)
	
	void View::dispatchClickEvent(UIEvent* ev)
	{
		if (! m_flagEnabled) {
			return;
		}
		SLIB_INVOKE_EVENT_HANDLER(ClickEvent, ev)
		getOnClick()(this);
	}
	
	DEFINE_VIEW_EVENT_HANDLER(SetCursor, UIEvent* ev)

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
			Ref<ChildAttributes>& childAttrs = m_childAttrs;
			if (childAttrs.isNotNull()) {
				if (!(dispatchSetCursorToChildren(ev, scrollBars, 2))) {
					if (childAttrs->flagPassEventToChildren) {
						ListElements< Ref<View> > children(getChildren());
						if (children.count > 0) {
							dispatchSetCursorToChildren(ev, children.data, children.count);
						}
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
		
		ev->resetFlags();
		
		SLIB_INVOKE_EVENT_HANDLER(SetCursor, ev)
		
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
			ev->resetFlags();
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

	DEFINE_VIEW_EVENT_HANDLER(ChangeFocus, sl_bool flagFocused)
	
	void View::dispatchChangeFocus(sl_bool flagFocused)
	{
		SLIB_INVOKE_EVENT_HANDLER(ChangeFocus, flagFocused)
	}

	DEFINE_VIEW_EVENT_HANDLER(Move, sl_ui_pos x, sl_ui_pos y)
	
	void View::dispatchMove(sl_ui_pos x, sl_ui_pos y)
	{
		SLIB_INVOKE_EVENT_HANDLER(Move, x, y)
	}

	DEFINE_VIEW_EVENT_HANDLER(Resize, sl_ui_len width, sl_ui_len height)
	
	void View::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		refreshScroll(UIUpdateMode::None);
		
		SLIB_INVOKE_EVENT_HANDLER(Resize, width, height)
		
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->onResizeChild(this, width, height);
		}
	}

	DEFINE_VIEW_EVENT_HANDLER(ChangeVisibility, Visibility oldVisibility, Visibility newVisibility)

	void View::dispatchChangeVisibility(Visibility oldVisibility, Visibility newVisibility)
	{
		SLIB_INVOKE_EVENT_HANDLER(ChangeVisibility, oldVisibility, newVisibility)
		
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			parent->onChangeVisibilityOfChild(this, oldVisibility, newVisibility);
		}
	}

	DEFINE_VIEW_EVENT_HANDLER(Scroll, sl_scroll_pos x, sl_scroll_pos)

	void View::dispatchScroll(sl_scroll_pos x, sl_scroll_pos y)
	{
		SLIB_INVOKE_EVENT_HANDLER(Scroll, x, y)
	}

	DEFINE_VIEW_EVENT_HANDLER(Swipe, GestureEvent* ev)

	void View::dispatchSwipe(GestureEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(Swipe, ev)
	}
	
	DEFINE_VIEW_EVENT_HANDLER(OK, UIEvent* ev)

	void View::dispatchOK(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(OK, ev)

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
	
	DEFINE_VIEW_EVENT_HANDLER(Cancel, UIEvent* ev)

	void View::dispatchCancel(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(Cancel, ev)
		
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
	
	void View::_cancelPressState()
	{
		Ref<View> view = this;
		while (view.isNotNull()) {
			if (view->m_flagPressed) {
				view->setPressedState(sl_false);
			}
			Ref<ChildAttributes>& childAttrs = view->m_childAttrs;
			if (childAttrs.isNotNull()) {
				view = childAttrs->childMouseDown;
			} else {
				return;
			}
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
						dispatchClickEvent(ev);
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

	namespace priv
	{
		namespace view
		{

			static void ScrollPagingElement(sl_scroll_pos& value, sl_scroll_pos speed, sl_scroll_pos pageSize)
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

			static void SmoothScrollElement(sl_scroll_pos& value, sl_scroll_pos& target, sl_scroll_pos dt, sl_scroll_pos T, sl_bool& flagAnimating)
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

		}
	}

	void View::_processContentScrollingEvents(UIEvent* ev)
	{
		Ref<ScrollAttributes>& scrollAttrs = m_scrollAttrs;
		if (scrollAttrs.isNull()) {
			return;
		}
		if (isNativeWidget()) {
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
				if (!scrollAttrs->flagDownContent) {
					scrollAttrs->flagDownContent = sl_true;
					scrollAttrs->mousePointBefore = ev->getPoint();
					scrollAttrs->touchPointerIdBefore = ev->getTouchPoint().pointerId;
					if (scrollAttrs->flagSmoothContentScrolling) {
						scrollAttrs->motionTracker->clearMovements();
						scrollAttrs->motionTracker->addMovement(ev);
					}
				}
				ev->stopPropagation();
				break;
			case UIAction::LeftButtonDrag:
			case UIAction::TouchMove:
				_stopContentScrollingFlow();
				if (scrollAttrs->flagDownContent) {
					if (ev->getTouchPoint().pointerId == scrollAttrs->touchPointerIdBefore) {
						Point offset = ev->getPoint() - scrollAttrs->mousePointBefore;
						sl_scroll_pos sx = scrollAttrs->x;
						sl_scroll_pos sy = scrollAttrs->y;
						if (flagHorz) {
							sx -= offset.x;
						}
						if (flagVert) {
							sy -= offset.y;
						}
						if (scrollAttrs->flagSmoothContentScrolling) {
							_scrollTo(sx, sy, sl_true, sl_true, sl_false);
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
						Ref<ChildAttributes>& childAttrs = m_childAttrs;
						if (childAttrs.isNotNull()) {
							if (offset.getLength2p() > T * T) {
								Ref<View> view = childAttrs->childMouseDown;
								if (view.isNotNull()) {
									ev->setAction(UIAction::TouchCancel);
									dispatchTouchEventToChild(ev, view.get());
									ev->setAction(action);
								}
							}
						}
					}
					scrollAttrs->mousePointBefore = ev->getPoint();
					scrollAttrs->touchPointerIdBefore = ev->getTouchPoint().pointerId;
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
							priv::view::ScrollPagingElement(x, speed.x, pageWidth);
						}
						if (flagVert) {
							sl_scroll_pos pageHeight = (sl_scroll_pos)(scrollAttrs->pageHeight == 0 ? getHeight() : scrollAttrs->pageHeight);
							priv::view::ScrollPagingElement(y, speed.y, pageHeight);
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
		if (scrollAttrs->timerFlow.isNull()) {
			scrollAttrs->timerFlow = startTimer(SLIB_FUNCTION_WEAKREF(View, _processContentScrollingFlow, this), SMOOTH_SCROLL_FRAME_MS);
		}
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

	void View::_processContentScrollingFlow(Timer* timer)
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
			priv::view::SmoothScrollElement(x, scrollAttrs->xSmoothTarget, dt, T, flagX);
			priv::view::SmoothScrollElement(y, scrollAttrs->ySmoothTarget, dt, T, flagY);
			
			_scrollTo(x, y, sl_true, sl_false, sl_true);
			
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

			_scrollTo(x, y, sl_true, flagFinish, sl_true);
			
		}
		
		invalidate();
		
	}
	
	void View::_setInstancePaging()
	{
		Ref<ViewInstance> instance = getNativeWidget();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&View::_setInstancePaging)
			instance->setPaging(this, isPaging(), getPageWidth(), getPageHeight());
		}
	}

	void View::_onScroll_NW(sl_scroll_pos x, sl_scroll_pos y)
	{
		_scrollTo(x, y, sl_false, sl_true, sl_false);
	}
	
	
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
	
	void ViewInstance::setBorder(View* view, sl_bool flag)
	{
	}
	
	void ViewInstance::setBackgroundColor(View* view, const Color& color)
	{
	}
	
	void ViewInstance::setFont(View* view, const Ref<Font>& font)
	{
	}

	void ViewInstance::setPadding(View* view, const UIEdgeInsets& padding)
	{
	}
	
	void ViewInstance::setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical)
	{
	}
	
	void ViewInstance::scrollTo(View* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
	{
	}

	void ViewInstance::setPaging(View* view, sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight)
	{
	}
	
	namespace priv
	{
		namespace view
		{
			Color GetDefaultBackColor();
		}
	}

	void ViewInstance::onDraw(Canvas* canvas)
	{
		Ref<View> view = getView();
		if (view.isNotNull()) {
#if defined(SLIB_UI_IS_MACOS)
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
#if defined(SLIB_UI_IS_WIN32)
			{
				static Ref<Bitmap> bitmap;
				static Ref<Canvas> canvasBitmap;
				UIRect rc = canvas->getInvalidatedRect();
				UISize size = rc.getSize();
				if (size.x < 1) {
					return;
				}
				if (size.y < 1) {
					return;
				}
				UISize screenSize = UI::getScreenSize();
				if (size.x > screenSize.x) {
					size.x = screenSize.x;
				}
				if (size.y > screenSize.y) {
					size.y = screenSize.y;
				}
				sl_uint32 widthBitmap = (sl_uint32)(size.x);
				sl_uint32 heightBitmap = (sl_uint32)(size.y);
				if (bitmap.isNull() || canvasBitmap.isNull() || bitmap->getWidth() < widthBitmap || bitmap->getHeight() < heightBitmap) {
					bitmap = Bitmap::create(widthBitmap, heightBitmap);
					if (bitmap.isNull()) {
						return;
					}
					canvasBitmap = bitmap->getCanvas();
					if (canvasBitmap.isNull()) {
						return;
					}
				}
				Color colorClear = Color::zero();
				do {
					if (view->isOpaque()) {
						break;
					}
					Color color = view->getBackgroundColor();
					if (color.a == 255) {
						break;
					}
					Ref<Window> window = view->getWindow();
					if (window.isNotNull() && window->getContentView() == view) {
						colorClear = window->getBackgroundColor();
						if (colorClear.a < 255) {
							Color c = priv::view::GetDefaultBackColor();
							c.blend_PA_NPA(colorClear);
							colorClear = c;
						}
						break;
					} else {
						colorClear = priv::view::GetDefaultBackColor();
					}
				} while (0);
				rc.setSize(size);
				if (colorClear.isNotZero()) {
					bitmap->resetPixels(0, 0, widthBitmap, heightBitmap, colorClear);
				} else {
					bitmap->resetPixels(0, 0, widthBitmap, 1, Color::White);
				}
				canvasBitmap->setInvalidatedRect(rc);
				CanvasStateScope scope(canvasBitmap.get());
				canvasBitmap->translate(-(sl_real)(rc.left), -(sl_real)(rc.top));
				view->dispatchDraw(canvasBitmap.get());
				canvasBitmap->translate((sl_real)(rc.left), (sl_real)(rc.top));
				canvas->draw(rc, bitmap, Rectangle(0, 0, (sl_real)(size.x), (sl_real)(size.y)));
			}
#else
			view->dispatchDraw(canvas);
#endif
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
			if (ev->getFlags() & UIEventFlags::DispatchToParent) {
				view->dispatchKeyEvent(ev);
				view = view->getParent();
				while (view.isNotNull()) {
					if (!(view->isNativeWidget())) {
						ev->addFlag(UIEventFlags::NotDispatchToChildren);
						view->dispatchKeyEvent(ev);
					}
					view = view->getParent();
				}
			} else {
				view->dispatchKeyEvent(ev);
			}
		}
	}

	void ViewInstance::onMouseEvent(UIEvent* ev)
	{
		Ref<View> view = getView();
		if (view.isNotNull()) {
			if (ev->getFlags() & UIEventFlags::DispatchToParent) {
				view->dispatchMouseEvent(ev);
				UIPoint pt = ev->getPoint();
				Ref<View> child = view;
				view = view->getParent();
				while (view.isNotNull()) {
					pt = child->convertCoordinateToParent(pt);
					if (!(view->isNativeWidget())) {
						ev->setPoint(pt);
						ev->addFlag(UIEventFlags::NotDispatchToChildren);
						view->dispatchMouseEvent(ev);
					}
					child = view;
					view = view->getParent();
				}
			} else {
				view->dispatchMouseEvent(ev);
			}
		}
	}

	void ViewInstance::onTouchEvent(UIEvent* ev)
	{
		Ref<View> view = getView();
		
		if (view.isNotNull()) {
		
			if (ev->getFlags() & UIEventFlags::DispatchToParent) {
			
				view->dispatchTouchEvent(ev);
				
				UIPoint pt = ev->getPoint();
				Array<TouchPoint> arrPts = ev->getTouchPoints();
				sl_size nPts = arrPts.getCount();
				TouchPoint* pts;

				Ref<View> child = view;
				view = view->getParent();
				
				if (view.isNotNull()) {
					if (nPts > 0) {
						arrPts = arrPts.duplicate();
						if (arrPts.isNull()) {
							return;
						}
					}
					pts = arrPts.getData();
				} else {
					return;
				}
				for (;;) {
					pt = child->convertCoordinateToParent(pt);
					for (sl_size i = 0; i < nPts; i++) {
						pts[i].point = child->convertCoordinateToParent(pts[i].point);
					}
					if (!(view->isNativeWidget())) {
						ev->setPoint(pt);
						ev->setTouchPoints(arrPts);
						ev->addFlag(UIEventFlags::NotDispatchToChildren);
						view->dispatchTouchEvent(ev);
					}
					child = view;
					view = view->getParent();
					if (view.isNull()) {
						return;
					}
				}
			} else {
				view->dispatchTouchEvent(ev);
			}
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
	
	void ViewInstance::onSetFocus()
	{
		Ref<View> view = getView();
		if (view.isNotNull()) {
			view->_setFocus(sl_true, sl_false, UIUpdateMode::Redraw);
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
		setCreatingChildInstances(sl_true);	
	}

	ViewGroup::~ViewGroup()
	{
	}

}
