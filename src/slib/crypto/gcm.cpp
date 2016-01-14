#include "../../../inc/slib/crypto/gcm.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

void BlockCipher_GCM_Base::generateTable(const void* _H)
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

static const sl_uint64 _BlockCipher_GCM_Base_R[16] =
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

void BlockCipher_GCM_Base::multiplyH(const void* _X, void* _O) const
{
	const sl_uint8* X = (const sl_uint8*)_X;
	sl_uint8* O = (sl_uint8*)_O;
	Uint128 Z;
	const sl_uint64* R = _BlockCipher_GCM_Base_R;
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

void BlockCipher_GCM_Base::multiplyData(void* _X, const void* _D, sl_size lenD) const
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

void BlockCipher_GCM_Base::multiplyLength(void* _X, sl_size len1, sl_size len2) const
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

void BlockCipher_GCM_Base::calculateGHash(const void* A, sl_size lenA, const void* C, sl_size lenC, void* _O) const
{
	sl_uint8* O = (sl_uint8*)_O;

	Base::zeroMemory(O, 16);
	
	multiplyData(O, A, lenA);

	multiplyData(O, C, lenC);

	multiplyLength(O, lenA, lenC);
}

void BlockCipher_GCM_Base::calculateCIV(const void* _IV, sl_size lenIV, void* _CIV) const
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

SLIB_CRYPTO_NAMESPACE_END
