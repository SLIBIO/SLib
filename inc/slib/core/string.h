#ifndef CHECKHEADER_SLIB_CORE_STRING
#define CHECKHEADER_SLIB_CORE_STRING

#include "definition.h"

#include "memory.h"
#include "algorithm.h"
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
class String16;
class SafeString8;
class SafeString16;
class StringData;

/** auto-referencing object **/
class SLIB_EXPORT String8
{
private:
	StringContainer8* m_container;
	
private:
	String8(StringContainer8* container);
	
public:
	String8();
	
	String8(String8&& src);
	
	String8(SafeString8&& _src);
	
	String8(const String8& src);
	
	String8(const SafeString8& src);
	
	String8(const String16& src);
	
	String8(const SafeString16& src);

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
	
	static String8 fromUtf16(const void* utf16, sl_reg len = -1);
	
	static String8 fromUtf16(const Memory& mem);
	
	static String8 fromUtf32(const void* utf32, sl_reg len = -1);
	
	static String8 fromUtf32(const Memory& mem);

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
	
	void setLength(sl_size len);
	
	sl_uint32 getHashCode() const;
	
	void setHashCode(sl_uint32 hash);

	sl_char8 getAt(sl_reg index) const;
	
	sl_bool setAt(sl_reg index, sl_char8 ch);
	
public:
	String8& operator=(String8&& other);
	
	String8& operator=(SafeString8&& _other);

	String8& operator=(const String8& other);
	
	String8& operator=(const SafeString8& other);
	
	String8& operator=(const String16& other);
	
	String8& operator=(const SafeString16& other);
	
	String8& operator=(const sl_char8* utf8);
	
	String8& operator=(const sl_char16* utf16);
	
	String8& operator=(const sl_char32* utf32);

public:
	String8 operator+(const String8& other) const;
	
	String8& operator+=(const String8& other);
	
	String8 operator+(const String16& other) const;
	
	String8& operator+=(const String16& other);
	
	String8 operator+(const SafeString8& _other) const;
	
	String8& operator+=(const SafeString8& other);
	
	String8 operator+(const SafeString16& other) const;
	
	String8& operator+=(const SafeString16& other);
	
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
	
	sl_bool equals(const SafeString8& other) const;
	
	sl_bool equals(const SafeString16& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const SafeString8& other) const;
	
