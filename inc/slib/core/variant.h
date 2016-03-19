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

enum class VariantType
{
	Null = 0,
	Int32 = 1,
	Uint32 = 2,
	Int64 = 3,
	Uint64 = 4,
	Float = 5,
	Double = 6,
	Time = 7,
	Pointer = 8,
	Boolean = 9,
	String8 = 10,
	String16 = 11,
	Object = 20,
	Weak = 30
};

struct _Variant_Const
{
	sl_uint64 value;
	VariantType type;
	sl_int32 lock;
};

extern const _Variant_Const _Variant_Null;

class SafeVariant;

class SLIB_EXPORT Variant
{
public:
	sl_uint64 _value;
	VariantType _type;

public:
	Variant();
	
	Variant(Variant&& other);
	
	Variant(const Variant& other);
	
	Variant(SafeVariant&& _other);
	
	Variant(const SafeVariant& other);

	~Variant();
	
public:
	Variant(sl_int32 value);
	
	Variant(sl_uint32 value);
	
	Variant(sl_int64 value);
	
	Variant(sl_uint64 value);
	
	Variant(float value);

	Variant(double value);
	
	Variant(const Time& value);
	
	Variant(const void* ptr);
	
	Variant(const sl_bool value);
	
	Variant(const String8& value);
	
	Variant(const SafeString8& value);
	
	Variant(const String16& value);
	
	Variant(const SafeString16& value);

	template <class T>
	Variant(const Ref<T>& ref);
	
	template <class T>
	Variant(const SafeRef<T>& ref);
	
	Variant(const Memory& mem);
	
	Variant(const SafeMemory& mem);
	
