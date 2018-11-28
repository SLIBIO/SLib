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

#include "slib/crypto/block_cipher.h"

#include "slib/core/mio.h"
#include "slib/core/base.h"
#include "slib/core/math.h"

#include "slib/crypto/aes.h"
#include "slib/crypto/blowfish.h"

namespace slib
{

/*
				BlockCipherPadding_PKCS7
 
	Defines padding method described in PKCS#5, PKCS#7.
 
	Padding is added at the end of the message as following.
 
	01
	02 02
	03 03 03
	04 04 04 04
	05 05 05 05 05
 
	only applicable up to 256-bytes
*/

	void BlockCipherPadding_PKCS7::addPadding(void* buf, sl_size padding)
	{
		sl_uint8* c = (sl_uint8*)buf;
		sl_uint8 n = (sl_uint8)padding;
		for (sl_uint8 i = 0; i < n; i++) {
			c[i] = n;
		}
	}

	sl_uint32 BlockCipherPadding_PKCS7::removePadding(const void* buf, sl_uint32 blockSize)
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


/**************************************
			BlockCipher_Blocks
***************************************/

	// Output Size = (size / block) * block
	template <class BlockCipher>
	sl_size BlockCipher_Blocks<BlockCipher>::encryptBlocks(const BlockCipher* crypto, const void* _src, void* _dst, sl_size size)
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
	template <class BlockCipher>
	sl_size BlockCipher_Blocks<BlockCipher>::decryptBlocks(const BlockCipher* crypto, const void* _src, void* _dst, sl_size size)
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


/**************************************
		BlockCipher_ECB
 
	Electronic codebook (ECB)
***************************************/

	// Output Size = (size / block + 1) * block (< size + block)
	template <class BlockCipher, class Padding>
	sl_size BlockCipher_ECB<BlockCipher, Padding>::encrypt(const BlockCipher* crypto, const void* _src, sl_size size, void* _dst)
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
	template <class BlockCipher, class Padding>
	sl_size BlockCipher_ECB<BlockCipher, Padding>::decrypt(const BlockCipher* crypto, const void* _src, sl_size size, void* _dst)
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


/**************************************
		BlockCipher_CBC
 
	Cipher-block chaining (CBC)
***************************************/

	template <class BlockCipher, class Padding>
	sl_size BlockCipher_CBC<BlockCipher, Padding>::encrypt(const BlockCipher* crypto, const void* _iv, const void* _src, sl_size size, void* _dst)
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
	template <class BlockCipher, class Padding>
	sl_size BlockCipher_CBC<BlockCipher, Padding>::encrypt(const BlockCipher* crypto, const void* src, sl_size size, void* dst)
	{
		sl_uint32 block = crypto->getBlockSize();
		Math::randomMemory(dst, block);
		return encrypt(crypto, dst, src, size, ((char*)dst) + block) + block;
	}

	template <class BlockCipher, class Padding>
	Memory BlockCipher_CBC<BlockCipher, Padding>::encrypt(const BlockCipher* crypto, const void* src, sl_size size)
	{
		sl_uint32 block = crypto->getBlockSize();
		Memory mem = Memory::create(size + block * 2);
		if (mem.isNotNull()) {
			sl_size n = encrypt(crypto, src, size, mem.getData());
			if (n) {
				return mem.sub(0, n);
			}
		}
		return sl_null;
	}

	// destination buffer size must equals to or greater than size
	template <class BlockCipher, class Padding>
	sl_size BlockCipher_CBC<BlockCipher, Padding>::decrypt(const BlockCipher* crypto, const void* _iv, const void* _src, sl_size size, void* _dst)
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
	template <class BlockCipher, class Padding>
	sl_size BlockCipher_CBC<BlockCipher, Padding>::decrypt(const BlockCipher* crypto, const void* src, sl_size size, void* dst)
	{
		sl_uint32 block = crypto->getBlockSize();
		if (size < (sl_size)block) {
			return 0;
		}
		return decrypt(crypto, src, ((char*)src) + block, size - block, dst);
	}

	template <class BlockCipher, class Padding>
	Memory BlockCipher_CBC<BlockCipher, Padding>::decrypt(const BlockCipher* crypto, const void* src, sl_size size)
	{
		Memory mem = Memory::create(size);
		if (mem.isNotNull()) {
			sl_size n = decrypt(crypto, src, size, mem.getData());
			if (n) {
				return mem.sub(0, n);
			}
		}
		return sl_null;
	}


/**************************************
		 BlockCipher_CTR
 
		Counter Mode (CTR)
***************************************/

	template <class BlockCipher>
	sl_size BlockCipher_CTR<BlockCipher>::encrypt(const BlockCipher* crypto, const void* _input, sl_size _size, void* _output, void* _counter, sl_uint32 offset)
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

	template <class BlockCipher>
	sl_size BlockCipher_CTR<BlockCipher>::encrypt(const BlockCipher* crypto, const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output)
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

	template <class BlockCipher>
	sl_size BlockCipher_CTR<BlockCipher>::encrypt(const BlockCipher* crypto, const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output)
	{
		sl_uint32 sizeBlock = crypto->getBlockSize();
		if (sizeBlock < 16) {
			return 0;
		}
		return encrypt(crypto, iv, pos / sizeBlock, (sl_uint32)(pos % sizeBlock), input, size, output);
	}


#define DEFINE_BLOCKCIPHER(CLASS) \
	sl_size CLASS::encryptBlocks(const void* src, void* dst, sl_size size) const \
	{ return BlockCipher_Blocks<CLASS>::encryptBlocks(this, src, dst, size); } \
	sl_size CLASS::decryptBlocks(const void* src, void* dst, sl_size size) const \
	{ return BlockCipher_Blocks<CLASS>::decryptBlocks(this, src, dst, size); } \
	sl_size CLASS::encrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, src, size, dst); } \
	sl_size CLASS::decrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_ECB<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, src, size, dst); } \
	sl_size CLASS::encrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, iv, src, size, dst); } \
	sl_size CLASS::decrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, iv, src, size, dst); } \
	sl_size CLASS::encrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, src, size, dst); } \
	sl_size CLASS::decrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, src, size, dst); } \
	Memory CLASS::encrypt_CBC_PKCS7Padding(const void* src, sl_size size) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, src, size); } \
	Memory CLASS::decrypt_CBC_PKCS7Padding(const void* src, sl_size size) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, src, size); } \
	Memory CLASS::encrypt_CBC_PKCS7Padding(const Memory& mem) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::encrypt(this, mem.getData(), mem.getSize()); } \
	Memory CLASS::decrypt_CBC_PKCS7Padding(const Memory& mem) const \
	{ return BlockCipher_CBC<CLASS, BlockCipherPadding_PKCS7>::decrypt(this, mem.getData(), mem.getSize()); } \
	sl_size CLASS::encrypt_CTR(const void* input, sl_size size, void* output, void* counter, sl_uint32 offset) const \
	{ return BlockCipher_CTR<CLASS>::encrypt(this, input, size, output, counter, offset); } \
	sl_size CLASS::encrypt_CTR(const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output) const \
	{ return BlockCipher_CTR<CLASS>::encrypt(this, iv, counter, offset, input, size, output); } \
	sl_size CLASS::encrypt_CTR(const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output) const \
	{ return BlockCipher_CTR<CLASS>::encrypt(this, iv, pos, input, size, output); }

	DEFINE_BLOCKCIPHER(AES);
	DEFINE_BLOCKCIPHER(Blowfish);

}
