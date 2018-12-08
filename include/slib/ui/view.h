/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_UI_VIEW
#define CHECKHEADER_SLIB_UI_VIEW

#include "definition.h"

#include "constants.h"
#include "event.h"
#include "cursor.h"

#include "../core/object.h"
#include "../core/ptr.h"
#include "../core/function.h"
#include "../core/animation.h"
#include "../core/time.h"

#include "../graphics/canvas.h"

namespace slib
{

	class Window;
	class ViewInstance;
	
	class Timer;
	class Dispatcher;
	class Bitmap;
	
	class ViewPage;
	class ScrollBar;
	class MotionTracker;
	class GestureDetector;
	class GestureEvent;

	class SLIB_EXPORT View : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		View();
		
		~View();
		
	public:
		Ref<ViewInstance> getViewInstance();
		
		sl_bool isInstance();

		sl_bool isValidInstance();
		
		sl_bool isCreatingInstance();
		
		// set before attaching
		void setCreatingInstance(sl_bool flag);
		
		sl_bool isCreatingChildInstances();
		
		// set before attaching
		void setCreatingChildInstances(sl_bool flag);
		
		sl_bool isCreatingNativeWidget();
		
		// set before attaching
		void setCreatingNativeWidget(sl_bool flag);
		
		UIAttachMode getAttachMode();
		
		// set before attaching
		void setAttachMode(UIAttachMode mode);

		virtual Ref<ViewInstance> createGenericInstance(ViewInstance* parent);
		
		virtual Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
		
		Ref<ViewInstance> createInstance(ViewInstance* parent);
		
		sl_bool isNativeWidget();
		
		sl_bool isHardwareLayer();
		
		// set before attaching
		void setHardwareLayer(sl_bool flagLayered);
		
		
		Ref<Window> getWindow();
		
		void setWindow(const Ref<Window>& window);
		
		
		Ref<View> getParent();
		
		void setParent(const Ref<View>& parent);
		
		void removeParent(View* parent = sl_null);
		
		void attach(const Ref<ViewInstance>& instance);
		
		void detach();
		
		Ref<ViewInstance> attachToNewInstance(const Ref<ViewInstance>& parent);
		
		
		String getId();
		
		void setId(const String& _id);
		
		List< Ref<View> > getChildren();
		
		sl_size getChildrenCount();
		
		Ref<View> getChild(sl_size index);
		
