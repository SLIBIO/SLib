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

#include "slib/core/charset.h"

#include "slib/core/base.h"
#include "slib/core/endian.h"
#include "slib/core/string.h"
#include "slib/core/scoped.h"

namespace slib
{
	
	namespace priv
	{
		namespace charset
		{
			
			sl_size Encode8(const sl_char8* utf8, sl_size lenUtf8, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer);

			Memory EncodeString8(const sl_char8* utf8, sl_size lenUtf8, sl_uint32 codepage);
			
			sl_size Decode8(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char8* utf8, sl_reg lenUtf8Buffer);

			String DecodeString8(sl_uint32 codepage, const void* data, sl_size size);

			sl_size Encode16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage, void* _output, sl_reg sizeOutputBuffer);

			Memory EncodeString16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage);
			
			sl_size Decode16(sl_uint32 codepage, const void* _input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer);

			String16 DecodeString16(sl_uint32 codepage, const void* data, sl_size size);

#if !defined(SLIB_PLATFORM_IS_APPLE)
			sl_size Encode8(const sl_char8* utf8, sl_size lenUtf8, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer)
			{
				if (lenUtf8) {
					sl_size len = Charsets::utf8ToUtf16(utf8, lenUtf8, sl_null, -1);
					if (len) {
						SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, len)
						Charsets::utf8ToUtf16(utf8, lenUtf8, buf, len);
						return Encode16(buf, len, codepage, output, sizeOutputBuffer);
					}
				}
				return 0;
			}
			
			sl_size Decode8(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char8* utf8, sl_reg lenUtf8Buffer)
			{
				if (sizeInput) {
					sl_size len = Decode16(codepage, input, sizeInput, sl_null, -1);
					if (len) {
						SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, len)
						Decode16(codepage, input, sizeInput, buf, len);
						return Charsets::utf16ToUtf8(buf, len, utf8, lenUtf8Buffer);
					}
				}
				return 0;
			}
			
			Memory EncodeString8(const sl_char8* utf8, sl_size lenUtf8, sl_uint32 codepage)
			{
				String16 str = String16::create(utf8, lenUtf8);
				return EncodeString16(str.getData(), str.getLength(), codepage);
			}
			
			String DecodeString8(sl_uint32 codepage, const void* data, sl_size size)
			{
				SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, size)
				sl_size len = Decode16(codepage, data, size, buf, size);
				if (len) {
					return String::create(buf, len);
				}
				return sl_null;
			}
			
			Memory EncodeString16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage)
			{
				sl_size len = Encode16(utf16, lenUtf16, codepage, sl_null, -1);
				if (len) {
					Memory mem = Memory::create(len);
					if (mem.isNotNull()) {
						Encode16(utf16, lenUtf16, codepage, mem.getData(), len);
						return mem;
					}
				}
				return sl_null;
			}
			
			String16 DecodeString16(sl_uint32 codepage, const void* data, sl_size size)
			{
				String16 str = String16::allocate(size);
				if (str.isNotNull()) {
					sl_char16* s = str.getData();
					sl_size len = Decode16(codepage, data, size, s, size);
					if (len) {
						s[len] = 0;
						str.setLength(len);
						return str;
					}
				}
				return sl_null;
			}
