#include "../../../inc/slib/core/string.h"
#include "../../../inc/slib/core/queue.h"
#include "../../../inc/slib/core/endian.h"
#include "../../../inc/slib/core/scoped_pointer.h"


SLIB_NAMESPACE_BEGIN

SLIB_ALIGN(sl_uint32 _g_bufNullString8[5], 8) = { 0x7f7f7f7f, 0x7f7f7f7f, 0, 0, 0 };
sl_char8* const _String8_nullBuf = (sl_char8*)(((sl_uint8*)(void*)_g_bufNullString8) + SLIB_STR_HEADER_LEN);

SLIB_ALIGN(sl_uint32 _g_bufEmptyString8[5], 8) = { 0x7f7f7f7f, 0x7f7f7f7f, 0, 0, 0 };
sl_char8* const _String8_emptyBuf = (sl_char8*)(((sl_uint8*)(void*)_g_bufEmptyString8) + SLIB_STR_HEADER_LEN);

SLIB_ALIGN(sl_uint32 _g_bufNullString16[5], 8) = { 0x7f7f7f7f, 0x7f7f7f7f, 0, 0, 0 };
sl_char16* const _String16_nullBuf = (sl_char16*)(((sl_uint8*)(void*)_g_bufNullString16) + SLIB_STR_HEADER_LEN);

SLIB_ALIGN(sl_uint32 _g_bufEmptyString16[5], 8) = { 0x7f7f7f7f, 0x7f7f7f7f, 0, 0, 0 };
sl_char16* const _String16_emptyBuf = (sl_char16*)(((sl_uint8*)(void*)_g_bufEmptyString16) + SLIB_STR_HEADER_LEN);

class _StringBase
{
public:
	static sl_int32 getSz8Length(const sl_char8* utf8, sl_int32 count = -1);
	static sl_int32 getSz16Length(const sl_char16* utf16, sl_int32 count = -1);
	static sl_int32 getSz32Length(const sl_char32* utf32, sl_int32 count = -1);

	// utf8 conversion
	static sl_uint32 utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer);
	static sl_uint32 utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer);
	static sl_uint32 utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer);
	static sl_uint32 utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer);
};

class _TemplateFunc8
{
public:
	SLIB_INLINE static const void* findMemory(const void* mem, sl_uint8 pattern, sl_size count)
	{
		return Base::findMemory(mem, pattern, count);
	}

	SLIB_INLINE static const void* findMemoryReverse(const void* mem, sl_uint8 pattern, sl_size count)
	{
		return Base::findMemoryReverse(mem, pattern, count);
	}

	SLIB_INLINE static sl_int8 compareMemory(const void* mem1, const void* mem2, sl_size count)
	{
		return Base::compareMemory(mem1, mem2, count);
	}

	SLIB_INLINE static void resetMemory(void* dst, sl_uint8 value, sl_size count)
	{
		return Base::resetMemory(dst, value, count);
	}
};

class _TemplateFunc16
{
public:
	SLIB_INLINE static const void* findMemory(const void* mem, sl_uint16 pattern, sl_size count)
	{
		return Base::findMemory2(mem, pattern, count);
	}

	SLIB_INLINE static const void* findMemoryReverse(const void* mem, sl_uint16 pattern, sl_size count)
	{
		return Base::findMemoryReverse2(mem, pattern, count);
	}

	SLIB_INLINE static sl_int16 compareMemory(const void* mem1, const void* mem2, sl_size count)
	{
		return Base::compareMemory2(mem1, mem2, count);
	}

	SLIB_INLINE static void resetMemory(void* dst, sl_uint16 value, sl_size count)
	{
		return Base::resetMemory2(dst, value, count);
	}
};

void String8::copy(const String8& other)
{
	if ((void*)(&other) == (void*)(&_String8_nullBuf)) {
		m_buf = _String8_nullBuf;
	} else if ((void*)(&other) == (void*)(&_String8_emptyBuf)) {
		m_buf = _String8_emptyBuf;
	} else {
		SpinLocker lock(SpinLockPoolForString::get(&other));
		sl_char8* newBuf = other.m_buf;
		increaseReference(newBuf);
		m_buf = newBuf;
	}
}

void String16::copy(const String16& other)
{
	if ((void*)(&other) == (void*)(&_String16_nullBuf)) {
		m_buf = _String16_nullBuf;
	} else if ((void*)(&other) == (void*)(&_String16_emptyBuf)) {
		m_buf = _String16_emptyBuf;
	} else {
		SpinLocker lock(SpinLockPoolForString::get(&other));
		sl_char16* newBuf = other.m_buf;
		increaseReference(newBuf);
		m_buf = newBuf;
	}
}

void String8::set(const String8& other)
{
	sl_char8* newBuf;
	if ((void*)(&other) == (void*)(&_String8_nullBuf)) {
		newBuf = _String8_nullBuf;
	} else if ((void*)(&other) == (void*)(&_String8_emptyBuf)) {
		newBuf = _String8_emptyBuf;
	} else {
		SpinLocker lock(SpinLockPoolForString::get(&other));
		newBuf = other.m_buf;
		increaseReference(newBuf);
	}
	{
		SpinLocker lock(SpinLockPoolForString::get(this));
		sl_char8* before = m_buf;
		m_buf = newBuf;
		decreaseReference(before);
	}
}

void String16::set(const String16& other)
{
	sl_char16* newBuf;
	if ((void*)(&other) == (void*)(&_String16_nullBuf)) {
		newBuf = _String16_nullBuf;
	} else if ((void*)(&other) == (void*)(&_String16_emptyBuf)) {
		newBuf = _String16_emptyBuf;
	} else {
		SpinLocker lock(SpinLockPoolForString::get(&other));
		newBuf = other.m_buf;
		increaseReference(newBuf);
	}
	{
		SpinLocker lock(SpinLockPoolForString::get(this));
		sl_char16* before = m_buf;
		m_buf = newBuf;
		decreaseReference(before);
	}
}

String8::String8(const String16& _src)
{
	String16 src = _src;
	if (src.isNull()) {
		m_buf = _String8_nullBuf;
	} else if (src.isEmpty()) {
		m_buf = _String8_emptyBuf;
	} else {
		construct(src.getBuf(), src.getLength());
	}
}

String16::String16(const String8& _src)
{
	String8 src = _src;
	if (src.isNull()) {
		m_buf = _String16_nullBuf;
	} else if (src.isEmpty()) {
		m_buf = _String16_emptyBuf;
	} else {
		construct(src.getBuf(), src.getLength());
	}
}

void String8::construct(sl_char8 ch, sl_uint32 nRepeatCount)
{
	sl_char8* mem = alloc(nRepeatCount);
	if (nRepeatCount > 0) {
		Base::resetMemory(mem, ch, nRepeatCount);
	}
	m_buf = mem;
}

void String16::construct(sl_char16 ch, sl_uint32 nRepeatCount)
{
	sl_char16* mem = alloc(nRepeatCount);
	if (nRepeatCount > 0) {
		Base::resetMemory2(mem, ch, nRepeatCount);
	}
	m_buf = mem;
}

