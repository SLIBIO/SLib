#include "../../../inc/slib/core/string.h"
#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/endian.h"
#include "../../../inc/slib/core/scoped_pointer.h"
#include "../../../inc/slib/core/math.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(String)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, Ref<Referable>)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, String)

class _StringBase
{
public:
	static sl_size getSz8Length(const sl_char8* utf8, sl_reg count = -1);
	static sl_size getSz16Length(const sl_char16* utf16, sl_reg count = -1);
	static sl_size getSz32Length(const sl_char32* utf32, sl_reg count = -1);
	
	// utf8 conversion
	static sl_size utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer);
	static sl_size utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer);
	static sl_size utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer);
	static sl_size utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer);
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


const sl_char8 _g_string8_null_buf[] = {0, 0};

const StringContainer8 _g_string8_null_container = {(sl_char8*)(_g_string8_null_buf), 0, 0, -1};
const _String8_Const _String8_Null = {(StringContainer8*)(&_g_string8_null_container), 0};

const StringContainer8 _g_string8_empty_container = {(sl_char8*)(_g_string8_null_buf), 0, 0, -1};
const _String8_Const _String8_Empty = {(StringContainer8*)(&_g_string8_empty_container), 0};

const sl_char16 _g_string16_null_buf[] = {0, 0};

const StringContainer16 _g_string16_null_container = {(sl_char16*)(_g_string16_null_buf), 0, 0, -1};
const _String16_Const _String16_Null = {(StringContainer16*)(&_g_string16_null_container), 0};

const StringContainer16 _g_string16_empty_container = {(sl_char16*)(_g_string16_null_buf), 0, 0, -1};
const _String16_Const _String16_Empty = {(StringContainer16*)(&_g_string16_empty_container), 0};


SLIB_INLINE sl_reg StringContainer8::increaseReference()
{
	if (ref >= 0) {
		return Base::interlockedIncrement(&ref);
	}
	return 1;
}

SLIB_INLINE sl_reg StringContainer16::increaseReference()
{
	if (ref >= 0) {
		return Base::interlockedIncrement(&ref);
	}
	return 1;
}


SLIB_INLINE sl_reg StringContainer8::decreaseReference()
{
	if (ref > 0) {
		sl_reg nRef = Base::interlockedDecrement(&ref);
		if (nRef == 0) {
			Base::freeMemory(this);
		}
		return nRef;
	}
	return 1;
}

SLIB_INLINE sl_reg StringContainer16::decreaseReference()
{
	if (ref > 0) {
		sl_reg nRef = Base::interlockedDecrement(&ref);
		if (nRef == 0) {
			Base::freeMemory(this);
		}
		return nRef;
	}
	return 1;
}


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


SLIB_INLINE StringContainer8* String8::_alloc(sl_size len)
{
	if (len == 0) {
		return _String8_Empty.container;
	}
	sl_char8* buf = (sl_char8*)(Base::createMemory(sizeof(StringContainer8) + len + 1));
	if (buf) {
		StringContainer8* container = (StringContainer8*)((void*)buf);
		container->sz = buf + sizeof(StringContainer8);
		container->len = len;
		container->hash = 0;
		container->ref = 1;
		container->sz[len] = 0;
		return container;
	}
	return _String8_Null.container;
}

SLIB_INLINE StringContainer16* String16::_alloc(sl_size len)
{
	if (len == 0) {
		return _String16_Empty.container;
	}
	sl_char8* buf = (sl_char8*)(Base::createMemory(sizeof(StringContainer8) + ((len + 1) << 1)));
	if (buf) {
		StringContainer16* container = (StringContainer16*)((void*)buf);
		container->sz = (sl_char16*)((void*)(buf + sizeof(StringContainer16)));
		container->len = len;
		container->hash = 0;
		container->ref = 1;
		container->sz[len] = 0;
		return container;
	}
	return _String16_Null.container;
}


SLIB_INLINE StringContainer8* SafeString8::_retainContainer() const
{
	if ((void*)(this) == (void*)(&_String8_Null)) {
		return _String8_Null.container;
	} else if ((void*)(this) == (void*)(&_String8_Empty)) {
		return _String8_Empty.container;
	} else {
		SpinLocker lock(&m_lock);
		StringContainer8* container = m_container;
		container->increaseReference();
		return container;
	}
}

SLIB_INLINE StringContainer16* SafeString16::_retainContainer() const
{
	if ((void*)(this) == (void*)(&_String16_Null)) {
		return _String16_Null.container;
	} else if ((void*)(this) == (void*)(&_String16_Empty)) {
		return _String16_Empty.container;
	} else {
		SpinLocker lock(&m_lock);
		StringContainer16* container = m_container;
		container->increaseReference();
		return container;
	}
}


SLIB_INLINE void String8::_replaceContainer(StringContainer8* container)
{
	m_container->decreaseReference();
	m_container = container;
}

SLIB_INLINE void String16::_replaceContainer(StringContainer16* container)
{
	m_container->decreaseReference();
	m_container = container;
}

SLIB_INLINE void SafeString8::_replaceContainer(StringContainer8* container)
{
	StringContainer8* before;
	{
		SpinLocker lock(&m_lock);
		before = m_container;
		m_container = container;
	}
	before->decreaseReference();
}

SLIB_INLINE void SafeString16::_replaceContainer(StringContainer16* container)
{
	StringContainer16* before;
	{
		SpinLocker lock(&m_lock);
		before = m_container;
		m_container = container;
	}
	before->decreaseReference();
}


SLIB_INLINE StringContainer8* String8::_create(sl_char8 ch, sl_size nRepeatCount)
{
	StringContainer8* container = _alloc(nRepeatCount);
	if (container != _String8_Null.container) {
		Base::resetMemory(container->sz, ch, nRepeatCount);
	}
	return container;
}

SLIB_INLINE StringContainer16* String16::_create(sl_char16 ch, sl_size nRepeatCount)
{
	StringContainer16* container = _alloc(nRepeatCount);
	if (container != _String16_Null.container) {
		Base::resetMemory2(container->sz, ch, nRepeatCount);
	}
	return container;
}

SLIB_INLINE StringContainer8* String8::_create(const sl_char8* utf8, sl_reg lenUtf8)
{
	if (utf8) {
		if (lenUtf8 < 0) {
			lenUtf8 = _StringBase::getSz8Length(utf8);
		}
		sl_size len = lenUtf8;
		StringContainer8* container = _alloc(len);
		if (container != _String8_Null.container) {
			Base::copyMemory(container->sz, utf8, len);
		}
		return container;
	}
	return _String8_Null.container;
}

SLIB_INLINE StringContainer16* String16::_create(const sl_char8* utf8, sl_reg lenUtf8)
{
	if (utf8) {
		if (lenUtf8 < 0) {
			lenUtf8 = _StringBase::getSz8Length(utf8);
		}
		sl_size len = _StringBase::utf8ToUtf16(utf8, lenUtf8, sl_null, -1);
		StringContainer16* container = _alloc(len);
		if (container != _String16_Null.container) {
			_StringBase::utf8ToUtf16(utf8, lenUtf8, container->sz, len);
			container->sz[len] = 0;
		}
		return container;
	}
	return _String16_Null.container;
}

SLIB_INLINE StringContainer8* String8::_create(const sl_char16* utf16, sl_reg lenUtf16)
{
	if (utf16) {
		if (lenUtf16 < 0) {
			lenUtf16 = _StringBase::getSz16Length(utf16);
		}
		sl_size len = _StringBase::utf16ToUtf8(utf16, lenUtf16, sl_null, -1);
		StringContainer8* container = _alloc(len);
		if (container != _String8_Null.container) {
			_StringBase::utf16ToUtf8(utf16, lenUtf16, container->sz, len);
			container->sz[len] = 0;
		}
		return container;
	}
	return _String8_Null.container;
}

SLIB_INLINE StringContainer16* String16::_create(const sl_char16* utf16, sl_reg lenUtf16)
{
	if (utf16) {
		if (lenUtf16 < 0) {
			lenUtf16 = _StringBase::getSz16Length(utf16);
		}
		sl_size len = lenUtf16;
		StringContainer16* container = _alloc(len);
		if (container != _String16_Null.container) {
			Base::copyMemory(container->sz, utf16, len*sizeof(sl_char16));
		}
		return container;
	}
	return _String16_Null.container;
}

SLIB_INLINE StringContainer8* String8::_create(const sl_char32* utf32, sl_reg lenUtf32)
{
	if (utf32) {
		if (lenUtf32 < 0) {
			lenUtf32 = _StringBase::getSz32Length(utf32);
		}
		sl_size len = _StringBase::utf32ToUtf8(utf32, lenUtf32, sl_null, -1);
		StringContainer8* container = _alloc(len);
		if (container != _String8_Null.container) {
			_StringBase::utf32ToUtf8(utf32, lenUtf32, container->sz, len);
			container->sz[len] = 0;
		}
		return container;
	}
	return _String8_Null.container;
}

SLIB_INLINE void _String_copyUtf32ToUtf16(sl_char16* utf16, const sl_char32* utf32, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		sl_char16 ch = (sl_char16)(utf32[i]);
		utf16[i] = ch;
	}
}

SLIB_INLINE StringContainer16* String16::_create(const sl_char32* utf32, sl_reg lenUtf32)
{
	if (utf32) {
		if (lenUtf32 < 0) {
			lenUtf32 = _StringBase::getSz32Length(utf32);
		}
		sl_size len = lenUtf32;
		StringContainer16* container = _alloc(len);
		if (container != _String16_Null.container) {
			_String_copyUtf32ToUtf16(container->sz, utf32, len);
		}
		return container;
	}
	return _String16_Null.container;
}


