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

#include "slib/graphics/pen.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace gdi
		{

			class PenPlatformObject : public Referable
			{
			public:
				Gdiplus::Pen* m_pen;

			public:
				PenPlatformObject(const PenDesc& desc)
				{
					m_pen = NULL;
					const Color& _color = desc.color;
					Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);

					GraphicsPlatform::startGdiplus();
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

				~PenPlatformObject()
				{
					delete m_pen;
				}

			};


			class PenHelper : public Pen
			{
			public:
				PenPlatformObject* getPenPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new PenPlatformObject(m_desc);
						}
					}
					return (PenPlatformObject*)(m_platformObject.get());;
				}

				Gdiplus::Pen* getPlatformHandle()
				{
					PenPlatformObject* po = getPenPlatformObject();
					if (po) {
						return po->m_pen;
					}
					return NULL;
				}
			};

		}
	}

	Gdiplus::Pen* GraphicsPlatform::getPenHandle(Pen* pen)
	{
		if (pen) {
			return ((priv::gdi::PenHelper*)pen)->getPlatformHandle();
		}
		return NULL;
	}

}

#endif
