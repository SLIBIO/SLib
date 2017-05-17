/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/crypto/hash.h"

#include "slib/crypto/md5.h"
#include "slib/crypto/sha1.h"
#include "slib/crypto/sha2.h"

#include "slib/core/scoped.h"
#include "slib/core/mio.h"

namespace slib
{

	CryptoHash::CryptoHash()
	{
	}

	CryptoHash::~CryptoHash()
	{
	}

	Ref<CryptoHash> CryptoHash::create(CryptoHashType type)
	{
		switch (type) {
		case CryptoHashType::MD5:
			return new MD5();
		case CryptoHashType::SHA1:
			return new SHA1();
		case CryptoHashType::SHA224:
			return new SHA224();
		case CryptoHashType::SHA256:
			return new SHA256();
		case CryptoHashType::SHA384:
			return new SHA384();
		case CryptoHashType::SHA512:
			return new SHA512();
		}
		return sl_null;
	}

	Ref<CryptoHash> CryptoHash::md5()
	{
		return new MD5();
	}

	Ref<CryptoHash> CryptoHash::sha1()
	{
		return new SHA1();
	}

	Ref<CryptoHash> CryptoHash::sha224()
	{
		return new SHA224();
	}

	Ref<CryptoHash> CryptoHash::sha256()
	{
		return new SHA256();
	}

	Ref<CryptoHash> CryptoHash::sha384()
	{
		return new SHA384();
	}

	Ref<CryptoHash> CryptoHash::sha512()
	{
		return new SHA512();
	}

	void CryptoHash::execute(const void* input, sl_size n, void* output)
	{
		start();
		update(input, n);
		finish(output);
	}


/*
	Public-Key Cryptography Standards (PKCS) #1: RSA Cryptography Specifications Version 2.1
	Appendix B.2 Masking Generation Functions

	https://tools.ietf.org/html/rfc3447#appendix-B.2
*/
	void CryptoHash::applyMask_MGF1(const void* seed, sl_uint32 sizeSeed, void* _target, sl_uint32 sizeTarget)
	{
		sl_uint32 n = getSize();
		if (n == 0) {
			return;
		}
		SLIB_SCOPED_BUFFER(sl_uint8, 128, h, n);
		sl_uint8* target = (sl_uint8*)(_target);
		sl_uint32 i = 0;
		sl_uint8 C[4];
		while (sizeTarget >= n) {
			start();
			update(seed, sizeSeed);
			MIO::writeUint32BE(C, i);
			update(C, 4);
			finish(h);
			for (sl_uint32 k = 0; k < n; k++) {
				target[k] ^= h[k];
			}
			i++;
			target += n;
			sizeTarget -= n;
		}
		start();
		update(seed, sizeSeed);
		MIO::writeUint32BE(C, i);
		update(C, 4);
		finish(h);
		for (sl_uint32 k = 0; k < sizeTarget; k++) {
			target[k] ^= h[k];
		}
	}


#define DEFINE_CRYPTO_HASH(CLASS, HASH_SIZE) \
	void CLASS::hash(const void* input, sl_size n, void* output) \
	{ \
		CLASS h; \
		h.CLASS::start(); \
		h.CLASS::update(input, n); \
		h.CLASS::finish(output); \
	} \
	sl_uint32 CLASS::getHashSize() \
	{ \
		return HASH_SIZE; \
	} \
	void CLASS::hash(const String& s, void* output) \
	{ \
		CLASS::hash(s.getData(), s.getLength(), output); \
	} \
	void CLASS::hash(const Memory& data, void* output) \
	{ \
		CLASS::hash(data.getData(), data.getSize(), output); \
	} \
	Memory CLASS::hash(const void* input, sl_size n) \
	{ \
		char v[HASH_SIZE]; \
		CLASS::hash(input, n, v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	Memory CLASS::hash(const String& s) \
	{ \
		char v[HASH_SIZE]; \
		CLASS::hash(s.getData(), s.getLength(), v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	Memory CLASS::hash(const Memory& data) \
	{ \
		char v[HASH_SIZE]; \
		CLASS::hash(data.getData(), data.getSize(), v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	sl_uint32 CLASS::getSize() const \
	{ \
		return getHashSize(); \
	}

	DEFINE_CRYPTO_HASH(MD5, 16)
	DEFINE_CRYPTO_HASH(SHA1, 20)
	DEFINE_CRYPTO_HASH(SHA224, 28)
	DEFINE_CRYPTO_HASH(SHA256, 32)
	DEFINE_CRYPTO_HASH(SHA384, 48)
	DEFINE_CRYPTO_HASH(SHA512, 64)

}
