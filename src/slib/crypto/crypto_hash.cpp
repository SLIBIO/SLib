#include "../../../inc/slib/crypto/hash.h"

#include "../../../inc/slib/crypto/md5.h"
#include "../../../inc/slib/crypto/sha1.h"
#include "../../../inc/slib/crypto/sha2.h"

#include "../../../inc/slib/core/scoped_pointer.h"
#include "../../../inc/slib/core/mio.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

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
	return Ref<CryptoHash>::null();
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

SLIB_CRYPTO_NAMESPACE_END