void String8::construct(const sl_char8* utf8, sl_int32 lenUtf8)
{
	if (utf8) {
		if (lenUtf8 < 0) {
			lenUtf8 = _StringBase::getSz8Length(utf8, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = lenUtf8;
		sl_char8* mem = alloc(len);
		if (len > 0) {
			Base::copyMemory(mem, utf8, len*sizeof(sl_char8));
		}
		m_buf = mem;
	} else {
		init();
	}
}

void String16::construct(const sl_char8* utf8, sl_int32 lenUtf8)
{
	if (utf8) {
		if (lenUtf8 < 0) {
			lenUtf8 = _StringBase::getSz8Length(utf8, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = _StringBase::utf8ToUtf16(utf8, lenUtf8, sl_null, -1);
		sl_char16* mem = alloc(len);
		if (len > 0) {
			_StringBase::utf8ToUtf16(utf8, lenUtf8, mem, len);
			mem[len] = 0;
		}
		m_buf = mem;
	} else {
		init();
	}
}

void String8::construct(const sl_char16* utf16, sl_int32 lenUtf16)
{
	if (utf16) {
		if (lenUtf16 < 0) {
			lenUtf16 = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = _StringBase::utf16ToUtf8(utf16, lenUtf16, sl_null, -1);
		sl_char8* mem = alloc(len);
		if (len > 0) {
			_StringBase::utf16ToUtf8(utf16, lenUtf16, mem, len);
			mem[len] = 0;
		}
		m_buf = mem;
	} else {
		init();
	}
}

void String16::construct(const sl_char16* utf16, sl_int32 lenUtf16)
{
	if (utf16) {
		if (lenUtf16 < 0) {
			lenUtf16 = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = lenUtf16;
		sl_char16* mem = alloc(len);
		if (len > 0) {
			Base::copyMemory(mem, utf16, len*sizeof(sl_char16));
		}
		m_buf = mem;
	} else {
		init();
	}
}

void String8::construct(const sl_char32* utf32, sl_int32 lenUtf32)
{
	if (utf32) {
		if (lenUtf32 < 0) {
			lenUtf32 = _StringBase::getSz32Length(utf32, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = _StringBase::utf32ToUtf8(utf32, lenUtf32, sl_null, -1);
		sl_char8* mem = alloc(len);
		if (len > 0) {
			_StringBase::utf32ToUtf8(utf32, lenUtf32, mem, len);
			mem[len] = 0;
		}
		m_buf = mem;
	} else {
		init();
	}
}

void String16::construct(const sl_char32* utf32, sl_int32 lenUtf32)
{
	if (utf32) {
		if (lenUtf32 < 0) {
			lenUtf32 = _StringBase::getSz32Length(utf32, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = lenUtf32;
		sl_char16* mem = alloc(len);
		if (len > 0) {
			copyUtf32ToUtf16(mem, utf32, len);
		}
		m_buf = mem;
	} else {
		init();
	}
}

String8 String8::fromUtf16BE(const void* _utf16, sl_int32 len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		if (Endian::isLE()) {
			if (len < 0) {
				len = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
			}
			SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, len);
			for (sl_int32 i = 0; i < len; i++) {
				buf[i] = Endian::swap16(utf16[i]);
			}
			return String8(buf, len);
		} else {
			return String8(utf16, len);
		}
	}
	return String8::null();
}

String16 String16::fromUtf16BE(const void* _utf16, sl_int32 len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		String16 ret(utf16, len);
		if (Endian::isLE()) {
			sl_char16* buf = ret.getBuf();
			sl_uint32 n = ret.getLength();
			for (sl_uint32 i = 0; i < n; i++) {
				buf[i] = Endian::swap16(buf[i]);
			}
		}
		return ret;
	}
	return String16::null();
}

String8 String8::fromUtf16LE(const void* _utf16, sl_int32 len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		if (Endian::isBE()) {
			if (len < 0) {
				len = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
			}
			SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, len);
			for (sl_int32 i = 0; i < len; i++) {
				buf[i] = Endian::swap16(utf16[i]);
			}
			return String8(buf, len);
		} else {
			return String8(utf16, len);
		}
	}
	return String8::null();
}

String16 String16::fromUtf16LE(const void* _utf16, sl_int32 len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		String16 ret(utf16, len);
		if (Endian::isBE()) {
			sl_char16* buf = ret.getBuf();
			sl_uint32 n = ret.getLength();
			for (sl_uint32 i = 0; i < n; i++) {
				buf[i] = Endian::swap16(buf[i]);
			}
		}
		return ret;
	}
	return String16::null();
}

String8 String8::fromUtf(const void* _buf, sl_size _len)
{
	sl_char8* buf = (sl_char8*)_buf;
	if (!buf) {
		return String8::null();
	}
	if (_len == 0) {
		return String8::getEmpty();
	}
	sl_size _max = SLIB_STR_MAX_LEN << 1;
	if (_len > _max) {
		_len = _max;
	}
	sl_int32 len = (sl_int32)_len;
	if (len >= 2) {
		if (buf[0] == (sl_char8)0xFF && buf[1] == (sl_char8)0xFE) {
			return String8::fromUtf16LE(buf, (len - 2) >> 1);
		}
		if (buf[0] == (sl_char8)0xFE && buf[1] == (sl_char8)0xFF) {
			return String8::fromUtf16BE(buf, (len - 2) >> 1);
		}
	}
	if (len >= 3) {
		if (buf[0] == (sl_char8)0xEF && buf[1] == (sl_char8)0xBB && buf[2] == (sl_char8)0xBF) {
			return String8(buf, len - 3);
		}
	}
	return String8(buf, len);
}

String16 String16::fromUtf(const void* _buf, sl_size _len)
{
	sl_char8* buf = (sl_char8*)_buf;
	if (!buf) {
		return String16::null();
	}
	if (_len == 0) {
		return String16::getEmpty();
	}
	sl_size _max = SLIB_STR_MAX_LEN << 1;
	if (_len > _max) {
		_len = _max;
	}
	sl_int32 len = (sl_int32)_len;
	if (len >= 2) {
		if (buf[0] == (sl_char8)0xFF && buf[1] == (sl_char8)0xFE) {
			return String16::fromUtf16LE(buf, (len - 2) >> 1);
		}
		if (buf[0] == (sl_char8)0xFE && buf[1] == (sl_char8)0xFF) {
			return String16::fromUtf16BE(buf, (len - 2) >> 1);
		}
	}
	if (len >= 3) {
		if (buf[0] == (sl_char8)0xEF && buf[1] == (sl_char8)0xBB && buf[2] == (sl_char8)0xBF) {
			return String16(buf, len - 3);
		}
	}
	return String16(buf, len);
}

void String8::initStaticMemory(void* mem)
{
	sl_char8* buf = (sl_char8*)(mem);
	if (*(SLIB_STR_PREF(buf)) == 0) {
		*(SLIB_STR_PLEN(buf)) = _StringBase::getSz8Length(buf);
		*(SLIB_STR_PHASH(buf)) = 0;
		*(SLIB_STR_PREF(buf)) = SLIB_SIZE_MAX >> 1;
	}
}

void String16::initStaticMemory(void* mem)
{
	sl_char16* buf = (sl_char16*)(mem);
	if (*(SLIB_STR_PREF(buf)) == 0) {
		*(SLIB_STR_PLEN(buf)) = _StringBase::getSz16Length(buf);
		*(SLIB_STR_PHASH(buf)) = 0;
		*(SLIB_STR_PREF(buf)) = SLIB_SIZE_MAX >> 1;
	}
}

String8& String8::operator=(const String16& _other)
{
	String8 other(_other);
	*this = other;
	return *this;
}

String16& String16::operator=(const String8& _other)
{
	String16 other(_other);
	*this = other;
	return *this;
}

String8& String8::operator=(const sl_char8* utf8)
{
	String8 other(utf8);
	*this = other;
	return *this;
}

String16& String16::operator=(const sl_char8* utf8)
{
	String16 other(utf8);
	*this = other;
	return *this;
}

String8& String8::operator=(const sl_char16* utf16)
{
	String8 other(utf16);
	*this = other;
	return *this;
}

String16& String16::operator=(const sl_char16* utf16)
{
	String16 other(utf16);
	*this = other;
	return *this;
}

String8& String8::operator=(const sl_char32* utf32)
{
	String8 other(utf32);
	*this = other;
	return *this;
}

String16& String16::operator=(const sl_char32* utf32)
{
	String16 other(utf32);
	*this = other;
	return *this;
}

String8 String8::operator+(const String8& other) const
{
	String8 oper1(*this), oper2(other);
	sl_int32 len1, len2;
	len1 = oper1.getLength();
	len2 = oper2.getLength();
	sl_uint32 len = len1 + len2;
	sl_char8* mem = alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, oper1.m_buf, len1*sizeof(sl_char8));
		Base::copyMemory(mem + len1, oper2.m_buf, len2*sizeof(sl_char8));
	}
	String8 ret;
	ret.m_buf = mem;
	return ret;
}

String16 String16::operator+(const String16& other) const
{
	String16 oper1(*this), oper2(other);
	sl_int32 len1, len2;
	len1 = oper1.getLength();
	len2 = oper2.getLength();
	sl_uint32 len = len1 + len2;
	sl_char16* mem = alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, oper1.m_buf, len1*sizeof(sl_char16));
		Base::copyMemory(mem + len1, oper2.m_buf, len2*sizeof(sl_char16));
	}
	String16 ret;
	ret.m_buf = mem;
	return ret;
}

String8 String8::operator+(const String16& _other) const
{
	String8 other(_other);
	return (*this) + other;
}

String16 String16::operator+(const String8& _other) const
{
	String16 other(_other);
	return (*this) + other;
}

