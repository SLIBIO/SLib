#include "../../../inc/slib/core/string.h"
#include "../../../inc/slib/core/queue.h"
#include "../../../inc/slib/core/endian.h"
#include "../../../inc/slib/core/scoped_pointer.h"


SLIB_NAMESPACE_BEGIN

SLIB_ALIGN(sl_uint32 _g_bufNullString[5], 8) = { 0x7f7f7f7f, 0x7f7f7f7f, 0, 0, 0 };
const _String_Const _String_Null = {((sl_uint8*)(void*)_g_bufNullString) + SLIB_STR_HEADER_LEN, 0};

SLIB_ALIGN(sl_uint32 _g_bufEmptyString[5], 8) = { 0x7f7f7f7f, 0x7f7f7f7f, 0, 0, 0 };
const _String_Const _String_Empty = {((sl_uint8*)(void*)_g_bufEmptyString) + SLIB_STR_HEADER_LEN, 0};

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

void String8::_destroy(sl_char8* data)
{
	if (data != (sl_char8*)(_String_Null.data) && data != (sl_char8*)(_String_Empty.data)) {
		Base::freeMemory((sl_char8*)(data)-SLIB_STR_HEADER_LEN);
	} else {
		*SLIB_STR_PREF(data) = SLIB_SIZE_MAX >> 1;
	}
}

void String16::_destroy(sl_char16* data)
{
	if (data != (sl_char16*)(_String_Null.data) && data != (sl_char16*)(_String_Empty.data)) {
		Base::freeMemory((sl_char8*)(data)-SLIB_STR_HEADER_LEN);
	} else {
		*SLIB_STR_PREF(data) = SLIB_SIZE_MAX >> 1;
	}
}

sl_char8* String8::_alloc(sl_uint32& length)
{
	if (length == 0) {
		return (sl_char8*)(_String_Empty.data);
	}
	if (length > SLIB_STR_MAX_LEN) {
		length = SLIB_STR_MAX_LEN;
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
		return (sl_char8*)(_String_Null.data);
	}
}

sl_char16* String16::_alloc(sl_uint32& length)
{
	if (length == 0) {
		return (sl_char16*)(_String_Empty.data);
	}
	if (length > SLIB_STR_MAX_LEN) {
		length = SLIB_STR_MAX_LEN;
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
		return (sl_char16*)(_String_Null.data);
	}
}

sl_char8* String8::_create(sl_char8 ch, sl_uint32 nRepeatCount)
{
	sl_char8* mem = _alloc(nRepeatCount);
	if (nRepeatCount > 0) {
		Base::resetMemory(mem, ch, nRepeatCount);
	}
	return mem;
}

sl_char16* String16::_create(sl_char16 ch, sl_uint32 nRepeatCount)
{
	sl_char16* mem = _alloc(nRepeatCount);
	if (nRepeatCount > 0) {
		Base::resetMemory2(mem, ch, nRepeatCount);
	}
	return mem;
}

