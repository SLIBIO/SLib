/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#define SLIB_SUPPORT_STD_TYPES

#include "slib/core/string.h"

namespace slib
{

	StringView::StringView(const String& value) noexcept
	{
		if (value.isNotNull()) {
			value.getData(*((sl_size*)&length));
		} else {
			data = sl_null;
			length = 0;
		}
	}

	StringView16::StringView16(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			value.getData(*((sl_size*)&length));
		} else {
			data = sl_null;
			length = 0;
		}
	}

	StringView::StringView(const sl_char8* str) noexcept
	{
		data = (sl_char8*)str;
		if (str) {
			length = -1;
		} else {
			length = 0;
		}
	}

	StringView16::StringView16(const sl_char16* str) noexcept
	{
		data = (sl_char16*)str;
		if (str) {
			length = -1;
		} else {
			length = 0;
		}
	}

	StringView::StringView(const sl_char8* str, sl_reg _length) noexcept
	{
		data = (sl_char8*)str;
		if (str) {
			length = _length;
		} else {
			length = 0;
		}
	}

	StringView16::StringView16(const sl_char16* str, sl_reg _length) noexcept
	{
		data = (sl_char16*)str;
		if (str) {
			length = _length;
		} else {
			length = 0;
		}
	}

	StringView::StringView(const std::string& str) noexcept: data((sl_char8*)(str.c_str())), length((sl_reg)(str.length()))
	{
	}

	StringView16::StringView16(const std::u16string& str) noexcept: data((sl_char16*)(str.c_str())), length((sl_reg)(str.length()))
	{
	}

	StringView& StringView::operator=(sl_null_t) noexcept
	{
		data = sl_null;
		length = 0;
		return *this;
	}

	StringView16& StringView16::operator=(sl_null_t) noexcept
	{
		data = sl_null;
		length = 0;
		return *this;
	}

	StringView& StringView::operator=(const StringView& other) noexcept
	{
		data = other.data;
		length = other.length;
		return *this;
	}

	StringView16& StringView16::operator=(const StringView16& other) noexcept
	{
		data = other.data;
		length = other.length;
		return *this;
	}

	StringView& StringView::operator=(const String& value) noexcept
	{
		if (value.isNotNull()) {
			value.getData(*((sl_size*)&length));
		} else {
			data = sl_null;
			length = 0;
		}
		return *this;
	}

	StringView16& StringView16::operator=(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			value.getData(*((sl_size*)&length));
		} else {
			data = sl_null;
			length = 0;
		}
		return *this;
	}

	StringView& StringView::operator=(const sl_char8* str) noexcept
	{
		data = (sl_char8*)str;
		if (str) {
			length = -1;
		} else {
			length = 0;
		}
		return *this;
	}

	StringView16& StringView16::operator=(const sl_char16* str) noexcept
	{
		data = (sl_char16*)str;
		if (str) {
			length = -1;
		} else {
			length = 0;
		}
		return *this;
	}

	StringView& StringView::operator=(const std::string& str) noexcept
	{
		data = (sl_char8*)(str.c_str());
		length = (sl_reg)(str.length());
		return *this;
	}

	StringView16& StringView16::operator=(const std::u16string& str) noexcept
	{
		data = (sl_char16*)(str.c_str());
		length = (sl_reg)(str.length());
		return *this;
	}

	sl_size StringView::getLength() const noexcept
	{
		if (length < 0) {
			length = Base::getStringLength(data);
		}
		return length;
	}

	sl_size StringView16::getLength() const noexcept
	{
		if (length < 0) {
			length = Base::getStringLength2(data);
		}
		return length;
	}


	sl_compare_result Compare<StringView>::operator()(const StringView& a, const StringView& b) const noexcept
	{
		return a.compare(b);
	}

	sl_compare_result Compare<StringView16>::operator()(const StringView16& a, const StringView16& b) const noexcept
	{
		return a.compare(b);
	}

	sl_bool Equals<StringView>::operator()(const StringView& a, const StringView& b) const noexcept
	{
		return a.equals(b);
	}

	sl_bool Equals<StringView16>::operator()(const StringView16& a, const StringView16& b) const noexcept
	{
		return a.equals(b);
	}

	sl_size Hash<StringView>::operator()(const StringView& a) const noexcept
	{
		return a.getHashCode();
	}

	sl_size Hash<StringView16>::operator()(const StringView16& a) const noexcept
	{
		return a.getHashCode();
	}

}