	sl_int32 compare(const SafeString16& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String8& other, sl_size len) const;
	

public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const SafeString8& other) const;
	
	sl_bool operator==(const SafeString16& other) const;

	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const SafeString8& other) const;
	
	sl_bool operator!=(const SafeString16& other) const;

	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const SafeString8& other) const;
	
	sl_bool operator>=(const SafeString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const SafeString8& other) const;
	
	sl_bool operator<=(const SafeString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const String8& second);

	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const SafeString8& other) const;
	
	sl_bool operator>(const SafeString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const String8& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const SafeString8& other) const;
	
	sl_bool operator<(const SafeString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const String8& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const String8& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const String8& second);
	
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
	
	sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String8& str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String8& str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String8& str) const;

	sl_bool contains(sl_char8 ch) const;

	sl_bool contains(const String8& str) const;

	void makeUpper();
	
	void makeLower();
	
	String8 toUpper() const;
	
	String8 toLower() const;
	
	String8 replaceAll(const String8& pattern, const String8& replacement) const;
	
	String8 trim() const;
	
	String8 trimLeft() const;
	
	String8 trimRight() const;
	
	List<String8> split(const String8& pattern) const;
	
	String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	
	static String8 parseBackslashEscapes(const sl_char8* input, sl_size len, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	
	String8 parseBackslashEscapes(sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) const;
	
	static sl_size countLineNumber(const sl_char8* input, sl_size len, sl_size* columnLast = sl_null);
	
	sl_size countLineNumber(sl_size pos, sl_size* column = sl_null) const;
	
public:
	// radix: 2 ~ 64,   flagUpperCase only works if radix <= 36 (0~9a~z)
	static String8 fromInt32(sl_int32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromUint32(sl_uint32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromInt64(sl_int64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromUint64(sl_uint64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromInt(sl_reg value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromSize(sl_size value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);

	static String8 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);
	
	static String8 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);

	static String8 fromPointerValue(const void* pointer);

	static String8 fromBoolean(sl_bool value);
	
	static String8 makeHexString(const void* data, sl_size size);
	
	static String8 makeHexString(const Memory& mem);

	
	static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const char*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;

	sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;

	
	static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;

	sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;
	

	static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;

	sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;

	
	static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;

	sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;


	sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;

	sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;


	sl_bool parseSize(sl_int32 radix, sl_size* _out) const;

	sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;


	static sl_reg parseFloat(float* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseFloat(float* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseFloat(float* _out) const;

	float parseFloat(float def = 0) const;

	
	static sl_reg parseDouble(double* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseDouble(double* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseDouble(double* _out) const;

	double parseDouble(double def = 0) const;

	
	static sl_reg parseHexString(void* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseHexString(void* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseHexString(void* _out) const;


public:
	// utf8 conversion
	static sl_size utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer);
	
	static sl_size utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer);
	
	static sl_size utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer);
	
	static sl_size utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer);
	
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
	friend class SafeString8;
	
};

/** auto-referencing object **/
class SLIB_EXPORT String16
{
private:
	StringContainer16* m_container;
	
private:
	String16(StringContainer16* container);

public:
	String16();
	
	String16(String16&& src);
	
	String16(SafeString16&& _src);
	
	String16(const String16& src);
	
	String16(const SafeString16& src);

	String16(const String8& src);
	
	String16(const SafeString8& src);

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

	static String16 fromUtf16(const void* utf16, sl_reg len = -1);

	static String16 fromUtf16(const Memory& mem);

	static String16 fromUtf32(const void* utf32, sl_reg len = -1);

	static String16 fromUtf32(const Memory& mem);

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
	
	void setLength(sl_size len);
	
	sl_uint32 getHashCode() const;
	
	void setHashCode(sl_uint32 hash);
	
	sl_char16 getAt(sl_reg index) const;
	
	sl_bool setAt(sl_reg index, sl_char16 ch);
	
public:
	String16& operator=(String16&& other);
	
	String16& operator=(SafeString16&& _other);

	String16& operator=(const String16& other);
	
	String16& operator=(const SafeString16& other);
	
	String16& operator=(const String8& other);
	
	String16& operator=(const SafeString8& other);
	
	String16& operator=(const sl_char8* utf8);
	
	String16& operator=(const sl_char16* utf16);
	
	String16& operator=(const sl_char32* utf32);
	
public:
	String16 operator+(const String16& other) const;
	
	String16& operator+=(const String16& other);

	String16 operator+(const String8& other) const;
	
	String16& operator+=(const String8& other);

	String16 operator+(const SafeString16& _other) const;
	
	String16& operator+=(const SafeString16& other);

	String16 operator+(const SafeString8& other) const;
	
	String16& operator+=(const SafeString8& other);

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
	
	sl_bool equals(const SafeString16& other) const;
	
	sl_bool equals(const SafeString8& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const SafeString16& other) const;
	
	sl_int32 compare(const SafeString8& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String16& other, sl_size len) const;
	
public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const SafeString8& other) const;
	
	sl_bool operator==(const SafeString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const SafeString8& other) const;
	
	sl_bool operator!=(const SafeString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const SafeString8& other) const;
	
	sl_bool operator>=(const SafeString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const SafeString8& other) const;
	
	sl_bool operator<=(const SafeString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const SafeString8& other) const;
	
	sl_bool operator>(const SafeString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const SafeString8& other) const;
	
	sl_bool operator<(const SafeString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const String16& second);

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
	
	sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String16& str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char16 ch) const;
	
	sl_bool startsWith(const String16& str) const;
	
	sl_bool endsWith(sl_char16 ch) const;
	
	sl_bool endsWith(const String16& str) const;

	sl_bool contains(sl_char16 ch) const;

	sl_bool contains(const String16& str) const;
	
	void makeUpper();
	
	void makeLower();
	
	String16 toUpper() const;
	
	String16 toLower() const;
	
	String16 replaceAll(const String16& pattern, const String16& replacement) const;
	
	String16 trim() const;
	
	String16 trimLeft() const;
	
	String16 trimRight() const;
	
	List<String16> split(const String16& pattern) const;
	
	String16 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	
	static String16 parseBackslashEscapes(const sl_char16* input, sl_size len, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	
	String16 parseBackslashEscapes(sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null) const;
	
	static sl_size countLineNumber(const sl_char16* input, sl_size len, sl_size* columnLast = sl_null);
	
	sl_size countLineNumber(sl_size pos, sl_size* column = sl_null) const;

public:
	// radix: 2 ~ 64,   flagUpperCase only works if radix <= 36 (0~9a~z)
	static String16 fromInt32(sl_int32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromUint32(sl_uint32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromInt64(sl_int64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromUint64(sl_uint64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromInt(sl_reg value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromSize(sl_size value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);

	static String16 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);
	
	static String16 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);

	static String16 fromPointerValue(const void* pointer);

	static String16 fromBoolean(sl_bool value);

	static String16 makeHexString(const void* data, sl_size size);
	
	static String16 makeHexString(const Memory& mem);

	
	static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const char*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16*sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt32(sl_int32 radix, sl_int32* _out) const;

	sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const;

	
	static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint32(sl_int32 radix, sl_uint32* _out) const;

	sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const;

	
	static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseInt64(sl_int32 radix, sl_int64* _out) const;
	
	sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const;


	static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseUint64(sl_int32 radix, sl_uint64* _out) const;
	
	sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const;


	sl_bool parseInt(sl_int32 radix, sl_reg* _out) const;

	sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const;


	sl_bool parseSize(sl_int32 radix, sl_size* _out) const;

	sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const;


	static sl_reg parseFloat(float* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseFloat(float* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseFloat(float* _out) const;

	float parseFloat(float def = 0) const;


	static sl_reg parseDouble(double* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseDouble(double* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseDouble(double* _out) const;

	double parseDouble(double def = 0) const;


	static sl_reg parseHexString(void* _out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	static sl_reg parseHexString(void* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);
	
	sl_bool parseHexString(void* _out) const;

public:
	// utf8 conversion
	static sl_size utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer);
	
	static sl_size utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer);
	
	static sl_size utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer);
	
	static sl_size utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer);

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
	friend class SafeString16;
	
};


/** auto-referencing object **/
class SLIB_EXPORT SafeString8
{
private:
	StringContainer8* m_container;
	SpinLock m_lock;
	
public:
	SafeString8();
	
	SafeString8(SafeString8&& src);
	
	SafeString8(String8&& src);
	
	SafeString8(const String8& src);
	
	SafeString8(const SafeString8& src);
	
	SafeString8(const String16& src);
	
	SafeString8(const SafeString16& src);

	~SafeString8();

public:
	// From a single charactor
	SafeString8(sl_char8 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	SafeString8(const sl_char8* strUtf8);
	
	// From an utf-8 string
	SafeString8(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	SafeString8(const sl_char16* strUtf16);
	
	// From an utf-16 string
	SafeString8(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	SafeString8(const sl_char32* strUtf32);
	
	// From an utf-32 string
	SafeString8(const sl_char32* strUtf32, sl_reg length);
	
public:
	static const SafeString8& null();
	
	static const SafeString8& getEmpty();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	void setNull();
	
	void setEmpty();

public:
	sl_size getLength() const;
	
	sl_uint32 getHashCode() const;
	
public:
	SafeString8& operator=(SafeString8&& other);
	
	SafeString8& operator=(String8&& other);
	
	SafeString8& operator=(const String8& other);
	
	SafeString8& operator=(const SafeString8& other);
	
	SafeString8& operator=(const String16& other);
	
	SafeString8& operator=(const SafeString16& _other);
	
	SafeString8& operator=(const sl_char8* utf8);
	
	SafeString8& operator=(const sl_char16* utf16);
	
	SafeString8& operator=(const sl_char32* utf32);
	
public:
	String8 operator+(const String8& other) const;
	
	SafeString8& operator+=(const String8& other);
	
	String8 operator+(const String16& other) const;
	
	SafeString8& operator+=(const String16& other);
	
	String8 operator+(const SafeString8& other) const;
	
	SafeString8& operator+=(const SafeString8& other);
	
	String8 operator+(const SafeString16& other) const;
	
	SafeString8& operator+=(const SafeString16& other);
	
	String8 operator+(const sl_char8* utf8) const;
	
	SafeString8& operator+=(const sl_char8* utf8);

	String8 operator+(const sl_char16* utf16) const;
	
	SafeString8& operator+=(const sl_char16* utf16);

	String8 operator+(const sl_char32* utf32) const;
	
	SafeString8& operator+=(const sl_char32* utf32);

	friend String8 operator+(const sl_char8* utf8, const SafeString8& second);
	
	friend String8 operator+(const sl_char16* utf16, const SafeString8& second);
	
	friend String8 operator+(const sl_char32* utf32, const SafeString8& second);
	
public:
	String8 operator+(sl_int32 number) const;
	
	SafeString8& operator+=(sl_int32 number);
	
	friend String8 operator+(sl_int32 number, const SafeString8& other);
	
	
	String8 operator+(sl_uint32 number) const;
	
	SafeString8& operator+=(sl_uint32 number);
	
	friend String8 operator+(sl_uint32 number, const SafeString8& other);
	
	
	String8 operator+(sl_int64 number) const;
	
	SafeString8& operator+=(sl_int64 number);
	
	friend String8 operator+(sl_int64 number, const SafeString8& other);
	
	
	String8 operator+(sl_uint64 number) const;
	
	SafeString8& operator+=(sl_uint64 number);
	
	friend String8 operator+(sl_uint64 number, const SafeString8& other);
	
	
	String8 operator+(float number) const;
	
	SafeString8& operator+=(float number);
	
	friend String8 operator+(float number, const SafeString8& other);
	
	
	String8 operator+(double number) const;
	
	SafeString8& operator+=(double number);
	
	friend String8 operator+(double number, const SafeString8& other);
	
	
	String8 operator+(sl_bool value) const;
	
	SafeString8& operator+=(sl_bool value);
	
	friend String8 operator+(sl_bool value, const SafeString8& other);
	
public:
	sl_bool equals(const String8& other) const;
	
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const SafeString8& other) const;
	
	sl_bool equals(const SafeString16& other) const;

	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const SafeString8& other) const;
	
	sl_int32 compare(const SafeString16& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String8& other, sl_size len) const;
	

public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const SafeString8& other) const;
	
	sl_bool operator==(const SafeString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const SafeString8& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const SafeString8& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const SafeString8& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const SafeString8& other) const;
	
	sl_bool operator!=(const SafeString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const SafeString8& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const SafeString8& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const SafeString8& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const SafeString8& other) const;
	
	sl_bool operator>=(const SafeString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const SafeString8& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const SafeString8& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const SafeString8& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const SafeString8& other) const;
	
	sl_bool operator<=(const SafeString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const SafeString8& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const SafeString8& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const SafeString8& second);
	
	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const SafeString8& other) const;
	
	sl_bool operator>(const SafeString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const SafeString8& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const SafeString8& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const SafeString8& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const SafeString8& other) const;
	
	sl_bool operator<(const SafeString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const SafeString8& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const SafeString8& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const SafeString8& second);
	
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
	
	sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String8& str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String8& str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String8& str) const;

	sl_bool constains(sl_char8 ch) const;

	sl_bool contains(const String8& str) const;

	void makeUpper();
	
	void makeLower();
	
	String8 toUpper() const;
	
	String8 toLower() const;
	
	String8 replaceAll(const String8& pattern, const String8& replacement) const;

	String8 trim() const;
	
	String8 trimLeft() const;
	
	String8 trimRight() const;
	
	List<String8> split(const String8& pattern) const;
	
	String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	
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

	sl_bool parseHexString(void* _out) const;

private:
	StringContainer8* _retainContainer() const;
	
	void _replaceContainer(StringContainer8* other);

	friend class String8;
};

/** auto-referencing object **/
class SLIB_EXPORT SafeString16
{
private:
	StringContainer16* m_container;
	SpinLock m_lock;
	
public:
	SafeString16();
	
	SafeString16(SafeString16&& src);
	
	SafeString16(String16&& src);
	
	SafeString16(const String16& src);
	
	SafeString16(const SafeString16& src);
	
	SafeString16(const String8& src);
	
	SafeString16(const SafeString8& src);
	
	~SafeString16();
	
public:
	// From a single charactor
	SafeString16(sl_char16 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	SafeString16(const sl_char8* strUtf8);
	
	SafeString16(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	SafeString16(const sl_char16* strUtf16);
	
	SafeString16(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	SafeString16(const sl_char32* strUtf32);
	
	SafeString16(const sl_char32* strUtf32, sl_reg length);
	
public:
	static const SafeString16& null();
	
	static const SafeString16& getEmpty();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	void setNull();
	
	void setEmpty();
	
public:
	sl_size getLength() const;
	
	sl_uint32 getHashCode() const;

public:
	SafeString16& operator=(SafeString16&& other);
	
	SafeString16& operator=(String16&& other);

	SafeString16& operator=(const String16& other);
	
	SafeString16& operator=(const SafeString16& other);
	
	SafeString16& operator=(const String8& other);
	
	SafeString16& operator=(const SafeString8& _other);
	
	SafeString16& operator=(const sl_char8* utf8);
	
	SafeString16& operator=(const sl_char16* utf16);
	
	SafeString16& operator=(const sl_char32* utf32);
    
public:
	String16 operator+(const String16& other) const;
	
	SafeString16& operator+=(const SafeString16& other);

	String16 operator+(const String8& other) const;
	
	SafeString16& operator+=(const SafeString8& other);

	String16 operator+(const SafeString16& other) const;
	
	SafeString16& operator+=(const String16& other);

	String16 operator+(const SafeString8& other) const;
	
	SafeString16& operator+=(const String8& other);

	String16 operator+(const sl_char8* utf8) const;
	
	SafeString16& operator+=(const sl_char8* utf8);

	String16 operator+(const sl_char16* utf16) const;
	
	SafeString16& operator+=(const sl_char16* utf16);

	String16 operator+(const sl_char32* utf32) const;
	
	SafeString16& operator+=(const sl_char32* utf32);

	friend String16 operator+(const sl_char8* utf8, const SafeString16& second);
	
	friend String16 operator+(const sl_char16* utf16, const SafeString16& second);
	
	friend String16 operator+(const sl_char32* utf32, const SafeString16& second);
	
public:
	String16 operator+(sl_int32 number) const;
	
	SafeString16& operator+=(sl_int32 number);
	
	friend String16 operator+(sl_int32 number, const SafeString16& other);
	
	
	String16 operator+(sl_uint32 number) const;
	
	SafeString16& operator+=(sl_uint32 number);
	
	friend String16 operator+(sl_uint32 number, const SafeString16& other);
	
	
	String16 operator+(sl_int64 number) const;
	
	SafeString16& operator+=(sl_int64 number);
	
	friend String16 operator+(sl_int64 number, const SafeString16& other);
	
	
	String16 operator+(sl_uint64 number) const;
	
	SafeString16& operator+=(sl_uint64 number);
	
	friend String16 operator+(sl_uint64 number, const SafeString16& other);
	
	
	String16 operator+(float number) const;
	
	SafeString16& operator+=(float number);
	
	friend String16 operator+(float number, const SafeString16& other);
	
	
	String16 operator+(double number) const;
	
	SafeString16& operator+=(double number);
	
	friend String16 operator+(double number, const SafeString16& other);
	
	
	String16 operator+(sl_bool value) const;
	
	SafeString16& operator+=(sl_bool value);
	
	friend String16 operator+(sl_bool value, const SafeString16& other);
	
public:
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const String8& other) const;
	
	sl_bool equals(const SafeString16& other) const;
	
	sl_bool equals(const SafeString8& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const String8& other) const;
	
	sl_int32 compare(const SafeString16& other) const;
	
	sl_int32 compare(const SafeString8& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String16& other, sl_size len) const;

public:
	sl_bool operator==(const String8& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const SafeString8& other) const;
	
	sl_bool operator==(const SafeString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const SafeString16& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const SafeString16& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const SafeString16& second);
	
	
	sl_bool operator!=(const String8& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const SafeString8& other) const;
	
	sl_bool operator!=(const SafeString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const SafeString16& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const SafeString16& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const SafeString16& second);
	
	
	sl_bool operator>=(const String8& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const SafeString8& other) const;
	
	sl_bool operator>=(const SafeString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const SafeString16& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const SafeString16& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const SafeString16& second);
	
	
	sl_bool operator<=(const String8& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const SafeString8& other) const;
	
	sl_bool operator<=(const SafeString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const SafeString16& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const SafeString16& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const SafeString16& second);
	
	
	sl_bool operator>(const String8& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const SafeString8& other) const;
	
	sl_bool operator>(const SafeString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const SafeString16& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const SafeString16& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const SafeString16& second);
	
	
	sl_bool operator<(const String8& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const SafeString8& other) const;
	
	sl_bool operator<(const SafeString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const SafeString16& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const SafeString16& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const SafeString16& second);
	
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
	
	sl_reg lastIndexOf(sl_char16 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String16& str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char16 ch) const;
	
	sl_bool startsWith(const String16& str) const;
	
	sl_bool endsWith(sl_char16 ch) const;
	
	sl_bool endsWith(const String16& str) const;

	sl_bool contains(sl_char16 ch) const;

	sl_bool contains(const String16& str) const;

	void makeUpper();
	
	void makeLower();
	
	String16 toUpper() const;
	
	String16 toLower() const;
	
	String16 replaceAll(const String16& pattern, const String16& replacement) const;
	
	String16 trim() const;
	
	String16 trimLeft() const;
	
	String16 trimRight() const;
	
	List<String16> split(const String16& pattern) const;
	
	String16 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	
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
	
	sl_bool parseHexString(void* _out) const;


private:
	StringContainer16* _retainContainer() const;
	
	void _replaceContainer(StringContainer16* other);
	
	friend class String16;
};


template <>
int Compare<String8>::compare(const String8& a, const String8& b);

template <>
int Compare<String16>::compare(const String16& a, const String16& b);

template <>
int Compare<SafeString8>::compare(const SafeString8& a, const SafeString8& b);

template <>
int Compare<SafeString16>::compare(const SafeString16& a, const SafeString16& b);


template <>
sl_bool Compare<String8>::equals(const String8& a, const String8& b);

template <>
sl_bool Compare<String16>::equals(const String16& a, const String16& b);

template <>
sl_bool Compare<SafeString8>::equals(const SafeString8& a, const SafeString8& b);

template <>
sl_bool Compare<SafeString16>::equals(const SafeString16& a, const SafeString16& b);


template <>
sl_uint32 Hash<String8>::hash(const String8& v);

template <>
sl_uint32 Hash<String16>::hash(const String16& v);

template <>
sl_uint32 Hash<SafeString8>::hash(const SafeString8& v);

template <>
sl_uint32 Hash<SafeString16>::hash(const SafeString16& v);

typedef String8 String;
typedef SafeString8 SafeString;

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
	sl_bool add(const String8& str);
	
	sl_bool add(const StringData& str);
	
	sl_bool addStatic(const sl_char8* buf, sl_size length);
	
	sl_size getLength() const;

	String8 merge() const;
	
	Memory mergeToMemory() const;
	
private:
	Queue<StringData> m_queue;
	sl_size m_len;
	
};

class SLIB_EXPORT StringBuffer16 : public Object
{
public:
	StringBuffer16();

public:
	sl_bool add(const String16& str);
	
	sl_bool add(const StringData& str);
	
	sl_bool addStatic(const sl_char16* buf, sl_size length);

	sl_size getLength() const;
	
	String16 merge() const;
	
	Memory mergeToMemory() const;
	
private:
	Queue<StringData> m_queue;
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

#define SLIB_CHAR_DIGIT_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : 10)
#define SLIB_CHAR_HEX_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : (((c) >= 'A' && (c) <= 'F') ? ((c) -  55) : ((c) >= 'a' && (c) <= 'f') ? ((c) -  87) : 16))


#define SLIB_STATIC_STRING8(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer8 _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer8* _static_string_##name = &_static_string_container_##name; \
	static slib::String8& name = *((slib::String8*)((void*)(&_static_string_##name)));

#if !defined(SLIB_USE_UNICODE32)
#define SLIB_STATIC_STRING16(name, str) \
	static wchar_t _static_string_buf_##name[] = L##str; \
	static slib::StringContainer16 _static_string_container_##name = {(sl_char16*)_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
	static slib::StringContainer16* _static_string_##name = &_static_string_container_##name; \
	static slib::String16& name = *((slib::String16*)((void*)(&_static_string_##name)));
#endif

#define SLIB_STATIC_STRING8_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer8 _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer8* _static_string_##name = &_static_string_container_##name; \
	static slib::String8& name = *((slib::String8*)((void*)(&_static_string_##name)));

#define SLIB_STATIC_STRING16_BY_ARRAY(name, ...) \
	static sl_char16 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer16 _static_string_container_##name = {_static_string_buf_##name, (sizeof(_static_string_buf_##name)/2)-1, 0, -1}; \
	static slib::StringContainer16* _static_string_##name = &_static_string_container_##name; \
	static slib::String16& name = *((slib::String16*)((void*)(&_static_string_##name)));

#define SLIB_STATIC_STRING8_NULL(name) \
	static slib::_String8_Const _static_string_null_##name = {slib::_String8_Null.container, 0}; \
	static slib::String8& name = *((slib::String8*)((void*)(&_static_string_null_##name)));

#define SLIB_STATIC_STRING16_NULL(name) \
	static slib::_String16_Const _static_string_null_##name = {slib::_String16_Null.container, 0}; \
	static slib::String16& name = *((slib::String16*)((void*)(&_static_string_null_##name)));

#define SLIB_STATIC_STRING SLIB_STATIC_STRING8
#define SLIB_STATIC_STRING_BY_ARRAY SLIB_STATIC_STRING8_BY_ARRAY
#define SLIB_STATIC_STRING_NULL SLIB_STATIC_STRING8_NULL


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

SLIB_INLINE String8::String8(StringContainer8* container) : m_container(container)
{
}

SLIB_INLINE String16::String16(StringContainer16* container) : m_container(container)
{
}


SLIB_INLINE String8::String8() : m_container(_String8_Null.container)
{
}

SLIB_INLINE String16::String16() : m_container(_String16_Null.container)
{
}

SLIB_INLINE SafeString8::SafeString8() : m_container(_String8_Null.container)
{
}

SLIB_INLINE SafeString16::SafeString16() : m_container(_String16_Null.container)
{
}


SLIB_INLINE String8::String8(String8&& src)
{
	m_container = src.m_container;
	src.m_container = _String8_Null.container;
}

SLIB_INLINE String16::String16(String16&& src)
{
	m_container = src.m_container;
	src.m_container = _String16_Null.container;
}

SLIB_INLINE SafeString8::SafeString8(SafeString8&& src)
{
	m_container = src.m_container;
	src.m_container = _String8_Null.container;
}

SLIB_INLINE SafeString16::SafeString16(SafeString16&& src)
{
	m_container = src.m_container;
	src.m_container = _String16_Null.container;
}


SLIB_INLINE String8::String8(SafeString8&& _src)
{
	String8& src = *((String8*)((void*)(&_src)));
	m_container = src.m_container;
	src.m_container = _String8_Null.container;
}

SLIB_INLINE String16::String16(SafeString16&& _src)
{
	String16& src = *((String16*)((void*)(&_src)));
	m_container = src.m_container;
	src.m_container = _String16_Null.container;
}

SLIB_INLINE SafeString8::SafeString8(String8&& _src)
{
	SafeString8& src = *((SafeString8*)((void*)(&_src)));
	m_container = src.m_container;
	src.m_container = _String8_Null.container;
}

SLIB_INLINE SafeString16::SafeString16(String16&& _src)
{
	SafeString16& src = *((SafeString16*)((void*)(&_src)));
	m_container = src.m_container;
	src.m_container = _String16_Null.container;
}


SLIB_INLINE const String8& String8::null()
{
	return *((String8*)((void*)&_String8_Null));
}

SLIB_INLINE const String16& String16::null()
{
	return *((String16*)((void*)&_String16_Null));
}

SLIB_INLINE const SafeString8& SafeString8::null()
{
	return *((SafeString8*)((void*)&_String8_Null));
}

SLIB_INLINE const SafeString16& SafeString16::null()
{
	return *((SafeString16*)((void*)&_String16_Null));
}


SLIB_INLINE const String8& String8::getEmpty()
{
	return *((String8*)((void*)&_String8_Empty));
}

SLIB_INLINE const String16& String16::getEmpty()
{
	return *((String16*)((void*)&_String16_Empty));
}

SLIB_INLINE const SafeString8& SafeString8::getEmpty()
{
	return *((SafeString8*)((void*)&_String8_Empty));
}

SLIB_INLINE const SafeString16& SafeString16::getEmpty()
{
	return *((SafeString16*)((void*)&_String16_Empty));
}


SLIB_INLINE const String8& String8::getNotNull() const
{
	if (m_container == _String8_Null.container) {
		return *((String8*)((void*)&_String8_Empty));
	}
	return *this;
}

SLIB_INLINE const String16& String16::getNotNull() const
{
	if (m_container == _String16_Null.container) {
		return *((String16*)((void*)&_String16_Empty));
	}
	return *this;
}


SLIB_INLINE sl_bool String8::isNull() const
{
	return m_container == _String8_Null.container;
}

SLIB_INLINE sl_bool String16::isNull() const
{
	return m_container == _String16_Null.container;
}

SLIB_INLINE sl_bool SafeString8::isNull() const
{
	return m_container == _String8_Null.container;
}

SLIB_INLINE sl_bool SafeString16::isNull() const
{
	return m_container == _String16_Null.container;
}


SLIB_INLINE sl_bool String8::isNotNull() const
{
	return m_container != _String8_Null.container;
}

SLIB_INLINE sl_bool String16::isNotNull() const
{
	return m_container != _String16_Null.container;
}

SLIB_INLINE sl_bool SafeString8::isNotNull() const
{
	return m_container != _String8_Null.container;
}

SLIB_INLINE sl_bool SafeString16::isNotNull() const
{
	return m_container != _String16_Null.container;
}


SLIB_INLINE sl_bool String8::isEmpty() const
{
	return (m_container->len == 0);
}

SLIB_INLINE sl_bool String16::isEmpty() const
{
	return (m_container->len == 0);
}


SLIB_INLINE sl_bool String8::isNotEmpty() const
{
	return (m_container->len != 0);
}

SLIB_INLINE sl_bool String16::isNotEmpty() const
{
	return (m_container->len != 0);
}


SLIB_INLINE sl_char8* String8::getData() const
{
	return m_container->sz;
}

SLIB_INLINE sl_char16* String16::getData() const
{
	return m_container->sz;
}


SLIB_INLINE sl_size String8::getLength() const
{
	return m_container->len;
}

SLIB_INLINE sl_size String16::getLength() const
{
	return m_container->len;
}


SLIB_NAMESPACE_END

#endif
