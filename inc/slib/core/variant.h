#ifndef CHECKHEADER_SLIB_CORE_VARIANT
#define CHECKHEADER_SLIB_CORE_VARIANT

#include "definition.h"
#include "ref.h"
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

class Variant;
typedef Atomic<Variant> AtomicVariant;

class SLIB_EXPORT Variant
{
public:
	sl_uint64 _value;
	VariantType _type;

public:
	SLIB_CONSTEXPR Variant() : _value(0), _type(VariantType::Null) {}
	
	Variant(Variant&& other);
	
	Variant(const Variant& other);
	
	Variant(AtomicVariant&& _other);
	
	Variant(const AtomicVariant& other);

	~Variant();
	
public:
	Variant(sl_null_t);
	
	Variant(sl_int32 value);
	
	Variant(sl_uint32 value);
	
	Variant(sl_int64 value);
	
	Variant(sl_uint64 value);
	
	Variant(float value);

	Variant(double value);
	
	Variant(const sl_bool value);
	
	Variant(const String8& value);
	
	Variant(const String16& value);
	
	Variant(const AtomicString8& value);
	
	Variant(const AtomicString16& value);
	
	Variant(const sl_char8* sz8);
	
	Variant(const sl_char16* sz16);

	Variant(const Time& value);
	
	Variant(const void* ptr);
	
	template <class T>
	Variant(const Ref<T>& ref);
	
	template <class T>
	Variant(const AtomicRef<T>& ref);
	
	template <class T>
	Variant(const WeakRef<T>& weak);
	
	template <class T>
	Variant(const AtomicWeakRef<T>& weak);

	Variant(const Memory& mem);
	
	Variant(const AtomicMemory& mem);
	
	template <class T>
	Variant(const Array<T>& object);
	
	template <class T>
	Variant(const AtomicArray<T>& object);

	template <class T>
	Variant(const List<T>& object);
	
	template <class T>
	Variant(const AtomicList<T>& object);
	
