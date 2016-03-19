#ifndef CHECKHEADER_SLIB_CRYPTO_HASH
#define CHECKHEADER_SLIB_CRYPTO_HASH

#include "definition.h"

#include "../core/string.h"
#include "../core/memory.h"
#include "../core/object.h"

/*
	Supported Hash Functions
		MD5, SHA1, SHA2(224, 256, 384, 512)

	Attention: Hash classes are not thread-safe
*/

SLIB_CRYPTO_NAMESPACE_BEGIN

enum class CryptoHashType
{
	MD5 = 0,
	SHA1 = 101,
	SHA224 = 102,
	SHA256 = 103,
	SHA384 = 104,
	SHA512 = 105
};

class SLIB_EXPORT CryptoHash : public Object
{
public:
	static Ref<CryptoHash> create(CryptoHashType type);
	
	static Ref<CryptoHash> md5();
	
	static Ref<CryptoHash> sha1();
	
	static Ref<CryptoHash> sha224();
	
	static Ref<CryptoHash> sha256();
	
	static Ref<CryptoHash> sha384();
	
	static Ref<CryptoHash> sha512();

public:
	virtual sl_uint32 getSize() const = 0;
	
	virtual void start() = 0;
	
	virtual void update(const void* input, sl_size n) = 0;
	
	virtual void finish(void* output) = 0;

	
	void execute(const void* input, sl_size n, void* output);

	void applyMask_MGF1(const void* seed, sl_uint32 sizeSeed, void* target, sl_uint32 sizeTarget);
	
};

SLIB_CRYPTO_NAMESPACE_END

#define SLIB_DECLARE_CRYPTO_HASH \
public: \
	static void hash(const void* input, sl_size n, void* output); \
	static sl_uint32 getHashSize(); \
	static void hash(const String& s, void* output); \
	static void hash(const Memory& data, void* output); \
	static Memory hash(const void* input, sl_size n); \
	static Memory hash(const String& s); \
	static Memory hash(const Memory& data); \
	sl_uint32 getSize() const;

#define SLIB_DEFINE_CRYPTO_HASH(CLASS, HASH_SIZE) \
	void CLASS::hash(const void* input, sl_size n, void* output) \
	{ \
		CLASS h; \
		h.execute(input, n, output); \
	} \
	sl_uint32 CLASS::getHashSize() \
	{ \
		return HASH_SIZE; \
	} \
	void CLASS::hash(const String& s, void* output) \
	{ \
		hash(s.getData(), s.getLength(), output); \
	} \
	void CLASS::hash(const Memory& data, void* output) \
	{ \
		hash(data.getData(), data.getSize(), output); \
	} \
	Memory CLASS::hash(const void* input, sl_size n) \
	{ \
		char v[HASH_SIZE]; \
		hash(input, n, v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	Memory CLASS::hash(const String& s) \
	{ \
		char v[HASH_SIZE]; \
		hash(s.getData(), s.getLength(), v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	Memory CLASS::hash(const Memory& data) \
	{ \
		char v[HASH_SIZE]; \
		hash(data.getData(), data.getSize(), v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	sl_uint32 CLASS::getSize() const \
	{ \
		return getHashSize(); \
	}

#endif
