#ifndef CHECKHEADER_SLIB_CORE_DETAIL_JSON
#define CHECKHEADER_SLIB_CORE_DETAIL_JSON

#include "../json.h"

namespace slib
{
	
	template <class T>
	SLIB_INLINE void Json::fromJson(const Variant& v, T& _out)
	{
		_out.fromJson(v);
	}
	
	template <class T>
	SLIB_INLINE void Json::fromJson(const Variant& v, T& _out, const T& _def)
	{
		_out.fromJson(v);
	}
	
	template <class T>
	SLIB_INLINE Variant Json::toJson(const T& _in)
	{
		return _in.toJson();
	}
	
	template <class T>
	SLIB_INLINE void Json::get(const Variant& v, const String& fieldName, T& _out)
	{
		Map<String, Variant> map = v.getVariantMap();
		Variant item;
		if (map.get(fieldName, &item)) {
			Json::fromJson(item, _out);
		}
	}
	
	template <class T>
	SLIB_INLINE void Json::put(const Variant& v, const String& fieldName, const T& _in)
	{
		Map<String, Variant> map = v.getVariantMap();
		map.put(fieldName, Json::toJson(_in));
	}
	
	template <class T>
	SLIB_INLINE void Json::get(const Variant& v, sl_size index, T& _out)
	{
		List<Variant> list = v.getVariantList();
		Variant item;
		if (list.getAt(index, &item)) {
			Json::fromJson(item, _out);
		}
	}
	
	template <class T>
	SLIB_INLINE void Json::add(const Variant& v, const T& _in)
	{
		List<Variant> list = v.getVariantList();
		if (list.isNotNull()) {
			list.add(Json::toJson(_in));
		}
	}


	template <class T>
	void Json::fromJson(const Variant& v, List<T>& _out)
	{
		List<T> dst;
		Ref<Referable> obj(v.getObject());
		if (obj.isNotNull()) {
			if (CList<Variant>* _src = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*_src);
				for (sl_size i = 0; i < src.count; i++) {
					Variant& v = src[i];
					T o;
					Json::fromJson(v, o);
					dst.add_NoLock(o);
				}
			} else if (CList< Map<String, Variant> >* _src = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*_src);
				for (sl_size i = 0; i < src.count; i++) {
					Variant v(src[i]);
					T o;
					Json::fromJson(v, o);
					dst.add_NoLock(o);
				}
			}
		}
		_out = dst;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, List<T>& _out, const List<T>& def)
	{
		if (v.isNotNull()) {
			Json::fromJson(v, _out);
		} else {
			_out = def;
		}
	}
	
	template <class T>
	Variant Json::toJson(const List<T>& _in)
	{
		List<Variant> list;
		if (_in.isNotNull()) {
			ListLocker<T> src(_in);
			for (sl_size i = 0; i < src.count; i++) {
				T& o = src[i];
				list.add_NoLock(Json::toJson(o));
			}
			return list;
		}
		return sl_null;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicList<T>& _out)
	{
		List<T> __out;
		Json::fromJson(v, __out);
		_out = __out;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicList<T>& _out, const AtomicList<T>& def)
	{
		if (v.isNotNull()) {
			Json::fromJson(v, _out);
		} else {
			_out = def;
		}
	}
	
	template <class T>
	Variant Json::toJson(const AtomicList<T>& _in)
	{
		return Json::toJson(List<T>(_in));
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, Map<String, T>& _out)
	{
		Iterator< Pair<String, Variant> > iterator = v.getVariantMap().toIterator();
		if (iterator.isNotNull()) {
			Map<String, T> map;
			map.initHash();
			Pair<String, Variant> pair;
			while (iterator.next(&pair)) {
				T o;
				Json::fromJson(pair.value, o);
				map.put_NoLock(pair.key, o);
			}
			_out = map;
		} else {
			_out.setNull();
		}
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, Map<String, T>& _out, const Map<String, T>& def)
	{
		if (v.isNotNull()) {
			Json::fromJson(v, _out);
		} else {
			_out = def;
		}
	}
	
	template <class T>
	Variant Json::toJson(const Map<String, T>& _in)
	{
		Map<String, Variant> map;
		Iterator< Pair<String, T> > iterator = _in.toIterator();
		if (iterator.isNotNull()) {
			map.initList();
			Pair<String, T> pair;
			while (iterator.next(&pair)) {
				map.put_NoLock(pair.key, Json::toJson(pair.value), MapPutMode::AddAlways);
			}
		}
		return map;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicMap<String, T>& _out)
	{
		Map<String, T> __out;
		Json::fromJson(v, __out);
		_out = __out;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicMap<String, T>& _out, const AtomicMap<String, T>& def)
	{
		if (v.isNotNull()) {
			Json::fromJson(v, _out);
		} else {
			_out = def;
		}
	}
	
	template <class T>
	Variant Json::toJson(const AtomicMap<String, T>& _in)
	{
		return Json::toJson(Map<String, T>(_in));
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, Ref<T>& _out)
	{
		if (v.isNotNull()) {
			Ref<T> o = new T;
			if (o.isNotNull()) {
				Json::fromJson(v, *o);
				_out = o;
				return;
			}
		}
		_out.setNull();
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, Ref<T>& _out, const Ref<T>& def)
	{
		if (v.isNotNull()) {
			fromJson(v, _out);
		} else {
			_out = def;
		}
	}
	
	template <class T>
	Variant Json::toJson(const Ref<T>& _in)
	{
		if (_in.isNotNull()) {
			return toJson(*_in);
		}
		return sl_null;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicRef<T>& _out)
	{
		Ref<T> __out;
		fromJson(v, __out);
		_out = __out;
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicRef<T>& _out, const AtomicRef<T>& def)
	{
		if (v.isNotNull()) {
			fromJson(v, _out);
		} else {
			_out = def;
		}
	}
	
	template <class T>
	Variant Json::toJson(const AtomicRef<T>& _in)
	{
		return toJson(Ref<T>(_in));
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, WeakRef<T>& _out)
	{
		_out.setNull();
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, WeakRef<T>& _out, const WeakRef<T>& def)
	{
		_out = def;
	}
	
	template <class T>
	Variant Json::toJson(const WeakRef<T>& _in)
	{
		return toJson(Ref<T>(_in));
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicWeakRef<T>& _out)
	{
		_out.setNull();
	}
	
	template <class T>
	void Json::fromJson(const Variant& v, AtomicWeakRef<T>& _out, const AtomicWeakRef<T>& def)
	{
		_out = def;
	}
	
	template <class T>
	Variant Json::toJson(const AtomicWeakRef<T>& _in)
	{
		return toJson(Ref<T>(_in));
	}

}

#endif
