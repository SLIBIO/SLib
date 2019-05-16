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

#include "slib/crypto/ecc.h"

#include "slib/crypto/sha2.h"

#include "slib/core/safe_static.h"

#include "ecc_secp256k1.inc"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ECPoint)
	
	ECPoint::ECPoint()
	{
	}
	
	sl_bool ECPoint::isO() const
	{
		return y.isZero();
	}
	
	Memory ECPoint::toUncompressedFormat(const EllipticCurve& curve) const
	{
		return toUncompressedFormat(curve.n.getMostSignificantBytes());
	}
	
	Memory ECPoint::toUncompressedFormat(sl_size nBytesPerComponent) const
	{
		if (isO()) {
			sl_uint8 c = 0;
			return Memory::create(&c, 1);
		}
		Memory ret = Memory::create((nBytesPerComponent << 1) + 1);
		if (ret.isNotNull()) {
			sl_uint8* buf = (sl_uint8*)(ret.getData());
			buf[0] = 4;
			if (x.getBytesBE(buf + 1, nBytesPerComponent)) {
				if (y.getBytesBE(buf + 1 + nBytesPerComponent, nBytesPerComponent)) {
					return ret;
				}
			}
		}
		return sl_null;
	}
	
	void ECPoint::parseUncompressedFormat(const void* _buf, sl_size size)
	{
		const sl_uint8* buf = (const sl_uint8*)_buf;
		if (size) {
			if (buf[0] == 4) {
				size--;
				if (!(size & 1)) {
					size >>= 1;
					x = BigInt::fromBytesBE(buf + 1, size);
					if (x.isNotZero()) {
						y = BigInt::fromBytesBE(buf + 1 + size, size);
					} else {
						y.setNull();
					}
					return;
				}
			}
		}
		x.setNull();
		y.setNull();
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EllipticCurve)

	EllipticCurve::EllipticCurve(): h(1)
	{
	}
	
	ECPoint EllipticCurve::addPoint(const ECPoint& p1, const ECPoint& p2) const
	{
		if (p1.isO()) {
			return p2;
		} else if (p2.isO()) {
			return p1;
		}
		if (p1.x == p2.x) {
			if (p1.y + p2.y == p) {
				return ECPoint();
			} else {
				return doublePoint(p1);
			}
		} else {
			ECPoint ret;
			BigInt lambda = ((p2.y - p1.y) * BigInt::inverseMod(p2.x - p1.x, p)) % p;
			ret.x = ((lambda * lambda) - p1.x - p2.x) % p;
			ret.y = ((lambda * (p1.x - ret.x)) - p1.y) % p;
			return ret;
		}
	}
	
	ECPoint EllipticCurve::doublePoint(const ECPoint& pt) const
	{
		if (pt.isO()) {
			return pt;
		}
		ECPoint ret;
		BigInt x2 = pt.x * pt.x;
		BigInt lambda = ((x2 + x2 + x2 + a) * BigInt::inverseMod(pt.y + pt.y, p)) % p;
		ret.x = ((lambda * lambda) - pt.x - pt.x) % p;
		ret.y = ((lambda * (pt.x - ret.x)) - pt.y) % p;
		return ret;
	}
	
	ECPoint EllipticCurve::multiplyPoint(const ECPoint& pt, const BigInt& _k) const
	{
		CBigInt* k = _k.ref.get();
		if (!k) {
			return ECPoint();
		}
		if (k->isZero()) {
			return ECPoint();
		}
		if (k->equals(1)) {
			return pt;
		}
		sl_size nBits = k->getMostSignificantBits();
		ECPoint ret;
		ECPoint pt2 = pt;
		for (sl_size i = 0; i < nBits; i++) {
			if (k->getBit(i)) {
				ret = addPoint(ret, pt2);
			}
			pt2 = doublePoint(pt2);
		}
		return ret;
	}
	
	ECPoint EllipticCurve::multiplyG(const BigInt& _k) const
	{
		if (pow2g.isNull()) {
			return multiplyPoint(G, _k);
		}
		CBigInt* k = _k.ref.get();
		if (!k) {
			return ECPoint();
		}
		if (k->isZero()) {
			return ECPoint();
		}
		if (k->equals(1)) {
			return G;
		}
		sl_size nBits = k->getMostSignificantBits();
		ECPoint ret;
		ECPoint pt;
		for (sl_size i = 0; i < nBits; i++) {
			if (k->getBit(i)) {
				if (i) {
					pow2g.getAt(i - 1, &pt);
					ret = addPoint(ret, pt);
				} else {
					ret = addPoint(ret, G);
				}
			}
		}
		return ret;
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ECPublicKey)
	
	ECPublicKey::ECPublicKey()
	{
	}
	
	sl_bool ECPublicKey::checkValid(const EllipticCurve& curve) const
	{
		if (Q.isO()) {
			return sl_false;
		}
		if (Q.x >= curve.p) {
			return sl_false;
		}
		if (Q.y >= curve.p) {
			return sl_false;
		}
		BigInt dy = ((Q.x * Q.x * Q.x) + (curve.a * Q.x) + curve.b - (Q.y * Q.y)) % curve.p;
		if (dy.isNotZero()) {
			return sl_false;
		}
		ECPoint nQ = curve.multiplyPoint(Q, curve.n);
		if (!(nQ.isO())) {
			return sl_false;
		}
		return sl_true;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ECPrivateKey)
	
	ECPrivateKey::ECPrivateKey()
	{
	}
	
	sl_bool ECPrivateKey::generate(const EllipticCurve& curve)
	{
		if (curve.n < 3) {
			return sl_false;
		}
		BigInt n2 = curve.n - 2;
		for (;;) {
			d = BigInt::random(curve.n.getMostSignificantBits());
			if (d.isNull()) {
				return sl_false;
			}
			d = d % n2 + 2;
			Q = curve.multiplyG(d);
			if (checkValid(curve)) {
				break;
			}
		}
		return sl_true;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ECDSA_Signature)
	
	ECDSA_Signature::ECDSA_Signature()
	{
	}
	
	namespace priv
	{
		namespace ecdsa
		{
			template <class HASH>
			static BigInt makeZ(const EllipticCurve& curve, const void* data, sl_size size)
			{
				sl_uint8 hash[HASH::HashSize];
				HASH::hash(data, size, hash);
				sl_size nBits = curve.n.getMostSignificantBits();
				if (!nBits) {
					return BigInt::null();
				}
				sl_uint32 hashBits = sizeof(hash) << 3;
				if (nBits > hashBits) {
					return BigInt::fromBytesBE(hash, sizeof(hash));
				} else {
					sl_uint32 leftBits = (sl_uint32)(nBits & 7);
					if (leftBits) {
						sl_size nBytes = (nBits >> 3) + 1;
						return BigInt::fromBytesBE(hash, nBytes) >> (8 - leftBits);
					} else {
						return BigInt::fromBytesBE(hash, nBits >> 3);
					}
				}
			}
		}
	}
	
	ECDSA_Signature ECDSA::sign(const EllipticCurve& curve, const ECPrivateKey& key, const BigInt& z, BigInt* _k)
	{
		if (curve.G.isO()) {
			return ECDSA_Signature();
		}
		sl_size nBitsOrder = curve.n.getMostSignificantBits();
		if (nBitsOrder < 2) {
			return ECDSA_Signature();
		}
		BigInt r, s;
		for (;;) {
			sl_bool flagInputK = sl_false;
			BigInt k;
			if (_k) {
				if (_k->isNull()) {
					k = (BigInt::random(nBitsOrder) % (curve.n - 1)) + 1;
				} else {
					flagInputK = sl_true;
					k = *_k;
				}
			} else {
				k = (BigInt::random(nBitsOrder) % (curve.n - 1)) + 1;
			}
			ECPoint kG = curve.multiplyG(k);
			if (kG.isO()) {
				if (flagInputK) {
					return ECDSA_Signature();
				}
				continue;
			}
			r = kG.x % curve.n;
			if (r.isZero()) {
				if (flagInputK) {
					return ECDSA_Signature();
				}
				continue;
			}
			BigInt k1 = BigInt::inverseMod(k, curve.n);
			s = (k1 * (z + r * key.d)) % curve.n;
			if (s.isZero()) {
				if (flagInputK) {
					return ECDSA_Signature();
				}
				continue;
			}
			if (!flagInputK) {
				if (_k) {
					*_k = k;
				}
			}
			break;
		}
		ECDSA_Signature ret;
		ret.r = r;
		ret.s = s;
		return ret;
	}
	
	ECDSA_Signature ECDSA::sign_SHA256(const EllipticCurve& curve, const ECPrivateKey& key, const void* data, sl_size size, BigInt* k)
	{
		if (curve.n < 2) {
			return ECDSA_Signature();
		}
		return sign(curve, key, priv::ecdsa::makeZ<SHA256>(curve, data, size), k);
	}
	
	sl_bool ECDSA::verify(const EllipticCurve& curve, const ECPublicKey& key, const BigInt& z, const ECDSA_Signature& signature)
	{
		if (!(key.checkValid(curve))) {
			return sl_false;
		}
		if (signature.r.isZero()) {
			return sl_false;
		}
		if (signature.r >= curve.n) {
			return sl_false;
		}
		if (signature.s.isZero()) {
			return sl_false;
		}
		if (signature.s >= curve.n) {
			return sl_false;
		}
		BigInt s1 = BigInt::inverseMod(signature.s, curve.n);
		BigInt u1 = (z * s1) % curve.n;
		BigInt u2 = (signature.r * s1) % curve.n;
		ECPoint p1 = curve.multiplyG(u1);
		ECPoint p2 = curve.multiplyPoint(key.Q, u2);
		ECPoint kG = curve.addPoint(p1, p2);
		if (kG.isO()) {
			return sl_false;
		}
		return kG.x == signature.r;
	}
	
	sl_bool ECDSA::verify_SHA256(const EllipticCurve& curve, const ECPublicKey& key, const void* data, sl_size size, const ECDSA_Signature& signature)
	{
		if (curve.n < 2) {
			return sl_false;
		}
		return verify(curve, key, priv::ecdsa::makeZ<SHA256>(curve, data, size), signature);
	}
	
	BigInt ECDH::getSharedKey(const EllipticCurve& curve, const ECPrivateKey& keyLocal, const ECPublicKey& keyRemote)
	{
		if (!(keyRemote.checkValid(curve))) {
			return BigInt::null();
		}
		ECPoint pt = curve.multiplyPoint(keyRemote.Q, keyLocal.d);
		return pt.x;
	}
	
}
