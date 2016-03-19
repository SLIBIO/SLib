#include "../../../inc/slib/crypto/sha2.h"
#include "../../../inc/slib/core/mio.h"
#include "../../../inc/slib/core/math.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

_SHA256Base::_SHA256Base()
{
	rdata_len = 0;
}

void _SHA256Base::_start()
{
	rdata_len = 0;
	sizeTotalInput = 0;
}

void _SHA256Base::update(const void* _input, sl_size sizeInput)
{
	if (rdata_len >= 64) {
		return;
	}
	const sl_uint8* input = (const sl_uint8*)_input;
	if (sizeInput == 0) {
		return;
	}
	sizeTotalInput += sizeInput;
	if (rdata_len > 0) {
		sl_uint32 n = 64 - rdata_len;
		if (sizeInput < n) {
			Base::copyMemory(rdata + rdata_len, input, sizeInput);
			rdata_len += (sl_uint32)sizeInput;
			return;
		} else {
			Base::copyMemory(rdata + rdata_len, input, n);
			_updateSection(rdata);
			rdata_len = 0;
			sizeInput -= n;
			input += n;
			if (sizeInput == 0) {
				return;
			}
		}
	}
	while (sizeInput >= 64) {
		_updateSection(input);
		sizeInput -= 64;
		input += 64;
	}
	if (sizeInput) {
		Base::copyMemory(rdata, input, sizeInput);
		rdata_len = (sl_uint32)sizeInput;
	}
}

void _SHA256Base::_finish()
{
	if (rdata_len >= 64) {
		return;
	}
	rdata[rdata_len] = (sl_uint8)0x80;
	if (rdata_len < 56) {
		Base::zeroMemory(rdata + rdata_len + 1, 55 - rdata_len);
		MIO::writeUint64BE(rdata + 56, sizeTotalInput << 3);
		_updateSection(rdata);
	} else {
		Base::zeroMemory(rdata + rdata_len + 1, 63 - rdata_len);
		_updateSection(rdata);
		Base::zeroMemory(rdata, 56);
		MIO::writeUint64BE(rdata + 56, sizeTotalInput << 3);
		_updateSection(rdata);
	}
	rdata_len = 0;
}

void _SHA256Base::_updateSection(const sl_uint8* input)
{
	static sl_uint32 K[64] = {
		0x428a2f98ul, 0x71374491ul, 0xb5c0fbcful, 0xe9b5dba5ul,
		0x3956c25bul, 0x59f111f1ul, 0x923f82a4ul, 0xab1c5ed5ul,
		0xd807aa98ul, 0x12835b01ul, 0x243185beul, 0x550c7dc3ul,
		0x72be5d74ul, 0x80deb1feul, 0x9bdc06a7ul, 0xc19bf174ul,
		0xe49b69c1ul, 0xefbe4786ul, 0x0fc19dc6ul, 0x240ca1ccul,
		0x2de92c6ful, 0x4a7484aaul, 0x5cb0a9dcul, 0x76f988daul,
		0x983e5152ul, 0xa831c66dul, 0xb00327c8ul, 0xbf597fc7ul,
		0xc6e00bf3ul, 0xd5a79147ul, 0x06ca6351ul, 0x14292967ul,
		0x27b70a85ul, 0x2e1b2138ul, 0x4d2c6dfcul, 0x53380d13ul,
		0x650a7354ul, 0x766a0abbul, 0x81c2c92eul, 0x92722c85ul,
		0xa2bfe8a1ul, 0xa81a664bul, 0xc24b8b70ul, 0xc76c51a3ul,
		0xd192e819ul, 0xd6990624ul, 0xf40e3585ul, 0x106aa070ul,
		0x19a4c116ul, 0x1e376c08ul, 0x2748774cul, 0x34b0bcb5ul,
		0x391c0cb3ul, 0x4ed8aa4aul, 0x5b9cca4ful, 0x682e6ff3ul,
		0x748f82eeul, 0x78a5636ful, 0x84c87814ul, 0x8cc70208ul,
		0x90befffaul, 0xa4506cebul, 0xbef9a3f7ul, 0xc67178f2ul,
	};

	sl_uint32 W[64];
	sl_uint32 v[8];
	sl_uint32 i;
	for (i = 0; i < 16; i++) {
		W[i] = MIO::readUint32BE(input + (i << 2));
	}
	for (i = 16; i < 64; i++) {
		sl_uint32 s0 = Math::rotateRight32(W[i - 15], 7) ^ Math::rotateRight32(W[i - 15], 18) ^ (W[i - 15] >> 3);
		sl_uint32 s1 = Math::rotateRight32(W[i - 2], 17) ^ Math::rotateRight32(W[i - 2], 19) ^ (W[i - 2] >> 10);
		W[i] = W[i - 16] + s0 + W[i - 7] + s1;
	}
	for (i = 0; i < 8; i++) {
		v[i] = h[i];
	}
	for (i = 0; i < 64; i++) {
		sl_uint32 S1 = Math::rotateRight32(v[4], 6) ^ Math::rotateRight32(v[4], 11) ^ Math::rotateRight32(v[4], 25);
		sl_uint32 ch = (v[4] & v[5]) ^ ((~v[4]) & v[6]);
		sl_uint32 temp1 = v[7] + S1 + ch + K[i] + W[i];
		sl_uint32 S0 = Math::rotateRight32(v[0], 2) ^ Math::rotateRight32(v[0], 13) ^ Math::rotateRight32(v[0], 22);
		sl_uint32 maj = (v[0] & v[1]) ^ (v[0] & v[2]) ^ (v[1] & v[2]);
		sl_uint32 temp2 = S0 + maj;
		v[7] = v[6];
		v[6] = v[5];
		v[5] = v[4];
		v[4] = v[3] + temp1;
		v[3] = v[2];
		v[2] = v[1];
		v[1] = v[0];
		v[0] = temp1 + temp2;
	}
	for (i = 0; i < 8; i++) {
		h[i] += v[i];
	}
}


