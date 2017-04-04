/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/crypto/gcm.h"

#include "slib/crypto/aes.h"

namespace slib
{

	void GCM_Table::generateTable(const void* _H)
	{
		sl_uint32 i, j;
		Uint128 H;

		H.setBytesBE(_H);

/*
	  In 4-bit table
	    M[8]=H, M[4]=H*P, M[2]=H*P*P, M[1]=H*P*P*P, M[0]=0
	 And, in GF(2^128)
	    f = 1 + P + P^2 + P^7 + P^128 => (P^128 + x) mod f = x - (1 + P + P^2 + P^7) = x + (1 + P + P^2 + P^7)
		  H*P = (H&1) ? ((H>>1)|0xE1000000000000000000000000000000) : (H>>1)
*/

		M[0].setZero();
		M[8] = H;

		i = 4;
		while (i > 0) {
			sl_uint64 K = ((sl_uint64)((-((sl_int32)(H.low & 1))) & 0xe1000000)) << 32;
			H.shiftRight();
			H.high ^= K;
			M[i] = H;
			i >>= 1;
		}

		// n = a + b => M[n] = M[a] + M[b]
		i = 2;
		while (i < 16) {
			for (j = 1; j < i; j++) {
				M[i + j] = M[i] ^ M[j];
			}
			i <<= 1;
		}
	}

	static const sl_uint64 _GCM_R[16] =
	{
		SLIB_UINT64(0x0000000000000000)
		, SLIB_UINT64(0x1c20000000000000)
		, SLIB_UINT64(0x3840000000000000)
		, SLIB_UINT64(0x2460000000000000)
		, SLIB_UINT64(0x7080000000000000)
		, SLIB_UINT64(0x6ca0000000000000)
		, SLIB_UINT64(0x48c0000000000000)
		, SLIB_UINT64(0x54e0000000000000)
		, SLIB_UINT64(0xe100000000000000)
		, SLIB_UINT64(0xfd20000000000000)
		, SLIB_UINT64(0xd940000000000000)
		, SLIB_UINT64(0xc560000000000000)
		, SLIB_UINT64(0x9180000000000000)
		, SLIB_UINT64(0x8da0000000000000)
		, SLIB_UINT64(0xa9c0000000000000)
		, SLIB_UINT64(0xb5e0000000000000)
	};

	void GCM_Table::multiplyH(const void* _X, void* _O) const
	{
		const sl_uint8* X = (const sl_uint8*)_X;
		sl_uint8* O = (sl_uint8*)_O;
		Uint128 Z;
		const sl_uint64* R = _GCM_R;
		Z.setZero();
		for (sl_uint32 i = 15; i >= 1; i--) {
			// process low 4-bit
			{
				sl_uint32 n = X[i] & 15;
				Z ^= M[n];
				sl_uint32 r = Z.low & 15;
				Z >>= 4;
				Z.high ^= R[r];
			}
			// process high 4-bit
			{
				sl_uint32 n = (X[i] >> 4) & 15;
				Z ^= M[n];
				sl_uint32 r = Z.low & 15;
				Z >>= 4;
				Z.high ^= R[r];
			}
		}
		// process low 4-bit
		{
			sl_uint32 n = X[0] & 15;
			Z ^= M[n];
			sl_uint32 r = Z.low & 15;
			Z >>= 4;
			Z.high ^= R[r];
		}
		// process high 4-bit
		{
			sl_uint32 n = (X[0] >> 4) & 15;
			Z ^= M[n];
		}
		Z.getBytesBE(O);
	}

	void GCM_Table::multiplyData(void* _X, const void* _D, sl_size lenD) const
	{
		sl_uint8* X = (sl_uint8*)_X;
		const sl_uint8* D = (const sl_uint8*)_D;
		sl_size i, k, n;

		n = lenD >> 4;
		for (i = 0; i < n; i++) {
			for (k = 0; k < 16; k++) {
				X[k] ^= *D;
				D++;
			}
			multiplyH(X, X);
		}
		n = lenD & 15;
		if (n) {
			for (k = 0; k < n; k++) {
				X[k] ^= *D;
				D++;
			}
			multiplyH(X, X);
		}
	}

