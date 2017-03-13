#ifndef CHECKHEADER_SLIB_CORE_STRING16
#define CHECKHEADER_SLIB_CORE_STRING16

#include "definition.h"

#include "charset.h"
#include "memory.h"
#include "list.h"
#include "hash.h"

namespace slib
{

	class String;
	typedef Atomic<String> AtomicString;
	class String16;
	typedef Atomic<String16> AtomicString16;
	class StringData;
	class Variant;

	class SLIB_EXPORT StringContainer16
	{
	public:
#if defined(SLIB_COMPILER_IS_VC) || (defined(SLIB_COMPILER_IS_GCC) && defined(__SIZEOF_WCHAR_T__) && (__SIZEOF_WCHAR_T__ == 2))
		union {
			sl_char16* sz;
			wchar_t* szDebug;
		};
#else
		sl_char16* sz;
#endif
		sl_size len;
		sl_uint32 hash;
		sl_reg ref;
		
	public:
		sl_reg increaseReference();
		
		sl_reg decreaseReference();
		
	};


/** 
* @addtogroup core
*  @{
*/

/** @class String16
 * @brief String16 class provides an extensive set of APIs for working with strings, including method for comparing,
 searching, and modifying strings.
*/

	class SLIB_EXPORT String16
	{
	private:
		StringContainer16* m_container;
		
	private:
		constexpr String16(StringContainer16* container) : m_container(container) {}

	public:

/**
* Allocates and initializes a String, with default settings.
*/
		constexpr String16() : m_container(sl_null) {}
		
/**
* Allocates and initializes a String, with default settings.
*/
		constexpr String16(sl_null_t) : m_container(sl_null) {}

/**
* Move constructor of String.
*/	
		String16(String16&& src);
		
/**
* Move constructor of String.
*/
		String16(AtomicString16&& _src);
		
/**
* Copy constructor
*/
		String16(const String16& src);
		
/**
* Copy constructor
*/
		String16(const AtomicString16& src);

/**
* Copy constructor
*/
		String16(const String& src);
		
/**
* Copy constructor
*/
		String16(const AtomicString& src);

		~String16();
		
	public:

/**
* Fill constructor
*
* Fill the string with nRepeatCount consecutive copies of charactor ch
*/
		String16(sl_char16 ch, sl_size nRepeatCount);
		
/**
* From utf-8 null-terminated string buffer
*
* Copies the null-terminated character sequence pointed by strUtf8.
*/
		String16(const sl_char8* strUtf8);
		
/**
* From utf-8 string buffer
*
* Copies the first 'length' characters from the array of characters pointed by strUtf8
*/
		String16(const sl_char8* strUtf8, sl_reg length);
		
/**
* From utf-16 null-terminated string buffer
*
* Copies the null-terminated character sequence pointed by strUtf16.
*/
		String16(const sl_char16* strUtf16);
		
/**
* From utf-16 string buffer
*
* Copies the first 'length' characters from the array of characters pointed by strUtf16
*/
		String16(const sl_char16* strUtf16, sl_reg length);
		
/**
* From utf-32 null-terminated string buffer
*
* Copies the null-terminated character sequence pointed by strUtf32.
*/
		String16(const sl_char32* strUtf32);
		
/**
* From utf-32 string buffer
*
* Copies the first 'length' characters from the array of characters pointed by strUtf32
*/
		String16(const sl_char32* strUtf32, sl_reg length);

	public:

/**
* Create a String of 'len' characters
* @return An automatically released String16 object.
*/
		static String16 allocate(sl_size len);

/**
* Copies the first 'len' characters from the array of characters pointed by sz16
* @return An automatically released String16 object.
*/	
		static String16 fromStatic(const sl_char16* sz16, sl_reg len = -1);

/**
* From Utf8 string
*
* Copies the first 'len' characters from the array of characters pointed by utf8
* @return An automatically released String16 object.
*/
		static String16 fromUtf8(const void* utf8, sl_reg len = -1);

/**
* From string buffer
*
* Copies characters in mem.
* @return An automatically released String16 object.
*/
		static String16 fromUtf8(const Memory& mem);

/**
* From Utf16 string
*
* Copies the first 'len' characters from the array of characters pointed by utf16
* @return An automatically released String16 object.
*/
		static String16 fromUtf16(const sl_char16* utf16, sl_reg len = -1);

/**
* From Utf32 string
*
* Copies the first 'len' characters from the array of characters pointed by utf32
* @return An automatically released String16 object.
*/
		static String16 fromUtf32(const sl_char32* utf32, sl_reg len = -1);

/**
* From Utf16 Big-Endian string
*
* @return An automatically released String16 object.
*/
		static String16 fromUtf16BE(const void* utf16, sl_reg len = -1);

/**
* From Utf16 Big-Endian buffer
*
* @return An automatically released String16 object.
*/
		static String16 fromUtf16BE(const Memory& mem);

/**
* From Utf16 Little-Endian buffer
*
* @return An automatically released String16 object.
*/
		static String16 fromUtf16LE(const void* utf16, sl_reg len = -1);

/**
* From Utf16 Big-Endian buffer
*
* @return An automatically released String16 object.
*/
		static String16 fromUtf16LE(const Memory& mem);

/**
* Process UTF16BE, UTF16LE, UTF8 strings automatically.
*
* @return An automatically released String16 object.
*/
		static String16 fromUtf(const void* buf, sl_size len);

/**
* Process UTF16BE, UTF16LE, UTF8 strings automatically.
*
* @return An automatically released String16 object.
*/
		static String16 fromUtf(const Memory& mem);
		