	template <class KT, class VT>
	Variant(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	Variant(const AtomicMap<KT, VT>& object);

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
	
	template <class T>
	static Variant fromArray(const Array<T>& value);
	
	template <class T>
	static Variant fromList(const List<T>& value);
	
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
	
	Variant& operator=(AtomicVariant&& other);
	
	Variant& operator=(const AtomicVariant& other);
	
	Variant& operator=(sl_null_t);
	
	Variant& operator=(sl_int32 value);
	
	Variant& operator=(sl_uint32 value);
	
	Variant& operator=(sl_int64 value);
	
	Variant& operator=(sl_uint64 value);
	
	Variant& operator=(float value);
	
	Variant& operator=(double value);
	
	Variant& operator=(const sl_bool value);
	
	Variant& operator=(const String8& value);
	
	Variant& operator=(const AtomicString8& value);
	
	Variant& operator=(const String16& value);
	
	Variant& operator=(const AtomicString16& value);
	
	Variant& operator=(const sl_char8* sz8);
	
	Variant& operator=(const sl_char16* sz16);
	
	Variant& operator=(const Time& value);
	
	Variant& operator=(const void* ptr);
	
	template <class T>
	Variant& operator=(const Ref<T>& ref);
	
	template <class T>
	Variant& operator=(const AtomicRef<T>& ref);
	
	template <class T>
	Variant& operator=(const WeakRef<T>& weak);
	
	template <class T>
	Variant& operator=(const AtomicWeakRef<T>& weak);

	Variant& operator=(const Memory& mem);
	
	Variant& operator=(const AtomicMemory& mem);
	
	template <class T>
	Variant& operator=(const Array<T>& object);
	
	template <class T>
	Variant& operator=(const AtomicArray<T>& object);

	template <class T>
	Variant& operator=(const List<T>& object);
	
	template <class T>
	Variant& operator=(const AtomicList<T>& object);
	
	template <class KT, class VT>
	Variant& operator=(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	Variant& operator=(const AtomicMap<KT, VT>& object);
	
	Variant operator[](sl_size indexForVariantList) const;
	
	Variant operator[](const String& keyForVariantMap) const;
	
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
	
	void setString(const AtomicString8& value);
	
	void setString(const AtomicString16& value);
	
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
	
	sl_object_type getObjectType() const;
	
	sl_bool isMemory() const;
	
	Memory getMemory() const;
	
	void setMemory(const Memory& mem);
	
	template <class T>
	Array<T> getArray(const Array<T>& def) const;
	
	template <class T>
	void setArray(const Array<T>& array);

	template <class T>
	List<T> getList(const List<T>& def) const;
	
	template <class T>
	void setList(const List<T>& list);
	
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

	sl_size getListElementsCount() const;
	
	Variant getListElement(sl_size index) const;

	sl_bool setListElement(sl_size index, const Variant& value);

	sl_bool addListElement(const Variant& value);

	Variant getField(const String& key) const;

	sl_bool putField(const String& key, const Variant& value);

	
	String toString() const;

	String toJsonString() const;
	
private:
	static void _free(VariantType type, sl_uint64 value);
	
	friend class Atomic<Variant>;
	
};

template <>
class SLIB_EXPORT Atomic<Variant>
{
public:
	sl_uint64 _value;
	VariantType _type;
private:
	SpinLock m_lock;
	
public:
	SLIB_CONSTEXPR Atomic() : _value(0), _type(VariantType::Null) {}
	
	Atomic(AtomicVariant&& other);
	
	Atomic(const AtomicVariant& other);
	
	Atomic(Variant&& other);
	
	Atomic(const Variant& other);
	
	~Atomic();
	
public:
	Atomic(sl_null_t);
	
	Atomic(sl_int32 value);
	
	Atomic(sl_uint32 value);
	
	Atomic(sl_int64 value);
	
	Atomic(sl_uint64 value);
	
	Atomic(float value);
	
	Atomic(double value);
	
	Atomic(const sl_bool value);
	
	Atomic(const String8& value);

	Atomic(const String16& value);
	
	Atomic(const AtomicString8& value);
	
	Atomic(const AtomicString16& value);
	
	Atomic(const sl_char8* sz8);
	
	Atomic(const sl_char16* sz16);
	
	Atomic(const Time& value);
	
	Atomic(const void* ptr);

	template <class T>
	Atomic(const Ref<T>& ref);
	
	template <class T>
	Atomic(const AtomicRef<T>& ref);
	
	template <class T>
	Atomic(const WeakRef<T>& weak);
	
	template <class T>
	Atomic(const AtomicWeakRef<T>& weak);

	Atomic(const Memory& mem);
	
	Atomic(const AtomicMemory& mem);
	
	template <class T>
	Atomic(const Array<T>& object);
	
	template <class T>
	Atomic(const AtomicArray<T>& object);
	
	template <class T>
	Atomic(const List<T>& object);
	
	template <class T>
	Atomic(const AtomicList<T>& object);

	template <class KT, class VT>
	Atomic(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	Atomic(const AtomicMap<KT, VT>& object);
	
public:
	static const AtomicVariant& null();
	
public:
	AtomicVariant& operator=(AtomicVariant&& other);
	
	AtomicVariant& operator=(const AtomicVariant& other);
	
	AtomicVariant& operator=(Variant&& other);
	
	AtomicVariant& operator=(const Variant& other);
	
	AtomicVariant& operator=(sl_null_t);
	
	AtomicVariant& operator=(sl_int32 value);
	
	AtomicVariant& operator=(sl_uint32 value);
	
	AtomicVariant& operator=(sl_int64 value);
	
	AtomicVariant& operator=(sl_uint64 value);
	
	AtomicVariant& operator=(float value);
	
	AtomicVariant& operator=(double value);
	
	AtomicVariant& operator=(const sl_bool value);
	
	AtomicVariant& operator=(const String8& value);
	
	AtomicVariant& operator=(const AtomicString8& value);
	
	AtomicVariant& operator=(const String16& value);
	
	AtomicVariant& operator=(const AtomicString16& value);

	AtomicVariant& operator=(const sl_char8* sz8);
	
	AtomicVariant& operator=(const sl_char16* sz16);
	
	AtomicVariant& operator=(const Time& value);
	
	AtomicVariant& operator=(const void* ptr);
	
	template <class T>
	AtomicVariant& operator=(const Ref<T>& ref);
	
	template <class T>
	AtomicVariant& operator=(const AtomicRef<T>& ref);
	
	template <class T>
	AtomicVariant& operator=(const WeakRef<T>& weak);
	
	template <class T>
	AtomicVariant& operator=(const AtomicWeakRef<T>& weak);

	AtomicVariant& operator=(const Memory& mem);
	
	AtomicVariant& operator=(const AtomicMemory& mem);
	
	template <class T>
	AtomicVariant& operator=(const Array<T>& object);
	
	template <class T>
	AtomicVariant& operator=(const AtomicArray<T>& object);
	
	template <class T>
	AtomicVariant& operator=(const List<T>& object);
	
	template <class T>
	AtomicVariant& operator=(const AtomicList<T>& object);

	template <class KT, class VT>
	AtomicVariant& operator=(const Map<KT, VT>& object);
	
	template <class KT, class VT>
	AtomicVariant& operator=(const AtomicMap<KT, VT>& object);
	
	Variant operator[](sl_size indexForVariantList) const;
	
	Variant operator[](const String& keyForVariantMap) const;

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
	
	void setString(const AtomicString8& value);
	
	void setString(const String16& value);
	
	void setString(const AtomicString16& value);
	
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
	
	sl_object_type getObjectType() const;
	
	sl_bool isMemory() const;
	
	Memory getMemory() const;
	
	void setMemory(const Memory& mem);
	
	template <class T>
	Array<T> getArray(const Array<T>& def) const;
	
	template <class T>
	void setArray(const Array<T>& array);
	
	template <class T>
	List<T> getList(const List<T>& def) const;
	
	template <class T>
	void setList(const List<T>& list);
	
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
	
	sl_size getListElementsCount() const;
	
	Variant getListElement(sl_size index) const;
	
	sl_bool setListElement(sl_size index, const Variant& value);
	
	sl_bool addListElement(const Variant& value);
	
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
class Compare<Variant>
{
public:
	int operator()(const Variant &a, const Variant &b) const;
};


template <class... ARGS>
String8 String8::format(const sl_char8* szFormat, ARGS&&... args)
{
	Variant params[] = {Forward<ARGS>(args)...};
	return formatv(szFormat, params, sizeof...(args));
}

template <class... ARGS>
String8 String8::format(const String8& strFormat, ARGS&&... args)
{
	Variant params[] = {Forward<ARGS>(args)...};
	return formatv(strFormat, params, sizeof...(args));
}

template <class... ARGS>
String16 String16::format(const sl_char16* szFormat, ARGS&&... args)
{
	Variant params[] = {Forward<ARGS>(args)...};
	return formatv(szFormat, params, sizeof...(args));
}

template <class... ARGS>
String16 String16::format(const String16& strFormat, ARGS&&... args)
{
	Variant params[] = {Forward<ARGS>(args)...};
	return formatv(strFormat, params, sizeof...(args));
}

template <class... ARGS>
String8 String8::arg(ARGS&&... args) const
{
	Variant params[] = {Forward<ARGS>(args)...};
	return argv(params, sizeof...(args));
}

template <class... ARGS>
String16 String16::arg(ARGS&&... args) const
{
	Variant params[] = {Forward<ARGS>(args)...};
	return argv(params, sizeof...(args));
}

template <class... ARGS>
String8 Atomic<String8>::arg(ARGS&&... args) const
{
	Variant params[] = {Forward<ARGS>(args)...};
	return argv(params, sizeof...(args));
}

template <class... ARGS>
String16 Atomic<String16>::arg(ARGS&&... args) const
{
	Variant params[] = {Forward<ARGS>(args)...};
	return argv(params, sizeof...(args));
}

typedef List<Variant> VariantList;
typedef Map<String, Variant> VariantMap;
typedef List< Map<String, Variant> > VariantMapList;


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T>
Variant::Variant(const Ref<T>& ref)
{
	if (ref.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Ref<T>*>(&_value)) Ref<T>(ref);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const AtomicRef<T>& _ref)
{
	Ref<T> ref(_ref);
	if (ref.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Ref<T>*>(&_value)) Ref<T>(ref);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const WeakRef<T>& weak)
{
	if (weak.isNotNull()) {
		_type = VariantType::Weak;
		new (reinterpret_cast<WeakRef<T>*>(&_value)) WeakRef<T>(weak);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const AtomicWeakRef<T>& _weak)
{
	Ref<T> weak(_weak);
	if (weak.isNotNull()) {
		_type = VariantType::Weak;
		new (reinterpret_cast<WeakRef<T>*>(&_value)) WeakRef<T>(weak);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const Array<T>& object)
{
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Array<T>*>(&_value)) Array<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const AtomicArray<T>& _object)
{
	Array<T> object(_object);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Array<T>*>(&_value)) Array<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const List<T>& object)
{
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<List<T>*>(&_value)) List<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Variant::Variant(const AtomicList<T>& _object)
{
	List<T> object(_object);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<List<T>*>(&_value)) List<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class KT, class VT>
Variant::Variant(const Map<KT, VT>& object)
{
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Map<KT, VT>*>(&_value)) Map<KT, VT>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class KT, class VT>
Variant::Variant(const AtomicMap<KT, VT>& _object)
{
	Map<KT, VT> object(_object);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Map<KT, VT>*>(&_value)) Map<KT, VT>(object);
	} else {
		_type = VariantType::Null;
	}
}

SLIB_INLINE const Variant& Variant::null()
{
	return *(reinterpret_cast<Variant const*>(&_Variant_Null));
}

template <class T>
Variant Variant::fromRef(const Ref<T>& ref)
{
	return ref;
}

template <class T>
static Variant fromArray(const Array<T>& value)
{
	return value;
}

template <class T>
static Variant fromList(const List<T>& value)
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
Variant& Variant::operator=(const AtomicRef<T>& ref)
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
Variant& Variant::operator=(const AtomicWeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T>
Variant& Variant::operator=(const Array<T>& object)
{
	setArray(object);
	return *this;
}

template <class T>
Variant& Variant::operator=(const AtomicArray<T>& object)
{
	setArray(object);
	return *this;
}

template <class T>
Variant& Variant::operator=(const List<T>& object)
{
	setList(object);
	return *this;
}

template <class T>
Variant& Variant::operator=(const AtomicList<T>& object)
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
Variant& Variant::operator=(const AtomicMap<KT, VT>& object)
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
	return CastRef<T>(getObject(), def);
}

template <class T>
void Variant::setObject(const Ref<T>& ref)
{
	_free(_type, _value);
	if (ref.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Ref<T>*>(&_value)) Ref<T>(ref);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
void Variant::setWeak(const WeakRef<T>& weak)
{
	_free(_type, _value);
	if (weak.isNotNull()) {
		_type = VariantType::Weak;
		new (reinterpret_cast<WeakRef<T>*>(&_value)) WeakRef<T>(weak);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Array<T> Variant::getArray(const Array<T>& def) const
{
	Ref<Referable> obj(getObject());
	if (CArray<T>* p = CastInstance< CArray<T> >(obj._ptr)) {
		return p;
	}
	return def;
}

template <class T>
void Variant::setArray(const Array<T>& object)
{
	_free(_type, _value);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Array<T>*>(&_value)) Array<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
List<T> Variant::getList(const List<T>& def) const
{
	Ref<Referable> obj(getObject());
	if (CList<T>* p = CastInstance< CList<T> >(obj._ptr)) {
		return p;
	}
	return def;
}

template <class T>
void Variant::setList(const List<T>& object)
{
	_free(_type, _value);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<List<T>*>(&_value)) List<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class KT, class VT>
Map<KT, VT> Variant::getMap(const Map<KT, VT>& def) const
{
	Ref<Referable> obj(getObject());
	if (IMap<KT, VT>* p = CastInstance< IMap<KT, VT> >(obj._ptr)) {
		return p;
	}
	return def;
}

template <class KT, class VT>
void Variant::setMap(const Map<KT, VT>& object)
{
	_free(_type, _value);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Map<KT, VT>*>(&_value)) Map<KT, VT>(object);
	} else {
		_type = VariantType::Null;
	}
}


template <class T>
Atomic<Variant>::Atomic(const Ref<T>& ref)
{
	if (ref.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Ref<T>*>(&_value)) Ref<T>(ref);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const AtomicRef<T>& _ref)
{
	Ref<T> ref(_ref);
	if (ref.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Ref<T>*>(&_value)) Ref<T>(ref);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const WeakRef<T>& weak)
{
	if (weak.isNotNull()) {
		_type = VariantType::Weak;
		new (reinterpret_cast<WeakRef<T>*>(&_value)) WeakRef<T>(weak);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const AtomicWeakRef<T>& _weak)
{
	WeakRef<T> weak(_weak);
	if (weak.isNotNull()) {
		_type = VariantType::Weak;
		new (reinterpret_cast<WeakRef<T>*>(&_value)) WeakRef<T>(weak);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const Array<T>& object)
{
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Array<T>*>(&_value)) Array<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const AtomicArray<T>& _object)
{
	Array<T> object(_object);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Array<T>*>(&_value)) Array<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const List<T>& object)
{
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<List<T>*>(&_value)) List<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class T>
Atomic<Variant>::Atomic(const AtomicList<T>& _object)
{
	List<T> object(_object);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<List<T>*>(&_value)) List<T>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class KT, class VT>
Atomic<Variant>::Atomic(const Map<KT, VT>& object)
{
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Map<KT, VT>*>(&_value)) Map<KT, VT>(object);
	} else {
		_type = VariantType::Null;
	}
}

template <class KT, class VT>
Atomic<Variant>::Atomic(const AtomicMap<KT, VT>& _object)
{
	Map<KT, VT> object(_object);
	if (object.isNotNull()) {
		_type = VariantType::Object;
		new (reinterpret_cast<Map<KT, VT>*>(&_value)) Map<KT, VT>(object);
	} else {
		_type = VariantType::Null;
	}
}

SLIB_INLINE const AtomicVariant& Atomic<Variant>::null()
{
	return *(reinterpret_cast<AtomicVariant const*>(&_Variant_Null));
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const Ref<T>& ref)
{
	setObject(ref);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const AtomicRef<T>& ref)
{
	setObject(ref);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const WeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const AtomicWeakRef<T>& weak)
{
	setWeak(weak);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const Array<T>& object)
{
	setArray(object);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const AtomicArray<T>& object)
{
	setArray(object);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const List<T>& object)
{
	setList(object);
	return *this;
}

template <class T>
AtomicVariant& Atomic<Variant>::operator=(const AtomicList<T>& object)
{
	setList(object);
	return *this;
}

template <class KT, class VT>
AtomicVariant& Atomic<Variant>::operator=(const Map<KT, VT>& object)
{
	setMap(object);
	return *this;
}

template <class KT, class VT>
AtomicVariant& Atomic<Variant>::operator=(const AtomicMap<KT, VT>& object)
{
	setMap(object);
	return *this;
}

SLIB_INLINE VariantType Atomic<Variant>::getType() const
{
	return _type;
}

SLIB_INLINE sl_bool Atomic<Variant>::isNull() const
{
	return _type == VariantType::Null;
}

SLIB_INLINE sl_bool Atomic<Variant>::isNotNull() const
{
	return _type != VariantType::Null;
}

template <class T>
Ref<T> Atomic<Variant>::getObject(const Ref<T>& def) const
{
	Variant var(*this);
	return var.getObject(def);
}

template <class T>
void Atomic<Variant>::setObject(const Ref<T>& object)
{
	if (object.isNotNull()) {
		sl_int64 v;
		new (reinterpret_cast<Ref<T>*>(&v)) Ref<T>(object);
		_replace(VariantType::Object, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

template <class T>
void Atomic<Variant>::setWeak(const WeakRef<T>& weak)
{
	if (weak.isNotNull()) {
		sl_int64 v;
		new (reinterpret_cast<WeakRef<T>*>(&v)) WeakRef<T>(weak);
		_replace(VariantType::Weak, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

template <class T>
Array<T> Atomic<Variant>::getArray(const Array<T>& def) const
{
	Variant var(*this);
	return var.getArray(def);
}

template <class T>
void Atomic<Variant>::setArray(const Array<T>& object)
{
	if (object.isNotNull()) {
		sl_int64 v;
		new (reinterpret_cast<Array<T>*>(&v)) Array<T>(object);
		_replace(VariantType::Object, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

template <class T>
List<T> Atomic<Variant>::getList(const List<T>& def) const
{
	Variant var(*this);
	return var.getList(def);
}

template <class T>
void Atomic<Variant>::setList(const List<T>& object)
{
	if (object.isNotNull()) {
		sl_int64 v;
		new (reinterpret_cast<List<T>*>(&v)) List<T>(object);
		_replace(VariantType::Object, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

template <class KT, class VT>
Map<KT, VT> Atomic<Variant>::getMap(const Map<KT, VT>& def) const
{
	Variant var(*this);
	return var.getMap(def);
}

template <class KT, class VT>
void Atomic<Variant>::setMap(const Map<KT, VT>& object)
{
	if (object.isNotNull()) {
		sl_int64 v;
		new (reinterpret_cast<Map<KT, VT>*>(&v)) Map<KT, VT>(object);
		_replace(VariantType::Object, v);
	} else {
		_replace(VariantType::Null, 0);
	}
}

extern const char _VariantMap_ClassID[];

template <>
SLIB_INLINE sl_object_type IMap<String, Variant>::ObjectType()
{
	return _VariantMap_ClassID;
}

template <>
SLIB_INLINE sl_bool IMap<String, Variant>::checkObjectType(sl_object_type type)
{
	if (type == _VariantMap_ClassID || type == _Map_ClassID) {
		return sl_true;
	}
	return Object::checkObjectType(type);
}

template <>
SLIB_INLINE sl_object_type IMap<String, Variant>::getObjectType() const
{
	return _VariantMap_ClassID;
}

template <>
SLIB_INLINE sl_bool IMap<String, Variant>::isInstanceOf(sl_object_type type) const
{
	if (type == _VariantMap_ClassID || type == _Map_ClassID) {
		return sl_true;
	}
	return Object::checkObjectType(type);
}


extern const char _VariantList_ClassID[];

template <>
SLIB_INLINE sl_object_type CList<Variant>::ObjectType()
{
	return _VariantList_ClassID;
}

template <>
SLIB_INLINE sl_bool CList<Variant>::checkObjectType(sl_object_type type)
{
	if (type == _VariantList_ClassID || type == _List_ClassID) {
		return sl_true;
	}
	return Object::checkObjectType(type);
}

template <>
SLIB_INLINE sl_object_type CList<Variant>::getObjectType() const
{
	return _VariantList_ClassID;
}

template <>
SLIB_INLINE sl_bool CList<Variant>::isInstanceOf(sl_object_type type) const
{
	if (type == _VariantList_ClassID || type == _List_ClassID) {
		return sl_true;
	}
	return Object::checkObjectType(type);
}


extern const char _VariantMapList_ClassID[];

template <>
SLIB_INLINE sl_object_type CList< Map<String, Variant> >::ObjectType()
{
	return _VariantMapList_ClassID;
}

template <>
SLIB_INLINE sl_bool CList< Map<String, Variant> >::checkObjectType(sl_object_type type)
{
	if (type == _VariantMapList_ClassID || type == _Map_ClassID) {
		return sl_true;
	}
	return Object::checkObjectType(type);
}

template <>
SLIB_INLINE sl_object_type CList< Map<String, Variant> >::getObjectType() const
{
	return _VariantMapList_ClassID;
}

template <>
SLIB_INLINE sl_bool CList< Map<String, Variant> >::isInstanceOf(sl_object_type type) const
{
	if (type == _VariantMapList_ClassID || type == _Map_ClassID) {
		return sl_true;
	}
	return Object::checkObjectType(type);
}

SLIB_NAMESPACE_END

#endif
