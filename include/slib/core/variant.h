/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
		
		Variant(char value);
		
		Variant(unsigned char value);

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
		
		
		static Variant createList();
		
		static Variant createMap();
		
		static Variant createListMap();
		
		static Variant createTreeMap();
		
		static Variant createHashMap();
		
		static Variant createMapList();

	
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
		
		static Variant fromVariantList(const List<Variant>& value);

		static Variant fromVariantMap(const Map<String, Variant>& value);
		
		static Variant fromVariantMapList(const List< Map<String, Variant> >& value);
		
	public:
		Variant& operator=(Variant&& other);

		Variant& operator=(const Variant& other);

		Variant& operator=(AtomicVariant&& other);

		Variant& operator=(const AtomicVariant& other);

		Variant& operator=(sl_null_t);
		
		template <class T>
		Variant& operator=(const T& value);
		
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

		sl_size getElementsCount() const;

		Variant getElement(sl_size index) const;
	
		sl_bool setElement(sl_size index, const Variant& value);
	
		sl_bool addElement(const Variant& value);
	
		Variant getItem(const String& key) const;
	
		sl_bool putItem(const String& key, const Variant& value);
	

		String toString() const;
	
		String toJsonString() const;
		
	public:
		void get(Variant& _out) const;
		void set(const Variant& _in);
		
		void get(AtomicVariant& _out) const;
		void set(const AtomicVariant& _in);
		
		void get(char& _out) const;
		void get(char& _out, char def) const;
		void set(char _in);
		
		void get(unsigned char& _out) const;
		void get(unsigned char& _out, unsigned char def) const;
		void set(unsigned char _in);
		
		void get(short& _out) const;
		void get(short& _out, short def) const;
		void set(short _in);
		
		void get(unsigned short& _out) const;
		void get(unsigned short& _out, unsigned short def) const;
		void set(unsigned short _in);
		
		void get(int& _out) const;
		void get(int& _out, int def) const;
		void set(int _in);
		
		void get(unsigned int& _out) const;
		void get(unsigned int& _out, unsigned int def) const;
		void set(unsigned int _in);
		
		void get(long& _out) const;
		void get(long& _out, long def) const;
		void set(long _in);
		
		void get(unsigned long& _out) const;
		void get(unsigned long& _out, unsigned long def) const;
		void set(unsigned long _in);
		
		void get(sl_int64& _out) const;
		void get(sl_int64& _out, sl_int64 def) const;
		void set(sl_int64 _in);
		
		void get(sl_uint64& _out) const;
		void get(sl_uint64& _out, sl_uint64 def) const;
		void set(sl_uint64 _in);
		
		void get(float& _out) const;
		void get(float& _out, float def) const;
		void set(float _in);
		
		void get(double& _out) const;
		void get(double& _out, double def) const;
		void set(double _in);
		
		void get(bool& _out) const;
		void get(bool& _out, bool def) const;
		void set(bool _in);
		
		void get(String& _out) const;
		void get(String& _out, const String& def) const;
		void set(const String& _in);
		
		void get(AtomicString& _out) const;
		void get(AtomicString& _out, const String& def) const;
		void set(const AtomicString& _in);
		
		void get(String16& _out) const;
		void get(String16& _out, const String16& def) const;
		void set(const String16& _in);
		
		void get(AtomicString16& _out) const;
		void get(AtomicString16& _out, const String16& def) const;
		void set(const AtomicString16& _in);

		void set(const sl_char8* sz8);
		void set(const sl_char16* sz16);
		
		void get(Time& _out) const;
		void get(Time& _out, const Time& def) const;
		void set(const Time& _in);
		
		void get(void const*& _out) const;
		void get(void const*& _out, const void* def) const;
		void set(const void* _in);
		
		template <class T>
		void get(Ref<T>& _out) const;
		template <class T>
		void set(const Ref<T>& _in);
		
		template <class T>
		void get(AtomicRef<T>& _out) const;
		template <class T>
		void set(const AtomicRef<T>& _in);
		
		template <class T>
		void get(WeakRef<T>& _out) const;
		template <class T>
		void set(const WeakRef<T>& _in);
		
		template <class T>
		void get(AtomicWeakRef<T>& _out) const;
		template <class T>
		void set(const AtomicWeakRef<T>& _in);
		
		void get(Memory& _out) const;
		void set(const Memory& _in);
		
		void get(AtomicMemory& _out) const;
		void set(const AtomicMemory& _in);

		void get(List<Variant>& _out) const;
		void set(const List<Variant>& _in);
		
		void get(AtomicList<Variant>& _out) const;
		void set(const AtomicList<Variant>& _in);
		
		void get(Map<String, Variant>& _out) const;
		void set(const Map<String, Variant>& _in);
		
		void get(AtomicMap<String, Variant>& _out) const;
		void set(const AtomicMap<String, Variant>& _in);
		
		void get(List< Map<String, Variant> >& _out) const;
		void set(const List< Map<String, Variant> >& _in);
		
		void get(AtomicList< Map<String, Variant> >& _out) const;
		void set(const AtomicList< Map<String, Variant> >& _in);
		
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
		
		Atomic(char value);
		
		Atomic(unsigned char value);
		
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
		
		template <class T>
		AtomicVariant& operator=(const T& value);
		
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

		sl_size getElementsCount() const;

		Variant getElement(sl_size index) const;

		sl_bool setElement(sl_size index, const Variant& value);

		sl_bool addElement(const Variant& value);

		Variant getItem(const String& key) const;

		sl_bool putItem(const String& key, const Variant& value);


		String toString() const;

		String toJsonString() const;
		
		
	public:
		void get(Variant& _out) const;
		void set(const Variant& _in);
		
		void get(AtomicVariant& _out) const;
		void set(const AtomicVariant& _in);
		
		void get(char& _out) const;
		void get(char& _out, char def) const;
		void set(char _in);
		
		void get(unsigned char& _out) const;
		void get(unsigned char& _out, unsigned char def) const;
		void set(unsigned char _in);
		
		void get(short& _out) const;
		void get(short& _out, short def) const;
		void set(short _in);
		
		void get(unsigned short& _out) const;
		void get(unsigned short& _out, unsigned short def) const;
		void set(unsigned short _in);
		
		void get(int& _out) const;
		void get(int& _out, int def) const;
		void set(int _in);
		
		void get(unsigned int& _out) const;
		void get(unsigned int& _out, unsigned int def) const;
		void set(unsigned int _in);
		
		void get(long& _out) const;
		void get(long& _out, long def) const;
		void set(long _in);
		
		void get(unsigned long& _out) const;
		void get(unsigned long& _out, unsigned long def) const;
		void set(unsigned long _in);
		
		void get(sl_int64& _out) const;
		void get(sl_int64& _out, sl_int64 def) const;
		void set(sl_int64 _in);
		
		void get(sl_uint64& _out) const;
		void get(sl_uint64& _out, sl_uint64 def) const;
		void set(sl_uint64 _in);
		
		void get(float& _out) const;
		void get(float& _out, float def) const;
		void set(float _in);
		
		void get(double& _out) const;
		void get(double& _out, double def) const;
		void set(double _in);
		
		void get(bool& _out) const;
		void get(bool& _out, bool def) const;
		void set(bool _in);
		
		void get(String& _out) const;
		void get(String& _out, const String& def) const;
		void set(const String& _in);
		
		void get(AtomicString& _out) const;
		void get(AtomicString& _out, const String& def) const;
		void set(const AtomicString& _in);
		
		void get(String16& _out) const;
		void get(String16& _out, const String16& def) const;
		void set(const String16& _in);
		
		void get(AtomicString16& _out) const;
		void get(AtomicString16& _out, const String16& def) const;
		void set(const AtomicString16& _in);
		
		void set(const sl_char8* sz8);
		void set(const sl_char16* sz16);
		
		void get(Time& _out) const;
		void get(Time& _out, const Time& def) const;
		void set(const Time& _in);
		
		void get(void const*& _out) const;
		void get(void const*& _out, const void* def) const;
		void set(const void* ptr);
		
		template <class T>
		void get(Ref<T>& _out) const;
		template <class T>
		void set(const Ref<T>& ref);
		
		template <class T>
		void get(AtomicRef<T>& _out) const;
		template <class T>
		void set(const AtomicRef<T>& ref);
		
		template <class T>
		void get(WeakRef<T>& _out) const;
		template <class T>
		void set(const WeakRef<T>& ref);
		
		template <class T>
		void get(AtomicWeakRef<T>& _out) const;
		template <class T>
		void set(const AtomicWeakRef<T>& ref);
		
		void get(Memory& _out) const;
		void set(const Memory& ref);
		
		void get(AtomicMemory& _out) const;
		void set(const AtomicMemory& ref);
		
		void get(List<Variant>& _out) const;
		void set(const List<Variant>& _in);
		
		void get(AtomicList<Variant>& _out) const;
		void set(const AtomicList<Variant>& _in);
		
		void get(Map<String, Variant>& _out) const;
		void set(const Map<String, Variant>& _in);
		
		void get(AtomicMap<String, Variant>& _out) const;
		void set(const AtomicMap<String, Variant>& _in);
		
		void get(List< Map<String, Variant> >& _out) const;
		void set(const List< Map<String, Variant> >& _in);
		
		void get(AtomicList< Map<String, Variant> >& _out) const;
		void set(const AtomicList< Map<String, Variant> >& _in);

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
	typedef AtomicList<Variant> AtomicVariantList;
	typedef Map<String, Variant> VariantMap;
	typedef AtomicMap<String, Variant> AtomicVariantMap;
	typedef List< Map<String, Variant> > VariantMapList;
	typedef AtomicList< Map<String, Variant> > AtomicVariantMapList;

}

#include "detail/variant.inc"

#endif
