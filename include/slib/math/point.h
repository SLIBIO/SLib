/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_POINT
#define CHECKHEADER_SLIB_MATH_POINT

#include "definition.h"

#include "vector2.h"

namespace slib
{

	template <typename T, typename FT = T>
	using PointT = Vector2T<T, FT>;

	typedef PointT<sl_real> Point;
	typedef PointT<sl_int32, float> Pointi;
	typedef PointT<sl_int64, double> Pointli;
	typedef PointT<float> Pointf;
	typedef PointT<double> Pointlf;

}

#endif
