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
	
	template <class T, class FT>
	SLIB_INLINE const Vector3T<T, FT>& Vector3T<T, FT>::zero() noexcept
	{
		return *(reinterpret_cast<Vector3T<T, FT> const*>(&_zero));
	}
	
	template <class T, class FT>
	SLIB_INLINE const Vector3T<T, FT>& Vector3T<T, FT>::fromArray(const T arr[3]) noexcept
	{
		return *(reinterpret_cast<Vector3T<T, FT> const*>(arr));
	}
	
	template <class T, class FT>
	SLIB_INLINE Vector3T<T, FT>& Vector3T<T, FT>::fromArray(T arr[3]) noexcept
	{
		return *(reinterpret_cast<Vector3T<T, FT>*>(arr));
	}
	
	template <class T, class FT>
	template <class O, class FO>
	SLIB_INLINE Vector3T<T, FT>& Vector3T<T, FT>::operator=(const Vector3T<O, FO>& other) noexcept
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		return *this;
	}
	
	template <class T, class FT>
	Vector3T<T, FT> operator*(T f, const Vector3T<T, FT>& v) noexcept
	{
		return {f * v.x, f * v.y, f * v.z};
	}
	
	template <class T, class FT>
	Vector3T<T, FT> operator/(T f, const Vector3T<T, FT>& v) noexcept
	{
		return {f / v.x, f / v.y, f / v.z};
	}
	
	
	template <class T, class FT>
	SLIB_INLINE Vector3T<T, FT> Interpolation< Vector3T<T, FT> >::interpolate(const Vector3T<T, FT>& a, const Vector3T<T, FT>& b, float factor) noexcept
	{
		return a.lerp(b, factor);
	}

}