String8 String8::operator+(const sl_char8* utf8) const
{
	if (utf8 == sl_null)
		return *this;
	String8 oper1(*this);
	sl_int32 len1, len2;
	len1 = oper1.getLength();
	len2 = _StringBase::getSz8Length(utf8, SLIB_STR_MAX_LEN);
	sl_uint32 len = len1 + len2;
	sl_char8* mem = alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, oper1.m_buf, len1*sizeof(sl_char8));
		Base::copyMemory(mem + len1, utf8, len2*sizeof(sl_char8));
	}
	String8 ret;
	ret.m_buf = mem;
	return ret;
}

String16 String16::operator+(const sl_char8* utf8) const
{
	String16 other(utf8);
	return (*this) + other;
}

String8 String8::operator+(const sl_char16* utf16) const
{
	String8 other(utf16);
	return (*this) + other;
}

String16 String16::operator+(const sl_char16* utf16) const
{
	if (utf16 == sl_null)
		return *this;
	String16 oper1(*this);
	sl_int32 len1, len2;
	len1 = oper1.getLength();
	len2 = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
	sl_uint32 len = len1 + len2;
	sl_char16* mem = alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, oper1.m_buf, len1*sizeof(sl_char16));
		Base::copyMemory(mem + len1, utf16, len2*sizeof(sl_char16));
	}
	String16 ret;
	ret.m_buf = mem;
	return ret;
}

String8 String8::operator+(const sl_char32* utf32) const
{
	String8 other(utf32);
	return (*this) + other;
}

String16 String16::operator+(const sl_char32* utf32) const
{
	if (utf32 == sl_null)
		return *this;
	String16 oper1(*this);
	sl_int32 len1, len2;
	len1 = oper1.getLength();
	len2 = _StringBase::getSz32Length(utf32, SLIB_STR_MAX_LEN);
	sl_uint32 len = len1 + len2;
	sl_char16* mem = alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, oper1.m_buf, len1*sizeof(sl_char16));
		copyUtf32ToUtf16(mem + len1, utf32, len2);
	}
	String16 ret;
	ret.m_buf = mem;
	return ret;
}

String8 operator+(const sl_char8* utf8, const String8& second)
{
	if (utf8 == sl_null)
		return second;
	String8 oper2(second);
	sl_int32 len1, len2;
	len1 = _StringBase::getSz8Length(utf8, SLIB_STR_MAX_LEN);
	len2 = oper2.getLength();
	sl_uint32 len = len1 + len2;
	sl_char8* mem = String8::alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, utf8, len1*sizeof(sl_char8));
		Base::copyMemory(mem + len1, oper2.m_buf, len2*sizeof(sl_char8));
	}
	String8 ret;
	ret.m_buf = mem;
	return ret;
}

String16 operator+(const sl_char8* utf8, const String16& second)
{
	String16 other(utf8);
	return other + second;
}

String8 operator+(const sl_char16* utf16, const String8& second)
{
	String8 other(utf16);
	return other + second;
}

String16 operator+(const sl_char16* utf16, const String16& second)
{
	if (utf16 == sl_null)
		return second;
	String16 oper2(second);
	sl_int32 len1, len2;
	len1 = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
	len2 = oper2.getLength();
	sl_uint32 len = len1 + len2;
	sl_char16* mem = String16::alloc(len);
	if (len > 0) {
		Base::copyMemory(mem, utf16, len1*sizeof(sl_char16));
		Base::copyMemory(mem + len1, oper2.m_buf, len2*sizeof(sl_char16));
	}
	String16 ret;
	ret.m_buf = mem;
	return ret;
}

String8 operator+(const sl_char32* utf32, const String8& second)
{
	String8 other(utf32);
	return other + second;
}

String16 operator+(const sl_char32* utf32, const String16& second)
{
	if (utf32 == sl_null)
		return second;
	String16 oper2(second);
	sl_int32 len1, len2;
	len1 = _StringBase::getSz32Length(utf32, SLIB_STR_MAX_LEN);
	len2 = oper2.getLength();
	sl_uint32 len = len1 + len2;
	sl_char16* mem = String16::alloc(len);
	if (len > 0) {
		String16::copyUtf32ToUtf16(mem, utf32, len1);
		Base::copyMemory(mem + len1, oper2.m_buf, len2*sizeof(sl_char16));
	}
	String16 ret;
	ret.m_buf = mem;
	return ret;
}

String8 String8::substring(sl_int32 start, sl_int32 end) const
{
	String8 copy = *this;
	sl_int32 count = copy.getLength();
	if (start < 0) {
		start = 0;
	}
	if (end < 0 || end > count) {
		end = count;
	}
	if (start >= end) {
		return String8::null();
	}
	if (start == 0 && end == count) {
		return copy;
	}
	String8 ret(copy.m_buf + start, end - start);
	return ret;
}

String16 String16::substring(sl_int32 start, sl_int32 end) const
{
	String16 copy = *this;
	sl_int32 count = copy.getLength();
	if (start < 0) {
		start = 0;
	}
	if (end < 0 || end > count) {
		end = count;
	}
	if (start >= end) {
		return String16::null();
	}
	if (start == 0 && end == count) {
		return copy;
	}
	String16 ret(copy.m_buf + start, end - start);
	return ret;
}

sl_int32 String8::indexOf(sl_char8 ch, sl_int32 start) const
{
	String8 copy = *this;
	if (copy.isEmpty()) {
		return -1;
	}
	sl_int32 count = copy.getLength();
	if (start < 0) {
		start = 0;
	}
	if (start >= count) {
		return -1;
	}
	sl_char8* pt = (sl_char8*)(Base::findMemory(copy.m_buf + start, ch, count - start));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - copy.m_buf);
	}
}

sl_int32 String16::indexOf(sl_char16 ch, sl_int32 start) const
{
	String16 copy = *this;
	if (copy.isEmpty()) {
		return -1;
	}
	sl_int32 count = copy.getLength();
	if (start < 0) {
		start = 0;
	}
	if (start >= count) {
		return -1;
	}
	sl_char16* pt = (sl_char16*)(Base::findMemory2(copy.m_buf + start, ch, count - start));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - copy.m_buf);
	}
}

sl_int32 String8::lastIndexOf(sl_char8 ch, sl_int32 start) const
{
	String8 copy = *this;
	if (copy.isEmpty()) {
		return -1;
	}
	sl_int32 count = copy.getLength();
	if (start < 0 || start >= count) {
		start = count - 1;
	}
	sl_char8* pt = (sl_char8*)(Base::findMemoryReverse(copy.m_buf, ch, start + 1));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - copy.m_buf);
	}
}

sl_int32 String16::lastIndexOf(sl_char16 ch, sl_int32 start) const
{
	String16 copy = *this;
	if (copy.isEmpty()) {
		return -1;
	}
	sl_int32 count = copy.getLength();
	if (start < 0 || start >= count) {
		start = count - 1;
	}
	sl_char16* pt = (sl_char16*)(Base::findMemoryReverse2(copy.m_buf, ch, start + 1));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - copy.m_buf);
	}
}

template <class ST, class CT, class TT>
SLIB_INLINE sl_int32 _String_indexOf(const ST& str, const ST& _pattern, sl_int32 start)
{
	ST pattern = _pattern;
	if (pattern.isEmpty()) {
		return 0;
	}
	ST copy = str;
	if (copy.isEmpty()) {
		return -1;
	}
	const CT* bufPat = pattern.getBuf();
	sl_int32 countPat = pattern.getLength();
	sl_int32 count = copy.getLength();
	if (start < 0) {
		start = 0;
	}
	if (start > count - countPat) {
		return -1;
	}
	while (start <= count - countPat) {
		const CT* pt = (const CT*)(TT::findMemory(copy.getBuf() + start, bufPat[0], count - start - countPat + 1));
		if (pt == sl_null)
			return -1;
		if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
			return (sl_int32)(pt - copy.getBuf());
		} else {
			start = (sl_int32)(pt - copy.getBuf() + 1);
		}
	}
	return -1;
}

sl_int32 String8::indexOf(const String8& pattern, sl_int32 start) const
{
	return _String_indexOf<String8, sl_char8, _TemplateFunc8>(*this, pattern, start);
}

sl_int32 String16::indexOf(const String16& pattern, sl_int32 start) const
{
	return _String_indexOf<String16, sl_char16, _TemplateFunc16>(*this, pattern, start);
}

