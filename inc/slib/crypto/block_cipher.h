#ifndef CHECKHEADER_SLIB_CRYPTO_BLOCKCIPHER
#define CHECKHEADER_SLIB_CRYPTO_BLOCKCIPHER

#include "definition.h"

#include "../core/mio.h"
#include "../core/base.h"
#include "../core/math.h"

#define SLIB_CRYPTO_BLOCK_CIPHER_BLOCK_MAX_LEN 128

SLIB_CRYPTO_NAMESPACE_BEGIN

/*
	Defines padding method described in PKCS#5, PKCS#7.

	Padding is added at the end of the message as following.

	01
	02 02
	03 03 03
	04 04 04 04
	05 05 05 05 05

	only applicable up to 256-bytes
*/
class SLIB_EXPORT BlockCipherPadding_PKCS7
{
public:
	SLIB_INLINE static void addPadding(void* buf, sl_size padding)
	{
		sl_uint8* c = (sl_uint8*)buf;
		sl_uint8 n = (sl_uint8)padding;
		for (sl_uint8 i = 0; i < n; i++) {
			c[i] = n;
		}
	}

	SLIB_INLINE static sl_uint32 removePadding(const void* buf, sl_uint32 blockSize)
	{
		sl_uint8* c = (sl_uint8*)buf;
		sl_uint8 n = c[blockSize - 1];
		for (sl_uint32 i = blockSize - n; i < blockSize - 1; i++) {
			if (c[i] != n) {
				return 0;
			}
		}
		if (blockSize < n) {
			return 0;
		}
		return n;
	}
};


template <class BlockCipher>
class SLIB_EXPORT BlockCipher_Blocks
{
public:
	// Output Size = (size / block) * block
	static sl_size encryptBlocks(const BlockCipher* crypto, const void* _src, void* _dst, sl_size size)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		sl_uint32 block = crypto->getBlockSize();
		if (size % block != 0) {
			return 0;
		}
		sl_size n = size / block;
		for (sl_size i = 0; i < n; i++) {
			crypto->encryptBlock(src, dst);
			src += block;
			dst += block;
		}
		return size;
	}

	// Output Size = (size / block) * block
	static sl_size decryptBlocks(const BlockCipher* crypto, const void* _src, void* _dst, sl_size size)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		sl_uint32 block = crypto->getBlockSize();
		if (size % block != 0) {
			return 0;
		}
		sl_size n = size / block;
		for (sl_size i = 0; i < n; i++) {
			crypto->decryptBlock(src, dst);
			src += block;
			dst += block;
		}
		return size;
	}
};

/*
	Electronic codebook (ECB)
*/
template <class BlockCipher, class Padding>
class SLIB_EXPORT BlockCipher_ECB
{
public:
	// Output Size = (size / block + 1) * block (< size + block)
	static sl_size encrypt(const BlockCipher* crypto, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		sl_uint32 block = crypto->getBlockSize();
		if (block > 256) {
			return 0;
		}
		sl_size n = size / block;
		for (sl_size i = 0; i < n; i++) {
			crypto->encryptBlock(src, dst);
			src += block;
			dst += block;
		}
		char last[256];
		sl_size p = n * block;
		sl_uint32 m = (sl_uint32)(size - p);
		Base::copyMemory(last, src, m);
		Padding::addPadding(last + m, block - m);
		crypto->encryptBlock(last, dst);
		return p + block;
	}

	// destination buffer size must equals to or greater than size
	static sl_size decrypt(const BlockCipher* crypto, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		sl_uint32 block = crypto->getBlockSize();
		if (block > 256) {
			return 0;
		}
		if (size % block != 0) {
			return 0;
		}
		sl_size n = size / block;
		for (sl_size i = 0; i < n; i++) {
			crypto->decryptBlock(src, dst);
			src += block;
			dst += block;
		}
		sl_uint32 padding = Padding::removePadding(dst - block, block);
		if (padding > 0) {
			return size - padding;
		} else {
			return 0;
		}
	}
};

/*
	Cipher-block chaining (CBC)
*/
template <class BlockCipher, class Padding>
class SLIB_EXPORT BlockCipher_CBC
{
public:
	// Output Size = (size / block + 1) * block (< size + block)
	static sl_size encrypt(const BlockCipher* crypto, const void* _iv, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		const char* iv = (const char*)_iv;
		sl_uint32 block = crypto->getBlockSize();
		if (block > 256) {
			return 0;
		}
		sl_size n = size / block;
		char msg[256];
		for (sl_size i = 0; i < n; i++) {
			for (sl_uint32 k = 0; k < block; k++) {
				msg[k] = src[k] ^ iv[k];
			}
			crypto->encryptBlock(msg, dst);
			iv = dst;
			src += block;
			dst += block;
		}
		sl_size p = n * block;
		{
			sl_uint32 m = (sl_uint32)(size - p);
			for (sl_uint32 k = 0; k < m; k++) {
				msg[k] = src[k] ^ iv[k];
			}
			Padding::addPadding(msg + m, block - m);
			for (sl_uint32 k = m; k < block; k++) {
				msg[k] = msg[k] ^ iv[k];
			}
			crypto->encryptBlock(msg, dst);
		}
		return p + block;
	}

	// Output Size = (size / block + 2) * block (< size + block*2)
	static sl_size encrypt(const BlockCipher* crypto, const void* src, sl_size size, void* dst)
	{
		sl_uint32 block = crypto->getBlockSize();
		Math::randomMemory(dst, block);
		return encrypt(crypto, dst, src, size, ((char*)dst) + block) + block;
	}

