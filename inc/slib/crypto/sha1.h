#ifndef CHECKHEADER_SLIB_CRYPTO_SHA1
#define CHECKHEADER_SLIB_CRYPTO_SHA1

#include "definition.h"

#include "hash.h"

/*
	SHA1 - Secure Hash Algorithm

	Output: 160bits (20 bytes)
*/

namespace slib
{
	
	class SLIB_EXPORT SHA1 : public CryptoHash
	{
	public:
		SHA1();

		~SHA1();
	
	public:
		// override
		void start();

		// override
		void update(const void* input, sl_size n);

		// override
		void finish(void* output);

	public: /* common functions for CryptoHash */
		static void hash(const void* input, sl_size n, void* output);

		static sl_uint32 getHashSize();

		static void hash(const String& s, void* output);

		static void hash(const Memory& data, void* output);

		static Memory hash(const void* input, sl_size n);

		static Memory hash(const String& s);

		static Memory hash(const Memory& data);

		sl_uint32 getSize() const;
	
	private:
		void _updateSection(const sl_uint8* input);
	
	private:
		sl_size sizeTotalInput;
		sl_uint32 rdata_len;
		sl_uint8 rdata[64];
		sl_uint32 h[5];

	};

}

#endif