SLIB_INLINE StringContainer8* String8::_merge8(const sl_char8* s1, sl_reg len1, const sl_char8* s2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz8Length(s1);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz8Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer8* s = _alloc(len);
	if (s != _String8_Null.container) {
		Base::copyMemory(s->sz, s1, len1);
		Base::copyMemory(s->sz + len1, s2, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer16* String16::_merge16(const sl_char16* s1, sl_reg len1, const sl_char16* s2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz16Length(s1);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz16Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer16* s = _alloc(len);
	if (s != _String16_Null.container) {
		Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
		Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer8* String8::_merge16(const sl_char8* s1, sl_reg len1, const sl_char16* s2_u16, sl_reg len2_u16)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz8Length(s1);
	}
	sl_size len2 = _StringBase::utf16ToUtf8(s2_u16, len2_u16, sl_null, -1);
	sl_size len = len1 + len2;
	StringContainer8* s = _alloc(len);
	if (s != _String8_Null.container) {
		Base::copyMemory(s->sz, s1, len1);
		_StringBase::utf16ToUtf8(s2_u16, len2_u16, s->sz + len1, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer8* String8::_merge16(const sl_char16* s1_u16, sl_reg len1_u16, const sl_char8* s2, sl_reg len2)
{
	sl_size len1 = _StringBase::utf16ToUtf8(s1_u16, len1_u16, sl_null, -1);
	if (len2 < 0) {
		len2 = _StringBase::getSz8Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer8* s = _alloc(len);
	if (s != _String8_Null.container) {
		_StringBase::utf16ToUtf8(s1_u16, len1_u16, s->sz, len1);
		Base::copyMemory(s->sz + len1, s2, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer8* String8::_merge32(const sl_char8* s1, sl_reg len1, const sl_char32* s2_u32, sl_reg len2_u32)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz8Length(s1);
	}
	sl_size len2 = _StringBase::utf32ToUtf8(s2_u32, len2_u32, sl_null, -1);
	sl_size len = len1 + len2;
	StringContainer8* s = _alloc(len);
	if (s != _String8_Null.container) {
		Base::copyMemory(s->sz, s1, len1*sizeof(sl_char8));
		_StringBase::utf32ToUtf8(s2_u32, len2_u32, s->sz + len1, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer8* String8::_merge32(const sl_char32* s1_u32, sl_reg len1_u32, const sl_char8* s2, sl_reg len2)
{
	sl_size len1 = _StringBase::utf32ToUtf8(s1_u32, len1_u32, sl_null, -1);
	if (len2 < 0) {
		len2 = _StringBase::getSz8Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer8* s = _alloc(len);
	if (s != _String8_Null.container) {
		_StringBase::utf32ToUtf8(s1_u32, len1_u32, s->sz, len1);
		Base::copyMemory(s->sz + len1, s2, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer16* String16::_merge8(const sl_char16* s1, sl_reg len1, const sl_char8* s2_u8, sl_reg len2_u8)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz16Length(s1);
	}
	sl_size len2 = _StringBase::utf8ToUtf16(s2_u8, len2_u8, sl_null, -1);
	sl_size len = len1 + len2;
	StringContainer16* s = _alloc(len);
	if (s != _String16_Null.container) {
		Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
		_StringBase::utf8ToUtf16(s2_u8, len2_u8, s->sz + len1, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer16* String16::_merge8(const sl_char8* s1_u8, sl_reg len1_u8, const sl_char16* s2, sl_reg len2)
{
	sl_size len1 = _StringBase::utf8ToUtf16(s1_u8, len1_u8, sl_null, -1);
	if (len2 < 0) {
		len2 = _StringBase::getSz16Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer16* s = _alloc(len);
	if (s != _String16_Null.container) {
		_StringBase::utf8ToUtf16(s1_u8, len1_u8, s->sz, len1);
		Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer16* String16::_merge32(const sl_char16* s1, sl_reg len1, const sl_char32* s2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz16Length(s1);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz32Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer16* s = _alloc(len);
	if (s != _String16_Null.container) {
		Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
		_String_copyUtf32ToUtf16(s->sz + len1, s2, len2);
		s->sz[len] = 0;
	}
	return s;
}

SLIB_INLINE StringContainer16* String16::_merge32(const sl_char32* s1, sl_reg len1, const sl_char16* s2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = _StringBase::getSz32Length(s1);
	}
	if (len2 < 0) {
		len2 = _StringBase::getSz16Length(s2);
	}
	sl_size len = len1 + len2;
	StringContainer16* s = _alloc(len);
	if (s != _String16_Null.container) {
		_String_copyUtf32ToUtf16(s->sz, s1, len1);
		Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
		s->sz[len] = 0;
	}
	return s;
}


SLIB_INLINE sl_bool String8::_equals8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = SLIB_REG_MAX;
	}
	if (len2 < 0) {
		len2 = SLIB_REG_MAX;
	}
	sl_size len = SLIB_MIN(len1, len2);
	sl_uint8* s1 = (sl_uint8*)str1;
	sl_uint8* s2 = (sl_uint8*)str2;
	for (sl_size i = 0; i < len; i++) {
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

SLIB_INLINE sl_bool String16::_equals16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = SLIB_REG_MAX;
	}
	if (len2 < 0) {
		len2 = SLIB_REG_MAX;
	}
	sl_size len = SLIB_MIN(len1, len2);
	sl_uint16* s1 = (sl_uint16*)str1;
	sl_uint16* s2 = (sl_uint16*)str2;
	for (sl_size i = 0; i < len; i++) {
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

SLIB_INLINE sl_bool String8::_equals16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2)
{
	String8 other(str2, len2);
	return _equals8(str1, len1, other.getData(), other.getLength());
}

SLIB_INLINE sl_bool String16::_equals8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2)
{
	String16 other(str2, len2);
	return _equals16(str1, len1, other.getData(), other.getLength());
}

SLIB_INLINE sl_bool String8::_equals32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2)
{
	String8 other(str2, len2);
	return _equals8(str1, len1, other.getData(), other.getLength());
}

SLIB_INLINE sl_bool String16::_equals32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2)
{
	return _compare32(str1, len1, str2, len2) == 0;
}


SLIB_INLINE sl_int32 String8::_compare8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = SLIB_REG_MAX;
	}
	if (len2 < 0) {
		len2 = SLIB_REG_MAX;
	}
	sl_size len = SLIB_MIN(len1, len2);
	sl_uint8* s1 = (sl_uint8*)str1;
	sl_uint8* s2 = (sl_uint8*)str2;
	for (sl_size i = 0; i < len; i++) {
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

SLIB_INLINE sl_int32 String16::_compare16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = SLIB_REG_MAX;
	}
	if (len2 < 0) {
		len2 = SLIB_REG_MAX;
	}
	sl_size len = SLIB_MIN(len1, len2);
	sl_uint16* s1 = (sl_uint16*)str1;
	sl_uint16* s2 = (sl_uint16*)str2;
	for (sl_size i = 0; i < len; i++) {
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

SLIB_INLINE sl_int32 String8::_compare16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2)
{
	String8 other(str2, len2);
	return _compare8(str1, len1, other.getData(), other.getLength());
}

SLIB_INLINE sl_int32 String16::_compare8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2)
{
	String16 other(str2, len2);
	return _compare16(str1, len1, other.getData(), other.getLength());
}

SLIB_INLINE sl_int32 String8::_compare32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2)
{
	String8 other(str2, len2);
	return _compare8(str1, len1, other.getData(), other.getLength());
}

SLIB_INLINE sl_int32 String16::_compare32(const sl_char16* m1, sl_reg len1, const sl_char32* m2, sl_reg len2)
{
	if (len1 < 0) {
		len1 = SLIB_REG_MAX;
	}
	if (len2 < 0) {
		len2 = SLIB_REG_MAX;
	}
	sl_size len = SLIB_MIN(len1, len2);
	sl_uint16* s1 = (sl_uint16*)m1;
	sl_uint32* s2 = (sl_uint32*)m2;
	for (sl_size i = 0; i < len; i++) {
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


String8::String8(const String8& src)
{
	StringContainer8* container = src.m_container;
	container->increaseReference();
	m_container= container;
}

String16::String16(const String16& src)
{
	StringContainer16* container = src.m_container;
	container->increaseReference();
	m_container = container;
}

SafeString8::SafeString8(const String8& src)
{
	StringContainer8* container = src.m_container;
	container->increaseReference();
	m_container = container;
}

SafeString16::SafeString16(const String16& src)
{
	StringContainer16* container = src.m_container;
	container->increaseReference();
	m_container = container;
}


String8::String8(const SafeString8& src)
{
	m_container = src._retainContainer();
}

String16::String16(const SafeString16& src)
{
	m_container = src._retainContainer();
}

SafeString8::SafeString8(const SafeString8& src)
{
	m_container = src._retainContainer();
}

SafeString16::SafeString16(const SafeString16& src)
{
	m_container = src._retainContainer();
}


String8::String8(const String16& src)
{
	if (src.isNull()) {
		m_container = _String8_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String8_Empty.container;
		} else {
			m_container = _create(src.getData(), len);
		}
	}
}

String16::String16(const String8& src)
{
	if (src.isNull()) {
		m_container = _String16_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String16_Empty.container;
		} else {
			m_container = _create(src.getData(), len);
		}
	}
}

SafeString8::SafeString8(const String16& src)
{
	if (src.isNull()) {
		m_container = _String8_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String8_Empty.container;
		} else {
			m_container = String8::_create(src.getData(), len);
		}
	}
}

SafeString16::SafeString16(const String8& src)
{
	if (src.isNull()) {
		m_container = _String16_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String16_Empty.container;
		} else {
			m_container = String16::_create(src.getData(), len);
		}
	}
}


String8::String8(const SafeString16& _src)
{
	String16 src(_src);
	if (src.isNull()) {
		m_container = _String8_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String8_Empty.container;
		} else {
			m_container = _create(src.getData(), len);
		}
	}
}

String16::String16(const SafeString8& _src)
{
	String8 src(_src);
	if (src.isNull()) {
		m_container = _String16_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String16_Empty.container;
		} else {
			m_container = _create(src.getData(), len);
		}
	}
}

SafeString8::SafeString8(const SafeString16& _src)
{
	String16 src(_src);
	if (src.isNull()) {
		m_container = _String8_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String8_Empty.container;
		} else {
			m_container = String8::_create(src.getData(), len);
		}
	}
}

SafeString16::SafeString16(const SafeString8& _src)
{
	String8 src(_src);
	if (src.isNull()) {
		m_container = _String16_Null.container;
	} else {
		sl_size len = src.getLength();
		if (len == 0) {
			m_container = _String16_Empty.container;
		} else {
			m_container = String16::_create(src.getData(), len);
		}
	}
}


String8::~String8()
{
	m_container->decreaseReference();
}

String16::~String16()
{
	m_container->decreaseReference();
}

SafeString8::~SafeString8()
{
	m_container->decreaseReference();
}

SafeString16::~SafeString16()
{
	m_container->decreaseReference();
}


String8::String8(sl_char8 ch, sl_size nRepeatCount)
{
	m_container = _create(ch, nRepeatCount);
}

SafeString8::SafeString8(sl_char8 ch, sl_size nRepeatCount)
{
	m_container = String8::_create(ch, nRepeatCount);
}

String16::String16(sl_char16 ch, sl_size nRepeatCount)
{
	m_container = _create(ch, nRepeatCount);
}

SafeString16::SafeString16(sl_char16 ch, sl_size nRepeatCount)
{
	m_container = String16::_create(ch, nRepeatCount);
}


String8::String8(const sl_char8* strUtf8)
{
	m_container = _create(strUtf8, -1);
}

String8::String8(const sl_char8* strUtf8, sl_reg length)
{
	m_container = _create(strUtf8, length);
}

SafeString8::SafeString8(const sl_char8* strUtf8)
{
	m_container = String8::_create(strUtf8, -1);
}

SafeString8::SafeString8(const sl_char8* strUtf8, sl_reg length)
{
	m_container = String8::_create(strUtf8, length);
}

String16::String16(const sl_char8* strUtf8)
{
	m_container = _create(strUtf8, -1);
}

String16::String16(const sl_char8* strUtf8, sl_reg length)
{
	m_container = _create(strUtf8, length);
}

SafeString16::SafeString16(const sl_char8* strUtf8)
{
	m_container = String16::_create(strUtf8, -1);
}

SafeString16::SafeString16(const sl_char8* strUtf8, sl_reg length)
{
	m_container = String16::_create(strUtf8, length);
}


String8::String8(const sl_char16* strUtf16)
{
	m_container = _create(strUtf16, -1);
}

String8::String8(const sl_char16* strUtf16, sl_reg length)
{
	m_container = _create(strUtf16, length);
}

SafeString8::SafeString8(const sl_char16* strUtf16)
{
	m_container = String8::_create(strUtf16, -1);
}

SafeString8::SafeString8(const sl_char16* strUtf16, sl_reg length)
{
	m_container = String8::_create(strUtf16, length);
}

String16::String16(const sl_char16* strUtf16)
{
	m_container = _create(strUtf16, -1);
}

String16::String16(const sl_char16* strUtf16, sl_reg length)
{
	m_container = _create(strUtf16, length);
}

SafeString16::SafeString16(const sl_char16* strUtf16)
{
	m_container = String16::_create(strUtf16, -1);
}

SafeString16::SafeString16(const sl_char16* strUtf16, sl_reg length)
{
	m_container = String16::_create(strUtf16, length);
}


String8::String8(const sl_char32* strUtf32)
{
	m_container = _create(strUtf32, -1);
}

String8::String8(const sl_char32* strUtf32, sl_reg length)
{
	m_container = _create(strUtf32, length);
}

SafeString8::SafeString8(const sl_char32* strUtf32)
{
	m_container = String8::_create(strUtf32, -1);
}

SafeString8::SafeString8(const sl_char32* strUtf32, sl_reg length)
{
	m_container = String8::_create(strUtf32, length);
}

String16::String16(const sl_char32* strUtf32)
{
	m_container = _create(strUtf32, -1);
}

String16::String16(const sl_char32* strUtf32, sl_reg length)
{
	m_container = _create(strUtf32, length);
}

SafeString16::SafeString16(const sl_char32* strUtf32)
{
	m_container = String16::_create(strUtf32, -1);
}

SafeString16::SafeString16(const sl_char32* strUtf32, sl_reg length)
{
	m_container = String16::_create(strUtf32, length);
}


String8 String8::allocate(sl_size len)
{
	return _alloc(len);
}

String16 String16::allocate(sl_size len)
{
	return _alloc(len);
}


String8 String8::fromStatic(const sl_char8 *sz, sl_reg len)
{
	if (sz) {
		if (len == 0) {
			return _String8_Empty.container;
		}
		StringContainer8* container = (StringContainer8*)(Base::createMemory(sizeof(StringContainer8)));
		if (container) {
			container->sz = (sl_char8*)sz;
			container->len = len;
			container->hash = 0;
			container->ref = 0;
			return container;
		}
	}
	return _String8_Null.container;
}

String16 String16::fromStatic(const sl_char16 *sz, sl_reg len)
{
	if (sz) {
		if (len == 0) {
			return _String16_Empty.container;
		}
		StringContainer16* container = (StringContainer16*)(Base::createMemory(sizeof(StringContainer16)));
		if (container) {
			container->sz = (sl_char16*)sz;
			container->len = len;
			container->hash = 0;
			container->ref = 0;
			return container;
		}
	}
	return _String16_Null.container;
}


String8 String8::fromUtf8(const void* utf8, sl_reg len)
{
	return String8((const sl_char8*)utf8, len);
}

String8 String8::fromUtf8(const Memory& mem)
{
	return fromUtf8(mem.getData(), mem.getSize());
}

String16 String16::fromUtf8(const void* utf8, sl_reg len)
{
	return String16((const sl_char8*)utf8, len);
}

String16 String16::fromUtf8(const Memory& mem)
{
	return fromUtf8(mem.getData(), mem.getSize());
}


String8 String8::fromUtf16(const void* utf16, sl_reg len)
{
	return String8((const sl_char16*)utf16, len);
}

String8 String8::fromUtf16(const Memory& mem)
{
	return fromUtf16(mem.getData(), mem.getSize());
}

String16 String16::fromUtf16(const void* utf16, sl_reg len)
{
	String16 str((const sl_char16*)utf16, len);
	return str;
}

String16 String16::fromUtf16(const Memory& mem)
{
	return fromUtf16(mem.getData(), mem.getSize());
}


String8 String8::fromUtf32(const void* utf32, sl_reg len)
{
	return String8((const sl_char32*)utf32, len);
}

String8 String8::fromUtf32(const Memory& mem)
{
	return fromUtf32(mem.getData(), mem.getSize());
}

String16 String16::fromUtf32(const void* utf32, sl_reg len)
{
	String16 str((const sl_char32*)utf32, len);
	return str;
}

String16 String16::fromUtf32(const Memory& mem)
{
	return fromUtf32(mem.getData(), mem.getSize());
}



String8 String8::fromUtf16BE(const void* _utf16, sl_reg len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		if (Endian::isLE()) {
			if (len < 0) {
				len = _StringBase::getSz16Length(utf16);
			}
			SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, len);
			for (sl_reg i = 0; i < len; i++) {
				buf[i] = Endian::swap16(utf16[i]);
			}
			return String8(buf, len);
		} else {
			return String8(utf16, len);
		}
	}
	return String8::null();
}

String8 String8::fromUtf16BE(const Memory& mem)
{
	return fromUtf16BE(mem.getData(), mem.getSize());
}

String16 String16::fromUtf16BE(const void* _utf16, sl_reg len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		String16 ret(utf16, len);
		if (Endian::isLE()) {
			sl_char16* buf = ret.getData();
			sl_size n = ret.getLength();
			for (sl_size i = 0; i < n; i++) {
				buf[i] = Endian::swap16(buf[i]);
			}
		}
		return ret;
	}
	return String16::null();
}

String16 String16::fromUtf16BE(const Memory& mem)
{
	return fromUtf16BE(mem.getData(), mem.getSize());
}


String8 String8::fromUtf16LE(const void* _utf16, sl_reg len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		if (Endian::isBE()) {
			if (len < 0) {
				len = _StringBase::getSz16Length(utf16);
			}
			SLIB_SCOPED_BUFFER(sl_char16, 1024, buf, len);
			for (sl_reg i = 0; i < len; i++) {
				buf[i] = Endian::swap16(utf16[i]);
			}
			return String8(buf, len);
		} else {
			return String8(utf16, len);
		}
	}
	return String8::null();
}

String8 String8::fromUtf16LE(const Memory& mem)
{
	return fromUtf16LE(mem.getData(), mem.getSize());
}

String16 String16::fromUtf16LE(const void* _utf16, sl_reg len)
{
	sl_char16* utf16 = (sl_char16*)_utf16;
	if (utf16) {
		String16 ret(utf16, len);
		if (Endian::isBE()) {
			sl_char16* buf = ret.getData();
			sl_size n = ret.getLength();
			for (sl_size i = 0; i < n; i++) {
				buf[i] = Endian::swap16(buf[i]);
			}
		}
		return ret;
	}
	return String16::null();
}

String16 String16::fromUtf16LE(const Memory& mem)
{
	return fromUtf16LE(mem.getData(), mem.getSize());
}


String8 String8::fromUtf(const void* _buf, sl_size len)
{
	sl_char8* buf = (sl_char8*)_buf;
	if (!buf) {
		return String8::null();
	}
	if (len == 0) {
		return String8::getEmpty();
	}
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

String8 String8::fromUtf(const Memory& mem)
{
	return fromUtf(mem.getData(), mem.getSize());
}

String16 String16::fromUtf(const void* _buf, sl_size len)
{
	sl_char8* buf = (sl_char8*)_buf;
	if (!buf) {
		return String16::null();
	}
	if (len == 0) {
		return String16::getEmpty();
	}
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

String16 String16::fromUtf(const Memory& mem)
{
	return fromUtf(mem.getData(), mem.getSize());
}


sl_bool SafeString8::isEmpty() const
{
	String8 s(*this);
	return s.getLength() == 0;
}

sl_bool SafeString16::isEmpty() const
{
	String16 s(*this);
	return s.getLength() == 0;
}


sl_bool SafeString8::isNotEmpty() const
{
	String8 s(*this);
	return s.getLength() != 0;
}

sl_bool SafeString16::isNotEmpty() const
{
	String16 s(*this);
	return s.getLength() != 0;
}


void String8::setNull()
{
	if (m_container != _String8_Null.container) {
		_replaceContainer(_String8_Null.container);
	}
}

void String16::setNull()
{
	if (m_container != _String16_Null.container) {
		_replaceContainer(_String16_Null.container);
	}
}

void SafeString8::setNull()
{
	if (m_container != _String8_Null.container) {
		_replaceContainer(_String8_Null.container);
	}
}

void SafeString16::setNull()
{
	if (m_container != _String16_Null.container) {
		_replaceContainer(_String16_Null.container);
	}
}


void String8::setEmpty()
{
	if (m_container != _String8_Empty.container) {
		_replaceContainer(_String8_Empty.container);
	}
}

void String16::setEmpty()
{
	if (m_container != _String16_Empty.container) {
		_replaceContainer(_String16_Empty.container);
	}
}

void SafeString8::setEmpty()
{
	if (m_container != _String8_Empty.container) {
		_replaceContainer(_String8_Empty.container);
	}
}

void SafeString16::setEmpty()
{
	if (m_container != _String16_Empty.container) {
		_replaceContainer(_String16_Empty.container);
	}
}


sl_size SafeString8::getLength() const
{
	String8 s(*this);
	return s.getLength();
}

sl_size SafeString16::getLength() const
{
	String16 s(*this);
	return s.getLength();
}


void String8::setLength(sl_size len)
{
	m_container->len = len;
}

void String16::setLength(sl_size len)
{
	m_container->len = len;
}


template <class CT>
SLIB_INLINE sl_uint32 _String_calcHash(const CT* buf, sl_size len)
{
	sl_uint32 hash = 0;
	for (sl_size i = 0; i < len; i++) {
		sl_uint32 ch = buf[i];
		hash = hash * 31 + ch;
	}
	hash = sl_rehash(hash);
	return hash;
}

sl_uint32 String8::getHashCode() const
{
	sl_size n = m_container->len;
	if (n == 0) {
		return 0;
	}
	sl_uint32 hash = m_container->hash;
	if (hash == 0) {
		hash = _String_calcHash(m_container->sz, n);
		m_container->hash = hash;
	}
	return hash;
}

sl_uint32 String16::getHashCode() const
{
	sl_size n = m_container->len;
	if (n == 0) {
		return 0;
	}
	sl_uint32 hash = m_container->hash;
	if (hash == 0) {
		hash = _String_calcHash(m_container->sz, n);
		m_container->hash = hash;
	}
	return hash;
}

sl_uint32 SafeString8::getHashCode() const
{
	String8 s(*this);
	return s.getHashCode();
}

sl_uint32 SafeString16::getHashCode() const
{
	String16 s(*this);
	return s.getHashCode();
}


void String8::setHashCode(sl_uint32 hash)
{
	m_container->hash = hash;
}

void String16::setHashCode(sl_uint32 hash)
{
	m_container->hash = hash;
}


sl_char8 String8::getAt(sl_reg index) const
{
	if (index >= 0 && index < (sl_reg)(m_container->len)) {
		return m_container->sz[index];
	}
	return 0;
}

sl_char16 String16::getAt(sl_reg index) const
{
	if (index >= 0 && index < (sl_reg)(m_container->len)) {
		return m_container->sz[index];
	}
	return 0;
}


sl_bool String8::setAt(sl_reg index, sl_char8 ch)
{
	if (index >= 0 && index < (sl_reg)(m_container->len)) {
		m_container->sz[index] = ch;
		return sl_true;
	}
	return sl_false;
}

sl_bool String16::setAt(sl_reg index, sl_char16 ch)
{
	if (index >= 0 && index < (sl_reg)(m_container->len)) {
		m_container->sz[index] = ch;
		return sl_true;
	}
	return sl_false;
}


String8& String8::operator=(String8&& other)
{
	if (this != &other) {
		_replaceContainer(other.m_container);
		other.m_container = _String8_Null.container;
	}
	return *this;
}

String16& String16::operator=(String16&& other)
{
	if (this != &other) {
		_replaceContainer(other.m_container);
		other.m_container = _String16_Null.container;
	}
	return *this;
}

SafeString8& SafeString8::operator=(SafeString8&& other)
{
	if (this != &other) {
		_replaceContainer(other.m_container);
		other.m_container = _String8_Null.container;
	}
	return *this;
}

SafeString16& SafeString16::operator=(SafeString16&& other)
{
	if (this != &other) {
		_replaceContainer(other.m_container);
		other.m_container = _String16_Null.container;
	}
	return *this;
}


String8& String8::operator=(SafeString8&& other)
{
	if ((void*)this != (void*)(&other)) {
		_replaceContainer(other.m_container);
		other.m_container = _String8_Null.container;
	}
	return *this;
}

String16& String16::operator=(SafeString16&& other)
{
	if ((void*)this != (void*)(&other)) {
		_replaceContainer(other.m_container);
		other.m_container = _String16_Null.container;
	}
	return *this;
}

SafeString8& SafeString8::operator=(String8&& other)
{
	if ((void*)this != (void*)(&other)) {
		_replaceContainer(other.m_container);
		other.m_container = _String8_Null.container;
	}
	return *this;
}

SafeString16& SafeString16::operator=(String16&& other)
{
	if ((void*)this != (void*)(&other)) {
		_replaceContainer(other.m_container);
		other.m_container = _String16_Null.container;
	}
	return *this;
}


String8& String8::operator=(const String8& other)
{
	StringContainer8* container = other.m_container;
	if (m_container != container) {
		container->increaseReference();
		_replaceContainer(container);
	}
	return *this;
}

String16& String16::operator=(const String16& other)
{
	StringContainer16* container = other.m_container;
	if (m_container != container) {
		container->increaseReference();
		_replaceContainer(container);
	}
	return *this;
}

SafeString8& SafeString8::operator=(const String8& other)
{
	StringContainer8* container = other.m_container;
	if (m_container != container) {
		container->increaseReference();
		_replaceContainer(container);
	}
	return *this;
}

SafeString16& SafeString16::operator=(const String16& other)
{
	StringContainer16* container = other.m_container;
	if (m_container != container) {
		container->increaseReference();
		_replaceContainer(container);
	}
	return *this;
}


String8& String8::operator=(const SafeString8& other)
{
	if (m_container != other.m_container) {
		_replaceContainer(other._retainContainer());
	}
	return *this;
}

String16& String16::operator=(const SafeString16& other)
{
	if (m_container != other.m_container) {
		_replaceContainer(other._retainContainer());
	}
	return *this;
}

SafeString8& SafeString8::operator=(const SafeString8& other)
{
	if (m_container != other.m_container) {
		_replaceContainer(other._retainContainer());
	}
	return *this;
}

SafeString16& SafeString16::operator=(const SafeString16& other)
{
	if (m_container != other.m_container) {
		_replaceContainer(other._retainContainer());
	}
	return *this;
}


String8& String8::operator=(const String16& other)
{
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(_create(other.getData(), len));
		}
	}
	return *this;
}

String16& String16::operator=(const String8& other)
{
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(_create(other.getData(), len));
		}
	}
	return *this;
}

