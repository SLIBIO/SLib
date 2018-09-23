/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
