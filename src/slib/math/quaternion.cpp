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

#include "slib/math/quaternion.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T>
	SLIB_ALIGN(8) T QuaternionT<T>::_identity[4] = { 0, 0, 0, 1 };

	template <class T>
	T QuaternionT<T>::getLength2p() const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	template <class T>
	T QuaternionT<T>::getLength() const noexcept
	{
		return Math::sqrt(x * x + y * y + z * z + w * w);
	}

	template <class T>
	void QuaternionT<T>::multiply(const QuaternionT<T>& other) noexcept
	{
		T ox = other.x, oy = other.y, oz = other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}

	template <class T>
	void QuaternionT<T>::divide(const QuaternionT<T>& other) noexcept
	{
		T ox = -other.x, oy = -other.y, oz = -other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}

	template <class T>
	void QuaternionT<T>::setRotation(const Vector3T<T>& vAxis, T fAngle) noexcept
	{
		T f = Math::sin(fAngle / 2) / vAxis.getLength();
		x = vAxis.x * f;
		y = vAxis.y * f;
		z = vAxis.z * f;
		w = Math::cos(fAngle / 2);
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::getRotation(const Vector3T<T>& vAxis, T fAngle) noexcept
	{
		QuaternionT<T> ret;
		ret.setRotation(vAxis, fAngle);
		return ret;
	}

	template <class T>
	T QuaternionT<T>::getAngle() const noexcept
	{
		return 2 * Math::arccos(w);
	}

	template <class T>
	Vector4T<T> QuaternionT<T>::getAxis() const noexcept
	{
		return {x, y, z, 0};
	}

	template <class T>
	void QuaternionT<T>::makeInverse() noexcept
	{
		x = -x;
		y = -y;
		z = -z;
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::inverse() const noexcept
	{
		QuaternionT<T> ret = *this;
		ret.makeInverse();
		return ret;
	}

	template <class T>
	QuaternionT<T>& QuaternionT<T>::operator*=(const QuaternionT<T>& other) noexcept
	{
		multiply(other);
		return *this;
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::operator*(const QuaternionT<T>& other) const noexcept
	{
		QuaternionT<T> ret = *this;
		ret.multiply(other);
		return ret;
	}

	template <class T>
	QuaternionT<T>& QuaternionT<T>::operator/=(const QuaternionT<T>& other) noexcept
	{
		divide(other);
		return *this;
	}

	template <class T>
	QuaternionT<T> QuaternionT<T>::operator/(QuaternionT<T>& other) const noexcept
	{
		QuaternionT<T> ret = *this;
		ret.divide(other);
		return ret;
	}

	template <class T>
	sl_bool QuaternionT<T>::operator==(const QuaternionT<T>& other) const noexcept
	{
		return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
	}

	template <class T>
	sl_bool QuaternionT<T>::operator!=(const QuaternionT<T>& other) const noexcept
	{
		return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Quaternion)

}
