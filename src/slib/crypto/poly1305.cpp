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
 
 Adapted from the public domain code
 
 https://github.com/floodyberry/poly1305-donna
 
 */

#include "slib/crypto/poly1305.h"

#include "slib/core/mio.h"

namespace slib
{
	
#define U8TO32(A,B,C,D) ((((sl_uint32)(sl_uint8)(A))) | (((sl_uint32)(sl_uint8)(B))<<8) | (((sl_uint32)(sl_uint8)(C))<<16) | (((sl_uint32)(sl_uint8)(D))<<24))
	
	Poly1305::Poly1305()
	{
	}
	
	Poly1305::~Poly1305()
	{
	}
	
	void Poly1305::start(const void* _key)
	{
		const sl_uint8* key = (const sl_uint8*)_key;
		sl_uint32* r = m_r;
		sl_uint32* h = m_h;
		sl_uint32* pad = m_pad;
		
		// r &= 0xffffffc0ffffffc0ffffffc0fffffff
		r[0] = U8TO32(key[0], key[1], key[2], key[3]) & 0x3ffffff;
		r[1] = (U8TO32(key[3], key[4], key[5], key[6]) >> 2) & 0x3ffff03;
		r[2] = (U8TO32(key[6], key[7], key[8], key[9]) >> 4) & 0x3ffc0ff;
		r[3] = (U8TO32(key[9], key[10], key[11], key[12]) >> 6) & 0x3f03fff;
		r[4] = (U8TO32(key[12], key[13], key[14], key[15]) >> 8) & 0x00fffff;
		
		// h = 0
		h[0] = 0;
		h[1] = 0;
		h[2] = 0;
		h[3] = 0;
		h[4] = 0;
		
		// save pad for later
		pad[0] = U8TO32(key[16], key[17], key[18], key[19]);
		pad[1] = U8TO32(key[20], key[21], key[22], key[23]);
		pad[2] = U8TO32(key[24], key[25], key[26], key[27]);
		pad[3] = U8TO32(key[28], key[29], key[30], key[31]);
		
		m_leftOver = 0;
		m_flagFinal = sl_false;
	}
	
	void Poly1305::updateBlocks(const void* input, sl_size nBlocks)
	{
		const sl_uint8* m = (const sl_uint8*)input;
		
		const sl_uint32 hibit = m_flagFinal ? 0 : (1 << 24); // 1 << 128
		
		sl_uint32 r0 = m_r[0];
		sl_uint32 r1 = m_r[1];
		sl_uint32 r2 = m_r[2];
		sl_uint32 r3 = m_r[3];
		sl_uint32 r4 = m_r[4];
		
		sl_uint32 s1 = r1 * 5;
		sl_uint32 s2 = r2 * 5;
		sl_uint32 s3 = r3 * 5;
		sl_uint32 s4 = r4 * 5;
		
		sl_uint32 h0 = m_h[0];
		sl_uint32 h1 = m_h[1];
		sl_uint32 h2 = m_h[2];
		sl_uint32 h3 = m_h[3];
		sl_uint32 h4 = m_h[4];
		
		for (sl_size i = 0; i < nBlocks; i++) {
			// h += m[i]
			h0 += U8TO32(m[0], m[1], m[2], m[3]) & 0x3ffffff;
			h1 += (U8TO32(m[3], m[4], m[5], m[6]) >> 2) & 0x3ffffff;
			h2 += (U8TO32(m[6], m[7], m[8], m[9]) >> 4) & 0x3ffffff;
			h3 += (U8TO32(m[9], m[10], m[11], m[12]) >> 6) & 0x3ffffff;
			h4 += (U8TO32(m[12], m[13], m[14], m[15]) >> 8) | hibit;
			// h *= r
			sl_uint64 d0 = ((sl_uint64)h0 * r0) + ((sl_uint64)h1 * s4) + ((sl_uint64)h2 * s3) + ((sl_uint64)h3 * s2) + ((sl_uint64)h4 * s1);
			sl_uint64 d1 = ((sl_uint64)h0 * r1) + ((sl_uint64)h1 * r0) + ((sl_uint64)h2 * s4) + ((sl_uint64)h3 * s3) + ((sl_uint64)h4 * s2);
			sl_uint64 d2 = ((sl_uint64)h0 * r2) + ((sl_uint64)h1 * r1) + ((sl_uint64)h2 * r0) + ((sl_uint64)h3 * s4) + ((sl_uint64)h4 * s3);
			sl_uint64 d3 = ((sl_uint64)h0 * r3) + ((sl_uint64)h1 * r2) + ((sl_uint64)h2 * r1) + ((sl_uint64)h3 * r0) + ((sl_uint64)h4 * s4);
			sl_uint64 d4 = ((sl_uint64)h0 * r4) + ((sl_uint64)h1 * r3) + ((sl_uint64)h2 * r2) + ((sl_uint64)h3 * r1) + ((sl_uint64)h4 * r0);
			// (partial) h %= p
			h0 = (sl_uint32)d0 & 0x3ffffff;
			d1 += (sl_uint32)(d0 >> 26);
			h1 = (sl_uint32)d1 & 0x3ffffff;
			d2 += (sl_uint32)(d1 >> 26);
			h2 = (sl_uint32)d2 & 0x3ffffff;
			d3 += (sl_uint32)(d2 >> 26);
			h3 = (sl_uint32)d3 & 0x3ffffff;
			d4 += (sl_uint32)(d3 >> 26);
			h4 = (sl_uint32)d4 & 0x3ffffff;
			h0 += ((sl_uint32)(d4 >> 26)) * 5;
			sl_uint32 c = (h0 >> 26);
			h0 = h0 & 0x3ffffff;
			h1 += c;
			m += 16;
		}
		
		m_h[0] = h0;
		m_h[1] = h1;
		m_h[2] = h2;
		m_h[3] = h3;
		m_h[4] = h4;
	}
	
