/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_STRING8
#define CHECKHEADER_SLIB_CORE_STRING8

#include "definition.h"

#include "charset.h"
#include "memory.h"
#include "list.h"
#include "hash.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <string>
#endif

/**
 * @addtogroup core
 *  @{
 */

namespace slib
{

	class String;
	typedef Atomic<String> AtomicString;
	class String16;
	typedef Atomic<String16> AtomicString16;
	class StringData;
	class Variant;

	class SLIB_EXPORT StringContainer
	{
	public:
		sl_char8* sz;
		sl_size len;
		sl_uint32 hash;
		sl_uint32 type;
		sl_reg ref;
		
	public:
		sl_reg increaseReference();
		
		sl_reg decreaseReference();
		
	};
	
	/** 
	 * @class String
	 * @brief String class provides an extensive set of APIs for working with strings, including method for comparing, searching, and modifying strings.
	 */
	class SLIB_EXPORT String
	{
	private:
		StringContainer* m_container;
		
	private:
		constexpr String(StringContainer* container) : m_container(container) {}
		
	public:
		/**
		 * Initializes as a null string.
		 */
		constexpr String() : m_container(sl_null) {}
		constexpr String(sl_null_t) : m_container(sl_null) {}
		
		/**
		 * Move constructor
		 */
		String(String&& src);
		String(AtomicString&& _src);
		
		/**
		 * Copy constructor
		 */
		String(const String& src);
		String(const String16& src);		
		String(const AtomicString& src);
		String(const AtomicString16& src);
		
		/**
		 * Destructor
		 */
		~String();
		
	public:
		/**
		 * Fill the string with `nRepeatCount` consecutive copies of charactor `ch`
		 */
		String(sl_char8 ch, sl_size nRepeatCount);
		
		/**
		 * Copies the null-terminated character sequence pointed by `str`.
		 */
		String(const char* str);
		String(const wchar_t* str);
		String(const char16_t* str);
		String(const char32_t* str);

		/**
		 * Copies the first `length` characters from the array of characters pointed by `str`
		 */
		String(const char* str, sl_reg length);
		String(const wchar_t* str, sl_reg length);
		String(const char16_t* str, sl_reg length);
		String(const char32_t* str, sl_reg length);

#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Initialize from `std::string`.
		 * This does not copy the data of the string, but keep the reference to the original string.
		 */
		String(const std::string& str);
#endif
		
	public:
		
		/**
		 * Creates a string of `len` characters
		 */
		static String allocate(sl_size len);
		
		/**
		 * Creates a string pointing the `str` as the content, without copying the data.
		 * `str` should not be freed while the returned string is being used.
		 */
		static String fromStatic(const sl_char8* str, sl_reg len = -1);
		
		/**
		 * Creates a string pointing the `str` as the content, without copying the data.
		 * `ref` should be used to keep the alive of the string content.
		 */
		static String fromRef(const Ref<Referable>& ref, const sl_char8* str, sl_reg len = -1);
		
		/**
		 * Creates a string pointing the `mem` as the UTF-8 content, without copying the data.
		 */
		static String fromMemory(const Memory& mem);

		/**
		 * Creates a string copying the characters from the UTF-8 text.
		 *
		 * @param[in] text string buffer containing the UTF-8 text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf8(const void* text, sl_reg len = -1);
		
		/**
		 * Creates a string copying the characters from the UTF-8 text in `mem`.
		 */
		static String fromUtf8(const Memory& mem);
		
		/**
		 * Creates a string copying the characters from the UTF-16 text.
		 *
		 * @param[in] text string buffer containing the UTF-16 text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf16(const sl_char16* text, sl_reg len = -1);
		
		/**
		 * Creates a string copying the characters from the UTF-32 text.
		 *
		 * @param[in] text string buffer containing the UTF-32 text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf32(const sl_char32* text, sl_reg len = -1);
		
		/**
		 * Creates a string copying the characters from the UTF-16 Big Endian text.
		 *
		 * @param[in] text string buffer containing the UTF-16 Big Endian text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf16BE(const void* text, sl_reg len = -1);
		
		/**
		 * Creates a string copying the characters from the UTF-16 Big Endian text in `mem`.
		 */
		static String fromUtf16BE(const Memory& mem);
		
		/**
		 * Creates a string copying the characters from the UTF-16 Little Endian text.
		 *
		 * @param[in] text string buffer containing the UTF-16 Little Endian text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf16LE(const void* text, sl_reg len = -1);
		
		/**
		 * Creates a string copying the characters from the UTF-16 Little Endian text in `mem`.
		 */
		static String fromUtf16LE(const Memory& mem);
		
		/**
		 * Creates a string copying the characters from the UTF-8, UTF-16BE, or UTF-16LE text.
		 * This function detects the encoding type from the first 3 bytes of `text`.
		 *
		 * @param[in] text string buffer containing the unicode text.
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf(const void* text, sl_size len);
		
		/**
		 * Creates a string copying the characters from the UTF-8, UTF-16BE, or UTF-16LE text in `mem`.
		 * This function detects the encoding type from the first 3 bytes of the text.
		 */
		static String fromUtf(const Memory& mem);
		
