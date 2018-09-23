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
	
	class SLIB_EXPORT _priv_SHA256Base : public CryptoHash
	{
	public:
		_priv_SHA256Base();

		~_priv_SHA256Base();

	public:
		void update(const void* input, sl_size n) final;
	
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
	
	class SLIB_EXPORT SHA224 : public _priv_SHA256Base
	{
	public:
		enum {
			HashSize = 28,
			BlockSize = 64
		};
		
	public:
		SHA224();

		~SHA224();

	public:
		void start() final;

		void finish(void* output) final;
	
	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const final;

	};
	
	class SLIB_EXPORT SHA256 : public _priv_SHA256Base
	{
	public:
		enum {
			HashSize = 32,
			BlockSize = 64
		};
		
	public:
		SHA256();

		~SHA256();

	public:
		void start() final;

		void finish(void* output) final;

	public:
		static sl_uint32 make32bitChecksum(const void* input, sl_size n);

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const final;

	};
	
	class SLIB_EXPORT _priv_SHA512Base : public CryptoHash
	{
	public:
		_priv_SHA512Base();

		~_priv_SHA512Base();
	
	public:
		void update(const void* input, sl_size n) final;
	
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
	
	class SLIB_EXPORT SHA384 : public _priv_SHA512Base
	{
	public:
		enum {
			HashSize = 48,
			BlockSize = 128
		};
		
	public:
		SHA384();

		~SHA384();

	public:
		void start() final;

		void finish(void* output) final;

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const final;

	};
	
	class SLIB_EXPORT SHA512 : public _priv_SHA512Base
	{
	public:
		enum {
			HashSize = 64,
			BlockSize = 128
		};
		
	public:
		SHA512();

		~SHA512();

	public:
		void start() final;

		void finish(void* output) final;

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const final;

	};

}

#endif
