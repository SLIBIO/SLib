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
	void Json::get(std::vector<T>& _out) const
	{
		_out.clear();
		Ref<Referable> obj(getObject());
		if (obj.isNotNull()) {
			if (CList<Variant>* s1 = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*s1);
				for (sl_size i = 0; i < src.count; i++) {
					Json& v = *(static_cast<Json*>(&(src[i])));
					T o;
					v.get(o);
					_out.emplace_back(Move(o));
				}
			} else if (CList< Map<String, Variant> >* s2 = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*s2);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					v.get(o);
					_out.emplace_back(Move(o));
				}
			} else if (CList< HashMap<String, Variant> >* s3 = CastInstance< CList< HashMap<String, Variant> > >(obj.get())) {
				ListLocker< HashMap<String, Variant> > src(*s3);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					v.get(o);
					_out.emplace_back(Move(o));
				}
			}
		}
	}
	
	template <class T>
	void Json::set(const std::vector<T>& _in)
	{
		JsonList list;
		for (auto& item : _in) {
			list.add_NoLock(Json(item));
		}
		*this = Move(list);
	}

	template <class KT, class VT, class COMPARE, class ALLOC>
	void Json::get(std::map<KT, VT, COMPARE, ALLOC>& _out) const
	{
		_out.clear();
		Ref<Referable> obj(getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				const CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					_out.emplace(pair.key, Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				const CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					_out.emplace(pair.key, Move(o));
				}
			}
		}
	}
	
	template <class KT, class VT, class COMPARE, class ALLOC>
	void Json::set(const std::map<KT, VT, COMPARE, ALLOC>& _in)
	{
		JsonMap map;
		for (auto& item : _in) {
			map.put_NoLock(Cast<KT, String>()(item.first), Json(item.second));
		}
		*this = Move(map);
	}

	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void Json::get(std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _out) const
	{
		_out.clear();
		Ref<Referable> obj(getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				const CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					_out.emplace(pair.key, Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				const CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					v.get(o);
					_out.emplace(pair.key, Move(o));
				}
			}
		}
	}

	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void Json::set(const std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _in)
	{
		JsonMap map;
		for	(auto& item : _in) {
			map.put_NoLock(Cast<KT, String>()(item.first), Json(item.second));
		}
		*this = Move(map);
	}

}
