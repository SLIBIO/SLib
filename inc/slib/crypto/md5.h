#ifndef CHECKHEADER_SLIB_CRYPTO_MD5
#define CHECKHEADER_SLIB_CRYPTO_MD5

#include "definition.h"

#include "hash.h"

/*
	MD5 - Message Digest algorithm

	Output: 128bits (16 bytes)
*/

SLIB_CRYPTO_NAMESPACE_BEGIN

class SLIB_EXPORT MD5 : public CryptoHash
{
public:
	SLIB_CRYPTO_DEFINE_HASH(MD5, 16)
	
public:
	MD5();

public:
	// override
	void start();
	
	// override
	void update(const void* input, sl_size n);
	
	// override
	void finish(void* output);

private:
	void _updateSection(const sl_uint8* input);

private:
	sl_size sizeTotalInput;
	sl_uint32 rdata_len;
	sl_uint8 rdata[64];
	sl_uint32 A[4];
};

SLIB_CRYPTO_NAMESPACE_END

#endif
