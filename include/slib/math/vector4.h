/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_VECTOR4
#define CHECKHEADER_SLIB_MATH_VECTOR4

#include "definition.h"

#include "vector3.h"

#include "../core/interpolation.h"

namespace slib
{
	
	template <class T, class FT = T>
	class SLIB_EXPORT Vector4T
	{
	public:
		T x;
		T y;
		T z;
		T w;

	public:
		SLIB_INLINE Vector4T() noexcept = default;

		SLIB_INLINE constexpr Vector4T(const Vector4T<T, FT>& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {}
	
		template <class O, class FO>
		SLIB_INLINE constexpr Vector4T(const Vector4T<O, FO>& other) noexcept : x((T)(other.x)), y((T)(other.y)), z((T)(other.z)), w((T)(other.w)) {}
	
		SLIB_INLINE constexpr Vector4T(T _x, T _y, T _z, T _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	
		SLIB_INLINE constexpr Vector4T(const Vector3T<T, FT>& xyz, T _w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(_w) {}
	
	public:
		static const Vector4T<T, FT>& zero() noexcept;

		static const Vector4T<T, FT>& fromArray(const T arr[4]) noexcept;
	
		static Vector4T<T, FT>& fromArray(T arr[4]) noexcept;
	
	public:
		const Vector3T<T, FT>& xyz() const noexcept;

		Vector3T<T, FT>& xyz() noexcept;

		static Vector4T<T, FT> fromLocation(const Vector3T<T, FT>& v) noexcept;

		static Vector4T<T, FT> fromDirection(const Vector3T<T, FT>& v) noexcept;

		T dot(const Vector4T<T, FT>& other) const noexcept;

		T getLength2p() const noexcept;

		FT getLength() const noexcept;

		T getLength2p(const Vector4T<T, FT>& other) const noexcept;

		FT getLength(const Vector4T<T, FT>& other) const noexcept;

		void normalize() noexcept;

		Vector4T<T, FT> getNormalized() noexcept;

		FT getCosBetween(const Vector4T<T, FT>& other) const noexcept;

		FT getAngleBetween(const Vector4T<T, FT>& other) const noexcept;

		sl_bool equals(const Vector4T<T, FT>& other) const noexcept;
	
		sl_bool isAlmostEqual(const Vector4T<T, FT>& other) const noexcept;

		Vector4T<T, FT> lerp(const Vector4T<T, FT>& target, float factor) const noexcept;
	
	public:
		Vector4T<T, FT>& operator=(const Vector4T<T, FT>& other) noexcept = default;

		template <class O, class FO>
		Vector4T<T, FT>& operator=(const Vector4T<O, FO>& other) noexcept;

		Vector4T<T, FT> operator+(const Vector4T<T, FT>& other) const noexcept;

		Vector4T<T, FT>& operator+=(const Vector4T<T, FT>& other) noexcept;

		Vector4T<T, FT> operator-(const Vector4T<T, FT>& other) const noexcept;

		Vector4T<T, FT>& operator-=(const Vector4T<T, FT>& other) noexcept;

		Vector4T<T, FT> operator*(T f) const noexcept;

		Vector4T<T, FT>& operator*=(T f) noexcept;

		Vector4T<T, FT> operator*(const Vector4T<T, FT>& other) const noexcept;

		Vector4T<T, FT>& operator*=(const Vector4T<T, FT>& other) noexcept;

		Vector4T<T, FT> operator/(T f) const noexcept;

		Vector4T<T, FT>& operator/=(T f) noexcept;

		Vector4T<T, FT> operator/(const Vector4T<T, FT>& other) const noexcept;

		Vector4T<T, FT>& operator/(const Vector4T<T, FT>& other) noexcept;

		Vector4T<T, FT> operator-() const noexcept;

		sl_bool operator==(const Vector4T<T, FT>& other) const noexcept;

		sl_bool operator!=(const Vector4T<T, FT>& other) const noexcept;

	private:
		static T _zero[4];

	};

	extern template class Vector4T<float>;
	extern template class Vector4T<double>;
	extern template class Vector4T<sl_int32, float>;
	extern template class Vector4T<sl_int64, double>;
	typedef Vector4T<sl_real> Vector4;
	typedef Vector4T<float> Vector4f;
	typedef Vector4T<double> Vector4lf;
	typedef Vector4T<sl_int32, float> Vector4i;
	typedef Vector4T<sl_int64, double> Vector4li;

	template <class T, class FT>
	Vector4T<T, FT> operator*(T f, const Vector4T<T, FT>& v) noexcept;

	template <class T, class FT>
	Vector4T<T, FT> operator/(T f, const Vector4T<T, FT>& v) noexcept;

	template <class T, class FT>
	class Interpolation< Vector4T<T, FT> >
	{
	public:
		static Vector4T<T, FT> interpolate(const Vector4T<T, FT>& a, const Vector4T<T, FT>& b, float factor) noexcept;
	};

}

#include "detail/vector4.inc"

#endif
