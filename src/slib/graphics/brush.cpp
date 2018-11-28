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

#include "slib/core/safe_static.h"

namespace slib
{

	BrushDesc::BrushDesc()
	{
		style = BrushStyle::Solid;
		color = Color::White;
	}

	BrushDesc::~BrushDesc()
	{
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
		SLIB_SAFE_STATIC(Ref<Brush>, defaultBrush, create(BrushDesc()))
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
		}
		return ret;
	}

	Ref<Brush> Brush::createSolidBrush(const Color& color)
	{
		BrushDesc desc;
		desc.color = color;
		return create(desc);
	}

	void Brush::getDesc(BrushDesc& desc)
	{
		desc = m_desc;
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
