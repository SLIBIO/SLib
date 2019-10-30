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

#include "slib/crypto/openssl.h"

#include "slib/crypto/sha2.h"
#include "slib/core/scoped.h"

#include "openssl/aes.h"
#include "openssl/rand.h"
#include "openssl/ec.h"
#include "openssl/evp.h"
#include "openssl/pem.h"

namespace slib
{
	
	namespace priv
	{
		namespace openssl
		{

#if defined(SLIB_PLATFORM_IS_WIN32)
			void initThread();
#else
			static void initThread()
			{
			}
#endif
		
			static Memory Generate_RSA_Signature(EVP_PKEY* key, const EVP_MD* md, const void* data, sl_size size)
			{
				unsigned int len = EVP_PKEY_size(key);
				if (len > 0) {
					EVP_MD_CTX* ctx = EVP_MD_CTX_new();
					if (ctx) {
						if (EVP_DigestInit(ctx, md)) {
							if (EVP_DigestUpdate(ctx, data, (size_t)size)) {
								Memory mem = Memory::create(len);
								if (mem.isNotNull()) {
									if (EVP_SignFinal(ctx, (unsigned char*)(mem.getData()), &len, key)) {
										EVP_MD_CTX_free(ctx);
										return mem;
									}
								}
							}
						}
						EVP_MD_CTX_free(ctx);
					}
				}
				return sl_null;
			}
		