#endif

			
			SLIB_INLINE sl_size Copy8(const void* input, sl_size lenInput, void* output, sl_reg lenOutputBuffer) noexcept
			{
				sl_size n;
				if (lenOutputBuffer < 0) {
					n = lenInput;
				} else {
					n = lenOutputBuffer;
					if (lenInput < n) {
						n = lenInput;
					}
				}
				if (output) {
					Base::copyMemory(output, input, n);
				}
				return n;
			}

			SLIB_INLINE sl_size Copy16(const void* input, sl_size lenInput, void* output, sl_reg lenOutputBuffer) noexcept
			{
				sl_size n;
				if (lenOutputBuffer < 0) {
					n = lenInput;
				} else {
					n = lenOutputBuffer;
					if (lenInput < n) {
						n = lenInput;
					}
				}
				if (output) {
					Base::copyMemory(output, input, n << 1);
				}
				return n;
			}
			
			SLIB_INLINE sl_size Swap16(const void* input, sl_size lenInput, void* output, sl_reg lenOutputBuffer) noexcept
			{
				sl_size n;
				if (lenOutputBuffer < 0) {
					n = lenInput;
				} else {
					n = lenOutputBuffer;
					if (lenInput < n) {
						n = lenInput;
					}
				}
				if (output) {
					sl_uint8* s = (sl_uint8*)input;
					sl_uint8* d = (sl_uint8*)output;
					sl_uint8* e = s + (n << 1);
					while (s < e) {
						*d = *(s+1);
						*(d+1) = *s;
						s += 2;
						d += 2;
					}
				}
				return n;
			}
			
			SLIB_INLINE static sl_uint32 ToWindowsCodepage(Charset charset) noexcept
			{
				return ((sl_uint32)charset) & 0xffff;
			}
			
		}
	}
	
	using namespace priv::charset;
	
	
	sl_size Charsets::encode8(const sl_char8* utf8, sl_size lenUtf8, Charset charset, void* output, sl_reg sizeOutputBuffer)
	{
		switch (charset) {
			case Charset::Unknown:
				return 0;
			case Charset::UTF8:
				return Copy8(utf8, lenUtf8, output, sizeOutputBuffer);
			case Charset::UTF16BE:
				return encode8_UTF16BE(utf8, lenUtf8, output, sizeOutputBuffer);
			case Charset::UTF16LE:
				return encode8_UTF16LE(utf8, lenUtf8, output, sizeOutputBuffer);
			case Charset::UTF32BE:
				return encode8_UTF32BE(utf8, lenUtf8, output, sizeOutputBuffer);
			case Charset::UTF32LE:
				return encode8_UTF32LE(utf8, lenUtf8, output, sizeOutputBuffer);
			default:
				return Encode8(utf8, lenUtf8, ToWindowsCodepage(charset), output, sizeOutputBuffer);
		}
	}
	
	sl_size Charsets::decode8(Charset charset, const void* input, sl_size sizeInput, sl_char8* utf8, sl_reg lenUtf8Buffer)
	{
		switch (charset) {
			case Charset::Unknown:
				return 0;
			case Charset::UTF8:
				return Copy8(input, sizeInput, utf8, lenUtf8Buffer);
			case Charset::UTF16BE:
				return decode8_UTF16BE(input, sizeInput, utf8, lenUtf8Buffer);
			case Charset::UTF16LE:
				return decode8_UTF16LE(input, sizeInput, utf8, lenUtf8Buffer);
			case Charset::UTF32BE:
				return decode8_UTF32BE(input, sizeInput, utf8, lenUtf8Buffer);
			case Charset::UTF32LE:
				return decode8_UTF32LE(input, sizeInput, utf8, lenUtf8Buffer);
			default:
				return Decode8(ToWindowsCodepage(charset), input, sizeInput, utf8, lenUtf8Buffer);
		}
	}
	
	sl_size Charsets::encode16(const sl_char16* utf16, sl_size lenUtf16, Charset charset, void* output, sl_reg sizeOutputBuffer)
	{
		switch (charset) {
			case Charset::Unknown:
				return 0;
			case Charset::UTF8:
				return utf16ToUtf8(utf16, lenUtf16, (sl_char8*)output, sizeOutputBuffer);
			case Charset::UTF16BE:
				if (Endian::isBE()) {
					return Copy16(utf16, lenUtf16, output, sizeOutputBuffer < 0 ? -1 : (((sl_size)sizeOutputBuffer) >> 1)) << 1;
				} else {
					return Swap16(utf16, lenUtf16, output, sizeOutputBuffer < 0 ? -1 : (((sl_size)sizeOutputBuffer) >> 1)) << 1;
				}
			case Charset::UTF16LE:
				if (Endian::isLE()) {
					return Copy16(utf16, lenUtf16, output, sizeOutputBuffer < 0 ? -1 : (((sl_size)sizeOutputBuffer) >> 1)) << 1;
				} else {
					return Swap16(utf16, lenUtf16, output, sizeOutputBuffer < 0 ? -1 : (((sl_size)sizeOutputBuffer) >> 1)) << 1;
				}
			case Charset::UTF32BE:
				return encode16_UTF32BE(utf16, lenUtf16, output, sizeOutputBuffer);
			case Charset::UTF32LE:
				return encode16_UTF32LE(utf16, lenUtf16, output, sizeOutputBuffer);
			default:
				return Encode16(utf16, lenUtf16, ToWindowsCodepage(charset), output, sizeOutputBuffer);
		}
	}
	
	sl_size Charsets::decode16(Charset charset, const void* input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer)
	{
		switch (charset) {
			case Charset::Unknown:
				return 0;
			case Charset::UTF8:
				return utf8ToUtf16((sl_char8*)input, sizeInput, utf16, lenUtf16Buffer);
			case Charset::UTF16BE:
				if (Endian::isBE()) {
					return Copy16(input, sizeInput >> 1, utf16, lenUtf16Buffer);
				} else {
					return Swap16(input, sizeInput >> 1, utf16, lenUtf16Buffer);
				}
			case Charset::UTF16LE:
				if (Endian::isLE()) {
					return Copy16(input, sizeInput >> 1, utf16, lenUtf16Buffer);
				} else {
					return Swap16(input, sizeInput >> 1, utf16, lenUtf16Buffer);
				}
			case Charset::UTF32BE:
				return decode16_UTF32BE(input, sizeInput, utf16, lenUtf16Buffer);
			case Charset::UTF32LE:
				return decode16_UTF32LE(input, sizeInput, utf16, lenUtf16Buffer);
			default:
				return Decode16(ToWindowsCodepage(charset), input, sizeInput, utf16, lenUtf16Buffer);
		}
	}
	
	String String::decode(Charset charset, const void* text, sl_size size)
	{
		if (size) {
			switch (charset) {
				case Charset::Unknown:
				case Charset::UTF8:
				case Charset::UTF16BE:
				case Charset::UTF16LE:
				case Charset::UTF32BE:
				case Charset::UTF32LE:
					{
						sl_size len = Charsets::decode8(charset, text, size, sl_null, -1);
						if (len) {
							String str = String::allocate(len);
							if (str.isNotNull()) {
								Charsets::decode8(charset, text, size, str.getData(), len);
								return str;
							}
						}
						break;
					}
				default:
					{
						sl_uint32 codepage = ToWindowsCodepage(charset);
						return DecodeString8(codepage, text, size);
					}
			}
		}
		return sl_null;
	}
	
	String String::decode(Charset charset, const Memory& mem)
	{
		return decode(charset, mem.getData(), mem.getSize());
	}
	
	String16 String16::decode(Charset charset, const void* text, sl_size size)
	{
		if (size) {
			switch (charset) {
				case Charset::Unknown:
				case Charset::UTF8:
				case Charset::UTF16BE:
				case Charset::UTF16LE:
				case Charset::UTF32BE:
				case Charset::UTF32LE:
					{
						sl_size len = Charsets::decode16(charset, text, size, sl_null, -1);
						if (len) {
							String16 str = String16::allocate(len);
							if (str.isNotNull()) {
								Charsets::decode16(charset, text, size, str.getData(), len);
								return str;
							}
						}
						break;
					}
				default:
					{
						sl_uint32 codepage = ToWindowsCodepage(charset);
						String16 t = DecodeString16(codepage, text, size);
						return t;
					}
			}
		}
		return sl_null;
	}
	
	String16 String16::decode(Charset charset, const Memory& mem)
	{
		return decode(charset, mem.getData(), mem.getSize());
	}
	
	Memory String::encode(Charset charset) const
	{
		sl_char8* src = getData();
		sl_size len = getLength();
		if (src && len) {
			switch (charset) {
				case Charset::Unknown:
				case Charset::UTF8:
				case Charset::UTF16BE:
				case Charset::UTF16LE:
				case Charset::UTF32BE:
				case Charset::UTF32LE:
					{
						sl_size size = Charsets::encode8(src, len, charset, sl_null, -1);
						if (size) {
							Memory mem = Memory::create(size);
							if (mem.isNotNull()) {
								Charsets::encode8(src, len, charset, mem.getData(), size);
								return mem;
							}
						}
						break;
					}
				default:
					{
						sl_uint32 codepage = ToWindowsCodepage(charset);
						return EncodeString8(src, len, codepage);
					}
			}
		}
		return sl_null;
	}
	
	Memory Atomic<String>::encode(Charset charset) const
	{
		String s(*this);
		return s.encode(charset);
	}
	
	Memory String16::encode(Charset charset) const
	{
		sl_char16* src = getData();
		sl_size len = getLength();
		if (src && len) {
			switch (charset) {
				case Charset::Unknown:
				case Charset::UTF8:
				case Charset::UTF16BE:
				case Charset::UTF16LE:
				case Charset::UTF32BE:
				case Charset::UTF32LE:
					{
						sl_size size = Charsets::encode16(src, len, charset, sl_null, -1);
						if (size) {
							Memory mem = Memory::create(size);
							if (mem.isNotNull()) {
								Charsets::encode16(src, len, charset, mem.getData(), size);
								return mem;
							}
						}
						break;
					}
				default:
					{
						sl_uint32 codepage = ToWindowsCodepage(charset);
						return EncodeString16(src, len, codepage);
					}
			}
		}
		return sl_null;
	}
	
	Memory Atomic<String16>::encode(Charset charset) const
	{
		String16 s(*this);
		return s.encode(charset);
	}
	
}
