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

#include "slib/core/variant.h"

namespace slib
{

#define STRING_TYPE_STRING8_REF ((sl_reg)(StringType::String8_Ref))
#define STRING_TYPE_STRING16_REF ((sl_reg)(StringType::String16_Ref))
#define STRING_TYPE_STRING8_NOREF ((sl_reg)(StringType::String8_NoRef))
#define STRING_TYPE_STRING16_NOREF ((sl_reg)(StringType::String16_NoRef))

#ifdef SLIB_ARCH_IS_64BIT
#define IS_SZ8(length) (((length) >> 62) == 0)
#define IS_SZ16(length) (((length) >> 62) == 1)
#define STRING_TYPE_SZ16_PREFIX SLIB_UINT64(0x4000000000000000)
#define GET_LENGTH(length) ((length) & SLIB_UINT64(0x3FFFFFFFFFFFFFFF))
#else
#define IS_SZ8(length) (((length) >> 30) == 0)
#define IS_SZ16(length) (((length) >> 30) == 1)
#define STRING_TYPE_SZ16_PREFIX 0x40000000
#define GET_LENGTH(length) ((length) & 0x3FFFFFFF)
#endif

#define STRING_PTR(s) ((String*)(void*)(&((s)._value)))
#define STRING_REF(s) (*STRING_PTR(s))
#define STRING16_PTR(s) ((String16*)(void*)(&((s)._value)))
#define STRING16_REF(s) (*STRING16_PTR(s))
#define STRING_CONTAINER(s) (*((void**)(void*)(&(s))))
#define STRING_CONTAINER16(s) (*((void**)(void*)(&(s))))

	namespace priv
	{
		namespace string_param
		{
			
			const ConstContainer g_undefined = {sl_null, 0};
			const ConstContainer g_null = {sl_null, 1};
		
