#ifndef CHECKHEADER_SLIB_UI_VIEW
#define CHECKHEADER_SLIB_UI_VIEW

#include "definition.h"

#include "constants.h"
#include "event.h"
#include "cursor.h"

#include "../core/object.h"
#include "../core/ptr.h"
#include "../core/function.h"
#include "../core/queue.h"
#include "../core/animation.h"
#include "../graphics/canvas.h"
#include "../graphics/bitmap.h"

SLIB_UI_NAMESPACE_BEGIN

class Window;
class ViewInstance;
class ScrollBar;

struct ViewPrepareLayoutParam;

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
	
	void addChild(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);

	void insertChild(sl_size index, const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);

	void removeChild(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);

	void removeChild(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void removeAllChildren(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<View> getChildAt(sl_ui_pos x, sl_ui_pos y);
	
	Ref<View> getChildAt(const UIPoint& point);
	
	Ref<View> getChildById(const String& _id);
	
	Ref<View> getRootView();
	
	sl_bool isRootView();
	
	Ref<View> getInstanceView();
	
	Ref<ViewInstance> getInstanceViewInstance();
	
	void removeFromParent();
	
	void attachChild(const Ref<View>& child);
	
	void addChildInstance(const Ref<ViewInstance>& instance);
	
	void removeChildInstance(const Ref<ViewInstance>& instance);
	
	sl_bool isOnAddChildEnabled();
	
	void setOnAddChildEnabled(sl_bool flagEnabled);
	
	sl_bool isOnRemoveChildEnabled();
	
	void setOnRemoveChildEnabled(sl_bool flagEnabled);
	
	void bringToFront(UIUpdateMode mode = UIUpdateMode::Redraw);

	
	virtual void invalidate();

	// local coordinate
	virtual void invalidate(const UIRect& rect);
	
	void invalidateBoundsInParent();
	
	void updateAndInvalidateBoundsInParent(UIUpdateMode mode = UIUpdateMode::Redraw);

	sl_bool checkSelfInvalidatable();
	
	
	// parent coordinate
	const UIRect& getFrame();
	
	// parent coordinate
	UIRect getInstanceFrame();
	
	// parent coordinate
	void setFrame(const UIRect& frame, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	// parent coordinate
	void setFrame(sl_ui_pos x, sl_ui_pos y, sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_len getWidth();
	
	void setWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_len getHeight();
	
	void setHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	UISize getSize();
	
	void setSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);

	// parent coordinate
	sl_ui_pos getLeft();
	
	// parent coordinate
	sl_ui_pos getTop();
	
	// parent coordinate
	void setLeft(sl_ui_pos x, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	// parent coordinate
	void setTop(sl_ui_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);

	// parent coordinate
	UIPoint getPosition();
	
	// parent coordinate
	void setPosition(sl_ui_pos x, sl_ui_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	// parent coordinate
	void setPosition(const UIPoint& point, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	// local coordinate
	UIRect getBounds();
	
	// local coordinate
	UIRect getBoundsInnerPadding();
	
	// parent coordinate
	UIRect getBoundsInParent();
	
	// parent coordinate
	void requestFrame(const UIRect& frame, UIUpdateMode mode = UIUpdateMode::Redraw);

	
	Visibility getVisibility();
	
	void setVisibility(Visibility visibility, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isVisible();
	
	void setVisible(sl_bool flagVisible, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isEnabled();
	
	virtual void setEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	
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
	
	sl_bool isOccurringClick();
	
	void setOccurringClick(sl_bool flag);
	
	
	Ref<Cursor> getCursor();
	
	void setCursor(const Ref<Cursor>& cursor);

	
	void measureLayout();
	
	sl_ui_len getMeasuredWidth();
	
	void setMeasuredWidth(sl_ui_len width);
	
	sl_ui_len getMeasuredHeight();
	
	void setMeasuredHeight(sl_ui_len height);

	void requestLayout(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void requestParentLayout(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void requestParentAndSelfLayout(UIUpdateMode mode = UIUpdateMode::Redraw);

	
	sl_bool isLayouting();
	
	void resetLayout(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isMakingLayout();
	
	void setMakingLayout(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	UIRect getLayoutFrame();
	
	void setLayoutFrame(const UIRect& rect);
	
	sl_bool isLayoutFrameUpdated();
	
	void setLayoutFrameUpdated(sl_bool flag);
	
	sl_bool isOnPrepareLayoutEnabled();
	
	void setOnPrepareLayoutEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isOnMakeLayoutEnabled();
	
	void setOnMakeLayoutEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	SizeMode getWidthMode();
	
	SizeMode getHeightMode();
	
	sl_bool isWidthFixed();
	
	void setWidthFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isHeightFixed();
	
	void setHeightFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setSizeFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_real getWidthWeight();
	
	sl_real getHeightWeight();
	
	sl_bool isWidthFilling();
	
	void setWidthFilling(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isHeightFilling();
	
	void setHeightFilling(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setSizeFilling(sl_real widthWeight = 1, sl_real heightWeight = 1, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isWidthWrapping();
	
	void setWidthWrapping(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isHeightWrapping();
	
	void setHeightWrapping(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setSizeWrapping(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isWidthWeight();
	
	void setWidthWeight(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isHeightWeight();
	
	void setHeightWeight(sl_real weight = 1, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setSizeWeight(sl_real widthWeight = 1, sl_real heightWeight = 1, UIUpdateMode mode = UIUpdateMode::Redraw);

	sl_bool isLayoutLeftFixed();
	
	void setLayoutLeftFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignParentLeft();
	
	void setAlignParentLeft(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignLeft();
	
	void setAlignLeft(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isRightOf();
	
	void setRightOf(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<View> getLayoutLeftReferingView();
	
	sl_bool isLayoutRightFixed();
	
	void setLayoutRightFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignParentRight();
	
	void setAlignParentRight(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignRight();
	
	void setAlignRight(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isLeftOf();
	
	void setLeftOf(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<View> getLayoutRightReferingView();

	sl_bool isLayoutTopFixed();
	
	void setLayoutTopFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignParentTop();
	
	void setAlignParentTop(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignTop();
	
	void setAlignTop(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isBelow();
	
	void setBelow(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<View> getLayoutTopReferingView();
	
	sl_bool isLayoutBottomFixed();
	
	void setLayoutBottomFixed(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignParentBottom();
	
	void setAlignParentBottom(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignBottom();
	
	void setAlignBottom(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAbove();
	
	void setAbove(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);

	Ref<View> getLayoutBottomReferingView();
	
	sl_bool isCenterHorizontal();
	
	void setCenterHorizontal(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isCenterVertical();
	
	void setCenterVertical(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setCenterInParent(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignCenterHorizontal();
	
	void setAlignCenterHorizontal(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isAlignCenterVertical();
	
	void setAlignCenterVertical(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getMarginLeft();
	
	void setMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getMarginTop();
	
	void setMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getMarginRight();
	
	void setMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getMarginBottom();
	
	void setMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setMargin(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isRelativeMarginLeft();
	
	sl_real getRelativeMarginLeftWeight();
	
	void setRelativeMarginLeft(sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getAbsoluteMarginLeft();
	
	void setAbsoluteMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);

	sl_bool isRelativeMarginTop();
	
	sl_real getRelativeMarginTopWeight();
	
	void setRelativeMarginTop(sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getAbsoluteMarginTop();

	void setAbsoluteMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isRelativeMarginRight();

	sl_real getRelativeMarginRightWeight();
	
	void setRelativeMarginRight(sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getAbsoluteMarginRight();

	void setAbsoluteMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);

	sl_bool isRelativeMarginBottom();
	
	sl_real getRelativeMarginBottomWeight();
	
	void setRelativeMarginBottom(sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getAbsoluteMarginBottom();

	void setAbsoluteMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void applyRelativeMargins(sl_ui_len parentWidth, sl_ui_len parentHeight);
	
	
	sl_ui_pos getPaddingLeft();
	
	void setPaddingLeft(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getPaddingTop();
	
	void setPaddingTop(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getPaddingRight();
	
	void setPaddingRight(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_ui_pos getPaddingBottom();
	
	void setPaddingBottom(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setPadding(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	
	sl_bool getFinalTransform(Matrix3* _out);
	
	sl_bool getFinalInverseTransform(Matrix3* _out);
	
	const Matrix3& getTransform();
	
	void setTransform(const Matrix3& matrix, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void resetTransform(UIUpdateMode mode = UIUpdateMode::Redraw);

	void setTransformFromAnimation(const Matrix3& matrix, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void resetTransformFromAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool getFinalTranslationRotationScale(Vector2* translation = sl_null, sl_real* rotation = sl_null, Vector2* scale = sl_null, Vector2* anchor = sl_null);
	
	sl_real getTranslationX();
	
	sl_real getTranslationY();
	
	const Vector2& getTranslation();
	
	void setTranslationX(sl_real x, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTranslationY(sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTranslation(sl_real x, sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTranslation(const Vector2& t, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTranslationFromAnimation(const Vector2& t, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void resetTranslationFromAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_real getScaleX();
	
	sl_real getScaleY();
	
	const Vector2& getScale();
	
	void setScaleX(sl_real sx, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScaleY(sl_real sy, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScale(sl_real x, sl_real y, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScale(sl_real factor, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScale(const Vector2& factor, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScaleFromAnimation(const Vector2& factor, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void resetScaleFromAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_real getRotation();
	
	void setRotation(sl_real radian, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setRotationFromAnimation(sl_real radian, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void resetRotationFromAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
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
	
	Ref<Drawable> getPressedBackground();
	
	virtual void setPressedBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Drawable> getHoverBackground();
	
	virtual void setHoverBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	ScaleMode getBackgroundScaleMode();
	
	virtual void setBackgroundScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
	
	Alignment getBackgroundAlignment();
	
	virtual void setBackgroundAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);

	Color getBackgroundColor();
	
	virtual void setBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
	
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
	
	virtual void setFont(const Ref<Font>& font, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setFont(const String& fontFamily, sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setFontAttributes(sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false, UIUpdateMode mode = UIUpdateMode::Redraw);

	sl_bool isUsingFont();
	
	void setUsingFont(sl_bool flag);
	
	
	sl_bool isOpaque();
	
	void setOpaque(sl_bool flagOpaque, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_real getFinalAlpha();
	
	sl_real getAlpha();
	
	void setAlpha(sl_real alpha, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setAlphaFromAnimation(sl_real alpha, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void resetAlphaFromAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);

	sl_bool isLayer();
	
	void setLayer(sl_bool flagLayer, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void invalidateLayer();
	
	void invalidateLayer(const UIRect& rect);
	
	
	void detachAnimations();

	void resetAnimations(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> getTransformAnimation();
	
	void setTransformAnimation(const Ref<Animation>& animation, const AnimationFrames<Matrix3>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTransformAnimation(const Ref<Animation>& animation, const Matrix3& startValue, const Matrix3& endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startTransformAnimation(const AnimationFrames<Matrix3>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startTransformAnimation(const Matrix3& startValue, const Matrix3& endValue, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	void resetTransformAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> getTranslateAnimation();
	
	void setTranslateAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTranslateAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startTranslateAnimation(const AnimationFrames<Vector2>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startTranslateAnimation(const Vector2& startValue, const Vector2& endValue, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	void resetTranslateAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> getScaleAnimation();
	
	void setScaleAnimation(const Ref<Animation>& animation, const AnimationFrames<Vector2>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScaleAnimation(const Ref<Animation>& animation, const Vector2& startValue, const Vector2& endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setScaleAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startScaleAnimation(const AnimationFrames<Vector2>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startScaleAnimation(const Vector2& startValue, const Vector2& endValue, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startScaleAnimation(sl_real startValue, sl_real endValue, float duration, AnimationCurve curve = AnimationCurve::Default);

	void resetScaleAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> getRotateAnimation();
	
	void setRotateAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setRotateAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startRotateAnimation(const AnimationFrames<sl_real>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startRotateAnimation(sl_real startValue, sl_real endValue, float duration, AnimationCurve curve = AnimationCurve::Default);

	void resetRotateAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> getFrameAnimation();
	
	void setFrameAnimation(const Ref<Animation>& animation, const AnimationFrames<Rectangle>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setFrameAnimation(const Ref<Animation>& animation, const Rectangle& startValue, const Rectangle& endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startFrameAnimation(const AnimationFrames<Rectangle>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startFrameAnimation(const Rectangle& startValue, const Rectangle& endValue, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	void resetFrameAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);

	Ref<Animation> getAlphaAnimation();
	
	void setAlphaAnimation(const Ref<Animation>& animation, const AnimationFrames<sl_real>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setAlphaAnimation(const Ref<Animation>& animation, sl_real startValue, sl_real endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startAlphaAnimation(const AnimationFrames<sl_real>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startAlphaAnimation(sl_real startValue, sl_real endValue, float duration, AnimationCurve curve = AnimationCurve::Default);

	void resetAlphaAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> getBackgroundColorAnimation();
	
	void setBackgroundColorAnimation(const Ref<Animation>& animation, const AnimationFrames<Color4f>& frames, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setBackgroundColorAnimation(const Ref<Animation>& animation, const Color4f& startValue, const Color4f& endValue, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Animation> startBackgroundColorAnimation(const AnimationFrames<Color4f>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startBackgroundColorAnimation(const Color4f& startValue, const Color4f& endValue, float duration, AnimationCurve curve = AnimationCurve::Default);

	void resetBackgroundColorAnimation(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	
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
	
	void setScrollBarsVisible(sl_bool flagVisible, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getScrollX();
	
	sl_scroll_pos getScrollY();
	
	virtual Pointlf getScrollPosition();
	
	virtual void scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void scrollTo(const Pointlf& position, UIUpdateMode mode = UIUpdateMode::Redraw);
	
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
	
	
	sl_bool isContentScrollingByMouse();
	
	void setContentScrollingByMouse(sl_bool flag);
	
	sl_bool isContentScrollingByTouch();
	
	void setContentScrollingByTouch(sl_bool flag);
	
	sl_bool isContentScrollingByMouseWheel();
	
	void setContentScrollingByMouseWheel(sl_bool flag);
	
	sl_bool isContentScrollingByKeyboard();
	
	void setContentScrollingByKeyboard(sl_bool flag);

	
	sl_bool isMultiTouchMode();
	
	void setMultiTouchMode(sl_bool flag);
	
	sl_bool isPassingEventsToChildren();
	
	void setPassingEventsToChildren(sl_bool flag);
	
	
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
	
	
	void drawBackground(Canvas* canvas, const Color& color, const Ref<Drawable>& background);
	
	void drawBorder(Canvas* canvas, const Ref<Pen>& pen);
	
	void drawChildren(Canvas* canvas, const Ref<View>* children, sl_size count);
	
	void drawContent(Canvas* canvas);
	
	Ref<Bitmap> drawLayer();
	
	void draw(Canvas* canvas);
	
	void post(const Function<void()>& callback, sl_bool flagInvalidate = sl_true);

public:
	SLIB_PTR_PROPERTY(IViewListener, EventListener)
	SLIB_CALLBACK_PROPERTY(OnClick)
	
protected:
	virtual void onDraw(Canvas* canvas);
	
	virtual void onPreDraw(Canvas* canvas);
	
	virtual void onPostDraw(Canvas* canvas);
	
	virtual void onDrawChildren(Canvas* canvas);
	
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
	
	virtual void onAttach();
	
	virtual void onAddChild(View* child);
	
	virtual void onRemoveChild(View* child);
	
	virtual void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
	
	virtual void onPrepareLayout(ViewPrepareLayoutParam& param);
	
	virtual void onMakeLayout();
	
	virtual void onChangePadding();
	
public:
	virtual void dispatchDraw(Canvas* canvas);
	
	virtual void dispatchMouseEvent(UIEvent* ev);
	
	sl_bool dispatchMouseEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
	
	void dispatchMouseEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
	
	virtual void dispatchTouchEvent(UIEvent* ev);
	
	sl_bool dispatchTouchEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
	
	void dispatchMultiTouchEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
	
	void dispatchTouchEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
	
	virtual void dispatchMouseWheelEvent(UIEvent* ev);
	
	sl_bool dispatchMouseWheelEventToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
	
	void dispatchMouseWheelEventToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
	
	virtual void dispatchKeyEvent(UIEvent* ev);
	
	virtual void dispatchClick(UIEvent* ev);
	
	void dispatchClickWithNoEvent();
	
	virtual void dispatchSetCursor(UIEvent* ev);
	
	sl_bool dispatchSetCursorToChildren(UIEvent* ev, const Ref<View>* children, sl_size count);
	
	void dispatchSetCursorToChild(UIEvent* ev, View* child, sl_bool flagTransformPoints = sl_true);
	
	virtual void dispatchResize(sl_ui_len width, sl_ui_len height);
	
	virtual void dispatchChangeVisibility(Visibility oldVisibility, Visibility newVisibility);
	
	virtual void dispatchScroll(sl_scroll_pos x, sl_scroll_pos y);
	
private:
	void _processAttachOnUiThread();
	
	void _addChild(const Ref<View>& view, UIUpdateMode mode);
	
	void _removeChild(const Ref<View>& view);
	
	void _killFocusFromParent();
	
	void _setFocusedChild(View* child, UIUpdateMode mode);
	
	void _setFrame(const UIRect& frame, UIUpdateMode mode, sl_bool flagLayouting);
	
	void _prepareLayout(ViewPrepareLayoutParam& param);
	
	void _makeLayout(sl_bool flagApplyLayout);
	
	void _measureRelativeBoundWidth();
	
	void _measureRelativeBoundHeight();
	
	void _requestMakeLayout();
	
	void _requestInvalidateLayout(UIUpdateMode mode);
	
	void _requestInvalidateMeasure(sl_bool flagWidth, sl_bool flagHeight, UIUpdateMode mode);
	
	void _applyCalcTransform(UIUpdateMode mode);
	
	void _applyFinalTransform(UIUpdateMode mode);
	
	void _invalidateInstanceTransform();
	
	void _refreshBorderPen(UIUpdateMode mode);
	
	void _setFontInvalidateChildInstances();

	void _applyFinalAlpha(UIUpdateMode mode);
	

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
	
	void _processEventForStateAndClick(UIEvent* ev);
	
	void _processContentScrollingEvents(UIEvent* ev);
	
protected:
	void measureRelativeLayout(sl_bool flagHorizontal, sl_bool flagVertical);
	
	CList< Ref<View> >& _getChildren();

public:
	void _setFrame_NI(const UIRect& frame);
	
	void _setTransform_NI(const Matrix3& transform);
	
	virtual void _setBorder_NW(sl_bool flag);

	virtual void _setBackgroundColor_NW(const Color& color);

	virtual void _setFont_NW(const Ref<Font>& font);

private:
	SafeRef<ViewInstance> m_instance;
	SafeWeakRef<Window> m_window;
	SafeWeakRef<View> m_parent;
	
	sl_bool m_flagCreatingInstance;
	sl_bool m_flagCreatingChildInstances;
	sl_bool m_flagCreatingNativeWidget;
	UIAttachMode m_attachMode;
	
	UIRect m_frame;
	UIRect m_boundsInParent;
	
	Visibility m_visibility;
	sl_bool m_flagEnabled;
	sl_bool m_flagHitTestable;
	sl_bool m_flagFocusable;

	sl_bool m_flagFocused;
	sl_bool m_flagPressed;
	sl_bool m_flagHover;
	sl_bool m_flagOccurringClick;
	
	sl_bool m_flagCurrentDrawing;
	UIRect m_rectCurrentDrawing;
	
	SafeRef<Cursor> m_cursor;
	
	SafeString m_id;
	CList< Ref<View> > m_children;
	SafeList< Ref<View> > m_childrenMultiTouch;
	SafeRef<View> m_childMouseMove;
	SafeRef<View> m_childMouseDown;
	UIAction m_actionMouseDown;
	sl_bool m_flagMultiTouchMode;
	sl_bool m_flagPassEventToChildren;
	SafeRef<View> m_childFocused;
	
	sl_bool m_flagOnAddChild;
	sl_bool m_flagOnRemoveChild;
	
	sl_bool m_flagProcessingTabStop;
	SafeWeakRef<View> m_viewNextTabStop;
	SafeWeakRef<View> m_viewPrevTabStop;
	sl_bool m_flagCapturingChildInstanceEvents;
	
	sl_ui_pos m_paddingLeft;
	sl_ui_pos m_paddingTop;
	sl_ui_pos m_paddingRight;
	sl_ui_pos m_paddingBottom;
	
protected:
	class LayoutAttributes : public Referable
	{
	public:
		SizeMode widthMode;
		SizeMode heightMode;
		sl_real widthWeight;
		sl_real heightWeight;
		
		PositionMode leftMode;
		PositionMode topMode;
		PositionMode rightMode;
		PositionMode bottomMode;
		SafeWeakRef<View> leftReferingView;
		SafeWeakRef<View> topReferingView;
		SafeWeakRef<View> rightReferingView;
		SafeWeakRef<View> bottomReferingView;
		
		sl_ui_len measuredWidth;
		sl_ui_len measuredHeight;
		UIRect frame;
		UIRect requestedFrame;
		sl_bool flagInvalidMeasure;
		sl_bool flagInvalidLayout;
		sl_bool flagMakeLayout;

		sl_ui_len measuredRelativeBoundWidth;
		sl_bool flagInvalidRelativeBoundWidth;
		sl_bool flagBadRelativeBoundWidth;
		sl_ui_len measuredRelativeBoundHeight;
		sl_bool flagInvalidRelativeBoundHeight;
		sl_bool flagBadRelativeBoundHeight;

		sl_bool flagOnPrepareLayout;
		sl_bool flagOnMakeLayout;
		
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
		
		sl_bool flagRecursiveMakeLayout;
		sl_bool flagUpdatedLayoutFrame;
		
	};
	SafeRef<LayoutAttributes> m_layoutAttributes;
	
	Ref<LayoutAttributes> _initializeLayoutAttibutes();

	class TransformAttributes : public Referable
	{
	public:
		sl_bool flagTransformFinalInvalid;
		sl_bool flagTransformFinal;
		Matrix3 transformFinal;
		
		sl_bool flagInverseTransformFinalInvalid;
		sl_bool flagInverseTransformFinal;
		Matrix3 inverseTransformFinal;
		
		sl_bool flagTransformStatic;
		Matrix3 transformStatic;
		sl_bool flagTransformAnimation;
		Matrix3 transformAnimation;
		
		sl_bool flagTransformCalcInvalid;
		sl_bool flagTransformCalc;
		Matrix3 transformCalc;
		
		Vector2 translationStatic;
		Vector2 translationAnimation;
		
		Vector2 scaleStatic;
		Vector2 scaleAnimation;
		
		sl_real rotationAngleStatic;
		sl_real rotationAngleAnimation;
		
		Vector2 anchorOffset;
		
	};
	SafeRef<TransformAttributes> m_transformAttributes;
	
	Ref<TransformAttributes> _initializeTransformAttributes();
	
	class DrawAttributes : public Referable
	{
	public:
		SafeRef<Drawable> background;
		SafeRef<Drawable> backgroundPressed;
		SafeRef<Drawable> backgroundHover;
		ScaleMode backgroundScaleMode;
		Alignment backgroundAlignment;
		Color backgroundColor;
		
		SafeRef<Pen> penBorder;
		PenStyle borderStyle;
		sl_real borderWidth;
		Color borderColor;
		
		BoundShape boundShape;
		Size roundRectBoundShapeRadius;
		SafeRef<GraphicsPath> boundShapePath;
		
		sl_bool flagPreDrawEnabled;
		sl_bool flagPostDrawEnabled;
		sl_bool flagOnDrawBackgroundAlways;
		sl_bool flagOnDrawBorderAlways;
		
		SafeRef<Font> font;
		sl_bool flagUsingFont;
		
		sl_bool flagOpaque;
		sl_real alphaStatic;
		sl_real alphaAnimation;
		
		sl_bool flagLayer;
		SafeRef<Bitmap> bitmapLayer;
		SafeRef<Canvas> canvasLayer;
		sl_bool flagInvalidatedLayer;
		sl_bool flagInvalidatedWholeLayer;
		UIRect rectInvalidatedLayer;
		
	};
	SafeRef<DrawAttributes> m_drawAttributes;
	
	Ref<DrawAttributes> _initializeDrawAttributes();

	class AnimationAttributes : public Referable
	{
	public:
		SafeRef<Animation> animationTransform;
		SafeRef< AnimationTargetT<Matrix3> > targetTransform;
		
		SafeRef<Animation> animationTranslation;
		SafeRef< AnimationTargetT<Vector2> > targetTranslation;
		
		SafeRef<Animation> animationScale;
		SafeRef< AnimationTargetT<Vector2> > targetScale;
		
		SafeRef<Animation> animationRotation;
		SafeRef< AnimationTargetT<sl_real> > targetRotation;
		
		SafeRef<Animation> animationFrame;
		SafeRef< AnimationTargetT<Rectangle> > targetFrame;
		
		SafeRef<Animation> animationAlpha;
		SafeRef< AnimationTargetT<sl_real> > targetAlpha;
		
		SafeRef<Animation> animationBackgroundColor;
		SafeRef< AnimationTargetT<Color4f> > targetBackgroundColor;
		
	};
	SafeRef<AnimationAttributes> m_animationAttributes;
	
	Ref<AnimationAttributes> _initializeAnimationAttributes();
	
	class ScrollAttributes : public Referable
	{
	public:
		SafeRef<ScrollBar> horz;
		SafeRef<ScrollBar> vert;
		sl_bool flagValidHorz;
		sl_bool flagValidVert;
		sl_scroll_pos x;
		sl_scroll_pos y;
		sl_scroll_pos contentWidth;
		sl_scroll_pos contentHeight;
		sl_ui_len barWidth;
		
		sl_bool flagContentScrollingByMouse;
		sl_bool flagContentScrollingByTouch;
		sl_bool flagContentScrollingByMouseWheel;
		sl_bool flagContentScrollingByKeyboard;
		sl_bool flagDownContent;
		sl_scroll_pos scrollX_DownContent;
		sl_scroll_pos scrollY_DownContent;
		sl_ui_posf mouseX_DownContent;
		sl_ui_posf mouseY_DownContent;
	};
	SafeRef<ScrollAttributes> m_scrollAttributes;
	
	Ref<ScrollAttributes> _initializeScrollAttributes();
	
	Queue< Function<void()> > m_postCallbacks;

	friend class ListView;

};

struct ViewPrepareLayoutParam
{
	UIRect parentContentFrame;
	sl_bool flagUseLayoutFrame;
};

class SLIB_EXPORT ViewInstance : public Object
{
	SLIB_DECLARE_OBJECT

public:
	ViewInstance();
	
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
	
protected:
	SafeWeakRef<View> m_view;
	sl_bool m_flagNativeWidget;
	sl_bool m_flagWindowContent;

};

class ViewGroup : public View
{
	SLIB_DECLARE_OBJECT
public:
	ViewGroup();
	
};

SLIB_UI_NAMESPACE_END

#endif
