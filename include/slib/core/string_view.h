/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_CORE_STRING_VIEW
#define CHECKHEADER_SLIB_CORE_STRING_VIEW

#include "definition.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <string>
#endif

#include "string_op.h"

namespace slib
{

	class String;
	class String16;

	class SLIB_EXPORT StringView
	{
	public:
		SLIB_INLINE StringView() noexcept: data(sl_null), length(0) {}
		
		SLIB_INLINE StringView(sl_null_t) noexcept: data(sl_null), length(0) {}
		
		SLIB_INLINE StringView(const StringView& other) noexcept: data(other.data), length(other.length) {}
		
	public:
		StringView(const String& value) noexcept;
		
		StringView(String const&&) = delete;
		
		StringView(const sl_char8* sz) noexcept;
		
		StringView(const sl_char8* str, sl_reg length) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		StringView(const std::string& str) noexcept;
		
		StringView(std::string const&&) = delete;
#endif
		
	public:
		StringView& operator=(sl_null_t) noexcept;
		
		StringView& operator=(const StringView& other) noexcept;
		
		StringView& operator=(const String& value) noexcept;
				
		StringView& operator=(String const&&) = delete;
		
		StringView& operator=(const sl_char8* sz) noexcept;
				
#ifdef SLIB_SUPPORT_STD_TYPES
		StringView& operator=(const std::string& str) noexcept;
		
		StringView& operator=(std::string const&&) = delete;
#endif
		
		sl_char8 operator[](sl_size index) const noexcept;
		
		sl_char8& operator[](sl_size index) noexcept;
		
	public:
		template <sl_size N>
		static StringView literal(const sl_char8 (&s)[N]) noexcept;
		
		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		sl_char8* getUnsafeData() const noexcept;
		
		sl_reg getUnsafeLength() const noexcept;
		
		sl_char8* getData() const noexcept;
		
		sl_size getLength() const noexcept;
		
		sl_size getHashCode() const noexcept;
		
		sl_size getHashCodeIgnoreCase() const noexcept;
		
	public:
		PRIV_SLIB_DECLARE_STRING_CLASS_OP_TEMPLATE(sl_bool, equals)

		PRIV_SLIB_DECLARE_STRING_CLASS_OP_TEMPLATE(sl_compare_result, compare)

	public:
		/**
		 * Compares this string to the specified string.
		 * This functions stops searching on the index of `len-1` and returns 0.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_compare_result compare(const StringParam& other, sl_size len) const noexcept;
		
		/**
		 * @return true if this string is equal to the specified string ignoring the case.
		 */
		sl_bool equalsIgnoreCase(const StringParam& other) const noexcept;
		
		/**
		 * Compares this string to the specified string ignoring the case.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_compare_result compareIgnoreCase(const StringParam& other) const noexcept;
		
		/**
		 * @return a view of the substring
		 */
		StringView substring(sl_reg start, sl_reg end = -1) const noexcept;
		
		/**
		 * @return a view of the left substring
		 */
		StringView left(sl_reg len) const noexcept;
		
		/**
		 * @return a view of the right substring
		 */
		StringView right(sl_reg len) const noexcept;
		
		/**
		 * @return a view of the substring
		 */
		StringView mid(sl_reg start, sl_reg len) const noexcept;
		
		/**
		 * @return the index within this string of the first occurrence of the specified character, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const noexcept;
		
		/**
		 * @return the index within this string of the first occurrence of the specified string, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(const StringParam& str, sl_reg start = 0) const noexcept;
		
		/**
		 * @return the index within this string of the last occurrence of the specified character, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const noexcept;
		
		/**
		 * @return the index within this string of the last occurrence of the specified string, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(const StringParam& str, sl_reg start = -1) const noexcept;
		
		/**
		 * @return `true` if this string starts with the specified character.
		 */
		sl_bool startsWith(sl_char8 ch) const noexcept;
		
		/**
		 * @return `true` if this string starts with the specified string.
		 */
		sl_bool startsWith(const StringParam& str) const noexcept;
		
