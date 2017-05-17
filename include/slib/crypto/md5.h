/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CRYPTO_MD5
#define CHECKHEADER_SLIB_CRYPTO_MD5

#include "definition.h"

#include "hash.h"

/*
	MD5 - Message Digest algorithm

	Output: 128bits (16 bytes)
*/

namespace slib
{
	
	class SLIB_EXPORT MD5 : public CryptoHash
	{
	public:
		MD5();

		~MD5();
	
	public:
		void start() override;

		void update(const void* input, sl_size n) override;

		void finish(void* output) override;

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static sl_uint32 getHashSize();

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const override;
	
	private:
		void _updateSection(const sl_uint8* input);
	
	private:
		sl_size sizeTotalInput;
		sl_uint32 rdata_len;
		sl_uint8 rdata[64];
		sl_uint32 A[4];
	};

}

#endif
