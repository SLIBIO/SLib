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
	void FromJson(const Json& json, std::vector<T>& _out)
	{
		_out.clear();
		Ref<Referable> obj(json.getObject());
		if (obj.isNotNull()) {
			if (CList<Variant>* s1 = CastInstance< CList<Variant> >(obj.get())) {
				ListLocker<Variant> src(*s1);
				for (sl_size i = 0; i < src.count; i++) {
					Json& v = *(static_cast<Json*>(&(src[i])));
					T o;
					FromJson(v, o);
					_out.emplace_back(Move(o));
				}
			} else if (CList< Map<String, Variant> >* s2 = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
				ListLocker< Map<String, Variant> > src(*s2);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					FromJson(v, o);
					_out.emplace_back(Move(o));
				}
			} else if (CList< HashMap<String, Variant> >* s3 = CastInstance< CList< HashMap<String, Variant> > >(obj.get())) {
				ListLocker< HashMap<String, Variant> > src(*s3);
				for (sl_size i = 0; i < src.count; i++) {
					Json v(src[i]);
					T o;
					FromJson(v, o);
					_out.emplace_back(Move(o));
				}
			}
		}
	}
	
	template <class T>
	void ToJson(Json& json, const std::vector<T>& _in)
	{
		JsonList list;
		for (auto& item : _in) {
			list.add_NoLock(Json(item));
		}
		json = Move(list);
	}

	template <class KT, class VT, class COMPARE, class ALLOC>
	void FromJson(const Json& json, std::map<KT, VT, COMPARE, ALLOC>& _out)
	{
		_out.clear();
		Ref<Referable> obj(json.getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				const CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					_out.emplace(pair.key, Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				const CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					_out.emplace(pair.key, Move(o));
				}
			}
		}
	}
	
	template <class KT, class VT, class COMPARE, class ALLOC>
	void ToJson(Json& json, const std::map<KT, VT, COMPARE, ALLOC>& _in)
	{
		JsonMap map;
		for (auto& item : _in) {
			map.put_NoLock(Cast<KT, String>()(item.first), Json(item.second));
		}
		json = Move(map);
	}

	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void FromJson(const Json& json, std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _out)
	{
		_out.clear();
		Ref<Referable> obj(json.getObject());
		if (obj.isNotNull()) {
			if (CMap<String, Variant>* s1 = CastInstance< CMap<String, Variant> >(obj.get())) {
				const CMap<String, Variant>& map = *s1;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					_out.emplace(pair.key, Move(o));
				}
			} else if (CHashMap<String, Variant>* s2 = CastInstance< CHashMap<String, Variant> >(obj.get())) {
				const CHashMap<String, Variant>& map = *s2;
				MutexLocker locker(map.getLocker());
				for (auto& pair : map) {
					Json& v = *(static_cast<Json*>(&(pair.value)));
					VT o;
					FromJson(v, o);
					_out.emplace(pair.key, Move(o));
				}
			}
		}
	}

	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void ToJson(Json& json, const std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _in)
	{
		JsonMap map;
		for	(auto& item : _in) {
			map.put_NoLock(Cast<KT, String>()(item.first), Json(item.second));
		}
		json = Move(map);
	}

}