sl_char8* String8::_create(const sl_char8* utf8, sl_int32 lenUtf8)
{
	if (utf8) {
		if (lenUtf8 < 0) {
			lenUtf8 = _StringBase::getSz8Length(utf8, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = lenUtf8;
		sl_char8* mem = _alloc(len);
		if (len > 0) {
			Base::copyMemory(mem, utf8, len*sizeof(sl_char8));
		}
		return mem;
	}
	return (sl_char8*)(_String_Null.data);
}

sl_char16* String16::_create(const sl_char8* utf8, sl_int32 lenUtf8)
{
	if (utf8) {
		if (lenUtf8 < 0) {
			lenUtf8 = _StringBase::getSz8Length(utf8, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = _StringBase::utf8ToUtf16(utf8, lenUtf8, sl_null, -1);
		sl_char16* mem = _alloc(len);
		if (len > 0) {
			_StringBase::utf8ToUtf16(utf8, lenUtf8, mem, len);
			mem[len] = 0;
		}
		return mem;
	}
	return (sl_char16*)(_String_Null.data);
}

sl_char8* String8::_create(const sl_char16* utf16, sl_int32 lenUtf16)
{
	if (utf16) {
		if (lenUtf16 < 0) {
			lenUtf16 = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = _StringBase::utf16ToUtf8(utf16, lenUtf16, sl_null, -1);
		sl_char8* mem = _alloc(len);
		if (len > 0) {
			_StringBase::utf16ToUtf8(utf16, lenUtf16, mem, len);
			mem[len] = 0;
		}
		return mem;
	}
	return (sl_char8*)(_String_Null.data);
}

sl_char16* String16::_create(const sl_char16* utf16, sl_int32 lenUtf16)
{
	if (utf16) {
		if (lenUtf16 < 0) {
			lenUtf16 = _StringBase::getSz16Length(utf16, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = lenUtf16;
		sl_char16* mem = _alloc(len);
		if (len > 0) {
			Base::copyMemory(mem, utf16, len*sizeof(sl_char16));
		}
		return mem;
	}
	return (sl_char16*)(_String_Null.data);
}

sl_char8* String8::_create(const sl_char32* utf32, sl_int32 lenUtf32)
{
	if (utf32) {
		if (lenUtf32 < 0) {
			lenUtf32 = _StringBase::getSz32Length(utf32, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = _StringBase::utf32ToUtf8(utf32, lenUtf32, sl_null, -1);
		sl_char8* mem = _alloc(len);
		if (len > 0) {
			_StringBase::utf32ToUtf8(utf32, lenUtf32, mem, len);
			mem[len] = 0;
		}
		return mem;
	}
	return (sl_char8*)(_String_Null.data);
}

static void _String_copyUtf32ToUtf16(sl_char16* utf16, const sl_char32* utf32, sl_int32 count)
{
	for (sl_int32 i = 0; i < count; i++) {
		sl_char16 ch = (sl_char16)(utf32[i]);
		utf16[i] = ch;
	}
}

sl_char16* String16::_create(const sl_char32* utf32, sl_int32 lenUtf32)
{
	if (utf32) {
		if (lenUtf32 < 0) {
			lenUtf32 = _StringBase::getSz32Length(utf32, SLIB_STR_MAX_LEN);
		}
		sl_uint32 len = lenUtf32;
		sl_char16* mem = _alloc(len);
		if (len > 0) {
			_String_copyUtf32ToUtf16(mem, utf32, len);
		}
		return mem;
	}
	return (sl_char16*)(_String_Null.data);
}

void String8::_initStaticMemory(void* mem)
{
	sl_char8* buf = (sl_char8*)(mem);
	if (*(SLIB_STR_PREF(buf)) == 0) {
		*(SLIB_STR_PLEN(buf)) = _StringBase::getSz8Length(buf);
		*(SLIB_STR_PHASH(buf)) = 0;
		*(SLIB_STR_PREF(buf)) = SLIB_SIZE_MAX >> 1;
	}
}

void String16::_initStaticMemory(void* mem)
{
	sl_char16* buf = (sl_char16*)(mem);
	if (*(SLIB_STR_PREF(buf)) == 0) {
		*(SLIB_STR_PLEN(buf)) = _StringBase::getSz16Length(buf);
		*(SLIB_STR_PHASH(buf)) = 0;
		*(SLIB_STR_PREF(buf)) = SLIB_SIZE_MAX >> 1;
	}
}

String8::String8(const SafeString8& src)
{
	sl_char8* data = src._retainContainer();
	m_data = data;
}

String16::String16(const SafeString16& src)
{
	sl_char16* data = src._retainContainer();
	m_data = data;
}

String8::String8(const String16& src)
{
	if (src.isNull()) {
		m_data = (sl_char8*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char8*)(_String_Empty.data);
	} else {
		m_data = _create(src.data(), src.length());
	}
}

String16::String16(const String8& src)
{
	if (src.isNull()) {
		m_data = (sl_char16*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char16*)(_String_Empty.data);
	} else {
		m_data = _create(src.data(), src.length());
	}
}

String8::String8(const SafeString16& _src)
{
	String16 src(_src);
	if (src.isNull()) {
		m_data = (sl_char8*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char8*)(_String_Empty.data);
	} else {
		m_data = _create(src.data(), src.length());
	}
}

String16::String16(const SafeString8& _src)
{
	String8 src(_src);
	if (src.isNull()) {
		m_data = (sl_char16*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char16*)(_String_Empty.data);
	} else {
		m_data = _create(src.data(), src.length());
	}
}

sl_char8* SafeString8::_retainContainer() const
{
	if ((void*)(this) == (void*)(&_String_Null)) {
		return (sl_char8*)(_String_Null.data);
	} else if ((void*)(this) == (void*)(&_String_Empty)) {
		return (sl_char8*)(_String_Empty.data);
	} else {
		SpinLocker lock(&m_lock);
		sl_char8* data = m_data;
		_increaseReference(data);
		return data;
	}
}

sl_char16* SafeString16::_retainContainer() const
{
	if ((void*)(this) == (void*)(&_String_Null)) {
		return (sl_char16*)(_String_Null.data);
	} else if ((void*)(this) == (void*)(&_String_Empty)) {
		return (sl_char16*)(_String_Empty.data);
	} else {
		SpinLocker lock(&m_lock);
		sl_char16* data = m_data;
		_increaseReference(data);
		return data;
	}
}

void SafeString8::_replaceContainer(sl_char8* data)
{
	sl_char8* before;
	{
		SpinLocker lock(&m_lock);
		before = m_data;
		m_data = data;
	}
	_decreaseReference(before);
}

void SafeString16::_replaceContainer(sl_char16* data)
{
	sl_char16* before;
	{
		SpinLocker lock(&m_lock);
		before = m_data;
		m_data = data;
	}
	_decreaseReference(before);
}

SafeString8::SafeString8(const SafeString16& _src)
{
	String16 src(_src);
	if (src.isNull()) {
		m_data = (sl_char8*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char8*)(_String_Empty.data);
	} else {
		m_data = String8::_create(src.data(), src.length());
	}
}

SafeString16::SafeString16(const SafeString8& _src)
{
	String8 src(_src);
	if (src.isNull()) {
		m_data = (sl_char16*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char16*)(_String_Empty.data);
	} else {
		m_data = String16::_create(src.data(), src.length());
	}
}

SafeString8::SafeString8(const String16& src)
{
	if (src.isNull()) {
		m_data = (sl_char8*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char8*)(_String_Empty.data);
	} else {
		m_data = String8::_create(src.data(), src.length());
	}
}

SafeString16::SafeString16(const String8& src)
{
	if (src.isNull()) {
		m_data = (sl_char16*)(_String_Null.data);
	} else if (src.isEmpty()) {
		m_data = (sl_char16*)(_String_Empty.data);
	} else {
		m_data = String16::_create(src.data(), src.length());
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
			sl_char16* buf = ret.data();
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
			sl_char16* buf = ret.data();
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


String8& String8::operator=(const SafeString8& src)
{
	if (m_data != src.m_data) {
		_replaceContainer(src._retainContainer());
	}
	return *this;
}

String16& String16::operator=(const SafeString16& src)
{
	if (m_data != src.m_data) {
		_replaceContainer(src._retainContainer());
	}
	return *this;
}

String8& String8::operator=(const String16& other)
{
	_replaceContainer(_create(other.data(), other.length()));
	return *this;
}

String16& String16::operator=(const String8& other)
{
	_replaceContainer(_create(other.data(), other.length()));
	return *this;
}

String8& String8::operator=(const SafeString16& other)
{
	return *this = String16(other);
}

String16& String16::operator=(const SafeString8& other)
{
	return *this = String8(other);
}

sl_char8* String8::_merge8(const sl_char8* s1, sl_int32 len1, const sl_char8* s2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz8Length(s1, SLIB_STR_MAX_LEN);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz8Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char8* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		Base::copyMemory(s, s1, len1*sizeof(sl_char8));
		Base::copyMemory(s + len1, s2, (len-len1)*sizeof(sl_char8));
		s[len] = 0;
	}
	return s;
}

sl_char16* String16::_merge16(const sl_char16* s1, sl_int32 len1, const sl_char16* s2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz16Length(s1, SLIB_STR_MAX_LEN);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz16Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char16* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		Base::copyMemory(s, s1, len1*sizeof(sl_char16));
		Base::copyMemory(s + len1, s2, (len-len1)*sizeof(sl_char16));
		s[len] = 0;
	}
	return s;
}

sl_char8* String8::_merge16(const sl_char8* s1, sl_int32 len1, const sl_char16* s2_u16, sl_int32 len2_u16)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz8Length(s1, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len2 = _StringBase::utf16ToUtf8(s2_u16, len2_u16, sl_null, -1);
	sl_uint32 len = len1 + len2;
	sl_char8* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		Base::copyMemory(s, s1, len1*sizeof(sl_char8));
		_StringBase::utf16ToUtf8(s2_u16, len2_u16, s + len1, len-len1);
		s[len] = 0;
	}
	return s;
}

sl_char8* String8::_merge16(const sl_char16* s1_u16, sl_int32 len1_u16, const sl_char8* s2, sl_int32 len2)
{
	sl_uint32 len1 = _StringBase::utf16ToUtf8(s1_u16, len1_u16, sl_null, -1);
	if (len2 < 0) {
		len2 = _StringBase::getSz8Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char8* s = _alloc(len);
	if (len > 0) {
		if (len1 > len) {
			len1 = len;
		}
		_StringBase::utf16ToUtf8(s1_u16, len1_u16, s, len1);
		Base::copyMemory(s + len1, s2, (len-len1)*sizeof(sl_char8));
		s[len] = 0;
	}
	return s;
}

sl_char8* String8::_merge32(const sl_char8* s1, sl_int32 len1, const sl_char32* s2_u32, sl_int32 len2_u32)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz8Length(s1, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len2 = _StringBase::utf32ToUtf8(s2_u32, len2_u32, sl_null, -1);
	sl_uint32 len = len1 + len2;
	sl_char8* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		Base::copyMemory(s, s1, len1*sizeof(sl_char8));
		_StringBase::utf32ToUtf8(s2_u32, len2_u32, s + len1, len-len1);
		s[len] = 0;
	}
	return s;
}

sl_char8* String8::_merge32(const sl_char32* s1_u32, sl_int32 len1_u32, const sl_char8* s2, sl_int32 len2)
{
	sl_uint32 len1 = _StringBase::utf32ToUtf8(s1_u32, len1_u32, sl_null, -1);
	if (len2 < 0) {
		len2 = _StringBase::getSz8Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char8* s = _alloc(len);
	if (len > 0) {
		if (len1 > len) {
			len1 = len;
		}
		_StringBase::utf32ToUtf8(s1_u32, len1_u32, s, len1);
		Base::copyMemory(s + len1, s2, (len-len1)*sizeof(sl_char8));
		s[len] = 0;
	}
	return s;
}

sl_char16* String16::_merge8(const sl_char16* s1, sl_int32 len1, const sl_char8* s2_u8, sl_int32 len2_u8)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz16Length(s1, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len2 = _StringBase::utf8ToUtf16(s2_u8, len2_u8, sl_null, -1);
	sl_uint32 len = len1 + len2;
	sl_char16* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		Base::copyMemory(s, s1, len1*sizeof(sl_char16));
		_StringBase::utf8ToUtf16(s2_u8, len2_u8, s + len1, len-len1);
		s[len] = 0;
	}
	return s;
}

sl_char16* String16::_merge8(const sl_char8* s1_u8, sl_int32 len1_u8, const sl_char16* s2, sl_int32 len2)
{
	sl_uint32 len1 = _StringBase::utf8ToUtf16(s1_u8, len1_u8, sl_null, -1);
	if (len2 < 0) {
		len2 = _StringBase::getSz16Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char16* s = _alloc(len);
	if (len > 0) {
		if (len1 > len) {
			len1 = len;
		}
		_StringBase::utf8ToUtf16(s1_u8, len1_u8, s, len1);
		Base::copyMemory(s + len1, s2, (len-len1)*sizeof(sl_char16));
		s[len] = 0;
	}
	return s;
}

sl_char16* String16::_merge32(const sl_char16* s1, sl_int32 len1, const sl_char32* s2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz16Length(s1, SLIB_STR_MAX_LEN);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz32Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char16* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		Base::copyMemory(s, s1, len1*sizeof(sl_char16));
		_String_copyUtf32ToUtf16(s + len1, s2, len-len1);
		s[len] = 0;
	}
	return s;
}

sl_char16* String16::_merge32(const sl_char32* s1, sl_int32 len1, const sl_char16* s2, sl_int32 len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz32Length(s1, SLIB_STR_MAX_LEN);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz16Length(s2, SLIB_STR_MAX_LEN);
	}
	sl_uint32 len = len1 + len2;
	sl_char16* s = _alloc(len);
	if (len > 0) {
		if ((sl_uint32)len1 > len) {
			len1 = len;
		}
		_String_copyUtf32ToUtf16(s, s1, len1);
		Base::copyMemory(s + len1, s2, (len-len1)*sizeof(sl_char16));
		s[len] = 0;
	}
	return s;
}


String8 String8::operator+(const String8& other) const
{
	sl_uint32 n = length();
	if (n == 0) {
		return other.getNotNull();
	}
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const String8& other)
{
	sl_uint32 n = length();
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			_replaceContainer(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 String16::operator+(const String16& other) const
{
	sl_uint32 n = length();
	if (n == 0) {
		return other.getNotNull();
	}
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const String16& other)
{
	sl_uint32 n = length();
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			_replaceContainer(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const SafeString8& _other) const
{
	String8 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return (StringContainer*)(String8::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const SafeString8& _other)
{
	String8 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(String8::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const SafeString16& _other) const
{
	String16 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return (StringContainer*)(String16::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const SafeString16& _other)
{
	String16 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(String16::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}


String8 String8::operator+(const String16& other) const
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const String16& other)
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			_replaceContainer(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 String16::operator+(const String8& other) const
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const String8& other)
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		if (other.isNotEmpty()) {
			_replaceContainer(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const SafeString16& _other) const
{
	String8 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (other.isNotEmpty()) {
			return (StringContainer*)(String8::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const SafeString16& _other)
{
	String8 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (other.isNotEmpty()) {
			_replaceContainer(String8::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const SafeString8& _other) const
{
	String16 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (other.isNotEmpty()) {
			return (StringContainer*)(String16::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const SafeString8& _other)
{
	String16 s(*this);
	sl_uint32 n = s.length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (other.isNotEmpty()) {
			_replaceContainer(String16::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}


String8 String8::operator+(const SafeString8& _other) const
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const SafeString8& _other)
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 String16::operator+(const SafeString16& _other) const
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const SafeString16& _other)
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const String8& other) const
{
	String8 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n == 0) {
			return other.getNotNull();
		}
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String8::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const String8& other)
{
	String8 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String8::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const String16& other) const
{
	String16 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n == 0) {
			return other.getNotNull();
		}
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String16::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const String16& other)
{
	String16 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String16::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}


String8 String8::operator+(const SafeString16& _other) const
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const SafeString16& _other)
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String16 other(_other);
		if (other.isNotEmpty()) {
			_replaceContainer(_merge16(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 String16::operator+(const SafeString8& _other) const
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (other.isNotEmpty()) {
			return (StringContainer*)(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const SafeString8& _other)
{
	sl_uint32 n = length();
	if (n < SLIB_STR_MAX_LEN) {
		String8 other(_other);
		if (other.isNotEmpty()) {
			_replaceContainer(_merge8(data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const String16& other) const
{
	String8 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String8::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const String16& other)
{
	String8 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String8::_merge16(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const String8& other) const
{
	String16 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String16::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const String8& other)
{
	String16 s(*this);
	if (other.isNotEmpty()) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String16::_merge8(s.data(), n, other.data(), other.length()));
		}
	}
	return *this;
}


String8 String8::operator+(const sl_char8* utf8) const
{
	if (utf8) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(_merge8(data(), n, utf8, -1));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const sl_char8* utf8)
{
	if (utf8) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(_merge8(data(), n, utf8, -1));
		}
	}
	return *this;
}

String16 String16::operator+(const sl_char8* utf8) const
{
	if (utf8) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(_merge8(data(), n, utf8, -1));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const sl_char8* utf8)
{
	if (utf8) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(_merge8(data(), n, utf8, -1));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const sl_char8* utf8) const
{
	String8 s(*this);
	if (utf8) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String8::_merge8(s.data(), n, utf8, -1));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const sl_char8* utf8)
{
	String8 s(*this);
	if (utf8) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String8::_merge8(s.data(), n, utf8, -1));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const sl_char8* utf8) const
{
	String16 s(*this);
	if (utf8) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String16::_merge8(s.data(), n, utf8, -1));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const sl_char8* utf8)
{
	String16 s(*this);
	if (utf8) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String16::_merge8(s.data(), n, utf8, -1));
		}
	}
	return *this;
}


String8 String8::operator+(const sl_char16* utf16) const
{
	if (utf16) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(_merge16(data(), n, utf16, -1));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const sl_char16* utf16)
{
	if (utf16) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(_merge16(data(), n, utf16, -1));
		}
	}
	return *this;
}

String16 String16::operator+(const sl_char16* utf16) const
{
	if (utf16) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(_merge16(data(), n, utf16, -1));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const sl_char16* utf16)
{
	if (utf16) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(_merge16(data(), n, utf16, -1));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const sl_char16* utf16) const
{
	String8 s(*this);
	if (utf16) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String8::_merge16(s.data(), n, utf16, -1));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const sl_char16* utf16)
{
	String8 s(*this);
	if (utf16) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String8::_merge16(s.data(), n, utf16, -1));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const sl_char16* utf16) const
{
	String16 s(*this);
	if (utf16) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String16::_merge16(s.data(), n, utf16, -1));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const sl_char16* utf16)
{
	String16 s(*this);
	if (utf16) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String16::_merge16(s.data(), n, utf16, -1));
		}
	}
	return *this;
}


String8 String8::operator+(const sl_char32* utf32) const
{
	if (utf32) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(_merge32(data(), n, utf32, -1));
		}
	}
	return getNotNull();
}

String8& String8::operator+=(const sl_char32* utf32)
{
	if (utf32) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(_merge32(data(), n, utf32, -1));
		}
	}
	return *this;
}

String16 String16::operator+(const sl_char32* utf32) const
{
	if (utf32) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(_merge32(data(), n, utf32, -1));
		}
	}
	return getNotNull();
}

