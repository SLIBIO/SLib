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

#include "slib/core/string_buffer.h"
#include "slib/core/parse.h"
#include "slib/core/math.h"
#include "slib/core/time.h"
#include "slib/core/variant.h"
#include "slib/core/json.h"
#include "slib/core/cast.h"

namespace slib
{

	namespace priv
	{
		namespace string
		{
		
			SLIB_INLINE static const void* FindMemory(const sl_char8* mem, sl_char8 pattern, sl_size count) noexcept
			{
				return Base::findMemory(mem, pattern, count);
			}
			
			SLIB_INLINE static const void* FindMemory(const sl_char16* mem, sl_char16 pattern, sl_size count) noexcept
			{
				return Base::findMemory2((sl_uint16*)mem, pattern, count);
			}
		
			SLIB_INLINE static const void* FindMemoryReverse(const sl_char8* mem, sl_char8 pattern, sl_size count) noexcept
			{
				return Base::findMemoryReverse(mem, pattern, count);
			}
		
			SLIB_INLINE static const void* FindMemoryReverse(const sl_char16* mem, sl_char16 pattern, sl_size count) noexcept
			{
				return Base::findMemoryReverse2((sl_uint16*)mem, pattern, count);
			}
			
			SLIB_INLINE static sl_compare_result CompareMemory(const sl_char8* mem1, const sl_char8* mem2, sl_size count) noexcept
			{
				return Base::compareMemory((sl_uint8*)mem1, (sl_uint8*)mem2, count);
			}
			
			SLIB_INLINE static sl_compare_result CompareMemory(const sl_char16* mem1, const sl_char16* mem2, sl_size count) noexcept
			{
				return Base::compareMemory2((sl_uint16*)mem1, (sl_uint16*)mem2, count);
			}
			
		
			template <class CT>
			SLIB_INLINE static sl_size CalcHash(const CT* buf, sl_size len) noexcept
			{
				sl_size hash = 0;
				for (sl_size i = 0; i < len; i++) {
					sl_uint32 ch = buf[i];
					if (ch) {
						hash = hash * 31 + ch;
					} else {
						break;
					}
				}
				if (hash) {
					hash = Rehash(hash);
				}
				return hash;
			}
			
			template <class CT>
			SLIB_INLINE static sl_size CalcHashIgnoreCase(const CT* buf, sl_size len) noexcept
			{
				sl_size hash = 0;
				for (sl_size i = 0; i < len; i++) {
					sl_uint32 ch = buf[i];
					if (ch) {
						ch = SLIB_CHAR_LOWER_TO_UPPER(ch);
						hash = hash * 31 + ch;
					} else {
						break;
					}
				}
				if (hash) {
					hash = Rehash(hash);
				}
				return hash;
			}
			
		}
	}

	sl_size String::getHashCode() const noexcept
	{
		if (m_container) {
			sl_size n = m_container->len;
			if (n > 0) {
				sl_size hash = m_container->hash;
				if (hash == 0) {
					hash = priv::string::CalcHash(m_container->sz, n);
					m_container->hash = hash;
				}
				return hash;
			}
		}
		return 0;
	}

	sl_size String::getHashCode(sl_char8* str, sl_reg len) noexcept
	{
		return priv::string::CalcHash(str, len);
	}

	sl_size String16::getHashCode() const noexcept
	{
		if (m_container) {
			sl_size n = m_container->len;
			if (n > 0) {
				sl_size hash = m_container->hash;
				if (hash == 0) {
					hash = priv::string::CalcHash(m_container->sz, n);
					m_container->hash = hash;
				}
				return hash;
			}
		}
		return 0;
	}

