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
	Boolean = 7,
	String8 = 8,
	String16 = 9,
	Sz8 = 10,
	Sz16 = 11,
	Time = 12,
	Pointer = 13,
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
	
	Variant(const sl_bool value);
	
	Variant(const String8& value);
	
	Variant(const SafeString8& value);
	
	Variant(const String16& value);
	
	Variant(const SafeString16& value);
	
	Variant(const sl_char8* sz8);
	
	Variant(const sl_char16* sz16);

	Variant(const Time& value);
	
	Variant(const void* ptr);
	
	template <class T>
	Variant(const Ref<T>& ref);
	
	template <class T>
	Variant(const SafeRef<T>& ref);
	
	template <class T>
	Variant(const WeakRef<T>& weak);
	
	template <class T>
	Variant(const SafeWeakRef<T>& weak);

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

public:
	static const Variant& null();

	static Variant fromInt32(sl_int32 value);
	
	static Variant fromUint32(sl_uint32 value);
	
	static Variant fromInt64(sl_int64 value);
	
	static Variant fromUint64(sl_uint64 value);
	
	static Variant fromFloat(float value);
	
	static Variant fromDouble(double value);
	
	static Variant fromBoolean(sl_bool value);
	
	static Variant fromString(const String& value);
	
	static Variant fromString8(const String8& value);
	
	static Variant fromString16(const String16& value);
	
	static Variant fromSz8(const sl_char8* value);
	
	static Variant fromSz16(const sl_char16* value);
	
	static Variant fromTime(const Time& value);
	
	static Variant fromPointer(const void* value);
	
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
	
	Variant& operator=(const sl_bool value);
	
	Variant& operator=(const String8& value);
	
	Variant& operator=(const SafeString8& value);
	
	Variant& operator=(const String16& value);
	
	Variant& operator=(const SafeString16& value);
	
	Variant& operator=(const sl_char8* sz8);
	
	Variant& operator=(const sl_char16* sz16);
	
	Variant& operator=(const Time& value);
	
	Variant& operator=(const void* ptr);
	
	template <class T>
	Variant& operator=(const Ref<T>& ref);
	
	template <class T>
	Variant& operator=(const SafeRef<T>& ref);
	
	template <class T>
	Variant& operator=(const WeakRef<T>& weak);
	
	template <class T>
	Variant& operator=(const SafeWeakRef<T>& weak);

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
	
