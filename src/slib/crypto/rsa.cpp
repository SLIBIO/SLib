#include "../../../inc/slib/crypto/rsa.h"
#include "../../../inc/slib/crypto/sha2.h"

#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_CRYPTO_NAMESPACE_BEGIN
sl_bool RSA::executePublic(const RSAPublicKey& key, const void* src, void* dst)
{
	sl_uint32 n = key.N.getSizeInBytes();
	BigInt T = BigInt::fromBytesBE(src, n);
	if (T >= key.N) {
		return sl_false;
	}
	T = T.pow_montgomery(key.E, key.N);
	if (T.isNotNull()) {
		if (T.getBytesBE(dst, n)) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool RSA::executePrivate(const RSAPrivateKey& key, const void* src, void* dst)
{
	sl_uint32 n = key.N.getSizeInBytes();
	BigInt T = BigInt::fromBytesBE(src, n);
	if (T >= key.N) {
		return sl_false;
	}
	if (key.flagUseOnlyD) {
		T = T.pow_montgomery(key.D, key.N);
	} else {
		BigInt TP = T.pow_montgomery(key.DP, key.P);
		BigInt TQ = T.pow_montgomery(key.DQ, key.Q);
		T = ((TP - TQ) * key.IQ) % key.P;
		T = TQ + T * key.Q;
	}
	if (T.isNotNull()) {
		if (T.getBytesBE(dst, n)) {
			return sl_true;
		}
	}
	return sl_false;
}

static sl_bool _rsa_execute(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate
	, const void* src, void* dst)
{
	if (keyPublic) {
		return RSA::executePublic(*keyPublic, src, dst);
	} else {
		return RSA::executePrivate(*keyPrivate, src, dst);
	}
}

#define RSA_PKCS1_SIGN		1
#define RSA_PKCS1_CRYPT		2

static sl_bool _rsa_encrypt_pkcs1_v15(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate
	, const void* src, sl_uint32 n, void* dst)
{
	sl_uint32 len;
	if (keyPublic) {
		len = keyPublic->getLength();
	} else {
		len = keyPrivate->getLength();
	}
	// check (len - 8 < n + 3), 8 bytes is for enough random region
	if (n == 0 || len < n + 11) {
		return sl_false;
	}
	char* p = (char*)dst;
	
	// avoid error when input=output
	for (sl_uint32 i = n; i > 0; i--) {
		p[len - n + i - 1] = ((char*)src)[i - 1];
	}
	
	*(p++) = 0;
	sl_uint32 lenPadding = len - 3 - n;
	if (keyPublic) {
		// encrypt mode
		*(p++) = RSA_PKCS1_CRYPT;
		Math::randomMemory(p, lenPadding);
		for (sl_uint32 i = 0; i < lenPadding; i++) {
			if (*p == 0) {
				*p = (char)((Math::randomInt() % 255) + 1);
			}
			p++;
		}
	} else {
		// sign mode
		*(p++) = RSA_PKCS1_SIGN;
		for (sl_uint32 i = 0; i < lenPadding; i++) {
			*(p++) = (char)0xFF;
		}
	}
	*(p++) = 0;
	return _rsa_execute(keyPublic, keyPrivate, dst, dst);
}

static sl_uint32 _rsa_decrypt_pkcs1_v15(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate
	, const void* src, void* dst, sl_uint32 n, sl_bool* pFlagSign)
{
	sl_uint32 len;
	if (keyPublic) {
		len = keyPublic->getLength();
	} else {
		len = keyPrivate->getLength();
	}
	if (len < 32) {
		return sl_false;
	}
	SLIB_SCOPED_BUFFER(char, 4096, buf, len);
	if (!buf) {
		return 0;
	}
	if (!_rsa_execute(keyPublic, keyPrivate, src, buf)) {
		return 0;
	}
	if (buf[0] != 0) {
		return sl_false;
	}
	sl_uint32 type = buf[1];
	if (type == RSA_PKCS1_SIGN) {
		if (pFlagSign) {
			*pFlagSign = sl_true;
		}
	} else if (type == RSA_PKCS1_CRYPT) {
		if (pFlagSign) {
			*pFlagSign = sl_false;
		}
	} else {
		return sl_false;
	}
	sl_uint32 pos;
	for (pos = 2; pos < len - 1; pos++) {
		if (buf[pos] == 0) {
			break;
		}
	}
	pos++;
	Base::copyMemory(dst, buf + pos, Math::min(len - pos, n));
	return len - pos;
}

sl_bool RSA::encryptPublic_pkcs1_v15(const RSAPublicKey& key, const void* src, sl_uint32 n, void* dst)
{
	return _rsa_encrypt_pkcs1_v15(&key, sl_null, src, n, dst);
}

sl_bool RSA::encryptPrivate_pkcs1_v15(const RSAPrivateKey& key, const void* src, sl_uint32 n, void* dst)
{
	return _rsa_encrypt_pkcs1_v15(sl_null, &key, src, n, dst);
}

sl_uint32 RSA::decryptPublic_pkcs1_v15(const RSAPublicKey& key, const void* src, void* dst, sl_uint32 n, sl_bool* pFlagSign)
{
	return _rsa_decrypt_pkcs1_v15(&key, sl_null, src, dst, n, pFlagSign);
}

sl_uint32 RSA::decryptPrivate_pkcs1_v15(const RSAPrivateKey& key, const void* src, void* dst, sl_uint32 n, sl_bool* pFlagSign)
{
	return _rsa_decrypt_pkcs1_v15(sl_null, &key, src, dst, n, pFlagSign);
}

/*
	Public-Key Cryptography Standards (PKCS) #1: RSA Cryptography Specifications Version 2.1

	https://tools.ietf.org/html/rfc3447#section-7.1

	Section 7.1 RSAES-OAEP
*/

static sl_bool _rsa_encrypt_oaep_v21(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, const Ref<CryptoHash>& hash
	, const void* _input, sl_uint32 sizeInput, void* _output, const void* label, sl_uint32 sizeLabel)
{
	if (hash.isNull()) {
		return 0;
	}
	sl_uint32 sizeRSA;
	if (keyPublic) {
		sizeRSA = keyPublic->getLength();
	} else {
		sizeRSA = keyPrivate->getLength();
	}
	sl_uint32 sizeHash = hash->getSize();
	if (sizeInput == 0 || sizeHash == 0 || sizeRSA < sizeInput + 2 * sizeHash + 2) {
		return sl_false;
	}

	const sl_uint8* input = (const sl_uint8*)_input;
	sl_uint8* output = (sl_uint8*)_output;
	sl_uint8* seed = output + 1;
	sl_uint8* DB = seed + sizeHash;
	sl_uint32 sizeDB = sizeRSA - sizeHash - 1;
	sl_uint8* lHash = DB;
	sl_uint8* PS = lHash + sizeHash; // Zero Area
	sl_uint8* M = output + (sizeRSA - sizeInput);

	// avoid error when input=output
	for (sl_uint32 i = sizeInput; i > 0; i--) {
		M[i - 1] = input[i - 1];
	}
	*output = 0;
	Math::randomMemory(seed, sizeHash);
	hash->execute(label, sizeLabel, lHash);
	Base::zeroMemory(PS, M - PS - 1);
	*(M - 1) = 1;
	hash->applyMask_MGF1(seed, sizeHash, DB, sizeDB);
	hash->applyMask_MGF1(DB, sizeDB, seed, sizeHash);

	return _rsa_execute(keyPublic, keyPrivate, output, output);
}

static sl_uint32 _rsa_decrypt_oaep_v21(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, const Ref<CryptoHash>& hash
	, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label, sl_uint32 sizeLabel)
{
	if (hash.isNull()) {
		return 0;
	}
	sl_uint32 sizeRSA;
	if (keyPublic) {
		sizeRSA = keyPublic->getLength();
	} else {
		sizeRSA = keyPrivate->getLength();
	}
	sl_uint32 sizeHash = hash->getSize();
	if (sizeHash == 0 || sizeRSA < 2 * sizeHash + 2) {
		return 0;
	}
	SLIB_SCOPED_BUFFER(sl_uint8, 4096, buf, sizeRSA);
	if (!buf) {
		return 0;
	}
	if (!_rsa_execute(keyPublic, keyPrivate, input, buf)) {
		return 0;
	}

	sl_uint8* seed = buf + 1;
	sl_uint8* DB = seed + sizeHash;
	sl_uint32 sizeDB = sizeRSA - sizeHash - 1;
	sl_uint8* lHash = DB;

	hash->applyMask_MGF1(DB, sizeDB, seed, sizeHash);
	hash->applyMask_MGF1(seed, sizeHash, DB, sizeDB);
	
	hash->execute(label, sizeLabel, seed);
	sl_uint8 _check = buf[0];
	sl_uint32 i;
	for (i = 0; i < sizeHash; i++) {
		_check |= (seed[i] - lHash[i]);
	}
	for (i = sizeHash; i < sizeDB; i++) {
		if (_check == 0 && DB[i] == 1) {
			sl_uint32 size = sizeDB - i - 1;
			if (size > sizeOutputBuffer) {
				size = sizeOutputBuffer;
			}
			Base::copyMemory(output, DB + i + 1, size);
			return size;
		}
		if (DB[i] != 0) {
			_check = 1;
		}
	}
	return 0;
}

sl_bool RSA::encryptPublic_oaep_v21(const RSAPublicKey& key, const Ref<CryptoHash>& hash, const void* input, sl_uint32 sizeInput, void* output, const void* label, sl_uint32 sizeLabel)
{
	return _rsa_encrypt_oaep_v21(&key, sl_null, hash, input, sizeInput, output, label, sizeLabel);
}

sl_bool RSA::encryptPrivate_oaep_v21(const RSAPrivateKey& key, const Ref<CryptoHash>& hash, const void* input, sl_uint32 sizeInput, void* output, const void* label, sl_uint32 sizeLabel)
{
	return _rsa_encrypt_oaep_v21(sl_null, &key, hash, input, sizeInput, output, label, sizeLabel);
}

sl_uint32 RSA::decryptPublic_oaep_v21(const RSAPublicKey& key, const Ref<CryptoHash>& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label, sl_uint32 sizeLabel)
{
	return _rsa_decrypt_oaep_v21(&key, sl_null, hash, input, output, sizeOutputBuffer, label, sizeLabel);
}

sl_uint32 RSA::decryptPrivate_oaep_v21(const RSAPrivateKey& key, const Ref<CryptoHash>& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label, sl_uint32 sizeLabel)
{
	return _rsa_decrypt_oaep_v21(sl_null, &key, hash, input, output, sizeOutputBuffer, label, sizeLabel);
}
SLIB_CRYPTO_NAMESPACE_END
