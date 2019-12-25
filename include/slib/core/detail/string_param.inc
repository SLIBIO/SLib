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
	
	namespace priv
	{
		namespace string_param
		{
			struct ConstContainer
			{
				void* value;
				sl_size length;
			};

			extern const ConstContainer g_undefined;
			extern const ConstContainer g_null;
		}
	}
	
	SLIB_INLINE const StringParam& StringParam::undefined() noexcept
	{
		return *(reinterpret_cast<StringParam const*>(&(priv::string_param::g_undefined)));
	}

	SLIB_INLINE const StringParam& StringParam::null() noexcept
	{
		return *(reinterpret_cast<StringParam const*>(&(priv::string_param::g_null)));
	}
	
	template <sl_size N>
	StringParam StringParam::literal(const sl_char8 (&s)[N]) noexcept
	{
		return StringParam(s, N-1);
	}
	
	template <sl_size N>
	StringParam StringParam::literal(const sl_char16 (&s)[N]) noexcept
	{
		return StringParam(s, N-1);
	}
	
	SLIB_INLINE sl_bool StringParam::isUndefined() const noexcept
	{
		return (!_value) && (!_length);
	}

	SLIB_INLINE sl_bool StringParam::isNotUndefined() const noexcept
	{
		return _value || _length;
	}

	SLIB_INLINE sl_bool StringParam::isNull() const noexcept
	{
		return !_value;
	}
	
	SLIB_INLINE sl_bool StringParam::isNotNull() const noexcept
	{
		return _value != 0;
	}
	

	template <class... ARGS>
	String String::join(const StringParam& s, ARGS&&... args) noexcept
	{
		StringParam params[] = {s, Forward<ARGS>(args)...};
		return join(params, 1 + sizeof...(args));
	}

	template <class... ARGS>
	String16 String16::join(const StringParam& s, ARGS&&... args) noexcept
	{
		StringParam params[] = {s, Forward<ARGS>(args)...};
		return join(params, 1 + sizeof...(args));
	}

}
