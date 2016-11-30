#include "../../../inc/slib/core/variant.h"

SLIB_NAMESPACE_BEGIN

#define PTR_VAR(TYPE, x) ((TYPE*)((void*)(&x)))
#define REF_VAR(TYPE, x) (*PTR_VAR(TYPE, x))

template <>
sl_int32 Compare<Variant>::compare(const Variant &a, const Variant &b)
{
	return Compare<sl_uint64>::compare(a._value, b._value);
}

const char _VariantMap_ClassID[] = "VariantMap";

template <>
sl_class_type IMap<String, Variant>::ClassType()
{
	return _VariantMap_ClassID;
}

template <>
sl_class_type IMap<String, Variant>::getClassType() const
{
	return _VariantMap_ClassID;
}

template <>
sl_bool IMap<String, Variant>::checkClassType(sl_class_type type) const
{
	if (type == _VariantMap_ClassID) {
		return sl_true;
	}
	return _checkClassType(type);
}

const char _VariantList_ClassID[] = "VariantList";

template <>
sl_class_type CList<Variant>::ClassType()
{
	return _VariantList_ClassID;
}

template <>
sl_class_type CList<Variant>::getClassType() const
{
	return _VariantList_ClassID;
}

template <>
sl_bool CList<Variant>::checkClassType(sl_class_type type) const
{
	if (type == _VariantMap_ClassID) {
		return sl_true;
	}
	return _checkClassType(type);
}

const char _VariantMapList_ClassID[] = "VariantMapList";

template <>
sl_class_type CList< Map<String, Variant> >::ClassType()
{
	return _VariantMapList_ClassID;
}

template <>
sl_class_type CList< Map<String, Variant> >::getClassType() const
{
	return _VariantMapList_ClassID;
}

template <>
sl_bool CList< Map<String, Variant> >::checkClassType(sl_class_type type) const
{
	if (type == _VariantMapList_ClassID) {
		return sl_true;
	}
	return _checkClassType(type);
}


SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Variant)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, Variant)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(sl_uint64, Variant)

const _Variant_Const _Variant_Null = {0, VariantType::Null, 0};