	void GCM_Table::multiplyLength(void* _X, sl_size len1, sl_size len2) const
	{
		sl_uint8* X = (sl_uint8*)_X;
		sl_uint64 v;
		v = len1 << 3;
		X[0] ^= (sl_uint8)(v >> 56);
		X[1] ^= (sl_uint8)(v >> 48);
		X[2] ^= (sl_uint8)(v >> 40);
		X[3] ^= (sl_uint8)(v >> 32);
		X[4] ^= (sl_uint8)(v >> 24);
		X[5] ^= (sl_uint8)(v >> 16);
		X[6] ^= (sl_uint8)(v >> 8);
		X[7] ^= (sl_uint8)(v);
		v = len2 << 3;
		X[8] ^= (sl_uint8)(v >> 56);
		X[9] ^= (sl_uint8)(v >> 48);
		X[10] ^= (sl_uint8)(v >> 40);
		X[11] ^= (sl_uint8)(v >> 32);
		X[12] ^= (sl_uint8)(v >> 24);
		X[13] ^= (sl_uint8)(v >> 16);
		X[14] ^= (sl_uint8)(v >> 8);
		X[15] ^= (sl_uint8)(v);
		multiplyH(X, X);
	}

	void GCM_Table::calculateGHash(const void* A, sl_size lenA, const void* C, sl_size lenC, void* _O) const
	{
		sl_uint8* O = (sl_uint8*)_O;

		Base::zeroMemory(O, 16);
		
		multiplyData(O, A, lenA);

		multiplyData(O, C, lenC);

		multiplyLength(O, lenA, lenC);
	}

	void GCM_Table::calculateCIV(const void* _IV, sl_size lenIV, void* _CIV) const
	{
		const sl_uint8* IV = (const sl_uint8*)_IV;
		sl_uint8* CIV = (sl_uint8*)_CIV;
		sl_size i;
		if (lenIV == 12) {
			for (i = 0; i < 12; i++) {
				CIV[i] = IV[i];
			}
			CIV[12] = 0;
			CIV[13] = 0;
			CIV[14] = 0;
			CIV[15] = 1;
		} else {
			calculateGHash(sl_null, 0, IV, lenIV, CIV);
		}
	}


	void GCM_Base::increaseCIV()
	{
		for (sl_uint32 i = 15; i >= 12; i--) {
			if (++(CIV[i]) != 0) {
				break;
			}
		}
	}

