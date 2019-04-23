#include "drawables.h"

#include "raws.h"

namespace slib
{
	namespace drawable
	{

		SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(camera_view_control_flash_auto, 256, 256, raw::image_camera_view_control_flash_auto_png::size, raw::image_camera_view_control_flash_auto_png::bytes)

		SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(camera_view_control_flash_off, 256, 256, raw::image_camera_view_control_flash_off_png::size, raw::image_camera_view_control_flash_off_png::bytes)

		SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(camera_view_control_flash_on, 256, 256, raw::image_camera_view_control_flash_on_png::size, raw::image_camera_view_control_flash_on_png::bytes)

		SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(camera_view_control_switch, 256, 256, raw::image_camera_view_control_switch_png::size, raw::image_camera_view_control_switch_png::bytes)

		SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(round_shadow_back, 128, 128, raw::image_round_shadow_back_png::size, raw::image_round_shadow_back_png::bytes)

		SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_BEGIN
			SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_ITEM(camera_view_control_flash_auto)
			SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_ITEM(camera_view_control_flash_off)
			SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_ITEM(camera_view_control_flash_on)
			SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_ITEM(camera_view_control_switch)
			SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_ITEM(round_shadow_back)
		SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_END

	}
}
