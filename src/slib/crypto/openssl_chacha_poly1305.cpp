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

#include "slib/crypto/openssl.h"

#include "openssl/evp.h"

extern "C"
{
	void ChaCha20_ctr32(unsigned char *out, const unsigned char *inp, size_t len, const unsigned int key[8], const unsigned int counter[4]);
	
	struct poly1305_context;
	typedef struct poly1305_context POLY1305;
	
	size_t Poly1305_ctx_size(void);
	void Poly1305_Init(POLY1305 *ctx, const unsigned char key[32]);
	void Poly1305_Update(POLY1305 *ctx, const unsigned char *inp, size_t len);
	void Poly1305_Final(POLY1305 *ctx, unsigned char mac[16]);
}

namespace slib
{

	OpenSSL_ChaCha20::OpenSSL_ChaCha20(): m_pos(0)
	{
	}
	
	OpenSSL_ChaCha20::~OpenSSL_ChaCha20()
	{
	}
	
	void OpenSSL_ChaCha20::setKey(const void* _key)
	{
		const sl_uint8* key = (const sl_uint8*)_key;
		sl_uint32* input = m_key;
		for (sl_uint32 i = 0; i < 8; i++) {
			sl_uint32 j = i << 2;
			input[i] = SLIB_MAKE_DWORD(key[j+3], key[j+2], key[j+1], key[j]);
		}
	}
	
	void OpenSSL_ChaCha20::generateBlock(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3, void* output)
	{
		sl_uint32 nonces[] = {nonce0, nonce1, nonce2, nonce3};
		Base::zeroMemory(output, 64);
		ChaCha20_ctr32((unsigned char*)output, (const unsigned char*)output, 64, (const unsigned int*)m_key, (const unsigned int*)nonces);
	}
	
	void OpenSSL_ChaCha20::start(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3)
	{
		m_nonce[0] = nonce0;
		m_nonce[1] = nonce1;
		m_nonce[2] = nonce2;
		m_nonce[3] = nonce3;
		m_pos = 0;
	}
	
	void OpenSSL_ChaCha20::start(const void* _iv, sl_uint32 counter)
	{
		const sl_uint8* iv = (const sl_uint8*)_iv;
		m_nonce[0] = counter;
		m_nonce[1] = SLIB_MAKE_DWORD(iv[3], iv[2], iv[1], iv[0]);
		m_nonce[2] = SLIB_MAKE_DWORD(iv[7], iv[6], iv[5], iv[4]);
		m_nonce[3] = SLIB_MAKE_DWORD(iv[11], iv[10], iv[9], iv[8]);
		m_pos = 0;
	}
	
	void OpenSSL_ChaCha20::encrypt(const void* _src, void* _dst, sl_size len)
	{
		if (!len) {
			return;
		}
		sl_uint8* lastBlock = m_lastBlock;
		const sl_uint8* src = (const sl_uint8*)_src;
		sl_uint8* dst = (sl_uint8*)_dst;
		sl_uint32 pos = m_pos;
		if (pos) {
			sl_uint32 remain = 64 - pos;
			if (len < remain) {
				sl_uint32 _len = (sl_uint32)len;
				for (sl_uint32 i = 0; i < _len; i++) {
					dst[i] = src[i] ^ lastBlock[pos + i];
				}
				m_pos += _len;
				return;
			} else {
				for (sl_uint32 i = 0; i < remain; i++) {
					dst[i] = src[i] ^ lastBlock[pos + i];
				}
				len -= remain;
				src += remain;
				dst += remain;
				m_pos = 0;
			}
		}
		sl_size nBlocks = len >> 6;
		sl_size nBytes = nBlocks << 6;
		if (nBlocks) {
			ChaCha20_ctr32((unsigned char*)dst, (const unsigned char*)src, (size_t)(nBytes), (const unsigned int*)m_key, (const unsigned int*)m_nonce);
			m_nonce[0] += (sl_uint32)nBlocks;
		}
		pos = (sl_uint32)(len & 0x3F);
		if (pos) {
			src += nBytes;
			dst += nBytes;
			Base::zeroMemory(lastBlock, 64);
			ChaCha20_ctr32((unsigned char*)lastBlock, (const unsigned char*)lastBlock, 64, (const unsigned int*)m_key, (const unsigned int*)m_nonce);
			m_nonce[0]++;
			for (sl_uint32 i = 0; i < pos; i++) {
				dst[i] = lastBlock[i] ^ src[i];
			}
			m_pos = pos;
		}
	}
	
	void OpenSSL_ChaCha20::decrypt(const void* src, void* dst, sl_size len)
	{
		encrypt(src, dst, len);
	}
	
	
	OpenSSL_Poly1305::OpenSSL_Poly1305(): m_context(sl_null)
	{
	}
	
	OpenSSL_Poly1305::~OpenSSL_Poly1305()
	{
		if (m_context) {
			Base::freeMemory(m_context);
		}
	}
	
