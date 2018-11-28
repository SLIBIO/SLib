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
	SLIB_INLINE const RectangleT<T, FT>& RectangleT<T, FT>::zero() noexcept
	{
		return *(reinterpret_cast<RectangleT<T, FT> const*>(&_zero));
	}

	template <class T, class FT>
	SLIB_INLINE T RectangleT<T, FT>::getWidth() const noexcept
	{
		return right - left;
	}
	
	template <class T, class FT>
	SLIB_INLINE T RectangleT<T, FT>::getHeight() const noexcept
	{
		return bottom - top;
	}
	
	template <class T, class FT>
	SLIB_INLINE SizeT<T, FT> RectangleT<T, FT>::getSize() const noexcept
	{
		return {right - left, bottom - top};
	}
	
	template <class T, class FT>
	template <class O, class FO>
	SLIB_INLINE RectangleT<T, FT>& RectangleT<T, FT>::operator=(const RectangleT<O, FO>& other) noexcept
	{
		left = (T)(other.left);
		top = (T)(other.top);
		right = (T)(other.right);
		bottom = (T)(other.bottom);
		return *this;
	}
	
	
	template <class T, class FT>
	SLIB_INLINE RectangleT<T, FT> Interpolation< RectangleT<T, FT> >::interpolate(const RectangleT<T, FT>& a, const RectangleT<T, FT>& b, float factor) noexcept
	{
		return a.lerp(b, factor);
	}

}