SLIB_DEFINE_CRYPTO_HASH(SHA224, 28)

void SHA224::start()
{
	_start();
	h[0] = 0xc1059ed8ul;
	h[1] = 0x367cd507ul;
	h[2] = 0x3070dd17ul;
	h[3] = 0xf70e5939ul;
	h[4] = 0xffc00b31ul;
	h[5] = 0x68581511ul;
	h[6] = 0x64f98fa7ul;
	h[7] = 0xbefa4fa4ul;
}

void SHA224::finish(void* _output)
{
	_finish();
	sl_uint8* output = (sl_uint8*)_output;
	for (sl_uint32 ih = 0; ih < 7; ih ++) {
		MIO::writeUint32BE(output, h[ih]);
		output += 4;
	}
}


SLIB_DEFINE_CRYPTO_HASH(SHA256, 32)

void SHA256::start()
{
	_start();
	h[0] = 0x6a09e667ul;
	h[1] = 0xbb67ae85ul;
	h[2] = 0x3c6ef372ul;
	h[3] = 0xa54ff53aul;
	h[4] = 0x510e527ful;
	h[5] = 0x9b05688cul;
	h[6] = 0x1f83d9abul;
	h[7] = 0x5be0cd19ul;
}

void SHA256::finish(void* _output)
{
	_finish();
	sl_uint8* output = (sl_uint8*)_output;
	for (sl_uint32 ih = 0; ih < 8; ih++) {
		MIO::writeUint32BE(output, h[ih]);
		output += 4;
	}
}


_SHA512Base::_SHA512Base()
{
	rdata_len = 0;
}

void _SHA512Base::_start()
{
	rdata_len = 0;
	sizeTotalInput = 0;
}

void _SHA512Base::update(const void* _input, sl_size sizeInput)
{
	if (rdata_len >= 128) {
		return;
	}
	const sl_uint8* input = (const sl_uint8*)_input;
	if (sizeInput == 0) {
		return;
	}
	sizeTotalInput += sizeInput;
	if (rdata_len > 0) {
		sl_uint32 n = 128 - rdata_len;
		if (sizeInput < n) {
			Base::copyMemory(rdata + rdata_len, input, sizeInput);
			rdata_len += (sl_uint32)sizeInput;
			return;
		} else {
			Base::copyMemory(rdata + rdata_len, input, n);
			_updateSection(rdata);
			rdata_len = 0;
			sizeInput -= n;
			input += n;
			if (sizeInput == 0) {
				return;
			}
		}
	}
	while (sizeInput >= 128) {
		_updateSection(input);
		sizeInput -= 128;
		input += 128;
	}
	if (sizeInput) {
		Base::copyMemory(rdata, input, sizeInput);
		rdata_len = (sl_uint32)sizeInput;
	}
}

void _SHA512Base::_finish()
{
	if (rdata_len >= 128) {
		return;
	}
	rdata[rdata_len] = (sl_uint8)0x80;
	if (rdata_len < 120) {
		Base::zeroMemory(rdata + rdata_len + 1, 119 - rdata_len);
		MIO::writeUint64BE(rdata + 120, sizeTotalInput << 3);
		_updateSection(rdata);
	} else {
		Base::zeroMemory(rdata + rdata_len + 1, 127 - rdata_len);
		_updateSection(rdata);
		Base::zeroMemory(rdata, 120);
		MIO::writeUint64BE(rdata + 120, sizeTotalInput << 3);
		_updateSection(rdata);
	}
	rdata_len = 0;
}