	sl_size String16::getHashCode(sl_char16* str, sl_reg len) noexcept
	{
		return priv::string::CalcHash(str, len);
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
	
	sl_size StringView::getHashCode() const noexcept
	{
		return priv::string::CalcHash(data, length);
	}

	sl_size StringView16::getHashCode() const noexcept
	{
		return priv::string::CalcHash(data, length);
	}

	sl_size String::getHashCodeIgnoreCase() const noexcept
	{
		if (m_container) {
			return priv::string::CalcHashIgnoreCase(m_container->sz, m_container->len);
		}
		return 0;
	}

	sl_size String::getHashCodeIgnoreCase(sl_char8* str, sl_reg len) noexcept
	{
		return priv::string::CalcHashIgnoreCase(str, len);
	}

	sl_size String16::getHashCodeIgnoreCase() const noexcept
	{
		if (m_container) {
			return priv::string::CalcHashIgnoreCase(m_container->sz, m_container->len);
		}
		return 0;
	}

	sl_size String16::getHashCodeIgnoreCase(sl_char16* str, sl_reg len) noexcept
	{
		return priv::string::CalcHashIgnoreCase(str, len);
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

	sl_size StringView::getHashCodeIgnoreCase() const noexcept
	{
		return priv::string::CalcHashIgnoreCase(data, length);
	}

	sl_size StringView16::getHashCodeIgnoreCase() const noexcept
	{
		return priv::string::CalcHashIgnoreCase(data, length);
	}


	namespace priv
	{
		namespace string
		{

			SLIB_INLINE static sl_compare_result CompareString(const sl_char8* str1, sl_size len1, const sl_char8* str2, sl_size len2) noexcept
			{
				if (len1 < len2) {
					if (len1 == 0) {
						return -1;
					}
					sl_compare_result r = Base::compareMemory((sl_uint8*)str1, (sl_uint8*)str2, len1);
					if (r == 0) {
						return -1;
					} else {
						return r;
					}
				} else if (len1 > len2) {
					if (len2 == 0) {
						return 1;
					}
					sl_compare_result r = Base::compareMemory((sl_uint8*)str1, (sl_uint8*)str2, len2);
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
			
			SLIB_INLINE static sl_compare_result CompareString(const sl_char16* str1, sl_size len1, const sl_char16* str2, sl_size len2) noexcept
			{
				if (len1 < len2) {
					if (len1 == 0) {
						return -1;
					}
					sl_compare_result r = Base::compareMemory2((sl_uint16*)str1, (sl_uint16*)str2, len1);
					if (r == 0) {
						return -1;
					} else {
						return r;
					}
				} else if (len1 > len2) {
					if (len2 == 0) {
						return 1;
					}
					sl_compare_result r = Base::compareMemory2((sl_uint16*)str1, (sl_uint16*)str2, len2);
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
			
			SLIB_INLINE static sl_compare_result Compare8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
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
						return priv::string::CompareString(str1, len1, str2, len2);
					}
				}
			}
			
			SLIB_INLINE static sl_compare_result Compare16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
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
						return priv::string::CompareString(str1, len1, str2, len2);
					}
				}
			}
			
			SLIB_INLINE static sl_compare_result Compare8_16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
			{
				String other = String::create(str2, len2);
				return priv::string::Compare8(str1, len1, other.getData(), other.getLength());
			}
			
			SLIB_INLINE static sl_compare_result Compare16_8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
			{
				String16 other = String16::create(str2, len2);
				return priv::string::Compare16(str1, len1, other.getData(), other.getLength());
			}
			
			SLIB_INLINE static sl_compare_result Compare8_32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2) noexcept
			{
				String other = String::create(str2, len2);
				return priv::string::Compare8(str1, len1, other.getData(), other.getLength());
			}
			
			SLIB_INLINE static sl_compare_result Compare16_32(const sl_char16* m1, sl_reg len1, const sl_char32* m2, sl_reg len2) noexcept
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
			
			
			SLIB_INLINE static sl_bool EqualsObjects(const sl_char8* str1, sl_size len1, const sl_char8* str2, sl_size len2) noexcept
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
			
			SLIB_INLINE static sl_bool EqualsObjects(const sl_char16* str1, sl_size len1, const sl_char16* str2, sl_size len2) noexcept
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
			
			SLIB_INLINE static sl_bool Equals8(const sl_char8* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
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
						return priv::string::EqualsObjects(str1, len1, str2, len2);
					}
				}
			}
			
			SLIB_INLINE static sl_bool Equals16(const sl_char16* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
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
						return priv::string::EqualsObjects(str1, len1, str2, len2);
					}
				}
			}
			
			SLIB_INLINE static sl_bool Equals8_16(const sl_char8* str1, sl_reg len1, const sl_char16* str2, sl_reg len2) noexcept
			{
				String other = String::create(str2, len2);
				return priv::string::Equals8(str1, len1, other.getData(), other.getLength());
			}
			
			SLIB_INLINE static sl_bool Equals16_8(const sl_char16* str1, sl_reg len1, const sl_char8* str2, sl_reg len2) noexcept
			{
				String16 other = String16::create(str2, len2);
				return priv::string::Equals16(str1, len1, other.getData(), other.getLength());
			}
			
			SLIB_INLINE static sl_bool Equals8_32(const sl_char8* str1, sl_reg len1, const sl_char32* str2, sl_reg len2) noexcept
			{
				String other = String::create(str2, len2);
				return priv::string::Equals8(str1, len1, other.getData(), other.getLength());
			}
			
			SLIB_INLINE static sl_bool Equals16_32(const sl_char16* str1, sl_reg len1, const sl_char32* str2, sl_reg len2) noexcept
			{
				return priv::string::Compare16_32(str1, len1, str2, len2) == 0;
			}
			
		}
	}

	sl_bool String::equals(const sl_char8* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept
	{
		return priv::string::Equals8(a1, len1, a2, len2);
	}

	sl_bool String::equals(const sl_char8* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::Equals8_16(a1, len1, (sl_char16*)a2, len2);
		} else {
			return priv::string::Equals8_32(a1, len1, (sl_char32*)a2, len2);
		}
	}

	sl_bool String::equals(const sl_char8* a1, sl_reg len1, const char16_t* a2, sl_reg len2) noexcept
	{
		return priv::string::Equals8_16(a1, len1, a2, len2);
	}

	sl_bool String::equals(const sl_char8* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept
	{
		return priv::string::Equals8_32(a1, len1, a2, len2);
	}

	sl_bool String16::equals(const sl_char16* a1, sl_reg len1, const sl_char16* a2, sl_reg len2) noexcept
	{
		return priv::string::Equals16(a1, len1, a2, len2);
	}

	sl_bool String16::equals(const sl_char16* a1, sl_reg len1, const char* a2, sl_reg len2) noexcept
	{
		return priv::string::Equals16_8(a1, len1, a2, len2);
	}

	sl_bool String16::equals(const sl_char16* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::Equals16(a1, len1, (sl_char16*)a2, len2);
		} else {
			return priv::string::Equals16_32(a1, len1, (sl_char32*)a2, len2);
		}
	}

	sl_bool String16::equals(const sl_char16* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept
	{
		return priv::string::Equals16_32(a1, len1, a2, len2);
	}


	sl_compare_result String::compare(const sl_char8* a1, sl_reg len1, const sl_char8* a2, sl_reg len2) noexcept
	{
		return priv::string::Compare8(a1, len1, a2, len2);
	}

	sl_compare_result String::compare(const sl_char8* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::Compare8_16(a1, len1, (sl_char16*)a2, len2);
		} else {
			return priv::string::Compare8_32(a1, len1, (sl_char32*)a2, len2);
		}
	}

	sl_compare_result String::compare(const sl_char8* a1, sl_reg len1, const char16_t* a2, sl_reg len2) noexcept
	{
		return priv::string::Compare8_16(a1, len1, a2, len2);
	}

	sl_compare_result String::compare(const sl_char8* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept
	{
		return priv::string::Compare8_32(a1, len1, a2, len2);
	}

	sl_compare_result String16::compare(const sl_char16* a1, sl_reg len1, const sl_char16* a2, sl_reg len2) noexcept
	{
		return priv::string::Compare16(a1, len1, a2, len2);
	}

	sl_compare_result String16::compare(const sl_char16* a1, sl_reg len1, const char* a2, sl_reg len2) noexcept
	{
		return priv::string::Compare16_8(a1, len1, a2, len2);
	}

	sl_compare_result String16::compare(const sl_char16* a1, sl_reg len1, const wchar_t* a2, sl_reg len2) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return priv::string::Compare16(a1, len1, (sl_char16*)a2, len2);
		} else {
			return priv::string::Compare16_32(a1, len1, (sl_char32*)a2, len2);
		}
	}

	sl_compare_result String16::compare(const sl_char16* a1, sl_reg len1, const char32_t* a2, sl_reg len2) noexcept
	{
		return priv::string::Compare16_32(a1, len1, a2, len2);
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

	sl_bool String::equals(const AtomicString& other) const noexcept
	{
		return equals(String(other));
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

	sl_bool String16::equals(const AtomicString16& other) const noexcept
	{
		return equals(String16(other));
	}

	sl_bool Atomic<String>::equals(const String& other) const noexcept
	{
		return String(*this).equals(other);
	}

	sl_bool Atomic<String>::equals(const AtomicString& other) const noexcept
	{
		return String(*this).equals(String(other));
	}

	sl_bool Atomic<String16>::equals(const String16& other) const noexcept
	{
		return String16(*this).equals(other);
	}

	sl_bool Atomic<String16>::equals(const AtomicString16& other) const noexcept
	{
		return String16(*this).equals(String16(other));
	}


	sl_compare_result String::compare(const String& other) const noexcept
	{
		sl_char8* s1 = getData();
		sl_char8* s2 = other.getData();
		if (s1 == s2) {
			return 0;
		}
		sl_size len1 = getLength();
		sl_size len2 = other.getLength();
		return priv::string::CompareString(s1, len1, s2, len2);
	}

	sl_compare_result String::compare(const AtomicString& other) const noexcept
	{
		return compare(String(other));
	}

	sl_compare_result String16::compare(const String16& other) const noexcept
	{
		sl_char16* s1 = getData();
		sl_char16* s2 = other.getData();
		if (s1 == s2) {
			return 0;
		}
		sl_size len1 = getLength();
		sl_size len2 = other.getLength();
		return priv::string::CompareString(s1, len1, s2, len2);
	}

	sl_compare_result String16::compare(const AtomicString16& other) const noexcept
	{
		return compare(String16(other));
	}

	sl_compare_result Atomic<String>::compare(const String& other) const noexcept
	{
		return String(*this).compare(other);
	}

	sl_compare_result Atomic<String>::compare(const AtomicString& other) const noexcept
	{
		return String(*this).compare(String(other));
	}

	sl_compare_result Atomic<String16>::compare(const String16& other) const noexcept
	{
		return String16(*this).compare(other);
	}

	sl_compare_result Atomic<String16>::compare(const AtomicString16& other) const noexcept
	{
		return String16(*this).compare(String16(other));
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_compare_result CompareLimited(const CT* s1, sl_size l1, const CT* s2, sl_size l2, sl_size len) noexcept
			{
				if (s1 == s2) {
					return 0;
				}
				if (l1 > len) {
					l1 = len;
				}
				if (l2 > len) {
					l2 = len;
				}
				return CompareString(s1, l1, s2, l2);
			}
		}
	}

	sl_compare_result String::compare(const StringParam& _other, sl_size len) const noexcept
	{
		StringData other(_other);
		return priv::string::CompareLimited(getData(), getLength(), other.getData(), other.getLength(), len);
	}

	sl_compare_result String16::compare(const StringParam& _other, sl_size len) const noexcept
	{
		StringData16 other(_other);
		return priv::string::CompareLimited(getData(), getLength(), other.getData(), other.getLength(), len);
	}

	sl_compare_result Atomic<String>::compare(const StringParam& other, sl_size len) const noexcept
	{
		String s(*this);
		return s.compare(other, len);
	}

	sl_compare_result Atomic<String16>::compare(const StringParam& other, sl_size len) const noexcept
	{
		String16 s(*this);
		return s.compare(other, len);
	}

	sl_compare_result StringView::compare(const StringParam& _other, sl_size len) const noexcept
	{
		StringData other(_other);
		return priv::string::CompareLimited(getData(), getLength(), other.getData(), other.getLength(), len);
	}

	sl_compare_result StringView16::compare(const StringParam& _other, sl_size len) const noexcept
	{
		StringData16 other(_other);
		return priv::string::CompareLimited(getData(), getLength(), other.getData(), other.getLength(), len);
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_bool EqualsIgnoreCase(const CT* s1, sl_size l1, const CT* s2, sl_size l2) noexcept
			{
				if (s1 == s2) {
					return sl_true;
				}
				if (l1 != l2) {
					return sl_false;
				}
				for (sl_size i = 0; i < l1; i++) {
					CT c1 = s1[i];
					CT c2 = s2[i];
					c1 = SLIB_CHAR_LOWER_TO_UPPER(c1);
					c2 = SLIB_CHAR_LOWER_TO_UPPER(c2);
					if (c1 != c2) {
						return sl_false;
					}
				}
				return sl_true;
			}
		}
	}

	sl_bool String::equalsIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData other(_other);
		return priv::string::EqualsIgnoreCase((sl_uint8*)(getData()), getLength(), (sl_uint8*)(other.getData()), other.getLength());
	}

	sl_bool String16::equalsIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData16 other(_other);
		return priv::string::EqualsIgnoreCase((sl_uint16*)(getData()), getLength(), (sl_uint16*)(other.getData()), other.getLength());
	}

	sl_bool Atomic<String>::equalsIgnoreCase(const StringParam& other) const noexcept
	{
		String s(*this);
		return s.equalsIgnoreCase(other);
	}

	sl_bool Atomic<String16>::equalsIgnoreCase(const StringParam& other) const noexcept
	{
		String16 s(*this);
		return s.equalsIgnoreCase(other);
	}

	sl_bool StringView::equalsIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData other(_other);
		return priv::string::EqualsIgnoreCase((sl_uint8*)(getData()), getLength(), (sl_uint8*)(other.getData()), other.getLength());
	}

	sl_bool StringView16::equalsIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData16 other(_other);
		return priv::string::EqualsIgnoreCase((sl_uint16*)(getData()), getLength(), (sl_uint16*)(other.getData()), other.getLength());
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_compare_result CompareIgnoreCase(const CT* s1, sl_size len1, const CT* s2, sl_size len2) noexcept
			{
				if (s1 == s2) {
					return 0;
				}
				sl_size len = SLIB_MIN(len1, len2);
				for (sl_size i = 0; i < len; i++) {
					CT c1 = s1[i];
					CT c2 = s2[i];
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
		}
	}

	sl_compare_result String::compareIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData other(_other);
		return priv::string::CompareIgnoreCase((sl_uint8*)(getData()), getLength(), (sl_uint8*)(other.getData()), other.getLength());
	}

	sl_compare_result String16::compareIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData16 other(_other);
		return priv::string::CompareIgnoreCase((sl_uint16*)(getData()), getLength(), (sl_uint16*)(other.getData()), other.getLength());
	}

	sl_compare_result Atomic<String>::compareIgnoreCase(const StringParam& other) const noexcept
	{
		String s(*this);
		return s.compareIgnoreCase(other);
	}

	sl_compare_result Atomic<String16>::compareIgnoreCase(const StringParam& other) const noexcept
	{
		String16 s(*this);
		return s.compareIgnoreCase(other);
	}

	sl_compare_result StringView::compareIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData other(_other);
		return priv::string::CompareIgnoreCase((sl_uint8*)(getData()), getLength(), (sl_uint8*)(other.getData()), other.getLength());
	}

	sl_compare_result StringView16::compareIgnoreCase(const StringParam& _other) const noexcept
	{
		StringData16 other(_other);
		return priv::string::CompareIgnoreCase((sl_uint16*)(getData()), getLength(), (sl_uint16*)(other.getData()), other.getLength());
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

	StringView StringView::substring(sl_reg start, sl_reg end) const noexcept
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
		return StringView(getData() + start, end - start);
	}

	StringView16 StringView16::substring(sl_reg start, sl_reg end) const noexcept
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
		return StringView16(getData() + start, end - start);
	}


	String String::left(sl_reg len) const noexcept
	{
		return substring(0, len);
	}

	String16 String16::left(sl_reg len) const noexcept
	{
		return substring(0, len);
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

	StringView StringView::left(sl_reg len) const noexcept
	{
		return substring(0, len);
	}

	StringView16 StringView16::left(sl_reg len) const noexcept
	{
		return substring(0, len);
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

	StringView StringView::right(sl_reg len) const noexcept
	{
		return substring(getLength() - len);
	}

	StringView16 StringView16::right(sl_reg len) const noexcept
	{
		return substring(getLength() - len);
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

	StringView StringView::mid(sl_reg start, sl_reg len) const noexcept
	{
		return substring(start, start + len);
	}
		
	StringView16 StringView16::mid(sl_reg start, sl_reg len) const noexcept
	{
		return substring(start, start+len);
	}

	
	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_reg IndexOfChar(const CT* str, sl_size len, CT ch, sl_reg _start) noexcept
			{
				if (len == 0) {
					return -1;
				}
				sl_size start;
				if (_start < 0) {
					start = 0;
				} else {
					start = _start;
					if (start >= len) {
						return -1;
					}
				}
				CT* pt = (CT*)(FindMemory(str + start, ch, len - start));
				if (pt == sl_null) {
					return -1;
				} else {
					return (sl_reg)(pt - str);
				}
			}
		}
	}

	sl_reg String::indexOf(sl_char8 ch, sl_reg start) const noexcept
	{
		return priv::string::IndexOfChar(getData(), getLength(), ch, start);
	}

	sl_reg String16::indexOf(sl_char16 ch, sl_reg start) const noexcept
	{
		return priv::string::IndexOfChar(getData(), getLength(), ch, start);
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

	sl_reg StringView::indexOf(sl_char8 ch, sl_reg start) const noexcept
	{
		return priv::string::IndexOfChar(getData(), getLength(), ch, start);
	}

	sl_reg StringView16::indexOf(sl_char16 ch, sl_reg start) const noexcept
	{
		return priv::string::IndexOfChar(getData(), getLength(), ch, start);
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_reg IndexOf(const CT* buf, sl_size count, const CT* bufPat, sl_size countPat, sl_reg _start) noexcept
			{
				if (countPat == 0) {
					return 0;
				}
				if (countPat == 1) {
					return IndexOfChar(buf, count, bufPat[0], _start);
				}
				if (count < countPat) {
					return -1;
				}
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
					const CT* pt = (const CT*)(FindMemory(buf + start, bufPat[0], count - start - countPat + 1));
					if (pt == sl_null) {
						return -1;
					}
					if (CompareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
						return (sl_reg)(pt - buf);
					} else {
						start = (sl_size)(pt - buf + 1);
					}
				}
				return -1;
			}
		}
	}

	sl_reg String::indexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData pattern(_pattern);
		return priv::string::IndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}

	sl_reg String16::indexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData16 pattern(_pattern);
		return priv::string::IndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}

	sl_reg Atomic<String>::indexOf(const StringParam& str, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(str, start);
	}

	sl_reg Atomic<String16>::indexOf(const StringParam& str, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(str, start);
	}

	sl_reg StringView::indexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData pattern(_pattern);
		return priv::string::IndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}

	sl_reg StringView16::indexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData16 pattern(_pattern);
		return priv::string::IndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_reg LastIndexOfChar(const CT* str, sl_size len, CT ch, sl_reg _start) noexcept
			{
				if (len == 0) {
					return -1;
				}
				sl_size start;
				if (_start < 0) {
					start = len - 1;
				} else {
					start = _start;
					if (start >= len) {
						start = len - 1;
					}
				}
				CT* pt = (CT*)(FindMemoryReverse(str, ch, start + 1));
				if (pt == sl_null) {
					return -1;
				} else {
					return (sl_reg)(pt - str);
				}
			}
		}
	}

	sl_reg String::lastIndexOf(sl_char8 ch, sl_reg start) const noexcept
	{
		return priv::string::LastIndexOfChar(getData(), getLength(), ch, start);
	}

	sl_reg String16::lastIndexOf(sl_char16 ch, sl_reg start) const noexcept
	{
		return priv::string::LastIndexOfChar(getData(), getLength(), ch, start);
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

	sl_reg StringView::lastIndexOf(sl_char8 ch, sl_reg start) const noexcept
	{
		return priv::string::LastIndexOfChar(getData(), getLength(), ch, start);
	}

	sl_reg StringView16::lastIndexOf(sl_char16 ch, sl_reg start) const noexcept
	{
		return priv::string::LastIndexOfChar(getData(), getLength(), ch, start);
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static sl_reg LastIndexOf(const CT* buf, sl_size count, const CT* bufPat, sl_size countPat, sl_reg _start) noexcept
			{
				if (countPat == 0) {
					return 0;
				}
				if (countPat == 1) {
					return LastIndexOfChar(buf, count, bufPat[0], _start);
				}
				if (count < countPat) {
					return -1;
				}
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
					const CT* pt = (const CT*)(FindMemoryReverse(buf, bufPat[0], s));
					if (pt == sl_null) {
						return -1;
					}
					if (CompareMemory(pt + 1, bufPat + 1, countPat - 1) == 0) {
						return (sl_reg)(pt - buf);
					} else {
						s = (sl_size)(pt - buf);
					}
				}
				return -1;
			}
		}
	}

	sl_reg String::lastIndexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData pattern(_pattern);
		return priv::string::LastIndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}

	sl_reg String16::lastIndexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData16 pattern(_pattern);
		return priv::string::LastIndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}

	sl_reg Atomic<String>::lastIndexOf(const StringParam& str, sl_reg start) const noexcept
	{
		String s(*this);
		return s.indexOf(str, start);
	}

	sl_reg Atomic<String16>::lastIndexOf(const StringParam& str, sl_reg start) const noexcept
	{
		String16 s(*this);
		return s.indexOf(str, start);
	}

	sl_reg StringView::lastIndexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData pattern(_pattern);
		return priv::string::LastIndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
	}

	sl_reg StringView16::lastIndexOf(const StringParam& _pattern, sl_reg start) const noexcept
	{
		StringData16 pattern(_pattern);
		return priv::string::LastIndexOf(getData(), getLength(), pattern.getData(), pattern.getLength(), start);
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

	sl_bool StringView::startsWith(sl_char8 ch) const noexcept
	{
		if (isEmpty()) {
			return sl_false;
		} else {
			return getData()[0] == ch;
		}
	}

	sl_bool StringView16::startsWith(sl_char16 ch) const noexcept
	{
		if (isEmpty()) {
			return sl_false;
		} else {
			return getData()[0] == ch;
		}
	}


	sl_bool String::startsWith(const StringParam& _str) const noexcept
	{
		StringData str(_str);
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

	sl_bool String16::startsWith(const StringParam& _str) const noexcept
	{
		StringData16 str(_str);
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

	sl_bool Atomic<String>::startsWith(const StringParam& str) const noexcept
	{
		String s(*this);
		return s.startsWith(str);
	}

	sl_bool Atomic<String16>::startsWith(const StringParam& str) const noexcept
	{
		String16 s(*this);
		return s.startsWith(str);
	}

	sl_bool StringView::startsWith(const StringParam& _str) const noexcept
	{
		StringData str(_str);
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

	sl_bool StringView16::startsWith(const StringParam& _str) const noexcept
	{
		StringData16 str(_str);
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

	sl_bool StringView::endsWith(sl_char8 ch) const noexcept
	{
		sl_size count = getLength();
		if (count < 1) {
			return sl_false;
		} else {
			return getData()[count - 1] == ch;
		}
	}

	sl_bool StringView16::endsWith(sl_char16 ch) const noexcept
	{
		sl_size count = getLength();
		if (count < 1) {
			return sl_false;
		} else {
			return getData()[count - 1] == ch;
		}
	}


	sl_bool String::endsWith(const StringParam& _str) const noexcept
	{
		StringData str(_str);
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

	sl_bool String16::endsWith(const StringParam& _str) const noexcept
	{
		StringData16 str(_str);
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

	sl_bool Atomic<String>::endsWith(const StringParam& str) const noexcept
	{
		String s(*this);
		return s.endsWith(str);
	}

	sl_bool Atomic<String16>::endsWith(const StringParam& str) const noexcept
	{
		String16 s(*this);
		return s.endsWith(str);
	}

	sl_bool StringView::endsWith(const StringParam& _str) const noexcept
	{
		StringData str(_str);
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

	sl_bool StringView16::endsWith(const StringParam& _str) const noexcept
	{
		StringData16 str(_str);
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

	sl_bool StringView::contains(sl_char8 ch) const noexcept
	{
		return indexOf(ch) >= 0;
	}

	sl_bool StringView16::contains(sl_char16 ch) const noexcept
	{
		return indexOf(ch) >= 0;
	}


	sl_bool String::contains(const StringParam& str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool String16::contains(const StringParam& str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool Atomic<String>::contains(const StringParam& str) const noexcept
	{
		String s(*this);
		return s.contains(str);
	}

	sl_bool Atomic<String16>::contains(const StringParam& str) const noexcept
	{
		String16 s(*this);
		return s.contains(str);
	}

	sl_bool StringView::contains(const StringParam& str) const noexcept
	{
		return indexOf(str) >= 0;
	}

	sl_bool StringView16::contains(const StringParam& str) const noexcept
	{
		return indexOf(str) >= 0;
	}


	namespace priv
	{
		namespace string
		{
			template <class CT>
			SLIB_INLINE static void CopyMakingUpper(CT* dst, const CT* src, sl_size len) noexcept
			{
				for (sl_size i = 0; i < len; i++) {
					dst[i] = SLIB_CHAR_LOWER_TO_UPPER(src[i]);
				}
			}
			
			template <class CT>
			SLIB_INLINE static void CopyMakingLower(CT* dst, const CT* src, sl_size len) noexcept
			{
				for (sl_size i = 0; i < len; i++) {
					dst[i] = SLIB_CHAR_UPPER_TO_LOWER(src[i]);
				}
			}
		}
	}

	void String::makeUpper() noexcept
	{
		priv::string::CopyMakingUpper(getData(), getData(), getLength());
	}

	void String16::makeUpper() noexcept
	{
		priv::string::CopyMakingUpper(getData(), getData(), getLength());
	}

	void Atomic<String>::makeUpper() noexcept
	{
		String s(*this);
		priv::string::CopyMakingUpper(s.getData(), s.getData(), s.getLength());
	}

	void Atomic<String16>::makeUpper() noexcept
	{
		String16 s(*this);
		priv::string::CopyMakingUpper(s.getData(), s.getData(), s.getLength());
	}

	void StringView::makeUpper() noexcept
	{
		priv::string::CopyMakingUpper(getData(), getData(), getLength());
	}

	void StringView16::makeUpper() noexcept
	{
		priv::string::CopyMakingUpper(getData(), getData(), getLength());
	}

	void String::makeLower() noexcept
	{
		priv::string::CopyMakingLower(getData(), getData(), getLength());
	}

	void String16::makeLower() noexcept
	{
		priv::string::CopyMakingLower(getData(), getData(), getLength());
	}

	void Atomic<String>::makeLower() noexcept
	{
		String s(*this);
		priv::string::CopyMakingLower(s.getData(), s.getData(), s.getLength());
	}

	void Atomic<String16>::makeLower() noexcept
	{
		String16 s(*this);
		priv::string::CopyMakingLower(s.getData(), s.getData(), s.getLength());
	}

	void StringView::makeLower() noexcept
	{
		priv::string::CopyMakingLower(getData(), getData(), getLength());
	}

	void StringView16::makeLower() noexcept
	{
		priv::string::CopyMakingLower(getData(), getData(), getLength());
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
		priv::string::CopyMakingUpper(ret.getData(), sz, len);
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
		priv::string::CopyMakingUpper(ret.getData(), sz, len);
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
		priv::string::CopyMakingLower(ret.getData(), sz, len);
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
		priv::string::CopyMakingLower(ret.getData(), sz, len);
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

	String StringView::toUpper() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return String::toUpper(getData(), getLength());
	}

	String16 StringView16::toUpper() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return String16::toUpper(getData(), getLength());
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

	String StringView::toLower() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return String::toLower(getData(), getLength());
	}

	String16 StringView16::toLower() const noexcept
	{
		if (isNull()) {
			return sl_null;
		}
		return String16::toLower(getData(), getLength());
	}


	namespace priv
	{
		namespace string
		{
			
			struct STRING_REPLACE_SUBSET
			{
				sl_reg start;
				sl_reg len;
			};
			
			template <class ST, class CT>
			SLIB_INLINE static ST ReplaceAll(const CT* buf, sl_reg count, const CT* pattern, sl_reg countPat, const CT* bufReplace, sl_reg countReplace) noexcept
			{
				if (countPat == 0) {
					return sl_null;
				}
				if (count == 0) {
					return sl_null;
				}
				LinkedQueue<STRING_REPLACE_SUBSET> queue;
				STRING_REPLACE_SUBSET subset;
				sl_reg size = 0;
				sl_reg start = 0;
				while (start <= count + countPat - 1) {
					sl_reg index = IndexOf(buf, count, pattern, countPat, start);
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

		}
	}

	String String::replaceAll(const StringParam& _pattern, const StringParam& _replacement) const noexcept
	{
		StringData pattern(_pattern);
		StringData replacement(_replacement);
		return priv::string::ReplaceAll<String, sl_char8>(getData(), getLength(), pattern.getData(), pattern.getLength(), replacement.getData(), replacement.getLength());
	}

	String16 String16::replaceAll(const StringParam& _pattern, const StringParam& _replacement) const noexcept
	{
		StringData16 pattern(_pattern);
		StringData16 replacement(_replacement);
		return priv::string::ReplaceAll<String16, sl_char16>(getData(), getLength(), pattern.getData(), pattern.getLength(), replacement.getData(), replacement.getLength());
	}

	String Atomic<String>::replaceAll(const StringParam& pattern, const StringParam& replacement) const noexcept
	{
		String s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String16 Atomic<String16>::replaceAll(const StringParam& pattern, const StringParam& replacement) const noexcept
	{
		String16 s(*this);
		return s.replaceAll(pattern, replacement);
	}

	String StringView::replaceAll(const StringParam& _pattern, const StringParam& _replacement) const noexcept
	{
		StringData pattern(_pattern);
		StringData replacement(_replacement);
		return priv::string::ReplaceAll<String, sl_char8>(getData(), getLength(), pattern.getData(), pattern.getLength(), replacement.getData(), replacement.getLength());
	}

	String16 StringView16::replaceAll(const StringParam& _pattern, const StringParam& _replacement) const noexcept
	{
		StringData16 pattern(_pattern);
		StringData16 replacement(_replacement);
		return priv::string::ReplaceAll<String16, sl_char16>(getData(), getLength(), pattern.getData(), pattern.getLength(), replacement.getData(), replacement.getLength());
	}


	namespace priv
	{
		namespace string
		{
			
			template <class ST, class CT>
			SLIB_INLINE static ST Trim(const ST& str) noexcept
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

			template <class ST, class CT>
			SLIB_INLINE static ST TrimLeft(const ST& str) noexcept
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
			
			template <class ST, class CT>
			SLIB_INLINE static ST TrimRight(const ST& str) noexcept
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
			
		}
	}

	String String::trim() const noexcept
	{
		return priv::string::Trim<String, sl_char8>(*this);
	}

	String16 String16::trim() const noexcept
	{
		return priv::string::Trim<String16, sl_char16>(*this);
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

	StringView StringView::trim() const noexcept
	{
		return priv::string::Trim<StringView, sl_char8>(*this);
	}

	StringView16 StringView16::trim() const noexcept
	{
		return priv::string::Trim<StringView16, sl_char16>(*this);
	}

	String String::trimLeft() const noexcept
	{
		return priv::string::TrimLeft<String, sl_char8>(*this);
	}

	String16 String16::trimLeft() const noexcept
	{
		return priv::string::TrimLeft<String16, sl_char16>(*this);
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

	StringView StringView::trimLeft() const noexcept
	{
		return priv::string::TrimLeft<StringView, sl_char8>(*this);
	}

	StringView16 StringView16::trimLeft() const noexcept
	{
		return priv::string::TrimLeft<StringView16, sl_char16>(*this);
	}

	String String::trimRight() const noexcept
	{
		return priv::string::TrimRight<String, sl_char8>(*this);
	}

	String16 String16::trimRight() const noexcept
	{
		return priv::string::TrimRight<String16, sl_char16>(*this);
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

	StringView StringView::trimRight() const noexcept
	{
		return priv::string::TrimRight<StringView, sl_char8>(*this);
	}

	StringView16 StringView16::trimRight() const noexcept
	{
		return priv::string::TrimRight<StringView16, sl_char16>(*this);
	}


	namespace priv
	{
		namespace string
		{
			template <class ST, class CT>
			static List<ST> Split(const CT* buf, sl_size count, const CT* pattern, sl_size countPattern) noexcept
			{
				if (countPattern == 0) {
					return sl_null;
				}
				if (count == 0) {
					return sl_null;
				}
				List<ST> ret;
				sl_reg start = 0;
				while (1) {
					sl_reg index = IndexOf(buf, count, pattern, countPattern, start);
					if (index < 0) {
						ret.add_NoLock(ST(buf + start, count - start));
						break;
					}
					ret.add_NoLock(ST(buf + start, index - start));
					start = index + countPattern;
				}
				return Move(ret);
			}

		}
	}

	List<String> String::split(const StringParam& _pattern) const noexcept
	{
		StringData pattern(_pattern);
		return priv::string::Split<String, sl_char8>(getData(), getLength(), pattern.getData(), pattern.getLength());
	}

	List<String16> String16::split(const StringParam& _pattern) const noexcept
	{
		StringData16 pattern(_pattern);
		return priv::string::Split<String16, sl_char16>(getData(), getLength(), pattern.getData(), pattern.getLength());
	}

	List<String> Atomic<String>::split(const StringParam& pattern) const noexcept
	{
		String s(*this);
		return s.split(pattern);
	}

	List<String16> Atomic<String16>::split(const StringParam& pattern) const noexcept
	{
		String16 s(*this);
		return s.split(pattern);
	}

	List<StringView> StringView::split(const StringParam& _pattern) const noexcept
	{
		StringData pattern(_pattern);
		return priv::string::Split<StringView, sl_char8>(getData(), getLength(), pattern.getData(), pattern.getLength());
	}

	List<StringView16> StringView16::split(const StringParam& _pattern) const noexcept
	{
		StringData16 pattern(_pattern);
		return priv::string::Split<StringView16, sl_char16>(getData(), getLength(), pattern.getData(), pattern.getLength());
	}


	String String::join(const String* strings, sl_size count, const StringParam& _delimiter) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0];
		}
		StringData delimiter(_delimiter);
		sl_char8* szDelimiter = delimiter.getData();
		sl_size lenDelimiter = delimiter.getLength();
		StringBuffer buf;
		for (sl_size i = 0; i < count; i++) {
			if (i) {
				buf.addStatic(szDelimiter, lenDelimiter);
			}
			buf.addStatic(strings[i].getData(), strings[i].getLength());
		}
		return buf.merge();
	}

	String16 String16::join(const String16* strings, sl_size count, const StringParam& _delimiter) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0];
		}
		StringData16 delimiter(_delimiter);
		sl_char16* szDelimiter = delimiter.getData();
		sl_size lenDelimiter = delimiter.getLength();
		StringBuffer16 buf;
		for (sl_size i = 0; i < count; i++) {
			if (i) {
				buf.addStatic(szDelimiter, lenDelimiter);
			}
			buf.addStatic(strings[i].getData(), strings[i].getLength());
		}
		return buf.merge();
	}

	String String::join(const String* strings, sl_size count) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0];
		}
		StringBuffer buf;
		for (sl_size i = 0; i < count; i++) {
			buf.addStatic(strings[i].getData(), strings[i].getLength());
		}
		return buf.merge();
	}

	String16 String16::join(const String16* strings, sl_size count) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0];
		}
		StringBuffer16 buf;
		for (sl_size i = 0; i < count; i++) {
			buf.addStatic(strings[i].getData(), strings[i].getLength());
		}
		return buf.merge();
	}

	String String::join(const StringParam* strings, sl_size count, const StringParam& _delimiter) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0].toString();
		}
		StringData delimiter(_delimiter);
		sl_char8* szDelimiter = delimiter.getData();
		sl_size lenDelimiter = delimiter.getLength();
		StringBuffer buf;
		for (sl_size i = 0; i < count; i++) {
			if (i) {
				buf.addStatic(szDelimiter, lenDelimiter);
			}
			StringData data(strings[i]);
			if (data.string.isNotNull()) {
				buf.add(data.string);
			} else {
				buf.addStatic(data.getData(), data.getLength());
			}
		}
		return buf.merge();
	}

	String16 String16::join(const StringParam* strings, sl_size count, const StringParam& _delimiter) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0].toString16();
		}
		StringData16 delimiter(_delimiter);
		sl_char16* szDelimiter = delimiter.getData();
		sl_size lenDelimiter = delimiter.getLength();
		StringBuffer16 buf;
		for (sl_size i = 0; i < count; i++) {
			if (i) {
				buf.addStatic(szDelimiter, lenDelimiter);
			}
			StringData16 data(strings[i]);
			if (data.string.isNotNull()) {
				buf.add(data.string);
			} else {
				buf.addStatic(data.getData(), data.getLength());
			}
		}
		return buf.merge();
	}

	String String::join(const StringParam* strings, sl_size count) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0].toString();
		}
		StringBuffer buf;
		for (sl_size i = 0; i < count; i++) {
			StringData data(strings[i]);
			if (data.string.isNotNull()) {
				buf.add(data.string);
			} else {
				buf.addStatic(data.getData(), data.getLength());
			}
		}
		return buf.merge();
	}

	String16 String16::join(const StringParam* strings, sl_size count) noexcept
	{
		if (!count) {
			return sl_null;
		}
		if (count == 1) {
			return strings[0].toString16();
		}
		StringBuffer16 buf;
		for (sl_size i = 0; i < count; i++) {
			StringData16 data(strings[i]);
			if (data.string.isNotNull()) {
				buf.add(data.string);
			} else {
				buf.addStatic(data.getData(), data.getLength());
			}
		}
		return buf.merge();
	}

	String String::join(const ListParam<String>& list, sl_size startIndex, sl_size count, const StringParam& delimiter) noexcept
	{
		if (count == 0) {
			return sl_null;
		}
		ListLocker<String> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size limit = items.count - startIndex;
		if (count > limit) {
			count = limit;
		}
		return join(items.data + startIndex, count, delimiter);
	}

	String16 String16::join(const ListParam<String16>& list, sl_size startIndex, sl_size count, const StringParam& delimiter) noexcept
	{
		if (count == 0) {
			return sl_null;
		}
		ListLocker<String16> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size limit = items.count - startIndex;
		if (count > limit) {
			count = limit;
		}
		return join(items.data + startIndex, count, delimiter);
	}

	String String::join(const ListParam<String>& list, sl_size startIndex, sl_size count) noexcept
	{
		if (count == 0) {
			return sl_null;
		}
		ListLocker<String> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size limit = items.count - startIndex;
		if (count > limit) {
			count = limit;
		}
		return join(items.data + startIndex, count);
	}

	String16 String16::join(const ListParam<String16>& list, sl_size startIndex, sl_size count) noexcept
	{
		if (count == 0) {
			return sl_null;
		}
		ListLocker<String16> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size limit = items.count - startIndex;
		if (count > limit) {
			count = limit;
		}
		return join(items.data + startIndex, count);
	}

	String String::join(const ListParam<String>& list, sl_size startIndex, const StringParam& delimiter) noexcept
	{
		ListLocker<String> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size count = items.count - startIndex;
		return join(items.data + startIndex, count, delimiter);
	}

	String16 String16::join(const ListParam<String16>& list, sl_size startIndex, const StringParam& delimiter) noexcept
	{
		ListLocker<String16> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size count = items.count - startIndex;
		return join(items.data + startIndex, count, delimiter);
	}

	String String::join(const ListParam<String>& list, sl_size startIndex) noexcept
	{
		ListLocker<String> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size count = items.count - startIndex;
		return join(items.data + startIndex, count);
	}

	String16 String16::join(const ListParam<String16>& list, sl_size startIndex) noexcept
	{
		ListLocker<String16> items(list);
		if (startIndex >= items.count) {
			return sl_null;
		}
		sl_size count = items.count - startIndex;
		return join(items.data + startIndex, count);
	}

	String String::join(const ListParam<String>& list, const StringParam& delimiter) noexcept
	{
		ListLocker<String> items(list);
		return join(items.data, items.count, delimiter);
	}

	String16 String16::join(const ListParam<String16>& list, const StringParam& delimiter) noexcept
	{
		ListLocker<String16> items(list);
		return join(items.data, items.count, delimiter);
	}

	String String::join(const ListParam<String>& list) noexcept
	{
		ListLocker<String> items(list);
		return join(items.data, items.count);
	}

	String16 String16::join(const ListParam<String16>& list) noexcept
	{
		ListLocker<String16> items(list);
		return join(items.data, items.count);
	}

