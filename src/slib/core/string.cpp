/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/base.h"
#include "slib/core/endian.h"
#include "slib/core/mio.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringStorage)
	
	StringStorage::StringStorage() noexcept : data8(0), length(0)
	{
	}
	
	enum STRING_CONTAINER_TYPES
	{
		STRING_CONTAINER_TYPE_NORMAL = 0,
		STRING_CONTAINER_TYPE_STD = 10,
		STRING_CONTAINER_TYPE_REF = 11
	};

	namespace priv
	{
		namespace string
		{
			
			const ConstContainer g_null = {sl_null, 0};

			const sl_char8 g_empty_buf[] = {0, 0};
			const StringContainer g_empty_container = {const_cast<sl_char8*>(g_empty_buf), 0, 0, STRING_CONTAINER_TYPE_NORMAL, -1};
			const ConstContainer g_empty = {const_cast<StringContainer*>(&g_empty_container), 0};

			const ConstContainer16 g_null16 = {sl_null, 0};
			
			const sl_char16 g_empty_buf16[] = {0, 0};
			const StringContainer16 g_empty_container16 = {const_cast<sl_char16*>(g_empty_buf16), 0, 0, STRING_CONTAINER_TYPE_NORMAL, -1};
			const ConstContainer16 g_empty16 = {const_cast<StringContainer16*>(&g_empty_container16), 0};
			
			
			class Container_std : public StringContainer
			{
			public:
				std::string str;
				
			public:
				Container_std(const std::string& _str): str(_str)
				{
				}
				
				Container_std(std::string&& _str): str(Move(_str))
				{
				}
				
			};
			
			class Container_ref : public StringContainer
			{
			public:
				Ref<Referable> obj;
				
			public:
				Container_ref(const Ref<Referable>& _obj): obj(_obj)
				{
				}
				
			};
			
			class Container16_std : public StringContainer16
			{
			public:
				std::u16string str;
				
			public:
				Container16_std(const std::u16string& _str): str(_str)
				{
				}
				
				Container16_std(std::u16string&& _str): str(Move(_str))
				{
				}
				
			};
			
			class Container16_ref : public StringContainer16
			{
			public:
				Ref<Referable> obj;
				
			public:
				Container16_ref(const Ref<Referable>& _obj): obj(_obj)
				{
				}
				
			};

			
			SLIB_INLINE static StringContainer* alloc(sl_size len) noexcept
			{
				if (len == 0) {
					return priv::string::g_empty.container;
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
			
			SLIB_INLINE static StringContainer16* alloc16(sl_size len) noexcept
			{
				if (len == 0) {
					return priv::string::g_empty16.container;
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
			
			SLIB_INLINE static StringContainer* alloc_static(const sl_char8* sz, sl_size len) noexcept
			{
				if (len == 0) {
					return priv::string::g_empty.container;
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
			
			SLIB_INLINE static StringContainer16* alloc16_static(const sl_char16* sz, sl_size len) noexcept
			{
				if (len == 0) {
					return priv::string::g_empty16.container;
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
			SLIB_INLINE static StringContainer* alloc_std(T&& str) noexcept
			{
				sl_size len = (sl_size)(str.length());
				if (len == 0) {
					return priv::string::g_empty.container;
				}
				if (len < 40) {
					StringContainer* container = priv::string::alloc(len);
					if (container) {
						Base::copyMemory(container->sz, str.c_str(), len);
						return container;
					}
				} else {
					priv::string::Container_std* container = (priv::string::Container_std*)(Base::createMemory(sizeof(priv::string::Container_std)));
					if (container) {
						new (container) priv::string::Container_std(Forward<T>(str));
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
			SLIB_INLINE static StringContainer16* alloc16_std(T&& str) noexcept
			{
				sl_size len = (sl_size)(str.length());
				if (len == 0) {
					return priv::string::g_empty16.container;
				}
				if (len < 20) {
					StringContainer16* container = priv::string::alloc16(len);
					if (container) {
						Base::copyMemory(container->sz, str.c_str(), len * sizeof(sl_char16));
						return container;
					}
				} else {
					priv::string::Container16_std* container = (priv::string::Container16_std*)(Base::createMemory(sizeof(priv::string::Container16_std)));
					if (container) {
						new (container) priv::string::Container16_std(Forward<T>(str));
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
			
			SLIB_INLINE static StringContainer* alloc_ref(const Ref<Referable>& obj, const sl_char8* sz, sl_size len) noexcept
			{
				if (len == 0) {
					return priv::string::g_empty.container;
				}
				priv::string::Container_ref* container = (priv::string::Container_ref*)(Base::createMemory(sizeof(priv::string::Container_ref)));
				if (container) {
					new (container) priv::string::Container_ref(obj);
					container->sz = (sl_char8*)(sz);
					container->len = len;
					container->hash = 0;
					container->type = STRING_CONTAINER_TYPE_REF;
					container->ref = 1;
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer16* alloc16_ref(const Ref<Referable>& obj, const sl_char16* sz, sl_size len) noexcept
			{
				if (len == 0) {
					return priv::string::g_empty16.container;
				}
				priv::string::Container16_ref* container = (priv::string::Container16_ref*)(Base::createMemory(sizeof(priv::string::Container16_ref)));
				if (container) {
					new (container) priv::string::Container16_ref(obj);
					container->sz = (sl_char16*)(sz);
					container->len = len;
					container->hash = 0;
					container->type = STRING_CONTAINER_TYPE_REF;
					container->ref = 1;
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer* create(sl_char8 ch, sl_size nRepeatCount) noexcept
			{
				StringContainer* container = priv::string::alloc(nRepeatCount);
				if (container) {
					Base::resetMemory(container->sz, ch, nRepeatCount);
				}
				return container;
			}
			
			SLIB_INLINE static StringContainer16* create16(sl_char16 ch, sl_size nRepeatCount) noexcept
			{
				StringContainer16* container = priv::string::alloc16(nRepeatCount);
				if (container) {
					Base::resetMemory2((sl_uint16*)(container->sz), ch, nRepeatCount);
				}
				return container;
			}
			
			SLIB_INLINE static StringContainer* create(const sl_char8* utf8, sl_reg lenUtf8) noexcept
			{
				if (utf8) {
					if (lenUtf8 < 0) {
						lenUtf8 = Base::getStringLength(utf8);
					}
					sl_size len = lenUtf8;
					StringContainer* container = priv::string::alloc(len);
					if (container) {
						Base::copyMemory(container->sz, utf8, len);
					}
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer16* create16(const sl_char8* utf8, sl_reg lenUtf8) noexcept
			{
				if (utf8) {
					if (lenUtf8 < 0) {
						lenUtf8 = Base::getStringLength(utf8);
					}
					sl_size len = Charsets::utf8ToUtf16(utf8, lenUtf8, sl_null, -1);
					StringContainer16* container = priv::string::alloc16(len);
					if (container && len > 0) {
						Charsets::utf8ToUtf16(utf8, lenUtf8, container->sz, len);
						container->sz[len] = 0;
					}
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer* create(const sl_char16* utf16, sl_reg lenUtf16) noexcept
			{
				if (utf16) {
					if (lenUtf16 < 0) {
						lenUtf16 = Base::getStringLength2(utf16);
					}
					sl_size len = Charsets::utf16ToUtf8(utf16, lenUtf16, sl_null, -1);
					StringContainer* container = priv::string::alloc(len);
					if (container && len > 0) {
						Charsets::utf16ToUtf8(utf16, lenUtf16, container->sz, len);
						container->sz[len] = 0;
					}
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer16* create16(const sl_char16* utf16, sl_reg lenUtf16) noexcept
			{
				if (utf16) {
					if (lenUtf16 < 0) {
						lenUtf16 = Base::getStringLength2(utf16);
					}
					sl_size len = lenUtf16;
					StringContainer16* container = priv::string::alloc16(len);
					if (container) {
						Base::copyMemory(container->sz, utf16, len*sizeof(sl_char16));
					}
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer* create(const sl_char32* utf32, sl_reg lenUtf32) noexcept
			{
				if (utf32) {
					if (lenUtf32 < 0) {
						lenUtf32 = Base::getStringLength4(utf32);
					}
					sl_size len = Charsets::utf32ToUtf8(utf32, lenUtf32, sl_null, -1);
					StringContainer* container = priv::string::alloc(len);
					if (container && len > 0) {
						Charsets::utf32ToUtf8(utf32, lenUtf32, container->sz, len);
						container->sz[len] = 0;
					}
					return container;
				}
				return sl_null;
			}
			
			SLIB_INLINE static StringContainer16* create16(const sl_char32* utf32, sl_reg lenUtf32) noexcept
			{
				if (utf32) {
					if (lenUtf32 < 0) {
						lenUtf32 = Base::getStringLength4(utf32);
					}
					sl_size len = Charsets::utf32ToUtf16(utf32, lenUtf32, sl_null, -1);
					StringContainer16* container = priv::string::alloc16(len);
					if (container && len > 0) {
						Charsets::utf32ToUtf16(utf32, lenUtf32, container->sz, len);
						container->sz[len] = 0;
					}
					return container;
				}
				return sl_null;
			}
			
			
			SLIB_INLINE static StringContainer* merge8_8(const sl_char8* s1, sl_reg len1, const sl_char8* s2, sl_reg len2) noexcept
			{
				if (len1 < 0) {
					len1 = Base::getStringLength(s1);
				}
				if (len2 < 0) {
					len2 = Base::getStringLength(s2);
				}
				sl_size len = len1 + len2;
				StringContainer* s = priv::string::alloc(len);
				if (s && len > 0) {
					Base::copyMemory(s->sz, s1, len1);
					Base::copyMemory(s->sz + len1, s2, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer16* merge16_16(const sl_char16* s1, sl_reg len1, const sl_char16* s2, sl_reg len2) noexcept
			{
				if (len1 < 0) {
					len1 = Base::getStringLength2(s1);
				}
				if (len2 < 0) {
					len2 = Base::getStringLength2(s2);
				}
				sl_size len = len1 + len2;
				StringContainer16* s = priv::string::alloc16(len);
				if (s && len > 0) {
					Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
					Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer* merge8_16(const sl_char8* s1, sl_reg len1, const sl_char16* s2_u16, sl_reg len2_u16) noexcept
			{
				if (len1 < 0) {
					len1 = Base::getStringLength(s1);
				}
				if (len2_u16 < 0) {
					len2_u16 = Base::getStringLength2(s2_u16);
				}
				sl_size len2 = Charsets::utf16ToUtf8(s2_u16, len2_u16, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer* s = priv::string::alloc(len);
				if (s && len > 0) {
					Base::copyMemory(s->sz, s1, len1);
					Charsets::utf16ToUtf8(s2_u16, len2_u16, s->sz + len1, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer* merge8_16(const sl_char16* s1_u16, sl_reg len1_u16, const sl_char8* s2, sl_reg len2) noexcept
			{
				if (len1_u16 < 0) {
					len1_u16 = Base::getStringLength2(s1_u16);
				}
				if (len2 < 0) {
					len2 = Base::getStringLength(s2);
				}
				sl_size len1 = Charsets::utf16ToUtf8(s1_u16, len1_u16, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer* s = priv::string::alloc(len);
				if (s && len > 0) {
					Charsets::utf16ToUtf8(s1_u16, len1_u16, s->sz, len1);
					Base::copyMemory(s->sz + len1, s2, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer* merge8_32(const sl_char8* s1, sl_reg len1, const sl_char32* s2_u32, sl_reg len2_u32) noexcept
			{
				if (len1 < 0) {
					len1 = Base::getStringLength(s1);
				}
				if (len2_u32 < 0) {
					len2_u32 = Base::getStringLength4(s2_u32);
				}
				sl_size len2 = Charsets::utf32ToUtf8(s2_u32, len2_u32, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer* s = priv::string::alloc(len);
				if (s && len > 0) {
					Base::copyMemory(s->sz, s1, len1*sizeof(sl_char8));
					Charsets::utf32ToUtf8(s2_u32, len2_u32, s->sz + len1, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer* merge8_32(const sl_char32* s1_u32, sl_reg len1_u32, const sl_char8* s2, sl_reg len2) noexcept
			{
				if (len1_u32 < 0) {
					len1_u32 = Base::getStringLength4(s1_u32);
				}
				if (len2 < 0) {
					len2 = Base::getStringLength(s2);
				}
				sl_size len1 = Charsets::utf32ToUtf8(s1_u32, len1_u32, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer* s = priv::string::alloc(len);
				if (s && len > 0) {
					Charsets::utf32ToUtf8(s1_u32, len1_u32, s->sz, len1);
					Base::copyMemory(s->sz + len1, s2, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer16* merge16_8(const sl_char16* s1, sl_reg len1, const sl_char8* s2_u8, sl_reg len2_u8) noexcept
			{
				if (len1 < 0) {
					len1 = Base::getStringLength2(s1);
				}
				if (len2_u8 < 0) {
					len2_u8 = Base::getStringLength(s2_u8);
				}
				sl_size len2 = Charsets::utf8ToUtf16(s2_u8, len2_u8, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer16* s = priv::string::alloc16(len);
				if (s && len > 0) {
					Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
					Charsets::utf8ToUtf16(s2_u8, len2_u8, s->sz + len1, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer16* merge16_8(const sl_char8* s1_u8, sl_reg len1_u8, const sl_char16* s2, sl_reg len2) noexcept
			{
				if (len1_u8 < 0) {
					len1_u8 = Base::getStringLength(s1_u8);
				}
				if (len2 < 0) {
					len2 = Base::getStringLength2(s2);
				}
				sl_size len1 = Charsets::utf8ToUtf16(s1_u8, len1_u8, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer16* s = priv::string::alloc16(len);
				if (s && len > 0) {
					Charsets::utf8ToUtf16(s1_u8, len1_u8, s->sz, len1);
					Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer16* merge16_32(const sl_char16* s1, sl_reg len1, const sl_char32* s2_u32, sl_reg len2_u32) noexcept
			{
				if (len1 < 0) {
					len1 = Base::getStringLength2(s1);
				}
				if (len2_u32 < 0) {
					len2_u32 = Base::getStringLength4(s2_u32);
				}
				sl_size len2 = Charsets::utf32ToUtf16(s2_u32, len2_u32, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer16* s = priv::string::alloc16(len);
				if (s && len > 0) {
					Base::copyMemory(s->sz, s1, len1*sizeof(sl_char16));
					Charsets::utf32ToUtf16(s2_u32, len2_u32, s->sz + len1, len2);
					s->sz[len] = 0;
				}
				return s;
			}
			
			SLIB_INLINE static StringContainer16* merge16_32(const sl_char32* s1_u32, sl_reg len1_u32, const sl_char16* s2, sl_reg len2) noexcept
			{
				if (len1_u32 < 0) {
					len1_u32 = Base::getStringLength4(s1_u32);
				}
				if (len2 < 0) {
					len2 = Base::getStringLength2(s2);
				}
				sl_size len1 = Charsets::utf32ToUtf16(s1_u32, len1_u32, sl_null, -1);
				sl_size len = len1 + len2;
				StringContainer16* s = priv::string::alloc16(len);
				if (s && len > 0) {
					Charsets::utf32ToUtf16(s1_u32, len1_u32, s->sz, len1);
					Base::copyMemory(s->sz + len1, s2, len2*sizeof(sl_char16));
					s->sz[len] = 0;
				}
				return s;
			}
			
		}
	}

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

	SLIB_INLINE sl_reg StringContainer::decreaseReference() noexcept
	{
		if (ref > 0) {
			sl_reg nRef = Base::interlockedDecrement(&ref);
			if (nRef == 0) {
				if (type == STRING_CONTAINER_TYPE_STD) {
					priv::string::Container_std* container = static_cast<priv::string::Container_std*>(this);
					container->priv::string::Container_std::~Container_std();
				} else if (type == STRING_CONTAINER_TYPE_REF) {
					priv::string::Container_ref* container = static_cast<priv::string::Container_ref*>(this);
					container->priv::string::Container_ref::~Container_ref();
				}
				Base::freeMemory(this);
			}
			return nRef;
		}
		return 1;
	}
	
	SLIB_INLINE sl_reg StringContainer16::decreaseReference() noexcept
	{
		if (ref > 0) {
			sl_reg nRef = Base::interlockedDecrement(&ref);
			if (nRef == 0) {
				if (type == STRING_CONTAINER_TYPE_STD) {
					priv::string::Container16_std* container = static_cast<priv::string::Container16_std*>(this);
					container->priv::string::Container16_std::~Container16_std();
				} else if (type == STRING_CONTAINER_TYPE_REF) {
					priv::string::Container16_ref* container = static_cast<priv::string::Container16_ref*>(this);
					container->priv::string::Container16_ref::~Container16_ref();
				}
				Base::freeMemory(this);
			}
			return nRef;
		}
		return 1;
	}

	SLIB_INLINE StringContainer* Atomic<String>::_retainContainer() const noexcept
	{
		if (!m_container) {
			return sl_null;
		}
		m_lock.lock();
		StringContainer* container = m_container;
		if (container) {
			container->increaseReference();
		}
		m_lock.unlock();
		return container;
	}

	SLIB_INLINE StringContainer16* Atomic<String16>::_retainContainer() const noexcept
	{
		if (!m_container) {
			return sl_null;
		}
		m_lock.lock();
		StringContainer16* container = m_container;
		if (container) {
			container->increaseReference();
		}
		m_lock.unlock();
		return container;
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
		m_lock.lock();
		StringContainer* before = m_container;
		m_container = container;
		m_lock.unlock();
		if (before) {
			before->decreaseReference();
		}
	}

	SLIB_INLINE void Atomic<String16>::_replaceContainer(StringContainer16* container) noexcept
	{
		m_lock.lock();
		StringContainer16* before = m_container;
		m_container = container;
		m_lock.unlock();
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


	String::String(const StringView& src) noexcept
	{
		m_container = priv::string::create(src.getUnsafeData(), src.getUnsafeLength());
	}

	String16::String16(const StringView16& src) noexcept
	{
		m_container = priv::string::create16(src.getUnsafeData(), src.getUnsafeLength());
	}

	Atomic<String>::Atomic(const StringView& src) noexcept
	{
		m_container = priv::string::create(src.getUnsafeData(), src.getUnsafeLength());
	}

	Atomic<String16>::Atomic(const StringView16& src) noexcept
	{
		m_container = priv::string::create16(src.getUnsafeData(), src.getUnsafeLength());
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
		m_container = priv::string::create(ch, nRepeatCount);
	}

	String16::String16(sl_char16 ch, sl_size nRepeatCount) noexcept
	{
		m_container = priv::string::create16(ch, nRepeatCount);
	}

	Atomic<String>::Atomic(sl_char8 ch, sl_size nRepeatCount) noexcept
	{
		m_container = priv::string::create(ch, nRepeatCount);
	}

	Atomic<String16>::Atomic(sl_char16 ch, sl_size nRepeatCount) noexcept
	{
		m_container = priv::string::create16(ch, nRepeatCount);
	}


	String::String(const sl_char8* str) noexcept
	{
		m_container = priv::string::create(str, -1);
	}

	String::String(const sl_char8* str, sl_reg length) noexcept
	{
		m_container = priv::string::create(str, length);
	}

	String16::String16(const sl_char16* str) noexcept
	{
		m_container = priv::string::create16(str, -1);
	}

	String16::String16(const sl_char16* str, sl_reg length) noexcept
	{
		m_container = priv::string::create16(str, length);
	}

	Atomic<String>::Atomic(const sl_char8* str) noexcept
	{
		m_container = priv::string::create(str, -1);
	}

	Atomic<String>::Atomic(const sl_char8* str, sl_reg length) noexcept
	{
		m_container = priv::string::create(str, length);
	}

	Atomic<String16>::Atomic(const sl_char16* str) noexcept
	{
		m_container = priv::string::create16(str, -1);
	}

	Atomic<String16>::Atomic(const sl_char16* str, sl_reg length) noexcept
	{
		m_container = priv::string::create16(str, length);
	}


	String::String(const std::string& str) noexcept
	{
		m_container = priv::string::alloc_std(str);
	}

	String::String(std::string&& str) noexcept
	{
		m_container = priv::string::alloc_std(Move(str));
	}

	String16::String16(const std::u16string& str) noexcept
	{
		m_container = priv::string::alloc16_std(str);
	}

	String16::String16(std::u16string&& str) noexcept
	{
		m_container = priv::string::alloc16_std(Move(str));
	}

	Atomic<String>::Atomic(const std::string& str) noexcept
	{
		m_container = priv::string::alloc_std(str);
	}

	Atomic<String>::Atomic(std::string&& str) noexcept
	{
		m_container = priv::string::alloc_std(Move(str));
	}

	Atomic<String16>::Atomic(const std::u16string& str) noexcept
	{
		m_container = priv::string::alloc16_std(str);
	}

	Atomic<String16>::Atomic(std::u16string&& str) noexcept
	{
		m_container = priv::string::alloc16_std(Move(str));
	}


	String String::allocate(sl_size len) noexcept
	{
		return priv::string::alloc(len);
	}

	String16 String16::allocate(sl_size len) noexcept
	{
		return priv::string::alloc16(len);
	}

	String String::create(const String& str) noexcept
	{
		if (str.isNotNull()) {
			return priv::string::create(str.getData(), str.getLength());
		}
		return sl_null;
	}

	String16 String16::create(const String& str) noexcept
	{
		if (str.isNotNull()) {
			return priv::string::create16(str.getData(), str.getLength());
		}
		return sl_null;
	}

	String String::create(const String16& str) noexcept
	{
		if (str.isNotNull()) {
			return priv::string::create(str.getData(), str.getLength());
		}
		return sl_null;
	}

	String16 String16::create(const String16& str) noexcept
	{
		if (str.isNotNull()) {
			return priv::string::create16(str.getData(), str.getLength());
		}
		return sl_null;
	}

	String String::create(const StringView& str) noexcept
	{
		return priv::string::create(str.getUnsafeData(), str.getUnsafeLength());
	}

	String16 String16::create(const StringView& str) noexcept
	{
		return priv::string::create16(str.getUnsafeData(), str.getUnsafeLength());
	}

	String String::create(const StringView16& str) noexcept
	{
		return priv::string::create(str.getUnsafeData(), str.getUnsafeLength());
	}

	String16 String16::create(const StringView16& str) noexcept
	{
		return priv::string::create16(str.getUnsafeData(), str.getUnsafeLength());
	}


	String String::create(const char* str, sl_reg length) noexcept
	{
		return priv::string::create(str, length);
	}

	String16 String16::create(const char* str, sl_reg length) noexcept
	{
		return priv::string::create16(str, length);
	}

	String String::create(const wchar_t* str, sl_reg length) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::create((sl_char16*)str, length);
		} else {
			return priv::string::create((sl_char32*)str, length);
		}
	}

	String16 String16::create(const wchar_t* str, sl_reg length) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::create16((sl_char16*)str, length);
		} else {
			return priv::string::create16((sl_char32*)str, length);
		}
	}

	String String::create(const char16_t* str, sl_reg length) noexcept
	{
		return priv::string::create(str, length);
	}

	String16 String16::create(const char16_t* str, sl_reg length) noexcept
	{
		return priv::string::create16(str, length);
	}

	String String::create(const char32_t* str, sl_reg length) noexcept
	{
		return priv::string::create(str, length);
	}

	String16 String16::create(const char32_t* str, sl_reg length) noexcept
	{
		return priv::string::create16(str, length);
	}


	String String::create(const std::string& str) noexcept
	{
		return priv::string::create(str.c_str(), str.length());
	}

	String16 String16::create(const std::string& str) noexcept
	{
		return priv::string::create16(str.c_str(), str.length());
	}

	String String::create(const std::wstring& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String16 String16::create(const std::wstring& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String String::create(const std::u16string& str) noexcept
	{
		return priv::string::create(str.c_str(), str.length());
	}

	String16 String16::create(const std::u16string& str) noexcept
	{
		return priv::string::create16(str.c_str(), str.length());
	}

	String String::create(const std::u32string& str) noexcept
	{
		return priv::string::create(str.c_str(), str.length());
	}

	String16 String16::create(const std::u32string& str) noexcept
	{
		return priv::string::create16(str.c_str(), str.length());
	}


	String String::fromStatic(const sl_char8 *sz, sl_reg len) noexcept
	{
		if (sz) {
			if (len < 0) {
				len = Base::getStringLength(sz);
			}
			return priv::string::alloc_static(sz, len);
		}
		return sl_null;
	}

	String16 String16::fromStatic(const sl_char16 *sz, sl_reg len) noexcept
	{
		if (sz) {
			if (len < 0) {
				len = Base::getStringLength2(sz);
			}
			return priv::string::alloc16_static(sz, len);
		}
		return sl_null;
	}
	
	String String::fromRef(const Ref<Referable>& ref, const sl_char8* str, sl_reg len) noexcept
	{
		if (str) {
			if (len < 0) {
				len = Base::getStringLength(str);
			}
			return priv::string::alloc_ref(ref, str, len);
		}
		return sl_null;
	}
	
	String16 String16::fromRef(const Ref<Referable>& ref, const sl_char16* str, sl_reg len) noexcept
	{
		if (str) {
			if (len < 0) {
				len = Base::getStringLength2(str);
			}
			return priv::string::alloc16_ref(ref, str, len);
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
		return create((const sl_char8*)utf8, len);
	}

	String String::fromUtf8(const Memory& mem) noexcept
	{
		return fromUtf8(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf8(const void* utf8, sl_reg len) noexcept
	{
		return create((const sl_char8*)utf8, len);
	}

	String16 String16::fromUtf8(const Memory& mem) noexcept
	{
		return fromUtf8(mem.getData(), mem.getSize());
	}


	String String::fromUtf16(const sl_char16* utf16, sl_reg len) noexcept
	{
		return create(utf16, len);
	}

	String16 String16::fromUtf16(const sl_char16* utf16, sl_reg len) noexcept
	{
		return create(utf16, len);
	}

	String String::fromUtf32(const sl_char32* utf32, sl_reg len) noexcept
	{
		return create(utf32, len);
	}

	String16 String16::fromUtf32(const sl_char32* utf32, sl_reg len) noexcept
	{
		return create(utf32, len);
	}


	String String::fromUtf16BE(const void* utf16, sl_size size) noexcept
	{
		if (!utf16) {
			return sl_null;
		}
		if (!size) {
			return getEmpty();
		}
		sl_size n = Charsets::decode8_UTF16BE(utf16, size, sl_null, -1);
		if (n) {
			String str = allocate(n);
			if (str.isNotNull()) {
				Charsets::decode8_UTF16BE(utf16, size, str.getData(), n);
				return str;
			}
		}
		return sl_null;
	}

	String String::fromUtf16BE(const Memory& mem) noexcept
	{
		return fromUtf16BE(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf16BE(const void* _utf16, sl_size size) noexcept
	{
		if (!_utf16) {
			return sl_null;
		}
		sl_size len = size >> 1;
		if (!len) {
			return getEmpty();
		}
		if (((((sl_reg)(_utf16)) & 1) == 0) && Endian::isBE()) {
			return String16((sl_char16*)_utf16, len);
		}
		String16 str = allocate(len);
		if (str.isNotNull()) {
			sl_char16* utf16 = str.getData();
			const sl_uint8* c = (const sl_uint8*)_utf16;
			for (sl_size i = 0; i < len; i++) {
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


	String String::fromUtf16LE(const void* utf16, sl_size size) noexcept
	{
		if (!utf16) {
			return sl_null;
		}
		if (!size) {
			return getEmpty();
		}
		sl_size n = Charsets::decode8_UTF16LE(utf16, size, sl_null, -1);
		if (n) {
			String str = allocate(n);
			if (str.isNotNull()) {
				Charsets::decode8_UTF16LE(utf16, size, str.getData(), n);
				return str;
			}
		}
		return sl_null;
	}

	String String::fromUtf16LE(const Memory& mem) noexcept
	{
		return fromUtf16LE(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf16LE(const void* _utf16, sl_size size) noexcept
	{
		if (!_utf16) {
			return sl_null;
		}
		sl_size len = size >> 1;
		if (!len) {
			return getEmpty();
		}
		if ((((sl_reg)(_utf16)) & 1) == 0 && Endian::isLE()) {
			return String16((sl_char16*)_utf16, len);
		}
		String16 str = allocate(len);
		if (str.isNotNull()) {
			sl_char16* utf16 = str.getData();
			const sl_uint8* c = (const sl_uint8*)_utf16;
			for (sl_size i = 0; i < len; i++) {
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


	String String::fromUtf(const void* _buf, sl_size size) noexcept
	{
		sl_char8* buf = (sl_char8*)_buf;
		if (!buf) {
			return sl_null;
		}
		if (size == 0) {
			return getEmpty();
		}
		if (size >= 2) {
			if (buf[0] == (sl_char8)0xFF && buf[1] == (sl_char8)0xFE) {
				return fromUtf16LE(buf, size - 2);
			}
			if (buf[0] == (sl_char8)0xFE && buf[1] == (sl_char8)0xFF) {
				return fromUtf16BE(buf, size - 2);
			}
		}
		if (size >= 3) {
			if (buf[0] == (sl_char8)0xEF && buf[1] == (sl_char8)0xBB && buf[2] == (sl_char8)0xBF) {
				return create(buf, size - 3);
			}
		}
		return create(buf, size);
	}

	String String::fromUtf(const Memory& mem) noexcept
	{
		return fromUtf(mem.getData(), mem.getSize());
	}

	String16 String16::fromUtf(const void* _buf, sl_size size) noexcept
	{
		sl_char8* buf = (sl_char8*)_buf;
		if (!buf) {
			return sl_null;
		}
		if (size == 0) {
			return getEmpty();
		}
		if (size >= 2) {
			if (buf[0] == (sl_char8)0xFF && buf[1] == (sl_char8)0xFE) {
				return fromUtf16LE(buf, size - 2);
			}
			if (buf[0] == (sl_char8)0xFE && buf[1] == (sl_char8)0xFF) {
				return fromUtf16BE(buf, size - 2);
			}
		}
		if (size >= 3) {
			if (buf[0] == (sl_char8)0xEF && buf[1] == (sl_char8)0xBB && buf[2] == (sl_char8)0xBF) {
				return create(buf, size - 3);
			}
		}
		return create(buf, size);
	}

	String16 String16::fromUtf(const Memory& mem) noexcept
	{
		return fromUtf(mem.getData(), mem.getSize());
	}


	const String& String::from(const String& str) noexcept
	{
		return str;
	}

	String String::from(const AtomicString& str) noexcept
	{
		return str;
	}

	String String::from(const String16& str) noexcept
	{
		return create(str);
	}

	String String::from(const AtomicString16& str) noexcept
	{
		return create(str);
	}

	String String::from(const StringView& str) noexcept
	{
		return create(str);
	}

	String String::from(const StringView16& str) noexcept
	{
		return create(str);
	}

	String String::from(const StringParam& str) noexcept
	{
		return str.toString();
	}

	String String::from(const char* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String String::from(const wchar_t* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String String::from(const char16_t* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String String::from(const char32_t* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String String::from(const std::string& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String String::from(const std::wstring& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String String::from(const std::u16string& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String String::from(const std::u32string& str) noexcept
	{
		return create(str.c_str(), str.length());
	}


	String16 String16::from(const String& str) noexcept
	{
		return create(str);
	}

	String16 String16::from(const AtomicString& str) noexcept
	{
		return create(str);
	}

	const String16& String16::from(const String16& str) noexcept
	{
		return str;
	}

	String16 String16::from(const AtomicString16& str) noexcept
	{
		return str;
	}

	String16 String16::from(const StringView& str) noexcept
	{
		return create(str);
	}

	String16 String16::from(const StringView16& str) noexcept
	{
		return create(str);
	}

	String16 String16::from(const StringParam& str) noexcept
	{
		return str.toString16();
	}

	String16 String16::from(const char* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String16 String16::from(const wchar_t* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String16 String16::from(const char16_t* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String16 String16::from(const char32_t* str, sl_reg length) noexcept
	{
		return create(str, length);
	}

	String16 String16::from(const std::string& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String16 String16::from(const std::wstring& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String16 String16::from(const std::u16string& str) noexcept
	{
		return create(str.c_str(), str.length());
	}

	String16 String16::from(const std::u32string& str) noexcept
	{
		return create(str.c_str(), str.length());
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
		if (m_container != priv::string::g_empty.container) {
			_replaceContainer(priv::string::g_empty.container);
		}
	}

	void String16::setEmpty() noexcept
	{
		if (m_container != priv::string::g_empty16.container) {
			_replaceContainer(priv::string::g_empty16.container);
		}
	}

	void Atomic<String>::setEmpty() noexcept
	{
		if (m_container != priv::string::g_empty.container) {
			_replaceContainer(priv::string::g_empty.container);
		}
	}

	void Atomic<String16>::setEmpty() noexcept
	{
		if (m_container != priv::string::g_empty16.container) {
			_replaceContainer(priv::string::g_empty16.container);
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
		if (m_container && m_container != &(priv::string::g_empty_container)) {
			m_container->len = len;
		}
	}

	void String16::setLength(sl_size len) noexcept
	{
		if (m_container && m_container != &(priv::string::g_empty_container16)) {
			m_container->len = len;
		}
	}

	void String::setHashCode(sl_size hash) noexcept
	{
		if (m_container && m_container != &(priv::string::g_empty_container)) {
			m_container->hash = hash;
		}
	}

	void String16::setHashCode(sl_size hash) noexcept
	{
		if (m_container && m_container != &(priv::string::g_empty_container16)) {
			m_container->hash = hash;
		}
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


	sl_char8 String::operator[](sl_size index) const noexcept
	{
		return m_container->sz[index];
	}

	sl_char8& String::operator[](sl_size index) noexcept
	{
		return m_container->sz[index];
	}

	sl_char16 String16::operator[](sl_size index) const noexcept
	{
		return m_container->sz[index];
	}
	
	sl_char16& String16::operator[](sl_size index) noexcept
	{
		return m_container->sz[index];
	}
	
	
	std::string String::toStd() const noexcept
	{
		if (m_container) {
			if (m_container->type == STRING_CONTAINER_TYPE_STD) {
				return (static_cast<priv::string::Container_std*>(m_container))->str;
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
				return (static_cast<priv::string::Container16_std*>(m_container))->str;
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


	String& String::operator=(const StringView& other) noexcept
	{
		return *this = String(other);
	}

	String16& String16::operator=(const StringView16& other) noexcept
	{
		return *this = String16(other);
	}

	AtomicString& Atomic<String>::operator=(const StringView& other) noexcept
	{
		return *this = String(other);
	}

	AtomicString16& Atomic<String16>::operator=(const StringView16& other) noexcept
	{
		return *this = String16(other);
	}

	
	String& String::operator=(const sl_char8* str) noexcept
	{
		return *this = String(str);
	}

	String16& String16::operator=(const sl_char16* str) noexcept
	{
		return *this = String16(str);
	}

	AtomicString& Atomic<String>::operator=(const sl_char8* str) noexcept
	{
		return *this = String(str);
	}

	AtomicString16& Atomic<String16>::operator=(const sl_char16* str) noexcept
	{
		return *this = String16(str);
	}


	String& String::operator=(const std::string& str) noexcept
	{
		_replaceContainer(priv::string::alloc_std(str));
		return *this;
	}

	String& String::operator=(std::string&& str) noexcept
	{
		_replaceContainer(priv::string::alloc_std(Move(str)));
		return *this;
	}

	String16& String16::operator=(const std::u16string& str) noexcept
	{
		_replaceContainer(priv::string::alloc16_std(str));
		return *this;
	}

	String16& String16::operator=(std::u16string&& str) noexcept
	{
		_replaceContainer(priv::string::alloc16_std(Move(str)));
		return *this;
	}

	AtomicString& Atomic<String>::operator=(const std::string& str) noexcept
	{
		_replaceContainer(priv::string::alloc_std(str));
		return *this;
	}

	AtomicString& Atomic<String>::operator=(std::string&& str) noexcept
	{
		_replaceContainer(priv::string::alloc_std(Move(str)));
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(const std::u16string& str) noexcept
	{
		_replaceContainer(priv::string::alloc16_std(str));
		return *this;
	}

	AtomicString16& Atomic<String16>::operator=(std::u16string&& str) noexcept
	{
		_replaceContainer(priv::string::alloc16_std(Move(str)));
		return *this;
	}


	String& String::operator+=(sl_null_t) noexcept
	{
		return *this;
	}

	String16& String16::operator+=(sl_null_t) noexcept
	{
		return *this;
	}

	AtomicString& Atomic<String>::operator+=(sl_null_t) noexcept
	{
		return *this;
	}

	AtomicString16& Atomic<String16>::operator+=(sl_null_t) noexcept
	{
		return *this;
	}

#define DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(STRING, STRING_REF, ARG, ARG_REF) \
	STRING& STRING::operator+=(const ARG& other) noexcept \
	{ \
		ARG_REF s2 = other; \
		if (s2.isNotEmpty()) { \
			STRING_REF s1 = *this; \
			sl_size n = s1.getLength(); \
			if (n) { \
				*this = s1.merge(s1.getData(), n, s2.getData(), s2.getLength()); \
			} else { \
				*this = s2; \
			} \
		} \
		return *this; \
	}

#define DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(STRING, STRING_REF, ARG, ARG_REF) \
	STRING& STRING::operator+=(ARG&& other) noexcept \
	{ \
		ARG_REF s2 = Move(other); \
		if (s2.isNotEmpty()) { \
			STRING_REF s1 = *this; \
			sl_size n = s1.getLength(); \
			if (n) { \
				*this = s1.merge(s1.getData(), n, s2.getData(), s2.getLength()); \
			} else { \
				*this = Move(s2); \
			} \
		} \
		return *this; \
	}

	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(String, String&, String, const String&)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(String, String&, AtomicString, String)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(String, String&, String, String)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(String, String&, AtomicString, String)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(String16, String16&, String16, const String16&)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(String16, String16&, AtomicString16, String16)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(String16, String16&, String16, String16)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(String16, String16&, AtomicString16, String16)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(AtomicString, String, String, const String&)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(AtomicString, String, AtomicString, String)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(AtomicString, String, String, String)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(AtomicString, String, AtomicString, String)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(AtomicString16, String16, String16, const String16&)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB(AtomicString16, String16, AtomicString16, String16)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(AtomicString16, String16, String16, String16)
	DEFINE_STRING_PLUS_ASSIGN_OPERATOR_SUB_MOVE(AtomicString16, String16, AtomicString16, String16)


	String String::merge(const sl_char8* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept
	{
		return priv::string::merge8_8(a1, len1, a2, len2);
	}

	String String::merge(const sl_char8* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::merge8_16(a1, len1, (sl_char16*)a2, len2);
		} else {
			return priv::string::merge8_32(a1, len1, (sl_char32*)a2, len2);
		}
	}

	String String::merge(const sl_char8* a1, sl_reg len1, const char16_t* a2, sl_reg len2) noexcept
	{
		return priv::string::merge8_16(a1, len1, a2, len2);
	}

	String String::merge(const sl_char8* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept
	{
		return priv::string::merge8_32(a1, len1, a2, len2);
	}

	String String::merge(const wchar_t* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::merge8_16((sl_char16*)a1, len1, a2, len2);
		} else {
			return priv::string::merge8_32((sl_char32*)a1, len1, a2, len2);
		}
	}

	String String::merge(const char16_t* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept
	{
		return priv::string::merge8_16(a1, len1, a2, len2);
	}

	String String::merge(const char32_t* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept
	{
		return priv::string::merge8_32(a1, len1, a2, len2);
	}

	
	String16 String16::merge(const sl_char16* a1, sl_reg len1, const sl_char16* a2, sl_reg len2) noexcept
	{
		return priv::string::merge16_16(a1, len1, a2, len2);
	}

	String16 String16::merge(const sl_char16* a1, sl_reg len1, const char* a2, sl_reg len2) noexcept
	{
		return priv::string::merge16_8(a1, len1, a2, len2);
	}

	String16 String16::merge(const sl_char16* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::merge16_16(a1, len1, (sl_char16*)a2, len2);
		} else {
			return priv::string::merge16_32(a1, len1, (sl_char32*)a2, len2);
		}
	}

	String16 String16::merge(const sl_char16* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept
	{
		return priv::string::merge16_32(a1, len1, a2, len2);
	}

	String16 String16::merge(const char* a1, sl_reg len1, const sl_char16* a2, sl_reg len2) noexcept
	{
		return priv::string::merge16_8(a1, len1, a2, len2);
	}

	String16 String16::merge(const wchar_t* a1, sl_reg len1, const sl_char16* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::merge16_16((sl_char16*)a1, len1, a2, len2);
		} else {
			return priv::string::merge16_32((sl_char32*)a1, len1, a2, len2);
		}
	}

	String16 String16::merge(const char32_t* a1, sl_reg len1, const sl_char16* a2, sl_reg len2) noexcept
	{
		return priv::string::merge16_32(a1, len1, a2, len2);
	}


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

	sl_bool String::getUtf16(StringStorage& output) const noexcept
	{
		if (isEmpty()) {
			output.data16 = (sl_char16*)("\0\0");
			output.length = 0;
			return sl_true;
		}
		Memory mem = toUtf16();
		output.refer = mem.ref;
		if (mem.isNotNull()) {
			output.data16 = (const sl_char16*)(mem.getData());
			output.length = mem.getSize() / 2 - 1;
			if (output.data16) {
				return sl_true;
			}
		}
		output.data16 = (sl_char16*)("\0\0");
		output.length = 0;
		return sl_false;
	}

	sl_bool String16::getUtf8(StringStorage& output) const noexcept
	{
		if (isEmpty()) {
			output.data8 = "";
			output.length = 0;
			return sl_true;
		}
		Memory mem = toUtf8();
		if (mem.isNotNull()) {
			output.data8 = (const sl_char8*)(mem.getData());
			output.length = mem.getSize() - 1;
			if (output.data8) {
				return sl_true;
			}
		}
		output.data8 = "";
		output.length = 0;
		return sl_false;
	}

	sl_size Atomic<String>::getUtf16(sl_char16* utf16, sl_size len) const noexcept
	{
		String s(*this);
		return s.getUtf16(utf16, len);
	}

	sl_bool Atomic<String>::getUtf16(StringStorage& output) const noexcept
	{
		String s(*this);
		return s.getUtf16(output);
	}

	sl_size Atomic<String16>::getUtf8(sl_char8* utf8, sl_size len) const noexcept
	{
		String16 s(*this);
		return s.getUtf8(utf8, len);
	}

	sl_bool Atomic<String16>::getUtf8(StringStorage& output) const noexcept
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

	sl_bool String::getUtf32(StringStorage& output) const noexcept
	{
		if (isEmpty()) {
			output.data32 = (sl_char32*)("\0\0\0\0");
			output.length = 0;
			return sl_true;
		}
		Memory mem = toUtf32();
		if (mem.isNotNull()) {
			output.data32 = (const sl_char32*)(mem.getData());
			output.length = mem.getSize() / 4 - 1;
			if (output.data32) {
				return sl_true;
			}
		}
		output.data32 = (sl_char32*)("\0\0\0\0");
		output.length = 0;
		return sl_false;
	}

	sl_size Atomic<String>::getUtf32(sl_char32* utf32, sl_size len) const noexcept
	{
		String s(*this);
		return s.getUtf32(utf32, len);
	}

	sl_bool Atomic<String>::getUtf32(StringStorage& output) const noexcept
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

}
