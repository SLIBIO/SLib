#ifndef CHECKHEADER_SLIB_CORE_WIN32_COM
#define CHECKHEADER_SLIB_CORE_WIN32_COM

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

#include "object.h"
#include "memory.h"

namespace slib
{

	class Win32_COM
	{
	public:
		static Memory readAllBytesFromStream(IStream* stream);
	};

}

#define SLIB_WIN32_COM_SAFE_RELEASE(x) {if (x) {x->Release(); x=NULL;}}

#endif

#endif
