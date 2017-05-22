/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	struct _priv_String_Const
	{
		StringContainer* container;
		sl_int32 lock;
	};
	
	extern const _priv_String_Const _priv_String_Null;
	extern const _priv_String_Const _priv_String_Empty;

	template <>
	struct StringTypeFromCharType<sl_char8> { typedef String Type; };

	
	SLIB_INLINE String::String(String&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE String::String(AtomicString&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE const String& String::null() noexcept
	{
		return *(reinterpret_cast<String const*>(&_priv_String_Null));
	}
	
	SLIB_INLINE const String& String::getEmpty() noexcept
	{
		return *(reinterpret_cast<String const*>(&_priv_String_Empty));
	}

	SLIB_INLINE const String& String::getNotNull() const noexcept
	{
		if (!m_container) {
			return *(reinterpret_cast<String const*>(&_priv_String_Empty));
		}
		return *this;
	}
	
	SLIB_INLINE sl_bool String::isNull() const noexcept
	{
		return m_container == sl_null;
	}

	SLIB_INLINE sl_bool String::isNotNull() const noexcept
	{
		return m_container != sl_null;
	}
	
	SLIB_INLINE sl_bool String::isEmpty() const noexcept
	{
		if (m_container) {
			return (m_container->len == 0);
		} else {
			return sl_true;
		}
	}
	
	SLIB_INLINE sl_bool String::isNotEmpty() const noexcept
	{
		if (m_container) {
			return (m_container->len != 0);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_char8* String::getData() const noexcept
	{
		if (m_container) {
			return m_container->sz;
		} else {
			return (sl_char8*)((void*)(""));
		}
	}
	
	SLIB_INLINE sl_size String::getLength() const noexcept
	{
		if (m_container) {
			return m_container->len;
		} else {
			return 0;
		}
	}
	
	
	SLIB_INLINE AtomicString::Atomic(AtomicString&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE AtomicString::Atomic(String&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE const AtomicString& AtomicString::null() noexcept
	{
		return *(reinterpret_cast<AtomicString const*>(&_priv_String_Null));
	}
	
	SLIB_INLINE const AtomicString& AtomicString::getEmpty() noexcept
	{
		return *(reinterpret_cast<AtomicString const*>(&_priv_String_Empty));
	}

	SLIB_INLINE sl_bool AtomicString::isNull() const noexcept
	{
		return m_container == sl_null;
	}

	SLIB_INLINE sl_bool AtomicString::isNotNull() const noexcept
	{
		return m_container != sl_null;
	}


	extern const char* _priv_StringConv_radixPatternUpper;
	extern const char* _priv_StringConv_radixPatternLower;
	extern const sl_uint8* _priv_StringConv_radixInversePatternBig;
	extern const sl_uint8* _priv_StringConv_radixInversePatternSmall;

}
