#ifndef CHECKHEADER_SLIB_CORE_DETAIL_STRING16
#define CHECKHEADER_SLIB_CORE_DETAIL_STRING16

#include "../string16.h"

namespace slib
{
	
	struct _String16_Const
	{
		StringContainer16* container;
		sl_int32 lock;
	};
	
	extern const _String16_Const _String16_Null;
	extern const _String16_Const _String16_Empty;

	template <>
	struct StringTypeFromCharType<sl_char16> { typedef String16 Type; };

	
	struct SLIB_EXPORT _StringContainer16
	{
		sl_char16* sz;
		sl_size len;
		sl_uint32 hash;
		sl_reg ref;
	};

	
	SLIB_INLINE String16::String16(String16&& src)
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE String16::String16(AtomicString16&& src)
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE const String16& String16::null()
	{
		return *(reinterpret_cast<String16 const*>(&_String16_Null));
	}
	
	SLIB_INLINE const String16& String16::getEmpty()
	{
		return *(reinterpret_cast<String16 const*>(&_String16_Empty));
	}

	SLIB_INLINE const String16& String16::getNotNull() const
	{
		if (!m_container) {
			return *(reinterpret_cast<String16 const*>(&_String16_Empty));
		}
		return *this;
	}
	
	SLIB_INLINE sl_bool String16::isNull() const
	{
		return m_container == sl_null;
	}
	
	SLIB_INLINE sl_bool String16::isNotNull() const
	{
		return m_container != sl_null;
	}
	
	SLIB_INLINE sl_bool String16::isEmpty() const
	{
		if (m_container) {
			return (m_container->len == 0);
		} else {
			return sl_true;
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
	
	SLIB_INLINE sl_char16* String16::getData() const
	{
		if (m_container) {
			return m_container->sz;
		} else {
			return (sl_char16*)((void*)(L""));
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


	SLIB_INLINE AtomicString16::Atomic(AtomicString16&& src)
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE AtomicString16::Atomic(String16&& src)
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE const AtomicString16& AtomicString16::null()
	{
		return *(reinterpret_cast<AtomicString16 const*>(&_String16_Null));
	}
	
	SLIB_INLINE const AtomicString16& AtomicString16::getEmpty()
	{
		return *(reinterpret_cast<AtomicString16 const*>(&_String16_Empty));
	}
	
	SLIB_INLINE sl_bool AtomicString16::isNull() const
	{
		return m_container == sl_null;
	}
	
	SLIB_INLINE sl_bool AtomicString16::isNotNull() const
	{
		return m_container != sl_null;
	}

}

#endif
