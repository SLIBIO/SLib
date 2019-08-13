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

#include "slib/graphics/brush.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace gdi
		{

			class BrushPlatformObject : public Referable
			{
			public:
				Gdiplus::Brush* m_brush;

			public:
				BrushPlatformObject(const BrushDesc& desc)
				{
					m_brush = NULL;
					const Color& _color = desc.color;
					Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
					if (desc.style == BrushStyle::Solid) {
						GraphicsPlatform::startGdiplus();
						Gdiplus::Brush* brush = new Gdiplus::SolidBrush(color);
						if (brush) {
							m_brush = brush;
						}
					}
				}

				~BrushPlatformObject()
				{
					delete m_brush;
				}

			};

			class BrushHelper : public Brush
			{
			public:
				BrushPlatformObject* getPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new BrushPlatformObject(m_desc);
						}
					}
					return (BrushPlatformObject*)(m_platformObject.get());;
				}

				Gdiplus::Brush* getPlatformHandle()
				{
					BrushPlatformObject* po = getPlatformObject();
					if (po) {
						return po->m_brush;
					}
					return NULL;
				}
			};

		}
	}

	using namespace priv::gdi;

	Gdiplus::Brush* GraphicsPlatform::getBrushHandle(Brush* brush)
	{
		if (brush) {
			return ((BrushHelper*)brush)->getPlatformHandle();
		}
		return NULL;
	}

}

#endif
