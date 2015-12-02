#include "../../../inc/slib/crypto/sha1.h"
#include "../../../inc/slib/core/mio.h"

SLIB_CRYPTO_NAMESPACE_BEGIN
SHA1::SHA1()
{
	rdata_len = 0;
}

void SHA1::start()
{
	h[0] = 0x67452301ul;
	h[1] = 0xEFCDAB89ul;
	h[2] = 0x98BADCFEul;
	h[3] = 0x10325476ul;
	h[4] = 0xC3D2E1F0ul;
	rdata_len = 0;
	sizeTotalInput = 0;
}

void SHA1::update(const void* _input, sl_size sizeInput)
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

void SHA1::finish(void* _output)
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

	sl_uint8* output = (sl_uint8*)_output;
	for (sl_uint32 ih = 0; ih < 5; ih++) {
		MIO::writeUint32BE(output, h[ih]);
		output += 4;
	}
}

void SHA1::_updateSection(const sl_uint8* input)
{
	static sl_uint32 K[4] = {
		0x5A827999ul, 0x6ED9EBA1ul, 0x8F1BBCDCul, 0xCA62C1D6ul
	};

	sl_uint32 W[80];
	sl_uint32 v[5];
	sl_uint32 i;
	for (i = 0; i < 16; i++) {
		W[i] = MIO::readUint32BE(input + (i << 2));
	}
	for (i = 16; i < 80; i++) {
		W[i] = Math::rotateLeft32(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
	}
	for (i = 0; i < 5; i++) {
		v[i] = h[i];
	}
	sl_uint32 f[4];
	for (i = 0; i < 80; i++) {
		sl_uint32 j = i / 20;
		f[0] = v[3] ^ (v[1] & (v[2] ^ v[3]));
		f[1] = v[1] ^ v[2] ^ v[3];
		f[2] = (v[1] & v[2]) | (v[3] & (v[1] | v[2]));
		f[3] = f[1];
		sl_uint32 t = Math::rotateLeft32(v[0], 5) + f[j] + v[4] + K[j] + W[i];
		v[4] = v[3];
		v[3] = v[2];
		v[2] = Math::rotateLeft32(v[1], 30);
		v[1] = v[0];
		v[0] = t;
	}
	for (i = 0; i < 5; i++) {
		h[i] += v[i];
	}
}
SLIB_CRYPTO_NAMESPACE_END

