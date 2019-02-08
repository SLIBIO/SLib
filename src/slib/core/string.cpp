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

#define SLIB_SUPPORT_STD_TYPES

#include "slib/core/string.h"
#include "slib/core/string_buffer.h"

#include "slib/core/base.h"
#include "slib/core/mio.h"
#include "slib/core/endian.h"
#include "slib/core/scoped.h"
#include "slib/core/variant.h"
#include "slib/core/json.h"
#include "slib/core/cast.h"
#include "slib/core/math.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringData)
	
	StringData::StringData() noexcept : sz8(0), len(0)
	{
	}

	class _priv_TemplateFunc8
	{
	public:
		SLIB_INLINE static const void* findMemory(const sl_char8* mem, sl_char8 pattern, sl_size count) noexcept
		{
			return Base::findMemory(mem, pattern, count);
		}
		
		SLIB_INLINE static const void* findMemoryReverse(const sl_char8* mem, sl_char8 pattern, sl_size count) noexcept
		{
			return Base::findMemoryReverse(mem, pattern, count);
		}
		
		SLIB_INLINE static sl_int32 compareMemory(const sl_char8* mem1, const sl_char8* mem2, sl_size count) noexcept
		{
			return Base::compareMemory((sl_uint8*)mem1, (sl_uint8*)mem2, count);
		}
		
		SLIB_INLINE static void resetMemory(sl_char8* dst, sl_char8 value, sl_size count) noexcept
		{
			return Base::resetMemory(dst, value, count);
		}
	};

	class _priv_TemplateFunc16
	{
	public:
		SLIB_INLINE static const void* findMemory(const sl_char16* mem, sl_char16 pattern, sl_size count) noexcept
		{
			return Base::findMemory2((sl_uint16*)mem, pattern, count);
		}
		
		SLIB_INLINE static const void* findMemoryReverse(const sl_char16* mem, sl_char16 pattern, sl_size count) noexcept
		{
			return Base::findMemoryReverse2((sl_uint16*)mem, pattern, count);
		}
		
		SLIB_INLINE static sl_int16 compareMemory(const sl_char16* mem1, const sl_char16* mem2, sl_size count) noexcept
		{
			return Base::compareMemory2((sl_uint16*)mem1, (sl_uint16*)mem2, count);
		}
		
		SLIB_INLINE static void resetMemory(sl_char16* dst, sl_char16 value, sl_size count) noexcept
		{
			return Base::resetMemory2((sl_uint16*)dst, value, count);
		}
	};
	
	enum STRING_CONTAINER_TYPES {
		STRING_CONTAINER_TYPE_NORMAL = 0,
		STRING_CONTAINER_TYPE_STD = 10,
		STRING_CONTAINER_TYPE_REF = 11
	};

	const _priv_String_Const _priv_String_Null = {sl_null, 0};

	const sl_char8 _g_string8_empty_buf[] = {0, 0};
	const StringContainer _g_string8_empty_container = {const_cast<sl_char8*>(_g_string8_empty_buf), 0, 0, STRING_CONTAINER_TYPE_NORMAL, -1};
	const _priv_String_Const _priv_String_Empty = {const_cast<StringContainer*>(&_g_string8_empty_container), 0};

	const _priv_String16_Const _priv_String16_Null = {sl_null, 0};

	const sl_char16 _g_string16_empty_buf[] = {0, 0};
	const StringContainer16 _g_string16_empty_container = {const_cast<sl_char16*>(_g_string16_empty_buf), 0, 0, STRING_CONTAINER_TYPE_NORMAL, -1};
	const _priv_String16_Const _priv_String16_Empty = {const_cast<StringContainer16*>(&_g_string16_empty_container), 0};


	SLIB_INLINE sl_reg StringContainer::increaseReference() noexcept
	{
		if (ref >= 0) {
			return Base::interlockedIncrement(&ref);
		}
		return 1;
	}

	SLIB_INLINE sl_reg StringContainer16::increaseReference() noexcept
	{
		if (ref >= 0) {
			return Base::interlockedIncrement(&ref);
		}
		return 1;
	}

	class _priv_StringContainer_std : public StringContainer
	{
	public:
		std::string str;
		
	public:
		_priv_StringContainer_std(const std::string& _str): str(_str)
		{
		}
		
		_priv_StringContainer_std(std::string&& _str): str(Move(_str))
		{
		}

	};
	
	class _priv_StringContainer_ref : public StringContainer
	{
	public:
		Ref<Referable> obj;
		
	public:
		_priv_StringContainer_ref(const Ref<Referable>& _obj): obj(_obj)
		{
		}
		
	};

	SLIB_INLINE sl_reg StringContainer::decreaseReference() noexcept
	{
		if (ref > 0) {
			sl_reg nRef = Base::interlockedDecrement(&ref);
			if (nRef == 0) {
				if (type == STRING_CONTAINER_TYPE_STD) {
					_priv_StringContainer_std* container = static_cast<_priv_StringContainer_std*>(this);
					container->_priv_StringContainer_std::~_priv_StringContainer_std();
				} else if (type == STRING_CONTAINER_TYPE_REF) {
					_priv_StringContainer_ref* container = static_cast<_priv_StringContainer_ref*>(this);
					container->_priv_StringContainer_ref::~_priv_StringContainer_ref();
				}
				Base::freeMemory(this);
			}
			return nRef;
		}
		return 1;
	}
	
	class _priv_StringContainer16_std : public StringContainer16
	{
	public:
		std::u16string str;
		
	public:
		_priv_StringContainer16_std(const std::u16string& _str): str(_str)
		{
		}
		
		_priv_StringContainer16_std(std::u16string&& _str): str(Move(_str))
		{
		}

	};
	
	class _priv_StringContainer16_ref : public StringContainer16
	{
	public:
		Ref<Referable> obj;
		
	public:
		_priv_StringContainer16_ref(const Ref<Referable>& _obj): obj(_obj)
		{
		}
		
	};

	SLIB_INLINE sl_reg StringContainer16::decreaseReference() noexcept
	{
		if (ref > 0) {
			sl_reg nRef = Base::interlockedDecrement(&ref);
			if (nRef == 0) {
				if (type == STRING_CONTAINER_TYPE_STD) {
					_priv_StringContainer16_std* container = static_cast<_priv_StringContainer16_std*>(this);
					container->_priv_StringContainer16_std::~_priv_StringContainer16_std();
				} else if (type == STRING_CONTAINER_TYPE_REF) {
					_priv_StringContainer16_ref* container = static_cast<_priv_StringContainer16_ref*>(this);
					container->_priv_StringContainer16_ref::~_priv_StringContainer16_ref();
				}
				Base::freeMemory(this);
			}
			return nRef;
		}
		return 1;
	}

	SLIB_INLINE static StringContainer* _priv_String_alloc(sl_size len) noexcept
	{
		if (len == 0) {
			return _priv_String_Empty.container;
		}
		sl_char8* buf = (sl_char8*)(Base::createMemory(sizeof(StringContainer) + len + 1));
		if (buf) {
			StringContainer* container = reinterpret_cast<StringContainer*>(buf);
			container->sz = buf + sizeof(StringContainer);
			container->len = len;
			container->hash = 0;
			container->type = STRING_CONTAINER_TYPE_NORMAL;
			container->ref = 1;
			container->sz[len] = 0;
			return container;
		}
		return sl_null;
	}

	SLIB_INLINE static StringContainer16* _priv_String16_alloc(sl_size len) noexcept
	{
		if (len == 0) {
			return _priv_String16_Empty.container;
		}
		sl_char8* buf = (sl_char8*)(Base::createMemory(sizeof(StringContainer) + ((len + 1) << 1)));
		if (buf) {
			StringContainer16* container = reinterpret_cast<StringContainer16*>(buf);
			container->sz = (sl_char16*)((void*)(buf + sizeof(StringContainer16)));
			container->len = len;
			container->hash = 0;
			container->type = STRING_CONTAINER_TYPE_NORMAL;
			container->ref = 1;
			container->sz[len] = 0;
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_alloc_static(const sl_char8* sz, sl_size len) noexcept
	{
		if (len == 0) {
			return _priv_String_Empty.container;
		}
		StringContainer* container = (StringContainer*)(Base::createMemory(sizeof(StringContainer)));
		if (container) {
			container->sz = (sl_char8*)sz;
			container->len = len;
			container->hash = 0;
			container->type = STRING_CONTAINER_TYPE_NORMAL;
			container->ref = 1;
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_alloc_static(const sl_char16* sz, sl_size len) noexcept
	{
		if (len == 0) {
			return _priv_String16_Empty.container;
		}
		StringContainer16* container = (StringContainer16*)(Base::createMemory(sizeof(StringContainer16)));
		if (container) {
			container->sz = (sl_char16*)sz;
			container->len = len;
			container->hash = 0;
			container->type = STRING_CONTAINER_TYPE_NORMAL;
			container->ref = 1;
			return container;
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE static StringContainer* _priv_String_alloc_std(T&& str) noexcept
	{
		sl_size len = (sl_size)(str.length());
		if (len == 0) {
			return _priv_String_Empty.container;
		}
		if (len < 40) {
			StringContainer* container = _priv_String_alloc(len);
			if (container) {
				Base::copyMemory(container->sz, str.c_str(), len);
				return container;
			}
		} else {
			_priv_StringContainer_std* container = (_priv_StringContainer_std*)(Base::createMemory(sizeof(_priv_StringContainer_std)));
			if (container) {
				new (container) _priv_StringContainer_std(Forward<T>(str));
				container->sz = (sl_char8*)(container->str.c_str());
				container->len = len;
				container->hash = 0;
				container->type = STRING_CONTAINER_TYPE_STD;
				container->ref = 1;
				return container;
			}
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE static StringContainer16* _priv_String16_alloc_std(T&& str) noexcept
	{
		sl_size len = (sl_size)(str.length());
		if (len == 0) {
			return _priv_String16_Empty.container;
		}
		if (len < 20) {
			StringContainer16* container = _priv_String16_alloc(len);
			if (container) {
				Base::copyMemory(container->sz, str.c_str(), len * sizeof(sl_char16));
				return container;
			}
		} else {
			_priv_StringContainer16_std* container = (_priv_StringContainer16_std*)(Base::createMemory(sizeof(_priv_StringContainer16_std)));
			if (container) {
				new (container) _priv_StringContainer16_std(Forward<T>(str));
				container->sz = (sl_char16*)(container->str.c_str());
				container->len = len;
				container->hash = 0;
				container->type = STRING_CONTAINER_TYPE_STD;
				container->ref = 1;
				return container;
			}
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_alloc_ref(const Ref<Referable>& obj, const sl_char8* sz, sl_size len) noexcept
	{
		if (len == 0) {
			return _priv_String_Empty.container;
		}
		_priv_StringContainer_ref* container = (_priv_StringContainer_ref*)(Base::createMemory(sizeof(_priv_StringContainer_ref)));
		if (container) {
			new (container) _priv_StringContainer_ref(obj);
			container->sz = (sl_char8*)(sz);
			container->len = len;
			container->hash = 0;
			container->type = STRING_CONTAINER_TYPE_REF;
			container->ref = 1;
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_alloc_ref(const Ref<Referable>& obj, const sl_char16* sz, sl_size len) noexcept
	{
		if (len == 0) {
			return _priv_String16_Empty.container;
		}
		_priv_StringContainer16_ref* container = (_priv_StringContainer16_ref*)(Base::createMemory(sizeof(_priv_StringContainer16_ref)));
		if (container) {
			new (container) _priv_StringContainer16_ref(obj);
			container->sz = (sl_char16*)(sz);
			container->len = len;
			container->hash = 0;
			container->type = STRING_CONTAINER_TYPE_REF;
			container->ref = 1;
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_create(sl_char8 ch, sl_size nRepeatCount) noexcept
	{
		StringContainer* container = _priv_String_alloc(nRepeatCount);
		if (container) {
			Base::resetMemory(container->sz, ch, nRepeatCount);
		}
		return container;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_create(sl_char16 ch, sl_size nRepeatCount) noexcept
	{
		StringContainer16* container = _priv_String16_alloc(nRepeatCount);
		if (container) {
			Base::resetMemory2((sl_uint16*)(container->sz), ch, nRepeatCount);
		}
		return container;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_create(const sl_char8* utf8, sl_reg lenUtf8) noexcept
	{
		if (utf8) {
			if (lenUtf8 < 0) {
				lenUtf8 = Base::getStringLength(utf8);
			}
			sl_size len = lenUtf8;
			StringContainer* container = _priv_String_alloc(len);
			if (container) {
				Base::copyMemory(container->sz, utf8, len);
			}
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_create(const sl_char8* utf8, sl_reg lenUtf8) noexcept
	{
		if (utf8) {
			if (lenUtf8 < 0) {
				lenUtf8 = Base::getStringLength(utf8);
			}
			sl_size len = Charsets::utf8ToUtf16(utf8, lenUtf8, sl_null, -1);
			StringContainer16* container = _priv_String16_alloc(len);
			if (container && len > 0) {
				Charsets::utf8ToUtf16(utf8, lenUtf8, container->sz, len);
				container->sz[len] = 0;
			}
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_create(const sl_char16* utf16, sl_reg lenUtf16) noexcept
	{
		if (utf16) {
			if (lenUtf16 < 0) {
				lenUtf16 = Base::getStringLength2(utf16);
			}
			sl_size len = Charsets::utf16ToUtf8(utf16, lenUtf16, sl_null, -1);
			StringContainer* container = _priv_String_alloc(len);
			if (container && len > 0) {
				Charsets::utf16ToUtf8(utf16, lenUtf16, container->sz, len);
				container->sz[len] = 0;
			}
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_create(const sl_char16* utf16, sl_reg lenUtf16) noexcept
	{
		if (utf16) {
			if (lenUtf16 < 0) {
				lenUtf16 = Base::getStringLength2(utf16);
			}
			sl_size len = lenUtf16;
			StringContainer16* container = _priv_String16_alloc(len);
			if (container) {
				Base::copyMemory(container->sz, utf16, len*sizeof(sl_char16));
			}
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_create(const sl_char32* utf32, sl_reg lenUtf32) noexcept
	{
		if (utf32) {
			if (lenUtf32 < 0) {
				lenUtf32 = Base::getStringLength4(utf32);
			}
			sl_size len = Charsets::utf32ToUtf8(utf32, lenUtf32, sl_null, -1);
			StringContainer* container = _priv_String_alloc(len);
			if (container && len > 0) {
				Charsets::utf32ToUtf8(utf32, lenUtf32, container->sz, len);
				container->sz[len] = 0;
			}
			return container;
		}
		return sl_null;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_create(const sl_char32* utf32, sl_reg lenUtf32) noexcept
	{
		if (utf32) {
			if (lenUtf32 < 0) {
				lenUtf32 = Base::getStringLength4(utf32);
			}
			sl_size len = Charsets::utf32ToUtf16(utf32, lenUtf32, sl_null, -1);
			StringContainer16* container = _priv_String16_alloc(len);
			if (container && len > 0) {
				Charsets::utf32ToUtf16(utf32, lenUtf32, container->sz, len);
				container->sz[len] = 0;
			}
			return container;
		}
		return sl_null;
	}
	
	
	SLIB_INLINE static StringContainer* _priv_String_merge8(const sl_char8* s1, sl_reg len1, const sl_char8* s2, sl_reg len2) noexcept
	{
		if (len1 < 0) {
			len1 = Base::getStringLength(s1);
		}
		if (len2 < 0) {
			len2 = Base::getStringLength(s2);
		}
		sl_size len = len1 + len2;
		StringContainer* s = _priv_String_alloc(len);
		if (s && len > 0) {
			Base::copyMemory(s->sz, s1, len1);
			Base::copyMemory(s->sz + len1, s2, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_merge16(const sl_char16* s1, sl_reg len1, const sl_char16* s2, sl_reg len2) noexcept
	{
		if (len1 < 0) {
			len1 = Base::getStringLength2(s1);
		}
		if (len2 < 0) {
			len2 = Base::getStringLength2(s2);
		}
		sl_size len = len1 + len2;
		StringContainer16* s = _priv_String16_alloc(len);
		if (s && len > 0) {
			Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
			Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_merge16(const sl_char8* s1, sl_reg len1, const sl_char16* s2_u16, sl_reg len2_u16) noexcept
	{
		if (len1 < 0) {
			len1 = Base::getStringLength(s1);
		}
		if (len2_u16 < 0) {
			len2_u16 = Base::getStringLength2(s2_u16);
		}
		sl_size len2 = Charsets::utf16ToUtf8(s2_u16, len2_u16, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer* s = _priv_String_alloc(len);
		if (s && len > 0) {
			Base::copyMemory(s->sz, s1, len1);
			Charsets::utf16ToUtf8(s2_u16, len2_u16, s->sz + len1, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_merge16(const sl_char16* s1_u16, sl_reg len1_u16, const sl_char8* s2, sl_reg len2) noexcept
	{
		if (len1_u16 < 0) {
			len1_u16 = Base::getStringLength2(s1_u16);
		}
		if (len2 < 0) {
			len2 = Base::getStringLength(s2);
		}
		sl_size len1 = Charsets::utf16ToUtf8(s1_u16, len1_u16, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer* s = _priv_String_alloc(len);
		if (s && len > 0) {
			Charsets::utf16ToUtf8(s1_u16, len1_u16, s->sz, len1);
			Base::copyMemory(s->sz + len1, s2, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_merge32(const sl_char8* s1, sl_reg len1, const sl_char32* s2_u32, sl_reg len2_u32) noexcept
	{
		if (len1 < 0) {
			len1 = Base::getStringLength(s1);
		}
		if (len2_u32 < 0) {
			len2_u32 = Base::getStringLength4(s2_u32);
		}
		sl_size len2 = Charsets::utf32ToUtf8(s2_u32, len2_u32, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer* s = _priv_String_alloc(len);
		if (s && len > 0) {
			Base::copyMemory(s->sz, s1, len1*sizeof(sl_char8));
			Charsets::utf32ToUtf8(s2_u32, len2_u32, s->sz + len1, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer* _priv_String_merge32(const sl_char32* s1_u32, sl_reg len1_u32, const sl_char8* s2, sl_reg len2) noexcept
	{
		if (len1_u32 < 0) {
			len1_u32 = Base::getStringLength4(s1_u32);
		}
		if (len2 < 0) {
			len2 = Base::getStringLength(s2);
		}
		sl_size len1 = Charsets::utf32ToUtf8(s1_u32, len1_u32, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer* s = _priv_String_alloc(len);
		if (s && len > 0) {
			Charsets::utf32ToUtf8(s1_u32, len1_u32, s->sz, len1);
			Base::copyMemory(s->sz + len1, s2, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_merge8(const sl_char16* s1, sl_reg len1, const sl_char8* s2_u8, sl_reg len2_u8) noexcept
	{
		if (len1 < 0) {
			len1 = Base::getStringLength2(s1);
		}
		if (len2_u8 < 0) {
			len2_u8 = Base::getStringLength(s2_u8);
		}
		sl_size len2 = Charsets::utf8ToUtf16(s2_u8, len2_u8, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer16* s = _priv_String16_alloc(len);
		if (s && len > 0) {
			Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
			Charsets::utf8ToUtf16(s2_u8, len2_u8, s->sz + len1, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_merge8(const sl_char8* s1_u8, sl_reg len1_u8, const sl_char16* s2, sl_reg len2) noexcept
	{
		if (len1_u8 < 0) {
			len1_u8 = Base::getStringLength(s1_u8);
		}
		if (len2 < 0) {
			len2 = Base::getStringLength2(s2);
		}
		sl_size len1 = Charsets::utf8ToUtf16(s1_u8, len1_u8, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer16* s = _priv_String16_alloc(len);
		if (s && len > 0) {
			Charsets::utf8ToUtf16(s1_u8, len1_u8, s->sz, len1);
			Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_merge32(const sl_char16* s1, sl_reg len1, const sl_char32* s2_u32, sl_reg len2_u32) noexcept
	{
		if (len1 < 0) {
			len1 = Base::getStringLength2(s1);
		}
		if (len2_u32 < 0) {
			len2_u32 = Base::getStringLength4(s2_u32);
		}
		sl_size len2 = Charsets::utf32ToUtf16(s2_u32, len2_u32, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer16* s = _priv_String16_alloc(len);
		if (s && len > 0) {
			Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
			Charsets::utf32ToUtf16(s2_u32, len2_u32, s->sz + len1, len2);
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static StringContainer16* _priv_String16_merge32(const sl_char32* s1_u32, sl_reg len1_u32, const sl_char16* s2, sl_reg len2) noexcept
	{
		if (len1_u32 < 0) {
			len1_u32 = Base::getStringLength4(s1_u32);
		}
		if (len2 < 0) {
			len2 = Base::getStringLength2(s2);
		}
		sl_size len1 = Charsets::utf32ToUtf16(s1_u32, len1_u32, sl_null, -1);
		sl_size len = len1 + len2;
		StringContainer16* s = _priv_String16_alloc(len);
		if (s && len > 0) {
			Charsets::utf32ToUtf16(s1_u32, len1_u32, s->sz, len1);
			Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
			s->sz[len] = 0;
		}
		return s;
	}
	
	SLIB_INLINE static sl_int32 _priv_String_compare_objects(const sl_char8* str1, sl_size len1, const sl_char8* str2, sl_size len2) noexcept
	{
		if (len1 < len2) {
			if (len1 == 0) {
				return -1;
			}
			sl_int32 r = Base::compareMemory((sl_uint8*)str1, (sl_uint8*)str2, len1);
			if (r == 0) {
				return -1;
			} else {
				return r;
			}
		} else if (len1 > len2) {
			if (len2 == 0) {
				return 1;
			}
			sl_int32 r = Base::compareMemory((sl_uint8*)str1, (sl_uint8*)str2, len2);
			if (r == 0) {
				return 1;
			} else {
				return r;
			}
		} else {
			if (len1 == 0) {
				return 0;
			}
			return Base::compareMemory((sl_uint8*)str1, (sl_uint8*)str2, len1);
		}
	}
	
	SLIB_INLINE static sl_int32 _priv_String16_compare_objects(const sl_char16* str1, sl_size len1, const sl_char16* str2, sl_size len2) noexcept
	{
		if (len1 < len2) {
			if (len1 == 0) {
				return -1;
			}
			sl_int32 r = Base::compareMemory2((sl_uint16*)str1, (sl_uint16*)str2, len1);
			if (r == 0) {
				return -1;
			} else {
				return r;
			}
		} else if (len1 > len2) {
			if (len2 == 0) {
				return 1;
			}
			sl_int32 r = Base::compareMemory2((sl_uint16*)str1, (sl_uint16*)str2, len2);
			if (r == 0) {
				return 1;
			} else {
				return r;
			}
		} else {
			if (len1 == 0) {
				return 0;
			}
			return Base::compareMemory2((sl_uint16*)str1, (sl_uint16*)str2, len1);
		}
	}
	
	SLIB_INLINE static sl_int32 _priv_String_compare8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
	{
		if (len1 < 0) {
			if (len2 < 0) {
				return Base::compareString(str1, str2);
			} else {
				for (sl_reg i = 0; i < len2; i++) {
					if (str1[i] < str2[i]) {
						return -1;
					} else if (str1[i] > str2[i]) {
						return 1;
					}
					if (str1[i] == 0) {
						break;
					}
				}
				if (str1[len2] < 0) {
					return -1;
				} else if (str1[len2] > 0) {
					return 1;
				} else {
					return 0;
				}
			}
		} else {
			if (len2 < 0) {
				for (sl_reg i = 0; i < len1; i++) {
					if (str1[i] < str2[i]) {
						return -1;
					} else if (str1[i] > str2[i]) {
						return 1;
					}
					if (str1[i] == 0) {
						break;
					}
				}
				if (0 < str2[len1]) {
					return -1;
				} else if (0 > str2[len1]) {
					return 1;
				} else {
					return 0;
				}
			} else {
				return _priv_String_compare_objects(str1, len1, str2, len2);
			}
		}
	}
	
	SLIB_INLINE static sl_int32 _priv_String16_compare16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
	{
		if (len1 < 0) {
			if (len2 < 0) {
				return Base::compareString2(str1, str2);
			} else {
				for (sl_reg i = 0; i < len2; i++) {
					if (str1[i] < str2[i]) {
						return -1;
					} else if (str1[i] > str2[i]) {
						return 1;
					}
					if (str1[i] == 0) {
						break;
					}
				}
				if (str1[len2] < 0) {
					return -1;
				} else if (str1[len2] > 0) {
					return 1;
				} else {
					return 0;
				}
			}
		} else {
			if (len2 < 0) {
				for (sl_reg i = 0; i < len1; i++) {
					if (str1[i] < str2[i]) {
						return -1;
					} else if (str1[i] > str2[i]) {
						return 1;
					}
					if (str1[i] == 0) {
						break;
					}
				}
				if (0 < str2[len1]) {
					return -1;
				} else if (0 > str2[len1]) {
					return 1;
				} else {
					return 0;
				}
			} else {
				return _priv_String16_compare_objects(str1, len1, str2, len2);
			}
		}
	}
	
	SLIB_INLINE static sl_int32 _priv_String_compare16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
	{
		String other(str2, len2);
		return _priv_String_compare8(str1, len1, other.getData(), other.getLength());
	}
	
	SLIB_INLINE static sl_int32 _priv_String16_compare8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
	{
		String16 other(str2, len2);
		return _priv_String16_compare16(str1, len1, other.getData(), other.getLength());
	}
	
	SLIB_INLINE static sl_int32 _priv_String_compare32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2) noexcept
	{
		String other(str2, len2);
		return _priv_String_compare8(str1, len1, other.getData(), other.getLength());
	}
	
	SLIB_INLINE static sl_int32 _priv_String16_compare32(const sl_char16* m1, sl_reg len1, const sl_char32* m2, sl_reg len2) noexcept
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

	
	SLIB_INLINE static sl_bool _priv_String_equals_objects(const sl_char8* str1, sl_size len1, const sl_char8* str2, sl_size len2) noexcept
	{
		if (len1 == len2) {
			if (len1 == 0) {
				return sl_true;
			}
			return Base::equalsMemory(str1, str2, len1);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE static sl_bool _priv_String16_equals_objects(const sl_char16* str1, sl_size len1, const sl_char16* str2, sl_size len2) noexcept
	{
		if (len1 == len2) {
			if (len1 == 0) {
				return sl_true;
			}
			return Base::equalsMemory2((sl_uint16*)str1, (sl_uint16*)str2, len1);
		} else {
			return sl_false;
		}
	}

	SLIB_INLINE static sl_bool _priv_String_equals8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
	{
		if (len1 < 0) {
			if (len2 < 0) {
				return Base::equalsString(str1, str2);
			} else {
				for (sl_reg i = 0; i < len2; i++) {
					if (str1[i] != str2[i]) {
						return sl_false;
					}
					if (str1[i] == 0) {
						return sl_true;
					}
				}
				return str1[len2] == 0;
			}
		} else {
			if (len2 < 0) {
				for (sl_reg i = 0; i < len1; i++) {
					if (str1[i] != str2[i]) {
						return sl_false;
					}
					if (str1[i] == 0) {
						return sl_true;
					}
				}
				return str2[len1] == 0;
			} else {
				return _priv_String_equals_objects(str1, len1, str2, len2);
			}
		}
	}
	
	SLIB_INLINE static sl_bool _priv_String16_equals16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
	{
		if (len1 < 0) {
			if (len2 < 0) {
				return Base::equalsString2(str1, str2);
			} else {
				for (sl_reg i = 0; i < len2; i++) {
					if (str1[i] != str2[i]) {
						return sl_false;
					}
					if (str1[i] == 0) {
						return sl_true;
					}
				}
				return str1[len2] == 0;
			}
		} else {
			if (len2 < 0) {
				for (sl_reg i = 0; i < len1; i++) {
					if (str1[i] != str2[i]) {
						return sl_false;
					}
					if (str1[i] == 0) {
						return sl_true;
					}
				}
				return str2[len1] == 0;
			} else {
				return _priv_String16_equals_objects(str1, len1, str2, len2);
			}
		}
	}
	
	SLIB_INLINE static sl_bool _priv_String_equals16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
	{
		String other(str2, len2);
		return _priv_String_equals8(str1, len1, other.getData(), other.getLength());
	}
	
	SLIB_INLINE static sl_bool _priv_String16_equals8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
	{
		String16 other(str2, len2);
		return _priv_String16_equals16(str1, len1, other.getData(), other.getLength());
	}
	
	SLIB_INLINE static sl_bool _priv_String_equals32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2) noexcept
	{
		String other(str2, len2);
		return _priv_String_equals8(str1, len1, other.getData(), other.getLength());
	}
	
	SLIB_INLINE static sl_bool _priv_String16_equals32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2) noexcept
	{
		return _priv_String16_compare32(str1, len1, str2, len2) == 0;
	}
	
	SLIB_INLINE StringContainer* Atomic<String>::_retainContainer() const noexcept
	{
		if (m_container) {
			SpinLocker lock(&m_lock);
			StringContainer* container = m_container;
			if (container) {
				container->increaseReference();
			}
			return container;
		}
		return sl_null;
	}

	SLIB_INLINE StringContainer16* Atomic<String16>::_retainContainer() const noexcept
	{
		if (m_container) {
			SpinLocker lock(&m_lock);
			StringContainer16* container = m_container;
			if (container) {
				container->increaseReference();
			}
			return container;
		}
		return sl_null;
	}


	SLIB_INLINE void String::_replaceContainer(StringContainer* container) noexcept
	{
		if (m_container) {
			m_container->decreaseReference();
		}
		m_container = container;
	}

	SLIB_INLINE void String16::_replaceContainer(StringContainer16* container) noexcept
	{
		if (m_container) {
			m_container->decreaseReference();
		}
		m_container = container;
	}

	SLIB_INLINE void Atomic<String>::_replaceContainer(StringContainer* container) noexcept
	{
		StringContainer* before;
		{
			SpinLocker lock(&m_lock);
			before = m_container;
			m_container = container;
		}
		if (before) {
			before->decreaseReference();
		}
	}

	SLIB_INLINE void Atomic<String16>::_replaceContainer(StringContainer16* container) noexcept
	{
		StringContainer16* before;
		{
			SpinLocker lock(&m_lock);
			before = m_container;
			m_container = container;
		}
		if (before) {
			before->decreaseReference();
		}
	}

	String::String(const String& src) noexcept
	{
		StringContainer* container = src.m_container;
		if (container) {
			container->increaseReference();
		}
		m_container= container;
	}

	String16::String16(const String16& src) noexcept
	{
		StringContainer16* container = src.m_container;
		if (container) {
			container->increaseReference();
		}
		m_container = container;
	}

	Atomic<String>::Atomic(const String& src) noexcept
	{
		StringContainer* container = src.m_container;
		if (container) {
			container->increaseReference();
		}
		m_container = container;
	}

	Atomic<String16>::Atomic(const String16& src) noexcept
	{
		StringContainer16* container = src.m_container;
		if (container) {
			container->increaseReference();
		}
		m_container = container;
	}


	String::String(const AtomicString& src) noexcept
	{
		m_container = src._retainContainer();
	}

	String16::String16(const AtomicString16& src) noexcept
	{
		m_container = src._retainContainer();
	}

	Atomic<String>::Atomic(const AtomicString& src) noexcept
	{
		m_container = src._retainContainer();
	}

	Atomic<String16>::Atomic(const AtomicString16& src) noexcept
	{
		m_container = src._retainContainer();
	}


	String::String(const String16& src) noexcept
	{
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String_Empty.container;
			} else {
				m_container = _priv_String_create(src.getData(), len);
			}
		}
	}

	String16::String16(const String& src) noexcept
	{
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String16_Empty.container;
			} else {
				m_container = _priv_String16_create(src.getData(), len);
			}
		}
	}

	Atomic<String>::Atomic(const String16& src) noexcept
	{
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String_Empty.container;
			} else {
				m_container = _priv_String_create(src.getData(), len);
			}
		}
	}

	Atomic<String16>::Atomic(const String& src) noexcept
	{
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String16_Empty.container;
			} else {
				m_container = _priv_String16_create(src.getData(), len);
			}
		}
	}


	String::String(const AtomicString16& _src) noexcept
	{
		String16 src(_src);
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String_Empty.container;
			} else {
				m_container = _priv_String_create(src.getData(), len);
			}
		}
	}

	String16::String16(const AtomicString& _src) noexcept
	{
		String src(_src);
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String16_Empty.container;
			} else {
				m_container = _priv_String16_create(src.getData(), len);
			}
		}
	}

	Atomic<String>::Atomic(const AtomicString16& _src) noexcept
	{
		String16 src(_src);
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String_Empty.container;
			} else {
				m_container = _priv_String_create(src.getData(), len);
			}
		}
	}

	Atomic<String16>::Atomic(const AtomicString& _src) noexcept
	{
		String src(_src);
		if (src.isNull()) {
			m_container = sl_null;
		} else {
			sl_size len = src.getLength();
			if (len == 0) {
				m_container = _priv_String16_Empty.container;
			} else {
				m_container = _priv_String16_create(src.getData(), len);
			}
		}
	}


	String::~String() noexcept
	{
		StringContainer* container = m_container;
		if (container) {
			container->decreaseReference();
		}
	}

	String16::~String16() noexcept
	{
		StringContainer16* container = m_container;
		if (container) {
			container->decreaseReference();
		}
	}

	Atomic<String>::~Atomic() noexcept
	{
		StringContainer* container = m_container;
		if (container) {
			container->decreaseReference();
		}
	}

	Atomic<String16>::~Atomic() noexcept
	{
		StringContainer16* container = m_container;
		if (container) {
			container->decreaseReference();
		}
	}


	String::String(sl_char8 ch, sl_size nRepeatCount) noexcept
	{
		m_container = _priv_String_create(ch, nRepeatCount);
	}

	Atomic<String>::Atomic(sl_char8 ch, sl_size nRepeatCount) noexcept
	{
		m_container = _priv_String_create(ch, nRepeatCount);
	}

	String16::String16(sl_char16 ch, sl_size nRepeatCount) noexcept
	{
		m_container = _priv_String16_create(ch, nRepeatCount);
	}

	Atomic<String16>::Atomic(sl_char16 ch, sl_size nRepeatCount) noexcept
	{
		m_container = _priv_String16_create(ch, nRepeatCount);
	}


	String::String(const char* strUtf8) noexcept
	{
		m_container = _priv_String_create(strUtf8, -1);
	}

	String::String(const char* strUtf8, sl_reg length) noexcept
	{
		m_container = _priv_String_create(strUtf8, length);
	}

	Atomic<String>::Atomic(const char* strUtf8) noexcept
	{
		m_container = _priv_String_create(strUtf8, -1);
	}

	Atomic<String>::Atomic(const char* strUtf8, sl_reg length) noexcept
	{
		m_container = _priv_String_create(strUtf8, length);
	}

	String16::String16(const char* strUtf8) noexcept
	{
		m_container = _priv_String16_create(strUtf8, -1);
	}

	String16::String16(const char* strUtf8, sl_reg length) noexcept
	{
		m_container = _priv_String16_create(strUtf8, length);
	}

	Atomic<String16>::Atomic(const char* strUtf8) noexcept
	{
		m_container = _priv_String16_create(strUtf8, -1);
	}

	Atomic<String16>::Atomic(const char* strUtf8, sl_reg length) noexcept
	{
		m_container = _priv_String16_create(strUtf8, length);
	}

	
	String::String(const wchar_t* strUnicode) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String_create((sl_char16*)strUnicode, -1);
		} else {
			m_container = _priv_String_create((sl_char32*)strUnicode, -1);
		}
	}
	
	String::String(const wchar_t* strUnicode, sl_reg length) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String_create((sl_char16*)strUnicode, length);
		} else {
			m_container = _priv_String_create((sl_char32*)strUnicode, length);
		}
	}
	
	Atomic<String>::Atomic(const wchar_t* strUnicode) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String_create((sl_char16*)strUnicode, -1);
		} else {
			m_container = _priv_String_create((sl_char32*)strUnicode, -1);
		}
	}
	
	Atomic<String>::Atomic(const wchar_t* strUnicode, sl_reg length) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String_create((sl_char16*)strUnicode, length);
		} else {
			m_container = _priv_String_create((sl_char32*)strUnicode, length);
		}
	}
	
	String16::String16(const wchar_t* strUnicode) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String16_create((sl_char16*)strUnicode, -1);
		} else {
			m_container = _priv_String16_create((sl_char32*)strUnicode, -1);
		}
	}
	
	String16::String16(const wchar_t* strUnicode, sl_reg length) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String16_create((sl_char16*)strUnicode, length);
		} else {
			m_container = _priv_String16_create((sl_char32*)strUnicode, length);
		}
	}
	
	Atomic<String16>::Atomic(const wchar_t* strUnicode) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String16_create((sl_char16*)strUnicode, -1);
		} else {
			m_container = _priv_String16_create((sl_char32*)strUnicode, -1);
		}
	}
	
	Atomic<String16>::Atomic(const wchar_t* strUnicode, sl_reg length) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			m_container = _priv_String16_create((sl_char16*)strUnicode, length);
		} else {
			m_container = _priv_String16_create((sl_char32*)strUnicode, length);
		}
	}

	
	String::String(const char16_t* strUtf16) noexcept
	{
		m_container = _priv_String_create(strUtf16, -1);
	}

	String::String(const char16_t* strUtf16, sl_reg length) noexcept
	{
		m_container = _priv_String_create(strUtf16, length);
	}

	Atomic<String>::Atomic(const char16_t* strUtf16) noexcept
	{
		m_container = _priv_String_create(strUtf16, -1);
	}

	Atomic<String>::Atomic(const char16_t* strUtf16, sl_reg length) noexcept
	{
		m_container = _priv_String_create(strUtf16, length);
	}

	String16::String16(const char16_t* strUtf16) noexcept
	{
		m_container = _priv_String16_create(strUtf16, -1);
	}

	String16::String16(const char16_t* strUtf16, sl_reg length) noexcept
	{
		m_container = _priv_String16_create(strUtf16, length);
	}

	Atomic<String16>::Atomic(const char16_t* strUtf16) noexcept
	{
		m_container = _priv_String16_create(strUtf16, -1);
	}

	Atomic<String16>::Atomic(const char16_t* strUtf16, sl_reg length) noexcept
	{
		m_container = _priv_String16_create(strUtf16, length);
	}


	String::String(const char32_t* strUtf32) noexcept
	{
		m_container = _priv_String_create(strUtf32, -1);
	}

	String::String(const char32_t* strUtf32, sl_reg length) noexcept
	{
		m_container = _priv_String_create(strUtf32, length);
	}

	Atomic<String>::Atomic(const char32_t* strUtf32) noexcept
	{
		m_container = _priv_String_create(strUtf32, -1);
	}

	Atomic<String>::Atomic(const char32_t* strUtf32, sl_reg length) noexcept
	{
		m_container = _priv_String_create(strUtf32, length);
	}

	String16::String16(const char32_t* strUtf32) noexcept
	{
		m_container = _priv_String16_create(strUtf32, -1);
	}

	String16::String16(const char32_t* strUtf32, sl_reg length) noexcept
	{
		m_container = _priv_String16_create(strUtf32, length);
	}

	Atomic<String16>::Atomic(const char32_t* strUtf32) noexcept
	{
		m_container = _priv_String16_create(strUtf32, -1);
	}

	Atomic<String16>::Atomic(const char32_t* strUtf32, sl_reg length) noexcept
	{
		m_container = _priv_String16_create(strUtf32, length);
	}
	
	
	String::String(const std::string& str) noexcept
	{
		m_container = _priv_String_alloc_std(str);
	}
	
	String::String(std::string&& str) noexcept
	{
		m_container = _priv_String_alloc_std(Move(str));
	}
	
	String16::String16(const std::u16string& str) noexcept
	{
		m_container = _priv_String16_alloc_std(str);
	}
	
	String16::String16(std::u16string&& str) noexcept
	{
		m_container = _priv_String16_alloc_std(Move(str));
	}
	
	Atomic<String>::Atomic(const std::string& str) noexcept
	{
		m_container = _priv_String_alloc_std(str);
	}
	
	Atomic<String>::Atomic(std::string&& str) noexcept
	{
		m_container = _priv_String_alloc_std(Move(str));
	}
	
	Atomic<String16>::Atomic(const std::u16string& str) noexcept
	{
		m_container = _priv_String16_alloc_std(str);
	}

	Atomic<String16>::Atomic(std::u16string&& str) noexcept
	{
		m_container = _priv_String16_alloc_std(Move(str));
	}


	String String::allocate(sl_size len) noexcept
	{
		return _priv_String_alloc(len);
	}

	String16 String16::allocate(sl_size len) noexcept
	{
		return _priv_String16_alloc(len);
	}


	String String::fromStatic(const sl_char8 *sz, sl_reg len) noexcept
	{
		if (sz) {
			if (len < 0) {
				len = Base::getStringLength(sz);
			}
			return _priv_String_alloc_static(sz, len);
		}
		return sl_null;
	}

	String16 String16::fromStatic(const sl_char16 *sz, sl_reg len) noexcept
	{
		if (sz) {
			if (len < 0) {
				len = Base::getStringLength2(sz);
			}
			return _priv_String16_alloc_static(sz, len);
		}
		return sl_null;
	}
	
	String String::fromRef(const Ref<Referable>& ref, const sl_char8* str, sl_reg len) noexcept
	{
		if (str) {
			if (len < 0) {
				len = Base::getStringLength(str);
			}
			return _priv_String_alloc_ref(ref, str, len);
		}
		return sl_null;
	}
	
	String16 String16::fromRef(const Ref<Referable>& ref, const sl_char16* str, sl_reg len) noexcept
	{
		if (str) {
			if (len < 0) {
				len = Base::getStringLength2(str);
			}
			return _priv_String16_alloc_ref(ref, str, len);
		}
		return sl_null;
	}

	String String::fromMemory(const Memory& mem) noexcept
	{
		if (mem.isNull()) {
			return String::null();
		}
		sl_char8* s = (sl_char8*)(mem.getData());
		sl_size n = mem.getSize();
		if (n == 0) {
			return String::getEmpty();
		}
		if (s[n-1] == 0) {
			n--;
		}
		return fromRef(mem.ref, s, n);
	}
	
	String16 String16::fromMemory(const Memory& mem) noexcept
	{
		if (mem.isNull()) {
			return String16::null();
		}
		sl_char16* s = (sl_char16*)(mem.getData());
		sl_size n = mem.getSize() >> 1;
		if (n == 0) {
			return String16::getEmpty();
		}
		if (s[n-1] == 0) {
			n--;
		}
		return fromRef(mem.ref, s, n);
	}
	
	String String::fromUtf8(const void* utf8, sl_reg len) noexcept
	{
		return String((const sl_char8*)utf8, len);
	}

	String String::fromUtf8(const Memory& mem) noexcept
	{
		return fromUtf8(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf8(const void* utf8, sl_reg len) noexcept
	{
		return String16((const sl_char8*)utf8, len);
	}

	String16 String16::fromUtf8(const Memory& mem) noexcept
	{
		return fromUtf8(mem.getData(), mem.getSize());
	}


	String String::fromUtf16(const sl_char16* utf16, sl_reg len) noexcept
	{
		return String(utf16, len);
	}

	String16 String16::fromUtf16(const sl_char16* utf16, sl_reg len) noexcept
	{
		String16 str(utf16, len);
		return str;
	}


	String String::fromUtf32(const sl_char32* utf32, sl_reg len) noexcept
	{
		return String(utf32, len);
	}

	String16 String16::fromUtf32(const sl_char32* utf32, sl_reg len) noexcept
	{
		String16 str(utf32, len);
		return str;
	}


	String String::fromUtf16BE(const void* _utf16, sl_reg len) noexcept
	{
		if (!_utf16) {
			return sl_null;
		}
		if (len == 0) {
			return String::getEmpty();
		}
		if (((((sl_reg)(_utf16)) & 1) == 0) && Endian::isBE()) {
			return String((sl_char16*)_utf16, len);
		}
		SLIB_SCOPED_BUFFER(sl_char16, 4096, utf16, len);
		if (utf16) {
			Base::copyMemory(utf16, _utf16, len<<1);
			if (Endian::isLE()) {
				for (sl_reg i = 0; i < len; i++) {
					utf16[i] = Endian::swap16(utf16[i]);
				}
			}
			return String(utf16, len);
		}
		return sl_null;
	}

	String String::fromUtf16BE(const Memory& mem) noexcept
	{
		return fromUtf16BE(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf16BE(const void* _utf16, sl_reg len) noexcept
	{
		if (!_utf16) {
			return sl_null;
		}
		if (len == 0) {
			return String16::getEmpty();
		}
		if (((((sl_reg)(_utf16)) & 1) == 0) && Endian::isBE()) {
			return String16((sl_char16*)_utf16, len);
		}
		String16 str = String16::allocate(len);
		if (str.isNotNull()) {
			sl_char16* utf16 = str.getData();
			const sl_uint8* c = (const sl_uint8*)_utf16;
			for (sl_reg i = 0; i < len; i++) {
				utf16[i] = MIO::readUint16BE(c);
				c += 2;
			}
			return str;
		}
		return sl_null;
	}

	String16 String16::fromUtf16BE(const Memory& mem) noexcept
	{
		return fromUtf16BE(mem.getData(), mem.getSize());
	}


	String String::fromUtf16LE(const void* _utf16, sl_reg len) noexcept
	{
		if (!_utf16) {
			return sl_null;
		}
		if (len == 0) {
			return String::getEmpty();
		}
		if ((((sl_reg)(_utf16)) & 1) == 0 && Endian::isLE()) {
			return String((sl_char16*)_utf16, len);
		}
		SLIB_SCOPED_BUFFER(sl_char16, 4096, utf16, len);
		if (utf16) {
			Base::copyMemory(utf16, _utf16, len<<1);
			if (Endian::isBE()) {
				for (sl_reg i = 0; i < len; i++) {
					utf16[i] = Endian::swap16(utf16[i]);
				}
			}
			return String(utf16, len);
		}
		return sl_null;
	}

	String String::fromUtf16LE(const Memory& mem) noexcept
	{
		return fromUtf16LE(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf16LE(const void* _utf16, sl_reg len) noexcept
	{
		if (!_utf16) {
			return sl_null;
		}
		if (len == 0) {
			return String16::getEmpty();
		}
		if ((((sl_reg)(_utf16)) & 1) == 0 && Endian::isLE()) {
			return String16((sl_char16*)_utf16, len);
		}

		String16 str = String16::allocate(len);
		if (str.isNotNull()) {
			sl_char16* utf16 = str.getData();
			const sl_uint8* c = (const sl_uint8*)_utf16;
			for (sl_reg i = 0; i < len; i++) {
				utf16[i] = MIO::readUint16LE(c);
				c += 2;
			}
			return str;
		}
		return sl_null;
	}

	String16 String16::fromUtf16LE(const Memory& mem) noexcept
	{
		return fromUtf16LE(mem.getData(), mem.getSize());
	}


	String String::fromUtf(const void* _buf, sl_size len) noexcept
	{
		sl_char8* buf = (sl_char8*)_buf;
		if (!buf) {
			return sl_null;
		}
		if (len == 0) {
			return String::getEmpty();
		}
		if (len >= 2) {
			if (buf[0] == (sl_char8)0xFF && buf[1] == (sl_char8)0xFE) {
				return String::fromUtf16LE(buf, (len - 2) >> 1);
			}
			if (buf[0] == (sl_char8)0xFE && buf[1] == (sl_char8)0xFF) {
				return String::fromUtf16BE(buf, (len - 2) >> 1);
			}
		}
		if (len >= 3) {
			if (buf[0] == (sl_char8)0xEF && buf[1] == (sl_char8)0xBB && buf[2] == (sl_char8)0xBF) {
				return String(buf, len - 3);
			}
		}
		return String(buf, len);
	}

	String String::fromUtf(const Memory& mem) noexcept
	{
		return fromUtf(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf(const void* _buf, sl_size len) noexcept
	{
		sl_char8* buf = (sl_char8*)_buf;
		if (!buf) {
			return sl_null;
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

	String16 String16::fromUtf(const Memory& mem) noexcept
	{
		return fromUtf(mem.getData(), mem.getSize());
	}


	sl_bool Atomic<String>::isEmpty() const noexcept
	{
		String s(*this);
		return s.getLength() == 0;
	}

	sl_bool Atomic<String16>::isEmpty() const noexcept
	{
		String16 s(*this);
		return s.getLength() == 0;
	}


	sl_bool Atomic<String>::isNotEmpty() const noexcept
	{
		String s(*this);
		return s.getLength() != 0;
	}

	sl_bool Atomic<String16>::isNotEmpty() const noexcept
	{
		String16 s(*this);
		return s.getLength() != 0;
	}


	void String::setNull() noexcept
	{
		_replaceContainer(sl_null);
	}

	void String16::setNull() noexcept
	{
		_replaceContainer(sl_null);
	}

	void Atomic<String>::setNull() noexcept
	{
		if (m_container) {
			_replaceContainer(sl_null);
		}
	}

	void Atomic<String16>::setNull() noexcept
	{
		if (m_container) {
			_replaceContainer(sl_null);
		}
	}


	void String::setEmpty() noexcept
	{
		if (m_container != _priv_String_Empty.container) {
			_replaceContainer(_priv_String_Empty.container);
		}
	}

	void String16::setEmpty() noexcept
	{
		if (m_container != _priv_String16_Empty.container) {
			_replaceContainer(_priv_String16_Empty.container);
		}
	}

	void Atomic<String>::setEmpty() noexcept
	{
		if (m_container != _priv_String_Empty.container) {
			_replaceContainer(_priv_String_Empty.container);
		}
	}

	void Atomic<String16>::setEmpty() noexcept
	{
		if (m_container != _priv_String16_Empty.container) {
			_replaceContainer(_priv_String16_Empty.container);
		}
	}


	sl_size Atomic<String>::getLength() const noexcept
	{
		String s(*this);
		return s.getLength();
	}

	sl_size Atomic<String16>::getLength() const noexcept
	{
		String16 s(*this);
		return s.getLength();
	}


	void String::setLength(sl_size len) noexcept
	{
		if (m_container && m_container != &_g_string8_empty_container) {
			m_container->len = len;
		}
	}

	void String16::setLength(sl_size len) noexcept
	{
		if (m_container && m_container != &_g_string16_empty_container) {
			m_container->len = len;
		}
	}
	
	
	template <class CT>
	SLIB_INLINE static sl_size _priv_String_calcHash(const CT* buf, sl_size len) noexcept
	{
		sl_size hash = 0;
		for (sl_size i = 0; i < len; i++) {
			sl_uint32 ch = buf[i];
			hash = hash * 31 + ch;
		}
		hash = Rehash(hash);
		return hash;
	}
	
	sl_size String::getHashCode() const noexcept
	{
		if (m_container) {
			sl_size n = m_container->len;
			if (n > 0) {
				sl_size hash = m_container->hash;
				if (hash == 0) {
					hash = _priv_String_calcHash(m_container->sz, n);
					m_container->hash = hash;
				}
				return hash;
			}
		}
		return 0;
	}
	
	sl_size String16::getHashCode() const noexcept
	{
		if (m_container) {
			sl_size n = m_container->len;
			if (n > 0) {
				sl_size hash = m_container->hash;
				if (hash == 0) {
					hash = _priv_String_calcHash(m_container->sz, n);
					m_container->hash = hash;
				}
				return hash;
			}
		}
		return 0;
	}
	
	sl_size Atomic<String>::getHashCode() const noexcept
	{
		String s(*this);
		return s.getHashCode();
	}
	
	sl_size Atomic<String16>::getHashCode() const noexcept
	{
		String16 s(*this);
		return s.getHashCode();
	}

	
	void String::setHashCode(sl_size hash) noexcept
	{
		if (m_container && m_container != &_g_string8_empty_container) {
			m_container->hash = hash;
		}
	}

	void String16::setHashCode(sl_size hash) noexcept
	{
		if (m_container && m_container != &_g_string16_empty_container) {
			m_container->hash = hash;
		}
	}


	template <class CT>
	SLIB_INLINE static sl_size _priv_String_calcHashIgnoreCase(const CT* buf, sl_size len) noexcept
	{
		sl_size hash = 0;
		for (sl_size i = 0; i < len; i++) {
			sl_uint32 ch = buf[i];
			ch = SLIB_CHAR_LOWER_TO_UPPER(ch);
			hash = hash * 31 + ch;
		}
		hash = Rehash(hash);
		return hash;
	}
	
	sl_size String::getHashCodeIgnoreCase() const noexcept
	{
		if (m_container) {
			sl_size n = m_container->len;
			if (n > 0) {
				sl_size hash = _priv_String_calcHashIgnoreCase(m_container->sz, n);
				return hash;
			}
		}
		return 0;
	}
	
	sl_size String16::getHashCodeIgnoreCase() const noexcept
	{
		if (m_container) {
			sl_size n = m_container->len;
			if (n > 0) {
				sl_size hash = _priv_String_calcHashIgnoreCase(m_container->sz, n);
				return hash;
			}
		}
		return 0;
	}

	sl_size Atomic<String>::getHashCodeIgnoreCase() const noexcept
	{
		String s(*this);
		return s.getHashCodeIgnoreCase();
	}

	sl_size Atomic<String16>::getHashCodeIgnoreCase() const noexcept
	{
		String16 s(*this);
		return s.getHashCodeIgnoreCase();
	}


	sl_char8 String::getAt(sl_reg index) const noexcept
	{
		if (m_container) {
			if (index >= 0 && index < (sl_reg)(m_container->len)) {
				return m_container->sz[index];
			}
		}
		return 0;
	}

	sl_char16 String16::getAt(sl_reg index) const noexcept
	{
		if (m_container) {
			if (index >= 0 && index < (sl_reg)(m_container->len)) {
				return m_container->sz[index];
			}
		}
		return 0;
	}


	sl_bool String::setAt(sl_reg index, sl_char8 ch) noexcept
	{
		if (m_container) {
			if (index >= 0 && index < (sl_reg)(m_container->len)) {
				m_container->sz[index] = ch;
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool String16::setAt(sl_reg index, sl_char16 ch) noexcept
	{
		if (m_container) {
			if (index >= 0 && index < (sl_reg)(m_container->len)) {
				m_container->sz[index] = ch;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	
	std::string String::toStd() const noexcept
	{
		if (m_container) {
			if (m_container->type == STRING_CONTAINER_TYPE_STD) {
				return (static_cast<_priv_StringContainer_std*>(m_container))->str;
			} else {
				return std::string(m_container->sz, m_container->len);
			}
		}
		return std::string();
	}
	
	std::u16string String16::toStd() const noexcept
	{
		if (m_container) {
			if (m_container->type == STRING_CONTAINER_TYPE_STD) {
				return (static_cast<_priv_StringContainer16_std*>(m_container))->str;
			} else {
				return std::u16string(m_container->sz, m_container->len);
			}
		}
		return std::u16string();
	}

	std::string Atomic<String>::toStd() const noexcept
	{
		String s(*this);
		return s.toStd();
	}
	
	std::u16string Atomic<String16>::toStd() const noexcept
	{
		String16 s(*this);
		return s.toStd();
	}


	String& String::operator=(sl_null_t) noexcept
	{
		_replaceContainer(sl_null);
		return *this;
	}

	String16& String16::operator=(sl_null_t) noexcept
	{
		_replaceContainer(sl_null);
		return *this;
	}

	AtomicString& Atomic<String>::operator=(sl_null_t) noexcept
	{
		if (m_container) {
			_replaceContainer(sl_null);
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(sl_null_t) noexcept
	{
		if (m_container) {
			_replaceContainer(sl_null);
		}
		return *this;
	}


	String& String::operator=(String&& other) noexcept
	{
		if (this != &other) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}

	String16& String16::operator=(String16&& other) noexcept
	{
		if (this != &other) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(AtomicString&& other) noexcept
	{
		if (this != &other) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(AtomicString16&& other) noexcept
	{
		if (this != &other) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}


	String& String::operator=(AtomicString&& other) noexcept
	{
		if ((void*)this != (void*)(&other)) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}

	String16& String16::operator=(AtomicString16&& other) noexcept
	{
		if ((void*)this != (void*)(&other)) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(String&& other) noexcept
	{
		if ((void*)this != (void*)(&other)) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(String16&& other) noexcept
	{
		if ((void*)this != (void*)(&other)) {
			_replaceContainer(other.m_container);
			other.m_container = sl_null;
		}
		return *this;
	}


	String& String::operator=(const String& other) noexcept
	{
		StringContainer* container = other.m_container;
		if (m_container != container) {
			if (container) {
				container->increaseReference();
			}
			_replaceContainer(container);
		}
		return *this;
	}

	String16& String16::operator=(const String16& other) noexcept
	{
		StringContainer16* container = other.m_container;
		if (m_container != container) {
			if (container) {
				container->increaseReference();
			}
			_replaceContainer(container);
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const String& other) noexcept
	{
		StringContainer* container = other.m_container;
		if (m_container != container) {
			if (container) {
				container->increaseReference();
			}
			_replaceContainer(container);
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const String16& other) noexcept
	{
		StringContainer16* container = other.m_container;
		if (m_container != container) {
			if (container) {
				container->increaseReference();
			}
			_replaceContainer(container);
		}
		return *this;
	}


	String& String::operator=(const AtomicString& other) noexcept
	{
		if (m_container != other.m_container) {
			_replaceContainer(other._retainContainer());
		}
		return *this;
	}

	String16& String16::operator=(const AtomicString16& other) noexcept
	{
		if (m_container != other.m_container) {
			_replaceContainer(other._retainContainer());
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const AtomicString& other) noexcept
	{
		if (m_container != other.m_container) {
			_replaceContainer(other._retainContainer());
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const AtomicString16& other) noexcept
	{
		if (m_container != other.m_container) {
			_replaceContainer(other._retainContainer());
		}
		return *this;
	}


	String& String::operator=(const String16& other) noexcept
	{
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String_create(other.getData(), len));
			}
		}
		return *this;
	}

	String16& String16::operator=(const String& other) noexcept
	{
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String16_create(other.getData(), len));
			}
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const String16& other) noexcept
	{
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String_create(other.getData(), len));
			}
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const String& other) noexcept
	{
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String16_create(other.getData(), len));
			}
		}
		return *this;
	}


	String& String::operator=(const AtomicString16& _other) noexcept
	{
		String16 other(_other);
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String_create(other.getData(), len));
			}
		}
		return *this;
	}

	String16& String16::operator=(const AtomicString& _other) noexcept
	{
		String other(_other);
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String16_create(other.getData(), len));
			}
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const AtomicString16& _other) noexcept
	{
		String16 other(_other);
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String_create(other.getData(), len));
			}
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const AtomicString& _other) noexcept
	{
		String other(_other);
		if (other.isNull()) {
			setNull();
		} else {
			sl_size len = other.getLength();
			if (len == 0) {
				setEmpty();
			} else {
				_replaceContainer(_priv_String16_create(other.getData(), len));
			}
		}
		return *this;
	}


	String& String::operator=(const char* utf8) noexcept
	{
		if (utf8) {
			_replaceContainer(_priv_String_create(utf8, -1));
		} else {
			setNull();
		}
		return *this;
	}

	String16& String16::operator=(const char* utf8) noexcept
	{
		if (utf8) {
			_replaceContainer(_priv_String16_create(utf8, -1));
		} else {
			setNull();
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const char* utf8) noexcept
	{
		if (utf8) {
			_replaceContainer(_priv_String_create(utf8, -1));
		} else {
			setNull();
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const char* utf8) noexcept
	{
		if (utf8) {
			_replaceContainer(_priv_String16_create(utf8, -1));
		} else {
			setNull();
		}
		return *this;
	}
	
	
	String& String::operator=(const wchar_t* sz) noexcept
	{
		if (sz) {
			if (sizeof(wchar_t) == 2) {
				_replaceContainer(_priv_String_create((sl_char16*)sz, -1));
			} else {
				_replaceContainer(_priv_String_create((sl_char32*)sz, -1));
			}
		} else {
			setNull();
		}
		return *this;
	}
	
	String16& String16::operator=(const wchar_t* sz) noexcept
	{
		if (sz) {
			if (sizeof(wchar_t) == 2) {
				_replaceContainer(_priv_String16_create((sl_char16*)sz, -1));
			} else {
				_replaceContainer(_priv_String16_create((sl_char32*)sz, -1));
			}
		} else {
			setNull();
		}
		return *this;
	}
	
	AtomicString& Atomic<String>::operator=(const wchar_t* sz) noexcept
	{
		if (sz) {
			if (sizeof(wchar_t) == 2) {
				_replaceContainer(_priv_String_create((sl_char16*)sz, -1));
			} else {
				_replaceContainer(_priv_String_create((sl_char32*)sz, -1));
			}
		} else {
			setNull();
		}
		return *this;
	}
	
	AtomicString16& Atomic<String16>::operator=(const wchar_t* sz) noexcept
	{
		if (sz) {
			if (sizeof(wchar_t) == 2) {
				_replaceContainer(_priv_String16_create((sl_char16*)sz, -1));
			} else {
				_replaceContainer(_priv_String16_create((sl_char32*)sz, -1));
			}
		} else {
			setNull();
		}
		return *this;
	}
	

	String& String::operator=(const char16_t* utf16) noexcept
	{
		if (utf16) {
			_replaceContainer(_priv_String_create(utf16, -1));
		} else {
			setNull();
		}
		return *this;
	}

	String16& String16::operator=(const char16_t* utf16) noexcept
	{
		if (utf16) {
			_replaceContainer(_priv_String16_create(utf16, -1));
		} else {
			setNull();
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const char16_t* utf16) noexcept
	{
		if (utf16) {
			_replaceContainer(_priv_String_create(utf16, -1));
		} else {
			setNull();
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const char16_t* utf16) noexcept
	{
		if (utf16) {
			_replaceContainer(_priv_String16_create(utf16, -1));
		} else {
			setNull();
		}
		return *this;
	}


	String& String::operator=(const char32_t* utf32) noexcept
	{
		if (utf32) {
			_replaceContainer(_priv_String_create(utf32, -1));
		} else {
			setNull();
		}
		return *this;
	}

	String16& String16::operator=(const char32_t* utf32) noexcept
	{
		if (utf32) {
			_replaceContainer(_priv_String16_create(utf32, -1));
		} else {
			setNull();
		}
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const char32_t* utf32) noexcept
	{
		if (utf32) {
			_replaceContainer(_priv_String_create(utf32, -1));
		} else {
			setNull();
		}
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const char32_t* utf32) noexcept
	{
		if (utf32) {
			_replaceContainer(_priv_String16_create(utf32, -1));
		} else {
			setNull();
		}
		return *this;
	}

	
	String& String::operator=(const std::string& str) noexcept
	{
		_replaceContainer(_priv_String_alloc_std(str));
		return *this;
	}
	
	String& String::operator=(std::string&& str) noexcept
	{
		_replaceContainer(_priv_String_alloc_std(Move(str)));
		return *this;
	}
	
	String16& String16::operator=(const std::u16string& str) noexcept
	{
		_replaceContainer(_priv_String16_alloc_std(str));
		return *this;
	}
	
	String16& String16::operator=(std::u16string&& str) noexcept
	{
		_replaceContainer(_priv_String16_alloc_std(Move(str)));
		return *this;
	}
	
	AtomicString& Atomic<String>::operator=(const std::string& str) noexcept
	{
		_replaceContainer(_priv_String_alloc_std(str));
		return *this;
	}
	
	AtomicString& Atomic<String>::operator=(std::string&& str) noexcept
	{
		_replaceContainer(_priv_String_alloc_std(Move(str)));
		return *this;
	}
	
	AtomicString16& Atomic<String16>::operator=(const std::u16string& str) noexcept
	{
		_replaceContainer(_priv_String16_alloc_std(str));
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(std::u16string&& str) noexcept
	{
		_replaceContainer(_priv_String16_alloc_std(Move(str)));
		return *this;
	}
	
	
	String String::operator+(const String& other) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String_merge8(getData(), n, other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String& String::operator+=(const String& other) noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String_merge8(getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 String16::operator+(const String16& other) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String16_merge16(getData(), n, other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String16& String16::operator+=(const String16& other) noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String16_merge16(getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}

	String Atomic<String>::operator+(const String& other) const noexcept
	{
		String s(*this);
		sl_size n = s.getLength();
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String_merge8(s.getData(), n, other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const String& other) noexcept
	{
		String s(*this);
		sl_size n = s.getLength();
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String_merge8(s.getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const String16& other) const noexcept
	{
		String16 s(*this);
		sl_size n = s.getLength();
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String16_merge16(s.getData(), n, other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const String16& other) noexcept
	{
		String16 s(*this);
		sl_size n = s.getLength();
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String16_merge16(s.getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}


	String String::operator+(const String16& other) const noexcept
	{
		if (other.isNotEmpty()) {
			return _priv_String_merge16(getData(), getLength(), other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String& String::operator+=(const String16& other) noexcept
	{
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String_merge16(getData(), getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 String16::operator+(const String& other) const noexcept
	{
		if (other.isNotEmpty()) {
			return _priv_String16_merge8(getData(), getLength(), other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String16& String16::operator+=(const String& other) noexcept
	{
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String16_merge8(getData(), getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}

	String Atomic<String>::operator+(const String16& other) const noexcept
	{
		String s(*this);
		if (other.isNotEmpty()) {
			return _priv_String_merge16(s.getData(), s.getLength(), other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const String16& other) noexcept
	{
		if (other.isNotEmpty()) {
			String s(*this);
			_replaceContainer(_priv_String_merge16(s.getData(), s.getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const String& other) const noexcept
	{
		String16 s(*this);
		if (other.isNotEmpty()) {
			return _priv_String16_merge8(s.getData(), s.getLength(), other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const String& other) noexcept
	{
		if (other.isNotEmpty()) {
			String16 s(*this);
			_replaceContainer(_priv_String16_merge8(s.getData(), s.getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}


	String String::operator+(const AtomicString& _other) const noexcept
	{
		sl_size n = getLength();
		String other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String_merge8(getData(), n, other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String& String::operator+=(const AtomicString& _other) noexcept
	{
		sl_size n = getLength();
		String other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String_merge8(getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 String16::operator+(const AtomicString16& _other) const noexcept
	{
		sl_size n = getLength();
		String16 other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String16_merge16(getData(), n, other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String16& String16::operator+=(const AtomicString16& _other) noexcept
	{
		sl_size n = getLength();
		String16 other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String16_merge16(getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}

	String Atomic<String>::operator+(const AtomicString& _other) const noexcept
	{
		String s(*this);
		sl_size n = s.getLength();
		String other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String_merge8(s.getData(), n, other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const AtomicString& _other) noexcept
	{
		String s(*this);
		sl_size n = s.getLength();
		String other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String_merge8(s.getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const AtomicString16& _other) const noexcept
	{
		String16 s(*this);
		sl_size n = s.getLength();
		String16 other(_other);
		if (n == 0) {
			return other.getNotNull();
		}
		if (other.isNotEmpty()) {
			return _priv_String16_merge16(s.getData(), n, other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const AtomicString16& _other) noexcept
	{
		String16 s(*this);
		sl_size n = s.getLength();
		String16 other(_other);
		if (n == 0) {
			return (*this = other.getNotNull());
		}
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String16_merge16(s.getData(), n, other.getData(), other.getLength()));
		}
		return *this;
	}


	String String::operator+(const AtomicString16& _other) const noexcept
	{
		String16 other(_other);
		if (other.isNotEmpty()) {
			return _priv_String_merge16(getData(), getLength(), other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String& String::operator+=(const AtomicString16& _other) noexcept
	{
		String16 other(_other);
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String_merge16(getData(), getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 String16::operator+(const AtomicString& _other) const noexcept
	{
		String other(_other);
		if (other.isNotEmpty()) {
			return _priv_String16_merge8(getData(), getLength(), other.getData(), other.getLength());
		}
		return getNotNull();
	}

	String16& String16::operator+=(const AtomicString& _other) noexcept
	{
		String other(_other);
		if (other.isNotEmpty()) {
			_replaceContainer(_priv_String16_merge8(getData(), getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}

	String Atomic<String>::operator+(const AtomicString16& _other) const noexcept
	{
		String s(*this);
		String16 other(_other);
		if (other.isNotEmpty()) {
			return _priv_String_merge16(s.getData(), s.getLength(), other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const AtomicString16& _other) noexcept
	{
		String16 other(_other);
		if (other.isNotEmpty()) {
			String s(*this);
			_replaceContainer(_priv_String_merge16(s.getData(), s.getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const AtomicString& _other) const noexcept
	{
		String16 s(*this);
		String other(_other);
		if (other.isNotEmpty()) {
			return _priv_String16_merge8(s.getData(), s.getLength(), other.getData(), other.getLength());
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const AtomicString& _other) noexcept
	{
		String other(_other);
		if (other.isNotEmpty()) {
			String16 s(*this);
			_replaceContainer(_priv_String16_merge8(s.getData(), s.getLength(), other.getData(), other.getLength()));
		}
		return *this;
	}


	String String::operator+(const sl_char8* utf8) const noexcept
	{
		if (utf8) {
			return _priv_String_merge8(getData(), getLength(), utf8, -1);
		}
		return getNotNull();
	}

	String& String::operator+=(const sl_char8* utf8) noexcept
	{
		if (utf8) {
			_replaceContainer(_priv_String_merge8(getData(), getLength(), utf8, -1));
		}
		return *this;
	}

	String16 String16::operator+(const sl_char8* utf8) const noexcept
	{
		if (utf8) {
			return _priv_String16_merge8(getData(), getLength(), utf8, -1);
		}
		return getNotNull();
	}

	String16& String16::operator+=(const sl_char8* utf8) noexcept
	{
		if (utf8) {
			_replaceContainer(_priv_String16_merge8(getData(), getLength(), utf8, -1));
		}
		return *this;
	}

	String Atomic<String>::operator+(const sl_char8* utf8) const noexcept
	{
		String s(*this);
		if (utf8) {
			return _priv_String_merge8(s.getData(), s.getLength(), utf8, -1);
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const sl_char8* utf8) noexcept
	{
		if (utf8) {
			String s(*this);
			_replaceContainer(_priv_String_merge8(s.getData(), s.getLength(), utf8, -1));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const sl_char8* utf8) const noexcept
	{
		String16 s(*this);
		if (utf8) {
			return _priv_String16_merge8(s.getData(), s.getLength(), utf8, -1);
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const sl_char8* utf8) noexcept
	{
		if (utf8) {
			String16 s(*this);
			_replaceContainer(_priv_String16_merge8(s.getData(), s.getLength(), utf8, -1));
		}
		return *this;
	}


	String String::operator+(const sl_char16* utf16) const noexcept
	{
		if (utf16) {
			return _priv_String_merge16(getData(), getLength(), utf16, -1);
		}
		return getNotNull();
	}

	String& String::operator+=(const sl_char16* utf16) noexcept
	{
		if (utf16) {
			_replaceContainer(_priv_String_merge16(getData(), getLength(), utf16, -1));
		}
		return *this;
	}

	String16 String16::operator+(const sl_char16* utf16) const noexcept
	{
		if (utf16) {
			return _priv_String16_merge16(getData(), getLength(), utf16, -1);
		}
		return getNotNull();
	}

	String16& String16::operator+=(const sl_char16* utf16) noexcept
	{
		if (utf16) {
			_replaceContainer(_priv_String16_merge16(getData(), getLength(), utf16, -1));
		}
		return *this;
	}

	String Atomic<String>::operator+(const sl_char16* utf16) const noexcept
	{
		String s(*this);
		if (utf16) {
			return _priv_String_merge16(s.getData(), s.getLength(), utf16, -1);
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const sl_char16* utf16) noexcept
	{
		if (utf16) {
			String s(*this);
			_replaceContainer(_priv_String_merge16(s.getData(), s.getLength(), utf16, -1));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const sl_char16* utf16) const noexcept
	{
		String16 s(*this);
		if (utf16) {
			return _priv_String16_merge16(s.getData(), s.getLength(), utf16, -1);
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const sl_char16* utf16) noexcept
	{
		if (utf16) {
			String16 s(*this);
			_replaceContainer(_priv_String16_merge16(s.getData(), s.getLength(), utf16, -1));
		}
		return *this;
	}


	String String::operator+(const sl_char32* utf32) const noexcept
	{
		if (utf32) {
			return _priv_String_merge32(getData(), getLength(), utf32, -1);
		}
		return getNotNull();
	}

	String& String::operator+=(const sl_char32* utf32) noexcept
	{
		if (utf32) {
			_replaceContainer(_priv_String_merge32(getData(), getLength(), utf32, -1));
		}
		return *this;
	}

	String16 String16::operator+(const sl_char32* utf32) const noexcept
	{
		if (utf32) {
			return _priv_String16_merge32(getData(), getLength(), utf32, -1);
		}
		return getNotNull();
	}

	String16& String16::operator+=(const sl_char32* utf32) noexcept
	{
		if (utf32) {
			_replaceContainer(_priv_String16_merge32(getData(), getLength(), utf32, -1));
		}
		return *this;
	}

	String Atomic<String>::operator+(const sl_char32* utf32) const noexcept
	{
		String s(*this);
		if (utf32) {
			return _priv_String_merge32(s.getData(), s.getLength(), utf32, -1);
		}
		return s.getNotNull();
	}

	AtomicString& Atomic<String>::operator+=(const sl_char32* utf32) noexcept
	{
		if (utf32) {
			String s(*this);
			_replaceContainer(_priv_String_merge32(s.getData(), s.getLength(), utf32, -1));
		}
		return *this;
	}

	String16 Atomic<String16>::operator+(const sl_char32* utf32) const noexcept
	{
		String16 s(*this);
		if (utf32) {
			return _priv_String16_merge32(s.getData(), s.getLength(), utf32, -1);
		}
		return s.getNotNull();
	}

	AtomicString16& Atomic<String16>::operator+=(const sl_char32* utf32) noexcept
	{
		if (utf32) {
			String16 s(*this);
			_replaceContainer(_priv_String16_merge32(s.getData(), s.getLength(), utf32, -1));
		}
		return *this;
	}


	String operator+(const sl_char8* utf8, const String& s) noexcept
	{
		if (utf8) {
			return _priv_String_merge8(utf8, -1, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}

	String16 operator+(const sl_char8* utf8, const String16& s) noexcept
	{
		if (utf8) {
			return _priv_String16_merge8(utf8, -1, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}

	String operator+(const sl_char8* utf8, const AtomicString& s) noexcept
	{
		return utf8 + String(s);
	}

	String16 operator+(const sl_char8* utf8, const AtomicString16& s) noexcept
	{
		return utf8 + String16(s);
	}


	String operator+(const sl_char16* utf16, const String& s) noexcept
	{
		if (utf16) {
			return _priv_String_merge16(utf16, -1, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}

	String16 operator+(const sl_char16* utf16, const String16& s) noexcept
	{
		if (utf16) {
			return _priv_String16_merge16(utf16, -1, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}

	String operator+(const sl_char16* utf16, const AtomicString& s) noexcept
	{
		return utf16 + String(s);
	}

	String16 operator+(const sl_char16* utf16, const AtomicString16& s) noexcept
	{
		return utf16 + String16(s);
	}


	String operator+(const sl_char32* utf32, const String& s) noexcept
	{
		if (utf32) {
			return _priv_String_merge32(utf32, -1, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}

	String16 operator+(const sl_char32* utf32, const String16& s) noexcept
	{
		if (utf32) {
			return _priv_String16_merge32(utf32, -1, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}

	String operator+(const sl_char32* utf32, const AtomicString& s) noexcept
	{
		return utf32 + String(s);
	}

	String16 operator+(const sl_char32* utf32, const AtomicString16& s) noexcept
	{
		return utf32 + String16(s);
	}

	
	String String::operator+(const std::string& str) const noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			return _priv_String_merge8(getData(), getLength(), str.c_str(), n);
		}
		return getNotNull();
	}
	
	String& String::operator+=(const std::string& str) noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			_replaceContainer(_priv_String_merge8(getData(), getLength(), str.c_str(), n));
		}
		return *this;
	}
	
	String16 String16::operator+(const std::u16string& str) const noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			return _priv_String16_merge16(getData(), getLength(), str.c_str(), n);
		}
		return getNotNull();
	}
	
	String16& String16::operator+=(const std::u16string& str) noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			_replaceContainer(_priv_String16_merge16(getData(), getLength(), str.c_str(), n));
		}
		return *this;
	}
	
	String Atomic<String>::operator+(const std::string& str) const noexcept
	{
		String s(*this);
		sl_size n = str.length();
		if (n > 0) {
			return _priv_String_merge8(s.getData(), s.getLength(), str.c_str(), n);
		}
		return s.getNotNull();
	}
	
	AtomicString& Atomic<String>::operator+=(const std::string& str) noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			String s(*this);
			_replaceContainer(_priv_String_merge8(s.getData(), s.getLength(), str.c_str(), n));
		}
		return *this;
	}
	
	String16 Atomic<String16>::operator+(const std::u16string& str) const noexcept
	{
		String16 s(*this);
		sl_size n = str.length();
		if (n > 0) {
			return _priv_String16_merge16(s.getData(), s.getLength(), str.c_str(), n);
		}
		return s.getNotNull();
	}
	
	AtomicString16& Atomic<String16>::operator+=(const std::u16string& str) noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			String16 s(*this);
			_replaceContainer(_priv_String16_merge16(s.getData(), s.getLength(), str.c_str(), n));
		}
		return *this;
	}
	
	String operator+(const std::string& str, const String& s) noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			return _priv_String_merge8(str.c_str(), n, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}
	
	String16 operator+(const std::u16string& str, const String16& s) noexcept
	{
		sl_size n = str.length();
		if (n > 0) {
			return _priv_String16_merge16(str.c_str(), n, s.getData(), s.getLength());
		}
		return s.getNotNull();
	}
	
	String operator+(const std::string& str, const AtomicString& s) noexcept
	{
		return str + String(s);
	}
	
	String16 operator+(const std::u16string& str, const AtomicString16& s) noexcept
	{
		return str + String16(s);
	}


	String String::operator+(sl_int32 number) const noexcept
	{
		return *this + String::fromInt32(number);
	}

	String& String::operator+=(sl_int32 number) noexcept
	{
		return (*this += String::fromInt32(number));
	}

	String operator+(sl_int32 number, const String& other) noexcept
	{
		return String::fromInt32(number) + other;
	}

	String16 String16::operator+(sl_int32 number) const noexcept
	{
		return *this + String16::fromInt32(number);
	}

	String16& String16::operator+=(sl_int32 number) noexcept
	{
		return (*this += String16::fromInt32(number));
	}

	String16 operator+(sl_int32 number, const String16& other) noexcept
	{
		return String16::fromInt32(number) + other;
	}

	String Atomic<String>::operator+(sl_int32 number) const noexcept
	{
		return *this + String::fromInt32(number);
	}

	AtomicString& Atomic<String>::operator+=(sl_int32 number) noexcept
	{
		return (*this += String::fromInt32(number));
	}

	String operator+(sl_int32 number, const AtomicString& other) noexcept
	{
		return String::fromInt32(number) + other;
	}

	String16 Atomic<String16>::operator+(sl_int32 number) const noexcept
	{
		return *this + String16::fromInt32(number);
	}

	AtomicString16& Atomic<String16>::operator+=(sl_int32 number) noexcept
	{
		return (*this += String16::fromInt32(number));
	}

	String16 operator+(sl_int32 number, const AtomicString16& other) noexcept
	{
		return String16::fromInt32(number) + other;
	}


	String String::operator+(sl_uint32 number) const noexcept
	{
		return *this + String::fromUint32(number);
	}

	String& String::operator+=(sl_uint32 number) noexcept
	{
		return (*this += String::fromUint32(number));
	}

	String operator+(sl_uint32 number, const String& other) noexcept
	{
		return String::fromUint32(number) + other;
	}

	String16 String16::operator+(sl_uint32 number) const noexcept
	{
		return *this + String16::fromUint32(number);
	}

	String16& String16::operator+=(sl_uint32 number) noexcept
	{
		return (*this += String16::fromUint32(number));
	}

	String16 operator+(sl_uint32 number, const String16& other) noexcept
	{
		return String16::fromUint32(number) + other;
	}

	String Atomic<String>::operator+(sl_uint32 number) const noexcept
	{
		return *this + String::fromUint32(number);
	}

	AtomicString& Atomic<String>::operator+=(sl_uint32 number) noexcept
	{
		return (*this += String::fromUint32(number));
	}

	String operator+(sl_uint32 number, const AtomicString& other) noexcept
	{
		return String::fromUint32(number) + other;
	}

	String16 Atomic<String16>::operator+(sl_uint32 number) const noexcept
	{
		return *this + String16::fromUint32(number);
	}

	AtomicString16& Atomic<String16>::operator+=(sl_uint32 number) noexcept
	{
		return (*this += String16::fromUint32(number));
	}

	String16 operator+(sl_uint32 number, const AtomicString16& other) noexcept
	{
		return String16::fromUint32(number) + other;
	}


	String String::operator+(sl_int64 number) const noexcept
	{
		return *this + String::fromInt64(number);
	}

	String& String::operator+=(sl_int64 number) noexcept
	{
		return (*this += String::fromInt64(number));
	}

	String operator+(sl_int64 number, const String& other) noexcept
	{
		return String::fromInt64(number) + other;
	}

	String16 String16::operator+(sl_int64 number) const noexcept
	{
		return *this + String16::fromInt64(number);
	}

	String16& String16::operator+=(sl_int64 number) noexcept
	{
		return (*this += String16::fromInt64(number));
	}

	String16 operator+(sl_int64 number, const String16& other) noexcept
	{
		return String16::fromInt64(number) + other;
	}

	String Atomic<String>::operator+(sl_int64 number) const noexcept
	{
		return *this + String::fromInt64(number);
	}

	AtomicString& Atomic<String>::operator+=(sl_int64 number) noexcept
	{
		return (*this += String::fromInt64(number));
	}

	String operator+(sl_int64 number, const AtomicString& other) noexcept
	{
		return String::fromInt64(number) + other;
	}

	String16 Atomic<String16>::operator+(sl_int64 number) const noexcept
	{
		return *this + String16::fromInt64(number);
	}

	AtomicString16& Atomic<String16>::operator+=(sl_int64 number) noexcept
	{
		return (*this += String16::fromInt64(number));
	}

	String16 operator+(sl_int64 number, const AtomicString16& other) noexcept
	{
		return String16::fromInt64(number) + other;
	}


	String String::operator+(sl_uint64 number) const noexcept
	{
		return *this + String::fromUint64(number);
	}

	String& String::operator+=(sl_uint64 number) noexcept
	{
		return (*this += String::fromUint64(number));
	}

	String operator+(sl_uint64 number, const String& other) noexcept
	{
		return String::fromUint64(number) + other;
	}

	String16 String16::operator+(sl_uint64 number) const noexcept
	{
		return *this + String16::fromUint64(number);
	}

	String16& String16::operator+=(sl_uint64 number) noexcept
	{
		return (*this += String16::fromUint64(number));
	}

	String16 operator+(sl_uint64 number, const String16& other) noexcept
	{
		return String16::fromUint64(number) + other;
	}

	String Atomic<String>::operator+(sl_uint64 number) const noexcept
	{
		return *this + String::fromUint64(number);
	}

	AtomicString& Atomic<String>::operator+=(sl_uint64 number) noexcept
	{
		return (*this += String::fromUint64(number));
	}

	String operator+(sl_uint64 number, const AtomicString& other) noexcept
	{
		return String::fromUint64(number) + other;
	}

	String16 Atomic<String16>::operator+(sl_uint64 number) const noexcept
	{
		return *this + String16::fromUint64(number);
	}

	AtomicString16& Atomic<String16>::operator+=(sl_uint64 number) noexcept
	{
		return (*this += String16::fromUint64(number));
	}

	String16 operator+(sl_uint64 number, const AtomicString16& other) noexcept
	{
		return String16::fromUint64(number) + other;
	}


	String String::operator+(float number) const noexcept
	{
		return *this + String::fromFloat(number);
	}

	String& String::operator+=(float number) noexcept
	{
		return (*this += String::fromFloat(number));
	}

	String operator+(float number, const String& other) noexcept
	{
		return String::fromFloat(number) + other;
	}

	String16 String16::operator+(float number) const noexcept
	{
		return *this + String16::fromFloat(number);
	}

	String16& String16::operator+=(float number) noexcept
	{
		return (*this += String16::fromFloat(number));
	}

	String16 operator+(float number, const String16& other) noexcept
	{
		return String16::fromFloat(number) + other;
	}

	String Atomic<String>::operator+(float number) const noexcept
	{
		return *this + String::fromFloat(number);
	}

	AtomicString& Atomic<String>::operator+=(float number) noexcept
	{
		return (*this += String::fromFloat(number));
	}

	String operator+(float number, const AtomicString& other) noexcept
	{
		return String::fromFloat(number) + other;
	}

	String16 Atomic<String16>::operator+(float number) const noexcept
	{
		return *this + String16::fromFloat(number);
	}

	AtomicString16& Atomic<String16>::operator+=(float number) noexcept
	{
		return (*this += String16::fromFloat(number));
	}

	String16 operator+(float number, const AtomicString16& other) noexcept
	{
		return String16::fromFloat(number) + other;
	}


	String String::operator+(double number) const noexcept
	{
		return *this + String::fromDouble(number);
	}

	String& String::operator+=(double number) noexcept
	{
		return (*this += String::fromDouble(number));
	}

	String operator+(double number, const String& other) noexcept
	{
		return String::fromDouble(number) + other;
	}

	String16 String16::operator+(double number) const noexcept
	{
		return *this + String16::fromDouble(number);
	}

	String16& String16::operator+=(double number) noexcept
	{
		return (*this += String16::fromDouble(number));
	}

	String16 operator+(double number, const String16& other) noexcept
	{
		return String16::fromDouble(number) + other;
	}

	String Atomic<String>::operator+(double number) const noexcept
	{
		return *this + String::fromDouble(number);
	}

	AtomicString& Atomic<String>::operator+=(double number) noexcept
	{
		return (*this += String::fromDouble(number));
	}

	String operator+(double number, const AtomicString& other) noexcept
	{
		return String::fromDouble(number) + other;
	}

	String16 Atomic<String16>::operator+(double number) const noexcept
	{
		return *this + String16::fromDouble(number);
	}

	AtomicString16& Atomic<String16>::operator+=(double number) noexcept
	{
		return (*this += String16::fromDouble(number));
	}

	String16 operator+(double number, const AtomicString16& other) noexcept
	{
		return String16::fromDouble(number) + other;
	}


	String String::operator+(sl_bool value) const noexcept
	{
		return *this + String::fromBoolean(value);
	}

	String& String::operator+=(sl_bool value) noexcept
	{
		return (*this += String::fromBoolean(value));
	}

	String operator+(sl_bool value, const String& other) noexcept
	{
		return String::fromBoolean(value) + other;
	}

	String16 String16::operator+(sl_bool value) const noexcept
	{
		return *this + String16::fromBoolean(value);
	}

	String16& String16::operator+=(sl_bool value) noexcept
	{
		return (*this += String16::fromBoolean(value));
	}

	String16 operator+(sl_bool value, const String16& other) noexcept
	{
		return String::fromBoolean(value) + other;
	}

	String Atomic<String>::operator+(sl_bool value) const noexcept
	{
		return *this + String::fromBoolean(value);
	}

	AtomicString& Atomic<String>::operator+=(sl_bool value) noexcept
	{
		return (*this += String::fromBoolean(value));
	}

	String operator+(sl_bool value, const AtomicString& other) noexcept
	{
		return String::fromBoolean(value) + other;
	}

	String16 Atomic<String16>::operator+(sl_bool value) const noexcept
	{
		return *this + String16::fromBoolean(value);
	}

	AtomicString16& Atomic<String16>::operator+=(sl_bool value) noexcept
	{
		return (*this += String16::fromBoolean(value));
	}

	String16 operator+(sl_bool value, const AtomicString16& other) noexcept
	{
		return String::fromBoolean(value) + other;
	}


	sl_bool String::equals(const String& other) const noexcept
	{
		sl_char8* s1 = getData();
		sl_char8* s2 = other.getData();
		if (s1 == s2) {
			return sl_true;
		}
		sl_size len = getLength();
		if (len != other.getLength()) {
			return sl_false;
		}
		if (len == 0) {
			return sl_true;
		}
		sl_size h1 = m_container->hash;
		if (h1) {
			sl_size h2 = other.m_container->hash;
			if (h2) {
				if (h1 != h2) {
					return sl_false;
				}
			}
		}
		return Base::equalsMemory(s1, s2, len);
	}

	sl_bool String16::equals(const String16& other) const noexcept
	{
		sl_char16* s1 = getData();
		sl_char16* s2 = other.getData();
		if (s1 == s2) {
			return sl_true;
		}
		sl_size len = getLength();
		if (len != other.getLength()) {
			return sl_false;
		}
		if (len == 0) {
			return sl_true;
		}
		sl_size h1 = m_container->hash;
		if (h1) {
			sl_size h2 = other.m_container->hash;
			if (h2) {
				if (h1 != h2) {
					return sl_false;
				}
			}
		}
		return Base::equalsMemory2((sl_uint16*)s1, (sl_uint16*)s2, len);
	}

	sl_bool Atomic<String>::equals(const String& other) const noexcept
	{
		String s(*this);
		return s.equals(other);
	}

	sl_bool Atomic<String16>::equals(const String16& other) const noexcept
	{
		String16 s(*this);
		return s.equals(other);
	}


	sl_bool String::equals(const String16& other) const noexcept
	{
		return equals(String(other));
	}

	sl_bool String16::equals(const String& other) const noexcept
	{
		return equals(String16(other));
	}

	sl_bool Atomic<String>::equals(const String16& other) const noexcept
	{
		String s(*this);
		return s.equals(other);
	}

	sl_bool Atomic<String16>::equals(const String& other) const noexcept
	{
		String16 s(*this);
		return s.equals(other);
	}


	sl_bool String::equals(const AtomicString& _other) const noexcept
	{
		String other(_other);
		return equals(other);
	}

	sl_bool String16::equals(const AtomicString16& _other) const noexcept
	{
		String16 other(_other);
		return equals(other);
	}

	sl_bool Atomic<String>::equals(const AtomicString& _other) const noexcept
	{
		String s(*this);
		String other(_other);
		return s.equals(other);
	}

	sl_bool Atomic<String16>::equals(const AtomicString16& _other) const noexcept
	{
		String16 s(*this);
		String16 other(_other);
		return s.equals(other);
	}


	sl_bool String::equals(const AtomicString16& _other) const noexcept
	{
		String16 other(_other);
		return equals(other);
	}

	sl_bool String16::equals(const AtomicString& _other) const noexcept
	{
		String other(_other);
		return equals(other);
	}

	sl_bool Atomic<String>::equals(const AtomicString16& _other) const noexcept
	{
		String s(*this);
		String16 other(_other);
		return s.equals(other);
	}

	sl_bool Atomic<String16>::equals(const AtomicString& _other) const noexcept
	{
		String16 s(*this);
		String other(_other);
		return s.equals(other);
	}


	sl_bool String::equals(const sl_char8* utf8) const noexcept
	{
		sl_char8* sz = getData();
		if (sz == utf8) {
			return sl_true;
		}
		return _priv_String_equals8(sz, getLength(), utf8, -1);
	}

	sl_bool String16::equals(const sl_char8* utf8) const noexcept
	{
		return _priv_String16_equals8(getData(), getLength(), utf8, -1);
	}

	sl_bool Atomic<String>::equals(const sl_char8* utf8) const noexcept
	{
		String s(*this);
		sl_char8* sz = s.getData();
		if (sz == utf8) {
			return sl_true;
		}
		return _priv_String_equals8(sz, s.getLength(), utf8, -1);
	}

	sl_bool Atomic<String16>::equals(const sl_char8* utf8) const noexcept
	{
		String16 s(*this);
		return _priv_String16_equals8(s.getData(), s.getLength(), utf8, -1);
	}


	sl_bool String::equals(const sl_char16* utf16) const noexcept
	{
		return _priv_String_equals16(getData(), getLength(), utf16, -1);
	}

	sl_bool String16::equals(const sl_char16* utf16) const noexcept
	{
		sl_char16* sz = getData();
		if (sz == utf16) {
			return sl_true;
		}
		return _priv_String16_equals16(sz, getLength(), utf16, -1);
	}

	sl_bool Atomic<String>::equals(const sl_char16* utf16) const noexcept
	{
		String s(*this);
		return _priv_String_equals16(s.getData(), s.getLength(), utf16, -1);
	}

	sl_bool Atomic<String16>::equals(const sl_char16* utf16) const noexcept
	{
		String16 s(*this);
		sl_char16* sz = s.getData();
		if (sz == utf16) {
			return sl_true;
		}
		return _priv_String16_equals16(sz, s.getLength(), utf16, -1);
	}


	sl_bool String::equals(const sl_char32* utf32) const noexcept
	{
		return _priv_String_equals32(getData(), getLength(), utf32, -1);
	}

	sl_bool String16::equals(const sl_char32* utf32) const noexcept
	{
		return _priv_String16_equals32(getData(), getLength(), utf32, -1);
	}

	sl_bool Atomic<String>::equals(const sl_char32* utf32) const noexcept
	{
		String s(*this);
		return _priv_String_equals32(s.getData(), s.getLength(), utf32, -1);
	}

	sl_bool Atomic<String16>::equals(const sl_char32* utf32) const noexcept
	{
		String16 s(*this);
		return _priv_String16_equals32(s.getData(), s.getLength(), utf32, -1);
	}

	
	sl_bool String::equals(const std::string& str) const noexcept
	{
		const sl_char8* s1 = getData();
		const sl_char8* s2 = str.c_str();
		if (s1 == s2) {
			return sl_true;
		}
		return _priv_String_equals_objects(s1, getLength(), s2, str.length());
	}
	
	sl_bool String16::equals(const std::u16string& str) const noexcept
	{
		const sl_char16* s1 = getData();
		const sl_char16* s2 = str.c_str();
		if (s1 == s2) {
			return sl_true;
		}
		return _priv_String16_equals_objects(s1, getLength(), s2, str.length());
	}
	
	sl_bool Atomic<String>::equals(const std::string& str) const noexcept
	{
		String s(*this);
		return s.equals(str);
	}
	
	sl_bool Atomic<String16>::equals(const std::u16string& str) const noexcept
	{
		String16 s(*this);
		return s.equals(str);
	}

	
	sl_int32 String::compare(const String& other) const noexcept
	{
		sl_char8* s1 = getData();
		sl_char8* s2 = other.getData();
		if (s1 == s2) {
			return 0;
		}
		sl_size len1 = getLength();
		sl_size len2 = other.getLength();
		return _priv_String_compare_objects(s1, len1, s2, len2);
	}

	sl_int32 String16::compare(const String16& other) const noexcept
	{
		sl_char16* s1 = getData();
		sl_char16* s2 = other.getData();
		if (s1 == s2) {
			return 0;
		}
		sl_size len1 = getLength();
		sl_size len2 = other.getLength();
		return _priv_String16_compare_objects(s1, len1, s2, len2);
	}

	sl_int32 Atomic<String>::compare(const String& other) const noexcept
	{
		String s(*this);
		return s.compare(other);
	}

	sl_int32 Atomic<String16>::compare(const String16& other) const noexcept
	{
		String16 s(*this);
		return s.compare(other);
	}


	sl_int32 String::compare(const String16& _other) const noexcept
	{
		String other(_other);
		return compare(other);
	}

	sl_int32 String16::compare(const String& _other) const noexcept
	{
		String16 other(_other);
		return compare(other);
	}

	sl_int32 Atomic<String>::compare(const String16& other) const noexcept
	{
		String s(*this);
		return s.compare(other);
	}

	sl_int32 Atomic<String16>::compare(const String& other) const noexcept
	{
		String16 s(*this);
		return s.compare(other);
	}


	sl_int32 String::compare(const AtomicString& _other) const noexcept
	{
		String other(_other);
		return compare(other);
	}

	sl_int32 String16::compare(const AtomicString16& _other) const noexcept
	{
		String16 other(_other);
		return compare(other);
	}

	sl_int32 Atomic<String>::compare(const AtomicString& _other) const noexcept
	{
		String s(*this);
		String other(_other);
		return s.compare(other);
	}

	sl_int32 Atomic<String16>::compare(const AtomicString16& _other) const noexcept
	{
		String16 s(*this);
		String16 other(_other);
		return s.compare(other);
	}


	sl_int32 String::compare(const AtomicString16& _other) const noexcept
	{
		String16 other(_other);
		return compare(other);
	}

	sl_int32 String16::compare(const AtomicString& _other) const noexcept
	{
		String other(_other);
		return compare(other);
	}

	sl_int32 Atomic<String>::compare(const AtomicString16& _other) const noexcept
	{
		String s(*this);
		String16 other(_other);
		return s.compare(other);
	}

	sl_int32 Atomic<String16>::compare(const AtomicString& _other) const noexcept
	{
		String16 s(*this);
		String other(_other);
		return s.compare(other);
	}


	sl_int32 String::compare(const sl_char8* utf8) const noexcept
	{
		sl_char8* sz = getData();
		if (sz == utf8) {
			return 0;
		}
		return _priv_String_compare8(sz, getLength(), utf8, -1);
	}

	sl_int32 String16::compare(const sl_char8* utf8) const noexcept
	{
		return _priv_String16_compare8(getData(), getLength(), utf8, -1);
	}

	sl_int32 Atomic<String>::compare(const sl_char8* utf8) const noexcept
	{
		String s(*this);
		return s.compare(utf8);
	}

	sl_int32 Atomic<String16>::compare(const sl_char8* utf8) const noexcept
	{
		String16 s(*this);
		return s.compare(utf8);
	}


	sl_int32 String::compare(const sl_char16* utf16) const noexcept
	{
		return _priv_String_compare16(getData(), getLength(), utf16, -1);
	}

	sl_int32 String16::compare(const sl_char16* utf16) const noexcept
	{
		sl_char16* sz = getData();
		if (sz == utf16) {
			return 0;
		}
		return _priv_String16_compare16(sz, getLength(), utf16, -1);
	}

	sl_int32 Atomic<String>::compare(const sl_char16* utf16) const noexcept
	{
		String s(*this);
		return s.compare(utf16);
	}

	sl_int32 Atomic<String16>::compare(const sl_char16* utf16) const noexcept
	{
		String16 s(*this);
		return s.compare(utf16);
	}


	sl_int32 String::compare(const sl_char32* utf32) const noexcept
	{
		return _priv_String_compare32(getData(), getLength(), utf32, -1);
	}

	sl_int32 String16::compare(const sl_char32* utf32) const noexcept
	{
		return _priv_String16_compare32(getData(), getLength(), utf32, -1);
	}

	sl_int32 Atomic<String>::compare(const sl_char32* utf32) const noexcept
	{
		String s(*this);
		return s.compare(utf32);
	}

	sl_int32 Atomic<String16>::compare(const sl_char32* utf32) const noexcept
	{
		String16 s(*this);
		return s.compare(utf32);
	}
	
	
	sl_int32 String::compare(const std::string& str) const noexcept
	{
		const sl_char8* s1 = getData();
		const sl_char8* s2 = str.c_str();
		if (s1 == s2) {
			return 0;
		}
		return _priv_String_compare_objects(s1, getLength(), s2, str.length());
	}
	
	sl_int32 String16::compare(const std::u16string& str) const noexcept
	{
		const sl_char16* s1 = getData();
		const sl_char16* s2 = str.c_str();
		if (s1 == s2) {
			return 0;
		}
		return _priv_String16_compare_objects(s1, getLength(), s2, str.length());
	}
	
	sl_int32 Atomic<String>::compare(const std::string& str) const noexcept
	{
		String s(*this);
		return s.compare(str);
	}
	
	sl_int32 Atomic<String16>::compare(const std::u16string& str) const noexcept
	{
		String16 s(*this);
		return s.compare(str);
	}
	

	sl_int32 String::compare(const String& other, sl_size len) const noexcept
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
		return _priv_String_compare_objects(s1, l1, s2, l2);
	}

	sl_int32 String16::compare(const String16& other, sl_size len) const noexcept
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
		return _priv_String16_compare_objects(s1, l1, s2, l2);
	}

	sl_int32 Atomic<String>::compare(const String& other, sl_size len) const noexcept
	{
		String s(*this);
		return s.compare(other, len);
	}

	sl_int32 Atomic<String16>::compare(const String16& other, sl_size len) const noexcept
	{
		String16 s(*this);
		return s.compare(other, len);
	}


	sl_bool String::equalsIgnoreCase(const String& other) const noexcept
	{
		sl_char8* s1 = getData();
		sl_char8* s2 = other.getData();
		if (s1 == s2) {
			return sl_true;
		}
		sl_size len = getLength();
		if (len != other.getLength()) {
			return sl_false;
		}
		for (sl_size i = 0; i < len; i++) {
			sl_uint8 c1 = s1[i];
			sl_uint8 c2 = s2[i];
			c1 = SLIB_CHAR_LOWER_TO_UPPER(c1);
			c2 = SLIB_CHAR_LOWER_TO_UPPER(c2);
			if (c1 != c2) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool String16::equalsIgnoreCase(const String16& other) const noexcept
	{
		sl_char16* s1 = getData();
		sl_char16* s2 = other.getData();
		if (s1 == s2) {
			return sl_true;
		}
		sl_size len = getLength();
		if (len != other.getLength()) {
			return sl_false;
		}
		for (sl_size i = 0; i < len; i++) {
			sl_uint16 c1 = s1[i];
			sl_uint16 c2 = s2[i];
			c1 = SLIB_CHAR_LOWER_TO_UPPER(c1);
			c2 = SLIB_CHAR_LOWER_TO_UPPER(c2);
			if (c1 != c2) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Atomic<String>::equalsIgnoreCase(const String& other) const noexcept
	{
		String s(*this);
		return s.equalsIgnoreCase(other);
	}

	sl_bool Atomic<String16>::equalsIgnoreCase(const String16& other) const noexcept
	{
		String16 s(*this);
		return s.equalsIgnoreCase(other);
	}


	sl_int32 String::compareIgnoreCase(const String& other) const noexcept
	{
		sl_char8* s1 = getData();
		sl_char8* s2 = other.getData();
		if (s1 == s2) {
			return 0;
		}
		sl_size len1 = getLength();
		sl_size len2 = other.getLength();
		sl_size len = SLIB_MIN(len1, len2);
		for (sl_size i = 0; i < len; i++) {
			sl_uint8 c1 = s1[i];
			sl_uint8 c2 = s2[i];
			c1 = SLIB_CHAR_LOWER_TO_UPPER(c1);
			c2 = SLIB_CHAR_LOWER_TO_UPPER(c2);
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

	sl_int32 String16::compareIgnoreCase(const String16& other) const noexcept
	{
		sl_char16* s1 = getData();
		sl_char16* s2 = other.getData();
		if (s1 == s2) {
			return 0;
		}
		sl_size len1 = getLength();
		sl_size len2 = other.getLength();
		sl_size len = SLIB_MIN(len1, len2);
		for (sl_size i = 0; i < len; i++) {
			sl_uint16 c1 = s1[i];
			sl_uint16 c2 = s2[i];
			c1 = SLIB_CHAR_LOWER_TO_UPPER(c1);
			c2 = SLIB_CHAR_LOWER_TO_UPPER(c2);
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

	sl_int32 Atomic<String>::compareIgnoreCase(const String& other) const noexcept
	{
		String s(*this);
		return s.compareIgnoreCase(other);
	}

	sl_int32 Atomic<String16>::compareIgnoreCase(const String16& other) const noexcept
	{
		String16 s(*this);
		return s.compareIgnoreCase(other);
	}


#define PRIV_DEFINE_STRING_COMPARE_FUNCS(CLASS, OP, BODY, BODY_FRIEND, STD) \
	sl_bool CLASS::OP(const String& other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const String16& other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const AtomicString& other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const AtomicString16& other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const sl_char8* other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const sl_char16* other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const sl_char32* other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool CLASS::OP(const STD& other) const noexcept \
	{ \
		return BODY; \
	} \
	sl_bool OP(const sl_char8* other, const CLASS& s) noexcept \
	{ \
		return BODY_FRIEND; \
	} \
	sl_bool OP(const sl_char16* other, const CLASS& s) noexcept \
	{ \
		return BODY_FRIEND; \
	} \
	sl_bool OP(const sl_char32* other, const CLASS& s) noexcept \
	{ \
		return BODY_FRIEND; \
	} \
	sl_bool OP(const STD& other, const CLASS& s) noexcept \
	{ \
		return BODY_FRIEND; \
	}


	PRIV_DEFINE_STRING_COMPARE_FUNCS(String, operator==, equals(other), s.equals(other), std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String, operator!=, !(equals(other)), !(s.equals(other)), std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String, operator>=, compare(other)>=0, s.compare(other)<=0, std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String, operator<=, compare(other)<=0, s.compare(other)>=0, std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String, operator>, compare(other)>0, s.compare(other)<0, std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String, operator<, compare(other)<0, s.compare(other)>0, std::string)

	PRIV_DEFINE_STRING_COMPARE_FUNCS(String16, operator==, equals(other), s.equals(other), std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String16, operator!=, !(equals(other)), !(s.equals(other)), std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String16, operator>=, compare(other)>=0, s.compare(other)<=0, std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String16, operator<=, compare(other)<=0, s.compare(other)>=0, std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String16, operator>, compare(other)>0, s.compare(other)<0, std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(String16, operator<, compare(other)<0, s.compare(other)>0, std::u16string)

	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString, operator==, equals(other), s.equals(other), std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString, operator!=, !(equals(other)), !(s.equals(other)), std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString, operator>=, compare(other)>=0, s.compare(other)<=0, std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString, operator<=, compare(other)<=0, s.compare(other)>=0, std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString, operator>, compare(other)>0, s.compare(other)<0, std::string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString, operator<, compare(other)<0, s.compare(other)>0, std::string)

	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString16, operator==, equals(other), s.equals(other), std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString16, operator!=, !(equals(other)), !(s.equals(other)), std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString16, operator>=, compare(other)>=0, s.compare(other)<=0, std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString16, operator<=, compare(other)<=0, s.compare(other)>=0, std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString16, operator>, compare(other)>0, s.compare(other)<0, std::u16string)
	PRIV_DEFINE_STRING_COMPARE_FUNCS(AtomicString16, operator<, compare(other)<0, s.compare(other)>0, std::u16string)

	String String::duplicate() const noexcept
	{
		if (isNotNull()) {
			return String(getData(), getLength());
		}
		return sl_null;
	}

	String16 String16::duplicate() const noexcept
	{
		if (isNotNull()) {
			return String16(getData(), getLength());
		}
		return sl_null;
	}

	String Atomic<String>::duplicate() const noexcept
	{
		String s(*this);
		return s.duplicate();
	}

	String16 Atomic<String16>::duplicate() const noexcept
	{
		String16 s(*this);
		return s.duplicate();
	}

	Memory String::toMemory() const noexcept
	{
		return Memory::create(getData(), getLength()*sizeof(sl_char8));
	}

	Memory String::toStaticMemory() const noexcept
	{
		return Memory::createStatic(getData(), getLength()*sizeof(sl_char8));
	}

	Memory String16::toMemory() const noexcept
	{
		return Memory::create(getData(), getLength()*sizeof(sl_char16));
	}

	Memory String16::toStaticMemory() const noexcept
	{
		return Memory::createStatic(getData(), getLength()*sizeof(sl_char16));
	}

	Memory Atomic<String>::toMemory() const noexcept
	{
		String s(*this);
		return s.toMemory();
	}

	Memory Atomic<String16>::toMemory() const noexcept
	{
		String16 s(*this);
		return s.toMemory();
	}

	sl_size String::getUtf16(sl_char16* utf16, sl_size len) const noexcept
	{
		return Charsets::utf8ToUtf16(getData(), getLength(), utf16, len);
	}

	sl_size String16::getUtf8(sl_char8* utf8, sl_size len) const noexcept
	{
		return Charsets::utf16ToUtf8(getData(), getLength(), utf8, len);
	}

	sl_bool String::getUtf16(StringData& output) const noexcept
	{
		if (isEmpty()) {
			output.sz16 = (sl_char16*)("\0\0");
			output.len = 0;
			return sl_true;
		}
		Memory mem = toUtf16();
		output.refer = mem.ref;
		if (mem.isNotNull()) {
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

	sl_bool String16::getUtf8(StringData& output) const noexcept
	{
		if (isEmpty()) {
			output.sz8 = "";
			output.len = 0;
			return sl_true;
		}
		Memory mem = toUtf8();
		if (mem.isNotNull()) {
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

	sl_size Atomic<String>::getUtf16(sl_char16* utf16, sl_size len) const noexcept
	{
		String s(*this);
		return s.getUtf16(utf16, len);
	}

	sl_bool Atomic<String>::getUtf16(StringData& output) const noexcept
	{
		String s(*this);
		return s.getUtf16(output);
	}

	sl_size Atomic<String16>::getUtf8(sl_char8* utf8, sl_size len) const noexcept
	{
		String16 s(*this);
		return s.getUtf8(utf8, len);
	}

	sl_bool Atomic<String16>::getUtf8(StringData& output) const noexcept
	{
		String16 s(*this);
		return s.getUtf8(output);
	}

	Memory String::toUtf16() const noexcept
	{
		sl_char8* sz8 = getData();
		sl_size len8 = getLength();
		sl_size len16 = Charsets::utf8ToUtf16(sz8, len8, sl_null, -1);
		Memory memory = Memory::create(2 * (len16 + 1));
		if (memory.isNotNull()) {
			sl_char16* buf = (sl_char16*)(memory.getData());
			Charsets::utf8ToUtf16(sz8, len8, buf, len16);
			buf[len16] = 0;
		}
		return memory;
	}

	Memory String16::toUtf8() const noexcept
	{
		sl_char16* sz16 = getData();
		sl_size len16 = getLength();
		sl_size len8 = Charsets::utf16ToUtf8(sz16, len16, sl_null, -1);
		Memory memory = Memory::create(len8 + 1);
		if (memory.isNotNull()) {
			sl_char8* buf = (sl_char8*)(memory.getData());
			Charsets::utf16ToUtf8(sz16, len16, buf, len8);
			buf[len8] = 0;
		}
		return memory;
	}

	Memory Atomic<String>::toUtf16() const noexcept
	{
		String s(*this);
		return s.toUtf16();
	}

	Memory Atomic<String16>::toUtf8() const noexcept
	{
		String16 s(*this);
		return s.toUtf8();
	}

	sl_size String::getUtf32(sl_char32* utf32, sl_size len) const noexcept
	{
		return Charsets::utf8ToUtf32(getData(), getLength(), utf32, len);
	}

	sl_bool String::getUtf32(StringData& output) const noexcept
	{
		if (isEmpty()) {
			output.sz32 = (sl_char32*)("\0\0\0\0");
			output.len = 0;
			return sl_true;
		}
		Memory mem = toUtf32();
		if (mem.isNotNull()) {
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

	sl_size Atomic<String>::getUtf32(sl_char32* utf32, sl_size len) const noexcept
	{
		String s(*this);
		return s.getUtf32(utf32, len);
	}

	sl_bool Atomic<String>::getUtf32(StringData& output) const noexcept
	{
		String s(*this);
		return s.getUtf32(output);
	}

	Memory String::toUtf32() const noexcept
	{
		sl_char8* sz8 = getData();
		sl_size len8 = getLength();
		sl_size len32 = Charsets::utf8ToUtf32(sz8, len8, sl_null, -1);
		Memory memory = Memory::create(4 * (len32 + 1));
		if (memory.isNotNull()) {
			sl_char32* buf = (sl_char32*)(memory.getData());
			Charsets::utf8ToUtf32(sz8, len8, buf, len32);
			buf[len32] = 0;
		}
		return memory;
	}

	Memory Atomic<String>::toUtf32() const noexcept
	{
		String s(*this);
		return s.toUtf32();
	}

	String String::substring(sl_reg start, sl_reg end) const noexcept
	{
		sl_reg count = getLength();
		if (start < 0) {
			start = 0;
		}
		if (end < 0 || end > count) {
			end = count;
		}
		if (start >= end) {
			return sl_null;
		}
		if (start == 0 && end == count) {
			return *this;
		}
		return String(getData() + start, end - start);
	}

	String16 String16::substring(sl_reg start, sl_reg end) const noexcept
	{
		sl_reg count = getLength();
		if (start < 0) {
			start = 0;
		}
		if (end < 0 || end > count) {
			end = count;
		}
		if (start >= end) {
			return sl_null;
		}
		if (start == 0 && end == count) {
			return *this;
		}
		return String16(getData() + start, end - start);
	}

	String Atomic<String>::substring(sl_reg start, sl_reg end) const noexcept
	{
		String s(*this);
		return s.substring(start, end);
	}

	String16 Atomic<String16>::substring(sl_reg start, sl_reg end) const noexcept
	{
		String16 s(*this);
		return s.substring(start, end);
	}

	String String::left(sl_reg len) const noexcept
	{
		return substring(0, len);
	}

	String16 String16::left(sl_reg len) const noexcept
	{
		return substring( 0, len );
	}

	String Atomic<String>::left(sl_reg len) const noexcept
	{
		String s(*this);
		return s.left(len);
	}

	String16 Atomic<String16>::left(sl_reg len) const noexcept
	{
		String16 s(*this);
		return s.left(len);
	}

	String String::right(sl_reg len) const noexcept
	{
		return substring(getLength() - len);
	}

	String16 String16::right(sl_reg len) const noexcept
	{
		return substring(getLength() - len);
	}

	String Atomic<String>::right(sl_reg len) const noexcept
	{
		String s(*this);
		return s.right(len);
	}

	String16 Atomic<String16>::right(sl_reg len) const noexcept
	{
		String16 s(*this);
		return s.right(len);
	}

	String String::mid(sl_reg start, sl_reg len) const noexcept
	{
		return substring(start, start + len);
	}
		
	String16 String16::mid(sl_reg start, sl_reg len) const noexcept
	{
		return substring(start, start+len);
	}

	String Atomic<String>::mid(sl_reg start, sl_reg len) const noexcept
	{
		String s(*this);
		return s.mid(start, len);
	}

	String16 Atomic<String16>::mid(sl_reg start, sl_reg len) const noexcept
	{
		String16 s(*this);
		return s.mid(start, len);
	}

	sl_reg String::indexOf(sl_char8 ch, sl_reg _start) const noexcept
	{
		sl_size count = getLength();
		if (count == 0) {
			return -1;
		}
		sl_size start;
		if (_start < 0) {
			start = 0;
		} else {
			start = _start;
			if (start >= count) {
				return -1;
			}
		}
		sl_char8* sz = getData();
		sl_char8* pt = (sl_char8*)(Base::findMemory(sz + start, ch, count - start));
		if (pt == sl_null) {
			return -1;
		} else {
			return (sl_reg)(pt - sz);
		}
	}

	sl_reg String16::indexOf(sl_char16 ch, sl_reg _start) const noexcept
	{
		sl_size count = getLength();
		if (count == 0) {
			return -1;
		}
		sl_size start;
		if (_start < 0) {
			start = 0;
		} else {
			start = _start;
			if (start >= count) {
				return -1;
			}
		}
		sl_char16* sz = getData();
		sl_char16* pt = (sl_char16*)(Base::findMemory2((sl_uint16*)(sz + start), ch, count - start));
		if (pt == sl_null) {
			return -1;
		} else {
			return (sl_reg)(pt - sz);
		}
	}

	sl_reg Atomic<String>::indexOf(sl_char8 ch, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(ch, start);
	}

	sl_reg Atomic<String16>::indexOf(sl_char16 ch, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(ch, start);
	}

	template <class ST, class CT, class TT>
	SLIB_INLINE static sl_reg _priv_String_indexOf(const ST& str, const CT* bufPat, sl_size countPat, sl_reg _start) noexcept
	{
		if (countPat == 0) {
			return 0;
		}
		if (countPat == 1) {
			return str.indexOf(bufPat[0], _start);
		}
		sl_size count = str.getLength();
		if (count < countPat) {
			return -1;
		}
		const CT* buf = str.getData();
		sl_size start;
		if (_start < 0) {
			start = 0;
		} else {
			start = _start;
			if (start > count - countPat) {
				return -1;
			}
		}
		while (start <= count - countPat) {
			const CT* pt = (const CT*)(TT::findMemory(buf + start, bufPat[0], count - start - countPat + 1));
			if (pt == sl_null) {
				return -1;
			}
			if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
				return (sl_reg)(pt - buf);
			} else {
				start = (sl_size)(pt - buf + 1);
			}
		}
		return -1;
	}

	sl_reg String::indexOf(const String& pattern, sl_reg start) const noexcept
	{
		return _priv_String_indexOf<String, sl_char8, _priv_TemplateFunc8>(*this, pattern.getData(), pattern.getLength(), start);
	}

	sl_reg String16::indexOf(const String16& pattern, sl_reg start) const noexcept
	{
		return _priv_String_indexOf<String16, sl_char16, _priv_TemplateFunc16>(*this, pattern.getData(), pattern.getLength(), start);
	}

	sl_reg Atomic<String>::indexOf(const String& str, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(str, start);
	}

	sl_reg Atomic<String16>::indexOf(const String16& str, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(str, start);
	}

	sl_reg String::indexOf(const sl_char8* pattern, sl_reg start) const noexcept
	{
		return _priv_String_indexOf<String, sl_char8, _priv_TemplateFunc8>(*this, pattern, Base::getStringLength(pattern), start);
	}

	sl_reg String16::indexOf(const sl_char16* pattern, sl_reg start) const noexcept
	{
		return _priv_String_indexOf<String16, sl_char16, _priv_TemplateFunc16>(*this, pattern, Base::getStringLength2(pattern), start);
	}

	sl_reg Atomic<String>::indexOf(const sl_char8* pattern, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(pattern, start);
	}

	sl_reg Atomic<String16>::indexOf(const sl_char16* pattern, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(pattern, start);
	}

	sl_reg String::lastIndexOf(sl_char8 ch, sl_reg _start) const noexcept
	{
		sl_size count = getLength();
		if (count == 0) {
			return -1;
		}
		sl_size start;
		if (_start < 0) {
			start = count - 1;
		} else {
			start = _start;
			if (start >= count) {
				start = count - 1;
			}
		}
		sl_char8* sz = getData();
		sl_char8* pt = (sl_char8*)(Base::findMemoryReverse(sz, ch, start + 1));
		if (pt == sl_null) {
			return -1;
		} else {
			return (sl_reg)(pt - sz);
		}
	}

	sl_reg String16::lastIndexOf(sl_char16 ch, sl_reg _start) const noexcept
	{
		sl_size count = getLength();
		if (count == 0) {
			return -1;
		}
		sl_size start;
		if (_start < 0) {
			start = count - 1;
		} else {
			start = _start;
			if (start >= count) {
				start = count - 1;
			}
		}
		sl_char16* sz = getData();
		sl_char16* pt = (sl_char16*)(Base::findMemoryReverse2((sl_uint16*)sz, ch, start + 1));
		if (pt == sl_null) {
			return -1;
		} else {
			return (sl_reg)(pt - sz);
		}
	}

	sl_reg Atomic<String>::lastIndexOf(sl_char8 ch, sl_reg start) const noexcept
	{
		String s(*this);
		return s.lastIndexOf(ch, start);
	}

	sl_reg Atomic<String16>::lastIndexOf(sl_char16 ch, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.lastIndexOf(ch, start);
	}

	template <class ST, class CT, class TT>
	SLIB_INLINE static sl_reg _priv_String_lastIndexOf(const ST& str, const CT* bufPat, sl_size countPat, sl_reg _start) noexcept
	{
		if (countPat == 0) {
			return 0;
		}
		if (countPat == 1) {
			return str.lastIndexOf(bufPat[0], _start);
		}
		sl_size count = str.getLength();
		if (count < countPat) {
			return -1;
		}
		const CT* buf = str.getData();
		sl_size s;
		if (_start < 0) {
			s = count - countPat + 1;
		} else {
			s = _start;
			sl_size n = count - countPat + 1;
			if (s > n) {
				s = n;
			}
		}
		while (s > 0) {
			const CT* pt = (const CT*)(TT::findMemoryReverse(buf, bufPat[0], s));
			if (pt == sl_null) {
				return -1;
			}
			if (TT::compareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
				return (sl_reg)(pt - buf);
			} else {
				s = (sl_size)(pt - buf);
			}
		}
		return -1;
	}

	sl_reg String::lastIndexOf(const String& pattern, sl_reg start) const noexcept
	{
		return _priv_String_lastIndexOf<String, sl_char8, _priv_TemplateFunc8>(*this, pattern.getData(), pattern.getLength(), start);
	}

	sl_reg String16::lastIndexOf(const String16& pattern, sl_reg start) const noexcept
	{
		return _priv_String_lastIndexOf<String16, sl_char16, _priv_TemplateFunc16>(*this, pattern.getData(), pattern.getLength(), start);
	}

	sl_reg Atomic<String>::lastIndexOf(const String& str, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(str, start);
	}

	sl_reg Atomic<String16>::lastIndexOf(const String16& str, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(str, start);
	}

	sl_reg String::lastIndexOf(const sl_char8* pattern, sl_reg start) const noexcept
	{
		return _priv_String_lastIndexOf<String, sl_char8, _priv_TemplateFunc8>(*this, pattern, Base::getStringLength(pattern), start);
	}

	sl_reg String16::lastIndexOf(const sl_char16* pattern, sl_reg start) const noexcept
	{
		return _priv_String_lastIndexOf<String16, sl_char16, _priv_TemplateFunc16>(*this, pattern, Base::getStringLength2(pattern), start);
	}

	sl_reg Atomic<String>::lastIndexOf(const sl_char8* pattern, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(pattern, start);
	}

	sl_reg Atomic<String16>::lastIndexOf(const sl_char16* pattern, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(pattern, start);
	}

	sl_bool String::startsWith(sl_char8 ch) const noexcept
	{
		if (isEmpty()) {
			return sl_false;
		} else {
			return getData()[0] == ch;
		}
	}

	sl_bool String16::startsWith(sl_char16 ch) const noexcept
	{
		if (isEmpty()) {
			return sl_false;
		} else {
			return getData()[0] == ch;
		}
	}

	sl_bool Atomic<String>::startsWith(sl_char8 ch) const noexcept
	{
		String s(*this);
		return s.startsWith(ch);
	}

	sl_bool Atomic<String16>::startsWith(sl_char16 ch) const noexcept
	{
		String16 s(*this);
		return s.startsWith(ch);
	}


	sl_bool String::startsWith(const String& str) const noexcept
	{
		sl_size count2 = str.getLength();
		if (count2 == 0) {
			return sl_true;
		}
		sl_size count1 = getLength();
		if (count1 < count2) {
			return sl_false;
		} else {
			return Base::equalsMemory(getData(), str.getData(), count2);
		}
	}

	sl_bool String16::startsWith(const String16& str) const noexcept
	{
		sl_size count2 = str.getLength();
		if (count2 == 0) {
			return sl_true;
		}
		sl_size count1 = getLength();
		if (count1 < count2) {
			return sl_false;
		} else {
			return Base::equalsMemory2((sl_uint16*)(getData()), (sl_uint16*)(str.getData()), count2);
		}
	}

	sl_bool Atomic<String>::startsWith(const String& str) const noexcept
	{
		String s(*this);
		return s.startsWith(str);
	}

	sl_bool Atomic<String16>::startsWith(const String16& str) const noexcept
	{
		String16 s(*this);
		return s.startsWith(str);
	}

	sl_bool String::startsWith(const sl_char8* str) const noexcept
	{
		const sl_char8* s = getData();
		sl_size count = getLength();
		for (sl_size i = 0; i < count; i++) {
			if (str[i] == 0) {
				return sl_true;
			}
			if (s[i] != str[i]) {
				return sl_false;
			}
		}
		if (str[count] == 0) {
			return sl_true;
		}
		return sl_false;

	}

	sl_bool String16::startsWith(const sl_char16* str) const noexcept
	{
		const sl_char16* s = getData();
		sl_size count = getLength();
		for (sl_size i = 0; i < count; i++) {
			if (str[i] == 0) {
				return sl_true;
			}
			if (s[i] != str[i]) {
				return sl_false;
			}
		}
		if (str[count] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Atomic<String>::startsWith(const sl_char8* str) const noexcept
	{
		String s(*this);
		return s.startsWith(str);
	}

	sl_bool Atomic<String16>::startsWith(const sl_char16* str) const noexcept
	{
		String16 s(*this);
		return s.startsWith(str);
	}

	sl_bool String::endsWith(sl_char8 ch) const noexcept
	{
		sl_size count = getLength();
		if (count < 1) {
			return sl_false;
		} else {
			return getData()[count - 1] == ch;
		}
	}

	sl_bool String16::endsWith(sl_char16 ch) const noexcept
	{
		sl_size count = getLength();
		if (count < 1) {
			return sl_false;
		} else {
			return getData()[count - 1] == ch;
		}
	}

	sl_bool Atomic<String>::endsWith(sl_char8 ch) const noexcept
	{
		String s(*this);
		return s.endsWith(ch);
	}

	sl_bool Atomic<String16>::endsWith(sl_char16 ch) const noexcept
	{
		String16 s(*this);
		return s.endsWith(ch);
	}


	sl_bool String::endsWith(const String& str) const noexcept
	{
		sl_size count2 = str.getLength();
		if (count2 == 0) {
			return sl_true;
		}
		sl_size count1 = getLength();
		if (count1 < count2) {
			return sl_false;
		} else {
			return Base::equalsMemory(getData() + count1 - count2, str.getData(), count2);
		}
	}

	sl_bool String16::endsWith(const String16& str) const noexcept
	{
		sl_size count2 = str.getLength();
		if (count2 == 0) {
			return sl_true;
		}
		sl_size count1 = getLength();
		if (count1 < count2) {
			return sl_false;
		} else {
			return Base::equalsMemory2((sl_uint16*)(getData() + count1 - count2), (sl_uint16*)(str.getData()), count2);
		}
	}

	sl_bool Atomic<String>::endsWith(const String& str) const noexcept
	{
		String s(*this);
		return s.endsWith(str);
	}

	sl_bool Atomic<String16>::endsWith(const String16& str) const noexcept
	{
		String16 s(*this);
		return s.endsWith(str);
	}

	sl_bool String::endsWith(const sl_char8* str) const noexcept
	{
		sl_size count2 = Base::getStringLength(str);
		if (count2 == 0) {
			return sl_true;
		}
		sl_size count1 = getLength();
		if (count1 < count2) {
			return sl_false;
		} else {
			return Base::equalsMemory(getData() + count1 - count2, str, count2);
		}
	}

	sl_bool String16::endsWith(const sl_char16* str) const noexcept
	{
		sl_size count2 = Base::getStringLength2(str);
		if (count2 == 0) {
			return sl_true;
		}
		sl_size count1 = getLength();
		if (count1 < count2) {
			return sl_false;
		} else {
			return Base::equalsMemory2((sl_uint16*)(getData() + count1 - count2), (sl_uint16*)str, count2);
		}
	}

	sl_bool Atomic<String>::endsWith(const sl_char8* str) const noexcept
	{
		String s(*this);
		return s.endsWith(str);
	}

	sl_bool Atomic<String16>::endsWith(const sl_char16* str) const noexcept
	{
		String16 s(*this);
		return s.endsWith(str);
	}

	sl_bool String::contains(sl_char8 ch) const noexcept
	{
		return indexOf(ch) >= 0;
	}

	sl_bool String16::contains(sl_char16 ch) const noexcept
	{
		return indexOf(ch) >= 0;
	}

	sl_bool Atomic<String>::constains(sl_char8 ch) const noexcept
	{
		String s(*this);
		return s.contains(ch);
	}

	sl_bool Atomic<String16>::contains(sl_char16 ch) const noexcept
	{
		String16 s(*this);
		return s.contains(ch);
	}

	sl_bool String::contains(const String& str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool String16::contains(const String16& str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool Atomic<String>::contains(const String& str) const noexcept
	{
		String s(*this);
		return s.contains(str);
	}

	sl_bool Atomic<String16>::contains(const String16& str) const noexcept
	{
		String16 s(*this);
		return s.contains(str);
	}

	sl_bool String::contains(const sl_char8* str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool String16::contains(const sl_char16* str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool Atomic<String>::contains(const sl_char8* str) const noexcept
	{
		String s(*this);
		return s.contains(str);
	}

	sl_bool Atomic<String16>::contains(const sl_char16* str) const noexcept
	{
		String16 s(*this);
		return s.contains(str);
	}

	template <class CT>
	SLIB_INLINE static void _priv_String_copyMakingUpper(CT* dst, const CT* src, sl_size len) noexcept
	{
		for (sl_size i = 0; i < len; i++) {
			dst[i] = SLIB_CHAR_LOWER_TO_UPPER(src[i]);
		}
	}

	template <class CT>
	SLIB_INLINE static void _priv_String_copyMakingLower(CT* dst, const CT* src, sl_size len) noexcept
	{
		for (sl_size i = 0; i < len; i++) {
			dst[i] = SLIB_CHAR_UPPER_TO_LOWER(src[i]);
		}
	}

	void String::makeUpper() noexcept
	{
		_priv_String_copyMakingUpper(getData(), getData(), getLength());
	}

	void String16::makeUpper() noexcept
	{
		_priv_String_copyMakingUpper(getData(), getData(), getLength());
	}

	void Atomic<String>::makeUpper() noexcept
	{
		String s(*this);
		_priv_String_copyMakingUpper(s.getData(), s.getData(), s.getLength());
	}

	void Atomic<String16>::makeUpper() noexcept
	{
		String16 s(*this);
		_priv_String_copyMakingUpper(s.getData(), s.getData(), s.getLength());
	}

	void String::makeLower() noexcept
	{
		_priv_String_copyMakingLower(getData(), getData(), getLength());
	}

	void String16::makeLower() noexcept
	{
		_priv_String_copyMakingLower(getData(), getData(), getLength());
	}

	void Atomic<String>::makeLower() noexcept
	{
		String s(*this);
		_priv_String_copyMakingLower(s.getData(), s.getData(), s.getLength());
	}

	void Atomic<String16>::makeLower() noexcept
	{
		String16 s(*this);
		_priv_String_copyMakingLower(s.getData(), s.getData(), s.getLength());
	}

	String String::toUpper(const sl_char8* sz, sl_reg _len) noexcept
	{
		if (_len == 0) {
			return String::getEmpty();
		}
		sl_size len;
		if (_len < 0) {
			len = Base::getStringLength(sz);
		} else {
			len = _len;
		}
		String ret = String::allocate(len);
		if (ret.isNull()) {
			return ret;
		}
		_priv_String_copyMakingUpper(ret.getData(), sz, len);
		return ret;
	}

	String16 String16::toUpper(const sl_char16* sz, sl_reg _len) noexcept
	{
		if (_len == 0) {
			return String16::getEmpty();
		}
		sl_size len;
		if (_len < 0) {
			len = Base::getStringLength2(sz);
		} else {
			len = _len;
		}
		String16 ret = String16::allocate(len);
		if (ret.isNull()) {
			return ret;
		}
		_priv_String_copyMakingUpper(ret.getData(), sz, len);
		return ret;
	}

	String String::toLower(const sl_char8* sz, sl_reg _len) noexcept
	{
		if (_len == 0) {
			return String::getEmpty();
		}
		sl_size len;
		if (_len < 0) {
			len = Base::getStringLength(sz);
		} else {
			len = _len;
		}
		String ret = String::allocate(len);
		if (ret.isNull()) {
			return ret;
		}
		_priv_String_copyMakingLower(ret.getData(), sz, len);
		return ret;
	}

	String16 String16::toLower(const sl_char16* sz, sl_reg _len) noexcept
	{
		if (_len == 0) {
			return String16::getEmpty();
		}
		sl_size len;
		if (_len < 0) {
			len = Base::getStringLength2(sz);
		} else {
			len = _len;
		}
		String16 ret = String16::allocate(len);
		if (ret.isNull()) {
			return ret;
		}
		_priv_String_copyMakingLower(ret.getData(), sz, len);
		return ret;
	}

	String String::toUpper() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return toUpper(getData(), getLength());
	}

	String16 String16::toUpper() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return toUpper(getData(), getLength());
	}

	String Atomic<String>::toUpper() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		String s(*this);
		return String::toUpper(s.getData(), s.getLength());
	}

	String16 Atomic<String16>::toUpper() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		String16 s(*this);
		return String16::toUpper(s.getData(), s.getLength());
	}

	String String::toLower() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return toLower(getData(), getLength());
	}

	String16 String16::toLower() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return toLower(getData(), getLength());
	}

	String Atomic<String>::toLower() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		String s(*this);
		return String::toLower(s.getData(), s.getLength());
	}

	String16 Atomic<String16>::toLower() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		String16 s(*this);
		return String16::toLower(s.getData(), s.getLength());
	}


	struct STRING_REPLACE_SUBSET
	{
		sl_reg start;
		sl_reg len;
	};

	template <class ST, class CT>
	SLIB_INLINE static ST _priv_String_replaceAll(const ST& str, const CT* pattern, sl_reg countPat, const CT* bufReplace, sl_reg countReplace) noexcept
	{
		if (countPat == 0) {
			return sl_null;
		}
		sl_reg count = str.getLength();
		if (count == 0) {
			return sl_null;
		}
		LinkedQueue<STRING_REPLACE_SUBSET> queue;
		STRING_REPLACE_SUBSET subset;
		const CT* buf = str.getData();
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

	String String::replaceAll(const String& pattern, const String& replacement) const noexcept
	{
		return _priv_String_replaceAll<String, sl_char8>(*this, pattern.getData(), pattern.getLength(), replacement.getData(), replacement.getLength());
	}

	String16 String16::replaceAll(const String16& pattern, const String16& replacement) const noexcept
	{
		return _priv_String_replaceAll<String16, sl_char16>(*this, pattern.getData(), pattern.getLength(), replacement.getData(), replacement.getLength());
	}

	String Atomic<String>::replaceAll(const String& pattern, const String& replacement) const noexcept
	{
		String s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String16 Atomic<String16>::replaceAll(const String16& pattern, const String16& replacement) const noexcept
	{
		String16 s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String String::replaceAll(const String& pattern, const sl_char8* replacement) const noexcept
	{
		return _priv_String_replaceAll<String, sl_char8>(*this, pattern.getData(), pattern.getLength(), replacement, Base::getStringLength(replacement));
	}

	String16 String16::replaceAll(const String16& pattern, const sl_char16* replacement) const noexcept
	{
		return _priv_String_replaceAll<String16, sl_char16>(*this, pattern.getData(), pattern.getLength(), replacement, Base::getStringLength2(replacement));
	}

	String Atomic<String>::replaceAll(const String& pattern, const sl_char8* replacement) const noexcept
	{
		String s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String16 Atomic<String16>::replaceAll(const String16& pattern, const sl_char16* replacement) const noexcept
	{
		String16 s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String String::replaceAll(const sl_char8* pattern, const String& replacement) const noexcept
	{
		return _priv_String_replaceAll<String, sl_char8>(*this, pattern, Base::getStringLength(pattern), replacement.getData(), replacement.getLength());
	}

	String16 String16::replaceAll(const sl_char16* pattern, const String16& replacement) const noexcept
	{
		return _priv_String_replaceAll<String16, sl_char16>(*this, pattern, Base::getStringLength2(pattern), replacement.getData(), replacement.getLength());
	}

	String Atomic<String>::replaceAll(const sl_char8* pattern, const String& replacement) const noexcept
	{
		String s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String16 Atomic<String16>::replaceAll(const sl_char16* pattern, const String16& replacement) const noexcept
	{
		String16 s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String String::replaceAll(const sl_char8* pattern, const sl_char8* replacement) const noexcept
	{
		return _priv_String_replaceAll<String, sl_char8>(*this, pattern, Base::getStringLength(pattern), replacement, Base::getStringLength(replacement));
	}

	String16 String16::replaceAll(const sl_char16* pattern, const sl_char16* replacement) const noexcept
	{
		return _priv_String_replaceAll<String16, sl_char16>(*this, pattern, Base::getStringLength2(pattern), replacement, Base::getStringLength2(replacement));
	}

	String Atomic<String>::replaceAll(const sl_char8* pattern, const sl_char8* replacement) const noexcept
	{
		String s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String16 Atomic<String16>::replaceAll(const sl_char16* pattern, const sl_char16* replacement) const noexcept
	{
		String16 s(*this);
		return s.replaceAll(pattern, replacement);
	}

	template <class ST, class CT>
	SLIB_INLINE static ST _priv_String_trim(const ST& str) noexcept
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
			return sl_null;
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

	String String::trim() const noexcept
	{
		return _priv_String_trim<String, sl_char8>(*this);
	}

	String16 String16::trim() const noexcept
	{
		return _priv_String_trim<String16, sl_char16>(*this);
	}

	String Atomic<String>::trim() const noexcept
	{
		String s(*this);
		return s.trim();
	}

	String16 Atomic<String16>::trim() const noexcept
	{
		String16 s(*this);
		return s.trim();
	}


	template <class ST, class CT>
	SLIB_INLINE static ST _priv_String_trimLeft(const ST& str) noexcept
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
			return sl_null;
		}
		return str.substring(i);
	}

	String String::trimLeft() const noexcept
	{
		return _priv_String_trimLeft<String, sl_char8>(*this);
	}

	String16 String16::trimLeft() const noexcept
	{
		return _priv_String_trimLeft<String16, sl_char16>(*this);
	}

	String Atomic<String>::trimLeft() const noexcept
	{
		String s(*this);
		return s.trimLeft();
	}

	String16 Atomic<String16>::trimLeft() const noexcept
	{
		String16 s(*this);
		return s.trimLeft();
	}


	template <class ST, class CT>
	SLIB_INLINE static ST _priv_String_trimRight(const ST& str) noexcept
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
			return sl_null;
		}
		return str.substring(0, j);
	}

	String String::trimRight() const noexcept
	{
		return _priv_String_trimRight<String, sl_char8>(*this);
	}

	String16 String16::trimRight() const noexcept
	{
		return _priv_String_trimRight<String16, sl_char16>(*this);
	}

	String Atomic<String>::trimRight() const noexcept
	{
		String s(*this);
		return s.trimRight();
	}

	String16 Atomic<String16>::trimRight() const noexcept
	{
		String16 s(*this);
		return s.trimRight();
	}


	template <class ST, class CT, class TT>
	List<ST> _priv_String_split(const ST& str, const CT* pattern, sl_size countPattern) noexcept
	{
		if (countPattern == 0) {
			return sl_null;
		}
		if (str.isEmpty()) {
			return sl_null;
		}
		CList<ST>* ret = CList<ST>::create();
		if (ret) {
			sl_reg start = 0;
			while (1) {
				sl_reg index = _priv_String_indexOf<ST, CT, TT>(str, pattern, countPattern, start);
				if (index < 0) {
					ret->add_NoLock(str.substring(start));
					break;
				}
				ret->add_NoLock(str.substring(start, index));
				start = index + countPattern;
			}
		}
		return ret;
	}

	List<String> String::split(const String& pattern) const noexcept
	{
		return _priv_String_split<String, sl_char8, _priv_TemplateFunc8>(*this, pattern.getData(), pattern.getLength());
	}

	List<String16> String16::split(const String16& pattern) const noexcept
	{
		return _priv_String_split<String16, sl_char16, _priv_TemplateFunc16>(*this, pattern.getData(), pattern.getLength());
	}

	List<String> Atomic<String>::split(const String& pattern) const noexcept
	{
		String s(*this);
		return s.split(pattern);
	}

	List<String16> Atomic<String16>::split(const String16& pattern) const noexcept
	{
		String16 s(*this);
		return s.split(pattern);
	}

	List<String> String::split(const sl_char8* pattern) const noexcept
	{
		return _priv_String_split<String, sl_char8, _priv_TemplateFunc8>(*this, pattern, Base::getStringLength(pattern));
	}

	List<String16> String16::split(const sl_char16* pattern) const noexcept
	{
		return _priv_String_split<String16, sl_char16, _priv_TemplateFunc16>(*this, pattern, Base::getStringLength2(pattern));
	}

	List<String> Atomic<String>::split(const sl_char8* pattern) const noexcept
	{
		String s(*this);
		return s.split(pattern);
	}

	List<String16> Atomic<String16>::split(const sl_char16* pattern) const noexcept
	{
		String16 s(*this);
		return s.split(pattern);
	}

/**********************************************************
					String Conversion
**********************************************************/

#define MAX_NUMBER_STR_LEN 256
#define MAX_PRECISION 50

	const char _string_conv_radix_pattern_lower[65] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_";
	const char* _priv_StringConv_radixPatternLower = _string_conv_radix_pattern_lower;
	const char _string_conv_radix_pattern_upper[65] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@_";
	const char* _priv_StringConv_radixPatternUpper = _string_conv_radix_pattern_upper;

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
	const sl_uint8* _priv_StringConv_radixInversePatternBig = _string_conv_radix_inverse_pattern_big;

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
	const sl_uint8* _priv_StringConv_radixInversePatternSmall = _string_conv_radix_inverse_pattern_small;

	template <class IT, class CT>
	SLIB_INLINE static sl_reg _priv_String_parseInt(sl_int32 radix, const CT* sz, sl_size i, sl_size n, IT* _out) noexcept
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
			if (m < (sl_uint32)r) {
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
	SLIB_INLINE static sl_reg _priv_String_parseUint(sl_int32 radix, const CT* sz, sl_size i, sl_size n, IT* _out) noexcept
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

	sl_reg String::parseInt32(sl_int32 radix, sl_int32* _out, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		String s(*this);
		return s.parseInt32(radix, _out);
	}

	sl_bool Atomic<String16>::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		String16 s(*this);
		return s.parseInt32(radix, _out);
	}

	sl_int32 String::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		sl_int32 _out = def;
		parseInt32(radix, &_out);
		return _out;
	}

	sl_int32 String16::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		sl_int32 _out = def;
		parseInt32(radix, &_out);
		return _out;
	}

	sl_int32 Atomic<String>::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		String s(*this);
		return s.parseInt32(radix, def);
	}

	sl_int32 Atomic<String16>::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		String16 s(*this);
		return s.parseInt32(radix, def);
	}


	sl_reg String::parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		String s(*this);
		return s.parseUint32(radix, _out);
	}

	sl_bool Atomic<String16>::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		String16 s(*this);
		return s.parseUint32(radix, _out);
	}

	sl_uint32 String::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		sl_uint32 _out = def;
		parseUint32(radix, &_out);
		return _out;
	}

	sl_uint32 String16::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		sl_uint32 _out = def;
		parseUint32(radix, &_out);
		return _out;
	}

	sl_uint32 Atomic<String>::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		String s(*this);
		return s.parseUint32(radix, def);
	}

	sl_uint32 Atomic<String16>::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		String16 s(*this);
		return s.parseUint32(radix, def);
	}


	sl_reg String::parseInt64(sl_int32 radix, sl_int64* _out, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		String s(*this);
		return s.parseInt64(radix, _out);
	}

	sl_bool Atomic<String16>::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		String16 s(*this);
		return s.parseInt64(radix, _out);
	}

	sl_int64 String::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		sl_int64 _out = def;
		parseInt64(radix, &_out);
		return _out;
	}

	sl_int64 String16::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		sl_int64 _out = def;
		parseInt64(radix, &_out);
		return _out;
	}

	sl_int64 Atomic<String>::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		String s(*this);
		return s.parseInt64(radix, def);
	}

	sl_int64 Atomic<String16>::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		String16 s(*this);
		return s.parseInt64(radix, def);
	}


	sl_reg String::parseUint64(sl_int32 radix, sl_uint64* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		String s(*this);
		return s.parseUint64(radix, _out);
	}

	sl_bool Atomic<String16>::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		String16 s(*this);
		return s.parseUint64(radix, _out);
	}

	sl_uint64 String::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		sl_uint64 _out = def;
		parseUint64(radix, &_out);
		return _out;
	}

	sl_uint64 String16::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		sl_uint64 _out = def;
		parseUint64(radix, &_out);
		return _out;
	}

	sl_uint64 Atomic<String>::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		String s(*this);
		return s.parseUint64(radix, def);
	}

	sl_uint64 Atomic<String16>::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		String16 s(*this);
		return s.parseUint64(radix, def);
	}


	sl_bool String::parseInt(sl_int32 radix, sl_reg* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(radix, _out);
#else
		return parseInt32(radix, _out);
#endif
	}

	sl_bool String16::parseInt(sl_int32 radix, sl_reg* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(radix, _out);
#else
		return parseInt32(radix, _out);
#endif
	}

	sl_bool Atomic<String>::parseInt(sl_int32 radix, sl_reg* _out) const noexcept
	{
		String s(*this);
		return s.parseInt(radix, _out);
	}

	sl_bool Atomic<String16>::parseInt(sl_int32 radix, sl_reg* _out) const noexcept
	{
		String16 s(*this);
		return s.parseInt(radix, _out);
	}

	sl_reg String::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		sl_reg _out = def;
		parseInt(radix, &_out);
		return _out;
	}

	sl_reg String16::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		sl_reg _out = def;
		parseInt(radix, &_out);
		return _out;
	}

	sl_reg Atomic<String>::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		String s(*this);
		return s.parseInt(radix, def);
	}

	sl_reg Atomic<String16>::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		String16 s(*this);
		return s.parseInt(radix, def);
	}


	sl_bool String::parseSize(sl_int32 radix, sl_size* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(radix, _out);
#else
		return parseUint32(radix, _out);
#endif
	}

	sl_bool String16::parseSize(sl_int32 radix, sl_size* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(radix, _out);
#else
		return parseUint32(radix, _out);
#endif
	}

	sl_bool Atomic<String>::parseSize(sl_int32 radix, sl_size* _out) const noexcept
	{
		String s(*this);
		return s.parseSize(radix, _out);
	}

	sl_bool Atomic<String16>::parseSize(sl_int32 radix, sl_size* _out) const noexcept
	{
		String16 s(*this);
		return s.parseSize(radix, _out);
	}

	sl_size String::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		sl_size _out = def;
		parseSize(radix, &_out);
		return _out;
	}

	sl_size String16::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		sl_size _out = def;
		parseSize(radix, &_out);
		return _out;
	}

	sl_size Atomic<String>::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		String s(*this);
		return s.parseSize(radix, def);
	}

	sl_size Atomic<String16>::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		String16 s(*this);
		return s.parseSize(radix, def);
	}


	template <class FT, class CT>
	SLIB_INLINE static sl_reg _priv_String_parseFloat(const CT* sz, sl_size i, sl_size n, FT* _out) noexcept
	{
		if (i >= n) {
			return SLIB_PARSE_ERROR; // input string is empty
		}
		
		sl_bool bMinus = sl_false;
		sl_bool bEmpty = sl_true;
		
		if (sz[i] == '-') {
			i++;
			bMinus = sl_true;
		}
		for (; i < n; i++) {
			sl_uint32 c = (sl_uint32)(sz[i]);
			if (!(SLIB_CHAR_IS_SPACE_TAB(c))) {
				break;
			}
		}
		
		FT v = 0;
		sl_uint32 vi = 0;
		sl_bool flagMulInt = sl_true;

		for (; i < n; i++) {
			sl_uint32 c = (sl_uint32)sz[i];
			if (SLIB_CHAR_IS_DIGIT(c)) {
				if (flagMulInt) {
					vi = vi * 10 + (c - '0');
					if (vi >= 214748364) {
						v = (FT)vi;
						flagMulInt = sl_false;
					}
				} else {
					v = v * 10 + (c - '0');
				}
				bEmpty = sl_false;
			} else {
				break;
			}
		}
		if (bEmpty) {
			return SLIB_PARSE_ERROR; // integral number is required
		}
		if (flagMulInt) {
			v = (FT)vi;
		}
		
		if (i < n) {
			if (sz[i] == '.') {
				i++;
				bEmpty = sl_true;
				FT weight = (FT)(0.1);
				for (; i < n; i++) {
					sl_uint32 c = (sl_uint32)sz[i];
					if (SLIB_CHAR_IS_DIGIT(c)) {
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
						if (SLIB_CHAR_IS_DIGIT(c)) {
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

	sl_reg String::parseFloat(float* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseFloat(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseFloat(float* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseFloat(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseFloat(float* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseFloat(float* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseFloat(float* _out) const noexcept
	{
		String s(*this);
		return s.parseFloat(_out);
	}

	sl_bool Atomic<String16>::parseFloat(float* _out) const noexcept
	{
		String16 s(*this);
		return s.parseFloat(_out);
	}

	float String::parseFloat(float def) const noexcept
	{
		float _out = def;
		parseFloat(&_out);
		return _out;
	}

	float String16::parseFloat(float def) const noexcept
	{
		float _out = def;
		parseFloat(&_out);
		return _out;
	}

	float Atomic<String>::parseFloat(float def) const noexcept
	{
		String s(*this);
		return s.parseFloat(def);
	}

	float Atomic<String16>::parseFloat(float def) const noexcept
	{
		String16 s(*this);
		return s.parseFloat(def);
	}


	sl_reg String::parseDouble(double* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseFloat(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseDouble(double* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseFloat(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseDouble(double* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseDouble(double* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseDouble(double* _out) const noexcept
	{
		String s(*this);
		return s.parseDouble(_out);
	}

	sl_bool Atomic<String16>::parseDouble(double* _out) const noexcept
	{
		String16 s(*this);
		return s.parseDouble(_out);
	}

	double String::parseDouble(double def) const noexcept
	{
		double _out = def;
		parseDouble(&_out);
		return _out;
	}

	double String16::parseDouble(double def) const noexcept
	{
		double _out = def;
		parseDouble(&_out);
		return _out;
	}

	double Atomic<String>::parseDouble(double def) const noexcept
	{
		String s(*this);
		return s.parseDouble(def);
	}

	double Atomic<String16>::parseDouble(double def) const noexcept
	{
		String16 s(*this);
		return s.parseDouble(def);
	}

	template <class CT>
	SLIB_INLINE static sl_reg _priv_String_parseBoolean(const CT* sz, sl_size i, sl_size n, sl_bool* _out) noexcept
	{
		if (i >= n) {
			return SLIB_PARSE_ERROR;
		}
		sl_bool f = sl_false;
		switch(sz[i]) {
			case '1':
				i++;
				f = sl_true;
				break;
			case '0':
				i++;
				f = sl_false;
				break;
			case 'T':
			case 't':
				if (i + 4 <= n) {
					i++;
					if (sz[i] == 'R' || sz[i] == 'r') {
						i++;
						if (sz[i] == 'U' || sz[i] == 'u') {
							i++;
							if (sz[i] == 'E' || sz[i] == 'e') {
								i++;
								f = sl_true;
								break;
							}
						}
					}
				}
				return SLIB_PARSE_ERROR;
			case 'F':
			case 'f':
				if (i + 5 <= n) {
					i++;
					if (sz[i] == 'A' || sz[i] == 'a') {
						i++;
						if (sz[i] == 'L' || sz[i] == 'l') {
							i++;
							if (sz[i] == 'S' || sz[i] == 's') {
								i++;
								if (sz[i] == 'E' || sz[i] == 'e') {
									i++;
									f = sl_false;
									break;
								}
							}
						}
					}
				}
				return SLIB_PARSE_ERROR;
			case 'Y':
			case 'y':
				i++;
				if (i + 2 <= n && (sz[i] == 'E' || sz[i] == 'e')) {
					i++;
					if (sz[i] == 'S' || sz[i] == 's') {
						i++;
						f = sl_true;
						break;
					}
				} else {
					f = sl_true;
					break;
				}
				return SLIB_PARSE_ERROR;
			case 'N':
			case 'n':
				i++;
				if (i + 1 <= n && (sz[i] == 'O' || sz[i] == 'o')) {
					i++;
				}
				f = sl_false;
				break;
		}
		if (i < n && sz[i] && SLIB_CHAR_IS_C_NAME(sz[i])) {
			return SLIB_PARSE_ERROR;
		}
		if (_out) {
			*_out = f;
		}
		return i;
	}

	sl_reg String::parseBoolean(sl_bool* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseBoolean(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseBoolean(sl_bool* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseBoolean(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseBoolean(sl_bool* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseBoolean(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseBoolean(sl_bool* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseBoolean(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseBoolean(sl_bool* _out) const noexcept
	{
		String s(*this);
		return s.parseBoolean(_out);
	}

	sl_bool Atomic<String16>::parseBoolean(sl_bool* _out) const noexcept
	{
		String16 s(*this);
		return s.parseBoolean(_out);
	}

	sl_bool String::parseBoolean(sl_bool def) const noexcept
	{
		sl_bool _out = def;
		parseBoolean(&_out);
		return _out;
	}

	sl_bool String16::parseBoolean(sl_bool def) const noexcept
	{
		sl_bool _out = def;
		parseBoolean(&_out);
		return _out;
	}

	sl_bool Atomic<String>::parseBoolean(sl_bool def) const noexcept
	{
		String s(*this);
		return s.parseBoolean(def);
	}

	sl_bool Atomic<String16>::parseBoolean(sl_bool def) const noexcept
	{
		String16 s(*this);
		return s.parseBoolean(def);
	}


	template <class CT>
	SLIB_INLINE static sl_reg _priv_String_parseHexString(const CT* sz, sl_size i, sl_size n, void* _out) noexcept
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

	sl_reg String::parseHexString(void* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseHexString(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseHexString(void* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_String_parseHexString(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseHexString(void* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseHexString(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseHexString(void* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_String_parseHexString(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool Atomic<String>::parseHexString(void* _out) const noexcept
	{
		String s(*this);
		return s.parseHexString(_out);
	}

	sl_bool Atomic<String16>::parseHexString(void* _out) const noexcept
	{
		String16 s(*this);
		return s.parseHexString(_out);
	}



	template <class IT, class UT, class ST, class CT>
	SLIB_INLINE static ST _priv_String_fromInt(IT _value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase, CT chGroup = sl_false, sl_bool flagSignPositive = sl_false, sl_bool flagLeadingSpacePositive = sl_false, sl_bool flagEncloseNagtive = sl_false) noexcept
	{
		if (radix < 2 || radix > 64) {
			return sl_null;
		}
		
		const char* pattern = flagUpperCase && radix <= 36 ? _string_conv_radix_pattern_upper : _string_conv_radix_pattern_lower;
		
		CT buf[MAX_NUMBER_STR_LEN];
		
		sl_uint32 pos = MAX_NUMBER_STR_LEN;
		
		if (minWidth < 1) {
			minWidth = 1;
		}
		
		sl_bool flagMinus = sl_false;
		UT value;
		if (_value < 0) {
			value = -_value;
			flagMinus = sl_true;
			if (flagEncloseNagtive) {
				pos--;
				buf[pos] = ')';
			}
		} else {
			value = _value;
		}
		
		sl_uint32 nDigits = 0;
		while (value || minWidth > 0) {
			if (chGroup) {
				if (nDigits > 0) {
					if (pos > 0) {
						if (nDigits % 3 == 0) {
							pos--;
							buf[pos] = chGroup;
						}
					} else {
						break;
					}
				}
			}
			if (pos > 0) {
				pos--;
				buf[pos] = pattern[value % radix];
				value = value / radix;
				if (minWidth > 0) {
					minWidth--;
				}
				nDigits++;
			} else {
				break;
			}
		}
		
		if (flagMinus) {
			if (pos > 0) {
				pos--;
				buf[pos] = '-';
				if (flagEncloseNagtive) {
					if (pos > 0) {
						pos--;
						buf[pos] = '(';
					}
				}
			}
		} else {
			if (flagSignPositive) {
				if (pos > 0) {
					pos--;
					buf[pos] = '+';
				}
			}
			if (flagLeadingSpacePositive) {
				if (pos > 0) {
					pos--;
					buf[pos] = ' ';
				}
			}
		}
		return ST(buf + pos, MAX_NUMBER_STR_LEN - pos);
	}

	template <class IT, class ST, class CT>
	SLIB_INLINE static ST _priv_String_fromUint(IT value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase, CT chGroup = 0, sl_bool flagSignPositive = sl_false, sl_bool flagLeadingSpacePositive = sl_false) noexcept
	{
		if (radix < 2 || radix > 64) {
			return sl_null;
		}
		
		const char* pattern = flagUpperCase && radix <= 36 ? _string_conv_radix_pattern_upper : _string_conv_radix_pattern_lower;
		
		CT buf[MAX_NUMBER_STR_LEN];
		
		sl_uint32 pos = MAX_NUMBER_STR_LEN;
		
		if (minWidth < 1) {
			minWidth = 1;
		}
		
		sl_uint32 nDigits = 0;
		while (value || minWidth > 0) {
			if (chGroup) {
				if (nDigits > 0) {
					if (pos > 0) {
						if (nDigits % 3 == 0) {
							pos--;
							buf[pos] = chGroup;
						}
					} else {
						break;
					}
				}
			}
			if (pos > 0) {
				pos--;
				buf[pos] = pattern[value % radix];
				value = value / radix;
				if (minWidth > 0) {
					minWidth--;
				}
				nDigits++;
			} else {
				break;
			}
		}
		
		if (flagSignPositive) {
			if (pos > 0) {
				pos--;
				buf[pos] = '+';
			}
		}
		if (flagLeadingSpacePositive) {
			if (pos > 0) {
				pos--;
				buf[pos] = ' ';
			}
		}
		
		return ST(buf + pos, MAX_NUMBER_STR_LEN - pos);
	}

	String String::fromInt32(sl_int32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromInt<sl_int32, sl_uint32, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromInt32(sl_int32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromInt<sl_int32, sl_uint32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromUint32(sl_uint32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromUint<sl_uint32, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromUint32(sl_uint32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromUint<sl_uint32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromInt64(sl_int64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromInt<sl_int64, sl_uint64, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromInt64(sl_int64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromInt<sl_int64, sl_uint64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromUint64(sl_uint64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromUint<sl_uint64, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromUint64(sl_uint64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return _priv_String_fromUint<sl_uint64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromInt(sl_reg value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromInt64(value, radix, minWidth, flagUpperCase);
#else
		return fromInt32(value, radix, minWidth, flagUpperCase);
#endif
	}

	String16 String16::fromInt(sl_reg value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromInt64(value, radix, minWidth, flagUpperCase);
#else
		return fromInt32(value, radix, minWidth, flagUpperCase);
#endif
	}

	String String::fromSize(sl_size value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64(value, radix, minWidth, flagUpperCase);
#else
		return fromUint32(value, radix, minWidth, flagUpperCase);
#endif
	}

	String16 String16::fromSize(sl_size value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64(value, radix, minWidth, flagUpperCase);
#else
		return fromUint32(value, radix, minWidth, flagUpperCase);
#endif
	}

	template <class FT, class ST, class CT>
	SLIB_INLINE static ST _priv_String_fromFloat(FT value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral, CT chConv = 'g', CT chGroup = 0, sl_bool flagSignPositive = sl_false, sl_bool flagLeadingSpacePositive = sl_false, sl_bool flagEncloseNagtive = sl_false) noexcept
	{
		
		CT buf[MAX_NUMBER_STR_LEN];
		
		if (Math::isNaN(value)) {
			return "NaN";
		}
		if (Math::isInfinite(value)) {
			return "Infinity";
		}

		if (minWidthIntegral > MAX_PRECISION) {
			minWidthIntegral = MAX_PRECISION;
		}
		if (precision > MAX_PRECISION) {
			precision = MAX_PRECISION;
		}
		if (precision <= 0) {
			flagZeroPadding = sl_false;
		}

		if (value == 0) {
			sl_uint32 pos = 0;
			if (flagLeadingSpacePositive) {
				buf[pos++] = ' ';
			}
			if (flagSignPositive) {
				buf[pos++] = '+';
			}
			for (sl_int32 i = 0; i < minWidthIntegral; i++) {
				if (chGroup) {
					if (i > 0) {
						if ((minWidthIntegral - i) % 3 == 0) {
							buf[pos++] = chGroup;
						}
					}
				}
				buf[pos++] = '0';
			}
			if (precision != 0) {
				buf[pos++] = '.';
				if (flagZeroPadding) {
					for (sl_int32 k = 0; k < precision; k++) {
						buf[pos++] = '0';
					}
				} else {
					buf[pos++] = '0';
				}
			}
			return ST(buf, pos);
		}
		
		CT* str = buf;
		CT* str_last = buf + MAX_NUMBER_STR_LEN - MAX_PRECISION;
		
		sl_int32 flagMinus;
		if (value < 0) {
			flagMinus = 1;
			value = -value;
			if (flagEncloseNagtive) {
				*(str++) = '(';
			}
			*(str++) = '-';
		} else {
			flagMinus = 0;
			if (flagLeadingSpacePositive) {
				*(str++) = ' ';
			}
			if (flagSignPositive) {
				*(str++) = '+';
			}
		}
		
		sl_int32 nExp;
		sl_int32 nInt;
		if (chConv == 'f') {
			nInt = (sl_int32)(Math::log10(value));
			nExp = 0;
		} else if (chConv == 'e' || chConv == 'E') {
			nInt = minWidthIntegral - 1;
			nExp = (sl_int32)(Math::log10(value));
		} else {
			nInt = (sl_int32)(Math::log10(value));
			nExp = 0;
			if (nInt >= 15) { // use exp
				nExp = nInt;
				nInt = 0;
			}
			if (nInt < -15) { // use exp
				nExp = nInt - 1;
				nInt = 0;
			}
		}
		
		if (nExp != 0) {
			value = value / Math::pow((FT)10, (FT)(nExp));
		}
		
		FT min_value;
		if (precision < 0) {
			if (sizeof(FT) == 4) {
				precision = 5 - nInt;
			} else {
				precision = 15 - nInt;
			}
			if (precision < 0) {
				precision = 0;
			}
			if (precision > 50) {
				precision = 50;
			}
		}
		min_value = Math::pow((FT)10, (FT)(-precision));
		value += min_value / 3;
		if (flagZeroPadding) {
			min_value = 0;
		}

		if (nInt < minWidthIntegral - 1) {
			nInt = minWidthIntegral - 1;
		}
		FT weight = 1;
		if (nInt != 0) {
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
			if (weight > 0 && !(Math::isInfinite(weight))) {
				sl_int32 digit = (sl_int32)(value / weight);
				value -= (digit * weight);
				if (digit >= 0 && digit <= 9) {
					*(str++) = (CT)('0' + digit);
				} else {
					*(str++) = '#';
				}
			}
			if (chGroup) {
				if (nInt > 0 && nInt % 3 == 0) {
					*(str++) = chGroup;
				}
			}
			nInt--;
			weight /= 10;
		}
		if (precision > 0) {
			if (nInt >= -1) {
				*(str++) = '.';
				*(str++) = '0';
			}
		}
		
		if (nExp) {
			if (chConv == 'E' || chConv == 'G') {
				*(str++) = 'E';
			} else {
				*(str++) = 'e';
			}
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

		if (flagMinus) {
			if (flagEncloseNagtive) {
				*(str++) = ')';
			}
		}
		
		return ST(buf, str - buf);
	}

	String String::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return _priv_String_fromFloat<double, String, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String String::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return _priv_String_fromFloat<float, String, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String16 String16::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return _priv_String_fromFloat<double, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String16 String16::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return _priv_String_fromFloat<float, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String String::fromPointerValue(const void* pointer) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64((sl_uint64)(pointer), 16, 16, sl_true);
#else
		return fromUint32((sl_uint32)(pointer), 16, 8, sl_true);
#endif
	}

	String16 String16::fromPointerValue(const void* pointer) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return fromUint64((sl_uint64)(pointer), 16, 16, sl_true);
#else
		return fromUint32((sl_uint32)(pointer), 16, 8, sl_true);
#endif
	}

	String String::fromBoolean(sl_bool value) noexcept
	{
		if (value) {
			SLIB_STATIC_STRING(s, "true");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "false");
			return s;
		}
	}

	String16 String16::fromBoolean(sl_bool value) noexcept
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
	SLIB_INLINE static ST _priv_String_makeHexString(const void* buf, sl_size size) noexcept
	{
		if (!buf || size <= 0) {
			return sl_null;
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

	String String::makeHexString(const void* buf, sl_size size) noexcept
	{
		return _priv_String_makeHexString<String, sl_char8>(buf, size);
	}

	String16 String16::makeHexString(const void* buf, sl_size size) noexcept
	{
		return _priv_String_makeHexString<String16, sl_char16>(buf, size);
	}

	String String::makeHexString(const Memory& mem) noexcept
	{
		return makeHexString(mem.getData(), mem.getSize());
	}

	String16 String16::makeHexString(const Memory& mem) noexcept
	{
		return makeHexString(mem.getData(), mem.getSize());
	}

/*

	String Formatting is similar with Java Formatter

https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html

	%[argument_index$][flags][width][.precision]conversion

*/

	template <class ST, class CT, class BT>
	SLIB_INLINE static ST _priv_String_format(const CT* format, sl_size len, const Variant* params, sl_size _nParams) noexcept
	{
		if (len == 0) {
			return ST::getEmpty();
		}
		sl_uint32 nParams = (sl_uint32)_nParams;
		if (nParams == 0) {
			return format;
		}
		BT sb;
		sl_size pos = 0;
		sl_size posText = 0;
		sl_uint32 indexArgLast = 0;
		sl_uint32 indexArgAuto = 0;
		while (pos <= len) {
			CT ch;
			if (pos < len) {
				ch = format[pos];
			} else {
				ch = 0;
			}
			if (ch == '%' || ch == 0) {
				sb.addStatic(format + posText, pos - posText);
				posText = pos;
				pos++;
				if (pos >= len) {
					break;
				}
				if (ch == '%') {
					do {
						ch = format[pos];
						if (ch == '%') {
							CT t = '%';
							sb.addStatic(&t, 1);
							pos++;
							posText = pos;
							break;
						} else if (ch == 'n') {
							CT t[2] = {'\r', '\n'};
							sb.addStatic(t, 2);
							pos++;
							posText = pos;
							break;
						}
						// Argument Index
						sl_uint32 indexArg;
						if (ch == '<') {
							indexArg = indexArgLast;
							pos++;
						} else {
							sl_uint32 iv;
							sl_reg iRet = ST::parseUint32(10, &iv, format, pos, len);
							if (iRet == SLIB_PARSE_ERROR) {
								indexArg = indexArgAuto;
								indexArgAuto++;
							} else {
								if ((sl_uint32)iRet >= len) {
									break;
								}
								if (format[iRet] == '$') {
									if (iv > 0) {
										iv--;
									}
									indexArg = iv;
									pos = iRet + 1;
								} else {
									indexArg = indexArgAuto;
									indexArgAuto++;
								}
							}
						}
						if (indexArg >= nParams) {
							indexArg = nParams - 1;
						}
						indexArgLast = indexArg;
						if (pos >= len) {
							break;
						}
						
						// Flags
						sl_bool flagAlignLeft = sl_false; // '-'
						sl_bool flagSignPositive = sl_false; // '+'
						sl_bool flagLeadingSpacePositive = sl_false; // ' '
						sl_bool flagZeroPadded = sl_false; // '0'
						sl_bool flagGroupingDigits = sl_false; // ','
						sl_bool flagEncloseNegative = sl_false; // '('
						do {
							ch = format[pos];
							if (ch == '-') {
								flagAlignLeft = sl_true;
							} else if (ch == '+') {
								flagSignPositive = sl_true;
							} else if (ch == ' ') {
								flagLeadingSpacePositive = sl_true;
							} else if (ch == '0') {
								flagZeroPadded = sl_true;
							} else if (ch == ',') {
								flagGroupingDigits = sl_true;
							} else if (ch == '(') {
								flagEncloseNegative = sl_true;
							} else {
								break;
							}
							pos++;
						} while (pos < len);
						if (pos >= len) {
							break;
						}
						
						// Min-Width
						sl_uint32 minWidth = 0;
						sl_reg iRet = ST::parseUint32(10, &minWidth, format, pos, len);
						if (iRet != SLIB_PARSE_ERROR) {
							pos = iRet;
							if (pos >= len) {
								break;
							}
						}
						
						// Precision
						sl_uint32 precision = 0;
						sl_bool flagUsePrecision = sl_false;
						if (format[pos] == '.') {
							pos++;
							if (pos >= len) {
								break;
							}
							flagUsePrecision = sl_true;
							iRet = ST::parseUint32(10, &precision, format, pos, len);
							if (iRet != SLIB_PARSE_ERROR) {
								pos = iRet;
								if (pos >= len) {
									break;
								}
							}
						}
						
						// Conversion
						ch = format[pos];
						pos++;
						
						const Variant& arg = params[indexArg];
						
						sl_size lenContent = 0;
						sl_bool flagError = sl_false;
						
						if (arg.isTime()) {
							const TimeZone* zone;
							if (ch == 'u' || ch == 'U') {
								zone = &(TimeZone::UTC());
								if (pos < len) {
									ch = format[pos];
									pos++;
								} else {
									ch = 's';
								}
							} else {
								zone = &(TimeZone::Local);
							}
							ST content;
							Time time = arg.getTime();
							switch (ch) {
								case 'y':
									if (flagZeroPadded) {
										if (flagZeroPadded) {
											if (minWidth < 4) {
												minWidth = 4;
											}
										}
										content = ST::fromInt32(time.getYear(*zone), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getYear(*zone));
									}
									break;
								case 'm':
									if (flagZeroPadded) {
										if (flagZeroPadded) {
											if (minWidth < 2) {
												minWidth = 2;
											}
										}
										content = ST::fromInt32(time.getMonth(*zone), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getMonth(*zone));
									}
									break;
								case 'd':
									if (flagZeroPadded) {
										if (flagZeroPadded) {
											if (minWidth < 2) {
												minWidth = 2;
											}
										}
										content = ST::fromInt32(time.getDay(*zone), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getDay(*zone));
									}
									break;
								case 'w':
									content = time.getWeekdayShort(*zone);
									break;
								case 'W':
									content = time.getWeekdayLong(*zone);
									break;
								case 'H':
									if (flagZeroPadded) {
										if (flagZeroPadded) {
											if (minWidth < 2) {
												minWidth = 2;
											}
										}
										content = ST::fromInt32(time.getHour(*zone), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getHour(*zone));
									}
									break;
								case 'M':
									if (flagZeroPadded) {
										if (flagZeroPadded) {
											if (minWidth < 2) {
												minWidth = 2;
											}
										}
										content = ST::fromInt32(time.getMinute(*zone), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getMinute(*zone));
									}
									break;
								case 'S':
									if (flagZeroPadded) {
										if (flagZeroPadded) {
											if (minWidth < 2) {
												minWidth = 2;
											}
										}
										content = ST::fromInt32(time.getSecond(*zone), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getSecond(*zone));
									}
									break;
								case 'l':
									if (flagZeroPadded) {
										content = ST::fromInt32(time.getMillisecond(), 10, minWidth);
									} else {
										content = ST::fromInt32(time.getMillisecond());
									}
									break;
								case 'D':
									if (sizeof(CT) == 1) {
										content = time.getDateString(*zone);
									} else {
										content = time.getDateString16(*zone);
									}
									break;
								case 'T':
									if (sizeof(CT) == 1) {
										content = time.getTimeString(*zone);
									} else {
										content = time.getTimeString16(*zone);
									}
									break;
								case 's':
									if (sizeof(CT) == 1) {
										content = time.toString(*zone);
									} else {
										content = time.toString16(*zone);
									}
									break;
								default:
									flagError = sl_true;
									break;
							}
							if (flagError) {
								break;
							}
							lenContent = content.getLength();
							if (lenContent < minWidth) {
								if (flagAlignLeft) {
									sb.add(content);
									sb.add(ST(' ', minWidth - lenContent));
								} else {
									sb.add(ST(' ', minWidth - lenContent));
									sb.add(content);
								}
							} else {
								sb.add(content);
							}
						} else {
							switch (ch) {
								case 's':
								{
									if (sizeof(CT) == 1) {
										if (arg.isSz8() ) {
											const sl_char8* sz = arg.getSz8();
											lenContent = Base::getStringLength(sz);
											if (lenContent < minWidth) {
												if (flagAlignLeft) {
													sb.addStatic((const CT*)sz, lenContent);
													sb.add(ST(' ', minWidth - lenContent));
												} else {
													sb.add(ST(' ', minWidth - lenContent));
													sb.addStatic((const CT*)sz, lenContent);
												}
											} else {
												sb.addStatic((const CT*)sz, lenContent);
											}
										} else {
											String str = arg.getString();
											if (str.isEmpty()) {
												str = arg.toString();
											}
											lenContent = str.getLength();
											if (lenContent < minWidth) {
												if (flagAlignLeft) {
													sb.add(str);
													sb.add(ST(' ', minWidth - lenContent));
												} else {
													sb.add(ST(' ', minWidth - lenContent));
													sb.add(str);
												}
											} else {
												sb.add(str);
											}
										}
									} else {
										if (arg.isSz16() ) {
											const sl_char16* sz = arg.getSz16();
											lenContent = Base::getStringLength2(sz);
											if (lenContent < minWidth) {
												if (flagAlignLeft) {
													sb.addStatic((const CT*)sz, lenContent);
													sb.add(ST(' ', minWidth - lenContent));
												} else {
													sb.add(ST(' ', minWidth - lenContent));
													sb.addStatic((const CT*)sz, lenContent);
												}
											} else {
												sb.addStatic((const CT*)sz, lenContent);
											}
										} else {
											String16 str = arg.getString16();
											if (str.isEmpty()) {
												str = arg.toString();
											}
											lenContent = str.getLength();
											if (lenContent < minWidth) {
												if (flagAlignLeft) {
													sb.add(str);
													sb.add(ST(' ', minWidth - lenContent));
												} else {
													sb.add(ST(' ', minWidth - lenContent));
													sb.add(str);
												}
											} else {
												sb.add(str);
											}
										}
									}
									break;
								}
								case 'd':
								case 'x':
								case 'X':
								case 'o':
								{
									CT chGroup = 0;
									if (flagGroupingDigits) {
										chGroup = ',';
									}
									sl_uint32 radix = 10;
									sl_bool flagUpperCase = sl_false;
									if (ch == 'x') {
										radix = 16;
									} else if (ch == 'X') {
										radix = 16;
										flagUpperCase = sl_true;
									} else if (ch == 'o') {
										radix = 8;
									}
									sl_uint32 _minWidth = 0;
									if (flagZeroPadded) {
										_minWidth = minWidth;
									}
									ST content;
									if (arg.isUint32()) {
										content = _priv_String_fromUint<sl_uint32, ST, CT>(arg.getUint32(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive);
									} else if (arg.isInt32()) {
										content = _priv_String_fromInt<sl_int32, sl_uint32, ST, CT>(arg.getInt32(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
									} else if (arg.isUint64()) {
										content = _priv_String_fromUint<sl_uint64, ST, CT>(arg.getUint64(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive);
									} else {
										content = _priv_String_fromInt<sl_int64, sl_uint64, ST, CT>(arg.getInt64(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
									}
									lenContent = content.getLength();
									if (lenContent < minWidth) {
										if (flagAlignLeft) {
											sb.add(content);
											sb.add(ST(' ', minWidth - lenContent));
										} else {
											sb.add(ST(' ', minWidth - lenContent));
											sb.add(content);
										}
									} else {
										sb.add(content);
									}
									break;
								}
								case 'f':
								case 'e':
								case 'E':
								case 'g':
								case 'G':
								{
									CT chGroup = 0;
									if (flagGroupingDigits) {
										chGroup = ',';
									}
									sl_int32 _precision = -1;
									if (flagUsePrecision) {
										_precision = precision;
									}
									ST content;
									if (arg.isFloat()) {
										content = _priv_String_fromFloat<float, ST, CT>(arg.getFloat(), _precision, flagZeroPadded, 1, ch, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
									} else {
										content = _priv_String_fromFloat<double, ST, CT>(arg.getDouble(), _precision, flagZeroPadded, 1, ch, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
									}
									lenContent = content.getLength();
									if (lenContent < minWidth) {
										if (flagAlignLeft) {
											sb.add(content);
											sb.add(ST(' ', minWidth - lenContent));
										} else {
											sb.add(ST(' ', minWidth - lenContent));
											sb.add(content);
										}
									} else {
										sb.add(content);
									}
									break;
								}
								case 'c':
								{
									sl_char16 unicode = (sl_char16)(arg.getUint32());
									ST str(&unicode, 1);
									lenContent = str.getLength();
									if (lenContent < minWidth) {
										if (flagAlignLeft) {
											sb.add(str);
											sb.add(ST(' ', minWidth - lenContent));
										} else {
											sb.add(ST(' ', minWidth - lenContent));
											sb.add(str);
										}
									} else {
										sb.add(str);
									}
									break;
								}
								default:
									flagError = sl_true;
									break;
							}
							if (flagError) {
								break;
							}
						}
						posText = pos;
					} while (0);
				} else {
					break;
				}
			} else {
				pos++;
			}
		}
		return sb.merge();
	}

	String String::formatBy(const String& format, const Variant *params, sl_size nParams) noexcept
	{
		return _priv_String_format<String, sl_char8, StringBuffer>(format.getData(), format.getLength(), params, nParams);
	}

	String16 String16::formatBy(const String16& format, const Variant *params, sl_size nParams) noexcept
	{
		return _priv_String_format<String16, sl_char16, StringBuffer16>(format.getData(), format.getLength(), params, nParams);
	}

	String String::formatBy(const sl_char8* format, const Variant *params, sl_size nParams) noexcept
	{
		return _priv_String_format<String, sl_char8, StringBuffer>(format, Base::getStringLength(format), params, nParams);
	}

	String16 String16::formatBy(const sl_char16* format, const Variant *params, sl_size nParams) noexcept
	{
		return _priv_String_format<String16, sl_char16, StringBuffer16>(format, Base::getStringLength2(format), params, nParams);
	}

	String String::format(const String& strFormat) noexcept
	{
		return strFormat;
	}

	String16 String16::format(const String16& strFormat) noexcept
	{
		return strFormat;
	}

	String String::format(const sl_char8* szFormat) noexcept
	{
		return szFormat;
	}

	String16 String16::format(const sl_char16* szFormat) noexcept
	{
		return szFormat;
	}

	String String::argBy(const Variant* params, sl_size nParams) const noexcept
	{
		return formatBy(*this, params, nParams);
	}

	String16 String16::argBy(const Variant* params, sl_size nParams) const noexcept
	{
		return formatBy(*this, params, nParams);
	}

	String Atomic<String>::argBy(const Variant* params, sl_size nParams) const noexcept
	{
		return String::formatBy(*this, params, nParams);
	}

	String16 Atomic<String16>::argBy(const Variant* params, sl_size nParams) const noexcept
	{
		return String16::formatBy(*this, params, nParams);
	}


	int Compare<String>::operator()(const String& a, const String& b) const noexcept
	{
		return a.compare(b);
	}

	int Compare<String16>::operator()(const String16& a, const String16& b) const noexcept
	{
		return a.compare(b);
	}

	int Compare<AtomicString>::operator()(const AtomicString& a, const AtomicString& b) const noexcept
	{
		return a.compare(b);
	}

	int Compare<AtomicString16>::operator()(const AtomicString16& a, const AtomicString16& b) const noexcept
	{
		return a.compare(b);
	}


	sl_bool Equals<String>::operator()(const String& a, const String& b) const noexcept
	{
		return a.equals(b);
	}

	sl_bool Equals<String16>::operator()(const String16& a, const String16& b) const noexcept
	{
		return a.equals(b);
	}

	sl_bool Equals<AtomicString>::operator()(const AtomicString& a, const AtomicString& b) const noexcept
	{
		return a.equals(b);
	}

	sl_bool Equals<AtomicString16>::operator()(const AtomicString16& a, const AtomicString16& b) const noexcept
	{
		return a.equals(b);
	}


	sl_size Hash<String>::operator()(const String& v) const noexcept
	{
		return v.getHashCode();
	}

	sl_size Hash<String16>::operator()(const String16& v) const noexcept
	{
		return v.getHashCode();
	}

	sl_size Hash<AtomicString>::operator()(const AtomicString& v) const noexcept
	{
		return v.getHashCode();
	}

	sl_size Hash<AtomicString16>::operator()(const AtomicString16& v) const noexcept
	{
		return v.getHashCode();
	}


	int CompareIgnoreCaseString::operator()(const String& a, const String& b) const noexcept
	{
		return a.compareIgnoreCase(b);
	}

	int CompareIgnoreCaseString16::operator()(const String16& a, const String16& b) const noexcept
	{
		return a.compareIgnoreCase(b);
	}


	sl_bool EqualsIgnoreCaseString::operator()(const String& a, const String& b) const noexcept
	{
		return a.equalsIgnoreCase(b);
	}

	sl_bool EqualsIgnoreCaseString16::operator()(const String16& a, const String16& b) const noexcept
	{
		return a.equalsIgnoreCase(b);
	}


	sl_size HashIgnoreCaseString::operator()(const String& v) const noexcept
	{
		return v.getHashCodeIgnoreCase();
	}

	sl_size HashIgnoreCaseString16::operator()(const String16& v) const noexcept
	{
		return v.getHashCodeIgnoreCase();
	}

	
	signed char Cast<String, signed char>::operator()(const String& v) const noexcept
	{
		return (signed char)(v.parseInt32());
	}

	signed char Cast<String16, signed char>::operator()(const String16& v) const noexcept
	{
		return (signed char)(v.parseInt32());
	}
	
	unsigned char Cast<String, unsigned char>::operator()(const String& v) const noexcept
	{
		return (unsigned char)(v.parseUint32());
	}
	
	unsigned char Cast<String16, unsigned char>::operator()(const String16& v) const noexcept
	{
		return (unsigned char)(v.parseUint32());
	}

	short Cast<String, short>::operator()(const String& v) const noexcept
	{
		return (short)(v.parseInt32());
	}
	
	short Cast<String16, short>::operator()(const String16& v) const noexcept
	{
		return (short)(v.parseInt32());
	}
	
	unsigned short Cast<String, unsigned short>::operator()(const String& v) const noexcept
	{
		return (unsigned short)(v.parseUint32());
	}
	
	unsigned short Cast<String16, unsigned short>::operator()(const String16& v) const noexcept
	{
		return (unsigned short)(v.parseUint32());
	}
	
	int Cast<String, int>::operator()(const String& v) const noexcept
	{
		return (int)(v.parseInt32());
	}
	
	int Cast<String16, int>::operator()(const String16& v) const noexcept
	{
		return (int)(v.parseInt32());
	}
	
	unsigned int Cast<String, unsigned int>::operator()(const String& v) const noexcept
	{
		return (unsigned int)(v.parseUint32());
	}
	
	unsigned int Cast<String16, unsigned int>::operator()(const String16& v) const noexcept
	{
		return (unsigned int)(v.parseUint32());
	}

	long Cast<String, long>::operator()(const String& v) const noexcept
	{
		return (long)(v.parseInt32());
	}
	
	long Cast<String16, long>::operator()(const String16& v) const noexcept
	{
		return (long)(v.parseInt32());
	}
	
	unsigned long Cast<String, unsigned long>::operator()(const String& v) const noexcept
	{
		return (unsigned long)(v.parseUint32());
	}
	
	unsigned long Cast<String16, unsigned long>::operator()(const String16& v) const noexcept
	{
		return (unsigned long)(v.parseUint32());
	}
	
	sl_int64 Cast<String, sl_int64>::operator()(const String& v) const noexcept
	{
		return v.parseInt64();
	}
	
	sl_int64 Cast<String16, sl_int64>::operator()(const String16& v) const noexcept
	{
		return v.parseInt64();
	}
	
	sl_uint64 Cast<String, sl_uint64>::operator()(const String& v) const noexcept
	{
		return v.parseUint64();
	}
	
	sl_uint64 Cast<String16, sl_uint64>::operator()(const String16& v) const noexcept
	{
		return v.parseUint64();
	}
	
	float Cast<String, float>::operator()(const String& v) const noexcept
	{
		return v.parseFloat();
	}
	
	float Cast<String16, float>::operator()(const String16& v) const noexcept
	{
		return v.parseFloat();
	}
	
	double Cast<String, double>::operator()(const String& v) const noexcept
	{
		return v.parseDouble();
	}
	
	double Cast<String16, double>::operator()(const String16& v) const noexcept
	{
		return v.parseDouble();
	}
	
	std::string Cast<String, std::string>::operator()(const String& v) const noexcept
	{
		return v.toStd();
	}
	
	std::u16string Cast<String16, std::u16string>::operator()(const String16& v) const noexcept
	{
		return v.toStd();
	}
	
	
	String Cast<signed char, String>::operator()(signed char v) const noexcept
	{
		return String::fromInt32(v);
	}
	
	String16 Cast<signed char, String16>::operator()(signed char v) const noexcept
	{
		return String16::fromInt32(v);
	}
	
	String Cast<unsigned char, String>::operator()(unsigned char v) const noexcept
	{
		return String::fromUint32(v);
	}
	
	String16 Cast<unsigned char, String16>::operator()(unsigned char v) const noexcept
	{
		return String16::fromUint32(v);
	}
	
	String Cast<short, String>::operator()(short v) const noexcept
	{
		return String::fromInt32(v);
	}
	
	String16 Cast<short, String16>::operator()(short v) const noexcept
	{
		return String16::fromInt32(v);
	}
	
	String Cast<unsigned short, String>::operator()(unsigned short v) const noexcept
	{
		return String::fromUint32(v);
	}
	
	String16 Cast<unsigned short, String16>::operator()(unsigned short v) const noexcept
	{
		return String16::fromUint32(v);
	}
	
	String Cast<int, String>::operator()(int v) const noexcept
	{
		return String::fromInt32((sl_int32)v);
	}
	
	String16 Cast<int, String16>::operator()(int v) const noexcept
	{
		return String16::fromInt32((sl_int32)v);
	}
	
	String Cast<unsigned int, String>::operator()(unsigned int v) const noexcept
	{
		return String::fromUint32((sl_uint32)v);
	}
	
	String16 Cast<unsigned int, String16>::operator()(unsigned int v) const noexcept
	{
		return String16::fromUint32((sl_uint32)v);
	}
	
	String Cast<long, String>::operator()(long v) const noexcept
	{
		return String::fromInt32((sl_int32)v);
	}
	
	String16 Cast<long, String16>::operator()(long v) const noexcept
	{
		return String16::fromInt32((sl_int32)v);
	}
	
	String Cast<unsigned long, String>::operator()(unsigned long v) const noexcept
	{
		return String::fromUint32((sl_uint32)v);
	}
	
	String16 Cast<unsigned long, String16>::operator()(unsigned long v) const noexcept
	{
		return String16::fromUint32((sl_uint32)v);
	}
	
	String Cast<sl_int64, String>::operator()(sl_int64 v) const noexcept
	{
		return String::fromInt64(v);
	}
	
	String16 Cast<sl_int64, String16>::operator()(sl_int64 v) const noexcept
	{
		return String16::fromInt64(v);
	}
	
	String Cast<sl_uint64, String>::operator()(sl_uint64 v) const noexcept
	{
		return String::fromUint64(v);
	}
	
	String16 Cast<sl_uint64, String16>::operator()(sl_uint64 v) const noexcept
	{
		return String16::fromUint64(v);
	}
	
	String Cast<float, String>::operator()(float v) const noexcept
	{
		return String::fromFloat(v);
	}
	
	String16 Cast<float, String16>::operator()(float v) const noexcept
	{
		return String16::fromFloat(v);
	}
	
	String Cast<double, String>::operator()(double v) const noexcept
	{
		return String::fromDouble(v);
	}
	
	String16 Cast<double, String16>::operator()(double v) const noexcept
	{
		return String16::fromDouble(v);
	}

	
	String Cast<Variant, String>::operator()(const Variant& var) const noexcept
	{
		return var.getString();
	}

	String16 Cast<Variant, String16>::operator()(const Variant& var) const noexcept
	{
		return var.getString();
	}

	String Cast<Json, String>::operator()(const Json& json) const noexcept
	{
		return json.getString();
	}
	
	String16 Cast<Json, String16>::operator()(const Json& json) const noexcept
	{
		return json.getString();
	}

	
/**********************************************************
				String Buffer
**********************************************************/

	StringBuffer::StringBuffer() noexcept
	{
		m_len = 0;
	}
	
	StringBuffer::~StringBuffer() noexcept
	{
	}

	StringBuffer16::StringBuffer16() noexcept
	{
		m_len = 0;
	}
	
	StringBuffer16::~StringBuffer16() noexcept
	{
	}

	sl_size StringBuffer::getLength() const noexcept
	{
		return m_len;
	}

	sl_size StringBuffer16::getLength() const noexcept
	{
		return m_len;
	}
	
	sl_bool StringBuffer::add(const String& str) noexcept
	{
		sl_size len = str.getLength();
		if (len == 0) {
			return sl_true;
		}
		StringData data;
		data.sz8 = str.getData();
		data.len = len;
		data.str8 = str;
		if (m_queue.push_NoLock(data)) {
			m_len += len;
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool StringBuffer16::add(const String16& str) noexcept
	{
		sl_size len = str.getLength();
		if (len == 0) {
			return sl_true;
		}
		StringData data;
		data.sz16 = str.getData();
		data.len = len;
		data.str16 = str;
		if (m_queue.push_NoLock(data)) {
			m_len += len;
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool StringBuffer::add(const StringData& data) noexcept
	{
		sl_size len = data.len;
		if (len == 0) {
			return sl_true;
		}
		if (data.sz8) {
			if (m_queue.push_NoLock(data)) {
				m_len += len;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool StringBuffer16::add(const StringData& data) noexcept
	{
		sl_size len = data.len;
		if (len == 0) {
			return sl_true;
		}
		if (data.sz16) {
			if (m_queue.push_NoLock(data)) {
				m_len += len;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool StringBuffer::addStatic(const sl_char8* buf, sl_size length) noexcept
	{
		StringData data;
		data.sz8 = buf;
		data.len = length;
		return add(data);
	}

	sl_bool StringBuffer16::addStatic(const sl_char16* buf, sl_size length) noexcept
	{
		StringData data;
		data.sz16 = buf;
		data.len = length;
		return add(data);
	}

	void StringBuffer::link(StringBuffer& buf) noexcept
	{
		m_len += buf.m_len;
		buf.m_len = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void StringBuffer16::link(StringBuffer16& buf) noexcept
	{
		m_len += buf.m_len;
		buf.m_len = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void StringBuffer::clear() noexcept
	{
		m_queue.removeAll_NoLock();
		m_len = 0;
	}

	void StringBuffer16::clear() noexcept
	{
		m_queue.removeAll_NoLock();
		m_len = 0;
	}

	String StringBuffer::merge() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return String::getEmpty();
		}
		Link<StringData>* front = m_queue.getFront();
		sl_size total = m_len;
		String ret = String::allocate(total);
		if (ret.isNotEmpty()) {
			sl_char8* buf = (sl_char8*)(ret.getData());
			sl_size offset = 0;
			Link<StringData>* item = front;
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
	
	String16 StringBuffer16::merge() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return String16::getEmpty();
		}
		Link<StringData>* front = m_queue.getFront();
		sl_size total = m_len;
		String16 ret = String16::allocate(total);
		if (ret.isNotEmpty()) {
			sl_char16* buf = (sl_char16*)(ret.getData());
			sl_size offset = 0;
			Link<StringData>* item = front;
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
	
	Memory StringBuffer::mergeToMemory() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return sl_null;
		}
		Link<StringData>* front = m_queue.getFront();
		sl_size total = m_len;
		Memory ret = Memory::create(total);
		if (ret.isNotNull()) {
			sl_char8* buf = (sl_char8*)(ret.getData());
			sl_size offset = 0;
			Link<StringData>* item = front;
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
	
	Memory StringBuffer16::mergeToMemory() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return sl_null;
		}
		Link<StringData>* front = m_queue.getFront();
		sl_size total = m_len;
		Memory ret = Memory::create(total * 2);
		if (ret.isNotNull()) {
			sl_char16* buf = (sl_char16*)(ret.getData());
			sl_size offset = 0;
			Link<StringData>* item = front;
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
	
	String StringBuffer::join(const String& delimiter, const List<String>& list, sl_size startIndex, sl_size count)
	{
		if (count == 0) {
			return sl_null;
		}
		StringBuffer buf;
		ListLocker<String> items(list);
		sl_size endIndex = items.count;
		if (endIndex <= startIndex) {
			return sl_null;
		}
		if (endIndex - startIndex > count) {
			endIndex = startIndex + count;
		}
		count = SLIB_MIN(count, items.count);
		for (sl_size i = startIndex; i < endIndex; i++) {
			if (i > startIndex) {
				buf.add(delimiter);
			}
			buf.add(items[i]);
		}
		return buf.merge();
	}
	
	String16 StringBuffer16::join(const String16& delimiter, const List<String16>& list, sl_size startIndex, sl_size count)
	{
		if (count == 0) {
			return sl_null;
		}
		StringBuffer buf;
		ListLocker<String16> items(list);
		sl_size endIndex = items.count;
		if (endIndex <= startIndex) {
			return sl_null;
		}
		if (endIndex - startIndex > count) {
			endIndex = startIndex + count;
		}
		count = SLIB_MIN(count, items.count);
		for (sl_size i = startIndex; i < endIndex; i++) {
			if (i > startIndex) {
				buf.add(delimiter);
			}
			buf.add(items[i]);
		}
		return buf.merge();
	}
	
}
