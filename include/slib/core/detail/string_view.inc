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

	SLIB_INLINE sl_char8 StringView::operator[](sl_size index) const noexcept
	{
		return data[index];
	}
	
	SLIB_INLINE sl_char8& StringView::operator[](sl_size index) noexcept
	{
		return data[index];
	}
	
	template <sl_size N>
	SLIB_INLINE StringView StringView::literal(const sl_char8 (&s)[N]) noexcept
	{
		return StringView(s, N - 1);
	}
	
	SLIB_INLINE sl_bool StringView::isNull() const noexcept
	{
		return !data;
	}

	SLIB_INLINE sl_bool StringView::isNotNull() const noexcept
	{
		return data != sl_null;
	}

	SLIB_INLINE sl_bool StringView::isEmpty() const noexcept
	{
		if (length) {
			if (length > 0) {
				return sl_false;
			}
			return !(*data);
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool StringView::isNotEmpty() const noexcept
	{
		return !(isEmpty());
	}

	SLIB_INLINE sl_char8* StringView::getUnsafeData() const noexcept
	{
		return data;
	}

	SLIB_INLINE sl_reg StringView::getUnsafeLength() const noexcept
	{
		return length;
	}

	SLIB_INLINE sl_char8* StringView::getData() const noexcept
	{
		if (data) {
			return data;
		} else {
			return (sl_char8*)((void*)(""));
		}
	}
	

	SLIB_INLINE sl_char16 StringView16::operator[](sl_size index) const noexcept
	{
		return data[index];
	}
	
	SLIB_INLINE sl_char16& StringView16::operator[](sl_size index) noexcept
	{
		return data[index];
	}
	
	template <sl_size N>
	SLIB_INLINE StringView16 StringView16::literal(const sl_char16 (&s)[N]) noexcept
	{
		return StringView16(s, N - 1);
	}
	
	SLIB_INLINE sl_bool StringView16::isNull() const noexcept
	{
		return !data;
	}

	SLIB_INLINE sl_bool StringView16::isNotNull() const noexcept
	{
		return data != sl_null;
	}

	SLIB_INLINE sl_bool StringView16::isEmpty() const noexcept
	{
		if (length) {
			if (length > 0) {
				return sl_false;
			}
			return !(*data);
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool StringView16::isNotEmpty() const noexcept
	{
		return !(isEmpty());
	}

	SLIB_INLINE sl_char16* StringView16::getUnsafeData() const noexcept
	{
		return data;
	}

	SLIB_INLINE sl_reg StringView16::getUnsafeLength() const noexcept
	{
		return length;
	}

	SLIB_INLINE sl_char16* StringView16::getData() const noexcept
	{
		if (data) {
			return data;
		} else {
			return (sl_char16*)((void*)(u""));
		}
	}

}
