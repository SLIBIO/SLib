/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <new>

namespace slib
{
	
	template <class T>
	SLIB_INLINE T* NewHelper<T>::create() noexcept
	{
		T* data = (T*)(Base::createMemory(sizeof(T)));
		if (data) {
			new (data) T();
		}
		return data;
	}

	template <class T>
	SLIB_INLINE T* NewHelper<T>::create(sl_size count) noexcept
	{
		T* data = (T*)(Base::createMemory(sizeof(T)*count));
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				new (data + i) T();
			}
		}
		return data;
	}

	template <class T>
	template <class _T>
	SLIB_INLINE T* NewHelper<T>::create(const _T* other) noexcept
	{
		T* data = (T*)(Base::createMemory(sizeof(T)));
		if (data) {
			new (data) T(*other);
		}
		return data;
	}

	template <class T>
	template <class _T>
	SLIB_INLINE T* NewHelper<T>::create(const _T* other, sl_size count) noexcept
	{
		T* data = (T*)(Base::createMemory(sizeof(T)*count));
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				new (data + i) T(other[i]);
			}
		}
		return data;
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::free(T* data) noexcept
	{
		if (data) {
			data->~T();
			Base::freeMemory((void*)data);
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::free(T* data, sl_size count) noexcept
	{
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				(data+i)->~T();
			}
			Base::freeMemory((void*)data);
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data) noexcept
	{
		new (data) T();
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			new (data + i) T();
		}
	}

	template <class T>
	template <class _T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data, const _T* other) noexcept
	{
		new (data) T(*other);
	}

	template <class T>
	template <class _T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data, const _T* other, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			new (data + i) T(other[i]);
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::destructor(T* data) noexcept
	{
		if (data) {
			data->~T();
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::destructor(T* data, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			(data+i)->~T();
		}
	}

}
