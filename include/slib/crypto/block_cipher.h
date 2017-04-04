/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
