#include "graphics_image_soil2.h"

#ifdef SLIB_GRAPHICS_IMAGE_USE_SOIL2

#include "../../../inc/thirdparty/SOIL2/SOIL2.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
Ref<Image> Image_SOIL2::loadImage(const void* content, sl_size size)
{
	Ref<Image> ret;
	int width;
	int height;
	int channels;
	unsigned char *map = SOIL_load_image_from_memory((const unsigned char*)content, (int)size, &width, &height, &channels, SOIL_LOAD_RGBA);
	if (map) {
		ret = Image::create(width, height);
		if (ret.isNotNull()) {
			Color::convert(width, height, Color::RGBA, map, width * 4, ret->getColors(), width);
			return ret;
		}
	}
	return ret;
}
SLIB_GRAPHICS_NAMESPACE_END

#endif