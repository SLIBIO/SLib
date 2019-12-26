/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_VARIANT
#define CHECKHEADER_SLIB_CORE_VARIANT

#include "definition.h"

#include "ref.h"
#include "time.h"
#include "string.h"
#include "nullable.h"
#include "list.h"
#include "map.h"
#include "hash_map.h"
#include "promise.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <string>
#endif

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
		union {
			sl_uint64 _value;
			
			sl_int32 _m_int32;
			sl_uint32 _m_uint32;
			sl_int64 _m_int64;
			sl_uint64 _m_uint64;
			float _m_float;
			double _m_double;
			sl_bool _m_boolean;
			const StringContainer* _m_string8;
			const StringContainer16* _m_string16;
			const sl_char8* _m_sz8;
			const sl_char16* _m_sz16;
			Referable* _m_ref;
			CWeakRef* _m_wref;
			CMemory* _m_mem;
			CPromise<Variant>* _m_promise;
			CList<Variant>* _m_vlist;
			CHashMap<String, Variant>* _m_vhmap;
			CMap<String, Variant>* _m_vmap;
			CList< HashMap<String, Variant> >* _m_vhmaplist;
			CList< Map<String, Variant> >* _m_vmaplist;
		};
		VariantType _type;
	
	public:
		SLIB_INLINE constexpr Variant() noexcept : _value(0), _type(VariantType::Null) {}

		SLIB_INLINE constexpr Variant(sl_null_t) noexcept : _value(1), _type(VariantType::Null) {}

		Variant(Variant&& other) noexcept;

		Variant(const Variant& other) noexcept;

		Variant(AtomicVariant&& _other) noexcept;

		Variant(const AtomicVariant& other) noexcept;
	
		~Variant() noexcept;

	public:
		Variant(signed char value) noexcept;
		
		Variant(unsigned char value) noexcept;

		Variant(short value) noexcept;
		
		Variant(unsigned short value) noexcept;
		
		Variant(int value) noexcept;
		
		Variant(unsigned int value) noexcept;
		
		Variant(long value) noexcept;
		
		Variant(unsigned long value) noexcept;

		Variant(sl_int64 value) noexcept;

		Variant(sl_uint64 value) noexcept;

		Variant(float value) noexcept;
	
		Variant(double value) noexcept;

		Variant(sl_bool value) noexcept;

		Variant(const String& value) noexcept;

		Variant(const String16& value) noexcept;

		Variant(const AtomicString& value) noexcept;

		Variant(const AtomicString16& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		Variant(const std::string& value) noexcept;
		
		Variant(const std::u16string& value) noexcept;
#endif
		
		Variant(const sl_char8* sz8) noexcept;

		Variant(const sl_char16* sz16) noexcept;

		Variant(const StringParam& str) noexcept;

		Variant(const Time& value) noexcept;

		Variant(const void* ptr) noexcept;

		template <class T>
		Variant(const Nullable<T>& value) noexcept;

		template <class T>
		Variant(const Ref<T>& ref) noexcept;

		template <class T>
		Variant(const AtomicRef<T>& ref) noexcept;

		template <class T>
		Variant(const WeakRef<T>& weak) noexcept;

		template <class T>
		Variant(const AtomicWeakRef<T>& weak) noexcept;
	
		Variant(const Memory& mem) noexcept;

		Variant(const AtomicMemory& mem) noexcept;

		Variant(const List<Variant>& list) noexcept;

		Variant(const AtomicList<Variant>& list) noexcept;

		Variant(const Map<String, Variant>& map) noexcept;

		Variant(const AtomicMap<String, Variant>& map) noexcept;
		
		Variant(const HashMap<String, Variant>& map) noexcept;
		
		Variant(const AtomicHashMap<String, Variant>& map) noexcept;

		Variant(const List< Map<String, Variant> >& list) noexcept;
		
		Variant(const AtomicList< Map<String, Variant> >& list) noexcept;
		
		Variant(const List< HashMap<String, Variant> >& list) noexcept;
		
		Variant(const AtomicList< HashMap<String, Variant> >& list) noexcept;
		
		Variant(const Promise<Variant>& promise) noexcept;
		
		Variant(const AtomicPromise<Variant>& promise) noexcept;
		
	public:
		static const Variant& undefined() noexcept;
		
		static const Variant& null() noexcept;
		
		
		static Variant createList() noexcept;
		
		static Variant createMap() noexcept;
		
		static Variant createHashMap() noexcept;
		
		static Variant createMapList() noexcept;

		static Variant createHashMapList() noexcept;

	
		static Variant fromInt32(sl_int32 value) noexcept;

		static Variant fromUint32(sl_uint32 value) noexcept;

		static Variant fromInt64(sl_int64 value) noexcept;

		static Variant fromUint64(sl_uint64 value) noexcept;

		static Variant fromFloat(float value) noexcept;

		static Variant fromDouble(double value) noexcept;

		static Variant fromBoolean(sl_bool value) noexcept;

		static Variant fromString(const String& value) noexcept;

		static Variant fromString16(const String16& value) noexcept;

		static Variant fromSz8(const sl_char8* value) noexcept;

		static Variant fromSz16(const sl_char16* value) noexcept;

		static Variant fromStringParam(const StringParam& value) noexcept;

		static Variant fromTime(const Time& value) noexcept;

		static Variant fromPointer(const void* value) noexcept;

		template <class T>
		static Variant fromNullable(const Nullable<T>& value) noexcept;

		template <class T>
		static Variant fromRef(const Ref<T>& ref) noexcept;

		template <class T>
		static Variant fromWeakRef(const WeakRef<T>& weak) noexcept;
		
		static Variant fromMemory(const Memory& mem) noexcept;
		
		static Variant fromVariantList(const List<Variant>& value) noexcept;

		static Variant fromVariantMap(const Map<String, Variant>& value) noexcept;
		
		static Variant fromVariantHashMap(const HashMap<String, Variant>& value) noexcept;

		static Variant fromVariantMapList(const List< Map<String, Variant> >& value) noexcept;
		
		static Variant fromVariantHashMapList(const List< HashMap<String, Variant> >& value) noexcept;
		
		static Variant fromVariantPromise(const Promise<Variant>& value) noexcept;
		
	public:
		Variant& operator=(Variant&& other) noexcept;

		Variant& operator=(const Variant& other) noexcept;

		Variant& operator=(AtomicVariant&& other) noexcept;

		Variant& operator=(const AtomicVariant& other) noexcept;

		Variant& operator=(sl_null_t) noexcept;
		
		template <class T>
		Variant& operator=(const T& value) noexcept;
		
		Variant operator[](sl_size list_index) const noexcept;

		Variant operator[](const String& map_key) const noexcept;

	public:
		VariantType getType() const noexcept;

		void setUndefined() noexcept;
		
		sl_bool isUndefined() const noexcept;
		
		sl_bool isNotUndefined() const noexcept;

		void setNull() noexcept;

		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;


		sl_bool isInt32() const noexcept;
	
		sl_int32 getInt32(sl_int32 def = 0) const noexcept;

		void setInt32(sl_int32 value) noexcept;
	
		sl_bool isUint32() const noexcept;
	
		sl_uint32 getUint32(sl_uint32 def = 0) const noexcept;

		void setUint32(sl_uint32 value) noexcept;
	
		sl_bool isInt64() const noexcept;
	
		sl_int64 getInt64(sl_int64 def = 0) const noexcept;

		void setInt64(sl_int64 value) noexcept;
	
		sl_bool isUint64() const noexcept;
	
		sl_uint64 getUint64(sl_uint64 def = 0) const noexcept;

		void setUint64(sl_uint64 value) noexcept;
	
		sl_bool isInteger() const noexcept;

		sl_bool isSignedInteger() const noexcept;

		sl_bool isUnsignedInteger() const noexcept;

		sl_bool isFloat() const noexcept;
	
		float getFloat(float def = 0) const noexcept;

		void setFloat(float value) noexcept;
	
		sl_bool isDouble() const noexcept;
	
		double getDouble(double def = 0) const noexcept;

		void setDouble(double value) noexcept;
	
		sl_bool isNumber() const noexcept;
	

		sl_bool isBoolean() const noexcept;
	
		sl_bool isTrue() const noexcept;

		sl_bool isFalse() const noexcept;

		sl_bool getBoolean(sl_bool def = sl_false) const noexcept;

		void setBoolean(sl_bool value) noexcept;
	

		sl_bool isString() const noexcept;
	
		sl_bool isString8() const noexcept;
	
		sl_bool isString16() const noexcept;

		sl_bool isSz8() const noexcept;

		sl_bool isSz16() const noexcept;
	
		String getString(const String& def) const noexcept;

		String getString() const noexcept;

		String16 getString16(const String16& def) const noexcept;
	
		String16 getString16() const noexcept;

		const sl_char8* getSz8(const sl_char8* def = sl_null) const noexcept;

		const sl_char16* getSz16(const sl_char16* def = sl_null) const noexcept;

		StringParam getStringParam(const StringParam& def) const noexcept;
		
		StringParam getStringParam() const noexcept;

		void setString(const String& value) noexcept;

		void setString(const String16& value) noexcept;

		void setString(const AtomicString& value) noexcept;

		void setString(const AtomicString16& value) noexcept;

		void setString(const sl_char8* sz8) noexcept;

		void setString(const sl_char16* sz16) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		std::string getStdString() const noexcept;
		
		std::u16string getStdString16() const noexcept;

		void setString(const std::string& value) noexcept;
		
		void setString(const std::u16string& value) noexcept;
#endif
		
		void setString(const StringParam& value) noexcept;
		

		sl_bool isTime() const noexcept;

		Time getTime(const Time& def) const noexcept;

		Time getTime() const noexcept;

		void setTime(const Time& value) noexcept;


		sl_bool isPointer() const noexcept;

		void* getPointer(const void* def = sl_null) const noexcept;

		void setPointer(const void* ptr) noexcept;


		sl_bool isObject() const noexcept;

		sl_bool isWeak() const noexcept;
	
		Ref<Referable> getObject() const noexcept;
	
		template <class T>
		Ref<T> getObject(const Ref<T>& def) const noexcept;
	
		template <class T>
		void setObject(const Ref<T>& ref) noexcept;

		template <class T>
		void setWeak(const WeakRef<T>& weak) noexcept;

		sl_bool isObjectNotNull() const noexcept;

		sl_bool isObjectNull() const noexcept;

		sl_object_type getObjectType() const noexcept;

		sl_bool isMemory() const noexcept;

		Memory getMemory() const noexcept;

		void setMemory(const Memory& mem) noexcept;
		
		sl_bool isVariantList() const noexcept;

		List<Variant> getVariantList() const noexcept;
		
		void setVariantList(const List<Variant>& list) noexcept;
	
		sl_bool isVariantMapOrVariantHashMap() const noexcept;
		
		sl_bool isVariantMap() const noexcept;

		Map<String, Variant> getVariantMap() const noexcept;
		
		void setVariantMap(const Map<String, Variant>& map) noexcept;
		
		sl_bool isVariantHashMap() const noexcept;
		
		HashMap<String, Variant> getVariantHashMap() const noexcept;
		
		void setVariantHashMap(const HashMap<String, Variant>& map) noexcept;
		
		sl_bool isVariantMapListOrVariantHashMapList() const noexcept;
		
		sl_bool isVariantMapList() const noexcept;

		List< Map<String, Variant> > getVariantMapList() const noexcept;
		
		void setVariantMapList(const List< Map<String, Variant> >& list) noexcept;

		sl_bool isVariantHashMapList() const noexcept;
		
		List< HashMap<String, Variant> > getVariantHashMapList() const noexcept;
		
		void setVariantHashMapList(const List< HashMap<String, Variant> >& list) noexcept;

		sl_size getElementsCount() const noexcept;

		Variant getElement_NoLock(sl_size index) const noexcept;
		
		Variant getElement(sl_size index) const noexcept;
	
		sl_bool setElement_NoLock(sl_size index, const Variant& value) noexcept;
	
		sl_bool setElement(sl_size index, const Variant& value) noexcept;
	
		sl_bool addElement_NoLock(const Variant& value) noexcept;
	
		sl_bool addElement(const Variant& value) noexcept;
	
		Variant getItem_NoLock(const String& key) const noexcept;
	
		Variant getItem(const String& key) const noexcept;

		sl_bool putItem_NoLock(const String& key, const Variant& value) noexcept;
	
		sl_bool putItem(const String& key, const Variant& value) noexcept;

		sl_bool removeItem_NoLock(const String& key) noexcept;
		
		sl_bool removeItem(const String& key) noexcept;
	
		
		sl_bool isVariantPromise() const noexcept;
		
		Promise<Variant> getVariantPromise() const noexcept;
		
		void setVariantPromise(const Promise<Variant>& promise) noexcept;


		String toString() const noexcept;
	
		String toJsonString() const noexcept;
		
		
		sl_compare_result compare(const Variant& other) const noexcept;
		
		sl_bool equals(const Variant& other) const noexcept;
		
		sl_size getHashCode() const noexcept;
		
	public:
		void get(Variant& _out) const noexcept;
		void set(const Variant& _in) noexcept;
		
		void get(AtomicVariant& _out) const noexcept;
		void set(const AtomicVariant& _in) noexcept;
		
		void get(signed char& _out) const noexcept;
		void get(signed char& _out, signed char def) const noexcept;
		void set(signed char _in) noexcept;
		
		void get(unsigned char& _out) const noexcept;
		void get(unsigned char& _out, unsigned char def) const noexcept;
		void set(unsigned char _in) noexcept;
		
		void get(short& _out) const noexcept;
		void get(short& _out, short def) const noexcept;
		void set(short _in) noexcept;
		
		void get(unsigned short& _out) const noexcept;
		void get(unsigned short& _out, unsigned short def) const noexcept;
		void set(unsigned short _in) noexcept;
		
		void get(int& _out) const noexcept;
		void get(int& _out, int def) const noexcept;
		void set(int _in) noexcept;
		
		void get(unsigned int& _out) const noexcept;
		void get(unsigned int& _out, unsigned int def) const noexcept;
		void set(unsigned int _in) noexcept;
		
		void get(long& _out) const noexcept;
		void get(long& _out, long def) const noexcept;
		void set(long _in) noexcept;
		
		void get(unsigned long& _out) const noexcept;
		void get(unsigned long& _out, unsigned long def) const noexcept;
		void set(unsigned long _in) noexcept;
		
		void get(sl_int64& _out) const noexcept;
		void get(sl_int64& _out, sl_int64 def) const noexcept;
		void set(sl_int64 _in) noexcept;
		
		void get(sl_uint64& _out) const noexcept;
		void get(sl_uint64& _out, sl_uint64 def) const noexcept;
		void set(sl_uint64 _in) noexcept;
		
		void get(float& _out) const noexcept;
		void get(float& _out, float def) const noexcept;
		void set(float _in) noexcept;
		
		void get(double& _out) const noexcept;
		void get(double& _out, double def) const noexcept;
		void set(double _in) noexcept;
		
		void get(bool& _out) const noexcept;
		void get(bool& _out, bool def) const noexcept;
		void set(bool _in) noexcept;
		
		void get(String& _out) const noexcept;
		void get(String& _out, const String& def) const noexcept;
		void set(const String& _in) noexcept;
		
		void get(AtomicString& _out) const noexcept;
		void get(AtomicString& _out, const String& def) const noexcept;
		void set(const AtomicString& _in) noexcept;
		
		void get(String16& _out) const noexcept;
		void get(String16& _out, const String16& def) const noexcept;
		void set(const String16& _in) noexcept;
		
		void get(AtomicString16& _out) const noexcept;
		void get(AtomicString16& _out, const String16& def) const noexcept;
		void set(const AtomicString16& _in) noexcept;

		void set(const sl_char8* sz8) noexcept;
		void set(const sl_char16* sz16) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		void get(std::string& _out) const noexcept;
		void set(const std::string& _in) noexcept;
		
		void get(std::u16string& _out) const noexcept;
		void set(const std::u16string& _in) noexcept;
#endif
		void get(StringParam& _out) const noexcept;
		void get(StringParam& _out, const StringParam& def) const noexcept;
		void set(const StringParam& _in) noexcept;
		
		void get(Time& _out) const noexcept;
		void get(Time& _out, const Time& def) const noexcept;
		void set(const Time& _in) noexcept;
		
		void get(void const*& _out) const noexcept;
		void get(void const*& _out, const void* def) const noexcept;
		void set(const void* _in) noexcept;
		
		template <class T>
		void get(Nullable<T>& _out) const noexcept;
		template <class T>
		void set(const Nullable<T>& _in) noexcept;
		
		template <class T>
		void get(Ref<T>& _out) const noexcept;
		template <class T>
		void set(const Ref<T>& _in) noexcept;
		
		template <class T>
		void get(AtomicRef<T>& _out) const noexcept;
		template <class T>
		void set(const AtomicRef<T>& _in) noexcept;
		
		template <class T>
		void get(WeakRef<T>& _out) const noexcept;
		template <class T>
		void set(const WeakRef<T>& _in) noexcept;
		
		template <class T>
		void get(AtomicWeakRef<T>& _out) const noexcept;
		template <class T>
		void set(const AtomicWeakRef<T>& _in) noexcept;
		
		void get(Memory& _out) const noexcept;
		void set(const Memory& _in) noexcept;
		
		void get(AtomicMemory& _out) const noexcept;
		void set(const AtomicMemory& _in) noexcept;

		void get(List<Variant>& _out) const noexcept;
		void set(const List<Variant>& _in) noexcept;
		
		void get(AtomicList<Variant>& _out) const noexcept;
		void set(const AtomicList<Variant>& _in) noexcept;
		
		void get(Map<String, Variant>& _out) const noexcept;
		void set(const Map<String, Variant>& _in) noexcept;
		
		void get(AtomicMap<String, Variant>& _out) const noexcept;
		void set(const AtomicMap<String, Variant>& _in) noexcept;
		
		void get(HashMap<String, Variant>& _out) const noexcept;
		void set(const HashMap<String, Variant>& _in) noexcept;
		
		void get(AtomicHashMap<String, Variant>& _out) const noexcept;
		void set(const AtomicHashMap<String, Variant>& _in) noexcept;
		
		void get(List< Map<String, Variant> >& _out) const noexcept;
		void set(const List< Map<String, Variant> >& _in) noexcept;
		
		void get(AtomicList< Map<String, Variant> >& _out) const noexcept;
		void set(const AtomicList< Map<String, Variant> >& _in) noexcept;
		
		void get(List< HashMap<String, Variant> >& _out) const noexcept;
		void set(const List< HashMap<String, Variant> >& _in) noexcept;
		
		void get(AtomicList< HashMap<String, Variant> >& _out) const noexcept;
		void set(const AtomicList< HashMap<String, Variant> >& _in) noexcept;

		void get(Promise<Variant>& _out) const noexcept;
		void set(const Promise<Variant>& _in) noexcept;
		
		void get(AtomicPromise<Variant>& _out) const noexcept;
		void set(const AtomicPromise<Variant>& _in) noexcept;

	private:
		void _constructorRef(const void* ptr);
		
		void _constructorAtomicRef(const void* ptr);
		
		void _constructorWeakRef(const void* ptr);
		
		void _constructorAtomicWeakRef(const void* ptr);

		void _assignRef(const void* ptr);
		
		void _assignAtomicRef(const void* ptr);
		
		void _assignWeakRef(const void* ptr);
		
		void _assignAtomicWeakRef(const void* ptr);
		
		static void _free(VariantType type, sl_uint64 value) noexcept;

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
		SLIB_INLINE constexpr Atomic() : _value(0), _type(VariantType::Null) {}

		SLIB_INLINE constexpr Atomic(sl_null_t) : _value(1), _type(VariantType::Null) {}
		
		Atomic(AtomicVariant&& other) noexcept;

		Atomic(const AtomicVariant& other) noexcept;

		Atomic(Variant&& other) noexcept;

		Atomic(const Variant& other) noexcept;

		~Atomic() noexcept;

	public:
		Atomic(signed char value) noexcept;
		
		Atomic(unsigned char value) noexcept;
		
		Atomic(short value) noexcept;
		
		Atomic(unsigned short value) noexcept;
		
		Atomic(int value) noexcept;
		
		Atomic(unsigned int value) noexcept;
		
		Atomic(long value) noexcept;
		
		Atomic(unsigned long value) noexcept;
		
		Atomic(sl_int64 value) noexcept;
		
		Atomic(sl_uint64 value) noexcept;

		Atomic(float value) noexcept;

		Atomic(double value) noexcept;

		Atomic(sl_bool value) noexcept;

		Atomic(const String& value) noexcept;
	
		Atomic(const String16& value) noexcept;

		Atomic(const AtomicString& value) noexcept;

		Atomic(const AtomicString16& value) noexcept;

		Atomic(const sl_char8* sz8) noexcept;

		Atomic(const sl_char16* sz16) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::string& value) noexcept;
		
		Atomic(const std::u16string& value) noexcept;
