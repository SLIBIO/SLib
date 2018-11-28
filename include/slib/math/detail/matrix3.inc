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

namespace slib
{
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::zero() noexcept
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(&_zero));
	}
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::one() noexcept
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(&_one));
	}
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::identity() noexcept
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(&_identity));
	}
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::fromArray(const T arr[9]) noexcept
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE Matrix3T<T>& Matrix3T<T>::fromArray(T arr[9]) noexcept
	{
		return *(reinterpret_cast<Matrix3T<T>*>(arr));
	}
	
	template <class T>
	template <class O>
	Matrix3T<T>& Matrix3T<T>::operator=(const Matrix3T<O>& other) noexcept
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02);
		m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12);
		m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22);
		return *this;
	}
	
	
	template <class T>
	Matrix3T<T> operator*(T value, const Matrix3T<T>& m) noexcept
	{
		Matrix3T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	template <class T>
	Vector3T<T> operator*(const Vector3T<T>& v, const Matrix3T<T>& m) noexcept
	{
		return m.multiplyLeft(v);
	}
	
	
	template <class T>
	SLIB_INLINE Matrix3T<T> Interpolation< Matrix3T<T> >::interpolate(const Matrix3T<T>& a, const Matrix3T<T>& b, float factor) noexcept
	{
		return a.lerp(b, factor);
	}

}
