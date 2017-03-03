#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_TIZEN
#define CHECKHEADER_SLIB_CORE_PLATFORM_TIZEN

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_TIZEN

#include "string.h"

namespace slib
{

	// specific functions for Tizen
	class SLIB_EXPORT Tizen
	{
	public:
		static String getAssetFilePath(const String& path);

	};

}

#endif

#endif
