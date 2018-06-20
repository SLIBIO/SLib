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

#if defined(SLIB_GRAPHICS_IS_GDI)

#include "slib/graphics/brush.h"

#include "slib/graphics/platform.h"

#pragma comment(lib, "gdiplus.lib")

namespace slib
{

	class Gdiplus_BrushObject : public Referable
	{
	public:
		Gdiplus::Brush* m_brush;

	public:
		Gdiplus_BrushObject(const BrushDesc& desc)
		{
			m_brush = NULL;
			const Color& _color = desc.color;
			Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
			if (desc.style == BrushStyle::Solid) {
				Gdiplus::Brush* brush = new Gdiplus::SolidBrush(color);
				if (brush) {
					m_brush = brush;
				}
			}
		}

		~Gdiplus_BrushObject()
		{
			delete m_brush;
		}

	};

	class Brush_Ext : public Brush
	{
	public:
		Gdiplus_BrushObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new Gdiplus_BrushObject(m_desc);
				}
			}
			return (Gdiplus_BrushObject*)(m_platformObject.get());;
		}

		Gdiplus::Brush* getPlatformHandle()
		{
			Gdiplus_BrushObject* po = getPlatformObject();
			if (po) {
				return po->m_brush;
			}
			return NULL;
		}
	};

	Gdiplus::Brush* GraphicsPlatform::getBrushHandle(Brush* brush)
	{
		if (brush) {
			return ((Brush_Ext*)brush)->getPlatformHandle();
		}
		return NULL;
	}

}

#endif
