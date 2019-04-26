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

	/*
	 Public-Key Cryptography Standards (PKCS) #1: RSA Cryptography Specifications Version 2.1
	 Appendix B.2 Masking Generation Functions
	 
	 https://tools.ietf.org/html/rfc3447#appendix-B.2
	 */
	template <class CLASS>
	void CryptoHash<CLASS>::applyMask_MGF1(const void* seed, sl_uint32 sizeSeed, void* _target, sl_uint32 sizeTarget)
	{
		CLASS* thiz = ((CLASS*)this);
		sl_uint32 n = CLASS::HashSize;
		if (n == 0) {
			return;
		}
		SLIB_SCOPED_BUFFER(sl_uint8, 128, h, n);
		sl_uint8* target = (sl_uint8*)(_target);
		sl_uint32 i = 0;
		sl_uint8 C[4];
		while (sizeTarget >= n) {
			thiz->start();
			thiz->update(seed, sizeSeed);
			MIO::writeUint32BE(C, i);
			thiz->update(C, 4);
			thiz->finish(h);
			for (sl_uint32 k = 0; k < n; k++) {
				target[k] ^= h[k];
			}
			i++;
			target += n;
			sizeTarget -= n;
		}
		thiz->start();
		thiz->update(seed, sizeSeed);
		MIO::writeUint32BE(C, i);
		thiz->update(C, 4);
		thiz->finish(h);
		for (sl_uint32 k = 0; k < sizeTarget; k++) {
			target[k] ^= h[k];
		}
	}
	
}
