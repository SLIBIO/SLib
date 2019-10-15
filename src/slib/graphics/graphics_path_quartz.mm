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

#if defined(SLIB_GRAPHICS_IS_QUARTZ)

#include "slib/graphics/path.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace quartz
		{

			class GraphicsPathPlatformObject : public Referable
			{
			public:
				CGMutablePathRef path;
				
			public:
				GraphicsPathPlatformObject()
				{
					path = CGPathCreateMutable();
				}
				
				~GraphicsPathPlatformObject()
				{
					if (path) {
						CGPathRelease(path);
					}
				}
				
			};

			class GraphicsPathHelper : public GraphicsPath
			{
			public:
				CGPathRef getPlatformPath()
				{
					GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
					if (po) {
						return po->path;
					}
					return NULL;
				}
			};

		}
	}

	using namespace priv::quartz;

	sl_bool GraphicsPath::_initialize_PO()
	{
		Ref<GraphicsPathPlatformObject> po = new GraphicsPathPlatformObject;
		if (po.isNotNull() && po->path) {
			m_platformObject = po;
			return sl_true;
		}
		return sl_false;
	}

	void GraphicsPath::_moveTo_PO(sl_real x, sl_real y)
	{
		GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
		if (po) {
			CGPathMoveToPoint(po->path, NULL, x, y);
		}
	}

	void GraphicsPath::_lineTo_PO(sl_real x, sl_real y)
	{
		GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
		if (po) {
			CGPathAddLineToPoint(po->path, NULL, x, y);
		}
	}

	void GraphicsPath::_cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
	{
		GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
		if (po) {
			CGPathAddCurveToPoint(po->path, NULL, xc1, yc1, xc2, yc2, xe, ye);
		}
	}

	void GraphicsPath::_closeSubpath_PO()
	{
		GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
		if (po) {
			CGPathCloseSubpath(po->path);
		}
	}

	void GraphicsPath::_setFillMode_PO(FillMode mode)
	{
	}

	Rectangle GraphicsPath::_getBounds_PO()
	{
		GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
		if (po) {
			Rectangle ret;
			CGRect rc = CGPathGetPathBoundingBox(po->path);
			ret.left = (sl_real)(rc.origin.x);
			ret.top = (sl_real)(rc.origin.y);
			ret.right = ret.left + (sl_real)(rc.size.width);
			ret.bottom = ret.top + (sl_real)(rc.size.height);
			return ret;
		}
		return Rectangle::zero();
	}

	sl_bool GraphicsPath::_containsPoint_PO(sl_real x, sl_real y)
	{
		GraphicsPathPlatformObject* po = (GraphicsPathPlatformObject*)(m_platformObject.get());
		if (po) {
			CGPoint pt;
			pt.x = x;
			pt.y = y;
			return CGPathContainsPoint(po->path, NULL, pt, m_fillMode != FillMode::Winding);
		}
		return sl_false;
	}

	CGPathRef GraphicsPlatform::getGraphicsPath(GraphicsPath* path)
	{
		if (path) {
			return ((GraphicsPathHelper*)path)->getPlatformPath();
		}
		return NULL;
	}

}

#endif
