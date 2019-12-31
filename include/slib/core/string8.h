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

#include "string_op.h"

/**
 * @addtogroup core
 *  @{
 */

namespace slib
{

	class String;
	typedef Atomic<String> AtomicString;
	class String16;
	class StringView;
	class StringView16;
	class StringParam;
	class StringStorage;
	class Locale;
	class Variant;
	class Json;
	class Time;

	class SLIB_EXPORT StringContainer
	{
	public:
		sl_char8* sz;
		sl_size len;
		sl_size hash;
		sl_uint32 type;
		sl_reg ref;
		
	public:
		sl_reg increaseReference() noexcept;
		
		sl_reg decreaseReference() noexcept;
		
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
		SLIB_INLINE constexpr String(StringContainer* container) noexcept : m_container(container) {}
		
	public:
		/**
		 * Initializes as a null string.
		 */
		SLIB_INLINE constexpr String() noexcept : m_container(sl_null) {}
		SLIB_INLINE constexpr String(sl_null_t) noexcept : m_container(sl_null) {}
		
		/**
		 * Contructors
		 */
		String(String&& src) noexcept;
		String(AtomicString&& _src) noexcept;
		String(const String& src) noexcept;
		String(const AtomicString& src) noexcept;
		String(const StringView& src) noexcept;

		/**
		 * Destructor
		 */
		~String() noexcept;
		
	public:
		/**
		 * Fill the string with `nRepeatCount` consecutive copies of charactor `ch`
		 */
		String(sl_char8 ch, sl_size nRepeatCount) noexcept;

		/**
		 * Copies the null-terminated character sequence pointed by `str`.
		 */
		String(const char* str) noexcept;

		/**
		 * Copies the first `length` characters from the array of characters pointed by `str`
		 */
		String(const char* str, sl_reg length) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Initialize from `std::string`.
		 * This does not copy the data of the string, but keep the reference to the original string.
		 */
		String(const std::string& str) noexcept;
		String(std::string&& str) noexcept;
#endif
		
	public:
		/**
		 * Creates a string of `len` characters
		 */
		static String allocate(sl_size len) noexcept;

		/**
		 * Creates a string copying other string
		 */
		static String create(const String& str) noexcept;
		static String create(const String16& str) noexcept;
		static String create(const StringView& str) noexcept;
		static String create(const StringView16& str) noexcept;

		/**
		 * Creates a string from the array of characters pointed by `str`
		 */
		static String create(const char* str, sl_reg length = -1) noexcept;
		static String create(const wchar_t* str, sl_reg length = -1) noexcept;
		static String create(const char16_t* str, sl_reg length = -1) noexcept;
		static String create(const char32_t* str, sl_reg length = -1) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Create a string copying from std strings.
		 */
		static String create(const std::string& str) noexcept;
		static String create(const std::wstring& str) noexcept;
		static String create(const std::u16string& str) noexcept;
		static String create(const std::u32string& str) noexcept;
#endif
		
		/**
		 * Creates a string pointing the `str` as the content, without copying the data.
		 * `str` should not be freed while the returned string is being used.
		 */
		template <sl_size N> static String fromStatic(const sl_char8 (&str)[N]) noexcept;
		static String fromStatic(const sl_char8* str, sl_reg len) noexcept;
		
		/**
		 * Creates a string pointing the `str` as the content, without copying the data.
		 * `ref` should be used to keep the alive of the string content.
		 */
		static String fromRef(const Ref<Referable>& ref, const sl_char8* str, sl_reg len = -1) noexcept;
		
		/**
		 * Creates a string pointing the `mem` as the UTF-8 content, without copying the data.
		 */
		static String fromMemory(const Memory& mem) noexcept;

