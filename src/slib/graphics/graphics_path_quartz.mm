/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/path.h"

#include "../../../inc/slib/graphics/platform.h"

namespace slib
{

	class _Quartz_GraphicsPathObject : public Referable
	{
	public:
		CGMutablePathRef path;
		
	public:
		_Quartz_GraphicsPathObject()
		{
			path = CGPathCreateMutable();
		}
		
		~_Quartz_GraphicsPathObject()
		{
			if (path) {
				CGPathRelease(path);
			}
		}
		
	};

	void GraphicsPath::_initialize_PO()
	{
		Ref<_Quartz_GraphicsPathObject> po = new _Quartz_GraphicsPathObject;
		if (po.isNotNull() && po->path) {
			m_platformObject = po;
		}
	}

	void GraphicsPath::_moveTo_PO(sl_real x, sl_real y)
	{
		_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			CGPathMoveToPoint(po->path, NULL, x, y);
		}
	}

	void GraphicsPath::_lineTo_PO(sl_real x, sl_real y)
	{
		_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			CGPathAddLineToPoint(po->path, NULL, x, y);
		}
	}

	void GraphicsPath::_cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
	{
		_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			CGPathAddCurveToPoint(po->path, NULL, xc1, yc1, xc2, yc2, xe, ye);
		}
	}

	void GraphicsPath::_closeSubpath_PO()
	{
		_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			CGPathCloseSubpath(po->path);
		}
	}

	void GraphicsPath::_setFillMode_PO(FillMode mode)
	{
	}

	Rectangle GraphicsPath::_getBounds_PO()
	{
		_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
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
		_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			CGPoint pt;
			pt.x = x;
			pt.y = y;
			return CGPathContainsPoint(po->path, NULL, pt, m_fillMode != FillMode::Winding);
		}
		return sl_false;
	}

	class _GraphicsPath : public GraphicsPath
	{
	public:
		CGPathRef getPlatformPath()
		{
			_Quartz_GraphicsPathObject* po = (_Quartz_GraphicsPathObject*)(m_platformObject.get());
			if (po) {
				return po->path;
			}
			return NULL;
		}
	};

	CGPathRef GraphicsPlatform::getGraphicsPath(GraphicsPath* path)
	{
		if (path) {
			return ((_GraphicsPath*)path)->getPlatformPath();
		}
		return NULL;
	}

}

#endif