	void OpenSSL_Poly1305::start(const void* key)
	{
		if (!m_context) {
			m_context = Base::createMemory((sl_size)(Poly1305_ctx_size()));
		}
		Poly1305_Init((POLY1305*)m_context, (const unsigned char*)key);
	}
	
	void OpenSSL_Poly1305::update(const void* input, sl_size n)
	{
		Poly1305_Update((POLY1305*)m_context, (const unsigned char*)input, (size_t)n);
	}
	
	void OpenSSL_Poly1305::finish(void* output)
	{
		Poly1305_Final((POLY1305*)m_context, (unsigned char*)output);
	}
	
	void OpenSSL_Poly1305::execute(const void* key, const void* message, sl_size lenMessage, void* output)
	{
		OpenSSL_Poly1305 p;
		p.start(key);
		p.update(message, lenMessage);
		p.finish(output);
	}
	
	
	OpenSSL_ChaCha20_Poly1305::OpenSSL_ChaCha20_Poly1305()
	{
	}
	
	OpenSSL_ChaCha20_Poly1305::~OpenSSL_ChaCha20_Poly1305()
	{
	}
	
	void OpenSSL_ChaCha20_Poly1305::setKey(const void* key)
	{
		m_cipher.setKey(key);
	}
	
	void OpenSSL_ChaCha20_Poly1305::start(sl_uint32 senderId, const void* _iv)
	{
		const sl_uint8* iv = (const sl_uint8*)_iv;
		sl_uint32 n0 = SLIB_MAKE_DWORD(iv[3], iv[2], iv[1], iv[0]);
		sl_uint32 n1 = SLIB_MAKE_DWORD(iv[7], iv[6], iv[5], iv[4]);
		m_cipher.start(1, senderId, n0, n1);
		sl_uint8 block0[64];
		m_cipher.generateBlock(0, senderId, n0, n1, block0);
		m_auth.start(block0); // use first 32 bytes
		m_lenAAD = 0;
		m_lenInput = 0;
	}
	
	void OpenSSL_ChaCha20_Poly1305::putAAD(const void* data, sl_size len)
	{
		m_auth.update(data, len);
		m_lenAAD += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::finishAAD()
	{
		sl_uint32 n = (sl_uint32)(m_lenAAD & 15);
		if (n) {
			static const sl_uint8 zeros[16] = {0};
			m_auth.update(zeros, 16 - n);
		}
	}
	
	void OpenSSL_ChaCha20_Poly1305::encrypt(const void* src, void* dst, sl_size len)
	{
		if (!len) {
			return;
		}
		m_cipher.encrypt(src, dst, len);
		m_auth.update(dst, len);
		m_lenInput += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::decrypt(const void* src, void* dst, sl_size len)
	{
		if (!len) {
			return;
		}
		m_auth.update(src, len);
		m_cipher.encrypt(src, dst, len);
		m_lenInput += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::check(const void* src, sl_size len)
	{
		m_auth.update(src, len);
		m_lenInput += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::finish(void* outputTag)
	{
		sl_uint32 n = (sl_uint32)(m_lenInput & 15);
		if (n) {
			static const sl_uint8 zeros[16] = {0};
			m_auth.update(zeros, 16 - n);
		}
		sl_uint8 len[16];
		MIO::writeUint64LE(len, m_lenAAD);
		MIO::writeUint64LE(len + 8, m_lenInput);
		m_auth.update(len, 16);
		m_auth.finish(outputTag);
	}
	
	sl_bool OpenSSL_ChaCha20_Poly1305::finishAndCheckTag(const void* _tag)
	{
		const sl_uint8* tag = (const sl_uint8*)_tag;
		sl_uint8 outputTag[16];
		finish(outputTag);
		sl_uint32 n = 0;
		for (sl_uint32 i = 0; i < 16; i++) {
			n |= (outputTag[i] ^ tag[i]);
		}
		return n == 0;
	}
	
	void OpenSSL_ChaCha20_Poly1305::encrypt(sl_uint32 senderId, const void* iv, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, void* outputTag)
	{
		start(senderId, iv);
		if (lenAAD) {
			putAAD(AAD, lenAAD);
			finishAAD();
		}
		if (len) {
			encrypt(src, dst, len);
		}
		finish(outputTag);
	}
	
	sl_bool OpenSSL_ChaCha20_Poly1305::decrypt(sl_uint32 senderId, const void* iv, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, const void* tag)
	{
		start(senderId, iv);
		if (lenAAD) {
			putAAD(AAD, lenAAD);
			finishAAD();
		}
		if (len) {
			decrypt(src, dst, len);
		}
		return finishAndCheckTag(tag);
	}
	
	sl_bool OpenSSL_ChaCha20_Poly1305::check(sl_uint32 senderId, const void* iv, const void* AAD, sl_size lenAAD, const void* src, sl_size len, const void* tag)
	{
		start(senderId, iv);
		if (lenAAD) {
			putAAD(AAD, lenAAD);
			finishAAD();
		}
		if (len) {
			check(src, len);
		}
		return finishAndCheckTag(tag);
	}
	
}
