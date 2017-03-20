/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/math/quaternion.h"

#include "../../../inc/slib/core/math.h"

namespace slib
{

	template <class T>
	SLIB_ALIGN(8) T QuaternionT<T>::_identity[4] = { 0, 0, 0, 1 };

	template <class T>
	T QuaternionT<T>::getLength2p() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <class T>
	T QuaternionT<T>::getLength() const
	{
		return Math::sqrt(x * x + y * y + z * z + w * w);
	}

	template <class T>
	void QuaternionT<T>::multiply(const QuaternionT<T>& other)
	{
		T ox = other.x, oy = other.y, oz = other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}

	template <class T>
	void QuaternionT<T>::divide(const QuaternionT<T>& other)
	{
		T ox = -other.x, oy = -other.y, oz = -other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}

	template <class T>
	void QuaternionT<T>::setRotation(const Vector3T<T>& vAxis, T fAngle)
	{
		T f = Math::sin(fAngle / 2) / vAxis.getLength();
		x = vAxis.x * f;
		y = vAxis.y * f;
		z = vAxis.z * f;
		w = Math::cos(fAngle / 2);
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::getRotation(const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> ret;
		ret.setRotation(vAxis, fAngle);
		return ret;
	}

	template <class T>
	T QuaternionT<T>::getAngle() const
	{
		return 2 * Math::arccos(w);
	}

	template <class T>
	Vector4T<T> QuaternionT<T>::getAxis() const
	{
		return {x, y, z, 0};
	}

	template <class T>
	void QuaternionT<T>::makeInverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::inverse() const
	{
		QuaternionT<T> ret = *this;
		ret.makeInverse();
		return ret;
	}

	template <class T>
	QuaternionT<T>& QuaternionT<T>::operator*=(const QuaternionT<T>& other)
	{
		multiply(other);
		return *this;
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::operator*(const QuaternionT<T>& other) const
	{
		QuaternionT<T> ret = *this;
		ret.multiply(other);
		return ret;
	}

	template <class T>
	QuaternionT<T>& QuaternionT<T>::operator/=(const QuaternionT<T>& other)
	{
		divide(other);
		return *this;
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::operator/(QuaternionT<T>& other) const
	{
		QuaternionT<T> ret = *this;
		ret.divide(other);
		return ret;
	}

	template <class T>
	sl_bool QuaternionT<T>::operator==(const QuaternionT<T>& other) const
	{
		return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
	}

	template <class T>
	sl_bool QuaternionT<T>::operator!=(const QuaternionT<T>& other) const
	{
		return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Quaternion)

}
