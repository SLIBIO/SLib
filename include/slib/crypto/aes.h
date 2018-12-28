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

#ifndef CHECKHEADER_SLIB_CRYPTO_AES
#define CHECKHEADER_SLIB_CRYPTO_AES

#include "definition.h"

#include "../core/string.h"

#include "block_cipher.h"
#include "gcm.h"

/*
	AES - Advanced Encryption Standard

	User Key Size - 128 bits (16 bytes), 192 bits (24 bytes), 256 bits (32 bytes)
	Block Size - 128 bits (16 bytes)
*/

namespace slib
{

	class SLIB_EXPORT AES : public Object
	{
	public:
		enum {
			BlockSize = 16
		};
	public:
		AES();

		~AES();

	public:
		static sl_uint32 getBlockSize();

		sl_bool setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);

		void setKey_SHA256(const String& key);
		
		void encrypt(sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3) const;
	
		void decrypt(sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3) const;
		
		// 128 bits (16 bytes) block
		void encryptBlock(const void* src, void* dst) const;

		// 128 bits (16 bytes) block
		void decryptBlock(const void* src, void* dst) const;

	public: /* common functions for block ciphers */
		sl_size encryptBlocks(const void* src, void* dst, sl_size size) const;

		sl_size decryptBlocks(const void* src, void* dst, sl_size size) const;

		sl_size encrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const;

		sl_size decrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const;

		sl_size encrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const;

		sl_size decrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const;

		sl_size encrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const;

		sl_size decrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const;

		Memory encrypt_CBC_PKCS7Padding(const void* src, sl_size size) const;

		Memory decrypt_CBC_PKCS7Padding(const void* src, sl_size size) const;

		Memory encrypt_CBC_PKCS7Padding(const Memory& mem) const;

		Memory decrypt_CBC_PKCS7Padding(const Memory& mem) const;

		sl_size encrypt_CTR(const void* input, sl_size size, void* output, void* counter, sl_uint32 offset = 0) const;

		sl_size encrypt_CTR(const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output) const;

		sl_size encrypt_CTR(const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output) const;

	private:
		sl_uint32 m_roundKeyEnc[64];
		sl_uint32 m_roundKeyDec[64];
		sl_uint32 m_nCountRounds;

	};
	
	class SLIB_EXPORT AES_GCM : public Object, public GCM<AES>
	{
	public:
		AES_GCM();

		~AES_GCM();

	public:
		void setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);
	
		void setKey_SHA256(const String& key);

	private:
		AES m_cipher;

	};

}

#endif