template <class ST, class CT, class TT>
SLIB_INLINE sl_int32 _String_lastIndexOf(const ST& str, const ST& _pattern, sl_int32 start)
{
	ST pattern = _pattern;
	if (pattern.isEmpty()) {
		return -1;
	}
	ST copy = str;
	if (copy.isEmpty()) {
		return -1;
	}
	sl_int32 count = copy.getLength();
	sl_int32 countPat = pattern.getLength();
	const CT* bufPat = pattern.getBuf();
	if (start < 0 || start > count - countPat) {
		start = count - countPat;
	}
	while (start >= 0) {
		const CT* pt = (const CT*)(TT::findMemoryReverse(copy.getBuf(), bufPat[0], start + 1));
		if (pt == sl_null)
			return -1;
		if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
			return (sl_int32)(pt - copy.getBuf());
		} else {
			start = (sl_int32)(pt - copy.getBuf() - 1);
		}
	}
	return -1;
}

sl_int32 String8::lastIndexOf(const String8& pattern, sl_int32 start) const
{
	return _String_lastIndexOf<String8, sl_char8, _TemplateFunc8>(*this, pattern, start);
}

sl_int32 String16::lastIndexOf(const String16& pattern, sl_int32 start) const
{
	return _String_lastIndexOf<String16, sl_char16, _TemplateFunc16>(*this, pattern, start);
}

sl_bool String8::startsWith(sl_char8 ch) const
{
	String8 copy = *this;
	if (copy.isEmpty()) {
		return sl_false;
	} else {
		return copy[0] == ch;
	}
}

sl_bool String16::startsWith(sl_char16 ch) const
{
	String16 copy = *this;
	if (copy.isEmpty()) {
		return sl_false;
	} else {
		return copy[0] == ch;
	}
}

sl_bool String8::startsWith(const String8& _str) const
{
	String8 str = _str;
	sl_int32 count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	String8 copy = *this;
	sl_int32 count1 = copy.getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory(copy.getBuf(), str.getBuf(), count2) == 0;
	}
}

sl_bool String16::startsWith(const String16& _str) const
{
	String16 str = _str;
	sl_int32 count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	String16 copy = *this;
	sl_int32 count1 = copy.getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory2(copy.getBuf(), str.getBuf(), count2) == 0;
	}
}

sl_bool String8::endsWith(sl_char8 ch) const
{
	String8 copy = *this;
	sl_int32 count = copy.getLength();
	if (count < 1) {
		return sl_false;
	} else {
		return copy[count - 1] == ch;
	}
}

sl_bool String16::endsWith(sl_char16 ch) const
{
	String16 copy = *this;
	sl_int32 count = copy.getLength();
	if (count < 1) {
		return sl_false;
	} else {
		return copy[count - 1] == ch;
	}
}

sl_bool String8::endsWith(const String8& _str) const
{
	String8 str = _str;
	sl_int32 count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	String8 copy = *this;
	sl_int32 count1 = copy.getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory(copy.getBuf() + count1 - count2, str.getBuf(), count2) == 0;
	}
}

sl_bool String16::endsWith(const String16& _str) const
{
	String16 str = _str;
	sl_int32 count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	String16 copy = *this;
	sl_int32 count1 = copy.getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory2(copy.getBuf() + count1 - count2, str.getBuf(), count2) == 0;
	}
}

void String8::makeUpper()
{
	String8 oper = *this;
	sl_char8* buf = oper.m_buf;
	sl_int32 n = oper.getLength();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] = buf[i] - ('a' - 'A');
		}
	}
}

void String16::makeUpper()
{
	String16 oper = *this;
	sl_char16* buf = oper.m_buf;
	sl_int32 n = oper.getLength();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] = buf[i] - ('a' - 'A');
		}
	}
}

void String8::makeLower()
{
	String8 oper = *this;
	sl_char8* buf = oper.m_buf;
	sl_int32 n = oper.getLength();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'A' && buf[i] <= 'Z') {
			buf[i] = buf[i] + ('a' - 'A');
		}
	}
}

void String16::makeLower()
{
	String16 oper = *this;
	sl_char16* buf = oper.m_buf;
	sl_int32 n = oper.getLength();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'A' && buf[i] <= 'Z') {
			buf[i] = buf[i] + ('a' - 'A');
		}
	}
}

String8 String8::toUpper() const
{
	String8 ret = duplicate();
	ret.makeUpper();
	return ret;
}

String16 String16::toUpper() const
{
	String16 ret = duplicate();
	ret.makeUpper();
	return ret;
}

String8 String8::toLower() const
{
	String8 ret = duplicate();
	ret.makeLower();
	return ret;
}

String16 String16::toLower() const
{
	String16 ret = duplicate();
	ret.makeLower();
	return ret;
}

struct STRING_REPLACE_SUBSET
{
	sl_int32 start;
	sl_int32 len;

	STRING_REPLACE_SUBSET() {}
	STRING_REPLACE_SUBSET(const STRING_REPLACE_SUBSET& o)
	{
		start = o.start;
		len = o.len;
	}
};
template <class ST, class CT, class TT>
SLIB_INLINE ST _String_replaceAll(const ST& _str, const ST& _pattern, const ST& _replacement)
{
	ST pattern = _pattern;
	if (pattern.isEmpty()) {
		return String16::null();
	}
	ST replacement = _replacement;
	ST str = _str;
	if (str.isEmpty()) {
		return String16::null();
	}
	Queue<STRING_REPLACE_SUBSET, sl_false> queue;
	STRING_REPLACE_SUBSET subset;
	sl_int32 count = str.getLength();
	sl_int32 countPat = pattern.getLength();
	sl_int32 countReplace = replacement.getLength();
	const CT* buf = str.getBuf();
	const CT* bufReplace = replacement.getBuf();
	sl_int32 size = 0;
	sl_int32 start = 0;
	while (start <= count + countPat - 1) {
		sl_int32 index = str.indexOf(pattern, start);
		if (index < 0) {
			index = count;
		} else {
			size += countReplace;
		}
		subset.start = start;
		subset.len = index - start;
		queue.push(subset);
		size += subset.len;
		start = index + countPat;
	}

	ST ret = ST::memory(size);
	if (ret.isNotNull()) {
		CT* out = ret.getBuf();
		while (queue.pop(&subset)) {
			Base::copyMemory(out, buf + subset.start, subset.len*sizeof(CT));
			out += subset.len;
			if (queue.isNotEmpty()) {
				Base::copyMemory(out, bufReplace, countReplace*sizeof(CT));
				out += countReplace;
			}
		}
	}
	return ret;
}

String8 String8::replaceAll(const String8& pattern, const String8& replacement) const
{
	return _String_replaceAll<String8, sl_char8, _TemplateFunc8>(*this, pattern, replacement);
}

String16 String16::replaceAll(const String16& pattern, const String16& replacement) const
{
	return _String_replaceAll<String16, sl_char16, _TemplateFunc16>(*this, pattern, replacement);
}

template <class ST, class CT>
SLIB_INLINE ST _String_trim(const ST& _str)
{
	ST str = _str;
	const CT* sz = str.getBuf();
	sl_uint32 n = str.getLength();
	sl_uint32 i = 0;
	for (; i < n; i++) {
		CT c = sz[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			break;
		}
	}
	if (i >= n) {
		return ST::null();
	}
	sl_uint32 j = n - 1;
	for (; j >= i; j--) {
		CT c = sz[j];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			break;
		}
	}
	return str.substring(i, j + 1);
}

String8 String8::trim() const
{
	return _String_trim<String8, sl_char8>(*this);
}

String16 String16::trim() const
{
	return _String_trim<String16, sl_char16>(*this);
}

template <class ST, class CT>
SLIB_INLINE ST _String_trimLeft(const ST& _str)
{
	ST str = _str;
	const CT* sz = str.getBuf();
	sl_uint32 n = str.getLength();
	sl_uint32 i = 0;
	for (; i < n; i++) {
		CT c = sz[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			break;
		}
	}
	if (i >= n) {
		return ST::null();
	}
	return str.substring(i);
}

String8 String8::trimLeft() const
{
	return _String_trimLeft<String8, sl_char8>(*this);
}

String16 String16::trimLeft() const
{
	return _String_trimLeft<String16, sl_char16>(*this);
}

template <class ST, class CT>
SLIB_INLINE ST _String_trimRight(const ST& _str)
{
	ST str = _str;
	const CT* sz = str.getBuf();
	sl_uint32 n = str.getLength();
	sl_uint32 j = n;
	for (; j > 0; j--) {
		CT c = sz[j-1];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			break;
		}
	}
	if (j == 0) {
		return ST::null();
	}
	return str.substring(0, j);
}

String8 String8::trimRight() const
{
	return _String_trimRight<String8, sl_char8>(*this);
}

String16 String16::trimRight() const
{
	return _String_trimRight<String16, sl_char16>(*this);
}

