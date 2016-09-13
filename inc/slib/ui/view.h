#ifndef CHECKHEADER_SLIB_UI_VIEW
#define CHECKHEADER_SLIB_UI_VIEW

#include "definition.h"

#include "constants.h"
#include "event.h"
#include "cursor.h"

#include "../core/object.h"
#include "../core/pointer.h"
#include "../core/callback.h"
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
	
	Ref<GraphicsContext> getGraphicsContext();
	
	void setGraphicsContext(const Ref<GraphicsContext>& context);
	
	
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
	
	void addChild(const Ref<View>& view, sl_bool flagRedraw = sl_true);

	void insertChild(sl_size index, const Ref<View>& view, sl_bool flagRedraw = sl_true);

	void removeChild(sl_size index, sl_bool flagRedraw = sl_true);

	void removeChild(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	void removeAllChildren(sl_bool flagRedraw = sl_true);
	
	Ref<View> getChildAt(sl_ui_pos x, sl_ui_pos y);
	
	Ref<View> getChildAt(const UIPoint& point);
	
	Ref<View> getChildById(const String& _id);
	
	Ref<View> getRootView();
	
	sl_bool isRootView();
	
	void removeFromParent();
	
	void attachChild(const Ref<View>& child);
	
	void addChildInstance(const Ref<ViewInstance>& instance);
	
	void removeChildInstance(const Ref<ViewInstance>& instance);
	
	sl_bool isOnAddChildEnabled();
	
	void setOnAddChildEnabled(sl_bool flagEnabled);
	
	sl_bool isOnRemoveChildEnabled();
	
	void setOnRemoveChildEnabled(sl_bool flagEnabled);

	
	void invalidate();

	// local coordinate
	void invalidate(const UIRect& rect);
	
	
	// parent coordinate
	const UIRect& getFrame();
	
	// parent coordinate
	UIRect getInstanceFrame();
	
	// parent coordinate
	void setFrame(const UIRect& frame, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	void setFrame(sl_ui_pos x, sl_ui_pos y, sl_ui_len width, sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getWidth();
	
	void setWidth(sl_ui_len width, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getHeight();
	
	void setHeight(sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	UISize getSize();
	
	void setSize(const UISize& size, sl_bool flagRedraw = sl_true);
	
	void setSize(sl_ui_len width, sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	sl_ui_pos getLeft();
	
	// parent coordinate
	sl_ui_pos getTop();
	
	// parent coordinate
	void setLeft(sl_ui_pos x, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	void setTop(sl_ui_pos y, sl_bool flagRedraw = sl_true);

	// parent coordinate
	UIPoint getPosition();
	
	// parent coordinate
	void setPosition(sl_ui_pos x, sl_ui_pos y, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	void setPosition(const UIPoint& point, sl_bool flagRedraw = sl_true);
	
	// local coordinate
	UIRect getBounds();
	
	// local coordinate
	UIRect getBoundsInnerPadding();
	
	// parent coordinate
	UIRect getBoundsInParent();
	
	// parent coordinate
	UIRect getBoundsInParent(const UIRect& boundsLocal);
	
	
	Visibility getVisibility();
	
	void setVisibility(Visibility visibility, sl_bool flagRedraw = sl_true);
	
	sl_bool isVisible();
	
	void setVisible(sl_bool flagVisible, sl_bool flagRedraw = sl_true);
	
	sl_bool isEnabled();
	
	void setEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isOpaque();
	
	void setOpaque(sl_bool flagOpaque, sl_bool flagRedraw = sl_true);
	
	
	sl_bool isHitTestable();
	
	void setHitTestable(sl_bool flag);
	
	// local coordinate
	virtual sl_bool hitTest(sl_ui_pos x, sl_ui_pos y);
	
	// local coordinate
	sl_bool hitTest(const UIPoint& point);

	
	sl_bool isFocusable();
	
	void setFocusable(sl_bool flagFocusable);
	
	sl_bool isFocused();
	
	void setFocus(sl_bool flagFocused = sl_true, sl_bool flagRedraw = sl_true);
	
	Ref<View> getFocusedChild();
	
	
	sl_bool isDownState();
	
	virtual void setDownState(sl_bool flagState, sl_bool flagRedraw = sl_true);
	
	sl_bool isHoverState();
	
	virtual void setHoverState(sl_bool flagState, sl_bool flagRedraw = sl_true);
	
	sl_bool isOccurringClick();
	
	void setOccurringClick(sl_bool flag);
	
	
	Ref<Cursor> getCursor();
	
	void setCursor(const Ref<Cursor>& cursor);

	
	void measureLayout();
	
	sl_ui_len getMeasuredWidth();
	
	void setMeasuredWidth(sl_ui_len width);
	
	sl_ui_len getMeasuredHeight();
	
	void setMeasuredHeight(sl_ui_len height);

	void requestLayout(sl_bool flagRedraw = sl_true);
	
	void requestParentLayout(sl_bool flagRedraw = sl_true);
	
	void requestParentAndSelfLayout(sl_bool flagRedraw = sl_true);
	
	sl_bool isLayoutEnabled();
	
	void setLayoutEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	UIRect getLayoutFrame();
	
	void setLayoutFrame(const UIRect& rect);
	
	sl_bool isLayoutFrameUpdated();
	
	void setLayoutFrameUpdated(sl_bool flag);
	
	sl_bool isOnPrepareLayoutEnabled();
	
	void setOnPrepareLayoutEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isOnMakeLayoutEnabled();
	
	void setOnMakeLayoutEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	SizeMode getWidthMode();
	
	SizeMode getHeightMode();
	
	sl_bool isWidthFixed();
	
	void setWidthFixed(sl_bool flagRedraw = sl_true);
	
	sl_bool isHeightFixed();
	
	void setHeightFixed(sl_bool flagRedraw = sl_true);
	
	void setSizeFixed(sl_bool flagRedraw = sl_true);
	
	sl_real getWidthWeight();
	
	sl_real getHeightWeight();
	
	sl_bool isWidthFilling();
	
	void setWidthFilling(sl_real weight = 1, sl_bool flagRedraw = sl_true);
	
	sl_bool isHeightFilling();
	
	void setHeightFilling(sl_real weight = 1, sl_bool flagRedraw = sl_true);
	
	void setSizeFilling(sl_real widthWeight = 1, sl_real heightWeight = 1, sl_bool flagRedraw = sl_true);
	
	sl_bool isWidthWrapping();
	
	void setWidthWrapping(sl_bool flagRedraw = sl_true);
	
	sl_bool isHeightWrapping();
	
	void setHeightWrapping(sl_bool flagRedraw = sl_true);
	
	void setSizeWrapping(sl_bool flagRedraw = sl_true);
	
	sl_bool isWidthWeight();
	
	void setWidthWeight(sl_real weight = 1, sl_bool flagRedraw = sl_true);
	
	sl_bool isHeightWeight();
	
	void setHeightWeight(sl_real weight = 1, sl_bool flagRedraw = sl_true);
	
	void setSizeWeight(sl_real widthWeight = 1, sl_real heightWeight = 1, sl_bool flagRedraw = sl_true);

	sl_bool isLayoutLeftFixed();
	
	void setLayoutLeftFixed(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignParentLeft();
	
	void setAlignParentLeft(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignLeft();
	
	void setAlignLeft(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_bool isRightOf();
	
	void setRightOf(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	Ref<View> getLayoutLeftReferingView();
	
	sl_bool isLayoutRightFixed();
	
	void setLayoutRightFixed(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignParentRight();
	
	void setAlignParentRight(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignRight();
	
	void setAlignRight(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_bool isLeftOf();
	
	void setLeftOf(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	Ref<View> getLayoutRightReferingView();

	sl_bool isLayoutTopFixed();
	
	void setLayoutTopFixed(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignParentTop();
	
	void setAlignParentTop(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignTop();
	
	void setAlignTop(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_bool isBelow();
	
	void setBelow(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	Ref<View> getLayoutTopReferingView();
	
	sl_bool isLayoutBottomFixed();
	
	void setLayoutBottomFixed(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignParentBottom();
	
	void setAlignParentBottom(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignBottom();
	
	void setAlignBottom(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_bool isAbove();
	
	void setAbove(const Ref<View>& view, sl_bool flagRedraw = sl_true);

	Ref<View> getLayoutBottomReferingView();
	
	sl_bool isCenterHorizontal();
	
	void setCenterHorizontal(sl_bool flagRedraw = sl_true);
	
	sl_bool isCenterVertical();
	
	void setCenterVertical(sl_bool flagRedraw = sl_true);
	
	void setCenterInParent(sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignCenterHorizontal();
	
	void setAlignCenterHorizontal(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_bool isAlignCenterVertical();
	
	void setAlignCenterVertical(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getMarginLeft();
	
	void setMarginLeft(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getMarginTop();
	
	void setMarginTop(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getMarginRight();
	
	void setMarginRight(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getMarginBottom();
	
	void setMarginBottom(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	void setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw = sl_true);
	
	void setMargin(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_bool isRelativeMarginLeft();
	
	sl_real getRelativeMarginLeftWeight();
	
	void setRelativeMarginLeft(sl_real weight, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getAbsoluteMarginLeft();
	
	void setAbsoluteMarginLeft(sl_ui_pos margin, sl_bool flagDraw = sl_true);

	sl_bool isRelativeMarginTop();
	
	sl_real getRelativeMarginTopWeight();
	
	void setRelativeMarginTop(sl_real weight, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getAbsoluteMarginTop();

	void setAbsoluteMarginTop(sl_ui_pos margin, sl_bool flagDraw = sl_true);
	
	sl_bool isRelativeMarginRight();

	sl_real getRelativeMarginRightWeight();
	
	void setRelativeMarginRight(sl_real weight, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getAbsoluteMarginRight();

	void setAbsoluteMarginRight(sl_ui_pos margin, sl_bool flagDraw = sl_true);

	sl_bool isRelativeMarginBottom();
	
	sl_real getRelativeMarginBottomWeight();
	
	void setRelativeMarginBottom(sl_real weight, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getAbsoluteMarginBottom();

	void setAbsoluteMarginBottom(sl_ui_pos margin, sl_bool flagDraw = sl_true);
	
	void applyRelativeMargins(sl_ui_len parentWidth, sl_ui_len parentHeight);
	
	sl_ui_pos getPaddingLeft();
	
	void setPaddingLeft(sl_ui_pos padding, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getPaddingTop();
	
	void setPaddingTop(sl_ui_pos padding, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getPaddingRight();
	
	void setPaddingRight(sl_ui_pos padding, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getPaddingBottom();
	
	void setPaddingBottom(sl_ui_pos padding, sl_bool flagRedraw = sl_true);
	
	void setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw = sl_true);
	
	void setPadding(sl_ui_pos padding, sl_bool flagRedraw = sl_true);
	
	
	sl_bool isTransformEnabled();
	
	void setTransformEnabled(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	const Matrix3& getTransform();
	
	void setTransform(const Matrix3& matrix, sl_bool flagRedraw = sl_true);
	
	sl_real getScaleX();
	
	sl_real getScaleY();
	
	Vector2 getScale();
	
	void setScaleX(sl_real sx, sl_bool flagSetTransform = sl_true);
	
	void setScaleY(sl_real sy, sl_bool flagSetTransform = sl_true);
	
	void setScale(sl_real x, sl_real y, sl_bool flagSetTransform = sl_true);
	
	void setScale(const Vector2& factor, sl_bool flagSetTransform = sl_true);
	
	void setScale(sl_real factor, sl_bool flagSetTransform = sl_true);
	
	sl_real getRotation();
	
	void setRatation(sl_real radian, sl_bool flagSetTransform = sl_true);
	
	Point getAnchorOffset();
	
	void setAnchorOffset(sl_real x, sl_real y, sl_bool flagSetTransform = sl_true);
	
	void setAnchorOffset(const Point& pt, sl_bool flagSetTransform = sl_true);
	
	sl_real getAnchorOffsetX();
	
	void setAnchorOffsetX(sl_real x, sl_bool flagSetTransform = sl_true);
	
	sl_real getAnchorOffsetY();
	
	void setAnchorOffsetY(sl_real y, sl_bool flagSetTransform = sl_true);
	

	UIPointf convertCoordinateFromScreen(const UIPointf& ptScreen);
	
	UIPointf convertCoordinateToScreen(const UIPointf& ptView);
	
	UIPointf convertCoordinateFromParent(const UIPointf& ptParent);
	
	UIPointf convertCoordinateToParent(const UIPointf& ptView);
	
	
	Ref<Drawable> getBackground();
	
	virtual void setBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	ScaleMode getBackgroundScaleMode();
	
	virtual void setBackgroundScaleMode(ScaleMode mode, sl_bool flagRedraw = sl_true);
	
	Alignment getBackgroundAlignment();
	
	virtual void setBackgroundAlignment(Alignment align, sl_bool flagRedraw = sl_true);

	Color getBackgroundColor();
	
	virtual void setBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Pen> getBorder();
	
	virtual void setBorder(const Ref<Pen>& pen, sl_bool flagRedraw = sl_true);
	
	PenStyle getBorderStyle();
	
	void setBorderStyle(PenStyle style, sl_bool flagRedraw = sl_true);
	
	sl_real getBorderWidth();
	
	void setBorderWidth(sl_real width, sl_bool flagRedraw = sl_true);
	
	Color getBorderColor();
	
	void setBorderColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	sl_bool isBorder();
	
	void setBorder(sl_bool flagBorder, sl_bool flagRedraw = sl_true);
	
	BoundShape getBoundShape();
	
	void setBoundShape(BoundShape shape, sl_bool flagRedraw = sl_true);
	
	const Size& getRoundRectBoundShapeRadius();
	
	void setRoundRectBoundShapeRadius(const Size& radius, sl_bool flagRedraw = sl_true);
	
	void setRoundRectBoundShapeRadius(sl_real rx, sl_real ry, sl_bool flagRedraw = sl_true);
	
	void setRoundRectBoundShapeRadius(sl_real radius, sl_bool flagRedraw = sl_true);
	
	Ref<GraphicsPath> getBoundShapePath();
	
	void setBoundShapePath(const Ref<GraphicsPath>& path, sl_bool flagRedraw = sl_true);

	
	sl_bool isPreDrawEnabled();
	
	void setPreDrawEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isPostDrawEnabled();
	
	void setPostDrawEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isClippingBounds();
	
	void setClippingBounds(sl_bool flag);
	
	sl_bool isDoubleBuffering();
	
	void setDoubleBuffering(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isAlwaysOnDrawBackground();
	
	void setAlwaysOnDrawBackground(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isAlwaysOnDrawBorder();
	
	void setAlwaysOnDrawBorder(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);

	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font, sl_bool flagRedraw = sl_true);
	
	void setFont(const String& fontFamily, sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false, sl_bool flagRedraw = sl_true);
	
	void setFontAttributes(sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false, sl_bool flagRedraw = sl_true);

	
	Ref<ScrollBar> getHorizontalScrollBar();
	
	Ref<ScrollBar> getVerticalScrollBar();
	
	void setHorizontalScrollBar(const Ref<ScrollBar>& bar, sl_bool flagRefresh = sl_true);
	
	void setVerticalScrollBar(const Ref<ScrollBar>& bar, sl_bool flagRefresh = sl_true);
	
	void createHorizontalScrollBar(sl_bool flagRefresh = sl_true);
	
	void createVerticalScrollBar(sl_bool flagRefresh = sl_true);
	
	void removeHorizontalScrollBar(sl_bool flagRefresh = sl_true);
	
	void removeVerticalScrollBar(sl_bool flagRefresh = sl_true);
	
	void createScrollBars(sl_bool flagRefresh = sl_true);
	
	void removeScrollBars(sl_bool flagRefresh = sl_true);
	
	sl_bool isHorizontalScrollBarVisible();
	
	sl_bool isVerticalScrollBarVisible();
	
	void setHorizontalScrollBarVisible(sl_bool flagVisible, sl_bool flagRefresh = sl_true);
	
	void setVerticalScrollBarVisible(sl_bool flagVisible, sl_bool flagRefresh = sl_true);
	
	void setScrollBarsVisible(sl_bool flagVisible, sl_bool flagRefresh = sl_true);
	
	sl_scroll_pos getScrollX();
	
	sl_scroll_pos getScrollY();
	
	virtual Pointlf getScrollPosition();
	
	virtual void scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagRedraw = sl_true);
	
	void scrollTo(const Pointlf& position, sl_bool flagRedraw = sl_true);
	
	void setScrollX(sl_scroll_pos x, sl_bool flagRedraw = sl_true);
	
	void setScrollY(sl_scroll_pos y, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getContentWidth();
	
	sl_scroll_pos getContentHeight();
	
	ScrollPoint getContentSize();
	
	virtual void setContentSize(sl_scroll_pos width, sl_scroll_pos height, sl_bool flagRefresh = sl_true);
	
	void setContentSize(const ScrollPoint& size, sl_bool flagRefresh = sl_true);
	
	void setContentWidth(sl_scroll_pos width, sl_bool flagRefresh = sl_true);
	
	void setContentHeight(sl_scroll_pos height, sl_bool flagRefresh = sl_true);
	
	virtual ScrollPoint getScrollRange();
	
	sl_ui_len getScrollBarWidth();
	
	void setScrollBarWidth(sl_ui_len width, sl_bool flagRefresh = sl_true);
	
	
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
	
	
	void draw(Canvas* canvas);
	
	void drawBackground(Canvas* canvas, const Color& color, const Ref<Drawable>& background);
	
	void drawBorder(Canvas* canvas, const Ref<Pen>& pen);
	
	void drawChildren(Canvas* canvas, const Ref<View>* children, sl_size count);
	
	void drawChild(Canvas* canvas, View* view);

public:
	SLIB_PTR_PROPERTY(IViewListener, EventListener)
	SLIB_REF_PROPERTY(Runnable, OnClick)
	
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
	
	void _addChild(const Ref<View>& view, sl_bool flagRedraw);
	
	void _removeChild(const Ref<View>& view);
	
	void _attachChildOnUiThread(Ref<View> child);
	
	void _addChildInstanceOnUiThread(Ref<ViewInstance> instance);
	
	void _removeChildInstanceOnUiThread(Ref<ViewInstance> instance);
	
	void _killFocusFromParent();
	
	void _setFocusedChild(View* child, sl_bool flagRedraw);
	
	void _setFrame(const UIRect& frame, sl_bool flagRedraw, sl_bool flagLayouting);
	
	void _prepareLayout(ViewPrepareLayoutParam& param);
	
	void _makeLayout(sl_bool flagApplyLayout);
	
	void _measureRelativeBoundWidth();
	
	void _measureRelativeBoundHeight();
	
	void _requestInvalidateLayout();
	
	void _requestInvalidateMeasure(sl_bool flagWidth, sl_bool flagHeight);

	void _initializeLayout();
	
	void _initializeTransform();
	
	void _applyTransform();
	
	void _initializeDraw();
	
	void _refreshBorderPen(sl_bool flagRedraw);
	
	void _initializeScroll();
	
	Ref<ScrollBar> _createHorizontalScrollBar();
	
	Ref<ScrollBar> _createVerticalScrollBar();
	
	void _refreshScroll(sl_bool flagRedraw);
	
	void _getScrollBars(Ref<View> views[2]);
	
	void _processEventForStateAndClick(UIEvent* ev);
	
	void _processContentScrollingEvents(UIEvent* ev);
	
protected:
	void measureRelativeLayout(sl_bool flagHorizontal, sl_bool flagVertical);
	
protected:
	CList< Ref<View> >& _getChildren();

	void _setFontInstance(const Ref<FontInstance>& instance);

	virtual void _setBorder_NW(sl_bool flag);

	virtual void _setBackgroundColor_NW(const Color& color);

	virtual void _setFont_NW(const Ref<Font>& font);

private:
	SafeRef<ViewInstance> m_instance;
	SafeWeakRef<Window> m_window;
	SafeRef<GraphicsContext> m_graphicsContext;
	SafeWeakRef<View> m_parent;
	
	sl_bool m_flagCreatingInstance;
	sl_bool m_flagCreatingChildInstances;
	sl_bool m_flagCreatingNativeWidget;
	UIAttachMode m_attachMode;
	
	UIRect m_frame;
	
	Visibility m_visibility;
	sl_bool m_flagEnabled;
	sl_bool m_flagOpaque;
	sl_bool m_flagHitTestable;
	sl_bool m_flagFocusable;

	sl_bool m_flagFocused;
	sl_bool m_flagDown;
	sl_bool m_flagHover;
	sl_bool m_flagOccurringClick;
	
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
	
	class LayoutAttributes : public Referable
	{
	public:
		sl_bool flagEnabled;
		
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
		sl_bool flagInvalidMeasure;
		sl_bool flagInvalidLayout;

		sl_ui_len measuredRelativeBoundWidth;
		sl_bool flagInvalidRelativeBoundWidth;
		sl_bool flagBadRelativeBoundWidth;
		sl_ui_len measuredRelativeBoundHeight;
		sl_bool flagInvalidRelativeBoundHeight;
		sl_bool flagBadRelativeBoundHeight;

		sl_bool flagOnPrepareLayout;
		sl_bool flagOnMakeLayout;
		
		sl_ui_pos paddingLeft;
		sl_ui_pos paddingTop;
		sl_ui_pos paddingRight;
		sl_ui_pos paddingBottom;

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
	SafeRef<LayoutAttributes> m_layout;
	
	class TransformAttributes : public Referable
	{
	public:
		sl_bool flagEnabled;
		Matrix3 transform;
		Matrix3 transformInverse;
		sl_real scaleX;
		sl_real scaleY;
		sl_real rotationAngle;
		sl_real anchorOffsetX;
		sl_real anchorOffsetY;
	};
	SafeRef<TransformAttributes> m_transform;
	
	class DrawAttributes : public Referable
	{
	public:
		SafeRef<Drawable> background;
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
		sl_bool flagDoubleBuffer;
		sl_bool flagClippingBounds;
		SafeRef<Bitmap> bitmapDoubleBuffer;
		SafeRef<Canvas> canvasDoubleBuffer;
		
		SafeRef<Font> font;
		SafeRef<FontInstance> fontInstance;
		
		sl_bool flagOnDrawBackgroundAlways;
		sl_bool flagOnDrawBorderAlways;
	};
	SafeRef<DrawAttributes> m_draw;
	
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
	SafeRef<ScrollAttributes> m_scroll;
	
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
	
	virtual void setVisible(sl_bool flag) = 0;
	
	virtual void setEnabled(sl_bool flag) = 0;
	
	virtual void setOpaque(sl_bool flag) = 0;
	
	virtual UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen) = 0;
	
	virtual UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView) = 0;
	
	virtual void addChildInstance(const Ref<ViewInstance>& instance) = 0;
	
	virtual void removeChildInstance(const Ref<ViewInstance>& instance) = 0;
	
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
	SafeWeakRef<GraphicsContext> m_graphicsContext;
	sl_bool m_flagNativeWidget;
	sl_bool m_flagWindowContent;

};

class ViewGroup : public View
{
	SLIB_DECLARE_OBJECT
public:
	ViewGroup(sl_bool flagCreatingChildInstances = sl_true);
	
};

SLIB_UI_NAMESPACE_END

#endif
