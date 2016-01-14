#ifndef CHECKHEADER_SLIB_CRYPTO_GCM
#define CHECKHEADER_SLIB_CRYPTO_GCM

#include "definition.h"

#include "../math/int128.h"

/*
	GCM - Galois/Counter Mode

	https://en.wikipedia.org/wiki/Galois/Counter_Mode
	http://csrc.nist.gov/publications/nistpubs/800-38D/SP-800-38D.pdf
	http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-revised-spec.pdf

GCM is constructed from an approved symmetric key block cipher with a block size of 128 bits,
such as the Advanced Encryption Standard (AES) algorithm

*/

SLIB_CRYPTO_NAMESPACE_BEGIN

class SLIB_EXPORT BlockCipher_GCM_Base
{
private:
	Uint128 M[16]; // Shoup's, 4-bit table

public:
	void generateTable(const void* H /* 16 bytes */);
	
	void multiplyH(const void* X /* 16 bytes */, void* O /* 16 bytes */) const;
	
	void multiplyData(void* X /* 16 bytes, inout */, const void* D, sl_size lenD) const;

	void multiplyLength(void* X /* 16 bytes, inout */, sl_size len1, sl_size len2) const;

	void calculateGHash(const void* A, sl_size lenA, const void* C, sl_size lenC, void* O /* 16 bytes */) const;

	// lenIV shoud be at least 12
	void calculateCIV(const void* IV, sl_size lenIV, void* CIV /* 16 bytes */) const;
};

template <class BlockCipher>
class SLIB_EXPORT BlockCipher_GCM : public BlockCipher_GCM_Base
{
public:
	SLIB_INLINE BlockCipher_GCM(const BlockCipher* cipher = sl_null)
	{
		setCipher(cipher);
	}

public:
	sl_bool setCipher(const BlockCipher* cipher)
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

	sl_bool start(const void* IV, sl_size lenIV)
	{
		if (!m_cipher) {
			return sl_false;
		}
		calculateCIV(IV, lenIV, CIV);
		m_cipher->encryptBlock(CIV, GCTR0);
		Base::zeroMemory(GHASH_X, 16);		
		return sl_true;
	}

	void putBlock(const void* src, sl_uint32 n = 16 /* n <= 16 */)
	{
		const sl_uint8* A = (const sl_uint8*)src;
		for (sl_uint32 k = 0; k < n; k++) {
			GHASH_X[k] ^= *A;
			A++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	void put(const void* src, sl_size len)
	{
		multiplyData(GHASH_X, src, len);
	}

	void encryptBlock(const void* src, void* dst /* out */, sl_uint32 n = 16 /* n <= 16 */)
	{
		sl_uint8 GCTR[16];
		sl_size k;
		const sl_uint8* P = (const sl_uint8*)src;
		sl_uint8* C = (sl_uint8*)dst;
		_increaseCIV();
		m_cipher->encryptBlock(CIV, GCTR);
		for (k = 0; k < n; k++) {
			*C = *P ^ GCTR[k];
			GHASH_X[k] ^= *C;
			C++;
			P++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	void encrypt(const void* src, void *dst /* out */, sl_size len)
	{
		sl_uint8 GCTR[16];
		sl_size i, k, n;
		const sl_uint8* P = (const sl_uint8*)src;
		sl_uint8* C = (sl_uint8*)dst;

		for (i = 0; i < len; i += 16) {
			_increaseCIV();
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

	void decryptBlock(const void* src, void* dst /* out */, sl_uint32 n = 16 /* n <= 16 */)
	{
		sl_uint8 GCTR[16];
		sl_size k;
		const sl_uint8* C = (const sl_uint8*)src;
		sl_uint8* P = (sl_uint8*)dst;
		_increaseCIV();
		m_cipher->encryptBlock(CIV, GCTR);
		for (k = 0; k < n; k++) {
			GHASH_X[k] ^= *C;
			*P = *C ^ GCTR[k];
			C++;
			P++;
		}
		multiplyH(GHASH_X, GHASH_X);
	}

	void decrypt(const void* src, void *dst /* out */, sl_size len)
	{
		sl_uint8 GCTR[16];
		sl_size i, k, n;
		const sl_uint8* C = (const sl_uint8*)src;
		sl_uint8* P = (sl_uint8*)dst;

		for (i = 0; i < len; i += 16) {
			_increaseCIV();
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

	sl_bool finish(sl_size lenA, sl_size lenC, void* _tag /* out */, sl_size lenTag = 16 /* 4 <= lenTag <= 16 */)
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

	sl_bool finishAndCheckTag(sl_size lenA, sl_size lenC, const void* _tag, sl_size lenTag = 16 /* 4 <= lenTag <= 16 */)
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

	sl_bool encrypt(
		const void* IV, sl_size lenIV
		, const void* A, sl_size lenA
		, const void* input, void* output /* out */, sl_size len
		, void* tag /* out */, sl_size lenTag = 16 /* 4 <= lenTag <= 16 */
		)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		encrypt(input, output, len);
		return finish(lenA, len, tag, lenTag);
	}

	sl_bool decrypt(
		const void* IV, sl_size lenIV
		, const void* A, sl_size lenA
		, const void* input, void* output /* out */, sl_size len
		, const void* tag, sl_size lenTag = 16 /* 4 <= lenTag <= 16 */
		)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		decrypt(input, output, len);
		return finishAndCheckTag(lenA, len, tag, lenTag);
	}

#ifdef check
#undef check
#endif
	sl_bool check(
		const void* IV, sl_size lenIV
		, const void* A, sl_size lenA
		, const void* C, sl_size lenC
		, const void* tag, sl_size lenTag = 16 /* 4 <= lenTag <= 16 */
		)
	{
		if (!start(IV, lenIV)) {
			return sl_false;
		}
		put(A, lenA);
		put(C, lenC);
		return finishAndCheckTag(lenA, lenC, tag, lenTag);
	}

private:
	SLIB_INLINE void _increaseCIV()
	{
		for (sl_uint32 i = 15; i >= 12; i--) {
			if (++(CIV[i]) != 0) {
				break;
			}
		}
	}
	
private:
	const BlockCipher* m_cipher;
	
	sl_uint8 CIV[16];
	sl_uint8 GCTR0[16];
	sl_uint8 GHASH_X[16];
	
};

SLIB_CRYPTO_NAMESPACE_END

#endif
