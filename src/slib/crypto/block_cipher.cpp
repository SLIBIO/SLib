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

#include "slib/crypto/block_cipher.h"

namespace slib
{

/*
				BlockCipherPadding_PKCS7
 
	Defines padding method described in PKCS#5, PKCS#7.
 
	Padding is added at the end of the message as following.
 
	01
	02 02
	03 03 03
	04 04 04 04
	05 05 05 05 05
 
	only applicable up to 256-bytes
*/

	void BlockCipherPadding_PKCS7::addPadding(void* buf, sl_size padding)
	{
		sl_uint8* c = (sl_uint8*)buf;
		sl_uint8 n = (sl_uint8)padding;
		for (sl_uint8 i = 0; i < n; i++) {
			c[i] = n;
		}
	}

	sl_uint32 BlockCipherPadding_PKCS7::removePadding(const void* buf, sl_uint32 blockSize)
	{
		sl_uint8* c = (sl_uint8*)buf;
		sl_uint8 n = c[blockSize - 1];
		for (sl_uint32 i = blockSize - n; i < blockSize - 1; i++) {
			if (c[i] != n) {
				return 0;
			}
		}
		if (blockSize < n) {
			return 0;
		}
		return n;
	}

}
