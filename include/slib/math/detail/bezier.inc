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
	template <class O>
	CubicBezierCurveT<T>::CubicBezierCurveT(const CubicBezierCurveT<O>& other) noexcept
	 :x0((T)(other.x0)), y0((T)(other.y0)),
	  x1((T)(other.x1)), y1((T)(other.y1)),
	  x2((T)(other.x2)), y2((T)(other.y2)),
	  x3((T)(other.x3)), y3((T)(other.y3))
	{
	}

	template <class T>
	template <class O>
	CubicBezierCurveT<T>& CubicBezierCurveT<T>::operator=(const CubicBezierCurveT<O>& other) noexcept
	{
		x0 = (T)(other.x0);
		y0 = (T)(other.y0);
		x1 = (T)(other.x1);
		y1 = (T)(other.y1);
		x2 = (T)(other.x2);
		y2 = (T)(other.y2);
		x3 = (T)(other.x3);
		y3 = (T)(other.y3);
		return *this;
	}

}
