#include "../../../inc/slib/core/variant.h"

SLIB_NAMESPACE_BEGIN

sl_uint32 _Variant_null[4] = { Variant::typeEmpty, 0, 0, 0 };

Variant::Variant(const Variant& other)
{
	if ((void*)(&other) == (void*)(&_Variant_null)) {
		m_type = typeEmpty;
	} else {
		SpinLocker lock(SpinLockPoolForVariant::get(&other));
		m_type = other.m_type;
		switch (m_type) {
			case typeEmpty:
				break;
			case typeString8:
				new ((String8*)(void*)(&m_value)) String8(*(String8*)(void*)(&(other.m_value)));
				break;
			case typeString16:
				new ((String16*)(void*)(&m_value)) String16(*(String16*)(void*)(&(other.m_value)));
				break;
			case typeObject:
			case typeWeak:
				new ((Ref<Referable>*)(void*)(&m_value)) Ref<Referable>(*(Ref<Referable>*)(void*)(&(other.m_value)));
				break;
			default:
				m_value = other.m_value;
				break;
		}
	}

}

Variant::~Variant()
{
	switch (m_type)
	{
	case typeString8:
		(*(String8*)(void*)(&m_value)).String8::~String8();
		break;
	case typeString16:
		(*(String16*)(void*)(&m_value)).String16::~String16();
		break;
	case typeObject:
	case typeWeak:
		(*(Ref<Referable>*)(void*)(&m_value)).Ref<Referable>::~Ref();
		break;
	default:
		break;
	}
}

Variant& Variant::operator=(const Variant& other)
{
	if (this == &other) {
		return *this;
	}
	
	sl_uint64 valueFree;
	Type typeFree;
	
	if ((void*)(&other) == (void*)(&_Variant_null)) {
		SpinLock* l = SpinLockPoolForVariant::get(this);
		SpinLocker lock(l);
		valueFree = m_value;
		typeFree = m_type;
		m_type = typeEmpty;
	} else {
		SpinLock* l1 = SpinLockPoolForVariant::get(this);
		SpinLock* l2 = SpinLockPoolForVariant::get(&other);
		if (l2 < l1) {
			SpinLock* t = l2;
			l2 = l1;
			l1 = t;
		} else if (l2 == l1) {
			l2 = sl_null;
		}
		SpinLocker lock1(l1);
		SpinLocker lock2(l2);
		valueFree = m_value;
		typeFree = m_type;
		m_type = other.m_type;
		switch (other.m_type) {
			case typeEmpty:
				break;
			case typeString8:
				new ((String8*)(void*)(&m_value)) String8(*(String8*)(void*)(&(other.m_value)));
				break;
			case typeString16:
				new ((String16*)(void*)(&m_value)) String16(*(String16*)(void*)(&(other.m_value)));
				break;
			case typeObject:
			case typeWeak:
				new ((Ref<Referable>*)(void*)(&m_value)) Ref<Referable>(*(Ref<Referable>*)(void*)(&(other.m_value)));
				break;
			default:
				m_value = other.m_value;
				break;
		}
	}
    switch (typeFree)
    {
        case typeString8:
            (*(String8*)(void*)(&valueFree)).String8::~String8();
            break;
		case typeString16:
			(*(String16*)(void*)(&valueFree)).String16::~String16();
			break;
		case typeObject:
		case typeWeak:
            (*(Ref<Referable>*)(void*)(&valueFree)).Ref<Referable>::~Ref();
            break;
        default:
            break;
    }
	return *this;
}

sl_bool Variant::operator==(const Variant& other) const
{
	if (this == &other) {
		return sl_true;
	}
	Variant v1 = *this;
	Variant v2 = other;
	if (v1.m_type != v2.m_type) {
		return sl_false;
	} else {
		if (v1.m_value == v2.m_value) {
			return sl_true;
		}
	}
	switch (v1.m_type) {
	case typeEmpty:
		return sl_true;
	case typeInt32:
	case typeUint32:
		return *(sl_int32*)(void*)(&(v1.m_value)) == *(sl_int32*)(void*)(&(v2.m_value));
	case typeInt64:
	case typeUint64:
		return v1.m_value == v2.m_value;
	case typeFloat:
		return *(float*)(void*)(&(v1.m_value)) == *(float*)(void*)(&(v2.m_value));
	case typeDouble:
		return *(double*)(void*)(&(v1.m_value)) == *(double*)(void*)(&(v2.m_value));
	case typeTime:
		return *(Time*)(void*)(&(v1.m_value)) == *(Time*)(void*)(&(v2.m_value));
	case typeBoolean:
		return *(sl_bool*)(void*)(&(v1.m_value)) == *(sl_bool*)(void*)(&(v2.m_value));
	case typePointer:
		return *(void**)(void*)(&(v1.m_value)) == *(void**)(void*)(&(v2.m_value));
	case typeString8:
		return *(String8*)(void*)(&(v1.m_value)) == *(String8*)(void*)(&(v2.m_value));
	case typeString16:
		return *(String16*)(void*)(&(v1.m_value)) == *(String16*)(void*)(&(v2.m_value));
	case typeObject:
	case typeWeak:
		return *(Ref<Referable>*)(void*)(&(v1.m_value)) == *(Ref<Referable>*)(void*)(&(v2.m_value));
	}
	return sl_false;
}