			static sl_bool Verify_RSA_Signature(EVP_PKEY* key, const EVP_MD* md, const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
			{
				EVP_MD_CTX* ctx = EVP_MD_CTX_new();
				if (ctx) {
					if (EVP_DigestInit(ctx, md)) {
						if (EVP_DigestUpdate(ctx, data, (size_t)sizeData)) {
							if (1 == EVP_VerifyFinal(ctx, (unsigned char*)(signature), (unsigned int)(sizeSignature), key)) {
								EVP_MD_CTX_free(ctx);
								return sl_true;
							}
						}
					}
					EVP_MD_CTX_free(ctx);
				}
				return sl_false;
			}
		
			static Memory Generate_ECDSA_Signature(EVP_PKEY* key, const void* hash, sl_uint32 sizeHash)
			{
				EC_KEY* ekey = EVP_PKEY_get0_EC_KEY(key);
				if (!ekey) {
					return sl_null;
				}
				Memory ret;
				ECDSA_SIG* sig = ECDSA_do_sign((unsigned char*)hash, (int)sizeHash, ekey);
				if (sig) {
					const BIGNUM* r;
					const BIGNUM* s;
					ECDSA_SIG_get0(sig, &r, &s);
					sl_uint32 nr = (sl_uint32)(BN_num_bytes(r));
					sl_uint32 ns = (sl_uint32)(BN_num_bytes(s));
					sl_uint32 m = SLIB_MAX(nr, ns);
					sl_uint32 n = m << 1;
					ret = Memory::create(n);
					if (ret.isNotNull()) {
						unsigned char* t = (unsigned char*)(ret.getData());
						Base::zeroMemory(t, n);
						BN_bn2bin(r, t + (m - nr));
						BN_bn2bin(s, t + (n - ns));
					}
					ECDSA_SIG_free(sig);
				}
				return ret;
			}
		
			static sl_bool Verify_ECDSA_Signature(EVP_PKEY* key, const void* hash, sl_uint32 sizeHash, const void* signature, sl_uint32 sizeSignature)
			{
				if (sizeSignature & 1) {
					return sl_false;
				}
				EC_KEY* ekey = EVP_PKEY_get0_EC_KEY(key);
				if (!ekey) {
					return sl_false;
				}
				sl_bool bRet = sl_false;
				ECDSA_SIG* sig = ECDSA_SIG_new();
				if (sig) {
					unsigned char* t = (unsigned char*)signature;
					sl_uint32 m = (sl_uint32)(sizeSignature >> 1);
					BIGNUM* r = BN_bin2bn(t, (int)m, sl_null);
					BIGNUM* s = BN_bin2bn(t + m, (int)m, sl_null);
					ECDSA_SIG_set0(sig, r, s);
					bRet = ECDSA_do_verify((unsigned char*)hash, (int)sizeHash, sig, ekey) == 1;
					ECDSA_SIG_free(sig);
				}
				return bRet;
			}

			static Memory Generate_RSA_PSS_Signature(EVP_PKEY* key, const EVP_MD* md, const void* hash, sl_size sizeHash)
			{
				::RSA* rsa = EVP_PKEY_get0_RSA(key);
				if (!rsa) {
					return sl_null;
				}
				sl_uint32 sizeRsa = (sl_uint32)(RSA_size(rsa));
				SLIB_SCOPED_BUFFER(unsigned char, 1024, padded, sizeRsa)
				if (padded) {
					Base::zeroMemory(padded, sizeRsa);
					if (RSA_padding_add_PKCS1_PSS_mgf1(rsa, padded, (unsigned char*)hash, md, md, -1)) {
						Memory ret = Memory::create(sizeRsa);
						if (ret.isNotNull()) {
							if (RSA_private_encrypt((int)sizeRsa, padded, (unsigned char*)(ret.getData()), rsa, RSA_NO_PADDING)) {
								return ret;
							}
						}
					}
				}
				return sl_null;
			}
		
			static sl_bool Verify_RSA_PSS_Signature(EVP_PKEY* key, const EVP_MD* md, const void* hash, sl_size sizeHash, const void* signature, sl_uint32 sizeSignature)
			{
				::RSA* rsa = EVP_PKEY_get0_RSA(key);
				if (!rsa) {
					return sl_false;
				}
				sl_uint32 sizeRsa = (sl_uint32)(RSA_size(rsa));
				SLIB_SCOPED_BUFFER(unsigned char, 1024, sig, sizeRsa)
				if (sig) {
					Base::zeroMemory(sig, sizeRsa);
					if (RSA_public_decrypt((int)sizeRsa, (unsigned char*)signature, sig, rsa, RSA_NO_PADDING)) {
						if (RSA_verify_PKCS1_PSS_mgf1(rsa, (unsigned char*)hash, md, md, sig, -1)) {
							return sl_true;
						}
					}
				}
				return sl_false;
			}
		
			static BigInt Get_BigInt_from_BIGNUM(const BIGNUM* bn)
			{
				if (bn) {
					sl_size size = (sl_size)(BN_num_bytes(bn));
					if (size) {
						SLIB_SCOPED_BUFFER(unsigned char, 1024, t, size)
						if (t) {
							BN_bn2bin(bn, t);
							return BigInt::fromBytesBE(t, size);
						}
					}
				}
				return sl_null;
			}
			
			static BIGNUM* Get_BIGNUM_from_BigInt(const BigInt& n)
			{
				if (n.isNotNull()) {
					sl_size size = n.getMostSignificantBytes();
					if (size) {
						SLIB_SCOPED_BUFFER(unsigned char, 1024, t, size)
						if (t) {
							if (n.getBytesBE(t, size)) {
								return BN_bin2bn(t, (int)(size), sl_null);
							}
						}
					}
				}
				return sl_null;
			}
			
			static ECPoint Get_ECPoint_from_EC_POINT(EC_GROUP* group, const EC_POINT* pt)
			{
				ECPoint ret;
				BIGNUM* x = BN_new();
				if (x) {
					BIGNUM* y = BN_new();
					if (y) {
						if (1 == EC_POINT_get_affine_coordinates(group, pt, x, y, sl_null)) {
							ret.x = Get_BigInt_from_BIGNUM(x);
							if (ret.x.isNotZero()) {
								ret.y = Get_BigInt_from_BIGNUM(y);
							}
						}
						BN_free(y);
					}
					BN_free(x);
				}
				return ret;
			}
			
			static EC_POINT* Get_EC_POINT_from_ECPoint(EC_GROUP* group, const ECPoint& pt)
			{
				EC_POINT* ret = EC_POINT_new(group);
				if (ret) {
					sl_bool flagSuccess = sl_false;
					BIGNUM* x = Get_BIGNUM_from_BigInt(pt.x);
					if (x) {
						BIGNUM* y = Get_BIGNUM_from_BigInt(pt.y);
						if (y) {
							if (1 == EC_POINT_set_affine_coordinates(group, ret, x, y, sl_null)) {
								flagSuccess = sl_true;
							}
							BN_free(y);
						}
						BN_free(x);
					}
					if (flagSuccess) {
						return ret;
					}
					EC_POINT_free(ret);
				}
				return sl_null;
			}
			
			static EC_KEY* Get_EC_KEY_from_ECPublicKey(EC_GROUP* group, const ECPublicKey& key)
			{
				if (key.Q.isO()) {
					return sl_null;
				}
				EC_KEY* ek = EC_KEY_new();
				if (ek) {
					sl_bool flagSuccess = sl_false;
					EC_KEY_set_group(ek, group);
					EC_POINT* pt = Get_EC_POINT_from_ECPoint(group, key.Q);
					if (pt) {
						EC_KEY_set_public_key(ek, pt);
						flagSuccess = sl_true;
						EC_POINT_free(pt);
					}
					if (flagSuccess) {
						return ek;
					}
					EC_KEY_free(ek);
				}
				return sl_null;
			}
			
			static EC_KEY* Get_EC_KEY_from_ECPrivateKey(EC_GROUP* group, const ECPrivateKey& key)
			{
				EC_KEY* ek = Get_EC_KEY_from_ECPublicKey(group, key);
				if (ek) {
					sl_bool flagSuccess = sl_false;
					BIGNUM* bn = Get_BIGNUM_from_BigInt(key.d);
					if (bn) {
						EC_KEY_set_private_key(ek, bn);
						flagSuccess = sl_true;
						BN_free(bn);
					}
					if (flagSuccess) {
						return ek;
					}
					EC_KEY_free(ek);
				}
				return sl_null;
			}
			
			static ECDSA_SIG* get_ECDSA_SIG_from_ECDSA_Signature(const ECDSA_Signature& _sig)
			{
				ECDSA_SIG* sig = ECDSA_SIG_new();
				if (sig) {
					BIGNUM* r = Get_BIGNUM_from_BigInt(_sig.r);
					if (r) {
						BIGNUM* s = Get_BIGNUM_from_BigInt(_sig.s);
						if (s) {
							ECDSA_SIG_set0(sig, r, s);
							return sig;
						}
						BN_free(r);
					}
					ECDSA_SIG_free(sig);
				}
				return sl_null;
			}
			
			static ECDSA_Signature Do_sign_ECDSA_secp256k1(const ECPrivateKey& key, const void* hash, sl_size size)
			{
				initThread();
				ECDSA_Signature ret;
				EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
				if (group) {
					EC_KEY* ek = Get_EC_KEY_from_ECPrivateKey(group, key);
					if (ek) {
						ECDSA_SIG* sig = ECDSA_do_sign((unsigned char*)hash, (int)size, ek);
						if (sig) {
							const BIGNUM* r = ECDSA_SIG_get0_r(sig);
							if (r) {
								ret.r = Get_BigInt_from_BIGNUM(r);
							}
							const BIGNUM* s = ECDSA_SIG_get0_s(sig);
							if (s) {
								ret.s = Get_BigInt_from_BIGNUM(s);
							}
							ECDSA_SIG_free(sig);
						}
						EC_KEY_free(ek);
					}
					EC_GROUP_free(group);
				}
				return ret;
			}
			
			static sl_bool Do_verify_ECDSA_secp256k1(const ECPublicKey& key, const void* hash, sl_size size, const ECDSA_Signature& _sig)
			{
				initThread();
				sl_bool flagVerified = sl_false;
				EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
				if (group) {
					EC_KEY* ek = Get_EC_KEY_from_ECPublicKey(group, key);
					if (ek) {
						ECDSA_SIG* sig = get_ECDSA_SIG_from_ECDSA_Signature(_sig);
						if (sig) {
							if (1 == ECDSA_do_verify((unsigned char*)hash, (int)size, sig, ek)) {
								flagVerified = sl_true;
							}
							ECDSA_SIG_free(sig);
						}
						EC_KEY_free(ek);
					}
					EC_GROUP_free(group);
				}
				return flagVerified;
			}

		}
	}
	
