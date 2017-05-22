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
	SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::identity() noexcept
	{
		return *(reinterpret_cast<QuaternionT<T> const*>(&_identity));
	}

	template <class T>
	SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::fromArray(const T arr[2]) noexcept
	{
		return *(reinterpret_cast<QuaternionT<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::fromArray(T arr[2]) noexcept
	{
		return *(reinterpret_cast<QuaternionT<T>*>(arr));
	}
	
	template <class T>
	SLIB_INLINE const Vector4T<T>& QuaternionT<T>::toVector4() const noexcept
	{
		return *((Vector4T<T>*)((void*)this));
	}
	
	template <class T>
	SLIB_INLINE Vector4T<T>& QuaternionT<T>::toVector4() noexcept
	{
		return *((Vector4T<T>*)((void*)this));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::operator=(const QuaternionT<O>& other) noexcept
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::operator=(const Vector4T<T>& other) noexcept
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

}
