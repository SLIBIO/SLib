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

	SLIB_INLINE MacAddress::MacAddress(const MacAddress& other) noexcept = default;

	SLIB_INLINE MacAddress& MacAddress::operator=(const MacAddress& other) noexcept = default;


	SLIB_INLINE const MacAddress& MacAddress::zero() noexcept
	{
		return *(reinterpret_cast<MacAddress const*>(&_zero));
	}
	
	SLIB_INLINE sl_bool MacAddress::isZero() const noexcept
	{
		return m[0] == 0 && m[1] == 0 && m[2] == 0 && m[3] == 0 && m[4] == 0 && m[5] == 0;
	}
	
	SLIB_INLINE sl_bool MacAddress::isNotZero() const noexcept
	{
		return m[0] != 0 || m[1] != 0 || m[2] != 0 || m[3] != 0 || m[4] != 0 || m[5] != 0;
	}
	
	SLIB_INLINE const MacAddress& MacAddress::getBroadcast() noexcept
	{
		return *(reinterpret_cast<MacAddress const*>(&_broadcast));
	}
	
	SLIB_INLINE sl_bool MacAddress::isBroadcast() const noexcept
	{
		return m[0] == 255 && m[1] == 255 && m[2] == 255 && m[3] == 255 && m[4] == 255 && m[5] == 255;
	}
	
	SLIB_INLINE sl_bool MacAddress::isNotBroadcast() const noexcept
	{
		return m[0] != 255 || m[1] != 255 || m[2] != 255 || m[3] != 255 || m[4] != 255 || m[5] != 255;
	}
	
	SLIB_INLINE sl_bool MacAddress::isMulticast() const noexcept
	{
		return (m[0] & 1);
	}
	
	SLIB_INLINE sl_bool MacAddress::isNotMulticast() const noexcept
	{
		return (m[0] & 1) == 0;
	}

	SLIB_INLINE sl_bool MacAddress::operator==(const MacAddress& other) const noexcept
	{
		return m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2] && m[3] == other.m[3] && m[4] == other.m[4] && m[5] == other.m[5];
	}
	
	SLIB_INLINE sl_bool MacAddress::operator!=(const MacAddress& other) const noexcept
	{
		return m[0] != other.m[0] || m[1] != other.m[1] || m[2] != other.m[2] || m[3] != other.m[3] || m[4] != other.m[4] || m[5] != other.m[5];
	}

}
