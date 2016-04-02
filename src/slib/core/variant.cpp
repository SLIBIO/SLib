#include "../../../inc/slib/core/variant.h"

SLIB_NAMESPACE_BEGIN

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

SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Variant)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, Variant)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(sl_uint64, Variant)

const _Variant_Const _Variant_Null = {0, VariantType::Null, 0};


SLIB_INLINE void _Variant_copy(VariantType src_type, sl_uint64 src_value, sl_uint64& dst_value)
{
	switch (src_type) {
		case VariantType::String8:
			new ((String8*)(void*)(&dst_value)) String8(*(String8*)(void*)(&src_value));
			break;
		case VariantType::String16:
			new ((String16*)(void*)(&dst_value)) String16(*(String16*)(void*)(&src_value));
			break;
		case VariantType::Object:
		case VariantType::Weak:
			new ((Ref<Referable>*)(void*)(&dst_value)) Ref<Referable>(*(Ref<Referable>*)(void*)(&src_value));
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
			(*(String8*)(void*)(&value)).String8::~String8();
			break;
		case VariantType::String16:
			(*(String16*)(void*)(&value)).String16::~String16();
			break;
		case VariantType::Object:
		case VariantType::Weak:
			(*(Ref<Referable>*)(void*)(&value)).Ref<Referable>::~Ref();
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
	Variant& other = *((Variant*)((void*)(&_other)));
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
	*(sl_int32*)(void*)(&_value) = value;
}

Variant::Variant(sl_uint32 value)
{
	_type = VariantType::Uint32;
	*(sl_uint32*)(void*)(&_value) = value;
}

Variant::Variant(sl_int64 value)
{
	_type = VariantType::Int64;
	*(sl_int64*)(void*)(&_value) = value;
}

Variant::Variant(sl_uint64 value)
{
	_type = VariantType::Uint64;
	*(sl_uint64*)(void*)(&_value) = value;
}

Variant::Variant(float value)
{
	_type = VariantType::Float;
	*(float*)(void*)(&_value) = value;
}

Variant::Variant(double value)
{
	_type = VariantType::Double;
	*(double*)(void*)(&_value) = value;
}

Variant::Variant(const Time& value)
{
	_type = VariantType::Time;
	*(Time*)(void*)(&_value) = value;
}

Variant::Variant(const void* ptr)
{
	_type = VariantType::Pointer;
	*(const void**)(void*)(&_value) = ptr;
}

Variant::Variant(const sl_bool value)
{
	_type = VariantType::Boolean;
	*(sl_bool*)(void*)(&_value) = value;
}

Variant::Variant(const String8& value)
{
	_type = VariantType::String8;
	new ((String8*)(void*)(&_value)) String8(value);
}

Variant::Variant(const SafeString8& value)
{
	_type = VariantType::String8;
	new ((String8*)(void*)(&_value)) String8(value);
}

Variant::Variant(const String16& value)
{
	_type = VariantType::String16;
	new ((String16*)(void*)(&_value)) String16(value);
}

Variant::Variant(const SafeString16& value)
{
	_type = VariantType::String16;
	new ((String16*)(void*)(&_value)) String16(value);
}

Variant::Variant(const Memory& mem)
{
	_type = VariantType::Object;
	new ((Memory*)(void*)(&_value)) Memory(mem);
}

Variant::Variant(const SafeMemory& mem)
{
	_type = VariantType::Object;
	new ((Memory*)(void*)(&_value)) Memory(mem);
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

Variant Variant::fromTime(const Time& value)
{
	return value;
}

Variant Variant::fromPointer(const void* value)
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

Variant Variant::fromString(const String16& value)
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
	_Variant_free(_type, _value);
	_type = VariantType::Int32;
	*(sl_int32*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(sl_uint32 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Uint32;
	*(sl_uint32*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(sl_int64 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Int64;
	*(sl_int64*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(sl_uint64 value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Uint64;
	*(sl_uint64*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(float value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Float;
	*(float*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(double value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Double;
	*(double*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(const Time& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Time;
	*(Time*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(const void* ptr)
{
	_Variant_free(_type, _value);
	_type = VariantType::Pointer;
	*(const void**)(void*)(&_value) = ptr;
	return *this;
}

Variant& Variant::operator=(const sl_bool value)
{
	_Variant_free(_type, _value);
	_type = VariantType::Boolean;
	*(sl_bool*)(void*)(&_value) = value;
	return *this;
}

Variant& Variant::operator=(const String8& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String8;
	new ((String8*)(void*)(&_value)) String8(value);
	return *this;
}

Variant& Variant::operator=(const SafeString8& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String8;
	new ((String8*)(void*)(&_value)) String8(value);
	return *this;
}

Variant& Variant::operator=(const String16& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String16;
	new ((String16*)(void*)(&_value)) String16(value);
	return *this;
}

Variant& Variant::operator=(const SafeString16& value)
{
	_Variant_free(_type, _value);
	_type = VariantType::String16;
	new ((String16*)(void*)(&_value)) String16(value);
	return *this;
}

Variant& Variant::operator=(const Memory& mem)
{
	_Variant_free(_type, _value);
	_type = VariantType::Object;
	new ((Memory*)(void*)(&_value)) Memory(mem);
	return *this;
}

Variant& Variant::operator=(const SafeMemory& mem)
{
	_Variant_free(_type, _value);
	_type = VariantType::Object;
	new ((Memory*)(void*)(&_value)) Memory(mem);
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
			return (sl_int32)(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return (sl_int32)(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return (sl_int32)(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return (sl_int32)(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return (sl_int32)(*(float*)(void*)(&_value));
		case VariantType::Double:
			return (sl_int32)(*(double*)(void*)(&_value));
		case VariantType::String8:
			return (*(String8*)(void*)(&_value)).parseInt32(10, def);
		case VariantType::String16:
			return (*(String16*)(void*)(&_value)).parseInt32(10, def);
		default:
			break;
	}
	return def;
}

sl_bool Variant::isUint32() const
{
	return _type == VariantType::Uint32;
}

sl_uint32 Variant::getUint32(sl_uint32 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_uint32)(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return (sl_uint32)(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return (sl_uint32)(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return (sl_uint32)(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return (sl_uint32)(*(float*)(void*)(&_value));
		case VariantType::Double:
			return (sl_uint32)(*(double*)(void*)(&_value));
		case VariantType::String8:
			return (*(String8*)(void*)(&_value)).parseUint32(10, def);
		case VariantType::String16:
			return (*(String16*)(void*)(&_value)).parseUint32(10, def);
		default:
			break;
	}
	return def;
}

sl_bool Variant::isInt64() const
{
	return _type == VariantType::Int64;
}

sl_int64 Variant::getInt64(sl_int64 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_int64)(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return (sl_int64)(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return (sl_int64)(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return (sl_int64)(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return (sl_int64)(*(float*)(void*)(&_value));
		case VariantType::Double:
			return (sl_int64)(*(double*)(void*)(&_value));
		case VariantType::String8:
			return (*(String8*)(void*)(&_value)).parseInt64(10, def);
		case VariantType::String16:
			return (*(String16*)(void*)(&_value)).parseInt64(10, def);
		default:
			break;
	}
	return def;
}

sl_bool Variant::isUint64() const
{
	return _type == VariantType::Uint64;
}

sl_uint64 Variant::getUint64(sl_uint64 def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (sl_uint64)(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return (sl_uint64)(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return (sl_uint64)(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return (sl_uint64)(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return (sl_uint64)(*(float*)(void*)(&_value));
		case VariantType::Double:
			return (sl_uint64)(*(double*)(void*)(&_value));
		case VariantType::String8:
			return (*(String8*)(void*)(&_value)).parseUint64(10, def);
		case VariantType::String16:
			return (*(String16*)(void*)(&_value)).parseUint64(10, def);
		default:
			break;
	}
	return def;
}

sl_bool Variant::isInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Uint32 || _type == VariantType::Int64 || _type == VariantType::Uint64;
}

sl_bool Variant::isFloat() const
{
	return _type == VariantType::Float;
}

float Variant::getFloat(float def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (float)(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return (float)(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return (float)(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return (float)(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return (float)(*(float*)(void*)(&_value));
		case VariantType::Double:
			return (float)(*(double*)(void*)(&_value));
		case VariantType::String8:
			return (*(String8*)(void*)(&_value)).parseFloat(def);
		case VariantType::String16:
			return (*(String16*)(void*)(&_value)).parseFloat(def);
		default:
			break;
	}
	return def;
}

sl_bool Variant::isDouble() const
{
	return _type == VariantType::Double;
}

double Variant::getDouble(double def) const
{
	switch (_type) {
		case VariantType::Int32:
			return (double)(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return (double)(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return (double)(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return (double)(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return (double)(*(float*)(void*)(&_value));
		case VariantType::Double:
			return (double)(*(double*)(void*)(&_value));
		case VariantType::String8:
			return (*(String8*)(void*)(&_value)).parseDouble(def);
		case VariantType::String16:
			return (*(String16*)(void*)(&_value)).parseDouble(def);
		default:
			break;
	}
	return def;
}

sl_bool Variant::isNumber() const
{
	return isInteger() || _type == VariantType::Float || _type == VariantType::Double;
}

sl_bool Variant::isTime() const
{
	return _type == VariantType::Time;
}

Time Variant::getTime(Time def) const
{
	if (_type == VariantType::Time) {
		return *(Time*)(void*)(&_value);
	} else if (_type == VariantType::String8) {
		return Time::parse((*(String8*)(void*)(&_value)));
	} else if (_type == VariantType::String16) {
		return Time::parse((*(String16*)(void*)(&_value)));
	}
	return def;
}

Time Variant::getTime() const
{
	if (_type == VariantType::Time) {
		return *(Time*)(void*)(&_value);
	} else if (_type == VariantType::String8) {
		return Time::parse((*(String8*)(void*)(&_value)));
	} else if (_type == VariantType::String16) {
		return Time::parse((*(String16*)(void*)(&_value)));
	}
	return Time::zero();
}

sl_bool Variant::isPointer() const
{
	return _type == VariantType::Pointer;
}

void* Variant::getPointer(const void* def) const
{
	if (_type == VariantType::Pointer) {
		return *(void**)(void*)(&_value);
	}
	return (void*)def;
}

sl_bool Variant::isBoolean() const
{
	return _type == VariantType::Boolean;
}

sl_bool Variant::getBoolean(sl_bool def) const
{
	if (_type == VariantType::Boolean) {
		return *(sl_bool*)(void*)(&_value);
	} else if (_type == VariantType::String8) {
		String8 s((*(String8*)(void*)(&_value)).toLower());
		SLIB_STATIC_STRING8(_true, "true");
		SLIB_STATIC_STRING8(_false, "false");
		if (s == _true) {
			return sl_true;
		} else if (s == _false) {
			return sl_false;
		}
	} else if (_type == VariantType::String16) {
		String16 s((*(String16*)(void*)(&_value)).toLower());
		SLIB_STATIC_STRING16_BY_ARRAY(_true, 't', 'r', 'u', 'e');
		SLIB_STATIC_STRING16_BY_ARRAY(_false, 'f', 'a', 'l', 's', 'e');
		if (s == _true) {
			return sl_true;
		} else if (s == _false) {
			return sl_false;
		}
	}
	return def;
}

sl_bool Variant::isString() const
{
	return _type == VariantType::String8 || _type == VariantType::String16;
}

String Variant::getString(const String& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String::fromInt32(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return String::fromUint32(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return String::fromInt64(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return String::fromUint64(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return String::fromFloat(*(float*)(void*)(&_value));
		case VariantType::Double:
			return String::fromDouble(*(double*)(void*)(&_value));
		case VariantType::Boolean:
			if (*(sl_bool*)(void*)(&_value)) {
				SLIB_STATIC_STRING8_BY_ARRAY(ret, 't', 'r', 'u', 'e');
				return ret;
			} else {
				SLIB_STATIC_STRING8_BY_ARRAY(ret, 'f', 'a', 'l', 's', 'e');
				return ret;
			}
		case VariantType::Time:
			return (*(Time*)(void*)(&_value)).toString();
		case VariantType::String8:
			return (*(String8*)(void*)(&_value));
		case VariantType::String16:
			return (*(String16*)(void*)(&_value));
		default:
			break;
	}
	return def;
}

String Variant::getString() const
{
	return Variant::getString(String::null());
}

String8 Variant::getString8() const
{
	return Variant::getString8(String::null());
}

String Variant::getString8(const String& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String8::fromInt32(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return String8::fromUint32(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return String8::fromInt64(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return String8::fromUint64(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return String8::fromFloat(*(float*)(void*)(&_value));
		case VariantType::Double:
			return String8::fromDouble(*(double*)(void*)(&_value));
		case VariantType::Boolean:
			if (*(sl_bool*)(void*)(&_value)) {
				SLIB_STATIC_STRING8(ret, "true");
				return ret;
			} else {
				SLIB_STATIC_STRING8(ret, "false");
				return ret;
			}
		case VariantType::Time:
			return (*(Time*)(void*)(&_value)).toString();
		case VariantType::String8:
			return (*(String8*)(void*)(&_value));
		case VariantType::String16:
			return (*(String16*)(void*)(&_value));
		default:
			break;
	}
	return def;
}

sl_bool Variant::isString16() const
{
	return _type == VariantType::String16;
}

String16 Variant::getString16(const String16& def) const
{
	switch (_type) {
		case VariantType::Int32:
			return String16::fromInt32(*(sl_int32*)(void*)(&_value));
		case VariantType::Uint32:
			return String16::fromUint32(*(sl_uint32*)(void*)(&_value));
		case VariantType::Int64:
			return String16::fromInt64(*(sl_int64*)(void*)(&_value));
		case VariantType::Uint64:
			return String16::fromUint64(*(sl_uint64*)(void*)(&_value));
		case VariantType::Float:
			return String16::fromFloat(*(float*)(void*)(&_value));
		case VariantType::Double:
			return String16::fromDouble(*(double*)(void*)(&_value));
		case VariantType::Boolean:
			if (*(sl_bool*)(void*)(&_value)) {
				SLIB_STATIC_STRING16_BY_ARRAY(ret, 't', 'r', 'u', 'e');
				return ret;
			} else {
				SLIB_STATIC_STRING16_BY_ARRAY(ret, 'f', 'a', 'l', 's', 'e');
				return ret;
			}
		case VariantType::Time:
			return (*(Time*)(void*)(&_value)).toString();
		case VariantType::String8:
			return (*(String8*)(void*)(&_value));
		case VariantType::String16:
			return (*(String16*)(void*)(&_value));
		default:
			break;
	}
	return def;
}

String16 Variant::getString16() const
{
	return Variant::getString(String16::null());
}

sl_bool Variant::isObject() const
{
	return _type == VariantType::Object;
}

Ref<Referable> Variant::getObject() const
{
	if (_type == VariantType::Object) {
		return *((Ref<Referable>*)(void*)(&_value));
	}
	return Ref<Referable>::null();
}

sl_class_type Variant::getObjectClassType() const
{
	if (_type == VariantType::Object) {
		return (*((Referable**)(void*)(&_value)))->getClassType();
	}
	return 0;
}

sl_bool Variant::isObjectNotNull() const
{
	if (_type == VariantType::Object) {
		return (*((Ref<Referable>*)(void*)(&_value))).isNotNull();
	}
	return sl_false;
}

sl_bool Variant::isObjectNull() const
{
	if (_type == VariantType::Object) {
		return (*((Ref<Referable>*)(void*)(&_value))).isNull();
	}
	return sl_true;
}

sl_bool Variant::isMemory() const
{
	if (_type == VariantType::Object) {
		return Memory::checkInstance(*((Referable**)(void*)(&_value)));
	}
	return sl_false;
}

Memory Variant::getMemory() const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (Memory::checkInstance(ref)) {
			return (CMemory*)ref;
		}
	}
	return Memory::null();
}

sl_bool Variant::isWeak() const
{
	return _type == VariantType::Weak;
}

sl_bool Variant::isVariantList() const
{
	if (_type == VariantType::Object) {
		return List<Variant>::checkInstance(*((Referable**)(void*)(&_value)));
	}
	return sl_false;
}

List<Variant> Variant::getVariantList() const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (List<Variant>::checkInstance(ref)) {
			return (CList<Variant>*)ref;
		}
	}
	return List<Variant>::null();
}

sl_bool Variant::isVariantMap() const
{
	if (_type == VariantType::Object) {
		return Map<String, Variant>::checkInstance(*((Referable**)(void*)(&_value)));
	}
	return sl_false;
}

Map<String, Variant> Variant::getVariantMap() const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (Map<String, Variant>::checkInstance(ref)) {
			return (IMap<String, Variant>*)ref;
		}
	}
	return Map<String, Variant>::null();
}

sl_size Variant::getListItemsCount() const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (List<Variant>::checkInstance(ref)) {
			return ((CList<Variant>*)ref)->getCount();
		}
	}
	return 0;
	
}

Variant Variant::getListItem(sl_size index) const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (List<Variant>::checkInstance(ref)) {
			return ((CList<Variant>*)ref)->getItemValue(index, Variant::null());
		}
	}
	return Variant::null();
}

sl_bool Variant::setListItem(sl_size index, const Variant& value)
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (List<Variant>::checkInstance(ref)) {
			return ((CList<Variant>*)ref)->setItem(index, value);
		}
	}
	return sl_false;
}

sl_bool Variant::addListItem(const Variant& value)
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (List<Variant>::checkInstance(ref)) {
			return ((CList<Variant>*)ref)->add(value);
		}
	}
	return sl_false;
}

Variant Variant::getField(const String& key) const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (Map<String, Variant>::checkInstance(ref)) {
			return ((IMap<String, Variant>*)ref)->getValue(key, Variant::null());
		}
	}
	return Variant::null();
}

sl_bool Variant::putField(const String& key, const Variant& value)
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		if (Map<String, Variant>::checkInstance(ref)) {
			return ((IMap<String, Variant>*)ref)->put(key, value);
		}
	}
	return sl_false;
}


#define _MAX_VAR_STRING_LEN 20000
static sl_bool _Variant_getVariantMapJsonString(StringBuffer& ret, const Map<String, Variant>& map, sl_bool flagJSON);
static sl_bool _Variant_getVariantListJsonString(StringBuffer& ret, const List<Variant>& list, sl_bool flagJSON)
{
	ListLocker<Variant> l(list);
	sl_size n = l.count;
	Variant* lb = l.data;
	
	if (!(ret.addStatic("[", 1))) {
		return sl_false;
	}
	for (sl_size i = 0; i < n; i++)
	{
		String valueText;
		Variant& v = lb[i];
		if (!v.isVariantList() && !v.isVariantMap()) {
			if (flagJSON) {
				valueText = v.toJson();
				if (valueText.isEmpty()) {
					SLIB_STATIC_STRING(_null, "null");
					valueText = _null;
				}
			} else {
				valueText = v.toString();
			}
		}
		if (i) {
			if (!(ret.addStatic(", ", 2))) {
				return sl_false;
			}
		}
		if (v.isVariantList()) {
			if (!_Variant_getVariantListJsonString(ret, v.getVariantList(), flagJSON)) {
				return sl_false;
			}
		} else if (v.isVariantMap()) {
			if (!_Variant_getVariantMapJsonString(ret, v.getVariantMap(), flagJSON)) {
				return sl_false;
			}
		} else {
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

static sl_bool _Variant_getVariantMapJsonString(StringBuffer& ret, const Map<String, Variant>& map, sl_bool flagJSON)
{
	Iterator< Pair<String, Variant> > iterator(map.iterator());
	if (!(ret.addStatic("{", 1))) {
		return sl_false;
	}
	sl_bool flagFirst = sl_true;
	Pair<String, Variant> pair;
	while (iterator.next(&pair)) {
		String valueText;
		Variant& v = pair.value;
		if (!v.isVariantList() && !v.isVariantMap()) {
			if (flagJSON) {
				valueText = v.toJson();
				if (valueText.isEmpty()) {
					SLIB_STATIC_STRING(_null, "null");
					valueText = _null;
				}
			} else {
				valueText = v.toString();
			}
		}
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
		if (v.isVariantList()) {
			if (!_Variant_getVariantListJsonString(ret, v.getVariantList(), flagJSON)) {
				return sl_false;
			}
		} else if (v.isVariantMap()) {
			if (!_Variant_getVariantMapJsonString(ret, v.getVariantMap(), flagJSON)) {
				return sl_false;
			}
		} else {
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
		case VariantType::Time:
			return "\"" + (*(Time*)(void*)(&_value)).toString() + "\"";
		case VariantType::String8:
			return "\"" + (*(String8*)(void*)(&_value)) + "\"";
		case VariantType::String16:
			return "\"" + (*(String16*)(void*)(&_value)) + "\"";
		case VariantType::Pointer:
			return "#" + String::fromPointerValue(*(const void**)(void*)(&_value));
		case VariantType::Object:
			if (isVariantList()) {
				StringBuffer ret;
				if (!_Variant_getVariantListJsonString(ret, getVariantList(), sl_false)) {
					ret.addStatic(" ...", 4);
				}
				return ret.merge();
			} else if (isVariantMap()) {
				StringBuffer ret;
				if (!_Variant_getVariantMapJsonString(ret, getVariantMap(), sl_false)) {
					ret.addStatic(" ...", 4);
				}
				return ret.merge();
			} else {
				return "<object:" + String::fromUtf8(getObjectClassType()) + ">";
			}
		case VariantType::Weak:
			return "<weak-ref>";
		default:
			return "<error-type>";
	}
}

String Variant::toJson() const
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
			return getString(String8::null()).applyBackslashEscapes();
		case VariantType::String16:
			return getString16(String16::null()).applyBackslashEscapes();
		case VariantType::Object:
			if (isVariantList())
			{
				StringBuffer ret;
				if (!_Variant_getVariantListJsonString(ret, getVariantList(), sl_true)) {
					return String::null();
				}
				return ret.merge();
			} else if (isVariantMap()) {
				StringBuffer ret;
				if (!_Variant_getVariantMapJsonString(ret, getVariantMap(), sl_true)) {
					return String::null();
				}
				return ret.merge();
			}
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
	*(sl_int32*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(sl_uint32 value)
{
	_type = VariantType::Uint32;
	*(sl_uint32*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(sl_int64 value)
{
	_type = VariantType::Int64;
	*(sl_int64*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(sl_uint64 value)
{
	_type = VariantType::Uint64;
	*(sl_uint64*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(float value)
{
	_type = VariantType::Float;
	*(float*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(double value)
{
	_type = VariantType::Double;
	*(double*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(const Time& value)
{
	_type = VariantType::Time;
	*(Time*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(const void* ptr)
{
	_type = VariantType::Pointer;
	*(const void**)(void*)(&_value) = ptr;
}

SafeVariant::SafeVariant(const sl_bool value)
{
	_type = VariantType::Boolean;
	*(sl_bool*)(void*)(&_value) = value;
}

SafeVariant::SafeVariant(const String8& value)
{
	_type = VariantType::String8;
	new ((String8*)(void*)(&_value)) String8(value);
}

SafeVariant::SafeVariant(const SafeString8& value)
{
	_type = VariantType::String8;
	new ((String8*)(void*)(&_value)) String8(value);
}

SafeVariant::SafeVariant(const String16& value)
{
	_type = VariantType::String16;
	new ((String16*)(void*)(&_value)) String16(value);
}

SafeVariant::SafeVariant(const SafeString16& value)
{
	_type = VariantType::String16;
	new ((String16*)(void*)(&_value)) String16(value);
}

SafeVariant::SafeVariant(const Memory& mem)
{
	_type = VariantType::Object;
	new ((Memory*)(void*)(&_value)) Memory(mem);
}

SafeVariant::SafeVariant(const SafeMemory& mem)
{
	_type = VariantType::Object;
	new ((Memory*)(void*)(&_value)) Memory(mem);
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
	sl_int64 v;
	*(sl_int32*)(void*)(&v) = value;
	_replace(VariantType::Int32, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_uint32 value)
{
	sl_int64 v;
	*(sl_uint32*)(void*)(&v) = value;
	_replace(VariantType::Uint32, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_int64 value)
{
	_replace(VariantType::Int64, value);
	return *this;
}

SafeVariant& SafeVariant::operator=(sl_uint64 value)
{
	_replace(VariantType::Uint64, value);
	return *this;
}

SafeVariant& SafeVariant::operator=(float value)
{
	sl_int64 v;
	*(float*)(void*)(&v) = value;
	_replace(VariantType::Float, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(double value)
{
	sl_int64 v;
	*(double*)(void*)(&v) = value;
	_replace(VariantType::Double, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(const Time& value)
{
	sl_int64 v;
	*(Time*)(void*)(&v) = value;
	_replace(VariantType::Time, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(const void* ptr)
{
	sl_int64 v;
	*(const void**)(void*)(&v) = ptr;
	_replace(VariantType::Pointer, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(const sl_bool value)
{
	sl_int64 v;
	*(sl_bool*)(void*)(&v) = value;
	_replace(VariantType::Boolean, v);
	return *this;
}

SafeVariant& SafeVariant::operator=(const String8& value)
{
	return *this = Variant(value);
}

SafeVariant& SafeVariant::operator=(const SafeString8& value)
{
	return *this = Variant(value);
}

SafeVariant& SafeVariant::operator=(const String16& value)
{
	return *this = Variant(value);
}

SafeVariant& SafeVariant::operator=(const SafeString16& value)
{
	return *this = Variant(value);
}

SafeVariant& SafeVariant::operator=(const Memory& mem)
{
	return *this = Variant(mem);
}

SafeVariant& SafeVariant::operator=(const SafeMemory& mem)
{
	return *this = Variant(mem);
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

sl_bool SafeVariant::isUint32() const
{
	return _type == VariantType::Uint32;
}

sl_uint32 SafeVariant::getUint32(sl_uint32 def) const
{
	Variant var(*this);
	return var.getUint32(def);
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

sl_bool SafeVariant::isUint64() const
{
	return _type == VariantType::Uint64;
}

sl_uint64 SafeVariant::getUint64(sl_uint64 def) const
{
	Variant var(*this);
	return var.getUint64(def);
}

sl_bool SafeVariant::isInteger() const
{
	return _type == VariantType::Int32 || _type == VariantType::Uint32 || _type == VariantType::Int64 || _type == VariantType::Uint64;
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

sl_bool SafeVariant::isDouble() const
{
	return _type == VariantType::Double;
}

double SafeVariant::getDouble(double def) const
{
	Variant var(*this);
	return var.getDouble(def);
}

sl_bool SafeVariant::isNumber() const
{
	return isInteger() || _type == VariantType::Float || _type == VariantType::Double;
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

sl_bool SafeVariant::isPointer() const
{
	return _type == VariantType::Pointer;
}

void* SafeVariant::getPointer(const void* def) const
{
	Variant var(*this);
	return var.getPointer(def);
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

sl_bool SafeVariant::isString() const
{
	return _type == VariantType::String8 || _type == VariantType::String16;
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

sl_bool SafeVariant::isString8() const
{
	return _type == VariantType::String8;
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

sl_bool SafeVariant::isString16() const
{
	return _type == VariantType::String16;
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

sl_bool SafeVariant::isObject() const
{
	return _type == VariantType::Object;
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

sl_bool SafeVariant::isWeak() const
{
	return _type == VariantType::Weak;
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

String SafeVariant::toJson() const
{
	Variant var(*this);
	return var.toJson();
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
				return *(sl_int32*)(void*)(&(v1._value)) == *(sl_int32*)(void*)(&(v2._value));
			case VariantType::Float:
				return *(float*)(void*)(&(v1._value)) == *(float*)(void*)(&(v2._value));
			case VariantType::Double:
				return *(double*)(void*)(&(v1._value)) == *(double*)(void*)(&(v2._value));
			case VariantType::Boolean:
				return *(sl_bool*)(void*)(&(v1._value)) == *(sl_bool*)(void*)(&(v2._value));
			case VariantType::Pointer:
				return *(void**)(void*)(&(v1._value)) == *(void**)(void*)(&(v2._value));
			case VariantType::String8:
				return *(String8*)(void*)(&(v1._value)) == *(String8*)(void*)(&(v2._value));
			case VariantType::String16:
				return *(String16*)(void*)(&(v1._value)) == *(String16*)(void*)(&(v2._value));
			default:
				break;
		}
	}
	return sl_false;
}

sl_bool operator!=(const Variant& v1, const Variant& v2)
{
	VariantType type = v1._type;
	if (type == v2._type) {
		if (v1._value == v2._value) {
			return sl_false;
		}
		switch (type) {
			case VariantType::Null:
				return sl_false;
			case VariantType::Int32:
			case VariantType::Uint32:
				return *(sl_int32*)(void*)(&(v1._value)) != *(sl_int32*)(void*)(&(v2._value));
			case VariantType::Float:
				return *(float*)(void*)(&(v1._value)) != *(float*)(void*)(&(v2._value));
			case VariantType::Double:
				return *(double*)(void*)(&(v1._value)) != *(double*)(void*)(&(v2._value));
			case VariantType::Boolean:
				return *(sl_bool*)(void*)(&(v1._value)) != *(sl_bool*)(void*)(&(v2._value));
			case VariantType::Pointer:
				return *(void**)(void*)(&(v1._value)) != *(void**)(void*)(&(v2._value));
			case VariantType::String8:
				return *(String8*)(void*)(&(v1._value)) != *(String8*)(void*)(&(v2._value));
			case VariantType::String16:
				return *(String16*)(void*)(&(v1._value)) != *(String16*)(void*)(&(v2._value));
			default:
				break;
		}
	}
	return sl_true;
}

SLIB_NAMESPACE_END