	template <class T, class COMPARE>
	Variant(const Array<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	Variant(const SafeArray<T, COMPARE>& object);

	template <class T, class COMPARE>
	Variant(const List<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	Variant(const SafeList<T, COMPARE>& object);
	
	template <class KT, class VT>
	Variant(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	Variant(const SafeMap<KT, VT>& object);
	
	template <class T>
	Variant(const WeakRef<T>& weak);
	
	template <class T>
	Variant(const SafeWeakRef<T>& weak);

public:
	static const Variant& null();

	static Variant fromInt32(sl_int32 value);
	
	static Variant fromUint32(sl_uint32 value);
	
	static Variant fromInt64(sl_int64 value);
	
	static Variant fromUint64(sl_uint64 value);
	
	static Variant fromFloat(float value);
	
	static Variant fromDouble(double value);
	
	static Variant fromTime(const Time& value);
	
	static Variant fromPointer(const void* value);
	
	static Variant fromBoolean(sl_bool value);
	
	static Variant fromString(const String8& value);
	
	static Variant fromString(const String16& value);
	
	template <class T>
	static Variant fromRef(const Ref<T>& ref);
	
	static Variant fromMemory(const Memory& mem);
	
	template <class T, class COMPARE>
	static Variant fromArray(const Array<T, COMPARE>& value);
	
	template <class T, class COMPARE>
	static Variant fromList(const List<T, COMPARE>& value);
	
	static Variant fromVariantList(const List<Variant>& value);
	
	static Variant createVariantList();

	template <class KT, class VT>
	static Variant fromMap(const Map<KT, VT>& value);

	static Variant fromVariantMap(const Map<String, Variant>& value);
	
	static Variant createVariantListMap();
	
	static Variant createVariantTreeMap();
	
	static Variant createVariantHashMap();
	
	template <class T>
	static Variant fromWeakRef(const WeakRef<T>& weak);
	
public:
	Variant& operator=(Variant&& other);
	
	Variant& operator=(const Variant& other);
	
	Variant& operator=(SafeVariant&& other);
	
	Variant& operator=(const SafeVariant& other);
	
	Variant& operator=(sl_int32 value);
	
	Variant& operator=(sl_uint32 value);
	
	Variant& operator=(sl_int64 value);
	
	Variant& operator=(sl_uint64 value);
	
	Variant& operator=(float value);
	
	Variant& operator=(double value);
	
	Variant& operator=(const Time& value);
	
	Variant& operator=(const void* ptr);
	
	Variant& operator=(const sl_bool value);
	
	Variant& operator=(const String8& value);
	
	Variant& operator=(const SafeString8& value);
	
	Variant& operator=(const String16& value);
	
	Variant& operator=(const SafeString16& value);
	
	template <class T>
	Variant& operator=(const Ref<T>& ref);
	
	template <class T>
	Variant& operator=(const SafeRef<T>& ref);
	
	Variant& operator=(const Memory& mem);
	
	Variant& operator=(const SafeMemory& mem);
	
	template <class T, class COMPARE>
	Variant& operator=(const Array<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	Variant& operator=(const SafeArray<T, COMPARE>& object);

	template <class T, class COMPARE>
	Variant& operator=(const List<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	Variant& operator=(const SafeList<T, COMPARE>& object);
	
	template <class KT, class VT>
	Variant& operator=(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	Variant& operator=(const SafeMap<KT, VT>& object);
	
	template <class T>
	Variant& operator=(const WeakRef<T>& weak);
	
	template <class T>
	Variant& operator=(const SafeWeakRef<T>& weak);
	
public:
	VariantType getType() const;
	
	void setNull();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	
	sl_bool isInt32() const;

	sl_int32 getInt32(sl_int32 def = 0) const;

	sl_bool isUint32() const;

	sl_uint32 getUint32(sl_uint32 def = 0) const;

	sl_bool isInt64() const;

	sl_int64 getInt64(sl_int64 def = 0) const;

	sl_bool isUint64() const;

	sl_uint64 getUint64(sl_uint64 def = 0) const;

	sl_bool isInteger() const;

	sl_bool isFloat() const;

	float getFloat(float def = 0) const;

	sl_bool isDouble() const;

	double getDouble(double def = 0) const;

	sl_bool isNumber() const;

	
	sl_bool isTime() const;

	Time getTime(Time def) const;

	Time getTime() const;
	
	
	sl_bool isPointer() const;

	void* getPointer(const void* def = sl_null) const;

	
	sl_bool isBoolean() const;

	sl_bool getBoolean(sl_bool def) const;

	
	sl_bool isString() const;

	sl_bool isString8() const;

	sl_bool isString16() const;

	String getString(const String& def) const;
	
	String getString() const;
	
	String8 getString8(const String8& def) const;
	
	String8 getString8() const;

	String16 getString16(const String16& def) const;

	String16 getString16() const;

	
	sl_bool isObject() const;

	Ref<Referable> getObject() const;

	template <class T>
	Ref<T> getObject(const Ref<T>& def) const;
	
	sl_bool isObjectNotNull() const;
	
	sl_bool isObjectNull() const;
	
	sl_class_type getObjectClassType() const;
	
	sl_bool isMemory() const;
	
	Memory getMemory() const;

	template <class TYPE, class COMPARE>
	List<TYPE, COMPARE> getList(const List<TYPE, COMPARE>& def) const;
	
	template <class KT, class VT>
	Map<KT, VT> getMap(const Map<KT, VT>& def) const;

	sl_bool isWeak() const;
	
	template <class T>
	WeakRef<T> getWeak(const WeakRef<T>& def) const;
	

	sl_bool isVariantList() const;
	
	List<Variant> getVariantList() const;

	sl_bool isVariantMap() const;
	
	Map<String, Variant> getVariantMap() const;
	
	sl_size getListItemsCount() const;
	
	Variant getListItem(sl_size index) const;

	sl_bool setListItem(sl_size index, const Variant& value);

	sl_bool addListItem(const Variant& value);

	Variant getField(const String& key) const;

	sl_bool putField(const String& key, const Variant& value);

	
	String toString() const;

	String toJson() const;

	
private:
	static void _free(VariantType type, sl_uint64 value);
	
	friend class SafeVariant;
	
};


class SLIB_EXPORT SafeVariant
{
public:
	sl_uint64 _value;
	VariantType _type;
private:
	SpinLock m_lock;
	
public:
	SafeVariant();
	
	SafeVariant(SafeVariant&& other);
	
	SafeVariant(const SafeVariant& other);
	
	SafeVariant(Variant&& other);
	
	SafeVariant(const Variant& other);
	
	~SafeVariant();
	
public:
	SafeVariant(sl_int32 value);
	
	SafeVariant(sl_uint32 value);
	
	SafeVariant(sl_int64 value);
	
	SafeVariant(sl_uint64 value);
	
	SafeVariant(float value);
	
	SafeVariant(double value);
	
	SafeVariant(const Time& value);
	
	SafeVariant(const void* ptr);
	
	SafeVariant(const sl_bool value);
	
	SafeVariant(const String8& value);
	
	SafeVariant(const SafeString8& value);

	SafeVariant(const String16& value);
	
	SafeVariant(const SafeString16& value);

	template <class T>
	SafeVariant(const Ref<T>& ref);
	
	template <class T>
	SafeVariant(const SafeRef<T>& ref);
	
	SafeVariant(const Memory& mem);
	
	SafeVariant(const SafeMemory& mem);
	
	template <class T, class COMPARE>
	SafeVariant(const Array<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	SafeVariant(const SafeArray<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	SafeVariant(const List<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	SafeVariant(const SafeList<T, COMPARE>& object);

	template <class KT, class VT>
	SafeVariant(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	SafeVariant(const SafeMap<KT, VT>& object);
	
	template <class T>
	SafeVariant(const WeakRef<T>& weak);
	
	template <class T>
	SafeVariant(const SafeWeakRef<T>& weak);
	
public:
	static const SafeVariant& null();
	
public:
	SafeVariant& operator=(SafeVariant&& other);
	
	SafeVariant& operator=(const SafeVariant& other);
	
	SafeVariant& operator=(Variant&& other);
	
	SafeVariant& operator=(const Variant& other);
	
	SafeVariant& operator=(sl_int32 value);
	
	SafeVariant& operator=(sl_uint32 value);
	
	SafeVariant& operator=(sl_int64 value);
	
	SafeVariant& operator=(sl_uint64 value);
	
	SafeVariant& operator=(float value);
	
	SafeVariant& operator=(double value);
	
	SafeVariant& operator=(const Time& value);
	
	SafeVariant& operator=(const void* ptr);
	
	SafeVariant& operator=(const sl_bool value);
	
	SafeVariant& operator=(const String8& value);
	
	SafeVariant& operator=(const SafeString8& value);
	
	SafeVariant& operator=(const String16& value);
	
	SafeVariant& operator=(const SafeString16& value);
	
	template <class T>
	SafeVariant& operator=(const Ref<T>& ref);
	
	template <class T>
	SafeVariant& operator=(const SafeRef<T>& ref);
	
	SafeVariant& operator=(const Memory& mem);
	
	SafeVariant& operator=(const SafeMemory& mem);
	
	template <class T, class COMPARE>
	SafeVariant& operator=(const Array<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	SafeVariant& operator=(const SafeArray<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	SafeVariant& operator=(const List<T, COMPARE>& object);
	
	template <class T, class COMPARE>
	SafeVariant& operator=(const SafeList<T, COMPARE>& object);

	template <class KT, class VT>
	SafeVariant& operator=(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	SafeVariant& operator=(const SafeMap<KT, VT>& object);
	
	template <class T>
	SafeVariant& operator=(const WeakRef<T>& weak);
	
	template <class T>
	SafeVariant& operator=(const SafeWeakRef<T>& weak);
	
public:
	VariantType getType() const;
	
	void setNull();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
public:
	sl_bool isInt32() const;
	
	sl_int32 getInt32(sl_int32 def = 0) const;
	
	sl_bool isUint32() const;
	
	sl_uint32 getUint32(sl_uint32 def = 0) const;
	
	sl_bool isInt64() const;
	
	sl_int64 getInt64(sl_int64 def = 0) const;
	
	sl_bool isUint64() const;
	
	sl_uint64 getUint64(sl_uint64 def = 0) const;
	
	sl_bool isInteger() const;
	
	sl_bool isFloat() const;
	
	float getFloat(float def = 0) const;
	
	sl_bool isDouble() const;
	
	double getDouble(double def = 0) const;
	
	sl_bool isNumber() const;
	
	
	sl_bool isTime() const;
	
	Time getTime(Time def) const;
	
	Time getTime() const;
	
	
	sl_bool isPointer() const;
	
	void* getPointer(const void* def = sl_null) const;
	
	
	sl_bool isBoolean() const;
	
	sl_bool getBoolean(sl_bool def) const;
	
	
	sl_bool isString() const;
	
	sl_bool isString8() const;
	
	sl_bool isString16() const;
	
	String getString(const String& def) const;
	
	String getString() const;
	
	String8 getString8(const String8& def) const;
	
	String8 getString8() const;
	
	String16 getString16(const String16& def) const;
	
	String16 getString16() const;
	
	
	sl_bool isObject() const;
	
	Ref<Referable> getObject() const;
	
	template <class T>
	Ref<T> getObject(const Ref<T>& def) const;
	
	sl_bool isObjectNotNull() const;
	
	sl_bool isObjectNull() const;
	
	sl_class_type getObjectClassType() const;
	
	sl_bool isMemory() const;
	
	Memory getMemory() const;
	
	template <class TYPE, class COMPARE>
	List<TYPE, COMPARE> getList(const List<TYPE, COMPARE>& def) const;
	
	template <class KT, class VT>
	Map<KT, VT> getMap(const Map<KT, VT>& def) const;

	sl_bool isWeak() const;
	
	template <class T>
	WeakRef<T> getWeak(const WeakRef<T>& def) const;
	
	
	sl_bool isVariantList() const;
	
	List<Variant> getVariantList() const;
	
	sl_bool isVariantMap() const;
	
	Map<String, Variant> getVariantMap() const;
	
	sl_size getListItemsCount() const;
	
	Variant getListItem(sl_size index) const;
	
	sl_bool setListItem(sl_size index, const Variant& value);
	
	sl_bool addListItem(const Variant& value);
	
	Variant getField(const String& key) const;
	
	sl_bool putField(const String& key, const Variant& value);
	
	
	String toString() const;
	
	String toJson() const;
	
private:
	void _retain(VariantType& type, sl_uint64& value) const;
	
	void _replace(VariantType type, sl_uint64 value);
	
	friend class Variant;
	
};

sl_bool operator==(const Variant& v1, const Variant& v2);

sl_bool operator!=(const Variant& v1, const Variant& v2);


template <>
sl_int32 Compare<Variant>::compare(const Variant &a, const Variant &b);

template <>
sl_class_type IMap<String, Variant>::ClassType();

template <>
sl_class_type IMap<String, Variant>::getClassType() const;

template <>
sl_bool IMap<String, Variant>::checkClassType(sl_class_type type) const;

template <>
sl_class_type CList<Variant>::ClassType();

template <>
sl_class_type CList<Variant>::getClassType() const;

template <>
sl_bool CList<Variant>::checkClassType(sl_class_type type) const;

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Variant)
SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, Variant)
SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(sl_uint64, Variant)

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

SLIB_INLINE Variant::Variant() : _type(VariantType::Null)
{
}

template <class T>
Variant::Variant(const Ref<T>& ref)
{
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
}

template <class T>
Variant::Variant(const SafeRef<T>& ref)
{
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
}

template <class T, class COMPARE>
Variant::Variant(const Array<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
}

template <class T, class COMPARE>
Variant::Variant(const SafeArray<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
}

template <class T, class COMPARE>
Variant::Variant(const List<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
}

template <class T, class COMPARE>
Variant::Variant(const SafeList<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
}

template <class KT, class VT>
Variant::Variant(const Map<KT, VT>& object)
{
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
}

template <class KT, class VT>
Variant::Variant(const SafeMap<KT, VT>& object)
{
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
}

template <class T>
Variant::Variant(const WeakRef<T>& weak)
{
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
}

template <class T>
Variant::Variant(const SafeWeakRef<T>& weak)
{
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
}

SLIB_INLINE const Variant& Variant::null()
{
	return *((Variant*)((void*)(&_Variant_Null)));
}

template <class T>
Variant Variant::fromRef(const Ref<T>& ref)
{
	return ref;
}

template <class T, class COMPARE>
static Variant fromArray(const Array<T, COMPARE>& value)
{
	return value;
}

template <class T, class COMPARE>
static Variant fromList(const List<T, COMPARE>& value)
{
	return value;
}

template <class KT, class VT>
static Variant fromMap(const Map<KT, VT>& value)
{
	return value;
}

template <class T>
static Variant fromWeakRef(const WeakRef<T>& weak)
{
	return weak;
}

template <class T>
Variant& Variant::operator=(const Ref<T>& ref)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
	return *this;
}

template <class T>
Variant& Variant::operator=(const SafeRef<T>& ref)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const Array<T, COMPARE>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const SafeArray<T, COMPARE>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const List<T, COMPARE>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const SafeList<T, COMPARE>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
	return *this;
}

template <class KT, class VT>
Variant& Variant::operator=(const Map<KT, VT>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
	return *this;
}

template <class KT, class VT>
Variant& Variant::operator=(const SafeMap<KT, VT>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
	return *this;
}

template <class T>
Variant& Variant::operator=(const WeakRef<T>& weak)
{
	_free(_type, _value);
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
	return *this;
}

template <class T>
Variant& Variant::operator=(const SafeWeakRef<T>& weak)
{
	_free(_type, _value);
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
	return *this;
}

SLIB_INLINE VariantType Variant::getType() const
{
	return _type;
}

SLIB_INLINE sl_bool Variant::isNull() const
{
	return _type == VariantType::Null;
}

SLIB_INLINE sl_bool Variant::isNotNull() const
{
	return _type != VariantType::Null;
}

template <class T>
Ref<T> Variant::getObject(const Ref<T>& def) const
{
	if (_type == VariantType::Object) {
		return *((Ref<T>*)(void*)(&_value));
	}
	return def;
}

template <class TYPE, class COMPARE>
List<TYPE, COMPARE> Variant::getList(const List<TYPE, COMPARE>& def) const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		return (CList<TYPE, COMPARE>*)ref;
	}
	return def;
}

template <class KT, class VT>
Map<KT, VT> Variant::getMap(const Map<KT, VT>& def) const
{
	if (_type == VariantType::Object) {
		Referable* ref = *((Referable**)(void*)(&_value));
		return (IMap<KT, VT>*)ref;
	}
	return def;
}

template <class T>
WeakRef<T> Variant::getWeak(const WeakRef<T>& def) const
{
	if (_type == VariantType::Weak) {
		return *((WeakRef<T>*)(void*)(&_value));
	}
	return def;
}


SLIB_INLINE SafeVariant::SafeVariant() : _type(VariantType::Null)
{
}

template <class T>
SafeVariant::SafeVariant(const Ref<T>& ref)
{
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
}

template <class T>
SafeVariant::SafeVariant(const SafeRef<T>& ref)
{
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
}

template <class T, class COMPARE>
SafeVariant::SafeVariant(const Array<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
}

template <class T, class COMPARE>
SafeVariant::SafeVariant(const SafeArray<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
}

template <class T, class COMPARE>
SafeVariant::SafeVariant(const List<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
}

template <class T, class COMPARE>
SafeVariant::SafeVariant(const SafeList<T, COMPARE>& object)
{
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
}

template <class KT, class VT>
SafeVariant::SafeVariant(const Map<KT, VT>& object)
{
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
}

template <class KT, class VT>
SafeVariant::SafeVariant(const SafeMap<KT, VT>& object)
{
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
}

template <class T>
SafeVariant::SafeVariant(const WeakRef<T>& weak)
{
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
}

template <class T>
SafeVariant::SafeVariant(const SafeWeakRef<T>& weak)
{
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
}

SLIB_INLINE const SafeVariant& SafeVariant::null()
{
	return *((SafeVariant*)((void*)(&_Variant_Null)));
}

template <class T>
SafeVariant& SafeVariant::operator=(const Ref<T>& ref)
{
	return *this = Variant(ref);
}

template <class T>
SafeVariant& SafeVariant::operator=(const SafeRef<T>& ref)
{
	return *this = Variant(ref);
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const Array<T, COMPARE>& object)
{
	return *this = Variant(object);
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const SafeArray<T, COMPARE>& object)
{
	return *this = Variant(object);
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const List<T, COMPARE>& object)
{
	return *this = Variant(object);
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const SafeList<T, COMPARE>& object)
{
	return *this = Variant(object);
}

template <class KT, class VT>
SafeVariant& SafeVariant::operator=(const Map<KT, VT>& object)
{
	return *this = Variant(object);
}

template <class KT, class VT>
SafeVariant& SafeVariant::operator=(const SafeMap<KT, VT>& object)
{
	return *this = Variant(object);
}

template <class T>
SafeVariant& SafeVariant::operator=(const WeakRef<T>& weak)
{
	return *this = Variant(weak);
}

template <class T>
SafeVariant& SafeVariant::operator=(const SafeWeakRef<T>& weak)
{
	return *this = Variant(weak);
}

SLIB_INLINE VariantType SafeVariant::getType() const
{
	return _type;
}

SLIB_INLINE sl_bool SafeVariant::isNull() const
{
	return _type == VariantType::Null;
}

SLIB_INLINE sl_bool SafeVariant::isNotNull() const
{
	return _type != VariantType::Null;
}

template <class T>
Ref<T> SafeVariant::getObject(const Ref<T>& def) const
{
	Variant var(*this);
	return var.getObject(def);
}

template <class TYPE, class COMPARE>
List<TYPE, COMPARE> SafeVariant::getList(const List<TYPE, COMPARE>& def) const
{
	Variant var(*this);
	return var.getList(def);
}

template <class KT, class VT>
Map<KT, VT> SafeVariant::getMap(const Map<KT, VT>& def) const
{
	Variant var(*this);
	return var.getMap(def);
}

template <class T>
WeakRef<T> SafeVariant::getWeak(const WeakRef<T>& def) const
{
	Variant var(*this);
	return var.getWeak(def);
}

SLIB_NAMESPACE_END

#endif
