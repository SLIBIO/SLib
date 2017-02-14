#ifndef CHECKHEADER_SLIB_GRAPHICS_IMAGE_STB
#define CHECKHEADER_SLIB_GRAPHICS_IMAGE_STB

#include "../../../inc/slib/graphics/image.h"

#ifdef SLIB_GRAPHICS_IMAGE_USE_STB

namespace slib
{

	class Image_STB
	{
	public:
		static Ref<Image> loadImage(const void* content, sl_size size);
	};

}

#endif

#endif
