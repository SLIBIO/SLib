/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_BRUSH
#define CHECKHEADER_SLIB_GRAPHICS_BRUSH

#include "definition.h"

#include "constants.h"
#include "color.h"

#include "../core/object.h"

namespace slib
{
	
	class SLIB_EXPORT BrushDesc
	{
	public:
		BrushStyle style;
		Color color;

	public:
		BrushDesc();

		~BrushDesc();

	};
	
	class SLIB_EXPORT Brush : public Referable
	{
		SLIB_DECLARE_OBJECT

	protected:
		Brush();

		~Brush();

	public:
		static Ref<Brush> getDefault();

		static Ref<Brush> create(const BrushDesc& desc);

		static Ref<Brush> createSolidBrush(const Color& color);

	public:
		void getDesc(BrushDesc& desc);

		BrushStyle getStyle();

		Color getColor();

	protected:
		BrushDesc m_desc;

		Ref<Referable> m_platformObject;
		SpinLock m_lock;

	};

}

#endif