		/**
		 * @return `true` if this string ends with the specified character.
		 */
		sl_bool endsWith(sl_char8 ch) const noexcept;
		
		/**
		 * @return `true` if this string ends with the specified string.
		 */
		sl_bool endsWith(const StringParam& str) const noexcept;
		
		/**
		 * @return `true` if the specified character occurs within this string.
		 */
		sl_bool contains(sl_char8 ch) const noexcept;
		
		/**
		 * @return `true` if the specified substring occurs within this string.
		 */
		sl_bool contains(const StringParam& str) const noexcept;
		
		/**
		 * Converts the characters of this string to uppercase.
		 */
		void makeUpper() noexcept;
		
		/**
		 * Converts the characters of this string to lowercase.
		 */
		void makeLower() noexcept;
		
		/**
		 * @return a copy of this string converted to uppercase.
		 */
		String toUpper() const noexcept;
		
		/**
		 * @return a copy of this string converted to lowercase.
		 */
		String toLower() const noexcept;
		
		/**
		 * Replaces each substring of this string that matches the given `pattern` with the given `replacement`.
		 */
		String replaceAll(const StringParam& pattern, const StringParam& replacement) const noexcept;
		
		/**
		 * a view of the substring that removed whitespaces from both ends of the new string.
		 */
		StringView trim() const noexcept;
		
		/**
		 * a view of the substring that removed whitespaces from the left of the new string.
		 */
		StringView trimLeft() const noexcept;
		
		/**
		 * a view of the substring that removed whitespaces from the right of the new string.
		 */
		StringView trimRight() const noexcept;
		
		/**
		 * Splits this string into the list of strings by the `pattern` separator.
		 */
		List<StringView> split(const StringParam& pattern) const noexcept;
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt32(sl_int32 radix, sl_int32* value) const noexcept;
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const noexcept;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint32(sl_int32 radix, sl_uint32* value) const noexcept;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const noexcept;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt64(sl_int32 radix, sl_int64* value) const noexcept;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const noexcept;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint64(sl_int32 radix, sl_uint64* value) const noexcept;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const noexcept;
		
		/**
		 * Convert this string to an integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt(sl_int32 radix, sl_reg* value) const noexcept;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const noexcept;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseSize(sl_int32 radix, sl_size* value) const noexcept;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const noexcept;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseFloat(float* value) const noexcept;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		float parseFloat(float def = 0) const noexcept;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseDouble(double* value) const noexcept;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		double parseDouble(double def = 0) const noexcept;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseBoolean(sl_bool* value) const noexcept;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		sl_bool parseBoolean(sl_bool def = sl_false) const noexcept;
		
		/**
		 * Parses this hex string and writes the bytes to `output`. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @param[out] output Pointer to the output buffer.
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseHexString(void* output) const noexcept;
		
		/**
		 * Parses this hex string and returns hex data. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @return parsed hex data
		 */
		Memory parseHexString() const noexcept;		

	protected:
		sl_char8* data;
		mutable sl_reg length;

	};

	template <>
	class Compare<StringView>
	{
	public:
		sl_compare_result operator()(const StringView& a, const StringView& b) const noexcept;
	};

	template <>
	class Equals<StringView>
	{
	public:
		sl_bool operator()(const StringView& a, const StringView& b) const noexcept;
	};

	template <>
	class Hash<StringView>
	{
	public:
		sl_size operator()(const StringView& a) const noexcept;
	};


	class SLIB_EXPORT StringView16
	{
	public:
		SLIB_INLINE StringView16() noexcept: data(sl_null), length(0) {}
		
		SLIB_INLINE StringView16(sl_null_t) noexcept: data(sl_null), length(0) {}
		
		SLIB_INLINE StringView16(const StringView16& other) noexcept: data(other.data), length(other.length) {}
		
	public:
		StringView16(const String16& value) noexcept;
		
		StringView16(String16 const&&) = delete;
		
		StringView16(const sl_char16* sz) noexcept;
		
		StringView16(const sl_char16* str, sl_reg length) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		StringView16(const std::u16string& str) noexcept;
		
		StringView16(std::u16string const&&) = delete;
#endif
		