	using namespace priv::openssl;
	
	
	OpenSSL_AES::OpenSSL_AES()
	{
		m_keyEnc = sl_null;
		m_keyDec = sl_null;
	}
	
	OpenSSL_AES::~OpenSSL_AES()
	{
		if (m_keyEnc) {
			Base::freeMemory(m_keyEnc);
		}
		if (m_keyDec) {
			Base::freeMemory(m_keyDec);
		}
	}
	
	sl_bool OpenSSL_AES::setKey(const void* key, sl_uint32 lenKey)
	{
		if (!m_keyEnc) {
			m_keyEnc = Base::createMemory(sizeof(AES_KEY));
		}
		if (AES_set_encrypt_key((unsigned char*)key, lenKey << 3, (AES_KEY*)m_keyEnc) != 0) {
			return sl_false;
		}
		if (!m_keyDec) {
			m_keyDec = Base::createMemory(sizeof(AES_KEY));
		}
		if (AES_set_decrypt_key((unsigned char*)key, lenKey << 3, (AES_KEY*)m_keyDec) != 0) {
			return sl_false;
		}
		return sl_true;
	}
	
	void OpenSSL_AES::setKey_SHA256(const String& key)
	{
		char sig[32];
		SHA256::hash(key, sig);
		setKey(sig, 32);
	}
	
