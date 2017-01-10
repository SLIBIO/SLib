#ifndef CHECKHEADER_SLIB_CORE_STRING
#define CHECKHEADER_SLIB_CORE_STRING

#include "definition.h"

#include "charset.h"
#include "memory.h"
#include "list.h"
#include "map.h"
#include "queue.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT StringContainer8
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

class SLIB_EXPORT StringContainer16
{
public:
	union {
		sl_char16* sz;
		wchar_t* szDebug;
	};
	sl_size len;
	sl_uint32 hash;
	sl_reg ref;
	
public:
	sl_reg increaseReference();
	
	sl_reg decreaseReference();
	
};

struct _String8_Const
{
	StringContainer8* container;
	sl_int32 lock;
};

extern const _String8_Const _String8_Null;
extern const _String8_Const _String8_Empty;

struct _String16_Const
{
	StringContainer16* container;
	sl_int32 lock;
};

extern const _String16_Const _String16_Null;
extern const _String16_Const _String16_Empty;

extern const char* _StringConv_radixPatternUpper;
extern const char* _StringConv_radixPatternLower;
extern const sl_uint8* _StringConv_radixInversePatternBig;
extern const sl_uint8* _StringConv_radixInversePatternSmall;

class String8;
typedef Atomic<String8> AtomicString8;
class String16;
typedef Atomic<String16> AtomicString16;
class StringData;
class Variant;

/** auto-referencing object **/
class SLIB_EXPORT String8
{
private:
	StringContainer8* m_container;
	
private:
	SLIB_CONSTEXPR String8(StringContainer8* container) : m_container(container) {}
	
public:
	SLIB_CONSTEXPR String8() : m_container(sl_null) {}
	
	SLIB_CONSTEXPR String8(sl_null_t) : m_container(sl_null) {}
	
	String8(String8&& src);
	
	String8(AtomicString8&& _src);
	
	String8(const String8& src);
	
	String8(const AtomicString8& src);
	
	String8(const String16& src);
	
	String8(const AtomicString16& src);

	~String8();
	
public:
	// From a single charactor
	String8(sl_char8 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	String8(const sl_char8* strUtf8);
	
	// From an utf-8 string
	String8(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	String8(const sl_char16* strUtf16);
	
	// From an utf-16 string
	String8(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	String8(const sl_char32* strUtf32);
	
	// From an utf-32 string
	String8(const sl_char32* strUtf32, sl_reg length);

public:
	// create a string of 'len' charactors
	static String8 allocate(sl_size len);
	
	static String8 fromStatic(const sl_char8* sz8, sl_reg len = -1);

	static String8 fromUtf8(const void* utf8, sl_reg len = -1);
	
	static String8 fromUtf8(const Memory& mem);
	
	static String8 fromUtf16(const sl_char16* utf16, sl_reg len = -1);
	
	static String8 fromUtf32(const sl_char32* utf32, sl_reg len = -1);

	static String8 fromUtf16BE(const void* utf16, sl_reg len = -1);

	static String8 fromUtf16BE(const Memory& mem);
	
	static String8 fromUtf16LE(const void* utf16, sl_reg len = -1);

	static String8 fromUtf16LE(const Memory& mem);

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String8 fromUtf(const void* buf, sl_size len);

	static String8 fromUtf(const Memory& mem);

public:
	// Returns a null string
	static const String8& null();
	
	// Returns an empty string
	static const String8& getEmpty();
	
	const String8& getNotNull() const;
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;

	void setNull();
	
	void setEmpty();
	
public:
	sl_char8* getData() const;
	
	sl_size getLength() const;
	
	// don't use for null or empty string
	void setLength(sl_size len);
	
	sl_uint32 getHashCode() const;
	
	// don't use for null or empty string
	void setHashCode(sl_uint32 hash);
	
	sl_uint32 getHashCodeIgnoreCase() const;

	sl_char8 getAt(sl_reg index) const;
	
	sl_bool setAt(sl_reg index, sl_char8 ch);
	
public:
	String8& operator=(sl_null_t);
	
	String8& operator=(String8&& other);
	
	String8& operator=(AtomicString8&& _other);

	String8& operator=(const String8& other);
	
	String8& operator=(const AtomicString8& other);
	
	String8& operator=(const String16& other);
	
	String8& operator=(const AtomicString16& other);
	
	String8& operator=(const sl_char8* utf8);
	
	String8& operator=(const sl_char16* utf16);
	
	String8& operator=(const sl_char32* utf32);

public:
	String8 operator+(const String8& other) const;
	
	String8& operator+=(const String8& other);
	
	String8 operator+(const String16& other) const;
	
	String8& operator+=(const String16& other);
	
	String8 operator+(const AtomicString8& _other) const;
	
	String8& operator+=(const AtomicString8& other);
	
	String8 operator+(const AtomicString16& other) const;
	
	String8& operator+=(const AtomicString16& other);
	
	String8 operator+(const sl_char8* utf8) const;
	
	String8& operator+=(const sl_char8* utf8);
	
	String8 operator+(const sl_char16* utf16) const;
	
	String8& operator+=(const sl_char16* utf16);

	String8 operator+(const sl_char32* utf32) const;
	
	String8& operator+=(const sl_char32* utf32);

	friend String8 operator+(const sl_char8* utf8, const String8& second);
	
	friend String8 operator+(const sl_char16* utf16, const String8& second);
	
	friend String8 operator+(const sl_char32* utf32, const String8& second);

public:
	String8 operator+(sl_int32 number) const;
	
	String8& operator+=(sl_int32 number);
	
	friend String8 operator+(sl_int32 number, const String8& other);
	
	String8 operator+(sl_uint32 number) const;
	
	String8& operator+=(sl_uint32 number);

	friend String8 operator+(sl_uint32 number, const String8& other);
	
	String8 operator+(sl_int64 number) const;
	
	String8& operator+=(sl_int64 number);

	friend String8 operator+(sl_int64 number, const String8& other);
	
	String8 operator+(sl_uint64 number) const;
	
	String8& operator+=(sl_uint64 number);
	
	friend String8 operator+(sl_uint64 number, const String8& other);
	
	String8 operator+(float number) const;
	
	String8& operator+=(float number);

	friend String8 operator+(float number, const String8& other);
	
	String8 operator+(double number) const;
	
	String8& operator+=(double number);

	friend String8 operator+(double number, const String8& other);
	
	String8 operator+(sl_bool value) const;
	
	String8& operator+=(sl_bool value);
	
	friend String8 operator+(sl_bool value, const String8& other);

public:
	sl_bool equals(const String8& other) const;
	
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const AtomicString8& other) const;
	
	sl_bool equals(const AtomicString16& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const AtomicString8& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String8& other, sl_size len) const;
	
	
	sl_bool equalsIgnoreCase(const String8& other) const;
	
	sl_int32 compareIgnoreCase(const String8& other) const;

public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString8& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;

	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString8& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;

	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString8& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString8& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const String8& second);

	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString8& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString8& other) const;
	
