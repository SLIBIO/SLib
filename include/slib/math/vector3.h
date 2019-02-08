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

#ifndef CHECKHEADER_SLIB_MATH_VECTOR3
#define CHECKHEADER_SLIB_MATH_VECTOR3

#include "definition.h"

#include "vector2.h"

#include "../core/interpolation.h"

namespace slib
{
	
	template <class T, class FT = T>
	class SLIB_EXPORT Vector3T
	{
	public:
		T x;
		T y;
		T z;
	
	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(Vector3T)
		
		SLIB_INLINE Vector3T() noexcept = default;

		template <class O, class FO>
		SLIB_INLINE constexpr Vector3T(const Vector3T<O, FO>& other) noexcept : x((T)(other.x)), y((T)(other.y)), z((T)(other.z)) {}
	
		SLIB_INLINE constexpr Vector3T(T _x, T _y, T _z) noexcept : x(_x), y(_y), z(_z) {}
	
	public:
		static const Vector3T<T, FT>& zero() noexcept;

		static const Vector3T<T, FT>& fromArray(const T arr[3]) noexcept;

		static Vector3T<T, FT>& fromArray(T arr[3]) noexcept;

		static Vector3T<T, FT> fromLocation(const Vector2T<T>& v) noexcept;

		static Vector3T<T, FT> fromDirection(const Vector2T<T>& v) noexcept;

		T dot(const Vector3T<T, FT>& other) const noexcept;

		Vector3T<T, FT> cross(const Vector3T<T, FT>& other) const noexcept;

		T getLength2p() const noexcept;

		FT getLength() const noexcept;

		T getLength2p(const Vector3T<T, FT>& other) const noexcept;

		FT getLength(const Vector3T<T, FT>& other) const noexcept;

		void normalize() noexcept;

		Vector3T<T, FT> getNormalized() noexcept;

		FT getCosBetween(const Vector3T<T, FT>& other) const noexcept;

		FT getAngleBetween(const Vector3T<T, FT>& other) const noexcept;

		sl_bool equals(const Vector3T<T, FT>& other) const noexcept;

		sl_bool isAlmostEqual(const Vector3T<T, FT>& other) const noexcept;

		Vector3T<T, FT> lerp(const Vector3T<T, FT>& target, float factor) const noexcept;
	
	public:
		template <class O, class FO>
		Vector3T<T, FT>& operator=(const Vector3T<O, FO>& other) noexcept;

		Vector3T<T, FT> operator+(const Vector3T<T, FT>& other) const noexcept;

		Vector3T<T, FT>& operator+=(const Vector3T<T, FT>& other) noexcept;

		Vector3T<T, FT> operator-(const Vector3T<T, FT>& other) const noexcept;

		Vector3T<T, FT>& operator-=(const Vector3T<T, FT>& other) noexcept;

		Vector3T<T, FT> operator*(T f) const noexcept;

		Vector3T<T, FT>& operator*=(T f) noexcept;

		Vector3T<T, FT> operator*(const Vector3T<T, FT>& other) const noexcept;

		Vector3T<T, FT>& operator*=(const Vector3T<T, FT>& other) noexcept;

		Vector3T<T, FT> operator/(T f) const noexcept;

		Vector3T<T, FT>& operator/=(T f) noexcept;

		Vector3T<T, FT> operator/(const Vector3T<T, FT>& other) const noexcept;

		Vector3T<T, FT>& operator/(const Vector3T<T, FT>& other) noexcept;

		Vector3T<T, FT> operator-() const noexcept;

		sl_bool operator==(const Vector3T<T, FT>& other) const noexcept;

		sl_bool operator!=(const Vector3T<T, FT>& other) const noexcept;

	private:
		static T _zero[3];
	
	};
	
	extern template class Vector3T<float>;
	extern template class Vector3T<double>;
	extern template class Vector3T<sl_int32, float>;
	extern template class Vector3T<sl_int64, double>;
	typedef Vector3T<sl_real> Vector3;
	typedef Vector3T<float> Vector3f;
	typedef Vector3T<double> Vector3lf;
	typedef Vector3T<sl_int32, float> Vector3i;
	typedef Vector3T<sl_int64, double> Vector3li;
	
	template <class T, class FT>
	Vector3T<T, FT> operator*(T f, const Vector3T<T, FT>& v) noexcept;
	
	template <class T, class FT>
	Vector3T<T, FT> operator/(T f, const Vector3T<T, FT>& v) noexcept;
	
	template <class T, class FT>
	class Interpolation< Vector3T<T, FT> >
	{
	public:
		static Vector3T<T, FT> interpolate(const Vector3T<T, FT>& a, const Vector3T<T, FT>& b, float factor) noexcept;
	};

}

#include "detail/vector3.inc"

#endif
