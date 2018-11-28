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

#include "slib/math/vector4.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T, class FT>
	SLIB_ALIGN(8) T Vector4T<T, FT>::_zero[4] = {0, 0, 0, 0};

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::fromLocation(const Vector3T<T, FT>& v) noexcept
	{
		return {v.x, v.y, v.z, 1};
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::fromDirection(const Vector3T<T, FT>& v) noexcept
	{
		return {v.x, v.y, v.z, 0};
	}

	template <class T, class FT>
	T Vector4T<T, FT>::dot(const Vector4T<T, FT>& other) const noexcept
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	template <class T, class FT>
	T Vector4T<T, FT>::getLength2p() const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	template <class T, class FT>
	FT Vector4T<T, FT>::getLength() const noexcept
	{
		return Math::sqrt((FT)(x * x + y * y + z * z + w * w));
	}

	template <class T, class FT>
	T Vector4T<T, FT>::getLength2p(const Vector4T<T, FT>& other) const noexcept
	{
		T dx = x - other.x;
		T dy = y - other.y;
		T dz = z - other.z;
		T dw = w - other.w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	template <class T, class FT>
	FT Vector4T<T, FT>::getLength(const Vector4T<T, FT>& other) const noexcept
	{
		T dx = x - other.x;
		T dy = y - other.y;
		T dz = z - other.z;
		T dw = w - other.w;
		return Math::sqrt((FT)(dx * dx + dy * dy + dz * dz + dw * dw));
	}

	template <class T, class FT>
	void Vector4T<T, FT>::normalize() noexcept
	{
		T l = x * x + y * y + z * z + w * w;
		if (l > 0) {
			FT d = Math::sqrt((FT)l);
			x = (T)((FT)x / d);
			y = (T)((FT)y / d);
			z = (T)((FT)z / d);
			w = (T)((FT)w / d);
		}
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::getNormalized() noexcept
	{
		T l = x * x + y * y + z * z + w * w;
		if (l > 0) {
			FT d = Math::sqrt((FT)l);
			T _x = (T)((FT)x / d);
			T _y = (T)((FT)y / d);
			T _z = (T)((FT)z / d);
			T _w = (T)((FT)w / d);
			return {_x, _y, _z, _w};
		}
		return *this;
	}

	template <class T, class FT>
	FT Vector4T<T, FT>::getCosBetween(const Vector4T<T, FT>& other) const noexcept
	{
		return (FT)(dot(other)) / Math::sqrt((FT)(getLength2p() * other.getLength2p()));
	}

	template <class T, class FT>
	FT Vector4T<T, FT>::getAngleBetween(const Vector4T<T, FT>& other) const noexcept
	{
		return Math::arccos(getCosBetween(other));
	}

	template <class T, class FT>
	sl_bool Vector4T<T, FT>::equals(const Vector4T<T, FT>& other) const noexcept
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	template <class T, class FT>
	sl_bool Vector4T<T, FT>::isAlmostEqual(const Vector4T<T, FT>& other) const noexcept
	{
		return Math::isAlmostZero((FT)(x - other.x)) && Math::isAlmostZero((FT)(y - other.y)) && Math::isAlmostZero((FT)(z - other.z)) && Math::isAlmostZero((FT)(w - other.w));
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::lerp(const Vector4T<T, FT>& target, float factor) const noexcept
	{
		return {(T)SLIB_LERP(x, target.x, factor), (T)SLIB_LERP(y, target.y, factor), (T)SLIB_LERP(z, target.z, factor), (T)SLIB_LERP(w, target.w, factor)};
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator+(const Vector4T<T, FT>& other) const noexcept
	{
		return {x + other.x, y + other.y, z + other.z, w + other.w};
	}

	template <class T, class FT>
	Vector4T<T, FT>& Vector4T<T, FT>::operator+=(const Vector4T<T, FT>& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator-(const Vector4T<T, FT>& other) const noexcept
	{
		return {x - other.x, y - other.y, z - other.z, w - other.w};
	}

	template <class T, class FT>
	Vector4T<T, FT>& Vector4T<T, FT>::operator-=(const Vector4T<T, FT>& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator*(T f) const noexcept
	{
		return {x * f, y * f, z * f, w * f};
	}

	template <class T, class FT>
	Vector4T<T, FT>& Vector4T<T, FT>::operator*=(T f) noexcept
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator*(const Vector4T<T, FT>& other) const noexcept
	{
		return {x * other.x, y * other.y, z * other.z, w * other.w};
	}

	template <class T, class FT>
	Vector4T<T, FT>& Vector4T<T, FT>::operator*=(const Vector4T<T, FT>& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator/(T f) const noexcept
	{
		return {x / f, y / f, z / f, w / f};
	}

	template <class T, class FT>
	Vector4T<T, FT>& Vector4T<T, FT>::operator/=(T f) noexcept
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator/(const Vector4T<T, FT>& other) const noexcept
	{
		return {x / other.x, y / other.y, z / other.z, w / other.w};
	}

	template <class T, class FT>
	Vector4T<T, FT>& Vector4T<T, FT>::operator/(const Vector4T<T, FT>& other) noexcept
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	template <class T, class FT>
	Vector4T<T, FT> Vector4T<T, FT>::operator-() const noexcept
	{
		return {-x, -y, -z, -w};
	}

	template <class T, class FT>
	sl_bool Vector4T<T, FT>::operator==(const Vector4T<T, FT>& other) const noexcept
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	template <class T, class FT>
	sl_bool Vector4T<T, FT>::operator!=(const Vector4T<T, FT>& other) const noexcept
	{
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}


	SLIB_DEFINE_GEOMETRY_TYPE_EX(Vector4)

}
