/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_NEW_HELPER
#define CHECKHEADER_SLIB_CORE_NEW_HELPER

#include "definition.h"

#include "base.h"
#include "array_traits.h"

#include <new>

namespace slib
{
	
	template <class T>
	class NewHelper
	{
	public:
		SLIB_INLINE static T* create() noexcept
		{
			T* data = (T*)(Base::createMemory(sizeof(T)));
			if (data) {
				new (data) T();
			}
			return data;
		}

		SLIB_INLINE static T* create(sl_size count) noexcept
		{
			T* data = (T*)(Base::createMemory(sizeof(T)*count));
			if (data) {
				ArrayTraits<T>::construct(data, count);
			}
			return data;
		}

		template <class TYPE>
		SLIB_INLINE static T* create(const TYPE* other, sl_size count) noexcept
		{
			T* data = (T*)(Base::createMemory(sizeof(T)*count));
			if (data) {
				ArrayTraits<T>::copy_construct(data, other, count);
			}
			return data;
		}

		SLIB_INLINE static void free(T* data) noexcept
		{
			if (data) {
				data->~T();
				Base::freeMemory((void*)data);
			}
		}

		SLIB_INLINE static void free(T* data, sl_size count) noexcept
		{
			if (data) {
				ArrayTraits<T>::free(data, count);
				Base::freeMemory((void*)data);
			}
		}

	};

}

#endif