public:
	VariantType getType() const;
	
	void setNull();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	
	sl_bool isInt32() const;

	sl_int32 getInt32(sl_int32 def = 0) const;
	
	void setInt32(sl_int32 value);

	sl_bool isUint32() const;

	sl_uint32 getUint32(sl_uint32 def = 0) const;
	
	void setUint32(sl_uint32 value);

	sl_bool isInt64() const;

	sl_int64 getInt64(sl_int64 def = 0) const;
	
	void setInt64(sl_int64 value);

	sl_bool isUint64() const;

	sl_uint64 getUint64(sl_uint64 def = 0) const;
	
	void setUint64(sl_uint64 value);

	sl_bool isInteger() const;
	
	sl_bool isSignedInteger() const;
	
	sl_bool isUnsignedInteger() const;
	
	sl_bool isFloat() const;

	float getFloat(float def = 0) const;
	
	void setFloat(float value);

	sl_bool isDouble() const;

	double getDouble(double def = 0) const;
	
	void setDouble(double value);

	sl_bool isNumber() const;

	
	sl_bool isBoolean() const;

	sl_bool getBoolean(sl_bool def = sl_false) const;
	
	void setBoolean(sl_bool value);

	
	sl_bool isString() const;

	sl_bool isString8() const;

	sl_bool isString16() const;
	
	sl_bool isSz8() const;
	
	sl_bool isSz16() const;

	String getString(const String& def) const;
	
	String getString() const;
	
	String8 getString8(const String8& def) const;
	
	String8 getString8() const;

	String16 getString16(const String16& def) const;

	String16 getString16() const;
	
	const sl_char8* getSz8(const sl_char8* def = sl_null) const;
	
	const sl_char16* getSz16(const sl_char16* def = sl_null) const;
	
	void setString(const String8& value);
	
	void setString(const String16& value);
	
	void setString(const SafeString8& value);
	
	void setString(const SafeString16& value);
	
	void setString(const sl_char8* sz8);
	
	void setString(const sl_char16* sz16);

	
	sl_bool isTime() const;
	
	Time getTime(const Time& def) const;
	
	Time getTime() const;
	
	void setTime(const Time& value);
	
	
	sl_bool isPointer() const;
	
	void* getPointer(const void* def = sl_null) const;
	
	void setPointer(const void* ptr);
	
	
	sl_bool isObject() const;
	
	sl_bool isWeak() const;

	Ref<Referable> getObject() const;

	template <class T>
	Ref<T> getObject(const Ref<T>& def) const;

	template <class T>
	void setObject(const Ref<T>& ref);
	
	template <class T>
	void setWeak(const WeakRef<T>& weak);
	
	sl_bool isObjectNotNull() const;
	
	sl_bool isObjectNull() const;
	
	sl_class_type getObjectClassType() const;
	
	sl_bool isMemory() const;
	
	Memory getMemory() const;
	
	void setMemory(const Memory& mem);
	
	template <class TYPE, class COMPARE>
	Array<TYPE, COMPARE> getArray(const Array<TYPE, COMPARE>& def) const;
	
	template <class TYPE, class COMPARE>
	void setArray(const Array<TYPE, COMPARE>& array);

	template <class TYPE, class COMPARE>
	List<TYPE, COMPARE> getList(const List<TYPE, COMPARE>& def) const;
	
	template <class TYPE, class COMPARE>
	void setList(const List<TYPE, COMPARE>& list);
	
	template <class KT, class VT>
	Map<KT, VT> getMap(const Map<KT, VT>& def) const;
	
	template <class KT, class VT>
	void setMap(const Map<KT, VT>& map);

	sl_bool isVariantList() const;
	
	List<Variant> getVariantList() const;

	sl_bool isVariantMap() const;
	
	Map<String, Variant> getVariantMap() const;
	
	sl_bool isVariantMapList() const;
	
	List< Map<String, Variant> > getVariantMapList() const;

	sl_size getListItemsCount() const;
	
	Variant getListItem(sl_size index) const;

	sl_bool setListItem(sl_size index, const Variant& value);

	sl_bool addListItem(const Variant& value);

	Variant getField(const String& key) const;

	sl_bool putField(const String& key, const Variant& value);

	
	String toString() const;

	String toJsonString() const;
	
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
	
	SafeVariant(const sl_bool value);
	
	SafeVariant(const String8& value);
	
	SafeVariant(const SafeString8& value);

	SafeVariant(const String16& value);
	
	SafeVariant(const SafeString16& value);
	
	SafeVariant(const sl_char8* sz8);
	
	SafeVariant(const sl_char16* sz16);
	
	SafeVariant(const Time& value);
	
	SafeVariant(const void* ptr);

	template <class T>
	SafeVariant(const Ref<T>& ref);
	
	template <class T>
	SafeVariant(const SafeRef<T>& ref);
	
	template <class T>
	SafeVariant(const WeakRef<T>& weak);
	
	template <class T>
	SafeVariant(const SafeWeakRef<T>& weak);

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
	
	SafeVariant& operator=(const sl_bool value);
	
	SafeVariant& operator=(const String8& value);
	
	SafeVariant& operator=(const SafeString8& value);
	
	SafeVariant& operator=(const String16& value);
	
	SafeVariant& operator=(const SafeString16& value);

	SafeVariant& operator=(const sl_char8* sz8);
	
	SafeVariant& operator=(const sl_char16* sz16);
	
	SafeVariant& operator=(const Time& value);
	
	SafeVariant& operator=(const void* ptr);
	
	template <class T>
	SafeVariant& operator=(const Ref<T>& ref);
	
	template <class T>
	SafeVariant& operator=(const SafeRef<T>& ref);
	
	template <class T>
	SafeVariant& operator=(const WeakRef<T>& weak);
	
	template <class T>
	SafeVariant& operator=(const SafeWeakRef<T>& weak);

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
	
