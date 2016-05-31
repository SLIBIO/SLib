#include "../../../inc/slib/graphics/path.h"

#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/math/bezier.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(GraphicsPath, Object)

GraphicsPath::GraphicsPath()
{
	m_fillMode = FillMode::Winding;
	m_flagBegan = sl_false;
	m_pointBegin = Point::zero();
}

Ref<GraphicsPath> GraphicsPath::create()
{
	return new GraphicsPath;
}

void GraphicsPath::moveTo(sl_real x, sl_real y)
{
	ObjectLocker lock(this);
	m_flagBegan = sl_false;
	m_pointBegin.x = x;
	m_pointBegin.y = y;
	invalidate();
}

void GraphicsPath::moveTo(const Point& pt)
{
	moveTo(pt.x, pt.y);
}

void GraphicsPath::lineTo(sl_real x, sl_real y)
{
	ObjectLocker lock(this);
	if (!m_flagBegan) {
		GraphicsPathPoint point;
		point.pt = m_pointBegin;
		point.type = GraphicsPathPoint::Begin;
		points.add(point);
		m_flagBegan = sl_true;
	}
	GraphicsPathPoint point;
	point.pt.x = x;
	point.pt.y = y;
	point.type = GraphicsPathPoint::Line;
	points.add(point);
	invalidate();
}

void GraphicsPath::lineTo(const Point& pt)
{
	lineTo(pt.x, pt.y);
}

void GraphicsPath::cubicTo(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
{
	ObjectLocker lock(this);
	if (!m_flagBegan) {
		GraphicsPathPoint point;
		point.pt = m_pointBegin;
		point.type = GraphicsPathPoint::Begin;
		points.add(point);
		m_flagBegan = sl_true;
	}
	GraphicsPathPoint point;
	point.pt.x = xc1;
	point.pt.y = yc1;
	point.type = GraphicsPathPoint::BezierCubic;
	points.add(point);
	point.pt.x = xc2;
	point.pt.y = yc2;
	point.type = GraphicsPathPoint::BezierCubic;
	points.add(point);
	point.pt.x = xe;
	point.pt.y = ye;
	point.type = GraphicsPathPoint::BezierCubic;
	points.add(point);
	invalidate();
}

void GraphicsPath::cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd)
{
	cubicTo(ptControl1.x, ptControl1.y, ptControl2.x, ptControl2.y, ptEnd.x, ptEnd.y);
}

void GraphicsPath::closeSubpath()
{
	ObjectLocker lock(this);
	if (m_flagBegan) {
		sl_size n = points.getCount();
		if (n > 0) {
			GraphicsPathPoint* list = points.getData();
			list[n - 1].type |= GraphicsPathPoint::FlagClose;
			m_pointBegin = list[n - 1].pt;
			m_flagBegan = sl_false;
		}
		invalidate();
	}
}

void GraphicsPath::addArc(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, sl_bool flagMoveTo)
{
	Rectangle rect(x, y, x + width, y + height);
	addArc(rect, startDegrees, sweepDegrees, flagMoveTo);
}

void GraphicsPath::addArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, sl_bool flagMoveTo)
{
	Point pts[13];
	sl_uint32 n = CubicBezierCurve::convertArcToBezier(pts, rect, startDegrees, sweepDegrees);
	if (flagMoveTo) {
		moveTo(pts[0]);
	} else {
		lineTo(pts[0]);
	}
	for (sl_uint32 i = 1; i + 2 < n; i += 3) {
		cubicTo(pts[i], pts[i + 1], pts[i + 2]);
	}
}

void GraphicsPath::addRectangle(sl_real x, sl_real y, sl_real width, sl_real height)
{
	Rectangle rect(x, y, x + width, y + height);
	addRectangle(rect);
}

void GraphicsPath::addRectangle(const Rectangle& rect)
{
	moveTo(rect.left, rect.top);
	lineTo(rect.right, rect.top);
	lineTo(rect.right, rect.bottom);
	lineTo(rect.left, rect.bottom);
	closeSubpath();
}

void GraphicsPath::addRoundRect(sl_real x, sl_real y, sl_real w, sl_real h, sl_real rx, sl_real ry)
{
	float rw = rx * 2;
	float rh = ry * 2;
	float xr = x + w - rw;
	float yb = y + h - rh;
	float k0 = 0.5f;
	float k1 = 0.77614234f;
	float k2 = 0.22385763f;
	moveTo(xr + rw, yb + k0 * rh);
	cubicTo(xr + rw, yb + k1 * rh, xr + k1 * rw, yb + rh, xr + k0 * rw, yb + rh);
	lineTo(x + k0 * rw, yb + rh);
	cubicTo(x + k2 * rw, yb + rh, x, yb + k1 * rh, x, yb + k0 * rh);
	lineTo(x, y + k0 * rh);
	cubicTo(x, y + k2 * rh, x + k2 * rw, y, x + k0 * rw, y);
	lineTo(xr + k0 * rw, y);
	cubicTo(xr + k1 * rw, y, xr + rw, y + k2 * rh, xr + rw, y + k0 * rh);
	//lineTo(xr + rw, yb + k0 * rh);
	closeSubpath();
}

