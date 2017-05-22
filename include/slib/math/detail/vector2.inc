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
	
	template <class T, class FT>
	SLIB_INLINE const Vector2T<T, FT>& Vector2T<T, FT>::zero() noexcept
	{
		return *(reinterpret_cast<Vector2T<T, FT> const*>(&_zero));
	}
	
	template <class T, class FT>
	SLIB_INLINE const Vector2T<T, FT>& Vector2T<T, FT>::fromArray(const T arr[2]) noexcept
	{
		return *(reinterpret_cast<Vector2T<T, FT> const*>(arr));
	}
	
	template <class T, class FT>
	SLIB_INLINE Vector2T<T, FT>& Vector2T<T, FT>::fromArray(T arr[2]) noexcept
	{
		return *(reinterpret_cast<Vector2T<T, FT>*>(arr));
	}
	
	template <class T, class FT>
	template <class O, class FO>
	SLIB_INLINE Vector2T<T, FT>& Vector2T<T, FT>::operator=(const Vector2T<O, FO>& other) noexcept
	{
		x = (T)(other.x);
		y = (T)(other.y);
		return *this;
	}
	
	template <class T, class FT>
	Vector2T<T, FT> operator*(T f, const Vector2T<T, FT>& v) noexcept
	{
		return {f * v.x, f * v.y};
	}
	
	template <class T, class FT>
	Vector2T<T, FT> operator/(T f, const Vector2T<T, FT>& v) noexcept
	{
		return {f / v.x, f / v.y};
	}
	
	template <class T, class FT>
	SLIB_INLINE Vector2T<T, FT> Interpolation< Vector2T<T, FT> >::interpolate(const Vector2T<T, FT>& a, const Vector2T<T, FT>& b, float factor) noexcept
	{
		return a.lerp(b, factor);
	}

}