			SLIB_INLINE static void copy_param(StringParam& dst, const StringParam& src) noexcept
			{
				dst._value = src._value;
				dst._length = src._length;
				if (src._value) {
					switch (src._length) {
						case STRING_TYPE_STRING8_REF:
							dst._length = STRING_TYPE_STRING8_NOREF;
							break;
						case STRING_TYPE_STRING16_REF:
							dst._length = STRING_TYPE_STRING16_NOREF;
							break;
					}
				}
			}

			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_size len1, const String* v2, sl_size len2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_size len1, const String16* v2, sl_size len2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v2, sl_size len2, const String* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_size len1, const sl_char8* v2, sl_size len2) noexcept
			{
				if (len2) {
					if (v1->getLength() != len2) {
						return sl_false;
					}
					return Base::equalsMemory(v1->getData(), v2, len2);
				} else {
					if (!len2) {
						len2 = -1;
					}
					return String::equals(v1->getData(), v1->getLength(), v2, len2);
				}
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v2, sl_size len2, const String* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_size len1, const sl_char16* v2, sl_size len2) noexcept
			{
				if (!len2) {
					len2 = -1;
				}
				return String::equals(v1->getData(), v1->getLength(), v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v2, sl_size len2, const String* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, sl_size len1, const String16* v2, sl_size len2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, sl_size len1, const sl_char8* v2, sl_size len2) noexcept
			{
				if (!len2) {
					len2 = -1;
				}
				return String16::equals(v1->getData(), v1->getLength(), v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v2, sl_size len2, const String16* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, sl_size len1, const sl_char16* v2, sl_size len2) noexcept
			{
				if (len2) {
					if (v1->getLength() != len2) {
						return sl_false;
					}
					return Base::equalsMemory2((sl_uint16*)(v1->getData()), (sl_uint16*)v2, len2);
				} else {
					if (!len2) {
						len2 = -1;
					}
					return String16::equals(v1->getData(), v1->getLength(), v2, len2);
				}
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v2, sl_size len2, const String16* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v1, sl_size len1, const sl_char8* v2, sl_size len2) noexcept
			{
				if (len1 && len2) {
					if (len1 != len2) {
						return sl_false;
					}
					return Base::equalsMemory(v1, v2, len1);
				} else {
					return Base::compareString(v1, v2) == 0;
				}
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v1, sl_size len1, const sl_char16* v2, sl_size len2) noexcept
			{
				if (!len1) {
					len1 = -1;
				}
				if (!len2) {
					len2 = -1;
				}
				return String::equals(v1, len1, v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v2, sl_size len2, const sl_char8* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v1, sl_size len1, const sl_char16* v2, sl_size len2) noexcept
			{
				if (len1 && len2) {
					if (len1 != len2) {
						return sl_false;
					}
					return Base::equalsMemory2((sl_uint16*)v1, (sl_uint16*)v2, len1);
				} else {
					return Base::compareString2(v1, v2) == 0;
				}
			}
			
			
			template <class T>
			SLIB_INLINE static sl_bool equals_param(const T* v1, sl_size len1, const StringParam& v2) noexcept
			{
				if (v2.isNull()) {
					return sl_false;
				}
				switch (v2._length) {
					case STRING_TYPE_STRING8_REF:
					case STRING_TYPE_STRING8_NOREF:
						return equals_element(v1, len1, STRING_PTR(v2), 0);
					case STRING_TYPE_STRING16_REF:
					case STRING_TYPE_STRING16_NOREF:
						return equals_element(v1, len1, STRING16_PTR(v2), 0);
					default:
						if (IS_SZ16(v2._length)) {
							return equals_element(v1, len1, (sl_char16*)(v2._value), GET_LENGTH(v2._length));
						} else {
							return equals_element(v1, len1, (sl_char8*)(v2._value), GET_LENGTH(v2._length));
						}
				}
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, const String* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, const String16* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v2, const String* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, const sl_char8* v2) noexcept
			{
				return String::compare(v1->getData(), v1->getLength(), v2, -1);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char8* v2, const String* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, const sl_char16* v2) noexcept
			{
				return String::compare(v1->getData(), v1->getLength(), v2, -1);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char16* v2, const String* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, const String16* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, const sl_char8* v2) noexcept
			{
				return String16::compare(v1->getData(), v1->getLength(), v2, -1);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char8* v2, const String16* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, const sl_char16* v2) noexcept
			{
				return String16::compare(v1->getData(), v1->getLength(), v2, -1);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char16* v2, const String16* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char8* v1, const sl_char8* v2) noexcept
			{
				return Base::compareString(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char8* v1, const sl_char16* v2) noexcept
			{
				return String::compare(v1, -1, v2, -1);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char16* v2, const sl_char8* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char16* v1, const sl_char16* v2) noexcept
			{
				return Base::compareString2(v1, v2) == 0;
			}
			
			
			template <class T>
			SLIB_INLINE static sl_compare_result compare_param(const T* v1, const StringParam& v2) noexcept
			{
				if (v2.isNull()) {
					return 1;
				}
				switch (v2._length) {
					case STRING_TYPE_STRING8_REF:
					case STRING_TYPE_STRING8_NOREF:
						return compare_element(v1, STRING_PTR(v2));
					case STRING_TYPE_STRING16_REF:
					case STRING_TYPE_STRING16_NOREF:
						return compare_element(v1, STRING16_PTR(v2));
					default:
						if (IS_SZ16(v2._length)) {
							return compare_element(v1, (sl_char16*)(v2._value));
						} else {
							return compare_element(v1, (sl_char8*)(v2._value));
						}
				}
			}
		}
	}
	
	using namespace priv::string_param;
	
	void StringParam::_free() noexcept
	{
		switch (_length)
		{
			case STRING_TYPE_STRING8_REF:
				STRING_REF(*this).String::~String();
				break;
			case STRING_TYPE_STRING16_REF:
				STRING16_REF(*this).String16::~String16();
				break;
			default:
				break;
		}
	}
	
	StringParam::StringParam(StringParam&& other) noexcept
	{
		_value = other._value;
		_length = other._length;
		other._value = sl_null;
		other._length = 0;
	}
	
	StringParam::StringParam(const StringParam& other) noexcept
	{
		copy_param(*this, other);
	}
	
	StringParam::~StringParam() noexcept
	{
		_free();
	}

	StringParam::StringParam(const String& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING8_NOREF;
			_value = STRING_CONTAINER(value);
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(String&& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING8_REF;
			new STRING_PTR(*this) String(Move(value));
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING16_NOREF;
			_value = STRING_CONTAINER16(value);
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(String16&& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING16_REF;
			new STRING16_PTR(*this) String16(Move(value));
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const AtomicString& s) noexcept: StringParam(String(s))
	{
	}

	StringParam::StringParam(AtomicString&& s) noexcept: StringParam(String(Move(s)))
	{
	}

	StringParam::StringParam(const AtomicString16& s) noexcept: StringParam(String16(s))
	{
	}

	StringParam::StringParam(AtomicString16&& s) noexcept: StringParam(String16(Move(s)))
	{
	}

	StringParam::StringParam(const char* sz) noexcept
	{
		if (sz) {
			_value = (void*)sz;
			_length = 0;
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const wchar_t* sz) noexcept
	{
		if (sz) {
			if (sizeof(wchar_t) == 2) {
				_value = (void*)sz;
				_length = STRING_TYPE_SZ16_PREFIX;
			} else {
				_length = STRING_TYPE_STRING8_REF;
				new STRING_PTR(*this) String(sz);
			}
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char16_t* sz) noexcept
	{
		if (sz) {
			_value = (void*)sz;
			_length = STRING_TYPE_SZ16_PREFIX;
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char32_t* sz) noexcept: StringParam(String(sz))
	{
	}

	StringParam::StringParam(const char* str, sl_reg length) noexcept
	{
		if (str) {
			if (length < 0) {
				length = 0;
			}
			_value = (void*)str;
			_length = GET_LENGTH(length);
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const wchar_t* str, sl_reg length) noexcept
	{
		if (str) {
			if (sizeof(wchar_t) == 2) {
				if (length < 0) {
					length = 0;
				}
				_value = (void*)str;
				_length = STRING_TYPE_SZ16_PREFIX | GET_LENGTH(length);
			} else {
				_length = STRING_TYPE_STRING8_REF;
				new STRING_PTR(*this) String(str, length);
			}
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char16_t* str, sl_reg length) noexcept
	{
		if (str) {
			if (length < 0) {
				length = 0;
			}
			_value = (void*)str;
			_length = STRING_TYPE_SZ16_PREFIX | GET_LENGTH(length);
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char32_t* str, sl_reg length) noexcept: StringParam(String(str, length))
	{
	}

	StringParam::StringParam(const StringData& str) noexcept
	{
		_value = str.data;
		_length = str.getLengthForParser();
		if (_length & SLIB_SIZE_TEST_SIGN_BIT) {
			_length = 0;
		} else {
			_length = GET_LENGTH(_length);
		}
	}
	
	StringParam::StringParam(const StringData16& str) noexcept
	{
		_value = str.data;
		_length = str.getLengthForParser();
		if (_length & SLIB_SIZE_TEST_SIGN_BIT) {
			_length = STRING_TYPE_SZ16_PREFIX;
		} else {
			_length = STRING_TYPE_SZ16_PREFIX | GET_LENGTH(_length);
		}
	}

	StringParam::StringParam(const std::string& str) noexcept: StringParam(str.c_str(), str.length())
	{
	}

	StringParam::StringParam(const std::wstring& str) noexcept: StringParam(str.c_str(), str.length())
	{
	}

	StringParam::StringParam(const std::u16string& str) noexcept: StringParam(str.c_str(), str.length())
	{
	}

	StringParam::StringParam(const std::u32string& str) noexcept: StringParam(str.c_str(), str.length())
	{
	}

	StringParam& StringParam::operator=(StringParam&& other) noexcept
	{
		if (this != &other) {
			_free();
			_value = other._value;
			_length = other._length;
			other._value = sl_null;
			other._length = 0;
		}
		return *this;
	}

	StringParam& StringParam::operator=(const StringParam& other) noexcept
	{
		if (this != &other) {
			_free();
			copy_param(*this, other);
		}
		return *this;
	}

	StringParam& StringParam::operator=(sl_null_t) noexcept
	{
		setNull();
		return *this;
	}
	
	StringParam& StringParam::operator=(const String& value) noexcept
	{
		if (value.isNotNull()) {
			_free();
			_length = STRING_TYPE_STRING8_NOREF;
			_value = STRING_CONTAINER(value);
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(String&& value) noexcept
	{
		if (value.isNotNull()) {
			_free();
			_length = STRING_TYPE_STRING8_REF;
			new STRING_PTR(*this) String(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			_free();
			_length = STRING_TYPE_STRING16_NOREF;
			_value = STRING_CONTAINER16(value);
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(String16&& value) noexcept
	{
		if (value.isNotNull()) {
			_free();
			_length = STRING_TYPE_STRING16_REF;
			new STRING16_PTR(*this) String16(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const AtomicString& s) noexcept
	{
		return *this = String(s);
	}
	
	StringParam& StringParam::operator=(AtomicString&& s) noexcept
	{
		return *this = String(Move(s));
	}
	
	StringParam& StringParam::operator=(const AtomicString16& s) noexcept
	{
		return *this = String16(s);
	}
	
	StringParam& StringParam::operator=(AtomicString16&& s) noexcept
	{
		return *this = String16(Move(s));
	}
	
	StringParam& StringParam::operator=(const char* value) noexcept
	{
		if (value) {
			_free();
			_value = (void*)value;
			_length = 0;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const wchar_t* value) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			return *this = (char16_t*)value;
		} else {
			return *this = String(value);
		}
	}

	StringParam& StringParam::operator=(const char16_t* value) noexcept
	{
		if (value) {
			_free();
			_value = (void*)value;
			_length = STRING_TYPE_SZ16_PREFIX;
		} else {
			setNull();
		}
		return *this;
	}

	StringParam& StringParam::operator=(const char32_t* value) noexcept
	{
		return *this = String(value);
	}

	void StringParam::_assign(const sl_char8* str, sl_reg length) noexcept
	{
		_free();
		_value = (void*)str;
		_length = length;
		if (_length & SLIB_SIZE_TEST_SIGN_BIT) {
			_length = 0;
		} else {
			_length = GET_LENGTH(_length);
		}

	}

	void StringParam::_assign(const sl_char16* str, sl_reg length) noexcept
	{
		_free();
		_value = (void*)str;
		_length = length;
		if (_length & SLIB_SIZE_TEST_SIGN_BIT) {
			_length = STRING_TYPE_SZ16_PREFIX;
		} else {
			_length = STRING_TYPE_SZ16_PREFIX | GET_LENGTH(_length);
		}
	}

	StringParam& StringParam::operator=(const StringData& str) noexcept
	{
		_assign(str.data, str.getLengthForParser());
		return *this;
	}

	StringParam& StringParam::operator=(const StringData16& str) noexcept
	{
		_assign(str.data, str.getLengthForParser());
		return *this;
	}

	StringParam& StringParam::operator=(const std::string& str) noexcept
	{
		_assign(str.c_str(), str.length());
		return *this;
	}

	StringParam& StringParam::operator=(const std::wstring& str) noexcept
	{
		if (sizeof(wchar_t) == 2) {
			_assign((sl_char16*)(str.c_str()), str.length());
			return *this;
		} else {
			return *this = String(str);
		}
	}

	StringParam& StringParam::operator=(const std::u16string& str) noexcept
	{
		_assign((sl_char16*)(str.c_str()), str.length());
		return *this;
	}

	StringParam& StringParam::operator=(const std::u32string& str) noexcept
	{
		return *this = String(str);
	}

	void StringParam::setUndefined() noexcept
	{
		if (_value) {
			_free();
			_value = sl_null;
		}
		_length = 0;
	}

	void StringParam::setNull() noexcept
	{
		if (_value) {
			_free();
			_value = sl_null;
		}
		_length = 1;
	}

	sl_bool StringParam::isEmpty() const noexcept
	{
		if (!_value) {
			return sl_true;
		}
		switch (_length) {
			case STRING_TYPE_STRING8_REF:
			case STRING_TYPE_STRING8_NOREF:
				return STRING_REF(*this).isEmpty();
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this).isEmpty();
			default:
				if (IS_SZ16(_length)) {
					return !(*((sl_char16*)_value));
				} else {
					return !(*((sl_char8*)_value));
				}
		}
	}
	
	sl_bool StringParam::isNotEmpty() const noexcept
	{
		return !(isEmpty());
	}
	
	sl_bool StringParam::isString8() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING8_REF || _length == STRING_TYPE_STRING8_NOREF);
	}

	sl_bool StringParam::isString16() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING16_REF || _length == STRING_TYPE_STRING16_NOREF);
	}

	sl_bool StringParam::isSz8() const noexcept
	{
		return _value && IS_SZ8(_length);
	}

	sl_bool StringParam::isSz16() const noexcept
	{
		return _value && IS_SZ16(_length);
	}

	sl_bool StringParam::is8() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING8_REF || _length == STRING_TYPE_STRING8_NOREF || IS_SZ8(_length));
	}

	sl_bool StringParam::is16() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING16_REF || _length == STRING_TYPE_STRING16_NOREF || IS_SZ16(_length));
	}

	String StringParam::toString() const noexcept
	{
		if (!_value) {
			return sl_null;
		}
		switch (_length) {
			case STRING_TYPE_STRING8_REF:
			case STRING_TYPE_STRING8_NOREF:
				return STRING_REF(*this);
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this);
			default:
				if (IS_SZ16(_length)) {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String((sl_char16*)_value, len);
					} else {
						return String((sl_char16*)_value, -1);
					}
				} else {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String((sl_char8*)_value, len);
					} else {
						return String((sl_char8*)_value, -1);
					}
				}
		}
	}

	String16 StringParam::toString16() const noexcept
	{
		if (!_value) {
			return sl_null;
		}
		switch (_length) {
			case STRING_TYPE_STRING8_REF:
			case STRING_TYPE_STRING8_NOREF:
				return STRING_REF(*this);
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this);
			default:
				if (IS_SZ16(_length)) {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String16((sl_char16*)_value, len);
					} else {
						return String16((sl_char16*)_value, -1);
					}
				} else {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String16((sl_char8*)_value, len);
					} else {
						return String16((sl_char8*)_value, -1);
					}
				}
		}
	}

	Variant StringParam::toVariant() const noexcept
	{
		if (!_value) {
			return sl_null;
		}
		switch (_length) {
			case STRING_TYPE_STRING8_REF:
			case STRING_TYPE_STRING8_NOREF:
				return STRING_REF(*this);
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this);
			default:
				if (IS_SZ16(_length)) {
					return (sl_char16*)_value;
				} else {
					return (sl_char8*)_value;
				}
		}
	}

