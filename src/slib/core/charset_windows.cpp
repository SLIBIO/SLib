/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_WINDOWS

#include "slib/core/charset.h"

#include <windows.h>

namespace slib
{
	
	namespace priv
	{
		namespace charset
		{
			
			sl_size Encode16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer)
			{
				if (lenUtf16 && lenUtf16 <= 0x40000000 && sizeOutputBuffer != 0) {
					if (sizeOutputBuffer > 0x40000000) {
						sizeOutputBuffer = 0x40000000;
					}
					if (sizeOutputBuffer < 0) {
						sizeOutputBuffer = 0;
					}
					int ret = WideCharToMultiByte(
						(UINT)codepage,
						0, // Flags
						(WCHAR*)utf16, (int)lenUtf16,
						(char*)output, (int)sizeOutputBuffer,
						NULL, // default char
						NULL // Flag used default char
					);
					if (ret > 0) {
						return (sl_size)ret;
					}					
				}
				return 0;
			}
			
			sl_size Decode16(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer)
			{
				if (sizeInput && sizeInput <= 0x40000000 && lenUtf16Buffer != 0) {
					if (lenUtf16Buffer > 0x20000000) {
						lenUtf16Buffer = 0x20000000;
					}
					if (lenUtf16Buffer < 0) {
						lenUtf16Buffer = 0;
					}
					int ret = MultiByteToWideChar(
						(UINT)codepage,
						0, // Flags
						(char*)input, (int)sizeInput,
						(WCHAR*)utf16, (int)lenUtf16Buffer
					);
					if (ret) {
						return (sl_size)ret;
					}
				}
				return 0;
			}
			
		}
	}
	
}

#endif
