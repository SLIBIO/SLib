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
	Variant Variant::fromWeakRef(const WeakRef<T>& weak)
	{
		return weak;
	}
	
	template <class T>
	SLIB_INLINE Variant& Variant::operator=(const T& value)
	{
		set(value);
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
	void Variant::get(Ref<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Variant::set(const Ref<T>& _in)
	{
		setObject(_in);
	}
	
	template <class T>
	void Variant::get(AtomicRef<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Variant::set(const AtomicRef<T>& _in)
	{
		setObject(_in);
	}
	
	template <class T>
	void Variant::get(WeakRef<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Variant::set(const WeakRef<T>& _in)
	{
		setWeak(_in);
	}
	
	template <class T>
	void Variant::get(AtomicWeakRef<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Variant::set(const AtomicWeakRef<T>& _in)
	{
		setWeak(_in);
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
	
	SLIB_INLINE const AtomicVariant& Atomic<Variant>::null()
	{
		return *(reinterpret_cast<AtomicVariant const*>(&_Variant_Null));
	}
	
	template <class T>
	SLIB_INLINE AtomicVariant& Atomic<Variant>::operator=(const T& value)
	{
		set(value);
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
	void Atomic<Variant>::get(Ref<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Atomic<Variant>::set(const Ref<T>& _in)
	{
		setObject(_in);
	}
	
	template <class T>
	void Atomic<Variant>::get(AtomicRef<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Atomic<Variant>::set(const AtomicRef<T>& _in)
	{
		setObject(_in);
	}
	
	template <class T>
	void Atomic<Variant>::get(WeakRef<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Atomic<Variant>::set(const WeakRef<T>& _in)
	{
		setWeak(_in);
	}
	
	template <class T>
	void Atomic<Variant>::get(AtomicWeakRef<T>& _out) const
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	void Atomic<Variant>::set(const AtomicWeakRef<T>& _in)
	{
		setWeak(_in);
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