	sl_compare_result StringParam::compare(const StringParam& v2) const noexcept
	{
		const StringParam& v1 = *this;
		if (!(v1._value)) {
			if (v2._value) {
				return -1;
			}
			return 0;
		}
		if (v1._length == v2._length) {
			switch (v1._length) {
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					return STRING_REF(v1).compare(STRING_REF(v2));
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					return STRING16_REF(v1).compare(STRING16_REF(v2));
				default:
					if (IS_SZ16(v1._length)) {
						return Base::compareString2((sl_char16*)(v1._value), (sl_char16*)(v2._value));
					} else {
						return Base::compareString((sl_char8*)(v1._value), (sl_char8*)(v2._value));
					}

			}
		} else {
			switch (v1._length) {
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					return compare_param(STRING_PTR(v1), v2);
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					return compare_param(STRING16_PTR(v1), v2);
				default:
					if (IS_SZ16(v1._length)) {
						return compare_param((sl_char16*)(v1._value), v2);
					} else {
						return compare_param((sl_char8*)(v1._value), v2);
					}
			}
		}
		return 0;
	}
	
	sl_bool StringParam::equals(const StringParam& v2) const noexcept
	{
		const StringParam& v1 = *this;
		if (!(v1._value)) {
			return !(v2._value);
		}
		if (v1._length == v2._length) {
			switch (v1._length) {
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					return STRING_REF(v1) == STRING_REF(v2);
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					return STRING16_REF(v1) == STRING16_REF(v2);
				default:
					if (IS_SZ16(v1._length)) {
						sl_size len = GET_LENGTH(v1._length);
						if (len) {
							return Base::equalsMemory2((sl_uint16*)(v1._value), (sl_uint16*)(v2._value), len);
						} else {
							return Base::compareString2((sl_char16*)(v1._value), (sl_char16*)(v2._value)) == 0;
						}
					} else {
						sl_size len = GET_LENGTH(v1._length);
						if (len) {
							return Base::equalsMemory((sl_char8*)(v1._value), (sl_char8*)(v2._value), len);
						} else {
							return Base::compareString((sl_char8*)(v1._value), (sl_char8*)(v2._value)) == 0;
						}
					}
			}
		} else {
			switch (v1._length) {
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					return equals_param(STRING_PTR(v1), 0, v2);
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					return equals_param(STRING16_PTR(v1), 0, v2);
				default:
					if (IS_SZ16(v1._length)) {
						return equals_param((sl_char16*)(v1._value), GET_LENGTH(v1._length), v2);
					} else {
						return equals_param((sl_char8*)(v1._value), GET_LENGTH(v1._length), v2);
					}
			}
		}
		return sl_false;
	}
	