	public:
		StringView16& operator=(sl_null_t) noexcept;
		
		StringView16& operator=(const StringView16& other) noexcept;
		
		StringView16& operator=(const String16& value) noexcept;
				
		StringView16& operator=(String16 const&&) = delete;
		
		StringView16& operator=(const sl_char16* sz) noexcept;
				
#ifdef SLIB_SUPPORT_STD_TYPES
		StringView16& operator=(const std::u16string& str) noexcept;
		
		StringView16& operator=(std::u16string const&&) = delete;
#endif
		
		sl_char16 operator[](sl_size index) const noexcept;
		
		sl_char16& operator[](sl_size index) noexcept;
		
	public:
		template <sl_size N>
		static StringView16 literal(const sl_char16 (&s)[N]) noexcept;
		
		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		sl_char16* getUnsafeData() const noexcept;
		
		sl_reg getUnsafeLength() const noexcept;
		
		sl_char16* getData() const noexcept;
		
		sl_size getLength() const noexcept;
		
		sl_size getHashCode() const noexcept;
		
		sl_size getHashCodeIgnoreCase() const noexcept;
		
	public:
		PRIV_SLIB_DECLARE_STRING_CLASS_OP_TEMPLATE(sl_bool, equals)

		PRIV_SLIB_DECLARE_STRING_CLASS_OP_TEMPLATE(sl_compare_result, compare)

	public:
		/**
		 * Compares this string to the specified string.
		 * This functions stops searching on the index of `len-1` and returns 0.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_compare_result compare(const StringParam& other, sl_size len) const noexcept;
		
		/**
		 * @return true if this string is equal to the specified string ignoring the case.
		 */
		sl_bool equalsIgnoreCase(const StringParam& other) const noexcept;
		
		/**
		 * Compares this string to the specified string ignoring the case.
		 *
		 * @return signed integral indicating the relation between the strings:
		 * @return 0: They compare equal.
		 * @return <0: Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
		 * @return >0: Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
		 */
		sl_compare_result compareIgnoreCase(const StringParam& other) const noexcept;
		
		/**
		 * @return a view of the substring
		 */
		StringView16 substring(sl_reg start, sl_reg end = -1) const noexcept;
		
		/**
		 * @return a view of the left substring
		 */
		StringView16 left(sl_reg len) const noexcept;
		
		/**
		 * @return a view of the right substring
		 */
		StringView16 right(sl_reg len) const noexcept;
		
		/**
		 * @return a view of the substring
		 */
		StringView16 mid(sl_reg start, sl_reg len) const noexcept;
		
		/**
		 * @return the index within this string of the first occurrence of the specified character, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(sl_char16 ch, sl_reg start = 0) const noexcept;
		
		/**
		 * @return the index within this string of the first occurrence of the specified string, starting the search at `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg indexOf(const StringParam& str, sl_reg start = 0) const noexcept;
		
		/**
		 * @return the index within this string of the last occurrence of the specified character, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const noexcept;
		
		/**
		 * @return the index within this string of the last occurrence of the specified string, searching backwards from `start` index.
		 * @return -1 if no occurrence is found.
		 */
		sl_reg lastIndexOf(const StringParam& str, sl_reg start = -1) const noexcept;
		
		/**
		 * @return `true` if this string starts with the specified character.
		 */
		sl_bool startsWith(sl_char16 ch) const noexcept;
		
		/**
		 * @return `true` if this string starts with the specified string.
		 */
		sl_bool startsWith(const StringParam& str) const noexcept;
		
		/**
		 * @return `true` if this string ends with the specified character.
		 */
		sl_bool endsWith(sl_char16 ch) const noexcept;
		
		/**
		 * @return `true` if this string ends with the specified string.
		 */
		sl_bool endsWith(const StringParam& str) const noexcept;
		
		/**
		 * @return `true` if the specified character occurs within this string.
		 */
		sl_bool contains(sl_char16 ch) const noexcept;
		
		/**
		 * @return `true` if the specified substring occurs within this string.
		 */
		sl_bool contains(const StringParam& str) const noexcept;
		
