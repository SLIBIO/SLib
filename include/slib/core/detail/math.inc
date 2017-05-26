/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