SLIB_INLINE void _Variant_copy(VariantType src_type, sl_uint64 src_value, sl_uint64& dst_value)
{
	switch (src_type) {
		case VariantType::String8:
			new PTR_VAR(String8, dst_value) String8(REF_VAR(String8, src_value));
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
			REF_VAR(String8, value).String8::~String8();
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

SLIB_INLINE void SafeVariant::_retain(VariantType& type, sl_uint64& value) const
{
	if ((void*)(this) == (void*)(&_Variant_Null)) {
		type = VariantType::Null;
	} else {
		SpinLocker lock(&m_lock);
		type = _type;
		_Variant_copy(_type, _value, value);
	}
}

SLIB_INLINE void SafeVariant::_replace(VariantType type, sl_uint64 value)
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

Variant::Variant(SafeVariant&& _other)
{
	Variant& other = REF_VAR(Variant, _other);
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

Variant::Variant(const SafeVariant& other)
{
	other._retain(_type, _value);
}

Variant::~Variant()
{
	_Variant_free(_type, _value);
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

Variant::Variant(const String8& value)
{
	_type = VariantType::String8;
	new PTR_VAR(String8, _value) String8(value);
}

Variant::Variant(const SafeString8& value)
{
	_type = VariantType::String8;
	new PTR_VAR(String8, _value) String8(value);
}

Variant::Variant(const String16& value)
{
	_type = VariantType::String16;
	new PTR_VAR(String16, _value) String16(value);
}

Variant::Variant(const SafeString16& value)
{
	_type = VariantType::String16;
	new PTR_VAR(String16, _value) String16(value);
}

Variant::Variant(const sl_char8* sz8)
{
	_type = VariantType::Sz8;
	REF_VAR(const sl_char8*, _value) = sz8;
}

Variant::Variant(const sl_char16* sz16)
{
	_type = VariantType::Sz16;
	REF_VAR(const sl_char16*, _value) = sz16;
}

Variant::Variant(const Time& value)
{
	_type = VariantType::Time;
	REF_VAR(Time, _value) = value;
}

Variant::Variant(const void* ptr)
{
	_type = VariantType::Pointer;
	REF_VAR(const void*, _value) = ptr;
}

Variant::Variant(const Memory& mem)
{
	_type = VariantType::Object;
	new PTR_VAR(Memory, _value) Memory(mem);
}

Variant::Variant(const SafeMemory& mem)
{
	_type = VariantType::Object;
	new PTR_VAR(Memory, _value) Memory(mem);
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

Variant Variant::fromString(const String8& value)
{
	return value;
}

Variant Variant::fromString8(const String8& value)
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

Variant& Variant::operator=(SafeVariant&& other)
{
	if ((void*)this != (void*)(&other)) {
		_Variant_free(_type, _value);
		_type = other._type;
		_value = other._value;
		other._type = VariantType::Null;
	}
	return *this;
}

Variant& Variant::operator=(const SafeVariant& other)
{
	_Variant_free(_type, _value);
	other._retain(_type, _value);
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

Variant& Variant::operator=(const String8& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const SafeString8& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const String16& value)
{
	setString(value);
	return *this;
}

Variant& Variant::operator=(const SafeString16& value)
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

Variant& Variant::operator=(const SafeMemory& mem)
{
	setMemory(mem);
	return *this;
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
			return (sl_int32)(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return (sl_int32)(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return (sl_int32)(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return (sl_int32)(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return (sl_int32)(REF_VAR(float, _value));
		case VariantType::Double:
			return (sl_int32)(REF_VAR(double, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool, _value)) ? 1 : 0;
		case VariantType::String8:
			return REF_VAR(String8, _value).parseInt32(10, def);
		case VariantType::String16:
			return REF_VAR(String16, _value).parseInt32(10, def);
		case VariantType::Pointer:
			return (sl_int32)(REF_VAR(sl_size, _value));
		case VariantType::Sz8:
		{
			sl_int32 ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseInt32(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_int32 ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
			return (sl_uint32)(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return (sl_uint32)(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return (sl_uint32)(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return (sl_uint32)(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return (sl_uint32)(REF_VAR(float, _value));
		case VariantType::Double:
			return (sl_uint32)(REF_VAR(double, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool, _value)) ? 1 : 0;
		case VariantType::String8:
			return REF_VAR(String8, _value).parseUint32(10, def);
		case VariantType::String16:
			return REF_VAR(String16, _value).parseUint32(10, def);
		case VariantType::Pointer:
			return (sl_uint32)(REF_VAR(sl_size, _value));
		case VariantType::Sz8:
		{
			sl_uint32 ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseUint32(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_uint32 ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
			return (sl_int64)(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return (sl_int64)(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return (sl_int64)(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return (sl_int64)(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return (sl_int64)(REF_VAR(float, _value));
		case VariantType::Double:
			return (sl_int64)(REF_VAR(double, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool, _value)) ? 1 : 0;
		case VariantType::String8:
			return (REF_VAR(String8, _value)).parseInt64(10, def);
		case VariantType::String16:
			return (REF_VAR(String16, _value)).parseInt64(10, def);
		case VariantType::Pointer:
			return (sl_int64)(REF_VAR(sl_size, _value));
		case VariantType::Sz8:
		{
			sl_int64 ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseInt64(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_int64 ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
			return (sl_uint64)(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return (sl_uint64)(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return (sl_uint64)(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return (sl_uint64)(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return (sl_uint64)(REF_VAR(float, _value));
		case VariantType::Double:
			return (sl_uint64)(REF_VAR(double, _value));
		case VariantType::Boolean:
			return (REF_VAR(sl_bool, _value)) ? 1 : 0;
		case VariantType::String8:
			return (REF_VAR(String8, _value)).parseUint64(10, def);
		case VariantType::String16:
			return (REF_VAR(String16, _value)).parseUint64(10, def);
		case VariantType::Pointer:
			return (sl_uint64)(REF_VAR(sl_size, _value));
		case VariantType::Sz8:
		{
			sl_uint64 ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseUint64(10, &ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_uint64 ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
			return (float)(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return (float)(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return (float)(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return (float)(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return (float)(REF_VAR(float, _value));
		case VariantType::Double:
			return (float)(REF_VAR(double, _value));
		case VariantType::String8:
			return (REF_VAR(String8, _value)).parseFloat(def);
		case VariantType::String16:
			return (REF_VAR(String16, _value)).parseFloat(def);
		case VariantType::Sz8:
		{
			float ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseFloat(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			float ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
			return (double)(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return (double)(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return (double)(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return (double)(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return (double)(REF_VAR(float, _value));
		case VariantType::Double:
			return (double)(REF_VAR(double, _value));
		case VariantType::String8:
			return (REF_VAR(String8, _value)).parseDouble(def);
		case VariantType::String16:
			return (REF_VAR(String16, _value)).parseDouble(def);
		case VariantType::Sz8:
		{
			double ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseDouble(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			double ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
			sl_int32 n = REF_VAR(sl_int32, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Uint32:
		{
			sl_uint32 n = REF_VAR(sl_uint32, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Int64:
		{
			sl_int64 n = REF_VAR(sl_int64, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Uint64:
		{
			sl_uint64 n = REF_VAR(sl_uint64, _value);
			if (n != 0) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		case VariantType::Boolean:
			return REF_VAR(sl_bool, _value);
		case VariantType::String8:
			return (REF_VAR(String8, _value)).parseBoolean(def);
		case VariantType::String16:
			return (REF_VAR(String16, _value)).parseBoolean(def);
		case VariantType::Sz8:
		{
			sl_bool ret;
			const sl_char8* str = REF_VAR(const sl_char8*, _value);
			sl_reg pos = String8::parseBoolean(&ret, str);
			if (pos != SLIB_PARSE_ERROR && str[pos] == 0) {
				return ret;
			}
			break;
		}
		case VariantType::Sz16:
		{
			sl_bool ret;
			const sl_char16* str = REF_VAR(const sl_char16*, _value);
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
	return getString8(def);
}

String Variant::getString() const
{
	return getString8(String8::null());
}

String8 Variant::getString8(const String8& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String8::fromInt32(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return String8::fromUint32(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return String8::fromInt64(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return String8::fromUint64(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return String8::fromFloat(REF_VAR(float, _value));
		case VariantType::Double:
			return String8::fromDouble(REF_VAR(double, _value));
		case VariantType::Boolean:
			if (REF_VAR(sl_bool, _value)) {
				SLIB_STATIC_STRING8_BY_ARRAY(ret, 't', 'r', 'u', 'e');
				return ret;
			} else {
				SLIB_STATIC_STRING8_BY_ARRAY(ret, 'f', 'a', 'l', 's', 'e');
				return ret;
			}
		case VariantType::Time:
			return REF_VAR(Time, _value).toString();
		case VariantType::String8:
			return REF_VAR(String8, _value);
		case VariantType::String16:
			return REF_VAR(String16, _value);
		case VariantType::Sz8:
			return REF_VAR(const sl_char8*, _value);
		case VariantType::Sz16:
			return REF_VAR(const sl_char16*, _value);
		case VariantType::Pointer:
			return "#" + String8::fromPointerValue(REF_VAR(const void*, _value));
		default:
			break;
	}
	return def;
}

String8 Variant::getString8() const
{
	return getString8(String8::null());
}

String16 Variant::getString16(const String16& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String16::fromInt32(REF_VAR(sl_int32, _value));
		case VariantType::Uint32:
			return String16::fromUint32(REF_VAR(sl_uint32, _value));
		case VariantType::Int64:
			return String16::fromInt64(REF_VAR(sl_int64, _value));
		case VariantType::Uint64:
			return String16::fromUint64(REF_VAR(sl_uint64, _value));
		case VariantType::Float:
			return String16::fromFloat(REF_VAR(float, _value));
		case VariantType::Double:
			return String16::fromDouble(REF_VAR(double, _value));
		case VariantType::Boolean:
			if (REF_VAR(sl_bool, _value)) {
				SLIB_STATIC_STRING16_BY_ARRAY(ret, 't', 'r', 'u', 'e');
				return ret;
			} else {
				SLIB_STATIC_STRING16_BY_ARRAY(ret, 'f', 'a', 'l', 's', 'e');
				return ret;
			}
		case VariantType::Time:
			return REF_VAR(Time, _value).toString();
		case VariantType::String8:
			return REF_VAR(String8, _value);
		case VariantType::String16:
			return REF_VAR(String16, _value);
		case VariantType::Sz8:
			return REF_VAR(const sl_char8*, _value);
		case VariantType::Sz16:
			return REF_VAR(const sl_char16*, _value);
		case VariantType::Pointer:
			return "#" + String16::fromPointerValue(REF_VAR(const void*, _value));
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
			if (REF_VAR(sl_bool, _value)) {
				return "true";
			} else {
				return "false";
			}
		case VariantType::String8:
			return REF_VAR(String8, _value).getData();
		case VariantType::Sz8:
			return REF_VAR(const sl_char8*, _value);
		default:
			break;
	}
	return def;
}

const sl_char16* Variant::getSz16(const sl_char16* def) const
{
	switch (_type) {
		case VariantType::Boolean:
			if (REF_VAR(sl_bool, _value)) {
				static const sl_char16 _s[] = {'t', 'r', 'u', 'e', 0};
				return _s;
			} else {
				static const sl_char16 _s[] = {'f', 'a', 'l', 's', 'e', 0};
				return _s;
			}
		case VariantType::String16:
			return REF_VAR(String16, _value).getData();
		case VariantType::Sz16:
			return REF_VAR(const sl_char16*, _value);
		default:
			break;
	}
	return def;
}

void Variant::setString(const String8& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String8;
	new PTR_VAR(String8, _value) String8(value);
}

void Variant::setString(const String16& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String16;
	new PTR_VAR(String16, _value) String16(value);
}

void Variant::setString(const SafeString8& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String8;
	new PTR_VAR(String8, _value) String8(value);
}

void Variant::setString(const SafeString16& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String16;
	new PTR_VAR(String16, _value) String16(value);
}

void Variant::setString(const sl_char8* value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Sz8;
	REF_VAR(const sl_char8*, _value) = value;
}

void Variant::setString(const sl_char16* value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Sz16;
	REF_VAR(const sl_char16*, _value) = value;
}

sl_bool Variant::isTime() const
{
	return _type == VariantType::Time;
}

Time Variant::getTime(const Time& def) const
{
	switch (_type) {
		case VariantType::Time:
			return REF_VAR(Time, _value);
		case VariantType::String8:
			return Time(REF_VAR(String8, _value));
		case VariantType::String16:
			return Time(REF_VAR(String16, _value));
		case VariantType::Sz8:
			return Time(REF_VAR(const sl_char8*, _value));
		case VariantType::Sz16:
			return Time(REF_VAR(const sl_char16*, _value));
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
		return REF_VAR(void*, _value);
	}
	return (void*)def;
}

void Variant::setPointer(const void *ptr)
{
	_Variant_free(_type, _value);
	_type = VariantType::Pointer;
	REF_VAR(const void*, _value) = ptr;
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
		return REF_VAR(Ref<Referable>, _value);
	} else if (_type == VariantType::Weak) {
		return REF_VAR(WeakRef<Referable>, _value);
	}
	return Ref<Referable>::null();
}

sl_class_type Variant::getObjectClassType() const
{
	Ref<Referable> obj(getObject());
	if (obj.isNotNull()) {
		return obj->getClassType();
	}
	return 0;
}

sl_bool Variant::isObjectNotNull() const
{
	if (_type == VariantType::Object || _type == VariantType::Weak) {
		return REF_VAR(Ref<Referable>, _value).isNotNull();
	}
	return sl_false;
}

sl_bool Variant::isObjectNull() const
{
	if (_type == VariantType::Object || _type == VariantType::Weak) {
		return REF_VAR(Ref<Referable>, _value).isNull();
	}
	return sl_true;
}

sl_bool Variant::isMemory() const
{
	Ref<Referable> obj(getObject());
	return Memory::checkInstance(obj.ptr);
}

Memory Variant::getMemory() const
{
	Ref<Referable> obj(getObject());
	if (Memory::checkInstance(obj.ptr)) {
		return (CMemory*)(obj.ptr);
	}
	return Memory::null();
}

void Variant::setMemory(const Memory& mem)
{
	_Variant_free(_type, _value);
	_type = VariantType::Object;
	new PTR_VAR(Memory, _value) Memory(mem);

}

sl_bool Variant::isVariantList() const
{
	Ref<Referable> obj(getObject());
	return CList<Variant>::checkInstance(obj.ptr);
}

List<Variant> Variant::getVariantList() const
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>::checkInstance(obj.ptr)) {
		return (CList<Variant>*)(obj.ptr);
	}
	return List<Variant>::null();
}

sl_bool Variant::isVariantMap() const
{
	Ref<Referable> obj(getObject());
	return IMap<String, Variant>::checkInstance(obj.ptr);
}

Map<String, Variant> Variant::getVariantMap() const
{
	Ref<Referable> obj(getObject());
	if (IMap<String, Variant>::checkInstance(obj.ptr)) {
		return (IMap<String, Variant>*)(obj.ptr);
	}
	return Map<String, Variant>::null();
}

sl_bool Variant::isVariantMapList() const
{
	Ref<Referable> obj(getObject());
	return CList< Map<String, Variant> >::checkInstance(obj.ptr);
}

List< Map<String, Variant> > Variant::getVariantMapList() const
{
	Ref<Referable> obj(getObject());
	if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
		return (CList< Map<String, Variant> >*)(obj.ptr);
	}
	return List< Map<String, Variant> >::null();
}

sl_size Variant::getListItemsCount() const
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>::checkInstance(obj.ptr)) {
		return ((CList<Variant>*)(obj.ptr))->getCount();
	} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
		return ((CList< Map<String, Variant> >*)(obj.ptr))->getCount();
	}
	return 0;
}

Variant Variant::getListItem(sl_size index) const
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>::checkInstance(obj.ptr)) {
		return ((CList<Variant>*)(obj.ptr))->getItemValue(index, Variant::null());
	} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
		return ((CList< Map<String, Variant> >*)(obj.ptr))->getItemValue(index, Map<String, Variant>::null());
	}
	return Variant::null();
}

sl_bool Variant::setListItem(sl_size index, const Variant& value)
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>::checkInstance(obj.ptr)) {
		return ((CList<Variant>*)(obj.ptr))->setItem(index, value);
	}
	return sl_false;
}

sl_bool Variant::addListItem(const Variant& value)
{
	Ref<Referable> obj(getObject());
	if (CList<Variant>::checkInstance(obj.ptr)) {
		return ((CList<Variant>*)(obj.ptr))->add(value);
	}
	return sl_false;
}

Variant Variant::getField(const String& key) const
{
	Ref<Referable> obj(getObject());
	if (Map<String, Variant>::checkInstance(obj.ptr)) {
		return ((IMap<String, Variant>*)(obj.ptr))->getValue(key, Variant::null());
	}
	return Variant::null();
}

sl_bool Variant::putField(const String& key, const Variant& value)
{
	Ref<Referable> obj(getObject());
	if (Map<String, Variant>::checkInstance(obj.ptr)) {
		return ((IMap<String, Variant>*)(obj.ptr))->put(key, value);
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
				if (CList<Variant>::checkInstance(obj.ptr)) {
					if (!_Variant_getVariantListJsonString(ret, (CList<Variant>*)(obj.ptr), flagJSON)) {
						return sl_false;
					}
				} else if (IMap<String, Variant>::checkInstance(obj.ptr)) {
					if (!_Variant_getVariantMapJsonString(ret, (IMap<String, Variant>*)(obj.ptr), flagJSON)) {
						return sl_false;
					}
				} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
					if (!_Variant_getVariantMapListJsonString(ret, (CList< Map<String, Variant> >*)(obj.ptr), flagJSON)) {
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
	Iterator< Pair<String, Variant> > iterator(map.iterator());
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
				if (CList<Variant>::checkInstance(obj.ptr)) {
					if (!_Variant_getVariantListJsonString(ret, (CList<Variant>*)(obj.ptr), flagJSON)) {
						return sl_false;
					}
				} else if (IMap<String, Variant>::checkInstance(obj.ptr)) {
					if (!_Variant_getVariantMapJsonString(ret, (IMap<String, Variant>*)(obj.ptr), flagJSON)) {
						return sl_false;
					}
				} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
					if (!_Variant_getVariantMapListJsonString(ret, (CList< Map<String, Variant> >*)(obj.ptr), flagJSON)) {
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
			return getString(String::null());
		case VariantType::String8:
			return "\"" + REF_VAR(String8, _value) + "\"";
		case VariantType::String16:
			return "\"" + REF_VAR(String16, _value) + "\"";
		case VariantType::Sz8:
			return "\"" + String(REF_VAR(const sl_char8*, _value)) + "\"";
		case VariantType::Sz16:
			return "\"" + String(REF_VAR(const sl_char16*, _value)) + "\"";
		case VariantType::Time:
			return "\"" + REF_VAR(Time, _value).toString() + "\"";
		case VariantType::Pointer:
			return "#" + String::fromPointerValue(REF_VAR(const void*, _value));
		case VariantType::Object:
		case VariantType::Weak:
			{
				Ref<Referable> obj(getObject());
				if (obj.isNotNull()) {
					if (CList<Variant>::checkInstance(obj.ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantListJsonString(ret, (CList<Variant>*)(obj.ptr), sl_false)) {
							ret.addStatic(" ...", 4);
						}
						return ret.merge();
					} else if (IMap<String, Variant>::checkInstance(obj.ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapJsonString(ret, (IMap<String, Variant>*)(obj.ptr), sl_false)) {
							ret.addStatic(" ...", 4);
						}
						return ret.merge();
					} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapListJsonString(ret, (CList< Map<String, Variant> >*)(obj.ptr), sl_false)) {
							ret.addStatic(" ...", 4);
						}
						return ret.merge();
					} else {
						return "<object:" + String::fromUtf8(obj->getClassType()) + ">";
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
			return String::null();
		case VariantType::Int32:
		case VariantType::Uint32:
		case VariantType::Int64:
		case VariantType::Uint64:
		case VariantType::Float:
		case VariantType::Double:
		case VariantType::Boolean:
			return getString(String::null());
		case VariantType::Time:
		case VariantType::String8:
		case VariantType::Sz8:
			return getString8(String8::null()).applyBackslashEscapes();
		case VariantType::String16:
		case VariantType::Sz16:
			return getString16(String16::null()).applyBackslashEscapes();
		case VariantType::Object:
		case VariantType::Weak:
			{
				Ref<Referable> obj(getObject());
				if (obj.isNotNull()) {
					if (CList<Variant>::checkInstance(obj.ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantListJsonString(ret, (CList<Variant>*)(obj.ptr), sl_true)) {
							return String::null();
						}
						return ret.merge();
					} else if (IMap<String, Variant>::checkInstance(obj.ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapJsonString(ret, (IMap<String, Variant>*)(obj.ptr), sl_true)) {
							return String::null();
						}
						return ret.merge();
					} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
						StringBuffer ret;
						if (!_Variant_getVariantMapListJsonString(ret, (CList< Map<String, Variant> >*)(obj.ptr), sl_false)) {
							return String::null();
						}
						return ret.merge();
					} else {
						return String::null();
					}
				} else {
					return String::null();
				}
			}
			return String::null();
		default:
			return String::null();
	}
}


SafeVariant::SafeVariant(SafeVariant&& other)
{
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

SafeVariant::SafeVariant(const SafeVariant& other)
{
	other._retain(_type, _value);
}

SafeVariant::SafeVariant(Variant&& other)
{
	_type = other._type;
	_value = other._value;
	other._type = VariantType::Null;
}

SafeVariant::SafeVariant(const Variant& other)
{
	_type = other._type;
	_Variant_copy(_type, other._value, _value);
}

SafeVariant::~SafeVariant()
{
	_Variant_free(_type, _value);
}

SafeVariant::SafeVariant(sl_int32 value)
{
	_type = VariantType::Int32;
	REF_VAR(sl_int32, _value) = value;
}

SafeVariant::SafeVariant(sl_uint32 value)
{
	_type = VariantType::Uint32;
	REF_VAR(sl_uint32, _value) = value;
}

SafeVariant::SafeVariant(sl_int64 value)
{
	_type = VariantType::Int64;
	REF_VAR(sl_int64, _value) = value;
}

SafeVariant::SafeVariant(sl_uint64 value)
{
	_type = VariantType::Uint64;
	REF_VAR(sl_uint64, _value) = value;
}

SafeVariant::SafeVariant(float value)
{
	_type = VariantType::Float;
	REF_VAR(float, _value) = value;
}

SafeVariant::SafeVariant(double value)
{
	_type = VariantType::Double;
	REF_VAR(double, _value) = value;
}

SafeVariant::SafeVariant(const sl_bool value)
{
	_type = VariantType::Boolean;
	REF_VAR(sl_bool, _value) = value;
}

SafeVariant::SafeVariant(const String8& value)
{
	_type = VariantType::String8;
	new PTR_VAR(String8, _value) String8(value);
}

SafeVariant::SafeVariant(const String16& value)
{
	_type = VariantType::String16;
	new PTR_VAR(String16, _value) String16(value);
}

SafeVariant::SafeVariant(const SafeString8& value)
{
	_type = VariantType::String8;
	new PTR_VAR(String8, _value) String8(value);
}

SafeVariant::SafeVariant(const SafeString16& value)
{
	_type = VariantType::String16;
	new PTR_VAR(String16, _value) String16(value);
}

SafeVariant::SafeVariant(const sl_char8* value)
{
	_type = VariantType::Sz8;
	REF_VAR(const sl_char8*, _value) = value;
}

SafeVariant::SafeVariant(const sl_char16* value)
{
	_type = VariantType::Sz16;
	REF_VAR(const sl_char16*, _value) = value;
}

SafeVariant::SafeVariant(const Time& value)
{
	_type = VariantType::Time;
	REF_VAR(Time, _value) = value;
}

SafeVariant::SafeVariant(const void* ptr)
{
	_type = VariantType::Pointer;
	REF_VAR(const void*, _value) = ptr;
}

SafeVariant::SafeVariant(const Memory& mem)
{
	_type = VariantType::Object;
	new PTR_VAR(Memory, _value) Memory(mem);
}

SafeVariant::SafeVariant(const SafeMemory& mem)
{
	_type = VariantType::Object;
	new PTR_VAR(Memory, _value) Memory(mem);
}

SafeVariant& SafeVariant::operator=(SafeVariant&& other)
{
	if (this != &other) {
		_replace(other._type, other._value);
		other._type = VariantType::Null;
	}
	return *this;
}

SafeVariant& SafeVariant::operator=(const SafeVariant& other)
{
	if (this != &other) {
		VariantType type;
		sl_uint64 value;
		other._retain(type, value);
		_replace(type, value);
	}
	return *this;
}

SafeVariant& SafeVariant::operator=(Variant&& other)
{
	if ((void*)this != (void*)(&other)) {
		_replace(other._type, other._value);
		other._type = VariantType::Null;
	}
	return *this;
}

SafeVariant& SafeVariant::operator=(const Variant& other)
{
	VariantType type = other._type;
	sl_uint64 value;
	_Variant_copy(type, other._value, value);
	_replace(type, value);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_int32 value)
{
	setInt32(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_uint32 value)
{
	setUint32(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_int64 value)
{
	setInt64(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_uint64 value)
{
	setUint64(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(float value)
{
	setFloat(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(double value)
{
	setDouble(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const sl_bool value)
{
	setBoolean(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const String8& value)
{
	setString(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const String16& value)
{
	setString(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const SafeString8& value)
{
	setString(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const SafeString16& value)
{
	setString(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const sl_char8* value)
{
	setString(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const sl_char16* value)
{
	setString(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const Time& value)
{
	setTime(value);
	return *this;
}

SafeVariant& SafeVariant::operator=(const void* ptr)
{
	setPointer(ptr);
	return *this;
}

SafeVariant& SafeVariant::operator=(const Memory& mem)
{
	setMemory(mem);
	return *this;
}

SafeVariant& SafeVariant::operator=(const SafeMemory& mem)
{
	setMemory(mem);
	return *this;
}

void SafeVariant::setNull()
{
	if (_type != VariantType::Null) {
		_replace(VariantType::Null, 0);
	}
}

sl_bool SafeVariant::isInt32() const
{
	return _type == VariantType::Int32;
}

sl_int32 SafeVariant::getInt32(sl_int32 def) const
{
	Variant var(*this);
	return var.getInt32(def);
}

void SafeVariant::setInt32(sl_int32 value)
{
	sl_int64 v;
	REF_VAR(sl_int32, v) = value;
	_replace(VariantType::Int32, v);
}

sl_bool SafeVariant::isUint32() const
{
	return _type == VariantType::Uint32;
}

sl_uint32 SafeVariant::getUint32(sl_uint32 def) const
{
	Variant var(*this);
	return var.getUint32(def);
}

void SafeVariant::setUint32(sl_uint32 value)
{
	sl_int64 v;
	REF_VAR(sl_uint32, v) = value;
	_replace(VariantType::Uint32, v);
}

sl_bool SafeVariant::isInt64() const
{
	return _type == VariantType::Int64;
}

sl_int64 SafeVariant::getInt64(sl_int64 def) const
{
	Variant var(*this);
	return var.getInt64(def);
}

void SafeVariant::setInt64(sl_int64 value)
{
	_replace(VariantType::Int64, value);
}

sl_bool SafeVariant::isUint64() const
{
	return _type == VariantType::Uint64;
}

sl_uint64 SafeVariant::getUint64(sl_uint64 def) const
{
	Variant var(*this);
	return var.getUint64(def);
}

void SafeVariant::setUint64(sl_uint64 value)
{
	_replace(VariantType::Uint64, value);
}

sl_bool SafeVariant::isInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Uint32 || _type == VariantType::Int64 || _type == VariantType::Uint64;
}

sl_bool SafeVariant::isSignedInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Int64;
}

sl_bool SafeVariant::isUnsignedInteger() const
{
	return _type == VariantType::Uint32 || _type == VariantType::Uint64;
}

sl_bool SafeVariant::isFloat() const
{
	return _type == VariantType::Float;
}

float SafeVariant::getFloat(float def) const
{
	Variant var(*this);
	return var.getFloat(def);
}

void SafeVariant::setFloat(float value)
{
	sl_int64 v;
	REF_VAR(float, v) = value;
	_replace(VariantType::Float, v);
}

sl_bool SafeVariant::isDouble() const
{
	return _type == VariantType::Double;
}

double SafeVariant::getDouble(double def) const
{
	Variant var(*this);
	return var.getDouble(def);
}

void SafeVariant::setDouble(double value)
{
	sl_int64 v;
	REF_VAR(double, v) = value;
	_replace(VariantType::Double, v);
}

sl_bool SafeVariant::isNumber() const
{
	return isInteger() || _type == VariantType::Float || _type == VariantType::Double;
}

sl_bool SafeVariant::isBoolean() const
{
	return _type == VariantType::Boolean;
}

sl_bool SafeVariant::getBoolean(sl_bool def) const
{
	Variant var(*this);
	return var.getBoolean(def);
}

void SafeVariant::setBoolean(sl_bool value)
{
	sl_int64 v;
	REF_VAR(sl_bool, v) = value;
	_replace(VariantType::Boolean, v);
}

sl_bool SafeVariant::isString() const
{
	return _type == VariantType::String8 || _type == VariantType::String16 || _type == VariantType::Sz8 || _type == VariantType::Sz16;
}

sl_bool SafeVariant::isString8() const
{
	return _type == VariantType::String8;
}

sl_bool SafeVariant::isString16() const
{
	return _type == VariantType::String16;
}

sl_bool SafeVariant::isSz8() const
{
	return _type == VariantType::Sz8;
}

sl_bool SafeVariant::isSz16() const
{
	return _type == VariantType::Sz16;
}

String SafeVariant::getString(const String& def) const
{
	Variant var(*this);
	return var.getString(def);
}

String SafeVariant::getString() const
{
	Variant var(*this);
	return var.getString();
}

String8 SafeVariant::getString8(const String8& def) const
{
	Variant var(*this);
	return var.getString8(def);
}

String8 SafeVariant::getString8() const
{
	Variant var(*this);
	return var.getString8();
}

String16 SafeVariant::getString16(const String16& def) const
{
	Variant var(*this);
	return var.getString16(def);
}

String16 SafeVariant::getString16() const
{
	Variant var(*this);
	return var.getString16();
}

const sl_char8* SafeVariant::getSz8(const sl_char8* def) const
{
	Variant var(*this);
	return var.getSz8(def);
}

const sl_char16* SafeVariant::getSz16(const sl_char16* def) const
{
	Variant var(*this);
	return var.getSz16(def);
}

void SafeVariant::setString(const String8& value)
{
	sl_int64 v;
	new PTR_VAR(String8, v) String8(value);
	_replace(VariantType::String8, v);
}

void SafeVariant::setString(const String16& value)
{
	sl_int64 v;
	new PTR_VAR(String16, v) String16(value);
	_replace(VariantType::String16, v);
}

void SafeVariant::setString(const SafeString8& value)
{
	sl_int64 v;
	new PTR_VAR(String8, v) String8(value);
	_replace(VariantType::String8, v);
}

void SafeVariant::setString(const SafeString16& value)
{
	sl_int64 v;
	new PTR_VAR(String16*, v) String16(value);
	_replace(VariantType::String16, v);
}

void SafeVariant::setString(const sl_char8* value)
{
	sl_int64 v;
	REF_VAR(const sl_char8*, v) = value;
	_replace(VariantType::Sz8, v);
}

void SafeVariant::setString(const sl_char16* value)
{
	sl_int64 v;
	REF_VAR(const sl_char16*, v) = value;
	_replace(VariantType::Sz16, v);
}

sl_bool SafeVariant::isTime() const
{
	return _type == VariantType::Time;
}

Time SafeVariant::getTime(Time def) const
{
	Variant var(*this);
	return var.getTime(def);
}

Time SafeVariant::getTime() const
{
	Variant var(*this);
	return var.getTime();
}

void SafeVariant::setTime(const Time& value)
{
	sl_int64 v;
	REF_VAR(Time, v) = value;
	_replace(VariantType::Time, v);
}

sl_bool SafeVariant::isPointer() const
{
	return _type == VariantType::Pointer || _type == VariantType::Sz8 || _type == VariantType::Sz16;
}

void* SafeVariant::getPointer(const void* def) const
{
	Variant var(*this);
	return var.getPointer(def);
}

void SafeVariant::setPointer(const void *ptr)
{
	sl_int64 v;
	REF_VAR(const void*, v) = ptr;
	_replace(VariantType::Pointer, v);
}

sl_bool SafeVariant::isObject() const
{
	return _type == VariantType::Object;
}

sl_bool SafeVariant::isWeak() const
{
	return _type == VariantType::Weak;
}

Ref<Referable> SafeVariant::getObject() const
{
	Variant var(*this);
	return var.getObject();
}

sl_bool SafeVariant::isObjectNotNull() const
{
	Variant var(*this);
	return var.isObjectNotNull();
}

sl_bool SafeVariant::isObjectNull() const
{
	Variant var(*this);
	return var.isObjectNull();
}

sl_class_type SafeVariant::getObjectClassType() const
{
	Variant var(*this);
	return var.getObjectClassType();
}

sl_bool SafeVariant::isMemory() const
{
	Variant var(*this);
	return var.isMemory();
}

Memory SafeVariant::getMemory() const
{
	Variant var(*this);
	return var.getMemory();
}

void SafeVariant::setMemory(const Memory& mem)
{
	sl_int64 v;
	new PTR_VAR(Memory, v) Memory(mem);
	_replace(VariantType::Object, v);
}

sl_bool SafeVariant::isVariantList() const
{
	Variant var(*this);
	return var.isVariantList();
}

List<Variant> SafeVariant::getVariantList() const
{
	Variant var(*this);
	return var.getVariantList();
}

sl_bool SafeVariant::isVariantMap() const
{
	Variant var(*this);
	return var.isVariantMap();
}

Map<String, Variant> SafeVariant::getVariantMap() const
{
	Variant var(*this);
	return var.getVariantMap();
}

sl_bool SafeVariant::isVariantMapList() const
{
	Variant var(*this);
	return var.isVariantMap();
}

List< Map<String, Variant> > SafeVariant::getVariantMapList() const
{
	Variant var(*this);
	return var.getVariantMapList();
}

sl_size SafeVariant::getListItemsCount() const
{
	Variant var(*this);
	return var.getListItemsCount();
}

Variant SafeVariant::getListItem(sl_size index) const
{
	Variant var(*this);
	return var.getListItem(index);
}

sl_bool SafeVariant::setListItem(sl_size index, const Variant& value)
{
	Variant var(*this);
	return var.setListItem(index, value);
}

sl_bool SafeVariant::addListItem(const Variant& value)
{
	Variant var(*this);
	return var.addListItem(value);
}

Variant SafeVariant::getField(const String& key) const
{
	Variant var(*this);
	return var.getField(key);
}

sl_bool SafeVariant::putField(const String& key, const Variant& value)
{
	Variant var(*this);
	return var.putField(key, value);
}

String SafeVariant::toString() const
{
	Variant var(*this);
	return var.toString();
}

String SafeVariant::toJsonString() const
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
				return REF_VAR(sl_int32, v1._value) == REF_VAR(sl_int32, v2._value);
			case VariantType::Float:
				return REF_VAR(float, v1._value) == REF_VAR(float, v2._value);
			case VariantType::Double:
				return REF_VAR(double, v1._value) == REF_VAR(double, v2._value);
			case VariantType::Boolean:
				return REF_VAR(sl_bool, v1._value) == REF_VAR(sl_bool, v2._value);
			case VariantType::Pointer:
			case VariantType::Sz8:
			case VariantType::Sz16:
			case VariantType::Object:
				return REF_VAR(const void*, v1._value) == REF_VAR(const void*, v2._value);
			case VariantType::String8:
				return REF_VAR(String8, v1._value) == REF_VAR(String8, v2._value);
			case VariantType::String16:
				return REF_VAR(String16, v1._value) == REF_VAR(String16, v2._value);
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