void GraphicsPath::addRoundRect(const Rectangle& rect, const Size& radius)
{
	addRoundRect(rect.left, rect.top, rect.getWidth(), rect.getHeight(), radius.x, radius.y);
}

void GraphicsPath::addEllipse(sl_real x, sl_real y, sl_real width, sl_real height)
{
	Rectangle rect(x, y, x + width, y + height);
	addArc(rect, 0, 360, sl_true);
	closeSubpath();

}

void GraphicsPath::addEllipse(const Rectangle& rect)
{
	addArc(rect, 0, 360, sl_true);
	closeSubpath();
}

void GraphicsPath::addPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees)
{
	Rectangle rect(x, y, x + width, y + height);
	addPie(rect, startDegrees, sweepDegrees);
}

void GraphicsPath::addPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees)
{
	moveTo(rect.getCenter());
	addArc(rect, startDegrees, sweepDegrees, sl_false);
	closeSubpath();
}

FillMode GraphicsPath::getFillMode()
{
	return m_fillMode;
}

void GraphicsPath::setFillMode(FillMode mode)
{
	m_fillMode = mode;
}

Rectangle GraphicsPath::getBounds(const Ref<GraphicsContext>& context)
{
	if (context.isNotNull()) {
		return context->getPathBounds(this);
	}
	return Rectangle::zero();
}

sl_bool GraphicsPath::containsPoint(const Ref<GraphicsContext>& context, sl_real x, sl_real y)
{
	return containsPoint(context, Point(x, y));
}

sl_bool GraphicsPath::containsPoint(const Ref<GraphicsContext>& context, const Point& pt)
{
	if (context.isNotNull()) {
		return context->checkPointInPath(this, pt);
	}
	return sl_false;
}

void GraphicsPath::invalidate()
{
	setInstance(Ref<GraphicsPathInstance>::null());
}

sl_bool GraphicsPath::containsPointInRoundRect(const Point& pt, const Rectangle& rect, const Size& radius)
{
	sl_real x = pt.x;
	sl_real y = pt.y;
	if (x < rect.left) {
		return sl_false;
	}
	if (x > rect.right) {
		return sl_false;
	}
	if (y < rect.top) {
		return sl_false;
	}
	if (y > rect.bottom) {
		return sl_false;
	}
	x -= rect.left;
	if (x > radius.x) {
		sl_real t = rect.right - rect.left - radius.x;
		if (x < t) {
			return sl_true;
		}
		x = radius.x + x - t;
	}
	y -= rect.top;
	if (y > radius.y) {
		sl_real t = rect.bottom - rect.top - radius.y;
		if (y < t) {
			return sl_true;
		}
		y = radius.y + y - t;
	}
	if (Math::isNearZero(radius.x)) {
		return sl_false;
	}
	if (Math::isNearZero(radius.y)) {
		return sl_false;
	}
	sl_real rx2 = radius.x * radius.x;
	sl_real ry2 = radius.y * radius.y;
	return x * x * ry2 + y * y * rx2 <= rx2 * ry2;
}

sl_bool GraphicsPath::containsPointInEllipse(const Point& pt, const Rectangle& rect)
{
	sl_real rx = rect.right - rect.left;
	if (Math::isNearZero(rx)) {
		return sl_false;
	}
	sl_real ry = rect.bottom - rect.top;
	if (Math::isNearZero(ry)) {
		return sl_false;
	}
	sl_real rx2 = rx * rx;
	sl_real ry2 = ry * ry;
	sl_real x = (pt.x - (rect.right + rect.left) / 2);
	sl_real y = (pt.y - (rect.top + rect.bottom) / 2);
	return 4 * (x * x * ry2 + y * y * rx2) <= rx2 * ry2;
}


SLIB_DEFINE_OBJECT(GraphicsPathInstance, Object)

void GraphicsPathInstance::buildFrom(const Ref<GraphicsPath>& path)
{
	if (path.isNull()) {
		return;
	}
	ListLocker<GraphicsPathPoint> points(path->points);
	sl_uint32 nCubicCount = 0;
	for (sl_size i = 0; i < points.count; i++) {
		GraphicsPathPoint& point = points[i];
		sl_uint8 t = point.type & 0x7f;
		if (t == GraphicsPathPoint::Begin) {
			this->moveTo(point.pt);
			nCubicCount = 0;
		} else if (t == GraphicsPathPoint::Line) {
			this->lineTo(point.pt);
			nCubicCount = 0;
		} else if (t == GraphicsPathPoint::BezierCubic) {
			if (nCubicCount == 2) {
				this->cubicTo(points[i - 2].pt, points[i - 1].pt, points[i].pt);
				nCubicCount = 0;
			} else {
				nCubicCount++;
			}
		}
		if (point.type & GraphicsPathPoint::FlagClose) {
			this->closeSubpath();
		}
	}
}

SLIB_GRAPHICS_NAMESPACE_END


