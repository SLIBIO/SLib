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
	
	SLIB_INLINE const Color& Color::zero() noexcept
	{
		return *(reinterpret_cast<Color const*>(&_zero));
	}
	
	SLIB_INLINE void Color::setZero() noexcept
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
	
	SLIB_INLINE void Color::setRGBA(sl_uint8 _r, sl_uint8 _g, sl_uint8 _b, sl_uint8 _a) noexcept
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	
	SLIB_INLINE void Color::setARGB(sl_uint32 v) noexcept
	{
		b = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		r = (sl_uint8)((v >> 16) & 0xFF);
		a = (sl_uint8)((v >> 24) & 0xFF);
	}
	
	SLIB_INLINE void Color::setABGR(sl_uint32 v) noexcept
	{
		r = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		b = (sl_uint8)((v >> 16) & 0xFF);
		a = (sl_uint8)((v >> 24) & 0xFF);
	}
	
	SLIB_INLINE void Color::setRGB(sl_uint8 _r, sl_uint8 _g, sl_uint8 _b) noexcept
	{
		r = _r;
		g = _g;
		b = _b;
	}
	
	SLIB_INLINE void Color::setRGB(sl_uint32 v) noexcept
	{
		b = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		r = (sl_uint8)((v >> 16) & 0xFF);
	}
	
	SLIB_INLINE void Color::setBGR(sl_uint32 v) noexcept
	{
		r = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		b = (sl_uint8)((v >> 16) & 0xFF);
	}

	SLIB_INLINE Color& Color::operator=(const Color& other) noexcept
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
		return *this;
	}

}
