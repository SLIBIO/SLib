#ifndef CHECKHEADER_SLIB_CORE_BASE64
#define CHECKHEADER_SLIB_CORE_BASE64

#include "definition.h"

#include "string.h"

namespace slib
{
	
	class SLIB_EXPORT Base64
	{
	public:
		static String encode(const void* byte, sl_size size);

		static String encode(const Memory& mem);

		static sl_size decode(const String& base64, void* buf, sl_size size);

		static Memory decode(const String& base64);
	
	};

}

#endif
