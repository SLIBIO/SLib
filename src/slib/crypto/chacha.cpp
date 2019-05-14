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

/*

 Adapted from the public domain code by D. Bernstein
 
 https://github.com/floodyberry/supercop/blob/master/crypto_stream/chacha20/dolbeau/ppc-altivec/chacha.c

*/
 
#include "slib/crypto/chacha.h"

namespace slib
{
	
#define ROUNDS 20
	
#define ROTATE(v, c) (((v) << (c)) | ((v) >> (32 - (c))))
	
#define QUARTERROUND(a,b,c,d) \
	x[a] += x[b]; x[d] = ROTATE(x[d]^x[a], 16); \
	x[c] += x[d]; x[b] = ROTATE(x[b]^x[c], 12); \
	x[a] += x[b]; x[d] = ROTATE(x[d]^x[a], 8); \
	x[c] += x[d]; x[b] = ROTATE(x[b]^x[c], 7);
	
#define U8TO32_LITTLE(A,B,C,D) ((((sl_uint32)(sl_uint8)(A))) | (((sl_uint32)(sl_uint8)(B))<<8) | (((sl_uint32)(sl_uint8)(C))<<16) | (((sl_uint32)(sl_uint8)(D))<<24))

	namespace priv
	{
		
		namespace chacha
		{
			
			static void salsa20_wordtobyte(sl_uint8 output[64], const sl_uint32 input[12], sl_uint32 counter0, sl_uint32 counter1, sl_uint32 iv0, sl_uint32 iv1)
			{
				sl_uint32 x[16];
				sl_uint32 i;
				for (i = 0; i < 12; i++) {
					x[i] = input[i];
				}
				x[12] = counter0;
				x[13] = counter1;
				x[14] = iv0;
				x[15] = iv1;
				for (i = ROUNDS; i > 0; i -= 2) {
					QUARTERROUND(0, 4, 8, 12)
					QUARTERROUND(1, 5, 9, 13)
					QUARTERROUND(2, 6, 10, 14)
					QUARTERROUND(3, 7, 11, 15)
					QUARTERROUND(0, 5, 10, 15)
					QUARTERROUND(1, 6, 11, 12)
					QUARTERROUND(2, 7, 8, 13)
					QUARTERROUND(3, 4, 9, 14)
				}
				sl_uint8* t = output;
				for (i = 0; i < 16; i++) {
					sl_uint32 v = x[i] + input[i];
					t[0] = (sl_uint8)v;
					t[1] = (sl_uint8)(v >> 8);
					t[2] = (sl_uint8)(v >> 16);
					t[3] = (sl_uint8)(v >> 24);
					t += 4;
				}
			}
			
			static void salsa20_wordtobyte(const sl_uint8 data[64], sl_uint8 output[64], const sl_uint32 input[12], sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1)
			{
				sl_uint32 x[16];
				sl_uint32 i;
				for (i = 0; i < 12; i++) {
					x[i] = input[i];
				}
				x[12] = (sl_uint32)counter;
				x[13] = (sl_uint32)(counter >> 32);
				x[14] = iv0;
				x[15] = iv1;
				for (i = ROUNDS; i > 0; i -= 2) {
					QUARTERROUND(0, 4, 8, 12)
					QUARTERROUND(1, 5, 9, 13)
					QUARTERROUND(2, 6, 10, 14)
					QUARTERROUND(3, 7, 11, 15)
					QUARTERROUND(0, 5, 10, 15)
					QUARTERROUND(1, 6, 11, 12)
					QUARTERROUND(2, 7, 8, 13)
					QUARTERROUND(3, 4, 9, 14)
				}
				sl_uint8* t = output;
				const sl_uint8* u = data;
				for (i = 0; i < 16; i++) {
					sl_uint32 v = x[i] + input[i];
					t[0] = u[0] ^ (sl_uint8)v;
					t[1] = u[1] ^ (sl_uint8)(v >> 8);
					t[2] = u[2] ^ (sl_uint8)(v >> 16);
					t[3] = u[3] ^ (sl_uint8)(v >> 24);
					t += 4;
					u += 4;
				}
			}
			
		}
		
	}
	
	ChaCha20_Core::ChaCha20_Core()
	{
	}
	
	ChaCha20_Core::~ChaCha20_Core()
	{
	}
	
