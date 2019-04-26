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

	class SLIB_EXPORT AES : public BlockCipher<AES>
	{
	public:
		enum {
			BlockSize = 16
		};
	public:
		AES();

		~AES();

	public:
		sl_bool setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);

		void setKey_SHA256(const String& key);
		
		void encrypt(sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3) const;
	
		void decrypt(sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3) const;
		
		// 128 bits (16 bytes) block
		void encryptBlock(const void* src, void* dst) const;

		// 128 bits (16 bytes) block
		void decryptBlock(const void* src, void* dst) const;

	private:
		sl_uint32 m_roundKeyEnc[64];
		sl_uint32 m_roundKeyDec[64];
		sl_uint32 m_nCountRounds;

	};
	
	class SLIB_EXPORT AES_GCM : public GCM<AES>
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
