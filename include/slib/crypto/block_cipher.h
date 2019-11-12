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

#ifndef CHECKHEADER_SLIB_CRYPTO_BLOCKCIPHER
#define CHECKHEADER_SLIB_CRYPTO_BLOCKCIPHER

#include "definition.h"

#include "../core/memory.h"
#include "../core/base.h"
#include "../core/mio.h"
#include "../core/math.h"

namespace slib
{

/*
	Defines padding method described in PKCS#5, PKCS#7.

	Padding is added at the end of the message as following.

	01
	02 02
	03 03 03
	04 04 04 04
	05 05 05 05 05

	only applicable up to 256-bytes
*/
	class SLIB_EXPORT BlockCipherPadding_PKCS7
	{
	public:
		static void addPadding(void* buf, sl_size padding);

		static sl_uint32 removePadding(const void* buf, sl_uint32 blockSize);

	};

/*
	Electronic codebook (ECB)
*/
	template <class CLASS, class PADDING>
	class SLIB_EXPORT BlockCipher_ECB
	{
	public:
		// Output Size = (size / block + 1) * block (< size + block)
		static sl_size encrypt(const CLASS* crypto, const void* _src, sl_size size, void* _dst);

		static Memory encrypt(const CLASS* crypto, const void* _src, sl_size size);

		// destination buffer size must equals to or greater than size
		static sl_size decrypt(const CLASS* crypto, const void* _src, sl_size size, void* _dst);

		static Memory decrypt(const CLASS* crypto, const void* _src, sl_size size);
		
	};

/*
	Cipher-block chaining (CBC)
*/
	template <class CLASS, class PADDING>
	class SLIB_EXPORT BlockCipher_CBC
	{
	public:
		// Output Size = (size / block + 1) * block (< size + block)
		static sl_size encrypt(const CLASS* crypto, const void* iv, const void* src, sl_size size, void* dst);

		static Memory encrypt(const CLASS* crypto, const void* iv, const void* src, sl_size size);

		// Output Size = (size / block + 2) * block (< size + block*2)
		static sl_size encrypt(const CLASS* crypto, const void* src, sl_size size, void* dst);

		static Memory encrypt(const CLASS* crypto, const void* src, sl_size size);

		// destination buffer size must equals to or greater than size
		static sl_size decrypt(const CLASS* crypto, const void* iv, const void* src, sl_size size, void* dst);

		static Memory decrypt(const CLASS* crypto, const void* iv, const void* src, sl_size size);

		// destination buffer size must equals to or greater than size
		static sl_size decrypt(const CLASS* crypto, const void* src, sl_size size, void* dst);

		static Memory decrypt(const CLASS* crypto, const void* src, sl_size size);

	};

/*
	Counter Mode (CTR)
*/
	template <class CLASS>
	class SLIB_EXPORT BlockCipher_CTR
	{
	public:
		static sl_size encrypt(const CLASS* crypto, const void* input, sl_size size, void* output, void* counter, sl_uint32 offset);

		static sl_size encrypt(const CLASS* crypto, const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output);

		static sl_size encrypt(const CLASS* crypto, const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output);

	};
	
	template <class CLASS>
	class SLIB_EXPORT BlockCipher
	{
	public:
		void encryptBlocks(const void* _src, void* _dst, sl_size size) const
		{
			const sl_uint8* src = (const sl_uint8*)_src;
			sl_uint8* dst = (sl_uint8*)_dst;
			sl_size nBlocks = size / CLASS::BlockSize;
			for (sl_size i = 0; i < nBlocks; i++) {
				((CLASS*)this)->encryptBlock(src, dst);
				src += CLASS::BlockSize;
				dst += CLASS::BlockSize;
			}
		}
		
		void decryptBlocks(const void* _src, void* _dst, sl_size size) const
		{
			const sl_uint8* src = (const sl_uint8*)_src;
			sl_uint8* dst = (sl_uint8*)_dst;
			sl_size nBlocks = size / CLASS::BlockSize;
			for (sl_size i = 0; i < nBlocks; i++) {
				((CLASS*)this)->decryptBlock(src, dst);
				src += CLASS::BlockSize;
				dst += CLASS::BlockSize;
			}
		}
		
		sl_size encrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const
		{
			return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, src, size, dst);
		}
		
		sl_size decrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const
		{
			return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, src, size, dst);
		}
		
		Memory encrypt_ECB_PKCS7Padding(const void* src, sl_size size) const
		{
			return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, src, size);
		}
		
		Memory decrypt_ECB_PKCS7Padding(const void* src, sl_size size) const
		{
			return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, src, size);
		}
		
		Memory encrypt_ECB_PKCS7Padding(const Memory& mem) const
		{
			return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, mem.getData(), mem.getSize());
		}
		
		Memory decrypt_ECB_PKCS7Padding(const Memory& mem) const
		{
			return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, mem.getData(), mem.getSize());
		}
		
		sl_size encrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, iv, src, size, dst);
		}
		
		sl_size decrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, iv, src, size, dst);
		}
		
		sl_size encrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, src, size, dst);
		}
		
		sl_size decrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, src, size, dst);
		}
		
		Memory encrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, iv, src, size);
		}
		
		Memory decrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, iv, src, size);
		}
	
		Memory encrypt_CBC_PKCS7Padding(const void* src, sl_size size) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, src, size);
		}
		
		Memory decrypt_CBC_PKCS7Padding(const void* src, sl_size size) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, src, size);
		}
		
		Memory encrypt_CBC_PKCS7Padding(const Memory& mem) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt((CLASS*)this, mem.getData(), mem.getSize());
		}
		
		Memory decrypt_CBC_PKCS7Padding(const Memory& mem) const
		{
			return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt((CLASS*)this, mem.getData(), mem.getSize());
		}
		
		sl_size encrypt_CTR(const void* input, sl_size size, void* output, void* counter, sl_uint32 offset) const
		{
			return BlockCipher_CTR<CLASS>::encrypt((CLASS*)this, input, size, output, counter, offset);
		}
		
		sl_size encrypt_CTR(const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output) const
		{
			return BlockCipher_CTR<CLASS>::encrypt((CLASS*)this, iv, counter, offset, input, size, output);
		}
		
		sl_size encrypt_CTR(const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output) const
		{
			return BlockCipher_CTR<CLASS>::encrypt((CLASS*)this, iv, pos, input, size, output);
		}
		
	};
	
}

#include "detail/block_cipher.inc"

#endif