	void OpenSSL_AES::encryptBlock(const void* src, void* dst) const
	{
		AES_encrypt((unsigned char*)src, (unsigned char*)dst, (AES_KEY*)m_keyEnc);
	}
	
	void OpenSSL_AES::decryptBlock(const void* src, void* dst) const
	{
		AES_decrypt((unsigned char*)src, (unsigned char*)dst, (AES_KEY*)m_keyDec);
	}
	

	SLIB_DEFINE_OBJECT(OpenSSL_Key, Object)
	
	OpenSSL_Key::OpenSSL_Key()
	{
		m_key = sl_null;
	}

	OpenSSL_Key::~OpenSSL_Key()
	{
		if (m_key) {
			EVP_PKEY_free(m_key);
		}
	}

	Ref<OpenSSL_Key> OpenSSL_Key::createPublicKey(const String& pem)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (bio) {
			BIO_write(bio, pem.getData(), (int)(pem.getLength()));
			EVP_PKEY* key = PEM_read_bio_PUBKEY(bio, sl_null, sl_null, sl_null);
			BIO_free(bio);
			if (key) {
				Ref<OpenSSL_Key> ret = new OpenSSL_Key;
				if (ret.isNotNull()) {
					ret->m_key = key;
					return ret;
				}
				EVP_PKEY_free(key);
			}
		}
		return sl_null;
	}

