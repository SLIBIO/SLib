#ifndef CHECKHEADER_SLIB_CORE_STRING
#define CHECKHEADER_SLIB_CORE_STRING

#include "definition.h"

#include "charset.h"
#include "memory.h"
#include "list.h"
#include "map.h"
#include "queue.h"

SLIB_NAMESPACE_BEGIN

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

struct _String_Const
{
	StringContainer* container;
	sl_int32 lock;
};

extern const _String_Const _String_Null;
extern const _String_Const _String_Empty;

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

class String;
typedef Atomic<String> AtomicString;
class String16;
typedef Atomic<String16> AtomicString16;
class StringData;
class Variant;

/** auto-referencing object **/
class SLIB_EXPORT String
{
private:
	StringContainer* m_container;
	
private:
	constexpr String(StringContainer* container) : m_container(container) {}
	
public:
	constexpr String() : m_container(sl_null) {}
	
	constexpr String(sl_null_t) : m_container(sl_null) {}
	
	String(String&& src);
	
	String(AtomicString&& _src);
	
	String(const String& src);
	
	String(const AtomicString& src);
	
	String(const String16& src);
	
	String(const AtomicString16& src);

	~String();
	
public:
	// From a single charactor
	String(sl_char8 ch, sl_size nRepeatCount);
	
	// From an utf-8 string
	String(const sl_char8* strUtf8);
	
	// From an utf-8 string
	String(const sl_char8* strUtf8, sl_reg length);
	
	// From an utf-16 string
	String(const sl_char16* strUtf16);
	
	// From an utf-16 string
	String(const sl_char16* strUtf16, sl_reg length);
	
	// From an utf-32 string
	String(const sl_char32* strUtf32);
	
	// From an utf-32 string
	String(const sl_char32* strUtf32, sl_reg length);

public:
	// create a string of 'len' charactors
	static String allocate(sl_size len);
	
	static String fromStatic(const sl_char8* sz8, sl_reg len = -1);

	static String fromUtf8(const void* utf8, sl_reg len = -1);
	
	static String fromUtf8(const Memory& mem);
	
	static String fromUtf16(const sl_char16* utf16, sl_reg len = -1);
	
	static String fromUtf32(const sl_char32* utf32, sl_reg len = -1);

	static String fromUtf16BE(const void* utf16, sl_reg len = -1);

	static String fromUtf16BE(const Memory& mem);
	
	static String fromUtf16LE(const void* utf16, sl_reg len = -1);

	static String fromUtf16LE(const Memory& mem);

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String fromUtf(const void* buf, sl_size len);

	static String fromUtf(const Memory& mem);

public:
	// Returns a null string
	static const String& null();
	
	// Returns an empty string
	static const String& getEmpty();
	
	const String& getNotNull() const;
	
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
	String& operator=(sl_null_t);
	
	String& operator=(String&& other);
	
	String& operator=(AtomicString&& _other);

	String& operator=(const String& other);
	
	String& operator=(const AtomicString& other);
	
	String& operator=(const String16& other);
	
	String& operator=(const AtomicString16& other);
	
	String& operator=(const sl_char8* utf8);
	
	String& operator=(const sl_char16* utf16);
	
	String& operator=(const sl_char32* utf32);

public:
	String operator+(const String& other) const;
	
	String& operator+=(const String& other);
	
	String operator+(const String16& other) const;
	
	String& operator+=(const String16& other);
	
	String operator+(const AtomicString& _other) const;
	
	String& operator+=(const AtomicString& other);
	
	String operator+(const AtomicString16& other) const;
	
	String& operator+=(const AtomicString16& other);
	
	String operator+(const sl_char8* utf8) const;
	
	String& operator+=(const sl_char8* utf8);
	
	String operator+(const sl_char16* utf16) const;
	
	String& operator+=(const sl_char16* utf16);

	String operator+(const sl_char32* utf32) const;
	
	String& operator+=(const sl_char32* utf32);

	friend String operator+(const sl_char8* utf8, const String& second);
	
	friend String operator+(const sl_char16* utf16, const String& second);
	
	friend String operator+(const sl_char32* utf32, const String& second);

public:
	String operator+(sl_int32 number) const;
	
	String& operator+=(sl_int32 number);
	
	friend String operator+(sl_int32 number, const String& other);
	
	String operator+(sl_uint32 number) const;
	
