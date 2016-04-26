#ifndef CHECKHEADER_SLIB_GRAPHICS_UTIL
#define CHECKHEADER_SLIB_GRAPHICS_UTIL

#include "definition.h"

#include "constants.h"

#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT GraphicsUtil
{
public:
	static sl_real calculateAlignX(sl_real leftDst, sl_real rightDst, sl_real widthSrc, Alignment alignment);
	
	static sl_real calculateAlignY(sl_real topDst, sl_real bottomDst, sl_real heightSrc, Alignment alignment);
	
	static Point calculateAlignPosition(const Rectangle& rectDst, sl_real widthSrc, sl_real heightSrc, Alignment alignment);
	
	
	static Rectangle transformRectangle(const Rectangle& rectSrc, const Rectangle& rectFrom, const Rectangle& rectTo);
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
