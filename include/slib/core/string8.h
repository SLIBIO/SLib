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
		 * Allocates and initializes a String, with default settings.
		 */
		constexpr String() : m_container(sl_null) {}
		
		/**
		 * Allocates and initializes a String, with default settings.
		 */
		constexpr String(sl_null_t) : m_container(sl_null) {}
		
		/**
		 * Move constructor of String.
		 */
		String(String&& src);
		
		/**
		 * Move constructor of String.
		 */
		String(AtomicString&& _src);
		
		/**
		 * Copy constructor
		 */
		String(const String& src);
		
		/**
		 * Copy constructor
		 */
		String(const AtomicString& src);
		
		/**
		 * Copy constructor
		 */
		String(const String16& src);
		
		/**
		 * Copy constructor
		 */	
		String(const AtomicString16& src);
		
		~String();
		
	public:
		
		/**
		 * Fill constructor
		 *
		 * Fill the string with nRepeatCount consecutive copies of charactor ch
		 */
		String(sl_char8 ch, sl_size nRepeatCount);
		
		/**
		 * From utf-8 null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUtf8.
		 */
		String(const char* strUtf8);
		
		/**
		 * From utf-8 string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUtf8
		 */
		String(const char* strUtf8, sl_reg length);
				
		/**
		 * From unicode null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUnicode.
		 */
		String(const wchar_t* strUnicode);
		
		/**
		 * From unicode string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUnicode
		 */
		String(const wchar_t* strUnicode, sl_reg length);
		
		/**
		 * From utf-16 null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUtf16.
		 */
		String(const char16_t* strUtf16);
		
		/**
		 * From utf-16 string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUtf16
		 */
		String(const char16_t* strUtf16, sl_reg length);
		
		/**
		 * From utf-32 null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUtf32.
		 */
		String(const char32_t* strUtf32);
		
		/**
		 * From utf-32 string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUtf32
		 */
		String(const char32_t* strUtf32, sl_reg length);

	public:
		
		/**
		 * Create a String of 'len' characters
		 */
		static String allocate(sl_size len);
		
		/**
		 * Copies the first 'len' characters from the array of characters pointed by sz8
		 * @return An automatically released String object.
		 */
		static String fromStatic(const sl_char8* sz8, sl_reg len = -1);
		
		/**
		 * From Utf8 string
		 *
		 * Copies the first 'len' characters from the array of characters pointed by utf8
		 * @return An automatically released String object.
		 */
		static String fromUtf8(const void* utf8, sl_reg len = -1);
		
		/**
		 * From string buffer
		 *
		 * Copies characters in mem.
		 * @return An automatically released String object.
		 */
		static String fromUtf8(const Memory& mem);
		
		/**
		 * From Utf16 string
		 *
		 * Copies the first 'len' characters from the array of characters pointed by utf16
		 * @return An automatically released String object.
		 */
		static String fromUtf16(const sl_char16* utf16, sl_reg len = -1);
		
		/**
		 * From Utf32 string
		 *
		 * Copies the first 'len' characters from the array of characters pointed by utf32
		 * @return An automatically released String object.
		 */
		static String fromUtf32(const sl_char32* utf32, sl_reg len = -1);
		
		/**
		 * From Utf16 Big-Endian string
		 *
		 * @return An automatically released String object.
		 */
		static String fromUtf16BE(const void* utf16, sl_reg len = -1);
		
		/**
		 * From Utf16 Big-Endian buffer
		 *
		 * @return An automatically released String object.
		 */
		static String fromUtf16BE(const Memory& mem);
		
		/**
		 * From Utf16 Little-Endian buffer
		 *
		 * @return An automatically released String object.
		 */
		static String fromUtf16LE(const void* utf16, sl_reg len = -1);
		
		/**
		 * From Utf16 Big-Endian buffer
		 *
		 * @return An automatically released String object.
		 */
		static String fromUtf16LE(const Memory& mem);
		
		/**
		 * Process UTF16BE, UTF16LE, UTF8 strings automatically.
		 *
		 * @return An automatically released String object.
		 */
		static String fromUtf(const void* buf, sl_size len);
		
		/**
		 * Process UTF16BE, UTF16LE, UTF8 strings automatically.
		 *
		 * @return An automatically released String object.
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
		 * @return empty string if string is null.
		 */
		const String& getNotNull() const;
		
		/**
		 * @return whether the string is null.
		 */
		sl_bool isNull() const;
		
		/**
		 * @return whether the string is not null.
		 */
		sl_bool isNotNull() const;
		
		/**
		 * @return whether the string is empty.
		 */
		sl_bool isEmpty() const;
		
		/**
		 * @return whether the string is not empty.
		 */
		sl_bool isNotEmpty() const;
		
		/**
		 * Sets the string null.
		 */
		void setNull();
		
		/**
		 * Sets the string empty.
		 */
		void setEmpty();
		
	public:
		/**
		 * @return string buffer.
		 */
		sl_char8* getData() const;
		
		/**
		 * @return string length.
		 */
		sl_size getLength() const;
		
		/**
		 * Sets string length.
		 *
		 * Don't use for null or empty string
		 */
		void setLength(sl_size len);
		
		/**
		 * @return the hash code.
		 */
		sl_uint32 getHashCode() const;
		
		/**
		 * Sets hash code.
		 *
		 * Don't use for null or empty string
		 */
		void setHashCode(sl_uint32 hash);
		
		/**
		 * @return the hash code.
		 */
		sl_uint32 getHashCodeIgnoreCase() const;
		
		/**
		 * @return the character at index in string.
		 */
		sl_char8 getAt(sl_reg index) const;
		
		/**
		 * Sets the character at index in string.
		 * @return whether the setting is successed.
		 */
		sl_bool setAt(sl_reg index, sl_char8 ch);
		
	public:
		
		/**
		 * String assignment
		 */
		String& operator=(sl_null_t);
		
		/**
		 * String assignment
		 */
		String& operator=(String&& other);
		
		/**
		 * String assignment
		 */
		String& operator=(AtomicString&& _other);
		
		/**
		 * String assignment
		 */
		String& operator=(const String& other);
		
		/**
		 * String assignment
		 */
		String& operator=(const AtomicString& other);
		
		/**
		 * String assignment
		 */
		String& operator=(const String16& other);
		
		/**
		 * String assignment
		 */
		String& operator=(const AtomicString16& other);
		
		/**
		 * String assignment
		 */
		String& operator=(const char* utf8);
		
		/**
		 * String assignment
		 */
		String& operator=(const wchar_t* sz);
		
		/**
		 * String assignment
		 */
		String& operator=(const char16_t* utf16);
		
		/**
		 * String assignment
		 */
		String& operator=(const char32_t* utf32);
		
	public:
		
		/**
		 * Concatenate strings
		 */
		String operator+(const String& other) const;
		
		/**
		 * Append to strings
		 */
		String& operator+=(const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const String16& other) const;
		
		/**
		 * Append to strings
		 */
		String& operator+=(const String16& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const AtomicString& _other) const;
		
		/**
		 * Append to strings
		 */
		String& operator+=(const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const AtomicString16& other) const;
		
		/**
		 * Append to strings
		 */
		String& operator+=(const AtomicString16& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const sl_char8* utf8) const;
		
		/**
		 * Append to strings
		 */
		String& operator+=(const sl_char8* utf8);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const sl_char16* utf16) const;
		
		/**
		 * Append to strings
		 */	
		String& operator+=(const sl_char16* utf16);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const sl_char32* utf32) const;
		
		/**
		 * Append to strings
		 */	
		String& operator+=(const sl_char32* utf32);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(const sl_char8* utf8, const String& second);
		
		/**
		 * Concatenate strings
		 */	
		friend String operator+(const sl_char16* utf16, const String& second);
		
		/**
		 * Concatenate strings
		 */	
		friend String operator+(const sl_char32* utf32, const String& second);
		
	public:
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_int32 number) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(sl_int32 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_int32 number, const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_uint32 number) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(sl_uint32 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_uint32 number, const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_int64 number) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(sl_int64 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_int64 number, const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_uint64 number) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(sl_uint64 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_uint64 number, const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(float number) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(float number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(float number, const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(double number) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(double number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(double number, const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_bool value) const;
		
		/**
		 * Append integer to strings
		 */
		String& operator+=(sl_bool value);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_bool value, const String& other);
		
	public:
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const String& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const String16& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const AtomicString& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const AtomicString16& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const sl_char8* utf8) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const sl_char16* utf16) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const sl_char32* utf32) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String16& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const AtomicString& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const AtomicString16& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const sl_char8* utf8) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const sl_char16* utf16) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const sl_char32* utf32) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other, sl_size len) const;
		
		/**
		 * @return true if this string is equal to the specified string.
		 */
		sl_bool equalsIgnoreCase(const String& other) const;
		
		/**
		 * @return true if this string is equal to the specified string.
		 */
		sl_int32 compareIgnoreCase(const String& other) const;
		
	public:
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char8* utf8, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char16* utf16, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char32* utf32, const String& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char8* utf8, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char16* utf16, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char32* utf32, const String& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char8* utf8, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char16* utf16, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char32* utf32, const String& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char8* utf8, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char16* utf16, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char32* utf32, const String& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char8* utf8, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char16* utf16, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char32* utf32, const String& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char8* utf8, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char16* utf16, const String& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char32* utf32, const String& second);
		
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
		 * @return a newly constructed string object with its value initialized to a copy of a substring of this object.
		 */
		String substring(sl_reg start, sl_reg end = -1) const;
		
		/**
		 * @return a string containing a specified number of characters from the left side of a string.
		 */
		String left(sl_reg len) const;
		
		/**
		 * @return a string containing a specified number of characters from the right side of a string.
		 */
		String right(sl_reg len) const;
		
		/**
		 * @return a string that contains a specified number of characters starting from a specified position in a string.
		 */
		String mid(sl_reg start, sl_reg len) const;
		
		/**
		 * @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
		 */
		sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
		
		/**
		 * @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
		 */
		sl_reg indexOf(const String& str, sl_reg start = 0) const;
		
		/**
		 * @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
		 **/
		sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
		
		/**
		 * @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
		 */
		sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
		
		/**
		 * @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
		 */
		sl_reg lastIndexOf(const String& str, sl_reg start = -1) const;
		
		/**
		 * @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
		 */
		sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
		
		/**
		 * Determines whether a string begins with the character, returning true or false as appropriate.
		 */
		sl_bool startsWith(sl_char8 ch) const;
		
		/**
		 * Determines whether a string begins with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool startsWith(const String& str) const;
		
		/**
		 * Determines whether a string begins with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool startsWith(const sl_char8* str) const;
		
		/**
		 * Determines whether a string ends with the character, returning true or false as appropriate.
		 */
		sl_bool endsWith(sl_char8 ch) const;
		
		/**
		 * Determines whether a string ends with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool endsWith(const String& str) const;
		
		/**
		 * Determines whether a string ends with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool endsWith(const sl_char8* str) const;
		
		/**
		 * @return a value indicating whether a specified substring occurs within this string.
		 */
		sl_bool contains(sl_char8 ch) const;
		
		/**
		 * @return a value indicating whether a specified substring occurs within this string.
		 */
		sl_bool contains(const String& str) const;
		
		/**
		 * @return a value indicating whether a specified substring occurs within this string.
		 */
		sl_bool contains(const sl_char8* str) const;
		
		/**
		 * Converts string to an uppercase string.
		 */
		void makeUpper();
		
		/**
		 * Converts string to an lowercase string.
		 */
		void makeLower();
		
		/**
		 * @return a copy of this string converted to uppercase.
		 */
		static String toUpper(const sl_char8* sz, sl_reg len = -1);
		
		/**
		 * @return a copy of this string converted to lowercase.
		 */
		static String toLower(const sl_char8* sz, sl_reg len = -1);
		
		/**
		 * @return a copy of this string converted to uppercase.
		 */
		String toUpper() const;
		
		/**
		 * @return a copy of this string converted to lowercase.
		 */
		String toLower() const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const String& pattern, const String& replacement) const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const String& pattern, const sl_char8* replacement) const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const sl_char8* pattern, const String& replacement) const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
		
		/**
		 * Removes whitespace from both ends of a string.
		 */
		String trim() const;
		
		/**
		 * Removes whitespace from the left end of a string.
		 */
		String trimLeft() const;
		
		/**
		 * Removes whitespace from the right end of a string.
		 */
		String trimRight() const;
		
		/**
		 * Splits a String object into an array of strings by separating the string into substrings.
		 */
		List<String> split(const String& pattern) const;
		
		/**
		 * Splits a String object into an array of strings by separating the string into substrings.
		 */
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
		 * @return the formatted string from the format string and arbitrary list of arguments.
		 *
		 * @param szFormat The buffer containing the format string, this supports the conversion specifiers, length modifiers, and flags.
		 *
		 * String formatting is smiliar with Java Formatter
		 *
		 * https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html
		 *
		 * %[argument_index$][flags][width][.precision]conversion
		 *
		 * @param params Depending on the format string, the function may expect a sequence of additional arguments, each containing a value to be used to replace a format specifier in the format string.
		 * @param nParams Number of parameters
		 */
		static String formatBy(const sl_char8* szFormat, const Variant* params, sl_size nParams);
		
		/**
		 * @return the formatted string from the format string and arbitrary list of arguments.
		 *
		 * @param strFormat Format string, this supports the conversion specifiers, length modifiers, and flags.
		 *
		 * String formatting is smiliar with Java Formatter
		 *
		 * https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html
		 *
		 * %[argument_index$][flags][width][.precision]conversion
		 *
		 * @param params Depending on the format string, the function may expect a sequence of additional arguments, each containing a value to be used to replace a format specifier in the format string.
		 * @param nParams Number of parameters
		 */
		static String formatBy(const String& strFormat, const Variant* params, sl_size nParams);
		
		static String format(const sl_char8* szFormat);
		
		static String format(const String& strFormat);
		
		/**
		 * @return the formatted string from the format string and arbitrary list of arguments.
		 *
		 * @param szFormat The buffer containing the format string, this supports the conversion specifiers, length modifiers, and flags.
		 *
		 * String formatting is smiliar with Java Formatter
		 *
		 * https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html
		 *
		 * %[argument_index$][flags][width][.precision]conversion
		 *
		 * @param args Arbitrary list of arguments.
		 */
		template <class... ARGS>
		static String format(const sl_char8* szFormat, ARGS&&... args);
		
		/**
		 * @return the formatted string from the format string and arbitrary list of arguments.
		 *
		 * @param strFormat Format string, this supports the conversion specifiers, length modifiers, and flags.
		 *
		 * String formatting is smiliar with Java Formatter
		 *
		 * https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html
		 *
		 * %[argument_index$][flags][width][.precision]conversion
		 *
		 * @param args Arbitrary list of arguments.
		 */
		template <class... ARGS>
		static String format(const String& strFormat, ARGS&&... args);
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String::format(*this, params, nParams)
		 *
		 * @param params Arbitrary list of arguments.
		 * @param nParams Number of parameters.
		 */
		String argBy(const Variant* params, sl_size nParams) const;
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String::format(*this, args)
		 *
		 * @param args Arbitrary list of arguments.
		 */
		template <class... ARGS>
		String arg(ARGS&&... args) const;
		
	private:
		static StringContainer* _alloc(sl_size length);
		
		void _replaceContainer(StringContainer* container);
		
		
		static StringContainer* _create(sl_char8 ch, sl_size nRepeatCount);
		
		static StringContainer* _create(const sl_char8* strUtf8, sl_reg length);
		
		static StringContainer* _create(const sl_char16* strUtf16, sl_reg length);
		
		static StringContainer* _create(const sl_char32* strUtf32, sl_reg length);
		
		
		static StringContainer* _merge8(const sl_char8* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
		
		static StringContainer* _merge16(const sl_char8* s1, sl_reg len1, const sl_char16* s2, sl_reg len2);
		
		static StringContainer* _merge16(const sl_char16* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
		
		static StringContainer* _merge32(const sl_char8* s1, sl_reg len1, const sl_char32* s2, sl_reg len2);
		
		static StringContainer* _merge32(const sl_char32* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
		
		
		static sl_bool _equals8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
		
		static sl_bool _equals16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
		
		static sl_bool _equals32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
		
		
		static sl_int32 _compare8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
		
		static sl_int32 _compare16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
		
		static sl_int32 _compare32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
		
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
		 * Allocates and initializes a String, with default settings.
		 */
		constexpr Atomic() : m_container(sl_null) {}
		
		/**
		 * Allocates and initializes a String, with default settings.
		 */
		constexpr Atomic(sl_null_t) : m_container(sl_null) {}
		
		/**
		 * Move constructor of String.
		 */
		Atomic(AtomicString&& src);
		
		/**
		 * Move constructor of String.
		 */
		Atomic(String&& src);
		
		/**
		 * Copy constructor
		 */
		Atomic(const String& src);
		
		/**
		 * Copy constructor
		 */
		Atomic(const AtomicString& src);
		
		/**
		 * Copy constructor
		 */
		Atomic(const String16& src);
		
		/**
		 * Copy constructor
		 */
		Atomic(const AtomicString16& src);
		
		~Atomic();
		
	public:
		
		/**
		 * Fill constructor
		 *
		 * Fill the string with nRepeatCount consecutive copies of charactor ch
		 */
		Atomic(sl_char8 ch, sl_size nRepeatCount);
		
		/**
		 * From utf-8 null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUtf8.
		 */
		Atomic(const char* strUtf8);
		
		/**
		 * From utf-8 string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUtf8
		 */
		Atomic(const char* strUtf8, sl_reg length);
		
		/**
		 * From unicode null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUnicode.
		 */
		Atomic(const wchar_t* strUnicode);
		
		/**
		 * From unicode string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUnicode
		 */
		Atomic(const wchar_t* strUnicode, sl_reg length);
		
		/**
		 * From utf-16 null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUtf16.
		 */
		Atomic(const char16_t* strUtf16);
		
		/**
		 * From utf-16 string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUtf16
		 */
		Atomic(const char16_t* strUtf16, sl_reg length);
		
		/**
		 * From utf-32 null-terminated string buffer
		 *
		 * Copies the null-terminated character sequence pointed by strUtf32.
		 */
		Atomic(const char32_t* strUtf32);
		
		/**
		 * From utf-32 string buffer
		 *
		 * Copies the first 'length' characters from the array of characters pointed by strUtf32
		 */
		Atomic(const char32_t* strUtf32, sl_reg length);
		
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
		 * @return whether the string is null.
		 */
		sl_bool isNull() const;
		
		/**
		 * @return whether the string is not null.
		 */
		sl_bool isNotNull() const;
		
		/**
		 * @return whether the string is empty.
		 */
		sl_bool isEmpty() const;
		
		/**
		 * @return whether the string is not empty.
		 */
		sl_bool isNotEmpty() const;
		
		/**
		 * Sets the string null.
		 */
		void setNull();
		
		/**
		 * Sets the string empty.
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
		 * @return the hash code.
		 */
		sl_uint32 getHashCodeIgnoreCase() const;
		
	public:
		/**
		 * String assignment
		 */
		AtomicString& operator=(sl_null_t);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(AtomicString&& other);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(String&& other);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const String& other);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const AtomicString& other);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const String16& other);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const AtomicString16& _other);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const char* utf8);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const wchar_t* sz);

		/**
		 * String assignment
		 */
		AtomicString& operator=(const char16_t* utf16);
		
		/**
		 * String assignment
		 */
		AtomicString& operator=(const char32_t* utf32);
		
	public:
		/**
		 * Concatenate strings
		 */
		String operator+(const String& other) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const String& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const String16& other) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const String16& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const AtomicString& other) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const AtomicString16& other) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const AtomicString16& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const sl_char8* utf8) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const sl_char8* utf8);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const sl_char16* utf16) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const sl_char16* utf16);
		
		/**
		 * Concatenate strings
		 */
		String operator+(const sl_char32* utf32) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(const sl_char32* utf32);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(const sl_char32* utf32, const AtomicString& second);
		
	public:
		/**
		 * Concatenate strings
		 */
		String operator+(sl_int32 number) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(sl_int32 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_int32 number, const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_uint32 number) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(sl_uint32 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_uint32 number, const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_int64 number) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(sl_int64 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_int64 number, const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_uint64 number) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(sl_uint64 number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_uint64 number, const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(float number) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(float number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(float number, const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(double number) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(double number);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(double number, const AtomicString& other);
		
		/**
		 * Concatenate strings
		 */
		String operator+(sl_bool value) const;
		
		/**
		 * Append to strings
		 */
		AtomicString& operator+=(sl_bool value);
		
		/**
		 * Concatenate strings
		 */
		friend String operator+(sl_bool value, const AtomicString& other);
		
	public:
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const String& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const String16& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const AtomicString& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const AtomicString16& other) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const sl_char8* utf8) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const sl_char16* utf16) const;
		
		/**
		 * @returns true if this string is equal to the specified string.
		 */
		sl_bool equals(const sl_char32* utf32) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String16& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const AtomicString& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const AtomicString16& other) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const sl_char8* utf8) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const sl_char16* utf16) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const sl_char32* utf32) const;
		
		/**
		 * Compares this string to the specified object.
		 * @returns signed integral indicating the relation between the strings:
		 
		 0: They compare equal.
		 
		 < 0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 
		 >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_int32 compare(const String& other, sl_size len) const;
		
		/**
		 * @return true if this string is equal to the specified string.
		 */
		sl_bool equalsIgnoreCase(const String& other) const;
		
		/**
		 * @return true if this string is equal to the specified string.
		 */
		sl_int32 compareIgnoreCase(const String& other) const;
		
	public:
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator==(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator==(const sl_char32* utf32, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator!=(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator!=(const sl_char32* utf32, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>=(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>=(const sl_char32* utf32, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<=(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<=(const sl_char32* utf32, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator>(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator>(const sl_char32* utf32, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const String& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const String16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const AtomicString& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const AtomicString16& other) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const sl_char8* utf8) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const sl_char16* utf16) const;
		
		/**
		 * Comparison Operator
		 */
		sl_bool operator<(const sl_char32* utf32) const;
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char8* utf8, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char16* utf16, const AtomicString& second);
		
		/**
		 * Comparison Operator
		 */
		friend sl_bool operator<(const sl_char32* utf32, const AtomicString& second);
		
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
		 * @return a newly constructed string object with its value initialized to a copy of a substring of this object.
		 */
		String substring(sl_reg start, sl_reg end = -1) const;
		
		/**
		 * @return a string containing a specified number of characters from the left side of a string.
		 */
		String left(sl_reg len) const;
		
		/**
		 * @return a string containing a specified number of characters from the right side of a string.
		 */
		String right(sl_reg len) const;
		
		/**
		 * @return a string that contains a specified number of characters starting from a specified position in a string.
		 */
		String mid(sl_reg start, sl_reg len) const;
		
		/**
		 * @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
		 */
		sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
		
		/**
		 * @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
		 */
		sl_reg indexOf(const String& str, sl_reg start = 0) const;
		
		/**
		 * @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
		 **/
		sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
		
		/**
		 * @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
		 */
		sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
		
		/**
		 * @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
		 */
		sl_reg lastIndexOf(const String& str, sl_reg start = -1) const;
		
		/**
		 * @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
		 */
		sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
		
		/**
		 * Determines whether a string begins with the character, returning true or false as appropriate.
		 */
		sl_bool startsWith(sl_char8 ch) const;
		
		/**
		 * Determines whether a string begins with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool startsWith(const String& str) const;
		
		/**
		 * Determines whether a string begins with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool startsWith(const sl_char8* str) const;
		
		/**
		 * Determines whether a string ends with the character, returning true or false as appropriate.
		 */
		sl_bool endsWith(sl_char8 ch) const;
		
		/**
		 * Determines whether a string ends with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool endsWith(const String& str) const;
		
		/**
		 * Determines whether a string ends with the characters of another string, returning true or false as appropriate.
		 */
		sl_bool endsWith(const sl_char8* str) const;
		
		/**
		 * @return a value indicating whether a specified substring occurs within this string.
		 */
		sl_bool constains(sl_char8 ch) const;
		
		/**
		 * @return a value indicating whether a specified substring occurs within this string.
		 */
		sl_bool contains(const String& str) const;
		
		/**
		 * @return a value indicating whether a specified substring occurs within this string.
		 */
		sl_bool contains(const sl_char8* str) const;
		
		/**
		 * Converts string to an uppercase string.
		 */
		void makeUpper();
		
		/**
		 * Converts string to an lowercase string.
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
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const String& pattern, const String& replacement) const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const String& pattern, const sl_char8* replacement) const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const sl_char8* pattern, const String& replacement) const;
		
		/**
		 * Replaces each substring of this string that matches the given pattern with the given replacement.
		 */
		String replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
		
		/**
		 * Removes whitespace from both ends of a string.
		 */
		String trim() const;
		
		/**
		 * Removes whitespace from the left end of a string.
		 */
		String trimLeft() const;
		
		/**
		 * Removes whitespace from the right end of a string.
		 */
		String trimRight() const;
		
		/**
		 * Splits a String object into an array of strings by separating the string into substrings.
		 */
		List<String> split(const String& pattern) const;
		
		/**
		 * Splits a String object into an array of strings by separating the string into substrings.
		 */
		List<String> split(const sl_char8* pattern) const;
		
	public:
		/**
		 * Parses string argument and outputs a 32 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param _out Pointer to a store where the resulting 32 bit integer is stored.
		 */
		sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;
		
		/**
		 * Parses string argument and returns a 32 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param def Returns def if parsing is failed.
		 */
		sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;
		
		/**
		 * Parses string argument and outputs an unsigned 32 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param _out Pointer to a store where the resulting unsigned 32 bit integer is stored.
		 */
		sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;
		
		/**
		 * Parses string argument and returns an unsigned 32 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param def Returns def if parsing is failed.
		 */
		sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;
		
		/**
		 * Parses string argument and outputs a 64 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param _out Pointer to a store where the resulting 64 bit integer is stored.
		 */
		sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;
		
		/**
		 * Parses string argument and returns a 64 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param def Returns def if parsing is failed.
		 */
		sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;
		
		/**
		 * Parses string argument and outputs an unsigned 64 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param _out Pointer to a store where the resulting unsigned 64 bit integer is stored.
		 */
		sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;
		
		/**
		 * Parses string argument and outputs an unsigned 64 bit integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param def Returns def if parsing is failed.
		 */
		sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;
		
		/**
		 * Parses string argument and outputs an integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param _out Pointer to a store where the resulting integer is stored.
		 */
		sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;
		
		/**
		 * Parses string argument and outputs an integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param def Returns def if parsing is failed.
		 */
		sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;
		
		/**
		 * Parses string argument and outputs an unsigned integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param _out Pointer to a store where the resulting unsigned integer is stored.
		 */
		sl_bool parseSize(sl_int32 radix, sl_size* _out) const;
		
		/**
		 * Parses string argument and returns an unsigned integer of the specified radix
		 *
		 * @param radix This would be used to convert String into integer.
		 * @param def Returns def if parsign is failed.
		 */
		sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;
		
		/**
		 * Parses a string argument and returns a floating point number.
		 *
		 * @param _out Pointer to a store where the resulting flfloating pointoat is stored.
		 */
		sl_bool parseFloat(float* _out) const;
		
		/**
		 * Parses string argument and outputs float of the specified radix
		 *
		 * @param def Returns def if parsing is failed.
		 */
		float parseFloat(float def = 0) const;
		
		/**
		 * Outputs a new double initialized to the value represented by the specified String
		 *
		 * @param _out Pointer to a store.
		 */
		sl_bool parseDouble(double* _out) const;
		
		/**
		 * Outputs a new double initialized to the value represented by the specified String
		 *
		 * @param def Returns def if parsing is failed.
		 */
		double parseDouble(double def = 0) const;
		
		/**
		 * Parses the string as a boolean.
		 *
		 * @param _out Pointer to a store.
		 */
		sl_bool parseBoolean(sl_bool* _out) const;
		
		/**
		 * Parses the string as a boolean.
		 *
		 * @param def Returns def if parsing is failed.
		 */
		sl_bool parseBoolean(sl_bool def = sl_false) const;
		
		/**
		 * Parses the string as a byte array.
		 *
		 * @param _out Pointer to a store.
		 */
		sl_bool parseHexString(void* _out) const;
		
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String::format(*this, params, nParams)
		 *
		 * @param params Arbitrary list of arguments.
		 * @param nParams Number of parameters.
		 */
		String argBy(const Variant* params, sl_size nParams) const;
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String::format(*this, args)
		 *
		 * @param args Arbitrary list of arguments.
		 */
		template <class... ARGS>
		String arg(ARGS&&... args) const;
		
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
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::_String_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static const slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_##name));


#define SLIB_STATIC_STRING_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::_String_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static const slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_##name));

#define SLIB_STATIC_STRING_NULL(name) \
	static slib::_String_Const _static_string_null_##name = {sl_null, 0}; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING_NULL(name) \
	static slib::_String_Const _static_string_null_##name = {sl_null, 0}; \
	static const slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_null_##name));


#endif
