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

#ifdef SLIB_PLATFORM_IS_APPLE

#include "slib/core/charset.h"

#include "slib/core/string.h"
#include "slib/core/platform_apple.h"

namespace slib
{
	
	namespace priv
	{
		namespace charset
		{
			
			sl_size Encode8(const sl_char8* utf8, sl_size lenUtf8, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer)
			{
				if (lenUtf8) {
					NSString* nstr = [[NSString alloc] initWithBytesNoCopy:(void*)utf8 length:lenUtf8 encoding:NSUTF8StringEncoding freeWhenDone:NO];
					if (nstr != nil) {
						CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
						if (cf_encoding != kCFStringEncodingInvalidId) {
							NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
							if (output) {
								NSData* data = [nstr dataUsingEncoding:encoding];
								if (data != nil) {
									sl_size size = (sl_size)(data.length);
									if (sizeOutputBuffer >= 0 && size > sizeOutputBuffer) {
										size = sizeOutputBuffer;
									}
									Base::copyMemory(output, data.bytes, size);
									return size;
								}
							} else {
								sl_size size = (sl_size)([nstr lengthOfBytesUsingEncoding:encoding]);
								if (sizeOutputBuffer >= 0 && size > sizeOutputBuffer) {
									size = sizeOutputBuffer;
								}
								return size;
							}
						}
					}
				}
				return 0;
			}
			
			Memory EncodeString8(const sl_char8* utf8, sl_size lenUtf8, sl_uint32 codepage)
			{
				NSString* nstr = [[NSString alloc] initWithBytesNoCopy:(void*)utf8 length:lenUtf8 encoding:NSUTF8StringEncoding freeWhenDone:NO];
				if (nstr != nil) {
					CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
					if (cf_encoding != kCFStringEncodingInvalidId) {
						NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
						NSData* data = [nstr dataUsingEncoding:encoding];
						if (data != nil) {
							return Memory::create(data.bytes, (sl_size)(data.length));
						}
					}
				}
				return sl_null;
			}
			
			sl_size Decode8(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char8* utf8, sl_reg lenUtf8Buffer)
			{
				if (sizeInput) {
					CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
					if (cf_encoding != kCFStringEncodingInvalidId) {
						NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
						NSString* nstr = [[NSString alloc] initWithBytesNoCopy:(void*)input length:sizeInput encoding:encoding freeWhenDone:NO];
						if (nstr != nil) {
							const char* src = [nstr UTF8String];
							if (src) {
								sl_size size = Base::getStringLength(src);
								if (lenUtf8Buffer >= 0 && size > lenUtf8Buffer) {
									size = lenUtf8Buffer;
								}
								if (utf8) {
									Base::copyMemory(utf8, src, size);
								}
								return size;
							}
						}
					}
				}
				return 0;
			}
			
			String DecodeString8(sl_uint32 codepage, const void* data, sl_size size)
			{
				CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
				if (cf_encoding != kCFStringEncodingInvalidId) {
					NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
					NSString* nstr = [[NSString alloc] initWithBytesNoCopy:(void*)data length:size encoding:encoding freeWhenDone:NO];
					if (nstr != nil) {
						return Apple::getStringFromNSString(nstr);
					}
				}
				return sl_null;
			}
			
			sl_size Encode16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer)
			{
				if (lenUtf16) {
					NSString* nstr = [[NSString alloc] initWithCharactersNoCopy:(unichar*)utf16 length:lenUtf16 freeWhenDone:NO];
					if (nstr != nil) {
						CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
						if (cf_encoding != kCFStringEncodingInvalidId) {
							NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
							if (output) {
								NSData* data = [nstr dataUsingEncoding:encoding];
								if (data != nil) {
									sl_size size = (sl_size)(data.length);
									if (sizeOutputBuffer >= 0 && size > sizeOutputBuffer) {
										size = sizeOutputBuffer;
									}
									Base::copyMemory(output, data.bytes, size);
									return size;
								}
							} else {
								sl_size size = (sl_size)([nstr lengthOfBytesUsingEncoding:encoding]);
								if (sizeOutputBuffer >= 0 && size > sizeOutputBuffer) {
									size = sizeOutputBuffer;
								}
								return size;
							}
						}
					}
				}
				return 0;
			}
			
			Memory EncodeString16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage)
			{
				NSString* nstr = [[NSString alloc] initWithCharactersNoCopy:(unichar*)utf16 length:lenUtf16 freeWhenDone:NO];
				if (nstr != nil) {
					CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
					if (cf_encoding != kCFStringEncodingInvalidId) {
						NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
						NSData* data = [nstr dataUsingEncoding:encoding];
						if (data != nil) {
							return Memory::create(data.bytes, (sl_size)(data.length));
						}
					}
				}
				return sl_null;
			}
			
			sl_size Decode16(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer)
			{
				if (sizeInput) {
					CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
					if (cf_encoding != kCFStringEncodingInvalidId) {
						NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
						NSString* nstr = [[NSString alloc] initWithBytesNoCopy:(void*)input length:sizeInput encoding:encoding freeWhenDone:NO];
						if (nstr != nil) {
							sl_size len = (sl_size)([nstr length]);
							if (len) {
								if (lenUtf16Buffer >= 0 && len > lenUtf16Buffer) {
									len = lenUtf16Buffer;
								}
								if (utf16) {
									[nstr getCharacters:(unichar*)utf16];
								}
								return len;
							}
						}
					}
				}
				return 0;
			}
			
			String16 DecodeString16(sl_uint32 codepage, const void* data, sl_size size)
			{
				CFStringEncoding cf_encoding = CFStringConvertWindowsCodepageToEncoding(codepage);
				if (cf_encoding != kCFStringEncodingInvalidId) {
					NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(cf_encoding);
					NSString* nstr = [[NSString alloc] initWithBytesNoCopy:(void*)data length:size encoding:encoding freeWhenDone:NO];
					if (nstr != nil) {
						sl_size len = (sl_size)([nstr length]);
						if (len) {
							String16 ret = String16::allocate(len);
							if (ret.isNotNull()) {
								[nstr getCharacters:(unichar*)(ret.getData())];
								return ret;
							}
						}
					}
				}
				return sl_null;
			}
		
		}
	}
	
}

#endif
