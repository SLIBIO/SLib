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

#include "slib/crypto/base64.h"

#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_CHARS_URL "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"

namespace slib
{
	
	SLIB_INLINE String _priv_Base64_encode(const char* patterns, const void* buf, sl_size size, char padding)
	{
		if (size == 0) {
			return sl_null;
		}
		const sl_uint8* input = (const sl_uint8*)buf;
		sl_uint32 last = (sl_uint32)(size % 3);
		sl_size countBlock = (size / 3) + (last ? 1 : 0);
		String ret = String::allocate(countBlock << 2);
		if (ret.isEmpty()) {
			return ret;
		}
		sl_char8* output = ret.getData();
		sl_uint32 n = 0;
		for (sl_size iBlock = 0; iBlock < countBlock; iBlock++) {
			sl_uint8 n0 = input[0];
			sl_uint8 n1 = (n + 1 < size) ? input[1] : 0;
			sl_uint8 n2 = (n + 2 < size) ? input[2] : 0;
			output[0] = patterns[(n0 & 0xFC) >> 2];
			output[1] = patterns[((n0 & 0x03) << 4) + ((n1 & 0xF0) >> 4)];
			output[2] = patterns[((n1 & 0x0F) << 2) + ((n2 & 0xC0) >> 6)];
			output[3] = patterns[n2 & 0x3F];
			input += 3;
			n += 3;
			output += 4;
		}
		if (padding) {
			if (last == 1) {
				*(output - 2) = padding;
			}
			if (last >= 1) {
				*(output - 1) = padding;
			}
		} else {
			if (last == 1) {
				ret.setLength((countBlock << 2) - 2);
			} else if (last > 1) {
				ret.setLength((countBlock << 2) - 1);
			}
		}
		return ret;
	}

	String Base64::encode(const void* buf, sl_size size, sl_char8 padding)
	{
		return _priv_Base64_encode(BASE64_CHARS, buf, size, padding);
	}

	String Base64::encodeUrl(const void* buf, sl_size size, sl_char8 padding)
	{
		return _priv_Base64_encode(BASE64_CHARS_URL, buf, size, padding);
	}

	String Base64::encode(const Memory &mem, sl_char8 padding)
	{
		return _priv_Base64_encode(BASE64_CHARS, mem.getData(), mem.getSize(), padding);
	}

	String Base64::encodeUrl(const Memory &mem, sl_char8 padding)
	{
		return _priv_Base64_encode(BASE64_CHARS_URL, mem.getData(), mem.getSize(), padding);
	}

	String Base64::encode(const String& str, sl_char8 padding)
	{
		return _priv_Base64_encode(BASE64_CHARS, str.getData(), str.getLength(), padding);
	}

	String Base64::encodeUrl(const String& str, sl_char8 padding)
	{
		return _priv_Base64_encode(BASE64_CHARS_URL, str.getData(), str.getLength(), padding);
	}
	
	SLIB_INLINE sl_uint32 _priv_Base64_index(sl_char8 c)
	{
		if (c >= 'A' && c <= 'Z') {
			return c - 'A';
		}
		if (c >= 'a' && c <= 'z') {
			return 26 + (c - 'a');
		}
		if (c >= '0' && c <= '9') {
			return 52 + (c - '0');
		}
		if (c == '+') {
			return 62;
		}
		if (c == '/') {
			return 63;
		}
		if (c == '-') {
			return 62;
		}
		if (c == '_') {
			return 63;
		}
		return 64;
	}
	
	sl_size Base64::getDecodeOutputSize(sl_size len)
	{
		sl_size size = (len >> 2) * 3;
		if ((len & 3) == 2) {
			size++;
		} else if ((len & 3) == 3) {
			size += 2;
		}
		return size;
	}

	sl_size Base64::decode(const String& str, void* buf, sl_char8 padding)
	{
		sl_uint8* output = (sl_uint8*)buf;
		sl_size len = str.getLength();
		const sl_char8* input = str.getData();
		// trim right (CR, LF)
		while (len > 0) {
			sl_char16 ch = input[len - 1];
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
				len--;
			} else {
				break;
			}
		}
		sl_uint32 data[4];
		sl_size indexInput = 0;
		sl_size indexOutput = 0;
		sl_uint32 posInBlock = 0;
		while (indexInput < len) {
			sl_char8 ch = input[indexInput];
			if (SLIB_CHAR_IS_WHITE_SPACE(ch) || ch == padding) {
				indexInput++;
				continue;
			}
			sl_uint32 sig = _priv_Base64_index(ch);
			if (sig >= 64) {
				return 0;
			}
			data[posInBlock] = sig;
			switch (posInBlock) {
				case 0:
					++posInBlock;
					break;
				case 1:
					output[indexOutput] = (sl_uint8)((data[0] << 2) + ((data[1] & 0x30) >> 4));
					++indexOutput;
					++posInBlock;
					break;
				case 2:
					output[indexOutput] = (sl_uint8)(((data[1] & 0xf) << 4) + ((data[2] & 0x3c) >> 2));
					++indexOutput;
					++posInBlock;
					break;
				case 3:
					output[indexOutput] = (sl_uint8)(((data[2] & 0x3) << 6) + data[3]);
					++indexOutput;
					posInBlock = 0;
					break;
			}
			indexInput++;
		}
		return indexOutput;
	}

	Memory Base64::decode(const String& base64, sl_char8 padding)
	{
		sl_size size = getDecodeOutputSize(base64.getLength());
		Memory mem = Memory::create(size);
		if (mem.isNull()) {
			return sl_null;
		}
		sl_size sizeOutput = decode(base64, mem.getData(), padding);
		if (sizeOutput) {
			if (size == sizeOutput) {
				return mem;
			} else {
				return mem.sub(0, sizeOutput);
			}
		}
		return sl_null;
	}

}