String16& String16::operator+=(const sl_char32* utf32)
{
	if (utf32) {
		sl_uint32 n = length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(_merge32(data(), n, utf32, -1));
		}
	}
	return *this;
}

String8 SafeString8::operator+(const sl_char32* utf32) const
{
	String8 s(*this);
	if (utf32) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String8::_merge32(s.data(), n, utf32, -1));
		}
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const sl_char32* utf32)
{
	String8 s(*this);
	if (utf32) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String8::_merge32(s.data(), n, utf32, -1));
		}
	}
	return *this;
}

String16 SafeString16::operator+(const sl_char32* utf32) const
{
	String16 s(*this);
	if (utf32) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			return (StringContainer*)(String16::_merge32(s.data(), n, utf32, -1));
		}
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const sl_char32* utf32)
{
	String16 s(*this);
	if (utf32) {
		sl_uint32 n = s.length();
		if (n < SLIB_STR_MAX_LEN) {
			_replaceContainer(String16::_merge32(s.data(), n, utf32, -1));
		}
	}
	return *this;
}


String8 operator+(const sl_char8* utf8, const String8& s)
{
	if (utf8) {
		return (StringContainer*)(String8::_merge8(utf8, -1, s.data(), s.length()));
	}
	return s.getNotNull();
}

String16 operator+(const sl_char8* utf8, const String16& s)
{
	if (utf8) {
		return (StringContainer*)(String16::_merge8(utf8, -1, s.data(), s.length()));
	}
	return s.getNotNull();
}

