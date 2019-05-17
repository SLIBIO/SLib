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

#ifndef CHECKHEADER_SLIB_CRYPTO_OPENSSL
#define CHECKHEADER_SLIB_CRYPTO_OPENSSL

#include "definition.h"

#include "tls.h"
#include "rsa.h"
#include "ecc.h"
#include "block_cipher.h"

struct ssl_ctx_st;
struct ssl_st;

namespace slib
{
	
	class SLIB_EXPORT OpenSSL_Context : public TlsContext
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		OpenSSL_Context();
		
		~OpenSSL_Context();
		
	public:
		virtual ssl_ctx_st* getContext() = 0;
		
	};
	
	class SLIB_EXPORT OpenSSL_AsyncStream : public TlsAsyncStream
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		OpenSSL_AsyncStream();
		
		~OpenSSL_AsyncStream();
		
	public:
		virtual ssl_st* getSSL() = 0;

	};
	
	class SLIB_EXPORT OpenSSL_AES : public BlockCipher<OpenSSL_AES>
	{
	public:
		enum {
			BlockSize = 16
		};

	public:
		OpenSSL_AES();
		
		~OpenSSL_AES();
		
	public:
		sl_bool setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);
		
		void setKey_SHA256(const String& key);
		
		// 128 bits (16 bytes) block
		void encryptBlock(const void* src, void* dst) const;
		
		// 128 bits (16 bytes) block
		void decryptBlock(const void* src, void* dst) const;

	private:
		void* m_keyEnc;
		void* m_keyDec;
		
	};
	
	class SLIB_EXPORT OpenSSL_ChaCha20
	{
	public:
		OpenSSL_ChaCha20();
		
		~OpenSSL_ChaCha20();
		
	public:
		// key: 32 bytes (256 bits)
		void setKey(const void* key);
		
		void generateBlock(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3, void* output);

		void start(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3);
		
		// IV: 12 bytes
		void start(const void* IV, sl_uint32 counter = 0);
		
		void encrypt(const void* src, void* dst, sl_size len);
		
		void decrypt(const void* src, void* dst, sl_size len);
		
	private:
		sl_uint32 m_key[8];
		sl_uint32 m_nonce[4];
		sl_uint8 m_lastBlock[64];
		sl_uint32 m_pos;
		
	};
	
	class SLIB_EXPORT OpenSSL_Poly1305
	{
	public:
		OpenSSL_Poly1305();
		
		~OpenSSL_Poly1305();
		
	public:
		// key: 32 bytes (256 bits)
		void start(const void* key);
		
		void update(const void* input, sl_size n);
		
		// output: 16 bytes (128 bits)
		void finish(void* output);
		
		// key: 32 bytes (256 bits)
		// output: 16 bytes (128 bits)
		static void execute(const void* key, const void* message, sl_size lenMessage, void* output);
		
	private:
		void* m_context;
		
	};
	
	class SLIB_EXPORT OpenSSL_ChaCha20_Poly1305
	{
	public:
		OpenSSL_ChaCha20_Poly1305();
		
		~OpenSSL_ChaCha20_Poly1305();
		
	public:
		// key: 32 bytes (256 bits)
		void setKey(const void* key);
		
		// IV: 8 bytes (64 bits)
		void start(sl_uint32 senderId, const void* IV);
		
		// put on AAD (additional authenticated data)
		void putAAD(const void* data, sl_size len);
		
		void finishAAD();
		
		void encrypt(const void* src, void* dst, sl_size len);
		
		void decrypt(const void* src, void* dst, sl_size len);
		
#ifdef check
#undef check
#endif
		// src: cipher text
		void check(const void* src, sl_size len);
		
		// outputTag: 16 bytes (128 bits)
		void finish(void* outputTag);
		
		// tag: 16 bytes (128 bits)
		sl_bool finishAndCheckTag(const void* tag);
		
		// IV: 8 bytes (64 bits)
		// outputTag: 16 bytes (128 bits)
		void encrypt(sl_uint32 senderId, const void* IV, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, void* outputTag);
		
		// IV: 8 bytes (64 bits)
		// tag: 16 bytes (128 bits)
		sl_bool decrypt(sl_uint32 senderId, const void* IV, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, const void* tag);
		
		// IV: 8 bytes (64 bits)
		// tag: 16 bytes (128 bits)
		sl_bool check(sl_uint32 senderId, const void* IV, const void* AAD, sl_size lenAAD, const void* src, sl_size len, const void* tag);
		
	private:
		OpenSSL_ChaCha20 m_cipher;
		OpenSSL_Poly1305 m_auth;
		sl_size m_lenAAD;
		sl_size m_lenInput;
		
	};
	
	class SLIB_EXPORT OpenSSL
	{
	public:
		static Ref<OpenSSL_Context> createContext(const TlsContextParam& param);
		
		static Ref<OpenSSL_AsyncStream> connectStream(const Ref<AsyncStream>& baseStream, const TlsConnectStreamParam& param);
		
		static Ref<OpenSSL_AsyncStream> acceptStream(const Ref<AsyncStream>& baseStream, const TlsAcceptStreamParam& param);
		
		
		static sl_bool isProbablePrime(const void* num_BigEndian, sl_uint32 nBytes, sl_bool* pFlagError = sl_null);
		
		// returns value in Big-endian form
		static Memory generatePrime(sl_uint32 nBits);
		
		static sl_bool randomBytes(void* bytes, sl_uint32 nBytes, sl_bool flagPrivate = sl_false);
		
		
		static void generateRSA(RSAPrivateKey& _output, sl_uint32 nBits);
		
		
		static sl_bool generate_ECKey_secp256k1(ECPrivateKey& _output);
		
		static ECDSA_Signature sign_ECDSA_secp256k1(const ECPrivateKey& key, const BigInt& z);
		
		static ECDSA_Signature sign_ECDSA_SHA256_secp256k1(const ECPrivateKey& key, const void* data, sl_size size);
		
		static sl_bool verify_ECDSA_secp256k1(const ECPublicKey& key, const BigInt& z, const ECDSA_Signature& signature);
		
		static sl_bool verify_ECDSA_SHA256_secp256k1(const ECPublicKey& key, const void* data, sl_size size, const ECDSA_Signature& signature);
		
		static BigInt getSharedKey_ECDH_secp256k1(const ECPrivateKey& keyLocal, const ECPublicKey& keyRemote);

	};

}

#endif
