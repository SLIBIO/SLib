/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_PAIR
#define CHECKHEADER_SLIB_CORE_PAIR

#include "definition.h"

#include "cpp.h"

namespace slib
{
	
	template <class FIRST_T, class SECOND_T>
	class SLIB_EXPORT Pair
	{
	public:
		FIRST_T first;
		SECOND_T second;

	public:
		SLIB_INLINE Pair() noexcept
		 {}

		SLIB_INLINE Pair(const Pair& other)
		 = default;

		SLIB_INLINE Pair(Pair&& other)
		 = default;

		template <class FIRST, class SECOND>
		SLIB_INLINE Pair(FIRST&& _first, SECOND&& _second) noexcept
		 : first(Forward<FIRST>(_first)), second(Forward<SECOND>(_second))
		 {}

	public:
		SLIB_INLINE Pair& operator=(const Pair& other)
		 = default;

		SLIB_INLINE Pair& operator=(Pair&& other)
		 = default;

	};

}

#endif