String8 operator+(const sl_char8* utf8, const SafeString8& s)
{
	return utf8 + String8(s);
}

String16 operator+(const sl_char8* utf8, const SafeString16& s)
{
	return utf8 + String16(s);
}


String8 operator+(const sl_char16* utf16, const String8& s)
{
	if (utf16) {
		return (StringContainer*)(String8::_merge16(utf16, -1, s.data(), s.length()));
	}
	return s.getNotNull();
}

String16 operator+(const sl_char16* utf16, const String16& s)
{
	if (utf16) {
		return (StringContainer*)(String16::_merge16(utf16, -1, s.data(), s.length()));
	}
	return s.getNotNull();
}

String8 operator+(const sl_char16* utf16, const SafeString8& s)
{
	return utf16 + String8(s);
}

String16 operator+(const sl_char16* utf16, const SafeString16& s)
{
	return utf16 + String16(s);
}


String8 operator+(const sl_char32* utf32, const String8& s)
{
	if (utf32) {
		return (StringContainer*)(String8::_merge32(utf32, -1, s.data(), s.length()));
	}
	return s.getNotNull();
}

String16 operator+(const sl_char32* utf32, const String16& s)
{
	if (utf32) {
		return (StringContainer*)(String16::_merge32(utf32, -1, s.data(), s.length()));
	}
	return s.getNotNull();
}