	public:
		/**
		 * @return null string.
		 */
		static const String& null();
		
		/**
		 * @return empty string.
		 */
		static const String& getEmpty();
		
		/**
		 * @return empty string if this string is null. otherwise returns this string.
		 */
		const String& getNotNull() const;
		
		/**
		 * @return `true` if this string is null.
		 */
		sl_bool isNull() const;
		
		/**
		 * @return `true` if this string is not null.
		 */
		sl_bool isNotNull() const;
		
		/**
		 * @return `true` if this string is empty.
		 */
		sl_bool isEmpty() const;
		
		/**
		 * @return `true` if this string is not empty.
		 */
		sl_bool isNotEmpty() const;
		
		/**
		 * Sets this string as a null.
		 */
		void setNull();
		
		/**
		 * Sets this string as an empty.
		 */
		void setEmpty();
		
	public:
		/**
		 * @return string content.
		 */
		sl_char8* getData() const;
		
		/**
		 * @return string length.
		 */
		sl_size getLength() const;
		
		/**
		 * Sets the string length.
		 *
		 * Don't use for null or empty string
		 */
		void setLength(sl_size len);
		
		/**
		 * @return the hash code.
		 */
		sl_uint32 getHashCode() const;
		
		/**
		 * Sets the hash code.
		 *
		 * Don't use for null or empty string
		 */
		void setHashCode(sl_uint32 hash);
		
		/**
		 * @return the hash code ignoring the case.
		 */
		sl_uint32 getHashCodeIgnoreCase() const;
		
		/**
		 * @return the character at `index` in string.
		 */
		sl_char8 getAt(sl_reg index) const;
		
		/**
		 * Sets the character at `index` in string.
		 * @return `true` on success.
		 */
		sl_bool setAt(sl_reg index, sl_char8 ch);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Convert this string to std::string.
		 */
		std::string toStd() const;
#endif
		
	public:
		/**
		 * String assignment
		 */
		String& operator=(const String& other);
		String& operator=(const String16& other);
		String& operator=(const AtomicString& other);
		String& operator=(const AtomicString16& other);
		String& operator=(String&& other);
		String& operator=(AtomicString&& other);
		String& operator=(sl_null_t);
		String& operator=(const char* other);
		String& operator=(const wchar_t* other);
		String& operator=(const char16_t* other);
		String& operator=(const char32_t* other);
#ifdef SLIB_SUPPORT_STD_TYPES
		String& operator=(const std::string& other);
#endif