void _SHA512Base::_updateSection(const sl_uint8* input)
{
	static sl_uint64 K[80] = {
		SLIB_UINT64(0x428a2f98d728ae22), SLIB_UINT64(0x7137449123ef65cd), SLIB_UINT64(0xb5c0fbcfec4d3b2f), SLIB_UINT64(0xe9b5dba58189dbbc), SLIB_UINT64(0x3956c25bf348b538),
		SLIB_UINT64(0x59f111f1b605d019), SLIB_UINT64(0x923f82a4af194f9b), SLIB_UINT64(0xab1c5ed5da6d8118), SLIB_UINT64(0xd807aa98a3030242), SLIB_UINT64(0x12835b0145706fbe),
		SLIB_UINT64(0x243185be4ee4b28c), SLIB_UINT64(0x550c7dc3d5ffb4e2), SLIB_UINT64(0x72be5d74f27b896f), SLIB_UINT64(0x80deb1fe3b1696b1), SLIB_UINT64(0x9bdc06a725c71235),
		SLIB_UINT64(0xc19bf174cf692694), SLIB_UINT64(0xe49b69c19ef14ad2), SLIB_UINT64(0xefbe4786384f25e3), SLIB_UINT64(0x0fc19dc68b8cd5b5), SLIB_UINT64(0x240ca1cc77ac9c65),
		SLIB_UINT64(0x2de92c6f592b0275), SLIB_UINT64(0x4a7484aa6ea6e483), SLIB_UINT64(0x5cb0a9dcbd41fbd4), SLIB_UINT64(0x76f988da831153b5), SLIB_UINT64(0x983e5152ee66dfab),
		SLIB_UINT64(0xa831c66d2db43210), SLIB_UINT64(0xb00327c898fb213f), SLIB_UINT64(0xbf597fc7beef0ee4), SLIB_UINT64(0xc6e00bf33da88fc2), SLIB_UINT64(0xd5a79147930aa725),
		SLIB_UINT64(0x06ca6351e003826f), SLIB_UINT64(0x142929670a0e6e70), SLIB_UINT64(0x27b70a8546d22ffc), SLIB_UINT64(0x2e1b21385c26c926), SLIB_UINT64(0x4d2c6dfc5ac42aed),
		SLIB_UINT64(0x53380d139d95b3df), SLIB_UINT64(0x650a73548baf63de), SLIB_UINT64(0x766a0abb3c77b2a8), SLIB_UINT64(0x81c2c92e47edaee6), SLIB_UINT64(0x92722c851482353b),
		SLIB_UINT64(0xa2bfe8a14cf10364), SLIB_UINT64(0xa81a664bbc423001), SLIB_UINT64(0xc24b8b70d0f89791), SLIB_UINT64(0xc76c51a30654be30), SLIB_UINT64(0xd192e819d6ef5218),
		SLIB_UINT64(0xd69906245565a910), SLIB_UINT64(0xf40e35855771202a), SLIB_UINT64(0x106aa07032bbd1b8), SLIB_UINT64(0x19a4c116b8d2d0c8), SLIB_UINT64(0x1e376c085141ab53),
		SLIB_UINT64(0x2748774cdf8eeb99), SLIB_UINT64(0x34b0bcb5e19b48a8), SLIB_UINT64(0x391c0cb3c5c95a63), SLIB_UINT64(0x4ed8aa4ae3418acb), SLIB_UINT64(0x5b9cca4f7763e373),
		SLIB_UINT64(0x682e6ff3d6b2b8a3), SLIB_UINT64(0x748f82ee5defb2fc), SLIB_UINT64(0x78a5636f43172f60), SLIB_UINT64(0x84c87814a1f0ab72), SLIB_UINT64(0x8cc702081a6439ec),
		SLIB_UINT64(0x90befffa23631e28), SLIB_UINT64(0xa4506cebde82bde9), SLIB_UINT64(0xbef9a3f7b2c67915), SLIB_UINT64(0xc67178f2e372532b), SLIB_UINT64(0xca273eceea26619c),
		SLIB_UINT64(0xd186b8c721c0c207), SLIB_UINT64(0xeada7dd6cde0eb1e), SLIB_UINT64(0xf57d4f7fee6ed178), SLIB_UINT64(0x06f067aa72176fba), SLIB_UINT64(0x0a637dc5a2c898a6),
		SLIB_UINT64(0x113f9804bef90dae), SLIB_UINT64(0x1b710b35131c471b), SLIB_UINT64(0x28db77f523047d84), SLIB_UINT64(0x32caab7b40c72493), SLIB_UINT64(0x3c9ebe0a15c9bebc),
		SLIB_UINT64(0x431d67c49c100d4c), SLIB_UINT64(0x4cc5d4becb3e42b6), SLIB_UINT64(0x597f299cfc657e2a), SLIB_UINT64(0x5fcb6fab3ad6faec), SLIB_UINT64(0x6c44198c4a475817)
	};

	sl_uint64 W[80];
	sl_uint64 v[8];
	sl_uint32 i;
	for (i = 0; i < 16; i++) {
		W[i] = MIO::readUint64BE(input + (i << 3));
	}
	for (i = 16; i < 80; i++) {
		sl_uint64 s0 = Math::rotateRight64(W[i - 15], 1) ^ Math::rotateRight64(W[i - 15], 8) ^ (W[i - 15] >> 7);
		sl_uint64 s1 = Math::rotateRight64(W[i - 2], 19) ^ Math::rotateRight64(W[i - 2], 61) ^ (W[i - 2] >> 6);
		W[i] = W[i - 16] + s0 + W[i - 7] + s1;
	}
	for (i = 0; i < 8; i++) {
		v[i] = h[i];
	}
	for (i = 0; i < 80; i++) {
		sl_uint64 S1 = Math::rotateRight64(v[4], 14) ^ Math::rotateRight64(v[4], 18) ^ Math::rotateRight64(v[4], 41);
		sl_uint64 ch = (v[4] & v[5]) ^ ((~v[4]) & v[6]);
		sl_uint64 temp1 = v[7] + S1 + ch + K[i] + W[i];
		sl_uint64 S0 = Math::rotateRight64(v[0], 28) ^ Math::rotateRight64(v[0], 34) ^ Math::rotateRight64(v[0], 39);
		sl_uint64 maj = (v[0] & v[1]) ^ (v[0] & v[2]) ^ (v[1] & v[2]);
		sl_uint64 temp2 = S0 + maj;
		v[7] = v[6];
		v[6] = v[5];
		v[5] = v[4];
		v[4] = v[3] + temp1;
		v[3] = v[2];
		v[2] = v[1];
		v[1] = v[0];
		v[0] = temp1 + temp2;
	}
	for (i = 0; i < 8; i++) {
		h[i] += v[i];
	}
}