	String& operator+=(sl_uint32 number);

	friend String operator+(sl_uint32 number, const String& other);
	
	String operator+(sl_int64 number) const;
	
	String& operator+=(sl_int64 number);

	friend String operator+(sl_int64 number, const String& other);
	
	String operator+(sl_uint64 number) const;
	
	String& operator+=(sl_uint64 number);
	
	friend String operator+(sl_uint64 number, const String& other);
	
	String operator+(float number) const;
	
	String& operator+=(float number);

	friend String operator+(float number, const String& other);
	
	String operator+(double number) const;
	
	String& operator+=(double number);

	friend String operator+(double number, const String& other);
	
	String operator+(sl_bool value) const;
	
	String& operator+=(sl_bool value);
	
	friend String operator+(sl_bool value, const String& other);

public:
	sl_bool equals(const String& other) const;
	
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const AtomicString& other) const;
	
	sl_bool equals(const AtomicString16& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String& other) const;
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const AtomicString& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String& other, sl_size len) const;
	
	
	sl_bool equalsIgnoreCase(const String& other) const;
	
	sl_int32 compareIgnoreCase(const String& other) const;

public:
	sl_bool operator==(const String& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;

	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const String& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const String& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const String& second);
	
	
	sl_bool operator!=(const String& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;

	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const String& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const String& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const String& second);
	
	
	sl_bool operator>=(const String& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const String& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const String& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const String& second);
	
	
	sl_bool operator<=(const String& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const String& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const String& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const String& second);

	
	sl_bool operator>(const String& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const String& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const String& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const String& second);
	
	
	sl_bool operator<(const String& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString& other) const;
	
	sl_bool operator<(const AtomicString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const String& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const String& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const String& second);
	
public:
	String duplicate() const;

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

	String substring(sl_reg start, sl_reg end = -1) const;
	
	String left(sl_reg len) const;
	
	String right(sl_reg len) const;
	
	String mid(sl_reg start, sl_reg len) const;

	sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
	
	sl_reg indexOf(const String& str, sl_reg start = 0) const;
	
	sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String& str, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String& str) const;
	
	sl_bool startsWith(const sl_char8* str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String& str) const;

	sl_bool endsWith(const sl_char8* str) const;
	
	sl_bool contains(sl_char8 ch) const;
	
	sl_bool contains(const String& str) const;
	
	sl_bool contains(const sl_char8* str) const;

	void makeUpper();
	
	void makeLower();
	
	static String toUpper(const sl_char8* sz, sl_reg len = -1);
	
	static String toLower(const sl_char8* sz, sl_reg len = -1);
	
	String toUpper() const;
	
	String toLower() const;
	
	String replaceAll(const String& pattern, const String& replacement) const;
	
	String replaceAll(const String& pattern, const sl_char8* replacement) const;
	
	String replaceAll(const sl_char8* pattern, const String& replacement) const;
	
	String replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
	
	String trim() const;
	
	String trimLeft() const;
	
	String trimRight() const;
	
	List<String> split(const String& pattern) const;
	
	List<String> split(const sl_char8* pattern) const;
	
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
	static String fromInt32(sl_int32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String fromUint32(sl_uint32 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String fromInt64(sl_int64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String fromUint64(sl_uint64 value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String fromInt(sl_reg value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String fromSize(sl_size value, sl_uint32 radix = 10, sl_uint32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
	
	static String fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_uint32 minWidthIntegral = 1);
	
	static String fromPointerValue(const void* pointer);
	
	static String fromBoolean(sl_bool value);
	
	static String makeHexString(const void* data, sl_size size);
	
	static String makeHexString(const Memory& mem);
	
	
	static String formatBy(const sl_char8* szFormat, const Variant* params, sl_size nParams);
	
	static String formatBy(const String& strFormat, const Variant* params, sl_size nParams);
	
	static String format(const sl_char8* szFormat);
	
	static String format(const String& strFormat);
	
	template <class... ARGS>
	static String format(const sl_char8* szFormat, ARGS&&... args);
	
	template <class... ARGS>
	static String format(const String& strFormat, ARGS&&... args);
	
	String argBy(const Variant* params, sl_size nParams) const;
	
	template <class... ARGS>
	String arg(ARGS&&... args) const;
	
private:
	// Allocates memory required for a string for the specified length
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
	
	
	// Check two unicode string is Equal
	static sl_bool _equals8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
	
	static sl_bool _equals16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
	
	static sl_bool _equals32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);

	
	// Compare two unicode string
	static sl_int32 _compare8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2);
	
	static sl_int32 _compare16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2);
	
	static sl_int32 _compare32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2);
	
public:
	friend class Atomic<String>;
	
};