String8 operator+(const sl_char32* utf32, const SafeString8& s)
{
	return utf32 + String8(s);
}

String16 operator+(const sl_char32* utf32, const SafeString16& s)
{
	return utf32 + String16(s);
}



sl_bool String8::_equals8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
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

sl_bool String16::_equals16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
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

sl_bool String8::_equals16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return _equals8(str1, len1, other.m_data, other.length());
}

sl_bool String16::_equals8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
{
	String16 other(str2, len2);
	return _equals16(str1, len1, other.m_data, other.length());
}

sl_bool String8::_equals32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return _equals8(str1, len1, other.m_data, other.length());
}

sl_bool String16::_equals32(const sl_char16* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	return _compare32(str1, len1, str2, len2) == 0;
}


sl_int32 String8::_compare8(const sl_char8* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
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

sl_int32 String16::_compare16(const sl_char16* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
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

sl_int32 String8::_compare16(const sl_char8* str1, sl_int32 len1, const sl_char16* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return _compare8(str1, len1, other.m_data, other.length());
}

sl_int32 String16::_compare8(const sl_char16* str1, sl_int32 len1, const sl_char8* str2, sl_int32 len2)
{
	String16 other(str2, len2);
	return _compare16(str1, len1, other.m_data, other.length());
}

sl_int32 String8::_compare32(const sl_char8* str1, sl_int32 len1, const sl_char32* str2, sl_int32 len2)
{
	String8 other(str2, len2);
	return _compare8(str1, len1, other.m_data, other.length());
}

sl_int32 String16::_compare32(const sl_char16* m1, sl_int32 len1, const sl_char32* m2, sl_int32 len2)
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

sl_int32 String8::compare(const String8& other, sl_uint32 len) const
{
	if (m_data == other.m_data) {
		return 0;
	}
	sl_uint32 l1 = length();
	if (l1 > len) {
		l1 = len;
	}
	sl_uint32 l2 = other.length();
	if (l2 > len) {
		l2 = len;
	}
	return _compare8(m_data, l1, other.m_data, l2);
}

sl_int32 String16::compare(const String16& other, sl_uint32 len) const
{
	if (m_data == other.m_data) {
		return 0;
	}
	sl_uint32 l1 = length();
	if (l1 > len) {
		l1 = len;
	}
	sl_uint32 l2 = other.length();
	if (l2 > len) {
		l2 = len;
	}
	return _compare16(m_data, l1, other.m_data, l2);
}


Memory String8::toMemory() const
{
	return Memory::create(m_data, length()*sizeof(sl_char8));
}

Memory String16::toMemory() const
{
	return Memory::create(m_data, length()*sizeof(sl_char16));
}

sl_uint32 String8::getUtf16(sl_char16* utf16, sl_int32 len) const
{
	return _StringBase::utf8ToUtf16(m_data, length(), utf16, len);
}

sl_uint32 String16::getUtf8(sl_char8* utf8, sl_int32 len) const
{
	return _StringBase::utf16ToUtf8(m_data, length(), utf8, len);
}

StringData String8::getUtf16() const
{
	StringData ret;
	ret.mem = toUtf16();
	if (ret.mem.isNotEmpty()) {
		ret.sz16 = (const sl_char16*)(ret.mem.getBuf());
		ret.len = (sl_uint32)(ret.mem.getSize() / 2 - 1);
		if (!ret.sz16) {
			ret.sz16 = (sl_char16*)("\0\0");
			ret.len = 0;
		}
	} else {
		ret.sz16 = (sl_char16*)("\0\0");
		ret.len = 0;
	}
	return ret;
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

Memory String8::toUtf16() const
{
	sl_int32 len = _StringBase::utf8ToUtf16(m_data, length(), sl_null, -1);
	Memory memory = Memory::create(2 * (len + 1));
	if (memory.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(memory.getBuf());
		_StringBase::utf8ToUtf16(m_data, length(), buf, len);
		buf[len] = 0;
	}
	return memory;
}

Memory String16::toUtf8() const
{
	sl_int32 len = _StringBase::utf16ToUtf8(m_data, length(), sl_null, -1);
	Memory memory = Memory::create(len + 1);
	if (memory.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(memory.getBuf());
		_StringBase::utf16ToUtf8(m_data, length(), buf, len);
		buf[len] = 0;
	}
	return memory;
}

sl_uint32 String8::getUtf32(sl_char32* utf32, sl_int32 len) const
{
	return _StringBase::utf8ToUtf32(m_data, length(), utf32, len);
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
	sl_int32 len = _StringBase::utf8ToUtf32(m_data, length(), sl_null, -1);
	Memory memory = Memory::create(4 * (len + 1));
	if (memory.isNotEmpty()) {
		sl_char32* buf = (sl_char32*)(memory.getBuf());
		_StringBase::utf8ToUtf32(m_data, length(), buf, len);
		buf[len] = 0;
	}
	return memory;
}


String8 String8::substring(sl_int32 start, sl_int32 end) const
{
	sl_int32 count = length();
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
		return *this;
	}
	return String8(m_data + start, end - start);
}

String16 String16::substring(sl_int32 start, sl_int32 end) const
{
	sl_int32 count = length();
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
		return *this;
	}
	return String16(m_data + start, end - start);
}

sl_int32 String8::indexOf(sl_char8 ch, sl_int32 start) const
{
	sl_int32 count = length();
	if (count == 0) {
		return -1;
	}
	if (start < 0) {
		start = 0;
	}
	if (start >= count) {
		return -1;
	}
	sl_char8* pt = (sl_char8*)(Base::findMemory(m_data + start, ch, count - start));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - m_data);
	}
}

