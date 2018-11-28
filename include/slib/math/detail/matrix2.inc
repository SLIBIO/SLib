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
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::zero() noexcept
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(&_zero));
	}
	
	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::one() noexcept
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(&_one));
	}
	
	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::identity() noexcept
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(&_identity));
	}
	
	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::fromArray(const T arr[4]) noexcept
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE Matrix2T<T>& Matrix2T<T>::fromArray(T arr[4]) noexcept
	{
		return *(reinterpret_cast<Matrix2T<T>*>(arr));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Matrix2T<T>& Matrix2T<T>::operator=(const Matrix2T<O>& other) noexcept
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01);
		m10 = (T)(other.m10); m11 = (T)(other.m11);
		return *this;
	}
	
	template <class T>
	Matrix2T<T> operator*(T value, const Matrix2T<T>& m) noexcept
	{
		Matrix2T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	template <class T>
	Vector2T<T> operator*(const Vector2T<T>& v, const Matrix2T<T>& m) noexcept
	{
		return m.multiplyLeft(v);
	}
	
	
	template <class T>
	SLIB_INLINE Matrix2T<T> Interpolation< Matrix2T<T> >::interpolate(const Matrix2T<T>& a, const Matrix2T<T>& b, float factor) noexcept
	{
		return a.lerp(b, factor);
	}

}
