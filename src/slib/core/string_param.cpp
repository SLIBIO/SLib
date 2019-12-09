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

#define PTR_VAR(TYPE, x) ((TYPE*)((void*)(&(x))))
#define REF_VAR(TYPE, x) (*PTR_VAR(TYPE, x))

namespace slib
{
	
	namespace priv
	{
		namespace string_param
		{
			
			typedef sl_size value_t;
			
			const ConstContainer g_undefined = {0, StringType::Null};
			const ConstContainer g_null = {(value_t)1, StringType::Null};

			SLIB_INLINE static void copy_param(StringType src_type, value_t src_value, value_t& dst_value) noexcept
			{
				switch (src_type) {
					case StringType::String8:
						new PTR_VAR(String, dst_value) String(REF_VAR(String, src_value));
						break;
					case StringType::String16:
						new PTR_VAR(String16, dst_value) String16(REF_VAR(String16, src_value));
						break;
					default:
						dst_value = src_value;
						break;
				}
			}

			SLIB_INLINE static void free_param(StringType type, value_t value) noexcept
			{
				switch (type)
				{
					case StringType::String8:
						REF_VAR(String, value).String::~String();
						break;
					case StringType::String16:
						REF_VAR(String16, value).String16::~String16();
						break;
					default:
						break;
				}
			}

			
			SLIB_INLINE static sl_bool equals_element(const String* v1, const String* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, const String16* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v2, const String* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_char8 const* const* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(sl_char8 const* const* v2, const String* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, sl_char16 const* const* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(sl_char16 const* const* v2, const String* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, const String16* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, sl_char8 const* const* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(sl_char8 const* const* v2, const String16* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, sl_char16 const* const* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(sl_char16 const* const* v2, const String16* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(sl_char8 const* const* v1, sl_char8 const* const* v2) noexcept
			{
				return Base::compareString(*v1, *v2) == 0;
			}
			
			SLIB_INLINE static sl_bool equals_element(sl_char8 const* const* v1, sl_char16 const* const* v2) noexcept
			{
				return *v1 == String(*v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(sl_char16 const* const* v2, sl_char8 const* const* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(sl_char16 const* const* v1, sl_char16 const* const* v2) noexcept
			{
				return Base::compareString2(*v1, *v2) == 0;
			}
			
			
			template <class T>
			SLIB_INLINE static sl_bool equals_param(const T* v1, const StringParam& v2) noexcept
			{
				StringType type = v2._type;
				switch (type) {
					case StringType::String8:
						return equals_element(v1, PTR_VAR(String const, v2._value));
					case StringType::String16:
						return equals_element(v1, PTR_VAR(String16 const, v2._value));
					case StringType::Sz8:
						return equals_element(v1, PTR_VAR(sl_char8 const* const, v2._value));
					case StringType::Sz16:
						return equals_element(v1, PTR_VAR(sl_char16 const* const, v2._value));
					default:
						break;
				}
				return sl_false;
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
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, sl_char8 const* const* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char8 const* const* v2, const String* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, sl_char16 const* const* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char16 const* const* v2, const String* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, const String16* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, sl_char8 const* const* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char8 const* const* v2, const String16* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, sl_char16 const* const* v2) noexcept
			{
				return v1->compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char16 const* const* v2, const String16* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char8 const* const* v1, sl_char8 const* const* v2) noexcept
			{
				return Base::compareString(*v1, *v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char8 const* const* v1, sl_char16 const* const* v2) noexcept
			{
				return String(*v1).compare(*v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char16 const* const* v2, sl_char8 const* const* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_compare_result compare_element(sl_char16 const* const* v1, sl_char16 const* const* v2) noexcept
			{
				return Base::compareString2(*v1, *v2) == 0;
			}
			
			
			template <class T>
			SLIB_INLINE static sl_compare_result compare_param(const T* v1, const StringParam& v2) noexcept
			{
				StringType type = v2._type;
				switch (type) {
					case StringType::String8:
						return compare_element(v1, PTR_VAR(String const, v2._value));
					case StringType::String16:
						return compare_element(v1, PTR_VAR(String16 const, v2._value));
					case StringType::Sz8:
						return compare_element(v1, PTR_VAR(sl_char8 const* const, v2._value));
					case StringType::Sz16:
						return compare_element(v1, PTR_VAR(sl_char16 const* const, v2._value));
					default:
						break;
				}
				return sl_false;
			}
		}
	}
	
	using namespace priv::string_param;
	
	void StringParam::_free(StringType type, value_t value) noexcept
	{
		free_param(type, value);
	}
	
	StringParam::StringParam(StringParam&& other) noexcept
	{
		_type = other._type;
		_value = other._value;
		other._type = StringType::Null;
	}
	
	StringParam::StringParam(const StringParam& other) noexcept
	{
		_type = other._type;
		copy_param(_type, other._value, _value);
	}
	
	StringParam::~StringParam() noexcept
	{
		free_param(_type, _value);
	}

	StringParam::StringParam(const String& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(value);
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(value);
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(const AtomicString& s) noexcept
	{
		String value(s);
		if (value.isNotNull()) {
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(value);
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(const AtomicString16& s) noexcept
	{
		String16 value(s);
		if (value.isNotNull()) {
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(value);
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(const sl_char8* sz8) noexcept
	{
		if (sz8) {
			_type = StringType::Sz8;
			REF_VAR(const sl_char8*, _value) = sz8;
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(const sl_char16* sz16) noexcept
	{
		if (sz16) {
			_type = StringType::Sz16;
			REF_VAR(const sl_char16*, _value) = sz16;
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}
	
	StringParam::StringParam(const std::string& value) noexcept
	{
		_type = StringType::String8;
		new PTR_VAR(String, _value) String(value);
	}
	
	StringParam::StringParam(const std::u16string& value) noexcept
	{
		_type = StringType::String16;
		new PTR_VAR(String16, _value) String16(value);
	}

	StringParam StringParam::fromString(const String& value) noexcept
	{
		return value;
	}

	StringParam StringParam::fromString16(const String16& value) noexcept
	{
		return value;
	}

	StringParam StringParam::fromSz8(const sl_char8* value) noexcept
	{
		return value;
	}

	StringParam StringParam::fromSz16(const sl_char16* value) noexcept
	{
		return value;
	}

	StringParam& StringParam::operator=(StringParam&& other) noexcept
	{
		if (this != &other) {
			free_param(_type, _value);
			_type = other._type;
			_value = other._value;
			other._type = StringType::Null;
		}
		return *this;
	}

	StringParam& StringParam::operator=(const StringParam& other) noexcept
	{
		if (this != &other) {
			free_param(_type, _value);
			_type = other._type;
			copy_param(_type, other._value, _value);
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
		setString(value);
		return *this;
	}
	
	StringParam& StringParam::operator=(const String16& value) noexcept
	{
		setString16(value);
		return *this;
	}
	
	StringParam& StringParam::operator=(const AtomicString& value) noexcept
	{
		setString(value);
		return *this;
	}
	
	StringParam& StringParam::operator=(const AtomicString16& value) noexcept
	{
		setString16(value);
		return *this;
	}
	
	StringParam& StringParam::operator=(const std::string& value) noexcept
	{
		setStdString(value);
		return *this;
	}
	
	StringParam& StringParam::operator=(const std::u16string& value) noexcept
	{
		setStdString16(value);
		return *this;
	}
	
	StringParam& StringParam::operator=(const sl_char8* sz8) noexcept
	{
		setSz(sz8);
		return *this;
	}
	
	StringParam& StringParam::operator=(const sl_char16* sz16) noexcept
	{
		setSz(sz16);
		return *this;
	}

	void StringParam::setUndefined() noexcept
	{
		if (_type != StringType::Null) {
			free_param(_type, _value);
			_type = StringType::Null;
		}
		_value = 0;
	}

	void StringParam::setNull() noexcept
	{
		if (_type != StringType::Null) {
			free_param(_type, _value);
			_type = StringType::Null;
		}
		_value = (value_t)1;
	}

	sl_size StringParam::getLength() const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String const, _value).getLength();
			case StringType::String16:
				return REF_VAR(String16 const, _value).getLength();
			case StringType::Sz8:
				return Base::getStringLength(REF_VAR(sl_char8 const* const, _value));
			case StringType::Sz16:
				return Base::getStringLength2(REF_VAR(sl_char16 const* const, _value));
			default:
				break;
		}
		return 0;
	}
	
	sl_bool StringParam::isEmpty() const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String const, _value).isEmpty();
			case StringType::String16:
				return REF_VAR(String16 const, _value).isEmpty();
			case StringType::Sz8:
				return (REF_VAR(sl_char8 const* const, _value))[0] == 0;
			case StringType::Sz16:
				return (REF_VAR(sl_char16 const* const, _value))[0] == 0;
			default:
				break;
		}
		return sl_true;
	}
	
	sl_bool StringParam::isNotEmpty() const noexcept
	{
		return !(isEmpty());
	}
	
	sl_bool StringParam::isString8() const noexcept
	{
		return _type == StringType::String8;
	}

	sl_bool StringParam::isString16() const noexcept
	{
		return _type == StringType::String16;
	}

	sl_bool StringParam::isSz8() const noexcept
	{
		return _type == StringType::Sz8;
	}

	sl_bool StringParam::isSz16() const noexcept
	{
		return _type == StringType::Sz16;
	}

	String StringParam::getString(const String& def) const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String const, _value);
			case StringType::String16:
				return REF_VAR(String16 const, _value);
			case StringType::Sz8:
				return REF_VAR(sl_char8 const* const, _value);
			case StringType::Sz16:
				return REF_VAR(sl_char16 const* const, _value);
			default:
				break;
		}
		return def;
	}

	String StringParam::getString() const noexcept
	{
		return getString(String::null());
	}

	String16 StringParam::getString16(const String16& def) const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String const, _value);
			case StringType::String16:
				return REF_VAR(String16 const, _value);
			case StringType::Sz8:
				return REF_VAR(sl_char8 const* const, _value);
			case StringType::Sz16:
				return REF_VAR(sl_char16 const* const, _value);
			default:
				break;
		}
		return def;
	}

	String16 StringParam::getString16() const noexcept
	{
		return getString16(String16::null());
	}

	sl_char8* StringParam::getSz8(const sl_char8* def) const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String const, _value).getData();
			case StringType::Sz8:
				return REF_VAR(sl_char8* const, _value);
			default:
				break;
		}
		return (sl_char8*)def;
	}

	sl_char16* StringParam::getSz16(const sl_char16* def) const noexcept
	{
		switch (_type) {
			case StringType::String16:
				return REF_VAR(String16 const, _value).getData();
			case StringType::Sz16:
				return REF_VAR(sl_char16* const, _value);
			default:
				break;
		}
		return (sl_char16*)def;
	}

	void StringParam::setString(const String& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(value);
		} else {
			setNull();
		}
	}

	void StringParam::setString16(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(value);
		} else {
			setNull();
		}
	}

	void StringParam::setSz(const sl_char8* value) noexcept
	{
		if (value) {
			free_param(_type, _value);
			_type = StringType::Sz8;
			REF_VAR(const sl_char8*, _value) = value;
		} else {
			setNull();
		}
	}

	void StringParam::setSz(const sl_char16* value) noexcept
	{
		if (value) {
			free_param(_type, _value);
			_type = StringType::Sz16;
			REF_VAR(const sl_char16*, _value) = value;
		} else {
			setNull();
		}
	}
	
	std::string StringParam::getStdString(const std::string& def) const noexcept
	{
		return getString(def).toStd();
	}
	
	std::string StringParam::getStdString() const noexcept
	{
		return getString().toStd();
	}
	
	std::u16string StringParam::getStdString16(const std::u16string& def) const noexcept
	{
		return getString16(def).toStd();
	}
	
	std::u16string StringParam::getStdString16() const noexcept
	{
		return getString16().toStd();
	}
	
	void StringParam::setStdString(const std::string& value) noexcept
	{
		free_param(_type, _value);
		_type = StringType::String8;
		new PTR_VAR(String, _value) String(value);
	}
	
	void StringParam::setStdString16(const std::u16string& value) noexcept
	{
		free_param(_type, _value);
		_type = StringType::String16;
		new PTR_VAR(String16, _value) String16(value);
	}
	
	sl_compare_result StringParam::compare(const StringParam& v2) const noexcept
	{
		const StringParam& v1 = *this;
		StringType type = v1._type;
		if (type == v2._type) {
			switch (type) {
				case StringType::Null:
					return 0;
				case StringType::Sz8:
					return Base::compareString(REF_VAR(sl_char8 const* const, v1._value), REF_VAR(sl_char8 const* const, v2._value));
				case StringType::Sz16:
					return Base::compareString2(REF_VAR(sl_char16 const* const, v1._value), REF_VAR(sl_char16 const* const, v2._value));
				case StringType::String8:
					return REF_VAR(String const, v1._value).compare(REF_VAR(String const, v2._value));
				case StringType::String16:
					return REF_VAR(String16 const, v1._value).compare(REF_VAR(String16 const, v2._value));
				default:
					return ComparePrimitiveValues(v1._value, v2._value);
			}
		} else {
			switch (type) {
				case StringType::String8:
					return compare_param(PTR_VAR(String const, v1._value), v2);
				case StringType::String16:
					return compare_param(PTR_VAR(String16 const, v1._value), v2);
				case StringType::Sz8:
					return compare_param(PTR_VAR(sl_char8 const* const, v1._value), v2);
				case StringType::Sz16:
					return compare_param(PTR_VAR(sl_char16 const* const, v1._value), v2);
				default:
					break;
			}
		}
		return 0;
	}
	
	sl_bool StringParam::equals(const StringParam& v2) const noexcept
	{
		const StringParam& v1 = *this;
		StringType type = v1._type;
		if (type == v2._type) {
			switch (type) {
				case StringType::Null:
					return sl_true;
				case StringType::Sz8:
					return Base::compareString(REF_VAR(sl_char8 const* const, v1._value), REF_VAR(sl_char8 const* const, v2._value)) == 0;
				case StringType::Sz16:
					return Base::compareString2(REF_VAR(sl_char16 const* const, v1._value), REF_VAR(sl_char16 const* const, v2._value)) == 0;
				case StringType::String8:
					return REF_VAR(String const, v1._value) == REF_VAR(String const, v2._value);
				case StringType::String16:
					return REF_VAR(String16 const, v1._value) == REF_VAR(String16 const, v2._value);
				default:
					return v1._value == v2._value;
			}
		} else {
			switch (type) {
				case StringType::String8:
					return equals_param(PTR_VAR(String const, v1._value), v2);
				case StringType::String16:
					return equals_param(PTR_VAR(String16 const, v1._value), v2);
				case StringType::Sz8:
					return equals_param(PTR_VAR(sl_char8 const* const, v1._value), v2);
				case StringType::Sz16:
					return equals_param(PTR_VAR(sl_char16 const* const, v1._value), v2);
				default:
					break;
			}
		}
		return sl_false;
	}
	
	sl_size StringParam::getHashCode() const noexcept
	{
		StringType type = _type;
		switch (type) {
			case StringType::Null:
				return 0;
			case StringType::Sz8:
				return getString().getHashCode();
			case StringType::String8:
				return REF_VAR(String const, _value).getHashCode();
			case StringType::Sz16:
				return getString16().getHashCode();
			case StringType::String16:
				return REF_VAR(String16 const, _value).getHashCode();
			default:
				return Rehash((sl_size)_value);
		}
		return 0;
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


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringParamData)

	StringParamData::StringParamData(const StringParam& param) noexcept
	{
		switch (param._type) {
			case StringType::String8:
				data = REF_VAR(String const, param._value).getData();
				break;
			case StringType::Sz8:
				data = REF_VAR(sl_char8* const, param._value);
				break;
			case StringType::String16:
				string = REF_VAR(String16 const, param._value);
				data = string.getData();
				break;
			case StringType::Sz16:
				string = REF_VAR(sl_char16* const, param._value);
				data = string.getData();
				break;
			default:
				data = sl_null;
				break;
		}
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringParamData16)

	StringParamData16::StringParamData16(const StringParam& param) noexcept
	{
		switch (param._type) {
			case StringType::String8:
				string = REF_VAR(String const, param._value);
				data = string.getData();
				break;
			case StringType::Sz8:
				string = REF_VAR(sl_char8* const, param._value);
				data = string.getData();
				break;
			case StringType::String16:
				data = REF_VAR(String16 const, param._value).getData();
				break;
			case StringType::Sz16:
				data = REF_VAR(sl_char16* const, param._value);
				break;
			default:
				data = sl_null;
				break;
		}
	}

}
