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
