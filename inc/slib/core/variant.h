#ifndef CHECKHEADER_SLIB_CORE_VARIANT
#define CHECKHEADER_SLIB_CORE_VARIANT

#include "definition.h"
#include "reference.h"
#include "pointer.h"
#include "time.h"
#include "string.h"
#include "list.h"
#include "map.h"

SLIB_NAMESPACE_BEGIN

extern sl_uint32 _Variant_null[4];

class SLIB_EXPORT Variant
{
public:
	enum Type {
		typeEmpty = 0,
		typeInt32 = 1,
		typeUint32 = 2,
		typeInt64 = 3,
		typeUint64 = 4,
		typeFloat = 5,
		typeDouble = 6,
		typeTime = 7,
		typePointer = 8,
		typeBoolean = 9,
		typeString8 = 10,
		typeString16 = 11,
		typeObject = 20,
		typeWeak = 30,
	};

private:
	Type m_type;
	sl_uint64 m_value;

public:
	SLIB_INLINE Variant()
	{
		m_type = typeEmpty;
	}

	Variant(const Variant& other);

	~Variant();

	SLIB_INLINE static const Variant& null()
	{
		return *((Variant*)((void*)(&_Variant_null)));
	}

	Variant& operator=(const Variant& other);

	sl_bool operator==(const Variant& other) const;

