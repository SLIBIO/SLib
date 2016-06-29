#ifndef CHECKHEADER_SLIB_CORE_CHARSET
#define CHECKHEADER_SLIB_CORE_CHARSET

#include "definition.h"

SLIB_NAMESPACE_BEGIN

enum class Charset
{
	Unknown = 0,
	UTF8 = 10000,
	UTF16LE = 10001,
	UTF16BE = 10002
};

class Charsets
{
public:
	static sl_size utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer);
	
	static sl_size utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer);
	
	static sl_size utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer);
	
	static sl_size utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer);

	static sl_size utf16ToUtf32(const sl_char16* utf16, sl_reg lenUtf16, sl_char32* utf32, sl_reg lenUtf32Buffer);
	
	static sl_size utf32ToUtf16(const sl_char32* utf32, sl_reg lenUtf32, sl_char16* utf16, sl_reg lenUtf16Buffer);
	
};


SLIB_NAMESPACE_END

#endif