#endif

		Atomic(const StringParam& value) noexcept;
		
		Atomic(const Time& value) noexcept;

		Atomic(const void* ptr) noexcept;
	
		template <class T>
		Atomic(const Nullable<T>& value) noexcept;

		template <class T>
		Atomic(const Ref<T>& ref) noexcept;

		template <class T>
		Atomic(const AtomicRef<T>& ref) noexcept;

		template <class T>
		Atomic(const WeakRef<T>& weak) noexcept;

		template <class T>
		Atomic(const AtomicWeakRef<T>& weak) noexcept;
	
		Atomic(const Memory& mem) noexcept;

		Atomic(const AtomicMemory& mem) noexcept;

		Atomic(const List<Variant>& list) noexcept;
		
		Atomic(const AtomicList<Variant>& list) noexcept;
		
		Atomic(const Map<String, Variant>& map) noexcept;
		
		Atomic(const AtomicMap<String, Variant>& map) noexcept;
		
		Atomic(const HashMap<String, Variant>& map) noexcept;
		
		Atomic(const AtomicHashMap<String, Variant>& map) noexcept;
		
		Atomic(const List< Map<String, Variant> >& list) noexcept;
		
		Atomic(const AtomicList< Map<String, Variant> >& list) noexcept;
		
		Atomic(const List< HashMap<String, Variant> >& list) noexcept;
		
		Atomic(const AtomicList< HashMap<String, Variant> >& list) noexcept;

		Atomic(const Promise<Variant>& promise) noexcept;
		
		Atomic(const AtomicPromise<Variant>& promise) noexcept;

	public:
		static const AtomicVariant& undefined() noexcept;
		
		static const AtomicVariant& null() noexcept;

	public:
		AtomicVariant& operator=(AtomicVariant&& other) noexcept;

		AtomicVariant& operator=(const AtomicVariant& other) noexcept;

		AtomicVariant& operator=(Variant&& other) noexcept;

		AtomicVariant& operator=(const Variant& other) noexcept;

		AtomicVariant& operator=(sl_null_t) noexcept;
		
		template <class T>
		AtomicVariant& operator=(const T& value) noexcept;
		
		Variant operator[](sl_size list_index) const noexcept;

		Variant operator[](const String& map_key) const noexcept;
	
	public:
		VariantType getType() const noexcept;

		void setUndefined() noexcept;
		
		sl_bool isUndefined() const noexcept;
		
		sl_bool isNotUndefined() const noexcept;

		void setNull() noexcept;

		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

	public:
		sl_bool isInt32() const noexcept;

		sl_int32 getInt32(sl_int32 def = 0) const noexcept;

		void setInt32(sl_int32 value) noexcept;

		sl_bool isUint32() const noexcept;

		sl_uint32 getUint32(sl_uint32 def = 0) const noexcept;

		void setUint32(sl_uint32 value) noexcept;

		sl_bool isInt64() const noexcept;

		sl_int64 getInt64(sl_int64 def = 0) const noexcept;

		void setInt64(sl_int64 value) noexcept;

		sl_bool isUint64() const noexcept;

		sl_uint64 getUint64(sl_uint64 def = 0) const noexcept;

		void setUint64(sl_uint64 value) noexcept;

		sl_bool isInteger() const noexcept;

		sl_bool isSignedInteger() const noexcept;

		sl_bool isUnsignedInteger() const noexcept;
	
		sl_bool isFloat() const noexcept;

		float getFloat(float def = 0) const noexcept;

		void setFloat(float value) noexcept;

		sl_bool isDouble() const noexcept;

		double getDouble(double def = 0) const noexcept;

		void setDouble(double value) noexcept;

		sl_bool isNumber() const noexcept;


		sl_bool isTime() const noexcept;

		Time getTime(Time def) const noexcept;

		Time getTime() const noexcept;

		void setTime(const Time& time) noexcept;


		sl_bool isPointer() const noexcept;

		void* getPointer(const void* def = sl_null) const noexcept;

		void setPointer(const void* ptr) noexcept;


		sl_bool isBoolean() const noexcept;

		sl_bool getBoolean(sl_bool def = sl_false) const noexcept;

		void setBoolean(sl_bool value) noexcept;


		sl_bool isString() const noexcept;

		sl_bool isString8() const noexcept;

		sl_bool isString16() const noexcept;

		sl_bool isSz8() const noexcept;

		sl_bool isSz16() const noexcept;

		String getString(const String& def) const noexcept;

		String getString() const noexcept;

		String16 getString16(const String16& def) const noexcept;

		String16 getString16() const noexcept;

		const sl_char8* getSz8(const sl_char8* def = sl_null) const noexcept;

		const sl_char16* getSz16(const sl_char16* def = sl_null) const noexcept;

		StringParam getStringParam(const StringParam& def) const noexcept;
		
		StringParam getStringParam() const noexcept;

		void setString(const String& value) noexcept;

		void setString(const AtomicString& value) noexcept;

		void setString(const String16& value) noexcept;

		void setString(const AtomicString16& value) noexcept;

		void setString(const sl_char8* sz8) noexcept;

		void setString(const sl_char16* sz16) noexcept;
	
