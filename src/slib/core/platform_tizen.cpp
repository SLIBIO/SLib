#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/core/platform_tizen.h"

#include "../../../inc/slib/core/variant.h"

#include <app_common.h>
#include <stdlib.h>

namespace slib
{

	String Tizen::getAssetFilePath(const String& path)
	{
		char* root = ::app_get_resource_path();
		String s = String::format("%s/%s", root, path);
		::free(root);
		return s;
	}

}

#endif
