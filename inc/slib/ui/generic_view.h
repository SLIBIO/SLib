#ifndef CHECKHEADER_SLIB_UI_GENERIC_VIEW
#define CHECKHEADER_SLIB_UI_GENERIC_VIEW

#include "definition.h"

#include "view.h"
#include "cursor.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT GenericViewWithEvent : public View
{
	SLIB_DECLARE_OBJECT(GenericViewWithEvent, View)
public:
	GenericViewWithEvent();
	
public:
	Ref<Cursor> getCursor();
	
	virtual void setCursor(const Ref<Cursor>& cursor);
	
	
	sl_bool isFocusable();
	
	virtual void setFocusable(sl_bool flag);
	
	
	sl_bool isDownState();
	
	virtual void setDownState(sl_bool flag);
	
	
	sl_bool isHoverState();
	
	virtual void setHoverState(sl_bool flag);
	
public:
	SLIB_REF_PROPERTY(Runnable, OnClick)
	
	SLIB_BOOLEAN_PROPERTY(ClickEnabled)
	
protected:
	virtual void onClickView(UIEvent* ev);

protected:
	// override
	void onSetCursor(UIEvent* ev);
	
public:
	// override
	virtual void dispatchMouseEvent(UIEvent* event);
	
	// override
	virtual void dispatchTouchEvent(UIEvent* event);
	
private:
	void _processEvents(UIEvent* ev);
	
private:
	SafeRef<Cursor> m_cursor;
	
	sl_bool m_flagFocusable;
	sl_bool m_flagHover;
	sl_bool m_flagDown;
	
};

class SLIB_EXPORT GenericViewWithDrawing : public GenericViewWithEvent
{
	SLIB_DECLARE_OBJECT(GenericViewWithDrawing, GenericViewWithEvent)
public:
	GenericViewWithDrawing();
	
public:
	Ref<Drawable> getBackground();
	
	virtual void setBackground(const Ref<Drawable>& drawable);
	
	
	Color getBackgroundColor();
	
	virtual void setBackgroundColor(const Color& color);
	
	
	Ref<Pen> getBorderPen();
	
	virtual void setBorderPen(const Ref<Pen>& pen);
	
	
	PenStyle getBorderStyle();
	
	virtual void setBorderStyle(PenStyle style);
	
	
	sl_real getBorderWidth();
	
	virtual void setBorderWidth(sl_real width);
	
	
	Color getBorderColor();
	
	virtual void setBorderColor(const Color& color);
	
	
	BoundShape getBoundShape();
	
	virtual void setBoundShape(BoundShape shape);
	
	
	const Size& getRoundRectBoundShapeRadius();
	
	virtual void setRoundRectBoundShapeRadius(const Size& radius);
	
	void setRoundRectBoundShapeRadius(sl_real rx, sl_real ry);
	
	void setRoundRectBoundShapeRadius(sl_real radius);
	
	
	Ref<GraphicsPath> getBoundShapePath();
	
	virtual void setBoundShapePath(const Ref<GraphicsPath>& path);
	
protected:
	virtual void onPreDraw(Canvas* canvas);
	
	virtual void onPostDraw(Canvas* canvas);
	
	virtual void onDrawChildren(Canvas* canvas);
	
	virtual void onDrawBackground(Canvas* canvas);
	
	virtual void onDrawBorder(Canvas* canvas);
	
	virtual void drawBackground(Canvas* canvas, const Color& color, const Ref<Drawable>& background);
	
	virtual void drawBorder(Canvas* canvas, const Ref<Pen>& pen);
	
public:
	// override
	void dispatchDraw(Canvas* canvas);
	
private:
	void _refreshBorderPen();

private:
	SafeRef<Drawable> m_background;
	Color m_backgroundColor;
	
	SafeRef<Pen> m_penBorder;
	PenStyle m_borderStyle;
	sl_real m_borderWidth;
	Color m_borderColor;
	
	BoundShape m_boundShape;
	Size m_roundRectBoundShapeRadius;
	SafeRef<GraphicsPath> m_boundShapePath;
	
};


class SLIB_EXPORT GenericView : public GenericViewWithDrawing
{
	SLIB_DECLARE_OBJECT(GenericView, GenericViewWithDrawing)
public:
	GenericView();
	
public:
	// override
	Rectangle getContentBounds();
	
	
	virtual void setPadding(sl_real left, sl_real top, sl_real right, sl_real bottom);
	
	void setPadding(sl_real padding);
	
	sl_real getPaddingLeft();
	
	void setPaddingLeft(sl_real padding);
	
	sl_real getPaddingTop();
	
	void setPaddingTop(sl_real padding);
	
	sl_real getPaddingRight();
	
	void setPaddingRight(sl_real padding);
	
	sl_real getPaddingBottom();
	
	void setPaddingBottom(sl_real padding);

private:
	sl_real m_paddingLeft;
	sl_real m_paddingTop;
	sl_real m_paddingRight;
	sl_real m_paddingBottom;
	
};
SLIB_UI_NAMESPACE_END

#endif