/** auto-referencing object **/
class SLIB_EXPORT String16
{
private:
	StringContainer16* m_container;
	
private:
	constexpr String16(StringContainer16* container) : m_container(container) {}

public:
	constexpr String16() : m_container(sl_null) {}
	
	constexpr String16(sl_null_t) : m_container(sl_null) {}
	
	String16(String16&& src);
	
	String16(AtomicString16&& _src);
	
	String16(const String16& src);
	
	String16(const AtomicString16& src);

	String16(const String& src);
	
	String16(const AtomicString& src);

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
	
	String16& operator=(const String& other);
	
	String16& operator=(const AtomicString& other);
	
	String16& operator=(const sl_char8* utf8);
	
	String16& operator=(const sl_char16* utf16);
	
	String16& operator=(const sl_char32* utf32);
	
public:
	String16 operator+(const String16& other) const;
	
	String16& operator+=(const String16& other);

	String16 operator+(const String& other) const;
	
	String16& operator+=(const String& other);

	String16 operator+(const AtomicString16& _other) const;
	
	String16& operator+=(const AtomicString16& other);

	String16 operator+(const AtomicString& other) const;
	
	String16& operator+=(const AtomicString& other);

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
	
	sl_bool equals(const String& other) const;
	
	sl_bool equals(const AtomicString16& other) const;
	
	sl_bool equals(const AtomicString& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const String& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const AtomicString& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String16& other, sl_size len) const;

	
	sl_bool equalsIgnoreCase(const String16& other) const;
	
