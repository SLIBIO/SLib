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

namespace slib
{
	
	namespace priv
	{
		namespace string
		{
			struct ConstContainer16
			{
				StringContainer16* container;
				sl_int32 lock;
			};

			extern const ConstContainer16 g_null16;
			extern const ConstContainer16 g_empty16;
		}
	}
	
	SLIB_INLINE String16::String16(String16&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE String16::String16(AtomicString16&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE const String16& String16::null() noexcept
	{
		return *(reinterpret_cast<String16 const*>(&(priv::string::g_null16)));
	}
	
	SLIB_INLINE const String16& String16::getEmpty() noexcept
	{
		return *(reinterpret_cast<String16 const*>(&(priv::string::g_empty16)));
	}

	SLIB_INLINE const String16& String16::getNotNull() const noexcept
	{
		if (!m_container) {
			return *(reinterpret_cast<String16 const*>(&(priv::string::g_empty16)));
		}
		return *this;
	}
	
	SLIB_INLINE sl_bool String16::isNull() const noexcept
	{
		return m_container == sl_null;
	}
	
	SLIB_INLINE sl_bool String16::isNotNull() const noexcept
	{
		return m_container != sl_null;
	}
	
	SLIB_INLINE sl_bool String16::isEmpty() const noexcept
	{
		if (m_container) {
			return (m_container->len == 0);
		} else {
			return sl_true;
		}
	}
	
	SLIB_INLINE sl_bool String16::isNotEmpty() const noexcept
	{
		if (m_container) {
			return (m_container->len != 0);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_char16* String16::getData() const noexcept
	{
		if (m_container) {
			return m_container->sz;
		} else {
			return (sl_char16*)((void*)(L""));
		}
	}
	
	SLIB_INLINE sl_char16* String16::getData(sl_size& outLength) const noexcept
	{
		if (m_container) {
			outLength = m_container->len;
			return m_container->sz;
		} else {
			outLength = 0;
			return (sl_char16*)((void*)(L""));
		}
	}
	
	SLIB_INLINE sl_size String16::getLength() const noexcept
	{
		if (m_container) {
			return m_container->len;
		} else {
			return 0;
		}
	}


	SLIB_INLINE AtomicString16::Atomic(AtomicString16&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE AtomicString16::Atomic(String16&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE const AtomicString16& AtomicString16::null() noexcept
	{
		return *(reinterpret_cast<AtomicString16 const*>(&(priv::string::g_null16)));
	}
	
	SLIB_INLINE const AtomicString16& AtomicString16::getEmpty() noexcept
	{
		return *(reinterpret_cast<AtomicString16 const*>(&(priv::string::g_empty16)));
	}
	
	SLIB_INLINE sl_bool AtomicString16::isNull() const noexcept
	{
		return m_container == sl_null;
	}
	
	SLIB_INLINE sl_bool AtomicString16::isNotNull() const noexcept
	{
		return m_container != sl_null;
	}

}