sl_int32 Variant::getInt32(sl_int32 def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return (sl_int32)(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return (sl_int32)(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return (sl_int32)(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return (sl_int32)(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return (sl_int32)(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return (sl_int32)(*(double*)(void*)(&(v.m_value)));
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value))).parseInt32(10, def);
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value))).parseInt32(10, def);
	default:
		return def;
	}
}

sl_uint32 Variant::getUint32(sl_uint32 def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return (sl_uint32)(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return (sl_uint32)(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return (sl_uint32)(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return (sl_uint32)(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return (sl_uint32)(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return (sl_uint32)(*(double*)(void*)(&(v.m_value)));
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value))).parseUint32(10, def);
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value))).parseUint32(10, def);
	default:
		return def;
	}
}

sl_int64 Variant::getInt64(sl_int64 def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return (sl_int64)(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return (sl_int64)(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return (sl_int64)(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return (sl_int64)(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return (sl_int64)(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return (sl_int64)(*(double*)(void*)(&(v.m_value)));
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value))).parseInt64(10, def);
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value))).parseInt64(10, def);
	default:
		return def;
	}
}

sl_uint64 Variant::getUint64(sl_uint64 def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return (sl_uint64)(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return (sl_uint64)(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return (sl_uint64)(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return (sl_uint64)(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return (sl_uint64)(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return (sl_uint64)(*(double*)(void*)(&(v.m_value)));
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value))).parseUint64(10, def);
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value))).parseUint64(10, def);
	default:
		return def;
	}
}

float Variant::getFloat(float def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return (float)(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return (float)(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return (float)(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return (float)(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return (float)(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return (float)(*(double*)(void*)(&(v.m_value)));
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value))).parseFloat(def);
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value))).parseFloat(def);
	default:
		return def;
	}
}

double Variant::getDouble(double def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return (double)(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return (double)(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return (double)(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return (double)(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return (double)(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return (double)(*(double*)(void*)(&(v.m_value)));
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value))).parseDouble(def);
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value))).parseDouble(def);
	default:
		return def;
	}
}

Time Variant::getTime(Time def) const
{
	Variant v = *this;
	if (v.m_type == typeTime) {
		return *(Time*)(void*)(&(v.m_value));
	} else if (v.m_type == typeString8) {
		return Time::parse((*(String8*)(void*)(&(v.m_value))));
	} else if (v.m_type == typeString16) {
		return Time::parse((*(String16*)(void*)(&(v.m_value))));
	} else {
		return def;
	}
}

const void* Variant::getPointer(const void* def) const
{
	Variant v = *this;
	if (v.m_type == typePointer) {
		return *(const void**)(void*)(&v.m_value);
	} else {
		return def;
	}
}

sl_bool Variant::getBoolean(sl_bool def) const
{
	Variant v = *this;
	if (v.m_type == typeBoolean) {
		return *(sl_bool*)(void*)(&(v.m_value));
	} else if (v.m_type == typeString8) {
		String8 s = v.getString().toLower();
		SLIB_STATIC_STRING8(_true, "true");
		SLIB_STATIC_STRING8(_false, "false");
		if (s == _true) {
			return sl_true;
		} else if (s == _false) {
			return sl_false;
		} else {
			return def;
		}
	} else {
		return def;
	}
}

String Variant::getString(const String& def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return String::fromInt32(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return String::fromUint32(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return String::fromInt64(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return String::fromUint64(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return String::fromFloat(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return String::fromDouble(*(double*)(void*)(&(v.m_value)));
	case typeBoolean:
		if (*(sl_bool*)(void*)(&(v.m_value))) {
			SLIB_STATIC_STRING(ret, "true");
			return ret;
		} else {
			SLIB_STATIC_STRING(ret, "false");
			return ret;
		}
	case typeTime:
		return (*(Time*)(void*)(&(v.m_value))).toString();
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value)));
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value)));
	default:
		return def;
	}
}

