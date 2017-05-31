/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_MAP_COMMON
#define CHECKHEADER_SLIB_CORE_MAP_COMMON

#include "definition.h"

#include "cpp.h"

namespace slib
{
	
	enum class MapContainerType
	{
		Hash = 0,
		Tree = 1
	};
		
	template <class NODE>
	class SLIB_EXPORT MapEmplaceReturn
	{
	public:
		sl_bool isSuccess;
		NODE* node;
		
	public:
		SLIB_INLINE constexpr MapEmplaceReturn(sl_bool _isSuccess, NODE* _node) noexcept
		 : isSuccess(_isSuccess), node(_node)
		 {}
		
		SLIB_INLINE constexpr MapEmplaceReturn(sl_null_t) noexcept
		 : isSuccess(sl_false), node(sl_null)
		 {}

		SLIB_INLINE constexpr MapEmplaceReturn(const MapEmplaceReturn& other) noexcept = default;
		
		SLIB_INLINE MapEmplaceReturn& operator=(const MapEmplaceReturn& other) noexcept = default;
		
		SLIB_INLINE explicit constexpr operator sl_bool() const noexcept
		{
			return isSuccess;
		}
		
	};

}

#endif