	// destination buffer size must equals to or greater than size
	static sl_size decrypt(const BlockCipher* crypto, const void* _iv, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		const char* iv = (const char*)_iv;
		sl_uint32 block = crypto->getBlockSize();
		if (block > 256) {
			return 0;
		}
		if (size % block != 0) {
			return 0;
		}
		sl_size n = size / block;
		for (sl_size i = 0; i < n; i++) {
			crypto->decryptBlock(src, dst);
			for (sl_uint32 k = 0; k < block; k++) {
				dst[k] ^= iv[k];
			}
			iv = src;
			src += block;
			dst += block;
		}
		sl_uint32 padding = Padding::removePadding(dst - block, block);
		if (padding > 0) {
			return size - padding;
		} else {
			return 0;
		}
	}

	// destination buffer size must equals to or greater than size
	static sl_size decrypt(const BlockCipher* crypto, const void* src, sl_size size, void* dst)
	{
		sl_uint32 block = crypto->getBlockSize();
		if (size < (sl_size)block) {
			return 0;
		}
		return decrypt(crypto, src, ((char*)src) + block, size - block, dst);
	}
};

/*
	Counter Mode (CTR)
*/

template <class BlockCipher>
class SLIB_EXPORT BlockCipher_CTR
{
public:
	static sl_size encrypt(const BlockCipher* crypto, const void* _input, sl_size _size, void* _output, void* _counter, sl_uint32 offset)
	{
		if (_size == 0) {
			return 0;
		}
		sl_uint8 mask[SLIB_CRYPTO_BLOCK_CIPHER_BLOCK_MAX_LEN];

		sl_uint32 sizeBlock = crypto->getBlockSize();
		if (sizeBlock > SLIB_CRYPTO_BLOCK_CIPHER_BLOCK_MAX_LEN || offset > sizeBlock) {
			return 0;
		}

		sl_uint8* counter = (sl_uint8*)(_counter);
		const sl_uint8* input = (const sl_uint8*)_input;
		sl_uint8* output = (sl_uint8*)_output;
		sl_size size = _size;
		sl_size i, n;

		if (offset) {
			crypto->encryptBlock(counter, mask);
			n = sizeBlock - offset;
			if (size > n) {
				for (i = 0; i < n; i++) {
					output[i] = input[i] ^ mask[i + offset];
				}
				size -= n;
				input += n;
				output += n;
				MIO::increaseBE(counter, sizeBlock);
			} else {
				for (i = 0; i < size; i++) {
					output[i] = input[i] ^ mask[i + offset];
				}
				return size;
			}
		}
		while (size > 0) {
			crypto->encryptBlock(counter, mask);
			n = SLIB_MIN(sizeBlock, size);
			for (i = 0; i < n; i++) {
				output[i] = input[i] ^ mask[i];
			}
			size -= n;
			input += n;
			output += n;
			MIO::increaseBE(counter, sizeBlock);
		}
		return _size;
	}

	SLIB_INLINE static sl_size encrypt(const BlockCipher* crypto, const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output)
	{
		if (size == 0) {
			return 0;
		}
		sl_uint32 sizeBlock = crypto->getBlockSize();
		if (sizeBlock < 16) {
			return 0;
		}
		sl_uint8 IV[SLIB_CRYPTO_BLOCK_CIPHER_BLOCK_MAX_LEN];
		Base::copyMemory(IV, iv, sizeBlock - 8);
		MIO::writeUint64BE(IV + sizeBlock - 8, counter);
		return encrypt(crypto, input, size, output, IV, offset);
	}

	SLIB_INLINE static sl_size encrypt(const BlockCipher* crypto, const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output)
	{
		sl_uint32 sizeBlock = crypto->getBlockSize();
		if (sizeBlock < 16) {
			return 0;
		}
		return encrypt(crypto, iv, pos / sizeBlock, (sl_uint32)(pos % sizeBlock), input, size, output);
	}

};

#define _SLIB_BLOCKCIPHER_COMMON_BODY(CLASS) \
	sl_size encryptBlocks(const void* src, void* dst, sl_size size) const \
	{ return BlockCipher_Blocks<CLASS>::encryptBlocks(this, src, dst, size); } \
	sl_size decryptBlocks(const void* src, void* dst, sl_size size) const \
	{ return BlockCipher_Blocks<CLASS>::decryptBlocks(this, src, dst, size); } \
	sl_size encrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, src, size, dst); } \
	sl_size decrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, src, size, dst); } \
	sl_size encrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, iv, src, size, dst); } \
	sl_size decrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, iv, src, size, dst); } \
	sl_size encrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, src, size, dst); } \
	sl_size decrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, src, size, dst); } \
	sl_size encrypt_CTR(const void* input, sl_size size, void* output, void* counter, sl_uint32 offset = 0) const \
	{ return BlockCipher_CTR<CLASS>::encrypt(this, input, size, output, counter, offset); } \
	sl_size encrypt_CTR(const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output) const \
	{ return BlockCipher_CTR<CLASS>::encrypt(this, iv, counter, offset, input, size, output); } \
	sl_size encrypt_CTR(const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output) const \
	{ return BlockCipher_CTR<CLASS>::encrypt(this, iv, pos, input, size, output); }

SLIB_CRYPTO_NAMESPACE_END

#endif
