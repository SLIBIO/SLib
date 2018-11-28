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

#ifndef CHECKHEADER_SLIB_CRYPTO_HMAC
#define CHECKHEADER_SLIB_CRYPTO_HMAC

#include "definition.h"

/*
	HMAC: Keyed-Hashing for Message Authentication (RFC-2104)
*/

namespace slib
{
	
	template <class HASH>
	class SLIB_EXPORT HMAC
	{
	public:
		static void execute(const void* _key, sl_size lenKey, const void* message, sl_size lenMessage, void* output)
		{
			sl_size i;
			const sl_uint8* key = (const sl_uint8*)_key;
			sl_uint8 keyLocal[HASH::BlockSize];
			if (lenKey > HASH::BlockSize) {
				HASH::hash(key, lenKey, keyLocal);
				for (i = HASH::HashSize; i < HASH::BlockSize; i++) {
					keyLocal[i] = 0;
				}
				key = keyLocal;
			} else if (lenKey < HASH::BlockSize) {
				for (i = 0; i < lenKey; i++) {
					keyLocal[i] = key[i];
				}
				for (; i < HASH::BlockSize; i++) {
					keyLocal[i] = 0;
				}
				key = keyLocal;
			}
			// hash(o_key_pad | hash(i_key_pad | message)), i_key_pad = key xor [0x36 * BlockSize], o_key_pad = key xor [0x5c * BlockSize]
			HASH hash;
			hash.start();
			sl_uint8 key_pad[HASH::BlockSize];
			for (i = 0; i < HASH::BlockSize; i++) {
				key_pad[i] = key[i] ^ 0x36;
			}
			hash.update(key_pad, HASH::BlockSize);
			hash.update(message, lenMessage);
			hash.finish(output);
			
			hash.start();
			for (i = 0; i < HASH::BlockSize; i++) {
				key_pad[i] = key[i] ^ 0x5c;
			}
			hash.update(key_pad, HASH::BlockSize);
			hash.update(output, HASH::HashSize);
			hash.finish(output);
		}
		
	};

}

#endif