		/**
		 * Concatenate strings
		 */
		String operator+(const String& other) const;
		String operator+(const String16& other) const;
		String operator+(const AtomicString& other) const;
		String operator+(const AtomicString16& other) const;
		String operator+(const sl_char8* other) const;
		String operator+(const sl_char16* other) const;
		String operator+(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		String operator+(const std::string& other) const;
#endif
		String operator+(sl_int32 other) const;
		String operator+(sl_uint32 other) const;
		String operator+(sl_int64 other) const;
		String operator+(sl_uint64 other) const;
		String operator+(float other) const;
		String operator+(double other) const;
		String operator+(sl_bool other) const;

		/**
		 * Concatenate strings
		 */
		friend String operator+(const sl_char8* first, const String& second);
		friend String operator+(const sl_char16* first, const String& second);
		friend String operator+(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend String operator+(const std::string& first, const String& second);
#endif
		friend String operator+(sl_int32 first, const String& second);
		friend String operator+(sl_uint32 first, const String& second);
		friend String operator+(sl_int64 first, const String& second);
		friend String operator+(sl_uint64 first, const String& second);
		friend String operator+(float first, const String& second);
		friend String operator+(double first, const String& second);
		friend String operator+(sl_bool first, const String& second);

		/**
		 * Append to this string
		 */
		String& operator+=(const String& other);
		String& operator+=(const String16& other);
		String& operator+=(const AtomicString& other);
		String& operator+=(const AtomicString16& other);
		String& operator+=(const sl_char8* other);
		String& operator+=(const sl_char16* other);
		String& operator+=(const sl_char32* other);
#ifdef SLIB_SUPPORT_STD_TYPES
		String& operator+=(const std::string& other);
#endif
		String& operator+=(sl_int32 other);
		String& operator+=(sl_uint32 other);
		String& operator+=(sl_int64 other);
		String& operator+=(sl_uint64 other);
		String& operator+=(float other);
		String& operator+=(double other);
		String& operator+=(sl_bool other);
	
	public:
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const String& other) const;
		sl_bool equals(const String16& other) const;
		sl_bool equals(const AtomicString& other) const;
		sl_bool equals(const AtomicString16& other) const;
		sl_bool equals(const sl_char8* other) const;
		sl_bool equals(const sl_char16* other) const;
		sl_bool equals(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool equals(const std::string& other) const;
#endif
		
		/**
		 * Compares this string to the specified string.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other) const;
		sl_int32 compare(const String16& other) const;
		sl_int32 compare(const AtomicString& other) const;
		sl_int32 compare(const AtomicString16& other) const;
		sl_int32 compare(const sl_char8* other) const;
		sl_int32 compare(const sl_char16* other) const;
		sl_int32 compare(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_int32 compare(const std::string& other) const;
#endif
		
		/**
		 * Compares this string to the specified string.
		 * This functions stops searching on the index of `len-1` and returns 0.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other, sl_size len) const;
		
		/**
		 * @return true if this string is equal to the specified string ignoring the case.
		 */
		sl_bool equalsIgnoreCase(const String& other) const;
		
		/**
		 * Compares this string to the specified string ignoring the case.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compareIgnoreCase(const String& other) const;
		
	public:
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const String& other) const;
		sl_bool operator==(const String16& other) const;
		sl_bool operator==(const AtomicString& other) const;
		sl_bool operator==(const AtomicString16& other) const;
		sl_bool operator==(const sl_char8* other) const;
		sl_bool operator==(const sl_char16* other) const;
		sl_bool operator==(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator==(const std::string& other) const;
#endif

		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char8* first, const String& second);
		friend sl_bool operator==(const sl_char16* first, const String& second);
		friend sl_bool operator==(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator==(const std::string& first, const String& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const String& other) const;
		sl_bool operator!=(const String16& other) const;
		sl_bool operator!=(const AtomicString& other) const;
		sl_bool operator!=(const AtomicString16& other) const;
		sl_bool operator!=(const sl_char8* other) const;
		sl_bool operator!=(const sl_char16* other) const;
		sl_bool operator!=(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator!=(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char8* first, const String& second);
		friend sl_bool operator!=(const sl_char16* first, const String& second);
		friend sl_bool operator!=(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator!=(const std::string& first, const String& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const String& other) const;
		sl_bool operator>=(const String16& other) const;
		sl_bool operator>=(const AtomicString& other) const;
		sl_bool operator>=(const AtomicString16& other) const;
		sl_bool operator>=(const sl_char8* other) const;
		sl_bool operator>=(const sl_char16* other) const;
		sl_bool operator>=(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator>=(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char8* first, const String& second);
		friend sl_bool operator>=(const sl_char16* first, const String& second);
		friend sl_bool operator>=(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator>=(const std::string& first, const String& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const String& other) const;
		sl_bool operator<=(const String16& other) const;
		sl_bool operator<=(const AtomicString& other) const;
		sl_bool operator<=(const AtomicString16& other) const;
		sl_bool operator<=(const sl_char8* other) const;
		sl_bool operator<=(const sl_char16* other) const;
		sl_bool operator<=(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator<=(const std::string& other) const;
#endif

		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char8* first, const String& second);
		friend sl_bool operator<=(const sl_char16* first, const String& second);
		friend sl_bool operator<=(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator<=(const std::string& first, const String& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const String& other) const;
		sl_bool operator>(const String16& other) const;
		sl_bool operator>(const AtomicString& other) const;
		sl_bool operator>(const AtomicString16& other) const;
		sl_bool operator>(const sl_char8* other) const;
		sl_bool operator>(const sl_char16* other) const;
		sl_bool operator>(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator>(const std::string& other) const;
#endif

		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char8* first, const String& second);
		friend sl_bool operator>(const sl_char16* first, const String& second);
		friend sl_bool operator>(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator>(const std::string& first, const String& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const String& other) const;
		sl_bool operator<(const String16& other) const;
		sl_bool operator<(const AtomicString& other) const;
		sl_bool operator<(const AtomicString16& other) const;
		sl_bool operator<(const sl_char8* other) const;
		sl_bool operator<(const sl_char16* other) const;
		sl_bool operator<(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator<(const std::string& other) const;
#endif

		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char8* first, const String& second);
		friend sl_bool operator<(const sl_char16* first, const String& second);
		friend sl_bool operator<(const sl_char32* first, const String& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator<(const std::string& first, const String& second);
#endif
		
	public:
		/**
		 * @return duplicated string.
		 */
		String duplicate() const;
		
		/**
		 * @return memory containing string content.
		 */
		Memory toMemory() const;
		
		/**
		 * @return memory containing string content, without creating new heap memory.
		 */
		Memory toStaticMemory() const;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_size getUtf16(sl_char16* utf16, sl_size len) const;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_bool getUtf16(StringData& output) const;
		
		/**
		 * Converts to Utf16 and Returns a Memory containing the Utf16 characters and null at last
		 */
		Memory toUtf16() const;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_size getUtf32(sl_char32* utf32, sl_size len) const;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_bool getUtf32(StringData& output) const;
		
		/**
		 * Converts to Utf32 and Returns Memory containing the Utf32 characters and null at last
		 */
		Memory toUtf32() const;
		
		/**
		 * @return a newly constructed string with its value initialized to a copy of a substring of this string.
		 */
		String substring(sl_reg start, sl_reg end = -1) const;
		
		/**
		 * @return a string containing a specified number of characters from the left side of this string.
		 */
		String left(sl_reg len) const;
		
		/**
		 * @return a string containing a specified number of characters from the right side of this string.
		 */
		String right(sl_reg len) const;
		
		/**
		 * @return a string that contains a specified number of characters starting from a specified position in this string.
		 */
		String mid(sl_reg start, sl_reg len) const;
		
		/**
		 * @return the index within this string of the first occurrence of the specified character, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
		
		/**
		 * @return the index within this string of the first occurrence of the specified string, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(const String& str, sl_reg start = 0) const;
		sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
		
		/**
		 * @return the index within this string of the last occurrence of the specified character, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
		
		/**
		 * @return the index within this string of the last occurrence of the specified string, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(const String& str, sl_reg start = -1) const;
		sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
		
		/**
		 * @return `true` if this string starts with the specified character.
		 */
		sl_bool startsWith(sl_char8 ch) const;
		
		/**
		 * @return `true` if this string starts with the specified string.
		 */
		sl_bool startsWith(const String& str) const;
		sl_bool startsWith(const sl_char8* str) const;
		
		/**
		 * @return `true` if this string ends with the specified character.
		 */
		sl_bool endsWith(sl_char8 ch) const;
		
		/**
		 * @return `true` if this string ends with the specified string.
		 */
		sl_bool endsWith(const String& str) const;
		sl_bool endsWith(const sl_char8* str) const;
		
		/**
		 * @return `true` if the specified character occurs within this string.
		 */
		sl_bool contains(sl_char8 ch) const;
		
		/**
		 * @return `true` if the specified substring occurs within this string.
		 */
		sl_bool contains(const String& str) const;
		sl_bool contains(const sl_char8* str) const;
		
		/**
		 * Converts the characters of this string to uppercase.
		 */
		void makeUpper();
		
		/**
		 * Converts the characters of this string to lowercase.
		 */
		void makeLower();
		
		/**
		 * @return a copy of the specified string converted to uppercase.
		 */
		static String toUpper(const sl_char8* str, sl_reg len = -1);
		
		/**
		 * @return a copy of the specified string converted to lowercase.
		 */
		static String toLower(const sl_char8* str, sl_reg len = -1);
		
		/**
		 * @return a copy of this string converted to uppercase.
		 */
		String toUpper() const;
		
		/**
		 * @return a copy of this string converted to lowercase.
		 */
		String toLower() const;
		
		/**
		 * Replaces each substring of this string that matches the given `pattern` with the given `replacement`.
		 */
		String replaceAll(const String& pattern, const String& replacement) const;
		String replaceAll(const String& pattern, const sl_char8* replacement) const;
		String replaceAll(const sl_char8* pattern, const String& replacement) const;
		String replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
		
		/**
		 * Copy this string and then removes whitespaces from both ends of the new string.
		 */
		String trim() const;
		
		/**
		 * Copy this string and then removes whitespaces from the left of the new string.
		 */
		String trimLeft() const;
		
		/**
		 * Copy this string and then removes whitespaces from the right of the new string.
		 */
		String trimRight() const;
		
		/**
		 * Splits this string into the list of strings by the `pattern` separator.
		 */
		List<String> split(const String& pattern) const;
		List<String> split(const sl_char8* pattern) const;
		
	public:
		/**
		 * Convert the string (`str`) to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 * @param[in] str String containing the integer
		 * @param[in] posBegin Zero-based position of the integer
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 * 
		 * @return the position after the integer if a valid integer is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseInt32(sl_int32 radix, sl_int32* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt32(sl_int32 radix, sl_int32* value) const;
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;
		
		/**
		 * Convert the string (`str`) to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 * @param[in] str String containing the integer
		 * @param[in] posBegin Zero-based position of the integer
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the integer if a valid integer is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseUint32(sl_int32 radix, sl_uint32* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint32(sl_int32 radix, sl_uint32* value) const;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;
		
		/**
		 * Convert the string (`str`) to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 * @param[in] str String containing the integer
		 * @param[in] posBegin Zero-based position of the integer
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the integer if a valid integer is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseInt64(sl_int32 radix, sl_int64* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt64(sl_int32 radix, sl_int64* value) const;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;
		
		/**
		 * Convert the string (`str`) to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 * @param[in] str String containing the integer
		 * @param[in] posBegin Zero-based position of the integer
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the integer if a valid integer is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseUint64(sl_int32 radix, sl_uint64* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint64(sl_int32 radix, sl_uint64* value) const;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;
		
		/**
		 * Convert this string to an integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt(sl_int32 radix, sl_reg* value) const;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseSize(sl_int32 radix, sl_size* value) const;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;
		
		/**
		 * Convert the string (`str`) to a float number value
		 *
		 * @param[out] value Pointer to the result output
		 * @param[in] str String to convert
		 * @param[in] posBegin Zero-based position of the number
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the number if a valid number is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseFloat(float* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseFloat(float* value) const;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		float parseFloat(float def = 0) const;
		
		/**
		 * Convert the string (`str`) to a double number value
		 *
		 * @param[out] value Pointer to the result output
		 * @param[in] str String to convert
		 * @param[in] posBegin Zero-based position of the number
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the number if a valid number is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseDouble(double* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseDouble(double* value) const;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		double parseDouble(double def = 0) const;
		
		/**
		 * Convert the string (`str`) to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[out] value Pointer to the result output
		 * @param[in] str String to convert
		 * @param[in] posBegin Zero-based position of the boolean keyword
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the boolean keyword if a valid keyword is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseBoolean(sl_bool* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseBoolean(sl_bool* value) const;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		sl_bool parseBoolean(sl_bool def = sl_false) const;
		
		/**
		 * Parses the hex string and writes the bytes to `output`. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @param[out] output Pointer to the output buffer.
		 * @param[in] str The string containing the hex string.
		 * @param[in] posBegin The zero-based index of the first character in this string object.
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the boolean keyword if a valid keyword is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseHexString(void* output, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX);
		
		/**
		 * Parses this hex string and writes the bytes to `output`. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @param[out] output Pointer to the output buffer.
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseHexString(void* output) const;
		
		/**
		 * @return the string representation of 32 bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromInt32(sl_int32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
		/**
		 * @return the string representation of the unsigned 32bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromUint32(sl_uint32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
		/**
		 * @return the string representation of the 64bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromInt64(sl_int64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
		/**
		 * @return the string representation of the unsigned 64bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromUint64(sl_uint64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
		/**
		 * @return the string representation of the integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromInt(sl_reg value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
		/**
		 * @return the string representation of the unsigned integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromSize(sl_size value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
		/**
		 * @return the string representation of the float argument.
		 *
		 * @param value The float value to be parsed.
		 * @param precision The number of characters in decimal. Negative values is ignored and this parameter has not effect.
		 * @param flagZeroPadding If flagZeroPadding is true, zeros are used to pad the field instead of space characters.
		 * @param minWidthIntegral The minimum number of characters in integral field.
		 */
		static String fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
		
		/**
		 * @return the string representation of the double argument.
		 *
		 * @param value The double value to be parsed.
		 * @param precision The number of characters in decimal. Negative values is ignored and this parameter has not effect.
		 * @param flagZeroPadding If flagZeroPadding is true, zeros are used to pad the field instead of space characters.
		 * @param minWidthIntegral The minimum number of characters in integral field.
		 */
		static String fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
		
		/**
		 * @return the string representation of the memory address.
		 *
		 * @param pointer The memory address to be parsed.
		 */
		static String fromPointerValue(const void* pointer);
		
		/**
		 * @return the string representation of the boolean argument.
		 *
		 * @param value The boolean value to be parsed.
		 */
		static String fromBoolean(sl_bool value);
		
		/**
		 * @return the converted hex string from the buffer.
		 *
		 * @param data The buffer to be converted.
		 * @param size Size of the buffer.
		 */
		static String makeHexString(const void* data, sl_size size);
		
		/**
		 * @return the converted hex string from the buffer.
		 *
		 * @param mem The buffer to be converted.
		 */
		static String makeHexString(const Memory& mem);
		
		/**
		 * Returns the formatted string from the format string and arbitrary list of arguments.
		 *
		 * String formatting is smiliar with Java Formatter
		 *
		 * https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html
		 *
		 * %[argument_index$][flags][width][.precision]conversion
		 *
		 * @param strFormat The buffer containing the format string, this supports the conversion specifiers, length modifiers, and flags.
		 *
		 */
		static String format(const String& strFormat);
		static String format(const sl_char8* strFormat);
		template <class... ARGS>
		static String format(const String& strFormat, ARGS&&... args);
		template <class... ARGS>
		static String format(const sl_char8* strFormat, ARGS&&... args);
		static String formatBy(const String& strFormat, const Variant* params, sl_size nParams);
		static String formatBy(const sl_char8* strFormat, const Variant* params, sl_size nParams);
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments.
		 * It is same as `String::format(*this, ...)`.
		 */
		template <class... ARGS>
		String arg(ARGS&&... args) const;
		String argBy(const Variant* params, sl_size nParams) const;
		
	private:
		void _replaceContainer(StringContainer* container);
		
	public:
		friend class Atomic<String>;
		
	};
	
	
	template <>
	class SLIB_EXPORT Atomic<String>
	{
	private:
		StringContainer* volatile m_container;
		SpinLock m_lock;
		
	public:
		/**
		 * Initialize as a null string.
		 */
		constexpr Atomic() : m_container(sl_null) {}
		constexpr Atomic(sl_null_t) : m_container(sl_null) {}
		
		/**
		 * Move constructor
		 */
		Atomic(String&& src);
		Atomic(AtomicString&& src);
		
		/**
		 * Copy constructor
		 */
		Atomic(const String& src);
		Atomic(const String16& src);
		Atomic(const AtomicString& src);
		Atomic(const AtomicString16& src);

		/**
		 * Destructor
		 */		
		~Atomic();
		
	public:
		/**
		 * Fill the string with `nRepeatCount` consecutive copies of charactor `ch`
		 */
		Atomic(sl_char8 ch, sl_size nRepeatCount);
		
		/**
		 * Copies the null-terminated character sequence pointed by `str`.
		 */
		Atomic(const char* str);
		Atomic(const wchar_t* str);
		Atomic(const char16_t* str);
		Atomic(const char32_t* str);

		/**
		 * Copies the first `length` characters from the array of characters pointed by `str`
		 */
		Atomic(const char* str, sl_reg length);
		Atomic(const wchar_t* str, sl_reg length);
		Atomic(const char16_t* str, sl_reg length);
		Atomic(const char32_t* str, sl_reg length);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Initialize from `std::string`.
		 * This does not copy the data of the string, but keep the reference to the original string.
		 */
		Atomic(const std::string& str);
#endif

	public:
		/**
		 * @return null string.
		 */
		static const AtomicString& null();
		
		/**
		 * @return empty string.
		 */
		static const AtomicString& getEmpty();
		
		/**
		 * @return `true` if this string is null.
		 */
		sl_bool isNull() const;
		
		/**
		 * @return `true` if this string is not null.
		 */
		sl_bool isNotNull() const;
		
		/**
		 * @return `true` if this string is empty.
		 */
		sl_bool isEmpty() const;
		
		/**
		 * @return `true` if this string is not empty.
		 */
		sl_bool isNotEmpty() const;
		
		/**
		 * Sets this string as a null.
		 */
		void setNull();
		
		/**
		 * Sets this string as an empty.
		 */
		void setEmpty();
		
	public:
		/**
		 * @return string length.
		 */
		sl_size getLength() const;
		
		/**
		 * @return the hash code.
		 */
		sl_uint32 getHashCode() const;
		
		/**
		 * @return the hash code ignoring the case.
		 */
		sl_uint32 getHashCodeIgnoreCase() const;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Convert this string to std::string.
		 */
		std::string toStd() const;
#endif

	public:
		/**
		 * String assignment
		 */
		AtomicString& operator=(const String& other);
		AtomicString& operator=(const AtomicString& other);
		AtomicString& operator=(const String16& other);
		AtomicString& operator=(const AtomicString16& other);
		AtomicString& operator=(String&& other);
		AtomicString& operator=(AtomicString&& other);
		AtomicString& operator=(sl_null_t);
		AtomicString& operator=(const char* other);
		AtomicString& operator=(const wchar_t* other);
		AtomicString& operator=(const char16_t* other);
		AtomicString& operator=(const char32_t* other);
#ifdef SLIB_SUPPORT_STD_TYPES
		AtomicString& operator=(const std::string& other);
#endif
		
		/**
		 * Concatenate strings
		 */
		String operator+(const String& other) const;
		String operator+(const String16& other) const;
		String operator+(const AtomicString& other) const;
		String operator+(const AtomicString16& other) const;
		String operator+(const sl_char8* other) const;
		String operator+(const sl_char16* other) const;
		String operator+(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		String operator+(const std::string& other) const;
#endif
		String operator+(sl_int32 other) const;
		String operator+(sl_uint32 other) const;
		String operator+(sl_int64 other) const;
		String operator+(sl_uint64 other) const;
		String operator+(float other) const;
		String operator+(double other) const;
		String operator+(sl_bool other) const;
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(const sl_char8* first, const AtomicString& second);
		friend String operator+(const sl_char16* first, const AtomicString& second);
		friend String operator+(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend String operator+(const std::string& first, const AtomicString& second);
#endif
		friend String operator+(sl_int32 first, const AtomicString& second);
		friend String operator+(sl_uint32 first, const AtomicString& second);
		friend String operator+(sl_int64 first, const AtomicString& second);
		friend String operator+(sl_uint64 first, const AtomicString& second);
		friend String operator+(float first, const AtomicString& second);
		friend String operator+(double first, const AtomicString& second);
		friend String operator+(sl_bool first, const AtomicString& second);
		
		/**
		 * Append to this string
		 */
		AtomicString& operator+=(const String& other);
		AtomicString& operator+=(const String16& other);
		AtomicString& operator+=(const AtomicString& other);
		AtomicString& operator+=(const AtomicString16& other);
		AtomicString& operator+=(const sl_char8* other);
		AtomicString& operator+=(const sl_char16* other);
		AtomicString& operator+=(const sl_char32* other);
#ifdef SLIB_SUPPORT_STD_TYPES
		AtomicString& operator+=(const std::string& other);
#endif
		AtomicString& operator+=(sl_int32 other);
		AtomicString& operator+=(sl_uint32 other);
		AtomicString& operator+=(sl_int64 other);
		AtomicString& operator+=(sl_uint64 other);
		AtomicString& operator+=(float other);
		AtomicString& operator+=(double other);
		AtomicString& operator+=(sl_bool other);
		
	public:
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const String& other) const;
		sl_bool equals(const String16& other) const;
		sl_bool equals(const AtomicString& other) const;
		sl_bool equals(const AtomicString16& other) const;
		sl_bool equals(const sl_char8* other) const;
		sl_bool equals(const sl_char16* other) const;
		sl_bool equals(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool equals(const std::string& other) const;
#endif
		
		/**
		 * Compares this string to the specified string.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other) const;
		sl_int32 compare(const String16& other) const;
		sl_int32 compare(const AtomicString& other) const;
		sl_int32 compare(const AtomicString16& other) const;
		sl_int32 compare(const sl_char8* other) const;
		sl_int32 compare(const sl_char16* other) const;
		sl_int32 compare(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_int32 compare(const std::string& other) const;
#endif
		
		/**
		 * Compares this string to the specified string.
		 * This functions stops searching on the index of `len-1` and returns 0.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other, sl_size len) const;
		
		/**
		 * @return true if this string is equal to the specified string ignoring the case.
		 */
		sl_bool equalsIgnoreCase(const String& other) const;
		
		/**
		 * Compares this string to the specified string ignoring the case.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compareIgnoreCase(const String& other) const;
		
	public:
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const String& other) const;
		sl_bool operator==(const String16& other) const;
		sl_bool operator==(const AtomicString& other) const;
		sl_bool operator==(const AtomicString16& other) const;
		sl_bool operator==(const sl_char8* other) const;
		sl_bool operator==(const sl_char16* other) const;
		sl_bool operator==(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator==(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char8* first, const AtomicString& second);
		friend sl_bool operator==(const sl_char16* first, const AtomicString& second);
		friend sl_bool operator==(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator==(const std::string& first, const AtomicString& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const String& other) const;
		sl_bool operator!=(const String16& other) const;
		sl_bool operator!=(const AtomicString& other) const;
		sl_bool operator!=(const AtomicString16& other) const;
		sl_bool operator!=(const sl_char8* other) const;
		sl_bool operator!=(const sl_char16* other) const;
		sl_bool operator!=(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator!=(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char8* first, const AtomicString& second);
		friend sl_bool operator!=(const sl_char16* first, const AtomicString& second);
		friend sl_bool operator!=(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator!=(const std::string& first, const AtomicString& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const String& other) const;
		sl_bool operator>=(const String16& other) const;
		sl_bool operator>=(const AtomicString& other) const;
		sl_bool operator>=(const AtomicString16& other) const;
		sl_bool operator>=(const sl_char8* other) const;
		sl_bool operator>=(const sl_char16* other) const;
		sl_bool operator>=(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator>=(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char8* first, const AtomicString& second);
		friend sl_bool operator>=(const sl_char16* first, const AtomicString& second);
		friend sl_bool operator>=(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator>=(const std::string& first, const AtomicString& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const String& other) const;
		sl_bool operator<=(const String16& other) const;
		sl_bool operator<=(const AtomicString& other) const;
		sl_bool operator<=(const AtomicString16& other) const;
		sl_bool operator<=(const sl_char8* other) const;
		sl_bool operator<=(const sl_char16* other) const;
		sl_bool operator<=(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator<=(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char8* first, const AtomicString& second);
		friend sl_bool operator<=(const sl_char16* first, const AtomicString& second);
		friend sl_bool operator<=(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator<=(const std::string& first, const AtomicString& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const String& other) const;
		sl_bool operator>(const String16& other) const;
		sl_bool operator>(const AtomicString& other) const;
		sl_bool operator>(const AtomicString16& other) const;
		sl_bool operator>(const sl_char8* other) const;
		sl_bool operator>(const sl_char16* other) const;
		sl_bool operator>(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator>(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char8* first, const AtomicString& second);
		friend sl_bool operator>(const sl_char16* first, const AtomicString& second);
		friend sl_bool operator>(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator>(const std::string& first, const AtomicString& second);
#endif
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const String& other) const;
		sl_bool operator<(const String16& other) const;
		sl_bool operator<(const AtomicString& other) const;
		sl_bool operator<(const AtomicString16& other) const;
		sl_bool operator<(const sl_char8* other) const;
		sl_bool operator<(const sl_char16* other) const;
		sl_bool operator<(const sl_char32* other) const;
#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool operator<(const std::string& other) const;
#endif
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char8* first, const AtomicString& second);
		friend sl_bool operator<(const sl_char16* first, const AtomicString& second);
		friend sl_bool operator<(const sl_char32* first, const AtomicString& second);
#ifdef SLIB_SUPPORT_STD_TYPES
		friend sl_bool operator<(const std::string& first, const AtomicString& second);
#endif

	public:
		/**
		 * @return duplicated string.
		 */
		String duplicate() const;
		
		/**
		 * @return memory containing string content.
		 */
		Memory toMemory() const;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_size getUtf16(sl_char16* utf16, sl_size len) const;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_bool getUtf16(StringData& output) const;
		
		/**
		 * Converts to Utf16 and Returns a Memory containing the Utf16 characters and null at last
		 */
		Memory toUtf16() const;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_size getUtf32(sl_char32* utf32, sl_size len) const;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_bool getUtf32(StringData& output) const;
		
		/**
		 * Converts to Utf32 and Returns Memory containing the Utf32 characters and null at last
		 */
		Memory toUtf32() const;
		
		/**
		 * @return a newly constructed string with its value initialized to a copy of a substring of this string.
		 */
		String substring(sl_reg start, sl_reg end = -1) const;
		
		/**
		 * @return a string containing a specified number of characters from the left side of this string.
		 */
		String left(sl_reg len) const;
		
		/**
		 * @return a string containing a specified number of characters from the right side of this string.
		 */
		String right(sl_reg len) const;
		
		/**
		 * @return a string that contains a specified number of characters starting from a specified position in a string.
		 */
		String mid(sl_reg start, sl_reg len) const;
		
		/**
		 * @return the index within this string of the first occurrence of the specified character, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
		
		/**
		 * @return the index within this string of the first occurrence of the specified string, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(const String& str, sl_reg start = 0) const;
		sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
		
		/**
		 * @return the index within this string of the last occurrence of the specified character, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
		
		/**
		 * @return the index within this string of the last occurrence of the specified string, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(const String& str, sl_reg start = -1) const;
		sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
		
		/**
		 * @return `true` if this string starts with the specified character.
		 */
		sl_bool startsWith(sl_char8 ch) const;
		
		/**
		 * @return `true` if this string starts with the specified string.
		 */
		sl_bool startsWith(const String& str) const;
		sl_bool startsWith(const sl_char8* str) const;
		
		/**
		 * @return `true` if this string ends with the specified character.
		 */
		sl_bool endsWith(sl_char8 ch) const;
		
		/**
		 * @return `true` if this string ends with the specified string.
		 */
		sl_bool endsWith(const String& str) const;
		sl_bool endsWith(const sl_char8* str) const;
		
		/**
		 * @return `true` if the specified character occurs within this string.
		 */
		sl_bool constains(sl_char8 ch) const;
		
		/**
		 * @return `true` if the specified substring occurs within this string.
		 */
		sl_bool contains(const String& str) const;
		sl_bool contains(const sl_char8* str) const;
		
		/**
		 * Converts the characters of this string to uppercase.
		 */
		void makeUpper();
		
		/**
		 * Converts the characters of this string to lowercase.
		 */
		void makeLower();
		
		/**
		 * @return a copy of this string converted to uppercase.
		 */
		String toUpper() const;
		
		/**
		 * @return a copy of this string converted to lowercase.
		 */
		String toLower() const;
		
		/**
		 * Replaces each substring of this string that matches the given `pattern` with the given `replacement`.
		 */
		String replaceAll(const String& pattern, const String& replacement) const;
		String replaceAll(const String& pattern, const sl_char8* replacement) const;
		String replaceAll(const sl_char8* pattern, const String& replacement) const;
		String replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
		
		/**
		 * Copy this string and then removes whitespaces from both ends of the new string.
		 */
		String trim() const;
		
		/**
		 * Copy this string and then removes whitespaces from the left of the new string.
		 */
		String trimLeft() const;
		
		/**
		 * Copy this string and then removes whitespaces from the right of the new string.
		 */
		String trimRight() const;
		
		/**
		 * Splits this string into the list of strings by the `pattern` separator.
		 */
		List<String> split(const String& pattern) const;
		List<String> split(const sl_char8* pattern) const;
		
	public:
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt32(sl_int32 radix, sl_int32* value) const;
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint32(sl_int32 radix, sl_uint32* value) const;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt64(sl_int32 radix, sl_int64* value) const;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint64(sl_int32 radix, sl_uint64* value) const;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;
		
		/**
		 * Convert this string to an integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt(sl_int32 radix, sl_reg* value) const;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseSize(sl_int32 radix, sl_size* value) const;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseFloat(float* value) const;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		float parseFloat(float def = 0) const;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseDouble(double* value) const;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		double parseDouble(double def = 0) const;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseBoolean(sl_bool* value) const;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		sl_bool parseBoolean(sl_bool def = sl_false) const;
		
		/**
		 * Parses this hex string and writes the bytes to `output`. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @param[out] output Pointer to the output buffer.
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseHexString(void* output) const;
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments.
		 * It is same as `String::format(*this, ...)`.
		 */
		template <class... ARGS>
		String arg(ARGS&&... args) const;
		String argBy(const Variant* params, sl_size nParams) const;
		
	private:
		StringContainer* _retainContainer() const;
		
		void _replaceContainer(StringContainer* other);
		
		friend class String;
	};
	
	
	template <>
	class Compare<String>
	{
	public:
		int operator()(const String& a, const String& b) const;
	};
	
	template <>
	class Compare<AtomicString>
	{
	public:
		int operator()(const AtomicString& a, const AtomicString& b) const;
	};
	
	template <>
	class Equals<String>
	{
	public:
		sl_bool operator()(const String& a, const String& b) const;
	};
	
	template <>
	class Equals<AtomicString>
	{
	public:
		sl_bool operator()(const AtomicString& a, const AtomicString& b) const;
	};
	
	template <>
	class Hash<String>
	{
	public:
		sl_uint32 operator()(const String& a) const;
	};
	
	template <>
	class Hash<AtomicString>
	{
	public:
		sl_uint32 operator()(const AtomicString& a) const;
	};
	
	class CompareIgnoreCaseString
	{
	public:
		int operator()(const String& a, const String& b) const;
	};
	
	class EqualsIgnoreCaseString
	{
	public:
		sl_bool operator()(const String& a, const String& b) const;
	};
	
	class HashIgnoreCaseString
	{
	public:
		sl_uint32 operator()(const String& v) const;
		
	};

}

/// @}

#define SLIB_STATIC_STRING(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, 0, -1}; \
	static slib::_priv_String_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static const slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_##name));

#define SLIB_STATIC_STRING_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, 0, -1}; \
	static slib::_priv_String_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static const slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_##name));

#define SLIB_STATIC_STRING_NULL(name) \
	static slib::_priv_String_Const _static_string_null_##name = {sl_null, 0}; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING_NULL(name) \
	static slib::_priv_String_Const _static_string_null_##name = {sl_null, 0}; \
	static const slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_null_##name));


#endif
