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
	sl_bool Parse(const String& str, T* _out) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool Parse(const String16& str, T* _out) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}
	
	template <class T>
	sl_bool Parse(const AtomicString& _str, T* _out) noexcept
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool Parse(const AtomicString16& _str, T* _out) noexcept
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool Parse(const sl_char8* sz, T* _out) noexcept
	{
		sl_reg ret = Parser<T, sl_char8>::parse(_out, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_bool Parse(const sl_char16* sz, T* _out) noexcept
	{
		sl_reg ret = Parser<T, sl_char16>::parse(_out, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}


	template <class T>
	sl_bool ParseInt(const String& str, T* _out, sl_uint32 radix) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const String16& str, T* _out, sl_uint32 radix) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const AtomicString& _str, T* _out, sl_uint32 radix) noexcept
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const AtomicString16& _str, T* _out, sl_uint32 radix) noexcept
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const sl_char8* sz, T* _out, sl_uint32 radix ) noexcept
	{
		sl_reg ret = IntParser<T, sl_char8>::parse(_out, radix, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_bool ParseInt(const sl_char16* sz, T* _out, sl_uint32 radix) noexcept
	{
		sl_reg ret = IntParser<T, sl_char16>::parse(_out, radix, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

}
