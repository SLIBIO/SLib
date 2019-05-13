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

#include "slib/crypto/rc4.h"

#include "slib/core/cpp.h"

namespace slib
{
	
	RC4::RC4() : m_i(0), m_j(0)
	{
	}
	
	RC4::~RC4()
	{
	}
	
	sl_bool RC4::setKey(const void* _key, sl_uint32 lenKey)
	{
		if (lenKey < 5 || lenKey > 256) {
			return sl_false;
		}
		sl_uint8* S = m_S;
		const sl_uint8* key = (const sl_uint8*)_key;
		sl_uint32 i;
		for (i = 0; i < 256; i++) {
			S[i] = (sl_uint8)i;
		}
		sl_uint32 j = 0;
		for (i = 0; i < 256; i++) {
			j = (sl_uint8)(j + S[i] + key[i % lenKey]);
			Swap(S[i], S[j]);
		}
		m_i = 0;
		m_j = 0;
		return sl_true;
	}
	
	void RC4::encrypt(const void* _src, void* _dst, sl_size len)
	{
		const sl_uint8* src = (const sl_uint8*)_src;
		sl_uint8* dst = (sl_uint8*)_dst;
		sl_uint8* S = m_S;
		sl_uint8& i = m_i;
		sl_uint8& j = m_j;
		for (sl_size k = 0; k < len; k++) {
			i = i + 1;
			j = j + S[i];
			Swap(S[i], S[j]);
			sl_uint8 K = S[(sl_uint8)(S[i] + S[j])];
			dst[k] = src[k] ^ K;
		}
	}
	
	void RC4::decrypt(const void* src, void* dst, sl_size len)
	{
		encrypt(src, dst, len);
	}
	
}
