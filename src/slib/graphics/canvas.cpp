#include "../../../inc/slib/graphics/canvas.h"

#include "../../../inc/slib/graphics/util.h"
#include "../../../inc/slib/math/transform2d.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Canvas, Object)

Canvas::Canvas()
{
	m_time = Time::now();
}

CanvasType Canvas::getType()
{
	return m_type;
}

void Canvas::setType(CanvasType type)
{
	m_type = type;
}

Time Canvas::getTime()
{
	return m_time;
}

void Canvas::setTime(const Time& time)
{
	m_time = time;
}

Size Canvas::getSize()
{
	return m_size;
}

void Canvas::setSize(const Size& size)
{
	m_size = size;
	m_invalidatedRect.left = 0;
	m_invalidatedRect.top = 0;
	m_invalidatedRect.right = size.x;
	m_invalidatedRect.bottom = size.y;
}

const Rectangle& Canvas::getInvalidatedRect()
{
	return m_invalidatedRect;
}

void Canvas::setInvalidatedRect(const Rectangle& rect)
{
	m_invalidatedRect = rect;
}

void Canvas::clipToRectangle(sl_real x, sl_real y, sl_real width, sl_real height)
{
	clipToRectangle(Rectangle(x, y, x + width, y + height));
}

void Canvas::clipToRoundRect(const Rectangle& rect, const Size& radius)
{
	Ref<GraphicsPath> path = GraphicsPath::create();
	if (path.isNotNull()) {
		path->addRoundRect(rect, radius);
		clipToPath(path);
	}
}

void Canvas::clipToRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry)
{
	clipToRoundRect(Rectangle(x, y, x + width, y + height), Size(rx, ry));
}

void Canvas::clipToEllipse(const Rectangle& rect)
{
	Ref<GraphicsPath> path = GraphicsPath::create();
	if (path.isNotNull()) {
		path->addEllipse(rect);
		clipToPath(path);
	}
}

void Canvas::clipToEllipse(sl_real x, sl_real y, sl_real width, sl_real height)
{
	clipToEllipse(Rectangle(x, y, x+width, y+height));
}

void Canvas::translate(sl_real dx, sl_real dy)
{
	Matrix3 mat;
	Transform2::setTranslation(mat, dx, dy);
	concatMatrix(mat);
}

void Canvas::rotate(sl_real radians)
{
	Matrix3 mat;
	Transform2::setRotation(mat, radians);
	concatMatrix(mat);
}

void Canvas::rotate(sl_real cx, sl_real cy, sl_real radians)
{
	Matrix3 mat;
	Transform2::setRotation(mat, cx, cy, radians);
	concatMatrix(mat);
}

void Canvas::scale(sl_real sx, sl_real sy)
{
	Matrix3 mat;
	Transform2::setScaling(mat, sx, sy);
	concatMatrix(mat);
}

Size Canvas::getTextSize(const Ref<Font>& font, const String &text)
{
	if (font.isNotNull()) {
		return font->getTextSize(text);
	}
	return Size::zero();
}

void Canvas::drawText(const String& text, const Rectangle& rcDst, const Ref<Font>& _font, const Color& color, Alignment align)
{
	Ref<Font> font = _font;
	if (font.isNull()) {
		font = Font::getDefault();
		if (font.isNull()) {
			return;
		}
	}
	Size size = getTextSize(font, text);
	Point pt = GraphicsUtil::calculateAlignPosition(rcDst, size.x, size.y, align);
	drawText(text, pt.x, pt.y, font, color);
}

void Canvas::drawLine(sl_real x1, sl_real y1, sl_real x2, sl_real y2, const Ref<Pen>& pen)
{
	drawLine(Point(x1, y1), Point(x2, y2), pen);
}

void Canvas::drawLines(const List<Point>& _points, const Ref<Pen>& pen)
{
	ListLocker<Point> points(_points);
	drawLines(points.data, (sl_uint32)(points.count), pen);
}

