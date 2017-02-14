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

namespace slib
{
	
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
		CryptoHash();

		~CryptoHash();

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

}

#endif
