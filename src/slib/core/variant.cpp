#include "../../../inc/slib/core/variant.h"

SLIB_NAMESPACE_BEGIN

const _Variant_Const _Variant_Null = {0, Variant::typeNull, 0};

void Variant::_copy(Type src_type, sl_uint64 src_value, sl_uint64& dst_value)
{
	switch (src_type) {
		case typeString8:
			new ((String8*)(void*)(&dst_value)) String8(*(String8*)(void*)(&src_value));
			break;
		case typeString16:
			new ((String16*)(void*)(&dst_value)) String16(*(String16*)(void*)(&src_value));
			break;
		case typeObject:
		case typeWeak:
			new ((Ref<Referable>*)(void*)(&dst_value)) Ref<Referable>(*(Ref<Referable>*)(void*)(&src_value));
			break;
		default:
			dst_value = src_value;
			break;
	}
}

void Variant::_free(Type type, sl_uint64 value)
{
	switch (type)
	{
		case typeString8:
			(*(String8*)(void*)(&value)).String8::~String8();
			break;
		case typeString16:
			(*(String16*)(void*)(&value)).String16::~String16();
			break;
		case typeObject:
		case typeWeak:
			(*(Ref<Referable>*)(void*)(&value)).Ref<Referable>::~Ref();
			break;
		default:
			break;
	}
}

Variant::Variant(const Variant& other)
{
	m_type = other.m_type;
	_copy(m_type, other.m_value, m_value);
}

Variant::Variant(const SafeVariant& other)
{
	other._retain(m_type, m_value);
}

Variant::~Variant()
{
	_free(m_type, m_value);
}

Variant& Variant::operator=(const Variant& other)
{
	if (this != &other) {
		_free(m_type, m_value);
		m_type = other.m_type;
		_copy(m_type, other.m_value, m_value);
	}
	return *this;
}

Variant& Variant::operator=(const SafeVariant& other)
{
	_free(m_type, m_value);
	other._retain(m_type, m_value);
	return *this;
}

sl_bool operator==(const Variant& v1, const Variant& v2)
{
	Variant::Type type = v1.m_type;
	if (type == v2.m_type) {
		if (v1.m_value == v2.m_value) {
			return sl_true;
		}
		switch (type) {
			case Variant::typeNull:
				return sl_true;
			case Variant::typeInt32:
			case Variant::typeUint32:
				return *(sl_int32*)(void*)(&(v1.m_value)) == *(sl_int32*)(void*)(&(v2.m_value));
			case Variant::typeFloat:
				return *(float*)(void*)(&(v1.m_value)) == *(float*)(void*)(&(v2.m_value));
			case Variant::typeDouble:
				return *(double*)(void*)(&(v1.m_value)) == *(double*)(void*)(&(v2.m_value));
			case Variant::typeBoolean:
				return *(sl_bool*)(void*)(&(v1.m_value)) == *(sl_bool*)(void*)(&(v2.m_value));
			case Variant::typePointer:
				return *(void**)(void*)(&(v1.m_value)) == *(void**)(void*)(&(v2.m_value));
			case Variant::typeString8:
				return *(String8*)(void*)(&(v1.m_value)) == *(String8*)(void*)(&(v2.m_value));
			case Variant::typeString16:
				return *(String16*)(void*)(&(v1.m_value)) == *(String16*)(void*)(&(v2.m_value));
			default:
				break;
		}
	}
	return sl_false;
}

void SafeVariant::_retain(Variant::Type& type, sl_uint64& value) const
{
	if ((void*)(this) == (void*)(&_Variant_Null)) {
		type = Variant::typeNull;
	} else {
		SpinLocker lock(&m_lock);
		type = m_type;
		Variant::_copy(m_type, m_value, value);
	}
}

void SafeVariant::_replace(Variant::Type type, sl_uint64 value)
{
	Variant::Type typeOld;
	sl_uint64 valueOld;
	{
		SpinLocker lock(&m_lock);
		typeOld = m_type;
		valueOld = m_value;
		m_type = type;
		m_value = value;
	}
	Variant::_free(typeOld, valueOld);
}

SafeVariant::SafeVariant(const SafeVariant& other)
{
	other._retain(m_type, m_value);
}

SafeVariant::SafeVariant(const Variant& other)
{
	m_type = other.m_type;
	Variant::_copy(m_type, other.m_value, m_value);
}