	sl_size StringParam::getHashCode() const noexcept
	{
		if (!_value) {
			return 0;
		}
		switch (_length) {
			case STRING_TYPE_STRING8_REF:
			case STRING_TYPE_STRING8_NOREF:
				return STRING_REF(*this).getHashCode();
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this).getHashCode();
			default:
				if (IS_SZ16(_length)) {
					return toString16().getHashCode();
				} else {
					return toString().getHashCode();
				}
		}
	}
	
	sl_bool operator==(const StringParam& v1, const StringParam& v2) noexcept
	{
		return v1.equals(v2);
	}

	sl_bool operator!=(const StringParam& v1, const StringParam& v2) noexcept
	{
		return !(v1.equals(v2));
	}

	
	sl_compare_result Compare<StringParam>::operator()(const StringParam& a, const StringParam& b) const noexcept
	{
		return a.compare(b);
	}
	
	sl_bool Equals<StringParam>::operator()(const StringParam& a, const StringParam& b) const noexcept
	{
		return a.equals(b);
	}
	
	sl_size Hash<StringParam>::operator()(const StringParam& a) const noexcept
	{
		return a.getHashCode();
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringData)

	StringData::StringData(const StringParam& param) noexcept
	{
		if (param._value) {
			switch (param._length) {
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					data = STRING_REF(param).getData(length);
					break;
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					string = param.toString();
					data = string.getData(length);
					break;
				default:
					if (IS_SZ16(param._length)) {
						string = param.toString();
						data = string.getData(length);
					} else {
						data = (sl_char8*)(param._value);
						length = GET_LENGTH(param._length);
						if (!length) {
							if (data[0]) {
								length = SLIB_SIZE_MAX;
							} else {
								length = 0;
							}
						}
					}
					break;
			}
		} else {
			static sl_char8 s[] = "";
			data = s;
			length = 0;
		}
	}

