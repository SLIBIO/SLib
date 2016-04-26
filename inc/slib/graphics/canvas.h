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
	SLIB_DECLARE_OBJECT

public:
	Ref<GraphicsContext> getGraphicsContext();
	
public:
	virtual Size getSize() = 0;
	
	virtual Rectangle getInvalidatedRect() = 0;
	
	
	virtual void save() = 0;
	
	virtual void restore() = 0;

	
	virtual void setAntiAlias(sl_bool flag) = 0;
	
	
	virtual void clipToRectangle(const Rectangle& rect) = 0;
	
	void clipToRectangle(sl_real x, sl_real y, sl_real width, sl_real height);
	
	
	virtual void clipToPath(const Ref<GraphicsPath>& path) = 0;
	
	virtual void clipToRoundRect(const Rectangle& rect, const Size& radius);
	
	void clipToRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry);
	
	virtual void clipToEllipse(const Rectangle& rect);
	
	void clipToEllipse(sl_real x, sl_real y, sl_real width, sl_real height);
	
	
	// concat matrix to the left (pre-concat)
	virtual void concatMatrix(const Matrix3& matrix) = 0;
	
	virtual void translate(sl_real dx, sl_real dy);
	
	virtual void rotate(sl_real radians);
	
	virtual void rotate(sl_real cx, sl_real cy, sl_real radians);
	
	virtual void scale(sl_real sx, sl_real sy);
	

	Size getTextSize(const Ref<Font>& font, const String& text);

	virtual void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& font, const Color& color) = 0;
	
	void drawText(const String& text, const Rectangle& rcDst, const Ref<Font>& font, const Color& color, Alignment alignment);
	
	
	virtual void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& pen) = 0;
	
	void drawLine(sl_real x1, sl_real y1, sl_real x2, sl_real y2, const Ref<Pen>& pen);
	
	
	virtual void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen) = 0;
	
	void drawLines(const List<Point>& points, const Ref<Pen>& pen);
	
	
	virtual void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen) = 0;

	void drawArc(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen);
	
	
	virtual void drawRectangle(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	
	void drawRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen, const Ref<Brush>& brush);
	
	void drawRectangle(const Rectangle& rc, const Ref<Pen>& pen);
	
	void drawRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen);
	
	void fillRectangle(const Rectangle& rc, const Ref<Brush>& brush);
	
	void fillRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Brush>& brush);
	
	
	virtual void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	
	void drawRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Pen>& pen, const Ref<Brush>& brush);
	
	void drawRoundRect(const Rectangle& rc, const Size& radius, const Ref<Pen>& pen);
	
	void drawRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Pen>& pen);
	
	void fillRoundRect(const Rectangle& rc, const Size& radius, const Ref<Brush>& brush);
	
	void fillRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Brush>& brush);
	
	
	virtual void drawEllipse(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	
	void drawEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen, const Ref<Brush>& brush);
	
	void drawEllipse(const Rectangle& rc, const Ref<Pen>& pen);
	
	void drawEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen);
	
	void fillEllipse(const Rectangle& rc, const Ref<Brush>& brush);
	
	void fillEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Brush>& brush);
	
	
	virtual void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode = FillMode::Alternate) = 0;
	
	void drawPolygon(const List<Point>& points, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode = FillMode::Alternate);
	
	void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen);
	
	void drawPolygon(const List<Point>& points, const Ref<Pen>& pen);
	
	void fillPolygon(const Point* points, sl_uint32 countPoints, const Ref<Brush>& brush);
	
	void fillPolygon(const List<Point>& points, const Ref<Brush>& brush);
	
	
	virtual void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;
	
	void drawPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush);
	
	void drawPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen);
	
	void drawPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen);
	
	void fillPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Ref<Brush>& brush);
	
	void fillPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Brush>& brush);
	
	
	virtual void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen, const Ref<Brush>& brush) = 0;

	void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen);
	
	void fillPath(const Ref<GraphicsPath>& path, const Ref<Brush>& brush);
	
	
	void draw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc);
	
	void draw(const Rectangle& rectDst, const Ref<Drawable>& src);
	
	void draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst
			, const Ref<Drawable>& src, sl_real xSrc, sl_real ySrc, sl_real widthSrc, sl_real heightSrc);

	void draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst, const Ref<Drawable>& drawable);

	void draw(sl_real xDst, sl_real yDst, const Ref<Drawable>& drawable);
	
	void draw(const Rectangle& rectDst, const Ref<Drawable>& src, ScaleMode scaleMode, Alignment alignment);
	
protected:
	SafeWeakRef<GraphicsContext> m_context;
	
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
