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

#ifdef SLIB_PLATFORM_IS_LINUX

#include "slib/core/charset.h"

#include "slib/core/string.h"
#include "slib/core/file.h"
#include "slib/core/scoped.h"

#include "unicode/ucnv.h"

#define LOAD_DLL

#ifdef LOAD_DLL
#include <dlfcn.h>
#endif

namespace slib
{
	
	namespace priv
	{
		namespace charset
		{

#ifdef LOAD_DLL
			#ifdef ucnv_open
			#undef ucnv_open
			#undef ucnv_close
			#undef ucnv_fromUChars
			#undef ucnv_toUChars
			#undef ucnv_countAvailable
			#undef ucnv_getAvailableName
			#endif
			
			#define ucnv_open SLIB_ucnv_open
			typedef UConverter* (*TYPE_ucnv_open)(const char *converterName, UErrorCode *err);
			TYPE_ucnv_open SLIB_ucnv_open = sl_null;

			#define ucnv_close SLIB_ucnv_close
 			typedef void (*TYPE_ucnv_close)(UConverter * converter);
			TYPE_ucnv_close SLIB_ucnv_close = sl_null;

			#define ucnv_fromUChars SLIB_ucnv_fromUChars
			typedef int32_t (*TYPE_ucnv_fromUChars)(UConverter *cnv, char *dest, int32_t destCapacity, const UChar *src, int32_t srcLength, UErrorCode *pErrorCode);
			TYPE_ucnv_fromUChars SLIB_ucnv_fromUChars = sl_null;

			#define ucnv_toUChars SLIB_ucnv_toUChars
			typedef int32_t (*TYPE_ucnv_toUChars)(UConverter *cnv, UChar *dest, int32_t destCapacity, const char *src, int32_t srcLength, UErrorCode *pErrorCode);
			TYPE_ucnv_toUChars SLIB_ucnv_toUChars = sl_null;

			#define ucnv_countAvailable SLIB_ucnv_countAvailable
			typedef int32_t (*TYPE_ucnv_countAvailable)();
			TYPE_ucnv_countAvailable SLIB_ucnv_countAvailable = sl_null;

			#define ucnv_getAvailableName SLIB_ucnv_getAvailableName
			typedef const char* (*TYPE_ucnv_getAvailableName)(int32_t n);
			TYPE_ucnv_getAvailableName SLIB_ucnv_getAvailableName = sl_null;

			SLIB_INLINE static void* LoadSymbolWithSuffix(void* handle, const char* symbol, const String& suffix)
			{
				if (suffix.isNotEmpty()) {
					String s = symbol + suffix;
					void* ret = dlsym(handle, s.getData());
					if (ret) {
						return ret;
					}
				}
				return dlsym(handle, symbol);
			}

			SLIB_INLINE static sl_bool LoadIcu()
			{
				static void* hDll = sl_null;
				static sl_bool flagLoaded = sl_false;
				if (!flagLoaded) {
					String pathDll;
					String suffix;
					{
#ifdef SLIB_ARCH_IS_64BIT
#define LIB_PATH "/usr/lib64"
#else
#define LIB_PATH "/usr/lib"
#endif
						ListElements<String> files(File::getFiles(LIB_PATH));
						for (sl_size i = 0; i < files.count; i++) {
							String& fileName = files[i];
							if (fileName.startsWith("libicuuc.so") && fileName > pathDll) {
								pathDll = fileName;
							}
						}
					}
					if (pathDll.isNotEmpty()) {
						suffix = pathDll.substring(12);
						if (suffix.isNotEmpty()) {
							sl_reg index = suffix.indexOf('.');
							if (index > 0) {
								suffix = "_" + suffix.substring(0, index);
							}
						}
						pathDll = LIB_PATH + ("/" + pathDll);
					}
					hDll = dlopen(pathDll.getData(), RTLD_LAZY);
					if (hDll) {
						SLIB_ucnv_open = (TYPE_ucnv_open)(LoadSymbolWithSuffix(hDll, "ucnv_open", suffix));
						SLIB_ucnv_close = (TYPE_ucnv_close)(LoadSymbolWithSuffix(hDll, "ucnv_close", suffix));
						SLIB_ucnv_fromUChars = (TYPE_ucnv_fromUChars)(LoadSymbolWithSuffix(hDll, "ucnv_fromUChars", suffix));
						SLIB_ucnv_toUChars = (TYPE_ucnv_toUChars)(LoadSymbolWithSuffix(hDll, "ucnv_toUChars", suffix));
						SLIB_ucnv_countAvailable = (TYPE_ucnv_countAvailable)(LoadSymbolWithSuffix(hDll, "ucnv_countAvailable", suffix));
						SLIB_ucnv_getAvailableName = (TYPE_ucnv_getAvailableName)(LoadSymbolWithSuffix(hDll, "ucnv_getAvailableName", suffix));
					}
					flagLoaded = sl_true;
				}
				return SLIB_ucnv_open != sl_null;
			}
#endif

			SLIB_INLINE static UConverter* OpenConverter(sl_uint32 codepage)
			{
#ifdef LOAD_DLL
				if (!(LoadIcu())) {
					return sl_null;
				}
#endif
				UErrorCode err = U_ZERO_ERROR;
				SLIB_STATIC_STRING(strCodepagePrefix, "cp")
				String strCodepage = strCodepagePrefix + String::fromUint32(codepage);
				return ucnv_open(strCodepage.getData(), &err);
			}
			
			sl_size Encode16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer)
			{
				if (!lenUtf16) {
					return 0;
				}				
				if (!sizeOutputBuffer) {
					return 0;
				}
				if (lenUtf16 > 0x20000000) {
					lenUtf16 = 0x20000000;
				}
				if (sizeOutputBuffer > 0x40000000) {
					sizeOutputBuffer = 0x40000000;
				}
				if (sizeOutputBuffer < 0) {
					sizeOutputBuffer = 0;
				}
				sl_size ret = 0;
				UConverter* conv = OpenConverter(codepage);
				if (conv) {
					UErrorCode err = U_ZERO_ERROR;
					ret = (sl_size)(ucnv_fromUChars(conv, (char*)output, (int32_t)sizeOutputBuffer, (UChar*)utf16, (int32_t)lenUtf16, &err));
					ucnv_close(conv);
				}
				return ret;
			}
			
			sl_size Decode16(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer)
			{
				if (!sizeInput) {
					return 0;
				}				
				if (!lenUtf16Buffer) {
					return 0;
				}
				if (sizeInput > 0x40000000) {
					sizeInput = 0x40000000;
				}
				if (lenUtf16Buffer > 0x20000000) {
					lenUtf16Buffer = 0x20000000;
				}
				if (lenUtf16Buffer < 0) {
					lenUtf16Buffer = 0;
				}
				sl_size ret = 0;
				UConverter* conv = OpenConverter(codepage);
				if (conv) {
					UErrorCode err = U_ZERO_ERROR;
					ret = (sl_size)(ucnv_toUChars(conv, (UChar*)utf16, (int32_t)lenUtf16Buffer, (char*)input, (int32_t)sizeInput, &err));
					ucnv_close(conv);
				}
				return ret;
			}
			
		}
	}
	
}

#endif
