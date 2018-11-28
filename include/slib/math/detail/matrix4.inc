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
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::zero() noexcept
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(&_zero));
	}
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::one() noexcept
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(&_one));
	}
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::identity() noexcept
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(&_identity));
	}
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::fromArray(const T arr[16]) noexcept
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE Matrix4T<T>& Matrix4T<T>::fromArray(T arr[16]) noexcept
	{
		return *(reinterpret_cast<Matrix4T<T>*>(arr));
	}
	
	template <class T>
	template <class O>
	Matrix4T<T>& Matrix4T<T>::operator=(const Matrix4T<O>& other) noexcept
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02); m03 = (T)(other.m03);
		m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12); m13 = (T)(other.m13);
		m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22); m23 = (T)(other.m23);
		m30 = (T)(other.m30); m31 = (T)(other.m31); m32 = (T)(other.m32); m33 = (T)(other.m33);
		return *this;
	}
	
	template <class T>
	Matrix4T<T> operator*(T value, const Matrix4T<T>& m) noexcept
	{
		Matrix4T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	template <class T>
	Vector4T<T> operator*(const Vector4T<T>& v, const Matrix4T<T>& m) noexcept
	{
		return m.multiplyLeft(v);
	}
	
	
	template <class T>
	SLIB_INLINE Matrix4T<T> Interpolation< Matrix4T<T> >::interpolate(const Matrix4T<T>& a, const Matrix4T<T>& b, float factor) noexcept
	{
		return a.lerp(b, factor);
	}

}
