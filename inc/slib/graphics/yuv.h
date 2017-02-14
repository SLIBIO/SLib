#ifndef CHECKHEADER_SLIB_GRAPHICS_YUV
#define CHECKHEADER_SLIB_GRAPHICS_YUV

#include "definition.h"

namespace slib
{
	
	class SLIB_EXPORT YUV
	{
	public:
		static void convertRGBToYUV(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8& Y, sl_uint8& U, sl_uint8& V);

		static void convertYUVToRGB(sl_uint8 Y, sl_uint8 U, sl_uint8 V, sl_uint8& R, sl_uint8& G, sl_uint8& B);

	};

}

#endif
