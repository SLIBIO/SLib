#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/path.h"
#include "../../../inc/slib/graphics/platform.h"

#include "../../../inc/slib/core/scoped.h"

#pragma comment(lib, "gdiplus.lib")

SLIB_GRAPHICS_NAMESPACE_BEGIN

static Gdiplus::FillMode _Gdiplus_convertFillMode(FillMode _mode)
{
	Gdiplus::FillMode fillMode;
	switch (_mode) {
	case FillMode::Winding:
		fillMode = Gdiplus::FillModeWinding;
		break;
	case FillMode::Alternate:
	default:
		fillMode = Gdiplus::FillModeAlternate;
		break;
	}
	return fillMode;
}

class _Gdiplus_GraphicsPathObject : public Referable
{
public:
	Gdiplus::GraphicsPath* path;
	sl_real lastX;
	sl_real lastY;

public:
	_Gdiplus_GraphicsPathObject()
	{
		path = new Gdiplus::GraphicsPath(Gdiplus::FillModeWinding);
		lastX = 0;
		lastY = 0;
	}

	~_Gdiplus_GraphicsPathObject()
	{
		if (path) {
			delete path;
		}
	}

};


void GraphicsPath::_initialize_PO()
{
	Ref<_Gdiplus_GraphicsPathObject> po = new _Gdiplus_GraphicsPathObject;
	if (po.isNotNull() && po->path) {
		m_platformObject = po;
	}
}

void GraphicsPath::_moveTo_PO(sl_real x, sl_real y)
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		po->lastX = x;
		po->lastY = y;
	}
}

void GraphicsPath::_lineTo_PO(sl_real x, sl_real y)
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		po->path->AddLine((Gdiplus::REAL)(po->lastX), (Gdiplus::REAL)(po->lastY), (Gdiplus::REAL)(x), (Gdiplus::REAL)(y));
		po->lastX = x;
		po->lastY = y;
	}
}

void GraphicsPath::_cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		po->path->AddBezier(
			(Gdiplus::REAL)(po->lastX), (Gdiplus::REAL)(po->lastY), 
			(Gdiplus::REAL)(xc1), (Gdiplus::REAL)(yc1),
			(Gdiplus::REAL)(xc2), (Gdiplus::REAL)(yc2),
			(Gdiplus::REAL)(xe), (Gdiplus::REAL)(ye)
		);
		po->lastX = xe;
		po->lastY = ye;
	}
}

void GraphicsPath::_closeSubpath_PO()
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		po->path->CloseFigure();
	}
}

void GraphicsPath::_setFillMode_PO(FillMode mode)
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		po->path->SetFillMode(_Gdiplus_convertFillMode(mode));
	}
}

Rectangle GraphicsPath::_getBounds_PO()
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		Rectangle ret;
		Gdiplus::RectF rc;
		po->path->GetBounds(&rc);
		ret.left = (sl_real)(rc.X);
		ret.top = (sl_real)(rc.Y);
		ret.right = ret.left + (sl_real)(rc.Width);
		ret.bottom = ret.top + (sl_real)(rc.Height);
		return ret;		
	}
	return Rectangle::zero();
}

sl_bool GraphicsPath::_containsPoint_PO(sl_real x, sl_real y)
{
	_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
	if (po) {
		return (po->path->IsVisible((Gdiplus::REAL)x, (Gdiplus::REAL)y)) != FALSE;
	}
	return sl_false;
}

class _GraphicsPath : public GraphicsPath
{
public:
	Gdiplus::GraphicsPath* createHandle()
	{
		ListItems<GraphicsPathPoint> points(this->points);
		sl_size n = points.count;
		if (n > 0) {
			SLIB_SCOPED_BUFFER(Gdiplus::PointF, 1024, pts, n);
			SLIB_SCOPED_BUFFER(BYTE, 1024, types, n);
			for (sl_size i = 0; i < n; i++) {
				pts[i].X = points[i].pt.x;
				pts[i].Y = points[i].pt.y;
				types[i] = points[i].type;
			}
			Gdiplus::GraphicsPath* handle = new Gdiplus::GraphicsPath(pts, types, (INT)n, _Gdiplus_convertFillMode(getFillMode()));
			return handle;
		}
		return NULL;
	}

	Gdiplus::GraphicsPath* getPlatformPath()
	{
		_Gdiplus_GraphicsPathObject* po = (_Gdiplus_GraphicsPathObject*)(m_platformObject.ptr);
		if (po) {
			return po->path;
		}
		return NULL;
	}
};

Gdiplus::GraphicsPath* GraphicsPlatform::getGraphicsPath(GraphicsPath* path)
{
	if (path) {
		return ((_GraphicsPath*)path)->getPlatformPath();
	}
	return NULL;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