	sl_int32 compareIgnoreCase(const String16& other) const;

public:
	sl_bool operator==(const String& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator!=(const String& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator>=(const String& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator<=(const String& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator>(const String& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const String16& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const String16& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const String16& second);
	
	
	sl_bool operator<(const String& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString& other) const;
	
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
class SLIB_EXPORT Atomic<String>
{
private:
	StringContainer* volatile m_container;
	SpinLock m_lock;
	
public:
	constexpr Atomic() : m_container(sl_null) {}
	
	constexpr Atomic(sl_null_t) : m_container(sl_null) {}
	
	Atomic(AtomicString&& src);
	
	Atomic(String&& src);
	
	Atomic(const String& src);
	
	Atomic(const AtomicString& src);
	
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
	static const AtomicString& null();
	
	static const AtomicString& getEmpty();
	
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
	AtomicString& operator=(sl_null_t);
	
	AtomicString& operator=(AtomicString&& other);
	
	AtomicString& operator=(String&& other);
	
	AtomicString& operator=(const String& other);
	
	AtomicString& operator=(const AtomicString& other);
	
	AtomicString& operator=(const String16& other);
	
	AtomicString& operator=(const AtomicString16& _other);
	
	AtomicString& operator=(const sl_char8* utf8);
	
	AtomicString& operator=(const sl_char16* utf16);
	
	AtomicString& operator=(const sl_char32* utf32);
	
public:
	String operator+(const String& other) const;
	
	AtomicString& operator+=(const String& other);
	
	String operator+(const String16& other) const;
	
	AtomicString& operator+=(const String16& other);
	
	String operator+(const AtomicString& other) const;
	
	AtomicString& operator+=(const AtomicString& other);
	
	String operator+(const AtomicString16& other) const;
	
	AtomicString& operator+=(const AtomicString16& other);
	
	String operator+(const sl_char8* utf8) const;
	
	AtomicString& operator+=(const sl_char8* utf8);

	String operator+(const sl_char16* utf16) const;
	
	AtomicString& operator+=(const sl_char16* utf16);

	String operator+(const sl_char32* utf32) const;
	
	AtomicString& operator+=(const sl_char32* utf32);

	friend String operator+(const sl_char8* utf8, const AtomicString& second);
	
	friend String operator+(const sl_char16* utf16, const AtomicString& second);
	
	friend String operator+(const sl_char32* utf32, const AtomicString& second);
	
public:
	String operator+(sl_int32 number) const;
	
	AtomicString& operator+=(sl_int32 number);
	
	friend String operator+(sl_int32 number, const AtomicString& other);
	
	String operator+(sl_uint32 number) const;
	
	AtomicString& operator+=(sl_uint32 number);
	
	friend String operator+(sl_uint32 number, const AtomicString& other);
	
	String operator+(sl_int64 number) const;
	
	AtomicString& operator+=(sl_int64 number);
	
	friend String operator+(sl_int64 number, const AtomicString& other);
	
	String operator+(sl_uint64 number) const;
	
	AtomicString& operator+=(sl_uint64 number);
	
	friend String operator+(sl_uint64 number, const AtomicString& other);
	
	String operator+(float number) const;
	
	AtomicString& operator+=(float number);
	
	friend String operator+(float number, const AtomicString& other);
	
	String operator+(double number) const;
	
	AtomicString& operator+=(double number);
	
	friend String operator+(double number, const AtomicString& other);
	
	String operator+(sl_bool value) const;
	
	AtomicString& operator+=(sl_bool value);
	
	friend String operator+(sl_bool value, const AtomicString& other);
	
public:
	sl_bool equals(const String& other) const;
	
	sl_bool equals(const String16& other) const;
	
	sl_bool equals(const AtomicString& other) const;
	
	sl_bool equals(const AtomicString16& other) const;

	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String& other) const;
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const AtomicString& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String& other, sl_size len) const;
	
	
	sl_bool equalsIgnoreCase(const String& other) const;
	
	sl_int32 compareIgnoreCase(const String& other) const;
	
public:
	sl_bool operator==(const String& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const AtomicString& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const AtomicString& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const AtomicString& second);
	
	
	sl_bool operator!=(const String& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const AtomicString& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const AtomicString& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const AtomicString& second);
	
	
	sl_bool operator>=(const String& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const AtomicString& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const AtomicString& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const AtomicString& second);
	
	
	sl_bool operator<=(const String& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const AtomicString& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const AtomicString& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const AtomicString& second);
	
	
	sl_bool operator>(const String& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const AtomicString& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const AtomicString& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const AtomicString& second);
	
	
	sl_bool operator<(const String& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString& other) const;
	
	sl_bool operator<(const AtomicString16& other) const;
	
	sl_bool operator<(const sl_char8* utf8) const;
	
	sl_bool operator<(const sl_char16* utf16) const;
	
	sl_bool operator<(const sl_char32* utf32) const;
	
	friend sl_bool operator<(const sl_char8* utf8, const AtomicString& second);
	
	friend sl_bool operator<(const sl_char16* utf16, const AtomicString& second);
	
	friend sl_bool operator<(const sl_char32* utf32, const AtomicString& second);
	
public:
	String duplicate() const;
	
	Memory toMemory() const;
	
	sl_size getUtf16(sl_char16* utf16, sl_size len) const;
	
	sl_bool getUtf16(StringData& output) const;
	
	// contains null character at last
	Memory toUtf16() const;
	
	sl_size getUtf32(sl_char32* utf32, sl_size len) const;
	
	sl_bool getUtf32(StringData& output) const;
	
	// contains null character at last
	Memory toUtf32() const;
	
	String substring(sl_reg start, sl_reg end = -1) const;
	
	String left(sl_reg len) const;
	
	String right(sl_reg len) const;
	
	String mid(sl_reg start, sl_reg len) const;
	
	sl_reg indexOf(sl_char8 ch, sl_reg start = 0) const;
	
	sl_reg indexOf(const String& str, sl_reg start = 0) const;
	
	sl_reg indexOf(const sl_char8* str, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(sl_char8 ch, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const String& str, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const sl_char8* str, sl_reg start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String& str) const;
	
	sl_bool startsWith(const sl_char8* str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String& str) const;
	
	sl_bool endsWith(const sl_char8* str) const;

	sl_bool constains(sl_char8 ch) const;

	sl_bool contains(const String& str) const;
	
	sl_bool contains(const sl_char8* str) const;

	void makeUpper();
	
	void makeLower();
	
	String toUpper() const;
	
	String toLower() const;
	
	String replaceAll(const String& pattern, const String& replacement) const;

	String replaceAll(const String& pattern, const sl_char8* replacement) const;
	
	String replaceAll(const sl_char8* pattern, const String& replacement) const;
	
	String replaceAll(const sl_char8* pattern, const sl_char8* replacement) const;
	
	String trim() const;
	
	String trimLeft() const;
	
	String trimRight() const;
	
	List<String> split(const String& pattern) const;
	
	List<String> split(const sl_char8* pattern) const;
	
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
	
	
	String argBy(const Variant* params, sl_size nParams) const;
	
	template <class... ARGS>
	String arg(ARGS&&... args) const;
	
private:
	StringContainer* _retainContainer() const;
	
	void _replaceContainer(StringContainer* other);

	friend class String;
};

/** auto-referencing object **/
template <>
class SLIB_EXPORT Atomic<String16>
{
private:
	StringContainer16* volatile m_container;
	SpinLock m_lock;
	
public:
	constexpr Atomic() : m_container(sl_null) {}
	
	constexpr Atomic(sl_null_t) : m_container(sl_null) {}
	
	Atomic(AtomicString16&& src);
	
	Atomic(String16&& src);
	
	Atomic(const String16& src);
	
	Atomic(const AtomicString16& src);
	
	Atomic(const String& src);
	
	Atomic(const AtomicString& src);
	
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
	
	AtomicString16& operator=(const String& other);
	
	AtomicString16& operator=(const AtomicString& _other);
	
	AtomicString16& operator=(const sl_char8* utf8);
	
	AtomicString16& operator=(const sl_char16* utf16);
	
	AtomicString16& operator=(const sl_char32* utf32);
    
public:
	String16 operator+(const String16& other) const;
	
	AtomicString16& operator+=(const AtomicString16& other);

	String16 operator+(const String& other) const;
	
	AtomicString16& operator+=(const AtomicString& other);

	String16 operator+(const AtomicString16& other) const;
	
	AtomicString16& operator+=(const String16& other);

	String16 operator+(const AtomicString& other) const;
	
	AtomicString16& operator+=(const String& other);

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
	
	sl_bool equals(const String& other) const;
	
	sl_bool equals(const AtomicString16& other) const;
	
	sl_bool equals(const AtomicString& other) const;
	
	sl_bool equals(const sl_char8* utf8) const;
	
	sl_bool equals(const sl_char16* utf16) const;
	
	sl_bool equals(const sl_char32* utf32) const;
	
	
	sl_int32 compare(const String16& other) const;
	
	sl_int32 compare(const String& other) const;
	
	sl_int32 compare(const AtomicString16& other) const;
	
	sl_int32 compare(const AtomicString& other) const;
	
	sl_int32 compare(const sl_char8* utf8) const;
	
	sl_int32 compare(const sl_char16* utf16) const;
	
	sl_int32 compare(const sl_char32* utf32) const;
	
	sl_int32 compare(const String16& other, sl_size len) const;

	
	sl_bool equalsIgnoreCase(const String16& other) const;
	
	sl_int32 compareIgnoreCase(const String16& other) const;
	
public:
	sl_bool operator==(const String& other) const;
	
	sl_bool operator==(const String16& other) const;
	
	sl_bool operator==(const AtomicString& other) const;
	
	sl_bool operator==(const AtomicString16& other) const;
	
	sl_bool operator==(const sl_char8* utf8) const;
	
	sl_bool operator==(const sl_char16* utf16) const;
	
	sl_bool operator==(const sl_char32* utf32) const;
	
	friend sl_bool operator==(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator==(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator==(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator!=(const String& other) const;
	
	sl_bool operator!=(const String16& other) const;
	
	sl_bool operator!=(const AtomicString& other) const;
	
	sl_bool operator!=(const AtomicString16& other) const;
	
	sl_bool operator!=(const sl_char8* utf8) const;
	
	sl_bool operator!=(const sl_char16* utf16) const;
	
	sl_bool operator!=(const sl_char32* utf32) const;
	
	friend sl_bool operator!=(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator!=(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator!=(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator>=(const String& other) const;
	
	sl_bool operator>=(const String16& other) const;
	
	sl_bool operator>=(const AtomicString& other) const;
	
	sl_bool operator>=(const AtomicString16& other) const;
	
	sl_bool operator>=(const sl_char8* utf8) const;
	
	sl_bool operator>=(const sl_char16* utf16) const;
	
	sl_bool operator>=(const sl_char32* utf32) const;
	
	friend sl_bool operator>=(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator>=(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator>=(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator<=(const String& other) const;
	
	sl_bool operator<=(const String16& other) const;
	
	sl_bool operator<=(const AtomicString& other) const;
	
	sl_bool operator<=(const AtomicString16& other) const;
	
	sl_bool operator<=(const sl_char8* utf8) const;
	
	sl_bool operator<=(const sl_char16* utf16) const;
	
	sl_bool operator<=(const sl_char32* utf32) const;
	
	friend sl_bool operator<=(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator<=(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator<=(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator>(const String& other) const;
	
	sl_bool operator>(const String16& other) const;
	
	sl_bool operator>(const AtomicString& other) const;
	
	sl_bool operator>(const AtomicString16& other) const;
	
	sl_bool operator>(const sl_char8* utf8) const;
	
	sl_bool operator>(const sl_char16* utf16) const;
	
	sl_bool operator>(const sl_char32* utf32) const;
	
	friend sl_bool operator>(const sl_char8* utf8, const AtomicString16& second);
	
	friend sl_bool operator>(const sl_char16* utf16, const AtomicString16& second);
	
	friend sl_bool operator>(const sl_char32* utf32, const AtomicString16& second);
	
	
	sl_bool operator<(const String& other) const;
	
	sl_bool operator<(const String16& other) const;
	
	sl_bool operator<(const AtomicString& other) const;
	
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
struct StringTypeFromCharType<sl_char8> { typedef String Type; };

template <>
struct StringTypeFromCharType<sl_char16> { typedef String16 Type; };


template <>
class Compare<String>
{
public:
	int operator()(const String& a, const String& b) const;
};

template <>
class Compare<String16>
{
public:
	int operator()(const String16& a, const String16& b) const;
};

template <>
class Compare<AtomicString>
{
public:
	int operator()(const AtomicString& a, const AtomicString& b) const;
};

template <>
class Compare<AtomicString16>
{
public:
	int operator()(const AtomicString16& a, const AtomicString16& b) const;
};


template <>
class Equals<String>
{
public:
	sl_bool operator()(const String& a, const String& b) const;
};

template <>
class Equals<String16>
{
public:
	sl_bool operator()(const String16& a, const String16& b) const;
};

template <>
class Equals<AtomicString>
{
public:
	sl_bool operator()(const AtomicString& a, const AtomicString& b) const;
};

template <>
class Equals<AtomicString16>
{
public:
	sl_bool operator()(const AtomicString16& a, const AtomicString16& b) const;
};


template <>
class Hash<String>
{
public:
	sl_uint32 operator()(const String& a) const;
};

template <>
class Hash<String16>
{
public:
	sl_uint32 operator()(const String16& a) const;
};

template <>
class Hash<AtomicString>
{
public:
	sl_uint32 operator()(const AtomicString& a) const;
};

template <>
class Hash<AtomicString16>
{
public:
	sl_uint32 operator()(const AtomicString16& a) const;
};


class CompareIgnoreCaseString
{
public:
	int operator()(const String& a, const String& b) const;
};

class CompareIgnoreCaseString16
{
public:
	int operator()(const String16& a, const String16& b) const;
};

typedef CompareIgnoreCaseString CompareIgnoreCaseString;


class EqualsIgnoreCaseString
{
public:
	sl_bool operator()(const String& a, const String& b) const;
};

class EqualsIgnoreCaseString16
{
public:
	sl_bool operator()(const String16& a, const String16& b) const;
};

typedef EqualsIgnoreCaseString EqualsIgnoreCaseString;


class HashIgnoreCaseString
{
public:
	sl_uint32 operator()(const String& v) const;
	
};

class HashIgnoreCaseString16
{
public:
	sl_uint32 operator()(const String16& v) const;
	
};

typedef HashIgnoreCaseString HashIgnoreCaseString;

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
	String str8;
	String16 str16;
};

class SLIB_EXPORT StringBuffer : public Object
{
public:
	StringBuffer();

public:
	sl_size getLength() const;
	
	sl_bool add(const String& str);
	
	sl_bool add(const StringData& str);
	
	sl_bool addStatic(const sl_char8* buf, sl_size length);
	
	void link(StringBuffer& buf);
	
	void clear();
	
	String merge() const;
	
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

typedef StringBuffer StringBuffer;



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


#define SLIB_STATIC_STRING(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING(name, str) \
	static sl_char8 _static_string_buf_##name[] = str; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::_String_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_##name));

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

#define SLIB_STATIC_STRING_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::StringContainer* _static_string_##name = &_static_string_container_##name; \
	static slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_##name));

#define SLIB_STATIC_ATOMIC_STRING_BY_ARRAY(name, ...) \
	static sl_char8 _static_string_buf_##name[] = {__VA_ARGS__, 0}; \
	static slib::StringContainer _static_string_container_##name = {_static_string_buf_##name, sizeof(_static_string_buf_##name)-1, 0, -1}; \
	static slib::_String_Const _static_string_##name = {&_static_string_container_##name, 0}; \
	static slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_##name));

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

#define SLIB_STATIC_STRING_NULL(name) \
	static slib::_String_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::String& name = *(reinterpret_cast<slib::String*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING_NULL(name) \
	static slib::_String_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::AtomicString& name = *(reinterpret_cast<slib::AtomicString*>(&_static_string_null_##name));

#define SLIB_STATIC_STRING16_NULL(name) \
	static slib::_String16_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::String16& name = *(reinterpret_cast<slib::String16*>(&_static_string_null_##name));

#define SLIB_STATIC_ATOMIC_STRING16_NULL(name) \
	static slib::_String16_Const _static_string_null_##name = {sl_null, 0}; \
	static slib::AtomicString16& name = *(reinterpret_cast<slib::AtomicString16*>(&_static_string_null_##name));


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

SLIB_INLINE String::String(String&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE String16::String16(String16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString::Atomic(AtomicString&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString16::Atomic(AtomicString16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}


SLIB_INLINE String::String(AtomicString&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE String16::String16(AtomicString16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString::Atomic(String&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}

SLIB_INLINE AtomicString16::Atomic(String16&& src)
{
	m_container = src.m_container;
	src.m_container = sl_null;
}


SLIB_INLINE const String& String::null()
{
	return *(reinterpret_cast<String const*>(&_String_Null));
}

SLIB_INLINE const String16& String16::null()
{
	return *(reinterpret_cast<String16 const*>(&_String16_Null));
}

SLIB_INLINE const AtomicString& AtomicString::null()
{
	return *(reinterpret_cast<AtomicString const*>(&_String_Null));
}

SLIB_INLINE const AtomicString16& AtomicString16::null()
{
	return *(reinterpret_cast<AtomicString16 const*>(&_String16_Null));
}


SLIB_INLINE const String& String::getEmpty()
{
	return *(reinterpret_cast<String const*>(&_String_Empty));
}

SLIB_INLINE const String16& String16::getEmpty()
{
	return *(reinterpret_cast<String16 const*>(&_String16_Empty));
}

SLIB_INLINE const AtomicString& AtomicString::getEmpty()
{
	return *(reinterpret_cast<AtomicString const*>(&_String_Empty));
}

SLIB_INLINE const AtomicString16& AtomicString16::getEmpty()
{
	return *(reinterpret_cast<AtomicString16 const*>(&_String16_Empty));
}


SLIB_INLINE const String& String::getNotNull() const
{
	if (!m_container) {
		return *(reinterpret_cast<String const*>(&_String_Empty));
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


SLIB_INLINE sl_bool String::isNull() const
{
	return m_container == sl_null;
}

SLIB_INLINE sl_bool String16::isNull() const
{
	return m_container == sl_null;
}

SLIB_INLINE sl_bool AtomicString::isNull() const
{
	return m_container == sl_null;
}

SLIB_INLINE sl_bool AtomicString16::isNull() const
{
	return m_container == sl_null;
}


SLIB_INLINE sl_bool String::isNotNull() const
{
	return m_container != sl_null;
}

SLIB_INLINE sl_bool String16::isNotNull() const
{
	return m_container != sl_null;
}

SLIB_INLINE sl_bool AtomicString::isNotNull() const
{
	return m_container != sl_null;
}

SLIB_INLINE sl_bool AtomicString16::isNotNull() const
{
	return m_container != sl_null;
}


SLIB_INLINE sl_bool String::isEmpty() const
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


SLIB_INLINE sl_bool String::isNotEmpty() const
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


SLIB_INLINE sl_char8* String::getData() const
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


SLIB_INLINE sl_size String::getLength() const
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
