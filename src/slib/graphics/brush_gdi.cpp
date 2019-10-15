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

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

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
				Ref<Drawable> m_drawableCache;

			public:
				BrushPlatformObject(const BrushDesc& desc)
				{
					GraphicsPlatform::startGdiplus();
					m_brush = NULL;
					if (desc.style == BrushStyle::Solid) {
						const Color& _color = desc.color;
						Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
						m_brush = new Gdiplus::SolidBrush(color);
					} else if (desc.style == BrushStyle::LinearGradient || desc.style == BrushStyle::RadialGradient) {
						GradientBrushDetail* detail = (GradientBrushDetail*)(desc.detail.get());
						if (detail) {
							ListElements<Color> _colors(detail->colors);
							ListElements<sl_real> _locations(detail->locations);
							sl_size n = _colors.count;
							SLIB_SCOPED_BUFFER(Gdiplus::Color, 128, colors, n);
							SLIB_SCOPED_BUFFER(Gdiplus::REAL, 128, locations, n);
							if (desc.style == BrushStyle::LinearGradient) {
								for (sl_size i = 0; i < n; i++) {
									colors[i] = Gdiplus::Color(_colors[i].a, _colors[i].r, _colors[i].g, _colors[i].b);
									locations[i] = (Gdiplus::REAL)(_locations[i]);
								}
								Gdiplus::PointF pt1((Gdiplus::REAL)(detail->point1.x), (Gdiplus::REAL)(detail->point1.y));
								Gdiplus::PointF pt2((Gdiplus::REAL)(detail->point2.x), (Gdiplus::REAL)(detail->point2.y));
								Gdiplus::LinearGradientBrush* brush = new Gdiplus::LinearGradientBrush(pt1, pt2, colors[0], colors[1]);
								if (brush) {
									brush->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
									if (n > 2) {
										brush->SetInterpolationColors(colors, locations, (INT)n);
									}
									m_brush = brush;
								}
							} else {
								Gdiplus::GraphicsPath path;
								path.AddEllipse((Gdiplus::REAL)(detail->point1.x - detail->radius), (Gdiplus::REAL)(detail->point1.y - detail->radius), (Gdiplus::REAL)(detail->radius * 2), (Gdiplus::REAL)(detail->radius * 2));
								Gdiplus::PathGradientBrush* brush = new Gdiplus::PathGradientBrush(&path);
								if (brush) {
									if (n > 2) {
										for (sl_size i = 0; i < n; i++) {
											Color& color = _colors[n - 1 - i];
											colors[i] = Gdiplus::Color(color.a, color.r, color.g, color.b);
											locations[i] = (Gdiplus::REAL)(1 - _locations[n - 1 - i]);
										}
										brush->SetInterpolationColors(colors, locations, (INT)n);
									} else {
										Gdiplus::Color c0(_colors[0].a, _colors[0].r, _colors[0].g, _colors[0].b);
										Gdiplus::Color c1(_colors[1].a, _colors[1].r, _colors[1].g, _colors[1].b);
										brush->SetCenterColor(c0);
										INT k = 1;
										brush->SetSurroundColors(&c1, &k);
									}
									brush->SetCenterPoint(Gdiplus::PointF((Gdiplus::REAL)(detail->point1.x), (Gdiplus::REAL)(detail->point1.y)));
									m_brush = brush;
								}
							}
						}
					} else if (desc.style == BrushStyle::Texture) {
						TextureBrushDetail* detail = (TextureBrushDetail*)(desc.detail.get());
						if (detail) {
							Bitmap* pattern = detail->pattern.get();
							if (pattern->isImage()) {
								Ref<Drawable> drawable = PlatformDrawable::create((Image*)pattern);
								if (drawable.isNotNull()) {
									Gdiplus::Image* image = GraphicsPlatform::getImageDrawableHandle(drawable.get());
									if (image) {
										m_brush = new Gdiplus::TextureBrush(image);
										m_drawableCache = drawable;
									}
								}
							} else {
								Gdiplus::Bitmap* bitmap = GraphicsPlatform::getBitmapHandle(pattern);
								if (bitmap) {
									m_brush = new Gdiplus::TextureBrush(bitmap);
								}
							}
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
					return (BrushPlatformObject*)(m_platformObject.get());
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
