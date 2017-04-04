/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE
#define CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE

#include "../triangle.h"

namespace slib
{

	template <class T>
	template <class O>
	TriangleT<T>::TriangleT(const TriangleT<O>& other) : point1(other.point1), point2(other.point2), point3(other.point3)
	{
	}
	
	template <class T>
	template <class O>
	TriangleT<T>& TriangleT<T>::operator=(const TriangleT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}

}

#endif
