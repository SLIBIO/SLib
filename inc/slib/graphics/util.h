#ifndef CHECKHEADER_SLIB_GRAPHICS_UTIL
#define CHECKHEADER_SLIB_GRAPHICS_UTIL

#include "definition.h"

#include "constants.h"

#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

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

SLIB_GRAPHICS_NAMESPACE_END

#endif
