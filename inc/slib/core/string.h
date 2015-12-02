#ifndef CHECKHEADER_SLIB_CORE_STRING
#define CHECKHEADER_SLIB_CORE_STRING

#include "definition.h"
#include "base.h"

#include "memory.h"
#include "algorithm.h"
#include "list.h"

#define SLIB_STR_MAX_LEN 0x10000000

#define SLIB_CHAR_IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_ALPHA_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define SLIB_CHAR_IS_ALPHA_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define SLIB_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define SLIB_CHAR_IS_ALNUM(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_HEX(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

#define SLIB_CHAR_DIGIT_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : 10)
#define SLIB_CHAR_HEX_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : (((c) >= 'A' && (c) <= 'F') ? ((c) -  55) : ((c) >= 'a' && (c) <= 'f') ? ((c) -  87) : 16))

/************************************************************************/
/* String class Declaration                                             */
/************************************************************************/
#define SLIB_STR_PREF(buf) ((sl_reg*)(((char*)buf)-16))
#define SLIB_STR_PHASH(buf) ((sl_uint32*)(((char*)buf)-8))
#define SLIB_STR_PLEN(buf) ((sl_uint32*)(((char*)buf)-4))
#define SLIB_STR_HEADER_LEN 16

#if defined(SLIB_USE_UNICODE32)
#define SLIB_STATIC_STRING16(name, str) \
	SLIB_SAFE_STATIC(slib::String16, name, SLIB_UNICODE(str));
