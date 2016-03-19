#ifndef CHECKHEADER_SLIB_GRAPHICS_IMAGE_SOIL2
#define CHECKHEADER_SLIB_GRAPHICS_IMAGE_SOIL2

#include "../../../inc/slib/graphics/image.h"

#ifdef SLIB_GRAPHICS_IMAGE_USE_SOIL2

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Image_SOIL2
{
public:
	static Ref<Image> loadImage(const void* content, sl_size size);
};

SLIB_GRAPHICS_NAMESPACE_END

#endif

#endif
