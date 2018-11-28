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

namespace slib
{
	
	struct _priv_Variant_Const
	{
		sl_uint64 value;
		VariantType type;
		sl_int32 lock;
	};
	
	extern const _priv_Variant_Const _priv_Variant_Null;
	
	template <class... ARGS>
	String String::format(const sl_char8* szFormat, ARGS&&... args) noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(szFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String String::format(const String& strFormat, ARGS&&... args) noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(strFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 String16::format(const sl_char16* szFormat, ARGS&&... args) noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(szFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 String16::format(const String16& strFormat, ARGS&&... args) noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return formatBy(strFormat, params, sizeof...(args));
	}
	
	template <class... ARGS>
	String String::arg(ARGS&&... args) const noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 String16::arg(ARGS&&... args) const noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}
	
	template <class... ARGS>
	String Atomic<String>::arg(ARGS&&... args) const noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}
	
	template <class... ARGS>
	String16 Atomic<String16>::arg(ARGS&&... args) const noexcept
	{
		Variant params[] = {Forward<ARGS>(args)...};
		return argBy(params, sizeof...(args));
	}


	template <class T>
	SLIB_INLINE Variant::Variant(const Ref<T>& ref) noexcept
	{
		_constructorRef(&ref);
	}
	
	template <class T>
	SLIB_INLINE Variant::Variant(const AtomicRef<T>& ref) noexcept
	{
		_constructorAtomicRef(&ref);
	}
	
	template <class T>
	SLIB_INLINE Variant::Variant(const WeakRef<T>& weak) noexcept
	{
		_constructorWeakRef(&weak);
	}
	
	template <class T>
	SLIB_INLINE Variant::Variant(const AtomicWeakRef<T>& weak) noexcept
	{
		_constructorAtomicWeakRef(&weak);
	}
	
	SLIB_INLINE const Variant& Variant::null() noexcept
	{
		return *(reinterpret_cast<Variant const*>(&_priv_Variant_Null));
	}
	
	template <class T>
	SLIB_INLINE Variant Variant::fromRef(const Ref<T>& ref) noexcept
	{
		return ref;
	}
	
	template <class T>
	SLIB_INLINE Variant Variant::fromWeakRef(const WeakRef<T>& weak) noexcept
	{
		return weak;
	}
	
	template <class T>
	SLIB_INLINE Variant& Variant::operator=(const T& value) noexcept
	{
		set(value);
		return *this;
	}
	
	
	SLIB_INLINE VariantType Variant::getType() const noexcept
	{
		return _type;
	}
	
	SLIB_INLINE sl_bool Variant::isNull() const noexcept
	{
		return _type == VariantType::Null;
	}
	
	SLIB_INLINE sl_bool Variant::isNotNull() const noexcept
	{
		return _type != VariantType::Null;
	}
	
	
	template <class T>
	SLIB_INLINE Ref<T> Variant::getObject(const Ref<T>& def) const noexcept
	{
		return CastRef<T>(getObject(), def);
	}
	
	template <class T>
	SLIB_INLINE void Variant::setObject(const Ref<T>& ref) noexcept
	{
		_assignRef(&ref);
	}
	
	template <class T>
	SLIB_INLINE void Variant::setWeak(const WeakRef<T>& weak) noexcept
	{
		_assignWeakRef(&weak);
	}
	