sl_int32 String16::indexOf(sl_char16 ch, sl_int32 start) const
{
	sl_int32 count = length();
	if (count == 0) {
		return -1;
	}
	if (start < 0) {
		start = 0;
	}
	if (start >= count) {
		return -1;
	}
	sl_char16* pt = (sl_char16*)(Base::findMemory2(m_data + start, ch, count - start));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - m_data);
	}
}

sl_int32 String8::lastIndexOf(sl_char8 ch, sl_int32 start) const
{
	sl_int32 count = length();
	if (count == 0) {
		return -1;
	}
	if (start < 0 || start >= count) {
		start = count - 1;
	}
	sl_char8* pt = (sl_char8*)(Base::findMemoryReverse(m_data, ch, start + 1));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - m_data);
	}
}

sl_int32 String16::lastIndexOf(sl_char16 ch, sl_int32 start) const
{
	sl_int32 count = length();
	if (count == 0) {
		return -1;
	}
	if (start < 0 || start >= count) {
		start = count - 1;
	}
	sl_char16* pt = (sl_char16*)(Base::findMemoryReverse2(m_data, ch, start + 1));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_int32)(pt - m_data);
	}
}

template <class ST, class CT, class TT>
SLIB_INLINE sl_int32 _String_indexOf(const ST& str, const ST& pattern, sl_int32 start)
{
	sl_int32 countPat = pattern.length();
	if (countPat == 0) {
		return 0;
	}
	sl_int32 count = str.length();
	if (count == 0) {
		return -1;
	}
	const CT* buf = str.data();
	const CT* bufPat = pattern.data();
	if (start < 0) {
		start = 0;
	}
	if (start > count - countPat) {
		return -1;
	}
	while (start <= count - countPat) {
		const CT* pt = (const CT*)(TT::findMemory(buf + start, bufPat[0], count - start - countPat + 1));
		if (pt == sl_null)
			return -1;
		if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
			return (sl_int32)(pt - buf);
		} else {
			start = (sl_int32)(pt - buf + 1);
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
SLIB_INLINE sl_int32 _String_lastIndexOf(const ST& str, const ST& pattern, sl_int32 start)
{
	sl_int32 countPat = pattern.length();
	if (countPat == 0) {
		return -1;
	}
	sl_int32 count = str.length();
	if (count == 0) {
		return -1;
	}
	const CT* buf = str.data();
	const CT* bufPat = pattern.data();
	if (start < 0 || start > count - countPat) {
		start = count - countPat;
	}
	while (start >= 0) {
		const CT* pt = (const CT*)(TT::findMemoryReverse(buf, bufPat[0], start + 1));
		if (pt == sl_null)
			return -1;
		if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
			return (sl_int32)(pt - buf);
		} else {
			start = (sl_int32)(pt - buf - 1);
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
	if (isEmpty()) {
		return sl_false;
	} else {
		return m_data[0] == ch;
	}
}

sl_bool String16::startsWith(sl_char16 ch) const
{
	if (isEmpty()) {
		return sl_false;
	} else {
		return m_data[0] == ch;
	}
}

sl_bool String8::startsWith(const String8& str) const
{
	sl_int32 count2 = str.length();
	if (count2 == 0) {
		return sl_true;
	}
	sl_int32 count1 = length();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory(m_data, str.m_data, count2) == 0;
	}
}

sl_bool String16::startsWith(const String16& str) const
{
	sl_int32 count2 = str.length();
	if (count2 == 0) {
		return sl_true;
	}
	sl_int32 count1 = length();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory2(m_data, str.m_data, count2) == 0;
	}
}

sl_bool String8::endsWith(sl_char8 ch) const
{
	sl_int32 count = length();
	if (count < 1) {
		return sl_false;
	} else {
		return m_data[count - 1] == ch;
	}
}

sl_bool String16::endsWith(sl_char16 ch) const
{
	sl_int32 count = length();
	if (count < 1) {
		return sl_false;
	} else {
		return m_data[count - 1] == ch;
	}
}

sl_bool String8::endsWith(const String8& str) const
{
	sl_int32 count2 = str.length();
	if (count2 == 0) {
		return sl_true;
	}
	sl_int32 count1 = length();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory(m_data + count1 - count2, str.m_data, count2) == 0;
	}
}

