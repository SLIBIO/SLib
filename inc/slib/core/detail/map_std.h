#ifndef CHECKHEADER_SLIB_CORE_DETAIL_MAP_STD
#define CHECKHEADER_SLIB_CORE_DETAIL_MAP_STD

#include "../map.h"

#include <initializer_list>

namespace slib
{

	template <class KT, class VT, class KEY_EQUALS>
	ListMap<KT, VT, KEY_EQUALS>::ListMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_EQUALS& equals): m_equals(equals), list(l)
	{
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	ListMap<KT, VT, KEY_EQUALS>* ListMap<KT, VT, KEY_EQUALS>::create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_EQUALS& key_equals)
	{
		return new ListMap<KT, VT, KEY_EQUALS>(l, key_equals);
	}
	
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals) : table(capacity)
	{
		const Pair<KT, VT>* data = l.begin();
		for (sl_size i = 0; i < l.size(); i++) {
			table.put(data[i].key, data[i].value, MapPutMode::AddAlways, sl_null);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>* HashMap<KT, VT, HASH, KEY_EQUALS>::create(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals)
	{
		HashMap<KT, VT, HASH, KEY_EQUALS>* ret = new HashMap<KT, VT, HASH, KEY_EQUALS>(l, capacity, hash, key_equals);
		if (ret) {
			if (ret->table.getCapacity() > 0) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>::TreeMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare) : tree(key_compare)
	{
		const Pair<KT, VT>* data = l.begin();
		for (sl_size i = 0; i < l.size(); i++) {
			tree.put(data[i].key, data[i].value, MapPutMode::AddAlways, sl_null);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>* TreeMap<KT, VT, KEY_COMPARE>::create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare)
	{
		TreeMap<KT, VT, KEY_COMPARE>* ret = new TreeMap<KT, VT, KEY_COMPARE>(l, key_compare);
		if (ret) {
			if (ret->tree.isValid()) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}

	
	template <class KT, class VT>
	Map<KT, VT>::Map(const std::initializer_list< Pair<KT, VT> >& l): ref(HashMap<KT, VT>::create(l))
	{
	}
	
	template <class KT, class VT>
	template <class KEY_EQUALS>
	Map<KT, VT> Map<KT, VT>::createList(const std::initializer_list< Pair<KT, VT> >& l, const KEY_EQUALS& key_equals)
	{
		return ListMap<KT, VT, KEY_EQUALS>::create(l, key_equals);
	}
	
	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	Map<KT, VT> Map<KT, VT>::createHash(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals)
	{
		return HashMap<KT, VT, HASH, KEY_EQUALS>::create(l, initialCapacity, hash, key_equals);
	}
	
	template <class KT, class VT>
	template <class KEY_COMPARE>
	Map<KT, VT> Map<KT, VT>::createTree(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare)
	{
		return TreeMap<KT, VT, KEY_COMPARE>::create(l, key_compare);
	}
	
	
	template <class KT, class VT>
	Atomic< Map<KT, VT> >::Atomic(const std::initializer_list< Pair<KT, VT> >& l): ref(HashMap<KT, VT>::create(l))
	{
	}
	
}

#endif