		/**
		 * Creates a string copying the characters from the UTF-8 text.
		 *
		 * @param[in] text string buffer containing the UTF-8 text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf8(const void* text, sl_reg len = -1) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-8 text in `mem`.
		 */
		static String fromUtf8(const Memory& mem) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-16 text.
		 *
		 * @param[in] text string buffer containing the UTF-16 text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf16(const sl_char16* text, sl_reg len = -1) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-32 text.
		 *
		 * @param[in] text string buffer containing the UTF-32 text
		 * @param[in] len length of the `text`. negative value means that `text` is null terminated.
		 */
		static String fromUtf32(const sl_char32* text, sl_reg len = -1) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-16 Big Endian text.
		 *
		 * @param[in] text string buffer containing the UTF-16 Big Endian text
		 * @param[in] size size (in bytes) of the `text`
		 */
		static String fromUtf16BE(const void* text, sl_size size) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-16 Big Endian text in `mem`.
		 */
		static String fromUtf16BE(const Memory& mem) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-16 Little Endian text.
		 *
		 * @param[in] text string buffer containing the UTF-16 Little Endian text
		 * @param[in] size size (in bytes) of the `text`
		 */
		static String fromUtf16LE(const void* text, sl_size size) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-16 Little Endian text in `mem`.
		 */
		static String fromUtf16LE(const Memory& mem) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-8, UTF-16BE, or UTF-16LE text.
		 * This function detects the encoding type from the first 3 bytes of `text`.
		 *
		 * @param[in] text string buffer containing the unicode text.
		 * @param[in] size size (in bytes) of the `text`
		 */
		static String fromUtf(const void* text, sl_size size) noexcept;
		
		/**
		 * Creates a string copying the characters from the UTF-8, UTF-16BE, or UTF-16LE text in `mem`.
		 * This function detects the encoding type from the first 3 bytes of the text.
		 */
		static String fromUtf(const Memory& mem) noexcept;
		
		/**
		 * Creates a string copying the characters from `text` encoded by `charset`.
		 */
		static String decode(Charset charset, const void* text, sl_size size);
		
		/**
		 * Creates a string copying the characters from text in `mem`, encoded by `charset`.
		 */
		static String decode(Charset charset, const Memory& mem);
		
		static const String& from(const String& str) noexcept;
		static String from(const Atomic<String>& str) noexcept;
		static String from(const String16& str) noexcept;
		static String from(const Atomic<String16>& str) noexcept;
		static String from(const StringView& str) noexcept;
		static String from(const StringView16& str) noexcept;
		static String from(const StringParam& str) noexcept;
		static String from(const char* str, sl_reg length = -1) noexcept;
		static String from(const wchar_t* str, sl_reg length = -1) noexcept;
		static String from(const char16_t* str, sl_reg length = -1) noexcept;
		static String from(const char32_t* str, sl_reg length = -1) noexcept;
#ifdef SLIB_SUPPORT_STD_TYPES
		static String from(const std::string& str) noexcept;
		static String from(const std::wstring& str) noexcept;
		static String from(const std::u16string& str) noexcept;
		static String from(const std::u32string& str) noexcept;
#endif
		static String from(signed char value) noexcept;
		static String from(unsigned char value) noexcept;
		static String from(short value) noexcept;
		static String from(unsigned short value) noexcept;
		static String from(int value) noexcept;
		static String from(unsigned int value) noexcept;
		static String from(long value) noexcept;
		static String from(unsigned long value) noexcept;
		static String from(sl_int64 value) noexcept;
		static String from(sl_uint64 value) noexcept;
		static String from(float value) noexcept;
		static String from(double value) noexcept;
		static String from(sl_bool value) noexcept;
		static String from(const Time& value) noexcept;
		static String from(const Json& json) noexcept;
		static String from(const Variant& var) noexcept;
		
	public:
		/**
		 * @return null string.
		 */
		static const String& null() noexcept;
		
		/**
		 * @return empty string.
		 */
		static const String& getEmpty() noexcept;
		
		/**
		 * @return empty string if this string is null. otherwise returns this string.
		 */
		const String& getNotNull() const noexcept;
		
		/**
		 * @return `true` if this string is null.
		 */
		sl_bool isNull() const noexcept;
		
		/**
		 * @return `true` if this string is not null.
		 */
		sl_bool isNotNull() const noexcept;
		
		/**
		 * @return `true` if this string is empty.
		 */
		sl_bool isEmpty() const noexcept;
		
		/**
		 * @return `true` if this string is not empty.
		 */
		sl_bool isNotEmpty() const noexcept;
		
