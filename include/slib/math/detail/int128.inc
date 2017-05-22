/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

	SLIB_INLINE Uint128& Uint128::operator=(const Uint128& other) noexcept
	{
		low = other.low;
		high = other.high;
		return *this;
	}
	
	SLIB_INLINE Uint128& Uint128::operator=(sl_uint64 num) noexcept
	{
		high = 0;
		low = num;
		return *this;
	}

	SLIB_INLINE const Uint128& Uint128::zero() noexcept
	{
		return *((Uint128*)((void*)_zero));
	}
	
	SLIB_INLINE void Uint128::setZero() noexcept
	{
		high = 0;
		low = 0;
	}

}
