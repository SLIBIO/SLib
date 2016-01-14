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

class SLIB_EXPORT CryptoHash : public Object
{
public:
	virtual sl_uint32 getSize() const = 0;
	
	virtual void start() = 0;
	
	virtual void update(const void* input, sl_size n) = 0;
	
	virtual void finish(void* output) = 0;

	
	SLIB_INLINE void execute(const void* input, sl_size n, void* output)
	{
		start();
		update(input, n);
		finish(output);
	}

public:
	enum Type
	{
		typeMD5 = 0,
		typeSHA1 = 101,
		typeSHA224 = 102,
		typeSHA256 = 103,
		typeSHA384 = 104,
		typeSHA512 = 105
	};
	
	static Ref<CryptoHash> create(Type type);

	
	static Ref<CryptoHash> md5();
	
	static Ref<CryptoHash> sha1();
	
	static Ref<CryptoHash> sha224();
	
	static Ref<CryptoHash> sha256();
	
	static Ref<CryptoHash> sha384();
	
	static Ref<CryptoHash> sha512();

public:
	void applyMask_MGF1(const void* seed, sl_uint32 sizeSeed, void* target, sl_uint32 sizeTarget);
};
SLIB_CRYPTO_NAMESPACE_END

#define SLIB_CRYPTO_DEFINE_HASH(CLASS, HASH_SIZE) \
public: \
	SLIB_INLINE static void hash(const void* input, sl_size n, void* output) \
	{ \
		CLASS h; \
		h.execute(input, n, output); \
	} \
	SLIB_INLINE static sl_uint32 getHashSize() \
	{ \
		return HASH_SIZE; \
	} \
	SLIB_INLINE static void hash(const String& s, void* output) \
	{ \
		hash(s.getBuf(), s.getLength(), output); \
	} \
	SLIB_INLINE static void hash(const Memory& data, void* output) \
	{ \
		hash(data.getBuf(), data.getSize(), output); \
	} \
	SLIB_INLINE static Memory hash(const void* input, sl_size n) \
	{ \
		char v[HASH_SIZE]; \
		hash(input, n, v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	SLIB_INLINE static Memory hash(const String& s) \
	{ \
		char v[HASH_SIZE]; \
		hash(s.getBuf(), s.getLength(), v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	SLIB_INLINE static Memory hash(const Memory& data) \
	{ \
		char v[HASH_SIZE]; \
		hash(data.getBuf(), data.getSize(), v); \
		return Memory::create(v, HASH_SIZE); \
	} \
	sl_uint32 getSize() const \
	{ \
		return getHashSize(); \
	}

#endif