template <class ST>
List<ST> _String_split(const ST& _str, const ST& _pattern)
{
	ST str = _str;
	ST pattern = _pattern;
	List<ST> ret;
	if (pattern.getLength() == 0) {
		return ret;
	}
	sl_int32 start = 0;
	while (1) {
		sl_int32 index = str.indexOf(pattern, start);
		if (index < 0) {
			ret.add(str.substring(start));
			break;
		}
		ret.add(str.substring(start, index));
		start = index + pattern.getLength();
	}
	return ret;
}

List<String8> String8::split(const String8& pattern) const
{
	return _String_split<String8>(*this, pattern);
}

List<String16> String16::split(const String16& pattern) const
{
	return _String_split<String16>(*this, pattern);
}

sl_int32 String8::compare(const String8& other, sl_uint32 len) const
{
	if (m_buf == other.m_buf) {
		return 0;
	}
	String8 oper1 = *this, oper2 = other;
	sl_uint32 l1 = oper1.getLength();
	if (l1 > len) {
		l1 = len;
	}
	sl_uint32 l2 = oper2.getLength();
	if (l2 > len) {
		l2 = len;
	}
	return compare8(oper1.getBuf(), l1, oper2.getBuf(), l2);
}

sl_int32 String16::compare(const String16& other, sl_uint32 len) const
{
	if (m_buf == other.m_buf) {
		return 0;
	}
	String16 oper1 = *this, oper2 = other;
	sl_uint32 l1 = oper1.getLength();
	if (l1 > len) {
		l1 = len;
	}
	sl_uint32 l2 = oper2.getLength();
	if (l2 > len) {
		l2 = len;
	}
	return compare16(oper1.getBuf(), l1, oper2.getBuf(), l2);
}

sl_int32 String8::compare8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = SLIB_STR_MAX_LEN;
	}
	if (len2 < 0) {
		len2 = SLIB_STR_MAX_LEN;
	}
	sl_int32 len = SLIB_MIN(len1, len2);
	sl_uint8* s1 = (sl_uint8*)str1;
	sl_uint8* s2 = (sl_uint8*)str2;
	for (sl_int32 i = 0; i < len; i++) {
		sl_uint8 c1 = s1[i];
		sl_uint8 c2 = s2[i];
		if (c1 < c2) {
			return -1;
		}
		if (c1 > c2) {
			return 1;
		}
		if (c1 == 0) {
			return 0;
		}
	}
	if (len1 < len2) {
		if (s2[len1] == 0) {
			return 0;
		} else {
			return -1;
		}
	}
	if (len1 > len2) {
		if (s1[len2] == 0) {
			return 0;
		} else {
			return 1;
		}
	}
	return 0;
}

sl_int32 String16::compare8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
{
	String16 other(str2, len2);
	return compare16(str1, len1, other.m_buf, other.getLength());
}

sl_int32 String8::compare16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return compare8(str1, len1, other.m_buf, other.getLength());
}

sl_int32 String16::compare16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = SLIB_STR_MAX_LEN;
	}
	if (len2 < 0) {
		len2 = SLIB_STR_MAX_LEN;
	}
	sl_int32 len = SLIB_MIN(len1, len2);
	sl_uint16* s1 = (sl_uint16*)str1;
	sl_uint16* s2 = (sl_uint16*)str2;
	for (sl_int32 i = 0; i < len; i++) {
		sl_uint16 c1 = s1[i];
		sl_uint16 c2 = s2[i];
		if (c1 < c2) {
			return -1;
		}
		if (c1 > c2) {
			return 1;
		}
		if (c1 == 0) {
			return 0;
		}
	}
	if (len1 < len2) {
		if (s2[len1] == 0) {
			return 0;
		} else {
			return -1;
		}
	}
	if (len1 > len2) {
		if (s1[len2] == 0) {
			return 0;
		} else {
			return 1;
		}
	}
	return 0;
}

sl_int32 String8::compare32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return compare8(str1, len1, other.m_buf, other.getLength());
}

sl_int32 String16::compare32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	return compareUtf16ToUtf32(str1, len1, str2, len2);
}

sl_bool String8::equals8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = SLIB_STR_MAX_LEN;
	}
	if (len2 < 0) {
		len2 = SLIB_STR_MAX_LEN;
	}
	sl_int32 len = SLIB_MIN(len1, len2);
	sl_uint8* s1 = (sl_uint8*)str1;
	sl_uint8* s2 = (sl_uint8*)str2;
	for (sl_int32 i = 0; i < len; i++) {
		sl_uint8 c1 = s1[i];
		sl_uint8 c2 = s2[i];
		if (c1 != c2) {
			return sl_false;
		}
		if (c1 == 0) {
			return sl_true;
		}
	}
	if (len1 < len2) {
		if (s2[len1] == 0) {
			return sl_true;
		} else {
			return sl_false;
		}
	}
	if (len1 > len2) {
		if (s1[len2] == 0) {
			return sl_true;
		} else {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool String16::equals8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
{
	String16 other(str2, len2);
	return equals16(str1, len1, other.m_buf, other.getLength());
}

sl_bool String8::equals16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return equals8(str1, len1, other.m_buf, other.getLength());
}

sl_bool String16::equals16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = SLIB_STR_MAX_LEN;
	}
	if (len2 < 0) {
		len2 = SLIB_STR_MAX_LEN;
	}
	sl_int32 len = SLIB_MIN(len1, len2);
	sl_uint16* s1 = (sl_uint16*)str1;
	sl_uint16* s2 = (sl_uint16*)str2;
	for (sl_int32 i = 0; i < len; i++) {
		sl_uint16 c1 = s1[i];
		sl_uint16 c2 = s2[i];
		if (c1 != c2) {
			return sl_false;
		}
		if (c1 == 0) {
			return sl_true;
		}
	}
	if (len1 < len2) {
		if (s2[len1] == 0) {
			return sl_true;
		} else {
			return sl_false;
		}
	}
	if (len1 > len2) {
		if (s1[len2] == 0) {
			return sl_true;
		} else {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool String8::equals32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return equals8(str1, len1, other.m_buf, other.getLength());
}

sl_bool String16::equals32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	return compareUtf16ToUtf32(str1, len1, str2, len2) == 0;
}

sl_int32 String8::compare(const String16& _other) const
{
	String8 other(_other);
	return compare(other);
}

sl_int32 String16::compare(const String8& _other) const
{
	String16 other(_other);
	return compare(other);
}

sl_bool String8::equals(const String16& _other) const
{
	String8 other(_other);
	return equals(other);
}

sl_bool String16::equals(const String8& _other) const
{
	String16 other(_other);
	return equals(other);
}

sl_char8* String8::alloc(sl_uint32& length)
{
	if (length == 0) {
		return _String8_emptyBuf;
	}
	sl_int8* mem = (sl_int8*)(Base::createMemory((length + 1)*sizeof(sl_char8)+SLIB_STR_HEADER_LEN));
	if (mem) {
		mem += SLIB_STR_HEADER_LEN;
		sl_char8* ret = (sl_char8*)mem;
		*(SLIB_STR_PLEN(ret)) = length;
		*(SLIB_STR_PREF(ret)) = 1;
		*(SLIB_STR_PHASH(ret)) = 0;
		ret[length] = 0;
		return ret;
	} else {
		length = 0;
		return _String8_nullBuf;
	}
}

sl_char16* String16::alloc(sl_uint32& length)
{
	if (length == 0) {
		return _String16_emptyBuf;
	}
	sl_int8* mem = (sl_int8*)(Base::createMemory((length + 1)*sizeof(sl_char16)+SLIB_STR_HEADER_LEN));
	if (mem) {
		mem += SLIB_STR_HEADER_LEN;
		sl_char16* ret = (sl_char16*)mem;
		*(SLIB_STR_PLEN(ret)) = length;
		*(SLIB_STR_PREF(ret)) = 1;
		*(SLIB_STR_PHASH(ret)) = 0;
		ret[length] = 0;
		return ret;
	} else {
		length = 0;
		return _String16_nullBuf;
	}
}

void String8::destroy(sl_char8* buf)
{
	if (buf != _String8_nullBuf) {
		Base::freeMemory((sl_char8*)(buf)-SLIB_STR_HEADER_LEN);
	} else {
		*SLIB_STR_PREF(buf) = SLIB_SIZE_MAX >> 1;
	}
}

void String16::destroy(sl_char16* buf)
{
	if (buf != _String16_nullBuf) {
		Base::freeMemory((sl_char8*)(buf)-SLIB_STR_HEADER_LEN);
	} else {
		*SLIB_STR_PREF(buf) = SLIB_SIZE_MAX >> 1;
	}
}

