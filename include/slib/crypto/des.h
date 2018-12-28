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

#ifndef CHECKHEADER_SLIB_CRYPTO_DES
#define CHECKHEADER_SLIB_CRYPTO_DES

#include "definition.h"

#include "../core/string.h"

#include "block_cipher.h"

/*
 	DES - Data Encryption Standard

	User Key Size - 56 bits (7 bytes)
	Block Size - 64 bits (8 bytes)
*/

namespace slib
{

	class SLIB_EXPORT DES : public Object
	{
	public:
		enum {
			BlockSize = 8
		};
	public:
		DES();

		~DES();

	public:
		static sl_uint32 getBlockSize();

		// 64 bit (8 bytes), note that most significant bit of each byte is not used
		void setKey(const void* key);
		void setKey(sl_uint64 key);
		
		sl_uint64 encrypt(sl_uint64 data) const;
		
		sl_uint64 decrypt(sl_uint64 data) const;

		// 64 bits (8 bytes) block
		void encryptBlock(const void* src, void* dst) const;

		// 64 bits (8 bytes) block
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
		sl_uint64 m_K[16];
		
	};
	
	class SLIB_EXPORT TripleDES : public Object
	{
	public:
		enum {
			BlockSize = 8
		};
	public:
		TripleDES();
		
		~TripleDES();
		
	public:
		static sl_uint32 getBlockSize();
		
		// 3 x 64 bit (8 bytes), note that most significant bit of each byte is not used
		void setKey(const void* key1, const void* key2, const void* key3);
		void setKey(sl_uint64 key1, sl_uint64 key2, sl_uint64 key3);
		void setKey(const void* key1, const void* key2);
		void setKey(sl_uint64 key1, sl_uint64 key2);

		sl_uint64 encrypt(sl_uint64 data) const;
		
		sl_uint64 decrypt(sl_uint64 data) const;
		
		// 64 bits (8 bytes) block
		void encryptBlock(const void* src, void* dst) const;
		
		// 64 bits (8 bytes) block
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
		DES m_des1;
		DES m_des2;
		DES m_des3;

	};
	
}

#endif
