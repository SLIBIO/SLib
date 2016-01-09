#include "../../../inc/slib/graphics/path.h"

#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/math/bezier.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
GraphicsPath::GraphicsPath()
{
	setFillMode(fillModeWinding);
	m_flagBegan = sl_false;
	m_pointBegin = Point::zero();
}

Ref<GraphicsPath> GraphicsPath::create()
{
	return new GraphicsPath;
}

void GraphicsPath::moveTo(const Point& pt)
{
	ObjectLocker lock(this);
	m_flagBegan = sl_false;
	m_pointBegin = pt;
	invalidate();
}

void GraphicsPath::lineTo(const Point& pt)
{
	ObjectLocker lock(this);
	if (!m_flagBegan) {
		GraphicsPathPoint point;
		point.pt = m_pointBegin;
		point.type = graphicsPathPointTypeBegin;
		points.add(point);
		m_flagBegan = sl_true;
	}
	GraphicsPathPoint point;
	point.pt = pt;
	point.type = graphicsPathPointTypeLine;
	points.add(point);
	invalidate();
}

void GraphicsPath::cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd)
{
	ObjectLocker lock(this);
	if (!m_flagBegan) {
		GraphicsPathPoint point;
		point.pt = m_pointBegin;
		point.type = graphicsPathPointTypeBegin;
		points.add(point);
		m_flagBegan = sl_true;
	}
	GraphicsPathPoint point;
	point.pt = ptControl1;
	point.type = graphicsPathPointTypeBezierCubic;
	points.add(point);
	point.pt = ptControl2;
	point.type = graphicsPathPointTypeBezierCubic;
	points.add(point);
	point.pt = ptEnd;
	point.type = graphicsPathPointTypeBezierCubic;
	points.add(point);
	invalidate();
}

void GraphicsPath::closeSubpath()
{
	ObjectLocker lock(this);
	if (m_flagBegan) {
		sl_size n = points.count();
		if (n > 0) {
			points[n - 1].type |= (sl_uint8)graphicsPathPointFlagClose;
			m_pointBegin = points[n - 1].pt;
			m_flagBegan = sl_false;
		}
		invalidate();
	}
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

void GraphicsPath::addRectangle(const Rectangle& rect)
{
	moveTo(rect.left, rect.top);
	lineTo(rect.right, rect.top);
	lineTo(rect.right, rect.bottom);
	lineTo(rect.left, rect.bottom);
	closeSubpath();
}

void GraphicsPath::addRoundRect(const Rectangle& rect, const Size& radius)
{
	sl_real rx = radius.x;
	sl_real ry = radius.y;
	float x = rect.left;
	float y = rect.top;
	float w = rect.getWidth();
	float h = rect.getHeight();
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

void GraphicsPath::addEllipse(const Rectangle& rect)
{
	addArc(rect, 0, 360, sl_true);
	closeSubpath();
}

void GraphicsPath::addPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees)
{
	moveTo(rect.center());
	addArc(rect, startDegrees, sweepDegrees, sl_false);
	closeSubpath();
}


Rectangle GraphicsPath::getBounds(const Ref<GraphicsContext>& context)
{
	if (context.isNotNull()) {
		return context->getPathBounds(this);
	}
	return Rectangle::zero();
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
	m_instance.setNull();
}


const Ref<GraphicsPathInstance>& GraphicsPath::getInstance()
{
	return m_instance;
}

void GraphicsPath::setInstance(const Ref<GraphicsPathInstance>& instance)
{
	m_instance = instance;
}

GraphicsPathInstance::GraphicsPathInstance()
{
}

void GraphicsPathInstance::buildFrom(const Ref<GraphicsPath>& path)
{
	if (path.isNull()) {
		return;
	}

	ListLocker<GraphicsPathPoint> points(path->points);
	sl_uint32 nCubicCount = 0;
	for (sl_size i = 0; i < points.count(); i++) {
		GraphicsPathPoint& point = points[i];
		sl_uint8 t = point.type & 0x7f;
		if (t == graphicsPathPointTypeBegin) {
			this->moveTo(point.pt);
			nCubicCount = 0;
		} else if (t == graphicsPathPointTypeLine) {
			this->lineTo(point.pt);
			nCubicCount = 0;
		} else if (t == graphicsPathPointTypeBezierCubic) {
			if (nCubicCount == 2) {
				this->cubicTo(points[i - 2].pt, points[i - 1].pt, points[i].pt);
				nCubicCount = 0;
			} else {
				nCubicCount++;
			}
		}
		if (point.type & graphicsPathPointFlagClose) {
			this->closeSubpath();
		}
	}
}
SLIB_GRAPHICS_NAMESPACE_END