	SLIB_INLINE sl_bool operator!=(const Variant& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE Variant(sl_int32 value)
	{
		m_type = typeInt32;
		*(sl_int32*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE static Variant fromInt32(sl_int32 value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(sl_uint32 value)
	{
		m_type = typeUint32;
		*(sl_uint32*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromUint32(sl_uint32 value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(sl_int64 value)
	{
		m_type = typeInt64;
		*(sl_int64*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromInt64(sl_int64 value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(sl_uint64 value)
	{
		m_type = typeUint64;
		*(sl_uint64*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromUint64(sl_uint64 value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(float value)
	{
		m_type = typeFloat;
		*(float*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromFloat(float value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(double value)
	{
		m_type = typeDouble;
		*(double*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromDouble(double value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const Time& value)
	{
		m_type = typeTime;
		*(Time*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromTime(const Time& value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const void* ptr)
	{
		m_type = typePointer;
		*(const void**)(void*)(&m_value) = ptr;
	}

	SLIB_INLINE static Variant fromPointer(const void* value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const sl_bool value)
	{
		m_type = typeBoolean;
		*(sl_bool*)(void*)(&m_value) = value;
	}

	SLIB_INLINE static Variant fromBoolean(sl_bool value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const String8& value)
	{
		m_type = typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
	}

	SLIB_INLINE static Variant fromString(const String8& value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const String16& value)
	{
		m_type = typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
	}

	SLIB_INLINE static Variant fromString(const String16& value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const List<Variant>& object)
	{
		m_type = typeObject;
		new ((List<Variant>*)(void*)(&m_value)) List<Variant>(object);
	}

	SLIB_INLINE static Variant fromVariantList(const List<Variant>& value)
	{
		Variant ret(value);
		return ret;
	}

	SLIB_INLINE Variant(const Map<String, Variant>& object)
	{
		m_type = typeObject;
		new ((Map<String, Variant>*)(void*)(&m_value)) Map<String, Variant>(object);
	}

	SLIB_INLINE static Variant fromVariantMap(const Map<String, Variant>& value)
	{
		Variant ret(value);
		return ret;
	}

	template <class T>
	SLIB_INLINE Variant(const Ref<T>& ref)
	{
		m_type = typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
	}

	template <class T>
	SLIB_INLINE static Variant fromRef(const Ref<T>& ref)
	{
		Variant ret(ref);
		return ret;
	}

	template <class T>
	SLIB_INLINE Variant(const WeakRef<T>& weak)
	{
		m_type = typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
	}

	template <class T>
	SLIB_INLINE static Variant fromWeakRef(const WeakRef<T>& weak)
	{
		Variant ret(weak);
		return ret;
	}

	SLIB_INLINE Variant(const Memory& mem)
	{
		m_type = typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
	}

	SLIB_INLINE static Variant fromMemory(const Memory& mem)
	{
		Variant ret(mem);
		return ret;
	}

	SLIB_INLINE static Variant createVariantList()
	{
		List<Variant> lst = List<Variant>::create();
		Variant var(lst);
		return var;
	}

	SLIB_INLINE static Variant createVariantListMap()
	{
		Map<String, Variant> lst = Map<String, Variant>::createList();
		Variant var(lst);
		return var;
	}

	SLIB_INLINE static Variant createVariantTreeMap()
	{
		Map<String, Variant> lst = Map<String, Variant>::createTree();
		Variant var(lst);
		return var;
	}

	SLIB_INLINE static Variant createVariantHashMap()
	{
		Map<String, Variant> lst = Map<String, Variant>::createHash();
		Variant var(lst);
		return var;
	}
	
	SLIB_INLINE Type getType() const
	{
		return m_type;
	}

	SLIB_INLINE void setNull()
	{
		*this = Variant::null();
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		return m_type == typeEmpty;
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return m_type == typeEmpty;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return m_type != typeEmpty;
	}

	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_type != typeEmpty;
	}

	SLIB_INLINE sl_bool isInt32() const
	{
		return m_type == typeInt32;
	}

	sl_int32 getInt32(sl_int32 def = 0) const;

	SLIB_INLINE sl_bool isUint32() const
	{
		return m_type == typeUint32;
	}

	sl_uint32 getUint32(sl_uint32 def = 0) const;

	SLIB_INLINE sl_bool isInt64() const
	{
		return m_type == typeInt64;
	}

	sl_int64 getInt64(sl_int64 def = 0) const;

	SLIB_INLINE sl_bool isUint64() const
	{
		return m_type == typeUint64;
	}

	sl_uint64 getUint64(sl_uint64 def = 0) const;

	SLIB_INLINE sl_bool isInteger() const
	{
		return m_type == typeInt32 || m_type == typeUint32 || m_type == typeInt64 || m_type == typeUint64;
	}

	SLIB_INLINE sl_bool isFloat() const
	{
		return m_type == typeFloat;
	}

	float getFloat(float def = 0) const;

	SLIB_INLINE sl_bool isDouble() const
	{
		return m_type == typeDouble;
	}

	double getDouble(double def = 0) const;

	SLIB_INLINE sl_bool isFloatOrDouble() const
	{
		return m_type == typeFloat || m_type == typeDouble;
	}

	SLIB_INLINE sl_bool isNumber() const
	{
		return isInteger() || isFloatOrDouble();
	}

	SLIB_INLINE sl_bool isTime() const
	{
		return m_type == typeTime;
	}

	Time getTime(Time def) const;

	Time getTime() const
	{
		return getTime(Time::zero());
	}

	SLIB_INLINE sl_bool isPointer() const
	{
		return m_type == typePointer;
	}

	const void* getPointer(const void* def = sl_null) const;

	SLIB_INLINE sl_bool isBoolean() const
	{
		return m_type == typeBoolean;
	}

	sl_bool getBoolean(sl_bool def) const;

	SLIB_INLINE sl_bool isString() const
	{
		return m_type == typeString8 || m_type == typeString16;
	}

	SLIB_INLINE sl_bool isString8() const
	{
		return m_type == typeString8;
	}

	SLIB_INLINE sl_bool isString16() const
	{
		return m_type == typeString16;
	}

	String getString(const String& def) const;

	String16 getString16(const String16& def) const;

	SLIB_INLINE String getString() const
	{
		return getString(String::null());
	}

	SLIB_INLINE String16 getString16() const
	{
		return getString16(String16::null());
	}

	String getStringOnly(const String& def) const;

	String16 getString16Only(const String16& def) const;

	SLIB_INLINE const String getStringOnly() const
	{
		return getStringOnly(String::null());
	}

	SLIB_INLINE const String16 getString16Only() const
	{
		return getString16Only(String16::null());
	}

	SLIB_INLINE sl_bool isObject() const
	{
		return m_type == typeObject;
	}

	SLIB_INLINE const Ref<Referable> getObject() const
	{
		Variant v = *this;
		if (v.m_type == typeObject) {
			return *(Ref<Referable>*)(void*)(&(v.m_value));
		} else {
			return Ref<Referable>::null();
		}
	}

	template <class T>
	SLIB_INLINE const Ref<T> getObject(const Ref<T>& def) const
	{
		Variant v = *this;
		if (v.m_type == typeObject) {
			return *(Ref<T>*)(void*)(&(v.m_value));
		} else {
			return def;
		}
	}

	SLIB_INLINE sl_bool isObjectNull() const
	{
		const Ref<Referable>& obj = getObject();
		return obj.isNull();
	}
	
	SLIB_INLINE sl_class_type getObjectClassType() const;
	
	SLIB_INLINE const char* getObjectClassTypeName() const;

	SLIB_INLINE sl_bool isMemory() const
	{
		return Memory::checkInstance(getObject());
	}
	
	SLIB_INLINE Memory getMemory() const
	{
		Ref<Referable> obj = getObject();
		if (Memory::checkInstance(obj)) {
			return *(Memory*)(void*)(&obj);
		} else {
			return Memory::null();
		}
	}

	SLIB_INLINE sl_bool isVariantList() const
	{
		return List<Variant>::checkInstance(getObject());
	}
	
	SLIB_INLINE List<Variant> getVariantList() const
	{
		Ref<Referable> obj = getObject();
		if (List<Variant>::checkInstance(obj)) {
			return *((List<Variant>*)(void*)(&obj));
		}
		return List<Variant>::null();
	}
	
	template <class TYPE, class COMPARE>
	SLIB_INLINE const List<TYPE, COMPARE>& getList(const List<TYPE, COMPARE>& def) const
	{
		Variant v = *this;
		Ref<Referable> obj = getObject();
		if (List<TYPE, COMPARE>::checkInstance(v)) {
			return *((List<TYPE, COMPARE>*)(void*)(&obj));
		} else {
			return def;
		}
	}

	SLIB_INLINE sl_bool isVariantMap() const
	{
		return Map<String, Variant>::checkInstance(getObject());
	}
	
	SLIB_INLINE Map<String, Variant> getVariantMap() const
	{
		Ref<Referable> obj = getObject();
		if (Map<String, Variant>::checkInstance(obj)) {
			return *((Map<String, Variant>*)(void*)(&obj));
		}
		return Map<String, Variant>::null();
	}
	
	template <class KT, class VT>
	SLIB_INLINE const Map<KT, VT>& getMap(const Map<KT, VT>& def) const
	{
		Ref<Referable> obj = getObject();
		if (Map<KT, VT>::checkInstance(obj)) {
			return *((Map<KT, VT>*)(void*)(&obj));
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_size getListItemsCount() const
	{
		List<Variant> list = getVariantList();
		return list.getCount();
	}

	SLIB_INLINE Variant getListItem(sl_size index) const
	{
		List<Variant> list = getVariantList();
		return list.getItemValue(index, Variant::null());
	}

	SLIB_INLINE sl_bool putListItem(sl_size index, const Variant& value)
	{
		List<Variant> list = getVariantList();
		if (list.isNotNull()) {
			return list.setItem(index, value);
		} else {
			return sl_false;
		}
	}

	SLIB_INLINE void addListItem(const Variant& value)
	{
		List<Variant> list = getVariantList();
		if (list.isNotNull()) {
			list.add(value);
		}
	}

	SLIB_INLINE Variant getField(const String& key) const
	{
		Map<String, Variant> map = getVariantMap();
		return map.getValue(key, Variant::null());
	}

	SLIB_INLINE void putField(const String& key, const Variant& value)
	{
		Map<String, Variant> map = getVariantMap();
		if (map.isNotNull()) {
			map.put(key, value);
		}
	}

	template <class T>
	SLIB_INLINE const WeakRef<T> getWeak(const WeakRef<T>& def) const
	{
		Variant v = *this;
		if (v.m_type == typeWeak) {
			return *(WeakRef<T>*)(void*)(&(v.m_value));
		} else {
			return def;
		}
	}
	
	String toString() const;

	String toJSON() const;

};

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_VARIANT
