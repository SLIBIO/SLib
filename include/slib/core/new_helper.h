/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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

