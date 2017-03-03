#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/graphics/drawable.h"

#include "../../../inc/slib/graphics/image.h"
#include "../../../inc/slib/graphics/platform.h"

namespace slib
{

	Ref<Drawable> PlatformDrawable::create(const ImageDesc& desc)
	{
		return Bitmap::create(desc);
	}

	Ref<Drawable> PlatformDrawable::loadFromMemory(const void* buf, sl_size size)
	{
		return Bitmap::loadFromMemory(buf, size);
	}

}

#endif
