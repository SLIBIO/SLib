#ifndef CHECKHEADER_SLIB_CORE_ASSET
#define CHECKHEADER_SLIB_CORE_ASSET

#include "definition.h"

#include "string.h"
#include "memory.h"

namespace slib
{

	class SLIB_EXPORT Assets
	{
	public:
		static sl_bool isBasedOnFileSystem();

		static String getFilePath(const String& path);
	

		static Memory readAllBytes(const String& path);
	
	};

}

#endif