		/**
		 * Sets this string as a null.
		 */
		void setNull() noexcept;
		
		/**
		 * Sets this string as an empty.
		 */
		void setEmpty() noexcept;
		
	public:
		/**
		 * @return string content.
		 */
		sl_char8* getData() const noexcept;
		
		/**
		 * @return string content and length.
		 */
		sl_char8* getData(sl_size& outLength) const noexcept;
		
		/**
		 * @return string length.
		 */
		sl_size getLength() const noexcept;
		
		/**
		 * @return the hash code.
		 */
		sl_size getHashCode() const noexcept;
		static sl_size getHashCode(sl_char8* str, sl_reg len = -1) noexcept;

		/**
		 * @return the hash code ignoring the case.
		 */
		sl_size getHashCodeIgnoreCase() const noexcept;
		static sl_size getHashCodeIgnoreCase(sl_char8* str, sl_reg len = -1) noexcept;

		/**
		 * Sets the string length.
		 *
		 * Don't use for null or empty string
		 */
		void setLength(sl_size len) noexcept;
		
		/**
		 * Sets the hash code.
		 *
		 * Don't use for null or empty string
		 */
		void setHashCode(sl_size hash) noexcept;
		
		/**
		 * @return the character at `index` in string.
		 */
		sl_char8 getAt(sl_reg index) const noexcept;
		
		/**
		 * Sets the character at `index` in string.
		 * @return `true` on success.
		 */
		sl_bool setAt(sl_reg index, sl_char8 ch) noexcept;
		
		sl_char8 operator[](sl_size index) const noexcept;
		
		sl_char8& operator[](sl_size index) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Convert this string to std::string.
		 */
		std::string toStd() const noexcept;
#endif
		
	public:
		/**
		 * String assignment
		 */
		String& operator=(String&& other) noexcept;
		String& operator=(AtomicString&& other) noexcept;
		String& operator=(const String& other) noexcept;
		String& operator=(const AtomicString& other) noexcept;
		String& operator=(const StringView& other) noexcept;
		String& operator=(sl_null_t) noexcept;
		String& operator=(const char* sz) noexcept;
#ifdef SLIB_SUPPORT_STD_TYPES
		String& operator=(const std::string& other) noexcept;
		String& operator=(std::string&& other) noexcept;
#endif

		String& operator+=(String&& other) noexcept;
		String& operator+=(AtomicString&& other) noexcept;
		String& operator+=(const String& other) noexcept;
		String& operator+=(const AtomicString& other) noexcept;
		String& operator+=(sl_null_t) noexcept;
		template <class T>
		String& operator+=(T&& other) noexcept;

