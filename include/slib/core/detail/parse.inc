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
	sl_bool Parse(const StringParam& _str, T* _out) noexcept
	{
		if (_str.isNotNull()) {
			if (_str.is8()) {
				StringData str(_str);
				sl_size n = str.getUnsafeLength();
				if (n) {
					if (n & SLIB_SIZE_TEST_SIGN_BIT) {
						const sl_char8* data = str.getData();
						sl_reg ret = Parser<T, sl_char8>::parse(_out, data, 0, n);
						return ret != SLIB_PARSE_ERROR && data[ret] == 0;
					} else {
						return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
					}
				}
			} else {
				StringData16 str(_str);
				sl_size n = str.getUnsafeLength();
				if (n) {
					if (n & SLIB_SIZE_TEST_SIGN_BIT) {
						const sl_char16* data = str.getData();
						sl_reg ret = Parser<T, sl_char16>::parse(_out, str.getData(), 0, n);
						return ret != SLIB_PARSE_ERROR && data[ret] == 0;
					} else {
						return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
					}
				}
			}
		}
		return sl_false;
	}

	template <class T, class ArgType>
	sl_bool Parse(const StringParam& _str, const ArgType& arg, T* _out) noexcept
	{
		if (_str.isNotNull()) {
			if (_str.is8()) {
				StringData str(_str);
				sl_size n = str.getUnsafeLength();
				if (n) {
					if (n & SLIB_SIZE_TEST_SIGN_BIT) {
						const sl_char8* data = str.getData();
						sl_reg ret = Parser2<T, sl_char8, ArgType>::parse(_out, arg, str.getData(), 0, n);
						return ret != SLIB_PARSE_ERROR && data[ret] == 0;
					} else {
						return Parser2<T, sl_char8, ArgType>::parse(_out, arg, str.getData(), 0, n) == (sl_reg)n;
					}
				}
			} else {
				StringData16 str(_str);
				sl_size n = str.getUnsafeLength();
				if (n) {
					if (n & SLIB_SIZE_TEST_SIGN_BIT) {
						const sl_char16* data = str.getData();
						sl_reg ret = Parser2<T, sl_char16, ArgType>::parse(_out, arg, str.getData(), 0, n);
						return ret != SLIB_PARSE_ERROR && data[ret] == 0;
					} else {
						return Parser2<T, sl_char16, ArgType>::parse(_out, arg, str.getData(), 0, n) == (sl_reg)n;
					}
				}
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool ParseInt(const StringParam& _str, T* _out, sl_uint32 radix) noexcept
	{
		if (_str.isNotNull()) {
			if (_str.is8()) {
				StringData str(_str);
				sl_size n = str.getUnsafeLength();
				if (n) {
					if (n & SLIB_SIZE_TEST_SIGN_BIT) {
						const sl_char8* data = str.getData();
						sl_reg ret = IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n);
						return ret != SLIB_PARSE_ERROR && data[ret] == 0;
					} else {
						return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
					}
				}
			} else {
				StringData16 str(_str);
				sl_size n = str.getUnsafeLength();
				if (n) {
					if (n & SLIB_SIZE_TEST_SIGN_BIT) {
						const sl_char16* data = str.getData();
						sl_reg ret = IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n);
						return ret != SLIB_PARSE_ERROR && data[ret] == 0;
					} else {
						return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
					}
				}
			}
		}
		return sl_false;
	}

}