		void addChild(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		void insertChild(sl_size index, const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		void removeChild(sl_size index, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		void removeChild(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void removeAllChildren(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<View> getChildAt(sl_ui_pos x, sl_ui_pos y);
		
		Ref<View> getChildAt(const UIPoint& point);
		
		Ref<View> getChildById(const String& _id);
		
		Ref<View> getRootView();
		
		sl_bool isRootView();
		
		Ref<View> getNearestViewWithInstance();
		
		Ref<ViewInstance> getNearestViewInstance();
		
		Ref<ViewPage> getNearestViewPage();
		
		void removeFromParent();
		
		void attachChild(const Ref<View>& child);
		
		void addChildInstance(const Ref<ViewInstance>& instance);
		
		void removeChildInstance(const Ref<ViewInstance>& instance);
		
		sl_bool isOnAddChildEnabled();
		
		void setOnAddChildEnabled(sl_bool flagEnabled);
		
		sl_bool isOnRemoveChildEnabled();
		
		void setOnRemoveChildEnabled(sl_bool flagEnabled);
		
		void bringToFront(UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		
		virtual void invalidate(UIUpdateMode mode = UIUpdateMode::Redraw);

		// local coordinate
		virtual void invalidate(const UIRect& rect, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void invalidateBoundsInParent(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void updateAndInvalidateBoundsInParent(UIUpdateMode mode = UIUpdateMode::Redraw);

		sl_bool checkSelfInvalidatable();
		
		
		// parent coordinate
		UIRect getInstanceFrame();
		
		// parent coordinate
		const UIRect& getFrame();
		
		// parent coordinate
		void setFrame(const UIRect& frame, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		// parent coordinate
		void setFrame(sl_ui_pos x, sl_ui_pos y, sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		// parent coordinate
		void requestFrame(const UIRect& frame, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getWidth();
		
		void setWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getHeight();
		
		void setHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		UISize getSize();
		
		void setSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		// parent coordinate
		sl_ui_pos getLeft();
		
		// parent coordinate
		void setLeft(sl_ui_pos x, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		// parent coordinate
		sl_ui_pos getTop();
		
		// parent coordinate
		void setTop(sl_ui_pos y, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		// parent coordinate
		UIPoint getLocation();
		
		// parent coordinate
		void setLocation(const UIPoint& point, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		// parent coordinate
		void setLocation(sl_ui_pos x, sl_ui_pos y, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		// local coordinate
		UIRect getBounds();
		
		// local coordinate
		UIRect getBoundsInnerPadding();
		
		// parent coordinate
		UIRect getBoundsInParent();

		
		Visibility getVisibility();
		
		void setVisibility(Visibility visibility, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isVisible();
		
		void setVisible(sl_bool flagVisible, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isEnabled();
		
		virtual void setEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isClipping();
		
		void setClipping(sl_bool flagClipping, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isDrawing();
		
		void setDrawing(sl_bool flagDrawing, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isSavingCanvasState();
		
		void setSavingCanvasState(sl_bool flagDrawing);
		
		
		sl_bool isHitTestable();
		
		void setHitTestable(sl_bool flag);
		
		// local coordinate
		virtual sl_bool hitTest(sl_ui_pos x, sl_ui_pos y);
		
		// local coordinate
		sl_bool hitTest(const UIPoint& point);

		
		sl_bool isFocusable();
		
		void setFocusable(sl_bool flagFocusable);
		
		sl_bool isFocused();
		
		void setFocus(sl_bool flagFocused = sl_true, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<View> getFocusedChild();
		
		Ref<View> getFocusedDescendant();
		
		
		sl_bool isPressedState();
		
		virtual void setPressedState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHoverState();
		
		virtual void setHoverState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Cursor> getCursor();
		
		void setCursor(const Ref<Cursor>& cursor);

		
		void measureLayoutWrappingSize(sl_bool flagHorizontalWrapping, sl_bool flagVerticalWrapping);
		
		sl_bool isAlwaysOnUpdateLayout();
		
		void setAlwaysOnUpdateLayout(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		const UIRect& getLayoutFrame();
		
		void setLayoutFrame(const UIRect& frame);
		
		UISize getLayoutSize();
		
		void setLayoutSize(sl_ui_len width, sl_ui_len height);
		
		void setLayoutSize(const UISize& size);
		
		sl_ui_len getLayoutWidth();
		
		void setLayoutWidth(sl_ui_len width);
		
		sl_ui_len getLayoutHeight();
		
		void setLayoutHeight(sl_ui_len height);

		void invalidateLayout(UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		void invalidateParentLayout(UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		void invalidateSelfAndParentLayout(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void invalidateLayoutOfWrappingControl(UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		SizeMode getWidthMode();
		
		SizeMode getHeightMode();
		
		sl_bool isWidthFixed();
		
		void setWidthFixed(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isHeightFixed();
		
		void setHeightFixed(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getWidthWeight();
		
		sl_real getHeightWeight();
		
		sl_bool isWidthFilling();
		
		void setWidthFilling(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isHeightFilling();
		
		void setHeightFilling(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isWidthWrapping();
		
		void setWidthWrapping(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isHeightWrapping();
		
		void setHeightWrapping(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isWidthWeight();
		
		void setWidthWeight(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isHeightWeight();
		
		void setHeightWeight(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isLeftFree();
		
		void setLeftFree(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignParentLeft();
		
		void setAlignParentLeft(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignLeft();
		
		void setAlignLeft(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isRightOf();
		
		void setRightOf(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<View> getLayoutLeftReferingView();
		
		sl_bool isRightFree();
		
		void setRightFree(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignParentRight();
		
		void setAlignParentRight(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignRight();
		
		void setAlignRight(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isLeftOf();
		
		void setLeftOf(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<View> getLayoutRightReferingView();

		sl_bool isTopFree();
		
		void setTopFree(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignParentTop();
		
		void setAlignParentTop(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignTop();
		
		void setAlignTop(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isBelow();
		
		void setBelow(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<View> getLayoutTopReferingView();
		
		sl_bool isBottomFree();
		
		void setBottomFree(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignParentBottom();
		
		void setAlignParentBottom(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignBottom();
		
		void setAlignBottom(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAbove();
		
		void setAbove(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		Ref<View> getLayoutBottomReferingView();
		
		sl_bool isCenterHorizontal();
		
		void setCenterHorizontal(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isCenterVertical();
		
		void setCenterVertical(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setCenterInParent(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignCenterHorizontal();
		
		void setAlignCenterHorizontal(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isAlignCenterVertical();
		
		void setAlignCenterVertical(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		
		sl_ui_len getMinimumWidth();
		
		void setMinimumWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getMaximumWidth();
		
		void setMaximumWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getMinimumHeight();
		
		void setMinimumHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getMaximumHeight();
		
		void setMaximumHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		AspectRatioMode getAspectRatioMode();
		
		void setAspectRatioMode(AspectRatioMode aspectRatioMode, UIUpdateMode updateMode = UIUpdateMode::UpdateLayout);
		
		sl_real getAspectRatio();
		
		void setAspectRatio(sl_real ratio, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		
		sl_ui_pos getMarginLeft();
		
		void setMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getMarginTop();
		
		void setMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getMarginRight();
		
		void setMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getMarginBottom();
		
		void setMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setMargin(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isRelativeMarginLeft();
		
		sl_real getRelativeMarginLeftWeight();
		
		void setRelativeMarginLeft(sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getAbsoluteMarginLeft();
		
		void setAbsoluteMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		sl_bool isRelativeMarginTop();
		
		sl_real getRelativeMarginTopWeight();
		
		void setRelativeMarginTop(sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getAbsoluteMarginTop();

		void setAbsoluteMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isRelativeMarginRight();

		sl_real getRelativeMarginRightWeight();
		
		void setRelativeMarginRight(sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getAbsoluteMarginRight();

		void setAbsoluteMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		sl_bool isRelativeMarginBottom();
		
		sl_real getRelativeMarginBottomWeight();
		
		void setRelativeMarginBottom(sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getAbsoluteMarginBottom();

		void setAbsoluteMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void applyRelativeMargins(sl_ui_len parentWidth, sl_ui_len parentHeight);
		
		
		sl_ui_pos getPaddingLeft();
		
		void setPaddingLeft(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getPaddingTop();
		
		void setPaddingTop(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getPaddingRight();
		
		void setPaddingRight(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_pos getPaddingBottom();
		
		void setPaddingBottom(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setPadding(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		
		sl_bool getFinalTransform(Matrix3* _out);
		
		sl_bool getFinalInverseTransform(Matrix3* _out);
		
		const Matrix3& getTransform();
		
		void setTransform(const Matrix3& matrix, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void resetTransform(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool getFinalTranslationRotationScale(Vector2* translation = sl_null, sl_real* rotation = sl_null, Vector2* scale = sl_null, Vector2* anchor = sl_null);
		
		sl_real getTranslationX();
		
		sl_real getTranslationY();
		
		const Vector2& getTranslation();
		
		void setTranslationX(sl_real x, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTranslationY(sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTranslation(sl_real x, sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTranslation(const Vector2& t, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getScaleX();
		
		sl_real getScaleY();
		
		const Vector2& getScale();
		
		void setScaleX(sl_real sx, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScaleY(sl_real sy, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScale(sl_real x, sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScale(sl_real factor, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScale(const Vector2& factor, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getRotation();
		
		void setRotation(sl_real radian, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getAnchorOffsetX();
		
		sl_real getAnchorOffsetY();

		const Vector2& getAnchorOffset();
		
		void setAnchorOffsetX(sl_real x, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setAnchorOffsetY(sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setAnchorOffset(sl_real x, sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setAnchorOffset(const Vector2& pt, UIUpdateMode mode = UIUpdateMode::Redraw);
		

		UIPointf convertCoordinateFromScreen(const UIPointf& ptScreen);
		
		UIPointf convertCoordinateToScreen(const UIPointf& ptView);
		
		UIPointf convertCoordinateFromParent(const UIPointf& ptParent);
		
		UIRectf convertCoordinateFromParent(const UIRectf& rectParent);
		
		UIPointf convertCoordinateToParent(const UIPointf& ptView);

		UIRectf convertCoordinateToParent(const UIRectf& rectView);

		
		Ref<Drawable> getBackground();
		
		virtual void setBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getBackgroundColor();
		
		void setBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getPressedBackground();
		
		virtual void setPressedBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getPressedBackgroundColor();
		
		void setPressedBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverBackground();
		
		virtual void setHoverBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getHoverBackgroundColor();
		
		void setHoverBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		ScaleMode getBackgroundScaleMode();
		
		virtual void setBackgroundScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
		
		Alignment getBackgroundAlignment();
		
		virtual void setBackgroundAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Pen> getBorder();
		
		virtual void setBorder(const Ref<Pen>& pen, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		PenStyle getBorderStyle();
		
		void setBorderStyle(PenStyle style, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getBorderWidth();
		
		void setBorderWidth(sl_real width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getBorderColor();
		
		void setBorderColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isBorder();
		
		void setBorder(sl_bool flagBorder, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		BoundShape getBoundShape();
		
		void setBoundShape(BoundShape shape, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		const Size& getRoundRectBoundShapeRadius();
		
		void setRoundRectBoundShapeRadius(const Size& radius, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setRoundRectBoundShapeRadius(sl_real rx, sl_real ry, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setRoundRectBoundShapeRadiusX(sl_real rx, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setRoundRectBoundShapeRadiusY(sl_real ry, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setRoundRectBoundShapeRadius(sl_real radius, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<GraphicsPath> getBoundShapePath();
		
		void setBoundShapePath(const Ref<GraphicsPath>& path, UIUpdateMode mode = UIUpdateMode::Redraw);

		
		sl_bool isPreDrawEnabled();
		
		void setPreDrawEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isPostDrawEnabled();
		
		void setPostDrawEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isAlwaysOnDrawBackground();
		
		void setAlwaysOnDrawBackground(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isAlwaysOnDrawBorder();
		
		void setAlwaysOnDrawBorder(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);

		
		Ref<Font> getFont();
		
		virtual void setFont(const Ref<Font>& font, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getFontSize();
		
		void setFontSize(sl_real size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		String getFontFamily();
		
		void setFontFamily(const String& fontFamily, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		sl_bool isUsingFont();
		
		void setUsingFont(sl_bool flag);
		
		
		sl_bool isOpaque();
		
		void setOpaque(sl_bool flagOpaque, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getAlpha();
		
		void setAlpha(sl_real alpha, UIUpdateMode mode = UIUpdateMode::Redraw);

		sl_bool isLayer();
		
		void setLayer(sl_bool flagLayer, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void invalidateLayer();
		
		void invalidateLayer(const UIRect& rect);
		
		sl_bool isForcedDraw();
		
		void forceDraw(sl_bool flagInvalidate = sl_true);
		
		
		virtual Ref<AnimationLoop> getAnimationLoop();
		
		Ref<Animation> createAnimation(float duration);

		Ref<Animation> createAnimation(const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startAnimation(const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);

		
		Ref<Animation> getTransformAnimation();
		
		void setTransformAnimation(const Ref<Animation>& animation, const AnimationFrames<Matrix3>& frames);
		
		void setTransformAnimation(const Ref<Animation>& animation, const Matrix3& startValue, const Matrix3& endValue);
		
		void setTransformAnimation(const Ref<Animation>& animation, const Matrix3& toValue);
		
		Ref<Animation> createTransformAnimation(const AnimationFrames<Matrix3>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startTransformAnimation(const AnimationFrames<Matrix3>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createTransformAnimationTo(const Matrix3& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startTransformAnimationTo(const Matrix3& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);

		
		Ref<Animation> getTranslateAnimation();
		
		void setTranslateAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames);
		
		void setTranslateAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue);
		
		void setTranslateAnimation(const Ref<Animation>& animation, const Vector2& toValue);
		
		Ref<Animation> createTranslateAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startTranslateAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createTranslateAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startTranslateAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		
		Ref<Animation> getScaleAnimation();
		
		void setScaleAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames);
		
		void setScaleAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue);
		
		void setScaleAnimation(const Ref<Animation>& animation, const Vector2& toValue);
		
		void setScaleAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue);
		
		void setScaleAnimation(const Ref<Animation>& animation, sl_real toValue);
		
		Ref<Animation> createScaleAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startScaleAnimation(const AnimationFrames<Vector2>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);

		Ref<Animation> createScaleAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startScaleAnimationTo(const Vector2& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createScaleAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startScaleAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createScaleAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startScaleAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		
		Ref<Animation> getRotateAnimation();
		
		void setRotateAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames);
		
		void setRotateAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue);
		
		void setRotateAnimation(const Ref<Animation>& animation, sl_real toValue);
		
		Ref<Animation> createRotateAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startRotateAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createRotateAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startRotateAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);

		Ref<Animation> createRotateAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startRotateAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);

		
		Ref<Animation> getFrameAnimation();
		
		void setFrameAnimation(const Ref<Animation>& animation, const AnimationFrames<Rectangle>& frames);
		
		void setFrameAnimation(const Ref<Animation>& animation, const Rectangle& startValue, const Rectangle& endValue);
		
		void setFrameAnimation(const Ref<Animation>& animation, const Rectangle& toValue);
		
		Ref<Animation> createFrameAnimation(const AnimationFrames<Rectangle>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startFrameAnimation(const AnimationFrames<Rectangle>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createFrameAnimation(const Rectangle& startValue, const Rectangle& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startFrameAnimation(const Rectangle& startValue, const Rectangle& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createFrameAnimationTo(const Rectangle& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startFrameAnimationTo(const Rectangle& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		
		Ref<Animation> getAlphaAnimation();
		
		void setAlphaAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames);
		
		void setAlphaAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue);
		
		void setAlphaAnimation(const Ref<Animation>& animation, sl_real toValue);
		
		Ref<Animation> createAlphaAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startAlphaAnimation(const AnimationFrames<sl_real>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createAlphaAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startAlphaAnimation(sl_real startValue, sl_real endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createAlphaAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startAlphaAnimationTo(sl_real toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);

		
		Ref<Animation> getBackgroundColorAnimation();
		
		void setBackgroundColorAnimation(const Ref<Animation>& animation, const AnimationFrames<Color4f>& frames);
		
		void setBackgroundColorAnimation(const Ref<Animation>& animation, const Color4f& startValue, const Color4f& endValue);
		
		void setBackgroundColorAnimation(const Ref<Animation>& animation, const Color4f& toValue);
		
		Ref<Animation> createBackgroundColorAnimation(const AnimationFrames<Color4f>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startBackgroundColorAnimation(const AnimationFrames<Color4f>& frames, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> createBackgroundColorAnimationTo(const Color4f& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		Ref<Animation> startBackgroundColorAnimationTo(const Color4f& toValue, float duration, const Function<void()>& onStop = sl_null, AnimationCurve curve = AnimationCurve::Default, const AnimationFlags& flags = AnimationFlags::Default);
		
		
		sl_bool isHorizontalScrolling();
		
		void setHorizontalScrolling(sl_bool flagHorizontal);
		
		sl_bool isVerticalScrolling();
		
		void setVerticalScrolling(sl_bool flagVertical);
		
		Ref<ScrollBar> getHorizontalScrollBar();
		
		Ref<ScrollBar> getVerticalScrollBar();
		
		void setHorizontalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setVerticalScrollBar(const Ref<ScrollBar>& bar, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void createHorizontalScrollBar(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void createVerticalScrollBar(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeHorizontalScrollBar(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeVerticalScrollBar(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void createScrollBars(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeScrollBars(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHorizontalScrollBarVisible();
		
		sl_bool isVerticalScrollBarVisible();
		
		void setHorizontalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setVerticalScrollBarVisible(sl_bool flagVisible, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHorizontalScrollBarValid();
		
		sl_bool isVerticalScrollBarValid();

		sl_scroll_pos getScrollX();
		
		sl_scroll_pos getScrollY();
		
		virtual Pointlf getScrollPosition();
		
		virtual void scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void scrollTo(const Pointlf& position, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void smoothScrollTo(const Pointlf& position, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScrollX(sl_scroll_pos x, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScrollY(sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_scroll_pos getContentWidth();
		
		sl_scroll_pos getContentHeight();
		
		ScrollPoint getContentSize();
		
		virtual void setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setContentSize(const ScrollPoint& size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setContentWidth(sl_scroll_pos width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setContentHeight(sl_scroll_pos height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual ScrollPoint getScrollRange();
		
		sl_ui_len getScrollBarWidth();
		
		void setScrollBarWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void refreshScroll(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isPaging();
		
		void setPaging(sl_bool flagPaging);
		
		sl_ui_len getPageWidth();
		
		void setPageWidth(sl_ui_len width);
		
		sl_ui_len getPageHeight();
		
		void setPageHeight(sl_ui_len height);
		
		
		sl_bool isContentScrollingByMouse();
		
		void setContentScrollingByMouse(sl_bool flag);
		
		sl_bool isContentScrollingByTouch();
		
		void setContentScrollingByTouch(sl_bool flag);
		
		sl_bool isContentScrollingByMouseWheel();
		
		void setContentScrollingByMouseWheel(sl_bool flag);
		
		sl_bool isContentScrollingByKeyboard();
		
		void setContentScrollingByKeyboard(sl_bool flag);
		
		sl_bool isSmoothContentScrolling();
		
		void setSmoothContentScrolling(sl_bool flag);

		
		sl_bool isTouchMultipleChildren();
		
		void setTouchMultipleChildren(sl_bool flag);
		
		sl_bool isPassingEventsToChildren();
		
		void setPassingEventsToChildren(sl_bool flag);
		
		
		sl_bool isProcessingOkCancel();
		
		void setProcessingOkCancel(sl_bool flag);

		
		Ref<View> getNextFocusableView();
		
		Ref<View> getPreviousFocusableView();
		
		Ref<View> getFirstFocusableDescendant();
		
		Ref<View> getLastFocusableDescendant();
		
		sl_bool isProcessingTabStop();
		
		void setProcessingTabStop(sl_bool flag);
		
		Ref<View> getNextTabStop();
		
		void setNextTabStop(const Ref<View>& view);
		
		Ref<View> getPreviousTabStop();
		
		void setPreviousTabStop(const Ref<View>& view);
		
		
		sl_bool isCapturingChildInstanceEvents();
		
		void setCapturingChildInstanceEvents(sl_bool flag);
		
		virtual sl_bool hitTestForCapturingChildInstanceEvents(const UIPoint& pt);
		
		
		Ref<GestureDetector> createGestureDetector();
		
		Ref<GestureDetector> getGestureDetector();
		
		
		Ref<Drawable> getCurrentBackground();
		
		void drawBackground(Canvas* canvas, const Ref<Drawable>& background);
		
		void drawBorder(Canvas* canvas, const Ref<Pen>& pen);
		
		void drawChildren(Canvas* canvas, const Ref<View>* children, sl_size count);
		
		void drawContent(Canvas* canvas);
		
		Ref<Bitmap> drawLayer();
		
		void draw(Canvas* canvas);
		
		void clipBounds(Canvas* canvas);
		
		virtual Size measureText(const String& size, const Ref<Font>& font, sl_bool flagMultiLine = sl_false);
		
		
		void runAfterDraw(const Function<void()>& callback, sl_bool flagInvalidate = sl_true);
		
		virtual sl_bool isDrawingThread();
		
		virtual void dispatchToDrawingThread(const Function<void()>& callback, sl_uint32 delayMillis = 0);
		
		virtual void runOnDrawingThread(const Function<void()>& callback);
		
		virtual Ref<Dispatcher> getDispatcher();
		
		Ref<Timer> createTimer(const Function<void(Timer*)>& task, sl_uint32 interval_ms);
		
		Ref<Timer> startTimer(const Function<void(Timer*)>& task, sl_uint32 interval_ms);

	public:
		Ptr<IViewListener> getEventListener();
		
		void setEventListener(const Ptr<IViewListener>& listener);
		
		Function<void(View*)> getOnAttach();
		
		void setOnAttach(const Function<void(View*)>& callback);
		
		Function<void(View*)> getOnDetach();
		
		void setOnDetach(const Function<void(View*)>& callback);

		Function<void(View*, Canvas*)> getOnDraw();
		
		void setOnDraw(const Function<void(View*, Canvas*)>& callback);
		
		Function<void(View*, Canvas*)> getOnPreDraw();
		
		void setOnPreDraw(const Function<void(View*, Canvas*)>& callback);
		
		Function<void(View*, Canvas*)> getOnPostDraw();
		
		void setOnPostDraw(const Function<void(View*, Canvas*)>& callback);

		Function<void(View*, UIEvent*)> getOnMouseEvent();
		
		void setOnMouseEvent(const Function<void(View*, UIEvent*)>& callback);
		
		Function<void(View*, UIEvent*)> getOnTouchEvent();
		
		void setOnTouchEvent(const Function<void(View*, UIEvent*)>& callback);
		
		Function<void(View*, UIEvent*)> getOnKeyEvent();
		
		void setOnKeyEvent(const Function<void(View*, UIEvent*)>& callback);
		
		Function<void(View*, UIEvent*)> getOnMouseWheelEvent();
		
		void setOnMouseWheelEvent(const Function<void(View*, UIEvent*)>& callback);
		
		Function<void(View*)> getOnClick();
		
		void setOnClick(const Function<void(View*)>& callback);
		
		Function<void(View*, UIEvent*)> getOnSetCursor();
		
		void setOnSetCursor(const Function<void(View*, UIEvent*)>& callback);
		
		Function<void(View*, sl_ui_len, sl_ui_len)> getOnResize();
		
		void setOnResize(const Function<void(View*, sl_ui_len, sl_ui_len)>& callback);
		
		Function<void(View*, sl_scroll_pos, sl_scroll_pos)> getOnScroll();
		
		void setOnScroll(const Function<void(View*, sl_scroll_pos, sl_scroll_pos)>& callback);
		
		Function<void(View*, GestureEvent*)> getOnSwipe();
		
		void setOnSwipe(const Function<void(View*, GestureEvent*)>& callback);
		
		Function<void(View*, UIEvent*)> getOnOK();
		
		void setOnOK(const Function<void(View*, UIEvent*)>& callback);
		
		Function<void(View*, UIEvent*)> getOnCancel();
		
		void setOnCancel(const Function<void(View*, UIEvent*)>& callback);
		
	protected:
		virtual void onDraw(Canvas* canvas);
		
		virtual void onPreDraw(Canvas* canvas);
		
		virtual void onPostDraw(Canvas* canvas);
		
		virtual void onDrawBackground(Canvas* canvas);
		
		virtual void onDrawBorder(Canvas* canvas);
		
		virtual void onMouseEvent(UIEvent* ev);
		
		virtual void onTouchEvent(UIEvent* ev);
		
		virtual void onMouseWheelEvent(UIEvent* ev);
		
		virtual void onKeyEvent(UIEvent* ev);
		
		virtual void onClick(UIEvent* ev);
		
		virtual void onSetCursor(UIEvent* ev);
		
		virtual void onResize(sl_ui_len width, sl_ui_len height);
		
		virtual void onResizeChild(View* child, sl_ui_len width, sl_ui_len height);
		
		virtual void onChangeVisibility(Visibility oldVisibility, Visibility newVisibility);
		
		virtual void onChangeVisibilityOfChild(View* child, Visibility oldVisibility, Visibility newVisibility);
		
		virtual void onScroll(sl_scroll_pos x, sl_scroll_pos y);
		
		virtual void onResizeContent(sl_scroll_pos width, sl_scroll_pos height);
								
		virtual void onSwipe(GestureEvent* ev);

		virtual void onAttach();
		
		virtual void onDetach();
		
		virtual void onAddChild(View* child);
		
		virtual void onRemoveChild(View* child);
		
		virtual void onUpdateLayout();
		
		virtual void onChangePadding();
		
		virtual void onUpdatePaging();
		
		virtual void onOK(UIEvent* ev);
		
		virtual void onCancel(UIEvent* ev);
		
	public:
		virtual void dispatchAttach();
		
		virtual void dispatchDetach();
		
		virtual void dispatchDraw(Canvas* canvas);
		
		virtual void dispatchMouseEvent(UIEvent* ev);
		
		sl_bool dispatchMouseEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
		
		void dispatchMouseEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
		
		virtual void dispatchTouchEvent(UIEvent* ev);
		
		sl_bool dispatchTouchEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
		
		void dispatchTouchEventToMultipleChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
		
		void dispatchTouchEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
		
		virtual void dispatchMouseWheelEvent(UIEvent* ev);
		
		sl_bool dispatchMouseWheelEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
		
		void dispatchMouseWheelEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
		
		virtual void dispatchKeyEvent(UIEvent* ev);
		
		virtual void dispatchClick(UIEvent* ev);
		
		void dispatchClick();
		
		virtual void dispatchSetCursor(UIEvent* ev);
		
		sl_bool dispatchSetCursorToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
		
		void dispatchSetCursorToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
		
		virtual void dispatchResize(sl_ui_len width, sl_ui_len height);
		
		virtual void dispatchChangeVisibility(Visibility oldVisibility, Visibility newVisibility);
		
		virtual void dispatchScroll(sl_scroll_pos x, sl_scroll_pos y);

		virtual void dispatchSwipe(GestureEvent* ev);
		
		virtual void dispatchOK(UIEvent* ev);
		
		void dispatchOK();
		
		virtual void dispatchCancel(UIEvent* ev);
		
		void dispatchCancel();
		
	private:
		void _processAttachOnUiThread();
		
		void _addChild(View* child, UIUpdateMode mode);

		void _removeChild(View* child);
		
		void _killFocusFromParent();
		
		void _setFocusedChild(View* child, UIUpdateMode mode);
		
		void _restrictSize(sl_ui_len& width, sl_ui_len& height);
		
		void _restrictSize(UIRect& rect);
		
		void _updateLayoutFrameInParent(void* param);
		
		void _updateLayout();
		
		void _applyLayout(UIUpdateMode mode);
		
		void _updateAndApplyChildLayout(View* child);

		void _updateAndApplyLayoutWithMode(UIUpdateMode mode);

		void _updateAndApplyLayout();
		
		void _applyCalcTransform(UIUpdateMode mode);
		
		void _applyFinalTransform(UIUpdateMode mode);
		
		void _invalidateInstanceTransform();
		
		void _refreshBorderPen(UIUpdateMode mode);
		
		void _setFontInvalidateChildren();
		

		void _resetTransformAnimation();
		
		void _resetTranslateAnimation();
		
		void _resetScaleAnimation();
		
		void _resetRotateAnimation();
		
		void _resetFrameAnimation();
		
		void _resetAlphaAnimation();
		
		void _resetBackgroundColorAnimation();

		
		Ref<ScrollBar> _createHorizontalScrollBar();
		
		Ref<ScrollBar> _createVerticalScrollBar();
		
		void _getScrollBars(Ref<View> views[2]);
		
		void _initScrollBars(UIUpdateMode mode);
		
		void _onScrollBarChangeValue(ScrollBar* scrollBar, sl_scroll_pos value);
		
		sl_bool _scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagFinish, sl_bool flagAnimate);
		
		void _cancelPressState();
		
		void _processEventForStateAndClick(UIEvent* ev);
		
		void _processContentScrollingEvents(UIEvent* ev);
		
		void _startContentScrollingFlow(sl_bool flagSmoothTarget, const Pointlf& speedOrTarget);
		
		void _stopContentScrollingFlow();
		
		void _processContentScrollingFlow(Timer* timer);
		
		
	public:
		void _setFrame_NI(const UIRect& frame);
		
		void _setTransform_NI(const Matrix3& transform);
		
		virtual void _setBorder_NW(sl_bool flag);

		virtual void _setBackgroundColor_NW(const Color& color);

		virtual void _setFont_NW(const Ref<Font>& font);

	private:
		AtomicRef<ViewInstance> m_instance;
		AtomicWeakRef<Window> m_window;
		AtomicWeakRef<View> m_parent;
		
		sl_bool m_flagCreatingInstance;
		sl_bool m_flagCreatingChildInstances;
		sl_bool m_flagCreatingNativeWidget;
		UIAttachMode m_attachMode;
		
		UIRect m_frame;
		UIRect m_boundsInParent;
		sl_bool m_flagInvalidLayout;
		sl_bool m_flagNeedApplyLayout;
		sl_ui_pos m_paddingLeft;
		sl_ui_pos m_paddingTop;
		sl_ui_pos m_paddingRight;
		sl_ui_pos m_paddingBottom;
		
		Visibility m_visibility;
		sl_bool m_flagEnabled;
		sl_bool m_flagHitTestable;
		sl_bool m_flagFocusable;
		sl_bool m_flagInstanceLayer;
		sl_bool m_flagClipping;
		sl_bool m_flagDrawing;
		sl_bool m_flagSavingCanvasState;

		sl_bool m_flagFocused;
		sl_bool m_flagPressed;
		sl_bool m_flagHover;
		
		sl_bool m_flagCurrentDrawing;
		UIRect m_rectCurrentDrawing;
		
		AtomicRef<Cursor> m_cursor;
		
		AtomicString m_id;
		AtomicList< Ref<View> > m_children;
		AtomicList< Ref<View> > m_childrenCache;
		List< Ref<View> > m_childrenMultiTouch;
		AtomicRef<View> m_childMouseMove;
		AtomicRef<View> m_childMouseDown;
		UIAction m_actionMouseDown;
		sl_bool m_flagTouchMultipleChildren;
		sl_bool m_flagPassEventToChildren;
		AtomicRef<View> m_childFocused;
		
		sl_bool m_flagOnAddChild;
		sl_bool m_flagOnRemoveChild;
		
		sl_bool m_flagProcessingOkCancel;
		sl_bool m_flagProcessingTabStop;
		AtomicWeakRef<View> m_viewNextTabStop;
		AtomicWeakRef<View> m_viewPrevTabStop;
		sl_bool m_flagCapturingChildInstanceEvents;
		
		AtomicRef<GestureDetector> m_gestureDetector;
		
	protected:
		class LayoutAttributes : public Referable
		{
		public:
			UIRect layoutFrame;
			UIRect requestedFrame;
			sl_bool flagRequestedFrame;

			SizeMode widthMode;
			SizeMode heightMode;
			sl_real widthWeight;
			sl_real heightWeight;
			
			PositionMode leftMode;
			PositionMode topMode;
			PositionMode rightMode;
			PositionMode bottomMode;
			AtomicWeakRef<View> leftReferingView;
			AtomicWeakRef<View> topReferingView;
			AtomicWeakRef<View> rightReferingView;
			AtomicWeakRef<View> bottomReferingView;
			
			sl_ui_len minWidth;
			sl_ui_len maxWidth;
			sl_ui_len minHeight;
			sl_ui_len maxHeight;
			AspectRatioMode aspectRatioMode;
			sl_real aspectRatio;
			
			sl_ui_pos marginLeft;
			sl_ui_pos marginTop;
			sl_ui_pos marginRight;
			sl_ui_pos marginBottom;
			sl_bool flagRelativeMarginLeft;
			sl_real relativeMarginLeftWeight;
			sl_bool flagRelativeMarginTop;
			sl_real relativeMarginTopWeight;
			sl_bool flagRelativeMarginRight;
			sl_real relativeMarginRightWeight;
			sl_bool flagRelativeMarginBottom;
			sl_real relativeMarginBottomWeight;
			
			sl_bool flagAlwaysOnUpdateLayout;
			sl_bool flagInvalidLayoutInParent;
			
		public:
			LayoutAttributes();
			
			~LayoutAttributes();
			
		};
		Ref<LayoutAttributes> m_layoutAttrs;
		
		void _initializeLayoutAttributes();

		class TransformAttributes : public Referable
		{
		public:
			sl_bool flagTransformFinalInvalid;
			sl_bool flagTransformFinal;
			Matrix3 transformFinal;
			
			sl_bool flagInverseTransformFinalInvalid;
			sl_bool flagInverseTransformFinal;
			Matrix3 inverseTransformFinal;
			
			sl_bool flagTransform;
			Matrix3 transform;
			
			sl_bool flagTransformCalcInvalid;
			sl_bool flagTransformCalc;
			Matrix3 transformCalc;
			
			Vector2 translation;
			Vector2 scale;
			sl_real rotationAngle;
			
			Vector2 anchorOffset;
			
			AtomicWeakRef<Animation> m_animationTransform;
			AtomicWeakRef<Animation> m_animationTranslate;
			AtomicWeakRef<Animation> m_animationScale;
			AtomicWeakRef<Animation> m_animationRotate;
			AtomicWeakRef<Animation> m_animationFrame;
			AtomicWeakRef<Animation> m_animationAlpha;
			AtomicWeakRef<Animation> m_animationBackgroundColor;

		public:
			TransformAttributes();
			
			~TransformAttributes();
			
		};
		Ref<TransformAttributes> m_transformAttrs;
		
		void _initializeTransformAttributes();
		
		class DrawAttributes : public Referable
		{
		public:
			AtomicRef<Drawable> background;
			AtomicRef<Drawable> backgroundPressed;
			AtomicRef<Drawable> backgroundHover;
			ScaleMode backgroundScaleMode;
			Alignment backgroundAlignment;
			
			AtomicRef<Pen> penBorder;
			PenStyle borderStyle;
			sl_real borderWidth;
			Color borderColor;
			
			BoundShape boundShape;
			Size roundRectBoundShapeRadius;
			AtomicRef<GraphicsPath> boundShapePath;
			
			sl_bool flagPreDrawEnabled;
			sl_bool flagPostDrawEnabled;
			sl_bool flagOnDrawBackgroundAlways;
			sl_bool flagOnDrawBorderAlways;
			
			AtomicRef<Font> font;
			sl_bool flagUsingFont;
			
			sl_bool flagOpaque;
			sl_real alpha;
			
			sl_bool flagLayer;
			AtomicRef<Bitmap> bitmapLayer;
			AtomicRef<Canvas> canvasLayer;
			sl_bool flagInvalidatedLayer;
			sl_bool flagInvalidatedWholeLayer;
			UIRect rectInvalidatedLayer;
			
			sl_bool flagForcedDraw;

			LinkedList< Function<void()> > runAfterDrawCallbacks;
			
		public:
			DrawAttributes();
			
			~DrawAttributes();
			
		};
		Ref<DrawAttributes> m_drawAttrs;
		
		void _initializeDrawAttributes();

		class ScrollAttributes : public Referable
		{
		public:
			sl_bool flagHorz;
			sl_bool flagVert;
			
			AtomicRef<ScrollBar> horz;
			AtomicRef<ScrollBar> vert;
			sl_bool flagValidHorz;
			sl_bool flagValidVert;
			sl_scroll_pos x;
			sl_scroll_pos y;
			sl_scroll_pos contentWidth;
			sl_scroll_pos contentHeight;
			sl_ui_len barWidth;
			sl_bool flagPaging;
			sl_ui_pos pageWidth;
			sl_ui_pos pageHeight;
			
			sl_bool flagContentScrollingByMouse;
			sl_bool flagContentScrollingByTouch;
			sl_bool flagContentScrollingByMouseWheel;
			sl_bool flagContentScrollingByKeyboard;
			sl_bool flagSmoothContentScrolling;
			
			sl_bool flagDownContent;
			Point mousePointBefore;
			Ref<MotionTracker> motionTracker;
			Ref<Timer> timerFlow;
			Time timeFlowFrameBefore;
			Point speedFlow;
			sl_bool flagSmoothTarget;
			sl_scroll_pos xSmoothTarget;
			sl_scroll_pos ySmoothTarget;
			
			sl_bool flagHorzScrollBarVisible;
			sl_bool flagVertScrollBarVisible;
			sl_bool flagInitHorzScrollBar;
			sl_bool flagInitVertScrollBar;
			
		public:
			ScrollAttributes();
			
			~ScrollAttributes();
			
		};
		Ref<ScrollAttributes> m_scrollAttrs;
		
		void _initializeScrollAttributes();
		
		class EventAttributes : public Referable
		{
		public:
			AtomicPtr<IViewListener> listener;
			AtomicFunction<void(View*)> attach;
			AtomicFunction<void(View*)> detach;
			AtomicFunction<void(View*, Canvas*)> draw;
			AtomicFunction<void(View*, Canvas*)> preDraw;
			AtomicFunction<void(View*, Canvas*)> postDraw;
			AtomicFunction<void(View*, UIEvent*)> mouse;
			AtomicFunction<void(View*, UIEvent*)> touch;
			AtomicFunction<void(View*, UIEvent*)> key;
			AtomicFunction<void(View*, UIEvent*)> mouseWheel;
			AtomicFunction<void(View*)> click;
			AtomicFunction<void(View*, UIEvent*)> setCursor;
			AtomicFunction<void(View*, sl_ui_len, sl_ui_len)> resize;
			AtomicFunction<void(View*, sl_scroll_pos, sl_scroll_pos)> scroll;
			AtomicFunction<void(View*, GestureEvent*)> swipe;
			AtomicFunction<void(View*, UIEvent*)> ok;
			AtomicFunction<void(View*, UIEvent*)> cancel;
			
		public:
			EventAttributes();
			
			~EventAttributes();
			
		};
		
		Ref<EventAttributes> m_eventAttrs;
		
		void _initializeEventAttributes();

		friend class ListView;

	};

	class SLIB_EXPORT ViewInstance : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		ViewInstance();
		
		~ViewInstance();
		
	public:
		Ref<View> getView();
		
		void setView(View* view);
		
		sl_bool isNativeWidget();
		
		void setNativeWidget(sl_bool flag);

		sl_bool isWindowContent();

		void setWindowContent(sl_bool flag);

	public:
		virtual sl_bool isValid() = 0;
		
		virtual void setFocus() = 0;
		
		virtual void invalidate() = 0;

		virtual void invalidate(const UIRect& rect) = 0;
		
		virtual UIRect getFrame() = 0;
		
		virtual void setFrame(const UIRect& frame) = 0;
		
		virtual void setTransform(const Matrix3& transform) = 0;
		
		virtual void setVisible(sl_bool flag) = 0;
		
		virtual void setEnabled(sl_bool flag) = 0;
		
		virtual void setOpaque(sl_bool flag) = 0;
		
		virtual void setAlpha(sl_real alpha) = 0;
		
		virtual void setClipping(sl_bool flag) = 0;
		
		virtual void setDrawing(sl_bool flag) = 0;

		virtual UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen) = 0;
		
		virtual UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView) = 0;
		
		virtual void addChildInstance(const Ref<ViewInstance>& instance) = 0;
		
		virtual void removeChildInstance(const Ref<ViewInstance>& instance) = 0;
		
		virtual void bringToFront() = 0;
		
	public:
		void onDraw(Canvas* canvas);
		
		void onClick();
		
		void onKeyEvent(UIEvent* event);
		
		void onMouseEvent(UIEvent* event);
		
		void onTouchEvent(UIEvent* event);
		
		void onMouseWheelEvent(UIEvent* event);
		
		void onSetCursor(UIEvent* event);
		
		void onSwipe(GestureType ev);
		
	protected:
		AtomicWeakRef<View> m_view;
		sl_bool m_flagNativeWidget;
		sl_bool m_flagWindowContent;

	};

	class ViewGroup : public View
	{
		SLIB_DECLARE_OBJECT
	public:
		ViewGroup();
		
		~ViewGroup();
		
	};

}

#endif
