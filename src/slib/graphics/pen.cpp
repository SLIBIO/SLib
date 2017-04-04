/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/graphics/pen.h"

#include "slib/core/safe_static.h"

namespace slib
{

	PenDesc::PenDesc()
	{
		width = 1;
		color = Color::Black;

		style = PenStyle::Solid;
		cap = LineCap::Flat;
		join = LineJoin::Miter;
		miterLimit = 10;
	}

	PenDesc::~PenDesc()
	{
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

	Ref<Pen> Pen::create(PenStyle style, sl_real width, Color color)
	{
		PenDesc desc;
		desc.style = style;
		desc.width = width;
		desc.color = color;
		return create(desc);
	}

	Ref<Pen> Pen::createSolidPen(sl_real width, Color color)
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
