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

	template <class T1, class T2>
	SLIB_INLINE int Compare<T1, T2>::operator()(const T1& a, const T2& b) const
	{
		return (a < b) ? -1 : (a > b);
	}

	template <class T1, class T2>
	SLIB_INLINE int CompareDescending<T1, T2>::operator()(const T1& a, const T2& b) const
	{
		return -(Compare<T1, T2>::compare(a, b));
	}

	template <class T1, class T2>
	SLIB_INLINE sl_bool Equals<T1, T2>::operator()(const T1& a, const T2& b) const
	{
		return a == b;
	}

}