	public:
/**
* @return null string.
*/
		static const String16& null();
		
/**
* @return empty string.
*/
		static const String16& getEmpty();
		
/**
* @return empty string if string is null.
*/
		const String16& getNotNull() const;
		
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
		sl_char16* getData() const;
		
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
		sl_char16 getAt(sl_reg index) const;
		
/**
* Sets the character at index in string.
* @return whether the setting is successed.
*/
		sl_bool setAt(sl_reg index, sl_char16 ch);
		
	public:
/**
* String16 assignment
*/
		String16& operator=(sl_null_t);

/**
* String16 assignment
*/	
		String16& operator=(String16&& other);

/**
* String16 assignment
*/	
		String16& operator=(AtomicString16&& _other);

/**
* String16 assignment
*/
		String16& operator=(const String16& other);

/**
* String16 assignment
*/	
		String16& operator=(const AtomicString16& other);

/**
* String16 assignment
*/	
		String16& operator=(const String& other);

/**
* String16 assignment
*/	
		String16& operator=(const AtomicString& other);

/**
* String16 assignment
*/	
		String16& operator=(const sl_char8* utf8);

/**
* String16 assignment
*/	
		String16& operator=(const sl_char16* utf16);

/**
* String16 assignment
*/	
		String16& operator=(const sl_char32* utf32);
		
	public:
/**
* Concatenate strings
*/
		String16 operator+(const String16& other) const;

/**
* Append to strings
*/	
		String16& operator+=(const String16& other);

/**
* Concatenate strings
*/
		String16 operator+(const String& other) const;

/**
* Append to strings
*/	
		String16& operator+=(const String& other);

/**
* Concatenate strings
*/
		String16 operator+(const AtomicString16& _other) const;
		
/**
* Append to strings
*/	
		String16& operator+=(const AtomicString16& other);

/**
* Concatenate strings
*/
		String16 operator+(const AtomicString& other) const;
		
/**
* Append to strings
*/	
		String16& operator+=(const AtomicString& other);

/**
* Concatenate strings
*/
		String16 operator+(const sl_char8* utf8) const;
		
/**
* Append to strings
*/	
		String16& operator+=(const sl_char8* utf8);

/**
* Concatenate strings
*/
		String16 operator+(const sl_char16* utf16) const;
		
/**
* Append to strings
*/	
		String16& operator+=(const sl_char16* utf16);

/**
* Concatenate strings
*/
		String16 operator+(const sl_char32* utf32) const;
		
/**
* Append to strings
*/	
		String16& operator+=(const sl_char32* utf32);

/**
* Concatenate strings
*/
		friend String16 operator+(const sl_char8* utf8, const String16& second);

/**
* Concatenate strings
*/	
		friend String16 operator+(const sl_char16* utf16, const String16& second);

/**
* Concatenate strings
*/
		friend String16 operator+(const sl_char32* utf32, const String16& second);

	public:
/**
* Concatenate strings
*/
		String16 operator+(sl_int32 number) const;
		
/**
* Append to strings
*/
		String16& operator+=(sl_int32 number);

/**
* Concatenate strings
*/
		friend String16 operator+(sl_int32 number, const String16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_uint32 number) const;
		
/**
* Append to strings
*/
		String16& operator+=(sl_uint32 number);

/**
* Concatenate strings
*/
		friend String16 operator+(sl_uint32 number, const String16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_int64 number) const;

/**
* Append to strings
*/	
		String16& operator+=(sl_int64 number);

/**
* Concatenate strings
*/
		friend String16 operator+(sl_int64 number, const String16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_uint64 number) const;

/**
* Append to strings
*/	
		String16& operator+=(sl_uint64 number);

/**
* Concatenate strings
*/
		friend String16 operator+(sl_uint64 number, const String16& other);

/**
* Concatenate strings
*/	
		String16 operator+(float number) const;

/**
* Append to strings
*/	
		String16& operator+=(float number);

/**
* Concatenate strings
*/	
		friend String16 operator+(float number, const String16& other);

/**
* Concatenate strings
*/	
		String16 operator+(double number) const;

/**
* Append to strings
*/	
		String16& operator+=(double number);

/**
* Concatenate strings
*/
		friend String16 operator+(double number, const String16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_bool value) const;

/**
* Append to strings
*/	
		String16& operator+=(sl_bool value);

/**
* Concatenate strings
*/	
		friend String16 operator+(sl_bool value, const String16& other);
		
