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
	
	template <class T>
	SLIB_INLINE Json::Json(const Nullable<T>& value) : Variant(value) {}
	
	template <class T>
	SLIB_INLINE Json::Json(const Ref<T>& ref)
	{
		ToJson(*this, ref);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const AtomicRef<T>& ref)
	{
		ToJson(*this, ref);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const WeakRef<T>& weak)
	{
		ToJson(*this, weak);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const AtomicWeakRef<T>& weak)
	{
		ToJson(*this, weak);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const List<T>& list)
	{
		ToJson(*this, list);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const AtomicList<T>& list)
	{
		ToJson(*this, list);
	}
	
	template <class T>
	SLIB_INLINE Json::Json(const ListParam<T>& list)
	{
		ToJson(*this, list);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const Map<KT, VT, KEY_COMPARE>& map)
	{
		ToJson(*this, map);
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const AtomicMap<KT, VT, KEY_COMPARE>& map)
	{
		ToJson(*this, map);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const HashMap<KT, VT, HASH, KEY_COMPARE>& map)
	{
		ToJson(*this, map);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE Json::Json(const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& map)
	{
		ToJson(*this, map);
	}

	SLIB_INLINE const Json& Json::undefined()
	{
		return *(reinterpret_cast<Json const*>(&(priv::variant::g_undefined)));
	}

	SLIB_INLINE const Json& Json::null()
	{
		return *(reinterpret_cast<Json const*>(&(priv::variant::g_null)));
	}

	template <class T>
	SLIB_INLINE void Json::getElement(sl_size index, T& _out) const
	{
		FromJson(getElement(index), _out);
	}
	
	template <class T>
	SLIB_INLINE void Json::getItem(const String& key, T& _out) const
	{
		FromJson(getItem(key), _out);
	}

	template <class T>
	SLIB_INLINE void FromJson(const Json& json, Nullable<T>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		if (json.isNull()) {
			_out.setNull();
		} else {
			_out.flagNull = sl_false;
			FromJson(json, _out.value);
		}
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const Nullable<T>& _in)
	{
		if (_in.isNull()) {
			json.setNull();
		} else {
			ToJson(json, _in.value);
		}
	}
	
	template <class T>
	SLIB_INLINE void FromJson(const Json& json, Ref<T>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		if (json.isNotNull()) {
			Ref<T> o = new T;
			if (o.isNotNull()) {
				FromJson(json, *(o._ptr));
				_out = Move(o);
				return;
			}
		}
		_out.setNull();
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const Ref<T>& _in)
	{
		if (_in.isNotNull()) {
			json = _in->toJson();
		} else {
			json.setNull();
		}
	}
	
	template <class T>
	SLIB_INLINE void FromJson(const Json& json, AtomicRef<T>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		Ref<T> t;
		FromJson(json, t);
		_out = Move(t);
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const AtomicRef<T>& _in)
	{
		ToJson(json, Ref<T>(_in));
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const WeakRef<T>& _in)
	{
		ToJson(json, Ref<T>(_in));
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const AtomicWeakRef<T>& _in)
	{
		ToJson(json, Ref<T>(_in));
	}

	template <class T>
	void FromJson(const Json& json, List<T>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		List<T> dst;
		Ref<Referable> obj(json.getObject());
		if (obj.isNotNull()) {
			if (CList<Variant>* s1 = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*s1);
				for (sl_size i = 0; i < src.count; i++) {
					Json& v = *(static_cast<Json*>(&(src[i])));
					T o;
					FromJson(v, o);
					dst.add_NoLock(Move(o));
				}
			} else if (CList< Map<String, Variant> >* s2 = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*s2);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					FromJson(v, o);
					dst.add_NoLock(Move(o));
				}
			} else if (CList< HashMap<String, Variant> >* s3 = CastInstance< CList< HashMap<String, Variant> > >(obj.get())) {
				ListLocker< HashMap<String, Variant> > src(*s3);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					FromJson(v, o);
					dst.add_NoLock(Move(o));
				}
			}
		}
		_out = Move(dst);
	}
	
	template <class T>
	void ToJson(Json& json, const List<T>& _in)
	{
		if (_in.isNotNull()) {
			JsonList list;
			ListLocker<T> src(_in);
			for (sl_size i = 0; i < src.count; i++) {
				T& o = src[i];
				list.add_NoLock(Json(o));
			}
			json = Move(list);
		} else {
			json.setNull();
		}
	}
	
	template <class T>
	SLIB_INLINE void FromJson(const Json& json, AtomicList<T>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		List<T> t;
		FromJson(json, t);
		_out = Move(t);
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const AtomicList<T>& _in)
	{
		ToJson(json, List<T>(_in));
	}
	
	template <class T>
	void ToJson(Json& json, const ListParam<T>& _in)
	{
		if (_in.isNotNull()) {
			JsonList list;
			ListLocker<T> src(_in);
			for (sl_size i = 0; i < src.count; i++) {
				T& o = src[i];
				list.add_NoLock(Json(o));
			}
			json = Move(list);
		} else {
			json.setNull();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void FromJson(const Json& json, Map<KT, VT, KEY_COMPARE>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		Map<KT, VT, KEY_COMPARE> dst;
		Ref<Referable> obj(json.getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			}
		}
		_out = Move(dst);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void ToJson(Json& json, const Map<KT, VT, KEY_COMPARE>& _in)
	{
		if (_in.isNotNull()) {
			MutexLocker locker(_in.getLocker());
			JsonMap map;
			for (auto& pair : _in) {
				map.put_NoLock(Cast<KT, String>()(pair.key), Json(pair.value));
			}
			json = Move(map);
		} else {
			json.setNull();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void FromJson(const Json& json, AtomicMap<KT, VT, KEY_COMPARE>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		Map<KT, VT, KEY_COMPARE> t;
		FromJson(json, t);
		_out = Move(t);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void ToJson(Json& json, const AtomicMap<KT, VT, KEY_COMPARE>& _in)
	{
		ToJson(json, Map<KT, VT, KEY_COMPARE>(_in));
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void FromJson(const Json& json, HashMap<KT, VT, HASH, KEY_COMPARE>& _out)
	{
		if (json.isUndefined()) {
			return;
		}
		HashMap<KT, VT, HASH, KEY_COMPARE> dst;
		Ref<Referable> obj(json.getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					dst.add_NoLock(Cast<String, KT>()(pair.key), Move(o));
				}
			}
		}
		_out = Move(dst);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void ToJson(Json& json, const HashMap<KT, VT, HASH, KEY_COMPARE>& _in)
	{
		if (_in.isNotNull()) {
			MutexLocker locker(_in.getLocker());
			JsonMap map;
			for (auto& pair : _in) {
				map.put_NoLock(Cast<KT, String>()(pair.key), Json(pair.value));
			}
			json = Move(map);
		} else {
			json.setNull();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void FromJson(const Json& json, AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _out)
	{
		if (json.isUndefined()) {
			return;
		}		
		HashMap<KT, VT, HASH, KEY_COMPARE> t;
		FromJson(json, t);
		_out = Move(t);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void ToJson(Json& json, const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _in)
	{
		ToJson(json, HashMap<KT, VT, HASH, KEY_COMPARE>(_in));
	}

	namespace priv
	{
		namespace json
		{

			template <class T, sl_bool isClass=__is_class(T), sl_bool isEnum=__is_enum(T)>
			class Converter
			{
			};

			template <class T>
			class Converter<T, sl_true, sl_false>
			{
			public:
				SLIB_INLINE static void fromJson(const Json& json, T& _out)
				{
					_out.fromJson(json);
				}

				SLIB_INLINE static void toJson(Json& json, const T& _in)
				{
					json = _in.toJson();
				}
			};

			template <class T>
			class Converter<T, sl_false, sl_true>
			{
			public:
				SLIB_INLINE static void fromJson(const Json& json, T& _out)
				{
					_out = (T)(json.getInt64((sl_int64)_out));
				}

				SLIB_INLINE static void toJson(Json& json, const T& _in)
				{
					json.setInt64((sl_int64)_in);
				}
			};

		}
	}

	template <class T>
	SLIB_INLINE void FromJson(const Json& json, T& _out)
	{
		priv::json::Converter<T>::fromJson(json, _out);
	}
	
	template <class T>
	SLIB_INLINE void ToJson(Json& json, const T& _in)
	{
		priv::json::Converter<T>::toJson(json, _in);
	}

	template <class T>
	SLIB_INLINE Json::Json(const T& value)
	{
		ToJson(*this, value);
	}
	
	template <class T>
	SLIB_INLINE Json& Json::operator=(const T& value)
	{
		ToJson(*this, value);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE void Json::get(T& value) const
	{
		FromJson(*this, value);
	}

	template <class T>
	SLIB_INLINE void Json::get(T& value, const T& defaultValue) const
	{
		FromJson(*this, value, defaultValue);
	}

	template <class T>
	SLIB_INLINE void Json::set(const T& value)
	{
		ToJson(*this, value);
	}

	
	template <>
	SLIB_INLINE sl_object_type CMap<String, Json>::ObjectType() noexcept
	{
		return priv::variant::g_variantMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CMap<String, Json>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == priv::variant::g_variantMap_ClassID || type == priv::map::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CMap<String, Json>::getObjectType() const noexcept
	{
		return priv::variant::g_variantMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CMap<String, Json>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == priv::variant::g_variantMap_ClassID || type == priv::map::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CHashMap<String, Json>::ObjectType() noexcept
	{
		return priv::variant::g_variantHashMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CHashMap<String, Json>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == priv::variant::g_variantHashMap_ClassID || type == priv::hash_map::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CHashMap<String, Json>::getObjectType() const noexcept
	{
		return priv::variant::g_variantHashMap_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CHashMap<String, Json>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == priv::variant::g_variantHashMap_ClassID || type == priv::hash_map::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CList<Json>::ObjectType() noexcept
	{
		return priv::variant::g_variantList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList<Json>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == priv::variant::g_variantList_ClassID || type == priv::list::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList<Json>::getObjectType() const noexcept
	{
		return priv::variant::g_variantList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList<Json>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == priv::variant::g_variantList_ClassID || type == priv::list::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CList< Map<String, Json> >::ObjectType() noexcept
	{
		return priv::variant::g_variantMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< Map<String, Json> >::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == priv::variant::g_variantMapList_ClassID || type == priv::list::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList< Map<String, Json> >::getObjectType() const noexcept
	{
		return priv::variant::g_variantMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< Map<String, Json> >::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == priv::variant::g_variantMapList_ClassID || type == priv::list::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	
	template <>
	SLIB_INLINE sl_object_type CList< HashMap<String, Json> >::ObjectType() noexcept
	{
		return priv::variant::g_variantHashMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< HashMap<String, Json> >::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == priv::variant::g_variantHashMapList_ClassID || type == priv::list::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	template <>
	SLIB_INLINE sl_object_type CList< HashMap<String, Json> >::getObjectType() const noexcept
	{
		return priv::variant::g_variantHashMapList_ClassID;
	}
	
	template <>
	SLIB_INLINE sl_bool CList< HashMap<String, Json> >::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == priv::variant::g_variantHashMapList_ClassID || type == priv::list::g_classID) {
			return sl_true;
		}
		return Object::isDerivedFrom(type);
	}
	
	namespace priv
	{
		namespace json
		{
		
			class SLIB_EXPORT JsonFieldContainer : public StringContainer
			{
			public:
				SLIB_INLINE JsonFieldContainer(sl_char8* _sz, sl_size _len)
				{
					sz = _sz;
					len = _len;
					hash = 0;
					type = 0;
					ref = -1;
				}
			};
		
		}
	}

}