sl_bool String16::endsWith(const String16& str) const
{
	sl_int32 count2 = str.length();
	if (count2 == 0) {
		return sl_true;
	}
	sl_int32 count1 = length();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory2(m_data + count1 - count2, str.m_data, count2) == 0;
	}
}

void String8::makeUpper()
{
	sl_char8* buf = m_data;
	sl_int32 n = length();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] = buf[i] - ('a' - 'A');
		}
	}
}

void String16::makeUpper()
{
	sl_char16* buf = m_data;
	sl_int32 n = length();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] = buf[i] - ('a' - 'A');
		}
	}
}

void String8::makeLower()
{
	sl_char8* buf = m_data;
	sl_int32 n = length();
	for (sl_int32 i = 0; i < n; i++) {
		if (buf[i] >= 'A' && buf[i] <= 'Z') {
			buf[i] = buf[i] + ('a' - 'A');
		}
	}
}

void String16::makeLower()
{
	sl_char16* buf = m_data;
	sl_int32 n = length();
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

	SLIB_INLINE STRING_REPLACE_SUBSET() {}
	
	SLIB_INLINE STRING_REPLACE_SUBSET(const STRING_REPLACE_SUBSET& o)
	{
		start = o.start;
		len = o.len;
	}
};
template <class ST, class CT, class TT>
SLIB_INLINE ST _String_replaceAll(const ST& str, const ST& pattern, const ST& replacement)
{
	sl_int32 countPat = pattern.length();
	if (countPat == 0) {
		return ST::null();
	}
	sl_int32 count = str.length();
	if (count == 0) {
		return ST::null();
	}
	Queue<STRING_REPLACE_SUBSET> queue;
	STRING_REPLACE_SUBSET subset;
	sl_int32 countReplace = replacement.length();
	const CT* buf = str.data();
	const CT* bufReplace = replacement.data();
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
		queue.push_NoLock(subset);
		size += subset.len;
		start = index + countPat;
	}

	ST ret = ST::allocate(size);
	if (ret.isNotNull()) {
		CT* out = ret.getBuf();
		while (queue.pop_NoLock(&subset)) {
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
SLIB_INLINE ST _String_trim(const ST& str)
{
	const CT* sz = str.data();
	sl_uint32 n = str.length();
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
SLIB_INLINE ST _String_trimLeft(const ST& str)
{
	const CT* sz = str.data();
	sl_uint32 n = str.length();
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
SLIB_INLINE ST _String_trimRight(const ST& str)
{
	const CT* sz = str.data();
	sl_uint32 n = str.length();
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
List<ST> _String_split(const ST& str, const ST& pattern)
{
	CList<ST>* ret = CList<ST>::create();
	if (ret) {
		if (pattern.isEmpty()) {
			return ret;
		}
		sl_int32 start = 0;
		while (1) {
			sl_int32 index = str.indexOf(pattern, start);
			if (index < 0) {
				ret->add_NoLock(str.substring(start));
				break;
			}
			ret->add_NoLock(str.substring(start, index));
			start = index + pattern.getLength();
		}
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


sl_uint32 String8::hashCode() const
{
	sl_char8* start = m_data;
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
	sl_char16* start = m_data;
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
	const CT* ch = s.data();
	sl_uint32 len = s.length();
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
	sl_uint32 n = _String_applyBackslashEscapes<String8, sl_char8>(*this, flagDoubleQuote, sl_null);
	String8 ret = allocate(n);
	if (ret.isEmpty()) {
		return String8::null();
	}
	_String_applyBackslashEscapes<String8, sl_char8>(*this, flagDoubleQuote, ret.data());
	return ret;
}

String16 String16::applyBackslashEscapes(sl_bool flagDoubleQuote)
{
	sl_uint32 n = _String_applyBackslashEscapes<String16, sl_char16>(*this, flagDoubleQuote, sl_null);
	String16 ret = allocate(n);
	if (ret.isEmpty()) {
		return String16::null();
	}
	_String_applyBackslashEscapes<String16, sl_char16>(*this, flagDoubleQuote, ret.data());
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
	ObjectLocker lock(this);
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<String8>* begin = m_queue.getBegin();
	if (m_queue.count() == 1) {
		return begin->value;
	}
	sl_uint32 total = m_len;
	String8 ret = String8::allocate(total);
	if (ret.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(ret.data());
		sl_uint32 offset = 0;
		Link<String8>* item = begin;
		while (item) {
			String8& s = item->value;
			sl_uint32 t = s.getLength();
			Base::copyMemory(buf + offset, s.data(), t * sizeof(sl_char8));
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

String16 StringBuffer16::merge() const
{
	ObjectLocker lock(this);
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<String16>* begin = m_queue.getBegin();
	if (m_queue.count() ==1) {
		return begin->value;
	}
	sl_uint32 total = m_len;
	String16 ret = String16::allocate(total);
	if (ret.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(ret.data());
		sl_uint32 offset = 0;
		Link<String16>* item = begin;
		while (item) {
			String16& s = item->value;
			sl_uint32 t = s.getLength();
			Base::copyMemory(buf + offset, s.data(), t * 2);
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
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_uint32 total = m_len;
	String8 ret = String8::allocate(total);
	if (ret.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(ret.data());
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
	ObjectLocker lock(this);
	if (m_queue.count() == 0) {
		return String16::null();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_uint32 total = m_len;
	String16 ret = String16::allocate(total);
	if (ret.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(ret.data());
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
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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
