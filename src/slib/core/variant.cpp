#include "../../../inc/slib/core/variant.h"

SLIB_NAMESPACE_BEGIN

#define PTR_VAR(TYPE, x) (reinterpret_cast<TYPE*>(&x))
#define REF_VAR(TYPE, x) (*PTR_VAR(TYPE, x))

int Compare<Variant>::operator()(const Variant& a, const Variant& b) const
{
	return Compare<sl_uint64>()(a._value, b._value);
}

const char _VariantMap_ClassID[] = "VariantMap";

const char _VariantList_ClassID[] = "VariantList";

const char _VariantMapList_ClassID[] = "VariantMapList";


const _Variant_Const _Variant_Null = {0, VariantType::Null, 0};


SLIB_INLINE void _Variant_copy(VariantType src_type, sl_uint64 src_value, sl_uint64& dst_value)
{
	switch (src_type) {
		case VariantType::String8:
			new PTR_VAR(String, dst_value) String(REF_VAR(String, src_value));
			break;
		case VariantType::String16:
			new PTR_VAR(String16, dst_value) String16(REF_VAR(String16, src_value));
			break;
		case VariantType::Object:
		case VariantType::Weak:
			new PTR_VAR(Ref<Referable>, dst_value) Ref<Referable>(REF_VAR(Ref<Referable>, src_value));
			break;
		default:
			dst_value = src_value;
			break;
	}
}

SLIB_INLINE void _Variant_free(VariantType type, sl_uint64 value)
{
	switch (type)
	{
		case VariantType::String8:
			REF_VAR(String, value).String::~String();
			break;
		case VariantType::String16:
			REF_VAR(String16, value).String16::~String16();
			break;
		case VariantType::Object:
		case VariantType::Weak:
			REF_VAR(Ref<Referable>, value).Ref<Referable>::~Ref();
			break;
		default:
			break;
	}
}

void Variant::_free(VariantType type, sl_uint64 value)
{
	_Variant_free(type, value);
}

SLIB_INLINE void Atomic<Variant>::_retain(VariantType& type, sl_uint64& value) const
{
	if ((void*)(this) == (void*)(&_Variant_Null)) {
		type = VariantType::Null;
	} else {
		SpinLocker lock(&m_lock);
		type = _type;
		_Variant_copy(_type, _value, value);
	}
}

SLIB_INLINE void Atomic<Variant>::_replace(VariantType type, sl_uint64 value)
{
	VariantType typeOld;
	sl_uint64 valueOld;
	{
		SpinLocker lock(&m_lock);
		typeOld = _type;
		valueOld = _value;
		_type = type;
		_value = value;
	}
	_Variant_free(typeOld, valueOld);
}