#define MAX_NUMBER_STR_LEN 256
#define MAX_PRECISION 50
	
	namespace priv
	{
		namespace string
		{
			
			const char g_conv_radix_pattern_lower[65] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_";
			const char* g_conv_radixPatternLower = g_conv_radix_pattern_lower;
			const char g_conv_radix_pattern_upper[65] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@_";
			const char* g_conv_radixPatternUpper = g_conv_radix_pattern_upper;
			
			const sl_uint8 g_conv_radix_inverse_pattern_big[128] = {
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
			const sl_uint8* g_conv_radixInversePatternBig = g_conv_radix_inverse_pattern_big;
			
			const sl_uint8 g_conv_radix_inverse_pattern_small[128] = {
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
			const sl_uint8* g_conv_radixInversePatternSmall = g_conv_radix_inverse_pattern_small;
			
			template <class IT, class CT>
			SLIB_INLINE static sl_reg ParseInt(sl_int32 radix, const CT* sz, sl_size i, sl_size n, IT* _out) noexcept
			{
				if (i >= n) {
					return SLIB_PARSE_ERROR;
				}
				sl_int32 r = radix;
				
				const sl_uint8* pattern = r <= 36 ? g_conv_radix_inverse_pattern_small : g_conv_radix_inverse_pattern_big;
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
			SLIB_INLINE static sl_reg ParseUint(sl_int32 radix, const CT* sz, sl_size i, sl_size n, IT* _out) noexcept
			{
				if (i >= n) {
					return SLIB_PARSE_ERROR;
				}
				sl_uint32 r = radix;
				sl_bool bEmpty = sl_true;
				const sl_uint8* pattern = r <= 36 ? g_conv_radix_inverse_pattern_small : g_conv_radix_inverse_pattern_big;
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

			template <class FT, class CT>
			SLIB_INLINE static sl_reg ParseFloat(const CT* sz, sl_size i, sl_size n, FT* _out) noexcept
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

			template <class CT>
			SLIB_INLINE static sl_reg ParseBoolean(const CT* sz, sl_size i, sl_size n, sl_bool* _out) noexcept
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

			template <class CT>
			SLIB_INLINE static sl_reg ParseHexString(const CT* sz, sl_size i, sl_size n, void* _out) noexcept
			{
				if (i >= n || (n & 1)) {
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

			template <class IT, class UT, class ST, class CT>
			SLIB_INLINE static ST FromInt(IT _value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase, CT chGroup = sl_false, sl_bool flagSignPositive = sl_false, sl_bool flagLeadingSpacePositive = sl_false, sl_bool flagEncloseNagtive = sl_false) noexcept
			{
				if (radix < 2 || radix > 64) {
					return sl_null;
				}
				
				const char* pattern = flagUpperCase && radix <= 36 ? g_conv_radix_pattern_upper : g_conv_radix_pattern_lower;
				
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
			SLIB_INLINE static ST FromUint(IT value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase, CT chGroup = 0, sl_bool flagSignPositive = sl_false, sl_bool flagLeadingSpacePositive = sl_false) noexcept
			{
				if (radix < 2 || radix > 64) {
					return sl_null;
				}
				
				const char* pattern = flagUpperCase && radix <= 36 ? g_conv_radix_pattern_upper : g_conv_radix_pattern_lower;
				
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

			template <class FT, class ST, class CT>
			SLIB_INLINE static ST FromFloat(FT value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral, CT chConv = 'g', CT chGroup = 0, sl_bool flagSignPositive = sl_false, sl_bool flagLeadingSpacePositive = sl_false, sl_bool flagEncloseNagtive = sl_false) noexcept
			{
				
				CT buf[MAX_NUMBER_STR_LEN];
				
				if (Math::isNaN(value)) {
					static CT s[] = {'N', 'a', 'N', 0};
					return ST::fromStatic(s);
				}
				if (Math::isInfinite(value)) {
					static CT s[] = {'I', 'n', 'f', 'i', 'n', 'i', 't', 'y', 0};
					return ST::fromStatic(s);
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
					if (precision < 1) {
						precision = 1;
					} else if (precision > 50) {
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

			template <class ST, class CT>
			SLIB_INLINE static ST MakeHexString(const void* buf, sl_size size, sl_bool flagUseLowerChar) noexcept
			{
				if (!buf || size <= 0) {
					return sl_null;
				}
				ST str = ST::allocate(size * 2);
				if (str.isEmpty()) {
					return str;
				}
				CT* sz = (CT*)(str.getData());
				if (flagUseLowerChar) {
					for (sl_size i = 0; i < size; i++) {
						sl_uint8 v = ((sl_uint8*)(buf))[i];
						sz[i << 1] = g_conv_radix_pattern_lower[v >> 4];
						sz[(i << 1) + 1] = g_conv_radix_pattern_lower[v & 15];
					}
				} else {
					for (sl_size i = 0; i < size; i++) {
						sl_uint8 v = ((sl_uint8*)(buf))[i];
						sz[i << 1] = g_conv_radix_pattern_upper[v >> 4];
						sz[(i << 1) + 1] = g_conv_radix_pattern_upper[v & 15];
					}
				}
				return str;
			}

		}
	}

	sl_reg String::parseInt32(sl_int32 radix, sl_int32* _out, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseInt32(sl_int32 radix, sl_int32* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseInt32(sl_int32 radix, sl_int32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_int32 String::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		sl_int32 _out;
		if (parseInt32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_int32 String16::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		sl_int32 _out;
		if (parseInt32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_int32 StringView::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		sl_int32 _out;
		if (parseInt32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_int32 StringView16::parseInt32(sl_int32 radix, sl_int32 def) const noexcept
	{
		sl_int32 _out;
		if (parseInt32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseUint32(sl_int32 radix, sl_uint32* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseUint32(sl_int32 radix, sl_uint32* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_uint32 String::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		sl_uint32 _out;
		if (parseUint32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_uint32 String16::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		sl_uint32 _out;
		if (parseUint32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_uint32 StringView::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		sl_uint32 _out;
		if (parseUint32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_uint32 StringView16::parseUint32(sl_int32 radix, sl_uint32 def) const noexcept
	{
		sl_uint32 _out;
		if (parseUint32(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseInt64(sl_int32 radix, sl_int64* _out, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseInt64(sl_int32 radix, sl_int64* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseInt(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseInt64(sl_int32 radix, sl_int64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseInt(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_int64 String::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		sl_int64 _out;
		if (parseInt64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_int64 String16::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		sl_int64 _out;
		if (parseInt64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_int64 StringView::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		sl_int64 _out;
		if (parseInt64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_int64 StringView16::parseInt64(sl_int32 radix, sl_int64 def) const noexcept
	{
		sl_int64 _out;
		if (parseInt64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseUint64(sl_int32 radix, sl_uint64* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseUint64(sl_int32 radix, sl_uint64* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseUint(radix, sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseUint64(sl_int32 radix, sl_uint64* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseUint(radix, getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_uint64 String::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		sl_uint64 _out;
		if (parseUint64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_uint64 String16::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		sl_uint64 _out;
		if (parseUint64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_uint64 StringView::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		sl_uint64 _out;
		if (parseUint64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_uint64 StringView16::parseUint64(sl_int32 radix, sl_uint64 def) const noexcept
	{
		sl_uint64 _out;
		if (parseUint64(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_bool StringView::parseInt(sl_int32 radix, sl_reg* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(radix, _out);
#else
		return parseInt32(radix, _out);
#endif
	}

	sl_bool StringView16::parseInt(sl_int32 radix, sl_reg* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseInt64(radix, _out);
#else
		return parseInt32(radix, _out);
#endif
	}


	sl_reg String::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		sl_reg _out;
		if (parseInt(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_reg String16::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		sl_reg _out;
		if (parseInt(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_reg StringView::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		sl_reg _out;
		if (parseInt(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_reg StringView16::parseInt(sl_int32 radix, sl_reg def) const noexcept
	{
		sl_reg _out;
		if (parseInt(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_bool StringView::parseSize(sl_int32 radix, sl_size* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(radix, _out);
#else
		return parseUint32(radix, _out);
#endif
	}

	sl_bool StringView16::parseSize(sl_int32 radix, sl_size* _out) const noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return parseUint64(radix, _out);
#else
		return parseUint32(radix, _out);
#endif
	}

	sl_size String::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		sl_size _out;
		if (parseSize(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_size String16::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		sl_size _out;
		if (parseSize(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_size StringView::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		sl_size _out;
		if (parseSize(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_size StringView16::parseSize(sl_int32 radix, sl_size def) const noexcept
	{
		sl_size _out;
		if (parseSize(radix, &_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseFloat(float* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseFloat(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseFloat(float* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseFloat(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseFloat(float* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseFloat(float* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseFloat(float* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseFloat(float* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	float String::parseFloat(float def) const noexcept
	{
		float _out;
		if (parseFloat(&_out)) {
			return _out;
		} else {
			return def;
		}
	}

	float String16::parseFloat(float def) const noexcept
	{
		float _out;
		if (parseFloat(&_out)) {
			return _out;
		} else {
			return def;
		}
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

	float StringView::parseFloat(float def) const noexcept
	{
		float _out;
		if (parseFloat(&_out)) {
			return _out;
		} else {
			return def;
		}
	}

	float StringView16::parseFloat(float def) const noexcept
	{
		float _out;
		if (parseFloat(&_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseDouble(double* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseFloat(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseDouble(double* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseFloat(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseDouble(double* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseDouble(double* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseDouble(double* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseDouble(double* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseFloat(getData(), 0, n, _out) == (sl_reg)n;
	}

	double String::parseDouble(double def) const noexcept
	{
		double _out;
		if (parseDouble(&_out)) {
			return _out;
		} else {
			return def;
		}
	}

	double String16::parseDouble(double def) const noexcept
	{
		double _out;
		if (parseDouble(&_out)) {
			return _out;
		} else {
			return def;
		}
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

	double StringView::parseDouble(double def) const noexcept
	{
		double _out;
		if (parseDouble(&_out)) {
			return _out;
		} else {
			return def;
		}
	}

	double StringView16::parseDouble(double def) const noexcept
	{
		double _out;
		if (parseDouble(&_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseBoolean(sl_bool* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseBoolean(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseBoolean(sl_bool* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseBoolean(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseBoolean(sl_bool* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseBoolean(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseBoolean(sl_bool* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseBoolean(getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseBoolean(sl_bool* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseBoolean(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseBoolean(sl_bool* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseBoolean(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String::parseBoolean(sl_bool def) const noexcept
	{
		sl_bool _out;
		if (parseBoolean(&_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_bool String16::parseBoolean(sl_bool def) const noexcept
	{
		sl_bool _out;
		if (parseBoolean(&_out)) {
			return _out;
		} else {
			return def;
		}
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

	sl_bool StringView::parseBoolean(sl_bool def) const noexcept
	{
		sl_bool _out;
		if (parseBoolean(&_out)) {
			return _out;
		} else {
			return def;
		}
	}

	sl_bool StringView16::parseBoolean(sl_bool def) const noexcept
	{
		sl_bool _out;
		if (parseBoolean(&_out)) {
			return _out;
		} else {
			return def;
		}
	}


	sl_reg String::parseHexString(void* _out, const char* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseHexString(sz, posBegin, posEnd, _out);
	}

	sl_reg String16::parseHexString(void* _out, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::string::ParseHexString(sz, posBegin, posEnd, _out);
	}

	sl_bool String::parseHexString(void* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseHexString(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool String16::parseHexString(void* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseHexString(getData(), 0, n, _out) == (sl_reg)n;
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

	sl_bool StringView::parseHexString(void* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseHexString(getData(), 0, n, _out) == (sl_reg)n;
	}

	sl_bool StringView16::parseHexString(void* _out) const noexcept
	{
		sl_size n = getLength();
		if (n == 0) {
			return sl_false;
		}
		return priv::string::ParseHexString(getData(), 0, n, _out) == (sl_reg)n;
	}

	Memory String::parseHexString() const noexcept
	{
		sl_size n = getLength();
		if (n > 0 && !(n & 1)) {
			Memory mem = Memory::create(n >> 1);
			if (mem.isNotNull()) {
				if (priv::string::ParseHexString(getData(), 0, n, mem.getData()) == (sl_reg)n) {
					return mem;
				}
			}
		}
		return sl_null;
	}

	Memory String16::parseHexString() const noexcept
	{
		sl_size n = getLength();
		if (n > 0 && !(n & 1)) {
			Memory mem = Memory::create(n >> 1);
			if (mem.isNotNull()) {
				if (priv::string::ParseHexString(getData(), 0, n, mem.getData()) == (sl_reg)n) {
					return mem;
				}
			}
		}
		return sl_null;
	}

	Memory Atomic<String>::parseHexString() const noexcept
	{
		String s(*this);
		return s.parseHexString();
	}
	
	Memory Atomic<String16>::parseHexString() const noexcept
	{
		String16 s(*this);
		return s.parseHexString();
	}

	Memory StringView::parseHexString() const noexcept
	{
		sl_size n = getLength();
		if (n > 0 && !(n & 1)) {
			Memory mem = Memory::create(n >> 1);
			if (mem.isNotNull()) {
				if (priv::string::ParseHexString(getData(), 0, n, mem.getData()) == (sl_reg)n) {
					return mem;
				}
			}
		}
		return sl_null;
	}

	Memory StringView16::parseHexString() const noexcept
	{
		sl_size n = getLength();
		if (n > 0 && !(n & 1)) {
			Memory mem = Memory::create(n >> 1);
			if (mem.isNotNull()) {
				if (priv::string::ParseHexString(getData(), 0, n, mem.getData()) == (sl_reg)n) {
					return mem;
				}
			}
		}
		return sl_null;
	}


	String String::fromInt32(sl_int32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromInt<sl_int32, sl_uint32, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromInt32(sl_int32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromInt<sl_int32, sl_uint32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromUint32(sl_uint32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromUint<sl_uint32, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromUint32(sl_uint32 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromUint<sl_uint32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromInt64(sl_int64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromInt<sl_int64, sl_uint64, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromInt64(sl_int64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromInt<sl_int64, sl_uint64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
	}

	String String::fromUint64(sl_uint64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromUint<sl_uint64, String, sl_char8>(value, radix, minWidth, flagUpperCase);
	}

	String16 String16::fromUint64(sl_uint64 value, sl_uint32 radix, sl_uint32 minWidth, sl_bool flagUpperCase) noexcept
	{
		return priv::string::FromUint<sl_uint64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
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

	String String::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return priv::string::FromFloat<double, String, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String String::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return priv::string::FromFloat<float, String, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String16 String16::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return priv::string::FromFloat<double, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
	}

	String16 String16::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_uint32 minWidthIntegral) noexcept
	{
		return priv::string::FromFloat<float, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
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
			SLIB_RETURN_STRING("true")
		} else {
			SLIB_RETURN_STRING("false")
		}
	}

	String16 String16::fromBoolean(sl_bool value) noexcept
	{
		if (value) {
			SLIB_RETURN_STRING16("true")
		} else {
			SLIB_RETURN_STRING16("false")
		}
	}

	
	String String::makeHexString(const void* buf, sl_size size, sl_bool flagUseLowerChar) noexcept
	{
		return priv::string::MakeHexString<String, sl_char8>(buf, size, flagUseLowerChar);
	}

	String16 String16::makeHexString(const void* buf, sl_size size, sl_bool flagUseLowerChar) noexcept
	{
		return priv::string::MakeHexString<String16, sl_char16>(buf, size, flagUseLowerChar);
	}

	String String::makeHexString(const Memory& mem, sl_bool flagUseLowerChar) noexcept
	{
		return makeHexString(mem.getData(), mem.getSize(), flagUseLowerChar);
	}

	String16 String16::makeHexString(const Memory& mem, sl_bool flagUseLowerChar) noexcept
	{
		return makeHexString(mem.getData(), mem.getSize(), flagUseLowerChar);
	}


	String String::from(signed char value) noexcept
	{
		return fromInt32(value);
	}

	String String::from(unsigned char value) noexcept
	{
		return fromUint32(value);
	}

	String String::from(short value) noexcept
	{
		return fromInt32(value);
	}

	String String::from(unsigned short value) noexcept
	{
		return fromUint32(value);
	}

	String String::from(int value) noexcept
	{
		return fromInt32((sl_int32)value);
	}

	String String::from(unsigned int value) noexcept
	{
		return fromUint32((sl_uint32)value);
	}

	String String::from(long value) noexcept
	{
		return fromInt32((sl_int32)value);
	}

	String String::from(unsigned long value) noexcept
	{
		return fromUint32((sl_uint32)value);
	}

	String String::from(sl_int64 value) noexcept
	{
		return fromInt64(value);
	}

	String String::from(sl_uint64 value) noexcept
	{
		return fromUint64(value);
	}

	String String::from(float value) noexcept
	{
		return fromFloat(value);
	}

	String String::from(double value) noexcept
	{
		return fromDouble(value);
	}

	String String::from(sl_bool value) noexcept
	{
		return fromBoolean(value);
	}

	String String::from(const Time& value) noexcept
	{
		return value.toString();
	}

	String String::from(const Json& json) noexcept
	{
		return json.toJsonString();
	}

	String String::from(const Variant& var) noexcept
	{
		return var.toString();
	}

	String16 String16::from(signed char value) noexcept
	{
		return fromInt32(value);
	}

	String16 String16::from(unsigned char value) noexcept
	{
		return fromUint32(value);
	}

	String16 String16::from(short value) noexcept
	{
		return fromInt32(value);
	}

	String16 String16::from(unsigned short value) noexcept
	{
		return fromUint32(value);
	}

	String16 String16::from(int value) noexcept
	{
		return fromInt32((sl_int32)value);
	}

	String16 String16::from(unsigned int value) noexcept
	{
		return fromUint32((sl_uint32)value);
	}

	String16 String16::from(long value) noexcept
	{
		return fromInt32((sl_int32)value);
	}

	String16 String16::from(unsigned long value) noexcept
	{
		return fromUint32((sl_uint32)value);
	}

	String16 String16::from(sl_int64 value) noexcept
	{
		return fromInt64(value);
	}

	String16 String16::from(sl_uint64 value) noexcept
	{
		return fromUint64(value);
	}

	String16 String16::from(float value) noexcept
	{
		return fromFloat(value);
	}

	String16 String16::from(double value) noexcept
	{
		return fromDouble(value);
	}

	String16 String16::from(sl_bool value) noexcept
	{
		return fromBoolean(value);
	}

	String16 String16::from(const Time& value) noexcept
	{
		return String16::create(value.toString());
	}

	String16 String16::from(const Json& json) noexcept
	{
		return String16::create(json.toJsonString());
	}

	String16 String16::from(const Variant& var) noexcept
	{
		String16 s = var.getString16();
		if (s.isNotNull()) {
			return s;
		}
		return create(var.toString());
	}


/*

	String Formatting is similar with Java Formatter

https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html

	%[argument_index$][flags][width][.precision]conversion

*/

	namespace priv
	{
		namespace string
		{
			
			template <class ST, class CT, class BT>
			static ST Format(const Locale& locale, const CT* format, sl_size len, const Variant* params, sl_size _nParams) noexcept
			{
				if (len == 0) {
					return ST::getEmpty();
				}
				sl_uint32 nParams = (sl_uint32)_nParams;
				if (nParams == 0) {
					return ST(format, len);
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
										case 'Y':
											content = ST::fromInt32(time.getYear(*zone) % 100, 10, 2);
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
											content = ST::from(time.getWeekdayShort(*zone, locale));
											break;
										case 'W':
											content = ST::from(time.getWeekdayLong(*zone, locale));
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
										case 'h': // Hour-12
											if (flagZeroPadded) {
												if (flagZeroPadded) {
													if (minWidth < 2) {
														minWidth = 2;
													}
												}
												content = ST::fromInt32(time.getHour12(*zone), 10, minWidth);
											} else {
												content = ST::fromInt32(time.getHour12(*zone));
											}
											break;
										case 'a':
											content = ST::from(time.getAM_PM(*zone, locale));
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
											content = ST::from(time.getDateString(*zone));
											break;
										case 'T':
											content = ST::from(time.getTimeString(*zone));
											break;
										case 'O':
											content = ST::from(time.getMonthLong(*zone));
											break;
										case 'o':
											content = ST::from(time.getMonthShort(*zone));
											break;
										case 's':
											content = ST::from(time.toString(*zone));
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
															sb.add(ST::from(str));
															sb.add(ST(' ', minWidth - lenContent));
														} else {
															sb.add(ST(' ', minWidth - lenContent));
															sb.add(ST::from(str));
														}
													} else {
														sb.add(ST::from(str));
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
													String16 str = String16::from(arg);
													lenContent = str.getLength();
													if (lenContent < minWidth) {
														if (flagAlignLeft) {
															sb.add(ST::from(str));
															sb.add(ST(' ', minWidth - lenContent));
														} else {
															sb.add(ST(' ', minWidth - lenContent));
															sb.add(ST::from(str));
														}
													} else {
														sb.add(ST::from(str));
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
												content = priv::string::FromUint<sl_uint32, ST, CT>(arg.getUint32(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive);
											} else if (arg.isInt32()) {
												content = priv::string::FromInt<sl_int32, sl_uint32, ST, CT>(arg.getInt32(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
											} else if (arg.isUint64()) {
												content = priv::string::FromUint<sl_uint64, ST, CT>(arg.getUint64(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive);
											} else {
												content = priv::string::FromInt<sl_int64, sl_uint64, ST, CT>(arg.getInt64(), radix, _minWidth, flagUpperCase, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
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
												content = priv::string::FromFloat<float, ST, CT>(arg.getFloat(), _precision, flagZeroPadded, 1, ch, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
											} else {
												content = priv::string::FromFloat<double, ST, CT>(arg.getDouble(), _precision, flagZeroPadded, 1, ch, chGroup, flagSignPositive, flagLeadingSpacePositive, flagEncloseNegative);
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
											ST str = ST::create(&unicode, 1);
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

		}
	}

	String String::formatBy(const StringParam& _format, const Variant *params, sl_size nParams) noexcept
	{
		StringData format(_format);
		return priv::string::Format<String, sl_char8, StringBuffer>(Locale::Unknown, format.getData(), format.getLength(), params, nParams);
	}

	String16 String16::formatBy(const StringParam& _format, const Variant *params, sl_size nParams) noexcept
	{
		StringData16 format(_format);
		return priv::string::Format<String16, sl_char16, StringBuffer16>(Locale::Unknown, format.getData(), format.getLength(), params, nParams);
	}

	String String::formatBy(const StringParam& _format, const ListParam<Variant>& _params) noexcept
	{
		StringData format(_format);
		ListLocker<Variant> params(_params);
		return priv::string::Format<String, sl_char8, StringBuffer>(Locale::Unknown, format.getData(), format.getLength(), params.data, params.count);
	}

	String16 String16::formatBy(const StringParam& _format, const ListParam<Variant>& _params) noexcept
	{
		StringData16 format(_format);
		ListLocker<Variant> params(_params);
		return priv::string::Format<String16, sl_char16, StringBuffer16>(Locale::Unknown, format.getData(), format.getLength(), params.data, params.count);
	}

	String String::formatBy(const Locale& locale, const StringParam& _format, const Variant *params, sl_size nParams) noexcept
	{
		StringData format(_format);
		return priv::string::Format<String, sl_char8, StringBuffer>(locale, format.getData(), format.getLength(), params, nParams);
	}
	
	String16 String16::formatBy(const Locale& locale, const StringParam& _format, const Variant *params, sl_size nParams) noexcept
	{
		StringData16 format(_format);
		return priv::string::Format<String16, sl_char16, StringBuffer16>(locale, format.getData(), format.getLength(), params, nParams);
	}
	
	String String::formatBy(const Locale& locale, const StringParam& _format, const ListParam<Variant>& _params) noexcept
	{
		StringData format(_format);
		ListLocker<Variant> params(_params);
		return priv::string::Format<String, sl_char8, StringBuffer>(locale, format.getData(), format.getLength(), params.data, params.count);
	}
	
	String16 String16::formatBy(const Locale& locale, const StringParam& _format, const ListParam<Variant>& _params) noexcept
	{
		StringData16 format(_format);
		ListLocker<Variant> params(_params);
		return priv::string::Format<String16, sl_char16, StringBuffer16>(locale, format.getData(), format.getLength(), params.data, params.count);
	}
	
	String String::format(const StringParam& strFormat) noexcept
	{
		return strFormat.toString();
	}

	String16 String16::format(const StringParam& strFormat) noexcept
	{
		return strFormat.toString16();
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

	String String::argBy(const ListParam<Variant>& params) const noexcept
	{
		return String::formatBy(*this, params);
	}

	String16 String16::argBy(const ListParam<Variant>& params) const noexcept
	{
		return String16::formatBy(*this, params);
	}

	String Atomic<String>::argBy(const ListParam<Variant>& params) const noexcept
	{
		return String::formatBy(*this, params);
	}

	String16 Atomic<String16>::argBy(const ListParam<Variant>& params) const noexcept
	{
		return String16::formatBy(*this, params);
	}


	sl_compare_result Compare<String>::operator()(const String& a, const String& b) const noexcept
	{
		return a.compare(b);
	}

	sl_compare_result Compare<String16>::operator()(const String16& a, const String16& b) const noexcept
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


	sl_size Hash<String>::operator()(const String& v) const noexcept
	{
		return v.getHashCode();
	}

	sl_size Hash<String16>::operator()(const String16& v) const noexcept
	{
		return v.getHashCode();
	}


	sl_compare_result CompareIgnoreCaseString::operator()(const String& a, const String& b) const noexcept
	{
		return a.compareIgnoreCase(b);
	}

	sl_compare_result CompareIgnoreCaseString16::operator()(const String16& a, const String16& b) const noexcept
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
		return String::from(var);
	}

	String16 Cast<Variant, String16>::operator()(const Variant& var) const noexcept
	{
		return String16::from(var);
	}

	String Cast<Json, String>::operator()(const Json& json) const noexcept
	{
		return json.getString();
	}
	
	String16 Cast<Json, String16>::operator()(const Json& json) const noexcept
	{
		return json.getString16();
	}


#define DEFINE_STRING_OPERATOR_SUB2(STRING1, STRING2, RET, FUNC, OP, OP_STRING) \
	RET FUNC(const STRING1& s1, const STRING2& s2) noexcept \
	{ \
		return priv::string::OperatorHelper< STRING1, priv::string::OP<OP_STRING> >::call(s1, s2); \
	}

#define DEFINE_STRING_OPERATOR_SUB2_ATOMIC(STRING1, STRING2, RET, FUNC, OP, OP_STRING) \
	DEFINE_STRING_OPERATOR_SUB2(STRING1, STRING2, RET, FUNC, OP, OP_STRING) \
	RET FUNC(const Atomic<STRING1>& _s1, const STRING2& s2) noexcept \
	{ \
		STRING1 s1(_s1); \
		return priv::string::OperatorHelper< STRING1, priv::string::OP<OP_STRING> >::call(s1, s2); \
	}

#define DEFINE_STRING8_OPERATOR_SUB(STRING, RET, FUNC, OP) \
	DEFINE_STRING_OPERATOR_SUB2_ATOMIC(String, STRING, RET, FUNC, OP, String) \
	DEFINE_STRING_OPERATOR_SUB2(StringView, STRING, RET, FUNC, OP, String)

#define DEFINE_STRING16_OPERATOR_SUB(STRING, RET, FUNC, OP) \
	DEFINE_STRING_OPERATOR_SUB2_ATOMIC(String16, STRING, RET, FUNC, OP, String16) \
	DEFINE_STRING_OPERATOR_SUB2(StringView16, STRING, RET, FUNC, OP, String16)

#define DEFINE_STRING_PARAM_OPERATOR_SUB(STRING, RET, FUNC, OP) \
	RET FUNC(const StringParam& s1, const STRING& s2) noexcept \
	{ \
		if (s1.isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(String::null(), s2); \
		} else { \
			if (s1.isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(s1.toString16(), s2); \
			} else if (s1.isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1.toString(), s2); \
			} else { \
				if (s1.isSz16()) { \
					StringData16 s(s1); \
					return priv::string::OperatorHelper< StringView16, priv::string::OP<String16> >::call((const StringView16&)s, s2); \
				} else { \
					StringData s(s1); \
					return priv::string::OperatorHelper< StringView, priv::string::OP<String> >::call((const StringView&)s, s2); \
				} \
			} \
		} \
	} \

#define DEFINE_STRING8_OPERATOR(RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(String, RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(AtomicString, RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(StringView, RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(String16, RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(AtomicString16, RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(StringView16, RET, FUNC, OP) \
	DEFINE_STRING8_OPERATOR_SUB(StringParam, RET, FUNC, OP)

#define DEFINE_STRING16_OPERATOR(RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(String, RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(AtomicString, RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(StringView, RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(String16, RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(AtomicString16, RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(StringView16, RET, FUNC, OP) \
	DEFINE_STRING16_OPERATOR_SUB(StringParam, RET, FUNC, OP)

#define DEFINE_STRING_PARAM_OPERATOR(RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(String, RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(AtomicString, RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(StringView, RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(String16, RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(AtomicString16, RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(StringView16, RET, FUNC, OP) \
	DEFINE_STRING_PARAM_OPERATOR_SUB(StringParam, RET, FUNC, OP)

	DEFINE_STRING8_OPERATOR(String, operator+, PlusOperator)
	DEFINE_STRING16_OPERATOR(String16, operator+, PlusOperator)
	DEFINE_STRING_PARAM_OPERATOR(StringParam, operator+, PlusOperator)

#define DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING1, STRING2, BODY) \
	sl_bool operator OP(const STRING1& s1, const STRING2& s2) noexcept \
	{ \
		return BODY; \
	}

#define DEFINE_COMPARE_OPERATOR_SUB(OP, STRING, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, String, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, AtomicString, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, String16, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, AtomicString16, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, StringView, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, StringView16, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB2(OP, STRING, StringParam, BODY)

#define DEFINE_COMPARE_OPERATOR(OP, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, String, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, AtomicString, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, String16, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, AtomicString16, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, StringView, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, StringView16, BODY) \
	DEFINE_COMPARE_OPERATOR_SUB(OP, StringParam, BODY)

	DEFINE_COMPARE_OPERATOR(==, s1.equals(s2))
	DEFINE_COMPARE_OPERATOR(!=, !(s1.equals(s2)))
	DEFINE_COMPARE_OPERATOR(>=, s1.compare(s2)>=0)
	DEFINE_COMPARE_OPERATOR(<=, s1.compare(s2)<=0)
	DEFINE_COMPARE_OPERATOR(>, s1.compare(s2)>0)
	DEFINE_COMPARE_OPERATOR(<, s1.compare(s2)<0)

}
