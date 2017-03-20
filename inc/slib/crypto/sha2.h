/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CRYPTO_SHA2
#define CHECKHEADER_SLIB_CRYPTO_SHA2

#include "definition.h"

#include "hash.h"

/*
	SHA2 - Secure Hash Algorithm

	Output:
		SHA224 - 224bits (28 bytes)
		SHA256 - 256bits (32 bytes)
		SHA384 - 384bits (48 bytes)
		SHA512 - 512bits (64 bytes)
*/

namespace slib
{
	
	class SLIB_EXPORT _SHA256Base : public CryptoHash
	{
	public:
		_SHA256Base();

		~_SHA256Base();

	public:
		// override
		void update(const void* input, sl_size n);
	
	protected:
		void _start();

		void _finish();

		void _updateSection(const sl_uint8* input);
	
	protected:
		sl_size sizeTotalInput;
		sl_uint32 rdata_len;
		sl_uint8 rdata[64];
		sl_uint32 h[8];
	};
	
	class SLIB_EXPORT SHA224 : public _SHA256Base
	{
	public:
		SHA224();

		~SHA224();

	public:
		// override
		void start();

		// override
		void finish(void* output);
	
	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static sl_uint32 getHashSize();

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const;

	};
	
	class SLIB_EXPORT SHA256 : public _SHA256Base
	{
	public:
		SHA256();

		~SHA256();

	public:
		// override
		void start();

		// override
		void finish(void* output);

	public:
		static sl_uint32 make32bitChecksum(const void* input, sl_size n);

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static sl_uint32 getHashSize();

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const;

	};
	
	class SLIB_EXPORT _SHA512Base : public CryptoHash
	{
	public:
		_SHA512Base();

		~_SHA512Base();
	
	public:
		// override
		void update(const void* input, sl_size n);
	
	protected:
		void _start();

		void _finish();

		void _updateSection(const sl_uint8* input);
	
	protected:
		sl_size sizeTotalInput;
		sl_uint32 rdata_len;
		sl_uint8 rdata[128];
		sl_uint64 h[8];

	};
	
	class SLIB_EXPORT SHA384 : public _SHA512Base
	{
	public:
		SHA384();

		~SHA384();

	public:
		// override
		void start();

		// override
		void finish(void* output);

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static sl_uint32 getHashSize();

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const;

	};
	
	class SLIB_EXPORT SHA512 : public _SHA512Base
	{
	public:
		SHA512();

		~SHA512();

	public:
		// override
		void start();

		// override
		void finish(void* output);

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static sl_uint32 getHashSize();

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const;

	};

}

#endif
