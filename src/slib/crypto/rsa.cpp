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

#include "slib/crypto/rsa.h"


namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(RSAPublicKey)
	
	RSAPublicKey::RSAPublicKey()
	{
	}

	sl_uint32 RSAPublicKey::getLength() const
	{
		return (sl_uint32)(N.getMostSignificantBytes());
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(RSAPrivateKey)
	
	RSAPrivateKey::RSAPrivateKey(): flagUseOnlyD(sl_false)
	{
	}
	
	sl_uint32 RSAPrivateKey::getLength() const
	{
		return (sl_uint32)(N.getMostSignificantBytes());
	}
	
	void RSAPrivateKey::generate(sl_uint32 nBits)
	{
		sl_uint32 h = nBits >> 1;
		nBits = h << 1;
		for (;;) {
			P = BigInt::generatePrime(h);
			Q = BigInt::generatePrime(h);
			if (generateFromPrimes(nBits)) {
				return;
			}
		}
	}
	
	sl_bool RSAPrivateKey::generateFromPrimes(sl_uint32 nBits)
	{
		sl_uint32 h = nBits >> 1;
		if (h > 100) {
			if (BigInt::shiftRight((P-Q).abs(), h - 100).isZero()) {
				return sl_false;
			}
		} else {
			if (P == Q) {
				return sl_false;
			}
		}
		N = P * Q;
		BigInt P1 = P - 1;
		BigInt Q1 = Q - 1;
		BigInt L = BigInt::lcm(P1, Q1);
		if (E.isZero()) {
			if (nBits > 20) {
				E = 65537;
			} else if (nBits > 8) {
				E = 17;
			} else {
				E = 3;
			}
		}
		if (E < 3) {
			return sl_false;
		}
		if (E >= L) {
			return sl_false;
		}
		if (BigInt::gcd(E, L) != 1) {
			return sl_false;
		}
		D = BigInt::inverseMod(E, L);
		DP = BigInt::mod(D, P1);
		DQ = BigInt::mod(D, Q1);
		IQ = BigInt::inverseMod(Q, P);
		
		// Test
		BigInt a = 3;
		BigInt b = BigInt::pow_montgomery(a, E, N);
		BigInt TP = BigInt::pow_montgomery(b, DP, P);
		BigInt TQ = BigInt::pow_montgomery(b, DQ, Q);
		BigInt c = ((TP - TQ) * IQ) % P;
		c = TQ + c * Q;
		return c == 3;
	}


	sl_bool RSA::executePublic(const RSAPublicKey& key, const void* src, void* dst)
	{
		sl_size n = key.N.getMostSignificantBytes();
		BigInt T = BigInt::fromBytesBE(src, n);
		if (T >= key.N) {
			return sl_false;
		}
		T = BigInt::pow_montgomery(T, key.E, key.N);
		if (T.isNotNull()) {
			if (T.getBytesBE(dst, n)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool RSA::executePrivate(const RSAPrivateKey& key, const void* src, void* dst)
	{
		sl_size n = key.N.getMostSignificantBytes();
		BigInt T = BigInt::fromBytesBE(src, n);
		if (T >= key.N) {
			return sl_false;
		}
		if (key.flagUseOnlyD) {
			T = BigInt::pow_montgomery(T, key.D, key.N);
		} else {
			BigInt TP = BigInt::pow_montgomery(T, key.DP, key.P);
			BigInt TQ = BigInt::pow_montgomery(T, key.DQ, key.Q);
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

	sl_bool RSA::execute(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, const void* src, void* dst)
	{
		if (keyPublic) {
			return RSA::executePublic(*keyPublic, src, dst);
		} else {
			return RSA::executePrivate(*keyPrivate, src, dst);
		}
	}

#define RSA_PKCS1_SIGN		1
#define RSA_PKCS1_CRYPT		2

	sl_bool RSA::encrypt_pkcs1_v15(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, const void* src, sl_uint32 n, void* dst)
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
		return execute(keyPublic, keyPrivate, dst, dst);
	}

	sl_uint32 RSA::decrypt_pkcs1_v15(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, const void* src, void* dst, sl_uint32 n, sl_bool* pFlagSign)
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
		if (!(execute(keyPublic, keyPrivate, src, buf))) {
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
		for (pos = 2; pos + 1 < len; pos++) {
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
		return encrypt_pkcs1_v15(&key, sl_null, src, n, dst);
	}

	sl_bool RSA::encryptPrivate_pkcs1_v15(const RSAPrivateKey& key, const void* src, sl_uint32 n, void* dst)
	{
		return encrypt_pkcs1_v15(sl_null, &key, src, n, dst);
	}

	sl_uint32 RSA::decryptPublic_pkcs1_v15(const RSAPublicKey& key, const void* src, void* dst, sl_uint32 n, sl_bool* pFlagSign)
	{
		return decrypt_pkcs1_v15(&key, sl_null, src, dst, n, pFlagSign);
	}

	sl_uint32 RSA::decryptPrivate_pkcs1_v15(const RSAPrivateKey& key, const void* src, void* dst, sl_uint32 n, sl_bool* pFlagSign)
	{
		return decrypt_pkcs1_v15(sl_null, &key, src, dst, n, pFlagSign);
	}

}
