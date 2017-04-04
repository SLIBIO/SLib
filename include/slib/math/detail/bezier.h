/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_DETAIL_BEZIER
#define CHECKHEADER_SLIB_MATH_DETAIL_BEZIER

#include "../bezier.h"

namespace slib
{

	template <class T>
	template <class O>
	CubicBezierCurveT<T>::CubicBezierCurveT(const CubicBezierCurveT<O>& other)
	: x0((T)(other.x0)), y0((T)(other.y0)),
	  x1((T)(other.x1)), y1((T)(other.y1)),
	  x2((T)(other.x2)), y2((T)(other.y2)),
	  x3((T)(other.x3)), y3((T)(other.y3))
	{
	}

	template <class T>
	template <class O>
	CubicBezierCurveT<T>& CubicBezierCurveT<T>::operator=(const CubicBezierCurveT<O>& other)
	{
		x0 = (T)(other.x0);
		y0 = (T)(other.y0);
		x1 = (T)(other.x1);
		y1 = (T)(other.y1);
		x2 = (T)(other.x2);
		y2 = (T)(other.y2);
		x3 = (T)(other.x3);
		y3 = (T)(other.y3);
		return *this;
	}

}

#endif
