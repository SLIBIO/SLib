/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_UTIL
#define CHECKHEADER_SLIB_GRAPHICS_UTIL

#include "definition.h"

#include "constants.h"

#include "../math/rectangle.h"

namespace slib
{
	
	class Canvas;
	class Drawable;
	class DrawParam;
	
	class SLIB_EXPORT GraphicsUtil
	{
	public:
		static sl_bool containsPointInRoundRect(const Point& pt, const Rectangle& rect, const Size& radius);

		static sl_bool containsPointInEllipse(const Point& pt, const Rectangle& rect);
	

		static sl_real calculateAlignX(sl_real leftDst, sl_real rightDst, sl_real widthSrc, Alignment alignment);

		static sl_real calculateAlignY(sl_real topDst, sl_real bottomDst, sl_real heightSrc, Alignment alignment);

		static Point calculateAlignPosition(const Rectangle& rectDst, sl_real widthSrc, sl_real heightSrc, Alignment alignment);

		static Rectangle transformRectangle(const Rectangle& rectTransform, const Rectangle& rectOtherFrom, const Rectangle& rectOtherTo);


		static void drawRepeat(Canvas* canvas, const Ref<Drawable>& source, sl_real _dx, sl_real _dy, sl_uint32 nRepeatX, sl_uint32 nRepeatY, const DrawParam& param);

	};

}

#endif