String16 Variant::getString16(const String16& def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeInt32:
		return String16::fromInt32(*(sl_int32*)(void*)(&(v.m_value)));
	case typeUint32:
		return String16::fromUint32(*(sl_uint32*)(void*)(&(v.m_value)));
	case typeInt64:
		return String16::fromInt64(*(sl_int64*)(void*)(&(v.m_value)));
	case typeUint64:
		return String16::fromUint64(*(sl_uint64*)(void*)(&(v.m_value)));
	case typeFloat:
		return String16::fromFloat(*(float*)(void*)(&(v.m_value)));
	case typeDouble:
		return String16::fromDouble(*(double*)(void*)(&(v.m_value)));
	case typeBoolean:
		if (*(sl_bool*)(void*)(&(v.m_value))) {
			SLIB_STATIC_STRING16(ret, "true");
			return ret;
		} else {
			SLIB_STATIC_STRING16(ret, "false");
			return ret;
		}
	case typeTime:
		return (*(Time*)(void*)(&(v.m_value))).toString();
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value)));
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value)));
	default:
		return def;
	}
}

String Variant::getStringOnly(const String& def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value)));
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value)));
	default:
		return def;
	}
}

String16 Variant::getString16Only(const String16& def) const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeString8:
		return (*(String8*)(void*)(&(v.m_value)));
	case typeString16:
		return (*(String16*)(void*)(&(v.m_value)));
	default:
		return def;
	}
}

sl_class_type Variant::getObjectClassType() const
{
	const Ref<Referable>& obj = getObject();
	if (obj.isNull()) {
		return 0;
	} else {
		return obj->getClassType();
	}
}

const char* Variant::getObjectClassTypeName() const
{
	const Ref<Referable>& obj = getObject();
	if (obj.isNull()) {
		return 0;
	} else {
		return obj->getClassTypeName();
	}
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
	Iterator< Pair<String, Variant> > iterator = map.iterator();
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
	Variant v = *this;
	switch (v.m_type) {
	case typeEmpty:
		return "<empty>";
	case typeInt32:
	case typeUint32:
	case typeInt64:
	case typeUint64:
	case typeFloat:
	case typeDouble:
	case typeBoolean:
		return v.getString(String::null());
	case typeTime:
		return "\"" + (*(Time*)(void*)(&(v.m_value))).toString() + "\"";
	case typeString8:
		return "\"" + (*(String8*)(void*)(&(v.m_value))) + "\"";
	case typeString16:
		return "\"" + (*(String16*)(void*)(&(v.m_value))) + "\"";
	case typePointer:
		return "#" + String::fromPointerValue(*(const void**)(void*)(&(v.m_value)));
	case typeObject:
		if (isVariantList()) {
			StringBuffer ret;
			if (!_Variant_getVariantListJSONString(ret, v.getVariantList(), sl_false)) {
				ret.add(" ...");
			}
			return ret.merge();
		} else if (isVariantMap()) {
			StringBuffer ret;
			if (!_Variant_getVariantMapJSONString(ret, v.getVariantMap(), sl_false)) {
				ret.add(" ...");
			}
			return ret.merge();
		} else {
			return "<object:" + String::fromUtf8(v.getObjectClassTypeName()) + ">";
		}
	case typeWeak:
		return "<weak-ref>";
	default:
		return "<error-type>";
	}
}

String Variant::toJSON() const
{
	Variant v = *this;
	switch (v.m_type) {
	case typeEmpty:
		return String::null();
	case typeInt32:
	case typeUint32:
	case typeInt64:
	case typeUint64:
	case typeFloat:
	case typeDouble:
	case typeBoolean:
		return v.getString(String::null());
	case typeTime:
	case typeString8:
		return v.getString(String8::null()).applyBackslashEscapes();
	case typeString16:
		return v.getString16(String16::null()).applyBackslashEscapes();
	case typeObject:
		if (isVariantList())
		{
			StringBuffer ret;
			if (!_Variant_getVariantListJSONString(ret, v.getVariantList(), sl_true)) {
				return String::null();
			}
			return ret.merge();
		} else if (isVariantMap()) {
			StringBuffer ret;
			if (!_Variant_getVariantMapJSONString(ret, v.getVariantMap(), sl_true)) {
				return String::null();
			}
			return ret.merge();
		}
	default:
		return String::null();
	}
}

SLIB_NAMESPACE_END