	public:
		static String merge(const sl_char8* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept;
		static String merge(const sl_char8* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept;
		static String merge(const sl_char8* a1, sl_reg len1, const char16_t* a2, sl_reg len2) noexcept;
		static String merge(const sl_char8* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept;
		static String merge(const wchar_t* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept;
		static String merge(const char16_t* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept;
		static String merge(const char32_t* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept;

		static sl_bool equals(const sl_char8* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept;
		static sl_bool equals(const sl_char8* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept;
		static sl_bool equals(const sl_char8* a1, sl_reg len1, const char16_t* a2, sl_reg len2) noexcept;
		static sl_bool equals(const sl_char8* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept;
		
		static sl_compare_result compare(const sl_char8* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept;
		static sl_compare_result compare(const sl_char8* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept;
		static sl_compare_result compare(const sl_char8* a1, sl_reg len1, const char16_t* a2, sl_reg len2) noexcept;
		static sl_compare_result compare(const sl_char8* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept;
		
	public:
		PRIV_SLIB_DECLARE_STRING_CLASS_OP(String, sl_bool, equals)

		PRIV_SLIB_DECLARE_STRING_CLASS_OP(String, sl_compare_result, compare)

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
		
	public:
		/**
		 * @return duplicated string.
		 */
		String duplicate() const noexcept;
		
		/**
		 * @return memory containing string content.
		 */
		Memory toMemory() const noexcept;
		
		/**
		 * @return memory containing string content, without creating new heap memory.
		 */
		Memory toStaticMemory() const noexcept;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_size getUtf16(sl_char16* utf16, sl_size len) const noexcept;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_bool getUtf16(StringStorage& output) const noexcept;
		
		/**
		 * Converts to Utf16 and Returns a Memory containing the Utf16 characters and null at last
		 */
		Memory toUtf16() const noexcept;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_size getUtf32(sl_char32* utf32, sl_size len) const noexcept;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_bool getUtf32(StringStorage& output) const noexcept;
		
		/**
		 * Converts to Utf32 and Returns Memory containing the Utf32 characters and null at last
		 */
		Memory toUtf32() const noexcept;
		
		/**
		 * Encodes using `charset` and Returns Memory containing the encoded bytes
		 */
		Memory encode(Charset charset) const;
		
		/**
		 * @return a newly constructed string with its value initialized to a copy of a substring of this string.
		 */
		String substring(sl_reg start, sl_reg end = -1) const noexcept;
		
		/**
		 * @return a string containing a specified number of characters from the left side of this string.
		 */
		String left(sl_reg len) const noexcept;
		
		/**
		 * @return a string containing a specified number of characters from the right side of this string.
		 */
		String right(sl_reg len) const noexcept;
		
		/**
		 * @return a string that contains a specified number of characters starting from a specified position in this string.
		 */
		String mid(sl_reg start, sl_reg len) const noexcept;
		
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
		 * @return a copy of the specified string converted to uppercase.
		 */
		static String toUpper(const sl_char8* str, sl_reg len = -1) noexcept;
		
		/**
		 * @return a copy of the specified string converted to lowercase.
		 */
		static String toLower(const sl_char8* str, sl_reg len = -1) noexcept;
		
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
		 * Copy this string and then removes whitespaces from both ends of the new string.
		 */
		String trim() const noexcept;
		
		/**
		 * Copy this string and then removes whitespaces from the left of the new string.
		 */
		String trimLeft() const noexcept;
		
		/**
		 * Copy this string and then removes whitespaces from the right of the new string.
		 */
		String trimRight() const noexcept;
		
		/**
		 * Splits this string into the list of strings by the `pattern` separator.
		 */
		List<String> split(const StringParam& pattern) const noexcept;
		
		/**
		 * Join all strings in the list
		 */
		static String join(const String* strings, sl_size count, const StringParam& delimiter) noexcept;
		static String join(const String* strings, sl_size count) noexcept;
		static String join(const StringParam* strings, sl_size count, const StringParam& delimiter) noexcept;
		static String join(const StringParam* strings, sl_size count) noexcept;
		static String join(const ListParam<String>& list, sl_size startIndex, sl_size count, const StringParam& delimiter) noexcept;
		static String join(const ListParam<String>& list, sl_size startIndex, sl_size count) noexcept;
		static String join(const ListParam<String>& list, sl_size startIndex, const StringParam& delimiter) noexcept;
		static String join(const ListParam<String>& list, sl_size startIndex) noexcept;
		static String join(const ListParam<String>& list, const StringParam& delimiter) noexcept;
		static String join(const ListParam<String>& list) noexcept;
		template <class... ARGS>
		static String join(const StringParam& s, ARGS&&... args) noexcept;
		
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
		static sl_reg parseInt32(sl_int32 radix, sl_int32* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		static sl_reg parseUint32(sl_int32 radix, sl_uint32* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		static sl_reg parseInt64(sl_int32 radix, sl_int64* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		static sl_reg parseUint64(sl_int32 radix, sl_uint64* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		 * Convert the string (`str`) to a float number value
		 *
		 * @param[out] value Pointer to the result output
		 * @param[in] str String to convert
		 * @param[in] posBegin Zero-based position of the number
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the number if a valid number is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseFloat(float* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		 * Convert the string (`str`) to a double number value
		 *
		 * @param[out] value Pointer to the result output
		 * @param[in] str String to convert
		 * @param[in] posBegin Zero-based position of the number
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the number if a valid number is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseDouble(double* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		static sl_reg parseBoolean(sl_bool* value, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		 * Parses the hex string and writes the bytes to `output`. Format example, "a1a1a1a1" is converted to 4 bytes of 0xA1.
		 *
		 * @param[out] output Pointer to the output buffer.
		 * @param[in] str The string containing the hex string.
		 * @param[in] posBegin The zero-based index of the first character in this string object.
		 * @param[in] posEnd Maximum length of the input string. Parser will stop at this position or at null found before this position.
		 *
		 * @return the position after the boolean keyword if a valid keyword is found at position of `posBegin`, otherwise returns `SLIB_PARSE_ERROR`.
		 */
		static sl_reg parseHexString(void* output, const sl_char8* str, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
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
		
		/**
		 * @return the string representation of 32 bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromInt32(sl_int32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false) noexcept;
		
		/**
		 * @return the string representation of the unsigned 32bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromUint32(sl_uint32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false) noexcept;
		
		/**
		 * @return the string representation of the 64bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromInt64(sl_int64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false) noexcept;
		
		/**
		 * @return the string representation of the unsigned 64bit integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromUint64(sl_uint64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false) noexcept;
		
		/**
		 * @return the string representation of the integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromInt(sl_reg value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false) noexcept;
		
		/**
		 * @return the string representation of the unsigned integer argument.
		 *
		 * @param value The integer to be parsed.
		 * @param radix This would be used to convert integer into String. radix:2 ~ 64.
		 * @param minWidth Minimum width of the result.
		 * @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
		 */
		static String fromSize(sl_size value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false) noexcept;
		
		/**
		 * @return the string representation of the float argument.
		 *
		 * @param value The float value to be parsed.
		 * @param precision The number of characters in decimal. Negative values is ignored and this parameter has not effect.
		 * @param flagZeroPadding If flagZeroPadding is true, zeros are used to pad the field instead of space characters.
		 * @param minWidthIntegral The minimum number of characters in integral field.
		 */
		static String fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1) noexcept;
		
		/**
		 * @return the string representation of the double argument.
		 *
		 * @param value The double value to be parsed.
		 * @param precision The number of characters in decimal. Negative values is ignored and this parameter has not effect.
		 * @param flagZeroPadding If flagZeroPadding is true, zeros are used to pad the field instead of space characters.
		 * @param minWidthIntegral The minimum number of characters in integral field.
		 */
		static String fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1) noexcept;
		
		/**
		 * @return the string representation of the memory address.
		 *
		 * @param pointer The memory address to be parsed.
		 */
		static String fromPointerValue(const void* pointer) noexcept;
		
		/**
		 * @return the string representation of the boolean argument.
		 *
		 * @param value The boolean value to be parsed.
		 */
		static String fromBoolean(sl_bool value) noexcept;
		
		/**
		 * @return the converted hex string from the buffer.
		 *
		 * @param data The buffer to be converted.
		 * @param size Size of the buffer.
		 * @param flagUseLowerChar uses a-f (`true`) or A-F (`false`) for encoding
		 */
		static String makeHexString(const void* data, sl_size size, sl_bool flagUseLowerChar = sl_true) noexcept;
		
		/**
		 * @return the converted hex string from the buffer.
		 *
		 * @param mem The buffer to be converted.
		 * @param flagUseLowerChar uses a-f (`true`) or A-F (`false`) for encoding
		 */
		static String makeHexString(const Memory& mem, sl_bool flagUseLowerChar = sl_true) noexcept;
		
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
		static String format(const StringParam& strFormat) noexcept;
		template <class... ARGS>
		static String format(const StringParam& strFormat, ARGS&&... args) noexcept;
		static String formatBy(const StringParam& strFormat, const Variant* params, sl_size nParams) noexcept;
		static String formatBy(const StringParam& strFormat, const ListParam<Variant>& params) noexcept;
		template <class... ARGS>
		static String format(const Locale& locale, const StringParam& strFormat, ARGS&&... args) noexcept;
		static String formatBy(const Locale& locale, const StringParam& strFormat, const Variant* params, sl_size nParams) noexcept;
		static String formatBy(const Locale& locale, const StringParam& strFormat, const ListParam<Variant>& params) noexcept;

		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments.
		 * It is same as `String::format(*this, ...)`.
		 */
		template <class... ARGS>
		String arg(ARGS&&... args) const noexcept;
		String argBy(const Variant* params, sl_size nParams) const noexcept;
		String argBy(const ListParam<Variant>& params) const noexcept;
		
	private:
		void _replaceContainer(StringContainer* container) noexcept;

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
		SLIB_INLINE constexpr Atomic() noexcept : m_container(sl_null) {}
		SLIB_INLINE constexpr Atomic(sl_null_t) noexcept : m_container(sl_null) {}
		
		/**
		 * Constructors
		 */
		Atomic(String&& src) noexcept;
		Atomic(AtomicString&& src) noexcept;
		Atomic(const String& src) noexcept;
		Atomic(const AtomicString& src) noexcept;
		Atomic(const StringView& src) noexcept;

		/**
		 * Destructor
		 */		
		~Atomic() noexcept;
		
	public:
		/**
		 * Fill the string with `nRepeatCount` consecutive copies of charactor `ch`
		 */
		Atomic(sl_char8 ch, sl_size nRepeatCount) noexcept;
		
		/**
		 * Copies the null-terminated character sequence pointed by `str`.
		 */
		Atomic(const char* str) noexcept;

		/**
		 * Copies the first `length` characters from the array of characters pointed by `str`
		 */
		Atomic(const char* str, sl_reg length) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Initialize from `std::string`.
		 * This does not copy the data of the string, but keep the reference to the original string.
		 */
		Atomic(const std::string& str) noexcept;
		Atomic(std::string&& str) noexcept;
#endif

	public:
		/**
		 * @return null string.
		 */
		static const AtomicString& null() noexcept;
		
		/**
		 * @return empty string.
		 */
		static const AtomicString& getEmpty() noexcept;
		
		/**
		 * @return `true` if this string is null.
		 */
		sl_bool isNull() const noexcept;
		
		/**
		 * @return `true` if this string is not null.
		 */
		sl_bool isNotNull() const noexcept;
		
		/**
		 * @return `true` if this string is empty.
		 */
		sl_bool isEmpty() const noexcept;
		
		/**
		 * @return `true` if this string is not empty.
		 */
		sl_bool isNotEmpty() const noexcept;
		
		/**
		 * Sets this string as a null.
		 */
		void setNull() noexcept;
		
		/**
		 * Sets this string as an empty.
		 */
		void setEmpty() noexcept;
		
		/**
		 * @return string length.
		 */
		sl_size getLength() const noexcept;
		
		/**
		 * @return the hash code.
		 */
		sl_size getHashCode() const noexcept;
		
		/**
		 * @return the hash code ignoring the case.
		 */
		sl_size getHashCodeIgnoreCase() const noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		/**
		 * Convert this string to std::string.
		 */
		std::string toStd() const noexcept;
#endif

	public:
		/**
		 * String assignment
		 */
		AtomicString& operator=(String&& other) noexcept;
		AtomicString& operator=(AtomicString&& other) noexcept;
		AtomicString& operator=(const String& other) noexcept;
		AtomicString& operator=(const AtomicString& other) noexcept;
		AtomicString& operator=(const StringView& other) noexcept;
		AtomicString& operator=(sl_null_t) noexcept;
		AtomicString& operator=(const char* sz) noexcept;
#ifdef SLIB_SUPPORT_STD_TYPES
		AtomicString& operator=(const std::string& other) noexcept;
		AtomicString& operator=(std::string&& other) noexcept;
#endif

		AtomicString& operator+=(String&& other) noexcept;
		AtomicString& operator+=(AtomicString&& other) noexcept;
		AtomicString& operator+=(const String& other) noexcept;
		AtomicString& operator+=(const AtomicString& other) noexcept;
		AtomicString& operator+=(sl_null_t) noexcept;
		template <class T>
		AtomicString& operator+=(T&& other) noexcept;

	public:
		PRIV_SLIB_DECLARE_STRING_CLASS_OP(String, sl_bool, equals)
		
		PRIV_SLIB_DECLARE_STRING_CLASS_OP(String, sl_compare_result, compare)

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
		
	public:
		/**
		 * @return duplicated string.
		 */
		String duplicate() const noexcept;
		
		/**
		 * @return memory containing string content.
		 */
		Memory toMemory() const noexcept;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_size getUtf16(sl_char16* utf16, sl_size len) const noexcept;
		
		/**
		 * Fills Utf16 characters to the provided buffer
		 */
		sl_bool getUtf16(StringStorage& output) const noexcept;
		
		/**
		 * Converts to Utf16 and Returns a Memory containing the Utf16 characters and null at last
		 */
		Memory toUtf16() const noexcept;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_size getUtf32(sl_char32* utf32, sl_size len) const noexcept;
		
		/**
		 * Fills Utf32 characters to the provided buffer
		 */
		sl_bool getUtf32(StringStorage& output) const noexcept;
		
		/**
		 * Converts to Utf32 and Returns Memory containing the Utf32 characters and null at last
		 */
		Memory toUtf32() const noexcept;
		
		/**
		 * Encodes using `charset` and Returns Memory containing the encoded bytes
		 */
		Memory encode(Charset charset) const;
		
		/**
		 * @return a newly constructed string with its value initialized to a copy of a substring of this string.
		 */
		String substring(sl_reg start, sl_reg end = -1) const noexcept;
		
		/**
		 * @return a string containing a specified number of characters from the left side of this string.
		 */
		String left(sl_reg len) const noexcept;
		
		/**
		 * @return a string containing a specified number of characters from the right side of this string.
		 */
		String right(sl_reg len) const noexcept;
		
		/**
		 * @return a string that contains a specified number of characters starting from a specified position in a string.
		 */
		String mid(sl_reg start, sl_reg len) const noexcept;
		
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
		sl_bool constains(sl_char8 ch) const noexcept;
		
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
		 * Copy this string and then removes whitespaces from both ends of the new string.
		 */
		String trim() const noexcept;
		
		/**
		 * Copy this string and then removes whitespaces from the left of the new string.
		 */
		String trimLeft() const noexcept;
		
		/**
		 * Copy this string and then removes whitespaces from the right of the new string.
		 */
		String trimRight() const noexcept;
		
		/**
		 * Splits this string into the list of strings by the `pattern` separator.
		 */
		List<String> split(const StringParam& pattern) const noexcept;
		
	public:
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
		
		/**
		 * Formats the current string which contains conversion specifications with arbitrary list of arguments.
		 * It is same as `String::format(*this, ...)`.
		 */
		template <class... ARGS>
		String arg(ARGS&&... args) const noexcept;
		String argBy(const Variant* params, sl_size nParams) const noexcept;
		String argBy(const ListParam<Variant>& params) const noexcept;
		
	private:
		StringContainer* _retainContainer() const noexcept;
		void _replaceContainer(StringContainer* other) noexcept;

		friend class String;
	};
	
	
	template <>
	class Compare<String>
	{
	public:
		sl_compare_result operator()(const String& a, const String& b) const noexcept;
	};
	
	template <>
	class Equals<String>
	{
	public:
		sl_bool operator()(const String& a, const String& b) const noexcept;
	};
	
	template <>
	class Hash<String>
	{
	public:
		sl_size operator()(const String& a) const noexcept;
	};
	
	class CompareIgnoreCaseString
	{
	public:
		sl_compare_result operator()(const String& a, const String& b) const noexcept;
	};
	
	class EqualsIgnoreCaseString
	{
	public:
		sl_bool operator()(const String& a, const String& b) const noexcept;
	};
	
	class HashIgnoreCaseString
	{
	public:
		sl_size operator()(const String& v) const noexcept;
		
	};

}

/// @}

#define SLIB_STATIC_STRING(name, str) \
	auto& _static_string_content_##name = str; \
	static slib::StringContainer _static_string_container_##name = {(sl_char8*)_static_string_content_##name, sizeof(_static_string_content_##name)-1, 0, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static const slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_RETURN_STRING(str) { SLIB_STATIC_STRING(strRetTemp, str) return strRetTemp; }

#endif