#else
#define SLIB_STATIC_STRING16(name, str) \
	SLIB_ALIGN(static wchar_t _static_string_buf_##name[], 8) = L"\0\0\0\0\0\0\0\0" L##str; \
	static void* _static_string_container_##name = ((char*)_static_string_buf_##name) + SLIB_STR_HEADER_LEN; \
	slib::String16::initStaticMemory(_static_string_container_##name); \
	slib::String16& name = *((slib::String16*)((void*)(&_static_string_container_##name)));
#endif

#define SLIB_STATIC_STRING8(name, str) \
	SLIB_ALIGN(static char _static_string_buf_##name[], 8) = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" str; \
	static void* _static_string_container_##name = ((char*)_static_string_buf_##name) + SLIB_STR_HEADER_LEN; \
	slib::String8::initStaticMemory(_static_string_container_##name); \
	slib::String8& name = *((slib::String8*)((void*)(&_static_string_container_##name)));

#define SLIB_STATIC_STRING8_NULL(name) \
	static sl_char8* _static_string_buffer_##name = _String8_nullBuf; \
	slib::String8& name = *((slib::String8*)((void*)(&_static_string_buffer_##name)));

#define SLIB_STATIC_STRING16_NULL(name) \
	static sl_char16* _static_string_buffer_##name = _String16_nullBuf; \
	slib::String16& name = *((slib::String16*)((void*)(&_static_string_buffer_##name)));

#define SLIB_STATIC_STRING SLIB_STATIC_STRING8

#define SLIB_STATIC_STRING_NULL SLIB_STATIC_STRING8_NULL

SLIB_NAMESPACE_BEGIN
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

extern sl_char8* const _String8_nullBuf;
extern sl_char16* const _String16_nullBuf;
extern sl_char8* const _String8_emptyBuf;
extern sl_char16* const _String16_emptyBuf;

extern const char* _StringConv_radixPatternUpper;
extern const char* _StringConv_radixPatternLower;
extern const sl_uint8* _StringConv_radixInversePatternBig;
extern const sl_uint8* _StringConv_radixInversePatternSmall;

/** auto-referencing object **/
class String16;
class SLIB_EXPORT String8
{
private:
	/* Private Members */

	// String(Information & Data) storage
	sl_char8* m_buf;

public:
	/* Constructors & De-constructor */

	// An null String Object
	SLIB_INLINE String8()
	{
		init();
	}
	
	// Returns a null string
	SLIB_INLINE static const String8& null()
	{
		return *((String8*)((void*)&_String8_nullBuf));
	}

	SLIB_INLINE static const String8& getEmpty()
	{
		return *((String8*)((void*)&_String8_emptyBuf));
	}
	
	// Copy constructor
	SLIB_INLINE String8(const String8& src)
	{
		copy(src);
	}

	// De-constructor
	SLIB_INLINE ~String8()
	{
		decreaseReference(m_buf);
	}

	// From 16bit-char String
	String8(const String16& src);

	// From a single charactor
	SLIB_INLINE String8(sl_char8 ch, sl_uint32 nRepeatCount = 1)
	{
		construct(ch, nRepeatCount);
	}
	// From an utf-8 string
	SLIB_INLINE String8(const sl_char8* strUtf8)
	{
		construct(strUtf8);
	}
	SLIB_INLINE String8(const sl_char8* strUtf8, sl_int32 length)
	{
		construct(strUtf8, length);
	}
	// From an utf-16 string
	SLIB_INLINE String8(const sl_char16* strUtf16)
	{
		construct(strUtf16);
	}
	SLIB_INLINE String8(const sl_char16* strUtf16, sl_int32 length)
	{
		construct(strUtf16, length);
	}
	// From an utf-32 string
	SLIB_INLINE String8(const sl_char32* strUtf32)
	{
		construct(strUtf32);
	}
	SLIB_INLINE String8(const sl_char32* strUtf32, sl_int32 length)
	{
		construct(strUtf32, length);
	}

private:
	void construct(sl_char8 ch, sl_uint32 nRepeatCount = 1);
	void construct(const sl_char8* strUtf8, sl_int32 length = -1);
	void construct(const sl_char16* strUtf16, sl_int32 length = -1);
	void construct(const sl_char32* strUtf32, sl_int32 length = -1);

	/* Static Functions */
public:
	// Create memory of string region for 'len' charactors
	static String8 memory(sl_uint32 len);

	SLIB_INLINE static String8 fromUtf8(const void* utf8, sl_int32 len = -1)
	{
		String8 str((const sl_char8*)utf8, len);
		return str;
	}
	SLIB_INLINE static String8 fromUtf8(Memory mem)
	{
		return fromUtf8(mem.getBuf(), (sl_int32)(mem.getSize()));
	}
	SLIB_INLINE static String8 fromUtf16(const void* utf16, sl_int32 len = -1)
	{
		String8 str((const sl_char16*)utf16, len);
		return str;
	}
	SLIB_INLINE static String8 fromUtf16(Memory mem)
	{
		return fromUtf16(mem.getBuf(), (sl_int32)(mem.getSize()));
	}
	SLIB_INLINE static String8 fromUtf32(const void* utf32, sl_int32 len = -1)
	{
		String8 str((const sl_char32*)utf32, len);
		return str;
	}
	SLIB_INLINE static String8 fromUtf32(Memory mem)
	{
		return fromUtf32(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String8 fromUtf16BE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String8 fromUtf16BE(Memory mem)
	{
		return fromUtf16BE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String8 fromUtf16LE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String8 fromUtf16LE(Memory mem)
	{
		return fromUtf16LE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String8 fromUtf(const void* buf, sl_size len);

	SLIB_INLINE static String8 fromUtf(Memory mem)
	{
		return fromUtf(mem.getBuf(), mem.getSize());
	}

	static void initStaticMemory(void* mem);

	/* Attributes */

	// Length of String
	SLIB_INLINE sl_uint32 getLength() const
	{
		return *SLIB_STR_PLEN(m_buf);
	}
	SLIB_INLINE sl_uint32 length() const
	{
		return getLength();
	}
	SLIB_INLINE void setLength(sl_uint32 len)
	{
		*(SLIB_STR_PLEN(m_buf)) = len;
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return m_buf == _String8_nullBuf;
	}
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_buf != _String8_nullBuf;
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
		return m_buf[index];
	}
	SLIB_INLINE void setAt(sl_int32 index, sl_char8 ch)
	{
		m_buf[index] = ch;
	}
	SLIB_INLINE const sl_char8* getBuf() const
	{
		return m_buf;
	}
	SLIB_INLINE sl_char8* getBuf()
	{
		return m_buf;
	}

	/* Operations */
	SLIB_INLINE void setNull()
	{
		*this = null();
	}
	
	SLIB_INLINE void setEmpty()
	{
		*this = getEmpty();
	}

	SLIB_INLINE String8& operator=(const String8& other)
	{
		if (this != &other && m_buf != other.m_buf) {
			set(other);
		}
		return *this;
	}
	String8& operator=(const String16& other);
	String8& operator=(const sl_char8* utf8);
	String8& operator=(const sl_char16* utf16);
	String8& operator=(const sl_char32* utf32);

	SLIB_INLINE sl_char8 operator[](sl_int32 index) const
	{
		return m_buf[index];
	}
	SLIB_INLINE sl_char8& operator[](sl_int32 index)
	{
		return m_buf[index];
	}

	String8 operator+(const String8& other) const;
	String8 operator+(const String16& other) const;
	String8 operator+(const sl_char8* utf8) const;
	String8 operator+(const sl_char16* utf16) const;
	String8 operator+(const sl_char32* utf32) const;
	friend String8 operator+(const sl_char8* utf8, const String8& second);
	friend String8 operator+(const sl_char16* utf16, const String8& second);
	friend String8 operator+(const sl_char32* utf32, const String8& second);

	SLIB_INLINE String8& operator+=(const String8& other)
	{
		*this = *this + other;
		return *this;
	}
	SLIB_INLINE String8& operator+=(const String16& other)
	{
		*this = *this + other;
		return *this;
	}
	SLIB_INLINE String8& operator+=(const sl_char8* utf8)
	{
		*this = *this + utf8;
		return *this;
	}
	SLIB_INLINE String8& operator+=(const sl_char16* utf16)
	{
		*this = *this + utf16;
		return *this;
	}
	SLIB_INLINE String8& operator+=(const sl_char32* utf32)
	{
		*this = *this + utf32;
		return *this;
	}
	SLIB_INLINE String8 operator+(sl_int32 number) const
	{
		return *this + String8::fromInt32(number);
	}
	SLIB_INLINE friend String8 operator+(sl_int32 number, const String8& other)
	{
		return String8::fromInt32(number) + other;
	}
	SLIB_INLINE String8 operator+(sl_uint32 number) const
	{
		return *this + String8::fromUint32(number);
	}
	SLIB_INLINE friend String8 operator+(sl_uint32 number, const String8& other)
	{
		return String8::fromUint32(number) + other;
	}
	SLIB_INLINE String8 operator+(sl_int64 number) const
	{
		return *this + String8::fromInt64(number);
	}
	SLIB_INLINE friend String8 operator+(sl_int64 number, const String8& other)
	{
		return String8::fromInt64(number) + other;
	}
	SLIB_INLINE String8 operator+(sl_uint64 number) const
	{
		return *this + String8::fromUint64(number);
	}
	SLIB_INLINE friend String8 operator+(sl_uint64 number, const String8& other)
	{
		return String8::fromUint64(number) + other;
	}
	SLIB_INLINE String8 operator+(float number) const
	{
		return *this + String8::fromFloat(number);
	}
	SLIB_INLINE friend String8 operator+(float number, const String8& other)
	{
		return String8::fromFloat(number) + other;
	}
	SLIB_INLINE String8 operator+(double number) const
	{
		return *this + String8::fromDouble(number);
	}
	SLIB_INLINE friend String8 operator+(double number, const String8& other)
	{
		return String8::fromDouble(number) + other;
	}
	SLIB_INLINE String8 operator+(sl_bool value) const
	{
		return *this + String8::fromBoolean(value);
	}
	SLIB_INLINE friend String8 operator+(sl_bool value, const String8& other)
	{
		return String8::fromBoolean(value) + other;
	}

	SLIB_INLINE String8& operator+=(sl_int32 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String8& operator+=(sl_uint32 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String8& operator+=(sl_int64 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String8& operator+=(sl_uint64 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String8& operator+=(float number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String8& operator+=(double number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String8& operator+=(sl_bool value)
	{
		*this = *this + value;
		return *this;
	}

	SLIB_INLINE sl_bool equals(const String8& other) const
	{
		if (m_buf == other.m_buf) {
			return sl_true;
		}
		String8 oper1 = *this, oper2 = other;
		if (oper1.length() != oper2.length()) {
			return sl_false;
		}
		return equals8(oper1.getBuf(), oper1.getLength(), oper2.getBuf(), oper2.getLength());
	}
	sl_bool equals(const String16& other) const;
	SLIB_INLINE sl_bool equals(const sl_char8* utf8) const
	{
		if (m_buf == utf8) {
			return sl_true;
		}
		String8 oper1 = *this;
		return equals8(oper1.getBuf(), oper1.getLength(), utf8, -1);
	}
	SLIB_INLINE sl_bool equals(const sl_char16* utf16) const
	{		
		String8 oper1 = *this;
		return equals16(oper1.getBuf(), oper1.getLength(), utf16, -1);
	}
	SLIB_INLINE sl_bool equals(const sl_char32* utf32) const
	{
		String8 oper1 = *this;
		return equals32(oper1.getBuf(), oper1.getLength(), utf32, -1);
	}

	SLIB_INLINE sl_int32 compare(const String8& other) const
	{
		if (m_buf == other.m_buf) {
			return 0;
		}
		String8 oper1 = *this, oper2 = other;
		return compare8(oper1.getBuf(), oper1.getLength(), oper2.getBuf(), oper2.getLength());
	}
	sl_int32 compare(const String8& other, sl_uint32 len) const;

	sl_int32 compare(const String16& other) const;
	SLIB_INLINE sl_int32 compare(const sl_char8* utf8) const
	{
		if (m_buf == utf8) {
			return 0;
		}
		String8 oper1 = *this;
		return compare8(oper1.getBuf(), oper1.getLength(), utf8, -1);
	}
	SLIB_INLINE sl_int32 compare(const sl_char16* utf16) const
	{
		String8 oper1 = *this;
		return compare16(oper1.getBuf(), oper1.getLength(), utf16, -1);
	}
	SLIB_INLINE sl_int32 compare(const sl_char32* utf32) const
	{
		String8 oper1 = *this;
		return compare32(oper1.getBuf(), oper1.getLength(), utf32, -1);
	}

	SLIB_INLINE sl_bool operator==(const String8& other) const
	{
		return equals(other);
	}
	SLIB_INLINE sl_bool operator==(const String16& other) const
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
	SLIB_INLINE sl_bool operator!=(const String8& other) const
	{
		return !(*this == other);
	}
	SLIB_INLINE sl_bool operator!=(const String16& other) const
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
	SLIB_INLINE sl_bool operator>=(const String8& other) const
	{
		return compare(other) >= 0;
	}
	SLIB_INLINE sl_bool operator>=(const String16& other) const
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
	SLIB_INLINE sl_bool operator<=(const String16& other) const
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
	SLIB_INLINE sl_bool operator>(const String8& other) const
	{
		return compare(other) > 0;
	}
	SLIB_INLINE sl_bool operator>(const String16& other) const
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
	SLIB_INLINE sl_bool operator<(const String8& other) const
	{
		return compare(other) < 0;
	}
	SLIB_INLINE sl_bool operator<(const String16& other) const
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

	/* Functions */
	SLIB_INLINE String8 duplicate() const
	{
		String8 str = *this;
		String8 ret(str.m_buf, str.getLength());
		return ret;
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
		String8 oper = *this;
		return oper.substring(oper.getLength() - len, -1);
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

	// utility functions
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

	SLIB_INLINE static String8 fromBoolean(sl_bool value)
	{
		if (value) {
			return String8("true");
		} else {
			return String8("false");
		}
	}

	static String8 makeHexString(const void* data, sl_int32 size);
	SLIB_INLINE static String8 makeHexString(Memory mem)
	{
		return makeHexString(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}

	static sl_int32 parseInt32(sl_int32* out, const char*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseInt32(sl_int32* out, const sl_char16*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseInt32(sl_int32* out, sl_int32 radix = 10) const;

	static sl_int32 parseUint32(sl_uint32* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseUint32(sl_uint32* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseUint32(sl_uint32* out, sl_int32 radix = 10) const;

	static sl_int32 parseInt64(sl_int64* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseInt64(sl_int64* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseInt64(sl_int64* out, sl_int32 radix = 10) const;

	static sl_int32 parseUint64(sl_uint64* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseUint64(sl_uint64* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseUint64(sl_uint64* out, sl_int32 radix = 10) const;

	static sl_int32 parseFloat(float* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	static sl_int32 parseFloat(float* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	sl_bool parseFloat(float* out) const;

	static sl_int32 parseDouble(double* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	static sl_int32 parseDouble(double* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	sl_bool parseDouble(double* out) const;

	static sl_int32 parseHexString(void* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	static sl_int32 parseHexString(void* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	sl_bool parseHexString(void* out) const;

	SLIB_INLINE sl_bool parseInt(sl_reg* out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(out, radix);
#else
		return parseInt32(out, radix);
#endif
	}
	SLIB_INLINE sl_bool parseSize(sl_size* out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(out, radix);
#else
		return parseUint32(out, radix);
#endif
	}

	SLIB_INLINE sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const
	{
		sl_int32 out = def;
		parseInt32(&out, radix);
		return out;
	}
	SLIB_INLINE sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const
	{
		sl_uint32 out = def;
		parseUint32(&out, radix);
		return out;
	}
	SLIB_INLINE sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const
	{
		sl_int64 out = def;
		parseInt64(&out, radix);
		return out;
	}
	SLIB_INLINE sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const
	{
		sl_uint64 out = def;
		parseUint64(&out, radix);
		return out;
	}
	SLIB_INLINE sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const
	{
		sl_reg out = def;
		parseInt(&out, radix);
		return out;
	}
	SLIB_INLINE sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const
	{
		sl_size out = def;
		parseSize(&out, radix);
		return out;
	}
	SLIB_INLINE float parseFloat(float def = 0) const
	{
		float out = def;
		parseFloat(&out);
		return out;
	}
	SLIB_INLINE double parseDouble(double def = 0) const
	{
		double out = def;
		parseDouble(&out);
		return out;
	}

	sl_uint32 hashCode() const;

	String8 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	static String8 parseBackslashEscapes(const sl_char8* input, sl_int32 len, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	SLIB_INLINE static String8 parseBackslashEscapes(String8 str, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null)
	{
		return parseBackslashEscapes(str.getBuf(), str.getLength(), lengthParsed, flagError);
	}

private:
	/* Private Functions */
	SLIB_INLINE void init()
	{
		m_buf = _String8_nullBuf;
	}

	SLIB_INLINE void increaseReference(sl_char8*buf)
	{
		Base::interlockedIncrement(SLIB_STR_PREF(buf));
	}

	SLIB_INLINE void decreaseReference(sl_char8*buf)
	{
		sl_reg iRef = Base::interlockedDecrement(SLIB_STR_PREF(buf));
		if (iRef == 0) {
			destroy(buf);
		}
	}

	void copy(const String8& other);
	void set(const String8& other);

	// Allocates memory required for a string for the specified length
	static sl_char8* alloc(sl_uint32& length);
	// Deallocates memory allocated for this string
	void destroy(sl_char8* buf);

	// Compare two unicode string
	static sl_int32 compare8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	static sl_int32 compare16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	static sl_int32 compare32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);
	// Check two unicode string is Equal
	static sl_bool equals8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	static sl_bool equals16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	static sl_bool equals32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);

public:
	// utf8 conversion
	static sl_int32 utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer);
	static sl_int32 utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer);
	static sl_int32 utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer);
	static sl_int32 utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer);

};

/** auto-referencing object **/
class SLIB_EXPORT String16
{
private:
	/* Private Members */

	// String(Information & Data) storage
#if defined(SLIB_USE_UNICODE16)
	union {
		sl_char16* m_buf;
		wchar_t* m_bufDebug;
	};
#else
	sl_char16* m_buf;
#endif

public:
	/* Constructors & De-constructor */

	// An null String Object
	SLIB_INLINE String16()
	{
		init();
	}
	
	// Returns a null string
	SLIB_INLINE static const String16& null()
	{
		return *((String16*)((void*)&_String16_nullBuf));
	}

	SLIB_INLINE static const String16& getEmpty()
	{
		return *((String16*)((void*)&_String16_emptyBuf));
	}
	
	// Copy constructor
	SLIB_INLINE String16(const String16& src)
	{
		copy(src);
	}

	// De-constructor
	SLIB_INLINE ~String16()
	{
		decreaseReference(m_buf);
	}

	// From 8bit-char String
	String16(const String8& src);

	// From a single charactor
	SLIB_INLINE String16(sl_char16 ch, sl_uint32 nRepeatCount = 1)
	{
		construct(ch, nRepeatCount);
	}
	// From an utf-8 string
	SLIB_INLINE String16(const sl_char8* strUtf8)
	{
		construct(strUtf8);
	}
	SLIB_INLINE String16(const sl_char8* strUtf8, sl_int32 length)
	{
		construct(strUtf8, length);
	}
	// From an utf-16 string
	SLIB_INLINE String16(const sl_char16* strUtf16)
	{
		construct(strUtf16);
	}
	SLIB_INLINE String16(const sl_char16* strUtf16, sl_int32 length)
	{
		construct(strUtf16, length);
	}
	// From an utf-32 string
	SLIB_INLINE String16(const sl_char32* strUtf32)
	{
		construct(strUtf32);
	}
	SLIB_INLINE String16(const sl_char32* strUtf32, sl_int32 length)
	{
		construct(strUtf32, length);
	}

private:
	void construct(sl_char16 ch, sl_uint32 nRepeatCount = 1);
	void construct(const sl_char8* strUtf8, sl_int32 length = -1);
	void construct(const sl_char16* strUtf16, sl_int32 length = -1);
	void construct(const sl_char32* strUtf32, sl_int32 length = -1);

	/* Static Functions */
public:
	// Create memory of string region for 'len' charactors
	static String16 memory(sl_uint32 len);

	SLIB_INLINE static String16 fromUtf8(const void* utf8, sl_int32 len = -1)
	{
		String16 str((const sl_char8*)utf8, len);
		return str;
	}

	SLIB_INLINE static String16 fromUtf8(Memory mem)
	{
		return fromUtf8(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	SLIB_INLINE static String16 fromUtf16(const void* utf16, sl_int32 len = -1)
	{
		String16 str((const sl_char16*)utf16, len);
		return str;
	}

	SLIB_INLINE static String16 fromUtf16(Memory mem)
	{
		return fromUtf16(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	SLIB_INLINE static String16 fromUtf32(const void* utf32, sl_int32 len = -1)
	{
		String16 str((const sl_char32*)utf32, len);
		return str;
	}

	SLIB_INLINE static String16 fromUtf32(Memory mem)
	{
		return fromUtf32(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String16 fromUtf16BE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String16 fromUtf16BE(Memory mem)
	{
		return fromUtf16BE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static String16 fromUtf16LE(const void* utf16, sl_int32 len = -1);

	SLIB_INLINE static String16 fromUtf16LE(Memory mem)
	{
		return fromUtf16LE(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	// process UTF16BE, UTF16LE, UTF8 strings automatically
	static String16 fromUtf(const void* buf, sl_size len);

	SLIB_INLINE static String16 fromUtf(Memory mem)
	{
		return fromUtf(mem.getBuf(), (sl_int32)(mem.getSize()));
	}

	static void initStaticMemory(void* mem);

/* Attributes */
	
	// Length of String
	SLIB_INLINE sl_uint32 getLength() const
	{
		return *SLIB_STR_PLEN(m_buf);
	}
	SLIB_INLINE sl_uint32 length() const
	{
		return getLength();
	}
	SLIB_INLINE void setLength(sl_uint32 len)
	{
		*(SLIB_STR_PLEN(m_buf)) = len;
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return m_buf == _String16_nullBuf;
	}
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_buf != _String16_nullBuf;
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
		return m_buf[index];
	}
	SLIB_INLINE void setAt(sl_int32 index, sl_char16 ch)
	{
		m_buf[index] = ch;
	}
	SLIB_INLINE const sl_char16* getBuf() const
	{
		return m_buf;
	}
	SLIB_INLINE sl_char16* getBuf()
	{
		return m_buf;
	}

/* Operations */
	SLIB_INLINE void setNull()
	{
		*this = null();
	}
	
	SLIB_INLINE void setEmpty()
	{
		*this = getEmpty();
	}

	SLIB_INLINE String16& operator=(const String16& other)
	{
		if (this != &other && m_buf != other.m_buf) {
			set(other);
		}
		return *this;
	}
	String16& operator=(const String8& other);
	String16& operator=(const sl_char8* utf8);
	String16& operator=(const sl_char16* utf16);
	String16& operator=(const sl_char32* utf32);
	
	SLIB_INLINE sl_char16 operator[](sl_int32 index) const
	{
		return m_buf[index];
	}
	SLIB_INLINE sl_char16& operator[](sl_int32 index)
	{
		return m_buf[index];
	}

	String16 operator+(const String16& other) const;
	String16 operator+(const String8& other) const;
	String16 operator+(const sl_char8* utf8) const;
	String16 operator+(const sl_char16* utf16) const;
	String16 operator+(const sl_char32* utf32) const;
	friend String16 operator+(const sl_char8* utf8, const String16& second);
	friend String16 operator+(const sl_char16* utf16, const String16& second);
	friend String16 operator+(const sl_char32* utf32, const String16& second);

	SLIB_INLINE String16& operator+=(const String16& other)
	{
		*this = *this + other;
		return *this;
	}
	SLIB_INLINE String16& operator+=(const String8& other)
	{
		*this = *this + other;
		return *this;
	}
	SLIB_INLINE String16& operator+=(const sl_char8* utf8)
	{
		*this = *this + utf8;
		return *this;
	}
	SLIB_INLINE String16& operator+=(const sl_char16* utf16)
	{
		*this = *this + utf16;
		return *this;
	}
	SLIB_INLINE String16& operator+=(const sl_char32* utf32)
	{
		*this = *this + utf32;
		return *this;
	}

	SLIB_INLINE String16 operator+(sl_int32 number) const
	{
		return *this + String16::fromInt32(number);
	}
	SLIB_INLINE friend String16 operator+(sl_int32 number, const String16& other)
	{
		return String16::fromInt32(number) + other;
	}
	SLIB_INLINE String16 operator+(sl_uint32 number) const
	{
		return *this + String16::fromUint32(number);
	}
	SLIB_INLINE friend String16 operator+(sl_uint32 number, const String16& other)
	{
		return String16::fromUint32(number) + other;
	}
	SLIB_INLINE String16 operator+(sl_int64 number) const
	{
		return *this + String16::fromInt64(number);
	}
	SLIB_INLINE friend String16 operator+(sl_int64 number, const String16& other)
	{
		return String16::fromInt64(number) + other;
	}
	SLIB_INLINE String16 operator+(sl_uint64 number) const
	{
		return *this + String16::fromUint64(number);
	}
	SLIB_INLINE friend String16 operator+(sl_uint64 number, const String16& other)
	{
		return String16::fromUint64(number) + other;
	}
	SLIB_INLINE String16 operator+(float number) const
	{
		return *this + String16::fromFloat(number);
	}
	SLIB_INLINE friend String16 operator+(float number, const String16& other)
	{
		return String16::fromFloat(number) + other;
	}
	SLIB_INLINE String16 operator+(double number) const
	{
		return *this + String16::fromDouble(number);
	}
	SLIB_INLINE friend String16 operator+(double number, const String16& other)
	{
		return String16::fromDouble(number) + other;
	}
	SLIB_INLINE String16 operator+(sl_bool value) const
	{
		return *this + String16::fromBoolean(value);
	}
	SLIB_INLINE friend String16 operator+(sl_bool value, const String16& other)
	{
		return String16::fromBoolean(value) + other;
	}

	SLIB_INLINE String16& operator+=(sl_int32 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String16& operator+=(sl_uint32 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String16& operator+=(sl_int64 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String16& operator+=(sl_uint64 number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String16& operator+=(float number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String16& operator+=(double number)
	{
		*this = *this + number;
		return *this;
	}
	SLIB_INLINE String16& operator+=(sl_bool value)
	{
		*this = *this + value;
		return *this;
	}

	SLIB_INLINE sl_bool equals(const String16& other) const
	{
		if (m_buf == other.m_buf) {
			return sl_true;
		}
		String16 oper1 = *this, oper2 = other;
		if (oper1.length() != oper2.length()) {
			return sl_false;
		}
		return equals16(oper1.getBuf(), oper1.getLength(), oper2.getBuf(), oper2.getLength());
	}
	sl_bool equals(const String8& other) const;
	SLIB_INLINE sl_bool equals(const sl_char8* utf8) const
	{
		String16 oper1 = *this;
		return equals8(oper1.getBuf(), oper1.getLength(), utf8, -1);
	}
	SLIB_INLINE sl_bool equals(const sl_char16* utf16) const
	{
		if (m_buf == utf16) {
			return sl_true;
		}
		String16 oper1 = *this;
		return equals16(oper1.getBuf(), oper1.getLength(), utf16, -1);
	}
	SLIB_INLINE sl_bool equals(const sl_char32* utf32) const
	{
		String16 oper1 = *this;
		return equals32(oper1.getBuf(), oper1.getLength(), utf32, -1);
	}

	SLIB_INLINE sl_int32 compare(const String16& other) const
	{
		if (m_buf == other.m_buf) {
			return 0;
		}
		String16 oper1 = *this, oper2 = other;
		return compare16(oper1.getBuf(), oper1.getLength(), oper2.getBuf(), oper2.getLength());
	}
	sl_int32 compare(const String16& other, sl_uint32 len) const;

	sl_int32 compare(const String8& other) const;

	SLIB_INLINE sl_int32 compare(const sl_char8* utf8) const
	{
		String16 oper1 = *this;
		return compare8(oper1.getBuf(), oper1.getLength(), utf8, -1);
	}
	SLIB_INLINE sl_int32 compare(const sl_char16* utf16) const
	{
		if (m_buf == utf16) {
			return 0;
		}
		String16 oper1 = *this;
		return compare16(oper1.getBuf(), oper1.getLength(), utf16, -1);
	}
	SLIB_INLINE sl_int32 compare(const sl_char32* utf32) const
	{
		String16 oper1 = *this;
		return compare32(oper1.getBuf(), oper1.getLength(), utf32, -1);
	}

	SLIB_INLINE sl_bool operator==(const String16& other) const
	{
		return equals(other);
	}
	SLIB_INLINE sl_bool operator==(const String8& other) const
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
	SLIB_INLINE sl_bool operator!=(const String16& other) const
	{
		return !(*this == other);
	}
	SLIB_INLINE sl_bool operator!=(const String8& other) const
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
	SLIB_INLINE sl_bool operator>=(const String16& other) const
	{
		return compare(other) >= 0;
	}
	SLIB_INLINE sl_bool operator>=(const String8& other) const
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
	SLIB_INLINE sl_bool operator<=(const String16& other) const
	{
		return compare(other) <= 0;
	}
	SLIB_INLINE sl_bool operator<=(const String8& other) const
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
	SLIB_INLINE sl_bool operator>(const String16& other) const
	{
		return compare(other) > 0;
	}
	SLIB_INLINE sl_bool operator>(const String8& other) const
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
	SLIB_INLINE sl_bool operator<(const String16& other) const
	{
		return compare(other) < 0;
	}
	SLIB_INLINE sl_bool operator<(const String8& other) const
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

/* Functions */
	SLIB_INLINE String16 duplicate() const
	{
		String16 str = *this;
		String16 ret(str.m_buf, str.getLength());
		return ret;
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
		String16 oper = *this;
		return oper.substring( oper.getLength()-len, -1 );
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

	// utility functions
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

	SLIB_INLINE static String16 fromBoolean(sl_bool value)
	{
		if (value) {
			return String16(SLIB_UNICODE("true"));
		} else {
			return String16(SLIB_UNICODE("false"));
		}
	}

	static String16 makeHexString(const void* data, sl_int32 size);
	SLIB_INLINE static String16 makeHexString(Memory mem)
	{
		return makeHexString(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}

	static sl_int32 parseInt32(sl_int32* out, const char*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseInt32(sl_int32* out, const sl_char16*sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseInt32(sl_int32* out, sl_int32 radix = 10) const;

	static sl_int32 parseUint32(sl_uint32* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseUint32(sl_uint32* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseUint32(sl_uint32* out, sl_int32 radix = 10) const;

	static sl_int32 parseInt64(sl_int64* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseInt64(sl_int64* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseInt64(sl_int64* out, sl_int32 radix = 10) const;

	static sl_int32 parseUint64(sl_uint64* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	static sl_int32 parseUint64(sl_uint64* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_int32 radix = 10);
	sl_bool parseUint64(sl_uint64* out, sl_int32 radix = 10) const;

	static sl_int32 parseFloat(float* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	static sl_int32 parseFloat(float* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	sl_bool parseFloat(float* out) const;

	static sl_int32 parseDouble(double* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	static sl_int32 parseDouble(double* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	sl_bool parseDouble(double* out) const;

	static sl_int32 parseHexString(void* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	static sl_int32 parseHexString(void* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	sl_bool parseHexString(void* out) const;

	SLIB_INLINE sl_bool parseInt(sl_reg* out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(out, radix);
#else
		return parseInt32(out, radix);
#endif
	}
	SLIB_INLINE sl_bool parseSize(sl_size* out, sl_int32 radix = 10) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(out, radix);
#else
		return parseUint32(out, radix);
#endif
	}

	SLIB_INLINE sl_int32 parseInt32(sl_int32 radix = 10, sl_int32 def = 0) const
	{
		sl_int32 out = def;
		parseInt32(&out, radix);
		return out;
	}
	SLIB_INLINE sl_uint32 parseUint32(sl_int32 radix = 10, sl_uint32 def = 0) const
	{
		sl_uint32 out = def;
		parseUint32(&out, radix);
		return out;
	}
	SLIB_INLINE sl_int64 parseInt64(sl_int32 radix = 10, sl_int64 def = 0) const
	{
		sl_int64 out = def;
		parseInt64(&out, radix);
		return out;
	}
	SLIB_INLINE sl_uint64 parseUint64(sl_int32 radix = 10, sl_uint64 def = 0) const
	{
		sl_uint64 out = def;
		parseUint64(&out, radix);
		return out;
	}
	SLIB_INLINE sl_reg parseInt(sl_int32 radix = 10, sl_reg def = 0) const
	{
		sl_reg out = def;
		parseInt(&out, radix);
		return out;
	}
	SLIB_INLINE sl_size parseSize(sl_int32 radix = 10, sl_size def = 0) const
	{
		sl_size out = def;
		parseSize(&out, radix);
		return out;
	}
	SLIB_INLINE float parseFloat(float def = 0) const
	{
		float out = def;
		parseFloat(&out);
		return out;
	}
	SLIB_INLINE double parseDouble(double def = 0) const
	{
		double out = def;
		parseDouble(&out);
		return out;
	}

	sl_uint32 hashCode() const;

	String16 applyBackslashEscapes(sl_bool flagDoubleQuote = sl_true);
	static String16 parseBackslashEscapes(const sl_char16* input, sl_int32 len, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null);
	SLIB_INLINE static String16 parseBackslashEscapes(String16 str, sl_int32* lengthParsed = sl_null, sl_bool* flagError = sl_null)
	{
		return parseBackslashEscapes(str.getBuf(), str.getLength(), lengthParsed, flagError);
	}

private:
/* Private Functions */
	SLIB_INLINE void init()
	{
		m_buf = _String16_nullBuf;
	}

	SLIB_INLINE void increaseReference(sl_char16*buf)
	{
		Base::interlockedIncrement(SLIB_STR_PREF(buf));
	}

	SLIB_INLINE void decreaseReference(sl_char16*buf)
	{
		sl_reg iRef = Base::interlockedDecrement(SLIB_STR_PREF(buf));
		if (iRef == 0) {
			destroy(buf);
		}
	}

	void copy(const String16& other);
	void set(const String16& other);

	// Allocates memory required for a string for the specified length
	static sl_char16* alloc(sl_uint32& length);
	// Deallocates memory allocated for this string
	void destroy(sl_char16* buf);

	// Compare two unicode string
	static sl_int32 compare8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	static sl_int32 compare16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	static sl_int32 compare32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);
	// Check two unicode string is Equal
	static sl_bool equals8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2);
	static sl_bool equals16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2);
	static sl_bool equals32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2);

public:
	// utf32-utf16 conversion
	static void copyUtf32ToUtf16(sl_char16* utf16, const sl_char32* utf32, sl_int32 lenUtf32Buffer);
	static sl_int32 compareUtf16ToUtf32(const sl_char16* m1, sl_int32 len1, const sl_char32* m2, sl_int32 len2);
	// utf8 conversion
	static sl_int32 utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer);
	static sl_int32 utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer);
	static sl_int32 utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer);
	static sl_int32 utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer);

};
SLIB_NAMESPACE_END

#include "queue.h"

SLIB_NAMESPACE_BEGIN
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
SLIB_INLINE sl_uint32 Hash<String8>::hash(const String8& v)
{
	return v.hashCode();
}
template<>
SLIB_INLINE sl_uint32 Hash<String16>::hash(const String16& v)
{
	return v.hashCode();
}

SLIB_NAMESPACE_END

SLIB_NAMESPACE_BEGIN
typedef String8 String;
typedef StringBuffer8 StringBuffer;
typedef StringDataBuffer8 StringDataBuffer;
SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_STRING
