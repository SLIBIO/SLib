#ifndef CHECKHEADER_SLIB_CORE_VARIANT
#define CHECKHEADER_SLIB_CORE_VARIANT

#include "definition.h"
#include "ref.h"
#include "time.h"
#include "string.h"
#include "list.h"
#include "map.h"

namespace slib
{
	
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
	
	class Variant;
	typedef Atomic<Variant> AtomicVariant;
	
	class SLIB_EXPORT Variant
	{
	public:
		sl_uint64 _value;
		VariantType _type;
	
	public:
		constexpr Variant() : _value(0), _type(VariantType::Null) {}

		Variant(Variant&& other);

		Variant(const Variant& other);

		Variant(AtomicVariant&& _other);

		Variant(const AtomicVariant& other);
	
		~Variant();

	public:
		Variant(sl_null_t);

		Variant(short value);
		
		Variant(unsigned short value);
		
		Variant(int value);
		
		Variant(unsigned int value);
		
		Variant(long value);
		
		Variant(unsigned long value);

		Variant(sl_int64 value);

		Variant(sl_uint64 value);

		Variant(float value);
	
		Variant(double value);

		Variant(sl_bool value);

		Variant(const String& value);

		Variant(const String16& value);

		Variant(const AtomicString& value);

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

		Variant(const List<Variant>& list);

		Variant(const AtomicList<Variant>& list);

		Variant(const Map<String, Variant>& map);

		Variant(const AtomicMap<String, Variant>& map);
		
		Variant(const List< Map<String, Variant> >& list);
		
		Variant(const AtomicList< Map<String, Variant> >& list);
	
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

		static Variant fromString16(const String16& value);

		static Variant fromSz8(const sl_char8* value);

		static Variant fromSz16(const sl_char16* value);

		static Variant fromTime(const Time& value);

		static Variant fromPointer(const void* value);

		template <class T>
		static Variant fromRef(const Ref<T>& ref);

		template <class T>
		static Variant fromWeakRef(const WeakRef<T>& weak);
		
		static Variant fromMemory(const Memory& mem);
		
		static Variant createList();
		
		static Variant createMap();
		
		static Variant createListMap();
		
		static Variant createTreeMap();
		
		static Variant createHashMap();
		
		static Variant createMapList();
		
		static Variant fromVariantList(const List<Variant>& value);

		static Variant fromVariantMap(const Map<String, Variant>& value);
		
		static Variant fromVariantMapList(const List< Map<String, Variant> >& value);
		
	public:
		Variant& operator=(Variant&& other);

		Variant& operator=(const Variant& other);

		Variant& operator=(AtomicVariant&& other);

		Variant& operator=(const AtomicVariant& other);

		Variant& operator=(sl_null_t);

		Variant& operator=(short value);
		
		Variant& operator=(unsigned short value);
		
		Variant& operator=(int value);
		
		Variant& operator=(unsigned int value);
		
		Variant& operator=(long value);
		
		Variant& operator=(unsigned long value);

		Variant& operator=(sl_int64 value);

		Variant& operator=(sl_uint64 value);

		Variant& operator=(float value);

		Variant& operator=(double value);

		Variant& operator=(sl_bool value);

		Variant& operator=(const String& value);

		Variant& operator=(const AtomicString& value);

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
		
		Variant& operator=(const List<Variant>& list);
		
		Variant& operator=(const AtomicList<Variant>& list);
		
		Variant& operator=(const Map<String, Variant>& map);
		
		Variant& operator=(const AtomicMap<String, Variant>& map);
		
		Variant& operator=(const List< Map<String, Variant> >& list);
		
		Variant& operator=(const AtomicList< Map<String, Variant> >& list);
		
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

		String16 getString16(const String16& def) const;
	
		String16 getString16() const;

		const sl_char8* getSz8(const sl_char8* def = sl_null) const;

		const sl_char16* getSz16(const sl_char16* def = sl_null) const;

		void setString(const String& value);

		void setString(const String16& value);

		void setString(const AtomicString& value);

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
		
		sl_bool isVariantList() const;

