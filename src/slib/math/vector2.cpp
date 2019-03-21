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

#include "slib/math/vector2.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T, class FT>
	SLIB_ALIGN(8) T Vector2T<T, FT>::_zero[2] = {0, 0};

	template <class T, class FT>
	T Vector2T<T, FT>::dot(const Vector2T<T, FT>& other) const noexcept
	{
		return x * other.x + y * other.y;
	}

	template <class T, class FT>
	T Vector2T<T, FT>::cross(const Vector2T<T, FT>& other) const noexcept
	{
		return x*other.y - y*other.x;
	}

	template <class T, class FT>
	T Vector2T<T, FT>::getLength2p() const noexcept
	{
		return x * x + y * y;
	}

	template <class T, class FT>
	FT Vector2T<T, FT>::getLength() const noexcept
	{
		return Math::sqrt((FT)(x * x + y * y));
	}

	template <class T, class FT>
	T Vector2T<T, FT>::getLength2p(const Vector2T<T, FT>& other) const noexcept
	{
		T dx = x - other.x;
		T dy = y - other.y;
		return dx * dx + dy * dy;
	}

	template <class T, class FT>
	FT Vector2T<T, FT>::getLength(const Vector2T<T, FT>& other) const noexcept
	{
		T dx = x - other.x;
		T dy = y - other.y;
		return Math::sqrt((FT)(dx * dx + dy * dy));
	}

	template <class T, class FT>
	void Vector2T<T, FT>::normalize() noexcept
	{
		T l = x * x + y * y;
		if (l > 0) {
			FT d = Math::sqrt((FT)l);
			x = (T)((FT)x / d);
			y = (T)((FT)y / d);
		}
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::getNormalized() noexcept
	{
		T l = x * x + y * y;
		if (l > 0) {
			FT d = Math::sqrt((FT)l);
			return {(T)((FT)x / d), (T)((FT)y / d)};
		}
		return *this;
	}

	template <class T, class FT>
	FT Vector2T<T, FT>::getCosBetween(const Vector2T<T, FT>& other) const noexcept
	{
		return (FT)(dot(other)) / Math::sqrt((FT)(getLength2p() * other.getLength2p()));
	}

	template <class T, class FT>
	FT Vector2T<T, FT>::getAbsAngleBetween(const Vector2T<T, FT>& other) const noexcept
	{
		return Math::arccos(getCosBetween(other));
	}

	template <class T, class FT>
	FT Vector2T<T, FT>::getAngleBetween(const Vector2T<T, FT>& other) const noexcept
	{
		FT a = getAbsAngleBetween(other);
		if (cross(other) > 0) {
			a = -a;
		}
		return a;
	}

	template <class T, class FT>
	sl_bool Vector2T<T, FT>::equals(const Vector2T<T, FT>& other) const noexcept
	{
		return x == other.x && y == other.y;
	}

	template <class T, class FT>
	sl_bool Vector2T<T, FT>::isAlmostEqual(const Vector2T<T, FT>& other) const noexcept
	{
		return Math::isAlmostZero((FT)(x - other.x)) && Math::isAlmostZero((FT)(y - other.y));
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::lerp(const Vector2T<T, FT>& target, float factor) const noexcept
	{
		return {(T)SLIB_LERP(x, target.x, factor), (T)SLIB_LERP(y, target.y, factor)};
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator+(const Vector2T<T, FT>& other) const noexcept
	{
		return {x + other.x, y + other.y};
	}

	template <class T, class FT>
	Vector2T<T, FT>& Vector2T<T, FT>::operator+=(const Vector2T<T, FT>& other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator-(const Vector2T<T, FT>& other) const noexcept
	{
		return {x - other.x, y - other.y};
	}

	template <class T, class FT>
	Vector2T<T, FT>& Vector2T<T, FT>::operator-=(const Vector2T<T, FT>& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator*(T f) const noexcept
	{
		return {x * f, y * f};
	}

	template <class T, class FT>
	Vector2T<T, FT>& Vector2T<T, FT>::operator*=(T f) noexcept
	{
		x *= f;
		y *= f;
		return *this;
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator*(const Vector2T<T, FT>& other) const noexcept
	{
		return {x * other.x, y * other.y};
	}

	template <class T, class FT>
	Vector2T<T, FT>& Vector2T<T, FT>::operator*=(const Vector2T<T, FT>& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator/(T f) const noexcept
	{
		return {x / f, y / f};
	}

	template <class T, class FT>
	Vector2T<T, FT>& Vector2T<T, FT>::operator/=(T f) noexcept
	{
		x /= f;
		y /= f;
		return *this;
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator/(const Vector2T<T, FT>& other) const noexcept
	{
		return {x / other.x, y / other.y};
	}

	template <class T, class FT>
	Vector2T<T, FT>& Vector2T<T, FT>::operator/(const Vector2T<T, FT>& other) noexcept
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	template <class T, class FT>
	Vector2T<T, FT> Vector2T<T, FT>::operator-() const noexcept
	{
		return {-x, -y};
	}

	template <class T, class FT>
	sl_bool Vector2T<T, FT>::operator==(const Vector2T<T, FT>& other) const noexcept
	{
		return x == other.x && y == other.y;
	}

	template <class T, class FT>
	sl_bool Vector2T<T, FT>::operator!=(const Vector2T<T, FT>& other) const noexcept
	{
		return x != other.x || y != other.y;
	}


	SLIB_DEFINE_GEOMETRY_TYPE_EX(Vector2)

}
