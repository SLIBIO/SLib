/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_DETAIL_PARSE
#define CHECKHEADER_SLIB_CORE_DETAIL_PARSE

#include "../parse.h"

namespace slib
{
	
	template <class T>
	sl_bool Parse(const String& str, T* _out)
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool Parse(const String16& str, T* _out)
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}
	
	template <class T>
	sl_bool Parse(const AtomicString& _str, T* _out)
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool Parse(const AtomicString16& _str, T* _out)
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool Parse(const sl_char8* sz, T* _out)
	{
		sl_reg ret = Parser<T, sl_char8>::parse(_out, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_bool Parse(const sl_char16* sz, T* _out)
	{
		sl_reg ret = Parser<T, sl_char16>::parse(_out, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}


	template <class T>
	sl_bool ParseInt(const String& str, T* _out, sl_uint32 radix)
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const String16& str, T* _out, sl_uint32 radix)
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const AtomicString& _str, T* _out, sl_uint32 radix)
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const AtomicString16& _str, T* _out, sl_uint32 radix)
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == (sl_reg)n;
	}

	template <class T>
	sl_bool ParseInt(const sl_char8* sz, T* _out, sl_uint32 radix )
	{
		sl_reg ret = IntParser<T, sl_char8>::parse(_out, radix, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_bool ParseInt(const sl_char16* sz, T* _out, sl_uint32 radix)
	{
		sl_reg ret = IntParser<T, sl_char16>::parse(_out, radix, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

}

#endif
