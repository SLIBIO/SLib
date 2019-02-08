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

#ifndef CHECKHEADER_SLIB_MATH_VECTOR2
#define CHECKHEADER_SLIB_MATH_VECTOR2

#include "definition.h"

#include "../core/interpolation.h"

namespace slib
{
	
	template <class T, class FT = T>
	class SLIB_EXPORT Vector2T
	{
	public:
		T x;
		T y;
	
	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(Vector2T)
		
		SLIB_INLINE Vector2T() noexcept = default;
	
		template <class O, class FO>
		SLIB_INLINE constexpr Vector2T(const Vector2T<O, FO>& other) noexcept : x((T)(other.x)), y((T)(other.y)) {}
	
		SLIB_INLINE constexpr Vector2T(T _x, T _y) noexcept : x(_x), y(_y) {}
	
	public:
		static const Vector2T<T, FT>& zero() noexcept;

		static const Vector2T<T, FT>& fromArray(const T arr[2]) noexcept;

		static Vector2T<T, FT>& fromArray(T arr[2]) noexcept;

		T dot(const Vector2T<T, FT>& other) const noexcept;

		T cross(const Vector2T<T, FT>& other) const noexcept;

		T getLength2p() const noexcept;

		FT getLength() const noexcept;

		T getLength2p(const Vector2T<T, FT>& other) const noexcept;

		FT getLength(const Vector2T<T, FT>& other) const noexcept;

		void normalize() noexcept;

		Vector2T<T, FT> getNormalized() noexcept;

		FT getCosBetween(const Vector2T<T, FT>& other) const noexcept;

		FT getAbsAngleBetween(const Vector2T<T, FT>& other) const noexcept;

		FT getAngleBetween(const Vector2T<T, FT>& other) const noexcept;

		sl_bool equals(const Vector2T<T, FT>& other) const noexcept;

		sl_bool isAlmostEqual(const Vector2T<T, FT>& other) const noexcept;

		Vector2T<T, FT> lerp(const Vector2T<T, FT>& target, float factor) const noexcept;
	
	public:
		template <class O, class FO>
		Vector2T<T, FT>& operator=(const Vector2T<O, FO>& other) noexcept;

		Vector2T<T, FT> operator+(const Vector2T<T, FT>& other) const noexcept;

		Vector2T<T, FT>& operator+=(const Vector2T<T, FT>& other) noexcept;

		Vector2T<T, FT> operator-(const Vector2T<T, FT>& other) const noexcept;

		Vector2T<T, FT>& operator-=(const Vector2T<T, FT>& other) noexcept;

		Vector2T<T, FT> operator*(T f) const noexcept;

		Vector2T<T, FT>& operator*=(T f) noexcept;

		Vector2T<T, FT> operator*(const Vector2T<T, FT>& other) const noexcept;

		Vector2T<T, FT>& operator*=(const Vector2T<T, FT>& other) noexcept;

		Vector2T<T, FT> operator/(T f) const noexcept;

		Vector2T<T, FT>& operator/=(T f) noexcept;

		Vector2T<T, FT> operator/(const Vector2T<T, FT>& other) const noexcept;

		Vector2T<T, FT>& operator/(const Vector2T<T, FT>& other) noexcept;

		Vector2T<T, FT> operator-() const noexcept;

		sl_bool operator==(const Vector2T<T, FT>& other) const noexcept;

		sl_bool operator!=(const Vector2T<T, FT>& other) const noexcept;

	private:
		static T _zero[2];
	
	};
	
	
	extern template class Vector2T<float>;
	extern template class Vector2T<double>;
	extern template class Vector2T<sl_int32, float>;
	extern template class Vector2T<sl_int64, double>;
	typedef Vector2T<sl_real> Vector2;
	typedef Vector2T<float> Vector2f;
	typedef Vector2T<double> Vector2lf;
	typedef Vector2T<sl_int32, float> Vector2i;
	typedef Vector2T<sl_int64, double> Vector2li;
	
	template <class T, class FT>
	Vector2T<T, FT> operator*(T f, const Vector2T<T, FT>& v) noexcept;
	
	template <class T, class FT>
	Vector2T<T, FT> operator/(T f, const Vector2T<T, FT>& v) noexcept;
	
	template <class T, class FT>
	class Interpolation< Vector2T<T, FT> >
	{
	public:
		static Vector2T<T, FT> interpolate(const Vector2T<T, FT>& a, const Vector2T<T, FT>& b, float factor) noexcept;
	};

}

#include "detail/vector2.inc"

#endif
