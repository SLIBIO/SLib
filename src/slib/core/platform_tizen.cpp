#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/core/platform_tizen.h"

#include "../../../inc/slib/core/variant.h"

#include <app_common.h>

namespace slib
{

	String Tizen::getAssetFilePath(const String& path)
	{
		return String::format("%s/%s", ::app_get_resource_path(), path);
	}

}

#endif
