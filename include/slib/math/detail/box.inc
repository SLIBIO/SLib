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

	template <class T>
	template <class O>
	BoxT<T>::BoxT(const BoxT<O>& other) noexcept
	: x1((T)(other.x1)), x2((T)(other.x2))
	, y1((T)(other.y1)), y2((T)(other.y2))
	, z1((T)(other.z1)), z2((T)(other.z2))
	{
	}
	
	template <class T>
	SLIB_INLINE const BoxT<T>& BoxT<T>::zero() noexcept
	{
		return *((BoxT*)((void*)_zero));
	}
	
	template <class T>
	template <class O>
	BoxT<T>& BoxT<T>::operator=(const BoxT<O>& other) noexcept
	{
		x1 = (T)(other.x1); x2 = (T)(other.x2);
		y1 = (T)(other.y1); y2 = (T)(other.y2);
		z1 = (T)(other.z1); z2 = (T)(other.z2);
		return *this;
	}

}
