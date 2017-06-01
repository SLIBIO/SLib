/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_SEARCH
#define CHECKHEADER_SLIB_CORE_SEARCH

#include "definition.h"

namespace slib
{

	class SLIB_EXPORT BinarySearch
	{
	public:
		template < class T1, class T2, class COMPARE=Compare<T1, T2> >
		static sl_bool search(const T1* list, sl_size size, const T2& what, sl_size* pIndexGreaterEqual = sl_null, const COMPARE& compare = COMPARE()) noexcept;

	};

}

#include "detail/search.inc"

#endif
