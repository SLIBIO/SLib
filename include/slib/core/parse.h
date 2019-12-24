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

#ifndef CHECKHEADER_SLIB_CORE_PARSE
#define CHECKHEADER_SLIB_CORE_PARSE

#include "definition.h"

#include "string_param.h"

#define SLIB_PARSE_ERROR (-1)

namespace slib
{

	template <class ObjectType, class CharType>
	class Parser
	{
	public:
		static sl_reg parse(ObjectType* _out, CharType const* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;

	};

	template <class ObjectType, class CharType, class ArgType>
	class Parser2
	{
	public:
		static sl_reg parse(ObjectType* _out, const ArgType& arg, CharType const* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
	};

	template <class ObjectType, class CharType>
	class IntParser
	{
	public:
		static sl_reg parse(ObjectType* _out, sl_uint32 radix, CharType const* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;

	};


	template <class T>
	sl_bool Parse(const StringParam& str, T* _out) noexcept;

	template <class T, class ArgType>
	sl_bool Parse(const StringParam& str, const ArgType& arg, T* _out) noexcept;
	
	template <class T>
	sl_bool ParseInt(const StringParam& str, T* _out, sl_uint32 radix = 10) noexcept;


	class ParseUtil
	{
	public:
		static String applyBackslashEscapes(const StringParam& str, sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false) noexcept;

		static String16 applyBackslashEscapes16(const StringParam& str, sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false) noexcept;

		
		static String parseBackslashEscapes(const StringParam& input, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) noexcept;

		static String16 parseBackslashEscapes16(const StringParam& input, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) noexcept;

		
		static sl_size countLineNumber(const StringParam& input, sl_size pos, sl_size* column = sl_null) noexcept;
		
		static sl_size countLineNumber(const StringParam& input, sl_size* column = sl_null) noexcept;

		static sl_reg indexOfLine(const StringParam& input, sl_reg start = 0) noexcept;
		
	};

}

#include "detail/parse.inc"

#endif