SLIB_DEFINE_CRYPTO_HASH(SHA384, 48)

void SHA384::start()
{
	_start();
	h[0] = SLIB_UINT64(0xcbbb9d5dc1059ed8);
	h[1] = SLIB_UINT64(0x629a292a367cd507);
	h[2] = SLIB_UINT64(0x9159015a3070dd17);
	h[3] = SLIB_UINT64(0x152fecd8f70e5939);
	h[4] = SLIB_UINT64(0x67332667ffc00b31);
	h[5] = SLIB_UINT64(0x8eb44a8768581511);
	h[6] = SLIB_UINT64(0xdb0c2e0d64f98fa7);
	h[7] = SLIB_UINT64(0x47b5481dbefa4fa4);
}

void SHA384::finish(void* _output)
{
	_finish();
	sl_uint8* output = (sl_uint8*)_output;
	for (sl_uint32 ih = 0; ih < 6; ih++) {
		MIO::writeUint64BE(output, h[ih]);
		output += 8;
	}
}


SLIB_DEFINE_CRYPTO_HASH(SHA512, 64)

void SHA512::start()
{
	_start();
	h[0] = SLIB_UINT64(0x6a09e667f3bcc908);
	h[1] = SLIB_UINT64(0xbb67ae8584caa73b);
	h[2] = SLIB_UINT64(0x3c6ef372fe94f82b);
	h[3] = SLIB_UINT64(0xa54ff53a5f1d36f1);
	h[4] = SLIB_UINT64(0x510e527fade682d1);
	h[5] = SLIB_UINT64(0x9b05688c2b3e6c1f);
	h[6] = SLIB_UINT64(0x1f83d9abfb41bd6b);
	h[7] = SLIB_UINT64(0x5be0cd19137e2179);
}

void SHA512::finish(void* _output)
{
	_finish();
	sl_uint8* output = (sl_uint8*)_output;
	for (sl_uint32 ih = 0; ih < 8; ih++) {
		MIO::writeUint64BE(output, h[ih]);
		output += 8;
	}
}

sl_uint32 SHA256::make32bitChecksum(const void* input, sl_size n)
{
	char hash[32];
	SHA256::hash(input, (sl_uint32)n, hash);
	for (sl_uint32 i = 4; i < 32; i++) {
		hash[i % 4] ^= hash[i];
	}
	return MIO::readUint32LE(hash);
}

SLIB_CRYPTO_NAMESPACE_END