SafeString8& SafeString8::operator=(const String16& other)
{
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(String8::_create(other.getData(), len));
		}
	}
	return *this;
}

SafeString16& SafeString16::operator=(const String8& other)
{
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(String16::_create(other.getData(), len));
		}
	}
	return *this;
}


String8& String8::operator=(const SafeString16& _other)
{
	String16 other(_other);
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(_create(other.getData(), len));
		}
	}
	return *this;
}

String16& String16::operator=(const SafeString8& _other)
{
	String8 other(_other);
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(_create(other.getData(), len));
		}
	}
	return *this;
}

SafeString8& SafeString8::operator=(const SafeString16& _other)
{
	String16 other(_other);
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(String8::_create(other.getData(), len));
		}
	}
	return *this;
}

SafeString16& SafeString16::operator=(const SafeString8& _other)
{
	String8 other(_other);
	if (other.isNull()) {
		setNull();
	} else {
		sl_size len = other.getLength();
		if (len == 0) {
			setEmpty();
		} else {
			_replaceContainer(String16::_create(other.getData(), len));
		}
	}
	return *this;
}


String8& String8::operator=(const sl_char8* utf8)
{
	if (utf8) {
		_replaceContainer(_create(utf8, -1));
	} else {
		setNull();
	}
	return *this;
}

String16& String16::operator=(const sl_char8* utf8)
{
	if (utf8) {
		_replaceContainer(_create(utf8, -1));
	} else {
		setNull();
	}
	return *this;
}

SafeString8& SafeString8::operator=(const sl_char8* utf8)
{
	if (utf8) {
		_replaceContainer(String8::_create(utf8, -1));
	} else {
		setNull();
	}
	return *this;
}

SafeString16& SafeString16::operator=(const sl_char8* utf8)
{
	if (utf8) {
		_replaceContainer(String16::_create(utf8, -1));
	} else {
		setNull();
	}
	return *this;
}


String8& String8::operator=(const sl_char16* utf16)
{
	if (utf16) {
		_replaceContainer(_create(utf16, -1));
	} else {
		setNull();
	}
	return *this;
}

String16& String16::operator=(const sl_char16* utf16)
{
	if (utf16) {
		_replaceContainer(_create(utf16, -1));
	} else {
		setNull();
	}
	return *this;
}

SafeString8& SafeString8::operator=(const sl_char16* utf16)
{
	if (utf16) {
		_replaceContainer(String8::_create(utf16, -1));
	} else {
		setNull();
	}
	return *this;
}

SafeString16& SafeString16::operator=(const sl_char16* utf16)
{
	if (utf16) {
		_replaceContainer(String16::_create(utf16, -1));
	} else {
		setNull();
	}
	return *this;
}


String8& String8::operator=(const sl_char32* utf32)
{
	if (utf32) {
		_replaceContainer(_create(utf32, -1));
	} else {
		setNull();
	}
	return *this;
}

String16& String16::operator=(const sl_char32* utf32)
{
	if (utf32) {
		_replaceContainer(_create(utf32, -1));
	} else {
		setNull();
	}
	return *this;
}

SafeString8& SafeString8::operator=(const sl_char32* utf32)
{
	if (utf32) {
		_replaceContainer(String8::_create(utf32, -1));
	} else {
		setNull();
	}
	return *this;
}

SafeString16& SafeString16::operator=(const sl_char32* utf32)
{
	if (utf32) {
		_replaceContainer(String16::_create(utf32, -1));
	} else {
		setNull();
	}
	return *this;
}


String8 String8::operator+(const String8& other) const
{
	sl_size n = getLength();
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return _merge8(getData(), n, other.getData(), other.getLength());
	}
	return getNotNull();
}

