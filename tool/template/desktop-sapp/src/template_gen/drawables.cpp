#include "drawables.h"

#include "raws.h"

namespace sapp
{
	namespace drawable
	{

		SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(slib_icon, 512, 512, raw::image_slib_icon_png::size, raw::image_slib_icon_png::bytes)

		SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_BEGIN
			SLIB_DEFINE_RESOURCE_MAP_ITEM(slib_icon)
		SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_END

	}
}