	template <class T>
	SLIB_INLINE void Variant::get(Ref<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Variant::set(const Ref<T>& _in) noexcept
	{
		_assignRef(&_in);
	}
	
	template <class T>
	SLIB_INLINE void Variant::get(AtomicRef<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Variant::set(const AtomicRef<T>& _in) noexcept
	{
		_assignAtomicRef(&_in);
	}
	
	template <class T>
	SLIB_INLINE void Variant::get(WeakRef<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Variant::set(const WeakRef<T>& _in) noexcept
	{
		_assignWeakRef(&_in);
	}
	
	template <class T>
	SLIB_INLINE void Variant::get(AtomicWeakRef<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Variant::set(const AtomicWeakRef<T>& _in) noexcept
	{
		_assignAtomicWeakRef(&_in);
	}

	
	template <class T>
	SLIB_INLINE Atomic<Variant>::Atomic(const Ref<T>& ref) noexcept
	{
		_constructorRef(&ref);
	}
	
	template <class T>
	SLIB_INLINE Atomic<Variant>::Atomic(const AtomicRef<T>& ref) noexcept
	{
		_constructorAtomicRef(&ref);
	}
	
	template <class T>
	SLIB_INLINE Atomic<Variant>::Atomic(const WeakRef<T>& weak) noexcept
	{
		_constructorWeakRef(&weak);
	}
	
	template <class T>
	SLIB_INLINE Atomic<Variant>::Atomic(const AtomicWeakRef<T>& weak) noexcept
	{
		_constructorAtomicWeakRef(&weak);
	}
	
	SLIB_INLINE const AtomicVariant& Atomic<Variant>::null() noexcept
	{
		return *(reinterpret_cast<AtomicVariant const*>(&_priv_Variant_Null));
	}
	
	template <class T>
	SLIB_INLINE AtomicVariant& Atomic<Variant>::operator=(const T& value) noexcept
	{
		set(value);
		return *this;
	}
	
	SLIB_INLINE VariantType Atomic<Variant>::getType() const noexcept
	{
		return _type;
	}
	
	SLIB_INLINE sl_bool Atomic<Variant>::isNull() const noexcept
	{
		return _type == VariantType::Null;
	}
	
	SLIB_INLINE sl_bool Atomic<Variant>::isNotNull() const noexcept
	{
		return _type != VariantType::Null;
	}
	
	template <class T>
	SLIB_INLINE Ref<T> Atomic<Variant>::getObject(const Ref<T>& def) const noexcept
	{
		Variant var(*this);
		return var.getObject(def);
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::setObject(const Ref<T>& object) noexcept
	{
		_assignRef(&object);
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::setWeak(const WeakRef<T>& weak) noexcept
	{
		_assignWeakRef(&weak);
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::get(Ref<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::set(const Ref<T>& _in) noexcept
	{
		_assignRef(&_in);
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::get(AtomicRef<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::set(const AtomicRef<T>& _in) noexcept
	{
		_assignAtomicRef(&_in);
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::get(WeakRef<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::set(const WeakRef<T>& _in) noexcept
	{
		_assignWeakRef(&_in);
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::get(AtomicWeakRef<T>& _out) const noexcept
	{
		_out = CastRef<T>(getObject());
	}
	
	template <class T>
	SLIB_INLINE void Atomic<Variant>::set(const AtomicWeakRef<T>& _in) noexcept
	{
		_assignAtomicWeakRef(&_in);
	}
	
	
	extern const char _priv_VariantMap_ClassID[];
	extern const char _priv_VariantHashMap_ClassID[];
	extern const char _priv_VariantList_ClassID[];
	extern const char _priv_VariantMapList_ClassID[];
	extern const char _priv_VariantHashMapList_ClassID[];
	
	template <>
	SLIB_INLINE sl_object_type CMap<String, Variant>::ObjectType() noexcept
	{
		return _priv_VariantMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CMap<String, Variant>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == _priv_VariantMap_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CMap<String, Variant>::getObjectType() const noexcept
	{
		return _priv_VariantMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CMap<String, Variant>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantMap_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CHashMap<String, Variant>::ObjectType() noexcept
	{
		return _priv_VariantHashMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CHashMap<String, Variant>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == _priv_VariantHashMap_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CHashMap<String, Variant>::getObjectType() const noexcept
	{
		return _priv_VariantHashMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CHashMap<String, Variant>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantHashMap_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CList<Variant>::ObjectType() noexcept
	{
		return _priv_VariantList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList<Variant>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == _priv_VariantList_ClassID || type == _priv_List_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList<Variant>::getObjectType() const noexcept
	{
		return _priv_VariantList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList<Variant>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantList_ClassID || type == _priv_List_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CList< Map<String, Variant> >::ObjectType() noexcept
	{
		return _priv_VariantMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< Map<String, Variant> >::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == _priv_VariantMapList_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList< Map<String, Variant> >::getObjectType() const noexcept
	{
		return _priv_VariantMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< Map<String, Variant> >::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantMapList_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CList< HashMap<String, Variant> >::ObjectType() noexcept
	{
		return _priv_VariantHashMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< HashMap<String, Variant> >::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == _priv_VariantHashMapList_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList< HashMap<String, Variant> >::getObjectType() const noexcept
	{
		return _priv_VariantHashMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< HashMap<String, Variant> >::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantHashMapList_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
}
