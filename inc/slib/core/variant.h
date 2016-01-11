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

struct _Variant_Const
{
	sl_uint64 value;
	int type;
	sl_int32 lock;
};

extern const _Variant_Const _Variant_Null;

class SafeVariant;

class SLIB_EXPORT Variant
{
public:
	enum Type {
		typeNull = 0,
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
	sl_uint64 m_value;
	Type m_type;

public:
	SLIB_INLINE Variant()
	{
		m_type = typeNull;
	}

	Variant(const Variant& other);
	
	Variant(const SafeVariant& other);

	~Variant();
	
public:
	SLIB_INLINE Variant(sl_int32 value)
	{
		m_type = typeInt32;
		*(sl_int32*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(sl_uint32 value)
	{
		m_type = typeUint32;
		*(sl_uint32*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(sl_int64 value)
	{
		m_type = typeInt64;
		*(sl_int64*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(sl_uint64 value)
	{
		m_type = typeUint64;
		*(sl_uint64*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(float value)
	{
		m_type = typeFloat;
		*(float*)(void*)(&m_value) = value;
	}

	SLIB_INLINE Variant(double value)
	{
		m_type = typeDouble;
		*(double*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(const Time& value)
	{
		m_type = typeTime;
		*(Time*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(const void* ptr)
	{
		m_type = typePointer;
		*(const void**)(void*)(&m_value) = ptr;
	}
	
	SLIB_INLINE Variant(const sl_bool value)
	{
		m_type = typeBoolean;
		*(sl_bool*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE Variant(const String8& value)
	{
		m_type = typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
	}
	
	SLIB_INLINE Variant(const SafeString8& value)
	{
		m_type = typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
	}
	
	SLIB_INLINE Variant(const String16& value)
	{
		m_type = typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
	}
	
	SLIB_INLINE Variant(const SafeString16& value)
	{
		m_type = typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
	}

	template <class T>
	SLIB_INLINE Variant(const Ref<T>& ref)
	{
		m_type = typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
	}
	
	template <class T>
	SLIB_INLINE Variant(const SafeRef<T>& ref)
	{
		m_type = typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
	}
	
	SLIB_INLINE Variant(const Memory& mem)
	{
		m_type = typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
	}
	
	SLIB_INLINE Variant(const SafeMemory& mem)
	{
		m_type = typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE Variant(const Array<T, COMPARE>& object)
	{
		m_type = typeObject;
		new ((Array<T, COMPARE>*)(void*)(&m_value)) Array<T, COMPARE>(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE Variant(const SafeArray<T, COMPARE>& object)
	{
		m_type = typeObject;
		new ((Array<T, COMPARE>*)(void*)(&m_value)) Array<T, COMPARE>(object);
	}

	template <class T, class COMPARE>
	SLIB_INLINE Variant(const List<T, COMPARE>& object)
	{
		m_type = typeObject;
		new ((List<T, COMPARE>*)(void*)(&m_value)) List<T, COMPARE>(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE Variant(const SafeList<T, COMPARE>& object)
	{
		m_type = typeObject;
		new ((List<T, COMPARE>*)(void*)(&m_value)) List<T, COMPARE>(object);
	}
	
	template <class KT, class VT>
	SLIB_INLINE Variant(const Map<KT, VT>& object)
	{
		m_type = typeObject;
		new ((Map<KT, VT>*)(void*)(&m_value)) Map<KT, VT>(object);
	}
	
	template <class KT, class VT>
	SLIB_INLINE Variant(const SafeMap<KT, VT>& object)
	{
		m_type = typeObject;
		new ((Map<KT, VT>*)(void*)(&m_value)) Map<KT, VT>(object);
	}
	
	template <class T>
	SLIB_INLINE Variant(const WeakRef<T>& weak)
	{
		m_type = typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
	}
	
	template <class T>
	SLIB_INLINE Variant(const SafeWeakRef<T>& weak)
	{
		m_type = typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
	}

public:
	SLIB_INLINE static const Variant& null()
	{
		return *((Variant*)((void*)(&_Variant_Null)));
	}

	SLIB_INLINE static Variant fromInt32(sl_int32 value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromUint32(sl_uint32 value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromInt64(sl_int64 value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromUint64(sl_uint64 value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromFloat(float value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromDouble(double value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromTime(const Time& value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromPointer(const void* value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromBoolean(sl_bool value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromString(const String8& value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromString(const String16& value)
	{
		return value;
	}
	
	template <class T>
	SLIB_INLINE static Variant fromRef(const Ref<T>& ref)
	{
		return ref;
	}
	
	SLIB_INLINE static Variant fromMemory(const Memory& mem)
	{
		return mem;
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE static Variant fromArray(const Array<T, COMPARE>& value)
	{
		return value;
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE static Variant fromList(const List<T, COMPARE>& value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant fromVariantList(const List<Variant>& value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant createVariantList()
	{
		return List<Variant>::create();
	}

	template <class KT, class VT>
	SLIB_INLINE static Variant fromMap(const Map<KT, VT>& value)
	{
		return value;
	}

	SLIB_INLINE static Variant fromVariantMap(const Map<String, Variant>& value)
	{
		return value;
	}
	
	SLIB_INLINE static Variant createVariantListMap()
	{
		return Map<String, Variant>::createList();
	}
	
	SLIB_INLINE static Variant createVariantTreeMap()
	{
		return Map<String, Variant>::createTree();
	}
	
	SLIB_INLINE static Variant createVariantHashMap()
	{
		return Map<String, Variant>::createHash();
	}
	
	template <class T>
	SLIB_INLINE static Variant fromWeakRef(const WeakRef<T>& weak)
	{
		return weak;
	}
	
public:
	Variant& operator=(const Variant& other);
	
	Variant& operator=(const SafeVariant& other);
	
public:
	SLIB_INLINE Variant& operator=(sl_int32 value)
	{
		_free(m_type, m_value);
		m_type = typeInt32;
		*(sl_int32*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(sl_uint32 value)
	{
		_free(m_type, m_value);
		m_type = typeUint32;
		*(sl_uint32*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(sl_int64 value)
	{
		_free(m_type, m_value);
		m_type = typeInt64;
		*(sl_int64*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(sl_uint64 value)
	{
		_free(m_type, m_value);
		m_type = typeUint64;
		*(sl_uint64*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(float value)
	{
		_free(m_type, m_value);
		m_type = typeFloat;
		*(float*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(double value)
	{
		_free(m_type, m_value);
		m_type = typeDouble;
		*(double*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const Time& value)
	{
		_free(m_type, m_value);
		m_type = typeTime;
		*(Time*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const void* ptr)
	{
		_free(m_type, m_value);
		m_type = typePointer;
		*(const void**)(void*)(&m_value) = ptr;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const sl_bool value)
	{
		_free(m_type, m_value);
		m_type = typeBoolean;
		*(sl_bool*)(void*)(&m_value) = value;
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const String8& value)
	{
		_free(m_type, m_value);
		m_type = typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const SafeString8& value)
	{
		_free(m_type, m_value);
		m_type = typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const String16& value)
	{
		_free(m_type, m_value);
		m_type = typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const SafeString16& value)
	{
		_free(m_type, m_value);
		m_type = typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE Variant& operator=(const Ref<T>& ref)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE Variant& operator=(const SafeRef<T>& ref)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const Memory& mem)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
		return *this;
	}
	
	SLIB_INLINE Variant& operator=(const SafeMemory& mem)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
		return *this;
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE Variant& operator=(const Array<T, COMPARE>& object)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Array<T, COMPARE>*)(void*)(&m_value)) Array<T, COMPARE>(object);
		return *this;
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE Variant& operator=(const SafeArray<T, COMPARE>& object)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Array<T, COMPARE>*)(void*)(&m_value)) Array<T, COMPARE>(object);
		return *this;
	}

	template <class T, class COMPARE>
	SLIB_INLINE Variant& operator=(const List<T, COMPARE>& object)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((List<T, COMPARE>*)(void*)(&m_value)) List<T, COMPARE>(object);
		return *this;
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE Variant& operator=(const SafeList<T, COMPARE>& object)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((List<T, COMPARE>*)(void*)(&m_value)) List<T, COMPARE>(object);
		return *this;
	}
	
	template <class KT, class VT>
	SLIB_INLINE Variant& operator=(const Map<KT, VT>& object)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Map<KT, VT>*)(void*)(&m_value)) Map<KT, VT>(object);
		return *this;
	}
	
	template <class KT, class VT>
	SLIB_INLINE Variant& operator=(const SafeMap<KT, VT>& object)
	{
		_free(m_type, m_value);
		m_type = typeObject;
		new ((Map<KT, VT>*)(void*)(&m_value)) Map<KT, VT>(object);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE Variant& operator=(const WeakRef<T>& weak)
	{
		_free(m_type, m_value);
		m_type = typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE Variant& operator=(const SafeWeakRef<T>& weak)
	{
		_free(m_type, m_value);
		m_type = typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
		return *this;
	}
	
public:
	friend sl_bool operator==(const Variant& v1, const Variant& v2);

public:
	SLIB_INLINE Type getType() const
	{
		return m_type;
	}
	
	SLIB_INLINE void setNull()
	{
		*this = Variant::null();
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_type == typeNull;
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_type != typeNull;
	}
	
public:
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

	SLIB_INLINE sl_bool isNumber() const
	{
		return isInteger() || m_type == typeFloat || m_type == typeDouble;
	}

public:
	SLIB_INLINE sl_bool isTime() const
	{
		return m_type == typeTime;
	}

	Time getTime(Time def) const;

	Time getTime() const
	{
		return getTime(Time::zero());
	}

public:
	SLIB_INLINE sl_bool isPointer() const
	{
		return m_type == typePointer;
	}

	const void* getPointer(const void* def = sl_null) const;

public:
	SLIB_INLINE sl_bool isBoolean() const
	{
		return m_type == typeBoolean;
	}

	sl_bool getBoolean(sl_bool def) const;

public:
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

public:
	SLIB_INLINE sl_bool isObject() const
	{
		return m_type == typeObject;
	}

	SLIB_INLINE Ref<Referable> getObject() const
	{
		if (m_type == typeObject) {
			return *((Ref<Referable>*)(void*)(&m_value));
		}
		return Ref<Referable>::null();
	}

	template <class T>
	SLIB_INLINE Ref<T> getObject(const Ref<T>& def) const
	{
		if (m_type == typeObject) {
			return *((Ref<T>*)(void*)(&m_value));
		}
		return def;
	}
	
	SLIB_INLINE sl_bool isObjectNotNull() const
	{
		if (m_type == typeObject) {
			return (*((Ref<Referable>*)(void*)(&m_value))).isNotNull();
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool isObjectNull() const
	{
		if (m_type == typeObject) {
			return (*((Ref<Referable>*)(void*)(&m_value))).isNull();
		}
		return sl_true;
	}
	
	SLIB_INLINE sl_class_type getObjectClassType() const
	{
		if (m_type == typeObject) {
			return (*((Referable**)(void*)(&m_value)))->getClassType();
		}
		return 0;
	}
	
	SLIB_INLINE const char* getObjectClassTypeName() const
	{
		if (m_type == typeObject) {
			return (*((Referable**)(void*)(&m_value)))->getClassTypeName();
		}
		return 0;
	}

	SLIB_INLINE sl_bool isMemory() const
	{
		if (m_type == typeObject) {
			return Memory::checkInstance(*((Referable**)(void*)(&m_value)));
		}
		return sl_false;
	}
	
	SLIB_INLINE Memory getMemory() const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (Memory::checkInstance(ref)) {
				return (CMemory*)ref;
			}
		}
		return Memory::null();
	}

	SLIB_INLINE sl_bool isVariantList() const
	{
		if (m_type == typeObject) {
			return List<Variant>::checkInstance(*((Referable**)(void*)(&m_value)));
		}
		return sl_false;
	}
	
	SLIB_INLINE List<Variant> getVariantList() const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (List<Variant>::checkInstance(ref)) {
				return (CList<Variant>*)ref;
			}
		}
		return List<Variant>::null();
	}
	
	template <class TYPE, class COMPARE>
	SLIB_INLINE const List<TYPE, COMPARE>& getList(const List<TYPE, COMPARE>& def) const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (List<TYPE, COMPARE>::checkInstance(ref)) {
				return (CList<TYPE, COMPARE>*)ref;
			}
		}
		return def;
	}

	SLIB_INLINE sl_bool isVariantMap() const
	{
		if (m_type == typeObject) {
			return Map<String, Variant>::checkInstance(*((Referable**)(void*)(&m_value)));
		}
		return sl_false;
	}
	
	SLIB_INLINE Map<String, Variant> getVariantMap() const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (Map<String, Variant>::checkInstance(ref)) {
				return (IMap<String, Variant>*)ref;
			}
		}
		return Map<String, Variant>::null();
	}
	
	template <class KT, class VT>
	SLIB_INLINE const Map<KT, VT>& getMap(const Map<KT, VT>& def) const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (Map<KT, VT>::checkInstance(ref)) {
				return (IMap<KT, VT>*)ref;
			}
		}
		return def;
	}
	
	SLIB_INLINE sl_size getListItemsCount() const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (List<Variant>::checkInstance(ref)) {
				return ((CList<Variant>*)ref)->count();
			}
		}
		return 0;

	}

	SLIB_INLINE Variant getListItem(sl_size index) const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (List<Variant>::checkInstance(ref)) {
				return ((CList<Variant>*)ref)->getItemValue(index, Variant::null());
			}
		}
		return Variant::null();
	}

	SLIB_INLINE sl_bool setListItem(sl_size index, const Variant& value)
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (List<Variant>::checkInstance(ref)) {
				return ((CList<Variant>*)ref)->setItem(index, value);
			}
		}
		return sl_false;
	}

	SLIB_INLINE sl_bool addListItem(const Variant& value)
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (List<Variant>::checkInstance(ref)) {
				return ((CList<Variant>*)ref)->add(value);
			}
		}
		return sl_false;
	}

	SLIB_INLINE Variant getField(const String& key) const
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (Map<String, Variant>::checkInstance(ref)) {
				return ((IMap<String, Variant>*)ref)->getValue(key, Variant::null());
			}
		}
		return Variant::null();
	}

	SLIB_INLINE sl_bool putField(const String& key, const Variant& value)
	{
		if (m_type == typeObject) {
			Referable* ref = *((Referable**)(void*)(&m_value));
			if (Map<String, Variant>::checkInstance(ref)) {
				return ((IMap<String, Variant>*)ref)->put(key, value);
			}
		}
		return sl_false;
	}

public:
	SLIB_INLINE sl_bool isWeak() const
	{
		return m_type == typeWeak;
	}
	
	template <class T>
	SLIB_INLINE const WeakRef<T> getWeak(const WeakRef<T>& def) const
	{
		if (m_type == typeWeak) {
			return *((WeakRef<T>*)(void*)(&m_value));
		}
		return def;
	}
	
public:
	String toString() const;

	String toJSON() const;

	
private:
	static void _copy(Type src_type, sl_uint64 src_value, sl_uint64& dst_value);
	
	static void _free(Type type, sl_uint64 value);
	
	friend class SafeVariant;
};


class SLIB_EXPORT SafeVariant
{
private:
	sl_uint64 m_value;
	Variant::Type m_type;
	SpinLock m_lock;
	
public:
	SLIB_INLINE SafeVariant()
	{
		m_type = Variant::typeNull;
	}
	
	SafeVariant(const SafeVariant& other);
	
	SafeVariant(const Variant& other);
	
	~SafeVariant();
	
public:
	SLIB_INLINE SafeVariant(sl_int32 value)
	{
		m_type = Variant::typeInt32;
		*(sl_int32*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(sl_uint32 value)
	{
		m_type = Variant::typeUint32;
		*(sl_uint32*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(sl_int64 value)
	{
		m_type = Variant::typeInt64;
		*(sl_int64*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(sl_uint64 value)
	{
		m_type = Variant::typeUint64;
		*(sl_uint64*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(float value)
	{
		m_type = Variant::typeFloat;
		*(float*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(double value)
	{
		m_type = Variant::typeDouble;
		*(double*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(const Time& value)
	{
		m_type = Variant::typeTime;
		*(Time*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(const void* ptr)
	{
		m_type = Variant::typePointer;
		*(const void**)(void*)(&m_value) = ptr;
	}
	
	SLIB_INLINE SafeVariant(const sl_bool value)
	{
		m_type = Variant::typeBoolean;
		*(sl_bool*)(void*)(&m_value) = value;
	}
	
	SLIB_INLINE SafeVariant(const String8& value)
	{
		m_type = Variant::typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
	}
	
	SLIB_INLINE SafeVariant(const SafeString8& value)
	{
		m_type = Variant::typeString8;
		new ((String8*)(void*)(&m_value)) String8(value);
	}

	SLIB_INLINE SafeVariant(const String16& value)
	{
		m_type = Variant::typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
	}
	
	SLIB_INLINE SafeVariant(const SafeString16& value)
	{
		m_type = Variant::typeString16;
		new ((String16*)(void*)(&m_value)) String16(value);
	}

	template <class T>
	SLIB_INLINE SafeVariant(const Ref<T>& ref)
	{
		m_type = Variant::typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant(const SafeRef<T>& ref)
	{
		m_type = Variant::typeObject;
		new ((Ref<T>*)(void*)(&m_value)) Ref<T>(ref);
	}
	
	SLIB_INLINE SafeVariant(const Memory& mem)
	{
		m_type = Variant::typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
	}
	
	SLIB_INLINE SafeVariant(const SafeMemory& mem)
	{
		m_type = Variant::typeObject;
		new ((Memory*)(void*)(&m_value)) Memory(mem);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant(const Array<T, COMPARE>& object)
	{
		m_type = Variant::typeObject;
		new ((Array<T, COMPARE>*)(void*)(&m_value)) Array<T, COMPARE>(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant(const SafeArray<T, COMPARE>& object)
	{
		m_type = Variant::typeObject;
		new ((Array<T, COMPARE>*)(void*)(&m_value)) Array<T, COMPARE>(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant(const List<T, COMPARE>& object)
	{
		m_type = Variant::typeObject;
		new ((List<T, COMPARE>*)(void*)(&m_value)) List<T, COMPARE>(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant(const SafeList<T, COMPARE>& object)
	{
		m_type = Variant::typeObject;
		new ((List<T, COMPARE>*)(void*)(&m_value)) List<T, COMPARE>(object);
	}

	template <class KT, class VT>
	SLIB_INLINE SafeVariant(const Map<KT, VT>& object)
	{
		m_type = Variant::typeObject;
		new ((Map<KT, VT>*)(void*)(&m_value)) Map<KT, VT>(object);
	}
	
	template <class KT, class VT>
	SLIB_INLINE SafeVariant(const SafeMap<KT, VT>& object)
	{
		m_type = Variant::typeObject;
		new ((Map<KT, VT>*)(void*)(&m_value)) Map<KT, VT>(object);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant(const WeakRef<T>& weak)
	{
		m_type = Variant::typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant(const SafeWeakRef<T>& weak)
	{
		m_type = Variant::typeWeak;
		new ((WeakRef<T>*)(void*)(&m_value)) WeakRef<T>(weak);
	}
	
public:
	SLIB_INLINE static const SafeVariant& null()
	{
		return *((SafeVariant*)((void*)(&_Variant_Null)));
	}
	
public:
	SafeVariant& operator=(const SafeVariant& other);
	
	SafeVariant& operator=(const Variant& other);
	
public:
	SLIB_INLINE SafeVariant& operator=(sl_int32 value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(sl_uint32 value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(sl_int64 value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(sl_uint64 value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(float value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(double value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(const Time& value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(const void* ptr)
	{
		return *this = Variant(ptr);
	}
	
	SLIB_INLINE SafeVariant& operator=(const sl_bool value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(const String8& value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(const SafeString8& value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(const String16& value)
	{
		return *this = Variant(value);
	}
	
	SLIB_INLINE SafeVariant& operator=(const SafeString16& value)
	{
		return *this = Variant(value);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant& operator=(const Ref<T>& ref)
	{
		return *this = Variant(ref);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant& operator=(const SafeRef<T>& ref)
	{
		return *this = Variant(ref);
	}
	
	SLIB_INLINE SafeVariant& operator=(const Memory& mem)
	{
		return *this = Variant(mem);
	}
	
	SLIB_INLINE SafeVariant& operator=(const SafeMemory& mem)
	{
		return *this = Variant(mem);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant& operator=(const Array<T, COMPARE>& object)
	{
		return *this = Variant(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant& operator=(const SafeArray<T, COMPARE>& object)
	{
		return *this = Variant(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant& operator=(const List<T, COMPARE>& object)
	{
		return *this = Variant(object);
	}
	
	template <class T, class COMPARE>
	SLIB_INLINE SafeVariant& operator=(const SafeList<T, COMPARE>& object)
	{
		return *this = Variant(object);
	}

	template <class KT, class VT>
	SLIB_INLINE SafeVariant& operator=(const Map<KT, VT>& object)
	{
		return *this = Variant(object);
	}
	
	template <class KT, class VT>
	SLIB_INLINE SafeVariant& operator=(const SafeMap<KT, VT>& object)
	{
		return *this = Variant(object);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant& operator=(const WeakRef<T>& weak)
	{
		return *this = Variant(weak);
	}
	
	template <class T>
	SLIB_INLINE SafeVariant& operator=(const SafeWeakRef<T>& weak)
	{
		return *this = Variant(weak);
	}
	
public:
	SLIB_INLINE Variant::Type getType() const
	{
		return m_type;
	}
	
	SLIB_INLINE void setNull()
	{
		*this = Variant::null();
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_type == Variant::typeNull;
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_type != Variant::typeNull;
	}
	
public:
	SLIB_INLINE sl_bool isInt32() const
	{
		return m_type == Variant::typeInt32;
	}
	
	SLIB_INLINE sl_int32 getInt32(sl_int32 def = 0) const
	{
		Variant var(*this);
		return var.getInt32(def);
	}
	
	SLIB_INLINE sl_bool isUint32() const
	{
		return m_type == Variant::typeUint32;
	}
	
	SLIB_INLINE sl_uint32 getUint32(sl_uint32 def = 0) const
	{
		Variant var(*this);
		return var.getUint32(def);
	}
	
	SLIB_INLINE sl_bool isInt64() const
	{
		return m_type == Variant::typeInt64;
	}
	
	SLIB_INLINE sl_int64 getInt64(sl_int64 def = 0) const
	{
		Variant var(*this);
		return var.getInt64(def);
	}
	
	SLIB_INLINE sl_bool isUint64() const
	{
		return m_type == Variant::typeUint64;
	}
	
	SLIB_INLINE sl_uint64 getUint64(sl_uint64 def = 0) const
	{
		Variant var(*this);
		return var.getUint64(def);
	}
	
	SLIB_INLINE sl_bool isInteger() const
	{
		return m_type == Variant::typeInt32 || m_type == Variant::typeUint32 || m_type == Variant::typeInt64 || m_type == Variant::typeUint64;
	}
	
	SLIB_INLINE sl_bool isFloat() const
	{
		return m_type == Variant::typeFloat;
	}
	
	SLIB_INLINE float getFloat(float def = 0) const
	{
		Variant var(*this);
		return var.getFloat(def);
	}
	
	SLIB_INLINE sl_bool isDouble() const
	{
		return m_type == Variant::typeDouble;
	}
	
	SLIB_INLINE double getDouble(double def = 0) const
	{
		Variant var(*this);
		return var.getDouble(def);
	}
	
	SLIB_INLINE sl_bool isNumber() const
	{
		return isInteger() || m_type == Variant::typeFloat || m_type == Variant::typeDouble;
	}
	
public:
	SLIB_INLINE sl_bool isTime() const
	{
		return m_type == Variant::typeTime;
	}
	
	SLIB_INLINE Time getTime(Time def) const
	{
		Variant var(*this);
		return var.getTime(def);
	}
	
	Time getTime() const
	{
		return getTime(Time::zero());
	}
	
public:
	SLIB_INLINE sl_bool isPointer() const
	{
		return m_type == Variant::typePointer;
	}
	
	SLIB_INLINE const void* getPointer(const void* def = sl_null) const
	{
		Variant var(*this);
		return var.getPointer(def);
	}
	
public:
	SLIB_INLINE sl_bool isBoolean() const
	{
		return m_type == Variant::typeBoolean;
	}
	
	SLIB_INLINE sl_bool getBoolean(sl_bool def) const
	{
		Variant var(*this);
		return var.getBoolean(def);
	}
	
public:
	SLIB_INLINE sl_bool isString() const
	{
		return m_type == Variant::typeString8 || m_type == Variant::typeString16;
	}
	
	SLIB_INLINE sl_bool isString8() const
	{
		return m_type == Variant::typeString8;
	}
	
	SLIB_INLINE sl_bool isString16() const
	{
		return m_type == Variant::typeString16;
	}
	
	SLIB_INLINE String getString(const String& def) const
	{
		Variant var(*this);
		return var.getString(def);
	}
	
	SLIB_INLINE String16 getString16(const String16& def) const
	{
		Variant var(*this);
		return var.getString(def);
	}
	
	SLIB_INLINE String getString() const
	{
		return getString(String::null());
	}
	
	SLIB_INLINE String16 getString16() const
	{
		return getString16(String16::null());
	}
	
public:
	SLIB_INLINE sl_bool isObject() const
	{
		return m_type == Variant::typeObject;
	}
	
	SLIB_INLINE Ref<Referable> getObject() const
	{
		Variant var(*this);
		return var.getObject();
	}
	
	template <class T>
	SLIB_INLINE Ref<T> getObject(const Ref<T>& def) const
	{
		Variant var(*this);
		return var.getObject(def);
	}
	
	SLIB_INLINE sl_bool isObjectNotNull() const
	{
		Variant var(*this);
		return var.isObjectNotNull();
	}
	
	SLIB_INLINE sl_bool isObjectNull() const
	{
		Variant var(*this);
		return var.isObjectNull();
	}
	
	SLIB_INLINE sl_class_type getObjectClassType() const
	{
		Variant var(*this);
		return var.getObjectClassType();
	}
	
	SLIB_INLINE const char* getObjectClassTypeName() const
	{
		Variant var(*this);
		return var.getObjectClassTypeName();
	}
	
	SLIB_INLINE sl_bool isMemory() const
	{
		Variant var(*this);
		return var.isMemory();
	}
	
	SLIB_INLINE Memory getMemory() const
	{
		Variant var(*this);
		return var.getMemory();
	}
	
	SLIB_INLINE sl_bool isVariantList() const
	{
		Variant var(*this);
		return var.isVariantList();
	}
	
	SLIB_INLINE List<Variant> getVariantList() const
	{
		Variant var(*this);
		return var.getVariantList();
	}
	
	template <class TYPE, class COMPARE>
	SLIB_INLINE const List<TYPE, COMPARE>& getList(const List<TYPE, COMPARE>& def) const
	{
		Variant var(*this);
		return var.getList(def);
	}
	
	SLIB_INLINE sl_bool isVariantMap() const
	{
		Variant var(*this);
		return var.isVariantMap();
	}
	
	SLIB_INLINE Map<String, Variant> getVariantMap() const
	{
		Variant var(*this);
		return var.getVariantMap();
	}
	
	template <class KT, class VT>
	SLIB_INLINE const Map<KT, VT>& getMap(const Map<KT, VT>& def) const
	{
		Variant var(*this);
		return var.getMap(def);
	}
	
	SLIB_INLINE sl_size getListItemsCount() const
	{
		Variant var(*this);
		return var.getListItemsCount();
	}
	
	SLIB_INLINE Variant getListItem(sl_size index) const
	{
		Variant var(*this);
		return var.getListItem(index);
	}
	
	SLIB_INLINE sl_bool setListItem(sl_size index, const Variant& value)
	{
		Variant var(*this);
		return var.setListItem(index, value);
	}
	
	SLIB_INLINE sl_bool addListItem(const Variant& value)
	{
		Variant var(*this);
		return var.addListItem(value);
	}
	
	SLIB_INLINE Variant getField(const String& key) const
	{
		Variant var(*this);
		return var.getField(key);
	}
	
	SLIB_INLINE sl_bool putField(const String& key, const Variant& value)
	{
		Variant var(*this);
		return var.putField(key, value);
	}
	
public:
	SLIB_INLINE sl_bool isWeak() const
	{
		return m_type == Variant::typeWeak;
	}
	
	template <class T>
	SLIB_INLINE const WeakRef<T> getWeak(const WeakRef<T>& def) const
	{
		Variant var(*this);
		return var.getWeak(def);
	}
	
public:
	SLIB_INLINE String toString() const
	{
		Variant var(*this);
		return var.toString();
	}
	
	SLIB_INLINE String toJSON() const
	{
		Variant var(*this);
		return var.toJSON();
	}
	
private:
	void _retain(Variant::Type& type, sl_uint64& value) const;
	
	void _replace(Variant::Type type, sl_uint64 value);
	
	friend class Variant;
};

sl_bool operator==(const Variant& v1, const Variant& v2);

SLIB_NAMESPACE_END

#endif
