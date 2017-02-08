#ifndef CHECKHEADER_SLIB_CRYPTO_AES
#define CHECKHEADER_SLIB_CRYPTO_AES

#include "definition.h"

#include "../core/string.h"

#include "block_cipher.h"
#include "gcm.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

/*
	AES - Advanced Encryption Standard

	User Key Size - 128 bits (16 bytes), 192 bits (24 bytes), 256 bits (32 bytes)
	Block Size - 128 bits (16 bytes)
*/

class SLIB_EXPORT AES : public Object
{
public:
	AES();
	
	~AES();
	
public:
	static sl_uint32 getBlockSize();
	
	sl_bool setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);
	
	void setKey_SHA256(const String& key);
	
	// 128 bit (16 byte) block
	void encryptBlock(const void* src, void* dst) const;
	
	// 128 bit (16 byte) block
	void decryptBlock(const void* src, void* dst) const;
	
public: /* common functions for block ciphers */
	sl_size encryptBlocks(const void* src, void* dst, sl_size size) const;
	
	sl_size decryptBlocks(const void* src, void* dst, sl_size size) const;
	
	sl_size encrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const;
	
	sl_size decrypt_ECB_PKCS7Padding(const void* src, sl_size size, void* dst) const;
	
	sl_size encrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const;
	
	sl_size decrypt_CBC_PKCS7Padding(const void* iv, const void* src, sl_size size, void* dst) const;
	
	sl_size encrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const;
	
	sl_size decrypt_CBC_PKCS7Padding(const void* src, sl_size size, void* dst) const;
	
	Memory encrypt_CBC_PKCS7Padding(const void* src, sl_size size) const;
	
	Memory decrypt_CBC_PKCS7Padding(const void* src, sl_size size) const;
	
	Memory encrypt_CBC_PKCS7Padding(const Memory& mem) const;
	
	Memory decrypt_CBC_PKCS7Padding(const Memory& mem) const;
	
	sl_size encrypt_CTR(const void* input, sl_size size, void* output, void* counter, sl_uint32 offset = 0) const;
	
	sl_size encrypt_CTR(const void* iv, sl_uint64 counter, sl_uint32 offset, const void* input, sl_size size, void* output) const;
	
	sl_size encrypt_CTR(const void* iv, sl_uint64 pos, const void* input, sl_size size, void* output) const;
	
private:
	sl_uint32 m_roundKeyEnc[64];
	sl_uint32 m_roundKeyDec[64];
	sl_uint32 m_nCountRounds;
	
};

class SLIB_EXPORT AES_GCM : public Object, public GCM<AES>
{
public:
	AES_GCM();
	
	~AES_GCM();
	
public:
	void setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);

	void setKey_SHA256(const String& key);
	
private:
	AES m_cipher;
	
};

SLIB_CRYPTO_NAMESPACE_END

#endif
