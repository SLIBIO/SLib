#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/platform.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

COLORREF GraphicsPlatform::getColorRef(const Color& color)
{
	return (COLORREF)(color.getBGR());
}

Color GraphicsPlatform::getColorFromColorRef(COLORREF cr)
{
	Color ret;
	ret.setBGR((sl_uint32)cr);
	ret.a = 255;
	return ret;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
