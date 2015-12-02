#ifndef CHECKHEADER_SLIB_CRYPTO_SHA1
#define CHECKHEADER_SLIB_CRYPTO_SHA1

#include "definition.h"
#include "hash.h"

/*
	SHA1 - Secure Hash Algorithm

	Output: 160bits (20 bytes)
*/

SLIB_CRYPTO_NAMESPACE_BEGIN
class SLIB_EXPORT SHA1 : public CryptoHash
{
public:
	SLIB_CRYPTO_DEFINE_HASH(SHA1, 20)
public:
	SHA1();

public:
	void start();
	void update(const void* input, sl_size n);
	void finish(void* output);

private:
	void _updateSection(const sl_uint8* input);

private:
	sl_size sizeTotalInput;
	sl_uint32 rdata_len;
	sl_uint8 rdata[64];
	sl_uint32 h[5];
};
SLIB_CRYPTO_NAMESPACE_END

#endif
