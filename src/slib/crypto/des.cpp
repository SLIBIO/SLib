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

#include "slib/crypto/des.h"

#include "slib/core/mio.h"

/*
	DES - Data Encryption Standard

	https://csrc.nist.gov/CSRC/media/Publications/fips/46/3/archive/1999-10-25/documents/fips46-3.pdf
*/

namespace slib
{

	// Permuted choice 1
	static const sl_uint8 _priv_DES_PC1[] = {
		57, 49, 41, 33, 25, 17,  9,
		1, 58, 50, 42, 34, 26, 18,
		10,  2, 59, 51, 43, 35, 27,
		19, 11,  3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14,  6, 61, 53, 45, 37, 29,
		21, 13,  5, 28, 20, 12,  4
	};
	
	// Permuted choice 2
	static const char _priv_DES_PC2[] = {
		14, 17, 11, 24,  1,  5,
		3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8,
		16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32
	};
	
	// Number Of Left Shifts
	static const char _priv_DES_LS[] = {1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1};
	
	// Initial Permutation Table
	static const sl_uint8 _priv_DES_IP[] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};
	
	// Inverse of Initial Permutation Table
	static const sl_uint8 _priv_DES_IIP[] = {
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41,  9, 49, 17, 57, 25
	};
	
	// E Bit-Selection Table
	static const sl_uint8 _priv_DES_E[] = {
		32,  1,  2,  3,  4,  5,
		4,  5,  6,  7,  8,  9,
		8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32,  1
	};
	
	static const sl_uint8 _priv_DES_S[8][64] = {
		{
			14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
			15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
		},
		{
			15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
		},
		{
			10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
		},
		{
			7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
		},
		{
			2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
		},
		{
			12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
		},
		{
			4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
		},
		{
			13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
			2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
		}
	};
	
	static const sl_uint8 _priv_DES_P[] = {
		16,  7, 20, 21,
		29, 12, 28, 17,
		1, 15, 23, 26,
		5, 18, 31, 10,
		2,  8, 24, 14,
		32, 27,  3,  9,
		19, 13, 30,  6,
		22, 11,  4, 25
	};


	DES::DES()
	{
	}

	DES::~DES()
	{
	}

	void DES::setKey(const void* key)
	{
		setKey(MIO::readUint64BE(key));
	}
	
	void DES::setKey(sl_uint64 key)
	{
		int i, j;
		sl_uint64 PC1 = 0;
		for (i = 0; i < 56; i++) {
			PC1 = (PC1 << 1) | ((key >> (64-_priv_DES_PC1[i])) & 1);
		}
		sl_uint32 C = (sl_uint32)(PC1 >> 28);
		sl_uint32 D = (sl_uint32)((PC1 & 0xfffffff));
		for (i = 0; i < 16; i++) {
			for (j = 0; j < _priv_DES_LS[i]; j++) {
				C = ((C << 1) | (C >> 27)) & 0xfffffff;
				D = ((D << 1) | (D >> 27)) & 0xfffffff;
			}
			sl_uint64 PC2 = (((sl_uint64)C) << 28) | D;
			sl_uint64 K = 0;
			for (j = 0; j < 48; j++) {
				K = (K << 1) | ((PC2 >> (56-_priv_DES_PC2[j])) & 1);
			}
			m_K[i] = K;
		}
	}
	
	SLIB_INLINE sl_uint64 _priv_DES_doInitialPermutation(sl_uint64 n)
	{
		sl_uint64 m = 0;
		for (int i = 0; i < 64; i++) {
			m = (m << 1) | ((n >> (64 - _priv_DES_IP[i])) & 1);
		}
		return m;
	}
	
	SLIB_INLINE sl_uint64 _priv_DES_doInverseInitialPermutation(sl_uint64 n)
	{
		sl_uint64 m = 0;
		for (int i = 0; i < 64; i++) {
			m = (m << 1) | ((n >> (64 - _priv_DES_IIP[i])) & 1);
		}
		return m;
	}
	
	SLIB_INLINE sl_uint32 _priv_DES_f(sl_uint32 R, sl_uint64 K)
	{
		int i;
		sl_uint64 M = 0;
		for (i = 0; i < 48; i++) {
			M = (M << 1) | ((R >> (32 - _priv_DES_E[i])) & 1);
		}
		M ^= K;
		sl_uint32 N = 0;
		for (i = 0; i < 8; i++) {
			int B = ((int)(M >> (42 - 6 * i))) & 0x3F;
			int row = (B & 1) | ((B >> 4) & 2);
			int column = (B >> 1) & 15;
			N = (N << 4) | _priv_DES_S[i][(row << 4) | column];
		}
		sl_uint32 f = 0;
		for (i = 0; i < 32; i++) {
			f = (f << 1) | ((N >> (32 - _priv_DES_P[i])) & 1);
		}
		return f;

	}
	
	sl_uint64 DES::encrypt(sl_uint64 n) const
	{
		n = _priv_DES_doInitialPermutation(n);
		sl_uint32 L = (sl_uint32)(n >> 32);
		sl_uint32 R = (sl_uint32)(n & 0xffffffff);
		for (int i = 0; i < 16; i++) {
			sl_uint32 f = _priv_DES_f(R, m_K[i]);
			sl_uint32 t = R;
			R = L ^ f;
			L = t;
		}
		n = SLIB_MAKE_QWORD4(R, L);
		n = _priv_DES_doInverseInitialPermutation(n);
		return n;
	}
	
	sl_uint64 DES::decrypt(sl_uint64 n) const
	{
		n = _priv_DES_doInitialPermutation(n);
		sl_uint32 L = (sl_uint32)(n >> 32);
		sl_uint32 R = (sl_uint32)(n & 0xffffffff);
		for (int i = 0; i < 16; i++) {
			sl_uint32 f = _priv_DES_f(R, m_K[15 - i]);
			sl_uint32 t = R;
			R = L ^ f;
			L = t;
		}
		n = SLIB_MAKE_QWORD4(R, L);
		n = _priv_DES_doInverseInitialPermutation(n);
		return n;
	}
	
	void DES::encryptBlock(const void* src, void* dst) const
	{
		sl_uint64 n = encrypt(MIO::readUint64BE(src));
		MIO::writeUint64BE(dst, n);
	}
	
	void DES::decryptBlock(const void* src, void* dst) const
	{
		sl_uint64 n = decrypt(MIO::readUint64BE(src));
		MIO::writeUint64BE(dst, n);
	}

	
	TripleDES::TripleDES()
	{
	}
	
	TripleDES::~TripleDES()
	{
	}
	
	void TripleDES::setKey(const void* key1, const void* key2, const void* key3)
	{
		m_des1.setKey(key1);
		m_des2.setKey(key2);
		m_des3.setKey(key3);
	}
	
	void TripleDES::setKey(sl_uint64 key1, sl_uint64 key2, sl_uint64 key3)
	{
		m_des1.setKey(key1);
		m_des2.setKey(key2);
		m_des3.setKey(key3);
	}
	
	void TripleDES::setKey(const void* key1, const void* key2)
	{
		setKey(key1, key2, key1);
	}
	
	void TripleDES::setKey(sl_uint64 key1, sl_uint64 key2)
	{
		setKey(key1, key2, key1);
	}
	
	sl_uint64 TripleDES::encrypt(sl_uint64 n) const
	{
		return m_des3.encrypt(m_des2.decrypt(m_des1.encrypt(n)));
	}
	
	sl_uint64 TripleDES::decrypt(sl_uint64 n) const
	{
		return m_des1.decrypt(m_des2.encrypt(m_des3.decrypt(n)));
	}
	
	void TripleDES::encryptBlock(const void* src, void* dst) const
	{
		sl_uint64 n = encrypt(MIO::readUint64BE(src));
		MIO::writeUint64BE(dst, n);
	}
	
	void TripleDES::decryptBlock(const void* src, void* dst) const
	{
		sl_uint64 n = decrypt(MIO::readUint64BE(src));
		MIO::writeUint64BE(dst, n);
	}
	
}
