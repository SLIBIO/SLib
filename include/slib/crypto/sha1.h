/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CRYPTO_SHA1
#define CHECKHEADER_SLIB_CRYPTO_SHA1

#include "definition.h"

#include "hash.h"

/*
	SHA1 - Secure Hash Algorithm

	Output: 160bits (20 bytes)
*/

namespace slib
{
	
	class SLIB_EXPORT SHA1 : public CryptoHash
	{
	public:
		enum {
			HashSize = 20,
			BlockSize = 64
		};
		
	public:
		SHA1();

		~SHA1();
	
	public:
		void start() final;

		void update(const void* input, sl_size n) final;

		void finish(void* output) final;

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const final;

	private:
		void _updateSection(const sl_uint8* input);
	
	private:
		sl_size sizeTotalInput;
		sl_uint32 rdata_len;
		sl_uint8 rdata[64];
		sl_uint32 h[5];

	};

}

#endif