void Canvas::drawArc(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
{
	drawArc(Rectangle(x, y, x + width, y + height), startDegrees, sweepDegrees, pen);
}

void Canvas::drawRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	drawRectangle(Rectangle(x, y, x+width, y+height), pen, brush);
}

void Canvas::drawRectangle(const Rectangle& rc, const Ref<Pen>& pen)
{
	drawRectangle(rc, pen, Ref<Brush>::null());
}

void Canvas::drawRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen)
{
	drawRectangle(Rectangle(x, y, x+width, y+height), pen, Ref<Brush>::null());
}

void Canvas::fillRectangle(const Rectangle& rc, const Ref<Brush>& brush)
{
	drawRectangle(rc, Ref<Pen>::null(), brush);
}

void Canvas::fillRectangle(const Rectangle& rc, const Color& color)
{
	drawRectangle(rc, Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::fillRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Brush>& brush)
{
	drawRectangle(Rectangle(x, y, x+width, y+height), Ref<Pen>::null(), brush);
}

void Canvas::fillRectangle(sl_real x, sl_real y, sl_real width, sl_real height, const Color& color)
{
	drawRectangle(Rectangle(x, y, x+width, y+height), Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::drawRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	drawRoundRect(Rectangle(x, y, x + width, y + height), Size(rx, ry), pen, brush);
}

void Canvas::drawRoundRect(const Rectangle& rc, const Size& radius, const Ref<Pen>& pen)
{
	drawRoundRect(rc, radius, pen, Ref<Brush>::null());
}

void Canvas::drawRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Pen>& pen)
{
	drawRoundRect(Rectangle(x, y, x+width, y+height), Size(rx, ry), pen, Ref<Brush>::null());
}

void Canvas::fillRoundRect(const Rectangle& rc, const Size& radius, const Ref<Brush>& brush)
{
	drawRoundRect(rc, radius, Ref<Pen>::null(), brush);
}

void Canvas::fillRoundRect(const Rectangle& rc, const Size& radius, const Color& color)
{
	drawRoundRect(rc, radius, Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::fillRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Ref<Brush>& brush)
{
	drawRoundRect(Rectangle(x, y, x+width, y+height), Size(rx, ry), Ref<Pen>::null(), brush);
}

void Canvas::fillRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry, const Color& color)
{
	drawRoundRect(Rectangle(x, y, x+width, y+height), Size(rx, ry), Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::drawEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	drawEllipse(Rectangle(x, y, x+width, y+height), pen, brush);
}

void Canvas::drawEllipse(const Rectangle& rc, const Ref<Pen>& pen)
{
	drawEllipse(rc, pen, Ref<Brush>::null());
}

void Canvas::drawEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Pen>& pen)
{
	drawEllipse(Rectangle(x, y, x+width, y+height), pen, Ref<Brush>::null());
}

void Canvas::fillEllipse(const Rectangle& rc, const Ref<Brush>& brush)
{
	drawEllipse(rc, Ref<Pen>::null(), brush);
}

void Canvas::fillEllipse(const Rectangle& rc, const Color& color)
{
	drawEllipse(rc, Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::fillEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Ref<Brush>& brush)
{
	drawEllipse(Rectangle(x, y, x+width, y+height), Ref<Pen>::null(), brush);
}

void Canvas::fillEllipse(sl_real x, sl_real y, sl_real width, sl_real height, const Color& color)
{
	drawEllipse(Rectangle(x, y, x+width, y+height), Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::drawPolygon(const List<Point>& _points, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode)
{
	ListLocker<Point> points(_points);
	drawPolygon(points.data, (sl_uint32)(points.count), pen, brush, fillMode);
}

void Canvas::drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen)
{
	drawPolygon(points, countPoints, pen, Ref<Brush>::null());
}

void Canvas::drawPolygon(const List<Point>& _points, const Ref<Pen>& pen)
{
	ListLocker<Point> points(_points);
	drawPolygon(points.data, (sl_uint32)(points.count), pen, Ref<Brush>::null());
}

void Canvas::fillPolygon(const Point* points, sl_uint32 countPoints, const Ref<Brush>& brush)
{
	drawPolygon(points, countPoints, Ref<Pen>::null(), brush);
}

void Canvas::fillPolygon(const Point* points, sl_uint32 countPoints, const Color& color)
{
	drawPolygon(points, countPoints, Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::fillPolygon(const List<Point>& _points, const Ref<Brush>& brush)
{
	ListLocker<Point> points(_points);
	drawPolygon(points.data, (sl_uint32)(points.count), Ref<Pen>::null(), brush);
}

void Canvas::fillPolygon(const List<Point>& _points, const Color& color)
{
	ListLocker<Point> points(_points);
	drawPolygon(points.data, (sl_uint32)(points.count), Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::drawPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	drawPie(Rectangle(x, y, x + width, y + height), startDegrees, sweepDegrees, pen, brush);
}

void Canvas::drawPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
{
	drawPie(rc, startDegrees, sweepDegrees, pen, Ref<Brush>::null());
}

void Canvas::drawPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
{
	drawPie(Rectangle(x, y, x+width, y+height), startDegrees, sweepDegrees, pen, Ref<Brush>::null());
}

void Canvas::fillPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Ref<Brush>& brush)
{
	drawPie(rc, startDegrees, sweepDegrees, Ref<Pen>::null(), brush);
}

void Canvas::fillPie(const Rectangle& rc, sl_real startDegrees, sl_real sweepDegrees, const Color& color)
{
	drawPie(rc, startDegrees, sweepDegrees, Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::fillPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, const Color& color)
{
	drawPie(Rectangle(x, y, x+width, y+height), startDegrees, sweepDegrees, Ref<Pen>::null(), Brush::createSolidBrush(color));
}

void Canvas::drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen)
{
	drawPath(path, pen, Ref<Brush>::null());
}

void Canvas::fillPath(const Ref<GraphicsPath>& path, const Ref<Brush>& brush)
{
	drawPath(path, Ref<Pen>::null(), brush);
}

void Canvas::fillPath(const Ref<GraphicsPath>& path, const Color& color)
{
	drawPath(path, Ref<Pen>::null(), Brush::createSolidBrush(color));
}


const char _g_globalDefaultDrawParamBuf[sizeof(DrawParam)] = {0};
const DrawParam& _g_globalDefaultDrawParam = *((const DrawParam*)((void*)_g_globalDefaultDrawParamBuf));

void Canvas::draw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc, const DrawParam& param)
{
	if (src.isNull()) {
		return;
	}
	if (param.isTransparent()) {
		return;
	}
	if (rectDst.getWidth() < SLIB_EPSILON) {
		return;
	}
	if (rectDst.getHeight() < SLIB_EPSILON) {
		return;
	}
	if (rectSrc.getWidth() < SLIB_EPSILON) {
		return;
	}
	if (rectSrc.getHeight() < SLIB_EPSILON) {
		return;
	}
	onDraw(rectDst, src, rectSrc, param);
}

void Canvas::draw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc)
{
	if (src.isNull()) {
		return;
	}
	if (rectDst.getWidth() < SLIB_EPSILON) {
		return;
	}
	if (rectDst.getHeight() < SLIB_EPSILON) {
		return;
	}
	if (rectSrc.getWidth() < SLIB_EPSILON) {
		return;
	}
	if (rectSrc.getHeight() < SLIB_EPSILON) {
		return;
	}
	onDraw(rectDst, src, rectSrc, _g_globalDefaultDrawParam);
}

void Canvas::draw(const Rectangle& rectDst, const Ref<Drawable>& src, const DrawParam& param)
{
	if (src.isNull()) {
		return;
	}
	if (param.isTransparent()) {
		return;
	}
	if (rectDst.getWidth() < SLIB_EPSILON) {
		return;
	}
	if (rectDst.getHeight() < SLIB_EPSILON) {
		return;
	}
	sl_real sw = src->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = src->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	onDrawAll(rectDst, src, param);
}

void Canvas::draw(const Rectangle& rectDst, const Ref<Drawable>& src)
{
	if (src.isNull()) {
		return;
	}
	if (rectDst.getWidth() < SLIB_EPSILON) {
		return;
	}
	if (rectDst.getHeight() < SLIB_EPSILON) {
		return;
	}
	sl_real sw = src->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = src->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	onDrawAll(rectDst, src, _g_globalDefaultDrawParam);
}

void Canvas::draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst, const Ref<Drawable>& src, sl_real xSrc, sl_real ySrc, sl_real widthSrc, sl_real heightSrc, const DrawParam& param)
{
	if (src.isNull()) {
		return;
	}
	if (param.isTransparent()) {
		return;
	}
	if (widthDst < SLIB_EPSILON) {
		return;
	}
	if (heightDst < SLIB_EPSILON) {
		return;
	}
	if (widthSrc < SLIB_EPSILON) {
		return;
	}
	if (widthSrc < SLIB_EPSILON) {
		return;
	}
	Rectangle rectDst(xDst, yDst, xDst + widthDst, yDst + heightDst);
	Rectangle rectSrc(xSrc, ySrc, xSrc + widthSrc, ySrc + heightSrc);
	onDraw(rectDst, src, rectSrc, param);
}

void Canvas::draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst, const Ref<Drawable>& src, sl_real xSrc, sl_real ySrc, sl_real widthSrc, sl_real heightSrc)
{
	if (src.isNull()) {
		return;
	}
	if (widthDst < SLIB_EPSILON) {
		return;
	}
	if (heightDst < SLIB_EPSILON) {
		return;
	}
	if (widthSrc < SLIB_EPSILON) {
		return;
	}
	if (widthSrc < SLIB_EPSILON) {
		return;
	}
	Rectangle rectDst(xDst, yDst, xDst + widthDst, yDst + heightDst);
	Rectangle rectSrc(xSrc, ySrc, xSrc + widthSrc, ySrc + heightSrc);
	onDraw(rectDst, src, rectSrc, _g_globalDefaultDrawParam);
}

void Canvas::draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst, const Ref<Drawable>& src, const DrawParam& param)
{
	if (src.isNull()) {
		return;
	}
	if (param.isTransparent()) {
		return;
	}
	if (widthDst < SLIB_EPSILON) {
		return;
	}
	if (heightDst < SLIB_EPSILON) {
		return;
	}
	sl_real sw = src->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = src->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	Rectangle rectDst(xDst, yDst, xDst + widthDst, yDst + heightDst);
	onDrawAll(rectDst, src, param);
}

