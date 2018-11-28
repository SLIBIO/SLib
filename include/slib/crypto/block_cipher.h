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

#define SLIB_CRYPTO_BLOCK_CIPHER_BLOCK_MAX_LEN 128


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
	
	
	template <class BlockCipher>
	class SLIB_EXPORT BlockCipher_Blocks
	{
	public:
		// Output Size = (size / block) * block
		static sl_size encryptBlocks(const BlockCipher* crypto, const void* _src, void* _dst, sl_size size);

		// Output Size = (size / block) * block
		static sl_size decryptBlocks(const BlockCipher* crypto, const void* _src, void* _dst, sl_size size);
	
	};

/*
	Electronic codebook (ECB)
*/
	template <class BlockCipher, class Padding>
	class SLIB_EXPORT BlockCipher_ECB
	{
	public:
		// Output Size = (size / block + 1) * block (< size + block)
		static sl_size encrypt(const BlockCipher* crypto, const void* _src, sl_size size, void* _dst);

		// destination buffer size must equals to or greater than size
		static sl_size decrypt(const BlockCipher* crypto, const void* _src, sl_size size, void* _dst);
	
	};

/*
	Cipher-block chaining (CBC)
*/
	template <class BlockCipher, class Padding>
	class SLIB_EXPORT BlockCipher_CBC
	{
	public:
		// Output Size = (size / block + 1) * block (< size + block)
		static sl_size encrypt(const BlockCipher* crypto, const void* iv, const void* src, sl_size size, void* dst);

		// Output Size = (size / block + 2) * block (< size + block*2)
		static sl_size encrypt(const BlockCipher* crypto, const void* src, sl_size size, void* dst);

		static Memory encrypt(const BlockCipher* crypto, const void* src, sl_size size);

		// destination buffer size must equals to or greater than size
		static sl_size decrypt(const BlockCipher* crypto, const void* iv, const void* src, sl_size size, void* dst);

		// destination buffer size must equals to or greater than size
		static sl_size decrypt(const BlockCipher* crypto, const void* src, sl_size size, void* dst);

		static Memory decrypt(const BlockCipher* crypto, const void* src, sl_size size);

	};

/*
	Counter Mode (CTR)
*/
	template <class BlockCipher>
	class SLIB_EXPORT BlockCipher_CTR
	{
	public:
		static sl_size encrypt(const BlockCipher* crypto, const void* input, sl_size size, void* output, void* counter, sl_uint32 offset);

		static sl_size encrypt(const BlockCipher* crypto, const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output);

		static sl_size encrypt(const BlockCipher* crypto, const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output);

	};

}

#endif
