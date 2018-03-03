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
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	CHashMap<KT, VT, HASH, KEY_COMPARE>::CHashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : m_hash(hash), m_compare(compare)
	{
		_priv_HashTable::init(reinterpret_cast<HashTableStructBase*>(&m_table), capacityMinimum, capacityMaximum);
		m_nodeFirst = sl_null;
		m_nodeLast = sl_null;
		const Pair<KT, VT>* data = l.begin();
		for (sl_size i = 0; i < l.size(); i++) {
			add_NoLock(data[i].key, data[i].value);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE>::HashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : ref(new CMAP(l, capacityMinimum, capacityMaximum, hash, compare))
	{}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE> HashMap<KT, VT, HASH, KEY_COMPARE>::create(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	{
		return new CMAP(l, capacityMinimum, capacityMaximum, hash, compare);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE>& HashMap<KT, VT, HASH, KEY_COMPARE>::operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept
	{
		ref = new CMAP(l);
		return *this;
	}
	
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::Atomic(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : ref(new CMAP(l, capacityMinimum, capacityMaximum, hash, compare))
	{}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >& Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept
	{
		ref = new CMAP(l);
		return *this;
	}
	
}
