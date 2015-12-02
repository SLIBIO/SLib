#ifndef CHECKHEADER_SLIB_CRYPTO_SHA2
#define CHECKHEADER_SLIB_CRYPTO_SHA2

#include "definition.h"
#include "hash.h"

/*
	SHA2 - Secure Hash Algorithm

	Output:
		SHA224 - 224bits (28 bytes)
		SHA256 - 256bits (32 bytes)
		SHA384 - 384bits (48 bytes)
		SHA512 - 512bits (64 bytes)
*/

SLIB_CRYPTO_NAMESPACE_BEGIN
class SLIB_EXPORT _SHA256Base : public CryptoHash
{
public:
	_SHA256Base();
	
public:
	void update(const void* input, sl_size n);

protected:
	void _start();
	void _finish();
	void _updateSection(const sl_uint8* input);

protected:
	sl_size sizeTotalInput;
	sl_uint32 rdata_len;
	sl_uint8 rdata[64];
	sl_uint32 h[8];
};

class SLIB_EXPORT SHA224 : public _SHA256Base
{
public:
	SLIB_CRYPTO_DEFINE_HASH(SHA224, 28)
public:
	void start();
	void finish(void* output);
};

class SLIB_EXPORT SHA256 : public _SHA256Base
{
public:
	SLIB_CRYPTO_DEFINE_HASH(SHA256, 32)
public:
	void start();
	void finish(void* output);
	static sl_uint32 make32bitChecksum(const void* input, sl_size n);
};

class SLIB_EXPORT _SHA512Base : public CryptoHash
{
public:
	_SHA512Base();

public:
	void update(const void* input, sl_size n);

protected:
	void _start();
	void _finish();
	void _updateSection(const sl_uint8* input);

protected:
	sl_size sizeTotalInput;
	sl_uint32 rdata_len;
	sl_uint8 rdata[128];
	sl_uint64 h[8];
};

class SLIB_EXPORT SHA384 : public _SHA512Base
{
public:
	SLIB_CRYPTO_DEFINE_HASH(SHA384, 48)
public:
	void start();
	void finish(void* output);
};

class SLIB_EXPORT SHA512 : public _SHA512Base
{
public:
	SLIB_CRYPTO_DEFINE_HASH(SHA512, 64)
public:
	void start();
	void finish(void* output);
};
SLIB_CRYPTO_NAMESPACE_END

#endif
