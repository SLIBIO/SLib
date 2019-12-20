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
		String8 = 1,
		String16 = 2,
		Sz8 = 3,
		Sz16 = 4,
		StringRef8 = 5,
		StringRef16 = 6,
		Std8 = 7,
		Std16 = 8
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
		
		StringParam(String&& value) noexcept;
		
		StringParam(const String16& value) noexcept;
		
		StringParam(String16&& value) noexcept;
		
		StringParam(const AtomicString& value) noexcept;
		
		StringParam(AtomicString&& value) noexcept;
		
		StringParam(const AtomicString16& value) noexcept;
		
		StringParam(AtomicString16&& value) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		StringParam(const std::string& value) noexcept;
		
		StringParam(std::string&& value) = delete;
		
		StringParam(const std::u16string& value) noexcept;

		StringParam(std::u16string&& value) = delete;
#endif
		
		StringParam(const sl_char8* sz8) noexcept;
		
		StringParam(const sl_char16* sz16) noexcept;
		
	public:
		static const StringParam& undefined() noexcept;
		
		static const StringParam& null() noexcept;
		
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
		
#ifdef SLIB_SUPPORT_STD_TYPES
		StringParam& operator=(const std::string& value) noexcept;
		
		StringParam& operator=(std::string&& value) = delete;
		
		StringParam& operator=(const std::u16string& value) noexcept;
		
		StringParam& operator=(std::u16string&& value) = delete;
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
		
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;

		
		sl_bool isString8() const noexcept;
		
		sl_bool isString16() const noexcept;
		
		sl_bool isSz8() const noexcept;
		
		sl_bool isSz16() const noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool isStdString() const noexcept;
		
		sl_bool isStdString16() const noexcept;
#endif
		
		String getString(const String& def) const noexcept;
		
		String getString() const noexcept;
		
		String16 getString16(const String16& def) const noexcept;
		
		String16 getString16() const noexcept;
		
		
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
		
	public:
		SLIB_INLINE sl_char8* getData() const noexcept
		{
			return data;
		}
		
		sl_size getLength() const noexcept;
		
	private:
		sl_size _length;
		
	};

	class SLIB_EXPORT StringParamData16
	{
	public:
		sl_char16* data;
		String16 string;
		
	public:
		StringParamData16(const StringParam& param) noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(StringParamData16)
		
	public:
		SLIB_INLINE sl_char16* getData() const noexcept
		{
			return data;
		}
		
		sl_size getLength() const noexcept;

	private:
		sl_size _length;			
	};
	
}

#include "detail/string_param.inc"

#endif
