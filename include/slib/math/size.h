/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_SIZE
#define CHECKHEADER_SLIB_MATH_SIZE

#include "definition.h"

#include "vector2.h"

namespace slib
{

	template <typename T, typename FT = T>
	using SizeT = Vector2T<T, FT>;

	typedef SizeT<sl_real> Size;
	typedef SizeT<sl_int32, float> Sizei;
	typedef SizeT<sl_int64, double> Sizeli;
	typedef SizeT<float> Sizef;
	typedef SizeT<double> Sizelf;

}

#endif
