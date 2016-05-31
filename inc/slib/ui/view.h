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
	
	Ref<ViewInstance> createGenericInstance(ViewInstance* parent);
	
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
	
	
	List< Ref<View> > getChildren();
	
	void addChild(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	void removeChild(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	void removeAllChildren(sl_bool flagRedraw = sl_true);
	
	Ref<View> getChildAt(sl_real x, sl_real y);
	
	Ref<View> getChildAt(const Point& point);
	
	void attachChild(const Ref<View>& child);
	
	void addChildInstance(const Ref<ViewInstance>& instance);
	
	void removeChildInstance(const Ref<ViewInstance>& instance);
	
	
	void invalidate();

	// local coordinate
	void invalidate(const Rectangle& rect);
	
	
	// parent coordinate
	const Rectangle& getFrame();
	
	// parent coordinate
	Rectangle getInstanceFrame();
	
	// parent coordinate
	void setFrame(const Rectangle& frame, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	void setFrame(sl_real x, sl_real y, sl_real width, sl_real height, sl_bool flagRedraw = sl_true);
	
	sl_real getWidth();
	
	sl_real getHeight();
	
	Size getSize();
	
	void setSize(const Size& size, sl_bool flagRedraw = sl_true);
	
	void setSize(sl_real width, sl_real height, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	sl_real getX();
	
	// parent coordinate
	sl_real getY();
	
	// parent coordinate
	void setX(sl_real x, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	void setY(sl_real y, sl_bool flagRedraw = sl_true);

	// parent coordinate
	Point getPosition();
	
	// parent coordinate
	void setPosition(sl_real x, sl_real y, sl_bool flagRedraw = sl_true);
	
	// parent coordinate
	void setPosition(const Point& point, sl_bool flagRedraw = sl_true);
	
	// local coordinate
	Rectangle getBounds();
	
	// local coordinate
	Rectangle getBoundsInnerPadding();
	
	// parent coordinate
	Rectangle getBoundsInParent();
	
	// parent coordinate
	Rectangle getBoundsInParent(const Rectangle& boundsLocal);
	
	
	sl_real getPaddingLeft();
	
	void setPaddingLeft(sl_real padding, sl_bool flagRedraw = sl_true);
	
	sl_real getPaddingTop();
	
	void setPaddingTop(sl_real padding, sl_bool flagRedraw = sl_true);
	
	sl_real getPaddingRight();
	
	void setPaddingRight(sl_real padding, sl_bool flagRedraw = sl_true);
	
	sl_real getPaddingBottom();
	
	void setPaddingBottom(sl_real padding, sl_bool flagRedraw = sl_true);
	
	void setPadding(sl_real left, sl_real top, sl_real right, sl_real bottom, sl_bool flagRedraw = sl_true);
	
	void setPadding(sl_real padding, sl_bool flagRedraw = sl_true);
	
	
	sl_bool isVisible();
	
	void setVisible(sl_bool flagVisible, sl_bool flagRedraw = sl_true);
	
	sl_bool isEnabled();
	
	void setEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	sl_bool isOpaque();
	
	void setOpaque(sl_bool flagOpaque, sl_bool flagRedraw = sl_true);
	
	
	sl_bool isHitTestable();
	
	void setHitTestable(sl_bool flag);
	
	// local coordinate
	virtual sl_bool hitTest(sl_real x, sl_real y);
	
	// local coordinate
	sl_bool hitTest(const Point& point);

	
	sl_bool isFocusable();
	
	void setFocusable(sl_bool flagFocusable, sl_bool flagRedraw = sl_true);
	
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
	

	Point convertCoordinateFromScreen(const Point& ptScreen);
	
	Point convertCoordinateToScreen(const Point& ptView);
	
	Point convertCoordinateFromParent(const Point& ptParent);
	
	Point convertCoordinateToParent(const Point& ptView);
	
	
	Ref<Drawable> getBackground();
	
	void setBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Color getBackgroundColor();
	
	void setBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Pen> getBorderPen();
	
	void setBorderPen(const Ref<Pen>& pen, sl_bool flagRedraw = sl_true);
	
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
	
	void setPreDrawEnabled(sl_bool flagEnable, sl_bool flagRedraw = sl_true);
	
	sl_bool isPostDrawEnabled();
	
	void setPostDrawEnabled(sl_bool flagEnable, sl_bool flagRedraw = sl_true);
	
	sl_bool isClippingBounds();
	
	void setClippingBounds(sl_bool flag);
	
	sl_bool isDoubleBuffering();
	
	void setDoubleBuffering(sl_bool flagEnable, sl_bool flagRedraw = sl_true);
	

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
	
	sl_real getScrollX();
	
	sl_real getScrollY();
	
	virtual Point getScrollPosition();
	
	void scrollTo(sl_real x, sl_real y, sl_bool flagRedraw = sl_true);
	
	void scrollTo(const Point& position, sl_bool flagRedraw = sl_true);
	
	sl_real getContentWidth();
	
	sl_real getContentHeight();
	
	Size getContentSize();
	
	void setContentSize(sl_real width, sl_real height, sl_bool flagRefresh = sl_true);
	
	void setContentSize(const Size& size, sl_bool flagRefresh = sl_true);
	
	void setContentWidth(sl_real width, sl_bool flagRefresh = sl_true);
	
	void setContentHeight(sl_real height, sl_bool flagRefresh = sl_true);
	
	virtual Size getScrollRange();
	
	sl_real getScrollBarWidth();
	
	void setScrollBarWidth(sl_real width, sl_bool flagRefresh = sl_true);
	
	
	sl_bool isContentScrollingByMouse();
	
	void setContentScrollingByMouse(sl_bool flag);
	
	sl_bool isContentScrollingByTouch();
	
	void setContentScrollingByTouch(sl_bool flag);
	
	sl_bool isContentScrollingByMouseWheel();
	
	void setContentScrollingByMouseWheel(sl_bool flag);
	
	sl_bool isContentScrollingByKeyboard();
	
	void setContentScrollingByKeyboard(sl_bool flag);


	Ref<Font> getFont();

	virtual void setFont(const Ref<Font>& font, sl_bool flagRedraw = sl_true);
	
	
	sl_bool isMultiTouchMode();
	
	void setMultiTouchMode(sl_bool flag);
	
	sl_bool isPassingEventsToChildren();
	
	void setPassingEventsToChildren(sl_bool flag);
	
	
	Ref<View> getRootView();
	
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
	
	virtual void onResize(sl_real width, sl_real height);
	
	virtual void onResizeChild(View* child, sl_real width, sl_real height);
	
	virtual void onShow();
	
	virtual void onHide();
	
	virtual void onScroll(sl_real x, sl_real y);
	
	virtual void onResizeContent(sl_real width, sl_real height);
	
	virtual void onAttach();
	
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
	
	virtual void dispatchResize(sl_real width, sl_real height);
	
	virtual void dispatchShow();
	
	virtual void dispatchHide();
	
	virtual void dispatchScroll(sl_real x, sl_real y);
	
private:
	void _processAttachOnUiThread();

	
	void _removeChild(const Ref<View>& view);
	
	void _attachChildOnUiThread(Ref<View> child);
	
	void _addChildInstanceOnUiThread(Ref<ViewInstance> instance);
	
	void _removeChildInstanceOnUiThread(Ref<ViewInstance> instance);
	
	
	void _killFocusFromParent();
	
	void _setFocusedChild(View* child, sl_bool flagRedraw);
	
	
	void _initializeTransform();
	
	void _applyTransform();
	
	
	void _initializeDraw();
	
	void _refreshBorderPen(sl_bool flagRedraw);
	
	
	void _initializeScroll();
	
	Ref<ScrollBar> _createHorizontalScrollBar();
	
	Ref<ScrollBar> _createVerticalScrollBar();
	
	void _refreshScroll(sl_bool flagRedraw);
	
	
	void _processEventForStateAndClick(UIEvent* ev);
	
	void _processContentScrollingEvents(UIEvent* ev);
	
protected:
	void _setFontInstance(const Ref<FontInstance>& instance);

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
	
	Rectangle m_frame;
	sl_real m_paddingLeft;
	sl_real m_paddingTop;
	sl_real m_paddingRight;
	sl_real m_paddingBottom;
	
	sl_bool m_flagVisible;
	sl_bool m_flagEnabled;
	sl_bool m_flagOpaque;
	
	sl_bool m_flagHitTestable;
	
	sl_bool m_flagFocusable;
	sl_bool m_flagFocused;
	SafeRef<View> m_childFocused;
	
	sl_bool m_flagDown;
	sl_bool m_flagHover;
	sl_bool m_flagOccurringClick;
	
	SafeRef<Cursor> m_cursor;
	
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
		Color backgroundColor;
		
		SafeRef<Pen> penBorder;
		PenStyle borderStyle;
		sl_real borderWidth;
		Color borderColor;
		
		BoundShape boundShape;
		Size roundRectBoundShapeRadius;
		SafeRef<GraphicsPath> boundShapePath;
	};
	SafeRef<DrawAttributes> m_draw;
	
	sl_bool m_flagPreDrawEnabled;
	sl_bool m_flagPostDrawEnabled;
	sl_bool m_flagDoubleBuffer;
	sl_bool m_flagClippingBounds;
	SafeRef<Bitmap> m_bitmapDoubleBuffer;
	SafeRef<Canvas> m_canvasDoubleBuffer;

	class ScrollAttributes : public Referable
	{
	public:
		SafeRef<ScrollBar> horz;
		SafeRef<ScrollBar> vert;
		sl_real x;
		sl_real y;
		sl_real contentWidth;
		sl_real contentHeight;
		sl_real barWidth;
		
		sl_bool flagContentScrollingByMouse;
		sl_bool flagContentScrollingByTouch;
		sl_bool flagContentScrollingByMouseWheel;
		sl_bool flagContentScrollingByKeyboard;
		sl_bool flagDownContent;
		sl_real scrollX_DownContent;
		sl_real scrollY_DownContent;
		sl_real mouseX_DownContent;
		sl_real mouseY_DownContent;
	};
	SafeRef<ScrollAttributes> m_scroll;

	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
	CList< Ref<View> > m_children;
	SafeList< Ref<View> > m_childrenMultiTouch;
	SafeRef<View> m_childMouseMove;
	SafeRef<View> m_childMouseDown;
	UIAction m_actionMouseDown;
	sl_bool m_flagMultiTouchMode;
	sl_bool m_flagPassEventToChildren;
	
	sl_bool m_flagProcessingTabStop;
	SafeRef<View> m_viewNextTabStop;
	SafeRef<View> m_viewPrevTabStop;
	
	friend class Window;
	
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
	
public:
	virtual sl_bool isValid() = 0;
	
	virtual void setFocus() = 0;
	
	virtual void invalidate() = 0;
	
	virtual void invalidate(const Rectangle& rect) = 0;
	
	virtual Rectangle getFrame() = 0;
	
	virtual void setFrame(const Rectangle& frame) = 0;
	
	virtual void setVisible(sl_bool flag) = 0;
	
	virtual void setEnabled(sl_bool flag) = 0;
	
	virtual void setOpaque(sl_bool flag) = 0;
	
	virtual Point convertCoordinateFromScreenToView(const Point& ptScreen) = 0;
	
	virtual Point convertCoordinateFromViewToScreen(const Point& ptView) = 0;
	
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

};

class ViewGroup : public View
{
	SLIB_DECLARE_OBJECT
public:
	ViewGroup(sl_bool flagCreatingChildInstances = sl_true);
	
};

SLIB_UI_NAMESPACE_END

#endif