public:
	VariantType getType() const;
	
	void setNull();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
public:
	sl_bool isInt32() const;
	
	sl_int32 getInt32(sl_int32 def = 0) const;
	
	void setInt32(sl_int32 value);
	
	sl_bool isUint32() const;
	
	sl_uint32 getUint32(sl_uint32 def = 0) const;
	
	void setUint32(sl_uint32 value);
	
	sl_bool isInt64() const;
	
	sl_int64 getInt64(sl_int64 def = 0) const;
	
	void setInt64(sl_int64 value);
	
	sl_bool isUint64() const;
	
	sl_uint64 getUint64(sl_uint64 def = 0) const;
	
	void setUint64(sl_uint64 value);
	
	sl_bool isInteger() const;
	
	sl_bool isSignedInteger() const;
	
	sl_bool isUnsignedInteger() const;

	sl_bool isFloat() const;
	
	float getFloat(float def = 0) const;
	
	void setFloat(float value);
	
	sl_bool isDouble() const;
	
	double getDouble(double def = 0) const;
	
	void setDouble(double value);
	
	sl_bool isNumber() const;
	
	
	sl_bool isTime() const;
	
	Time getTime(Time def) const;
	
	Time getTime() const;
	
	void setTime(const Time& time);
	
	
	sl_bool isPointer() const;
	
	void* getPointer(const void* def = sl_null) const;
	
	void setPointer(const void* ptr);
	
	
	sl_bool isBoolean() const;
	
	sl_bool getBoolean(sl_bool def = sl_false) const;
	
	void setBoolean(sl_bool value);
	
	
	sl_bool isString() const;
	
	sl_bool isString8() const;
	
	sl_bool isString16() const;
	
	sl_bool isSz8() const;
	
	sl_bool isSz16() const;
	
	String getString(const String& def) const;
	
	String getString() const;
	
	String8 getString8(const String8& def) const;
	
	String8 getString8() const;
	
	String16 getString16(const String16& def) const;
	
	String16 getString16() const;
	
	const sl_char8* getSz8(const sl_char8* def = sl_null) const;
	
	const sl_char16* getSz16(const sl_char16* def = sl_null) const;
	
	void setString(const String8& value);
	
	void setString(const SafeString8& value);
	
	void setString(const String16& value);
	
	void setString(const SafeString16& value);
	
	void setString(const sl_char8* sz8);
	
	void setString(const sl_char16* sz16);

	
	sl_bool isObject() const;
	
	sl_bool isWeak() const;
	
	Ref<Referable> getObject() const;

	template <class T>
	Ref<T> getObject(const Ref<T>& def) const;
	
	template <class T>
	void setObject(const Ref<T>& object);

	template <class T>
	void setWeak(const WeakRef<T>& weak);

	sl_bool isObjectNotNull() const;
	
	sl_bool isObjectNull() const;
	
	sl_class_type getObjectClassType() const;
	
	sl_bool isMemory() const;
	
	Memory getMemory() const;
	
	void setMemory(const Memory& mem);
	
	template <class TYPE, class COMPARE>
	Array<TYPE, COMPARE> getArray(const Array<TYPE, COMPARE>& def) const;
	
	template <class TYPE, class COMPARE>
	void setArray(const Array<TYPE, COMPARE>& array);
	
	template <class TYPE, class COMPARE>
	List<TYPE, COMPARE> getList(const List<TYPE, COMPARE>& def) const;
	
	template <class TYPE, class COMPARE>
	void setList(const List<TYPE, COMPARE>& list);
	
	template <class KT, class VT>
	Map<KT, VT> getMap(const Map<KT, VT>& def) const;
	
	template <class KT, class VT>
	void setMap(const Map<KT, VT>& map);
	
	
	sl_bool isVariantList() const;
	
	List<Variant> getVariantList() const;
	
	sl_bool isVariantMap() const;
	
	Map<String, Variant> getVariantMap() const;
	
	sl_bool isVariantMapList() const;
	
	List< Map<String, Variant> > getVariantMapList() const;
	
	sl_size getListItemsCount() const;
	
	Variant getListItem(sl_size index) const;
	
	sl_bool setListItem(sl_size index, const Variant& value);
	
	sl_bool addListItem(const Variant& value);
	
	Variant getField(const String& key) const;
	
	sl_bool putField(const String& key, const Variant& value);
	
	
	String toString() const;
	
	String toJsonString() const;
	
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