void Canvas::draw(sl_real xDst, sl_real yDst, sl_real widthDst, sl_real heightDst, const Ref<Drawable>& src)
{
	if (src.isNull()) {
		return;
	}
	if (widthDst < SLIB_EPSILON) {
		return;
	}
	if (heightDst < SLIB_EPSILON) {
		return;
	}
	sl_real sw = src->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = src->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	Rectangle rectDst(xDst, yDst, xDst + widthDst, yDst + heightDst);
	onDrawAll(rectDst, src, _g_globalDefaultDrawParam);
}

void Canvas::draw(sl_real xDst, sl_real yDst, const Ref<Drawable>& src, const DrawParam& param)
{
	if (src.isNull()) {
		return;
	}
	if (param.isTransparent()) {
		return;
	}
	sl_real sw = src->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = src->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	Rectangle rectDst(xDst, yDst, xDst + sw, yDst + sh);
	onDrawAll(rectDst, src, param);
}

void Canvas::draw(sl_real xDst, sl_real yDst, const Ref<Drawable>& src)
{
	if (src.isNull()) {
		return;
	}
	sl_real sw = src->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = src->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	Rectangle rectDst(xDst, yDst, xDst + sw, yDst + sh);
	onDrawAll(rectDst, src, _g_globalDefaultDrawParam);
}

