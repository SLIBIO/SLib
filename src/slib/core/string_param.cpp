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
#define STRING_TYPE_SZ8 ((sl_reg)(StringType::Sz8))
#define STRING_TYPE_SZ16 ((sl_reg)(StringType::Sz16))

#ifdef SLIB_ARCH_IS_64BIT
#define IS_STR8(length) (((length) >> 62) == 0)
#define IS_STR16(length) (((length) >> 62) == 1)
#define STRING_TYPE_STR16_PREFIX SLIB_UINT64(0x4000000000000000)
#define LENGTH_MASK SLIB_UINT64(0x3FFFFFFFFFFFFFFF)
#else
#define IS_STR8(length) (((length) >> 30) == 0)
#define IS_STR16(length) (((length) >> 30) == 1)
#define STRING_TYPE_STR16_PREFIX 0x40000000
#define LENGTH_MASK 0x3FFFFFFF
#endif

#define GET_LENGTH(length) ((length) & LENGTH_MASK)

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

	StringParam::StringParam(const StringView& str) noexcept: StringParam(str.getUnsafeData(), str.getUnsafeLength())
	{
	}

	StringParam::StringParam(const StringView16& str) noexcept: StringParam(str.getUnsafeData(), str.getUnsafeLength())
	{
	}

	StringParam::StringParam(const char* sz) noexcept
	{
		if (sz) {
			_value = (void*)sz;
			_length = STRING_TYPE_SZ8;
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
				_length = STRING_TYPE_SZ16;
			} else {
				_length = STRING_TYPE_STRING8_REF;
				new STRING_PTR(*this) String(String::create(sz));
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
			_length = STRING_TYPE_SZ16;
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char32_t* sz) noexcept: StringParam(String::create(sz))
	{
	}

	StringParam::StringParam(const char* str, sl_reg length) noexcept
	{
		if (str) {
			_value = (void*)str;
			if (length < 0) {
				_length = STRING_TYPE_SZ8;
			} else {
				_length = GET_LENGTH(length);
			}
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const wchar_t* str, sl_reg length) noexcept
	{
		if (str) {
			if (sizeof(wchar_t) == 2) {
				_value = (void*)str;
				if (length < 0) {
					_length = STRING_TYPE_SZ16;
				} else {
					_length = STRING_TYPE_STR16_PREFIX | GET_LENGTH(length);
				}
			} else {
				_length = STRING_TYPE_STRING8_REF;
				new STRING_PTR(*this) String(String::create(str, length));
			}
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char16_t* str, sl_reg length) noexcept
	{
		if (str) {
			_value = (void*)str;
			if (length < 0) {
				_length = STRING_TYPE_SZ16;
			} else {
				_length = STRING_TYPE_STR16_PREFIX | GET_LENGTH(length);
			}
		} else {
			_value = sl_null;
			_length = 1;
		}
	}

	StringParam::StringParam(const char32_t* str, sl_reg length) noexcept: StringParam(String::create(str, length))
	{
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
	
	StringParam& StringParam::operator=(const StringView& str) noexcept
	{
		_assign(str.getUnsafeData(), str.getUnsafeLength());
		return *this;
	}

	StringParam& StringParam::operator=(const StringView16& str) noexcept
	{
		_assign(str.getUnsafeData(), str.getUnsafeLength());
		return *this;
	}

	StringParam& StringParam::operator=(const char* value) noexcept
	{
		if (value) {
			_free();
			_value = (void*)value;
			_length = STRING_TYPE_SZ8;
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
			return *this = String::create(value);
		}
	}

	StringParam& StringParam::operator=(const char16_t* value) noexcept
	{
		if (value) {
			_free();
			_value = (void*)value;
			_length = STRING_TYPE_SZ16;
		} else {
			setNull();
		}
		return *this;
	}

	StringParam& StringParam::operator=(const char32_t* value) noexcept
	{
		return *this = String::create(value);
	}

	void StringParam::_assign(const sl_char8* str, sl_reg length) noexcept
	{
		_free();
		_value = (void*)str;
		if (length < 0) {
			_length = STRING_TYPE_SZ8;
		} else {
			_length = GET_LENGTH(length);
		}
	}

	void StringParam::_assign(const sl_char16* str, sl_reg length) noexcept
	{
		_free();
		_value = (void*)str;
		if (length < 0) {
			_length = STRING_TYPE_SZ16;
		} else {
			_length = STRING_TYPE_STR16_PREFIX | GET_LENGTH(length);
		}
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
			return *this = String::create(str);
		}
	}

	StringParam& StringParam::operator=(const std::u16string& str) noexcept
	{
		_assign((sl_char16*)(str.c_str()), str.length());
		return *this;
	}

	StringParam& StringParam::operator=(const std::u32string& str) noexcept
	{
		return *this = String::create(str);
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
			case 0:
			case STRING_TYPE_STR16_PREFIX:
				return sl_true;
			case STRING_TYPE_STRING8_REF:
			case STRING_TYPE_STRING8_NOREF:
				return STRING_REF(*this).isEmpty();
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this).isEmpty();
			case STRING_TYPE_SZ8:
				return !(*((sl_char8*)_value));
			case STRING_TYPE_SZ16:
				return !(*((sl_char16*)_value));
			default:
				return sl_false;
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
		return _value && (_length == STRING_TYPE_SZ8 || IS_STR8(_length));
	}

	sl_bool StringParam::isSz16() const noexcept
	{
		return _value && (_length == STRING_TYPE_SZ16 || IS_STR16(_length));
	}

	sl_bool StringParam::is8() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING8_REF || _length == STRING_TYPE_STRING8_NOREF || _length == STRING_TYPE_SZ8 || IS_STR8(_length));
	}

	sl_bool StringParam::is16() const noexcept
	{
		return _value && (_length == STRING_TYPE_STRING16_REF || _length == STRING_TYPE_STRING16_NOREF || _length == STRING_TYPE_SZ16 || IS_STR16(_length));
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
				return String::create(STRING16_REF(*this));
			case STRING_TYPE_SZ8:
				return String::create((sl_char8*)_value);
			case STRING_TYPE_SZ16:
				return String::create((sl_char16*)_value);
			default:
				if (IS_STR16(_length)) {
					return String::create((sl_char16*)_value, GET_LENGTH(_length));
				} else {
					return String::create((sl_char8*)_value, _length);
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
				return String16::create(STRING_REF(*this));
			case STRING_TYPE_STRING16_REF:
			case STRING_TYPE_STRING16_NOREF:
				return STRING16_REF(*this);
			case STRING_TYPE_SZ8:
				return String16::create((sl_char8*)_value);
			case STRING_TYPE_SZ16:
				return String16::create((sl_char16*)_value);
			default:
				if (IS_STR16(_length)) {
					return String16::create((sl_char16*)_value, GET_LENGTH(_length));
				} else {
					return String16::create((sl_char8*)_value, _length);
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
			case STRING_TYPE_SZ8:
				return (sl_char8*)_value;
			case STRING_TYPE_SZ16:
				return (sl_char16*)_value;
			default:
				if (IS_STR16(_length)) {
					sl_char16* s = (sl_char16*)_value;
					sl_size l = GET_LENGTH(_length);
					if (s[l]) {
						return String::create(s, l);
					} else {
						return s;
					}
				} else {
					sl_char8* s = (sl_char8*)_value;
					sl_size l = _length;
					if (s[l]) {
						return String::create(s, l);
					} else {
						return s;
					}
				}
		}
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
			case STRING_TYPE_SZ8:
				return String::getHashCode((sl_char8*)_value);
			case STRING_TYPE_SZ16:
				return String16::getHashCode((sl_char16*)_value);
			default:
				if (IS_STR16(_length)) {
					return String16::getHashCode((sl_char16*)_value, GET_LENGTH(_length));
				} else {
					return String::getHashCode((sl_char8*)_value, _length);
				}
		}
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
					data = STRING_REF(param).getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					string = String::create(STRING16_REF(param));
					data = string.getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_SZ8:
					data = (sl_char8*)(param._value);
					length = -1;
					break;
				case STRING_TYPE_SZ16:
					string = String::create((sl_char16*)(param._value));
					data = string.getData(*((sl_size*)&length));
					break;
				default:
					if (IS_STR16(param._length)) {
						string = String::create((sl_char16*)(param._value), GET_LENGTH(param._length));
						data = string.getData(*((sl_size*)&length));
					} else {
						data = (sl_char8*)(param._value);
						length = param._length;
					}
					break;
			}
		}
	}

	StringData::StringData(const sl_char8* str) noexcept: StringView(str)
	{
	}

	StringData::StringData(const sl_char8* str, sl_size length) noexcept: StringView(str, length)
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringData16)

	StringData16::StringData16(const StringParam& param) noexcept
	{
		if (param._value) {
			switch (param._length) {
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					data = STRING16_REF(param).getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					string = String16::create(STRING_REF(param));
					data = string.getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_SZ8:
					string = String16::create((sl_char8*)(param._value));
					data = string.getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_SZ16:
					data = (sl_char16*)(param._value);
					length = -1;
					break;
				default:
					if (IS_STR16(param._length)) {
						data = (sl_char16*)(param._value);
						length = GET_LENGTH(param._length);
					} else {
						string = String16::create((sl_char8*)(param._value), param._length);
						data = string.getData(*((sl_size*)&length));
					}
					break;
			}
		}
	}

	StringData16::StringData16(const sl_char16* str) noexcept: StringView16(str)
	{
	}

	StringData16::StringData16(const sl_char16* str, sl_size length) noexcept: StringView16(str, length)
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringCstr)

	StringCstr::StringCstr(const StringParam& param) noexcept
	{
		if (param._value) {
			switch (param._length) {
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					data = STRING_REF(param).getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					string = String::create(STRING16_REF(param));
					data = string.getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_SZ8:
					data = (sl_char8*)(param._value);
					length = -1;
					break;
				case STRING_TYPE_SZ16:
					string = String::create((sl_char16*)(param._value));
					data = string.getData(*((sl_size*)&length));
					break;
				default:
					if (IS_STR16(param._length)) {
						string = String::create((sl_char16*)(param._value), GET_LENGTH(param._length));
						data = string.getData(*((sl_size*)&length));
					} else {
						data = (sl_char8*)(param._value);
						length = param._length;
						if (data[length]) {
							string = String::create(data, length);
							data = string.getData(*((sl_size*)&length));
						}
					}
					break;
			}
		}
	}

	StringCstr::StringCstr(const sl_char8* str) noexcept: StringView(str)
	{
	}

	StringCstr::StringCstr(const sl_char8* str, sl_size length) noexcept: StringView(str, length)
	{
	}

	StringCstr::StringCstr(const String& str) noexcept
	{
		if (str.isNotNull()) {
			data = str.getData(*((sl_size*)&length));
		}
	}

	StringCstr::StringCstr(const AtomicString& str) noexcept: StringCstr(String(str))
	{
	}

	StringCstr::StringCstr(const StringView& str) noexcept: StringView(str.getUnsafeData(), str.getUnsafeLength())
	{
		if (data) {
			if (length > 0) {
				if (data[length]) {
					string = String::create(data, length);
					data = string.getData(*((sl_size*)&length));
				}
			}
		}
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StringCstr16)

	StringCstr16::StringCstr16(const StringParam& param) noexcept
	{
		if (param._value) {
			switch (param._length) {
				case STRING_TYPE_STRING16_REF:
				case STRING_TYPE_STRING16_NOREF:
					data = STRING16_REF(param).getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_STRING8_REF:
				case STRING_TYPE_STRING8_NOREF:
					string = String16::create(STRING_REF(param));
					data = string.getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_SZ8:
					string = String16::create((sl_char8*)(param._value));
					data = string.getData(*((sl_size*)&length));
					break;
				case STRING_TYPE_SZ16:
					data = (sl_char16*)(param._value);
					length = -1;
					break;
				default:
					if (IS_STR16(param._length)) {
						data = (sl_char16*)(param._value);
						length = GET_LENGTH(param._length);
						if (data[length]) {
							string = String16::create(data, length);
							data = string.getData(*((sl_size*)&length));
						}
					} else {
						string = String16::create((sl_char8*)(param._value), param._length);
						data = string.getData(*((sl_size*)&length));
					}
					break;
			}
		}
	}

	StringCstr16::StringCstr16(const sl_char16* str) noexcept: StringView16(str)
	{
	}

	StringCstr16::StringCstr16(const sl_char16* str, sl_size length) noexcept: StringView16(str, length)
	{
	}

	StringCstr16::StringCstr16(const String16& str) noexcept
	{
		if (str.isNotNull()) {
			data = str.getData(*((sl_size*)&length));
		}
	}

	StringCstr16::StringCstr16(const AtomicString16& str) noexcept: StringCstr16(String16(str))
	{
	}

	StringCstr16::StringCstr16(const StringView16& str) noexcept: StringView16(str.getUnsafeData(), str.getUnsafeLength())
	{
		if (data) {
			if (length > 0) {
				if (data[length]) {
					string = String16::create(data, length);
					data = string.getData(*((sl_size*)&length));
				}
			}
		}
	}

}