	public:

/**
* @returns true if this string is equal to the specified string.
*/
		sl_bool equals(const String16& other) const;
		
/**
* @returns true if this string is equal to the specified string.
*/	
		sl_bool equals(const String& other) const;
		
/**
* @returns true if this string is equal to the specified string.
*/	
		sl_bool equals(const AtomicString16& other) const;
		
/**
* @returns true if this string is equal to the specified string.
*/	
		sl_bool equals(const AtomicString& other) const;
		
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
		sl_int32 compare(const String16& other) const;

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
		sl_int32 compare(const AtomicString16& other) const;

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
		sl_int32 compare(const String16& other, sl_size len) const;

/**
* @return true if this string is equal to the specified string.
*/	
		sl_bool equalsIgnoreCase(const String16& other) const;
		
/**
* @return true if this string is equal to the specified string.
*/
		sl_int32 compareIgnoreCase(const String16& other) const;

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
		friend sl_bool operator==(const sl_char8* utf8, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator==(const sl_char16* utf16, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator==(const sl_char32* utf32, const String16& second);
		
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
		friend sl_bool operator!=(const sl_char8* utf8, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator!=(const sl_char16* utf16, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator!=(const sl_char32* utf32, const String16& second);
		
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
		friend sl_bool operator>=(const sl_char8* utf8, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>=(const sl_char16* utf16, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>=(const sl_char32* utf32, const String16& second);
		
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
		friend sl_bool operator<=(const sl_char8* utf8, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<=(const sl_char16* utf16, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<=(const sl_char32* utf32, const String16& second);
		
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
		friend sl_bool operator>(const sl_char8* utf8, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>(const sl_char16* utf16, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>(const sl_char32* utf32, const String16& second);
		
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
		friend sl_bool operator<(const sl_char8* utf8, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<(const sl_char16* utf16, const String16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<(const sl_char32* utf32, const String16& second);

	public:

/**
* @return duplicated string.
*/
		String16 duplicate() const;

/**
* @return memory containing string content.
*/
		Memory toMemory() const;

/**
* @return memory containing string content, without creating new heap memory.
*/
		Memory toStaticMemory() const;
		
/**
* Fills Utf8 characters to the provided buffer
*/
		sl_size getUtf8(sl_char8* utf8, sl_size len) const;

/**
* Fills Utf8 characters to the provided buffer
*/
		sl_bool getUtf8(StringData& output) const;

/**
* Converts to Utf8 and Returns a Memory containing the Utf16 characters and null at last
*/
		Memory toUtf8() const;

/**
* @return a newly constructed string object with its value initialized to a copy of a substring of this object.
*/
		String16 substring(sl_reg start, sl_reg end = -1) const;
		
/**
* @return a string containing a specified number of characters from the left side of a string.
*/
		String16 left(sl_reg len) const;
		
/**
* @return a string containing a specified number of characters from the right side of a string.
*/
		String16 right(sl_reg len) const;
		
/**
* @return a string that contains a specified number of characters starting from a specified position in a string.
*/
		String16 mid(sl_reg start, sl_reg len) const;

/**
* @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
*/
		sl_reg indexOf(sl_char16 ch, sl_reg start = 0) const;
		
/**
* @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
*/		
		sl_reg indexOf(const String16& str, sl_reg start = 0) const;
		
/**
* @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
*/
		sl_reg indexOf(const sl_char16* str, sl_reg start = 0) const;
		
/**
* @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
*/
		sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const;
		
/**
* @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
*/
		sl_reg lastIndexOf(const String16& str, sl_reg start = -1) const;
		
/**
* @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
*/
		sl_reg lastIndexOf(const sl_char16* str, sl_reg start = -1) const;
		
/**
* Determines whether a string begins with the character, returning true or false as appropriate.
*/
		sl_bool startsWith(sl_char16 ch) const;
		
/**
* Determines whether a string begins with the characters of another string, returning true or false as appropriate.
*/
		sl_bool startsWith(const String16& str) const;
		
/**
* Determines whether a string begins with the characters of another string, returning true or false as appropriate.
*/
		sl_bool startsWith(const sl_char16* str) const;
		
/**
* Determines whether a string ends with the character, returning true or false as appropriate.
*/
		sl_bool endsWith(sl_char16 ch) const;
		
/**
* Determines whether a string ends with the characters of another string, returning true or false as appropriate.
*/	
		sl_bool endsWith(const String16& str) const;

/**
* Determines whether a string ends with the characters of another string, returning true or false as appropriate.
*/
		sl_bool endsWith(const sl_char16* str) const;
		
/**
* @return a value indicating whether a specified substring occurs within this string.
*/
		sl_bool contains(sl_char16 ch) const;

/**
* @return a value indicating whether a specified substring occurs within this string.
*/
		sl_bool contains(const String16& str) const;
		
/**
* @return a value indicating whether a specified substring occurs within this string.
*/
		sl_bool contains(const sl_char16* str) const;
		
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
		static String16 toUpper(const sl_char16* sz, sl_reg len = -1);
		
/**
* @return a copy of this string converted to lowercase.
*/
		static String16 toLower(const sl_char16* sz, sl_reg len = -1);
		
/**
* @return a copy of this string converted to uppercase.
*/
		String16 toUpper() const;
		
/**
* @return a copy of this string converted to lowercase.
*/
		String16 toLower() const;

/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/
		String16 replaceAll(const String16& pattern, const String16& replacement) const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/
		String16 replaceAll(const String16& pattern, const sl_char16* replacement) const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/	
		String16 replaceAll(const sl_char16* pattern, const String16& replacement) const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/
		String16 replaceAll(const sl_char16* pattern, const sl_char16* replacement) const;
		
/**
* Removes whitespace from both ends of a string.
*/
		String16 trim() const;
		
/**
* Removes whitespace from the left end of a string.
*/	
		String16 trimLeft() const;
		
/**
* Removes whitespace from the right end of a string.
*/
		String16 trimRight() const;
		
/**
* Splits a String object into an array of strings by separating the string into substrings.
*/
		List<String16> split(const String16& pattern) const;
		
/**
* Splits a String object into an array of strings by separating the string into substrings.
*/
		List<String16> split(const sl_char16* pattern) const;
		
	public:
/**
* Parses string argument and outputs a 32 bit integer of the specified radix
*
* @param radix This would be used to convert String into integer.
* @param _out Pointer to a store where the resulting 32 bit integer is stored.
* @param sz This is a string representation of decimal
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
* @param sz This is a string representation of decimal.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
* @param sz This is a string representation of decimal.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
* @param sz This is a string representation of decimal.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
**/
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
* @param _out Pointer to a store where the resulting floating point is stored.
* @param sz This is a string representation.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseFloat(float* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
* @param sz The string to be parsed.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseDouble(double* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
* @param sz The string to be parsed.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseBoolean(sl_bool* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
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
* @param sz The string to be parsed.
* @param posBegin The zero-based index of the first character in this string object.
* @param len The number of characters to parse from this string object.
*/
		static sl_reg parseHexString(void* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
		
/**
* Parses the string as a byte array.
*
* @param _out Pointer to a store.
*/
		sl_bool parseHexString(void* _out) const;

/**
* @return the string representation of 32 bit integer argument.
*
* @param value The integer to be parsed.
* @param radix This would be used to convert integer into String. radix:2 ~ 64.
* @param minWidth Minimum width of the result.
* @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
*/
		static String16 fromInt32(sl_int32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
/**
* @return the string representation of the unsigned 32bit integer argument.
*
* @param value The integer to be parsed.
* @param radix This would be used to convert integer into String. radix:2 ~ 64.
* @param minWidth Minimum width of the result.
* @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
*/
		static String16 fromUint32(sl_uint32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
/**
* @return the string representation of the 64bit integer argument.
*
* @param value The integer to be parsed.
* @param radix This would be used to convert integer into String. radix:2 ~ 64.
* @param minWidth Minimum width of the result.
* @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
*/
		static String16 fromInt64(sl_int64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
/**
* @return the string representation of the unsigned 64bit integer argument.
*
* @param value The integer to be parsed.
* @param radix This would be used to convert integer into String. radix:2 ~ 64.
* @param minWidth Minimum width of the result.
* @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
*/
		static String16 fromUint64(sl_uint64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
/**
* @return the string representation of the integer argument.
*
* @param value The integer to be parsed.
* @param radix This would be used to convert integer into String. radix:2 ~ 64.
* @param minWidth Minimum width of the result.
* @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
*/
		static String16 fromInt(sl_reg value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
/**
* @return the string representation of the unsigned integer argument.
*
* @param value The integer to be parsed.
* @param radix This would be used to convert integer into String. radix:2 ~ 64.
* @param minWidth Minimum width of the result.
* @param flagUpperCase if flagUpperCase is true, converts string to an uppercase string. flagUpperCase only works if radix <=36 (0~9, a~z)
*/
		static String16 fromSize(sl_size value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
		
/**
* @return the string representation of the float argument.
*
* @param value The float value to be parsed.
* @param precision The number of characters in decimal. Negative values is ignored and this parameter has not effect.
* @param flagZeroPadding If flagZeroPadding is true, zeros are used to pad the field instead of space characters.
* @param minWidthIntegral The minimum number of characters in integral field.
*/
		static String16 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
		
/**
* @return the string representation of the double argument.
*
* @param value The double value to be parsed.
* @param precision The number of characters in decimal. Negative values is ignored and this parameter has not effect.
* @param flagZeroPadding If flagZeroPadding is true, zeros are used to pad the field instead of space characters.
* @param minWidthIntegral The minimum number of characters in integral field.
*/
		static String16 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
		
/**
* @return the string representation of the memory address.
*
* @param pointer The memory address to be parsed.
*/
		static String16 fromPointerValue(const void* pointer);
		
/**
* @return the string representation of the boolean argument.
*
* @param value The boolean value to be parsed.
*/
		static String16 fromBoolean(sl_bool value);
		
/**
* @return the converted hex string from the buffer.
*
* @param data The buffer to be converted.
* @param size Size of the buffer.
*/
		static String16 makeHexString(const void* data, sl_size size);
		
/**
* @return the converted hex string from the buffer.
*
* @param mem The buffer to be converted.
*/
		static String16 makeHexString(const Memory& mem);
		
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
		static String16 formatBy(const sl_char16* szFormat, const Variant* params, sl_size nParams);
		
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
		static String16 formatBy(const String16& strFormat, const Variant* params, sl_size nParams);
		
		static String16 format(const sl_char16* szFormat);
		
		static String16 format(const String16& strFormat);
		
		template <class... ARGS>
		static String16 format(const sl_char16* szFormat, ARGS&&... args);
		
		template <class... ARGS>
		static String16 format(const String16& strFormat, ARGS&&... args);
		
/**
* Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String16::format(*this, params, nParams)
*
* @param params Arbitrary list of arguments.
* @param nParams Number of parameters.
*/
		String16 argBy(const Variant* params, sl_size nParams) const;
		
/**
* Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String16::format(*this, args)
*
* @param args Arbitrary list of arguments.
*/
		template <class... ARGS>
		String16 arg(ARGS&&... args) const;
		
	private:	
		static StringContainer16* _alloc(sl_size length);
		
		void _replaceContainer(StringContainer16* container);

    
		static StringContainer16* _create(sl_char16 ch, sl_size nRepeatCount);
		
		static StringContainer16* _create(const sl_char8* strUtf8, sl_reg length);
		
		static StringContainer16* _create(const sl_char16* strUtf16, sl_reg length);
		
		static StringContainer16* _create(const sl_char32* strUtf32, sl_reg length);

    
		static StringContainer16* _merge16(const sl_char16* s1, sl_reg len1, const sl_char16* s2, sl_reg len2);
		
		static StringContainer16* _merge8(const sl_char16* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
		
		static StringContainer16* _merge8(const sl_char8* s1, sl_reg len1, const sl_char16* s2, sl_reg len2);
		
		static StringContainer16* _merge32(const sl_char16* s1, sl_reg len1, const sl_char32* s2, sl_reg len2);
		
		static StringContainer16* _merge32(const sl_char32* s1, sl_reg len1, const sl_char16* s2, sl_reg len2);
    
		
		static sl_bool _equals16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
		
		static sl_bool _equals8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
		
		static sl_bool _equals32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
		
		static sl_int32 _compare8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
		
		static sl_int32 _compare16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
		
		static sl_int32 _compare32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
		
	public:
		friend class Atomic<String16>;
		
	};


	template <>
	class SLIB_EXPORT Atomic<String16>
	{
	private:
		StringContainer16* volatile m_container;
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
		Atomic(AtomicString16&& src);
		
/**
* Move constructor of String.
*/	
		Atomic(String16&& src);
		
/**
* Copy constructor
*/
		Atomic(const String16& src);

/**
* Copy constructor
*/	
		Atomic(const AtomicString16& src);

/**
* Copy constructor
*/	
		Atomic(const String& src);

/**
* Copy constructor
*/	
		Atomic(const AtomicString& src);
		
		~Atomic();
		
	public:
/**
* Fill constructor
*
* Fill the string with nRepeatCount consecutive copies of charactor ch
*/
		Atomic(sl_char16 ch, sl_size nRepeatCount);
		
/**
* From utf-8 null-terminated string buffer
*
* Copies the null-terminated character sequence pointed by strUtf8.
*/
		Atomic(const sl_char8* strUtf8);
		
/**
* From utf-8 string buffer
*
* Copies the first 'length' characters from the array of characters pointed by strUtf8
*/
		Atomic(const sl_char8* strUtf8, sl_reg length);
		
/**
* From utf-16 null-terminated string buffer
*
* Copies the null-terminated character sequence pointed by strUtf16.
*/
		Atomic(const sl_char16* strUtf16);
		
/**
* From utf-16 string buffer
*
* Copies the first 'length' characters from the array of characters pointed by strUtf16
*/
		Atomic(const sl_char16* strUtf16, sl_reg length);
		
/**
* From utf-32 null-terminated string buffer
*
* Copies the null-terminated character sequence pointed by strUtf32.
*/
		Atomic(const sl_char32* strUtf32);
		
/**
* From utf-32 string buffer
*
* Copies the first 'length' characters from the array of characters pointed by strUtf32
*/
		Atomic(const sl_char32* strUtf32, sl_reg length);
		
	public:
/**
* @return null string.
*/
		static const AtomicString16& null();
		
/**
* @return empty string.
*/
		static const AtomicString16& getEmpty();
		
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
* AtomicString16 assignment
*/
		AtomicString16& operator=(sl_null_t);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(AtomicString16&& other);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(String16&& other);

/**
* AtomicString16 assignment
*/
		AtomicString16& operator=(const String16& other);
		
/**
* AtomicString16 assignment
*/
		AtomicString16& operator=(const AtomicString16& other);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(const String& other);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(const AtomicString& _other);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(const sl_char8* utf8);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(const sl_char16* utf16);

/**
* AtomicString16 assignment
*/	
		AtomicString16& operator=(const sl_char32* utf32);
    
	public:
/**
* Concatenate strings
*/
		String16 operator+(const String16& other) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(const AtomicString16& other);

/**
* Concatenate strings
*/
		String16 operator+(const String& other) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(const AtomicString& other);

/**
* Concatenate strings
*/
		String16 operator+(const AtomicString16& other) const;

/**
* Append to strings
*/		
		AtomicString16& operator+=(const String16& other);

/**
* Concatenate strings
*/
		String16 operator+(const AtomicString& other) const;

/**
* Append to strings
*/		
		AtomicString16& operator+=(const String& other);

/**
* Concatenate strings
*/
		String16 operator+(const sl_char8* utf8) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(const sl_char8* utf8);

/**
* Concatenate strings
*/
		String16 operator+(const sl_char16* utf16) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(const sl_char16* utf16);

/**
* Concatenate strings
*/
		String16 operator+(const sl_char32* utf32) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(const sl_char32* utf32);

/**
* Concatenate strings
*/
		friend String16 operator+(const sl_char8* utf8, const AtomicString16& second);

/**
* Concatenate strings
*/	
		friend String16 operator+(const sl_char16* utf16, const AtomicString16& second);

/**
* Concatenate strings
*/	
		friend String16 operator+(const sl_char32* utf32, const AtomicString16& second);
		
	public:
/**
* Concatenate strings
*/
		String16 operator+(sl_int32 number) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(sl_int32 number);

/**
* Concatenate strings
*/	
		friend String16 operator+(sl_int32 number, const AtomicString16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_uint32 number) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(sl_uint32 number);

/**
* Concatenate strings
*/	
		friend String16 operator+(sl_uint32 number, const AtomicString16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_int64 number) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(sl_int64 number);

/**
* Concatenate strings
*/	
		friend String16 operator+(sl_int64 number, const AtomicString16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_uint64 number) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(sl_uint64 number);

/**
* Concatenate strings
*/	
		friend String16 operator+(sl_uint64 number, const AtomicString16& other);

/**
* Concatenate strings
*/	
		String16 operator+(float number) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(float number);

/**
* Concatenate strings
*/	
		friend String16 operator+(float number, const AtomicString16& other);

/**
* Concatenate strings
*/	
		String16 operator+(double number) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(double number);

/**
* Concatenate strings
*/	
		friend String16 operator+(double number, const AtomicString16& other);

/**
* Concatenate strings
*/	
		String16 operator+(sl_bool value) const;
		
/**
* Append to strings
*/	
		AtomicString16& operator+=(sl_bool value);

/**
* Concatenate strings
*/	
		friend String16 operator+(sl_bool value, const AtomicString16& other);
		
	public:
/**
* @returns true if this string is equal to the specified string.
*/
		sl_bool equals(const String16& other) const;

/**
* @returns true if this string is equal to the specified string.
*/	
		sl_bool equals(const String& other) const;

/**
* @returns true if this string is equal to the specified string.
*/	
		sl_bool equals(const AtomicString16& other) const;

/**
* @returns true if this string is equal to the specified string.
*/	
		sl_bool equals(const AtomicString& other) const;

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
		sl_int32 compare(const String16& other) const;
		
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
		sl_int32 compare(const AtomicString16& other) const;
		
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
		sl_int32 compare(const String16& other, sl_size len) const;

/**
* @return true if this string is equal to the specified string.
*/	
		sl_bool equalsIgnoreCase(const String16& other) const;
		
/**
* @return true if this string is equal to the specified string.
*/
		sl_int32 compareIgnoreCase(const String16& other) const;
		
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
		friend sl_bool operator==(const sl_char8* utf8, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator==(const sl_char16* utf16, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator==(const sl_char32* utf32, const AtomicString16& second);
		
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
		friend sl_bool operator!=(const sl_char8* utf8, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator!=(const sl_char16* utf16, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator!=(const sl_char32* utf32, const AtomicString16& second);
		
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
		friend sl_bool operator>=(const sl_char8* utf8, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>=(const sl_char16* utf16, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>=(const sl_char32* utf32, const AtomicString16& second);
		
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
		friend sl_bool operator<=(const sl_char8* utf8, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<=(const sl_char16* utf16, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<=(const sl_char32* utf32, const AtomicString16& second);
		
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
		friend sl_bool operator>(const sl_char8* utf8, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>(const sl_char16* utf16, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator>(const sl_char32* utf32, const AtomicString16& second);
		
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
		friend sl_bool operator<(const sl_char8* utf8, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<(const sl_char16* utf16, const AtomicString16& second);

/**
* Comparison Operator
*/	
		friend sl_bool operator<(const sl_char32* utf32, const AtomicString16& second);
		
	public:
/**
* @return duplicated string.
*/
		String16 duplicate() const;
		
/**
* @return memory containing string content.
*/
		Memory toMemory() const;
		
/**
* Fills Utf8 characters to the provided buffer
*/
		sl_size getUtf8(sl_char8* utf8, sl_size len) const;
		
/**
* Fills Utf8 characters to the provided buffer
*/
		sl_bool getUtf8(StringData& output) const;
		
/**
* Converts to Utf8 and Returns a Memory containing the Utf16 characters and null at last
*/
		Memory toUtf8() const;
		
/**
* @return a newly constructed string object with its value initialized to a copy of a substring of this object.
*/
		String16 substring(sl_reg start, sl_reg end = -1) const;
		
/**
* @return a string containing a specified number of characters from the left side of a string.
*/
		String16 left(sl_reg len) const;
		
/**
* @return a string containing a specified number of characters from the right side of a string.
*/
		String16 right(sl_reg len) const;
		
/**
* @return a string that contains a specified number of characters starting from a specified position in a string.
*/
		String16 mid(sl_reg start, sl_reg len) const;
		
/**
* @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
*/
		sl_reg indexOf(sl_char16 ch, sl_reg start = 0) const;
		
/**
* @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
*/	
		sl_reg indexOf(const String16& str, sl_reg start = 0) const;
		
/**
* @return the index within the calling String object of the first occurrence of the specified value, starting the search at start. Returns -1 if the value is not found.
*/
		sl_reg indexOf(const sl_char16* str, sl_reg start = 0) const;
		
/**
* @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
*/
		sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const;
		
/**
* @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
*/
		sl_reg lastIndexOf(const String16& str, sl_reg start = -1) const;
		
/**
* @return the index within the calling String object of the last occurrence of the specified value, searching backwards from fromIndex. Returns -1 if the value is not found.
*/
		sl_reg lastIndexOf(const sl_char16* str, sl_reg start = -1) const;
		
/**
* Determines whether a string begins with the character, returning true or false as appropriate.
*/
		sl_bool startsWith(sl_char16 ch) const;
		
/**
* Determines whether a string begins with the characters of another string, returning true or false as appropriate.
*/
		sl_bool startsWith(const String16& str) const;
		
/**
* Determines whether a string begins with the characters of another string, returning true or false as appropriate.
*/
		sl_bool startsWith(const sl_char16* str) const;
		
/**
* Determines whether a string ends with the character, returning true or false as appropriate.
*/
		sl_bool endsWith(sl_char16 ch) const;
		
/**
* Determines whether a string ends with the characters of another string, returning true or false as appropriate.
*/	
		sl_bool endsWith(const String16& str) const;
		
/**
* Determines whether a string ends with the characters of another string, returning true or false as appropriate.
*/
		sl_bool endsWith(const sl_char16* str) const;

/**
* @return a value indicating whether a specified substring occurs within this string.
*/
		sl_bool contains(sl_char16 ch) const;

/**
* @return a value indicating whether a specified substring occurs within this string.
*/
		sl_bool contains(const String16& str) const;
		
/**
* @return a value indicating whether a specified substring occurs within this string.
*/
		sl_bool contains(const sl_char16* str) const;

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
		String16 toUpper() const;
		
/**
* @return a copy of this string converted to lowercase.
*/
		String16 toLower() const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/
		String16 replaceAll(const String16& pattern, const String16& replacement) const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/
		String16 replaceAll(const String16& pattern, const sl_char16* replacement) const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/	
		String16 replaceAll(const sl_char16* pattern, const String16& replacement) const;
		
/**
* Replaces each substring of this string that matches the given pattern with the given replacement.
*/
		String16 replaceAll(const sl_char16* pattern, const sl_char16* replacement) const;
		
/**
* Removes whitespace from both ends of a string.
*/
		String16 trim() const;
		
/**
* Removes whitespace from the left end of a string.
*/	
		String16 trimLeft() const;
		
/**
* Removes whitespace from the right end of a string.
*/
		String16 trimRight() const;
		
/**
* Splits a String object into an array of strings by separating the string into substrings.
*/
		List<String16> split(const String16& pattern) const;
		
/**
* Splits a String object into an array of strings by separating the string into substrings.
*/
		List<String16> split(const sl_char16* pattern) const;
		
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
**/
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
* Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String16::format(*this, params, nParams)
*
* @param params Arbitrary list of arguments.
* @param nParams Number of parameters.
*/
		String16 argBy(const Variant* params, sl_size nParams) const;
		
/**
* Formats the current string which contains conversion specifications with arbitrary list of arguments. It is same as String16::format(*this, args)
*
* @param args Arbitrary list of arguments.
*/
		template <class... ARGS>
		String16 arg(ARGS&&... args) const;
		
	private:
		StringContainer16* _retainContainer() const;
		
		void _replaceContainer(StringContainer16* other);
		
		friend class String16;
	};
/// @}


	template <>
	class Compare<String16>
	{
		public:
			int operator()(const String16& a, const String16& b) const;
	};

	template <>
	class Compare<AtomicString16>
	{
		public:
			int operator()(const AtomicString16& a, const AtomicString16& b) const;
	};

	template <>
	class Equals<String16>
	{
		public:
			sl_bool operator()(const String16& a, const String16& b) const;
	};

	template <>
	class Equals<AtomicString16>
	{
		public:
			sl_bool operator()(const AtomicString16& a, const AtomicString16& b) const;
	};

	template <>
	class Hash<String16>
	{
		public:
			sl_uint32 operator()(const String16& a) const;
	};

	template <>
	class Hash<AtomicString16>
	{
		public:
			sl_uint32 operator()(const AtomicString16& a) const;
	};

	class CompareIgnoreCaseString16
	{
		public:
			int operator()(const String16& a, const String16& b) const;
	};

	class EqualsIgnoreCaseString16
	{
		public:
			sl_bool operator()(const String16& a, const String16& b) const;
	};

	class HashIgnoreCaseString16
	{
		public:
			sl_uint32 operator()(const String16& v) const;

	};

}

#if !defined(SLIB_USE_UNICODE32)
#define SLIB_STATIC_STRING16(name, str) \
		static wchar_t _static_string_buf_##name[] = L##str; \
		static slib::_StringContainer16 _static_string_container_##name = {(sl_char16*)_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
		static slib::_StringContainer16* _static_string_##name = &_static_string_container_##name; \
		static const slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_##name));
#define SLIB_STATIC_ATOMIC_STRING16(name, str) \
		static wchar_t _static_string_buf_##name[] = L##str; \
		static slib::_StringContainer16 _static_string_container_##name = {(sl_char16*)_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
		static slib::_String16_Const _static_string_##name = {reinterpret_cast<slib::StringContainer16*>(&_static_string_container_##name), 0}; \
		static const slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_##name));
#endif

#define SLIB_STATIC_STRING16_BY_ARRAY(name, ...) \
		static sl_char16 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
		static slib::_StringContainer16 _static_string_container_##name = {_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
		static slib::_StringContainer16* _static_string_##name = &_static_string_container_##name; \
		static const slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING16_BY_ARRAY(name, ...) \
		static sl_char16 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
		static slib::_StringContainer16 _static_string_container_##name = {_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
		static slib::_String16_Const _static_string_##name = {reinterpret_cast<slib::StringContainer16*>(&_static_string_container_##name), 0}; \
		static const slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_##name));

#define SLIB_STATIC_STRING16_NULL(name) \
		static slib::_String16_Const _static_string_null_##name = {sl_null, 0}; \
		static const slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING16_NULL(name) \
		static slib::_String16_Const _static_string_null_##name = {sl_null, 0}; \
		static const slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_null_##name));

#endif
