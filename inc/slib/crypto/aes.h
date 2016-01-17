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
	sl_bool setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */);
	
	void setKey_SHA256(const String& key);

	// Common Block Cipher Functions
	SLIB_INLINE static sl_uint32 getBlockSize()
	{
		return 16;
	}
	
	// 128 bit (16 byte) block
	void encryptBlock(const void* src, void* dst) const;
	
	// 128 bit (16 byte) block
	void decryptBlock(const void* src, void* dst) const;

	_SLIB_BLOCKCIPHER_COMMON_BODY(AES)

private:
	sl_uint32 m_roundKeyEnc[64];
	sl_uint32 m_roundKeyDec[64];
	sl_uint32 m_nCountRounds;
	
};

class SLIB_EXPORT AES_GCM : public Object, public BlockCipher_GCM<AES>
{
public:
	SLIB_INLINE void setKey(const void* key, sl_uint32 lenKey /* 16, 24, 32 bytes */)
	{
		m_cipher.setKey(key, lenKey);
		setCipher(&m_cipher);
	}

	SLIB_INLINE void setKey_SHA256(const String& key)
	{
		m_cipher.setKey_SHA256(key);
		setCipher(&m_cipher);
	}
	
private:
	AES m_cipher;
};

SLIB_CRYPTO_NAMESPACE_END

#endif
