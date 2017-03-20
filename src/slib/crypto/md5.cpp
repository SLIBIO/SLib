/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/crypto/md5.h"

#include "../../../inc/slib/core/mio.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/math.h"

namespace slib
{

	MD5::MD5()
	{
		rdata_len = 0;
	}

	MD5::~MD5()
	{
	}

	void MD5::start()
	{
		A[0] = 0x67452301;
		A[1] = 0xefcdab89;
		A[2] = 0x98badcfe;
		A[3] = 0x10325476;
		rdata_len = 0;
		sizeTotalInput = 0;
	}

	void MD5::update(const void* _input, sl_size sizeInput)
	{
		if (rdata_len >= 64) {
			return;
		}
		const sl_uint8* input = (const sl_uint8*)_input;
		if (sizeInput == 0) {
			return;
		}
		sizeTotalInput += sizeInput;
		if (rdata_len > 0) {
			sl_uint32 n = 64 - rdata_len;
			if (sizeInput < n) {
				Base::copyMemory(rdata + rdata_len, input, sizeInput);
				rdata_len += (sl_uint32)sizeInput;
				return;
			} else {
				Base::copyMemory(rdata + rdata_len, input, n);
				_updateSection(rdata);
				rdata_len = 0;
				sizeInput -= n;
				input += n;
				if (sizeInput == 0) {
					return;
				}
			}
		}
		while (sizeInput >= 64) {
			_updateSection(input);
			sizeInput -= 64;
			input += 64;
		}
		if (sizeInput) {
			Base::copyMemory(rdata, input, sizeInput);
			rdata_len = (sl_uint32)sizeInput;
		}
	}

	void MD5::finish(void* _output)
	{
		if (rdata_len >= 64) {
			return;
		}
		rdata[rdata_len] = (sl_uint8)0x80;
		if (rdata_len < 56) {
			Base::zeroMemory(rdata + rdata_len + 1, 55 - rdata_len);
			MIO::writeUint64LE(rdata + 56, sizeTotalInput << 3);
			_updateSection(rdata);
		} else {
			Base::zeroMemory(rdata + rdata_len + 1, 63 - rdata_len);
			_updateSection(rdata);
			Base::zeroMemory(rdata, 56);
			MIO::writeUint64LE(rdata + 56, sizeTotalInput << 3);
			_updateSection(rdata);
		}
		rdata_len = 0;

		sl_uint8* output = (sl_uint8*)_output;
		MIO::writeUint32LE(output, A[0]);
		MIO::writeUint32LE(output + 4, A[1]);
		MIO::writeUint32LE(output + 8, A[2]);
		MIO::writeUint32LE(output + 12, A[3]);
	}

	void MD5::_updateSection(const sl_uint8* input)
	{
		static sl_uint32 K[64] = {
			0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
			0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
			0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
			0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
			0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
			0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
			0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
			0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
			0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
			0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
			0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
			0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
			0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
			0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
			0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
			0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

		const sl_uint32 S[64] = {
			7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
			5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
			4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
			6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

		sl_uint32 M[16];
		for (sl_uint32 i = 0; i < 16; i++) {
			M[i] = MIO::readUint32LE(input + (i << 2));
		}
			
		sl_uint32 a = A[0];
		sl_uint32 b = A[1];
		sl_uint32 c = A[2];
		sl_uint32 d = A[3];

		// 64 rounds
		for (sl_uint32 i = 0; i < 64; i++) {
			sl_uint32 f, g;
			if (i < 16) {
				f = (b & c) | ((~b) & d);
				g = i;
			} else if (i < 32) {
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			} else if (i < 48) {
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			} else {
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}
			sl_uint32 temp = d;
			d = c;
			c = b;
			b = b + Math::rotateLeft32((a + f + K[i] + M[g]), S[i]);
			a = temp;
		}

		A[0] += a;
		A[1] += b;
		A[2] += c;
		A[3] += d;
	}

}
