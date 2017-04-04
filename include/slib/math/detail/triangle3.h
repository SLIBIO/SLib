/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE3
#define CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE3

#include "../triangle3.h"

namespace slib
{
	
	template <class T>
	template <class O>
	Triangle3T<T>::Triangle3T(const Triangle3T<O>& other)
	 : point1(other.point1), point2(other.point2), point3(other.point3)
	{
	}
	
	template <class T>
	template <class O>
	Triangle3T<T>& Triangle3T<T>::operator=(const Triangle3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}

}

#endif