template <>
sl_class_type CList< Map<String, Variant> >::ClassType();

template <>
sl_class_type CList< Map<String, Variant> >::getClassType() const;

template <>
sl_bool CList< Map<String, Variant> >::checkClassType(sl_class_type type) const;


SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Variant)
SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(String, Variant)
SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(sl_uint64, Variant)


typedef List<Variant> VariantList;
typedef Map<String, Variant> VariantMap;
typedef List< Map<String, Variant> > VariantMapList;


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
	setObject(ref);
	return *this;
}

template <class T>
Variant& Variant::operator=(const SafeRef<T>& ref)
{
	setObject(ref);
	return *this;
}

template <class T>
Variant& Variant::operator=(const WeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T>
Variant& Variant::operator=(const SafeWeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const Array<T, COMPARE>& object)
{
	setArray(object);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const SafeArray<T, COMPARE>& object)
{
	setArray(object);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const List<T, COMPARE>& object)
{
	setList(object);
	return *this;
}

template <class T, class COMPARE>
Variant& Variant::operator=(const SafeList<T, COMPARE>& object)
{
	setList(object);
	return *this;
}

template <class KT, class VT>
Variant& Variant::operator=(const Map<KT, VT>& object)
{
	setMap(object);
	return *this;
}

template <class KT, class VT>
Variant& Variant::operator=(const SafeMap<KT, VT>& object)
{
	setMap(object);
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
	Ref<Referable> obj(getObject());
	if (T::checkInstance(obj.ptr)) {
		return Ref<T>::from(obj);
	} else {
		return def;
	}
}

template <class T>
void Variant::setObject(const Ref<T>& ref)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Ref<T>*)(void*)(&_value)) Ref<T>(ref);
}

template <class T>
void Variant::setWeak(const WeakRef<T>& weak)
{
	_free(_type, _value);
	_type = VariantType::Weak;
	new ((WeakRef<T>*)(void*)(&_value)) WeakRef<T>(weak);
}

template <class TYPE, class COMPARE>
Array<TYPE, COMPARE> Variant::getArray(const Array<TYPE, COMPARE>& def) const
{
	Ref<Referable> obj(getObject());
	if (CArray<TYPE, COMPARE>::checkInstance(obj.ptr)) {
		return (CArray<TYPE, COMPARE>*)(obj.ptr);
	}
	return def;
}

template <class T, class COMPARE>
void Variant::setArray(const Array<T, COMPARE>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Array<T, COMPARE>*)(void*)(&_value)) Array<T, COMPARE>(object);
}

template <class TYPE, class COMPARE>
List<TYPE, COMPARE> Variant::getList(const List<TYPE, COMPARE>& def) const
{
	Ref<Referable> obj(getObject());
	if (CList<TYPE, COMPARE>::checkInstance(obj.ptr)) {
		return (CList<TYPE, COMPARE>*)(obj.ptr);
	}
	return def;
}

template <class T, class COMPARE>
void Variant::setList(const List<T, COMPARE>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((List<T, COMPARE>*)(void*)(&_value)) List<T, COMPARE>(object);
}