	void GCM_Base::putBlock(const void* src, sl_uint32 n)
	{
		const sl_uint8* A = (const sl_uint8*)src;
		for (sl_uint32 k = 0; k < n; k++) {
			GHASH_X[k] ^= *A;
			A++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	void GCM_Base::put(const void* src, sl_size len)
	{
		multiplyData(GHASH_X, src, len);
	}

	sl_bool GCM_Base::finish(sl_size lenA, sl_size lenC, void* _tag, sl_size lenTag)
	{
		if (lenTag < 4 || lenTag > 16) {
			return sl_false;
		}
		sl_uint8* tag = (sl_uint8*)_tag;
		multiplyLength(GHASH_X, lenA, lenC);
		for (sl_size i = 0; i < lenTag; i++) {
			tag[i] = GHASH_X[i] ^ GCTR0[i];
		}
		return sl_true;
	}

	sl_bool GCM_Base::finishAndCheckTag(sl_size lenA, sl_size lenC, const void* _tag, sl_size lenTag)
	{
		if (lenTag < 4 || lenTag > 16) {
			return sl_false;
		}
		const sl_uint8* tag = (const sl_uint8*)_tag;
		multiplyLength(GHASH_X, lenA, lenC);
		for (sl_size i = 0; i < lenTag; i++) {
			if (tag[i] != (GHASH_X[i] ^ GCTR0[i])) {
				return sl_false;
			}
		}
		return sl_true;
	}


	template <class BlockCipher>
	GCM<BlockCipher>::GCM()
	{
		setCipher(sl_null);
	}

	template <class BlockCipher>
	GCM<BlockCipher>::GCM(const BlockCipher* cipher)
	{
		setCipher(cipher);
	}

	template <class BlockCipher>
	GCM<BlockCipher>::~GCM()
	{
	}

	template <class BlockCipher>
	sl_bool GCM<BlockCipher>::setCipher(const BlockCipher* cipher)
	{
		if (cipher) {
			if (cipher->getBlockSize() != 16) {
				return sl_false;
			}
			sl_uint8 H[16] = { 0 };
			cipher->encryptBlock(H, H);
			generateTable(H);
		}
		m_cipher = cipher;
		return sl_true;
	}

	template <class BlockCipher>
	sl_bool GCM<BlockCipher>::start(const void* IV, sl_size lenIV)
	{
		if (!m_cipher) {
			return sl_false;
		}
		calculateCIV(IV, lenIV, CIV);
		m_cipher->encryptBlock(CIV, GCTR0);
		Base::zeroMemory(GHASH_X, 16);
		return sl_true;
	}

	template <class BlockCipher>
	void GCM<BlockCipher>::encryptBlock(const void* src, void* dst, sl_uint32 n)
	{
		sl_uint8 GCTR[16];
		sl_size k;
		const sl_uint8* P = (const sl_uint8*)src;
		sl_uint8* C = (sl_uint8*)dst;
		increaseCIV();
		m_cipher->encryptBlock(CIV, GCTR);
		for (k = 0; k < n; k++) {
			*C = *P ^ GCTR[k];
			GHASH_X[k] ^= *C;
			C++;
			P++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	template <class BlockCipher>
	void GCM<BlockCipher>::encrypt(const void* src, void *dst, sl_size len)
	{
		sl_uint8 GCTR[16];
		sl_size i, k, n;
		const sl_uint8* P = (const sl_uint8*)src;
		sl_uint8* C = (sl_uint8*)dst;
		
		for (i = 0; i < len; i += 16) {
			increaseCIV();
			m_cipher->encryptBlock(CIV, GCTR);
			n = len - i;
			if (n > 16) {
				n = 16;
			}
			for (k = 0; k < n; k++) {
				*C = *P ^ GCTR[k];
				GHASH_X[k] ^= *C;
				C++;
				P++;
			}
			multiplyH(GHASH_X, GHASH_X);
		}
	}

	template <class BlockCipher>
	void GCM<BlockCipher>::decryptBlock(const void* src, void* dst, sl_uint32 n)
	{
		sl_uint8 GCTR[16];
		sl_size k;
		const sl_uint8* C = (const sl_uint8*)src;
		sl_uint8* P = (sl_uint8*)dst;
		increaseCIV();
		m_cipher->encryptBlock(CIV, GCTR);
		for (k = 0; k < n; k++) {
			GHASH_X[k] ^= *C;
			*P = *C ^ GCTR[k];
			C++;
			P++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	template <class BlockCipher>
	void GCM<BlockCipher>::decrypt(const void* src, void *dst, sl_size len)
	{
		sl_uint8 GCTR[16];
		sl_size i, k, n;
		const sl_uint8* C = (const sl_uint8*)src;
		sl_uint8* P = (sl_uint8*)dst;
		
		for (i = 0; i < len; i += 16) {
			increaseCIV();
			m_cipher->encryptBlock(CIV, GCTR);
			n = len - i;
			if (n > 16) {
				n = 16;
			}
			for (k = 0; k < n; k++) {
				GHASH_X[k] ^= *C;
				*P = *C ^ GCTR[k];
				C++;
				P++;
			}
			multiplyH(GHASH_X, GHASH_X);
		}
	}

	template <class BlockCipher>
	sl_bool GCM<BlockCipher>::encrypt(
					const void* IV, sl_size lenIV
					, const void* A, sl_size lenA
					, const void* input, void* output, sl_size len
					, void* tag, sl_size lenTag
	)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		encrypt(input, output, len);
		return finish(lenA, len, tag, lenTag);
	}

	template <class BlockCipher>
	sl_bool GCM<BlockCipher>::decrypt(
					const void* IV, sl_size lenIV
					, const void* A, sl_size lenA
					, const void* input, void* output, sl_size len
					, const void* tag, sl_size lenTag
	)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		decrypt(input, output, len);
		return finishAndCheckTag(lenA, len, tag, lenTag);
	}

	template <class BlockCipher>
	sl_bool GCM<BlockCipher>::check(
				const void* IV, sl_size lenIV
				, const void* A, sl_size lenA
				, const void* C, sl_size lenC
				, const void* tag, sl_size lenTag
	)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		put(C, lenC);
		return finishAndCheckTag(lenA, lenC, tag, lenTag);
	}


	template class GCM<AES>;

}
