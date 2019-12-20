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

			SLIB_INLINE static void copy_param(StringParam& dst, const StringParam& src) noexcept
			{
				switch (src._type) {
					case StringType::String8:
						dst._type = StringType::StringRef8;
						REF_VAR(String*, dst._value) = PTR_VAR(String, src._value);
						break;
					case StringType::String16:
						dst._type = StringType::StringRef16;
						REF_VAR(String16*, dst._value) = PTR_VAR(String16, src._value);
						break;
					default:
						dst._type = src._type;
						dst._value = src._value;
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
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, const sl_char8* v2) noexcept
			{
				return *v1 == v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v2, const String* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String* v1, const sl_char16* v2) noexcept
			{
				return *v1 == v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v2, const String* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, const String16* v2) noexcept
			{
				return *v1 == *v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, const sl_char8* v2) noexcept
			{
				return *v1 == v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v2, const String16* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const String16* v1, const sl_char16* v2) noexcept
			{
				return *v1 == v2;
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v2, const String16* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v1, const sl_char8* v2) noexcept
			{
				return Base::compareString(v1, v2) == 0;
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char8* v1, const sl_char16* v2) noexcept
			{
				return v1 == String(v2);
			}
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v2, const sl_char8* v1) noexcept
			{
				return equals_element(v1, v2);
			}
			
			
			SLIB_INLINE static sl_bool equals_element(const sl_char16* v1, const sl_char16* v2) noexcept
			{
				return Base::compareString2(v1, v2) == 0;
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
						return equals_element(v1, REF_VAR(sl_char8 const*, v2._value));
					case StringType::Sz16:
						return equals_element(v1, REF_VAR(sl_char16 const*, v2._value));
					case StringType::StringRef8:
						return equals_element(v1, REF_VAR(String const*, v2._value));
					case StringType::StringRef16:
						return equals_element(v1, REF_VAR(String16 const*, v2._value));
					case StringType::Std8:
						return equals_element(v1, (sl_char8*)(REF_VAR(std::string const*, v2._value)->c_str()));
					case StringType::Std16:
						return equals_element(v1, (sl_char16*)(REF_VAR(std::u16string const*, v2._value)->c_str()));
					case StringType::Null:
						return sl_false;
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
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, const sl_char8* v2) noexcept
			{
				return v1->compare(v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char8* v2, const String* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String* v1, const sl_char16* v2) noexcept
			{
				return v1->compare(v2);
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
				return v1->compare(v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const sl_char8* v2, const String16* v1) noexcept
			{
				return -compare_element(v1, v2);
			}
			
			SLIB_INLINE static sl_compare_result compare_element(const String16* v1, const sl_char16* v2) noexcept
			{
				return v1->compare(v2);
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
				return String(v1).compare(v2);
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
				StringType type = v2._type;
				switch (type) {
					case StringType::String8:
						return compare_element(v1, PTR_VAR(String const, v2._value));
					case StringType::String16:
						return compare_element(v1, PTR_VAR(String16 const, v2._value));
					case StringType::Sz8:
						return compare_element(v1, REF_VAR(sl_char8 const*, v2._value));
					case StringType::Sz16:
						return compare_element(v1, REF_VAR(sl_char16 const*, v2._value));
					case StringType::StringRef8:
						return compare_element(v1, REF_VAR(String const*, v2._value));
					case StringType::StringRef16:
						return compare_element(v1, REF_VAR(String16 const*, v2._value));
					case StringType::Std8:
						return compare_element(v1, (sl_char8*)(REF_VAR(std::string const*, v2._value)->c_str()));
					case StringType::Std16:
						return compare_element(v1, (sl_char16*)(REF_VAR(std::u16string const*, v2._value)->c_str()));
					case StringType::Null:
						return 1;
				}
				return 1;
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
		copy_param(*this, other);
	}
	
	StringParam::~StringParam() noexcept
	{
		free_param(_type, _value);
	}

	StringParam::StringParam(const String& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::StringRef8;
			REF_VAR(String const*, _value) = &value;
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(String&& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::StringRef16;
			REF_VAR(String16 const*, _value) = &value;
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(String16&& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(Move(value));
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
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(AtomicString&& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(Move(value));
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
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			_type = StringType::Null;
			_value = (value_t)1;
		}
	}

	StringParam::StringParam(AtomicString16&& value) noexcept
	{
		if (value.isNotNull()) {
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(Move(value));
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
		_type = StringType::Std8;
		REF_VAR(std::string const*, _value) = &value;
	}
	
	StringParam::StringParam(const std::u16string& value) noexcept
	{
		_type = StringType::Std16;
		REF_VAR(std::u16string const*, _value) = &value;
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
			free_param(_type, _value);
			_type = StringType::StringRef8;
			REF_VAR(String const*, _value) = &value;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(String&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const String16& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::StringRef16;
			REF_VAR(String16 const*, _value) = &value;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(String16&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::String16;
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
			free_param(_type, _value);
			_type = StringType::String8;
			new PTR_VAR(String, _value) String(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(AtomicString&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::String8;
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
			free_param(_type, _value);
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(AtomicString16&& value) noexcept
	{
		if (value.isNotNull()) {
			free_param(_type, _value);
			_type = StringType::String16;
			new PTR_VAR(String16, _value) String16(Move(value));
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const std::string& value) noexcept
	{
		free_param(_type, _value);
		_type = StringType::Std8;
		REF_VAR(std::string const*, _value) = &value;
		return *this;
	}
	
	StringParam& StringParam::operator=(const std::u16string& value) noexcept
	{
		free_param(_type, _value);
		_type = StringType::Std16;
		REF_VAR(std::u16string const*, _value) = &value;
		return *this;
	}
	
	StringParam& StringParam::operator=(const sl_char8* value) noexcept
	{
		if (value) {
			free_param(_type, _value);
			_type = StringType::Sz8;
			REF_VAR(const sl_char8*, _value) = value;
		} else {
			setNull();
		}
		return *this;
	}
	
	StringParam& StringParam::operator=(const sl_char16* value) noexcept
	{
		if (value) {
			free_param(_type, _value);
			_type = StringType::Sz16;
			REF_VAR(const sl_char16*, _value) = value;
		} else {
			setNull();
		}
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

	sl_bool StringParam::isEmpty() const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String, _value).isEmpty();
			case StringType::String16:
				return REF_VAR(String16, _value).isEmpty();
			case StringType::Sz8:
				return (REF_VAR(sl_char8 const*, _value))[0] == 0;
			case StringType::Sz16:
				return (REF_VAR(sl_char16 const*, _value))[0] == 0;
			case StringType::StringRef8:
				return REF_VAR(String const*, _value)->isEmpty();
			case StringType::StringRef16:
				return REF_VAR(String16 const*, _value)->isEmpty();
			case StringType::Std8:
				return REF_VAR(std::string const*, _value)->empty();
			case StringType::Std16:
				return REF_VAR(std::u16string const*, _value)->empty();
			case StringType::Null:
				return sl_true;
		}
		return sl_true;
	}
	
	sl_bool StringParam::isNotEmpty() const noexcept
	{
		return !(isEmpty());
	}
	
	sl_bool StringParam::isString8() const noexcept
	{
		return _type == StringType::String8 || _type == StringType::StringRef8;
	}

	sl_bool StringParam::isString16() const noexcept
	{
		return _type == StringType::String16 || _type == StringType::StringRef16;
	}

	sl_bool StringParam::isSz8() const noexcept
	{
		return _type == StringType::Sz8;
	}

	sl_bool StringParam::isSz16() const noexcept
	{
		return _type == StringType::Sz16;
	}

	sl_bool StringParam::isStdString() const noexcept
	{
		return _type == StringType::Std8;
	}

	sl_bool StringParam::isStdString16() const noexcept
	{
		return _type == StringType::Std16;
	}

	String StringParam::getString(const String& def) const noexcept
	{
		switch (_type) {
			case StringType::String8:
				return REF_VAR(String, _value);
			case StringType::String16:
				return REF_VAR(String16, _value);
			case StringType::Sz8:
				return REF_VAR(sl_char8 const*, _value);
			case StringType::Sz16:
				return REF_VAR(sl_char16 const*, _value);
			case StringType::StringRef8:
				return *REF_VAR(String const*, _value);
			case StringType::StringRef16:
				return *REF_VAR(String16 const*, _value);
			case StringType::Std8:
				return *REF_VAR(std::string const*, _value);
			case StringType::Std16:
				return REF_VAR(std::u16string const*, _value)->c_str();
			case StringType::Null:
				return def;
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
				return REF_VAR(String, _value);
			case StringType::String16:
				return REF_VAR(String16, _value);
			case StringType::Sz8:
				return REF_VAR(sl_char8 const*, _value);
			case StringType::Sz16:
				return REF_VAR(sl_char16 const*, _value);
			case StringType::StringRef8:
				return *REF_VAR(String const*, _value);
			case StringType::StringRef16:
				return *REF_VAR(String16 const*, _value);
			case StringType::Std8:
				return REF_VAR(std::string const*, _value)->c_str();
			case StringType::Std16:
				return *REF_VAR(std::u16string const*, _value);
			case StringType::Null:
				return def;
		}
		return def;
	}

	String16 StringParam::getString16() const noexcept
	{
		return getString16(String16::null());
	}

	sl_compare_result StringParam::compare(const StringParam& v2) const noexcept
	{
		const StringParam& v1 = *this;
		StringType type = v1._type;
		if (type == v2._type) {
			switch (type) {
				case StringType::Null:
					return 0;
				case StringType::String8:
					return REF_VAR(String, v1._value).compare(REF_VAR(String, v2._value));
				case StringType::String16:
					return REF_VAR(String16, v1._value).compare(REF_VAR(String16, v2._value));
				case StringType::Sz8:
					return Base::compareString(REF_VAR(sl_char8 const*, v1._value), REF_VAR(sl_char8 const*, v2._value));
				case StringType::Sz16:
					return Base::compareString2(REF_VAR(sl_char16 const*, v1._value), REF_VAR(sl_char16 const*, v2._value));
				case StringType::StringRef8:
					return REF_VAR(String const*, v1._value)->compare(*REF_VAR(String const*, v2._value));
				case StringType::StringRef16:
					return REF_VAR(String16 const*, v1._value)->compare(*REF_VAR(String16 const*, v2._value));
				case StringType::Std8:
					return (sl_compare_result)(REF_VAR(std::string const*, v1._value)->compare(*REF_VAR(std::string const*, v2._value)));
				case StringType::Std16:
					return (sl_compare_result)(REF_VAR(std::u16string const*, v1._value)->compare(*REF_VAR(std::u16string const*, v2._value)));
			}
		} else {
			switch (type) {
				case StringType::Null:
					return -1;
				case StringType::String8:
					return compare_param(PTR_VAR(String, v1._value), v2);
				case StringType::String16:
					return compare_param(PTR_VAR(String16, v1._value), v2);
				case StringType::Sz8:
					return compare_param(REF_VAR(sl_char8 const*, v1._value), v2);
				case StringType::Sz16:
					return compare_param(REF_VAR(sl_char16 const*, v1._value), v2);
				case StringType::StringRef8:
					return compare_param(REF_VAR(String const*, v1._value), v2);
				case StringType::StringRef16:
					return compare_param(REF_VAR(String16 const*, v1._value), v2);
				case StringType::Std8:
					return compare_param(REF_VAR(std::string const*, v1._value)->c_str(), v2);
				case StringType::Std16:
					return compare_param((sl_char16*)(REF_VAR(std::u16string const*, v1._value)->c_str()), v2);
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
				case StringType::String8:
					return REF_VAR(String, v1._value) == REF_VAR(String, v2._value);
				case StringType::String16:
					return REF_VAR(String16, v1._value) == REF_VAR(String16, v2._value);
				case StringType::Sz8:
					return Base::compareString(REF_VAR(sl_char8 const*, v1._value), REF_VAR(sl_char8 const*, v2._value)) == 0;
				case StringType::Sz16:
					return Base::compareString2(REF_VAR(sl_char16 const*, v1._value), REF_VAR(sl_char16 const*, v2._value)) == 0;
				case StringType::StringRef8:
					return REF_VAR(String const*, v1._value)->equals(*REF_VAR(String const*, v2._value));
				case StringType::StringRef16:
					return REF_VAR(String16 const*, v1._value)->equals(*REF_VAR(String16 const*, v2._value));
				case StringType::Std8:
					return *REF_VAR(std::string const*, v1._value) == *REF_VAR(std::string const*, v2._value);
				case StringType::Std16:
					return *REF_VAR(std::u16string const*, v1._value) == *REF_VAR(std::u16string const*, v2._value);
			}
		} else {
			switch (type) {
				case StringType::Null:
					return sl_false;
				case StringType::String8:
					return equals_param(PTR_VAR(String, v1._value), v2);
				case StringType::String16:
					return equals_param(PTR_VAR(String16, v1._value), v2);
				case StringType::Sz8:
					return equals_param(REF_VAR(sl_char8 const*, v1._value), v2);
				case StringType::Sz16:
					return equals_param(REF_VAR(sl_char16 const*, v1._value), v2);
				case StringType::StringRef8:
					return equals_param(REF_VAR(String const*, v1._value), v2);
				case StringType::StringRef16:
					return equals_param(REF_VAR(String16 const*, v1._value), v2);
				case StringType::Std8:
					return equals_param(REF_VAR(std::string const*, v1._value)->c_str(), v2);
				case StringType::Std16:
					return equals_param((sl_char16*)(REF_VAR(std::u16string const*, v1._value)->c_str()), v2);
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
			case StringType::String8:
				return REF_VAR(String, _value).getHashCode();
			case StringType::String16:
				return REF_VAR(String16, _value).getHashCode();
			case StringType::Sz8:
			case StringType::Std8:
				return getString().getHashCode();
			case StringType::Sz16:
			case StringType::Std16:
				return getString16().getHashCode();
			case StringType::StringRef8:
				return REF_VAR(String const*, _value)->getHashCode();
			case StringType::StringRef16:
				return REF_VAR(String16 const*, _value)->getHashCode();
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
			case StringType::Null:
				data = sl_null;
				_length = 0;
				break;
			case StringType::String8:
				data = REF_VAR(String, param._value).getData(_length);
				break;
			case StringType::Sz8:
				data = REF_VAR(sl_char8* const, param._value);
				_length = SLIB_SIZE_MAX;
				break;
			case StringType::StringRef8:
				data = REF_VAR(String const*, param._value)->getData(_length);
				break;
			case StringType::Std8:
				{
					const std::string& s = *REF_VAR(std::string const*, param._value);
					data = (sl_char8*)(s.c_str());
					_length = (sl_size)(s.length());
					break;
				}
			case StringType::String16:
			case StringType::Sz16:
			case StringType::StringRef16:
			case StringType::Std16:
				string = param.getString();
				data = string.getData(_length);
				break;
		}
	}

	sl_size StringParamData::getLength() const noexcept
	{
		if (_length & SLIB_SIZE_TEST_SIGN_BIT) {
			return Base::getStringLength(data);
		} else {
			return _length;
		}
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringParamData16)

	StringParamData16::StringParamData16(const StringParam& param) noexcept
	{
		switch (param._type) {
			case StringType::Null:
				data = sl_null;
				_length = 0;
				break;
			case StringType::String16:
				data = REF_VAR(String16, param._value).getData(_length);
				break;
			case StringType::Sz16:
				data = REF_VAR(sl_char16* const, param._value);
				_length = SLIB_SIZE_MAX;
				break;
			case StringType::StringRef16:
				data = REF_VAR(String16 const*, param._value)->getData(_length);
				break;
			case StringType::Std16:
				{
					const std::u16string& s = *REF_VAR(std::u16string const*, param._value);
					data = (sl_char16*)(s.c_str());
					_length = (sl_size)(s.length());
					break;
				}
			case StringType::String8:
			case StringType::Sz8:
			case StringType::StringRef8:
			case StringType::Std8:
				string = param.getString16();
				data = string.getData(_length);
				break;
		}
	}

	sl_size StringParamData16::getLength() const noexcept
	{
		if (_length & SLIB_SIZE_TEST_SIGN_BIT) {
			return Base::getStringLength2(data);
		} else {
			return _length;
		}
	}

}
