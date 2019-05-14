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

#include "poly1305.h"

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
		void generateBlock(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3, void* output) const;
		
		// input, output: 64 bytes
		void encryptBlock(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3, const void* input, void* output) const;
		
		// input, output: 64 bytes
		void decryptBlock(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3, const void* input, void* output) const;

	protected:
		sl_uint32 m_input[12];

	};
	
	class SLIB_EXPORT ChaCha20 : public ChaCha20_Core
	{
	public:
		ChaCha20();
		
		~ChaCha20();
		
	public:
		void start(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3);
		
		// IV: 8 bytes
		void start(const void* IV, sl_uint64 counter = 0);
		
		void start32(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3);

		// IV: 12 bytes
		void start32(const void* IV, sl_uint32 counter = 0);
		
		void encrypt(const void* src, void* dst, sl_size len);
		
		void decrypt(const void* src, void* dst, sl_size len);
		
		sl_bool is32BitCounter();
		
		void set32BitCounter(sl_bool flag = sl_true);

	private:
		sl_uint32 m_nonce[4];
		sl_uint8 m_output[64];
		sl_uint32 m_pos;
		sl_bool m_flagCounter32;
		
	};
	
	/*
	 	https://tools.ietf.org/html/rfc8439
	*/
	
	class SLIB_EXPORT ChaCha20_Poly1305
	{
	public:
		ChaCha20_Poly1305();
		
		~ChaCha20_Poly1305();
		
	public:
		// key: 32 bytes (256 bits)
		void setKey(const void* key);

		// IV: 8 bytes (64 bits)
		void start(sl_uint32 senderId, const void* IV);
		
		// put on AAD (additional authenticated data)
		void putAAD(const void* data, sl_size len);
		
		void finishAAD();
		
		void encrypt(const void* src, void* dst, sl_size len);
		
		void decrypt(const void* src, void* dst, sl_size len);
		
#ifdef check
#undef check
#endif
		// src: cipher text
		void check(const void* src, sl_size len);
		
		// outputTag: 16 bytes (128 bits)
		void finish(void* outputTag);
		
		// tag: 16 bytes (128 bits)
		sl_bool finishAndCheckTag(const void* tag);

		// IV: 8 bytes (64 bits)
		// outputTag: 16 bytes (128 bits)
		void encrypt(sl_uint32 senderId, const void* IV, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, void* outputTag);
		
		// IV: 8 bytes (64 bits)
		// tag: 16 bytes (128 bits)
		sl_bool decrypt(sl_uint32 senderId, const void* IV, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, const void* tag);
		
		// IV: 8 bytes (64 bits)
		// tag: 16 bytes (128 bits)
		sl_bool check(sl_uint32 senderId, const void* IV, const void* AAD, sl_size lenAAD, const void* src, sl_size len, const void* tag);
		
	private:
		ChaCha20 m_cipher;
		Poly1305 m_auth;
		sl_size m_lenAAD;
		sl_size m_lenInput;
		
	};

}

#endif