SafeVariant::~SafeVariant()
{
	Variant::_free(m_type, m_value);
}

SafeVariant& SafeVariant::operator=(const SafeVariant& other)
{
	if (this != &other) {
		Variant::Type type;
		sl_uint64 value;
		other._retain(type, value);
		_replace(type, value);
	}
	return *this;
}

SafeVariant& SafeVariant::operator=(const Variant& other)
{
	Variant::Type type = other.m_type;
	sl_uint64 value;
	Variant::_copy(type, other.m_value, value);
	_replace(type, value);
	return *this;
}

sl_int32 Variant::getInt32(sl_int32 def) const
{
	switch (m_type) {
		case typeInt32:
			return (sl_int32)(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return (sl_int32)(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return (sl_int32)(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return (sl_int32)(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return (sl_int32)(*(float*)(void*)(&m_value));
		case typeDouble:
			return (sl_int32)(*(double*)(void*)(&m_value));
		case typeString8:
			return (*(String8*)(void*)(&m_value)).parseInt32(10, def);
		case typeString16:
			return (*(String16*)(void*)(&m_value)).parseInt32(10, def);
		default:
			break;
	}
	return def;
}

sl_uint32 Variant::getUint32(sl_uint32 def) const
{
	switch (m_type) {
		case typeInt32:
			return (sl_uint32)(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return (sl_uint32)(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return (sl_uint32)(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return (sl_uint32)(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return (sl_uint32)(*(float*)(void*)(&m_value));
		case typeDouble:
			return (sl_uint32)(*(double*)(void*)(&m_value));
		case typeString8:
			return (*(String8*)(void*)(&m_value)).parseUint32(10, def);
		case typeString16:
			return (*(String16*)(void*)(&m_value)).parseUint32(10, def);
		default:
			break;
	}
	return def;
}

sl_int64 Variant::getInt64(sl_int64 def) const
{
	switch (m_type) {
		case typeInt32:
			return (sl_int64)(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return (sl_int64)(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return (sl_int64)(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return (sl_int64)(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return (sl_int64)(*(float*)(void*)(&m_value));
		case typeDouble:
			return (sl_int64)(*(double*)(void*)(&m_value));
		case typeString8:
			return (*(String8*)(void*)(&m_value)).parseInt64(10, def);
		case typeString16:
			return (*(String16*)(void*)(&m_value)).parseInt64(10, def);
		default:
			break;
	}
	return def;
}

sl_uint64 Variant::getUint64(sl_uint64 def) const
{
	switch (m_type) {
		case typeInt32:
			return (sl_uint64)(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return (sl_uint64)(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return (sl_uint64)(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return (sl_uint64)(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return (sl_uint64)(*(float*)(void*)(&m_value));
		case typeDouble:
			return (sl_uint64)(*(double*)(void*)(&m_value));
		case typeString8:
			return (*(String8*)(void*)(&m_value)).parseUint64(10, def);
		case typeString16:
			return (*(String16*)(void*)(&m_value)).parseUint64(10, def);
		default:
			break;
	}
	return def;
}

float Variant::getFloat(float def) const
{
	switch (m_type) {
		case typeInt32:
			return (float)(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return (float)(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return (float)(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return (float)(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return (float)(*(float*)(void*)(&m_value));
		case typeDouble:
			return (float)(*(double*)(void*)(&m_value));
		case typeString8:
			return (*(String8*)(void*)(&m_value)).parseFloat(def);
		case typeString16:
			return (*(String16*)(void*)(&m_value)).parseFloat(def);
		default:
			break;
	}
	return def;
}

double Variant::getDouble(double def) const
{
	switch (m_type) {
		case typeInt32:
			return (double)(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return (double)(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return (double)(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return (double)(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return (double)(*(float*)(void*)(&m_value));
		case typeDouble:
			return (double)(*(double*)(void*)(&m_value));
		case typeString8:
			return (*(String8*)(void*)(&m_value)).parseDouble(def);
		case typeString16:
			return (*(String16*)(void*)(&m_value)).parseDouble(def);
		default:
			break;
	}
	return def;
}

Time Variant::getTime(Time def) const
{
	if (m_type == typeTime) {
		return *(Time*)(void*)(&m_value);
	} else if (m_type == typeString8) {
		return Time::parse((*(String8*)(void*)(&m_value)));
	} else if (m_type == typeString16) {
		return Time::parse((*(String16*)(void*)(&m_value)));
	}
	return def;
}

const void* Variant::getPointer(const void* def) const
{
	if (m_type == typePointer) {
		return *(const void**)(void*)(&m_value);
	}
	return def;
}

sl_bool Variant::getBoolean(sl_bool def) const
{
	if (m_type == typeBoolean) {
		return *(sl_bool*)(void*)(&m_value);
	} else if (m_type == typeString8) {
		String8 s((*(String8*)(void*)(&m_value)).toLower());
		SLIB_STATIC_STRING8(_true, "true");
		SLIB_STATIC_STRING8(_false, "false");
		if (s == _true) {
			return sl_true;
		} else if (s == _false) {
			return sl_false;
		}
	} else if (m_type == typeString16) {
		String16 s((*(String16*)(void*)(&m_value)).toLower());
		SLIB_STATIC_STRING16(_true, "true");
		SLIB_STATIC_STRING16(_false, "false");
		if (s == _true) {
			return sl_true;
		} else if (s == _false) {
			return sl_false;
		}
	}
	return def;
}

String Variant::getString(const String& def) const
{
	switch (m_type) {
		case typeInt32:
			return String::fromInt32(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return String::fromUint32(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return String::fromInt64(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return String::fromUint64(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return String::fromFloat(*(float*)(void*)(&m_value));
		case typeDouble:
			return String::fromDouble(*(double*)(void*)(&m_value));
		case typeBoolean:
			if (*(sl_bool*)(void*)(&m_value)) {
				SLIB_STATIC_STRING(ret, "true");
				return ret;
			} else {
				SLIB_STATIC_STRING(ret, "false");
				return ret;
			}
		case typeTime:
			return (*(Time*)(void*)(&m_value)).toString();
		case typeString8:
			return (*(String8*)(void*)(&m_value));
		case typeString16:
			return (*(String16*)(void*)(&m_value));
		default:
			break;
	}
	return def;
}

String16 Variant::getString16(const String16& def) const
{
	switch (m_type) {
		case typeInt32:
			return String16::fromInt32(*(sl_int32*)(void*)(&m_value));
		case typeUint32:
			return String16::fromUint32(*(sl_uint32*)(void*)(&m_value));
		case typeInt64:
			return String16::fromInt64(*(sl_int64*)(void*)(&m_value));
		case typeUint64:
			return String16::fromUint64(*(sl_uint64*)(void*)(&m_value));
		case typeFloat:
			return String16::fromFloat(*(float*)(void*)(&m_value));
		case typeDouble:
			return String16::fromDouble(*(double*)(void*)(&m_value));
		case typeBoolean:
			if (*(sl_bool*)(void*)(&m_value)) {
				SLIB_STATIC_STRING16(ret, "true");
				return ret;
			} else {
				SLIB_STATIC_STRING16(ret, "false");
				return ret;
			}
		case typeTime:
			return (*(Time*)(void*)(&m_value)).toString();
		case typeString8:
			return (*(String8*)(void*)(&m_value));
		case typeString16:
			return (*(String16*)(void*)(&m_value));
		default:
			break;
	}
	return def;
}

#define _MAX_VAR_STRING_LEN 20000
static sl_bool _Variant_getVariantMapJSONString(StringBuffer& ret, const Map<String, Variant>& map, sl_bool flagJSON);
static sl_bool _Variant_getVariantListJSONString(StringBuffer& ret, const List<Variant>& list, sl_bool flagJSON)
{
	ListLocker<Variant> l(list);
	sl_size n = l.getCount();
	Variant* lb = l.data();

	ret.add("[");
	for (sl_size i = 0; i < n; i++)
	{
		String valueText;
		Variant& v = lb[i];
		if (!v.isVariantList() && !v.isVariantMap()) {
			if (flagJSON) {
				valueText = v.toJSON();
				if (valueText.isEmpty()) {
					SLIB_STATIC_STRING(_null, "null");
					valueText = _null;
				}
			} else {
				valueText = v.toString();
			}
		}
		if (i) {
			ret.add(", ");
		}
		if (v.isVariantList()) {
			if (!_Variant_getVariantListJSONString(ret, v.getVariantList(), flagJSON)) {
				return sl_false;
			}
		} else if (v.isVariantMap()) {
			if (!_Variant_getVariantMapJSONString(ret, v.getVariantMap(), flagJSON)) {
				return sl_false;
			}
		} else {
			ret.add(valueText);
		}
		if (!flagJSON) {
			if (ret.getLength() > _MAX_VAR_STRING_LEN) {
				return sl_false;
			}
		}
	}
	ret.add("]");
	return sl_true;
}

static sl_bool _Variant_getVariantMapJSONString(StringBuffer& ret, const Map<String, Variant>& map, sl_bool flagJSON)
{
	Iterator< Pair<String, Variant> > iterator(map.iterator());
	ret.add("{");
	sl_bool flagFirst = sl_true;
	Pair<String, Variant> pair;
	while (iterator.next(&pair)) {
		String valueText;
		Variant& v = pair.value;
		if (!v.isVariantList() && !v.isVariantMap()) {
			if (flagJSON) {
				valueText = v.toJSON();
				if (valueText.isEmpty()) {
					SLIB_STATIC_STRING(_null, "null");
					valueText = _null;
				}
			} else {
				valueText = v.toString();
			}
		}
		if (!flagFirst) {
			ret.add(", ");
		}
		if (flagJSON) {
			ret.add(pair.key.applyBackslashEscapes());
		} else {
			ret.add(pair.key);
		}
		ret.add(": ");
		if (v.isVariantList()) {
			if (!_Variant_getVariantListJSONString(ret, v.getVariantList(), flagJSON)) {
				return sl_false;
			}
		} else if (v.isVariantMap()) {
			if (!_Variant_getVariantMapJSONString(ret, v.getVariantMap(), flagJSON)) {
				return sl_false;
			}
		} else {
			ret.add(valueText);
		}
		if (!flagJSON) {
			if (ret.getLength() > _MAX_VAR_STRING_LEN) {
				return sl_false;
			}
		}
		flagFirst = sl_false;
	}
	ret.add("}");
	return sl_true;
}

String Variant::toString() const
{
	switch (m_type) {
		case typeNull:
			return "<null>";
		case typeInt32:
		case typeUint32:
		case typeInt64:
		case typeUint64:
		case typeFloat:
		case typeDouble:
		case typeBoolean:
			return getString(String::null());
		case typeTime:
			return "\"" + (*(Time*)(void*)(&m_value)).toString() + "\"";
		case typeString8:
			return "\"" + (*(String8*)(void*)(&m_value)) + "\"";
		case typeString16:
			return "\"" + (*(String16*)(void*)(&m_value)) + "\"";
		case typePointer:
			return "#" + String::fromPointerValue(*(const void**)(void*)(&m_value));
		case typeObject:
			if (isVariantList()) {
				StringBuffer ret;
				if (!_Variant_getVariantListJSONString(ret, getVariantList(), sl_false)) {
					ret.add(" ...");
				}
				return ret.merge();
			} else if (isVariantMap()) {
				StringBuffer ret;
				if (!_Variant_getVariantMapJSONString(ret, getVariantMap(), sl_false)) {
					ret.add(" ...");
				}
				return ret.merge();
			} else {
				return "<object:" + String::fromUtf8(getObjectClassTypeName()) + ">";
			}
		case typeWeak:
			return "<weak-ref>";
		default:
			return "<error-type>";
	}
}

String Variant::toJSON() const
{
	switch (m_type) {
		case typeNull:
			return String::null();
		case typeInt32:
		case typeUint32:
		case typeInt64:
		case typeUint64:
		case typeFloat:
		case typeDouble:
		case typeBoolean:
			return getString(String::null());
		case typeTime:
		case typeString8:
			return getString(String8::null()).applyBackslashEscapes();
		case typeString16:
			return getString16(String16::null()).applyBackslashEscapes();
		case typeObject:
			if (isVariantList())
			{
				StringBuffer ret;
				if (!_Variant_getVariantListJSONString(ret, getVariantList(), sl_true)) {
					return String::null();
				}
				return ret.merge();
			} else if (isVariantMap()) {
				StringBuffer ret;
				if (!_Variant_getVariantMapJSONString(ret, getVariantMap(), sl_true)) {
					return String::null();
				}
				return ret.merge();
			}
		default:
			return String::null();
	}
}

SLIB_NAMESPACE_END
