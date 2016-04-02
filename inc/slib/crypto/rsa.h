#ifndef CHECKHEADER_SLIB_CRYPTO_RSA
#define CHECKHEADER_SLIB_CRYPTO_RSA

#include "definition.h"

#include "hash.h"

#include "../math/bigint.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

class SLIB_EXPORT RSAPublicKey
{
public:
	BigInt N; // modulus
	BigInt E; // public exponent

public:
	sl_uint32 getLength() const;
	
};

class SLIB_EXPORT RSAPrivateKey
{
public:
	BigInt N; // modulus
	BigInt E; // public exponent
	BigInt D; // private exponent
	BigInt P; // prime 1
	BigInt Q; // prime 2
	BigInt DP; // exponent1, D mod (P-1)
	BigInt DQ; // exponent2, D mod (Q-1)
	BigInt IQ; // Q^-1 mod P
	
	// Use N and D only for decryption
	sl_bool flagUseOnlyD = sl_false;

public:
	sl_uint32 getLength() const;
	
};

class SLIB_EXPORT RSA
{
public:
	static sl_bool executePublic(const RSAPublicKey& key, const void* src, void* dst);
	
	static sl_bool executePrivate(const RSAPrivateKey& key, const void* src, void* dst);

	/*
		PKCS#1 v1.5 Random Padding
	*/
	static sl_bool encryptPublic_pkcs1_v15(const RSAPublicKey& key, const void* input, sl_uint32 sizeInput, void* output);
	
	static sl_bool encryptPrivate_pkcs1_v15(const RSAPrivateKey& key, const void* input, sl_uint32 sizeInput, void* output);
	
	static sl_uint32 decryptPublic_pkcs1_v15(const RSAPublicKey& key, const void* input, void* output, sl_uint32 sizeOutputBuffer, sl_bool* pFlagSign = sl_null);
	
	static sl_uint32 decryptPrivate_pkcs1_v15(const RSAPrivateKey& key, const void* input, void* output, sl_uint32 sizeOutputBuffer, sl_bool* pFlagSign = sl_null);

	/*
		PKCS#1 v2.1 OAEP - Optimal Asymmetric Encryption Padding
	*/
	static sl_bool encryptPublic_oaep_v21(const RSAPublicKey& key, const Ref<CryptoHash>& hash, const void* input, sl_uint32 sizeInput, void* output, const void* label = 0, sl_uint32 sizeLabel = 0);
	
	static sl_bool encryptPrivate_oaep_v21(const RSAPrivateKey& key, const Ref<CryptoHash>& hash, const void* input, sl_uint32 sizeInput, void* output, const void* label = 0, sl_uint32 sizeLabel = 0);
	
	static sl_uint32 decryptPublic_oaep_v21(const RSAPublicKey& key, const Ref<CryptoHash>& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label = 0, sl_uint32 sizeLabel = 0);
	
	static sl_uint32 decryptPrivate_oaep_v21(const RSAPrivateKey& key, const Ref<CryptoHash>& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label = 0, sl_uint32 sizeLabel = 0);

};

SLIB_CRYPTO_NAMESPACE_END


SLIB_CRYPTO_NAMESPACE_BEGIN

SLIB_INLINE sl_uint32 RSAPublicKey::getLength() const
{
	return (sl_uint32)(N.getMostSignificantBytes());
}

SLIB_INLINE sl_uint32 RSAPrivateKey::getLength() const
{
	return (sl_uint32)(N.getMostSignificantBytes());
}

SLIB_CRYPTO_NAMESPACE_END

#endif