		List<Variant> getVariantList() const;
		
		void setVariantList(const List<Variant>& list);
	
		sl_bool isVariantMap() const;

		Map<String, Variant> getVariantMap() const;
		
		void setVariantMap(const Map<String, Variant>& map);
		
		sl_bool isVariantMapList() const;

		List< Map<String, Variant> > getVariantMapList() const;
		
		void setVariantMapList(const List< Map<String, Variant> >& list);

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
		constexpr Atomic() : _value(0), _type(VariantType::Null) {}

		Atomic(AtomicVariant&& other);

		Atomic(const AtomicVariant& other);

		Atomic(Variant&& other);

		Atomic(const Variant& other);

		~Atomic();

	public:
		Atomic(sl_null_t);
		
		Atomic(short value);
		
		Atomic(unsigned short value);
		
		Atomic(int value);
		
		Atomic(unsigned int value);
		
		Atomic(long value);
		
		Atomic(unsigned long value);
		
		Atomic(sl_int64 value);
		
		Atomic(sl_uint64 value);

		Atomic(float value);

		Atomic(double value);

		Atomic(sl_bool value);

		Atomic(const String& value);
	
		Atomic(const String16& value);

		Atomic(const AtomicString& value);

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

		Atomic(const List<Variant>& list);
		
		Atomic(const AtomicList<Variant>& list);
		
		Atomic(const Map<String, Variant>& map);
		
		Atomic(const AtomicMap<String, Variant>& map);
		
		Atomic(const List< Map<String, Variant> >& list);
		
		Atomic(const AtomicList< Map<String, Variant> >& list);

	public:
		static const AtomicVariant& null();

	public:
		AtomicVariant& operator=(AtomicVariant&& other);

		AtomicVariant& operator=(const AtomicVariant& other);

		AtomicVariant& operator=(Variant&& other);

		AtomicVariant& operator=(const Variant& other);

		AtomicVariant& operator=(sl_null_t);

		AtomicVariant& operator=(short value);
		
		AtomicVariant& operator=(unsigned short value);
		
		AtomicVariant& operator=(int value);
		
		AtomicVariant& operator=(unsigned int value);
		
		AtomicVariant& operator=(long value);
		
		AtomicVariant& operator=(unsigned long value);
		
		AtomicVariant& operator=(sl_int64 value);

		AtomicVariant& operator=(sl_uint64 value);

		AtomicVariant& operator=(float value);

		AtomicVariant& operator=(double value);

		AtomicVariant& operator=(sl_bool value);

		AtomicVariant& operator=(const String& value);

		AtomicVariant& operator=(const AtomicString& value);

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
		
		AtomicVariant& operator=(const List<Variant>& list);
		
		AtomicVariant& operator=(const AtomicList<Variant>& list);
		
		AtomicVariant& operator=(const Map<String, Variant>& map);
		
		AtomicVariant& operator=(const AtomicMap<String, Variant>& map);
		
		AtomicVariant& operator=(const List< Map<String, Variant> >& list);
		
		AtomicVariant& operator=(const AtomicList< Map<String, Variant> >& list);

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

		String16 getString16(const String16& def) const;

		String16 getString16() const;

		const sl_char8* getSz8(const sl_char8* def = sl_null) const;

		const sl_char16* getSz16(const sl_char16* def = sl_null) const;

		void setString(const String& value);

		void setString(const AtomicString& value);

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


		sl_bool isVariantList() const;

		List<Variant> getVariantList() const;
		
		void setVariantList(const List<Variant>& list);

		sl_bool isVariantMap() const;

		Map<String, Variant> getVariantMap() const;
		
		void setVariantMap(const Map<String, Variant>& map);

		sl_bool isVariantMapList() const;

		List< Map<String, Variant> > getVariantMapList() const;
		
		void setVariantMapList(const List< Map<String, Variant> >& list);

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

	typedef List<Variant> VariantList;
	typedef Map<String, Variant> VariantMap;
	typedef List< Map<String, Variant> > VariantMapList;

}

#include "detail/variant.h"

#endif