Variant::Variant(Variant&& other)
{
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

Variant::Variant(const Variant& other)
{
	_type = other._type;
	_Variant_copy(_type, other._value, _value);
}

Variant::Variant(AtomicVariant&& _other)
{
	Variant& other = REF_VAR(Variant, _other);
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

Variant::Variant(const AtomicVariant& other)
{
	other._retain(_type, _value);
}

Variant::~Variant()
{
	_Variant_free(_type, _value);
}

Variant::Variant(sl_null_t)
: _type(VariantType::Null)
{
}

Variant::Variant(sl_int32 value)
{
	_type = VariantType::Int32;
	REF_VAR(sl_int32, _value) = value;
}

Variant::Variant(sl_uint32 value)
{
	_type = VariantType::Uint32;
	REF_VAR(sl_uint32, _value) = value;
}

Variant::Variant(sl_int64 value)
{
	_type = VariantType::Int64;
	REF_VAR(sl_int64, _value) = value;
}

Variant::Variant(sl_uint64 value)
{
	_type = VariantType::Uint64;
	REF_VAR(sl_uint64, _value) = value;
}

Variant::Variant(float value)
{
	_type = VariantType::Float;
	REF_VAR(float, _value) = value;
}

Variant::Variant(double value)
{
	_type = VariantType::Double;
	REF_VAR(double, _value) = value;
}

Variant::Variant(const sl_bool value)
{
	_type = VariantType::Boolean;
	REF_VAR(sl_bool, _value) = value;
}

Variant::Variant(const String& value)
{
	if (value.isNotNull()) {
		_type = VariantType::String8;
		new PTR_VAR(String, _value) String(value);
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const String16& value)
{
	if (value.isNotNull()) {
		_type = VariantType::String16;
		new PTR_VAR(String16, _value) String16(value);
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const AtomicString& s)
{
	String value(s);
	if (value.isNotNull()) {
		_type = VariantType::String8;
		new PTR_VAR(String, _value) String(value);
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const AtomicString16& s)
{
	String16 value(s);
	if (value.isNotNull()) {
		_type = VariantType::String16;
		new PTR_VAR(String16, _value) String16(value);
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const sl_char8* sz8)
{
	if (sz8) {
		_type = VariantType::Sz8;
		REF_VAR(const sl_char8*, _value) = sz8;
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const sl_char16* sz16)
{
	if (sz16) {
		_type = VariantType::Sz16;
		REF_VAR(const sl_char16*, _value) = sz16;
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const Time& value)
{
	_type = VariantType::Time;
	REF_VAR(Time, _value) = value;
}

Variant::Variant(const void* ptr)
{
	if (ptr) {
		_type = VariantType::Pointer;
		REF_VAR(const void*, _value) = ptr;
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const Memory& mem)
{
	if (mem.isNotNull()) {
		_type = VariantType::Object;
		new PTR_VAR(Memory, _value) Memory(mem);
	} else {
		_type = VariantType::Null;
	}
}

Variant::Variant(const AtomicMemory& _mem)
{
	Memory mem(_mem);
	if (mem.isNotNull()) {
		_type = VariantType::Object;
		new PTR_VAR(Memory, _value) Memory(mem);
	} else {
		_type = VariantType::Null;
	}
}

Variant Variant::fromInt32(sl_int32 value)
{
	return value;
}

Variant Variant::fromUint32(sl_uint32 value)
{
	return value;
}

Variant Variant::fromInt64(sl_int64 value)
{
	return value;
}

Variant Variant::fromUint64(sl_uint64 value)
{
	return value;
}

Variant Variant::fromFloat(float value)
{
	return value;
}

Variant Variant::fromDouble(double value)
{
	return value;
}

Variant Variant::fromBoolean(sl_bool value)
{
	return value;
}

Variant Variant::fromString(const String& value)
{
	return value;
}

Variant Variant::fromString16(const String16& value)
{
	return value;
}

Variant Variant::fromSz8(const sl_char8* value)
{
	return value;
}

Variant Variant::fromSz16(const sl_char16* value)
{
	return value;
}

Variant Variant::fromTime(const Time& value)
{
	return value;
}

Variant Variant::fromPointer(const void* value)
{
	return value;
}

Variant Variant::fromMemory(const Memory& mem)
{
	return mem;
}

Variant Variant::fromVariantList(const List<Variant>& value)
{
	return value;
}

Variant Variant::createVariantList()
{
	return List<Variant>::create();
}

Variant Variant::fromVariantMap(const Map<String, Variant>& value)
{
	return value;
}

Variant Variant::createVariantListMap()
{
	return Map<String, Variant>::createList();
}

Variant Variant::createVariantTreeMap()
{
	return Map<String, Variant>::createTree();
}

Variant Variant::createVariantHashMap()
{
	return Map<String, Variant>::createHash();
}


Variant& Variant::operator=(Variant&& other)
{
	if (this != &other) {
		_Variant_free(_type, _value);
		_type = other._type;
		_value = other._value;
		other._type = VariantType::Null;
	}
	return *this;
}

Variant& Variant::operator=(const Variant& other)
{
	if (this != &other) {
		_Variant_free(_type, _value);
		_type = other._type;
		_Variant_copy(_type, other._value, _value);
	}
	return *this;
}

Variant& Variant::operator=(AtomicVariant&& other)
{
	if ((void*)this != (void*)(&other)) {
		_Variant_free(_type, _value);
		_type = other._type;
		_value = other._value;
		other._type = VariantType::Null;
	}
	return *this;
}

Variant& Variant::operator=(const AtomicVariant& other)
{
	_Variant_free(_type, _value);
	other._retain(_type, _value);
	return *this;
}

Variant& Variant::operator=(sl_null_t)
{
	setNull();
	return *this;
}

Variant& Variant::operator=(sl_int32 value)
{
	setInt32(value);
	return *this;
}

Variant& Variant::operator=(sl_uint32 value)
{
	setUint32(value);
	return *this;
}

Variant& Variant::operator=(sl_int64 value)
{
	setInt64(value);
	return *this;
}

Variant& Variant::operator=(sl_uint64 value)
{
	setUint64(value);
	return *this;
}

Variant& Variant::operator=(float value)
{
	setFloat(value);
	return *this;
}

Variant& Variant::operator=(double value)
{
	setDouble(value);
	return *this;
}

Variant& Variant::operator=(const sl_bool value)
{
	setBoolean(value);
	return *this;
}

Variant& Variant::operator=(const String& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const AtomicString& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const String16& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const AtomicString16& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const sl_char8* sz8)
{
	setString(sz8);
	return *this;
}

Variant& Variant::operator=(const sl_char16* sz16)
{
	setString(sz16);
	return *this;
}

Variant& Variant::operator=(const Time& value)
{
	setTime(value);
	return *this;
}

Variant& Variant::operator=(const void* ptr)
{
	setPointer(ptr);
	return *this;
}

Variant& Variant::operator=(const Memory& mem)
{
	setMemory(mem);
	return *this;
}

Variant& Variant::operator=(const AtomicMemory& mem)
{
	setMemory(mem);
	return *this;
}

Variant Variant::operator[](sl_size indexForVariantList) const
{
	return getListElement(indexForVariantList);
}

Variant Variant::operator[](const String& keyForVariantMap) const
{
	return getField(keyForVariantMap);
}

void Variant::setNull()
{
	if (_type != VariantType::Null) {
		_Variant_free(_type, _value);
		_type = VariantType::Null;
	}
}

sl_bool Variant::isInt32() const
{
	return _type == VariantType::Int32;
}

sl_int32 Variant::getInt32(sl_int32 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_int32)(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return (sl_int32)(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return (sl_int32)(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return (sl_int32)(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return (sl_int32)(REF_VAR(float const, _value));
		case VariantType::Double:
			return (sl_int32)(REF_VAR(double const, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool const, _value)) ? 1 : 0;
		case VariantType::String8:
			return REF_VAR(String const, _value).parseInt32(10, def);
		case VariantType::String16:
			return REF_VAR(String16 const, _value).parseInt32(10, def);
		case VariantType::Pointer:
			return (sl_int32)(REF_VAR(const sl_size, _value));
		case VariantType::Sz8:
		{
			sl_int32 ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const , _value);
			sl_reg pos = String::parseInt32(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_int32 ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const , _value);
			sl_reg pos = String16::parseInt32(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setInt32(sl_int32 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Int32;
	REF_VAR(sl_int32, _value) = value;
}

sl_bool Variant::isUint32() const
{
	return _type == VariantType::Uint32;
}

sl_uint32 Variant::getUint32(sl_uint32 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_uint32)(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return (sl_uint32)(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return (sl_uint32)(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return (sl_uint32)(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return (sl_uint32)(REF_VAR(float const, _value));
		case VariantType::Double:
			return (sl_uint32)(REF_VAR(double const, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool const, _value)) ? 1 : 0;
		case VariantType::String8:
			return REF_VAR(String const, _value).parseUint32(10, def);
		case VariantType::String16:
			return REF_VAR(String16 const, _value).parseUint32(10, def);
		case VariantType::Pointer:
			return (sl_uint32)(REF_VAR(sl_size const, _value));
		case VariantType::Sz8:
		{
			sl_uint32 ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const, _value);
			sl_reg pos = String::parseUint32(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_uint32 ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const, _value);
			sl_reg pos = String16::parseUint32(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setUint32(sl_uint32 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Uint32;
	REF_VAR(sl_uint32, _value) = value;
}

sl_bool Variant::isInt64() const
{
	return _type == VariantType::Int64;
}

sl_int64 Variant::getInt64(sl_int64 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_int64)(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return (sl_int64)(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return (sl_int64)(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return (sl_int64)(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return (sl_int64)(REF_VAR(float const, _value));
		case VariantType::Double:
			return (sl_int64)(REF_VAR(double const, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool const, _value)) ? 1 : 0;
		case VariantType::String8:
			return (REF_VAR(String const, _value)).parseInt64(10, def);
		case VariantType::String16:
			return (REF_VAR(String16 const, _value)).parseInt64(10, def);
		case VariantType::Pointer:
			return (sl_int64)(REF_VAR(sl_size const, _value));
		case VariantType::Sz8:
		{
			sl_int64 ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const, _value);
			sl_reg pos = String::parseInt64(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_int64 ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const, _value);
			sl_reg pos = String16::parseInt64(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setInt64(sl_int64 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Int64;
	REF_VAR(sl_int64, _value) = value;
}

sl_bool Variant::isUint64() const
{
	return _type == VariantType::Uint64;
}

sl_uint64 Variant::getUint64(sl_uint64 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_uint64)(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return (sl_uint64)(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return (sl_uint64)(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return (sl_uint64)(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return (sl_uint64)(REF_VAR(float const, _value));
		case VariantType::Double:
			return (sl_uint64)(REF_VAR(double const, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool const, _value)) ? 1 : 0;
		case VariantType::String8:
			return (REF_VAR(String const, _value)).parseUint64(10, def);
		case VariantType::String16:
			return (REF_VAR(String16 const, _value)).parseUint64(10, def);
		case VariantType::Pointer:
			return (sl_uint64)(REF_VAR(sl_size const, _value));
		case VariantType::Sz8:
		{
			sl_uint64 ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const, _value);
			sl_reg pos = String::parseUint64(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_uint64 ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const, _value);
			sl_reg pos = String16::parseUint64(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setUint64(sl_uint64 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Uint64;
	REF_VAR(sl_uint64, _value) = value;
}

sl_bool Variant::isInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Uint32 || _type == VariantType::Int64 || _type == VariantType::Uint64;
}

sl_bool Variant::isSignedInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Int64;
}

sl_bool Variant::isUnsignedInteger() const
{
	return _type == VariantType::Uint32 || _type == VariantType::Uint64;
}

sl_bool Variant::isFloat() const
{
	return _type == VariantType::Float;
}

float Variant::getFloat(float def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (float)(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return (float)(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return (float)(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return (float)(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return (float)(REF_VAR(float const, _value));
		case VariantType::Double:
			return (float)(REF_VAR(double const, _value));
		case VariantType::String8:
			return (REF_VAR(String const, _value)).parseFloat(def);
		case VariantType::String16:
			return (REF_VAR(String16 const, _value)).parseFloat(def);
		case VariantType::Sz8:
		{
			float ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const, _value);
			sl_reg pos = String::parseFloat(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			float ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const, _value);
			sl_reg pos = String16::parseFloat(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setFloat(float value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Float;
	REF_VAR(float, _value) = value;
}

sl_bool Variant::isDouble() const
{
	return _type == VariantType::Double;
}

double Variant::getDouble(double def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (double)(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return (double)(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return (double)(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return (double)(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return (double)(REF_VAR(float const, _value));
		case VariantType::Double:
			return (double)(REF_VAR(double const, _value));
		case VariantType::String8:
			return (REF_VAR(String const, _value)).parseDouble(def);
		case VariantType::String16:
			return (REF_VAR(String16 const, _value)).parseDouble(def);
		case VariantType::Sz8:
		{
			double ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const, _value);
			sl_reg pos = String::parseDouble(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			double ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const, _value);
			sl_reg pos = String16::parseDouble(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setDouble(double value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Double;
	REF_VAR(double, _value) = value;
}

sl_bool Variant::isNumber() const
{
	return isInteger() || _type == VariantType::Float || _type == VariantType::Double;
}

sl_bool Variant::isBoolean() const
{
	return _type == VariantType::Boolean;
}

sl_bool Variant::getBoolean(sl_bool def) const
{
	switch (_type) {
		case VariantType::Int32:
		{
			sl_int32 n = REF_VAR(sl_int32 const, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Uint32:
		{
			sl_uint32 n = REF_VAR(sl_uint32 const, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Int64:
		{
			sl_int64 n = REF_VAR(sl_int64 const, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Uint64:
		{
			sl_uint64 n = REF_VAR(sl_uint64 const, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Boolean:
			return REF_VAR(sl_bool const, _value);
		case VariantType::String8:
			return (REF_VAR(String const, _value)).parseBoolean(def);
		case VariantType::String16:
			return (REF_VAR(String16 const, _value)).parseBoolean(def);
		case VariantType::Sz8:
		{
			sl_bool ret;
			const sl_char8* str = REF_VAR(sl_char8 const* const, _value);
			sl_reg pos = String::parseBoolean(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_bool ret;
			const sl_char16* str = REF_VAR(sl_char16 const* const, _value);
			sl_reg pos = String16::parseBoolean(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		default:
			break;
	}
	return def;
}

void Variant::setBoolean(sl_bool value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Boolean;
	REF_VAR(sl_bool, _value) = value;
}

sl_bool Variant::isString() const
{
	return _type == VariantType::String8 || _type == VariantType::String16 || _type == VariantType::Sz8 || _type == VariantType::Sz16;
}

sl_bool Variant::isString8() const
{
	return _type == VariantType::String8;
}

sl_bool Variant::isString16() const
{
	return _type == VariantType::String16;
}

sl_bool Variant::isSz8() const
{
	return _type == VariantType::Sz8;
}

sl_bool Variant::isSz16() const
{
	return _type == VariantType::Sz16;
}

String Variant::getString(const String& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String::fromInt32(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return String::fromUint32(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return String::fromInt64(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return String::fromUint64(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return String::fromFloat(REF_VAR(float const, _value));
		case VariantType::Double:
			return String::fromDouble(REF_VAR(double const, _value));
		case VariantType::Boolean:
			if (REF_VAR(sl_bool const, _value)) {
				SLIB_STATIC_STRING_BY_ARRAY(ret, 't', 'r', 'u', 'e');
				return ret;
			} else {
				SLIB_STATIC_STRING_BY_ARRAY(ret, 'f', 'a', 'l', 's', 'e');
				return ret;
			}
		case VariantType::Time:
			return REF_VAR(Time const, _value).toString();
		case VariantType::String8:
			return REF_VAR(String const, _value);
		case VariantType::String16:
			return REF_VAR(String16 const, _value);
		case VariantType::Sz8:
			return REF_VAR(sl_char8 const* const, _value);
		case VariantType::Sz16:
			return REF_VAR(sl_char16 const* const, _value);
		case VariantType::Pointer:
			return "#" + String::fromPointerValue(REF_VAR(void const* const, _value));
		default:
			break;
	}
	return def;
}

String Variant::getString() const
{
	return getString(String::null());
}

String16 Variant::getString16(const String16& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String16::fromInt32(REF_VAR(sl_int32 const, _value));
		case VariantType::Uint32:
			return String16::fromUint32(REF_VAR(sl_uint32 const, _value));
		case VariantType::Int64:
			return String16::fromInt64(REF_VAR(sl_int64 const, _value));
		case VariantType::Uint64:
			return String16::fromUint64(REF_VAR(sl_uint64 const, _value));
		case VariantType::Float:
			return String16::fromFloat(REF_VAR(float const, _value));
		case VariantType::Double:
			return String16::fromDouble(REF_VAR(double const, _value));
		case VariantType::Boolean:
			if (REF_VAR(sl_bool const, _value)) {
				SLIB_STATIC_STRING16_BY_ARRAY(ret, 't', 'r', 'u', 'e');
				return ret;
			} else {
				SLIB_STATIC_STRING16_BY_ARRAY(ret, 'f', 'a', 'l', 's', 'e');
				return ret;
			}
		case VariantType::Time:
			return REF_VAR(Time const, _value).toString();
		case VariantType::String8:
			return REF_VAR(String const, _value);
		case VariantType::String16:
			return REF_VAR(String16 const, _value);
		case VariantType::Sz8:
			return REF_VAR(sl_char8 const* const, _value);
		case VariantType::Sz16:
			return REF_VAR(sl_char16 const* const, _value);
		case VariantType::Pointer:
			return "#" + String16::fromPointerValue(REF_VAR(void const* const, _value));
		default:
			break;
	}
	return def;
}

String16 Variant::getString16() const
{
	return getString16(String16::null());
}

const sl_char8* Variant::getSz8(const sl_char8* def) const
{
	switch (_type) {
		case VariantType::Boolean:
			if (REF_VAR(sl_bool const, _value)) {
				return "true";
			} else {
				return "false";
			}
		case VariantType::String8:
			return REF_VAR(String const, _value).getData();
		case VariantType::Sz8:
			return REF_VAR(sl_char8 const* const, _value);
		default:
			break;
	}
	return def;
}

const sl_char16* Variant::getSz16(const sl_char16* def) const
{
	switch (_type) {
		case VariantType::Boolean:
			if (REF_VAR(sl_bool const, _value)) {
				static const sl_char16 _s[] = {'t', 'r', 'u', 'e', 0};
				return _s;
			} else {
				static const sl_char16 _s[] = {'f', 'a', 'l', 's', 'e', 0};
				return _s;
			}
		case VariantType::String16:
			return REF_VAR(String16 const, _value).getData();
		case VariantType::Sz16:
			return REF_VAR(sl_char16 const* const, _value);
		default:
			break;
	}
	return def;
}

void Variant::setString(const String& value)
{
	_Variant_free(_type, _value);
	if (value.isNotNull()) {
		_type = VariantType::String8;
		new PTR_VAR(String, _value) String(value);
	} else {
		_type = VariantType::Null;
	}
}

void Variant::setString(const String16& value)
{
	_Variant_free(_type, _value);
	if (value.isNotNull()) {
		_type = VariantType::String16;
		new PTR_VAR(String16, _value) String16(value);
	} else {
		_type = VariantType::Null;
	}
}

void Variant::setString(const AtomicString& s)
{
	_Variant_free(_type, _value);
	String value(s);
	if (value.isNotNull()) {
		_type = VariantType::String8;
		new PTR_VAR(String, _value) String(value);
	} else {
		_type = VariantType::Null;
	}
}

void Variant::setString(const AtomicString16& s)
{
	_Variant_free(_type, _value);
	String value(s);
	if (value.isNotNull()) {
		_type = VariantType::String16;
		new PTR_VAR(String16, _value) String16(value);
	} else {
		_type = VariantType::Null;
	}
}

void Variant::setString(const sl_char8* value)
{
	_Variant_free(_type, _value);
	if (value) {
		_type = VariantType::Sz8;
		REF_VAR(const sl_char8*, _value) = value;
	} else {
		_type = VariantType::Null;
	}
}

void Variant::setString(const sl_char16* value)
{
	_Variant_free(_type, _value);
	if (value) {
		_type = VariantType::Sz16;
		REF_VAR(const sl_char16*, _value) = value;
	} else {
		_type = VariantType::Null;
	}
}

sl_bool Variant::isTime() const
{
	return _type == VariantType::Time;
}

Time Variant::getTime(const Time& def) const
{
	switch (_type) {
		case VariantType::Time:
			return REF_VAR(Time const, _value);
		case VariantType::String8:
			return Time(REF_VAR(String const, _value));
		case VariantType::String16:
			return Time(REF_VAR(String16 const, _value));
		case VariantType::Sz8:
			return Time(REF_VAR(sl_char8 const* const, _value));
		case VariantType::Sz16:
			return Time(REF_VAR(sl_char16 const* const, _value));
		default:
			break;
	}
	return def;
}

Time Variant::getTime() const
{
	return getTime(Time::zero());
}

void Variant::setTime(const Time& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Time;
	REF_VAR(Time, _value) = value;
}

sl_bool Variant::isPointer() const
{
	return _type == VariantType::Pointer || _type == VariantType::Sz8 || _type == VariantType::Sz16;
}

void* Variant::getPointer(const void* def) const
{
	if (_type == VariantType::Pointer || _type == VariantType::Sz8 || _type == VariantType::Sz16) {
		return REF_VAR(void* const, _value);
	}
	return (void*)def;
}

void Variant::setPointer(const void *ptr)
{
	_Variant_free(_type, _value);
	if (ptr) {
		_type = VariantType::Pointer;
		REF_VAR(const void*, _value) = ptr;
	} else {
		_type = VariantType::Null;
	}
}

sl_bool Variant::isObject() const
{
	return _type == VariantType::Object || _type == VariantType::Weak;
}

sl_bool Variant::isWeak() const
{
	return _type == VariantType::Weak;
}

Ref<Referable> Variant::getObject() const
{
	if (_type == VariantType::Object) {
		return REF_VAR(Ref<Referable> const, _value);
	} else if (_type == VariantType::Weak) {
		return REF_VAR(WeakRef<Referable> const, _value);
	}
	return sl_null;
}

sl_object_type Variant::getObjectType() const
{
	Ref<Referable> obj(getObject());
	if (obj.isNotNull()) {
		return obj->getObjectType();
	}
	return 0;
}

sl_bool Variant::isObjectNotNull() const
{
	if (_type == VariantType::Object || _type == VariantType::Weak) {
		return REF_VAR(Ref<Referable> const, _value).isNotNull();
	}
	return sl_false;
}

sl_bool Variant::isObjectNull() const
{
	if (_type == VariantType::Object || _type == VariantType::Weak) {
		return REF_VAR(Ref<Referable> const, _value).isNull();
	}
	return sl_true;
}

sl_bool Variant::isMemory() const
{
	return IsInstanceOf<CMemory>(getObject());
}

Memory Variant::getMemory() const
{
	Ref<Referable> obj(getObject());
	if (CMemory* p = CastInstance<CMemory>(obj._ptr)) {
		return p;
	}
	return sl_null;
}

void Variant::setMemory(const Memory& mem)
{
	_Variant_free(_type, _value);
	if (mem.isNotNull()) {
		_type = VariantType::Object;
		new PTR_VAR(Memory, _value) Memory(mem);
	} else {
		_type = VariantType::Null;
	}
}

sl_bool Variant::isVariantList() const
{
	return IsInstanceOf< CList<Variant> >(getObject());
}

List<Variant> Variant::getVariantList() const
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>* p = CastInstance< CList<Variant> >(obj._ptr)) {
		return p;
	}
	return sl_null;
}

sl_bool Variant::isVariantMap() const
{
	return IsInstanceOf< IMap<String, Variant> >(getObject());
}

Map<String, Variant> Variant::getVariantMap() const
{
	Ref<Referable> obj(getObject());
	if (IMap<String, Variant>* p = CastInstance< IMap<String, Variant> >(obj._ptr)) {
		return p;
	}
	return sl_null;
}

sl_bool Variant::isVariantMapList() const
{
	return IsInstanceOf< CList< Map<String, Variant> > >(getObject());
}

List< Map<String, Variant> > Variant::getVariantMapList() const
{
	Ref<Referable> obj(getObject());
	if (CList< Map<String, Variant> >* p = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
		return p;
	}
	return sl_null;
}

sl_size Variant::getListElementsCount() const
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>* p1 = CastInstance< CList<Variant> >(obj._ptr)) {
		return p1->getCount();
	} else if (CList< Map<String, Variant> >* p2 = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
		return p2->getCount();
	}
	return 0;
}

Variant Variant::getListElement(sl_size index) const
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>* p1 = CastInstance< CList<Variant> >(obj._ptr)) {
		return p1->getValueAt(index);
	} else if (CList< Map<String, Variant> >* p2 = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
		return p2->getValueAt(index);
	}
	return sl_null;
}

sl_bool Variant::setListElement(sl_size index, const Variant& value)
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>* p = CastInstance< CList<Variant> >(obj._ptr)) {
		return p->setAt(index, value);
	}
	return sl_false;
}

sl_bool Variant::addListElement(const Variant& value)
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>* p = CastInstance< CList<Variant> >(obj._ptr)) {
		return p->add(value);
	}
	return sl_false;
}

Variant Variant::getField(const String& key) const
{
	Ref<Referable> obj(getObject());
	if (IMap<String, Variant>* p = CastInstance< IMap<String, Variant> >(obj._ptr)) {
		return p->getValue(key);
	}
	return sl_null;
}

sl_bool Variant::putField(const String& key, const Variant& value)
{
	Ref<Referable> obj(getObject());
	if (IMap<String, Variant>* p = CastInstance< IMap<String, Variant> >(obj._ptr)) {
		return p->put(key, value);
	}
	return sl_false;
}


#define _MAX_VAR_STRING_LEN 20000
static sl_bool _Variant_getVariantMapJsonString(StringBuffer& ret, const Map<String, Variant>& map, sl_bool flagJSON);
static sl_bool _Variant_getVariantMapListJsonString(StringBuffer& ret, const List< Map<String, Variant> >& list, sl_bool flagJSON);
static sl_bool _Variant_getVariantListJsonString(StringBuffer& ret, const List<Variant>& list, sl_bool flagJSON)
{
	ListLocker<Variant> l(list);
	sl_size n = l.count;
	Variant* lb = l.data;
	
	if (!(ret.addStatic("[", 1))) {
		return sl_false;
	}
	for (sl_size i = 0; i < n; i++) {
		Variant& v = lb[i];
		if (i) {
			if (!(ret.addStatic(", ", 2))) {
				return sl_false;
			}
		}
		if (v.isObject()) {
			Ref<Referable> obj(v.getObject());
			if (obj.isNotNull()) {
				if (CList<Variant>* p1 = CastInstance< CList<Variant> >(obj._ptr)) {
					if (!_Variant_getVariantListJsonString(ret, p1, flagJSON)) {
						return sl_false;
					}
				} else if (IMap<String, Variant>* p2 = CastInstance< IMap<String, Variant> >(obj._ptr)) {
					if (!_Variant_getVariantMapJsonString(ret, p2, flagJSON)) {
						return sl_false;
					}
				} else if (CList< Map<String, Variant> >* p3 = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
					if (!_Variant_getVariantMapListJsonString(ret, p3, flagJSON)) {
						return sl_false;
					}
				}
			}
		} else {
			String valueText;
			if (flagJSON) {
				valueText = v.toJsonString();
				if (valueText.isEmpty()) {
					SLIB_STATIC_STRING(_null, "null");
					valueText = _null;
				}
			} else {
				valueText = v.toString();
			}
			if (!(ret.add(valueText))) {
				return sl_false;
			}
		}
		if (!flagJSON) {
			if (ret.getLength() > _MAX_VAR_STRING_LEN) {
				return sl_false;
			}
		}
	}
	if (!(ret.addStatic("]", 1))) {
		return sl_false;
	}
	return sl_true;
}

static sl_bool _Variant_getVariantMapListJsonString(StringBuffer& ret, const List< Map<String, Variant> >& list, sl_bool flagJSON)
{
	ListLocker< Map<String, Variant> > l(list);
	sl_size n = l.count;
	Map<String, Variant>* lb = l.data;
	
	if (!(ret.addStatic("[", 1))) {
		return sl_false;
	}
	for (sl_size i = 0; i < n; i++) {
		Map<String, Variant>& v = lb[i];
		if (i) {
			if (!(ret.addStatic(", ", 2))) {
				return sl_false;
			}
		}
		if (!_Variant_getVariantMapJsonString(ret, v, flagJSON)) {
			return sl_false;
		}
		if (!flagJSON) {
			if (ret.getLength() > _MAX_VAR_STRING_LEN) {
				return sl_false;
			}
		}
	}
	if (!(ret.addStatic("]", 1))) {
		return sl_false;
	}
	return sl_true;
}

static sl_bool _Variant_getVariantMapJsonString(StringBuffer& ret, const Map<String, Variant>& map, sl_bool flagJSON)
{
	Iterator< Pair<String, Variant> > iterator(map.toIterator());
	if (!(ret.addStatic("{", 1))) {
		return sl_false;
	}
	sl_bool flagFirst = sl_true;
	Pair<String, Variant> pair;
	while (iterator.next(&pair)) {
		Variant& v = pair.value;
		if (!flagFirst) {
			if (!(ret.addStatic(", ", 2))) {
				return sl_false;
			}
		}
		if (flagJSON) {
			if (!(ret.add(pair.key.applyBackslashEscapes()))) {
				return sl_false;
			}
		} else {
			if (!(ret.add(pair.key))) {
				return sl_false;
			}
		}
		if (!(ret.addStatic(": ", 2))) {
			return sl_false;
		}
		if (v.isObject()) {
			Ref<Referable> obj(v.getObject());
			if (obj.isNotNull()) {
				if (CList<Variant>* p1 = CastInstance< CList<Variant> >(obj._ptr)) {
					if (!_Variant_getVariantListJsonString(ret, p1, flagJSON)) {
						return sl_false;
					}
				} else if (IMap<String, Variant>* p2 = CastInstance< IMap<String, Variant> >(obj._ptr)) {
					if (!_Variant_getVariantMapJsonString(ret, p2, flagJSON)) {
						return sl_false;
					}
				} else if (CList< Map<String, Variant> >* p3 = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
					if (!_Variant_getVariantMapListJsonString(ret, p3, flagJSON)) {
						return sl_false;
					}
				}
			}
		} else {
			String valueText;
			if (!v.isVariantList() && !v.isVariantMap()) {
				if (flagJSON) {
					valueText = v.toJsonString();
					if (valueText.isEmpty()) {
						SLIB_STATIC_STRING(_null, "null");
						valueText = _null;
					}
				} else {
					valueText = v.toString();
				}
			}
			if (!(ret.add(valueText))) {
				return sl_false;
			}
		}
		if (!flagJSON) {
			if (ret.getLength() > _MAX_VAR_STRING_LEN) {
				return sl_false;
			}
		}
		flagFirst = sl_false;
	}
	if (!(ret.addStatic("}", 1))) {
		return sl_false;
	}
	return sl_true;
}

String Variant::toString() const
{
	switch (_type) {
		case VariantType::Null:
			{
				SLIB_STATIC_STRING(s, "<null>");
				return s;
			}
		case VariantType::Int32:
		case VariantType::Uint32:
		case VariantType::Int64:
		case VariantType::Uint64:
		case VariantType::Float:
		case VariantType::Double:
		case VariantType::Boolean:
			return getString();
		case VariantType::String8:
			return "\"" + REF_VAR(String const, _value) + "\"";
		case VariantType::String16:
			return "\"" + REF_VAR(String16 const, _value) + "\"";
		case VariantType::Sz8:
			return "\"" + String(REF_VAR(sl_char8 const* const, _value)) + "\"";
		case VariantType::Sz16:
			return "\"" + String(REF_VAR(sl_char16 const* const, _value)) + "\"";
		case VariantType::Time:
			return "\"" + REF_VAR(Time const, _value).toString() + "\"";
		case VariantType::Pointer:
			return "#" + String::fromPointerValue(REF_VAR(void const* const, _value));
		case VariantType::Object:
		case VariantType::Weak:
			{
				Ref<Referable> obj(getObject());
				if (obj.isNotNull()) {
					if (CList<Variant>* p1 = CastInstance< CList<Variant> >(obj._ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantListJsonString(ret, p1, sl_false)) {
							ret.addStatic(" ...", 4);
						}
						return ret.merge();
					} else if (IMap<String, Variant>* p2 = CastInstance< IMap<String, Variant> >(obj._ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapJsonString(ret, p2, sl_false)) {
							ret.addStatic(" ...", 4);
						}
						return ret.merge();
					} else if (CList< Map<String, Variant> >* p3 = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapListJsonString(ret, p3, sl_false)) {
							ret.addStatic(" ...", 4);
						}
						return ret.merge();
					} else {
						return "<object:" + String::fromUtf8(obj->getObjectType()) + ">";
					}
				} else {
					return "<object:null>";
				}
			}
		default:
			return "<error-type>";
	}
}

String Variant::toJsonString() const
{
	switch (_type) {
		case VariantType::Null:
			return sl_null;
		case VariantType::Int32:
		case VariantType::Uint32:
		case VariantType::Int64:
		case VariantType::Uint64:
		case VariantType::Float:
		case VariantType::Double:
		case VariantType::Boolean:
			return getString();
		case VariantType::Time:
		case VariantType::String8:
		case VariantType::Sz8:
			return getString().applyBackslashEscapes();
		case VariantType::String16:
		case VariantType::Sz16:
			return getString16().applyBackslashEscapes();
		case VariantType::Object:
		case VariantType::Weak:
			{
				Ref<Referable> obj(getObject());
				if (obj.isNotNull()) {
					if (CList<Variant>* p1 = CastInstance< CList<Variant> >(obj._ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantListJsonString(ret, p1, sl_false)) {
							return sl_null;
						}
						return ret.merge();
					} else if (IMap<String, Variant>* p2 = CastInstance< IMap<String, Variant> >(obj._ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapJsonString(ret, p2, sl_false)) {
							return sl_null;
						}
						return ret.merge();
					} else if (CList< Map<String, Variant> >* p3 = CastInstance< CList< Map<String, Variant> > >(obj._ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapListJsonString(ret, p3, sl_false)) {
							return sl_null;
						}
						return ret.merge();
					} else {
						return sl_null;
					}
				} else {
					return sl_null;
				}
			}
			return sl_null;
		default:
			return sl_null;
	}
}


Atomic<Variant>::Atomic(AtomicVariant&& other)
{
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

Atomic<Variant>::Atomic(const AtomicVariant& other)
{
	other._retain(_type, _value);
}

Atomic<Variant>::Atomic(Variant&& other)
{
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

Atomic<Variant>::Atomic(const Variant& other)
{
	_type = other._type;
	_Variant_copy(_type, other._value, _value);
}

Atomic<Variant>::~Atomic()
{
	_Variant_free(_type, _value);
}

Atomic<Variant>::Atomic(sl_null_t)
: _type(VariantType::Null)
{
}

Atomic<Variant>::Atomic(sl_int32 value)
{
	_type = VariantType::Int32;
	REF_VAR(sl_int32, _value) = value;
}

Atomic<Variant>::Atomic(sl_uint32 value)
{
	_type = VariantType::Uint32;
	REF_VAR(sl_uint32, _value) = value;
}

Atomic<Variant>::Atomic(sl_int64 value)
{
	_type = VariantType::Int64;
	REF_VAR(sl_int64, _value) = value;
}

Atomic<Variant>::Atomic(sl_uint64 value)
{
	_type = VariantType::Uint64;
	REF_VAR(sl_uint64, _value) = value;
}

Atomic<Variant>::Atomic(float value)
{
	_type = VariantType::Float;
	REF_VAR(float, _value) = value;
}

Atomic<Variant>::Atomic(double value)
{
	_type = VariantType::Double;
	REF_VAR(double, _value) = value;
}

Atomic<Variant>::Atomic(const sl_bool value)
{
	_type = VariantType::Boolean;
	REF_VAR(sl_bool, _value) = value;
}

Atomic<Variant>::Atomic(const String& value)
{
	if (value.isNotNull()) {
		_type = VariantType::String8;
		new PTR_VAR(String, _value) String(value);
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const String16& value)
{
	if (value.isNotNull()) {
		_type = VariantType::String16;
		new PTR_VAR(String16, _value) String16(value);
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const AtomicString& s)
{
	String value(s);
	if (value.isNotNull()) {
		_type = VariantType::String8;
		new PTR_VAR(String, _value) String(value);
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const AtomicString16& s)
{
	String16 value(s);
	if (value.isNotNull()) {
		_type = VariantType::String16;
		new PTR_VAR(String16, _value) String16(value);
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const sl_char8* value)
{
	if (value) {
		_type = VariantType::Sz8;
		REF_VAR(const sl_char8*, _value) = value;
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const sl_char16* value)
{
	if (value) {
		_type = VariantType::Sz16;
		REF_VAR(const sl_char16*, _value) = value;
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const Time& value)
{
	_type = VariantType::Time;
	REF_VAR(Time, _value) = value;
}

Atomic<Variant>::Atomic(const void* ptr)
{
	if (ptr) {
		_type = VariantType::Pointer;
		REF_VAR(const void*, _value) = ptr;
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const Memory& mem)
{
	if (mem.isNotNull()) {
		_type = VariantType::Object;
		new PTR_VAR(Memory, _value) Memory(mem);
	} else {
		_type = VariantType::Null;
	}
}

Atomic<Variant>::Atomic(const AtomicMemory& _mem)
{
	Memory mem(_mem);
	if (mem.isNotNull()) {
		_type = VariantType::Object;
		new PTR_VAR(Memory, _value) Memory(mem);
	} else {
		_type = VariantType::Null;
	}
}

AtomicVariant& Atomic<Variant>::operator=(AtomicVariant&& other)
{
	if (this != &other) {
		_replace(other._type, other._value);
		other._type = VariantType::Null;
	}
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const AtomicVariant& other)
{
	if (this != &other) {
		VariantType type;
		sl_uint64 value;
		other._retain(type, value);
		_replace(type, value);
	}
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(Variant&& other)
{
	if ((void*)this != (void*)(&other)) {
		_replace(other._type, other._value);
		other._type = VariantType::Null;
	}
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const Variant& other)
{
	VariantType type = other._type;
	sl_uint64 value;
	_Variant_copy(type, other._value, value);
	_replace(type, value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(sl_null_t)
{
	setNull();
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(sl_int32 value)
{
	setInt32(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(sl_uint32 value)
{
	setUint32(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(sl_int64 value)
{
	setInt64(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(sl_uint64 value)
{
	setUint64(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(float value)
{
	setFloat(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(double value)
{
	setDouble(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const sl_bool value)
{
	setBoolean(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const String& value)
{
	setString(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const String16& value)
{
	setString(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const AtomicString& value)
{
	setString(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const AtomicString16& value)
{
	setString(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const sl_char8* value)
{
	setString(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const sl_char16* value)
{
	setString(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const Time& value)
{
	setTime(value);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const void* ptr)
{
	setPointer(ptr);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const Memory& mem)
{
	setMemory(mem);
	return *this;
}

AtomicVariant& Atomic<Variant>::operator=(const AtomicMemory& mem)
{
	setMemory(mem);
	return *this;
}

Variant Atomic<Variant>::operator[](sl_size indexForVariantList) const
{
	return getListElement(indexForVariantList);
}

Variant Atomic<Variant>::operator[](const String& keyForVariantMap) const
{
	return getField(keyForVariantMap);
}

void Atomic<Variant>::setNull()
{
	if (_type != VariantType::Null) {
		_replace(VariantType::Null, 0);
	}
}

sl_bool Atomic<Variant>::isInt32() const
{
	return _type == VariantType::Int32;
}

sl_int32 Atomic<Variant>::getInt32(sl_int32 def) const
{
	Variant var(*this);
	return var.getInt32(def);
}

void Atomic<Variant>::setInt32(sl_int32 value)
{
	sl_int64 v;
	REF_VAR(sl_int32, v) = value;
	_replace(VariantType::Int32, v);
}

sl_bool Atomic<Variant>::isUint32() const
{
	return _type == VariantType::Uint32;
}

sl_uint32 Atomic<Variant>::getUint32(sl_uint32 def) const
{
	Variant var(*this);
	return var.getUint32(def);
}

void Atomic<Variant>::setUint32(sl_uint32 value)
{
	sl_int64 v;
	REF_VAR(sl_uint32, v) = value;
	_replace(VariantType::Uint32, v);
}

sl_bool Atomic<Variant>::isInt64() const
{
	return _type == VariantType::Int64;
}

sl_int64 Atomic<Variant>::getInt64(sl_int64 def) const
{
	Variant var(*this);
	return var.getInt64(def);
}

void Atomic<Variant>::setInt64(sl_int64 value)
{
	_replace(VariantType::Int64, value);
}

sl_bool Atomic<Variant>::isUint64() const
{
	return _type == VariantType::Uint64;
}

sl_uint64 Atomic<Variant>::getUint64(sl_uint64 def) const
{
	Variant var(*this);
	return var.getUint64(def);
}

void Atomic<Variant>::setUint64(sl_uint64 value)
{
	_replace(VariantType::Uint64, value);
}

sl_bool Atomic<Variant>::isInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Uint32 || _type == VariantType::Int64 || _type == VariantType::Uint64;
}

sl_bool Atomic<Variant>::isSignedInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Int64;
}

sl_bool Atomic<Variant>::isUnsignedInteger() const
{
	return _type == VariantType::Uint32 || _type == VariantType::Uint64;
}

sl_bool Atomic<Variant>::isFloat() const
{
	return _type == VariantType::Float;
}

float Atomic<Variant>::getFloat(float def) const
{
	Variant var(*this);
	return var.getFloat(def);
}

void Atomic<Variant>::setFloat(float value)
{
	sl_int64 v;
	REF_VAR(float, v) = value;
	_replace(VariantType::Float, v);
}

sl_bool Atomic<Variant>::isDouble() const
{
	return _type == VariantType::Double;
}

double Atomic<Variant>::getDouble(double def) const
{
	Variant var(*this);
	return var.getDouble(def);
}

void Atomic<Variant>::setDouble(double value)
{
	sl_int64 v;
	REF_VAR(double, v) = value;
	_replace(VariantType::Double, v);
}

sl_bool Atomic<Variant>::isNumber() const
{
	return isInteger() || _type == VariantType::Float || _type == VariantType::Double;
}

sl_bool Atomic<Variant>::isBoolean() const
{
	return _type == VariantType::Boolean;
}

sl_bool Atomic<Variant>::getBoolean(sl_bool def) const
{
	Variant var(*this);
	return var.getBoolean(def);
}

void Atomic<Variant>::setBoolean(sl_bool value)
{
	sl_int64 v;
	REF_VAR(sl_bool, v) = value;
	_replace(VariantType::Boolean, v);
}

sl_bool Atomic<Variant>::isString() const
{
	return _type == VariantType::String8 || _type == VariantType::String16 || _type == VariantType::Sz8 || _type == VariantType::Sz16;
}

sl_bool Atomic<Variant>::isString8() const
{
	return _type == VariantType::String8;
}

sl_bool Atomic<Variant>::isString16() const
{
	return _type == VariantType::String16;
}

sl_bool Atomic<Variant>::isSz8() const
{
	return _type == VariantType::Sz8;
}

sl_bool Atomic<Variant>::isSz16() const
{
	return _type == VariantType::Sz16;
}

String Atomic<Variant>::getString(const String& def) const
{
	Variant var(*this);
	return var.getString(def);
}

String Atomic<Variant>::getString() const
{
	Variant var(*this);
	return var.getString();
}

String16 Atomic<Variant>::getString16(const String16& def) const
{
	Variant var(*this);
	return var.getString16(def);
}

String16 Atomic<Variant>::getString16() const
{
	Variant var(*this);
	return var.getString16();
}

const sl_char8* Atomic<Variant>::getSz8(const sl_char8* def) const
{
	Variant var(*this);
	return var.getSz8(def);
}

const sl_char16* Atomic<Variant>::getSz16(const sl_char16* def) const
{
	Variant var(*this);
	return var.getSz16(def);
}

void Atomic<Variant>::setString(const String& value)
{
	if (value.isNotNull()) {
		sl_int64 v;
		new PTR_VAR(String, v) String(value);
		_replace(VariantType::String8, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

void Atomic<Variant>::setString(const String16& value)
{
	if (value.isNotNull()) {
		sl_int64 v;
		new PTR_VAR(String16, v) String16(value);
		_replace(VariantType::String16, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

void Atomic<Variant>::setString(const AtomicString& s)
{
	String value(s);
	if (value.isNotNull()) {
		sl_int64 v;
		new PTR_VAR(String, v) String(value);
		_replace(VariantType::String8, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

void Atomic<Variant>::setString(const AtomicString16& s)
{
	String16 value(s);
	if (value.isNotNull()) {
		sl_int64 v;
		new PTR_VAR(String16*, v) String16(value);
		_replace(VariantType::String16, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

void Atomic<Variant>::setString(const sl_char8* value)
{
	if (value) {
		sl_int64 v;
		REF_VAR(const sl_char8*, v) = value;
		_replace(VariantType::Sz8, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

void Atomic<Variant>::setString(const sl_char16* value)
{
	if (value) {
		sl_int64 v;
		REF_VAR(const sl_char16*, v) = value;
		_replace(VariantType::Sz16, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

sl_bool Atomic<Variant>::isTime() const
{
	return _type == VariantType::Time;
}

Time Atomic<Variant>::getTime(Time def) const
{
	Variant var(*this);
	return var.getTime(def);
}

Time Atomic<Variant>::getTime() const
{
	Variant var(*this);
	return var.getTime();
}

void Atomic<Variant>::setTime(const Time& value)
{
	sl_int64 v;
	REF_VAR(Time, v) = value;
	_replace(VariantType::Time, v);
}

sl_bool Atomic<Variant>::isPointer() const
{
	return _type == VariantType::Pointer || _type == VariantType::Sz8 || _type == VariantType::Sz16;
}

void* Atomic<Variant>::getPointer(const void* def) const
{
	Variant var(*this);
	return var.getPointer(def);
}

void Atomic<Variant>::setPointer(const void *ptr)
{
	if (ptr) {
		sl_int64 v;
		REF_VAR(const void*, v) = ptr;
		_replace(VariantType::Pointer, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

sl_bool Atomic<Variant>::isObject() const
{
	return _type == VariantType::Object;
}

sl_bool Atomic<Variant>::isWeak() const
{
	return _type == VariantType::Weak;
}

Ref<Referable> Atomic<Variant>::getObject() const
{
	Variant var(*this);
	return var.getObject();
}

sl_bool Atomic<Variant>::isObjectNotNull() const
{
	Variant var(*this);
	return var.isObjectNotNull();
}

sl_bool Atomic<Variant>::isObjectNull() const
{
	Variant var(*this);
	return var.isObjectNull();
}

sl_object_type Atomic<Variant>::getObjectType() const
{
	Variant var(*this);
	return var.getObjectType();
}

sl_bool Atomic<Variant>::isMemory() const
{
	Variant var(*this);
	return var.isMemory();
}

Memory Atomic<Variant>::getMemory() const
{
	Variant var(*this);
	return var.getMemory();
}

void Atomic<Variant>::setMemory(const Memory& mem)
{
	if (mem.isNotNull()) {
		sl_int64 v;
		new PTR_VAR(Memory, v) Memory(mem);
		_replace(VariantType::Object, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

sl_bool Atomic<Variant>::isVariantList() const
{
	Variant var(*this);
	return var.isVariantList();
}

List<Variant> Atomic<Variant>::getVariantList() const
{
	Variant var(*this);
	return var.getVariantList();
}

sl_bool Atomic<Variant>::isVariantMap() const
{
	Variant var(*this);
	return var.isVariantMap();
}

Map<String, Variant> Atomic<Variant>::getVariantMap() const
{
	Variant var(*this);
	return var.getVariantMap();
}

sl_bool Atomic<Variant>::isVariantMapList() const
{
	Variant var(*this);
	return var.isVariantMap();
}

List< Map<String, Variant> > Atomic<Variant>::getVariantMapList() const
{
	Variant var(*this);
	return var.getVariantMapList();
}

sl_size Atomic<Variant>::getListElementsCount() const
{
	Variant var(*this);
	return var.getListElementsCount();
}

Variant Atomic<Variant>::getListElement(sl_size index) const
{
	Variant var(*this);
	return var.getListElement(index);
}

sl_bool Atomic<Variant>::setListElement(sl_size index, const Variant& value)
{
	Variant var(*this);
	return var.setListElement(index, value);
}

sl_bool Atomic<Variant>::addListElement(const Variant& value)
{
	Variant var(*this);
	return var.addListElement(value);
}

Variant Atomic<Variant>::getField(const String& key) const
{
	Variant var(*this);
	return var.getField(key);
}

sl_bool Atomic<Variant>::putField(const String& key, const Variant& value)
{
	Variant var(*this);
	return var.putField(key, value);
}

String Atomic<Variant>::toString() const
{
	Variant var(*this);
	return var.toString();
}

String Atomic<Variant>::toJsonString() const
{
	Variant var(*this);
	return var.toJsonString();
}

sl_bool operator==(const Variant& v1, const Variant& v2)
{
	VariantType type = v1._type;
	if (type == v2._type) {
		if (v1._value == v2._value) {
			return sl_true;
		}
		switch (type) {
			case VariantType::Null:
				return sl_true;
			case VariantType::Int32:
			case VariantType::Uint32:
				return REF_VAR(sl_int32 const, v1._value) == REF_VAR(sl_int32 const, v2._value);
			case VariantType::Float:
				return REF_VAR(float const, v1._value) == REF_VAR(float const, v2._value);
			case VariantType::Double:
				return REF_VAR(double const, v1._value) == REF_VAR(double const, v2._value);
			case VariantType::Boolean:
				return REF_VAR(sl_bool const, v1._value) == REF_VAR(sl_bool const, v2._value);
			case VariantType::Pointer:
			case VariantType::Sz8:
			case VariantType::Sz16:
			case VariantType::Object:
				return REF_VAR(void const* const, v1._value) == REF_VAR(void const* const, v2._value);
			case VariantType::String8:
				return REF_VAR(String const, v1._value) == REF_VAR(String const, v2._value);
			case VariantType::String16:
				return REF_VAR(String16 const, v1._value) == REF_VAR(String16 const, v2._value);
			default:
				break;
		}
	}
	return sl_false;
}

sl_bool operator!=(const Variant& v1, const Variant& v2)
{
	return !(v1 == v2);
}

SLIB_NAMESPACE_END