	sl_bool ChaCha20_Core::setKey(const void* _key, sl_uint32 lenKey)
	{
		const sl_uint8* key = (const sl_uint8*)_key;
		sl_uint32* input = m_input;
		if (lenKey == 32) {
			input[0] = U8TO32_LITTLE('e', 'x', 'p', 'a');
			input[1] = U8TO32_LITTLE('n', 'd', ' ', '3');
			input[2] = U8TO32_LITTLE('2', '-', 'b', 'y');
			input[3] = U8TO32_LITTLE('t', 'e', ' ', 'k');
			for (sl_uint32 i = 0; i < 8; i++) {
				sl_uint32 j = i << 2;
				input[4 + i] = U8TO32_LITTLE(key[j], key[j+1], key[j+2], key[j+3]);
			}
		} else if (lenKey == 16) {
			input[0] = U8TO32_LITTLE('e', 'x', 'p', 'a');
			input[1] = U8TO32_LITTLE('n', 'd', ' ', '1');
			input[2] = U8TO32_LITTLE('6', '-', 'b', 'y');
			input[3] = U8TO32_LITTLE('t', 'e', ' ', 'k');
			for (sl_uint32 i = 0; i < 4; i++) {
				sl_uint32 j = i << 2;
				input[4 + i] = U8TO32_LITTLE(key[j], key[j+1], key[j+2], key[j+3]);
				input[8 + i] = U8TO32_LITTLE(key[j], key[j+1], key[j+2], key[j+3]);
			}
		} else {
			return sl_false;
		}
		return sl_true;
	}
	
	void ChaCha20_Core::generateBlock(sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1, void* output) const
	{
		priv::chacha::salsa20_wordtobyte((sl_uint8*)output, m_input, (sl_uint32)counter, (sl_uint32)(counter >> 32), iv0, iv1);
	}
	
	void ChaCha20_Core::encryptBlock(sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1, const void* input, void* output) const
	{
		priv::chacha::salsa20_wordtobyte((const sl_uint8*)input, (sl_uint8*)output, m_input, counter, iv0, iv1);
	}
	
	void ChaCha20_Core::decryptBlock(sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1, const void* input, void* output) const
	{
		encryptBlock(counter, iv0, iv1, input, output);
	}
	
	
	ChaCha20::ChaCha20(): m_pos(0), m_flagCounter32(sl_false)
	{
	}
	
	ChaCha20::~ChaCha20()
	{
	}
	
	void ChaCha20::start(const void* _iv, sl_uint64 counter)
	{
		const sl_uint8* iv = (const sl_uint8*)_iv;
		m_iv[0] = U8TO32_LITTLE(iv[0], iv[1], iv[2], iv[3]);
		m_iv[1] = U8TO32_LITTLE(iv[4], iv[5], iv[6], iv[7]);
		m_counter = counter;
		m_pos = 0;
		m_flagCounter32 = sl_false;
	}
	
	void ChaCha20::start32(const void* _iv, sl_uint32 counter)
	{
		const sl_uint8* iv = (const sl_uint8*)_iv;
		m_iv[0] = U8TO32_LITTLE(iv[0], iv[1], iv[2], iv[3]);
		m_iv[1] = U8TO32_LITTLE(iv[4], iv[5], iv[6], iv[7]);
		m_iv[2] = U8TO32_LITTLE(iv[8], iv[9], iv[10], iv[11]);
		m_counter32 = counter;
		m_pos = 0;
		m_flagCounter32 = sl_true;
	}
	
	void ChaCha20::encrypt(const void* _src, void* _dst, sl_size len)
	{
		const sl_uint8* src = (const sl_uint8*)_src;
		sl_uint8* dst = (sl_uint8*)_dst;
		sl_uint8* y = m_output;
		sl_uint32 pos = m_pos;
		for (sl_size k = 0; k < len; k++) {
			if (!pos) {
				if (m_flagCounter32) {
					priv::chacha::salsa20_wordtobyte(y, m_input, m_counter32, m_iv[0], m_iv[1], m_iv[2]);
					m_counter32++;
				} else {
					priv::chacha::salsa20_wordtobyte(y, m_input, (sl_uint32)m_counter, (sl_uint32)(m_counter >> 32), m_iv[0], m_iv[1]);
					m_counter++;
				}
			}
			dst[k] = src[k] ^ y[pos];
			pos = (pos + 1) & 0x3F;
		}
		m_pos = pos;
	}
	
	void ChaCha20::decrypt(const void* src, void* dst, sl_size len)
	{
		encrypt(src, dst, len);
	}
	
	sl_bool ChaCha20::is32BitCounter()
	{
		return m_flagCounter32;
	}
	
	void ChaCha20::set32BitCounter(sl_bool flag)
	{
		m_flagCounter32 = flag;
	}
	
}
