/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_GRAPHICS_IS_GDI)

#include "slib/graphics/path.h"

#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

#pragma comment(lib, "gdiplus.lib")

namespace slib
{

	static Gdiplus::FillMode _priv_Gdiplus_convertFillMode(FillMode _mode)
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

	class _priv_Gdiplus_GraphicsPathObject : public Referable
	{
	public:
		Gdiplus::GraphicsPath* path;
		sl_real lastX;
		sl_real lastY;

	public:
		_priv_Gdiplus_GraphicsPathObject()
		{
			path = new Gdiplus::GraphicsPath(Gdiplus::FillModeWinding);
			lastX = 0;
			lastY = 0;
		}

		~_priv_Gdiplus_GraphicsPathObject()
		{
			delete path;
		}

	};


	void GraphicsPath::_initialize_PO()
	{
		Ref<_priv_Gdiplus_GraphicsPathObject> po = new _priv_Gdiplus_GraphicsPathObject;
		if (po.isNotNull() && po->path) {
			m_platformObject = po;
		}
	}

	void GraphicsPath::_moveTo_PO(sl_real x, sl_real y)
	{
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			po->lastX = x;
			po->lastY = y;
		}
	}

	void GraphicsPath::_lineTo_PO(sl_real x, sl_real y)
	{
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			po->path->AddLine((Gdiplus::REAL)(po->lastX), (Gdiplus::REAL)(po->lastY), (Gdiplus::REAL)(x), (Gdiplus::REAL)(y));
			po->lastX = x;
			po->lastY = y;
		}
	}

	void GraphicsPath::_cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
	{
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
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
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			po->path->CloseFigure();
		}
	}

	void GraphicsPath::_setFillMode_PO(FillMode mode)
	{
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			po->path->SetFillMode(_priv_Gdiplus_convertFillMode(mode));
		}
	}

	Rectangle GraphicsPath::_getBounds_PO()
	{
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
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
		_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			return (po->path->IsVisible((Gdiplus::REAL)x, (Gdiplus::REAL)y)) != FALSE;
		}
		return sl_false;
	}

	class _priv_GraphicsPath : public GraphicsPath
	{
	public:
		Gdiplus::GraphicsPath* createHandle()
		{
			ListElements<GraphicsPathPoint> points(m_points);
			sl_size n = points.count;
			if (n > 0) {
				SLIB_SCOPED_BUFFER(Gdiplus::PointF, 1024, pts, n);
				SLIB_SCOPED_BUFFER(BYTE, 1024, types, n);
				for (sl_size i = 0; i < n; i++) {
					pts[i].X = points[i].pt.x;
					pts[i].Y = points[i].pt.y;
					types[i] = points[i].type;
				}
				Gdiplus::GraphicsPath* handle = new Gdiplus::GraphicsPath(pts, types, (INT)n, _priv_Gdiplus_convertFillMode(getFillMode()));
				return handle;
			}
			return NULL;
		}

		Gdiplus::GraphicsPath* getPlatformPath()
		{
			_priv_Gdiplus_GraphicsPathObject* po = (_priv_Gdiplus_GraphicsPathObject*)(m_platformObject.get());
			if (po) {
				return po->path;
			}
			return NULL;
		}
	};

	Gdiplus::GraphicsPath* GraphicsPlatform::getGraphicsPath(GraphicsPath* path)
	{
		if (path) {
			return ((_priv_GraphicsPath*)path)->getPlatformPath();
		}
		return NULL;
	}

}

#endif
