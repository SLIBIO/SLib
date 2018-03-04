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
	
	template <class KT, class VT, class KEY_COMPARE>
	CMap<KT, VT, KEY_COMPARE>::CMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare) noexcept
	 : m_compare(compare)
	{
		m_root = sl_null;
		m_count = 0;
		const Pair<KT, VT>* data = l.begin();
		for (sl_size i = 0; i < l.size(); i++) {
			RedBlackTree::add(&m_root, m_count, data[i].first, compare, data[i].second);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE>::Map(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare) noexcept
	 : ref(new CMAP(l, compare))
	{}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE> Map<KT, VT, KEY_COMPARE>::create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare) noexcept
	{
		return new CMAP(l, compare);
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE>& Map<KT, VT, KEY_COMPARE>::operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept
	{
		ref = new CMAP(l);
		return *this;
	}
	
	
	template <class KT, class VT, class KEY_COMPARE>
	Atomic< Map<KT, VT, KEY_COMPARE> >::Atomic(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare) noexcept
	 : ref(new CMAP(l, compare))
	{}
	
	template <class KT, class VT, class KEY_COMPARE>
	Atomic< Map<KT, VT, KEY_COMPARE> >& Atomic< Map<KT, VT, KEY_COMPARE> >::operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept
	{
		ref = new CMAP(l);
		return *this;
	}
	
}