	StringData::StringData(const sl_char8* _str) noexcept
	{
		if (_str) {
			data = (sl_char8*)_str;
			length = SLIB_SIZE_MAX;
		} else {
			static sl_char8 s[] = "";
			data = s;
			length = 0;
		}
	}

	StringData::StringData(const sl_char8* _str, sl_size _length) noexcept
	{
		if (_str && _length) {
			data = (sl_char8*)_str;
			length = _length;
		} else {
			static sl_char8 s[] = "";
			data = s;
			length = 0;
		}
	}

	sl_bool StringData::isEmpty() const noexcept
	{
		return !length;
	}

	sl_bool StringData::isNotEmpty() const noexcept
	{
		return length != 0;
	}

	sl_size StringData::getLength() const noexcept
	{
		if (length & SLIB_SIZE_TEST_SIGN_BIT) {
			length = Base::getStringLength(data);
		}
		return length;
	}

	sl_size StringData::getLengthForParser() const noexcept
	{
		return length;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringData16)

	StringData16::StringData16(const StringParam& param) noexcept
	{
		if (param._value) {
			switch (param._length) {
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					data = STRING16_REF(param).getData(length);
					break;
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					string = param.toString16();
					data = string.getData(length);
					break;
				default:
					if (IS_SZ16(param._length)) {
						data = (sl_char16*)(param._value);
						length = GET_LENGTH(param._length);
						if (!length) {
							if (data[0]) {
								length = SLIB_SIZE_MAX;
							} else {
								length = 0;
							}
						}
					} else {
						string = param.toString16();
						data = string.getData(length);
					}
					break;
			}
		} else {
			static sl_char16 s[] = SLIB_UNICODE("");
			data = s;
			length = 0;
		}
	}

	StringData16::StringData16(const sl_char16* _str) noexcept
	{
		if (_str) {
			data = (sl_char16*)_str;
			length = SLIB_SIZE_MAX;
		} else {
			static sl_char16 s[] = SLIB_UNICODE("");
			data = s;
			length = 0;
		}
	}

	StringData16::StringData16(const sl_char16* _str, sl_size _length) noexcept
	{
		if (_str && _length) {
			data = (sl_char16*)_str;
			length = _length;
		} else {
			static sl_char16 s[] = SLIB_UNICODE("");
			data = s;
			length = 0;
		}
	}

	sl_bool StringData16::isEmpty() const noexcept
	{
		return !length;
	}

	sl_bool StringData16::isNotEmpty() const noexcept
	{
		return length != 0;
	}

	sl_size StringData16::getLength() const noexcept
	{
		if (length & SLIB_SIZE_TEST_SIGN_BIT) {
			length = Base::getStringLength2(data);
		}
		return length;
	}

	sl_size StringData16::getLengthForParser() const noexcept
	{
		return length;
	}

}
