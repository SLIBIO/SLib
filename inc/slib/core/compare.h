/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_COMPARE
#define CHECKHEADER_SLIB_CORE_COMPARE

#include "definition.h"

namespace slib
{

	template <class T1, class T2 = T1>
	class SLIB_EXPORT Compare
	{
	public:
		int operator()(const T1& a, const T2& b) const;
	};

	template <class T1, class T2 = T1>
	class SLIB_EXPORT CompareDescending
	{
	public:
		int operator()(const T1& a, const T2& b) const;
	};

	template <class T1, class T2 = T1>
	class SLIB_EXPORT Equals
	{
	public:
		sl_bool operator()(const T1& a, const T2& b) const;
	};

}

#include "detail/compare.h"

#endif
