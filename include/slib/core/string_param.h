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
	
	// Same as VariantType values
	enum class StringType
	{
		Null = 0,
		String8 = 8,
		String16 = 9,
		Sz8 = 10,
		Sz16 = 11
	};
	
	class SLIB_EXPORT StringParam
	{
	public:
		sl_size _value;
		StringType _type;
		
	public:
		SLIB_INLINE constexpr StringParam() noexcept : _value(0), _type(StringType::Null) {}
		
		SLIB_INLINE constexpr StringParam(sl_null_t) noexcept : _value(1), _type(StringType::Null) {}
		
		StringParam(StringParam&& other) noexcept;
		
		StringParam(const StringParam& other) noexcept;
		
		~StringParam() noexcept;
		
	public:
		StringParam(const String& value) noexcept;
		
		StringParam(const String16& value) noexcept;
		
		StringParam(const AtomicString& value) noexcept;
		
		StringParam(const AtomicString16& value) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		StringParam(const std::string& value) noexcept;
		
		StringParam(const std::u16string& value) noexcept;
#endif
		
		StringParam(const sl_char8* sz8) noexcept;
		
		StringParam(const sl_char16* sz16) noexcept;
		
		
	public:
		static const StringParam& undefined() noexcept;
		
		static const StringParam& null() noexcept;
		
		
		static StringParam fromString(const String& value) noexcept;
		
		static StringParam fromString16(const String16& value) noexcept;
		
		static StringParam fromSz8(const sl_char8* value) noexcept;
		
		static StringParam fromSz16(const sl_char16* value) noexcept;
		
	public:
		StringParam& operator=(StringParam&& other) noexcept;
		
		StringParam& operator=(const StringParam& other) noexcept;

		StringParam& operator=(sl_null_t) noexcept;
		
		StringParam& operator=(const String& value) noexcept;
		
		StringParam& operator=(const String16& value) noexcept;
		
		StringParam& operator=(const AtomicString& value) noexcept;
		
		StringParam& operator=(const AtomicString16& value) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		StringParam& operator=(const std::string& value) noexcept;
		
		StringParam& operator=(const std::u16string& value) noexcept;
#endif
		
		StringParam& operator=(const sl_char8* sz8) noexcept;
		
		StringParam& operator=(const sl_char16* sz16) noexcept;

		
	public:
		StringType getType() const noexcept;
		
		void setUndefined() noexcept;
		
		sl_bool isUndefined() const noexcept;
		
		sl_bool isNotUndefined() const noexcept;
		
		void setNull() noexcept;
		
		sl_bool isNull() const noexcept;
		
		sl_bool isNotNull() const noexcept;
		
		
		sl_size getLength() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;

		
		sl_bool isString8() const noexcept;
		
		sl_bool isString16() const noexcept;
		
		sl_bool isSz8() const noexcept;
		
		sl_bool isSz16() const noexcept;
		
		String getString(const String& def) const noexcept;
		
		String getString() const noexcept;
		
		String16 getString16(const String16& def) const noexcept;
		
		String16 getString16() const noexcept;
		
		sl_char8* getSz8(const sl_char8* def = sl_null) const noexcept;
		
		sl_char16* getSz16(const sl_char16* def = sl_null) const noexcept;
		
		void setString(const String& value) noexcept;
		
		void setString16(const String16& value) noexcept;
		
		void setSz(const sl_char8* sz8) noexcept;
		
		void setSz(const sl_char16* sz16) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		std::string getStdString(const std::string& def) const noexcept;
		
		std::string getStdString() const noexcept;
		
		std::u16string getStdString16(const std::u16string& def) const noexcept;
		
		std::u16string getStdString16() const noexcept;
		
		void setStdString(const std::string& value) noexcept;
		
		void setStdString16(const std::u16string& value) noexcept;
#endif
		
		sl_compare_result compare(const StringParam& other) const noexcept;
		
		sl_bool equals(const StringParam& other) const noexcept;
		
		sl_size getHashCode() const noexcept;

	public:
		static void _free(StringType type, sl_size value) noexcept;
		
		friend class StringParamData;
		friend class StringParamData16;
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

	class SLIB_EXPORT StringParamData
	{
	public:
		sl_char8* data;
		String string;
		
	public:
		StringParamData(const StringParam& param) noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(StringParamData)
		
	};

	class SLIB_EXPORT StringParamData16
	{
	public:
		sl_char16* data;
		String16 string;
		
	public:
		StringParamData16(const StringParam& param) noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(StringParamData16)
		
	};
	
}

#include "detail/string_param.inc"

#endif
