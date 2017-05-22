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
	SLIB_INLINE PlaneT<T>& PlaneT<T>::operator=(const PlaneT<O>& other) noexcept
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		d = (T)(other.d);
		return *this;
	}

}