String8& String8::operator+=(const String8& other)
{
	sl_size n = getLength();
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(_merge8(getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}

String16 String16::operator+(const String16& other) const
{
	sl_size n = getLength();
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return _merge16(getData(), n, other.getData(), other.getLength());
	}
	return getNotNull();
}

String16& String16::operator+=(const String16& other)
{
	sl_size n = getLength();
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(_merge16(getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}

String8 SafeString8::operator+(const String8& other) const
{
	String8 s(*this);
	sl_size n = s.getLength();
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return String8::_merge8(s.getData(), n, other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const String8& other)
{
	String8 s(*this);
	sl_size n = s.getLength();
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(String8::_merge8(s.getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}

String16 SafeString16::operator+(const String16& other) const
{
	String16 s(*this);
	sl_size n = s.getLength();
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return String16::_merge16(s.getData(), n, other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const String16& other)
{
	String16 s(*this);
	sl_size n = s.getLength();
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(String16::_merge16(s.getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}


String8 String8::operator+(const String16& other) const
{
	if (other.isNotEmpty()) {
		return _merge16(getData(), getLength(), other.getData(), other.getLength());
	}
	return getNotNull();
}

String8& String8::operator+=(const String16& other)
{
	if (other.isNotEmpty()) {
		_replaceContainer(_merge16(getData(), getLength(), other.getData(), other.getLength()));
	}
	return *this;
}

String16 String16::operator+(const String8& other) const
{
	if (other.isNotEmpty()) {
		return _merge8(getData(), getLength(), other.getData(), other.getLength());
	}
	return getNotNull();
}

String16& String16::operator+=(const String8& other)
{
	if (other.isNotEmpty()) {
		_replaceContainer(_merge8(getData(), getLength(), other.getData(), other.getLength()));
	}
	return *this;
}

String8 SafeString8::operator+(const String16& other) const
{
	String8 s(*this);
	if (other.isNotEmpty()) {
		return String8::_merge16(s.getData(), s.getLength(), other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const String16& other)
{
	if (other.isNotEmpty()) {
		String8 s(*this);
		_replaceContainer(String8::_merge16(s.getData(), s.getLength(), other.getData(), other.getLength()));
	}
	return *this;
}

String16 SafeString16::operator+(const String8& other) const
{
	String16 s(*this);
	if (other.isNotEmpty()) {
		return String16::_merge8(s.getData(), s.getLength(), other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const String8& other)
{
	if (other.isNotEmpty()) {
		String16 s(*this);
		_replaceContainer(String16::_merge8(s.getData(), s.getLength(), other.getData(), other.getLength()));
	}
	return *this;
}


String8 String8::operator+(const SafeString8& _other) const
{
	sl_size n = getLength();
	String8 other(_other);
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return _merge8(getData(), n, other.getData(), other.getLength());
	}
	return getNotNull();
}

String8& String8::operator+=(const SafeString8& _other)
{
	sl_size n = getLength();
	String8 other(_other);
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(_merge8(getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}

String16 String16::operator+(const SafeString16& _other) const
{
	sl_size n = getLength();
	String16 other(_other);
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return _merge16(getData(), n, other.getData(), other.getLength());
	}
	return getNotNull();
}

String16& String16::operator+=(const SafeString16& _other)
{
	sl_size n = getLength();
	String16 other(_other);
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(_merge16(getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}

String8 SafeString8::operator+(const SafeString8& _other) const
{
	String8 s(*this);
	sl_size n = s.getLength();
	String8 other(_other);
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return String8::_merge8(s.getData(), n, other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const SafeString8& _other)
{
	String8 s(*this);
	sl_size n = s.getLength();
	String8 other(_other);
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(String8::_merge8(s.getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}

String16 SafeString16::operator+(const SafeString16& _other) const
{
	String16 s(*this);
	sl_size n = s.getLength();
	String16 other(_other);
	if (n == 0) {
		return other.getNotNull();
	}
	if (other.isNotEmpty()) {
		return String16::_merge16(s.getData(), n, other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const SafeString16& _other)
{
	String16 s(*this);
	sl_size n = s.getLength();
	String16 other(_other);
	if (n == 0) {
		return (*this = other.getNotNull());
	}
	if (other.isNotEmpty()) {
		_replaceContainer(String16::_merge16(s.getData(), n, other.getData(), other.getLength()));
	}
	return *this;
}


String8 String8::operator+(const SafeString16& _other) const
{
	String16 other(_other);
	if (other.isNotEmpty()) {
		return _merge16(getData(), getLength(), other.getData(), other.getLength());
	}
	return getNotNull();
}

String8& String8::operator+=(const SafeString16& _other)
{
	String16 other(_other);
	if (other.isNotEmpty()) {
		_replaceContainer(_merge16(getData(), getLength(), other.getData(), other.getLength()));
	}
	return *this;
}

String16 String16::operator+(const SafeString8& _other) const
{
	String8 other(_other);
	if (other.isNotEmpty()) {
		return _merge8(getData(), getLength(), other.getData(), other.getLength());
	}
	return getNotNull();
}

String16& String16::operator+=(const SafeString8& _other)
{
	String8 other(_other);
	if (other.isNotEmpty()) {
		_replaceContainer(_merge8(getData(), getLength(), other.getData(), other.getLength()));
	}
	return *this;
}

String8 SafeString8::operator+(const SafeString16& _other) const
{
	String8 s(*this);
	String16 other(_other);
	if (other.isNotEmpty()) {
		return String8::_merge16(s.getData(), s.getLength(), other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const SafeString16& _other)
{
	String16 other(_other);
	if (other.isNotEmpty()) {
		String8 s(*this);
		_replaceContainer(String8::_merge16(s.getData(), s.getLength(), other.getData(), other.getLength()));
	}
	return *this;
}

String16 SafeString16::operator+(const SafeString8& _other) const
{
	String16 s(*this);
	String8 other(_other);
	if (other.isNotEmpty()) {
		return String16::_merge8(s.getData(), s.getLength(), other.getData(), other.getLength());
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const SafeString8& _other)
{
	String16 s(*this);
	String8 other(_other);
	if (other.isNotEmpty()) {
		_replaceContainer(String16::_merge8(s.getData(), s.getLength(), other.getData(), other.getLength()));
	}
	return *this;
}


String8 String8::operator+(const sl_char8* utf8) const
{
	if (utf8) {
		return _merge8(getData(), getLength(), utf8, -1);
	}
	return getNotNull();
}

String8& String8::operator+=(const sl_char8* utf8)
{
	if (utf8) {
		_replaceContainer(_merge8(getData(), getLength(), utf8, -1));
	}
	return *this;
}

String16 String16::operator+(const sl_char8* utf8) const
{
	if (utf8) {
		return _merge8(getData(), getLength(), utf8, -1);
	}
	return getNotNull();
}

String16& String16::operator+=(const sl_char8* utf8)
{
	if (utf8) {
		_replaceContainer(_merge8(getData(), getLength(), utf8, -1));
	}
	return *this;
}

String8 SafeString8::operator+(const sl_char8* utf8) const
{
	String8 s(*this);
	if (utf8) {
		return String8::_merge8(s.getData(), s.getLength(), utf8, -1);
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const sl_char8* utf8)
{
	String8 s(*this);
	if (utf8) {
		_replaceContainer(String8::_merge8(s.getData(), s.getLength(), utf8, -1));
	}
	return *this;
}

String16 SafeString16::operator+(const sl_char8* utf8) const
{
	String16 s(*this);
	if (utf8) {
		return String16::_merge8(s.getData(), s.getLength(), utf8, -1);
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const sl_char8* utf8)
{
	String16 s(*this);
	if (utf8) {
		_replaceContainer(String16::_merge8(s.getData(), s.getLength(), utf8, -1));
	}
	return *this;
}


String8 String8::operator+(const sl_char16* utf16) const
{
	if (utf16) {
		return _merge16(getData(), getLength(), utf16, -1);
	}
	return getNotNull();
}

String8& String8::operator+=(const sl_char16* utf16)
{
	if (utf16) {
		_replaceContainer(_merge16(getData(), getLength(), utf16, -1));
	}
	return *this;
}

String16 String16::operator+(const sl_char16* utf16) const
{
	if (utf16) {
		return _merge16(getData(), getLength(), utf16, -1);
	}
	return getNotNull();
}

String16& String16::operator+=(const sl_char16* utf16)
{
	if (utf16) {
		_replaceContainer(_merge16(getData(), getLength(), utf16, -1));
	}
	return *this;
}

String8 SafeString8::operator+(const sl_char16* utf16) const
{
	String8 s(*this);
	if (utf16) {
		return String8::_merge16(s.getData(), s.getLength(), utf16, -1);
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const sl_char16* utf16)
{
	String8 s(*this);
	if (utf16) {
		_replaceContainer(String8::_merge16(s.getData(), s.getLength(), utf16, -1));
	}
	return *this;
}

String16 SafeString16::operator+(const sl_char16* utf16) const
{
	String16 s(*this);
	if (utf16) {
		return String16::_merge16(s.getData(), s.getLength(), utf16, -1);
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const sl_char16* utf16)
{
	String16 s(*this);
	if (utf16) {
		_replaceContainer(String16::_merge16(s.getData(), s.getLength(), utf16, -1));
	}
	return *this;
}


String8 String8::operator+(const sl_char32* utf32) const
{
	if (utf32) {
		return _merge32(getData(), getLength(), utf32, -1);
	}
	return getNotNull();
}

String8& String8::operator+=(const sl_char32* utf32)
{
	if (utf32) {
		_replaceContainer(_merge32(getData(), getLength(), utf32, -1));
	}
	return *this;
}

String16 String16::operator+(const sl_char32* utf32) const
{
	if (utf32) {
		return _merge32(getData(), getLength(), utf32, -1);
	}
	return getNotNull();
}

String16& String16::operator+=(const sl_char32* utf32)
{
	if (utf32) {
		_replaceContainer(_merge32(getData(), getLength(), utf32, -1));
	}
	return *this;
}

String8 SafeString8::operator+(const sl_char32* utf32) const
{
	String8 s(*this);
	if (utf32) {
		return String8::_merge32(s.getData(), s.getLength(), utf32, -1);
	}
	return s.getNotNull();
}

SafeString8& SafeString8::operator+=(const sl_char32* utf32)
{
	String8 s(*this);
	if (utf32) {
		_replaceContainer(String8::_merge32(s.getData(), s.getLength(), utf32, -1));
	}
	return *this;
}

String16 SafeString16::operator+(const sl_char32* utf32) const
{
	String16 s(*this);
	if (utf32) {
		return String16::_merge32(s.getData(), s.getLength(), utf32, -1);
	}
	return s.getNotNull();
}

SafeString16& SafeString16::operator+=(const sl_char32* utf32)
{
	String16 s(*this);
	if (utf32) {
		_replaceContainer(String16::_merge32(s.getData(), s.getLength(), utf32, -1));
	}
	return *this;
}


String8 operator+(const sl_char8* utf8, const String8& s)
{
	if (utf8) {
		return String8::_merge8(utf8, -1, s.getData(), s.getLength());
	}
	return s.getNotNull();
}

String16 operator+(const sl_char8* utf8, const String16& s)
{
	if (utf8) {
		return String16::_merge8(utf8, -1, s.getData(), s.getLength());
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
		return String8::_merge16(utf16, -1, s.getData(), s.getLength());
	}
	return s.getNotNull();
}

String16 operator+(const sl_char16* utf16, const String16& s)
{
	if (utf16) {
		return String16::_merge16(utf16, -1, s.getData(), s.getLength());
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
		return String8::_merge32(utf32, -1, s.getData(), s.getLength());
	}
	return s.getNotNull();
}

String16 operator+(const sl_char32* utf32, const String16& s)
{
	if (utf32) {
		return String16::_merge32(utf32, -1, s.getData(), s.getLength());
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


String8 String8::operator+(sl_int32 number) const
{
	return *this + String8::fromInt32(number);
}

String8& String8::operator+=(sl_int32 number)
{
	return (*this += String8::fromInt32(number));
}

String8 operator+(sl_int32 number, const String8& other)
{
	return String8::fromInt32(number) + other;
}

String16 String16::operator+(sl_int32 number) const
{
	return *this + String16::fromInt32(number);
}

String16& String16::operator+=(sl_int32 number)
{
	return (*this += String16::fromInt32(number));
}

String16 operator+(sl_int32 number, const String16& other)
{
	return String16::fromInt32(number) + other;
}

String8 SafeString8::operator+(sl_int32 number) const
{
	return *this + String8::fromInt32(number);
}

SafeString8& SafeString8::operator+=(sl_int32 number)
{
	return (*this += String8::fromInt32(number));
}

String8 operator+(sl_int32 number, const SafeString8& other)
{
	return String8::fromInt32(number) + other;
}

String16 SafeString16::operator+(sl_int32 number) const
{
	return *this + String16::fromInt32(number);
}

SafeString16& SafeString16::operator+=(sl_int32 number)
{
	return (*this += String16::fromInt32(number));
}

String16 operator+(sl_int32 number, const SafeString16& other)
{
	return String16::fromInt32(number) + other;
}


String8 String8::operator+(sl_uint32 number) const
{
	return *this + String8::fromUint32(number);
}

String8& String8::operator+=(sl_uint32 number)
{
	return (*this += String8::fromUint32(number));
}

String8 operator+(sl_uint32 number, const String8& other)
{
	return String8::fromUint32(number) + other;
}

String16 String16::operator+(sl_uint32 number) const
{
	return *this + String16::fromUint32(number);
}

String16& String16::operator+=(sl_uint32 number)
{
	return (*this += String16::fromUint32(number));
}

String16 operator+(sl_uint32 number, const String16& other)
{
	return String16::fromUint32(number) + other;
}

String8 SafeString8::operator+(sl_uint32 number) const
{
	return *this + String8::fromUint32(number);
}

SafeString8& SafeString8::operator+=(sl_uint32 number)
{
	return (*this += String8::fromUint32(number));
}

String8 operator+(sl_uint32 number, const SafeString8& other)
{
	return String8::fromUint32(number) + other;
}

String16 SafeString16::operator+(sl_uint32 number) const
{
	return *this + String16::fromUint32(number);
}

SafeString16& SafeString16::operator+=(sl_uint32 number)
{
	return (*this += String16::fromUint32(number));
}

String16 operator+(sl_uint32 number, const SafeString16& other)
{
	return String16::fromUint32(number) + other;
}


String8 String8::operator+(sl_int64 number) const
{
	return *this + String8::fromInt64(number);
}

String8& String8::operator+=(sl_int64 number)
{
	return (*this += String8::fromInt64(number));
}

String8 operator+(sl_int64 number, const String8& other)
{
	return String8::fromInt64(number) + other;
}

String16 String16::operator+(sl_int64 number) const
{
	return *this + String16::fromInt64(number);
}

String16& String16::operator+=(sl_int64 number)
{
	return (*this += String16::fromInt64(number));
}

String16 operator+(sl_int64 number, const String16& other)
{
	return String16::fromInt64(number) + other;
}

String8 SafeString8::operator+(sl_int64 number) const
{
	return *this + String8::fromInt64(number);
}

SafeString8& SafeString8::operator+=(sl_int64 number)
{
	return (*this += String8::fromInt64(number));
}

String8 operator+(sl_int64 number, const SafeString8& other)
{
	return String8::fromInt64(number) + other;
}

String16 SafeString16::operator+(sl_int64 number) const
{
	return *this + String16::fromInt64(number);
}

SafeString16& SafeString16::operator+=(sl_int64 number)
{
	return (*this += String16::fromInt64(number));
}

String16 operator+(sl_int64 number, const SafeString16& other)
{
	return String16::fromInt64(number) + other;
}


String8 String8::operator+(sl_uint64 number) const
{
	return *this + String8::fromUint64(number);
}

String8& String8::operator+=(sl_uint64 number)
{
	return (*this += String8::fromUint64(number));
}

String8 operator+(sl_uint64 number, const String8& other)
{
	return String8::fromUint64(number) + other;
}

String16 String16::operator+(sl_uint64 number) const
{
	return *this + String16::fromUint64(number);
}

String16& String16::operator+=(sl_uint64 number)
{
	return (*this += String16::fromUint64(number));
}

String16 operator+(sl_uint64 number, const String16& other)
{
	return String16::fromUint64(number) + other;
}

String8 SafeString8::operator+(sl_uint64 number) const
{
	return *this + String8::fromUint64(number);
}

SafeString8& SafeString8::operator+=(sl_uint64 number)
{
	return (*this += String8::fromUint64(number));
}

String8 operator+(sl_uint64 number, const SafeString8& other)
{
	return String8::fromUint64(number) + other;
}

String16 SafeString16::operator+(sl_uint64 number) const
{
	return *this + String16::fromUint64(number);
}

SafeString16& SafeString16::operator+=(sl_uint64 number)
{
	return (*this += String16::fromUint64(number));
}

String16 operator+(sl_uint64 number, const SafeString16& other)
{
	return String16::fromUint64(number) + other;
}


String8 String8::operator+(float number) const
{
	return *this + String8::fromFloat(number);
}

String8& String8::operator+=(float number)
{
	return (*this += String8::fromFloat(number));
}

String8 operator+(float number, const String8& other)
{
	return String8::fromFloat(number) + other;
}

String16 String16::operator+(float number) const
{
	return *this + String16::fromFloat(number);
}

String16& String16::operator+=(float number)
{
	return (*this += String16::fromFloat(number));
}

String16 operator+(float number, const String16& other)
{
	return String16::fromFloat(number) + other;
}

String8 SafeString8::operator+(float number) const
{
	return *this + String8::fromFloat(number);
}

SafeString8& SafeString8::operator+=(float number)
{
	return (*this += String8::fromFloat(number));
}

String8 operator+(float number, const SafeString8& other)
{
	return String8::fromFloat(number) + other;
}

String16 SafeString16::operator+(float number) const
{
	return *this + String16::fromFloat(number);
}

SafeString16& SafeString16::operator+=(float number)
{
	return (*this += String16::fromFloat(number));
}

String16 operator+(float number, const SafeString16& other)
{
	return String16::fromFloat(number) + other;
}


String8 String8::operator+(double number) const
{
	return *this + String8::fromDouble(number);
}

String8& String8::operator+=(double number)
{
	return (*this += String8::fromDouble(number));
}

String8 operator+(double number, const String8& other)
{
	return String8::fromDouble(number) + other;
}

String16 String16::operator+(double number) const
{
	return *this + String16::fromDouble(number);
}

String16& String16::operator+=(double number)
{
	return (*this += String16::fromDouble(number));
}

String16 operator+(double number, const String16& other)
{
	return String16::fromDouble(number) + other;
}

String8 SafeString8::operator+(double number) const
{
	return *this + String8::fromDouble(number);
}

SafeString8& SafeString8::operator+=(double number)
{
	return (*this += String8::fromDouble(number));
}

String8 operator+(double number, const SafeString8& other)
{
	return String8::fromDouble(number) + other;
}

String16 SafeString16::operator+(double number) const
{
	return *this + String16::fromDouble(number);
}

SafeString16& SafeString16::operator+=(double number)
{
	return (*this += String16::fromDouble(number));
}

String16 operator+(double number, const SafeString16& other)
{
	return String16::fromDouble(number) + other;
}


String8 String8::operator+(sl_bool value) const
{
	return *this + String8::fromBoolean(value);
}

String8& String8::operator+=(sl_bool value)
{
	return (*this += String8::fromBoolean(value));
}

String8 operator+(sl_bool value, const String8& other)
{
	return String8::fromBoolean(value) + other;
}

String16 String16::operator+(sl_bool value) const
{
	return *this + String16::fromBoolean(value);
}

String16& String16::operator+=(sl_bool value)
{
	return (*this += String16::fromBoolean(value));
}

String16 operator+(sl_bool value, const String16& other)
{
	return String8::fromBoolean(value) + other;
}

String8 SafeString8::operator+(sl_bool value) const
{
	return *this + String8::fromBoolean(value);
}

SafeString8& SafeString8::operator+=(sl_bool value)
{
	return (*this += String8::fromBoolean(value));
}

String8 operator+(sl_bool value, const SafeString8& other)
{
	return String8::fromBoolean(value) + other;
}

String16 SafeString16::operator+(sl_bool value) const
{
	return *this + String16::fromBoolean(value);
}

SafeString16& SafeString16::operator+=(sl_bool value)
{
	return (*this += String16::fromBoolean(value));
}

String16 operator+(sl_bool value, const SafeString16& other)
{
	return String8::fromBoolean(value) + other;
}


sl_bool String8::equals(const String8& other) const
{
	sl_char8* s1 = getData();
	sl_char8* s2 = other.getData();
	if (s1 == s2) {
		return sl_true;
	}
	sl_size l1 = getLength();
	sl_size l2 = other.getLength();
	if (l1 != l2) {
		return sl_false;
	}
	return _equals8(s1, l1, s2, l2);
}

sl_bool String16::equals(const String16& other) const
{
	sl_char16* s1 = getData();
	sl_char16* s2 = other.getData();
	if (s1 == s2) {
		return sl_true;
	}
	sl_size l1 = getLength();
	sl_size l2 = other.getLength();
	if (l1 != l2) {
		return sl_false;
	}
	return _equals16(s1, l1, s2, l2);
}

sl_bool SafeString8::equals(const String8& other) const
{
	String8 s(*this);
	return s.equals(other);
}

sl_bool SafeString16::equals(const String16& other) const
{
	String16 s(*this);
	return s.equals(other);
}


sl_bool String8::equals(const String16& _other) const
{
	String8 other(_other);
	sl_size l1 = getLength();
	sl_size l2 = other.getLength();
	if (l1 != l2) {
		return sl_false;
	}
	sl_char8* s1 = getData();
	sl_char8* s2 = other.getData();
	return _equals8(s1, l1, s2, l2);
}

sl_bool String16::equals(const String8& _other) const
{
	String16 other(_other);
	sl_size l1 = getLength();
	sl_size l2 = other.getLength();
	if (l1 != l2) {
		return sl_false;
	}
	sl_char16* s1 = getData();
	sl_char16* s2 = other.getData();
	return _equals16(s1, l1, s2, l2);
}

sl_bool SafeString8::equals(const String16& other) const
{
	String8 s(*this);
	return s.equals(other);
}

sl_bool SafeString16::equals(const String8& other) const
{
	String16 s(*this);
	return s.equals(other);
}


sl_bool String8::equals(const SafeString8& _other) const
{
	String8 other(_other);
	return equals(other);
}

sl_bool String16::equals(const SafeString16& _other) const
{
	String16 other(_other);
	return equals(other);
}

sl_bool SafeString8::equals(const SafeString8& _other) const
{
	String8 s(*this);
	String8 other(_other);
	return s.equals(other);
}

sl_bool SafeString16::equals(const SafeString16& _other) const
{
	String16 s(*this);
	String16 other(_other);
	return s.equals(other);
}


sl_bool String8::equals(const SafeString16& _other) const
{
	String16 other(_other);
	return equals(other);
}

sl_bool String16::equals(const SafeString8& _other) const
{
	String8 other(_other);
	return equals(other);
}

sl_bool SafeString8::equals(const SafeString16& _other) const
{
	String8 s(*this);
	String16 other(_other);
	return s.equals(other);
}

sl_bool SafeString16::equals(const SafeString8& _other) const
{
	String16 s(*this);
	String8 other(_other);
	return s.equals(other);
}


sl_bool String8::equals(const sl_char8* utf8) const
{
	sl_char8* sz = getData();
	if (sz == utf8) {
		return sl_true;
	}
	return _equals8(sz, getLength(), utf8, -1);
}

sl_bool String16::equals(const sl_char8* utf8) const
{
	return _equals8(getData(), getLength(), utf8, -1);
}

sl_bool SafeString8::equals(const sl_char8* utf8) const
{
	String8 s(*this);
	sl_char8* sz = s.getData();
	if (sz == utf8) {
		return sl_true;
	}
	return String8::_equals8(sz, s.getLength(), utf8, -1);
}

sl_bool SafeString16::equals(const sl_char8* utf8) const
{
	String16 s(*this);
	return String16::_equals8(s.getData(), s.getLength(), utf8, -1);
}


sl_bool String8::equals(const sl_char16* utf16) const
{
	return _equals16(getData(), getLength(), utf16, -1);
}

sl_bool String16::equals(const sl_char16* utf16) const
{
	sl_char16* sz = getData();
	if (sz == utf16) {
		return sl_true;
	}
	return _equals16(sz, getLength(), utf16, -1);
}

sl_bool SafeString8::equals(const sl_char16* utf16) const
{
	String8 s(*this);
	return String8::_equals16(s.getData(), s.getLength(), utf16, -1);
}

sl_bool SafeString16::equals(const sl_char16* utf16) const
{
	String16 s(*this);
	sl_char16* sz = s.getData();
	if (sz == utf16) {
		return sl_true;
	}
	return String16::_equals16(sz, s.getLength(), utf16, -1);
}


sl_bool String8::equals(const sl_char32* utf32) const
{
	return _equals32(getData(), getLength(), utf32, -1);
}

sl_bool String16::equals(const sl_char32* utf32) const
{
	return _equals32(getData(), getLength(), utf32, -1);
}

sl_bool SafeString8::equals(const sl_char32* utf32) const
{
	String8 s(*this);
	return String8::_equals32(s.getData(), s.getLength(), utf32, -1);
}

sl_bool SafeString16::equals(const sl_char32* utf32) const
{
	String16 s(*this);
	return String16::_equals32(s.getData(), s.getLength(), utf32, -1);
}


sl_int32 String8::compare(const String8& other) const
{
	sl_char8* s1 = getData();
	sl_char8* s2 = other.getData();
	if (s1 == s2) {
		return 0;
	}
	return _compare8(s1, getLength(), s2, other.getLength());
}

sl_int32 String16::compare(const String16& other) const
{
	sl_char16* s1 = getData();
	sl_char16* s2 = other.getData();
	if (s1 == s2) {
		return 0;
	}
	return _compare16(s1, getLength(), s2, other.getLength());
}

sl_int32 SafeString8::compare(const String8& other) const
{
	String s(*this);
	return s.compare(other);
}

sl_int32 SafeString16::compare(const String16& other) const
{
	String16 s(*this);
	return s.compare(other);
}


sl_int32 String8::compare(const String16& _other) const
{
	String8 other(_other);
	return _compare8(getData(), getLength(), other.getData(), other.getLength());
}

sl_int32 String16::compare(const String8& _other) const
{
	String16 other(_other);
	return _compare16(getData(), getLength(), other.getData(), other.getLength());
}

sl_int32 SafeString8::compare(const String16& other) const
{
	String8 s(*this);
	return s.compare(other);
}

sl_int32 SafeString16::compare(const String8& other) const
{
	String16 s(*this);
	return s.compare(other);
}


sl_int32 String8::compare(const SafeString8& _other) const
{
	String8 other(_other);
	return compare(other);
}

sl_int32 String16::compare(const SafeString16& _other) const
{
	String16 other(_other);
	return compare(other);
}

sl_int32 SafeString8::compare(const SafeString8& _other) const
{
	String8 s(*this);
	String8 other(_other);
	return s.compare(other);
}

sl_int32 SafeString16::compare(const SafeString16& _other) const
{
	String16 s(*this);
	String16 other(_other);
	return s.compare(other);
}


sl_int32 String8::compare(const SafeString16& _other) const
{
	String16 other(_other);
	return compare(other);
}

sl_int32 String16::compare(const SafeString8& _other) const
{
	String8 other(_other);
	return compare(other);
}

sl_int32 SafeString8::compare(const SafeString16& _other) const
{
	String8 s(*this);
	String16 other(_other);
	return s.compare(other);
}

sl_int32 SafeString16::compare(const SafeString8& _other) const
{
	String16 s(*this);
	String8 other(_other);
	return s.compare(other);
}


sl_int32 String8::compare(const sl_char8* utf8) const
{
	sl_char8* sz = getData();
	if (sz == utf8) {
		return sl_true;
	}
	return _compare8(sz, getLength(), utf8, -1);
}

sl_int32 String16::compare(const sl_char8* utf8) const
{
	return _compare8(getData(), getLength(), utf8, -1);
}

sl_int32 SafeString8::compare(const sl_char8* utf8) const
{
	String8 s(*this);
	return s.compare(utf8);
}

sl_int32 SafeString16::compare(const sl_char8* utf8) const
{
	String16 s(*this);
	return s.compare(utf8);
}


sl_int32 String8::compare(const sl_char16* utf16) const
{
	return _compare16(getData(), getLength(), utf16, -1);
}

sl_int32 String16::compare(const sl_char16* utf16) const
{
	sl_char16* sz = getData();
	if (sz == utf16) {
		return sl_true;
	}
	return _compare16(sz, getLength(), utf16, -1);
}

sl_int32 SafeString8::compare(const sl_char16* utf16) const
{
	String8 s(*this);
	return s.compare(utf16);
}

sl_int32 SafeString16::compare(const sl_char16* utf16) const
{
	String16 s(*this);
	return s.compare(utf16);
}


sl_int32 String8::compare(const sl_char32* utf32) const
{
	return _compare32(getData(), getLength(), utf32, -1);
}

sl_int32 String16::compare(const sl_char32* utf32) const
{
	return _compare32(getData(), getLength(), utf32, -1);
}

sl_int32 SafeString8::compare(const sl_char32* utf32) const
{
	String8 s(*this);
	return s.compare(utf32);
}

sl_int32 SafeString16::compare(const sl_char32* utf32) const
{
	String16 s(*this);
	return s.compare(utf32);
}


sl_int32 String8::compare(const String8& other, sl_size len) const
{
	sl_char8* s1 = getData();
	sl_char8* s2 = other.getData();
	if (s1 == s2) {
		return 0;
	}
	sl_size l1 = getLength();
	if (l1 > len) {
		l1 = len;
	}
	sl_size l2 = other.getLength();
	if (l2 > len) {
		l2 = len;
	}
	return _compare8(s1, l1, s2, l2);
}

sl_int32 String16::compare(const String16& other, sl_size len) const
{
	sl_char16* s1 = getData();
	sl_char16* s2 = other.getData();
	if (s1 == s2) {
		return 0;
	}
	sl_size l1 = getLength();
	if (l1 > len) {
		l1 = len;
	}
	sl_size l2 = other.getLength();
	if (l2 > len) {
		l2 = len;
	}
	return _compare16(s1, l1, s2, l2);
}

sl_int32 SafeString8::compare(const String8& other, sl_size len) const
{
	String8 s(*this);
	return s.compare(other, len);
}

sl_int32 SafeString16::compare(const String16& other, sl_size len) const
{
	String16 s(*this);
	return s.compare(other, len);
}


#define _DEFINE_STRING_COMPARE_FUNCS(CLASS, OP, BODY, BODY_FRIEND) \
sl_bool CLASS::OP(const String8& other) const \
{ \
	return BODY; \
} \
sl_bool CLASS::OP(const String16& other) const \
{ \
	return BODY; \
} \
sl_bool CLASS::OP(const SafeString8& other) const \
{ \
	return BODY; \
} \
sl_bool CLASS::OP(const SafeString16& other) const \
{ \
	return BODY; \
} \
sl_bool CLASS::OP(const sl_char8* other) const \
{ \
	return BODY; \
} \
sl_bool CLASS::OP(const sl_char16* other) const \
{ \
	return BODY; \
} \
sl_bool CLASS::OP(const sl_char32* other) const \
{ \
	return BODY; \
} \
sl_bool OP(const sl_char8* other, const CLASS& s) \
{ \
	return BODY_FRIEND; \
} \
sl_bool OP(const sl_char16* other, const CLASS& s) \
{ \
	return BODY_FRIEND; \
} \
sl_bool OP(const sl_char32* other, const CLASS& s) \
{ \
	return BODY_FRIEND; \
}


_DEFINE_STRING_COMPARE_FUNCS(String8, operator==, equals(other), s.equals(other))
_DEFINE_STRING_COMPARE_FUNCS(String8, operator!=, !(equals(other)), !(s.equals(other)))
_DEFINE_STRING_COMPARE_FUNCS(String8, operator>=, compare(other)>=0, s.compare(other)<=0)
_DEFINE_STRING_COMPARE_FUNCS(String8, operator<=, compare(other)<=0, s.compare(other)>=0)
_DEFINE_STRING_COMPARE_FUNCS(String8, operator>, compare(other)>0, s.compare(other)<0)
_DEFINE_STRING_COMPARE_FUNCS(String8, operator<, compare(other)<0, s.compare(other)>0)

_DEFINE_STRING_COMPARE_FUNCS(String16, operator==, equals(other), s.equals(other))
_DEFINE_STRING_COMPARE_FUNCS(String16, operator!=, !(equals(other)), !(s.equals(other)))
_DEFINE_STRING_COMPARE_FUNCS(String16, operator>=, compare(other)>=0, s.compare(other)<=0)
_DEFINE_STRING_COMPARE_FUNCS(String16, operator<=, compare(other)<=0, s.compare(other)>=0)
_DEFINE_STRING_COMPARE_FUNCS(String16, operator>, compare(other)>0, s.compare(other)<0)
_DEFINE_STRING_COMPARE_FUNCS(String16, operator<, compare(other)<0, s.compare(other)>0)

_DEFINE_STRING_COMPARE_FUNCS(SafeString8, operator==, equals(other), s.equals(other))
_DEFINE_STRING_COMPARE_FUNCS(SafeString8, operator!=, !(equals(other)), !(s.equals(other)))
_DEFINE_STRING_COMPARE_FUNCS(SafeString8, operator>=, compare(other)>=0, s.compare(other)<=0)
_DEFINE_STRING_COMPARE_FUNCS(SafeString8, operator<=, compare(other)<=0, s.compare(other)>=0)
_DEFINE_STRING_COMPARE_FUNCS(SafeString8, operator>, compare(other)>0, s.compare(other)<0)
_DEFINE_STRING_COMPARE_FUNCS(SafeString8, operator<, compare(other)<0, s.compare(other)>0)

_DEFINE_STRING_COMPARE_FUNCS(SafeString16, operator==, equals(other), s.equals(other))
_DEFINE_STRING_COMPARE_FUNCS(SafeString16, operator!=, !(equals(other)), !(s.equals(other)))
_DEFINE_STRING_COMPARE_FUNCS(SafeString16, operator>=, compare(other)>=0, s.compare(other)<=0)
_DEFINE_STRING_COMPARE_FUNCS(SafeString16, operator<=, compare(other)<=0, s.compare(other)>=0)
_DEFINE_STRING_COMPARE_FUNCS(SafeString16, operator>, compare(other)>0, s.compare(other)<0)
_DEFINE_STRING_COMPARE_FUNCS(SafeString16, operator<, compare(other)<0, s.compare(other)>0)



String8 String8::duplicate() const
{
    return String8(getData(), getLength());
}

String16 String16::duplicate() const
{
    return String16(getData(), getLength());
}

String8 SafeString8::duplicate() const
{
    String8 s(*this);
    return s.duplicate();
}

String16 SafeString16::duplicate() const
{
    String16 s(*this);
    return s.duplicate();
}


Memory String8::toMemory() const
{
	return Memory::create(getData(), getLength()*sizeof(sl_char8));
}

Memory String16::toMemory() const
{
	return Memory::create(getData(), getLength()*sizeof(sl_char16));
}

Memory SafeString8::toMemory() const
{
    String8 s(*this);
    return s.toMemory();
}

Memory SafeString16::toMemory() const
{
	String16 s(*this);
	return s.toMemory();
}


sl_size String8::getUtf16(sl_char16* utf16, sl_size len) const
{
	return _StringBase::utf8ToUtf16(getData(), getLength(), utf16, len);
}

sl_size String16::getUtf8(sl_char8* utf8, sl_size len) const
{
	return _StringBase::utf16ToUtf8(getData(), getLength(), utf8, len);
}

sl_bool String8::getUtf16(StringData& output) const
{
	if (isEmpty()) {
		output.sz16 = (sl_char16*)("\0\0");
		output.len = 0;
		return sl_true;
	}
	Memory mem = toUtf16();
	output.refer = mem.ref;
	if (mem.isNotEmpty()) {
		output.sz16 = (const sl_char16*)(mem.getData());
		output.len = mem.getSize() / 2 - 1;
		if (output.sz16) {
			return sl_true;
		}
	}
	output.sz16 = (sl_char16*)("\0\0");
	output.len = 0;
	return sl_false;
}

sl_bool String16::getUtf8(StringData& output) const
{
	if (isEmpty()) {
		output.sz8 = "";
		output.len = 0;
		return sl_true;
	}
	Memory mem = toUtf8();
	if (mem.isNotEmpty()) {
		output.sz8 = (const sl_char8*)(mem.getData());
		output.len = mem.getSize() - 1;
		if (output.sz8) {
			return sl_true;
		}
	}
	output.sz8 = "";
	output.len = 0;
	return sl_false;
}

sl_size SafeString8::getUtf16(sl_char16* utf16, sl_size len) const
{
    String8 s(*this);
    return s.getUtf16(utf16, len);
}

sl_bool SafeString8::getUtf16(StringData& output) const
{
    String8 s(*this);
    return s.getUtf16(output);
}

sl_size SafeString16::getUtf8(sl_char8* utf8, sl_size len) const
{
	String16 s(*this);
	return s.getUtf8(utf8, len);
}

sl_bool SafeString16::getUtf8(StringData& output) const
{
	String16 s(*this);
	return s.getUtf8(output);
}


Memory String8::toUtf16() const
{
	sl_char8* sz8 = getData();
	sl_size len8 = getLength();
	sl_size len16 = _StringBase::utf8ToUtf16(sz8, len8, sl_null, -1);
	Memory memory = Memory::create(2 * (len16 + 1));
	if (memory.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(memory.getData());
		_StringBase::utf8ToUtf16(sz8, len8, buf, len16);
		buf[len16] = 0;
	}
	return memory;
}

Memory String16::toUtf8() const
{
	sl_char16* sz16 = getData();
	sl_size len16 = getLength();
	sl_size len8 = _StringBase::utf16ToUtf8(sz16, len16, sl_null, -1);
	Memory memory = Memory::create(len8 + 1);
	if (memory.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(memory.getData());
		_StringBase::utf16ToUtf8(sz16, len16, buf, len8);
		buf[len8] = 0;
	}
	return memory;
}

Memory SafeString8::toUtf16() const
{
    String8 s(*this);
    return s.toUtf16();
}

Memory SafeString16::toUtf8() const
{
	String16 s(*this);
	return s.toUtf8();
}


sl_size String8::getUtf32(sl_char32* utf32, sl_size len) const
{
	return _StringBase::utf8ToUtf32(getData(), getLength(), utf32, len);
}

sl_bool String8::getUtf32(StringData& output) const
{
	if (isEmpty()) {
		output.sz32 = (sl_char32*)("\0\0\0\0");
		output.len = 0;
		return sl_true;
	}
	Memory mem = toUtf32();
	if (mem.isNotEmpty()) {
		output.sz32 = (const sl_char32*)(mem.getData());
		output.len = mem.getSize() / 4 - 1;
		if (output.sz32) {
			return sl_true;
		}
	}
	output.sz32 = (sl_char32*)("\0\0\0\0");
	output.len = 0;
	return sl_false;
}

sl_size SafeString8::getUtf32(sl_char32* utf32, sl_size len) const
{
    String8 s(*this);
    return s.getUtf32(utf32, len);
}

sl_bool SafeString8::getUtf32(StringData& output) const
{
    String8 s(*this);
    return s.getUtf32(output);
}

Memory String8::toUtf32() const
{
	sl_char8* sz8 = getData();
	sl_size len8 = getLength();
	sl_size len32 = _StringBase::utf8ToUtf32(sz8, len8, sl_null, -1);
	Memory memory = Memory::create(4 * (len32 + 1));
	if (memory.isNotEmpty()) {
		sl_char32* buf = (sl_char32*)(memory.getData());
		_StringBase::utf8ToUtf32(sz8, len8, buf, len32);
		buf[len32] = 0;
	}
	return memory;
}

Memory SafeString8::toUtf32() const
{
    String8 s(*this);
    return s.toUtf32();
}


String8 String8::substring(sl_reg start, sl_reg end) const
{
	sl_reg count = getLength();
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
	return String8(getData() + start, end - start);
}

String16 String16::substring(sl_reg start, sl_reg end) const
{
	sl_reg count = getLength();
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
	return String16(getData() + start, end - start);
}

String8 SafeString8::substring(sl_reg start, sl_reg end) const
{
    String8 s(*this);
    return s.substring(start, end);
}

String16 SafeString16::substring(sl_reg start, sl_reg end) const
{
	String16 s(*this);
	return s.substring(start, end);
}


String8 String8::left(sl_reg len) const
{
    return substring(0, len);
}

String16 String16::left(sl_reg len) const
{
    return substring( 0, len );
}

String8 SafeString8::left(sl_reg len) const
{
    String8 s(*this);
    return s.left(len);
}

String16 SafeString16::left(sl_reg len) const
{
	String16 s(*this);
	return s.left(len);
}


String8 String8::right(sl_reg len) const
{
    return substring(getLength() - len);
}

String16 String16::right(sl_reg len) const
{
    return substring(getLength() - len);
}

String8 SafeString8::right(sl_reg len) const
{
	String8 s(*this);
	return s.right(len);
}

String16 SafeString16::right(sl_reg len) const
{
	String16 s(*this);
	return s.right(len);
}


String8 String8::mid(sl_reg start, sl_reg len) const
{
    return substring(start, start + len);
}
	
String16 String16::mid(sl_reg start, sl_reg len) const
{
    return substring(start, start+len);
}

String8 SafeString8::mid(sl_reg start, sl_reg len) const
{
	String8 s(*this);
	return s.mid(start, len);
}

String16 SafeString16::mid(sl_reg start, sl_reg len) const
{
	String16 s(*this);
	return s.mid(start, len);
}


sl_reg String8::indexOf(sl_char8 ch, sl_reg start) const
{
	sl_reg count = getLength();
	if (count == 0) {
		return -1;
	}
	if (start < 0) {
		start = 0;
	}
	if (start >= count) {
		return -1;
	}
	sl_char8* sz = getData();
	sl_char8* pt = (sl_char8*)(Base::findMemory(sz + start, ch, count - start));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_reg)(pt - sz);
	}
}

sl_reg String16::indexOf(sl_char16 ch, sl_reg start) const
{
	sl_reg count = getLength();
	if (count == 0) {
		return -1;
	}
	if (start < 0) {
		start = 0;
	}
	if (start >= count) {
		return -1;
	}
	sl_char16* sz = getData();
	sl_char16* pt = (sl_char16*)(Base::findMemory2(sz + start, ch, count - start));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_reg)(pt - sz);
	}
}

sl_reg SafeString8::indexOf(sl_char8 ch, sl_reg start) const
{
	String8 s(*this);
	return s.indexOf(ch, start);
}

sl_reg SafeString16::indexOf(sl_char16 ch, sl_reg start) const
{
	String16 s(*this);
	return s.indexOf(ch, start);
}


template <class ST, class CT, class TT>
SLIB_INLINE sl_reg _String_indexOf(const ST& str, const ST& pattern, sl_reg start)
{
	sl_reg countPat = pattern.getLength();
	if (countPat == 0) {
		return 0;
	}
	sl_reg count = str.getLength();
	if (count == 0) {
		return -1;
	}
	const CT* buf = str.getData();
	const CT* bufPat = pattern.getData();
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
			return (sl_reg)(pt - buf);
		} else {
			start = (sl_reg)(pt - buf + 1);
		}
	}
	return -1;
}

sl_reg String8::indexOf(const String8& pattern, sl_reg start) const
{
	return _String_indexOf<String8, sl_char8, _TemplateFunc8>(*this, pattern, start);
}

sl_reg String16::indexOf(const String16& pattern, sl_reg start) const
{
	return _String_indexOf<String16, sl_char16, _TemplateFunc16>(*this, pattern, start);
}

sl_reg SafeString8::indexOf(const String8& str, sl_reg start) const
{
	String8 s(*this);
	return s.indexOf(str, start);
}

sl_reg SafeString16::indexOf(const String16& str, sl_reg start) const
{
	String16 s(*this);
	return s.indexOf(str, start);
}


sl_reg String8::lastIndexOf(sl_char8 ch, sl_reg start) const
{
	sl_reg count = getLength();
	if (count == 0) {
		return -1;
	}
	if (start < 0 || start >= count) {
		start = count - 1;
	}
	sl_char8* sz = getData();
	sl_char8* pt = (sl_char8*)(Base::findMemoryReverse(sz, ch, start + 1));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_reg)(pt - sz);
	}
}

sl_reg String16::lastIndexOf(sl_char16 ch, sl_reg start) const
{
	sl_reg count = getLength();
	if (count == 0) {
		return -1;
	}
	if (start < 0 || start >= count) {
		start = count - 1;
	}
	sl_char16* sz = getData();
	sl_char16* pt = (sl_char16*)(Base::findMemoryReverse2(sz, ch, start + 1));
	if (pt == sl_null) {
		return -1;
	} else {
		return (sl_reg)(pt - sz);
	}
}

sl_reg SafeString8::lastIndexOf(sl_char8 ch, sl_reg start) const
{
	String8 s(*this);
	return s.lastIndexOf(ch, start);
}

sl_reg SafeString16::lastIndexOf(sl_char16 ch, sl_reg start) const
{
	String16 s(*this);
	return s.lastIndexOf(ch, start);
}


template <class ST, class CT, class TT>
SLIB_INLINE sl_reg _String_lastIndexOf(const ST& str, const ST& pattern, sl_reg start)
{
	sl_reg countPat = pattern.getLength();
	if (countPat == 0) {
		return -1;
	}
	sl_reg count = str.getLength();
	if (count == 0) {
		return -1;
	}
	const CT* buf = str.getData();
	const CT* bufPat = pattern.getData();
	if (start < 0 || start > count - countPat) {
		start = count - countPat;
	}
	while (start >= 0) {
		const CT* pt = (const CT*)(TT::findMemoryReverse(buf, bufPat[0], start + 1));
		if (pt == sl_null)
			return -1;
		if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
			return (sl_reg)(pt - buf);
		} else {
			start = (sl_reg)(pt - buf - 1);
		}
	}
	return -1;
}

sl_reg String8::lastIndexOf(const String8& pattern, sl_reg start) const
{
	return _String_lastIndexOf<String8, sl_char8, _TemplateFunc8>(*this, pattern, start);
}

sl_reg String16::lastIndexOf(const String16& pattern, sl_reg start) const
{
	return _String_lastIndexOf<String16, sl_char16, _TemplateFunc16>(*this, pattern, start);
}

sl_reg SafeString8::lastIndexOf(const String8& str, sl_reg start) const
{
	String8 s(*this);
	return s.indexOf(str, start);
}

sl_reg SafeString16::lastIndexOf(const String16& str, sl_reg start) const
{
	String16 s(*this);
	return s.indexOf(str, start);
}


sl_bool String8::startsWith(sl_char8 ch) const
{
	if (isEmpty()) {
		return sl_false;
	} else {
		return m_container->sz[0] == ch;
	}
}

sl_bool String16::startsWith(sl_char16 ch) const
{
	if (isEmpty()) {
		return sl_false;
	} else {
		return m_container->sz[0] == ch;
	}
}

sl_bool SafeString8::startsWith(sl_char8 ch) const
{
	String8 s(*this);
	return s.startsWith(ch);
}

sl_bool SafeString16::startsWith(sl_char16 ch) const
{
	String16 s(*this);
	return s.startsWith(ch);
}


sl_bool String8::startsWith(const String8& str) const
{
	sl_size count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	sl_size count1 = getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory(getData(), str.getData(), count2) == 0;
	}
}

sl_bool String16::startsWith(const String16& str) const
{
	sl_size count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	sl_size count1 = getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory2(getData(), str.getData(), count2) == 0;
	}
}

sl_bool SafeString8::startsWith(const String8& str) const
{
	String8 s(*this);
	return s.startsWith(str);
}

sl_bool SafeString16::startsWith(const String16& str) const
{
	String16 s(*this);
	return s.startsWith(str);
}


sl_bool String8::endsWith(sl_char8 ch) const
{
	sl_size count = getLength();
	if (count < 1) {
		return sl_false;
	} else {
		return m_container->sz[count - 1] == ch;
	}
}

sl_bool String16::endsWith(sl_char16 ch) const
{
	sl_size count = getLength();
	if (count < 1) {
		return sl_false;
	} else {
		return m_container->sz[count - 1] == ch;
	}
}

sl_bool SafeString8::endsWith(sl_char8 ch) const
{
	String8 s(*this);
	return s.endsWith(ch);
}

sl_bool SafeString16::endsWith(sl_char16 ch) const
{
	String16 s(*this);
	return s.endsWith(ch);
}


sl_bool String8::endsWith(const String8& str) const
{
	sl_size count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	sl_size count1 = getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory(getData() + count1 - count2, str.getData(), count2) == 0;
	}
}

sl_bool String16::endsWith(const String16& str) const
{
	sl_size count2 = str.getLength();
	if (count2 == 0) {
		return sl_true;
	}
	sl_size count1 = getLength();
	if (count1 < count2) {
		return sl_false;
	} else {
		return Base::compareMemory2(getData() + count1 - count2, str.getData(), count2) == 0;
	}
}

sl_bool SafeString8::endsWith(const String8& str) const
{
	String8 s(*this);
	return s.endsWith(str);
}

sl_bool SafeString16::endsWith(const String16& str) const
{
	String16 s(*this);
	return s.endsWith(str);
}


sl_bool String8::contains(sl_char8 ch) const
{
    return indexOf(ch) >= 0;
}

sl_bool String16::contains(sl_char16 ch) const
{
    return indexOf(ch) >= 0;
}

sl_bool SafeString8::constains(sl_char8 ch) const
{
	String8 s(*this);
	return s.contains(ch);
}

sl_bool SafeString16::contains(sl_char16 ch) const
{
	String16 s(*this);
	return s.contains(ch);
}


sl_bool String8::contains(const String8& str) const
{
    return indexOf(str) >= 0;
}

sl_bool String16::contains(const String16& str) const
{
    return indexOf(str) >= 0;
}

sl_bool SafeString8::contains(const String8& str) const
{
	String8 s(*this);
	return s.contains(str);
}

sl_bool SafeString16::contains(const String16& str) const
{
	String16 s(*this);
	return s.contains(str);
}



void String8::makeUpper()
{
	sl_char8* buf = getData();
	sl_size n = getLength();
	for (sl_size i = 0; i < n; i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] = buf[i] - ('a' - 'A');
		}
	}
}

void String16::makeUpper()
{
	sl_char16* buf = getData();
	sl_size n = getLength();
	for (sl_size i = 0; i < n; i++) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] = buf[i] - ('a' - 'A');
		}
	}
}

void SafeString8::makeUpper()
{
	String8 s(*this);
	s.makeUpper();
}

void SafeString16::makeUpper()
{
	String16 s(*this);
	s.makeUpper();
}


void String8::makeLower()
{
	sl_char8* buf = getData();
	sl_size n = getLength();
	for (sl_size i = 0; i < n; i++) {
		if (buf[i] >= 'A' && buf[i] <= 'Z') {
			buf[i] = buf[i] + ('a' - 'A');
		}
	}
}

void String16::makeLower()
{
	sl_char16* buf = getData();
	sl_size n = getLength();
	for (sl_size i = 0; i < n; i++) {
		if (buf[i] >= 'A' && buf[i] <= 'Z') {
			buf[i] = buf[i] + ('a' - 'A');
		}
	}
}

void SafeString8::makeLower()
{
	String8 s(*this);
	s.makeLower();
}

void SafeString16::makeLower()
{
	String16 s(*this);
	s.makeLower();
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

String8 SafeString8::toUpper() const
{
	String8 s(*this);
	return s.toUpper();
}

String16 SafeString16::toUpper() const
{
	String16 s(*this);
	return s.toUpper();
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

String8 SafeString8::toLower() const
{
	String8 s(*this);
	return s.toLower();
}

String16 SafeString16::toLower() const
{
	String16 s(*this);
	return s.toLower();
}


struct STRING_REPLACE_SUBSET
{
	sl_reg start;
	sl_reg len;
};

template <class ST, class CT, class TT>
SLIB_INLINE ST _String_replaceAll(const ST& str, const ST& pattern, const ST& replacement)
{
	sl_reg countPat = pattern.getLength();
	if (countPat == 0) {
		return ST::null();
	}
	sl_reg count = str.getLength();
	if (count == 0) {
		return ST::null();
	}
	Queue<STRING_REPLACE_SUBSET> queue;
	STRING_REPLACE_SUBSET subset;
	sl_reg countReplace = replacement.getLength();
	const CT* buf = str.getData();
	const CT* bufReplace = replacement.getData();
	sl_reg size = 0;
	sl_reg start = 0;
	while (start <= count + countPat - 1) {
		sl_reg index = str.indexOf(pattern, start);
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
		CT* out = ret.getData();
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

String8 SafeString8::replaceAll(const String8& pattern, const String8& replacement) const
{
	String8 s(*this);
	return s.replaceAll(pattern, replacement);
}

String16 SafeString16::replaceAll(const String16& pattern, const String16& replacement) const
{
	String16 s(*this);
	return s.replaceAll(pattern, replacement);
}


template <class ST, class CT>
SLIB_INLINE ST _String_trim(const ST& str)
{
	const CT* sz = str.getData();
	sl_size n = str.getLength();
	sl_size i = 0;
	for (; i < n; i++) {
		CT c = sz[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			break;
		}
	}
	if (i >= n) {
		return ST::null();
	}
	sl_size j = n - 1;
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

String8 SafeString8::trim() const
{
	String8 s(*this);
	return s.trim();
}

String16 SafeString16::trim() const
{
	String16 s(*this);
	return s.trim();
}


template <class ST, class CT>
SLIB_INLINE ST _String_trimLeft(const ST& str)
{
	const CT* sz = str.getData();
	sl_size n = str.getLength();
	sl_size i = 0;
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

String8 SafeString8::trimLeft() const
{
	String8 s(*this);
	return s.trimLeft();
}

String16 SafeString16::trimLeft() const
{
	String16 s(*this);
	return s.trimLeft();
}


template <class ST, class CT>
SLIB_INLINE ST _String_trimRight(const ST& str)
{
	const CT* sz = str.getData();
	sl_size n = str.getLength();
	sl_size j = n;
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

String8 SafeString8::trimRight() const
{
	String8 s(*this);
	return s.trimRight();
}

String16 SafeString16::trimRight() const
{
	String16 s(*this);
	return s.trimRight();
}


template <class ST>
List<ST> _String_split(const ST& str, const ST& pattern)
{
	CList<ST>* ret = CList<ST>::create();
	if (ret) {
		if (pattern.isEmpty()) {
			return ret;
		}
		sl_reg start = 0;
		while (1) {
			sl_reg index = str.indexOf(pattern, start);
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

List<String8> SafeString8::split(const String8& pattern) const
{
	String8 s(*this);
	return s.split(pattern);
}

List<String16> SafeString16::split(const String16& pattern) const
{
	String16 s(*this);
	return s.split(pattern);
}


template <class ST, class CT>
static sl_size _String_applyBackslashEscapes(const ST& s, sl_bool flagDoubleQuote, CT* buf)
{
	const CT* ch = s.getData();
	sl_size len = s.getLength();
	sl_size d = 1;
	if (buf) {
		buf[0] = flagDoubleQuote ? '"' : '\'';
	}
	for (sl_size i = 0; i < len; i++) {
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
	sl_size n = _String_applyBackslashEscapes<String8, sl_char8>(*this, flagDoubleQuote, sl_null);
	String8 ret = allocate(n);
	if (ret.isEmpty()) {
		return String8::null();
	}
	_String_applyBackslashEscapes<String8, sl_char8>(*this, flagDoubleQuote, ret.getData());
	return ret;
}

String16 String16::applyBackslashEscapes(sl_bool flagDoubleQuote)
{
	sl_size n = _String_applyBackslashEscapes<String16, sl_char16>(*this, flagDoubleQuote, sl_null);
	String16 ret = allocate(n);
	if (ret.isEmpty()) {
		return String16::null();
	}
	_String_applyBackslashEscapes<String16, sl_char16>(*this, flagDoubleQuote, ret.getData());
	return ret;
}

String8 SafeString8::applyBackslashEscapes(sl_bool flagDoubleQuote)
{
	String8 s(*this);
	return s.applyBackslashEscapes(flagDoubleQuote);
}

String16 SafeString16::applyBackslashEscapes(sl_bool flagDoubleQuote)
{
	String16 s(*this);
	return s.applyBackslashEscapes(flagDoubleQuote);
}


template <class ST, class CT>
SLIB_INLINE ST _String_parseBackslashEscapes(const CT* sz, sl_size n, sl_size* lengthParsed, sl_bool* outFlagError)
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
	sl_size len = 0;
	sl_bool flagSuccess = sl_false;
	sl_size i = 1;
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

String8 String8::parseBackslashEscapes(const sl_char8* sz, sl_size n, sl_size* lengthParsed, sl_bool* outFlagError)
{
	return _String_parseBackslashEscapes<String8, sl_char8>(sz, n, lengthParsed, outFlagError);
}

String16 String16::parseBackslashEscapes(const sl_char16* sz, sl_size n, sl_size* lengthParsed, sl_bool* outFlagError)
{
	return _String_parseBackslashEscapes<String16, sl_char16>(sz, n, lengthParsed, outFlagError);
}

String8 String8::parseBackslashEscapes(sl_size* lengthParsed, sl_bool* flagError) const
{
    return parseBackslashEscapes(getData(), getLength(), lengthParsed, flagError);
}

String16 String16::parseBackslashEscapes(sl_size* lengthParsed, sl_bool* flagError) const
{
	return parseBackslashEscapes(getData(), getLength(), lengthParsed, flagError);
}

String8 SafeString8::parseBackslashEscapes(sl_size* lengthParsed, sl_bool* flagError) const
{
	String8 s(*this);
	return s.parseBackslashEscapes(lengthParsed, flagError);
}

String16 SafeString16::parseBackslashEscapes(sl_size* lengthParsed, sl_bool* flagError) const
{
	String16 s(*this);
	return s.parseBackslashEscapes(lengthParsed, flagError);
}

template <class CT>
SLIB_INLINE sl_size _String_countLineNumber(const CT* input, sl_size len, sl_size* columnLast)
{
	sl_size line = 1;
	sl_size col = 1;
	for (sl_size i = 0; i < len; i++) {
		CT ch = input[i];
		if (ch == '\r') {
			line++;
			col = 0;
			if (i + 1 < len && input[i+1] == '\n') {
				i++;
			}
		} else if (ch == '\n') {
			line++;
			col = 0;
		}
		col++;
	}
	if (columnLast) {
		*columnLast = col;
	}
	return line;
}

sl_size String8::countLineNumber(const sl_char8* input, sl_size len, sl_size* columnLast)
{
	return _String_countLineNumber(input, len, columnLast);
}

sl_size String16::countLineNumber(const sl_char16* input, sl_size len, sl_size* columnLast)
{
	return _String_countLineNumber(input, len, columnLast);
}

sl_size String8::countLineNumber(sl_size pos, sl_size* column) const
{
	return countLineNumber(getData(), Math::min(getLength(), pos), column);
}

sl_size String16::countLineNumber(sl_size pos, sl_size* column) const
{
	return countLineNumber(getData(), Math::min(getLength(), pos), column);
}

sl_size SafeString8::countLineNumber(sl_size pos, sl_size* column) const
{
	String8 s(*this);
	return s.countLineNumber(pos, column);
}

sl_size SafeString16::countLineNumber(sl_size pos, sl_size* column) const
{
	String16 s(*this);
	return s.countLineNumber(pos, column);
}


sl_size String8::utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer)
{
	return _StringBase::utf8ToUtf16(utf8, lenUtf8, utf16, lenUtf16Buffer);
}

sl_size String16::utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer)
{
	return _StringBase::utf8ToUtf16(utf8, lenUtf8, utf16, lenUtf16Buffer);
}

sl_size String8::utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer)
{
	return _StringBase::utf8ToUtf32(utf8, lenUtf8, utf32, lenUtf32Buffer);
}

sl_size String16::utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer)
{
	return _StringBase::utf8ToUtf32(utf8, lenUtf8, utf32, lenUtf32Buffer);
}

sl_size String8::utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer)
{
	return _StringBase::utf16ToUtf8(utf16, lenUtf16, utf8, lenUtf8Buffer);
}

sl_size String16::utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer)
{
	return _StringBase::utf16ToUtf8(utf16, lenUtf16, utf8, lenUtf8Buffer);
}

sl_size String8::utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer)
{
	return _StringBase::utf32ToUtf8(utf32, lenUtf32, utf8, lenUtf8Buffer);
}

sl_size String16::utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer)
{
	return _StringBase::utf32ToUtf8(utf32, lenUtf32, utf8, lenUtf8Buffer);
}


template <>
int Compare<String8>::compare(const String8& a, const String8& b)
{
	return a.compare(b);
}

template <>
int Compare<String16>::compare(const String16& a, const String16& b)
{
	return a.compare(b);
}

template <>
int Compare<SafeString8>::compare(const SafeString8& a, const SafeString8& b)
{
	return a.compare(b);
}

template <>
int Compare<SafeString16>::compare(const SafeString16& a, const SafeString16& b)
{
	return a.compare(b);
}


template <>
sl_bool Compare<String8>::equals(const String8& a, const String8& b)
{
	return a.equals(b);
}

template <>
sl_bool Compare<String16>::equals(const String16& a, const String16& b)
{
	return a.equals(b);
}

template <>
sl_bool Compare<SafeString8>::equals(const SafeString8& a, const SafeString8& b)
{
	return a.equals(b);
}

template <>
sl_bool Compare<SafeString16>::equals(const SafeString16& a, const SafeString16& b)
{
	return a.equals(b);
}


template <>
sl_uint32 Hash<String8>::hash(const String8& v)
{
	return v.getHashCode();
}
template <>
sl_uint32 Hash<String16>::hash(const String16& v)
{
	return v.getHashCode();
}

template <>
sl_uint32 Hash<SafeString8>::hash(const SafeString8& v)
{
	return v.getHashCode();
}

template <>
sl_uint32 Hash<SafeString16>::hash(const SafeString16& v)
{
	return v.getHashCode();
}


/**********************************************************
					String Buffer
**********************************************************/

StringBuffer8::StringBuffer8()
{
	m_len = 0;
}

StringBuffer16::StringBuffer16()
{
	m_len = 0;
}

sl_bool StringBuffer8::add(const String8& str)
{
	sl_size len = str.getLength();
	if (len == 0) {
		return sl_true;
	}
	ObjectLocker lock(this);
	StringData data;
	data.sz8 = str.getData();
	data.len = len;
	data.str8 = str;
	if (m_queue.push(data)) {
		m_len += len;
		return sl_true;
	}
	return sl_false;
}

sl_bool StringBuffer16::add(const String16& str)
{
	sl_size len = str.getLength();
	if (len == 0) {
		return sl_true;
	}
	ObjectLocker lock(this);
	StringData data;
	data.sz16 = str.getData();
	data.len = len;
	data.str16 = str;
	if (m_queue.push(data)) {
		m_len += len;
		return sl_true;
	}
	return sl_false;
}

sl_bool StringBuffer8::add(const StringData& data)
{
	sl_size len = data.len;
	if (len == 0) {
		return sl_true;
	}
	if (data.sz8) {
		ObjectLocker lock(this);
		if (m_queue.push(data)) {
			m_len += len;
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool StringBuffer16::add(const StringData& data)
{
	sl_size len = data.len;
	if (len == 0) {
		return sl_true;
	}
	if (data.sz16) {
		ObjectLocker lock(this);
		if (m_queue.push(data)) {
			m_len += len;
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool StringBuffer8::addStatic(const sl_char8* buf, sl_size length)
{
	StringData data;
	data.sz8 = buf;
	data.len = length;
	return add(data);
}

sl_bool StringBuffer16::addStatic(const sl_char16* buf, sl_size length)
{
	StringData data;
	data.sz16 = buf;
	data.len = length;
	return add(data);
}

sl_size StringBuffer8::getLength() const
{
	return m_len;
}

sl_size StringBuffer16::getLength() const
{
	return m_len;
}

String8 StringBuffer8::merge() const
{
	ObjectLocker lock(this);
	if (m_queue.getCount() == 0) {
		return String8::getEmpty();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_size total = m_len;
	String8 ret = String8::allocate(total);
	if (ret.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(ret.getData());
		sl_size offset = 0;
		Link<StringData>* item = begin;
		while (item) {
			StringData& s = item->value;
			sl_size t = s.len;
			if (offset + t > total) {
				Base::copyMemory(buf + offset, s.sz8, total - offset);
				return ret;
			}
			Base::copyMemory(buf + offset, s.sz8, t);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

String16 StringBuffer16::merge() const
{
	ObjectLocker lock(this);
	if (m_queue.getCount() == 0) {
		return String16::getEmpty();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_size total = m_len;
	String16 ret = String16::allocate(total);
	if (ret.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(ret.getData());
		sl_size offset = 0;
		Link<StringData>* item = begin;
		while (item) {
			StringData& s = item->value;
			sl_size t = s.len;
			if (offset + t > total) {
				Base::copyMemory(buf + offset, s.sz16, (total - offset) * 2);
				return ret;
			}
			Base::copyMemory(buf + offset, s.sz16, t * 2);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

Memory StringBuffer8::mergeToMemory() const
{
	ObjectLocker lock(this);
	if (m_queue.getCount() == 0) {
		return Memory::null();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_size total = m_len;
	Memory ret = Memory::create(total);
	if (ret.isNotEmpty()) {
		sl_char8* buf = (sl_char8*)(ret.getData());
		sl_size offset = 0;
		Link<StringData>* item = begin;
		while (item) {
			StringData& s = item->value;
			sl_size t = s.len;
			Base::copyMemory(buf + offset, s.sz8, t);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

Memory StringBuffer16::mergeToMemory() const
{
	ObjectLocker lock(this);
	if (m_queue.getCount() == 0) {
		return Memory::null();
	}
	Link<StringData>* begin = m_queue.getBegin();
	sl_size total = m_len;
	Memory ret = Memory::create(total * 2);
	if (ret.isNotEmpty()) {
		sl_char16* buf = (sl_char16*)(ret.getData());
		sl_size offset = 0;
		Link<StringData>* item = begin;
		while (item) {
			StringData& s = item->value;
			sl_size t = s.len;
			Base::copyMemory(buf + offset, s.sz16, t * 2);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

/**********************************************************
					String Base
**********************************************************/

sl_size _StringBase::getSz8Length(const sl_char8* utf8, sl_reg count)
{
	if (count == 0) {
		return 0;
	}
	if (count < 0) {
		for (sl_reg i = 0; ; i++) {
			if (utf8[i] == 0) {
				return i;
			}
		}
	} else {
		for (sl_reg i = 0; i < count; i++) {
			if (utf8[i] == 0) {
				return i;
			}
		}
	}
	return count;
}

sl_size _StringBase::getSz16Length(const sl_char16* utf16, sl_reg count)
{
	if (count == 0) {
		return 0;
	}
	if (count < 0) {
		for (sl_reg i = 0; ; i++) {
			if (utf16[i] == 0) {
				return i;
			}
		}
	} else {
		for (sl_reg i = 0; i < count; i++) {
			if (utf16[i] == 0) {
				return i;
			}
		}
	}
	return count;
}

sl_size _StringBase::getSz32Length(const sl_char32* utf32, sl_reg count)
{
	if (count == 0) {
		return 0;
	}
	if (count < 0) {
		for (sl_reg i = 0; ; i++) {
			if (utf32[i] == 0) {
				return i;
			}
		}
	} else {
		for (sl_reg i = 0; i < count; i++) {
			if (utf32[i] == 0) {
				return i;
			}
		}
	}
	return count;
}

sl_size _StringBase::utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer)
{
	if (lenUtf8 < 0) {
		lenUtf8 = _StringBase::getSz8Length(utf8, -1) + 1;
	}
	sl_reg n = 0;
	for (sl_reg i = 0; i < lenUtf8 && (lenUtf16Buffer < 0 || n < lenUtf16Buffer); i++) {
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

sl_size _StringBase::utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer)
{
	if (lenUtf8 < 0) {
		lenUtf8 = _StringBase::getSz8Length(utf8, -1) + 1;
	}
	sl_reg n = 0;
	for (sl_reg i = 0; i < lenUtf8 && (lenUtf32Buffer < 0 || n < lenUtf32Buffer); i++) {
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

sl_size _StringBase::utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer)
{
	if (lenUtf16 < 0) {
		lenUtf16 = _StringBase::getSz16Length(utf16, -1) + 1;
	}
	sl_reg n = 0;
	for (sl_reg i = 0; i < lenUtf16 && (lenUtf8Buffer < 0 || n < lenUtf8Buffer); i++) {
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

sl_size _StringBase::utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer)
{
	if (lenUtf32 < 0) {
		lenUtf32 = _StringBase::getSz32Length(utf32, -1) + 1;
	}
	sl_reg n = 0;
	for (sl_reg i = 0; i < lenUtf32 && (lenUtf8Buffer < 0 || n < lenUtf8Buffer); i++) {
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


/**********************************************************
					String Conversion
**********************************************************/

#define MAX_NUMBER_STR_LEN 256
#define MAX_PRECISION 50

const char _string_conv_radix_pattern_lower[65] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_";
const char* _StringConv_radixPatternLower = _string_conv_radix_pattern_lower;
const char _string_conv_radix_pattern_upper[65] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@_";
const char* _StringConv_radixPatternUpper = _string_conv_radix_pattern_upper;

const sl_uint8 _string_conv_radix_inverse_pattern_big[128] = {
	/*00*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*08*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*10*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*18*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*20*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*28*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*30*/ 0,   1,   2,   3,   4,   5,   6,   7,
	/*38*/ 8,   9,   255, 255, 255, 255, 255, 255,
	/*40*/ 62,  36,  37,  38,  39,  40,  41,  42,
	/*48*/ 43,  44,  45,  46,  47,  48,  49,  50,
	/*50*/ 51,  52,  53,  54,  55,  56,  57,  58,
	/*58*/ 59,  60,  61,  255, 255, 255, 255, 63,
	/*60*/ 255, 10,  11,  12,  13,  14,  15,  16,
	/*68*/ 17,  18,  19,  20,  21,  22,  23,  24,
	/*70*/ 25,  26,  27,  28,  29,  30,  31,  32,
	/*78*/ 33,  34,  35,  255, 255, 255, 255, 255
};
const sl_uint8* _StringConv_radixInversePatternBig = _string_conv_radix_inverse_pattern_big;

const sl_uint8 _string_conv_radix_inverse_pattern_small[128] = {
	/*00*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*08*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*10*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*18*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*20*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*28*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*30*/ 0,   1,   2,   3,   4,   5,   6,   7,
	/*38*/ 8,   9,   255, 255, 255, 255, 255, 255,
	/*40*/ 255, 10,  11,  12,  13,  14,  15,  16,
	/*48*/ 17,  18,  19,  20,  21,  22,  23,  24,
	/*50*/ 25,  26,  27,  28,  29,  30,  31,  32,
	/*58*/ 33,  34,  35,  255, 255, 255, 255, 255,
	/*60*/ 255, 10,  11,  12,  13,  14,  15,  16,
	/*68*/ 17,  18,  19,  20,  21,  22,  23,  24,
	/*70*/ 25,  26,  27,  28,  29,  30,  31,  32,
	/*78*/ 33,  34,  35,  255, 255, 255, 255, 255
};
const sl_uint8* _StringConv_radixInversePatternSmall = _string_conv_radix_inverse_pattern_small;

template <class IT, class ST, class CT>
SLIB_INLINE ST _String_fromInt(IT value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	if (radix < 2 || radix > 64) {
		return ST::null();
	}
	const char* pattern = flagUpperCase && radix <= 36 ? _string_conv_radix_pattern_upper : _string_conv_radix_pattern_lower;
	CT buf[MAX_NUMBER_STR_LEN];
	sl_int32 pos = MAX_NUMBER_STR_LEN - 1;
	buf[pos] = 0;
	if (minWidth < 1) {
		minWidth = 1;
	}
	
	sl_bool flagMinus = sl_false;
	if (value < 0) {
		value = -value;
		flagMinus = sl_true;
	}
	while (value || minWidth > 0) {
		pos--;
		buf[pos] = pattern[value % radix];
		value = value / radix;
		minWidth--;
	}
	if (flagMinus) {
		pos--;
		buf[pos] = '-';
	}
	
	ST ret(buf + pos);
	return ret;
}

template <class IT, class ST, class CT>
SLIB_INLINE ST _String_fromUint(IT value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	if (radix < 2 || radix > 64) {
		return ST::null();
	}
	const char* pattern = flagUpperCase && radix <= 36 ? _string_conv_radix_pattern_upper : _string_conv_radix_pattern_lower;
	CT buf[MAX_NUMBER_STR_LEN];
	sl_int32 pos = MAX_NUMBER_STR_LEN - 1;
	buf[pos] = 0;
	if (minWidth < 1) {
		minWidth = 1;
	}
	while (value || minWidth > 0) {
		pos--;
		buf[pos] = pattern[value % radix];
		value = value / radix;
		minWidth--;
	}
	ST ret(buf + pos);
	return ret;
}

String8 String8::fromInt32(sl_int32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int32, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromInt32(sl_int32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromUint32(sl_uint32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint32, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromUint32(sl_uint32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromInt64(sl_int64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int64, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromInt64(sl_int64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromUint64(sl_uint64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint64, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromUint64(sl_uint64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromInt(sl_reg value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
#ifdef SLIB_ARCH_IS_64BIT
	return fromInt64(value, radix, minWidth, flagUpperCase);
#else
	return fromInt32(value, radix, minWidth, flagUpperCase);
#endif
}

String16 String16::fromInt(sl_reg value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
#ifdef SLIB_ARCH_IS_64BIT
	return fromInt64(value, radix, minWidth, flagUpperCase);
#else
	return fromInt32(value, radix, minWidth, flagUpperCase);
#endif
}

String8 String8::fromSize(sl_size value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
#ifdef SLIB_ARCH_IS_64BIT
	return fromUint64(value, radix, minWidth, flagUpperCase);
#else
	return fromUint32(value, radix, minWidth, flagUpperCase);
#endif
}

String16 String16::fromSize(sl_size value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
#ifdef SLIB_ARCH_IS_64BIT
	return fromUint64(value, radix, minWidth, flagUpperCase);
#else
	return fromUint32(value, radix, minWidth, flagUpperCase);
#endif
}

template <class FT, class ST, class CT>
SLIB_INLINE ST _String_fromFloat(FT value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral)
{
	
	CT buf[MAX_NUMBER_STR_LEN];
	
	if (Math::isNaN(value)) {
		SLIB_SAFE_STATIC(ST, s, "NaN");
		return s;
	}
	if (Math::isInfinite(value)) {
		SLIB_SAFE_STATIC(ST, s, "Infinite");
		return s;
	}
	if (value == 0) {
		if (precision == 0) {
			if (minWidthIntegral > MAX_PRECISION - 1) {
				minWidthIntegral = MAX_PRECISION - 1;
			}
			for (sl_int32 i = 0; i < minWidthIntegral; i++) {
				buf[i] = '0';
			}
			buf[minWidthIntegral] = 0;
			return ST(buf);
		} else {
			if (minWidthIntegral > MAX_PRECISION - 1) {
				minWidthIntegral = MAX_PRECISION - 1;
			}
			sl_int32 i = 0;
			for (i = 0; i < minWidthIntegral; i++) {
				buf[i] = '0';
			}
			buf[i++] = '.';
			if (precision > 0 && flagZeroPadding) {
				if (precision > MAX_PRECISION) {
					precision = MAX_PRECISION;
				}
				for (sl_int32 k = 0; k < precision; k++) {
					buf[i++] = '0';
				}
			} else {
				buf[i++] = '0';
			}
			buf[i] = 0;
			return ST(buf);
		}
	}
	
	CT* str = buf;
	CT* str_last = buf + MAX_NUMBER_STR_LEN - 10;
	
	sl_int32 flagMinus;
	if (value < 0) {
		flagMinus = 1;
		value = -value;
	} else {
		flagMinus = 0;
	}
	
	FT min_value;
	if (precision < 0) {
		min_value = 0;
	} else {
		min_value = Math::pow((FT)10, (FT)(-precision));
	}
	value += min_value / 2;
	if (flagZeroPadding) {
		min_value = 0;
	}
	
	if (flagMinus) {
		*(str++) = '-';
	}
	sl_int32 nExp = 0;
	
	sl_int32 nInt = (sl_int32)(Math::log10(value));
	FT weight = Math::pow((FT)10, (FT)(nInt));
	if (nInt >= 15) { // use exp
		value = value / weight;
		nExp = nInt;
		nInt = 0;
		weight = 1;
	}
	if (nInt < -15) { // use exp
		value = value / weight * 10;
		nExp = nInt - 1;
		nInt = 0;
		weight = 1;
	}
	
	if (precision < 0) {
		precision = 15 - nInt;
		if (precision < 0) {
			precision = 0;
		}
		if (precision > 50) {
			precision = 50;
		}
		min_value = Math::pow((FT)10, (FT)(-precision));
		value += min_value / 3;
	}
	if (precision == 0) {
		flagZeroPadding = 0;
	}
	if (nInt < minWidthIntegral - 1) {
		nInt = minWidthIntegral - 1;
		weight = Math::pow((FT)10, (FT)nInt);
	}
	while (str < str_last && nInt >= -precision && (nInt >= 0 || value >= min_value)) {
		if (nInt == -1) {
			if (value >= min_value) {
				*(str++) = '.';
			} else {
				break;
			}
		}
		if (weight > 0 && !Math::isInfinite(weight)) {
			sl_int32 digit = (sl_int32)(value / weight);
			value -= (digit * weight);
			*(str++) = (CT)('0' + digit);
		}
		nInt--;
		weight /= 10;
	}
	
	if (nExp) {
		*(str++) = 'e';
		if (nExp > 0) {
			*(str++) = '+';
		} else {
			*(str++) = '-';
			nExp = -nExp;
		}
		CT* t1 = str;
		while (nExp > 0 && str < str_last) {
			*(str++) = (CT)('0' + (nExp % 10));
			nExp /= 10;
		}
		CT* t2 = str - 1;
		while (t1 < t2) {
			CT chTemp = *t1;
			*t1 = *t2;
			*t2 = chTemp;
			t1++;
			t2--;
		}
	}
	*str = 0;
	
	return buf;
}

String8 String8::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral) {
	return _String_fromFloat<double, String8, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
}

String8 String8::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral)
{
	return _String_fromFloat<float, String8, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
}

String16 String16::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral) {
	return _String_fromFloat<double, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
}

String16 String16::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral)
{
	return _String_fromFloat<float, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
}

String8 String8::fromPointerValue(const void* pointer)
{
#ifdef SLIB_ARCH_IS_64BIT
	return fromUint64((sl_uint64)(pointer), 16, 16, sl_true);
#else
	return fromUint32((sl_uint32)(pointer), 16, 8, sl_true);
#endif
}

String16 String16::fromPointerValue(const void* pointer)
{
#ifdef SLIB_ARCH_IS_64BIT
	return fromUint64((sl_uint64)(pointer), 16, 16, sl_true);
#else
	return fromUint32((sl_uint32)(pointer), 16, 8, sl_true);
#endif
}

String8 String8::fromBoolean(sl_bool value)
{
	if (value) {
		SLIB_STATIC_STRING8(s, "true");
		return s;
	} else {
		SLIB_STATIC_STRING8(s, "false");
		return s;
	}
}

String16 String16::fromBoolean(sl_bool value)
{
	if (value) {
		SLIB_STATIC_STRING16_BY_ARRAY(s, 't', 'r', 'u', 'e');
		return s;
	} else {
		SLIB_STATIC_STRING16_BY_ARRAY(s, 'f', 'a', 'l', 's', 'e');
		return s;
	}
}

template <class ST, class CT>
SLIB_INLINE ST _String_makeHexString(const void* buf, sl_size size)
{
	if (!buf || size <= 0) {
		return ST::null();
	}
	ST str = ST::allocate(size * 2);
	if (str.isEmpty()) {
		return str;
	}
	CT* sz = (CT*)(str.getData());
	for (sl_size i = 0; i < size; i++) {
		sl_uint8 v = ((sl_uint8*)(buf))[i];
		sz[i << 1] = _string_conv_radix_pattern_lower[v >> 4];
		sz[(i << 1) + 1] = _string_conv_radix_pattern_lower[v & 15];
	}
	return str;
}

String8 String8::makeHexString(const void* buf, sl_size size)
{
	return _String_makeHexString<String8, sl_char8>(buf, size);
}

String16 String16::makeHexString(const void* buf, sl_size size)
{
	return _String_makeHexString<String16, sl_char16>(buf, size);
}

String8 String8::makeHexString(const Memory& mem)
{
	return makeHexString(mem.getData(), mem.getSize());
}

String16 String16::makeHexString(const Memory& mem)
{
	return makeHexString(mem.getData(), mem.getSize());
}


template <class IT, class CT>
SLIB_INLINE sl_reg _String_parseInt(sl_int32 radix, const CT* sz, sl_size i, sl_size n, IT* _out)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	sl_int32 r = radix;
	
	const sl_uint8* pattern = r <= 36 ? _string_conv_radix_inverse_pattern_small : _string_conv_radix_inverse_pattern_big;
	IT v = 0;
	sl_bool bMinus = sl_false;
	sl_bool bEmpty = sl_true;
	if (sz[i] == '-') {
		i++;
		bMinus = sl_true;
	}
	for (; i < n; i++) {
		sl_uint32 c = (sl_uint32)(sz[i]);
		if (c != '\t' && c != ' ') {
			break;
		}
	}
	for (; i < n; i++) {
		sl_uint32 c = (sl_uint32)(sz[i]);
		sl_uint32 m = c < 128 ? pattern[c] : 255;
		if (m < r) {
			v = v * r + m;
			bEmpty = sl_false;
		} else {
			break;
		}
	}
	if (bEmpty) {
		return SLIB_PARSE_ERROR;
	}
	if (bMinus) {
		v = -v;
	}
	if (_out) {
		*_out = v;
	}
	return i;
}

template <class IT, class CT>
SLIB_INLINE sl_reg _String_parseUint(sl_int32 radix, const CT* sz, sl_size i, sl_size n, IT* _out)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	sl_uint32 r = radix;
	sl_bool bEmpty = sl_true;
	const sl_uint8* pattern = r <= 36 ? _string_conv_radix_inverse_pattern_small : _string_conv_radix_inverse_pattern_big;
	IT v = 0;
	for (; i < n; i++) {
		sl_uint32 c = (sl_uint32)(sz[i]);
		sl_uint32 m = c < 128 ? pattern[c] : 255;
		if (m < r) {
			v = v * r + m;
			bEmpty = sl_false;
		} else {
			break;
		}
	}
	if (bEmpty) {
		return SLIB_PARSE_ERROR;
	}
	if (_out) {
		*_out = v;
	}
	return i;
}

sl_reg String8::parseInt32(sl_int32 radix, sl_int32* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseInt32(sl_int32 radix, sl_int32* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_reg String8::parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_bool String8::parseInt32(sl_int32 radix, sl_int32* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(radix, getData(), 0, n, _out) == n;
}

sl_bool String16::parseInt32(sl_int32 radix, sl_int32* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(radix, getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseInt32(sl_int32 radix, sl_int32* _out) const
{
	String8 s(*this);
	return s.parseInt32(radix, _out);
}

sl_bool SafeString16::parseInt32(sl_int32 radix, sl_int32* _out) const
{
	String16 s(*this);
	return s.parseInt32(radix, _out);
}

sl_int32 String8::parseInt32(sl_int32 radix, sl_int32 def) const
{
	sl_int32 _out = def;
	parseInt32(radix, &_out);
	return _out;
}

sl_int32 String16::parseInt32(sl_int32 radix, sl_int32 def) const
{
	sl_int32 _out = def;
	parseInt32(radix, &_out);
	return _out;
}

sl_int32 SafeString8::parseInt32(sl_int32 radix, sl_int32 def) const
{
	String8 s(*this);
	return s.parseInt32(radix, def);
}

sl_int32 SafeString16::parseInt32(sl_int32 radix, sl_int32 def) const
{
	String16 s(*this);
	return s.parseInt32(radix, def);
}


sl_reg String8::parseUint32(sl_int32 radix, sl_uint32* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseUint32(sl_int32 radix, sl_uint32* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_reg String8::parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_bool String8::parseUint32(sl_int32 radix, sl_uint32* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(radix, getData(), 0, n, _out) == n;
}

sl_bool String16::parseUint32(sl_int32 radix, sl_uint32* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(radix, getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseUint32(sl_int32 radix, sl_uint32* _out) const
{
	String8 s(*this);
	return s.parseUint32(radix, _out);
}

sl_bool SafeString16::parseUint32(sl_int32 radix, sl_uint32* _out) const
{
	String16 s(*this);
	return s.parseUint32(radix, _out);
}

sl_uint32 String8::parseUint32(sl_int32 radix, sl_uint32 def) const
{
	sl_uint32 _out = def;
	parseUint32(radix, &_out);
	return _out;
}

sl_uint32 String16::parseUint32(sl_int32 radix, sl_uint32 def) const
{
	sl_uint32 _out = def;
	parseUint32(radix, &_out);
	return _out;
}

sl_uint32 SafeString8::parseUint32(sl_int32 radix, sl_uint32 def) const
{
	String8 s(*this);
	return s.parseUint32(radix, def);
}

sl_uint32 SafeString16::parseUint32(sl_int32 radix, sl_uint32 def) const
{
	String16 s(*this);
	return s.parseUint32(radix, def);
}


sl_reg String8::parseInt64(sl_int32 radix, sl_int64* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseInt64(sl_int32 radix, sl_int64* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_reg String8::parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseInt(radix, sz, posBegin, len, _out);
}

sl_bool String8::parseInt64(sl_int32 radix, sl_int64* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(radix, getData(), 0, n, _out) == n;
}

sl_bool String16::parseInt64(sl_int32 radix, sl_int64* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(radix, getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseInt64(sl_int32 radix, sl_int64* _out) const
{
	String8 s(*this);
	return s.parseInt64(radix, _out);
}

sl_bool SafeString16::parseInt64(sl_int32 radix, sl_int64* _out) const
{
	String16 s(*this);
	return s.parseInt64(radix, _out);
}

sl_int64 String8::parseInt64(sl_int32 radix, sl_int64 def) const
{
	sl_int64 _out = def;
	parseInt64(radix, &_out);
	return _out;
}

sl_int64 String16::parseInt64(sl_int32 radix, sl_int64 def) const
{
	sl_int64 _out = def;
	parseInt64(radix, &_out);
	return _out;
}

sl_int64 SafeString8::parseInt64(sl_int32 radix, sl_int64 def) const
{
	String8 s(*this);
	return s.parseInt64(radix, def);
}

sl_int64 SafeString16::parseInt64(sl_int32 radix, sl_int64 def) const
{
	String16 s(*this);
	return s.parseInt64(radix, def);
}


sl_reg String8::parseUint64(sl_int32 radix, sl_uint64* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseUint64(sl_int32 radix, sl_uint64* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_reg String8::parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_reg String16::parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseUint(radix, sz, posBegin, len, _out);
}

sl_bool String8::parseUint64(sl_int32 radix, sl_uint64* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(radix, getData(), 0, n, _out) == n;
}

sl_bool String16::parseUint64(sl_int32 radix, sl_uint64* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(radix, getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseUint64(sl_int32 radix, sl_uint64* _out) const
{
	String8 s(*this);
	return s.parseUint64(radix, _out);
}

sl_bool SafeString16::parseUint64(sl_int32 radix, sl_uint64* _out) const
{
	String16 s(*this);
	return s.parseUint64(radix, _out);
}

sl_uint64 String8::parseUint64(sl_int32 radix, sl_uint64 def) const
{
	sl_uint64 _out = def;
	parseUint64(radix, &_out);
	return _out;
}

sl_uint64 String16::parseUint64(sl_int32 radix, sl_uint64 def) const
{
	sl_uint64 _out = def;
	parseUint64(radix, &_out);
	return _out;
}

sl_uint64 SafeString8::parseUint64(sl_int32 radix, sl_uint64 def) const
{
	String8 s(*this);
	return s.parseUint64(radix, def);
}

sl_uint64 SafeString16::parseUint64(sl_int32 radix, sl_uint64 def) const
{
	String16 s(*this);
	return s.parseUint64(radix, def);
}


sl_bool String8::parseInt(sl_int32 radix, sl_reg* _out) const
{
#ifdef SLIB_ARCH_IS_64BIT
	return parseInt64(radix, _out);
#else
	return parseInt32(radix, _out);
#endif
}

sl_bool String16::parseInt(sl_int32 radix, sl_reg* _out) const
{
#ifdef SLIB_ARCH_IS_64BIT
	return parseInt64(radix, _out);
#else
	return parseInt32(radix, _out);
#endif
}

sl_bool SafeString8::parseInt(sl_int32 radix, sl_reg* _out) const
{
	String8 s(*this);
	return s.parseInt(radix, _out);
}

sl_bool SafeString16::parseInt(sl_int32 radix, sl_reg* _out) const
{
	String16 s(*this);
	return s.parseInt(radix, _out);
}

sl_reg String8::parseInt(sl_int32 radix, sl_reg def) const
{
	sl_reg _out = def;
	parseInt(radix, &_out);
	return _out;
}

sl_reg String16::parseInt(sl_int32 radix, sl_reg def) const
{
	sl_reg _out = def;
	parseInt(radix, &_out);
	return _out;
}

sl_reg SafeString8::parseInt(sl_int32 radix, sl_reg def) const
{
	String8 s(*this);
	return s.parseInt(radix, def);
}

sl_reg SafeString16::parseInt(sl_int32 radix, sl_reg def) const
{
	String16 s(*this);
	return s.parseInt(radix, def);
}


sl_bool String8::parseSize(sl_int32 radix, sl_size* _out) const
{
#ifdef SLIB_ARCH_IS_64BIT
	return parseUint64(radix, _out);
#else
	return parseUint32(radix, _out);
#endif
}

sl_bool String16::parseSize(sl_int32 radix, sl_size* _out) const
{
#ifdef SLIB_ARCH_IS_64BIT
	return parseUint64(radix, _out);
#else
	return parseUint32(radix, _out);
#endif
}

sl_bool SafeString8::parseSize(sl_int32 radix, sl_size* _out) const
{
	String8 s(*this);
	return s.parseSize(radix, _out);
}

sl_bool SafeString16::parseSize(sl_int32 radix, sl_size* _out) const
{
	String16 s(*this);
	return s.parseSize(radix, _out);
}

sl_size String8::parseSize(sl_int32 radix, sl_size def) const
{
	sl_size _out = def;
	parseSize(radix, &_out);
	return _out;
}

sl_size String16::parseSize(sl_int32 radix, sl_size def) const
{
	sl_size _out = def;
	parseSize(radix, &_out);
	return _out;
}

sl_size SafeString8::parseSize(sl_int32 radix, sl_size def) const
{
	String8 s(*this);
	return s.parseSize(radix, def);
}

sl_size SafeString16::parseSize(sl_int32 radix, sl_size def) const
{
	String16 s(*this);
	return s.parseSize(radix, def);
}


template <class FT, class CT>
SLIB_INLINE sl_reg _String_parseFloat(const CT* sz, sl_size i, sl_size n, FT* _out)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR; // input string is empty
	}
	
	FT v = 0;
	sl_bool bMinus = sl_false;
	sl_bool bEmpty = sl_true;
	
	if (sz[i] == '-') {
		i++;
		bMinus = sl_true;
	}
	for (; i < n; i++) {
		sl_uint32 c = (sl_uint32)(sz[i]);
		if (c != '\t' && c != ' ') {
			break;
		}
	}
	for (; i < n; i++) {
		sl_uint32 c = (sl_uint32)sz[i];
		if (c >= '0' && c <= '9') {
			v = v * 10 + (c - '0');
			bEmpty = sl_false;
		} else {
			break;
		}
	}
	if (bEmpty) {
		return SLIB_PARSE_ERROR; // integral number is required
	}
	if (i < n) {
		if (sz[i] == '.') {
			i++;
			bEmpty = sl_true;
			FT weight = (FT)(0.1);
			for (; i < n; i++) {
				sl_uint32 c = (sl_uint32)sz[i];
				if (c >= '0' && c <= '9') {
					v = v + (c - '0') * weight;
					weight /= 10;
					bEmpty = sl_false;
				} else {
					break;
				}
			}
			if (bEmpty) {
				return SLIB_PARSE_ERROR; // fraction number is required
			}
		}
		if (i < n) {
			if (sz[i] == 'e' || sz[i] == 'E') {
				i++;
				bEmpty = sl_true;
				sl_bool bMinuxExp = sl_false;
				FT exp = 0;
				if (i < n && (sz[i] == '+' || sz[i] == '-')) {
					if (sz[i] == '-') {
						bMinuxExp = sl_true;
					}
					i++;
				}
				for (; i < n; i++) {
					sl_uint32 c = (sl_uint32)sz[i];
					if (c >= '0' && c <= '9') {
						exp = exp * 10 + (c - '0');
						bEmpty = sl_false;
					} else {
						break; // invalid character
					}
				}
				if (bEmpty) {
					return SLIB_PARSE_ERROR; // exponent number is required
				}
				if (bMinuxExp) {
					exp = -exp;
				}
				v *= Math::pow((FT)(10.0), exp);
			}
		}
	}
	if (bMinus) {
		v = -v;
	}
	if (_out) {
		*_out = v;
	}
	return i;
}

sl_reg String8::parseFloat(float* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseFloat(sz, posBegin, len, _out);
}

sl_reg String16::parseFloat(float* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseFloat(sz, posBegin, len, _out);
}

sl_reg String8::parseFloat(float* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseFloat(sz, posBegin, len, _out);
}

sl_reg String16::parseFloat(float* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseFloat(sz, posBegin, len, _out);
}

sl_bool String8::parseFloat(float* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(getData(), 0, n, _out) == n;
}

sl_bool String16::parseFloat(float* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseFloat(float* _out) const
{
	String8 s(*this);
	return s.parseFloat(_out);
}

sl_bool SafeString16::parseFloat(float* _out) const
{
	String16 s(*this);
	return s.parseFloat(_out);
}

float String8::parseFloat(float def) const
{
	float _out = def;
	parseFloat(&_out);
	return _out;
}

float String16::parseFloat(float def) const
{
	float _out = def;
	parseFloat(&_out);
	return _out;
}

float SafeString8::parseFloat(float def) const
{
	String8 s(*this);
	return s.parseFloat(def);
}

float SafeString16::parseFloat(float def) const
{
	String16 s(*this);
	return s.parseFloat(def);
}


sl_reg String8::parseDouble(double* _out, const char* sz, sl_size posBegin, sl_size n)
{
	return _String_parseFloat(sz, posBegin, n, _out);
}

sl_reg String16::parseDouble(double* _out, const char* sz, sl_size posBegin, sl_size n)
{
	return _String_parseFloat(sz, posBegin, n, _out);
}

sl_reg String8::parseDouble(double* _out, const sl_char16* sz, sl_size posBegin, sl_size n)
{
	return _String_parseFloat(sz, posBegin, n, _out);
}

sl_reg String16::parseDouble(double* _out, const sl_char16* sz, sl_size posBegin, sl_size n)
{
	return _String_parseFloat(sz, posBegin, n, _out);
}

sl_bool String8::parseDouble(double* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(getData(), 0, n, _out) == n;
}

sl_bool String16::parseDouble(double* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseDouble(double* _out) const
{
	String8 s(*this);
	return s.parseDouble(_out);
}

sl_bool SafeString16::parseDouble(double* _out) const
{
	String16 s(*this);
	return s.parseDouble(_out);
}

double String8::parseDouble(double def) const
{
	double _out = def;
	parseDouble(&_out);
	return _out;
}

double String16::parseDouble(double def) const
{
	double _out = def;
	parseDouble(&_out);
	return _out;
}

double SafeString8::parseDouble(double def) const
{
	String8 s(*this);
	return s.parseDouble(def);
}

double SafeString16::parseDouble(double def) const
{
	String16 s(*this);
	return s.parseDouble(def);
}


template <class CT>
SLIB_INLINE sl_reg _String_parseHexString(const CT* sz, sl_size i, sl_size n, void* _out)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	sl_uint8* buf = (sl_uint8*)(_out);
	sl_size k = 0;
	for (; i < n; i += 2) {
		sl_uint32 v1, v2;
		{
			sl_uint32 ch = (sl_uint32)sz[i];
			if (ch >= '0' && ch <= '9') {
				v1 = ch - '0';
			} else if (ch >= 'A' && ch <= 'F') {
				v1 = ch - 'A' + 10;
			} else if (ch >= 'a' && ch <= 'f') {
				v1 = ch - 'a' + 10;
			} else {
				break;
			}
		}
		{
			sl_uint32 ch = (sl_uint32)sz[i + 1];
			if (ch >= '0' && ch <= '9') {
				v2 = ch - '0';
			} else if (ch >= 'A' && ch <= 'F') {
				v2 = ch - 'A' + 10;
			} else if (ch >= 'a' && ch <= 'f') {
				v2 = ch - 'a' + 10;
			} else {
				break;
			}
		}
		buf[k++] = (sl_uint8)((v1 << 4) | v2);
	}
	return i;
}

sl_reg String8::parseHexString(void* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseHexString(sz, posBegin, len, _out);
}

sl_reg String16::parseHexString(void* _out, const char* sz, sl_size posBegin, sl_size len)
{
	return _String_parseHexString(sz, posBegin, len, _out);
}

sl_reg String8::parseHexString(void* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseHexString(sz, posBegin, len, _out);
}

sl_reg String16::parseHexString(void* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _String_parseHexString(sz, posBegin, len, _out);
}

sl_bool String8::parseHexString(void* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseHexString(getData(), 0, n, _out) == n;
}

sl_bool String16::parseHexString(void* _out) const
{
	sl_size n = getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseHexString(getData(), 0, n, _out) == n;
}

sl_bool SafeString8::parseHexString(void* _out) const
{
	String8 s(*this);
	return s.parseHexString(_out);
}

sl_bool SafeString16::parseHexString(void* _out) const
{
	String16 s(*this);
	return s.parseHexString(_out);
}

SLIB_NAMESPACE_END