	Ref<OpenSSL_Key> OpenSSL_Key::createPrivateKey(const String& pem)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (bio) {
			BIO_write(bio, pem.getData(), (int)(pem.getLength()));
			EVP_PKEY* key = PEM_read_bio_PrivateKey(bio, sl_null, sl_null, sl_null);
			BIO_free(bio);
			if (key) {
				Ref<OpenSSL_Key> ret = new OpenSSL_Key;
				if (ret.isNotNull()) {
					ret->m_key = key;
					return ret;
				}
				EVP_PKEY_free(key);
			}
		}
		return sl_null;
	}

	Memory OpenSSL_Key::sign_RSA_SHA256(const void* data, sl_size sizeData)
	{
		return Generate_RSA_Signature(m_key, EVP_sha256(), data, sizeData);
	}
	
	sl_bool OpenSSL_Key::verify_RSA_SHA256(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		return Verify_RSA_Signature(m_key, EVP_sha256(), data, sizeData, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_RSA_SHA384(const void* data, sl_size sizeData)
	{
		return Generate_RSA_Signature(m_key, EVP_sha384(), data, sizeData);
	}

	sl_bool OpenSSL_Key::verify_RSA_SHA384(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		return Verify_RSA_Signature(m_key, EVP_sha384(), data, sizeData, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_RSA_SHA512(const void* data, sl_size sizeData)
	{
		return Generate_RSA_Signature(m_key, EVP_sha512(), data, sizeData);
	}

	sl_bool OpenSSL_Key::verify_RSA_SHA512(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		return Verify_RSA_Signature(m_key, EVP_sha512(), data, sizeData, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_RSA_PSS_SHA256(const void* data, sl_size sizeData)
	{
		unsigned char h[32];
		SHA256::hash(data, sizeData, h);
		return Generate_RSA_PSS_Signature(m_key, EVP_sha256(), h, 32);
	}

	sl_bool OpenSSL_Key::verify_RSA_PSS_SHA256(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		unsigned char h[32];
		SHA256::hash(data, sizeData, h);
		return Verify_RSA_PSS_Signature(m_key, EVP_sha256(), h, 32, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_RSA_PSS_SHA384(const void* data, sl_size sizeData)
	{
		unsigned char h[48];
		SHA384::hash(data, sizeData, h);
		return Generate_RSA_PSS_Signature(m_key, EVP_sha384(), h, 48);
	}

	sl_bool OpenSSL_Key::verify_RSA_PSS_SHA384(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		unsigned char h[48];
		SHA384::hash(data, sizeData, h);
		return Verify_RSA_PSS_Signature(m_key, EVP_sha384(), h, 48, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_RSA_PSS_SHA512(const void* data, sl_size sizeData)
	{
		unsigned char h[64];
		SHA384::hash(data, sizeData, h);
		return Generate_RSA_PSS_Signature(m_key, EVP_sha512(), h, 64);
	}

	sl_bool OpenSSL_Key::verify_RSA_PSS_SHA512(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		unsigned char h[64];
		SHA384::hash(data, sizeData, h);
		return Verify_RSA_PSS_Signature(m_key, EVP_sha512(), h, 64, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_ECDSA_SHA256(const void* data, sl_size sizeData)
	{
		unsigned char h[32];
		SHA256::hash(data, sizeData, h);
		return Generate_ECDSA_Signature(m_key, h, 32);
	}

	sl_bool OpenSSL_Key::verify_ECDSA_SHA256(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		unsigned char h[32];
		SHA256::hash(data, sizeData, h);
		return Verify_ECDSA_Signature(m_key, h, 32, signature, sizeSignature);
	}
	
	Memory OpenSSL_Key::sign_ECDSA_SHA384(const void* data, sl_size sizeData)
	{
		unsigned char h[48];
		SHA384::hash(data, sizeData, h);
		return Generate_ECDSA_Signature(m_key, h, 48);
	}

	sl_bool OpenSSL_Key::verify_ECDSA_SHA384(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		unsigned char h[48];
		SHA384::hash(data, sizeData, h);
		return Verify_ECDSA_Signature(m_key, h, 48, signature, sizeSignature);
	}

	Memory OpenSSL_Key::sign_ECDSA_SHA512(const void* data, sl_size sizeData)
	{
		unsigned char h[64];
		SHA512::hash(data, sizeData, h);
		return Generate_ECDSA_Signature(m_key, h, 64);
	}

	sl_bool OpenSSL_Key::verify_ECDSA_SHA512(const void* data, sl_size sizeData, const void* signature, sl_uint32 sizeSignature)
	{
		unsigned char h[64];
		SHA512::hash(data, sizeData, h);
		return Verify_ECDSA_Signature(m_key, h, 64, signature, sizeSignature);
	}

	sl_bool OpenSSL::isProbablePrime(const void* num_BigEndian, sl_uint32 nBytes, sl_bool* pFlagError)
	{
		initThread();
		if (!nBytes) {
			if (pFlagError) {
				*pFlagError = sl_false;
			}
			return sl_false;
		}
		BIGNUM* num = BN_bin2bn((unsigned char*)num_BigEndian, nBytes, sl_null);
		if (num) {
			int ret = BN_is_prime_fasttest_ex(num, 0, sl_null, sl_false, sl_null);
			BN_free(num);
			if (pFlagError) {
				if (ret < 0) {
					*pFlagError = sl_true;
				} else {
					*pFlagError = sl_false;
				}
			}
			return ret == 1;
		} else {
			if (pFlagError) {
				*pFlagError = sl_true;
			}
		}
		return sl_false;
	}
	
	Memory OpenSSL::generatePrime(sl_uint32 nBits)
	{
		initThread();
		BIGNUM* prime = BN_new();
		if (BN_generate_prime_ex(prime, (int)nBits, sl_false, sl_null, sl_null, sl_null)) {
			sl_size n = (sl_size)(BN_num_bytes(prime));
			Memory ret = Memory::create(n);
			if (ret.isNotNull()) {
				BN_bn2bin(prime, (unsigned char*)(ret.getData()));
			}
			BN_free(prime);
			return ret;
		}
		return sl_null;
	}
	
	sl_bool OpenSSL::randomBytes(void* bytes, sl_uint32 nBytes, sl_bool flagPrivate)
	{
		initThread();
		if (flagPrivate) {
			if (RAND_priv_bytes((unsigned char*)bytes, (int)nBytes) == 1) {
				return sl_true;
			}
		} else {
			if (RAND_bytes((unsigned char*)bytes, (int)nBytes) == 1) {
				return sl_true;
			}
		}
		return sl_false;
	}

	void OpenSSL::generateRSA(RSAPrivateKey& key, sl_uint32 nBits)
	{
		initThread();
		sl_uint32 h = nBits >> 1;
		nBits = h << 1;
		for (;;) {
			key.P = BigInt::fromBytesBE(generatePrime(h));
			key.Q = BigInt::fromBytesBE(generatePrime(h));
			if (key.generateFromPrimes(nBits)) {
				return;
			}
		}
	}

	sl_bool OpenSSL::generate_ECKey_secp256k1(ECPrivateKey& _output)
	{
		initThread();
		sl_bool flagSuccess = sl_false;
		EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		if (group) {
			EC_KEY* key = EC_KEY_new();
			if (key) {
				flagSuccess = sl_true;
				EC_KEY_set_group(key, group);
				for (;;) {
					if (1 == EC_KEY_generate_key(key)) {
						const BIGNUM* p = EC_KEY_get0_private_key(key);
						_output.d = Get_BigInt_from_BIGNUM(p);
						if (_output.d.isNull()) {
							flagSuccess = sl_false;
							break;
						}
						const EC_POINT* Q = EC_KEY_get0_public_key(key);
						_output.Q = Get_ECPoint_from_EC_POINT(group, Q);
						if (_output.Q.isO()) {
							flagSuccess = sl_false;
							break;
						}
						break;
					}
				}
				EC_KEY_free(key);
			}
			EC_GROUP_free(group);
		}
		return flagSuccess;
	}
	
	sl_bool OpenSSL::check_ECKey_secp256k1(const ECPublicKey& key)
	{
		initThread();
		sl_bool flagSuccess = sl_false;
		EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		if (group) {
			EC_KEY* ek = Get_EC_KEY_from_ECPublicKey(group, key);
			if (ek) {
				flagSuccess = EC_KEY_check_key(ek) == 1;
				EC_KEY_free(ek);
			}
			EC_GROUP_free(group);
		}
		return flagSuccess;
	}
	
	ECDSA_Signature OpenSSL::sign_ECDSA_secp256k1(const ECPrivateKey& key, const BigInt& z)
	{
		Memory mem = z.getBytesBE();
		if (mem.isNull()) {
			return ECDSA_Signature();
		}
		return Do_sign_ECDSA_secp256k1(key, mem.getData(), mem.getSize());
	}
	
	ECDSA_Signature OpenSSL::sign_ECDSA_secp256k1(const ECPrivateKey& key, const void* hash, sl_size size)
	{
		return Do_sign_ECDSA_secp256k1(key, hash, size);
	}
	
	ECDSA_Signature OpenSSL::sign_ECDSA_SHA256_secp256k1(const ECPrivateKey& key, const void* data, sl_size size)
	{
		sl_uint8 hash[SHA256::HashSize];
		SHA256::hash(data, size, hash);
		return Do_sign_ECDSA_secp256k1(key, hash, SHA256::HashSize);
	}
	
	sl_bool OpenSSL::verify_ECDSA_secp256k1(const ECPublicKey& key, const BigInt& z, const ECDSA_Signature& signature)
	{
		Memory mem = z.getBytesBE();
		if (mem.isNull()) {
			return sl_false;
		}
		return Do_verify_ECDSA_secp256k1(key, mem.getData(), mem.getSize(), signature);
	}
	
	sl_bool OpenSSL::verify_ECDSA_secp256k1(const ECPublicKey& key, const void* hash, sl_size size, const ECDSA_Signature& signature)
	{
		return Do_verify_ECDSA_secp256k1(key, hash, size, signature);
	}
	
	sl_bool OpenSSL::verify_ECDSA_SHA256_secp256k1(const ECPublicKey& key, const void* data, sl_size size, const ECDSA_Signature& signature)
	{
		sl_uint8 hash[SHA256::HashSize];
		SHA256::hash(data, size, hash);
		return Do_verify_ECDSA_secp256k1(key, hash, SHA256::HashSize, signature);
	}
	
	BigInt OpenSSL::getSharedKey_ECDH_secp256k1(const ECPrivateKey& keyLocal, const ECPublicKey& keyRemote)
	{
		initThread();
		BigInt ret;
		EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		if (group) {
			BIGNUM* priv = Get_BIGNUM_from_BigInt(keyLocal.d);
			if (priv) {
				EC_POINT* pub = Get_EC_POINT_from_ECPoint(group, keyRemote.Q);
				if (pub) {
					EC_POINT* pt = EC_POINT_new(group);
					if (pt) {
						if (EC_POINT_mul(group, pt, sl_null, pub, priv, sl_null)) {
							BIGNUM* x = BN_new();
							if (x) {
								if (EC_POINT_get_affine_coordinates(group, pt, x, sl_null, sl_null)) {
									ret = Get_BigInt_from_BIGNUM(x);
								}
								BN_free(x);
							}
						}
						EC_POINT_free(pt);
					}
					EC_POINT_free(pub);
				}
				BN_free(priv);
			}
			EC_GROUP_free(group);
		}
		return ret;
	}
	
}