void Canvas::draw(const Rectangle& rectDst, const Ref<Drawable>& source, ScaleMode scaleMode, Alignment alignment, const DrawParam& param)
{
	if (source.isNull()) {
		return;
	}
	if (param.isTransparent()) {
		return;
	}
	sl_real dw = rectDst.getWidth();
	if (dw < SLIB_EPSILON) {
		return;
	}
	sl_real dh = rectDst.getHeight();
	if (dh < SLIB_EPSILON) {
		return;
	}
	sl_real sw = source->getDrawableWidth();
	if (sw < SLIB_EPSILON) {
		return;
	}
	sl_real sh = source->getDrawableHeight();
	if (sh < SLIB_EPSILON) {
		return;
	}
	
	switch (scaleMode) {
		case ScaleMode::None:
			{
				Point pt = GraphicsUtil::calculateAlignPosition(rectDst, sw, sh, alignment);
				Rectangle rectDraw;
				rectDraw.left = pt.x;
				rectDraw.top = pt.y;
				rectDraw.right = rectDraw.left + sw;
				rectDraw.bottom = rectDraw.top + sh;
				onDrawAll(rectDraw, source, param);
				break;
			}
		case ScaleMode::Stretch:
			{
				onDrawAll(rectDst, source, param);
				break;
			}
		case ScaleMode::Contain:
			{
				sl_real fw = dw / sw;
				sl_real fh = dh / sh;
				sl_real tw, th;
				if (fw > fh) {
					th = dh;
					tw = sw * fh;
				} else {
					tw = dw;
					th = sh * fw;
				}
				Point pt = GraphicsUtil::calculateAlignPosition(rectDst, tw, th, alignment);
				Rectangle rectDraw;
				rectDraw.left = pt.x;
				rectDraw.top = pt.y;
				rectDraw.right = rectDraw.left + tw;
				rectDraw.bottom = rectDraw.top + th;
				onDrawAll(rectDraw, source, param);
				break;
			}
		case ScaleMode::Cover:
			{
				sl_real fw = sw / dw;
				sl_real fh = sh / dh;
				sl_real tw, th;
				if (fw > fh) {
					th = sh;
					tw = dw * fh;
				} else {
					tw = sw;
					th = dh * fw;
				}
				Rectangle rectSrc;
				rectSrc.left = 0;
				rectSrc.top = 0;
				rectSrc.right = sw;
				rectSrc.bottom = sh;
				Point pt = GraphicsUtil::calculateAlignPosition(rectSrc, tw, th, alignment);
				rectSrc.left = pt.x;
				rectSrc.top = pt.y;
				rectSrc.right = rectSrc.left + tw;
				rectSrc.bottom = rectSrc.top + th;
				onDraw(rectDst, source, rectSrc, param);
				break;
			}
	}

}

