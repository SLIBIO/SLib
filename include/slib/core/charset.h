/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_CHARSET
#define CHECKHEADER_SLIB_CORE_CHARSET

#include "definition.h"

namespace slib
{
	
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

}

#endif
