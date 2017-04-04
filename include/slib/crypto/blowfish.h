/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CRYPTO_BLOWFISH
#define CHECKHEADER_SLIB_CRYPTO_BLOWFISH

#include "definition.h"

#include "../core/string.h"

#include "block_cipher.h"

/*
	BlowFish

	User Key Size - 32 bits to 448 bits
	Block Size - 64 bits (8 bytes)
*/

namespace slib
{

	class SLIB_EXPORT Blowfish : public Object
	{
	public:
		Blowfish();

		~Blowfish();

	public:
		static sl_uint32 getBlockSize();

		sl_bool setKey(const void* key, sl_uint32 lenKey /* 4 to 56 bytes */);

		void setKey_SHA256(const String& key);
		
		void encrypt(sl_uint32& d0, sl_uint32& d1) const;
		
		void decrypt(sl_uint32& d0, sl_uint32& d1) const;

		// 64 bit (8 byte) block
		void encryptBlock(const void* src, void* dst) const;

		// 64 bit (8 byte) block
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
		/* The subkeys used by the blowfish cipher */
		sl_uint32 m_P[18];
		sl_uint32 m_S[4][256];

	};
	
}

#endif
