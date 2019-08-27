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
	

		static sl_real calculateAlignX(sl_real leftDst, sl_real rightDst, sl_real widthSrc, const Alignment& alignment);

		static sl_real calculateAlignY(sl_real topDst, sl_real bottomDst, sl_real heightSrc, const Alignment& alignment);

		static Point calculateAlignPosition(const Rectangle& rectDst, sl_real widthSrc, sl_real heightSrc, const Alignment& alignment);
		
		static sl_bool calculateAlignRectangle(Rectangle& rectOutput, const Rectangle& rectDst, sl_real widthSrc, sl_real heightSrc, ScaleMode scaleMode, const Alignment& alignment);

		static Rectangle transformRectangle(const Rectangle& rectTransform, const Rectangle& rectOtherFrom, const Rectangle& rectOtherTo);


		static void drawRepeat(Canvas* canvas, const Ref<Drawable>& source, sl_real _dx, sl_real _dy, sl_uint32 nRepeatX, sl_uint32 nRepeatY, const DrawParam& param);

		
		static sl_real pixelToInch(sl_real px);
		
		static sl_real inchToPixel(sl_real inch);
		
		static sl_real pixelToMeter(sl_real px);
		
		static sl_real meterToPixel(sl_real meters);
		
		static sl_real pixelToCentimeter(sl_real px);
		
		static sl_real centimeterToPixel(sl_real cm);
		
		static sl_real pixelToMillimeter(sl_real px);
		
		static sl_real millimeterToPixel(sl_real mm);
		
		static sl_real pixelToPoint(sl_real px);
		
		static sl_real pointToPixel(sl_real pt);
		
		static sl_real pixelToPicas(sl_real px);
		
		static sl_real picasToPixel(sl_real pc);
		
		static sl_real pixelToDp(sl_real px);
		
		static sl_real dpToPixel(sl_real dp);
		
	};

}

#endif
