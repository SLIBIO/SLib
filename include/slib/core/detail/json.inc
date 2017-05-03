/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	template <class T>
	SLIB_INLINE Json::Json(const Ref<T>& ref)
	{
		set(ref);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const AtomicRef<T>& ref)
	{
		set(ref);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const WeakRef<T>& weak)
	{
		set(weak);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const AtomicWeakRef<T>& weak)
	{
		set(weak);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const List<T>& list)
	{
		set(list);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const AtomicList<T>& list)
	{
		set(list);
	}
		
	template <class KT, class VT>
	SLIB_INLINE Json::Json(const Map<KT, VT>& map)
	{
		set(map);
	}

	template <class KT, class VT>
	SLIB_INLINE Json::Json(const AtomicMap<KT, VT>& map)
	{
		set(map);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const T& value)
	{
		set(value);
	}
	
	SLIB_INLINE const Json& Json::null()
	{
		return *(reinterpret_cast<Json const*>(&_priv_Variant_Null));
	}

	template <class T>
	SLIB_INLINE Json& Json::operator=(const T& value)
	{
		set(value);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE void Json::getElement(sl_size index, T& _out) const
	{
		getElement(index).get(_out);
	}
	
	template <class T>
	SLIB_INLINE void Json::getItem(const String& key, T& _out) const
	{
		getItem(key).get(_out);
	}

	template <class T>
	void Json::get(Ref<T>& _out) const
	{
		if (isNotNull()) {
			Ref<T> o = new T;
			if (o.isNotNull()) {
				o->fromJson(*this);
				_out = Move(o);
				return;
			}
		}
		_out.setNull();
	}
	
	template <class T>
	SLIB_INLINE void Json::set(const Ref<T>& _in)
	{
		if (_in.isNotNull()) {
			*this = _in->toJson();
		} else {
			setNull();
		}
	}
	
	template <class T>
	SLIB_INLINE void Json::get(AtomicRef<T>& _out) const
	{
		Ref<T> t;
		get(t);
		_out = Move(t);
	}
	
	template <class T>
	SLIB_INLINE void Json::set(const AtomicRef<T>& _in)
	{
		set(Ref<T>(_in));
	}
	
	template <class T>
	SLIB_INLINE void Json::set(const WeakRef<T>& _in)
	{
		set(Ref<T>(_in));
	}
	
	template <class T>
	SLIB_INLINE void Json::set(const AtomicWeakRef<T>& _in)
	{
		set(Ref<T>(_in));
	}

	template <class T>
	void Json::get(List<T>& _out) const
	{
		List<T> dst;
		Ref<Referable> obj(getObject());
		if (obj.isNotNull()) {
			if (CList<Variant>* _src = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*_src);
				for (sl_size i = 0; i < src.count; i++) {
					Json& v = *(static_cast<Json*>(&(src[i])));
					T o;
					v.get(o);
					dst.add_NoLock(Move(o));
				}
			} else if (CList< Map<String, Variant> >* _src = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*_src);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					v.get(o);
					dst.add_NoLock(Move(o));
				}
			}
		}
		_out = dst;
	}
	
	template <class T>
	void Json::set(const List<T>& _in)
	{
		if (_in.isNotNull()) {
			List<Variant> list;
			ListLocker<T> src(_in);
			for (sl_size i = 0; i < src.count; i++) {
				T& o = src[i];
				list.add_NoLock(Json(o));
			}
			*this = list;
		} else {
			setNull();
		}
	}
	
	template <class T>
	SLIB_INLINE void Json::get(AtomicList<T>& _out) const
	{
		List<T> t;
		get(t);
		_out = Move(t);
	}
	
	template <class T>
	SLIB_INLINE void Json::set(const AtomicList<T>& _in)
	{
		set(List<T>(_in));
	}
	
	template <class KT, class VT>
	void Json::get(Map<KT, VT>& _out) const
	{
		VariantMap _map = getVariantMap();
		if (_map.isNotNull()) {
			MutexLocker locker(_map.getLocker());
			Iterator< Pair<String, Variant> > iterator = _map.toIterator();
			if (iterator.isNotNull()) {
				Map<KT, VT> map;
				map.initHash();
				Pair<String, Variant> pair;
				while (iterator.next(&pair)) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					map.put_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
				_out = map;
				return;
			}
		}
		_out.setNull();
	}
	
	template <class KT, class VT>
	void Json::set(const Map<KT, VT>& _in)
	{
		if (_in.isNotNull()) {
			MutexLocker locker(_in.getLocker());
			Map<String, Variant> map;
			Iterator< Pair<KT, VT> > iterator = _in.toIterator();
			if (iterator.isNotNull()) {
				map.initHash();
				Pair<KT, VT> pair;
				while (iterator.next(&pair)) {
					map.put_NoLock(Cast<KT, String>()(pair.key), Json(pair.value), MapPutMode::AddAlways);
				}
			}
			*this = map;
		} else {
			setNull();
		}
	}
	
	template <class KT, class VT>
	SLIB_INLINE void Json::get(AtomicMap<KT, VT>& _out) const
	{
		Map<KT, VT> t;
		get(t);
		_out = Move(t);
	}
	
	template <class KT, class VT>
	SLIB_INLINE void Json::set(const AtomicMap<KT, VT>& _in)
	{
		set(Map<KT, VT>(_in));
	}
	
	template <class T>
	SLIB_INLINE void Json::get(T& _out) const
	{
		_out.fromJson(*this);
	}
	
	template <class T>
	SLIB_INLINE void Json::set(const T& _in)
	{
		*this = _in.toJson();
	}
	
	
	template <>
	SLIB_INLINE sl_object_type IMap<String, Json>::ObjectType() noexcept
	{
		return _priv_VariantMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool IMap<String, Json>::checkObjectType(sl_object_type type) noexcept
	{
		if (type == _priv_VariantMap_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::checkObjectType(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type IMap<String, Json>::getObjectType() const noexcept
	{
		return _priv_VariantMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool IMap<String, Json>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantMap_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::checkObjectType(type);
	}
	
		template <>
	SLIB_INLINE sl_object_type CList<Json>::ObjectType() noexcept
	{
		return _priv_VariantList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList<Json>::checkObjectType(sl_object_type type) noexcept
	{
		if (type == _priv_VariantList_ClassID || type == _priv_List_ClassID) {
			return sl_true;
		}
		return Object::checkObjectType(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList<Json>::getObjectType() const noexcept
	{
		return _priv_VariantList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList<Json>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantList_ClassID || type == _priv_List_ClassID) {
			return sl_true;
		}
		return Object::checkObjectType(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList< Map<String, Json> >::ObjectType() noexcept
	{
		return _priv_VariantMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< Map<String, Json> >::checkObjectType(sl_object_type type) noexcept
	{
		if (type == _priv_VariantMapList_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::checkObjectType(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList< Map<String, Json> >::getObjectType() const noexcept
	{
		return _priv_VariantMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< Map<String, Json> >::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_VariantMapList_ClassID || type == _priv_Map_ClassID) {
			return sl_true;
		}
		return Object::checkObjectType(type);
	}
	
}
