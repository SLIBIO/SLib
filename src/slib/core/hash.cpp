/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/hash.h"

namespace slib
{
	// based on adler32
	sl_uint32 HashBytes(const void* _buf, sl_size n)
	{
#define MOD_ADLER 65521
		sl_uint8* buf = (sl_uint8*)_buf;
		sl_uint32 a = 1, b = 0;
		for (sl_size i = 0; i < n; i++) {
			a = (a + buf[i]) % MOD_ADLER;
			b = (b + a) % MOD_ADLER;
		}
		return Rehash((b << 16) | a);
	}

}
