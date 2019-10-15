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

#include "slib/graphics/brush.h"

#include "slib/graphics/bitmap.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	GradientBrushDetail::GradientBrushDetail()
	{
	}
	
	GradientBrushDetail::~GradientBrushDetail()
	{
	}
	
	TextureBrushDetail::TextureBrushDetail()
	{
	}
	
	TextureBrushDetail::~TextureBrushDetail()
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(BrushDesc)

	BrushDesc::BrushDesc()
	{
		style = BrushStyle::Solid;
		color = Color::White;
	}

	SLIB_DEFINE_ROOT_OBJECT(Brush)

	Brush::Brush()
	{
	}

	Brush::~Brush()
	{
	}

	Ref<Brush> Brush::getDefault()
	{
		SLIB_SAFE_STATIC(Ref<Brush>, defaultBrush, createSolidBrush(Color::White))
		if (SLIB_SAFE_STATIC_CHECK_FREED(defaultBrush)) {
			return sl_null;
		}
		return defaultBrush;
	}

	Ref<Brush> Brush::create(const BrushDesc& desc)
	{
		Ref<Brush> ret = new Brush;
		if (ret.isNotNull()) {
			ret->m_desc = desc;
			return ret;
		}
		return sl_null;
	}

	Ref<Brush> Brush::createSolidBrush(const Color& color)
	{
		Ref<Brush> ret = new Brush;
		if (ret.isNotNull()) {
			ret->m_desc.color = color;
			return ret;
		}
		return sl_null;
	}

	Ref<Brush> Brush::createLinearGradientBrush(const Point& pt1, const Point& pt2, const Color& color1, const Color& color2)
	{
		Color colors[2] = {color1, color2};
		sl_real locations[] = {0, 1};
		return createLinearGradientBrush(pt1, pt2, 2, colors, locations);
	}
	
	Ref<Brush> Brush::createLinearGradientBrush(const Point& pt1, const Point& pt2, sl_uint32 nColors, const Color* colors, const sl_real* locations)
	{
		if (nColors >= 2) {
			Ref<Brush> ret = new Brush;
			if (ret.isNotNull()) {
				ret->m_desc.style = BrushStyle::LinearGradient;
				Ref<GradientBrushDetail> detail = new GradientBrushDetail;
				if (detail.isNotNull()) {
					detail->point1 = pt1;
					detail->point2 = pt2;
					detail->colors = List<Color>::create(colors, nColors);
					detail->locations = List<sl_real>::create(locations, nColors);
					if (detail->colors.isNotNull() && detail->locations.isNotNull()) {
						ret->m_desc.detail = detail;
						return ret;
					}
				}
			}
		}
		return sl_null;
	}
	
	Ref<Brush> Brush::createRadialGradientBrush(const Point& ptCenter, sl_real radius, const Color& colorCenter, const Color& colorEdge)
	{
		Color colors[2] = {colorCenter, colorEdge};
		sl_real locations[] = {0, 1};
		return createRadialGradientBrush(ptCenter, radius, 2, colors, locations);
	}
	
	Ref<Brush> Brush::createRadialGradientBrush(const Point& ptCenter, sl_real radius, sl_uint32 nColors, const Color* colors, const sl_real* locations)
	{
		if (nColors >= 2) {
			Ref<Brush> ret = new Brush;
			if (ret.isNotNull()) {
				ret->m_desc.style = BrushStyle::RadialGradient;
				Ref<GradientBrushDetail> detail = new GradientBrushDetail;
				if (detail.isNotNull()) {
					detail->point1 = ptCenter;
					detail->radius = radius;
					detail->colors = List<Color>::create(colors, nColors);
					detail->locations = List<sl_real>::create(locations, nColors);
					if (detail->colors.isNotNull() && detail->locations.isNotNull()) {
						ret->m_desc.detail = detail;
						return ret;
					}
				}
			}
		}
		return sl_null;
	}
	
	Ref<Brush> Brush::createTextureBrush(const Ref<Bitmap>& bitmap)
	{
		if (bitmap.isNotNull()) {
			Ref<Brush> ret = new Brush;
			if (ret.isNotNull()) {
				ret->m_desc.style = BrushStyle::Texture;
				Ref<TextureBrushDetail> detail = new TextureBrushDetail;
				if (detail.isNotNull()) {
					detail->pattern = bitmap;
					ret->m_desc.detail = detail;
					return ret;
				}
			}
		}
		return sl_null;
	}

	void Brush::getDesc(BrushDesc& desc)
	{
		desc = m_desc;
	}
	
	BrushDesc& Brush::getDesc()
	{
		return m_desc;
	}

	BrushStyle Brush::getStyle()
	{
		return m_desc.style;
	}

	Color Brush::getColor()
	{
		return m_desc.color;
	}

}
