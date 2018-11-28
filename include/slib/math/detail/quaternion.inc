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
	SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::identity() noexcept
	{
		return *(reinterpret_cast<QuaternionT<T> const*>(&_identity));
	}

	template <class T>
	SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::fromArray(const T arr[2]) noexcept
	{
		return *(reinterpret_cast<QuaternionT<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::fromArray(T arr[2]) noexcept
	{
		return *(reinterpret_cast<QuaternionT<T>*>(arr));
	}
	
	template <class T>
	SLIB_INLINE const Vector4T<T>& QuaternionT<T>::toVector4() const noexcept
	{
		return *((Vector4T<T>*)((void*)this));
	}
	
	template <class T>
	SLIB_INLINE Vector4T<T>& QuaternionT<T>::toVector4() noexcept
	{
		return *((Vector4T<T>*)((void*)this));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::operator=(const QuaternionT<O>& other) noexcept
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::operator=(const Vector4T<T>& other) noexcept
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

}
