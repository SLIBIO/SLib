#ifndef CHECKHEADER_SLIB_CORE_STRING
#define CHECKHEADER_SLIB_CORE_STRING

#include "definition.h"
#include "base.h"

#include "memory.h"
#include "algorithm.h"
#include "list.h"
#include "queue.h"

#define SLIB_STR_MAX_LEN 0x10000000

#define SLIB_CHAR_IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_ALPHA_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define SLIB_CHAR_IS_ALPHA_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define SLIB_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define SLIB_CHAR_IS_ALNUM(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_HEX(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

#define SLIB_CHAR_DIGIT_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : 10)
#define SLIB_CHAR_HEX_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : (((c) >= 'A' && (c) <= 'F') ? ((c) -  55) : ((c) >= 'a' && (c) <= 'f') ? ((c) -  87) : 16))

#define SLIB_STR_PREF(buf) ((sl_reg*)(((char*)buf)-16))
#define SLIB_STR_PHASH(buf) ((sl_uint32*)(((char*)buf)-8))
#define SLIB_STR_PLEN(buf) ((sl_uint32*)(((char*)buf)-4))
#define SLIB_STR_HEADER_LEN 16

#define SLIB_STATIC_STRING8(name, str) \
	SLIB_ALIGN(static char _static_string_buf_##name[], 8) = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" str; \
	static void* _static_string_container_##name = ((char*)_static_string_buf_##name) + SLIB_STR_HEADER_LEN; \
	slib::String8::_initStaticMemory(_static_string_container_##name); \
	slib::String8& name = *((slib::String8*)((void*)(&_static_string_container_##name)));

#if defined(SLIB_USE_UNICODE32)
#define SLIB_STATIC_STRING16(name, str) \
	SLIB_SAFE_STATIC(slib::String16, name, SLIB_UNICODE(str));
#else
#define SLIB_STATIC_STRING16(name, str) \
	SLIB_ALIGN(static wchar_t _static_string_buf_##name[], 8) = L"\0\0\0\0\0\0\0\0" L##str; \
	static void* _static_string_container_##name = ((char*)_static_string_buf_##name) + SLIB_STR_HEADER_LEN; \
	slib::String16::_initStaticMemory(_static_string_container_##name); \
	slib::String16& name = *((slib::String16*)((void*)(&_static_string_container_##name)));
#endif

#define SLIB_STATIC_STRING8_NULL(name) \
	static _String_Const _static_string_null_##name = {_String_Null.data, 0}; \
	slib::String8& name = *((slib::String8*)((void*)(&_static_string_null_##name)));

#define SLIB_STATIC_STRING16_NULL(name) \
	static _String_Const _static_string_null_##name = {_String_Null.data, 0}; \
	slib::String16& name = *((slib::String16*)((void*)(&_static_string_null_##name)));

#define SLIB_STATIC_STRING SLIB_STATIC_STRING8

#define SLIB_STATIC_STRING_NULL SLIB_STATIC_STRING8_NULL

SLIB_NAMESPACE_BEGIN

struct _String_Const
{
	void* data;
	sl_int32 lock;
};
extern const _String_Const _String_Null;
extern const _String_Const _String_Empty;

extern const char* _StringConv_radixPatternUpper;
extern const char* _StringConv_radixPatternLower;
extern const sl_uint8* _StringConv_radixInversePatternBig;
extern const sl_uint8* _StringConv_radixInversePatternSmall;

class String8;
class String16;
class SafeString8;
class SafeString16;

struct SLIB_EXPORT StringData
{
	union {
		const sl_char8* sz8;
		const sl_char16* sz16;
		const sl_char32* sz32;
	};
	sl_uint32 len;
	Memory mem;
};

struct SLIB_EXPORT StringContainer
{
	sl_char8 data[1];
};

/** auto-referencing object **/
class SLIB_EXPORT String8
{
private:
	// String(Information & Data) storage
	sl_char8* m_data;

public:
	SLIB_INLINE String8()
	{
		m_data = (sl_char8*)(_String_Null.data);
	}

	SLIB_INLINE String8(const String8& src)
	{
		sl_char8* data = src.m_data;
		_increaseReference(data);
		m_data = data;
	}

	SLIB_INLINE ~String8()
	{
		_decreaseReference(m_data);
	}
	
private:
	SLIB_INLINE String8(StringContainer* container)
	{
		m_data = (sl_char8*)((void*)container);
	}
	
public:
	// From 16bit-char String
	String8(const String16& src);
	
	// From Safe-String
	String8(const SafeString8& src);
	
	// From 16bit-char Safe-String
	String8(const SafeString16& src);

	// From a single charactor
	SLIB_INLINE String8(sl_char8 ch, sl_uint32 nRepeatCount = 1)
	{
		m_data = _create(ch, nRepeatCount);
	}
	
	// From an utf-8 string
	SLIB_INLINE String8(const sl_char8* strUtf8)
	{
		m_data = _create(strUtf8);
	}
	
	// From an utf-8 string
	SLIB_INLINE String8(const sl_char8* strUtf8, sl_int32 length)
	{
		m_data = _create(strUtf8, length);
	}
	
	// From an utf-16 string
	SLIB_INLINE String8(const sl_char16* strUtf16)
	{
		m_data = _create(strUtf16);
	}
	
	// From an utf-16 string
	SLIB_INLINE String8(const sl_char16* strUtf16, sl_int32 length)
	{
		m_data = _create(strUtf16, length);
	}
	
	// From an utf-32 string
	SLIB_INLINE String8(const sl_char32* strUtf32)
	{
		m_data = _create(strUtf32);
	}
	
	// From an utf-32 string
	SLIB_INLINE String8(const sl_char32* strUtf32, sl_int32 length)
	{
		m_data = _create(strUtf32, length);
	}

public:
	// Returns a null string
	SLIB_INLINE static const String8& null()
	{
		return *((String8*)((void*)&_String_Null));
	}
	
	// Returns an empty string
	SLIB_INLINE static const String8& getEmpty()
	{
		return *((String8*)((void*)&_String_Empty));
	}
	
	// create a string of 'len' charactors
	SLIB_INLINE static String8 allocate(sl_uint32 len)
	{
		return (StringContainer*)(_alloc(len));
	}

	SLIB_INLINE static String8 fromUtf8(const void* utf8, sl_int32 len = -1)
	{
		return String8((const sl_char8*)utf8, len);
	}
	
	SLIB_INLINE static String8 fromUtf8(const Memory& mem)
	{
		return fromUtf8(mem.getBuf(), (sl_int32)(mem.getSize()));
	}
	
	SLIB_INLINE static String8 fromUtf16(const void* utf16, sl_int32 len = -1)
	{
		return String8((const sl_char16*)utf16, len);
	}
	
	SLIB_INLINE static String8 fromUtf16(const Memory& mem)
	{
		return fromUtf16(mem.getBuf(), (sl_int32)(mem.getSize()));
	}
	
	SLIB_INLINE static String8 fromUtf32(const void* utf32, sl_int32 len = -1)
	{
		return String8((const sl_char32*)utf32, len);
	}
	
	SLIB_INLINE static String8 fromUtf32(const Memory& mem)
	{
		return fromUtf32(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String8 fromUtf16BE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String8 fromUtf16BE(const Memory& mem)
	{
		return fromUtf16BE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String8 fromUtf16LE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String8 fromUtf16LE(const Memory& mem)
	{
		return fromUtf16LE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String8 fromUtf(const void* buf, sl_size len);

	SLIB_INLINE static String8 fromUtf(const Memory& mem)
	{
		return fromUtf(mem.getBuf(), mem.getSize());
	}

public:
	// Length of String
	SLIB_INLINE sl_uint32 getLength() const
	{
		return *SLIB_STR_PLEN(m_data);
	}
	
	// Length of String
	SLIB_INLINE sl_uint32 length() const
	{
		return *SLIB_STR_PLEN(m_data);
	}
	
	SLIB_INLINE void setLength(sl_uint32 len)
	{
		*SLIB_STR_PLEN(m_data) = len;
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_data == (sl_char8*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_data != (sl_char8*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		return (getLength() == 0);
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return (getLength() != 0);
	}
	
	SLIB_INLINE sl_char8 getAt(sl_int32 index) const
	{
		return m_data[index];
	}
	
	SLIB_INLINE void setAt(sl_int32 index, sl_char8 ch)
	{
		m_data[index] = ch;
	}
	
	SLIB_INLINE sl_char8* getBuf() const
	{
		return m_data;
	}
	
	SLIB_INLINE sl_char8* getData() const
	{
		return m_data;
	}
	
	SLIB_INLINE sl_char8* data() const
	{
		return m_data;
	}

public:
	SLIB_INLINE void setNull()
	{
		if (m_data != (sl_char8*)(_String_Null.data)) {
			_replaceContainer((sl_char8*)(_String_Null.data));
		}
	}
	
	SLIB_INLINE void setEmpty()
	{
		if (m_data != (sl_char8*)(_String_Empty.data)) {
			_replaceContainer((sl_char8*)(_String_Empty.data));
		}
	}
	
	SLIB_INLINE const String8& getNotNull() const
	{
		if (m_data == (sl_char8*)(_String_Null.data)) {
			return getEmpty();
		}
		return *this;
	}
	
public:
	SLIB_INLINE String8& operator=(const String8& other)
	{
		sl_char8* data = other.m_data;
		if (m_data != data) {
			_increaseReference(data);
			_replaceContainer(data);
		}
		return *this;
	}
	
	String8& operator=(const String16& other);
	
	SLIB_INLINE String8& operator=(const SafeString8& other)
	{
		return *this = String8(other);
	}
	
	String8& operator=(const SafeString16& other);
	
	SLIB_INLINE String8& operator=(const sl_char8* utf8)
	{
		_replaceContainer(_create(utf8));
		return *this;
	}
	
	SLIB_INLINE String8& operator=(const sl_char16* utf16)
	{
		_replaceContainer(_create(utf16));
		return *this;
	}
	
	SLIB_INLINE String8& operator=(const sl_char32* utf32)
	{
		_replaceContainer(_create(utf32));
		return *this;
	}

public:
	SLIB_INLINE sl_char8 operator[](sl_int32 index) const
	{
		return m_data[index];
	}
	
	SLIB_INLINE sl_char8& operator[](sl_int32 index)
	{
		return m_data[index];
	}

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
	SLIB_INLINE String8 operator+(sl_int32 number) const
	{
		return *this + String8::fromInt32(number);
	}
	
	SLIB_INLINE String8& operator+=(sl_int32 number)
	{
		return (*this += String8::fromInt32(number));
	}
	
	SLIB_INLINE friend String8 operator+(sl_int32 number, const String8& other)
	{
		return String8::fromInt32(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_uint32 number) const
	{
		return *this + String8::fromUint32(number);
	}
	
	SLIB_INLINE String8& operator+=(sl_uint32 number)
	{
		return (*this += String8::fromUint32(number));
	}

	SLIB_INLINE friend String8 operator+(sl_uint32 number, const String8& other)
	{
		return String8::fromUint32(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_int64 number) const
	{
		return *this + String8::fromInt64(number);
	}
	
	SLIB_INLINE String8& operator+=(sl_int64 number)
	{
		return (*this += String8::fromInt64(number));
	}

	SLIB_INLINE friend String8 operator+(sl_int64 number, const String8& other)
	{
		return String8::fromInt64(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_uint64 number) const
	{
		return *this + String8::fromUint64(number);
	}
	
	SLIB_INLINE String8& operator+=(sl_uint64 number)
	{
		return (*this += String8::fromUint64(number));
	}
	
	SLIB_INLINE friend String8 operator+(sl_uint64 number, const String8& other)
	{
		return String8::fromUint64(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(float number) const
	{
		return *this + String8::fromFloat(number);
	}
	
	SLIB_INLINE String8& operator+=(float number)
	{
		return (*this += String8::fromFloat(number));
	}

	SLIB_INLINE friend String8 operator+(float number, const String8& other)
	{
		return String8::fromFloat(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(double number) const
	{
		return *this + String8::fromDouble(number);
	}
	
	SLIB_INLINE String8& operator+=(double number)
	{
		return (*this += String8::fromDouble(number));
	}

	SLIB_INLINE friend String8 operator+(double number, const String8& other)
	{
		return String8::fromDouble(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_bool value) const
	{
		return *this + String8::fromBoolean(value);
	}
	
	SLIB_INLINE String8& operator+=(sl_bool value)
	{
		return (*this += String8::fromBoolean(value));
	}

	SLIB_INLINE friend String8 operator+(sl_bool value, const String8& other)
	{
		return String8::fromBoolean(value) + other;
	}

public:
	SLIB_INLINE sl_bool equals(const String8& other) const
	{
		if (m_data == other.m_data) {
			return sl_true;
		}
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const String16& _other) const
	{
		String8 other(_other);
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const SafeString8& _other) const
	{
		String8 other(_other);
		if (m_data == other.m_data) {
			return sl_true;
		}
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const SafeString16& _other) const
	{
		String8 other(_other);
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const sl_char8* utf8) const
	{
		if (m_data == utf8) {
			return sl_true;
		}
		return _equals8(data(), length(), utf8, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char16* utf16) const
	{
		return _equals16(data(), length(), utf16, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char32* utf32) const
	{
		return _equals32(data(), length(), utf32, -1);
	}

public:
	sl_int32 compare(const String8& other, sl_uint32 len) const;
	
	SLIB_INLINE sl_int32 compare(const String8& other) const
	{
		if (m_data == other.m_data) {
			return 0;
		}
		return _compare8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const String16& _other) const
	{
		String8 other(_other);
		return _compare8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString8& _other) const
	{
		String8 other(_other);
		if (m_data == other.m_data) {
			return 0;
		}
		return _compare8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString16& _other) const
	{
		String8 other(_other);
		return _compare8(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char8* utf8) const
	{
		if (m_data == utf8) {
			return 0;
		}
		return _compare8(data(), length(), utf8, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char16* utf16) const
	{
		return _compare16(data(), length(), utf16, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char32* utf32) const
	{
		return _compare32(data(), length(), utf32, -1);
	}

public:
	SLIB_INLINE sl_bool operator==(const String8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const String16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString16& other) const
	{
		return equals(other);
	}

	SLIB_INLINE sl_bool operator==(const sl_char8* utf8) const
	{
		return equals(utf8);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char16* utf16) const
	{
		return equals(utf16);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char32* utf32) const
	{
		return equals(utf32);
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char8* utf8, const String8& second)
	{
		return second == utf8;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char16* utf16, const String8& second)
	{
		return second == utf16;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char32* utf32, const String8& second)
	{
		return second == utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const String8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const String16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString16& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE sl_bool operator!=(const sl_char8* utf8) const
	{
		return !(*this == utf8);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char16* utf16) const
	{
		return !(*this == utf16);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char32* utf32) const
	{
		return !(*this == utf32);
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char8* utf8, const String8& second)
	{
		return second != utf8;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char16* utf16, const String8& second)
	{
		return second != utf16;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char32* utf32, const String8& second)
	{
		return second != utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator>=(const String8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const String16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char8* utf8) const
	{
		return compare(utf8) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char16* utf16) const
	{
		return compare(utf16) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char32* utf32) const
	{
		return compare(utf32) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char8* utf8, const String8& second)
	{
		return second.compare(utf8) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char16* utf16, const String8& second)
	{
		return second.compare(utf16) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char32* utf32, const String8& second)
	{
		return second.compare(utf32) <= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<=(const String8& other) const
	{
		return compare(other) <= 0;
	}

	SLIB_INLINE sl_bool operator<=(const String16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString16& other) const
	{
		return compare(other) <= 0;
	}

	SLIB_INLINE sl_bool operator<=(const sl_char8* utf8) const
	{
		return compare(utf8) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char16* utf16) const
	{
		return compare(utf16) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char32* utf32) const
	{
		return compare(utf32) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char8* utf8, const String8& second)
	{
		return second.compare(utf8) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char16* utf16, const String8& second)
	{
		return second.compare(utf16) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char32* utf32, const String8& second)
	{
		return second.compare(utf32) >= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator>(const String8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const String16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char8* utf8) const
	{
		return compare(utf8) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char16* utf16) const
	{
		return compare(utf16) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char32* utf32) const
	{
		return compare(utf32) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char8* utf8, const String8& second)
	{
		return second.compare(utf8) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char16* utf16, const String8& second)
	{
		return second.compare(utf16) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char32* utf32, const String8& second)
	{
		return second.compare(utf32) < 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<(const String8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const String16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char8* utf8) const
	{
		return compare(utf8) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char16* utf16) const
	{
		return compare(utf16) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char32* utf32) const
	{
		return compare(utf32) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char8* utf8, const String8& second)
	{
		return second.compare(utf8) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char16* utf16, const String8& second)
	{
		return second.compare(utf16) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char32* utf32, const String8& second)
	{
		return second.compare(utf32) > 0;
	}

public:
	SLIB_INLINE String8 duplicate() const
	{
		return String8(m_data, length());
	}

	Memory toMemory() const;

	sl_uint32 getUtf16(sl_char16* utf16, sl_int32 len) const;

	StringData getUtf16() const;

	// contains null character at last
	Memory toUtf16() const;

	sl_uint32 getUtf32(sl_char32* utf32, sl_int32 len) const;

	StringData getUtf32() const;

	// contains null character at last
	Memory toUtf32() const;

	String8 substring(sl_int32 start, sl_int32 end = -1) const;
	
	SLIB_INLINE String8 left(sl_uint32 len) const
	{
		return substring(0, len);
	}
	
	SLIB_INLINE String8 right(sl_uint32 len) const
	{
		return substring(length() - len);
	}
	
	SLIB_INLINE String8 mid(sl_int32 start, sl_int32 len) const
	{
		return substring(start, start + len);
	}

	sl_int32 indexOf(sl_char8 ch, sl_int32 start = 0) const;
	
	sl_int32 indexOf(const String8& str, sl_int32 start = 0) const;
	
	sl_int32 lastIndexOf(sl_char8 ch, sl_int32 start = -1) const;
	
	sl_int32 lastIndexOf(const String8& str, sl_int32 start = -1) const;
	
	sl_bool startsWith(sl_char8 ch) const;
	
	sl_bool startsWith(const String8& str) const;
	
	sl_bool endsWith(sl_char8 ch) const;
	
	sl_bool endsWith(const String8& str) const;
	
	void makeUpper();
	
	void makeLower();
	
	String8 toLower() const;
	
	String8 toUpper() const;
	
	String8 replaceAll(const String8& pattern, const String8& replacement) const;
	
	String8 trim() const;
	
	String8 trimLeft() const;
	
	String8 trimRight() const;
	
	List<String8> split(const String8& pattern) const;
	
	sl_uint32 hashCode() const;
	
	String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	
	static String8 parseBackslashEscapes(const sl_char8* input, sl_int32 len, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	
	SLIB_INLINE static String8 parseBackslashEscapes(const String8& str, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null)
	{
		return parseBackslashEscapes(str.data(), str.length(), lengthParsed, flagError);
	}
	
public:
	// radix: 2 ~ 64,   flagUpperCase only works if radix <= 36 (0~9a~z)
	static String8 fromInt32(sl_int32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromUint32(sl_uint32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromInt64(sl_int64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String8 fromUint64(sl_uint64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	SLIB_INLINE static String8 fromInt(sl_reg value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromInt64(value, radix, minWidth, flagUpperCase);
#else
		return fromInt32(value, radix, minWidth, flagUpperCase);
#endif
	}
	
	SLIB_INLINE static String8 fromSize(sl_size value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64(value, radix, minWidth, flagUpperCase);
#else
		return fromUint32(value, radix, minWidth, flagUpperCase);
#endif
	}

	static String8 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);
	
	static String8 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);

	SLIB_INLINE static String8 fromPointerValue(const void* pointer)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64((sl_uint64)(pointer), 16, 16, sl_true);
#else
		return fromUint32((sl_uint32)(pointer), 16, 8, sl_true);
#endif
	}

	static String8 fromBoolean(sl_bool value);
	
	static String8 makeHexString(const void* data, sl_int32 size);
	
	SLIB_INLINE static String8 makeHexString(Memory mem)
	{
		return makeHexString(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}

	
	static sl_int32 parseInt32(sl_int32* _out, const char*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseInt32(sl_int32* _out, const sl_char16*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseInt32(sl_int32* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseUint32(sl_uint32* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseUint32(sl_uint32* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseUint32(sl_uint32* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseInt64(sl_int64* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseInt64(sl_int64* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseInt64(sl_int64* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseUint64(sl_uint64* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseUint64(sl_uint64* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseUint64(sl_uint64* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseFloat(float* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parseFloat(float* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	sl_bool parseFloat(float* _out) const;

	
	static sl_int32 parseDouble(double* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parseDouble(double* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	sl_bool parseDouble(double* _out) const;

	
	static sl_int32 parseHexString(void* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parseHexString(void* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	sl_bool parseHexString(void* _out) const;

	
	SLIB_INLINE sl_bool parseInt(sl_reg* _out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(_out, radix);
#else
		return parseInt32(_out, radix);
#endif
	}
	
	SLIB_INLINE sl_bool parseSize(sl_size* _out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(_out, radix);
#else
		return parseUint32(_out, radix);
#endif
	}

	SLIB_INLINE sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const
	{
		sl_int32 _out = def;
		parseInt32(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const
	{
		sl_uint32 _out = def;
		parseUint32(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const
	{
		sl_int64 _out = def;
		parseInt64(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const
	{
		sl_uint64 _out = def;
		parseUint64(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const
	{
		sl_reg _out = def;
		parseInt(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const
	{
		sl_size _out = def;
		parseSize(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE float parseFloat(float def = 0) const
	{
		float _out = def;
		parseFloat(&_out);
		return _out;
	}
	
	SLIB_INLINE double parseDouble(double def = 0) const
	{
		double _out = def;
		parseDouble(&_out);
		return _out;
	}
	
public:
	// utf8 conversion
	static sl_int32 utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer);
	
	static sl_int32 utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer);
	
	static sl_int32 utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer);
	
	static sl_int32 utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer);
	
private:
	SLIB_INLINE static void _increaseReference(sl_char8* data)
	{
		Base::interlockedIncrement(SLIB_STR_PREF(data));
	}

	SLIB_INLINE static void _decreaseReference(sl_char8* data)
	{
		sl_reg iRef = Base::interlockedDecrement(SLIB_STR_PREF(data));
		if (iRef == 0) {
			_destroy(data);
		}
	}
	
	SLIB_INLINE void _replaceContainer(sl_char8* data)
	{
		_decreaseReference(m_data);
		m_data = data;
	}
	
	
	// Deallocates memory allocated for this string
	static void _destroy(sl_char8* data);

	// Allocates memory required for a string for the specified length
	static sl_char8* _alloc(sl_uint32& length);
	
	
	static sl_char8* _create(sl_char8 ch, sl_uint32 nRepeatCount);
	
	static sl_char8* _create(const sl_char8* strUtf8, sl_int32 length = -1);
	
	static sl_char8* _create(const sl_char16* strUtf16, sl_int32 length = -1);
	
	static sl_char8* _create(const sl_char32* strUtf32, sl_int32 length = -1);
	
	
	static sl_char8* _merge8(const sl_char8* s1, sl_int32 len1, const sl_char8* s2, sl_int32 len2);
	
	static sl_char8* _merge16(const sl_char8* s1, sl_int32 len1, const sl_char16* s2, sl_int32 len2);
	
	static sl_char8* _merge16(const sl_char16* s1, sl_int32 len1, const sl_char8* s2, sl_int32 len2);
	
	static sl_char8* _merge32(const sl_char8* s1, sl_int32 len1, const sl_char32* s2, sl_int32 len2);
	
	static sl_char8* _merge32(const sl_char32* s1, sl_int32 len1, const sl_char8* s2, sl_int32 len2);
	
	
	// Check two unicode string is Equal
	static sl_bool _equals8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	
	static sl_bool _equals16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	
	static sl_bool _equals32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);

	
	// Compare two unicode string
	static sl_int32 _compare8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	
	static sl_int32 _compare16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	
	static sl_int32 _compare32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);
	
public:
	static void _initStaticMemory(void* mem);

	friend class SafeString8;
};

/** auto-referencing object **/
class SLIB_EXPORT String16
{
private:
	// String(Information & Data) storage
#if defined(SLIB_USE_UNICODE16)
	union {
		sl_char16* m_data;
		wchar_t* m_dataDebug;
	};
#else
	sl_char16* m_data;
#endif

public:
	SLIB_INLINE String16()
	{
		m_data = (sl_char16*)(_String_Null.data);
	}
	
	SLIB_INLINE String16(const String16& src)
	{
		sl_char16* data = src.m_data;
		_increaseReference(data);
		m_data = data;
	}

	SLIB_INLINE ~String16()
	{
		_decreaseReference(m_data);
	}
	
private:
	SLIB_INLINE String16(StringContainer* container)
	{
		m_data = (sl_char16*)((void*)container);
	}
    
public:
	// From 8bit-char String
	String16(const String8& src);

	// From 8bit-char Safe-String
	String16(const SafeString8& src);
	
	// From 16bit-char Safe-String
	String16(const SafeString16& src);
	
	// From a single charactor
	SLIB_INLINE String16(sl_char16 ch, sl_uint32 nRepeatCount = 1)
	{
		m_data = _create(ch, nRepeatCount);
	}
	
	// From an utf-8 string
	SLIB_INLINE String16(const sl_char8* strUtf8)
	{
		m_data = _create(strUtf8);
	}
	
	SLIB_INLINE String16(const sl_char8* strUtf8, sl_int32 length)
	{
		m_data = _create(strUtf8, length);
	}
	
	// From an utf-16 string
	SLIB_INLINE String16(const sl_char16* strUtf16)
	{
		m_data = _create(strUtf16);
	}
	
	SLIB_INLINE String16(const sl_char16* strUtf16, sl_int32 length)
	{
		m_data = _create(strUtf16, length);
	}
	
	// From an utf-32 string
	SLIB_INLINE String16(const sl_char32* strUtf32)
	{
		m_data = _create(strUtf32);
	}
	
	SLIB_INLINE String16(const sl_char32* strUtf32, sl_int32 length)
	{
		m_data = _create(strUtf32, length);
	}

public:
	// Returns a null string
	SLIB_INLINE static const String16& null()
	{
		return *((String16*)((void*)&_String_Null));
	}

    // Returns an empty string
	SLIB_INLINE static const String16& getEmpty()
	{
		return *((String16*)((void*)&_String_Empty));
	}
	
    // create a string of 'len' charactors
	SLIB_INLINE static String16 allocate(sl_uint32 len)
	{
		return (StringContainer*)(_alloc(len));
	}

	SLIB_INLINE static String16 fromUtf8(const void* utf8, sl_int32 len = -1)
	{
		return String16((const sl_char8*)utf8, len);
	}

	SLIB_INLINE static String16 fromUtf8(const Memory& mem)
	{
		return fromUtf8(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	SLIB_INLINE static String16 fromUtf16(const void* utf16, sl_int32 len = -1)
	{
		String16 str((const sl_char16*)utf16, len);
		return str;
	}

	SLIB_INLINE static String16 fromUtf16(const Memory& mem)
	{
		return fromUtf16(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	SLIB_INLINE static String16 fromUtf32(const void* utf32, sl_int32 len = -1)
	{
		String16 str((const sl_char32*)utf32, len);
		return str;
	}

	SLIB_INLINE static String16 fromUtf32(const Memory& mem)
	{
		return fromUtf32(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String16 fromUtf16BE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String16 fromUtf16BE(const Memory& mem)
	{
		return fromUtf16BE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String16 fromUtf16LE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String16 fromUtf16LE(const Memory& mem)
	{
		return fromUtf16LE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String16 fromUtf(const void* buf, sl_size len);

	SLIB_INLINE static String16 fromUtf(const Memory& mem)
	{
		return fromUtf(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

public:
	// Length of String
	SLIB_INLINE sl_uint32 getLength() const
	{
		return *SLIB_STR_PLEN(m_data);
	}
	
	SLIB_INLINE sl_uint32 length() const
	{
		return *SLIB_STR_PLEN(m_data);
	}
		
	SLIB_INLINE void setLength(sl_uint32 len)
	{
		*SLIB_STR_PLEN(m_data) = len;
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return m_data == (sl_char16*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_data != (sl_char16*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		return (getLength()==0);
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return (getLength() != 0);
	}
	
	SLIB_INLINE sl_char16 getAt(sl_int32 index) const
	{
		return m_data[index];
	}
	
	SLIB_INLINE void setAt(sl_int32 index, sl_char16 ch)
	{
		m_data[index] = ch;
	}
	
	SLIB_INLINE sl_char16* getBuf() const
	{
		return m_data;
	}
	
	SLIB_INLINE sl_char16* getData() const
	{
		return m_data;
	}
	
	SLIB_INLINE sl_char16* data() const
	{
		return m_data;
	}

public:
	SLIB_INLINE void setNull()
	{
		if (m_data != (sl_char16*)(_String_Null.data)) {
			_replaceContainer((sl_char16*)(_String_Null.data));
		}
	}
	
	SLIB_INLINE void setEmpty()
	{
		if (m_data != (sl_char16*)(_String_Empty.data)) {
			_replaceContainer((sl_char16*)(_String_Empty.data));
		}
	}
	
	SLIB_INLINE const String16& getNotNull() const
	{
		if (m_data == (sl_char16*)(_String_Null.data)) {
			return getEmpty();
		}
		return *this;
	}

public:
	SLIB_INLINE String16& operator=(const String16& other)
	{
		sl_char16* data = other.m_data;
		if (m_data != data) {
			_increaseReference(data);
			_replaceContainer(data);
		}
		return *this;
	}
	
	String16& operator=(const String8& other);
	
	SLIB_INLINE String16& operator=(const SafeString16& other)
    {
        return *this = String16(other);
    }
	
	String16& operator=(const SafeString8& other);
	
	SLIB_INLINE String16& operator=(const sl_char8* utf8)
    {
        _replaceContainer(_create(utf8));
        return *this;
    }
	
	SLIB_INLINE String16& operator=(const sl_char16* utf16)
    {
        _replaceContainer(_create(utf16));
        return *this;
    }
	
	SLIB_INLINE String16& operator=(const sl_char32* utf32)
    {
        _replaceContainer(_create(utf32));
        return *this;
    }
	
public:
	SLIB_INLINE sl_char16 operator[](sl_int32 index) const
	{
		return m_data[index];
	}
	
	SLIB_INLINE sl_char16& operator[](sl_int32 index)
	{
		return m_data[index];
	}
	
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
	SLIB_INLINE String16 operator+(sl_int32 number) const
	{
		return *this + String16::fromInt32(number);
	}
	
	SLIB_INLINE String16& operator+=(sl_int32 number)
	{
		return (*this += String16::fromInt32(number));
	}

	SLIB_INLINE friend String16 operator+(sl_int32 number, const String16& other)
	{
		return String16::fromInt32(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_uint32 number) const
	{
		return *this + String16::fromUint32(number);
	}
	
	SLIB_INLINE String16& operator+=(sl_uint32 number)
	{
		return (*this += String16::fromUint32(number));
	}

	SLIB_INLINE friend String16 operator+(sl_uint32 number, const String16& other)
	{
		return String16::fromUint32(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_int64 number) const
	{
		return *this + String16::fromInt64(number);
	}
	
	SLIB_INLINE String16& operator+=(sl_int64 number)
	{
		return (*this += String16::fromInt64(number));
	}

	SLIB_INLINE friend String16 operator+(sl_int64 number, const String16& other)
	{
		return String16::fromInt64(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_uint64 number) const
	{
		return *this + String16::fromUint64(number);
	}
	
	SLIB_INLINE String16& operator+=(sl_uint64 number)
	{
		return (*this += String16::fromUint64(number));
	}

	SLIB_INLINE friend String16 operator+(sl_uint64 number, const String16& other)
	{
		return String16::fromUint64(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(float number) const
	{
		return *this + String16::fromFloat(number);
	}
	
	SLIB_INLINE String16& operator+=(float number)
	{
		return (*this += String16::fromFloat(number));
	}
	
	SLIB_INLINE friend String16 operator+(float number, const String16& other)
	{
		return String16::fromFloat(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(double number) const
	{
		return *this + String16::fromDouble(number);
	}
	
	SLIB_INLINE String16& operator+=(double number)
	{
		return (*this += String16::fromDouble(number));
	}

	SLIB_INLINE friend String16 operator+(double number, const String16& other)
	{
		return String16::fromDouble(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_bool value) const
	{
		return *this + String16::fromBoolean(value);
	}
	
	SLIB_INLINE String16& operator+=(sl_bool value)
	{
		return (*this += String16::fromBoolean(value));
	}
	
	SLIB_INLINE friend String16 operator+(sl_bool value, const String16& other)
	{
		return String16::fromBoolean(value) + other;
	}
	
public:
	SLIB_INLINE sl_bool equals(const String16& other) const
	{
		if (m_data == other.m_data) {
			return sl_true;
		}
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals16(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const String8& _other) const
	{
		String16 other(_other);
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals16(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const SafeString16& _other) const
	{
		String16 other(_other);
		if (m_data == other.m_data) {
			return sl_true;
		}
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals16(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const SafeString8& _other) const
	{
		String16 other(_other);
		if (length() != other.length()) {
			return sl_false;
		}
		return _equals16(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const sl_char8* utf8) const
	{
		return _equals8(data(), length(), utf8, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char16* utf16) const
	{
		if (m_data == utf16) {
			return sl_true;
		}
		return _equals16(data(), length(), utf16, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char32* utf32) const
	{
		return _equals32(data(), length(), utf32, -1);
	}

public:
	sl_int32 compare(const String16& other, sl_uint32 len) const;
	
	SLIB_INLINE sl_int32 compare(const String16& other) const
	{
		if (m_data == other.m_data) {
			return 0;
		}
		return _compare16(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const String8& _other) const
	{
		String16 other(_other);
		return _compare16(m_data, length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString16& _other) const
	{
		String16 other(_other);
		if (m_data == other.m_data) {
			return 0;
		}
		return _compare16(m_data, length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString8& _other) const
	{
		String16 other(_other);
		return _compare16(m_data, length(), other.m_data, other.length());
	}

	SLIB_INLINE sl_int32 compare(const sl_char8* utf8) const
	{
		return _compare8(data(), length(), utf8, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char16* utf16) const
	{
		if (m_data == utf16) {
			return 0;
		}
		return _compare16(data(), length(), utf16, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char32* utf32) const
	{
		return _compare32(data(), length(), utf32, -1);
	}

public:
	SLIB_INLINE sl_bool operator==(const String16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const String8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char8* utf8) const
	{
		return equals(utf8);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char16* utf16) const
	{
		return equals(utf16);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char32* utf32) const
	{
		return equals(utf32);
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char8* utf8, const String16& second)
	{
		return second == utf8;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char16* utf16, const String16& second)
	{
		return second == utf16;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char32* utf32, const String16& second)
	{
		return second == utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const String16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const String8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char8* utf8) const
	{
		return !(*this == utf8);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char16* utf16) const
	{
		return !(*this == utf16);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char32* utf32) const
	{
		return !(*this == utf32);
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char8* utf8, const String16& second)
	{
		return second != utf8;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char16* utf16, const String16& second)
	{
		return second != utf16;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char32* utf32, const String16& second)
	{
		return second != utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator>=(const String16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const String8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char8* utf8) const
	{
		return compare(utf8) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char16* utf16) const
	{
		return compare(utf16) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char32* utf32) const
	{
		return compare(utf32) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char8* utf8, const String16& second)
	{
		return second.compare(utf8) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char16* utf16, const String16& second)
	{
		return second.compare(utf16) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char32* utf32, const String16& second)
	{
		return second.compare(utf32) <= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<=(const String16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const String8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char8* utf8) const
	{
		return compare(utf8) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char16* utf16) const
	{
		return compare(utf16) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char32* utf32) const
	{
		return compare(utf32) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char8* utf8, const String16& second)
	{
		return second.compare(utf8) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char16* utf16, const String16& second)
	{
		return second.compare(utf16) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char32* utf32, const String16& second)
	{
		return second.compare(utf32) >= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator>(const String16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const String8& other) const
	{
		return compare(other) > 0;
	}

	SLIB_INLINE sl_bool operator>(const SafeString16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char8* utf8) const
	{
		return compare(utf8) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char16* utf16) const
	{
		return compare(utf16) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char32* utf32) const
	{
		return compare(utf32) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char8* utf8, const String16& second)
	{
		return second.compare(utf8) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char16* utf16, const String16& second)
	{
		return second.compare(utf16) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char32* utf32, const String16& second)
	{
		return second.compare(utf32) < 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<(const String16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const String8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char8* utf8) const
	{
		return compare(utf8) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char16* utf16) const
	{
		return compare(utf16) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char32* utf32) const
	{
		return compare(utf32) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char8* utf8, const String16& second)
	{
		return second.compare(utf8) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char16* utf16, const String16& second)
	{
		return second.compare(utf16) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char32* utf32, const String16& second)
	{
		return second.compare(utf32) > 0;
	}

public:
	SLIB_INLINE String16 duplicate() const
	{
		return String16(m_data, length());
	}

	Memory toMemory() const;

	sl_uint32 getUtf8(sl_char8* utf8, sl_int32 len) const;

	StringData getUtf8() const;

	// contains null character at last
	Memory toUtf8() const;

	String16 substring(sl_int32 start, sl_int32 end = -1) const;
	
	SLIB_INLINE String16 left(sl_uint32 len) const
	{
		return substring( 0, len );
	}
	
	SLIB_INLINE String16 right(sl_uint32 len) const
	{
		return substring(length()-len);
	}
	
	SLIB_INLINE String16 mid(sl_int32 start, sl_int32 len) const
	{
		return substring(start, start+len);
	}

	sl_int32 indexOf(sl_char16 ch, sl_int32 start = 0) const;
	
	sl_int32 indexOf(const String16& str, sl_int32 start = 0) const;
	
	sl_int32 lastIndexOf(sl_char16 ch, sl_int32 start = -1) const;
	
	sl_int32 lastIndexOf(const String16& str, sl_int32 start = -1) const;
	
	sl_bool startsWith(sl_char16 ch) const;
	
	sl_bool startsWith(const String16& str) const;
	
	sl_bool endsWith(sl_char16 ch) const;
	
	sl_bool endsWith(const String16& str) const;
	
	void makeUpper();
	
	void makeLower();
	
	String16 toLower() const;
	
	String16 toUpper() const;
	
	String16 replaceAll(const String16& pattern, const String16& replacement) const;
	
	String16 trim() const;
	
	String16 trimLeft() const;
	
	String16 trimRight() const;
	
	List<String16> split(const String16& pattern) const;
	
	sl_uint32 hashCode() const;
	
	String16 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	
	static String16 parseBackslashEscapes(const sl_char16* input, sl_int32 len, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	
	SLIB_INLINE static String16 parseBackslashEscapes(String16& str, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null)
	{
		return parseBackslashEscapes(str.data(), str.length(), lengthParsed, flagError);
	}

public:
	// radix: 2 ~ 64,   flagUpperCase only works if radix <= 36 (0~9a~z)
	static String16 fromInt32(sl_int32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromUint32(sl_uint32 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromInt64(sl_int64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	static String16 fromUint64(sl_uint64 value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false);
	
	SLIB_INLINE static String16 fromInt(sl_reg value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromInt64(value, radix, minWidth, flagUpperCase);
#else
		return fromInt32(value, radix, minWidth, flagUpperCase);
#endif
	}
	
	SLIB_INLINE static String16 fromSize(sl_size value, sl_int32 radix = 10, sl_int32 minWidth = 0, sl_bool flagUpperCase = sl_false)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64(value, radix, minWidth, flagUpperCase);
#else
		return fromUint32(value, radix, minWidth, flagUpperCase);
#endif
	}

	static String16 fromFloat(float value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);
	
	static String16 fromDouble(double value, sl_int32 precision = -1, sl_bool flagZeroPadding = sl_false, sl_int32 minWidthIntegral = 1);

	SLIB_INLINE static String16 fromPointerValue(const void* pointer)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64((sl_uint64)(pointer), 16, 16, sl_true);
#else
		return fromUint32((sl_uint32)(pointer), 16, 8, sl_true);
#endif
	}

	static String16 fromBoolean(sl_bool value);

	static String16 makeHexString(const void* data, sl_int32 size);
	
	SLIB_INLINE static String16 makeHexString(Memory mem)
	{
		return makeHexString(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}

	
	static sl_int32 parseInt32(sl_int32* _out, const char*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseInt32(sl_int32* _out, const sl_char16*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseInt32(sl_int32* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseUint32(sl_uint32* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseUint32(sl_uint32* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseUint32(sl_uint32* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseInt64(sl_int64* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseInt64(sl_int64* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseInt64(sl_int64* _out, sl_int32 radix = 10) const;
	

	static sl_int32 parseUint64(sl_uint64* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	static sl_int32 parseUint64(sl_uint64* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	
	sl_bool parseUint64(sl_uint64* _out, sl_int32 radix = 10) const;

	
	static sl_int32 parseFloat(float* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parseFloat(float* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	sl_bool parseFloat(float* _out) const;

	
	static sl_int32 parseDouble(double* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parseDouble(double* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	sl_bool parseDouble(double* _out) const;

	
	static sl_int32 parseHexString(void* _out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parseHexString(void* _out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	sl_bool parseHexString(void* _out) const;

	
	SLIB_INLINE sl_bool parseInt(sl_reg* _out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(_out, radix);
#else
		return parseInt32(_out, radix);
#endif
	}
	
	SLIB_INLINE sl_bool parseSize(sl_size* _out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(_out, radix);
#else
		return parseUint32(_out, radix);
#endif
	}

	SLIB_INLINE sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const
	{
		sl_int32 _out = def;
		parseInt32(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const
	{
		sl_uint32 _out = def;
		parseUint32(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const
	{
		sl_int64 _out = def;
		parseInt64(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const
	{
		sl_uint64 _out = def;
		parseUint64(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const
	{
		sl_reg _out = def;
		parseInt(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const
	{
		sl_size _out = def;
		parseSize(&_out, radix);
		return _out;
	}
	
	SLIB_INLINE float parseFloat(float def = 0) const
	{
		float _out = def;
		parseFloat(&_out);
		return _out;
	}
	
	SLIB_INLINE double parseDouble(double def = 0) const
	{
		double _out = def;
		parseDouble(&_out);
		return _out;
	}

public:
	// utf8 conversion
	static sl_int32 utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer);
	
	static sl_int32 utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer);
	
	static sl_int32 utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer);
	
	static sl_int32 utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer);

private:
	SLIB_INLINE static void _increaseReference(sl_char16* data)
	{
		Base::interlockedIncrement(SLIB_STR_PREF(data));
	}

	SLIB_INLINE static void _decreaseReference(sl_char16* data)
	{
		sl_reg iRef = Base::interlockedDecrement(SLIB_STR_PREF(data));
		if (iRef == 0) {
			_destroy(data);
		}
	}
	
	SLIB_INLINE void _replaceContainer(sl_char16* data)
	{
		_decreaseReference(m_data);
		m_data = data;
	}
	
    
	// Deallocates memory allocated for this string
	static void _destroy(sl_char16* data);

	// Allocates memory required for a string for the specified length
	static sl_char16* _alloc(sl_uint32& length);
	
    
	static sl_char16* _create(sl_char16 ch, sl_uint32 nRepeatCount = 1);
	
	static sl_char16* _create(const sl_char8* strUtf8, sl_int32 length = -1);
	
	static sl_char16* _create(const sl_char16* strUtf16, sl_int32 length = -1);
	
	static sl_char16* _create(const sl_char32* strUtf32, sl_int32 length = -1);

    
	static sl_char16* _merge16(const sl_char16* s1, sl_int32 len1, const sl_char16* s2, sl_int32 len2);
	
	static sl_char16* _merge8(const sl_char16* s1, sl_int32 len1, const sl_char8* s2, sl_int32 len2);
	
	static sl_char16* _merge8(const sl_char8* s1, sl_int32 len1, const sl_char16* s2, sl_int32 len2);
	
	static sl_char16* _merge32(const sl_char16* s1, sl_int32 len1, const sl_char32* s2, sl_int32 len2);
	
	static sl_char16* _merge32(const sl_char32* s1, sl_int32 len1, const sl_char16* s2, sl_int32 len2);
    
	
	// Check two unicode string is Equal
	static sl_bool _equals16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	
	static sl_bool _equals8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	
	static sl_bool _equals32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);
	
	// Compare two unicode string
	static sl_int32 _compare8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	
	static sl_int32 _compare16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	
	static sl_int32 _compare32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);
	
public:
	static void _initStaticMemory(void* mem);
	
	friend class SafeString16;
};


/** auto-referencing object **/
class SLIB_EXPORT SafeString8
{
private:
	// String(Information & Data) storage
	sl_char8* m_data;
	SpinLock m_lock;
	
public:
	SLIB_INLINE SafeString8()
	{
		m_data = (sl_char8*)(_String_Null.data);
	}
	
	SLIB_INLINE SafeString8(const SafeString8& src)
	{
		sl_char8* data = src._retainContainer();
		m_data = data;
	}
	
	SLIB_INLINE ~SafeString8()
	{
		_decreaseReference(m_data);
	}

public:
	SafeString8(const SafeString16& src);
	
	SLIB_INLINE SafeString8(const String8& src)
	{
		sl_char8* data = src.m_data;
		_increaseReference(data);
		m_data = data;
	}
	
	SafeString8(const String16& src);
	
	// From a single charactor
	SLIB_INLINE SafeString8(sl_char8 ch, sl_uint32 nRepeatCount = 1)
	{
		m_data = String8::_create(ch, nRepeatCount);
	}
	
	// From an utf-8 string
	SLIB_INLINE SafeString8(const sl_char8* strUtf8)
	{
		m_data = String8::_create(strUtf8);
	}
	
	// From an utf-8 string
	SLIB_INLINE SafeString8(const sl_char8* strUtf8, sl_int32 length)
	{
		m_data = String8::_create(strUtf8, length);
	}
	
	// From an utf-16 string
	SLIB_INLINE SafeString8(const sl_char16* strUtf16)
	{
		m_data = String8::_create(strUtf16);
	}
	
	// From an utf-16 string
	SLIB_INLINE SafeString8(const sl_char16* strUtf16, sl_int32 length)
	{
		m_data = String8::_create(strUtf16, length);
	}
	
	// From an utf-32 string
	SLIB_INLINE SafeString8(const sl_char32* strUtf32)
	{
		m_data = String8::_create(strUtf32);
	}
	
	// From an utf-32 string
	SLIB_INLINE SafeString8(const sl_char32* strUtf32, sl_int32 length)
	{
		m_data = String8::_create(strUtf32, length);
	}
	
public:
	SLIB_INLINE static const SafeString8& null()
	{
		return *((SafeString8*)((void*)&_String_Null));
	}
	
	SLIB_INLINE static const SafeString8& getEmpty()
	{
		return *((SafeString8*)((void*)&_String_Empty));
	}
	
public:
	// Length of String
	SLIB_INLINE sl_uint32 getLength() const
	{
		String8 s(*this);
		return s.length();
	}
	
	// Length of String
	SLIB_INLINE sl_uint32 length() const
	{
		return getLength();
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_data == (sl_char8*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_data != (sl_char8*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		return (getLength() == 0);
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return (getLength() != 0);
	}
	
public:
	SLIB_INLINE void setNull()
	{
		if (m_data != (sl_char8*)(_String_Null.data)) {
			_replaceContainer((sl_char8*)(_String_Null.data));
		}
	}
	
	SLIB_INLINE void setEmpty()
	{
		if (m_data != (sl_char8*)(_String_Empty.data)) {
			_replaceContainer((sl_char8*)(_String_Empty.data));
		}
	}
	
public:
	SLIB_INLINE SafeString8& operator=(const SafeString8& other)
	{
		if (this != &other && m_data != other.m_data) {
			sl_char8* data = other._retainContainer();
			_replaceContainer(data);
		}
		return *this;
	}
	
	SLIB_INLINE SafeString8& operator=(const SafeString16& _other)
	{
		String16 other(_other);
		_replaceContainer(String8::_create(other.data(), other.length()));
		return *this;
	}
	
	SLIB_INLINE SafeString8& operator=(const String8& other)
	{
		if (m_data != other.m_data) {
			sl_char8* data = other.m_data;
			_increaseReference(data);
			_replaceContainer(data);
		}
		return *this;
	}
	
	SLIB_INLINE SafeString8& operator=(const String16& other)
	{
		_replaceContainer(String8::_create(other.data(), other.length()));
		return *this;
	}
	
	SLIB_INLINE SafeString8& operator=(const sl_char8* utf8)
	{
		_replaceContainer(String8::_create(utf8));
		return *this;
	}
	
	SLIB_INLINE SafeString8& operator=(const sl_char16* utf16)
	{
		_replaceContainer(String8::_create(utf16));
		return *this;
	}
	
	SLIB_INLINE SafeString8& operator=(const sl_char32* utf32)
	{
		_replaceContainer(String8::_create(utf32));
		return *this;
	}
	
public:
	String8 operator+(const SafeString8& other) const;
	SafeString8& operator+=(const SafeString8& other);
	
	String8 operator+(const SafeString16& other) const;
	SafeString8& operator+=(const SafeString16& other);

	String8 operator+(const String8& other) const;
	SafeString8& operator+=(const String8& other);
	
	String8 operator+(const String16& other) const;
	SafeString8& operator+=(const String16& other);
	
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
	SLIB_INLINE String8 operator+(sl_int32 number) const
	{
		return *this + String8::fromInt32(number);
	}
	
	SLIB_INLINE SafeString8& operator+=(sl_int32 number)
	{
		return (*this += String8::fromInt32(number));
	}
	
	SLIB_INLINE friend String8 operator+(sl_int32 number, const SafeString8& other)
	{
		return String8::fromInt32(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_uint32 number) const
	{
		return *this + String8::fromUint32(number);
	}
	
	SLIB_INLINE SafeString8& operator+=(sl_uint32 number)
	{
		return (*this += String8::fromUint32(number));
	}
	
	SLIB_INLINE friend String8 operator+(sl_uint32 number, const SafeString8& other)
	{
		return String8::fromUint32(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_int64 number) const
	{
		return *this + String8::fromInt64(number);
	}
	
	SLIB_INLINE SafeString8& operator+=(sl_int64 number)
	{
		return (*this += String8::fromInt64(number));
	}
	
	SLIB_INLINE friend String8 operator+(sl_int64 number, const SafeString8& other)
	{
		return String8::fromInt64(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_uint64 number) const
	{
		return *this + String8::fromUint64(number);
	}
	
	SLIB_INLINE SafeString8& operator+=(sl_uint64 number)
	{
		return (*this += String8::fromUint64(number));
	}
	
	SLIB_INLINE friend String8 operator+(sl_uint64 number, const SafeString8& other)
	{
		return String8::fromUint64(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(float number) const
	{
		return *this + String8::fromFloat(number);
	}
	
	SLIB_INLINE SafeString8& operator+=(float number)
	{
		return (*this += String8::fromFloat(number));
	}
	
	SLIB_INLINE friend String8 operator+(float number, const SafeString8& other)
	{
		return String8::fromFloat(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(double number) const
	{
		return *this + String8::fromDouble(number);
	}
	
	SLIB_INLINE SafeString8& operator+=(double number)
	{
		return (*this += String8::fromDouble(number));
	}
	
	SLIB_INLINE friend String8 operator+(double number, const SafeString8& other)
	{
		return String8::fromDouble(number) + other;
	}
	
	
	SLIB_INLINE String8 operator+(sl_bool value) const
	{
		return *this + String8::fromBoolean(value);
	}
	
	SLIB_INLINE SafeString8& operator+=(sl_bool value)
	{
		return (*this += String8::fromBoolean(value));
	}
	
	SLIB_INLINE friend String8 operator+(sl_bool value, const SafeString8& other)
	{
		return String8::fromBoolean(value) + other;
	}
	
public:
	SLIB_INLINE sl_bool equals(const SafeString8& other) const
	{
		String8 s1(*this);
		String8 s2(other);
		if (s1.m_data == s2.m_data) {
			return sl_true;
		}
		if (s1.length() != s2.length()) {
			return sl_false;
		}
		return String8::_equals8(s1.m_data, s1.length(), s2.m_data, s2.length());
	}
	
	SLIB_INLINE sl_bool equals(const SafeString16& other) const
	{
		String8 s1(*this);
		String16 s2(other);
		return String8::_equals16(s1.m_data, s1.length(), s2.data(), s2.length());
	}
	
	SLIB_INLINE sl_bool equals(const String8& other) const
	{
		String8 s(*this);
		if (s.m_data == other.m_data) {
			return sl_true;
		}
		if (s.length() != other.length()) {
			return sl_false;
		}
		return String8::_equals8(s.m_data, s.length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_bool equals(const String16& other) const
	{
		String8 s(*this);
		return String8::_equals16(s.m_data, s.length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_bool equals(const sl_char8* utf8) const
	{
		String8 s(*this);
		if (s.m_data == utf8) {
			return sl_true;
		}
		return String8::_equals8(s.m_data, s.length(), utf8, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char16* utf16) const
	{
		String8 s(*this);
		return String8::_equals16(s.m_data, s.length(), utf16, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char32* utf32) const
	{
		String8 s(*this);
		return String8::_equals32(s.m_data, s.length(), utf32, -1);
	}
	
public:
	SLIB_INLINE sl_int32 compare(const String8& other, sl_uint32 len) const
	{
		String8 s(*this);
		return s.compare(other, len);
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString8& other) const
	{
		String8 s1(*this);
		String8 s2(other);
		if (s1.m_data == s2.m_data) {
			return 0;
		}
		return String8::_compare8(s1.m_data, s1.length(), s2.m_data, s2.length());
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString16& other) const
	{
		String8 s1(*this);
		String16 s2(other);
		return String8::_compare16(s1.m_data, s1.length(), s2.data(), s2.length());
	}
	
	SLIB_INLINE sl_int32 compare(const String8& other) const
	{
		String8 s(*this);
		if (s.m_data == other.m_data) {
			return 0;
		}
		return String8::_compare8(s.m_data, s.length(), other.m_data, other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const String16& other) const
	{
		String8 s(*this);
		return String8::_compare16(s.m_data, s.length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char8* utf8) const
	{
		String8 s(*this);
		if (s.m_data == utf8) {
			return 0;
		}
		return String8::_compare8(s.m_data, s.length(), utf8, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char16* utf16) const
	{
		String8 s(*this);
		return String8::_compare16(s.m_data, s.length(), utf16, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char32* utf32) const
	{
		String8 s(*this);
		return String8::_compare32(s.m_data, s.length(), utf32, -1);
	}
	
public:
	SLIB_INLINE sl_bool operator==(const String8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const String16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char8* utf8) const
	{
		return equals(utf8);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char16* utf16) const
	{
		return equals(utf16);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char32* utf32) const
	{
		return equals(utf32);
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char8* utf8, const SafeString8& second)
	{
		return second == utf8;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char16* utf16, const SafeString8& second)
	{
		return second == utf16;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char32* utf32, const SafeString8& second)
	{
		return second == utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const String8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const String16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char8* utf8) const
	{
		return !(*this == utf8);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char16* utf16) const
	{
		return !(*this == utf16);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char32* utf32) const
	{
		return !(*this == utf32);
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char8* utf8, const SafeString8& second)
	{
		return second != utf8;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char16* utf16, const SafeString8& second)
	{
		return second != utf16;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char32* utf32, const SafeString8& second)
	{
		return second != utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator>=(const String8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const String16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char8* utf8) const
	{
		return compare(utf8) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char16* utf16) const
	{
		return compare(utf16) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char32* utf32) const
	{
		return compare(utf32) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char8* utf8, const SafeString8& second)
	{
		return second.compare(utf8) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char16* utf16, const SafeString8& second)
	{
		return second.compare(utf16) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char32* utf32, const SafeString8& second)
	{
		return second.compare(utf32) <= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<=(const String8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const String16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char8* utf8) const
	{
		return compare(utf8) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char16* utf16) const
	{
		return compare(utf16) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char32* utf32) const
	{
		return compare(utf32) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char8* utf8, const SafeString8& second)
	{
		return second.compare(utf8) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char16* utf16, const SafeString8& second)
	{
		return second.compare(utf16) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char32* utf32, const SafeString8& second)
	{
		return second.compare(utf32) >= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator>(const String8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const String16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char8* utf8) const
	{
		return compare(utf8) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char16* utf16) const
	{
		return compare(utf16) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char32* utf32) const
	{
		return compare(utf32) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char8* utf8, const SafeString8& second)
	{
		return second.compare(utf8) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char16* utf16, const SafeString8& second)
	{
		return second.compare(utf16) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char32* utf32, const SafeString8& second)
	{
		return second.compare(utf32) < 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<(const String8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const String16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char8* utf8) const
	{
		return compare(utf8) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char16* utf16) const
	{
		return compare(utf16) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char32* utf32) const
	{
		return compare(utf32) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char8* utf8, const SafeString8& second)
	{
		return second.compare(utf8) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char16* utf16, const SafeString8& second)
	{
		return second.compare(utf16) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char32* utf32, const SafeString8& second)
	{
		return second.compare(utf32) > 0;
	}
	
public:
	SLIB_INLINE String8 duplicate() const
	{
		String8 s(*this);
		return s.duplicate();
	}
	
	SLIB_INLINE Memory toMemory() const
	{
		String8 s(*this);
		return s.toMemory();
	}
	
	SLIB_INLINE sl_uint32 getUtf16(sl_char16* utf16, sl_int32 len) const
	{
		String8 s(*this);
		return s.getUtf16(utf16, len);
	}
	
	SLIB_INLINE StringData getUtf16() const
	{
		String8 s(*this);
		return s.getUtf16();
	}
	
	// contains null character at last
	SLIB_INLINE Memory toUtf16() const
	{
		String8 s(*this);
		return s.toUtf16();
	}
	
	SLIB_INLINE sl_uint32 getUtf32(sl_char32* utf32, sl_int32 len) const
	{
		String8 s(*this);
		return s.getUtf32(utf32, len);
	}
	
	SLIB_INLINE StringData getUtf32() const
	{
		String8 s(*this);
		return s.getUtf32();
	}
	
	// contains null character at last
	SLIB_INLINE Memory toUtf32() const
	{
		String8 s(*this);
		return s.toUtf32();
	}
	
	SLIB_INLINE String8 substring(sl_int32 start, sl_int32 end = -1) const
	{
		String8 s(*this);
		return s.substring(start, end);
	}
	
	SLIB_INLINE String8 left(sl_uint32 len) const
	{
		String8 s(*this);
		return s.left(len);
	}
	
	SLIB_INLINE String8 right(sl_uint32 len) const
	{
		String8 s(*this);
		return s.right(len);
	}
	
	SLIB_INLINE String8 mid(sl_int32 start, sl_int32 len) const
	{
		String8 s(*this);
		return s.mid(start, len);
	}
	
	SLIB_INLINE sl_int32 indexOf(sl_char8 ch, sl_int32 start = 0) const
	{
		String8 s(*this);
		return s.indexOf(ch, start);
	}
	
	SLIB_INLINE sl_int32 indexOf(const String8& str, sl_int32 start = 0) const
	{
		String8 s(*this);
		return s.indexOf(str, start);
	}
	
	SLIB_INLINE sl_int32 lastIndexOf(sl_char8 ch, sl_int32 start = -1) const
	{
		String8 s(*this);
		return s.lastIndexOf(ch, start);
	}
	
	SLIB_INLINE sl_int32 lastIndexOf(const String8& str, sl_int32 start = -1) const
	{
		String8 s(*this);
		return s.indexOf(str, start);
	}
	
	SLIB_INLINE sl_bool startsWith(sl_char8 ch) const
	{
		String8 s(*this);
		return s.startsWith(ch);
	}
	
	SLIB_INLINE sl_bool startsWith(const String8& str) const
	{
		String8 s(*this);
		return s.startsWith(str);
	}
	
	SLIB_INLINE sl_bool endsWith(sl_char8 ch) const
	{
		String8 s(*this);
		return s.endsWith(ch);
	}
	
	SLIB_INLINE sl_bool endsWith(const String8& str) const
	{
		String8 s(*this);
		return s.endsWith(str);
	}
	
	SLIB_INLINE void makeUpper()
	{
		String8 s(*this);
		s.makeUpper();
	}
	
	SLIB_INLINE void makeLower()
	{
		String8 s(*this);
		s.makeLower();
	}
	
	SLIB_INLINE String8 toLower() const
	{
		String8 s(*this);
		return s.toLower();
	}
	
	SLIB_INLINE String8 toUpper() const
	{
		String8 s(*this);
		return s.toUpper();
	}
	
	SLIB_INLINE String8 replaceAll(const String8& pattern, const String8& replacement) const
	{
		String8 s(*this);
		return s.replaceAll(pattern, replacement);
	}

	SLIB_INLINE String8 trim() const
	{
		String8 s(*this);
		return s.trim();
	}
	
	SLIB_INLINE String8 trimLeft() const
	{
		String8 s(*this);
		return s.trimLeft();
	}
	
	SLIB_INLINE String8 trimRight() const
	{
		String8 s(*this);
		return s.trimRight();
	}
	
	SLIB_INLINE List<String8> split(const String8& pattern) const
	{
		String8 s(*this);
		return s.split(pattern);
	}
	
	SLIB_INLINE sl_uint32 hashCode() const
	{
		String8 s(*this);
		return s.hashCode();
	}
	
	SLIB_INLINE String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true)
	{
		String8 s(*this);
		return s.applyBackslashEscapes(flagDoubleQuote);
	}
	
private:
	SLIB_INLINE static void _increaseReference(sl_char8* data)
	{
		Base::interlockedIncrement(SLIB_STR_PREF(data));
	}
	
	SLIB_INLINE static void _decreaseReference(sl_char8* data)
	{
		sl_reg iRef = Base::interlockedDecrement(SLIB_STR_PREF(data));
		if (iRef == 0) {
			String8::_destroy(data);
		}
	}
	
	sl_char8* _retainContainer() const;
	
	void _replaceContainer(sl_char8* other);

	friend class String8;
};

/** auto-referencing object **/
class SLIB_EXPORT SafeString16
{
private:
	// String(Information & Data) storage
#if defined(SLIB_USE_UNICODE16)
	union {
		sl_char16* m_data;
		wchar_t* m_dataDebug;
	};
#else
	sl_char16* m_data;
#endif
	SpinLock m_lock;
	
public:
	SLIB_INLINE SafeString16()
	{
		m_data = (sl_char16*)(_String_Null.data);
	}
	
	SLIB_INLINE SafeString16(const SafeString16& src)
	{
		sl_char16* data = src._retainContainer();
		m_data = data;
	}
	
	SLIB_INLINE ~SafeString16()
	{
		_decreaseReference(m_data);
	}
	
public:
	SafeString16(const SafeString8& src);
	
	SLIB_INLINE SafeString16(const String16& src)
    {
        sl_char16* data = src.m_data;
        _increaseReference(data);
        m_data = data;
    }
	
	SafeString16(const String8& src);
	
	// From a single charactor
	SLIB_INLINE SafeString16(sl_char16 ch, sl_uint32 nRepeatCount = 1)
	{
		m_data = String16::_create(ch, nRepeatCount);
	}
	
	// From an utf-8 string
	SLIB_INLINE SafeString16(const sl_char8* strUtf8)
	{
		m_data = String16::_create(strUtf8);
	}
	
	SLIB_INLINE SafeString16(const sl_char8* strUtf8, sl_int32 length)
	{
		m_data = String16::_create(strUtf8, length);
	}
	
	// From an utf-16 string
	SLIB_INLINE SafeString16(const sl_char16* strUtf16)
	{
		m_data = String16::_create(strUtf16);
	}
	
	SLIB_INLINE SafeString16(const sl_char16* strUtf16, sl_int32 length)
	{
		m_data = String16::_create(strUtf16, length);
	}
	
	// From an utf-32 string
	SLIB_INLINE SafeString16(const sl_char32* strUtf32)
	{
		m_data = String16::_create(strUtf32);
	}
	
	SLIB_INLINE SafeString16(const sl_char32* strUtf32, sl_int32 length)
	{
		m_data = String16::_create(strUtf32, length);
	}
	
public:
	SLIB_INLINE static const SafeString16& null()
	{
		return *((SafeString16*)((void*)&_String_Null));
	}
	
	SLIB_INLINE static const SafeString16& getEmpty()
	{
		return *((SafeString16*)((void*)&_String_Empty));
	}

public:
	// Length of String
	SLIB_INLINE sl_uint32 getLength() const
	{
		String16 s(*this);
		return s.length();
	}
	
	SLIB_INLINE sl_uint32 length() const
	{
		return getLength();
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_data == (sl_char16*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_data != (sl_char16*)(_String_Null.data);
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		return (getLength()==0);
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return (getLength() != 0);
	}
	
public:
	SLIB_INLINE void setNull()
	{
        if (m_data != (sl_char16*)(_String_Null.data)) {
            _replaceContainer((sl_char16*)(_String_Null.data));
        }
	}
	
	SLIB_INLINE void setEmpty()
	{
        if (m_data != (sl_char16*)(_String_Empty.data)) {
            _replaceContainer((sl_char16*)(_String_Empty.data));
        }
	}
	
public:
	SLIB_INLINE SafeString16& operator=(const SafeString16& other)
	{
		if (this != &other && m_data != other.m_data) {
			sl_char16* data = other._retainContainer();
			_replaceContainer(data);
		}
		return *this;
	}
	
	SLIB_INLINE SafeString16& operator=(const SafeString8& _other)
    {
        String8 other(_other);
	   _replaceContainer(String16::_create(other.data(), other.length()));
        return *this;
    }
    
	SLIB_INLINE SafeString16& operator=(const String16& other)
    {
        if (m_data != other.m_data) {
			sl_char16* data = other.m_data;
			_increaseReference(data);
			_replaceContainer(data);
		}
		return *this;
    }
	
	SLIB_INLINE SafeString16& operator=(const String8& other)
    {
		_replaceContainer(String16::_create(other.data(), other.length()));
		return *this;
	}
	
	SLIB_INLINE SafeString16& operator=(const sl_char8* utf8)
    {
		_replaceContainer(String16::_create(utf8));
		return *this;
	}
	
	SLIB_INLINE SafeString16& operator=(const sl_char16* utf16)
    {
		_replaceContainer(String16::_create(utf16));
		return *this;
	}
	
	SLIB_INLINE SafeString16& operator=(const sl_char32* utf32)	
    {
		_replaceContainer(String16::_create(utf32));
		return *this;
	}
    
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
	SLIB_INLINE String16 operator+(sl_int32 number) const
	{
		return *this + String16::fromInt32(number);
	}
	
	SLIB_INLINE SafeString16& operator+=(sl_int32 number)
	{
		return (*this += String16::fromInt32(number));
	}
	
	SLIB_INLINE friend String16 operator+(sl_int32 number, const SafeString16& other)
	{
		return String16::fromInt32(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_uint32 number) const
	{
		return *this + String16::fromUint32(number);
	}
	
	SLIB_INLINE SafeString16& operator+=(sl_uint32 number)
	{
		return (*this += String16::fromUint32(number));
	}
	
	SLIB_INLINE friend String16 operator+(sl_uint32 number, const SafeString16& other)
	{
		return String16::fromUint32(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_int64 number) const
	{
		return *this + String16::fromInt64(number);
	}
	
	SLIB_INLINE SafeString16& operator+=(sl_int64 number)
	{
		return (*this += String16::fromInt64(number));
	}
	
	SLIB_INLINE friend String16 operator+(sl_int64 number, const SafeString16& other)
	{
		return String16::fromInt64(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_uint64 number) const
	{
		return *this + String16::fromUint64(number);
	}
	
	SLIB_INLINE SafeString16& operator+=(sl_uint64 number)
	{
		return (*this += String16::fromUint64(number));
	}
	
	SLIB_INLINE friend String16 operator+(sl_uint64 number, const SafeString16& other)
	{
		return String16::fromUint64(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(float number) const
	{
		return *this + String16::fromFloat(number);
	}
	
	SLIB_INLINE SafeString16& operator+=(float number)
	{
		return (*this += String16::fromFloat(number));
	}
	
	SLIB_INLINE friend String16 operator+(float number, const SafeString16& other)
	{
		return String16::fromFloat(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(double number) const
	{
		return *this + String16::fromDouble(number);
	}
	
	SLIB_INLINE SafeString16& operator+=(double number)
	{
		return (*this += String16::fromDouble(number));
	}
	
	SLIB_INLINE friend String16 operator+(double number, const SafeString16& other)
	{
		return String16::fromDouble(number) + other;
	}
	
	
	SLIB_INLINE String16 operator+(sl_bool value) const
	{
		return *this + String16::fromBoolean(value);
	}
	
	SLIB_INLINE SafeString16& operator+=(sl_bool value)
	{
		return (*this += String16::fromBoolean(value));
	}
	
	SLIB_INLINE friend String16 operator+(sl_bool value, const SafeString16& other)
	{
		return String16::fromBoolean(value) + other;
	}
	
public:
	SLIB_INLINE sl_bool equals(const SafeString16& other) const
	{
		String16 s1(*this);
		String16 s2(other);
		if (s1.m_data == s2.m_data) {
			return sl_true;
		}
		if (s1.length() != s2.length()) {
			return sl_false;
		}
		return String16::_equals16(s1.m_data, s1.length(), s2.m_data, s2.length());
	}
	
	SLIB_INLINE sl_bool equals(const SafeString8& other) const
	{
		String16 s1(*this);
		String8 s2(other);
		return String16::_equals8(s1.m_data, s1.length(), s2.data(), s2.length());
	}
	
	SLIB_INLINE sl_bool equals(const String16& other) const
	{
		String16 s(*this);
		if (s.m_data == other.m_data) {
			return sl_true;
		}
		if (s.length() != other.length()) {
			return sl_false;
		}
		return String16::_equals16(s.m_data, s.length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_bool equals(const String8& other) const
	{
		String16 s(*this);
		return String16::_equals8(s.m_data, s.length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_bool equals(const sl_char8* utf8) const
	{
		String16 s(*this);
		return String16::_equals8(s.m_data, s.length(), utf8, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char16* utf16) const
	{
		String16 s(*this);
		if (s.m_data == utf16) {
			return sl_true;
		}
		return String16::_equals16(s.m_data, s.length(), utf16, -1);
	}
	
	SLIB_INLINE sl_bool equals(const sl_char32* utf32) const
	{
		String16 s(*this);
		return String16::_equals32(s.m_data, s.length(), utf32, -1);
	}
	
public:
	SLIB_INLINE sl_int32 compare(const String16& other, sl_uint32 len) const
	{
		String16 s(*this);
		return s.compare(other, len);
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString16& other) const
	{
		String16 s1(*this);
		String16 s2(other);
		if (s1.m_data == s2.m_data) {
			return 0;
		}
		return String16::_compare16(s1.m_data, s1.length(), s2.m_data, s2.length());
	}
	
	SLIB_INLINE sl_int32 compare(const SafeString8& other) const
	{
		String16 s1(*this);
		String8 s2(other);
		return String16::_compare8(s1.m_data, s1.length(), s2.data(), s2.length());
	}
	
	SLIB_INLINE sl_int32 compare(const String16& other) const
	{
		String16 s(*this);
		if (s.m_data == other.m_data) {
			return 0;
		}
		return String16::_compare16(s.m_data, s.length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const String8& other) const
	{
		String16 s(*this);
		return String16::_compare8(s.m_data, s.length(), other.data(), other.length());
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char8* utf8) const
	{
		String16 s(*this);
		return String16::_compare8(s.m_data, s.length(), utf8, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char16* utf16) const
	{
		String16 s(*this);
		if (s.m_data == utf16) {
			return 0;
		}
		return String16::_compare16(s.m_data, s.length(), utf16, -1);
	}
	
	SLIB_INLINE sl_int32 compare(const sl_char32* utf32) const
	{
		String16 s(*this);
		return String16::_compare32(s.m_data, s.length(), utf32, -1);
	}
	
public:
	SLIB_INLINE sl_bool operator==(const String16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const String8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString16& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const SafeString8& other) const
	{
		return equals(other);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char8* utf8) const
	{
		return equals(utf8);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char16* utf16) const
	{
		return equals(utf16);
	}
	
	SLIB_INLINE sl_bool operator==(const sl_char32* utf32) const
	{
		return equals(utf32);
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char8* utf8, const SafeString16& second)
	{
		return second == utf8;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char16* utf16, const SafeString16& second)
	{
		return second == utf16;
	}
	
	SLIB_INLINE friend sl_bool operator==(const sl_char32* utf32, const SafeString16& second)
	{
		return second == utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const String16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const String8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString16& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const SafeString8& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char8* utf8) const
	{
		return !(*this == utf8);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char16* utf16) const
	{
		return !(*this == utf16);
	}
	
	SLIB_INLINE sl_bool operator!=(const sl_char32* utf32) const
	{
		return !(*this == utf32);
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char8* utf8, const SafeString16& second)
	{
		return second != utf8;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char16* utf16, const SafeString16& second)
	{
		return second != utf16;
	}
	
	SLIB_INLINE friend sl_bool operator!=(const sl_char32* utf32, const SafeString16& second)
	{
		return second != utf32;
	}
	
public:
	SLIB_INLINE sl_bool operator>=(const String16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const String8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString16& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const SafeString8& other) const
	{
		return compare(other) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char8* utf8) const
	{
		return compare(utf8) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char16* utf16) const
	{
		return compare(utf16) >= 0;
	}
	
	SLIB_INLINE sl_bool operator>=(const sl_char32* utf32) const
	{
		return compare(utf32) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char8* utf8, const SafeString16& second)
	{
		return second.compare(utf8) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char16* utf16, const SafeString16& second)
	{
		return second.compare(utf16) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator>=(const sl_char32* utf32, const SafeString16& second)
	{
		return second.compare(utf32) <= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<=(const String16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const String8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString16& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const SafeString8& other) const
	{
		return compare(other) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char8* utf8) const
	{
		return compare(utf8) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char16* utf16) const
	{
		return compare(utf16) <= 0;
	}
	
	SLIB_INLINE sl_bool operator<=(const sl_char32* utf32) const
	{
		return compare(utf32) <= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char8* utf8, const SafeString16& second)
	{
		return second.compare(utf8) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char16* utf16, const SafeString16& second)
	{
		return second.compare(utf16) >= 0;
	}
	
	SLIB_INLINE friend sl_bool operator<=(const sl_char32* utf32, const SafeString16& second)
	{
		return second.compare(utf32) >= 0;
	}
	
public:
	SLIB_INLINE sl_bool operator>(const String16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const String8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString16& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const SafeString8& other) const
	{
		return compare(other) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char8* utf8) const
	{
		return compare(utf8) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char16* utf16) const
	{
		return compare(utf16) > 0;
	}
	
	SLIB_INLINE sl_bool operator>(const sl_char32* utf32) const
	{
		return compare(utf32) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char8* utf8, const SafeString16& second)
	{
		return second.compare(utf8) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char16* utf16, const SafeString16& second)
	{
		return second.compare(utf16) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator>(const sl_char32* utf32, const SafeString16& second)
	{
		return second.compare(utf32) < 0;
	}
	
public:
	SLIB_INLINE sl_bool operator<(const String16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const String8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString16& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const SafeString8& other) const
	{
		return compare(other) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char8* utf8) const
	{
		return compare(utf8) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char16* utf16) const
	{
		return compare(utf16) < 0;
	}
	
	SLIB_INLINE sl_bool operator<(const sl_char32* utf32) const
	{
		return compare(utf32) < 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char8* utf8, const SafeString16& second)
	{
		return second.compare(utf8) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char16* utf16, const SafeString16& second)
	{
		return second.compare(utf16) > 0;
	}
	
	SLIB_INLINE friend sl_bool operator<(const sl_char32* utf32, const SafeString16& second)
	{
		return second.compare(utf32) > 0;
	}
	
	
public:
	SLIB_INLINE String16 duplicate() const
	{
		String16 s(*this);
		return s.duplicate();
	}
	
	SLIB_INLINE Memory toMemory() const
	{
		String16 s(*this);
		return s.toMemory();
	}
	
	SLIB_INLINE sl_uint32 getUtf8(sl_char8* utf8, sl_int32 len) const
	{
		String16 s(*this);
		return s.getUtf8(utf8, len);
	}
	
	SLIB_INLINE StringData getUtf8() const
	{
		String16 s(*this);
		return s.getUtf8();
	}
	
	// contains null character at last
	SLIB_INLINE Memory toUtf8() const
	{
		String16 s(*this);
		return s.toUtf8();
	}
	
	SLIB_INLINE String16 substring(sl_int32 start, sl_int32 end = -1) const
	{
		String16 s(*this);
		return s.substring(start, end);
	}
	
	SLIB_INLINE String16 left(sl_uint32 len) const
	{
		String16 s(*this);
		return s.left(len);
	}
	
	SLIB_INLINE String16 right(sl_uint32 len) const
	{
		String16 s(*this);
		return s.right(len);
	}
	
	SLIB_INLINE String16 mid(sl_int32 start, sl_int32 len) const
	{
		String16 s(*this);
		return s.mid(start, len);
	}
	
	SLIB_INLINE sl_int32 indexOf(sl_char16 ch, sl_int32 start = 0) const
	{
		String16 s(*this);
		return s.indexOf(ch, start);
	}
	
	SLIB_INLINE sl_int32 indexOf(const String16& str, sl_int32 start = 0) const
	{
		String16 s(*this);
		return s.indexOf(str, start);
	}
	
	SLIB_INLINE sl_int32 lastIndexOf(sl_char16 ch, sl_int32 start = -1) const
	{
		String16 s(*this);
		return s.lastIndexOf(ch, start);
	}
	
	SLIB_INLINE sl_int32 lastIndexOf(const String16& str, sl_int32 start = -1) const
	{
		String16 s(*this);
		return s.indexOf(str, start);
	}
	
	SLIB_INLINE sl_bool startsWith(sl_char16 ch) const
	{
		String16 s(*this);
		return s.startsWith(ch);
	}
	
	SLIB_INLINE sl_bool startsWith(const String16& str) const
	{
		String16 s(*this);
		return s.startsWith(str);
	}
	
	SLIB_INLINE sl_bool endsWith(sl_char16 ch) const
	{
		String16 s(*this);
		return s.endsWith(ch);
	}
	
	SLIB_INLINE sl_bool endsWith(const String16& str) const
	{
		String16 s(*this);
		return s.endsWith(str);
	}
	
	SLIB_INLINE void makeUpper()
	{
		String16 s(*this);
		s.makeUpper();
	}
	
	SLIB_INLINE void makeLower()
	{
		String16 s(*this);
		s.makeLower();
	}
	
	SLIB_INLINE String16 toLower() const
	{
		String16 s(*this);
		return s.toLower();
	}
	
	SLIB_INLINE String16 toUpper() const
	{
		String16 s(*this);
		return s.toUpper();
	}
	
	SLIB_INLINE String16 replaceAll(const String16& pattern, const String16& replacement) const
	{
		String16 s(*this);
		return s.replaceAll(pattern, replacement);
	}
	
	SLIB_INLINE String16 trim() const
	{
		String16 s(*this);
		return s.trim();
	}
	
	SLIB_INLINE String16 trimLeft() const
	{
		String16 s(*this);
		return s.trimLeft();
	}
	
	SLIB_INLINE String16 trimRight() const
	{
		String16 s(*this);
		return s.trimRight();
	}
	
	SLIB_INLINE List<String16> split(const String16& pattern) const
	{
		String16 s(*this);
		return s.split(pattern);
	}
	
	SLIB_INLINE sl_uint32 hashCode() const
	{
		String16 s(*this);
		return s.hashCode();
	}
	
	SLIB_INLINE String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true)
	{
		String16 s(*this);
		return s.applyBackslashEscapes(flagDoubleQuote);
	}
	
	
private:
	SLIB_INLINE static void _increaseReference(sl_char16* data)
	{
		Base::interlockedIncrement(SLIB_STR_PREF(data));
	}
	
	SLIB_INLINE static void _decreaseReference(sl_char16* data)
	{
		sl_reg iRef = Base::interlockedDecrement(SLIB_STR_PREF(data));
		if (iRef == 0) {
			String16::_destroy(data);
		}
	}
	
	sl_char16* _retainContainer() const;
	
	void _replaceContainer(sl_char16* other);
	
	friend class String16;
};

class SLIB_EXPORT StringBuffer8 : public Object
{
private:
	Queue<String8> m_queue;
	sl_int32 m_len;

public:
	StringBuffer8();

	sl_bool add(const String8& str);

	SLIB_INLINE sl_int32 getLength() const
	{
		return m_len;
	}

	String8 merge() const;
};

class SLIB_EXPORT StringBuffer16 : public Object
{
private:
	Queue<String16> m_queue;
	sl_int32 m_len;

public:
	StringBuffer16();

	sl_bool add(const String16& str);

	SLIB_INLINE sl_int32 getLength() const
	{
		return m_len;
	}

	String16 merge() const;
};

class SLIB_EXPORT StringDataBuffer8 : public Object
{
private:
	Queue<StringData> m_queue;
	sl_int32 m_len;

public:
	StringDataBuffer8();

	sl_bool add(const StringData& str);
	
	SLIB_INLINE sl_bool add(const sl_char8* buf, sl_uint32 length)
	{
		StringData data;
		data.sz8 = buf;
		data.len = length;
		return add(data);
	}
	
	SLIB_INLINE sl_bool add(const sl_char8* buf, sl_uint32 length, const Memory& mem)
	{
		StringData data;
		data.sz8 = buf;
		data.len = length;
		data.mem = mem;
		return add(data);
	}

	SLIB_INLINE sl_int32 getLength() const
	{
		return m_len;
	}

	String8 merge() const;
};

class SLIB_EXPORT StringDataBuffer16 : public Object
{
private:
	Queue<StringData> m_queue;
	sl_int32 m_len;

public:
	StringDataBuffer16();

	sl_bool add(const StringData& str);
	
	SLIB_INLINE sl_bool add(const sl_char16* buf, sl_uint32 length)
	{
		StringData data;
		data.sz16 = buf;
		data.len = length;
		return add(data);
	}
	
	SLIB_INLINE sl_bool add(const sl_char16* buf, sl_uint32 length, const Memory& mem)
	{
		StringData data;
		data.sz16 = buf;
		data.len = length;
		data.mem = mem;
		return add(data);
	}

	SLIB_INLINE sl_int32 getLength() const
	{
		return m_len;
	}

	String16 merge() const;
};


template<>
SLIB_INLINE int Compare<String8>::compare(const String8& a, const String8& b)
{
	return a.compare(b);
}
template<>
SLIB_INLINE int Compare<String16>::compare(const String16& a, const String16& b)
{
	return a.compare(b);
}

template<>
SLIB_INLINE int Compare<SafeString8>::compare(const SafeString8& a, const SafeString8& b)
{
	return a.compare(b);
}
template<>
SLIB_INLINE int Compare<SafeString16>::compare(const SafeString16& a, const SafeString16& b)
{
	return a.compare(b);
}

template<>
SLIB_INLINE sl_bool Compare<String8>::equals(const String8& a, const String8& b)
{
	return a.equals(b);
}
template<>
SLIB_INLINE sl_bool Compare<String16>::equals(const String16& a, const String16& b)
{
	return a.equals(b);
}

template<>
SLIB_INLINE sl_bool Compare<SafeString8>::equals(const SafeString8& a, const SafeString8& b)
{
	return a.equals(b);
}
template<>
SLIB_INLINE sl_bool Compare<SafeString16>::equals(const SafeString16& a, const SafeString16& b)
{
	return a.equals(b);
}

template<>
SLIB_INLINE sl_uint32 Hash<String8>::hash(const String8& v)
{
	return v.hashCode();
}
template<>
SLIB_INLINE sl_uint32 Hash<String16>::hash(const String16& v)
{
	return v.hashCode();
}

template<>
SLIB_INLINE sl_uint32 Hash<SafeString8>::hash(const SafeString8& v)
{
	return v.hashCode();
}
template<>
SLIB_INLINE sl_uint32 Hash<SafeString16>::hash(const SafeString16& v)
{
	return v.hashCode();
}


typedef String8 String;
typedef SafeString8 SafeString;
typedef StringBuffer8 StringBuffer;
typedef StringDataBuffer8 StringDataBuffer;

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_STRING