	void Poly1305::update(const void* input, sl_size n)
	{
		const sl_uint8* m = (const sl_uint8*)input;

		sl_uint32 leftOver = m_leftOver;
		if (leftOver) {
			sl_uint32 want = (16 - leftOver);
			if (want > n) {
				want = (sl_uint32)n;
			}
			for (sl_uint32 i = 0; i < want; i++) {
				m_buffer[leftOver + i] = m[i];
			}
			n -= want;
			m += want;
			m_leftOver += want;
			if (m_leftOver < 16) {
				return;
			}
			updateBlocks(m_buffer, 1);
			m_leftOver = 0;
		}
		
		if (n >= 16) {
			sl_size nBlocks = n >> 4;
			updateBlocks(m, nBlocks);
			sl_size t = nBlocks << 4;
			m += t;
			n -= t;
		}
		
		if (n) {
			sl_uint32 t = (sl_uint32)n;
			for (sl_uint32 i = 0; i < t; i++) {
				m_buffer[i] = m[i];
			}
			m_leftOver = t;
		}
	}
	
	void Poly1305::finish(void* output)
	{
		sl_uint8* mac = (sl_uint8*)output;
		
		sl_uint32 leftOver = m_leftOver;
		if (leftOver) {
			sl_uint32 i = leftOver;
			m_buffer[i] = 1;
			i++;
			for (; i < 16; i++) {
				m_buffer[i] = 0;
			}
			m_flagFinal = sl_true;
			updateBlocks(m_buffer, 1);
		}
		
		sl_uint32 h0 = m_h[0];
		sl_uint32 h1 = m_h[1];
		sl_uint32 h2 = m_h[2];
		sl_uint32 h3 = m_h[3];
		sl_uint32 h4 = m_h[4];
		
		sl_uint32 c = h1 >> 26; h1 = h1 & 0x3ffffff;
		h2 += h1 >> 26; c = h2 >> 26; h2 = h2 & 0x3ffffff;
		h3 += c; c = h3 >> 26; h3 = h3 & 0x3ffffff;
		h4 += c; c = h4 >> 26; h4 = h4 & 0x3ffffff;
		h0 += c * 5; c = h0 >> 26; h0 = h0 & 0x3ffffff;
		h1 += c;
		
		// compute h + -p
		sl_uint32 g0 = h0 + 5; c = g0 >> 26; g0 &= 0x3ffffff;
		sl_uint32 g1 = h1 + c; c = g1 >> 26; g1 &= 0x3ffffff;
		sl_uint32 g2 = h2 + c; c = g2 >> 26; g2 &= 0x3ffffff;
		sl_uint32 g3 = h3 + c; c = g3 >> 26; g3 &= 0x3ffffff;
		sl_uint32 g4 = h4 + c - (1 << 26);
		
		// select h if h < p, or h + -p if h >= p
		sl_uint32 mask = (g4 >> 31) - 1;
		g0 &= mask;
		g1 &= mask;
		g2 &= mask;
		g3 &= mask;
		g4 &= mask;
		mask = ~mask;
		h0 = (h0 & mask) | g0;
		h1 = (h1 & mask) | g1;
		h2 = (h2 & mask) | g2;
		h3 = (h3 & mask) | g3;
		h4 = (h4 & mask) | g4;
		
		// h = h % (2^128)
		h0 = ((h0      ) | (h1 << 26)) & 0xffffffff;
		h1 = ((h1 >>  6) | (h2 << 20)) & 0xffffffff;
		h2 = ((h2 >> 12) | (h3 << 14)) & 0xffffffff;
		h3 = ((h3 >> 18) | (h4 <<  8)) & 0xffffffff;
		
		// mac = (h + pad) % (2^128)
		sl_uint64 f = (sl_uint64)h0 + m_pad[0]; h0 = (sl_uint32)f;
		f = (sl_uint64)h1 + m_pad[1] + (f >> 32); h1 = (sl_uint32)f;
		f = (sl_uint64)h2 + m_pad[2] + (f >> 32); h2 = (sl_uint32)f;
		f = (sl_uint64)h3 + m_pad[3] + (f >> 32); h3 = (sl_uint32)f;
		
		MIO::writeUint32LE(mac, h0);
		MIO::writeUint32LE(mac + 4, h1);
		MIO::writeUint32LE(mac + 8, h2);
		MIO::writeUint32LE(mac + 12, h3);
	}
	
	void Poly1305::execute(const void* key, const void* message, sl_size lenMessage, void* output)
	{
		Poly1305 p;
		p.start(key);
		p.update(message, lenMessage);
		p.finish(output);
	}
	
}
