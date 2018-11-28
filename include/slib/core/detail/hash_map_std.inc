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
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	CHashMap<KT, VT, HASH, KEY_COMPARE>::CHashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : m_hash(hash), m_compare(compare)
	{
		_priv_HashTable::init(reinterpret_cast<HashTableStructBase*>(&m_table), capacityMinimum, capacityMaximum);
		m_nodeFirst = sl_null;
		m_nodeLast = sl_null;
		const Pair<KT, VT>* data = l.begin();
		for (sl_size i = 0; i < l.size(); i++) {
			add_NoLock(data[i].first, data[i].second);
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
