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
		QuaternionT() = default;

		constexpr QuaternionT(const QuaternionT<T>& other):
		 x(other.x), y(other.y), z(other.z), w(other.w)
		{}

		template <class O>
		constexpr QuaternionT(const QuaternionT<O>& other):
		 x((T)(other.x)), y((T)(other.y)), z((T)(other.z)), w((T)(other.w))
		{}
	
		QuaternionT(T _x, T _y, T _z, T _w):
		 x(_x), y(_y), z(_z), w(_w)
		{}

		QuaternionT(const Vector4T<T>& other):
		 x(other.x), y(other.y), z(other.z), w(other.w)
		{}
	
	public:
		static const QuaternionT<T>& identity();

		static const QuaternionT<T>& fromArray(const T arr[2]);

		static QuaternionT<T>& fromArray(T arr[2]);
	
		const Vector4T<T>& toVector4() const;

		Vector4T<T>& toVector4();

		T getLength2p() const;

		T getLength() const;

		void multiply(const QuaternionT<T>& other);

		void divide(const QuaternionT<T>& other);

		void setRotation(const Vector3T<T>& vAxis, T fAngle);

		static QuaternionT<T> getRotation(const Vector3T<T>& vAxis, T fAngle);

		T getAngle() const;

		Vector4T<T> getAxis() const;

		void makeInverse();

		QuaternionT<T> inverse() const;

	public:
		QuaternionT<T>& operator=(const QuaternionT<T>& other) = default;

		template <class O>
		QuaternionT<T>& operator=(const QuaternionT<O>& other);

		QuaternionT<T>& operator=(const Vector4T<T>& other);

		QuaternionT<T>& operator*=(const QuaternionT<T>& other);

		QuaternionT<T> operator*(const QuaternionT<T>& other) const;

		QuaternionT<T>& operator/=(const QuaternionT<T>& other);

		QuaternionT<T> operator/(QuaternionT<T>& other) const;

		sl_bool operator==(const QuaternionT<T>& other) const;

		sl_bool operator!=(const QuaternionT<T>& other) const;

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
