#include "../../../inc/slib/core/hash.h"

SLIB_NAMESPACE_BEGIN

// based on adler32
#define MOD_ADLER 65521

sl_uint32 HashBytes(const void* _buf, sl_size n)
{
	sl_uint8* buf = (sl_uint8*)_buf;
	sl_uint32 a = 1, b = 0;
	for (sl_size i = 0; i < n; i++) {
		a = (a + buf[i]) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}
	return Rehash((b << 16) | a);
}

SLIB_NAMESPACE_END
