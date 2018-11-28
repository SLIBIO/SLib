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
