/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/math/vector3.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T, class FT>
	SLIB_ALIGN(8) T Vector3T<T, FT>::_zero[3] = {0, 0, 0};

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::fromLocation(const Vector2T<T>& v)
	{
		return {v.x, v.y, 1};
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::fromDirection(const Vector2T<T>& v)
	{
		return {v.x, v.y, 0};
	}

	template <class T, class FT>
	T Vector3T<T, FT>::dot(const Vector3T<T, FT>& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::cross(const Vector3T<T, FT>& other) const
	{
		T _x = y*other.z - z*other.y;
		T _y = z*other.x - x*other.z;
		T _z = x*other.y - y*other.x;
		return {_x, _y, _z};
	}

	template <class T, class FT>
	T Vector3T<T, FT>::getLength2p() const
	{
		return x * x + y * y + z * z;
	}

	template <class T, class FT>
	FT Vector3T<T, FT>::getLength() const
	{
		return Math::sqrt((FT)(x * x + y * y + z * z));
	}

	template <class T, class FT>
	T Vector3T<T, FT>::getLength2p(const Vector3T<T, FT>& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		T dz = z - other.z;
		return dx * dx + dy * dy + dz * dz;
	}

	template <class T, class FT>
	FT Vector3T<T, FT>::getLength(const Vector3T<T, FT>& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		T dz = z - other.z;
		return Math::sqrt((FT)(dx * dx + dy * dy + dz * dz));
	}

	template <class T, class FT>
	void Vector3T<T, FT>::normalize()
	{
		T l = x * x + y * y + z * z;
		if (l > 0) {
			FT d = Math::sqrt((FT)l);
			x = (T)((FT)x / d);
			y = (T)((FT)y / d);
			z = (T)((FT)z / d);
		}
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::getNormalized()
	{
		T l = x * x + y * y + z * z;
		if (l > 0) {
			FT d = Math::sqrt((FT)l);
			T _x = (T)((FT)x / d);
			T _y = (T)((FT)y / d);
			T _z = (T)((FT)z / d);
			return {_x, _y, _z};
		}
		return *this;
	}

	template <class T, class FT>
	FT Vector3T<T, FT>::getCosBetween(const Vector3T<T, FT>& other) const
	{
		return dot(other) / Math::sqrt((FT)(getLength2p() * other.getLength2p()));
	}

	template <class T, class FT>
	FT Vector3T<T, FT>::getAngleBetween(const Vector3T<T, FT>& other) const
	{
		return Math::arccos(getCosBetween(other));
	}

	template <class T, class FT>
	sl_bool Vector3T<T, FT>::equals(const Vector3T<T, FT>& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	template <class T, class FT>
	sl_bool Vector3T<T, FT>::isAlmostEqual(const Vector3T<T, FT>& other) const
	{
		return Math::isAlmostZero((FT)(x - other.x)) && Math::isAlmostZero((FT)(y - other.y)) && Math::isAlmostZero((FT)(z - other.z));
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::lerp(const Vector3T<T, FT>& target, float factor) const
	{
		return {(T)SLIB_LERP(x, target.x, factor), (T)SLIB_LERP(y, target.y, factor), (T)SLIB_LERP(z, target.z, factor)};
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator+(const Vector3T<T, FT>& other) const
	{
		return {x + other.x, y + other.y, z + other.z};
	}

	template <class T, class FT>
	Vector3T<T, FT>& Vector3T<T, FT>::operator+=(const Vector3T<T, FT>& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator-(const Vector3T<T, FT>& other) const
	{
		return {x - other.x, y - other.y, z - other.z};
	}

	template <class T, class FT>
	Vector3T<T, FT>& Vector3T<T, FT>::operator-=(const Vector3T<T, FT>& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator*(T f) const
	{
		return {x * f, y * f, z * f};
	}

	template <class T, class FT>
	Vector3T<T, FT>& Vector3T<T, FT>::operator*=(T f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator*(const Vector3T<T, FT>& other) const
	{
		return {x * other.x, y * other.y, z * other.z};
	}

	template <class T, class FT>
	Vector3T<T, FT>& Vector3T<T, FT>::operator*=(const Vector3T<T, FT>& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator/(T f) const
	{
		return {x / f, y / f, z / f};
	}

	template <class T, class FT>
	Vector3T<T, FT>& Vector3T<T, FT>::operator/=(T f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator/(const Vector3T<T, FT>& other) const
	{
		return {x / other.x, y / other.y, z / other.z};
	}

	template <class T, class FT>
	Vector3T<T, FT>& Vector3T<T, FT>::operator/(const Vector3T<T, FT>& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	template <class T, class FT>
	Vector3T<T, FT> Vector3T<T, FT>::operator-() const
	{
		return {-x, -y, -z};
	}

	template <class T, class FT>
	sl_bool Vector3T<T, FT>::operator==(const Vector3T<T, FT>& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	template <class T, class FT>
	sl_bool Vector3T<T, FT>::operator!=(const Vector3T<T, FT>& other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}


	SLIB_DEFINE_GEOMETRY_TYPE_EX(Vector3)

}
