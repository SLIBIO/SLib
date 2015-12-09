#ifndef CHECKHEADER_SLIB_GRAPHICS_CANVAS
#define CHECKHEADER_SLIB_GRAPHICS_CANVAS

#include "definition.h"
#include "constants.h"

#include "pen.h"
#include "brush.h"
#include "font.h"
#include "path.h"
#include "drawable.h"

#include "../core/object.h"
#include "../math/matrix3.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class GraphicsContext;

class SLIB_EXPORT Canvas : public Object
{
	SLIB_DECLARE_OBJECT(Canvas, Object)
protected:
	Canvas();

public:
	Ref<GraphicsContext> getGraphicsContext();
	void setGraphicsContext(const Ref<GraphicsContext>& context);
	
	virtual Size getSize() = 0;
	virtual Rectangle getInvalidatedRect() = 0;
	
	virtual void save() = 0;
	virtual void restore() = 0;

	virtual void setAntiAlias(sl_bool flag) = 0;
	virtual void clipToRectangle(const Rectangle& rect) = 0;
	virtual void clipToPath(const Ref<GraphicsPath>& path) = 0;

	// concat matrix to the left (pre-concat)
	virtual void concatMatrix(const Matrix3& matrix) = 0;

	virtual void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& font, const Color& color) = 0;
	
	virtual void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& pen) = 0;
	virtual void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen) = 0;
	virtual void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen) = 0;

	virtual void drawRectangle(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	virtual void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	virtual void drawEllipse(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	virtual void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode = fillModeAlternate) = 0;
	virtual void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	virtual void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;

public:
	SLIB_INLINE void clipToRectangle(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		Rectangle rect(x, y, x + width, y + height);
		clipToRectangle(rect);
	}
	
	SLIB_INLINE void clipToRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry)
	{
		Rectangle rect(x, y, x + width, y + height);
		Size radius(rx, ry);
		clipToRoundRect(rect, radius);
	}
	
	SLIB_INLINE void clipToEllipse(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		Rectangle rect(x, y, x+width, y+height);
		clipToEllipse(rect);
	}

	SLIB_INLINE void drawLine(sl_real x1, sl_real y1, sl_real x2, sl_real y2, const Ref<Pen>& pen)
	{
		Point pt1(x1, y1);
		Point pt2(x2, y2);
		drawLine(pt1, pt2, pen);
	}
	
	SLIB_INLINE void drawLines(const List<Point>& _points, const Ref<Pen>& pen)
	{
		ListLocker<Point> points(_points);
		drawLines(points.getBuffer(), (sl_uint32)(points.getCount()), pen);
	}
	
	SLIB_INLINE void drawArc(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
	{
		Rectangle rect(x, y, x + width, y + height);
		drawArc(rect, startDegrees, sweepDegrees, pen);
	}

	SLIB_INLINE void drawRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawRectangle(rect, pen, brush);
	}
	
	SLIB_INLINE void drawRectangle(const Rectangle& rc, const Ref<Pen>& pen)
	{
		drawRectangle(rc, pen, Ref<Brush>::null());
	}

	SLIB_INLINE void drawRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawRectangle(rect, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void fillRectangle(const Rectangle& rc, const Ref<Brush>& brush)
	{
		drawRectangle(rc, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void fillRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawRectangle(rect, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void drawRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Pen>& pen, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x + width, y + height);
		Size radius(rx, ry);
		drawRoundRect(rect, radius, pen, brush);
	}
	
	SLIB_INLINE void drawRoundRect(const Rectangle& rc, const Size& radius, const Ref<Pen>& pen)
	{
		drawRoundRect(rc, radius, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void drawRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Pen>& pen)
	{
		Rectangle rect(x, y, x+width, y+height);
		Size radius(rx, ry);
		drawRoundRect(rect, radius, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void fillRoundRect(const Rectangle& rc, const Size& radius, const Ref<Brush>& brush)
	{
		drawRoundRect(rc, radius, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void fillRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x+width, y+height);
		Size radius(rx, ry);
		drawRoundRect(rect, radius, Ref<Pen>::null(), brush);
	}

	SLIB_INLINE void drawEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawEllipse(rect, pen, brush);
	}
	
	SLIB_INLINE void drawEllipse(const Rectangle& rc, const Ref<Pen>& pen)
	{
		drawEllipse(rc, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void drawEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawEllipse(rect, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void fillEllipse(const Rectangle& rc, const Ref<Brush>& brush)
	{
		drawEllipse(rc, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void fillEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawEllipse(rect, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void drawPolygon(const List<Point>& _points, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode = fillModeAlternate)
	{
		ListLocker<Point> points(_points);
		drawPolygon(points.getBuffer(), (sl_uint32)(points.getCount()), pen, brush, fillMode);
	}
	
	SLIB_INLINE void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen)
	{
		drawPolygon(points, countPoints, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void drawPolygon(const List<Point>& _points, const Ref<Pen>& pen)
	{
		ListLocker<Point> points(_points);
		drawPolygon(points.getBuffer(), (sl_uint32)(points.getCount()), pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void fillPolygon(const Point* points, sl_uint32 countPoints, const Ref<Brush>& brush)
	{
		drawPolygon(points, countPoints, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void fillPolygon(const List<Point>& _points, const Ref<Brush>& brush)
	{
		ListLocker<Point> points(_points);
		drawPolygon(points.getBuffer(), (sl_uint32)(points.getCount()), Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void drawPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x + width, y + height);
		drawPie(rect, startDegrees, sweepDegrees, pen, brush);
	}
	
	SLIB_INLINE void drawPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
	{
		drawPie(rc, startDegrees, sweepDegrees, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void drawPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawPie(rect, startDegrees, sweepDegrees, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void fillPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Ref<Brush>& brush)
	{
		drawPie(rc, startDegrees, sweepDegrees, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void fillPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Brush>& brush)
	{
		Rectangle rect(x, y, x+width, y+height);
		drawPie(rect, startDegrees, sweepDegrees, Ref<Pen>::null(), brush);
	}
	
	SLIB_INLINE void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen)
	{
		drawPath(path, pen, Ref<Brush>::null());
	}
	
	SLIB_INLINE void fillPath(const Ref<GraphicsPath>& path, const Ref<Brush>& brush)
	{
		drawPath(path, Ref<Pen>::null(), brush);
	}

public:
	void draw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc);
	
	void draw(const Rectangle& rectDst, const Ref<Drawable>& src);
	
	void draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst
			, const Ref<Drawable>& src, sl_real xSrc, sl_real ySrc, sl_real widthSrc, sl_real heightSrc);

	void draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst, const Ref<Drawable>& drawable);

	void draw(sl_real xDst, sl_real yDst, const Ref<Drawable>& drawable);
	
public:
	virtual void clipToRoundRect(const Rectangle& rect, const Size& radius);
	virtual void clipToEllipse(const Rectangle& rect);

	virtual void translate(sl_real dx, sl_real dy);
	virtual void rotate(sl_real radians);
	virtual void rotate(sl_real cx, sl_real cy, sl_real radians);
	virtual void scale(sl_real sx, sl_real sy);
	
public:
	Size getTextSize(const Ref<Font>& font, const String& text);
	void drawText(const String& text, const Rectangle& rcDst, const Ref<Font>& font, const Color& color, Alignment alignment);
	
	void draw(const Rectangle& rectDst, const Ref<Drawable>& src, ScaleType scaleType, Alignment alignment);
	
private:
	WeakRef<GraphicsContext> m_context;
};

class SLIB_EXPORT CanvasStatusScope
{
public:
	CanvasStatusScope(const Ref<Canvas>& canvas);
	~CanvasStatusScope();
	
private:
	Ref<Canvas> m_canvas;
};
SLIB_GRAPHICS_NAMESPACE_END

#endif
