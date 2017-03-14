#ifndef CHECKHEADER_SLIB_CORE_STRING_STD
#define CHECKHEADER_SLIB_CORE_STRING_STD

#include "string.h"

#include <cstddef>

namespace slib
{
	String operator "" _s(const char*, std::size_t);
	
	String operator "" _s(const sl_char16*, std::size_t);
	
}

#endif