		/**
		 * Converts the characters of this string to uppercase.
		 */
		void makeUpper() noexcept;
		
		/**
		 * Converts the characters of this string to lowercase.
		 */
		void makeLower() noexcept;
		
		/**
		 * @return a copy of this string converted to uppercase.
		 */
		String16 toUpper() const noexcept;
		
		/**
		 * @return a copy of this string converted to lowercase.
		 */
		String16 toLower() const noexcept;
		
		/**
		 * Replaces each substring of this string that matches the given `pattern` with the given `replacement`.
		 */
		String16 replaceAll(const StringParam& pattern, const StringParam& replacement) const noexcept;
		
		/**
		 * a view of the substring that removed whitespaces from both ends of the new string.
		 */
		StringView16 trim() const noexcept;
		
		/**
		 * a view of the substring that removed whitespaces from the left of the new string.
		 */
		StringView16 trimLeft() const noexcept;
		
		/**
		 * a view of the substring that removed whitespaces from the right of the new string.
		 */
		StringView16 trimRight() const noexcept;
		
		/**
		 * Splits this string into the list of strings by the `pattern` separator.
		 */
		List<StringView16> split(const StringParam& pattern) const noexcept;
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt32(sl_int32 radix, sl_int32* value) const noexcept;
		
		/**
		 * Convert this string to a 32 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const noexcept;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint32(sl_int32 radix, sl_uint32* value) const noexcept;
		
		/**
		 * Convert this string to a 32 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const noexcept;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt64(sl_int32 radix, sl_int64* value) const noexcept;
		
		/**
		 * Convert this string to a 64 bit integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const noexcept;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseUint64(sl_int32 radix, sl_uint64* value) const noexcept;
		
		/**
		 * Convert this string to a 64 bit unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const noexcept;
		
		/**
		 * Convert this string to an integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseInt(sl_int32 radix, sl_reg* value) const noexcept;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const noexcept;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if this string is valid integer
		 */
		sl_bool parseSize(sl_int32 radix, sl_size* value) const noexcept;
		
		/**
		 * Convert this string to an unsigned integer of the specified radix.
		 *
		 * @param[in] radix Numerical base used to represent the integer. For example, use 10 for the decimal system.
		 * @param[in] def Default return value for the non-integer string
		 *
		 * @return Result integer if the conversion is successful, otherwise returns `def`
		 */
		sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const noexcept;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseFloat(float* value) const noexcept;
		
		/**
		 * Convert this string to a float number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		float parseFloat(float def = 0) const noexcept;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseDouble(double* value) const noexcept;
		
		/**
		 * Convert this string to a double number value.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		double parseDouble(double def = 0) const noexcept;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[out] value Pointer to the result output
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseBoolean(sl_bool* value) const noexcept;
		
		/**
		 * Convert this string to a boolean value.
		 * "yes", "YES", "Yes", "true", "TRUE" and "True" are converted to `true`.
		 * "no", "NO", "No", "false", "FALSE" and "False" are converted to `false`.
		 *
		 * @param[in] def Default return value on failure
		 *
		 * @return Result value if the conversion is successful, otherwise returns `def`
		 */
		sl_bool parseBoolean(sl_bool def = sl_false) const noexcept;
		
		/**
		 * Parses this hex string and writes the bytes to `output`. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @param[out] output Pointer to the output buffer.
		 *
		 * @return `true` if the conversion is success
		 */
		sl_bool parseHexString(void* output) const noexcept;
		
		/**
		 * Parses this hex string and returns hex data. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @return parsed hex data
		 */
		Memory parseHexString() const noexcept;		

	protected:
		sl_char16* data;
		mutable sl_reg length;

	};

	template <>
	class Compare<StringView16>
	{
	public:
		sl_compare_result operator()(const StringView16& a, const StringView16& b) const noexcept;
	};

	template <>
	class Equals<StringView16>
	{
	public:
		sl_bool operator()(const StringView16& a, const StringView16& b) const noexcept;
	};

	template <>
	class Hash<StringView16>
	{
	public:
		sl_size operator()(const StringView16& a) const noexcept;
	};

}

#endif