String8 String8::memory(sl_uint32 len)
{
	String8 ret;
	ret.m_buf = alloc(len);
	return ret;
}

String16 String16::memory(sl_uint32 len)
{
	String16 ret;
	ret.m_buf = alloc(len);
	return ret;
}

Memory String8::toMemory() const
{
	String8 oper = *this;
	return Memory::create(oper.getBuf(), oper.getLength());
}

Memory String16::toMemory() const
{
	String16 oper = *this;
	return Memory::create(oper.getBuf(), oper.getLength()*sizeof(sl_char16));
}

sl_uint32 String8::getUtf16(sl_char16* utf16, sl_int32 len) const
{
	String8 oper = *this;
	return _StringBase::utf8ToUtf16(oper.m_buf, oper.getLength(), utf16, len);
}

StringData String8::getUtf16() const
{
	StringData ret;
	ret.mem = toUtf16();
	if (ret.mem.isNotEmpty()) {
		ret.sz16 = (const sl_char16*)(ret.mem.getBuf());
		ret.len = (sl_uint32)(ret.mem.getSize() / 2 - 1);
		if (!ret.sz16) {
			ret.sz16 = (sl_char16*)("\0\0\0\0");
			ret.len = 0;
		}
	} else {
		ret.sz16 = (sl_char16*)("\0\0\0\0");
		ret.len = 0;
	}
	return ret;
}

Memory String8::toUtf16() const
{
	String8 oper = *this;
	sl_int32 len = _StringBase::utf8ToUtf16(oper.m_buf, oper.getLength(), sl_null, -1);
	Memory memory = Memory::create(2 * (len + 1));
	if (memory.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(memory.getBuf());
		_StringBase::utf8ToUtf16(oper.m_buf, oper.getLength(), buf, len);
		buf[len] = 0;
	}
	return memory;
}

sl_uint32 String8::getUtf32(sl_char32* utf32, sl_int32 len) const
{
	String8 oper = *this;
	return _StringBase::utf8ToUtf32(oper.m_buf, oper.getLength(), utf32, len);
}

StringData String8::getUtf32() const
{
	StringData ret;
	ret.mem = toUtf32();
	if (ret.mem.isNotEmpty()) {
		ret.sz32 = (const sl_char32*)(ret.mem.getBuf());
		ret.len = (sl_uint32)(ret.mem.getSize() / 4 - 1);
		if (!ret.sz32) {
			ret.sz32 = (sl_char32*)("\0\0\0\0");
			ret.len = 0;
		}
	} else {
		ret.sz32 = (sl_char32*)("\0\0\0\0");
		ret.len = 0;
	}
	return ret;
}

Memory String8::toUtf32() const
{
	String8 oper = *this;
	sl_int32 len = _StringBase::utf8ToUtf32(oper.m_buf, oper.getLength(), sl_null, -1);
	Memory memory = Memory::create(4 * (len + 1));
	if (memory.isNotEmpty()) {
		sl_char32* buf = (sl_char32*)(memory.getBuf());
		_StringBase::utf8ToUtf32(oper.m_buf, oper.getLength(), buf, len);
		buf[len] = 0;
	}
	return memory;
}

sl_uint32 String16::getUtf8(sl_char8* utf8, sl_int32 len) const
{
	String16 oper = *this;
	return _StringBase::utf16ToUtf8(oper.m_buf, oper.getLength(), utf8, len);
}

StringData String16::getUtf8() const
{
	StringData ret;
	ret.mem = toUtf8();
	if (ret.mem.isNotEmpty()) {
		ret.sz8 = (const sl_char8*)(ret.mem.getBuf());
		ret.len = (sl_uint32)(ret.mem.getSize() - 1);
		if (!ret.sz8) {
			ret.sz8 = "";
			ret.len = 0;
		}
	} else {
		ret.sz8 = "";
		ret.len = 0;
	}
	return ret;
}

Memory String16::toUtf8() const
{
	String16 oper = *this;
	sl_int32 len = _StringBase::utf16ToUtf8(oper.m_buf, oper.getLength(), sl_null, -1);
	Memory memory = Memory::create(len + 1);
	if (memory.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(memory.getBuf());
		_StringBase::utf16ToUtf8(oper.m_buf, oper.getLength(), buf, len);
		buf[len] = 0;
	}
	return memory;
}

void String16::copyUtf32ToUtf16(sl_char16* utf16, const sl_char32* utf32, sl_int32 count)
{
	for (sl_int32 i = 0; i < count; i++) {
		sl_char16 ch = (sl_char16)(utf32[i]);
		utf16[i] = ch;
	}
}

sl_int32 String16::compareUtf16ToUtf32(const sl_char16* m1, sl_int32 len1, const sl_char32* m2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = SLIB_STR_MAX_LEN;
	}
	if (len2 < 0) {
		len2 = SLIB_STR_MAX_LEN;
	}
	sl_int32 len = SLIB_MIN(len1, len2);
	sl_uint16* s1 = (sl_uint16*)m1;
	sl_uint32* s2 = (sl_uint32*)m2;
	for (sl_int32 i = 0; i < len; i++) {
		sl_uint32 c1 = s1[i];
		sl_uint32 c2 = s2[i];
		if (c1 < c2) {
			return -1;
		}
		if (c1 > c2) {
			return 1;
		}
		if (c1 == 0) {
			return 0;
		}
	}
	if (len1 < len2) {
		if (s2[len1] == 0) {
			return 0;
		} else {
			return -1;
		}
	}
	if (len1 > len2) {
		if (s1[len2] == 0) {
			return 0;
		} else {
			return 1;
		}
	}
	return 0;
}

sl_int32 String8::utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer)
{
	return _StringBase::utf8ToUtf16(utf8, lenUtf8, utf16, lenUtf16Buffer);
}

sl_int32 String16::utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer)
{
	return _StringBase::utf8ToUtf16(utf8, lenUtf8, utf16, lenUtf16Buffer);
}

sl_int32 String8::utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer)
{
	return _StringBase::utf8ToUtf32(utf8, lenUtf8, utf32, lenUtf32Buffer);
}

sl_int32 String16::utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer)
{
	return _StringBase::utf8ToUtf32(utf8, lenUtf8, utf32, lenUtf32Buffer);
}

sl_int32 String8::utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer)
{
	return _StringBase::utf16ToUtf8(utf16, lenUtf16, utf8, lenUtf8Buffer);
}

sl_int32 String16::utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer)
{
	return _StringBase::utf16ToUtf8(utf16, lenUtf16, utf8, lenUtf8Buffer);
}

sl_int32 String8::utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer)
{
	return _StringBase::utf32ToUtf8(utf32, lenUtf32, utf8, lenUtf8Buffer);
}

sl_int32 String16::utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer)
{
	return _StringBase::utf32ToUtf8(utf32, lenUtf32, utf8, lenUtf8Buffer);
}

sl_uint32 String8::hashCode() const
{
	String8 s = *this;
	sl_char8* start = s.m_buf;
	sl_char8* buf = start;
	sl_uint32 n = *SLIB_STR_PLEN(start);
	if (n == 0) {
		return 0;
	}
	sl_uint32 hash = *SLIB_STR_PHASH(start);
	if (hash == 0) {
		while (n > 0) {
			sl_uint32 ch = *buf;
			hash = hash * 31 + ch;
			buf++;
			n--;
		}
		hash = sl_rehash(hash);
		*SLIB_STR_PHASH(start) = hash;
	}
	return hash;
}

sl_uint32 String16::hashCode() const
{
	String16 s = *this;
	sl_char16* start = s.m_buf;
	sl_char16* buf = start;
	sl_uint32 n = *SLIB_STR_PLEN(start);
	if (n == 0) {
		return 0;
	}
	sl_uint32 hash = *SLIB_STR_PHASH(start);
	if (hash == 0) {
		while (n > 0) {
			sl_uint32 ch = *buf;
			hash = hash * 31 + ch;
			buf++;
			n--;
		}
		hash = sl_rehash(hash);
		*SLIB_STR_PHASH(start) = hash;
	}
	return hash;
}

