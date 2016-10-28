#ifndef CHECKHEADER_SLIB_GRAPHICS_IMAGE_STB
#define CHECKHEADER_SLIB_GRAPHICS_IMAGE_STB

#include "../../../inc/slib/graphics/image.h"

#ifdef SLIB_GRAPHICS_IMAGE_USE_STB

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Image_STB
{
public:
	static Ref<Image> loadImage(const void* content, sl_size size);
};

SLIB_GRAPHICS_NAMESPACE_END

#endif

#endif
