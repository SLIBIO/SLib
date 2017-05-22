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
	SLIB_INLINE Line3T<T>::Line3T(const Line3T<O>& other) noexcept
	 : point1(other.point1), point2(other.point2)
	 {}
	
	template <class T>
	template <class O>
	SLIB_INLINE Line3T<T>& Line3T<T>::operator=(const Line3T<O>& other) noexcept
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}

}

