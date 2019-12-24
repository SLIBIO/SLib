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

#include "slib/core/string.h"

#include "slib/core/variant.h"

namespace slib
{

#define PTR_VAR(TYPE, x) ((TYPE*)((void*)(&(x))))
#define REF_VAR(TYPE, x) (*PTR_VAR(TYPE, x))

#define STRING_TYPE_STRING8 ((sl_size)-1)
#define STRING_TYPE_STRING16 ((sl_size)-2)
#define STRING_TYPE_STRING_REF8 ((sl_size)-3)
#define STRING_TYPE_STRING_REF16 ((sl_size)-4)

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

	namespace priv
	{
		namespace string_param
		{
			
			const ConstContainer g_undefined = {0, 0};
			const ConstContainer g_null = {0, 1};

			SLIB_INLINE static void copy_param(StringParam& dst, const StringParam& src) noexcept
			{
				if (src._value) {
					switch (src._length) {
						case STRING_TYPE_STRING8:
							dst._length = STRING_TYPE_STRING_REF8;
							REF_VAR(String*, dst._value) = PTR_VAR(String, src._value);
							break;
						case STRING_TYPE_STRING16:
							dst._length = STRING_TYPE_STRING_REF16;
							REF_VAR(String16*, dst._value) = PTR_VAR(String16, src._value);
							break;
						default:
							dst._value = src._value;
							dst._length = src._length;
							break;
					}
				} else {
					dst._value = 0;
					dst._length = src._length;
				}
			}

			SLIB_INLINE static void free_param(sl_size value, sl_size length) noexcept
			{
				switch (length)
				{
					case STRING_TYPE_STRING8:
						REF_VAR(String, value).String::~String();
						break;
					case STRING_TYPE_STRING16:
						REF_VAR(String16, value).String16::~String16();
						break;
					default:
						break;
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
					return String::equals(v1->getData(), v1->getLength(), v2, len2);
				}
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v2, sl_size len2, const String* v1, sl_size len1) noexcept
			{
				return equals_element(v1, len1, v2, len2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_size len1, const sl_char16* v2, sl_size len2) noexcept
			{
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
					case STRING_TYPE_STRING8:
						return equals_element(v1, len1, PTR_VAR(String const, v2._value), 0);
					case STRING_TYPE_STRING16:
						return equals_element(v1, len1, PTR_VAR(String16 const, v2._value), 0);
					case STRING_TYPE_STRING_REF8:
						return equals_element(v1, len1, REF_VAR(String const*, v2._value), 0);
					case STRING_TYPE_STRING_REF16:
						return equals_element(v1, len1, REF_VAR(String16 const*, v2._value), 0);
					default:
						if (IS_SZ16(v2._length)) {
							return equals_element(v1, len1, REF_VAR(sl_char16 const*, v2._value), GET_LENGTH(v2._length));
						} else {
							return equals_element(v1, len1, REF_VAR(sl_char8 const*, v2._value), GET_LENGTH(v2._length));
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
					case STRING_TYPE_STRING8:
						return compare_element(v1, PTR_VAR(String const, v2._value));
					case STRING_TYPE_STRING16:
						return compare_element(v1, PTR_VAR(String16 const, v2._value));
					case STRING_TYPE_STRING_REF8:
						return compare_element(v1, REF_VAR(String const*, v2._value));
					case STRING_TYPE_STRING_REF16:
						return compare_element(v1, REF_VAR(String16 const*, v2._value));
					default:
						if (IS_SZ16(v2._length)) {
							return compare_element(v1, REF_VAR(sl_char16 const*, v2._value));
						} else {
							return compare_element(v1, REF_VAR(sl_char8 const*, v2._value));
						}
				}
			}
		}
	}
	
	using namespace priv::string_param;
	
	void StringParam::_free(sl_size value, sl_size length) noexcept
	{
		free_param(value, length);
	}
	
	StringParam::StringParam(StringParam&& other) noexcept
	{
		_value = other._value;
		_length = other._length;
		other._value = 0;
	}
	
	StringParam::StringParam(const StringParam& other) noexcept
	{
		copy_param(*this, other);
	}
	