	sl_bool operator<(const AtomicString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const String8& second);
	
public:
	String8 duplicate() const;

	Memory toMemory() const;
	
	Memory toStaticMemory() const;

	sl_size getUtf16(sl_char16* utf16, sl_size len) const;

	sl_bool getUtf16(StringData& output) const;

	// contains null character at last
	Memory toUtf16() const;

	sl_size getUtf32(sl_char32* utf32, sl_size len) const;

	sl_bool getUtf32(StringData& output) const;

	// contains null character at last
	Memory toUtf32() const;

	String8 substring(sl_reg start, sl_reg end = -1) const;
	
	String8 left(sl_reg len) const;
	
	String8 right(sl_reg len) const;
	
	String8 mid(sl_reg start, sl_reg len) const;

	sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
	
	sl_reg indexOf(const String8& str, sl_reg start = 0) const;
	
	sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String8& str, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String8& str) const;
	
	sl_bool startsWith(const sl_char8* str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String8& str) const;

	sl_bool endsWith(const sl_char8* str) const;
	
	sl_bool contains(sl_char8 ch) const;
	
	sl_bool contains(const String8& str) const;
	
	sl_bool contains(const sl_char8* str) const;

	void makeUpper();
	
	void makeLower();
	
	static String8 toUpper(const sl_char8* sz, sl_reg len = -1);
	
	static String8 toLower(const sl_char8* sz, sl_reg len = -1);
	
	String8 toUpper() const;
	
	String8 toLower() const;
	
	String8 replaceAll(const String8& pattern, const String8& replacement) const;
	
	String8 replaceAll(const String8& pattern, const sl_char8* replacement) const;
	
	String8 replaceAll(const sl_char8* pattern, const String8& replacement) const;
	
	String8 replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
	
	String8 trim() const;
	
	String8 trimLeft() const;
	
	String8 trimRight() const;
	
	List<String8> split(const String8& pattern) const;
	
	List<String8> split(const sl_char8* pattern) const;
	
	String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);
	
	static String8 parseBackslashEscapes(const sl_char8* input, sl_size len, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	
	String8 parseBackslashEscapes(sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) const;
	
	static sl_size countLineNumber(const sl_char8* input, sl_size len, sl_size* columnLast = sl_null);
	
	sl_size countLineNumber(sl_size pos, sl_size* column = sl_null) const;
	
public:
	static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const sl_char8*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;

	sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;

	static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;

	sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;

	static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;

	sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;
	
	static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;

	sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;

	sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;

	sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;

	sl_bool parseSize(sl_int32 radix, sl_size* _out) const;

	sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;

	static sl_reg parseFloat(float* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseFloat(float* _out) const;

	float parseFloat(float def = 0) const;
	
	static sl_reg parseDouble(double* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseDouble(double* _out) const;

	double parseDouble(double def = 0) const;

	static sl_reg parseBoolean(sl_bool* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseBoolean(sl_bool* _out) const;
	
	sl_bool parseBoolean(sl_bool def = sl_false) const;

	static sl_reg parseHexString(void* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseHexString(void* _out) const;

	
	// radix: 2 ~ 64,   flagUpperCase only works if radix <= 36 (0~9a~z)
	static String8 fromInt32(sl_int32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromUint32(sl_uint32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromInt64(sl_int64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromUint64(sl_uint64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromInt(sl_reg value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromSize(sl_size value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
	
	static String8 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
	
	static String8 fromPointerValue(const void* pointer);
	
	static String8 fromBoolean(sl_bool value);
	
	static String8 makeHexString(const void* data, sl_size size);
	
	static String8 makeHexString(const Memory& mem);
	
	
	static String8 formatBy(const sl_char8* szFormat, const Variant* params, sl_size nParams);
	
	static String8 formatBy(const String8& strFormat, const Variant* params, sl_size nParams);
	
	static String8 format(const sl_char8* szFormat);
	
	static String8 format(const String8& strFormat);
	
	template <class... ARGS>
	static String8 format(const sl_char8* szFormat, ARGS&&... args);
	
	template <class... ARGS>
	static String8 format(const String8& strFormat, ARGS&&... args);
	
	String8 argBy(const Variant* params, sl_size nParams) const;
	
	template <class... ARGS>
	String8 arg(ARGS&&... args) const;
	
private:
	// Allocates memory required for a string for the specified length
	static StringContainer8* _alloc(sl_size length);
	
	void _replaceContainer(StringContainer8* container);
	
	
	static StringContainer8* _create(sl_char8 ch, sl_size nRepeatCount);
	
	static StringContainer8* _create(const sl_char8* strUtf8, sl_reg length);
	
	static StringContainer8* _create(const sl_char16* strUtf16, sl_reg length);
	
	static StringContainer8* _create(const sl_char32* strUtf32, sl_reg length);
	
	
	static StringContainer8* _merge8(const sl_char8* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
	
	static StringContainer8* _merge16(const sl_char8* s1, sl_reg len1, const sl_char16* s2, sl_reg len2);
	
	static StringContainer8* _merge16(const sl_char16* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
	
	static StringContainer8* _merge32(const sl_char8* s1, sl_reg len1, const sl_char32* s2, sl_reg len2);
	
	static StringContainer8* _merge32(const sl_char32* s1, sl_reg len1, const sl_char8* s2, sl_reg len2);
	
	
	// Check two unicode string is Equal
	static sl_bool _equals8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
	
	static sl_bool _equals16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
	
	static sl_bool _equals32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);

	
	// Compare two unicode string
	static sl_int32 _compare8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
	
	static sl_int32 _compare16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
	
	static sl_int32 _compare32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
	
public:
	friend class Atomic<String8>;
	
};

/** auto-referencing object **/
class SLIB_EXPORT String16
{
private:
	StringContainer16* m_container;
	
private:
	SLIB_CONSTEXPR String16(StringContainer16* container) : m_container(container) {}

public:
	SLIB_CONSTEXPR String16() : m_container(sl_null) {}
	
	SLIB_CONSTEXPR String16(sl_null_t) : m_container(sl_null) {}
	
	String16(String16&& src);
	
	String16(AtomicString16&& _src);
	
	String16(const String16& src);
	
	String16(const AtomicString16& src);

	String16(const String8& src);
	
	String16(const AtomicString8& src);

	~String16();
	
public:
	// From a single charactor
	String16(sl_char16 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	String16(const sl_char8* strUtf8);
	
	String16(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	String16(const sl_char16* strUtf16);
	
	String16(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	String16(const sl_char32* strUtf32);
	
	String16(const sl_char32* strUtf32, sl_reg length);

public:
	// create a string of 'len' charactors
	static String16 allocate(sl_size len);
	
	static String16 fromStatic(const sl_char16* sz16, sl_reg len = -1);

	static String16 fromUtf8(const void* utf8, sl_reg len = -1);

	static String16 fromUtf8(const Memory& mem);

	static String16 fromUtf16(const sl_char16* utf16, sl_reg len = -1);

	static String16 fromUtf32(const sl_char32* utf32, sl_reg len = -1);

	static String16 fromUtf16BE(const void* utf16, sl_reg len = -1);

	static String16 fromUtf16BE(const Memory& mem);

	static String16 fromUtf16LE(const void* utf16, sl_reg len = -1);

	static String16 fromUtf16LE(const Memory& mem);

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String16 fromUtf(const void* buf, sl_size len);

	static String16 fromUtf(const Memory& mem);
	
public:
	// Returns a null string
	static const String16& null();
	
	// Returns an empty string
	static const String16& getEmpty();
	
	const String16& getNotNull() const;
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	void setNull();
	
	void setEmpty();

public:
	sl_char16* getData() const;
	
	sl_size getLength() const;

	// don't use for null or empty string
	void setLength(sl_size len);
	
	sl_uint32 getHashCode() const;
	
	void setHashCode(sl_uint32 hash);
	
	sl_uint32 getHashCodeIgnoreCase() const;
	
	sl_char16 getAt(sl_reg index) const;
	
	sl_bool setAt(sl_reg index, sl_char16 ch);
	
public:
	String16& operator=(sl_null_t);
	
	String16& operator=(String16&& other);
	
	String16& operator=(AtomicString16&& _other);

	String16& operator=(const String16& other);
	
	String16& operator=(const AtomicString16& other);
	
	String16& operator=(const String8& other);
	
	String16& operator=(const AtomicString8& other);
	
	String16& operator=(const sl_char8* utf8);
	
	String16& operator=(const sl_char16* utf16);
	
	String16& operator=(const sl_char32* utf32);
	
public:
	String16 operator+(const String16& other) const;
	
	String16& operator+=(const String16& other);

	String16 operator+(const String8& other) const;
	
	String16& operator+=(const String8& other);

	String16 operator+(const AtomicString16& _other) const;
	
	String16& operator+=(const AtomicString16& other);

	String16 operator+(const AtomicString8& other) const;
	
	String16& operator+=(const AtomicString8& other);

	String16 operator+(const sl_char8* utf8) const;
	
	String16& operator+=(const sl_char8* utf8);

	String16 operator+(const sl_char16* utf16) const;
	
	String16& operator+=(const sl_char16* utf16);

	String16 operator+(const sl_char32* utf32) const;
	
	String16& operator+=(const sl_char32* utf32);

	friend String16 operator+(const sl_char8* utf8, const String16& second);
	
	friend String16 operator+(const sl_char16* utf16, const String16& second);
	
	friend String16 operator+(const sl_char32* utf32, const String16& second);

public:
	String16 operator+(sl_int32 number) const;
	
	String16& operator+=(sl_int32 number);

	friend String16 operator+(sl_int32 number, const String16& other);
	
	String16 operator+(sl_uint32 number) const;
	
	String16& operator+=(sl_uint32 number);

	friend String16 operator+(sl_uint32 number, const String16& other);
	
	String16 operator+(sl_int64 number) const;
	
	String16& operator+=(sl_int64 number);

	friend String16 operator+(sl_int64 number, const String16& other);
	
	String16 operator+(sl_uint64 number) const;
	
	String16& operator+=(sl_uint64 number);

	friend String16 operator+(sl_uint64 number, const String16& other);
	
	String16 operator+(float number) const;
	
	String16& operator+=(float number);
	
	friend String16 operator+(float number, const String16& other);
	
	String16 operator+(double number) const;
	
	String16& operator+=(double number);

	friend String16 operator+(double number, const String16& other);
	
	String16 operator+(sl_bool value) const;
	
	String16& operator+=(sl_bool value);
	
	friend String16 operator+(sl_bool value, const String16& other);
	
public:
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const String8& other) const;
	
	sl_bool equals(const AtomicString16& other) const;
	
	sl_bool equals(const AtomicString8& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const AtomicString8& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String16& other, sl_size len) const;

	
	sl_bool equalsIgnoreCase(const String16& other) const;
	
	sl_int32 compareIgnoreCase(const String16& other) const;

public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString8& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString8& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString8& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString8& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString8& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString8& other) const;
	
	sl_bool operator<(const AtomicString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const String16& second);

public:
	String16 duplicate() const;

	Memory toMemory() const;

	Memory toStaticMemory() const;
	
	sl_size getUtf8(sl_char8* utf8, sl_size len) const;

	sl_bool getUtf8(StringData& output) const;

	// contains null character at last
	Memory toUtf8() const;

	String16 substring(sl_reg start, sl_reg end = -1) const;
	
	String16 left(sl_reg len) const;
	
	String16 right(sl_reg len) const;
	
	String16 mid(sl_reg start, sl_reg len) const;

	sl_reg indexOf(sl_char16 ch, sl_reg start = 0) const;
	
	sl_reg indexOf(const String16& str, sl_reg start = 0) const;
	
	sl_reg indexOf(const sl_char16* str, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String16& str, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const sl_char16* str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char16 ch) const;
	
	sl_bool startsWith(const String16& str) const;
	
	sl_bool startsWith(const sl_char16* str) const;
	
	sl_bool endsWith(sl_char16 ch) const;
	
	sl_bool endsWith(const String16& str) const;

	sl_bool endsWith(const sl_char16* str) const;
	
	sl_bool contains(sl_char16 ch) const;

	sl_bool contains(const String16& str) const;
	
	sl_bool contains(const sl_char16* str) const;
	
	void makeUpper();
	
	void makeLower();
	
	static String16 toUpper(const sl_char16* sz, sl_reg len = -1);
	
	static String16 toLower(const sl_char16* sz, sl_reg len = -1);
	
	String16 toUpper() const;
	
	String16 toLower() const;

	String16 replaceAll(const String16& pattern, const String16& replacement) const;
	
	String16 replaceAll(const String16& pattern, const sl_char16* replacement) const;
	
	String16 replaceAll(const sl_char16* pattern, const String16& replacement) const;
	
	String16 replaceAll(const sl_char16* pattern, const sl_char16* replacement) const;
	
	String16 trim() const;
	
	String16 trimLeft() const;
	
	String16 trimRight() const;
	
	List<String16> split(const String16& pattern) const;
	
	List<String16> split(const sl_char16* pattern) const;
	
	String16 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);
	
	static String16 parseBackslashEscapes(const sl_char16* input, sl_size len, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	
	String16 parseBackslashEscapes(sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) const;
	
	static sl_size countLineNumber(const sl_char16* input, sl_size len, sl_size* columnLast = sl_null);
	
	sl_size countLineNumber(sl_size pos, sl_size* column = sl_null) const;
	
public:
	static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;

	sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;
	
	static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;

	sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;

	static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;
	
	sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;

	static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;
	
	sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;

	sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;

	sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;

	sl_bool parseSize(sl_int32 radix, sl_size* _out) const;

	sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;

	static sl_reg parseFloat(float* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseFloat(float* _out) const;

	float parseFloat(float def = 0) const;

	static sl_reg parseDouble(double* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseDouble(double* _out) const;

	double parseDouble(double def = 0) const;
	
	static sl_reg parseBoolean(sl_bool* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseBoolean(sl_bool* _out) const;
	
	sl_bool parseBoolean(sl_bool def = sl_false) const;

	static sl_reg parseHexString(void* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseHexString(void* _out) const;

	
	// radix: 2 ~ 64,   flagUpperCase only works if radix <= 36 (0~9a~z)
	static String16 fromInt32(sl_int32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromUint32(sl_uint32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromInt64(sl_int64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromUint64(sl_uint64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromInt(sl_reg value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromSize(sl_size value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
	
	static String16 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
	
	static String16 fromPointerValue(const void* pointer);
	
	static String16 fromBoolean(sl_bool value);
	
	static String16 makeHexString(const void* data, sl_size size);
	
	static String16 makeHexString(const Memory& mem);
	
	
	static String16 formatBy(const sl_char16* szFormat, const Variant* params, sl_size nParams);
	
	static String16 formatBy(const String16& strFormat, const Variant* params, sl_size nParams);
	
	static String16 format(const sl_char16* szFormat);
	
	static String16 format(const String16& strFormat);
	
	template <class... ARGS>
	static String16 format(const sl_char16* szFormat, ARGS&&... args);
	
	template <class... ARGS>
	static String16 format(const String16& strFormat, ARGS&&... args);
	
	String16 argBy(const Variant* params, sl_size nParams) const;
	
	template <class... ARGS>
	String16 arg(ARGS&&... args) const;
	
private:	
	// Allocates memory required for a string for the specified length
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
    
	
	// Check two unicode string is Equal
	static sl_bool _equals16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
	
	static sl_bool _equals8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
	
	static sl_bool _equals32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
	
	// Compare two unicode string
	static sl_int32 _compare8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
	
	static sl_int32 _compare16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
	
	static sl_int32 _compare32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
	
public:
	friend class Atomic<String16>;
	
};


/** auto-referencing object **/
template <>
class SLIB_EXPORT Atomic<String8>
{
private:
	StringContainer8* volatile m_container;
	SpinLock m_lock;
	
public:
	SLIB_CONSTEXPR Atomic() : m_container(sl_null) {}
	
	SLIB_CONSTEXPR Atomic(sl_null_t) : m_container(sl_null) {}
	
	Atomic(AtomicString8&& src);
	
	Atomic(String8&& src);
	
	Atomic(const String8& src);
	
	Atomic(const AtomicString8& src);
	
	Atomic(const String16& src);
	
	Atomic(const AtomicString16& src);

	~Atomic();

public:
	// From a single charactor
	Atomic(sl_char8 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	Atomic(const sl_char8* strUtf8);
	
	// From an utf-8 string
	Atomic(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	Atomic(const sl_char16* strUtf16);
	
	// From an utf-16 string
	Atomic(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	Atomic(const sl_char32* strUtf32);
	
	// From an utf-32 string
	Atomic(const sl_char32* strUtf32, sl_reg length);
	
public:
	static const AtomicString8& null();
	
	static const AtomicString8& getEmpty();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	void setNull();
	
	void setEmpty();

public:
	sl_size getLength() const;
	
	sl_uint32 getHashCode() const;
	
	sl_uint32 getHashCodeIgnoreCase() const;
	
public:
	AtomicString8& operator=(sl_null_t);
	
	AtomicString8& operator=(AtomicString8&& other);
	
	AtomicString8& operator=(String8&& other);
	
	AtomicString8& operator=(const String8& other);
	
	AtomicString8& operator=(const AtomicString8& other);
	
	AtomicString8& operator=(const String16& other);
	
	AtomicString8& operator=(const AtomicString16& _other);
	
	AtomicString8& operator=(const sl_char8* utf8);
	
	AtomicString8& operator=(const sl_char16* utf16);
	
	AtomicString8& operator=(const sl_char32* utf32);
	
public:
	String8 operator+(const String8& other) const;
	
	AtomicString8& operator+=(const String8& other);
	
	String8 operator+(const String16& other) const;
	
	AtomicString8& operator+=(const String16& other);
	
	String8 operator+(const AtomicString8& other) const;
	
	AtomicString8& operator+=(const AtomicString8& other);
	
	String8 operator+(const AtomicString16& other) const;
	
	AtomicString8& operator+=(const AtomicString16& other);
	
	String8 operator+(const sl_char8* utf8) const;
	
	AtomicString8& operator+=(const sl_char8* utf8);

	String8 operator+(const sl_char16* utf16) const;
	
	AtomicString8& operator+=(const sl_char16* utf16);

	String8 operator+(const sl_char32* utf32) const;
	
	AtomicString8& operator+=(const sl_char32* utf32);

	friend String8 operator+(const sl_char8* utf8, const AtomicString8& second);
	
	friend String8 operator+(const sl_char16* utf16, const AtomicString8& second);
	
	friend String8 operator+(const sl_char32* utf32, const AtomicString8& second);
	
public:
	String8 operator+(sl_int32 number) const;
	
	AtomicString8& operator+=(sl_int32 number);
	
	friend String8 operator+(sl_int32 number, const AtomicString8& other);
	
	String8 operator+(sl_uint32 number) const;
	
	AtomicString8& operator+=(sl_uint32 number);
	
	friend String8 operator+(sl_uint32 number, const AtomicString8& other);
	
	String8 operator+(sl_int64 number) const;
	
	AtomicString8& operator+=(sl_int64 number);
	
	friend String8 operator+(sl_int64 number, const AtomicString8& other);
	
	String8 operator+(sl_uint64 number) const;
	
	AtomicString8& operator+=(sl_uint64 number);
	
	friend String8 operator+(sl_uint64 number, const AtomicString8& other);
	
	String8 operator+(float number) const;
	
	AtomicString8& operator+=(float number);
	
	friend String8 operator+(float number, const AtomicString8& other);
	
	String8 operator+(double number) const;
	
	AtomicString8& operator+=(double number);
	
	friend String8 operator+(double number, const AtomicString8& other);
	
	String8 operator+(sl_bool value) const;
	
	AtomicString8& operator+=(sl_bool value);
	
	friend String8 operator+(sl_bool value, const AtomicString8& other);
	
public:
	sl_bool equals(const String8& other) const;
	
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const AtomicString8& other) const;
	
	sl_bool equals(const AtomicString16& other) const;

	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const AtomicString8& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String8& other, sl_size len) const;
	
	
	sl_bool equalsIgnoreCase(const String8& other) const;
	
	sl_int32 compareIgnoreCase(const String8& other) const;
	
public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString8& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const AtomicString8& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const AtomicString8& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const AtomicString8& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString8& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const AtomicString8& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const AtomicString8& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const AtomicString8& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString8& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const AtomicString8& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const AtomicString8& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const AtomicString8& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString8& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const AtomicString8& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const AtomicString8& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const AtomicString8& second);
	
	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString8& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const AtomicString8& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const AtomicString8& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const AtomicString8& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString8& other) const;
	
	sl_bool operator<(const AtomicString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const AtomicString8& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const AtomicString8& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const AtomicString8& second);
	
public:
	String8 duplicate() const;
	
	Memory toMemory() const;
	
	sl_size getUtf16(sl_char16* utf16, sl_size len) const;
	
	sl_bool getUtf16(StringData& output) const;
	
	// contains null character at last
	Memory toUtf16() const;
	
	sl_size getUtf32(sl_char32* utf32, sl_size len) const;
	
	sl_bool getUtf32(StringData& output) const;
	
	// contains null character at last
	Memory toUtf32() const;
	
	String8 substring(sl_reg start, sl_reg end = -1) const;
	
	String8 left(sl_reg len) const;
	
	String8 right(sl_reg len) const;
	
	String8 mid(sl_reg start, sl_reg len) const;
	
	sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
	
	sl_reg indexOf(const String8& str, sl_reg start = 0) const;
	
	sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String8& str, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String8& str) const;
	
	sl_bool startsWith(const sl_char8* str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String8& str) const;
	
	sl_bool endsWith(const sl_char8* str) const;

	sl_bool constains(sl_char8 ch) const;

	sl_bool contains(const String8& str) const;
	
	sl_bool contains(const sl_char8* str) const;

	void makeUpper();
	
	void makeLower();
	
	String8 toUpper() const;
	
	String8 toLower() const;
	
	String8 replaceAll(const String8& pattern, const String8& replacement) const;

	String8 replaceAll(const String8& pattern, const sl_char8* replacement) const;
	
	String8 replaceAll(const sl_char8* pattern, const String8& replacement) const;
	
	String8 replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
	
	String8 trim() const;
	
	String8 trimLeft() const;
	
	String8 trimRight() const;
	
	List<String8> split(const String8& pattern) const;
	
	List<String8> split(const sl_char8* pattern) const;
	
	String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);
	
	String8 parseBackslashEscapes(sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) const;
	
	sl_size countLineNumber(sl_size pos, sl_size* column = sl_null) const;

public:
	sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;

	sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;

	sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;

	sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;

	sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;

	sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;

	sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;

	sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;

	sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;

	sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;

	sl_bool parseSize(sl_int32 radix, sl_size* _out) const;

	sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;

	sl_bool parseFloat(float* _out) const;

	float parseFloat(float def = 0) const;

	sl_bool parseDouble(double* _out) const;

	double parseDouble(double def = 0) const;
	
	sl_bool parseBoolean(sl_bool* _out) const;
	
	sl_bool parseBoolean(sl_bool def = sl_false) const;

	sl_bool parseHexString(void* _out) const;
	
	
	String8 argBy(const Variant* params, sl_size nParams) const;
	
	template <class... ARGS>
	String8 arg(ARGS&&... args) const;
	
private:
	StringContainer8* _retainContainer() const;
	
	void _replaceContainer(StringContainer8* other);

	friend class String8;
};

/** auto-referencing object **/
template <>
class SLIB_EXPORT Atomic<String16>
{
private:
	StringContainer16* volatile m_container;
	SpinLock m_lock;
	
public:
	SLIB_CONSTEXPR Atomic() : m_container(sl_null) {}
	
	SLIB_CONSTEXPR Atomic(sl_null_t) : m_container(sl_null) {}
	
	Atomic(AtomicString16&& src);
	
	Atomic(String16&& src);
	
	Atomic(const String16& src);
	
	Atomic(const AtomicString16& src);
	
	Atomic(const String8& src);
	
	Atomic(const AtomicString8& src);
	
	~Atomic();
	
public:
	// From a single charactor
	Atomic(sl_char16 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	Atomic(const sl_char8* strUtf8);
	
	Atomic(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	Atomic(const sl_char16* strUtf16);
	
	Atomic(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	Atomic(const sl_char32* strUtf32);
	
	Atomic(const sl_char32* strUtf32, sl_reg length);
	
public:
	static const AtomicString16& null();
	
	static const AtomicString16& getEmpty();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	void setNull();
	
	void setEmpty();
	
public:
	sl_size getLength() const;
	
	sl_uint32 getHashCode() const;

	sl_uint32 getHashCodeIgnoreCase() const;
	
public:
	AtomicString16& operator=(sl_null_t);
	
	AtomicString16& operator=(AtomicString16&& other);
	
	AtomicString16& operator=(String16&& other);

	AtomicString16& operator=(const String16& other);
	
	AtomicString16& operator=(const AtomicString16& other);
	
	AtomicString16& operator=(const String8& other);
	
	AtomicString16& operator=(const AtomicString8& _other);
	
	AtomicString16& operator=(const sl_char8* utf8);
	
	AtomicString16& operator=(const sl_char16* utf16);
	
	AtomicString16& operator=(const sl_char32* utf32);
    
public:
	String16 operator+(const String16& other) const;
	
	AtomicString16& operator+=(const AtomicString16& other);

	String16 operator+(const String8& other) const;
	
	AtomicString16& operator+=(const AtomicString8& other);

	String16 operator+(const AtomicString16& other) const;
	
	AtomicString16& operator+=(const String16& other);

	String16 operator+(const AtomicString8& other) const;
	
	AtomicString16& operator+=(const String8& other);

	String16 operator+(const sl_char8* utf8) const;
	
	AtomicString16& operator+=(const sl_char8* utf8);

	String16 operator+(const sl_char16* utf16) const;
	
	AtomicString16& operator+=(const sl_char16* utf16);

	String16 operator+(const sl_char32* utf32) const;
	
	AtomicString16& operator+=(const sl_char32* utf32);

	friend String16 operator+(const sl_char8* utf8, const AtomicString16& second);
	
	friend String16 operator+(const sl_char16* utf16, const AtomicString16& second);
	
	friend String16 operator+(const sl_char32* utf32, const AtomicString16& second);
	
public:
	String16 operator+(sl_int32 number) const;
	
	AtomicString16& operator+=(sl_int32 number);
	
	friend String16 operator+(sl_int32 number, const AtomicString16& other);
	
	String16 operator+(sl_uint32 number) const;
	
	AtomicString16& operator+=(sl_uint32 number);
	
	friend String16 operator+(sl_uint32 number, const AtomicString16& other);
	
	String16 operator+(sl_int64 number) const;
	
	AtomicString16& operator+=(sl_int64 number);
	
	friend String16 operator+(sl_int64 number, const AtomicString16& other);
	
	String16 operator+(sl_uint64 number) const;
	
	AtomicString16& operator+=(sl_uint64 number);
	
	friend String16 operator+(sl_uint64 number, const AtomicString16& other);
	
	String16 operator+(float number) const;
	
	AtomicString16& operator+=(float number);
	
	friend String16 operator+(float number, const AtomicString16& other);
	
	String16 operator+(double number) const;
	
	AtomicString16& operator+=(double number);
	
	friend String16 operator+(double number, const AtomicString16& other);
	
	String16 operator+(sl_bool value) const;
	
	AtomicString16& operator+=(sl_bool value);
	
	friend String16 operator+(sl_bool value, const AtomicString16& other);
	
public:
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const String8& other) const;
	
	sl_bool equals(const AtomicString16& other) const;
	
	sl_bool equals(const AtomicString8& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const AtomicString8& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String16& other, sl_size len) const;

	
	sl_bool equalsIgnoreCase(const String16& other) const;
	
	sl_int32 compareIgnoreCase(const String16& other) const;
	
public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString8& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString8& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString8& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString8& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString8& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString8& other) const;
	
	sl_bool operator<(const AtomicString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const AtomicString16& second);
	
public:
	String16 duplicate() const;
	
	Memory toMemory() const;
	
	sl_size getUtf8(sl_char8* utf8, sl_size len) const;
	
	sl_bool getUtf8(StringData& output) const;
	
	// contains null character at last
	Memory toUtf8() const;
	
	String16 substring(sl_reg start, sl_reg end = -1) const;
	
	String16 left(sl_reg len) const;
	
	String16 right(sl_reg len) const;
	
	String16 mid(sl_reg start, sl_reg len) const;
	
	sl_reg indexOf(sl_char16 ch, sl_reg start = 0) const;
	
	sl_reg indexOf(const String16& str, sl_reg start = 0) const;
	
	sl_reg indexOf(const sl_char16* str, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String16& str, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const sl_char16* str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char16 ch) const;
	
	sl_bool startsWith(const String16& str) const;
	
	sl_bool startsWith(const sl_char16* str) const;
	
	sl_bool endsWith(sl_char16 ch) const;
	
	sl_bool endsWith(const String16& str) const;
	
	sl_bool endsWith(const sl_char16* str) const;

	sl_bool contains(sl_char16 ch) const;

	sl_bool contains(const String16& str) const;
	
	sl_bool contains(const sl_char16* str) const;

	void makeUpper();
	
	void makeLower();
	
	String16 toUpper() const;
	
	String16 toLower() const;
	
	String16 replaceAll(const String16& pattern, const String16& replacement) const;
	
	String16 replaceAll(const String16& pattern, const sl_char16* replacement) const;
	
	String16 replaceAll(const sl_char16* pattern, const String16& replacement) const;
	
	String16 replaceAll(const sl_char16* pattern, const sl_char16* replacement) const;
	
	String16 trim() const;
	
	String16 trimLeft() const;
	
	String16 trimRight() const;
	
	List<String16> split(const String16& pattern) const;
	
	List<String16> split(const sl_char16* pattern) const;
	
	String16 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);
	
	String16 parseBackslashEscapes(sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) const;
	
	sl_size countLineNumber(sl_size pos, sl_size* column = sl_null) const;
	
public:
	sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;
	
	sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;
	
	sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;
	
	sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;
	
	sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;
	
	sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;
	
	sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;
	
	sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;
	
	sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;
	
	sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;
	
	sl_bool parseSize(sl_int32 radix, sl_size* _out) const;
	
	sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;
	
	sl_bool parseFloat(float* _out) const;
	
	float parseFloat(float def = 0) const;
	
	sl_bool parseDouble(double* _out) const;
	
	double parseDouble(double def = 0) const;
	
	sl_bool parseBoolean(sl_bool* _out) const;
	
	sl_bool parseBoolean(sl_bool def = sl_false) const;
	
	sl_bool parseHexString(void* _out) const;
	
	
	String16 argBy(const Variant* params, sl_size nParams) const;
	
	template <class... ARGS>
	String16 arg(ARGS&&... args) const;
	
private:
	StringContainer16* _retainContainer() const;
	
	void _replaceContainer(StringContainer16* other);
	
	friend class String16;
};


template <class CharType>
struct StringTypeFromCharType;

template <>
struct StringTypeFromCharType<sl_char8> { typedef String8 Type; };

template <>
struct StringTypeFromCharType<sl_char16> { typedef String16 Type; };


template <>
class Compare<String8>
{
public:
	int operator()(const String8& a, const String8& b) const;
};

template <>
class Compare<String16>
{
public:
	int operator()(const String16& a, const String16& b) const;
};

template <>
class Compare<AtomicString8>
{
public:
	int operator()(const AtomicString8& a, const AtomicString8& b) const;
};

template <>
class Compare<AtomicString16>
{
public:
	int operator()(const AtomicString16& a, const AtomicString16& b) const;
};


template <>
class Equals<String8>
{
public:
	sl_bool operator()(const String8& a, const String8& b) const;
};

template <>
class Equals<String16>
{
public:
	sl_bool operator()(const String16& a, const String16& b) const;
};

template <>
class Equals<AtomicString8>
{
public:
	sl_bool operator()(const AtomicString8& a, const AtomicString8& b) const;
};

template <>
class Equals<AtomicString16>
{
public:
	sl_bool operator()(const AtomicString16& a, const AtomicString16& b) const;
};


template <>
class Hash<String8>
{
public:
	sl_uint32 operator()(const String8& a) const;
};

template <>
class Hash<String16>
{
public:
	sl_uint32 operator()(const String16& a) const;
};

template <>
class Hash<AtomicString8>
{
public:
	sl_uint32 operator()(const AtomicString8& a) const;
};

template <>
class Hash<AtomicString16>
{
public:
	sl_uint32 operator()(const AtomicString16& a) const;
};


class CompareIgnoreCaseString8
{
public:
	int operator()(const String8& a, const String8& b) const;
};

class CompareIgnoreCaseString16
{
public:
	int operator()(const String16& a, const String16& b) const;
};

typedef CompareIgnoreCaseString8 CompareIgnoreCaseString;


class EqualsIgnoreCaseString8
{
public:
	sl_bool operator()(const String8& a, const String8& b) const;
};

class EqualsIgnoreCaseString16
{
public:
	sl_bool operator()(const String16& a, const String16& b) const;
};

typedef EqualsIgnoreCaseString8 EqualsIgnoreCaseString;


class HashIgnoreCaseString8
{
public:
	sl_uint32 operator()(const String8& v) const;
	
};

class HashIgnoreCaseString16
{
public:
	sl_uint32 operator()(const String16& v) const;
	
};

typedef HashIgnoreCaseString8 HashIgnoreCaseString;


typedef String8 String;
typedef AtomicString8 AtomicString;

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(String)
SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, Ref<Referable>)
SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, String)


class SLIB_EXPORT StringData
{
public:
	union {
		const sl_char8* sz8;
		const sl_char16* sz16;
		const sl_char32* sz32;
	};
	sl_size len;
	Ref<Referable> refer;
	String8 str8;
	String16 str16;
};

class SLIB_EXPORT StringBuffer8 : public Object
{
public:
	StringBuffer8();

public:
	sl_size getLength() const;
	
	sl_bool add(const String8& str);
	
	sl_bool add(const StringData& str);
	
	sl_bool addStatic(const sl_char8* buf, sl_size length);
	
	void link(StringBuffer8& buf);
	
	void clear();
	
	String8 merge() const;
	
	Memory mergeToMemory() const;
	
private:
	LinkedQueue<StringData> m_queue;
	sl_size m_len;
	
};

class SLIB_EXPORT StringBuffer16 : public Object
{
public:
	StringBuffer16();

public:
	sl_size getLength() const;
	
	sl_bool add(const String16& str);
	
	sl_bool add(const StringData& str);
	
	sl_bool addStatic(const sl_char16* buf, sl_size length);

	void link(StringBuffer16& buf);
	
	void clear();

	String16 merge() const;
	
	Memory mergeToMemory() const;
	
private:
	LinkedQueue<StringData> m_queue;
	sl_size m_len;
	
};

typedef StringBuffer8 StringBuffer;



#define SLIB_CHAR_IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_ALPHA_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define SLIB_CHAR_IS_ALPHA_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define SLIB_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define SLIB_CHAR_IS_ALNUM(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_HEX(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define SLIB_CHAR_IS_WHITE_SPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define SLIB_CHAR_IS_SPACE_TAB(c) (c == ' ' || c == '\t')
#define SLIB_CHAR_IS_C_NAME(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || c == '_')

#define SLIB_CHAR_DIGIT_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : 10)
#define SLIB_CHAR_HEX_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : (((c) >= 'A' && (c) <= 'F') ? ((c) -  55) : ((c) >= 'a' && (c) <= 'f') ? ((c) -  87) : 16))
#define SLIB_CHAR_UPPER_TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z')?((c) + ('a' - 'A')):(c))
#define SLIB_CHAR_LOWER_TO_UPPER(c) (((c) >= 'a' && (c) <= 'z')?((c) - ('a' - 'A')):(c))


#define SLIB_STATIC_STRING8(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer8 _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer8* _static_string_##name = &_static_string_container_##name; \
	static slib::String8& name = *(reinterpret_cast<slib::String8*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING8(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer8 _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::_String8_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static slib::AtomicString8& name = *(reinterpret_cast<slib::AtomicString8*>(&_static_string_##name));

#if !defined(SLIB_USE_UNICODE32)
#define SLIB_STATIC_STRING16(name, str) \
	static wchar_t _static_string_buf_##name[] = L##str; \
	static slib::StringContainer16 _static_string_container_##name = {(sl_char16*)_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
	static slib::StringContainer16* _static_string_##name = &_static_string_container_##name; \
	static slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_##name));
#define SLIB_STATIC_ATOMIC_STRING16(name, str) \
	static wchar_t _static_string_buf_##name[] = L##str; \
	static slib::StringContainer16 _static_string_container_##name = {(sl_char16*)_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
	static slib::_String16_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_##name));
#endif

#define SLIB_STATIC_STRING8_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer8 _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer8* _static_string_##name = &_static_string_container_##name; \
	static slib::String8& name = *(reinterpret_cast<slib::String8*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING8_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer8 _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::_String8_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static slib::AtomicString8& name = *(reinterpret_cast<slib::AtomicString8*>(&_static_string_##name));

#define SLIB_STATIC_STRING16_BY_ARRAY(name, ...) \
	static sl_char16 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer16 _static_string_container_##name = {_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
	static slib::StringContainer16* _static_string_##name = &_static_string_container_##name; \
	static slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING16_BY_ARRAY(name, ...) \
	static sl_char16 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer16 _static_string_container_##name = {_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
	static slib::_String16_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_##name));

#define SLIB_STATIC_STRING8_NULL(name) \
	static slib::_String8_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::String8& name = *(reinterpret_cast<slib::String8*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING8_NULL(name) \
	static slib::_String8_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::AtomicString8& name = *(reinterpret_cast<slib::AtomicString8*>(&_static_string_null_##name));

#define SLIB_STATIC_STRING16_NULL(name) \
	static slib::_String16_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING16_NULL(name) \
	static slib::_String16_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_null_##name));

#define SLIB_STATIC_STRING SLIB_STATIC_STRING8
#define SLIB_STATIC_STRING_BY_ARRAY SLIB_STATIC_STRING8_BY_ARRAY
#define SLIB_STATIC_STRING_NULL SLIB_STATIC_STRING8_NULL
#define SLIB_STATIC_ATOMIC_STRING SLIB_STATIC_ATOMIC_STRING8
#define SLIB_STATIC_ATOMIC_STRING_BY_ARRAY SLIB_STATIC_ATOMIC_STRING8_BY_ARRAY
#define SLIB_STATIC_ATOMIC_STRING_NULL SLIB_STATIC_ATOMIC_STRING8_NULL


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

SLIB_INLINE String8::String8(String8&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE String16::String16(String16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString8::Atomic(AtomicString8&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString16::Atomic(AtomicString16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}


SLIB_INLINE String8::String8(AtomicString8&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE String16::String16(AtomicString16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString8::Atomic(String8&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString16::Atomic(String16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}


SLIB_INLINE const String8& String8::null()
{
	return *(reinterpret_cast<String8 const*>(&_String8_Null));
}

SLIB_INLINE const String16& String16::null()
{
	return *(reinterpret_cast<String16 const*>(&_String16_Null));
}

SLIB_INLINE const AtomicString8& AtomicString8::null()
{
	return *(reinterpret_cast<AtomicString8 const*>(&_String8_Null));
}

SLIB_INLINE const AtomicString16& AtomicString16::null()
{
	return *(reinterpret_cast<AtomicString16 const*>(&_String16_Null));
}


SLIB_INLINE const String8& String8::getEmpty()
{
	return *(reinterpret_cast<String8 const*>(&_String8_Empty));
}

SLIB_INLINE const String16& String16::getEmpty()
{
	return *(reinterpret_cast<String16 const*>(&_String16_Empty));
}

SLIB_INLINE const AtomicString8& AtomicString8::getEmpty()
{
	return *(reinterpret_cast<AtomicString8 const*>(&_String8_Empty));
}

SLIB_INLINE const AtomicString16& AtomicString16::getEmpty()
{
	return *(reinterpret_cast<AtomicString16 const*>(&_String16_Empty));
}


SLIB_INLINE const String8& String8::getNotNull() const
{
	if (!m_container) {
		return *(reinterpret_cast<String8 const*>(&_String8_Empty));
	}
	return *this;
}

SLIB_INLINE const String16& String16::getNotNull() const
{
	if (!m_container) {
		return *(reinterpret_cast<String16 const*>(&_String16_Empty));
	}
	return *this;
}


SLIB_INLINE sl_bool String8::isNull() const
{
	return m_container == sl_null;
}

SLIB_INLINE sl_bool String16::isNull() const
{
	return m_container == sl_null;
}

SLIB_INLINE sl_bool AtomicString8::isNull() const
{
	return m_container == sl_null;
}

SLIB_INLINE sl_bool AtomicString16::isNull() const
{
	return m_container == sl_null;
}


SLIB_INLINE sl_bool String8::isNotNull() const
{
	return m_container != sl_null;
}

SLIB_INLINE sl_bool String16::isNotNull() const
{
	return m_container != sl_null;
}

SLIB_INLINE sl_bool AtomicString8::isNotNull() const
{
	return m_container != sl_null;
}

SLIB_INLINE sl_bool AtomicString16::isNotNull() const
{
	return m_container != sl_null;
}


SLIB_INLINE sl_bool String8::isEmpty() const
{
	if (m_container) {
		return (m_container->len == 0);
	} else {
		return sl_true;
	}
}

SLIB_INLINE sl_bool String16::isEmpty() const
{
	if (m_container) {
		return (m_container->len == 0);
	} else {
		return sl_true;
	}
}


SLIB_INLINE sl_bool String8::isNotEmpty() const
{
	if (m_container) {
		return (m_container->len != 0);
	} else {
		return sl_false;
	}
}

SLIB_INLINE sl_bool String16::isNotEmpty() const
{
	if (m_container) {
		return (m_container->len != 0);
	} else {
		return sl_false;
	}
}


SLIB_INLINE sl_char8* String8::getData() const
{
	if (m_container) {
		return m_container->sz;
	} else {
		return (sl_char8*)((void*)(""));
	}
}

SLIB_INLINE sl_char16* String16::getData() const
{
	if (m_container) {
		return m_container->sz;
	} else {
		return (sl_char16*)((void*)(L""));
	}
}


SLIB_INLINE sl_size String8::getLength() const
{
	if (m_container) {
		return m_container->len;
	} else {
		return 0;
	}
}

SLIB_INLINE sl_size String16::getLength() const
{
	if (m_container) {
		return m_container->len;
	} else {
		return 0;
	}
}


SLIB_NAMESPACE_END

#endif
