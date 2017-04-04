/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_PEN
#define CHECKHEADER_SLIB_GRAPHICS_PEN

#include "definition.h"

#include "constants.h"
#include "color.h"

#include "../core/object.h"

namespace slib
{
	
	class SLIB_EXPORT PenDesc
	{
	public:
		PenStyle style;
		LineCap cap;
		LineJoin join;
		sl_real width;
		Color color;
		sl_real miterLimit;

	public:
		PenDesc();

		~PenDesc();

	};
	
	class SLIB_EXPORT Pen : public Object
	{
		SLIB_DECLARE_OBJECT;

	protected:
		Pen();

		~Pen();

	public:
		static Ref<Pen> getDefault();

		static Ref<Pen> create(const PenDesc& desc);

		static Ref<Pen> create(PenStyle style, sl_real width = 1, Color color = Color::Black);

		static Ref<Pen> createSolidPen(sl_real width = 1, Color color = Color::Black);

	public:
		void getDesc(PenDesc& desc);
	
		PenStyle getStyle();
	
		sl_real getWidth();
	
		Color getColor();
	
		LineCap getCap();
	
		LineJoin getJoin();
	
		sl_real getMiterLimit();
	
	protected:
		PenDesc m_desc;

		Ref<Referable> m_platformObject;
		SpinLock m_lock;

	};

}

#endif
