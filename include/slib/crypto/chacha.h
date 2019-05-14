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

#ifndef CHECKHEADER_SLIB_CRYPTO_CHACHA
#define CHECKHEADER_SLIB_CRYPTO_CHACHA

#include "definition.h"

/*
	ChaCha20

 https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
 
 Stream Cipher
 	Key Size: 128 bits (16 bytes), 256 bits (32 bytes)
 	State Size: 512 bits (64 bytes)
*/

namespace slib
{

	class SLIB_EXPORT ChaCha20_Core
	{
	public:
		ChaCha20_Core();
		
		~ChaCha20_Core();
		
	public:
		// Recommended Key Size = 32 bytes (256 bits), lenKey = 16, 32 bytes
		sl_bool setKey(const void* key, sl_uint32 lenKey);
		
		// output: 64 bytes
		void generateBlock(sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1, void* output) const;
		
		// input, output: 64 bytes
		void encryptBlock(sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1, const void* input, void* output) const;
		
		// input, output: 64 bytes
		void decryptBlock(sl_uint64 counter, sl_uint32 iv0, sl_uint32 iv1, const void* input, void* output) const;

	protected:
		sl_uint32 m_input[12];

	};
	
	class SLIB_EXPORT ChaCha20 : public ChaCha20_Core
	{
	public:
		ChaCha20();
		
		~ChaCha20();
		
	public:
		// IV: 8 bytes
		void start(const void* IV, sl_uint64 counter = 0);
		
		void encrypt(const void* src, void* dst, sl_size len);
		
		void decrypt(const void* src, void* dst, sl_size len);
		
		sl_bool is32BitCounter();
		
		void set32BitCounter(sl_bool flag = sl_true);

	private:
		union {
			sl_uint64 m_counter;
			sl_uint32 m_counter32;
		};
		sl_uint32 m_iv[2];
		sl_uint8 m_output[64];
		sl_uint32 m_pos;
		sl_bool m_flagCounter32;
		
	};

}

#endif
