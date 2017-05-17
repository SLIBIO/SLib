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
	
	template <class TYPE>
	SLIB_INLINE TYPE Interpolation<TYPE>::interpolate(const TYPE& a, const TYPE& b, float factor)
	{
		return SLIB_LERP(a, b, factor);
	}

}