template <class ST, class CT>
static sl_uint32 _String_applyBackslashEscapes(const ST& s, sl_bool flagDoubleQuote, CT* buf)
{
	const CT* ch = s.getBuf();
	sl_uint32 len = s.getLength();
	sl_uint32 d = 1;
	if (buf) {
		buf[0] = flagDoubleQuote ? '"' : '\'';
	}
	for (sl_uint32 i = 0; i < len; i++) {
		CT c = ch[i];
		CT r = 0;
		switch (c) {
		case '\\':
			r = c;
			break;
		case '"':
			if (flagDoubleQuote) {
				r = c;
			}
			break;
		case '\'':
			if (! flagDoubleQuote) {
				r = c;
			}
			break;
		case 0:
			r = '0';
			break;
		case '\n':
			r = 'n';
			break;
		case '\r':
			r = 'r';
			break;
		case '\b':
			r = 'b';
			break;
		case '\f':
			r = 'f';
			break;
		case '\a':
			r = 'a';
			break;
		case '\v':
			r = 'v';
			break;
		}
		if (r) {
			if (buf) {
				buf[d++] = '\\';
				buf[d++] = r;
			} else {
				d += 2;
			}
		} else {
			if (buf) {
				buf[d++] = c;
			} else {
				d++;
			}
		}
	}
	if (buf) {
		buf[d++] = flagDoubleQuote ? '"' : '\'';
	} else {
		d++;
	}
	return d;
}

String8 String8::applyBackslashEscapes(sl_bool flagDoubleQuote)
{
	String8 s = *this;
	sl_uint32 n = _String_applyBackslashEscapes<String8, sl_char8>(s, flagDoubleQuote, sl_null);
	String8 ret = memory(n);
	if (ret.isEmpty()) {
		return String8::null();
	}
	_String_applyBackslashEscapes<String8, sl_char8>(s, flagDoubleQuote, ret.getBuf());
	return ret;
}

String16 String16::applyBackslashEscapes(sl_bool flagDoubleQuote)
{
	String16 s = *this;
	sl_uint32 n = _String_applyBackslashEscapes<String16, sl_char16>(s, flagDoubleQuote, sl_null);
	String16 ret = memory(n);
	if (ret.isEmpty()) {
		return String16::null();
	}
	_String_applyBackslashEscapes<String16, sl_char16>(s, flagDoubleQuote, ret.getBuf());
	return ret;
}

template <class ST, class CT>
SLIB_INLINE ST _String_parseBackslashEscapes(const CT* sz, sl_int32 n, sl_int32* lengthParsed, sl_bool* outFlagError)
{
	if (lengthParsed) {
		*lengthParsed = 0;
	}
	if (outFlagError) {
		*outFlagError = sl_true;
	}
	if (n <= 0) {
		return ST::null();
	}
	sl_bool flagDoubleQuote = sl_false;
	CT chEnd = 0;
	if (sz[0] == '"') {
		flagDoubleQuote = sl_true;
		chEnd = '"';
	} else if (sz[0] == '\'') {
		flagDoubleQuote = sl_false;
		chEnd = '\'';
	} else {
		return ST::null();
	}
	CT* buf = (CT*)(Base::createMemory(n * sizeof(CT)));
	if (buf == sl_null) {
		return ST::null();
	}
	sl_int32 len = 0;
	sl_bool flagSuccess = sl_false;
	sl_int32 i = 1;
	for (; i < n; i++) {
		CT ch = sz[i];
		sl_bool flagError = sl_false;
		sl_bool flagBackslash = sl_false;
		switch (ch) {
		case '\\':
			flagBackslash = sl_true;
			i++;
			if (i < n) {
				ch = sz[i];
				switch (ch) {
				case '0':
					ch = 0;
					break;
				case '\\':
				case '"':
				case '\'':
					break;
				case 'n':
					ch = '\n';
					break;
				case 'r':
					ch = '\r';
					break;
				case 't':
					ch = '\t';
					break;
				case 'b':
					ch = '\b';
					break;
				case 'f':
					ch = '\f';
					break;
				case 'a':
					ch = '\a';
					break;
				default:
					flagError = sl_true;
					break;
				}
			} else {
				flagError = sl_true;
			}
			break;
		case 0:
		case '\r':
		case '\n':
		case '\v':
			flagError = sl_true;
			break;
		}
		if (flagError) {
			break;
		} else {
			if (ch == chEnd && ! flagBackslash) {
				flagSuccess = sl_true;
				i++;
				break;
			} else {
				buf[len++] = ch;
			}
		}
	}
	if (lengthParsed) {
		*lengthParsed = i;
	}
	ST ret;
	if (flagSuccess) {
		ret = ST(buf, len);
		if (outFlagError) {
			*outFlagError = sl_false;
		}
	}
	Base::freeMemory(buf);
	return ret;
}

String8 String8::parseBackslashEscapes(const sl_char8* sz, sl_int32 n, sl_int32* lengthParsed, sl_bool* outFlagError)
{
	return _String_parseBackslashEscapes<String8, sl_char8>(sz, n, lengthParsed, outFlagError);
}

String16 String16::parseBackslashEscapes(const sl_char16* sz, sl_int32 n, sl_int32* lengthParsed, sl_bool* outFlagError)
{
	return _String_parseBackslashEscapes<String16, sl_char16>(sz, n, lengthParsed, outFlagError);
}

StringBuffer8::StringBuffer8()
{
	m_len = 0;
}

StringBuffer16::StringBuffer16()
{
	m_len = 0;
}