#ifdef SLIB_SUPPORT_STD_TYPES
		std::string getStdString() const noexcept;
				
		std::u16string getStdString16() const noexcept;
		
		void setString(const std::string& value) noexcept;
		
		void setString(const std::u16string& value) noexcept;
#endif

		void setString(const StringParam& value) noexcept;
		
		
		sl_bool isObject() const noexcept;

		sl_bool isWeak() const noexcept;

		Ref<Referable> getObject() const noexcept;
	
		template <class T>
		Ref<T> getObject(const Ref<T>& def) const noexcept;

		template <class T>
		void setObject(const Ref<T>& object) noexcept;
	
		template <class T>
		void setWeak(const WeakRef<T>& weak) noexcept;
	
		sl_bool isObjectNotNull() const noexcept;

		sl_bool isObjectNull() const noexcept;

		sl_object_type getObjectType() const noexcept;

		sl_bool isMemory() const noexcept;

		Memory getMemory() const noexcept;

		void setMemory(const Memory& mem) noexcept;


		sl_bool isVariantList() const noexcept;

		List<Variant> getVariantList() const noexcept;
		
		void setVariantList(const List<Variant>& list) noexcept;

		sl_bool isVariantMapOrVariantHashMap() const noexcept;

		sl_bool isVariantMap() const noexcept;

		Map<String, Variant> getVariantMap() const noexcept;
		
		void setVariantMap(const Map<String, Variant>& map) noexcept;

		sl_bool isVariantHashMap() const noexcept;
		
		HashMap<String, Variant> getVariantHashMap() const noexcept;
		
		void setVariantHashMap(const HashMap<String, Variant>& map) noexcept;

		sl_bool isVariantMapListOrVariantHashMapList() const noexcept;

		sl_bool isVariantMapList() const noexcept;

		List< Map<String, Variant> > getVariantMapList() const noexcept;
		
		void setVariantMapList(const List< Map<String, Variant> >& list) noexcept;

		sl_bool isVariantHashMapList() const noexcept;
		
		List< HashMap<String, Variant> > getVariantHashMapList() const noexcept;
		
		void setVariantHashMapList(const List< HashMap<String, Variant> >& list) noexcept;
		
		sl_size getElementsCount() const noexcept;

		Variant getElement(sl_size index) const noexcept;

		sl_bool setElement(sl_size index, const Variant& value) noexcept;

		sl_bool addElement(const Variant& value) noexcept;

		Variant getItem(const String& key) const noexcept;

		sl_bool putItem(const String& key, const Variant& value) noexcept;

		sl_bool removeItem(const String& key) noexcept;
		
		
		sl_bool isVariantPromise() const noexcept;
		
		Promise<Variant> getVariantPromise() const noexcept;
		
		void setVariantPromise(const Promise<Variant>& promise) noexcept;
		

		String toString() const noexcept;

		String toJsonString() const noexcept;
		
		
		sl_compare_result compare(const Variant& other) const noexcept;
		
		sl_bool equals(const Variant& other) const noexcept;
		
		sl_size getHashCode() const noexcept;

	public:
		void get(Variant& _out) const noexcept;
		void set(const Variant& _in) noexcept;
		
		void get(AtomicVariant& _out) const noexcept;
		void set(const AtomicVariant& _in) noexcept;
		
		void get(signed char& _out) const noexcept;
		void get(signed char& _out, signed char def) const noexcept;
		void set(signed char _in) noexcept;
		
		void get(unsigned char& _out) const noexcept;
		void get(unsigned char& _out, unsigned char def) const noexcept;
		void set(unsigned char _in) noexcept;
		
		void get(short& _out) const noexcept;
		void get(short& _out, short def) const noexcept;
		void set(short _in) noexcept;
		
		void get(unsigned short& _out) const noexcept;
		void get(unsigned short& _out, unsigned short def) const noexcept;
		void set(unsigned short _in) noexcept;
		
		void get(int& _out) const noexcept;
		void get(int& _out, int def) const noexcept;
		void set(int _in) noexcept;
		
		void get(unsigned int& _out) const noexcept;
		void get(unsigned int& _out, unsigned int def) const noexcept;
		void set(unsigned int _in) noexcept;
		
		void get(long& _out) const noexcept;
		void get(long& _out, long def) const noexcept;
		void set(long _in) noexcept;
		
		void get(unsigned long& _out) const noexcept;
		void get(unsigned long& _out, unsigned long def) const noexcept;
		void set(unsigned long _in) noexcept;
		
		void get(sl_int64& _out) const noexcept;
		void get(sl_int64& _out, sl_int64 def) const noexcept;
		void set(sl_int64 _in) noexcept;
		
		void get(sl_uint64& _out) const noexcept;
		void get(sl_uint64& _out, sl_uint64 def) const noexcept;
		void set(sl_uint64 _in) noexcept;
		
		void get(float& _out) const noexcept;
		void get(float& _out, float def) const noexcept;
		void set(float _in) noexcept;
		
		void get(double& _out) const noexcept;
		void get(double& _out, double def) const noexcept;
		void set(double _in) noexcept;
		
		void get(bool& _out) const noexcept;
		void get(bool& _out, bool def) const noexcept;
		void set(bool _in) noexcept;
		
		void get(String& _out) const noexcept;
		void get(String& _out, const String& def) const noexcept;
		void set(const String& _in) noexcept;
		
		void get(AtomicString& _out) const noexcept;
		void get(AtomicString& _out, const String& def) const noexcept;
		void set(const AtomicString& _in) noexcept;
		
		void get(String16& _out) const noexcept;
		void get(String16& _out, const String16& def) const noexcept;
		void set(const String16& _in) noexcept;
		
		void get(AtomicString16& _out) const noexcept;
		void get(AtomicString16& _out, const String16& def) const noexcept;
		void set(const AtomicString16& _in) noexcept;
		
		void set(const sl_char8* sz8) noexcept;
		void set(const sl_char16* sz16) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		void get(std::string& _out) const noexcept;
		void set(const std::string& _in) noexcept;
		
		void get(std::u16string& _out) const noexcept;
		void set(const std::u16string& _in) noexcept;