template <class KT, class VT>
Map<KT, VT> Variant::getMap(const Map<KT, VT>& def) const
{
	Ref<Referable> obj(getObject());
	if (IMap<KT, VT>::checkInstance(obj.ptr)) {
		return (IMap<KT, VT>*)(obj.ptr);
	}
	return def;
}

template <class KT, class VT>
void Variant::setMap(const Map<KT, VT>& object)
{
	_free(_type, _value);
	_type = VariantType::Object;
	new ((Map<KT, VT>*)(void*)(&_value)) Map<KT, VT>(object);
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

SLIB_INLINE const SafeVariant& SafeVariant::null()
{
	return *((SafeVariant*)((void*)(&_Variant_Null)));
}

template <class T>
SLIB_INLINE SafeVariant& SafeVariant::operator=(const Ref<T>& ref)
{
	setObject(ref);
	return *this;
}

template <class T>
SafeVariant& SafeVariant::operator=(const SafeRef<T>& ref)
{
	setObject(ref);
	return *this;
}

template <class T>
SafeVariant& SafeVariant::operator=(const WeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T>
SafeVariant& SafeVariant::operator=(const SafeWeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const Array<T, COMPARE>& object)
{
	setArray(object);
	return *this;
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const SafeArray<T, COMPARE>& object)
{
	setArray(object);
	return *this;
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const List<T, COMPARE>& object)
{
	setList(object);
	return *this;
}

template <class T, class COMPARE>
SafeVariant& SafeVariant::operator=(const SafeList<T, COMPARE>& object)
{
	setList(object);
	return *this;
}

template <class KT, class VT>
SafeVariant& SafeVariant::operator=(const Map<KT, VT>& object)
{
	setMap(object);
	return *this;
}

template <class KT, class VT>
SafeVariant& SafeVariant::operator=(const SafeMap<KT, VT>& object)
{
	setMap(object);
	return *this;
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

template <class T>
void SafeVariant::setObject(const Ref<T>& object)
{
	sl_int64 v;
	new ((Ref<T>*)(void*)(&v)) Ref<T>(object);
	_replace(VariantType::Object, v);
}

template <class T>
void SafeVariant::setWeak(const WeakRef<T>& weak)
{
	sl_int64 v;
	new ((WeakRef<T>*)(void*)(&v)) WeakRef<T>(weak);
	_replace(VariantType::Weak, v);
}

template <class TYPE, class COMPARE>
Array<TYPE, COMPARE> SafeVariant::getArray(const Array<TYPE, COMPARE>& def) const
{
	Variant var(*this);
	return var.getArray(def);
}

template <class TYPE, class COMPARE>
void SafeVariant::setArray(const Array<TYPE, COMPARE>& object)
{
	sl_int64 v;
	new ((Array<TYPE, COMPARE>*)(void*)(&v)) Array<TYPE, COMPARE>(object);
	_replace(VariantType::Object, v);
}

template <class TYPE, class COMPARE>
List<TYPE, COMPARE> SafeVariant::getList(const List<TYPE, COMPARE>& def) const
{
	Variant var(*this);
	return var.getList(def);
}

template <class TYPE, class COMPARE>
void SafeVariant::setList(const List<TYPE, COMPARE>& object)
{
	sl_int64 v;
	new ((List<TYPE, COMPARE>*)(void*)(&v)) List<TYPE, COMPARE>(object);
	_replace(VariantType::Object, v);
}

template <class KT, class VT>
Map<KT, VT> SafeVariant::getMap(const Map<KT, VT>& def) const
{
	Variant var(*this);
	return var.getMap(def);
}

template <class KT, class VT>
void SafeVariant::setMap(const Map<KT, VT>& object)
{
	sl_int64 v;
	new ((Map<KT, VT>*)(void*)(&v)) Map<KT, VT>(object);
	_replace(VariantType::Object, v);
}

SLIB_NAMESPACE_END

#endif
