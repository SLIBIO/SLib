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

namespace slib
{

/**************************************
		BlockCipher_ECB
 
	Electronic codebook (ECB)
***************************************/

	// Output Size = (size / block + 1) * block (< size + block)
	template <class CLASS, class PADDING>
	sl_size BlockCipher_ECB<CLASS, PADDING>::encrypt(const CLASS* crypto, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		sl_size n = size / CLASS::BlockSize;
		for (sl_size i = 0; i < n; i++) {
			crypto->encryptBlock(src, dst);
			src += CLASS::BlockSize;
			dst += CLASS::BlockSize;
		}
		char last[CLASS::BlockSize];
		sl_size p = n * CLASS::BlockSize;
		sl_uint32 m = (sl_uint32)(size - p);
		Base::copyMemory(last, src, m);
		PADDING::addPadding(last + m, CLASS::BlockSize - m);
		crypto->encryptBlock(last, dst);
		return p + CLASS::BlockSize;
	}

	template <class CLASS, class PADDING>
	Memory BlockCipher_ECB<CLASS, PADDING>::encrypt(const CLASS* crypto, const void* src, sl_size size)
	{
		Memory mem = Memory::create(size + CLASS::BlockSize);
		if (mem.isNotNull()) {
			sl_size n = encrypt(crypto, src, size, mem.getData());
			if (n) {
				return mem.sub(0, n);
			}
		}
		return sl_null;
	}

	// destination buffer size must equals to or greater than size
	template <class CLASS, class PADDING>
	sl_size BlockCipher_ECB<CLASS, PADDING>::decrypt(const CLASS* crypto, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		if (size % CLASS::BlockSize != 0) {
			return 0;
		}
		sl_size n = size / CLASS::BlockSize;
		for (sl_size i = 0; i < n; i++) {
			crypto->decryptBlock(src, dst);
			src += CLASS::BlockSize;
			dst += CLASS::BlockSize;
		}
		sl_uint32 padding = PADDING::removePadding(dst - CLASS::BlockSize, CLASS::BlockSize);
		if (padding > 0) {
			return size - padding;
		} else {
			return 0;
		}
	}

	template <class CLASS, class PADDING>
	Memory BlockCipher_ECB<CLASS, PADDING>::decrypt(const CLASS* crypto, const void* src, sl_size size)
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
		BlockCipher_CBC
 
	Cipher-block chaining (CBC)
***************************************/

	template <class CLASS, class PADDING>
	sl_size BlockCipher_CBC<CLASS, PADDING>::encrypt(const CLASS* crypto, const void* _iv, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		const char* iv = (const char*)_iv;
		sl_size n = size / CLASS::BlockSize;
		char msg[256];
		for (sl_size i = 0; i < n; i++) {
			for (sl_uint32 k = 0; k < CLASS::BlockSize; k++) {
				msg[k] = src[k] ^ iv[k];
			}
			crypto->encryptBlock(msg, dst);
			iv = dst;
			src += CLASS::BlockSize;
			dst += CLASS::BlockSize;
		}
		sl_size p = n * CLASS::BlockSize;
		{
			sl_uint32 m = (sl_uint32)(size - p);
			for (sl_uint32 k = 0; k < m; k++) {
				msg[k] = src[k] ^ iv[k];
			}
			PADDING::addPadding(msg + m, CLASS::BlockSize - m);
			for (sl_uint32 k = m; k < CLASS::BlockSize; k++) {
				msg[k] = msg[k] ^ iv[k];
			}
			crypto->encryptBlock(msg, dst);
		}
		return p + CLASS::BlockSize;
	}

	template <class CLASS, class PADDING>
	Memory BlockCipher_CBC<CLASS, PADDING>::encrypt(const CLASS* crypto, const void* iv, const void* src, sl_size size)
	{
		Memory mem = Memory::create(size + CLASS::BlockSize);
		if (mem.isNotNull()) {
			sl_size n = encrypt(crypto, iv, src, size, mem.getData());
			if (n) {
				return mem.sub(0, n);
			}
		}
		return sl_null;
	}

	// Output Size = (size / block + 2) * block (< size + block*2)
	template <class CLASS, class PADDING>
	sl_size BlockCipher_CBC<CLASS, PADDING>::encrypt(const CLASS* crypto, const void* src, sl_size size, void* dst)
	{
		Math::randomMemory(dst, CLASS::BlockSize);
		return encrypt(crypto, dst, src, size, ((char*)dst) + CLASS::BlockSize) + CLASS::BlockSize;
	}

	template <class CLASS, class PADDING>
	Memory BlockCipher_CBC<CLASS, PADDING>::encrypt(const CLASS* crypto, const void* src, sl_size size)
	{
		Memory mem = Memory::create(size + CLASS::BlockSize * 2);
		if (mem.isNotNull()) {
			sl_size n = encrypt(crypto, src, size, mem.getData());
			if (n) {
				return mem.sub(0, n);
			}
		}
		return sl_null;
	}

