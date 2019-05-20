/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_CRYPTO_ECC
#define CHECKHEADER_SLIB_CRYPTO_ECC

#include "definition.h"

#include "../math/bigint.h"
#include "../core/math.h"
#include "../core/list.h"

/*
 		Elliptic-curve cryptography
 
 	https://en.wikipedia.org/wiki/Elliptic-curve_cryptography
 
*/

namespace slib
{
	
	class EllipticCurve;
	
	class SLIB_EXPORT ECPoint
	{
	public:
		BigInt x;
		BigInt y;
		
	public:
		ECPoint();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ECPoint)
		
	public:
		sl_bool isO() const;

		Memory toUncompressedFormat(const EllipticCurve& curve) const;
		
		Memory toUncompressedFormat(sl_size nBytesPerComponent) const;
		
		void parseUncompressedFormat(const void* buf, sl_size n);
		
	};
	
	class SLIB_EXPORT EllipticCurve
	{
	public:
		BigInt p;
		BigInt a;
		BigInt b;
		ECPoint G;
		BigInt n; // order
		sl_uint8 h; // cofactor
		
		List<ECPoint> pow2g;
		
	public:
		EllipticCurve();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(EllipticCurve)
		
	public:
		static const EllipticCurve& secp256k1();
		
	public:
		ECPoint addPoint(const ECPoint& p1, const ECPoint& p2) const;
		
		ECPoint doublePoint(const ECPoint& pt) const;
		
		ECPoint multiplyPoint(const ECPoint& pt, const BigInt& k) const;
		
		ECPoint multiplyG(const BigInt& k) const;
		
	};
	
	class SLIB_EXPORT ECPublicKey
	{
	public:
		ECPoint Q; // Q = dG

	public:
		ECPublicKey();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ECPublicKey)
		
	public:		
		sl_bool checkValid(const EllipticCurve& curve) const;
		
	};
	
	class SLIB_EXPORT ECPrivateKey : public ECPublicKey
	{
	public:
		BigInt d;
		
	public:
		ECPrivateKey();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ECPrivateKey)
		
	public:
		sl_bool generate(const EllipticCurve& curve);
		
	};
	
	class SLIB_EXPORT ECDSA_Signature
	{
	public:
		BigInt r;
		BigInt s;
		
	public:
		ECDSA_Signature();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ECDSA_Signature)
		
	};
	
	// Elliptic Curve Digital Signature Algorithm
	class SLIB_EXPORT ECDSA
	{
	public:
		// z < curve.n
		static ECDSA_Signature sign(const EllipticCurve& curve, const ECPrivateKey& key, const BigInt& z, BigInt* k = sl_null);
		
		static ECDSA_Signature sign(const EllipticCurve& curve, const ECPrivateKey& key, const void* hash, sl_size sizeHash, BigInt* k = sl_null);

		static ECDSA_Signature sign_SHA256(const EllipticCurve& curve, const ECPrivateKey& key, const void* data, sl_size size, BigInt* k = sl_null);

		// z < curve.n
		static sl_bool verify(const EllipticCurve& curve, const ECPublicKey& key, const BigInt& z, const ECDSA_Signature& signature);
		
		static sl_bool verify(const EllipticCurve& curve, const ECPublicKey& key, const void* hash, sl_size sizeHash, const ECDSA_Signature& signature);

		static sl_bool verify_SHA256(const EllipticCurve& curve, const ECPublicKey& key, const void* data, sl_size size, const ECDSA_Signature& signature);

	};
	
	// Elliptic Curve Diffie–Hellman
	class SLIB_EXPORT ECDH
	{
	public:
		static BigInt getSharedKey(const EllipticCurve& curve, const ECPrivateKey& keyLocal, const ECPublicKey& keyRemote);
		
	};
	
}

#endif

