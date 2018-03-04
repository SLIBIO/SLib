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
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const Map<KT, VT, KEY_COMPARE>& map)
	{
		set(map);
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const AtomicMap<KT, VT, KEY_COMPARE>& map)
	{
		set(map);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const HashMap<KT, VT, HASH, KEY_COMPARE>& map)
	{
		set(map);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& map)
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
			if (CList<Variant>* s1 = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*s1);
				for (sl_size i = 0; i < src.count; i++) {
					Json& v = *(static_cast<Json*>(&(src[i])));
					T o;
					v.get(o);
					dst.add_NoLock(Move(o));
				}
			} else if (CList< Map<String, Variant> >* s2 = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*s2);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					v.get(o);
					dst.add_NoLock(Move(o));
				}
			} else if (CList< HashMap<String, Variant> >* s3 = CastInstance< CList< HashMap<String, Variant> > >(obj.get())) {
				ListLocker< HashMap<String, Variant> > src(*s3);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					v.get(o);
					dst.add_NoLock(Move(o));
				}
			}
		}
		_out = Move(dst);
	}
	
	template <class T>
	void Json::set(const List<T>& _in)
	{
		if (_in.isNotNull()) {
			JsonList list;
			ListLocker<T> src(_in);
			for (sl_size i = 0; i < src.count; i++) {
				T& o = src[i];
				list.add_NoLock(Json(o));
			}
			*this = Move(list);
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
	
	template <class KT, class VT, class KEY_COMPARE>
	void Json::get(Map<KT, VT, KEY_COMPARE>& _out) const
	{
		Map<KT, VT, KEY_COMPARE> dst;
		Ref<Referable> obj(getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			}
		}
		_out = Move(dst);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void Json::set(const Map<KT, VT, KEY_COMPARE>& _in)
	{
		if (_in.isNotNull()) {
			MutexLocker locker(_in.getLocker());
			JsonMap map;
			for (auto& pair : _in) {
				map.put_NoLock(Cast<KT, String>()(pair.key), Json(pair.value));
			}
			*this = Move(map);
		} else {
			setNull();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void Json::get(AtomicMap<KT, VT, KEY_COMPARE>& _out) const
	{
		Map<KT, VT, KEY_COMPARE> t;
		get(t);
		_out = Move(t);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void Json::set(const AtomicMap<KT, VT, KEY_COMPARE>& _in)
	{
		set(Map<KT, VT, KEY_COMPARE>(_in));
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void Json::get(HashMap<KT, VT, HASH, KEY_COMPARE>& _out) const
	{
		HashMap<KT, VT, HASH, KEY_COMPARE> dst;
		Ref<Referable> obj(getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			}
		}
		_out = Move(dst);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void Json::set(const HashMap<KT, VT, HASH, KEY_COMPARE>& _in)
	{
		if (_in.isNotNull()) {
			MutexLocker locker(_in.getLocker());
			JsonMap map;
			for (auto& pair : _in) {
				map.put_NoLock(Cast<KT, String>()(pair.key), Json(pair.value));
			}
			*this = Move(map);
		} else {
			setNull();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void Json::get(AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _out) const
	{
		HashMap<KT, VT, HASH, KEY_COMPARE> t;
		get(t);
		_out = Move(t);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void Json::set(const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _in)
	{
		set(HashMap<KT, VT, HASH, KEY_COMPARE>(_in));
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
	
	
	template <> sl_object_type CMap<String, Json>::ObjectType() noexcept;
	template <> sl_bool CMap<String, Json>::isDerivedFrom(sl_object_type type) noexcept;
	template <> sl_object_type CMap<String, Json>::getObjectType() const noexcept;
	template <> sl_bool CMap<String, Json>::isInstanceOf(sl_object_type type) const noexcept;
	
	template <> sl_object_type CHashMap<String, Json>::ObjectType() noexcept;
	template <> sl_bool CHashMap<String, Json>::isDerivedFrom(sl_object_type type) noexcept;
	template <> sl_object_type CHashMap<String, Json>::getObjectType() const noexcept;
	template <>	sl_bool CHashMap<String, Json>::isInstanceOf(sl_object_type type) const noexcept;
	
	template <> sl_object_type CList<Json>::ObjectType() noexcept;
	template <> sl_bool CList<Json>::isDerivedFrom(sl_object_type type) noexcept;
	template <> sl_object_type CList<Json>::getObjectType() const noexcept;
	template <> sl_bool CList<Json>::isInstanceOf(sl_object_type type) const noexcept;
	
	template <> sl_object_type CList< Map<String, Json> >::ObjectType() noexcept;
	template <> sl_bool CList< Map<String, Json> >::isDerivedFrom(sl_object_type type) noexcept;
	template <>	sl_object_type CList< Map<String, Json> >::getObjectType() const noexcept;
	template <> sl_bool CList< Map<String, Json> >::isInstanceOf(sl_object_type type) const noexcept;
	
	template <> sl_object_type CList< HashMap<String, Json> >::ObjectType() noexcept;
	template <> sl_bool CList< HashMap<String, Json> >::isDerivedFrom(sl_object_type type) noexcept;
	template <> sl_object_type CList< HashMap<String, Json> >::getObjectType() const noexcept;
	template <> sl_bool CList< HashMap<String, Json> >::isInstanceOf(sl_object_type type) const noexcept;
	
}