String8 StringBuffer8::merge() const
{
	MutexLocker lock(getLocker());
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<String8>* begin = m_queue.getBegin();
	if (m_queue.count() == 1) {
		return begin->value;
	}
	sl_uint32 total = m_len;
	String8 ret = String8::memory(total);
	if (ret.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(ret.getBuf());
		sl_uint32 offset = 0;
		Link<String8>* item = begin;
		while (item) {
			String8& s = item->value;
			sl_uint32 t = s.getLength();
			Base::copyMemory(buf + offset, s.getBuf(), t * sizeof(sl_char8));
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

String16 StringBuffer16::merge() const
{
	MutexLocker lock(getLocker());
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<String16>* begin = m_queue.getBegin();
	if (m_queue.count() ==1) {
		return begin->value;
	}
	sl_uint32 total = m_len;
	String16 ret = String16::memory(total);
	if (ret.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(ret.getBuf());
		sl_uint32 offset = 0;
		Link<String16>* item = begin;
		while (item) {
			String16& s = item->value;
			sl_uint32 t = s.getLength();
			Base::copyMemory(buf + offset, s.getBuf(), t * 2);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

sl_bool StringBuffer8::add(const String8& str)
{
	sl_int32 len = str.getLength();
	if (len == 0) {
		return sl_true;
	}
	MutexLocker lock(getLocker());
	if (m_len + len < SLIB_STR_MAX_LEN) {
		m_queue.push(str);
		m_len += len;
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool StringBuffer16::add(const String16& str)
{
	sl_int32 len = str.getLength();
	if (len == 0) {
		return sl_true;
	}
	MutexLocker lock(getLocker());
	if (m_len + len < SLIB_STR_MAX_LEN) {
		m_queue.push(str);
		m_len += len;
		return sl_true;
	} else {
		return sl_false;
	}
}


StringDataBuffer8::StringDataBuffer8()
{
	m_len = 0;
}

StringDataBuffer16::StringDataBuffer16()
{
	m_len = 0;
}

String8 StringDataBuffer8::merge() const
{
	MutexLocker lock(getLocker());
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_uint32 total = m_len;
	String8 ret = String8::memory(total);
	if (ret.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(ret.getBuf());
		sl_uint32 offset = 0;
		Link<StringData>* item = begin;
		while (item) {
			StringData& s = item->value;
			sl_uint32 t = s.len;
			Base::copyMemory(buf + offset, s.sz8, t * sizeof(sl_char8));
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

String16 StringDataBuffer16::merge() const
{
	MutexLocker lock(getLocker());
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_uint32 total = m_len;
	String16 ret = String16::memory(total);
	if (ret.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(ret.getBuf());
		sl_uint32 offset = 0;
		Link<StringData>* item = begin;
		while (item) {
			StringData& s = item->value;
			sl_uint32 t = s.len;
			Base::copyMemory(buf + offset, s.sz16, t * 2);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

sl_bool StringDataBuffer8::add(const StringData& str)
{
	sl_int32 len = str.len;
	if (len == 0) {
		return sl_true;
	}
	MutexLocker lock(getLocker());
	if (m_len + len < SLIB_STR_MAX_LEN) {
		m_queue.push(str);
		m_len += len;
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool StringDataBuffer16::add(const StringData& str)
{
	sl_int32 len = str.len;
	if (len == 0) {
		return sl_true;
	}
	MutexLocker lock(getLocker());
	if (m_len + len < SLIB_STR_MAX_LEN) {
		m_queue.push(str);
		m_len += len;
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_int32 _StringBase::getSz8Length(const sl_char8* utf8, sl_int32 count)
{
	if (count == 0) {
		return 0;
	}
	if (count < 0) {
		count = SLIB_STR_MAX_LEN - 1;
	}
	for (sl_int32 i = 0; i < count; i++) {
		if (utf8[i] == 0) {
			return i;
		}
	}
	return count;
}

sl_int32 _StringBase::getSz16Length(const sl_char16* utf16, sl_int32 count)
{
	if (count == 0) {
		return 0;
	}
	if (count < 0) {
		count = SLIB_STR_MAX_LEN - 1;
	}
	for (sl_int32 i = 0; i < count; i++) {
		if (utf16[i] == 0) {
			return i;
		}
	}
	return count;
}

sl_int32 _StringBase::getSz32Length(const sl_char32* utf32, sl_int32 count)
{
	if (count == 0) {
		return 0;
	}
	if (count < 0) {
		count = SLIB_STR_MAX_LEN - 1;
	}
	for (sl_int32 i = 0; i < count; i++) {
		if (utf32[i] == 0) {
			return i;
		}
	}
	return count;
}

sl_uint32 _StringBase::utf8ToUtf16(const sl_char8* utf8, sl_int32 lenUtf8, sl_char16* utf16, sl_int32 lenUtf16Buffer)
{
	if (lenUtf8 < 0) {
		lenUtf8 = _StringBase::getSz8Length(utf8, -1) + 1;
	}
	sl_int32 n = 0;
	for (sl_int32 i = 0; i < lenUtf8 && (lenUtf16Buffer < 0 || n < lenUtf16Buffer); i++) {
		sl_uint32 ch = (sl_uint32)((sl_uint8)utf8[i]);
		if (ch < 0x80) {
			if (utf16) {
				utf16[n++] = (sl_char16)ch;
			} else {
				n++;
			}
		} else if (ch < 0xC0) {
			// Corrupted data element
		} else if (ch < 0xE0) {
			if (i < lenUtf8 - 1) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				if ((ch1 & 0xC0) == 0x80) {
					if (utf16) {
						utf16[n++] = (sl_char16)(((ch & 0x1F) << 6) | (ch1 & 0x3F));
					} else {
						n++;
					}
				}
			}
		} else if (ch < 0xF0) {
			if (i < lenUtf8 - 2) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
				if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80)) {
					if (utf16) {
						utf16[n++] = (sl_char16)(((ch & 0x0F) << 12) | ((ch1 & 0x3F) << 6) | (ch2 & 0x3F));
					} else {
						n++;
					}
				}
			}
		}
	}
	return n;
}

sl_uint32 _StringBase::utf8ToUtf32(const sl_char8* utf8, sl_int32 lenUtf8, sl_char32* utf32, sl_int32 lenUtf32Buffer)
{
	if (lenUtf8 < 0) {
		lenUtf8 = _StringBase::getSz8Length(utf8, -1) + 1;
	}
	sl_int32 n = 0;
	for (sl_int32 i = 0; i < lenUtf8 && (lenUtf32Buffer < 0 || n < lenUtf32Buffer); i++) {
		sl_uint32 ch = (sl_uint32)((sl_uint8)utf8[i]);
		if (ch < 0x80) {
			if (utf32) {
				utf32[n++] = (sl_char32)ch;
			} else {
				n++;
			}
		} else if (ch < 0xC0) {
			// Corrupted data element
		} else if (ch < 0xE0) {
			if (i < lenUtf8 - 1) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				if ((ch1 & 0xC0) == 0x80) {
					if (utf32) {
						utf32[n++] = (sl_char32)(((ch & 0x1F) << 6) | (ch1 & 0x3F));
					} else {
						n++;
					}
				}
			}
		} else if (ch < 0xF0) {
			if (i < lenUtf8 - 2) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
				if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80)) {
					if (utf32) {
						utf32[n++] = (sl_char32)(((ch & 0x0F) << 12) | ((ch1 & 0x3F) << 6) | (ch2 & 0x3F));
					} else {
						n++;
					}
				}
			}
		} else if (ch < 0xF8) {
			if (i < lenUtf8 - 3) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch3 = (sl_uint32)((sl_uint8)utf8[++i]);
				if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80)) {
					if (utf32) {
						utf32[n++] = (sl_char32)(((ch & 0x07) << 18) | ((ch1 & 0x3F) << 12) | ((ch2 & 0x3F) << 6) | (ch3 & 0x3F));
					} else {
						n++;
					}
				}
			}
		} else if (ch < 0xFC) {
			if (i < lenUtf8 - 4) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch3 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch4 = (sl_uint32)((sl_uint8)utf8[++i]);
				if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80) && ((ch4 & 0xC0) == 0x80)) {
					if (utf32) {
						utf32[n++] = (sl_char32)(((ch & 0x03) << 24) | ((ch1 & 0x3F) << 18) | ((ch2 & 0x3F) << 12) | ((ch3 & 0x3F) << 6) | (ch4 & 0x3F));
					} else {
						n++;
					}
				}
			}
		} else if (ch < 0xFE) {
			if (i < lenUtf8 - 5) {
				sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch3 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch4 = (sl_uint32)((sl_uint8)utf8[++i]);
				sl_uint32 ch5 = (sl_uint32)((sl_uint8)utf8[++i]);
				if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80) && ((ch4 & 0xC0) == 0x80) && ((ch5 & 0xC0) == 0x80)) {
					if (utf32) {
						utf32[n++] = (sl_char32)(((ch & 0x01) << 30) | ((ch1 & 0x3F) << 24) | ((ch2 & 0x3F) << 18) | ((ch3 & 0x3F) << 12) | ((ch4 & 0x3F) << 6) | (ch5 & 0x3F));
					} else {
						n++;
					}
				}
			}
		}
	}
	return n;
}

sl_uint32 _StringBase::utf16ToUtf8(const sl_char16* utf16, sl_int32 lenUtf16, sl_char8* utf8, sl_int32 lenUtf8Buffer)
{
	if (lenUtf16 < 0) {
		lenUtf16 = _StringBase::getSz16Length(utf16, -1) + 1;
	}
	sl_int32 n = 0;
	for (sl_int32 i = 0; i < lenUtf16 && (lenUtf8Buffer < 0 || n < lenUtf8Buffer); i++) {
		sl_uint32 ch = (sl_uint32)(utf16[i]);
		if (ch < 0x80) {
			if (utf8) {
				utf8[n++] = (sl_char8)(ch);
			} else {
				n++;
			}
		} else if (ch < 0x800) {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 1) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 6) | 0xC0);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 2;
				}
			}
		} else {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 2) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 12) | 0xE0);
					utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 3;
				}
			}
		}
	}
	return n;
}

sl_uint32 _StringBase::utf32ToUtf8(const sl_char32* utf32, sl_int32 lenUtf32, sl_char8* utf8, sl_int32 lenUtf8Buffer)
{
	if (lenUtf32 < 0) {
		lenUtf32 = _StringBase::getSz32Length(utf32, -1) + 1;
	}
	sl_int32 n = 0;
	for (sl_int32 i = 0; i < lenUtf32 && (lenUtf8Buffer < 0 || n < lenUtf8Buffer); i++) {
		sl_uint32 ch = (sl_uint32)(utf32[i]);
		if (ch < 0x80) {
			if (utf8) {
				utf8[n++] = (sl_char8)(ch);
			} else {
				n++;
			}
		} else if (ch < 0x800) {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 1) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 6) | 0xC0);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 2;
				}
			}
		} else if (ch < 0x10000) {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 2) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 12) | 0xE0);
					utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 3;
				}
			}
		} else if (ch < 0x200000) {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 3) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 18) | 0xF0);
					utf8[n++] = (sl_char8)(((ch >> 12) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 4;
				}
			}
		} else if (ch < 0x4000000) {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 4) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 24) | 0xF8);
					utf8[n++] = (sl_char8)(((ch >> 18) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)(((ch >> 12) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 5;
				}
			}
		} else if (ch < 0x80000000) {
			if (lenUtf8Buffer < 0 || n < lenUtf8Buffer - 5) {
				if (utf8) {
					utf8[n++] = (sl_char8)((ch >> 30) | 0xFC);
					utf8[n++] = (sl_char8)(((ch >> 24) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)(((ch >> 18) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)(((ch >> 12) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
					utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
				} else {
					n += 6;
				}
			}
		}
	}
	return n;
}

SLIB_NAMESPACE_END
