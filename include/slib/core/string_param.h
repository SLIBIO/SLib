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

#ifndef CHECKHEADER_SLIB_CORE_STRING_PARAM
#define CHECKHEADER_SLIB_CORE_STRING_PARAM

#include "string8.h"
#include "string16.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <string>
#endif

namespace slib
{

	class Variant;
	
	class SLIB_EXPORT StringParam
	{
	public:
		sl_size _value;
		sl_size _length;
		
	public:
		SLIB_INLINE constexpr StringParam() noexcept : _value(0), _length(0) {}
		
		SLIB_INLINE constexpr StringParam(sl_null_t) noexcept : _value(0), _length(1) {}
		
		StringParam(StringParam&& other) noexcept;
		
		StringParam(const StringParam& other) noexcept;
		
		~StringParam() noexcept;
		
	public:
		StringParam(const String& value) noexcept;
		
		StringParam(String&& value) noexcept;
		
		StringParam(const String16& value) noexcept;
		
		StringParam(String16&& value) noexcept;
		
		StringParam(const AtomicString& value) noexcept;
		
		StringParam(AtomicString&& value) noexcept;
		
		StringParam(const AtomicString16& value) noexcept;
		
		StringParam(AtomicString16&& value) noexcept;
		
		StringParam(const sl_char8* sz8) noexcept;

		StringParam(const sl_char16* sz16) noexcept;

		StringParam(const sl_char8* sz8, sl_reg length) noexcept;
		
		StringParam(const sl_char16* sz16, sl_reg length) noexcept;
		
	public:
		static const StringParam& undefined() noexcept;
		
		static const StringParam& null() noexcept;
		
		template <sl_size N>
		static StringParam literal(const sl_char8 (&s)[N]) noexcept;
		
		template <sl_size N>
		static StringParam literal(const sl_char16 (&s)[N]) noexcept;
		
	public:
		StringParam& operator=(StringParam&& other) noexcept;
		
		StringParam& operator=(const StringParam& other) noexcept;

		StringParam& operator=(sl_null_t) noexcept;
		
		StringParam& operator=(const String& value) noexcept;
		
		StringParam& operator=(String&& value) noexcept;
		
		StringParam& operator=(const String16& value) noexcept;
		
		StringParam& operator=(String16&& value) noexcept;

		StringParam& operator=(const AtomicString& value) noexcept;
		
		StringParam& operator=(AtomicString&& value) noexcept;
		
		StringParam& operator=(const AtomicString16& value) noexcept;
		
		StringParam& operator=(AtomicString16&& value) noexcept;
		
		StringParam& operator=(const sl_char8* sz8) noexcept;
		
		StringParam& operator=(const sl_char16* sz16) noexcept;

	public:
		void setUndefined() noexcept;
		
		sl_bool isUndefined() const noexcept;
		
		sl_bool isNotUndefined() const noexcept;
		
		void setNull() noexcept;
		
		sl_bool isNull() const noexcept;
		
		sl_bool isNotNull() const noexcept;
		
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;

		
		sl_bool isString8() const noexcept;
		
		sl_bool isString16() const noexcept;
		
		sl_bool isSz8() const noexcept;
		
		sl_bool isSz16() const noexcept;
		
		sl_bool is8() const noexcept;

		sl_bool is16() const noexcept;
		
		
		String toString() const noexcept;
		
		String16 toString16() const noexcept;
		
		Variant toVariant() const noexcept;
		
		sl_compare_result compare(const StringParam& other) const noexcept;
		
		sl_bool equals(const StringParam& other) const noexcept;
		
		sl_size getHashCode() const noexcept;

	public:
		static void _free(sl_size value, sl_size length) noexcept;
		
		friend class StringData;
		friend class StringData16;
	};
	
	sl_bool operator==(const StringParam& v1, const StringParam& v2) noexcept;
	
	sl_bool operator!=(const StringParam& v1, const StringParam& v2) noexcept;
	
	
	template <>
	class Compare<StringParam>
	{
	public:
		sl_compare_result operator()(const StringParam &a, const StringParam &b) const noexcept;
	};
	
	template <>
	class Equals<StringParam>
	{
	public:
		sl_bool operator()(const StringParam &a, const StringParam &b) const noexcept;
	};
	
	template <>
	class Hash<StringParam>
	{
	public:
		sl_size operator()(const StringParam &a) const noexcept;
	};

	class SLIB_EXPORT StringData
	{
	public:
		sl_char8* data;
		String string;
		
	public:
		StringData(const StringParam& param) noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(StringData)
		
	public:
		SLIB_INLINE sl_char8* getData() const noexcept
		{
			return data;
		}
		
		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		sl_size getLength() const noexcept;
		
		sl_size getLengthForParser() const noexcept;
		
	private:
		mutable sl_size length;
		
	};

	class SLIB_EXPORT StringData16
	{
	public:
		sl_char16* data;
		String16 string;
		
	public:
		StringData16(const StringParam& param) noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(StringData16)
		
	public:
		SLIB_INLINE sl_char16* getData() const noexcept
		{
			return data;
		}
		
		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		sl_size getLength() const noexcept;
		
		sl_size getLengthForParser() const noexcept;

	private:
		mutable sl_size length;			
	};
	
}

#include "detail/string_param.inc"

#endif
