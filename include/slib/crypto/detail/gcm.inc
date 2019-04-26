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

namespace slib
{
	
	template <class CLASS>
	GCM<CLASS>::GCM()
	{
		setCipher(sl_null);
	}

	template <class CLASS>
	GCM<CLASS>::GCM(const CLASS* cipher)
	{
		setCipher(cipher);
	}

	template <class CLASS>
	GCM<CLASS>::~GCM()
	{
	}

	template <class CLASS>
	sl_bool GCM<CLASS>::setCipher(const CLASS* cipher)
	{
		if (cipher) {
			if (CLASS::BlockSize != 16) {
				return sl_false;
			}
			sl_uint8 H[16] = { 0 };
			cipher->encryptBlock(H, H);
			generateTable(H);
		}
		m_cipher = cipher;
		return sl_true;
	}

	template <class CLASS>
	sl_bool GCM<CLASS>::start(const void* IV, sl_size lenIV)
	{
		if (!m_cipher) {
			return sl_false;
		}
		calculateCIV(IV, lenIV, CIV);
		m_cipher->encryptBlock(CIV, GCTR0);
		Base::zeroMemory(GHASH_X, 16);
		return sl_true;
	}

	template <class CLASS>
	void GCM<CLASS>::encryptBlock(const void* src, void* dst, sl_uint32 n)
	{
		sl_uint8 GCTR[16];
		sl_size k;
		const sl_uint8* P = (const sl_uint8*)src;
		sl_uint8* C = (sl_uint8*)dst;
		increaseCIV();
		m_cipher->encryptBlock(CIV, GCTR);
		for (k = 0; k < n; k++) {
			*C = *P ^ GCTR[k];
			GHASH_X[k] ^= *C;
			C++;
			P++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	template <class CLASS>
	void GCM<CLASS>::encrypt(const void* src, void *dst, sl_size len)
	{
		sl_uint8 GCTR[16];
		sl_size i, k, n;
		const sl_uint8* P = (const sl_uint8*)src;
		sl_uint8* C = (sl_uint8*)dst;
		
		for (i = 0; i < len; i += 16) {
			increaseCIV();
			m_cipher->encryptBlock(CIV, GCTR);
			n = len - i;
			if (n > 16) {
				n = 16;
			}
			for (k = 0; k < n; k++) {
				*C = *P ^ GCTR[k];
				GHASH_X[k] ^= *C;
				C++;
				P++;
			}
			multiplyH(GHASH_X, GHASH_X);
		}
	}

	template <class CLASS>
	void GCM<CLASS>::decryptBlock(const void* src, void* dst, sl_uint32 n)
	{
		sl_uint8 GCTR[16];
		sl_size k;
		const sl_uint8* C = (const sl_uint8*)src;
		sl_uint8* P = (sl_uint8*)dst;
		increaseCIV();
		m_cipher->encryptBlock(CIV, GCTR);
		for (k = 0; k < n; k++) {
			GHASH_X[k] ^= *C;
			*P = *C ^ GCTR[k];
			C++;
			P++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	template <class CLASS>
	void GCM<CLASS>::decrypt(const void* src, void *dst, sl_size len)
	{
		sl_uint8 GCTR[16];
		sl_size i, k, n;
		const sl_uint8* C = (const sl_uint8*)src;
		sl_uint8* P = (sl_uint8*)dst;
		
		for (i = 0; i < len; i += 16) {
			increaseCIV();
			m_cipher->encryptBlock(CIV, GCTR);
			n = len - i;
			if (n > 16) {
				n = 16;
			}
			for (k = 0; k < n; k++) {
				GHASH_X[k] ^= *C;
				*P = *C ^ GCTR[k];
				C++;
				P++;
			}
			multiplyH(GHASH_X, GHASH_X);
		}
	}

	template <class CLASS>
	sl_bool GCM<CLASS>::encrypt(
					const void* IV, sl_size lenIV
					, const void* A, sl_size lenA
					, const void* input, void* output, sl_size len
					, void* tag, sl_size lenTag
	)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		encrypt(input, output, len);
		return finish(lenA, len, tag, lenTag);
	}

	template <class CLASS>
	sl_bool GCM<CLASS>::decrypt(
					const void* IV, sl_size lenIV
					, const void* A, sl_size lenA
					, const void* input, void* output, sl_size len
					, const void* tag, sl_size lenTag
	)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		decrypt(input, output, len);
		return finishAndCheckTag(lenA, len, tag, lenTag);
	}

	template <class CLASS>
	sl_bool GCM<CLASS>::check(
				const void* IV, sl_size lenIV
				, const void* A, sl_size lenA
				, const void* C, sl_size lenC
				, const void* tag, sl_size lenTag
	)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		put(C, lenC);
		return finishAndCheckTag(lenA, lenC, tag, lenTag);
	}

}
