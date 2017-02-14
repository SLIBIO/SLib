#ifndef CHECKHEADER_SLIB_CORE_DETAIL_VARIANT
#define CHECKHEADER_SLIB_CORE_DETAIL_VARIANT

#include "../variant.h"

namespace slib
{
	
	struct _Variant_Const
	{
		sl_uint64 value;
		VariantType type;
		sl_int32 lock;
	};
	
	extern const _Variant_Const _Variant_Null;
	
	template <class... ARGS>
	String String::format(const sl_char8* szFormat, ARGS&&... args)
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(szFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String String::format(const String& strFormat, ARGS&&... args)
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(strFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 String16::format(const sl_char16* szFormat, ARGS&&... args)
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(szFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 String16::format(const String16& strFormat, ARGS&&... args)
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(strFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String String::arg(ARGS&&... args) const
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 String16::arg(ARGS&&... args) const
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}
	
	template <class... ARGS>
	String Atomic<String>::arg(ARGS&&... args) const
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 Atomic<String16>::arg(ARGS&&... args) const
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}


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

}

#endif