	StringParam::~StringParam() noexcept
	{
		free_param(_value, _length);
	}

	StringParam::StringParam(const String& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING_REF8;
			REF_VAR(String const*, _value) = &value;
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(String&& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING_REF16;
			REF_VAR(String16 const*, _value) = &value;
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(String16&& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const AtomicString& s) noexcept
	{
		String value(s);
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(AtomicString&& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const AtomicString16& s) noexcept
	{
		String16 value(s);
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(AtomicString16&& value) noexcept
	{
		if (value.isNotNull()) {
			_length = STRING_TYPE_STRING16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const sl_char8* sz8) noexcept
	{
		if (sz8) {
			REF_VAR(const sl_char8*, _value) = sz8;
			_length = 0;
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const sl_char16* sz16) noexcept
	{
		if (sz16) {
			REF_VAR(const sl_char16*, _value) = sz16;
			_length = STRING_TYPE_SZ16_PREFIX;
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const sl_char8* sz8, sl_reg length) noexcept
	{
		if (sz8) {
			if (length < 0) {
				length = 0;
			}
			REF_VAR(const sl_char8*, _value) = sz8;
			_length = GET_LENGTH(length);
		} else {
			_value = 0;
			_length = 1;
		}
	}

	StringParam::StringParam(const sl_char16* sz16, sl_reg length) noexcept
	{
		if (sz16) {
			if (length < 0) {
				length = 0;
			}
			REF_VAR(const sl_char16*, _value) = sz16;
			_length = STRING_TYPE_SZ16_PREFIX | GET_LENGTH(length);
		} else {
			_value = 0;
			_length = 1;
		}
	}
	
	StringParam& StringParam::operator=(StringParam&& other) noexcept
	{
		if (this != &other) {
			free_param(_value, _length);
			_value = other._value;
			_length = other._length;
			other._value = 0;
		}
		return *this;
	}

	StringParam& StringParam::operator=(const StringParam& other) noexcept
	{
		if (this != &other) {
			free_param(_value, _length);
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
			free_param(_value, _length);
			_length = STRING_TYPE_STRING_REF8;
			REF_VAR(String const*, _value) = &value;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(String&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING_REF16;
			REF_VAR(String16 const*, _value) = &value;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(String16&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const AtomicString& s) noexcept
	{
		String value(s);
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(AtomicString&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const AtomicString16& s) noexcept
	{
		String16 value(s);
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(AtomicString16&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_value, _length);
			_length = STRING_TYPE_STRING16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const sl_char8* value) noexcept
	{
		if (value) {
			free_param(_value, _length);
			REF_VAR(const sl_char8*, _value) = value;
			_length = 0;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const sl_char16* value) noexcept
	{
		if (value) {
			free_param(_value, _length);
			REF_VAR(const sl_char16*, _value) = value;
			_length = STRING_TYPE_SZ16_PREFIX;
		} else {
			setNull();
		}
		return *this;
	}

	void StringParam::setUndefined() noexcept
	{
		if (_value) {
			free_param(_value, _length);
			_value = 0;
		}
		_length = 0;
	}

	void StringParam::setNull() noexcept
	{
		if (_value) {
			free_param(_value, _length);
			_value = 0;
		}
		_length = 1;
	}

	sl_bool StringParam::isEmpty() const noexcept
	{
		if (!_value) {
			return sl_true;
		}
		switch (_length) {
			case STRING_TYPE_STRING8:
				return REF_VAR(String, _value).isEmpty();
			case STRING_TYPE_STRING16:
				return REF_VAR(String16, _value).isEmpty();
			case STRING_TYPE_STRING_REF8:
				return REF_VAR(String const*, _value)->isEmpty();
			case STRING_TYPE_STRING_REF16:
				return REF_VAR(String16 const*, _value)->isEmpty();
			default:
				if (IS_SZ16(_length)) {
					return (REF_VAR(sl_char16 const*, _value))[0] == 0;
				} else {
					return (REF_VAR(sl_char8 const*, _value))[0] == 0;
				}
		}
	}
	
	sl_bool StringParam::isNotEmpty() const noexcept
	{
		return !(isEmpty());
	}
	
	sl_bool StringParam::isString8() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING8 || _length == STRING_TYPE_STRING_REF8);
	}

	sl_bool StringParam::isString16() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING16 || _length == STRING_TYPE_STRING_REF16);
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
		return _value && (_length == STRING_TYPE_STRING8 || _length == STRING_TYPE_STRING_REF8 || IS_SZ8(_length));
	}

	sl_bool StringParam::is16() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING16 || _length == STRING_TYPE_STRING_REF16 || IS_SZ16(_length));
	}

	String StringParam::toString() const noexcept
	{
		if (!_value) {
			return sl_null;
		}
		switch (_length) {
			case STRING_TYPE_STRING8:
				return REF_VAR(String, _value);
			case STRING_TYPE_STRING16:
				return REF_VAR(String16, _value);
			case STRING_TYPE_STRING_REF8:
				return *REF_VAR(String const*, _value);
			case STRING_TYPE_STRING_REF16:
				return *REF_VAR(String16 const*, _value);
			default:
				if (IS_SZ16(_length)) {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String(REF_VAR(sl_char16 const*, _value), len);
					} else {
						return String(REF_VAR(sl_char16 const*, _value), -1);
					}
				} else {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String(REF_VAR(sl_char8 const*, _value), len);
					} else {
						return String(REF_VAR(sl_char8 const*, _value), -1);
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
			case STRING_TYPE_STRING8:
				return REF_VAR(String, _value);
			case STRING_TYPE_STRING16:
				return REF_VAR(String16, _value);
			case STRING_TYPE_STRING_REF8:
				return *REF_VAR(String const*, _value);
			case STRING_TYPE_STRING_REF16:
				return *REF_VAR(String16 const*, _value);
			default:
				if (IS_SZ16(_length)) {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String16(REF_VAR(sl_char16 const*, _value), len);
					} else {
						return String16(REF_VAR(sl_char16 const*, _value), -1);
					}
				} else {
					sl_size len = GET_LENGTH(_length);
					if (len) {
						return String16(REF_VAR(sl_char8 const*, _value), len);
					} else {
						return String16(REF_VAR(sl_char8 const*, _value), -1);
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
			case STRING_TYPE_STRING8:
				return REF_VAR(String, _value);
			case STRING_TYPE_STRING16:
				return REF_VAR(String16, _value);
			case STRING_TYPE_STRING_REF8:
				return *REF_VAR(String const*, _value);
			case STRING_TYPE_STRING_REF16:
				return *REF_VAR(String16 const*, _value);
			default:
				if (IS_SZ16(_length)) {
					return Variant(REF_VAR(sl_char16 const*, _value));
				} else {
					return Variant(REF_VAR(sl_char8 const*, _value));
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
				case STRING_TYPE_STRING8:
					return REF_VAR(String, v1._value).compare(REF_VAR(String, v2._value));
				case STRING_TYPE_STRING16:
					return REF_VAR(String16, v1._value).compare(REF_VAR(String16, v2._value));
				case STRING_TYPE_STRING_REF8:
					return REF_VAR(String const*, v1._value)->compare(*REF_VAR(String const*, v2._value));
				case STRING_TYPE_STRING_REF16:
					return REF_VAR(String16 const*, v1._value)->compare(*REF_VAR(String16 const*, v2._value));
				default:
					if (IS_SZ16(v1._length)) {
						return Base::compareString2(REF_VAR(sl_char16 const*, v1._value), REF_VAR(sl_char16 const*, v2._value));
					} else {
						return Base::compareString(REF_VAR(sl_char8 const*, v1._value), REF_VAR(sl_char8 const*, v2._value));
					}

			}
		} else {
			switch (v1._length) {
				case STRING_TYPE_STRING8:
					return compare_param(PTR_VAR(String, v1._value), v2);
				case STRING_TYPE_STRING16:
					return compare_param(PTR_VAR(String16, v1._value), v2);
				case STRING_TYPE_STRING_REF8:
					return compare_param(REF_VAR(String const*, v1._value), v2);
				case STRING_TYPE_STRING_REF16:
					return compare_param(REF_VAR(String16 const*, v1._value), v2);
				default:
					if (IS_SZ16(v1._length)) {
						return compare_param(REF_VAR(sl_char16 const*, v1._value), v2);
					} else {
						return compare_param(REF_VAR(sl_char8 const*, v1._value), v2);
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
				case STRING_TYPE_STRING8:
					return REF_VAR(String, v1._value) == REF_VAR(String, v2._value);
				case STRING_TYPE_STRING16:
					return REF_VAR(String16, v1._value) == REF_VAR(String16, v2._value);
				case STRING_TYPE_STRING_REF8:
					return REF_VAR(String const*, v1._value)->equals(*REF_VAR(String const*, v2._value));
				case STRING_TYPE_STRING_REF16:
					return REF_VAR(String16 const*, v1._value)->equals(*REF_VAR(String16 const*, v2._value));
				default:
					if (IS_SZ16(v1._length)) {
						sl_size len = GET_LENGTH(v1._length);
						if (len) {
							return Base::equalsMemory2(REF_VAR(sl_uint16 const*, v1._value), REF_VAR(sl_uint16 const*, v2._value), len);
						} else {
							return Base::compareString2(REF_VAR(sl_char16 const*, v1._value), REF_VAR(sl_char16 const*, v2._value)) == 0;
						}
					} else {
						sl_size len = GET_LENGTH(v1._length);
						if (len) {
							return Base::equalsMemory(REF_VAR(sl_char8 const*, v1._value), REF_VAR(sl_char8 const*, v2._value), len);
						} else {
							return Base::compareString(REF_VAR(sl_char8 const*, v1._value), REF_VAR(sl_char8 const*, v2._value)) == 0;
						}
					}
			}
		} else {
			switch (v1._length) {
				case STRING_TYPE_STRING8:
					return equals_param(PTR_VAR(String, v1._value), 0, v2);
				case STRING_TYPE_STRING16:
					return equals_param(PTR_VAR(String16, v1._value), 0, v2);
				case STRING_TYPE_STRING_REF8:
					return equals_param(REF_VAR(String const*, v1._value), 0, v2);
				case STRING_TYPE_STRING_REF16:
					return equals_param(REF_VAR(String16 const*, v1._value), 0, v2);
				default:
					if (IS_SZ16(v1._length)) {
						return equals_param(REF_VAR(sl_char16 const*, v1._value), GET_LENGTH(v1._length), v2);
					} else {
						return equals_param(REF_VAR(sl_char8 const*, v1._value), GET_LENGTH(v1._length), v2);
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
			case STRING_TYPE_STRING8:
				return REF_VAR(String, _value).getHashCode();
			case STRING_TYPE_STRING16:
				return REF_VAR(String16, _value).getHashCode();
			case STRING_TYPE_STRING_REF8:
				return REF_VAR(String const*, _value)->getHashCode();
			case STRING_TYPE_STRING_REF16:
				return REF_VAR(String16 const*, _value)->getHashCode();
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
				case STRING_TYPE_STRING8:
					data = REF_VAR(String, param._value).getData(length);
					break;
				case STRING_TYPE_STRING_REF8:
					data = REF_VAR(String const*, param._value)->getData(length);
					break;
				case STRING_TYPE_STRING16:
				case STRING_TYPE_STRING_REF16:
					string = param.toString();
					data = string.getData(length);
					break;
				default:
					if (IS_SZ16(param._length)) {
						string = param.toString();
						data = string.getData(length);
					} else {
						data = REF_VAR(sl_char8* const, param._value);
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
				case STRING_TYPE_STRING16:
					data = REF_VAR(String16, param._value).getData(length);
					break;
				case STRING_TYPE_STRING_REF16:
					data = REF_VAR(String16 const*, param._value)->getData(length);
					break;
				case STRING_TYPE_STRING8:
				case STRING_TYPE_STRING_REF8:
					string = param.toString16();
					data = string.getData(length);
					break;
				default:
					if (IS_SZ16(param._length)) {
						data = REF_VAR(sl_char16* const, param._value);
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