void Canvas::draw(const Rectangle& rectDst, const Ref<Drawable>& source, ScaleMode scaleMode, Alignment alignment)
{
	draw(rectDst, source, scaleMode, alignment, _g_globalDefaultDrawParam);
}

void Canvas::onDraw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc, const DrawParam& param)
{
	src->onDraw(this, rectDst, rectSrc, param);
}

void Canvas::onDrawAll(const Rectangle& rectDst, const Ref<Drawable>& src, const DrawParam& param)
{
	src->onDrawAll(this, rectDst, param);
}

CanvasStatusScope::CanvasStatusScope()
{
}

CanvasStatusScope::CanvasStatusScope(const Ref<Canvas>& canvas)
{
	save(canvas);
}

CanvasStatusScope::~CanvasStatusScope()
{
	restore();
}

void CanvasStatusScope::save(const Ref<Canvas>& canvas)
{
	restore();
	if (canvas.isNotNull()) {
		canvas->save();
	}
	m_canvas = canvas;
}

void CanvasStatusScope::restore()
{
	Ref<Canvas> canvas = m_canvas;
	if (canvas.isNotNull()) {
		canvas->restore();
		m_canvas.setNull();
	}
}

Ref<Canvas> CanvasStatusScope::getCanvas()
{
	return m_canvas;
}

SLIB_GRAPHICS_NAMESPACE_END
