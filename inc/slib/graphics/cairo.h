#ifndef CHECKHEADER_SLIB_GRAPHICS_CAIRO
#define CHECKHEADER_SLIB_GRAPHICS_CAIRO

#include "definition.h"

#include "../graphics/bitmap.h"
#include "../graphics/font.h"

namespace slib
{
	
	class SLIB_EXPORT Cairo
	{
	public:
		static Ref<Bitmap> createBitmap(sl_uint32 width, sl_uint32 height);

		static Ref<Bitmap> loadBitmapFromMemory(const void* mem, sl_size size);

	};

}

#endif