#endif
		
		void get(StringParam& _out) const noexcept;
		void get(StringParam& _out, const StringParam& def) const noexcept;
		void set(const StringParam& _in) noexcept;
		
		void get(Time& _out) const noexcept;
		void get(Time& _out, const Time& def) const noexcept;
		void set(const Time& _in) noexcept;
		
		void get(void const*& _out) const noexcept;
		void get(void const*& _out, const void* def) const noexcept;
		void set(const void* ptr) noexcept;
		
		template <class T>
		void get(Nullable<T>& _out) const noexcept;
		template <class T>
		void set(const Nullable<T>& value) noexcept;
		
		template <class T>
		void get(Ref<T>& _out) const noexcept;
		template <class T>
		void set(const Ref<T>& ref) noexcept;
		
		template <class T>
		void get(AtomicRef<T>& _out) const noexcept;
		template <class T>
		void set(const AtomicRef<T>& ref) noexcept;
		
		template <class T>
		void get(WeakRef<T>& _out) const noexcept;
		template <class T>
		void set(const WeakRef<T>& ref) noexcept;
		
		template <class T>
		void get(AtomicWeakRef<T>& _out) const noexcept;
		template <class T>
		void set(const AtomicWeakRef<T>& ref) noexcept;
		
		void get(Memory& _out) const noexcept;
		void set(const Memory& ref) noexcept;
		
		void get(AtomicMemory& _out) const noexcept;
		void set(const AtomicMemory& ref) noexcept;
		
		void get(List<Variant>& _out) const noexcept;
		void set(const List<Variant>& _in) noexcept;
		
		void get(AtomicList<Variant>& _out) const noexcept;
		void set(const AtomicList<Variant>& _in) noexcept;
		
		void get(Map<String, Variant>& _out) const noexcept;
		void set(const Map<String, Variant>& _in) noexcept;
		
		void get(AtomicMap<String, Variant>& _out) const noexcept;
		void set(const AtomicMap<String, Variant>& _in) noexcept;
		
		void get(HashMap<String, Variant>& _out) const noexcept;
		void set(const HashMap<String, Variant>& _in) noexcept;
		
		void get(AtomicHashMap<String, Variant>& _out) const noexcept;
		void set(const AtomicHashMap<String, Variant>& _in) noexcept;
		
		void get(List< Map<String, Variant> >& _out) const noexcept;
		void set(const List< Map<String, Variant> >& _in) noexcept;
		
		void get(AtomicList< Map<String, Variant> >& _out) const noexcept;
		void set(const AtomicList< Map<String, Variant> >& _in) noexcept;

		void get(List< HashMap<String, Variant> >& _out) const noexcept;
		void set(const List< HashMap<String, Variant> >& _in) noexcept;
		
		void get(AtomicList< HashMap<String, Variant> >& _out) const noexcept;
		void set(const AtomicList< HashMap<String, Variant> >& _in) noexcept;
		
		void get(Promise<Variant>& _out) const noexcept;
		void set(const Promise<Variant>& _in) noexcept;
		
		void get(AtomicPromise<Variant>& _out) const noexcept;
		void set(const AtomicPromise<Variant>& _in) noexcept;
		
	private:
		void _constructorRef(const void* ptr);
		
		void _constructorAtomicRef(const void* ptr);
		
		void _constructorWeakRef(const void* ptr);
		
		void _constructorAtomicWeakRef(const void* ptr);
		
		void _assignRef(const void* ptr);
		
		void _assignAtomicRef(const void* ptr);
		
		void _assignWeakRef(const void* ptr);
		
		void _assignAtomicWeakRef(const void* ptr);
		
		void _retain(VariantType& type, sl_uint64& value) const noexcept;

		void _replace(VariantType type, sl_uint64 value) noexcept;

		friend class Variant;
		
	};
	
	sl_bool operator==(const Variant& v1, const Variant& v2) noexcept;
	
	sl_bool operator!=(const Variant& v1, const Variant& v2) noexcept;

	
	template <>
	class Compare<Variant>
	{
	public:
		sl_compare_result operator()(const Variant &a, const Variant &b) const noexcept;
	};
	
	template <>
	class Equals<Variant>
	{
	public:
		sl_bool operator()(const Variant &a, const Variant &b) const noexcept;
	};
	
	template <>
	class Hash<Variant>
	{
	public:
		sl_size operator()(const Variant &a) const noexcept;
	};

	typedef List<Variant> VariantList;
	typedef AtomicList<Variant> AtomicVariantList;
	typedef Map<String, Variant> VariantMap;
	typedef AtomicMap<String, Variant> AtomicVariantMap;
	typedef HashMap<String, Variant> VariantHashMap;
	typedef AtomicHashMap<String, Variant> AtomicVariantHashMap;
	typedef List< Map<String, Variant> > VariantMapList;
	typedef AtomicList< Map<String, Variant> > AtomicVariantMapList;
	typedef List< HashMap<String, Variant> > VariantHashMapList;
	typedef AtomicList< HashMap<String, Variant> > AtomicVariantHashMapList;
	
	typedef Promise<Variant> VariantPromise;

}

#include "detail/variant.inc"

#endif
