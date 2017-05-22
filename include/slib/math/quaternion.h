/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_QUATERNION
#define CHECKHEADER_SLIB_MATH_QUATERNION

#include "definition.h"

#include "vector4.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT QuaternionT
	{
	public:
		T x;
		T y;
		T z;
		T w;

	public:
		SLIB_INLINE QuaternionT() noexcept = default;

		SLIB_INLINE constexpr QuaternionT(const QuaternionT<T>& other)
		 : x(other.x), y(other.y), z(other.z), w(other.w)
		 {}

		template <class O>
		SLIB_INLINE constexpr QuaternionT(const QuaternionT<O>& other)
		 : x((T)(other.x)), y((T)(other.y)), z((T)(other.z)), w((T)(other.w))
		 {}
	
		SLIB_INLINE QuaternionT(T _x, T _y, T _z, T _w)
		 : x(_x), y(_y), z(_z), w(_w)
		 {}

		SLIB_INLINE QuaternionT(const Vector4T<T>& other)
		 : x(other.x), y(other.y), z(other.z), w(other.w)
		 {}
	
	public:
		static const QuaternionT<T>& identity() noexcept;

		static const QuaternionT<T>& fromArray(const T arr[2]) noexcept;

		static QuaternionT<T>& fromArray(T arr[2]) noexcept;
	
		const Vector4T<T>& toVector4() const noexcept;

		Vector4T<T>& toVector4() noexcept;

		T getLength2p() const noexcept;

		T getLength() const noexcept;

		void multiply(const QuaternionT<T>& other) noexcept;

		void divide(const QuaternionT<T>& other) noexcept;

		void setRotation(const Vector3T<T>& vAxis, T fAngle) noexcept;

		static QuaternionT<T> getRotation(const Vector3T<T>& vAxis, T fAngle) noexcept;

		T getAngle() const noexcept;

		Vector4T<T> getAxis() const noexcept;

		void makeInverse() noexcept;

		QuaternionT<T> inverse() const noexcept;

	public:
		QuaternionT<T>& operator=(const QuaternionT<T>& other) noexcept = default;

		template <class O>
		QuaternionT<T>& operator=(const QuaternionT<O>& other) noexcept;

		QuaternionT<T>& operator=(const Vector4T<T>& other) noexcept;

		QuaternionT<T>& operator*=(const QuaternionT<T>& other) noexcept;

		QuaternionT<T> operator*(const QuaternionT<T>& other) const noexcept;

		QuaternionT<T>& operator/=(const QuaternionT<T>& other) noexcept;

		QuaternionT<T> operator/(QuaternionT<T>& other) const noexcept;

		sl_bool operator==(const QuaternionT<T>& other) const noexcept;

		sl_bool operator!=(const QuaternionT<T>& other) const noexcept;

	private:
		static T _identity[4];

	};
	
	extern template class QuaternionT<float>;
	extern template class QuaternionT<double>;
	typedef QuaternionT<sl_real> Quaternion;
	typedef QuaternionT<float> Quaternionf;
	typedef QuaternionT<double> Quaternionlf;

}

#include "detail/quaternion.inc"

#endif
