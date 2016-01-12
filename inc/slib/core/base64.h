#ifndef CHECKHEADER_SLIB_CORE_BASE64
#define CHECKHEADER_SLIB_CORE_BASE64

#include "definition.h"

#include "string.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Base64
{
public:
	static String encode(const void* byte, sl_uint32 size);

	static sl_uint32 decode(const String& base64, void* buf, sl_uint32 size);
	
	static Memory decode(const String& base64);

};

SLIB_NAMESPACE_END

#endif
