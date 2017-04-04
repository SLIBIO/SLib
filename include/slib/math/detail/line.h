/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_DETAIL_LINE
#define CHECKHEADER_SLIB_MATH_DETAIL_LINE

#include "../line.h"

namespace slib
{
	
	template <class T>
	template <class O>
	SLIB_INLINE LineT<T>& LineT<T>::operator=(const LineT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		return *this;
	}

}

#endif
