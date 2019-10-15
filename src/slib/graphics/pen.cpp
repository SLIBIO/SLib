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

#include "slib/graphics/pen.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PenDesc)

	PenDesc::PenDesc()
	{
		width = 1;
		color = Color::Black;

		style = PenStyle::Solid;
		cap = LineCap::Flat;
		join = LineJoin::Miter;
		miterLimit = 10;
	}

	
	SLIB_DEFINE_OBJECT(Pen, Object)

	Pen::Pen()
	{
	}

	Pen::~Pen()
	{
	}

	Ref<Pen> Pen::getDefault()
	{
		SLIB_SAFE_STATIC(Ref<Pen>, defaultPen, create(PenDesc()))
		if (SLIB_SAFE_STATIC_CHECK_FREED(defaultPen)) {
			return sl_null;
		}
		return defaultPen;
	}

	Ref<Pen> Pen::create(const PenDesc& desc)
	{
		Ref<Pen> ret = new Pen;
		if (ret.isNotNull()) {
			ret->m_desc = desc;
		}
		return ret;
	}

	Ref<Pen> Pen::create(PenStyle style, sl_real width, const Color& color)
	{
		PenDesc desc;
		desc.style = style;
		desc.width = width;
		desc.color = color;
		return create(desc);
	}

	Ref<Pen> Pen::createSolidPen(sl_real width, const Color& color)
	{
		PenDesc desc;
		desc.width = width;
		desc.color = color;
		return create(desc);
	}

	void Pen::getDesc(PenDesc& desc)
	{
		desc = m_desc;
	}

	PenStyle Pen::getStyle()
	{
		return m_desc.style;
	}

	sl_real Pen::getWidth()
	{
		return m_desc.width;
	}

	Color Pen::getColor()
	{
		return m_desc.color;
	}

	LineCap Pen::getCap()
	{
		return m_desc.cap;
	}

	LineJoin Pen::getJoin()
	{
		return m_desc.join;
	}

	sl_real Pen::getMiterLimit()
	{
		return m_desc.miterLimit;
	}

}
