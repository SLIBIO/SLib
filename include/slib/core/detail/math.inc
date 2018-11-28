/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#if defined(SLIB_COMPILER_IS_VC)
#include <intrin.h>
#pragma intrinsic(__emulu)
#	if defined(SLIB_ARCH_IS_64BIT)
#pragma intrinsic(_umul128)
#	endif
#endif

namespace slib
{
	SLIB_INLINE void Math::mul32(sl_uint32 a, sl_uint32 b, sl_uint32& o_high, sl_uint32& o_low) noexcept
	{
#if defined(SLIB_COMPILER_IS_VC)
		sl_uint64 m = __emulu(a, b);
		o_high = (sl_uint32)(m >> 32);
		o_low = (sl_uint32)(m);
#else
		sl_uint64 m = a;
		m *= b;
		o_high = (sl_uint32)(m >> 32);
		o_low = (sl_uint32)(m);
#endif
	}

	SLIB_INLINE void Math::mul64(sl_uint64 a, sl_uint64 b, sl_uint64& o_high, sl_uint64& o_low) noexcept
	{
#if defined(SLIB_COMPILER_IS_VC) && defined(SLIB_ARCH_IS_64BIT)
		o_low = _umul128(a, b, &o_high);
#elif defined(SLIB_COMPILER_IS_GCC) && defined(__SIZEOF_INT128__)
		unsigned __int128 m = ((unsigned __int128)a) * ((unsigned __int128)b);
		o_high = (sl_uint64)(m >> 64);
		o_low = (sl_uint64)m;
#else
		sl_uint64 al = (sl_uint64)((sl_uint32)a);
		sl_uint64 ah = a >> 32;
		sl_uint64 bl = (sl_uint64)((sl_uint32)b);
		sl_uint64 bh = b >> 32;
		sl_uint64 m0 = al * bl;
		sl_uint64 m1 = al * bh + (m0 >> 32);
		sl_uint64 m2 = bh * al + (sl_uint32)(m1);
		o_low = (((sl_uint64)((sl_uint32)m2)) << 32) + ((sl_uint32)m0);
		o_high = ah * bh + (m1 >> 32) + (m2 >> 32);
#endif
	}

}
