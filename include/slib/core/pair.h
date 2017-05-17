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
	
	template <class KT, class VT>
	class SLIB_EXPORT Pair
	{
	public:
		KT key;
		VT value;

	public:
		SLIB_INLINE Pair() noexcept
		 {}

		SLIB_INLINE Pair(const Pair<KT, VT>& other)
		 = default;

		SLIB_INLINE Pair(Pair<KT, VT>&& other)
		 = default;

		template <class KEY, class VALUE>
		SLIB_INLINE Pair(KEY&& _key, VALUE&& _value) noexcept
		 : key(Forward<KEY>(_key)), value(Forward<VALUE>(_value))
		 {}

	public:
		SLIB_INLINE Pair<KT, VT>& operator=(const Pair<KT, VT>& other)
		 = default;

		SLIB_INLINE Pair<KT, VT>& operator=(Pair<KT, VT>&& other)
		 = default;

	};

}

#endif
