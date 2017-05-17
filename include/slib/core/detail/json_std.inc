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
			if (CList<Variant>* _src = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*_src);
				for (sl_size i = 0; i < src.count; i++) {
					Json& v = *(static_cast<Json*>(&(src[i])));
					T o;
					v.get(o);
					_out.emplace_back(Move(o));
				}
			} else if (CList< Map<String, Variant> >* _src = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*_src);
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
		List<Variant> list;
		for (auto& item : _in) {
			list.add_NoLock(Json(item));
		}
		*this = list;
	}

	template <class KT, class VT, class COMPARE, class ALLOC>
	void Json::get(std::map<KT, VT, COMPARE, ALLOC>& _out) const
	{
		_out.clear();
		VariantMap map = getVariantMap();
		if (map.isNotNull()) {
			MutexLocker locker(map.getLocker());
			for (auto& pair : map) {
				Json& v = *(static_cast<Json*>(&(pair.value)));
				VT o;
				v.get(o);
				_out.emplace(pair.key, Move(o));
			}
		}
	}
	
	template <class KT, class VT, class COMPARE, class ALLOC>
	void Json::set(const std::map<KT, VT, COMPARE, ALLOC>& _in)
	{
		Map<String, Variant> map;
		for (auto& item : _in) {
			map.put_NoLock(Cast<KT, String>()(item.first), Json(item.second), MapPutMode::AddAlways);
		}
		*this = map;
	}

	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void Json::get(std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _out) const
	{
		_out.clear();
		VariantMap map = getVariantMap();
		if (map.isNotNull()) {
			MutexLocker locker(map.getLocker());
			for (auto& pair : map) {
				Json& v = *(static_cast<Json*>(&(pair.value)));
				VT o;
				v.get(o);
				_out.emplace(pair.key, Move(o));
			}
		}
	}

	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void Json::set(const std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _in)
	{
		Map<String, Variant> map;
		for	(auto& item : _in) {
			map.put_NoLock(Cast<KT, String>()(item.first), Json(item.second), MapPutMode::AddAlways);
		}
		*this = map;
	}

}
