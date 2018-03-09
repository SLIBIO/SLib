/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/graphics/pen.h"

#include "slib/graphics/platform.h"

#pragma comment(lib, "gdiplus.lib")

namespace slib
{

	class _priv_Gdiplus_PenObject : public Referable
	{
	public:
		Gdiplus::Pen* m_pen;

	public:
		_priv_Gdiplus_PenObject(const PenDesc& desc)
		{
			m_pen = NULL;
			const Color& _color = desc.color;
			Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
			Gdiplus::Pen* pen = new Gdiplus::Pen(color, desc.width);
			if (pen) {
				Gdiplus::LineCap cap;
				Gdiplus::DashCap dashCap;
				switch (desc.cap) {
				case LineCap::Square:
					cap = Gdiplus::LineCapSquare;
					dashCap = Gdiplus::DashCapFlat;
					break;
				case LineCap::Round:
					cap = Gdiplus::LineCapRound;
					dashCap = Gdiplus::DashCapRound;
					break;
				case LineCap::Flat:
				default:
					cap = Gdiplus::LineCapFlat;
					dashCap = Gdiplus::DashCapFlat;
					break;
				}
				pen->SetLineCap(cap, cap, dashCap);

				Gdiplus::LineJoin join;
				switch (desc.join) {
				case LineJoin::Bevel:
					join = Gdiplus::LineJoinBevel;
					break;
				case LineJoin::Round:
					join = Gdiplus::LineJoinRound;
					break;
				case LineJoin::Miter:
				default:
					join = Gdiplus::LineJoinMiter;
					pen->SetMiterLimit(desc.miterLimit);
					break;
				}
				pen->SetLineJoin(join);
				
				Gdiplus::DashStyle style;
				switch (desc.style) {
				case PenStyle::Dot:
					style = Gdiplus::DashStyleDot;
					break;
				case PenStyle::Dash:
					style = Gdiplus::DashStyleDash;
					break;
				case PenStyle::DashDot:
					style = Gdiplus::DashStyleDashDot;
					break;
				case PenStyle::DashDotDot:
					style = Gdiplus::DashStyleDashDotDot;
					break;
				case PenStyle::Solid:
				default:
					style = Gdiplus::DashStyleSolid;
					break;
				}
				pen->SetDashStyle(style);
				m_pen = pen;
			}
		}

		~_priv_Gdiplus_PenObject()
		{
			delete m_pen;
		}

	};

	class _priv_Pen : public Pen
	{
	public:
		_priv_Gdiplus_PenObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new _priv_Gdiplus_PenObject(m_desc);
				}
			}
			return (_priv_Gdiplus_PenObject*)(m_platformObject.get());;
		}

		Gdiplus::Pen* getPlatformHandle()
		{
			_priv_Gdiplus_PenObject* po = getPlatformObject();
			if (po) {
				return po->m_pen;
			}
			return NULL;
		}
	};

	Gdiplus::Pen* GraphicsPlatform::getPenHandle(Pen* pen)
	{
		if (pen) {
			return ((_priv_Pen*)pen)->getPlatformHandle();
		}
		return NULL;
	}

}

#endif