	// destination buffer size must equals to or greater than size
	template <class CLASS, class PADDING>
	sl_size BlockCipher_CBC<CLASS, PADDING>::decrypt(const CLASS* crypto, const void* _iv, const void* _src, sl_size size, void* _dst)
	{
		const char* src = (const char*)(_src);
		char* dst = (char*)(_dst);
		const char* iv = (const char*)_iv;
		if (size % CLASS::BlockSize != 0) {
			return 0;
		}
		sl_size n = size / CLASS::BlockSize;
		for (sl_size i = 0; i < n; i++) {
			crypto->decryptBlock(src, dst);
			for (sl_uint32 k = 0; k < CLASS::BlockSize; k++) {
				dst[k] ^= iv[k];
			}
			iv = src;
			src += CLASS::BlockSize;
			dst += CLASS::BlockSize;
		}
		sl_uint32 padding = PADDING::removePadding(dst - CLASS::BlockSize, CLASS::BlockSize);
		if (padding > 0) {
			return size - padding;
		} else {
			return 0;
		}
	}

	template <class CLASS, class PADDING>
	Memory BlockCipher_CBC<CLASS, PADDING>::decrypt(const CLASS* crypto, const void* iv, const void* src, sl_size size)
	{
		Memory mem = Memory::create(size);
		if (mem.isNotNull()) {
			sl_size n = decrypt(crypto, iv, src, size, mem.getData());
			if (n) {
				return mem.sub(0, n);
			}
		}
		return sl_null;
	}

	// destination buffer size must equals to or greater than size
	template <class CLASS, class PADDING>
	sl_size BlockCipher_CBC<CLASS, PADDING>::decrypt(const CLASS* crypto, const void* src, sl_size size, void* dst)
	{
		if (size < CLASS::BlockSize) {
			return 0;
		}
		return decrypt(crypto, src, ((char*)src) + CLASS::BlockSize, size - CLASS::BlockSize, dst);
	}

	template <class CLASS, class PADDING>
	Memory BlockCipher_CBC<CLASS, PADDING>::decrypt(const CLASS* crypto, const void* src, sl_size size)
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

	template <class CLASS>
	sl_size BlockCipher_CTR<CLASS>::encrypt(const CLASS* crypto, const void* _input, sl_size _size, void* _output, void* _counter, sl_uint32 offset)
	{
		if (_size == 0) {
			return 0;
		}
		sl_uint8 mask[CLASS::BlockSize];
		
		if (offset > CLASS::BlockSize) {
			return 0;
		}
		
		sl_uint8* counter = (sl_uint8*)(_counter);
		const sl_uint8* input = (const sl_uint8*)_input;
		sl_uint8* output = (sl_uint8*)_output;
		sl_size size = _size;
		sl_size i, n;
		
		if (offset) {
			crypto->encryptBlock(counter, mask);
			n = CLASS::BlockSize - offset;
			if (size > n) {
				for (i = 0; i < n; i++) {
					output[i] = input[i] ^ mask[i + offset];
				}
				size -= n;
				input += n;
				output += n;
				MIO::increaseBE(counter, CLASS::BlockSize);
			} else {
				for (i = 0; i < size; i++) {
					output[i] = input[i] ^ mask[i + offset];
				}
				return size;
			}
		}
		while (size > 0) {
			crypto->encryptBlock(counter, mask);
			n = SLIB_MIN(CLASS::BlockSize, size);
			for (i = 0; i < n; i++) {
				output[i] = input[i] ^ mask[i];
			}
			size -= n;
			input += n;
			output += n;
			MIO::increaseBE(counter, CLASS::BlockSize);
		}
		return _size;
	}

	template <class CLASS>
	sl_size BlockCipher_CTR<CLASS>::encrypt(const CLASS* crypto, const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output)
	{
		if (size == 0) {
			return 0;
		}
		if (CLASS::BlockSize < 16) {
			return 0;
		}
		sl_uint8 IV[CLASS::BlockSize];
		Base::copyMemory(IV, iv, CLASS::BlockSize - 8);
		MIO::writeUint64BE(IV + CLASS::BlockSize - 8, counter);
		return encrypt(crypto, input, size, output, IV, offset);
	}

	template <class CLASS>
	sl_size BlockCipher_CTR<CLASS>::encrypt(const CLASS* crypto, const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output)
	{
		if (CLASS::BlockSize < 16) {
			return 0;
		}
		return encrypt(crypto, iv, pos / CLASS::BlockSize, (sl_uint32)(pos % CLASS::BlockSize), input, size, output);
	}

}
