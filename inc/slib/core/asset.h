#ifndef CHECKHEADER_SLIB_CORE_ASSET
#define CHECKHEADER_SLIB_CORE_ASSET

#include "definition.h"

#include "string.h"
#include "memory.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Assets
{
public:
	static sl_bool isBasedOnFileSystem();
	
	static String getFilePath(const String& path);

	
	static Memory readAllBytes(const String& path);

};

SLIB_NAMESPACE_END

#endif
