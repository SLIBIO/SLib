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

	template <class KT, class VT>
	template <class KEY, class... VALUE_ARGS>
	SLIB_INLINE HashMapNode<KT, VT>::HashMapNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept
	 : key(Forward<KEY>(_key)), value(Forward<VALUE_ARGS>(value_args)...), parent(sl_null), left(sl_null), right(sl_null), flagRed(sl_false)
	{}
	
	template <class KT, class VT>
	SLIB_INLINE HashMapNode<KT, VT>* HashMapNode<KT, VT>::getNext() const noexcept
	{
		return next;
	}
	
	template <class KT, class VT>
	SLIB_INLINE HashMapNode<KT, VT>* HashMapNode<KT, VT>::getPrevious() const noexcept
	{
		return previous;
	}

	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE CHashMap<KT, VT, HASH, KEY_COMPARE>::CHashMap(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : m_hash(hash), m_compare(compare)
	{
		_priv_HashTable::init(reinterpret_cast<HashTableStructBase*>(&m_table), capacityMinimum, capacityMaximum);
		m_nodeFirst = sl_null;
		m_nodeLast = sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE CHashMap<KT, VT, HASH, KEY_COMPARE>::~CHashMap() noexcept
	{
		_free();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE CHashMap<KT, VT, HASH, KEY_COMPARE>::CHashMap(CHashMap<KT, VT, HASH, KEY_COMPARE>&& other) noexcept
	 : m_hash(Move(other.m_hash)), m_compare(Move(other.m_compare))
	{
		_priv_HashTable::move(reinterpret_cast<HashTableStructBase*>(&m_table), reinterpret_cast<HashTableStructBase*>(&(other.m_table)));
		m_nodeFirst = other.m_nodeFirst;
		m_nodeLast = other.m_nodeLast;
		other.m_nodeFirst = sl_null;
		other.m_nodeLast = sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE CHashMap<KT, VT, HASH, KEY_COMPARE>& CHashMap<KT, VT, HASH, KEY_COMPARE>::operator=(CHashMap<KT, VT, HASH, KEY_COMPARE>&& other) noexcept
	{
		_free();
		_priv_HashTable::move(reinterpret_cast<HashTableStructBase*>(&m_table), reinterpret_cast<HashTableStructBase*>(&(other.m_table)));
		m_nodeFirst = other.m_nodeFirst;
		m_nodeLast = other.m_nodeLast;
		other.m_nodeFirst = sl_null;
		other.m_nodeLast = sl_null;
		m_hash = Move(other.m_hash);
		m_compare = Move(other.m_compare);
		return *this;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::getCount() const noexcept
	{
		return m_table.count;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::isEmpty() const noexcept
	{
		return m_table.count == 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::isNotEmpty() const noexcept
	{
		return m_table.count != 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::getCapacity() const noexcept
	{
		return m_table.capacity;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::getMinimumCapacity() const noexcept
	{
		return m_table.capacityMinimum;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void CHashMap<KT, VT, HASH, KEY_COMPARE>::setMinimumCapacity_NoLock(sl_size capacity) noexcept
	{
		_priv_HashTable::setMinimumCapacity(reinterpret_cast<HashTableStructBase*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::setMinimumCapacity(sl_size capacity) noexcept
	{
		ObjectLocker lock(this);
		setMinimumCapacity_NoLock(capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::getMaximumCapacity() const noexcept
	{
		return m_table.capacityMaximum;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void CHashMap<KT, VT, HASH, KEY_COMPARE>::setMaximumCapacity_NoLock(sl_size capacity) noexcept
	{
		_priv_HashTable::setMaximumCapacity(reinterpret_cast<HashTableStructBase*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::setMaximumCapacity(sl_size capacity) noexcept
	{
		ObjectLocker lock(this);
		setMaximumCapacity_NoLock(capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::getFirstNode() const noexcept
	{
		return m_nodeFirst;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::getLastNode() const noexcept
	{
		return m_nodeLast;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::find_NoLock(const KT& key) const noexcept
	{
		NODE* entry = _getEntry(key);
		return RedBlackTree::find(entry, key, m_compare);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::find(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		return RedBlackTree::find(entry, key, m_compare) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::getEqualRange(const KT& key, MapNode<KT, VT>** pStart, MapNode<KT, VT>** pEnd) const noexcept
	{
		NODE* entry = _getEntry(key);
		return RedBlackTree::getEqualRange(entry, key, m_compare, (NODE**)pStart, (NODE**)pEnd);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* entry = _getEntry(key);
		return RedBlackTree::findKeyAndValue(entry, key, m_compare, value, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		return RedBlackTree::findKeyAndValue(entry, key, m_compare, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT* CHashMap<KT, VT, HASH, KEY_COMPARE>::getItemPointer(const KT& key) const noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE VT* CHashMap<KT, VT, HASH, KEY_COMPARE>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::findKeyAndValue(entry, key, m_compare, value, value_equals);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			if (_out) {
				*_out = node->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::get(const KT& key, VT* _out) const noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			if (_out) {
				*_out = node->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::get_NoLock(const KT& key, Nullable<VT>* _out) const noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			if (_out) {
				*_out = node->value;
			}
			return sl_true;
		} else {
			if (_out) {
				_out->setNull();
			}
			return sl_false;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::get(const KT& key, Nullable<VT>* _out) const noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			if (_out) {
				*_out = node->value;
			}
			return sl_true;
		} else {
			if (_out) {
				_out->setNull();
			}
			return sl_false;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT CHashMap<KT, VT, HASH, KEY_COMPARE>::getValue_NoLock(const KT& key) const noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	VT CHashMap<KT, VT, HASH, KEY_COMPARE>::getValue(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT CHashMap<KT, VT, HASH, KEY_COMPARE>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	VT CHashMap<KT, VT, HASH, KEY_COMPARE>::getValue(const KT& key, const VT& def) const noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getValues_NoLock(const KT& key) const noexcept
	{
		List<VT> list;
		NODE* entry = _getEntry(key);
		RedBlackTree::getValues(list, entry, key, m_compare);
		return list;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getValues(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		List<VT> list;
		NODE* entry = _getEntry(key);
		RedBlackTree::getValues(list, entry, key, m_compare);
		return list;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getValuesByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		List<VT> list;
		NODE* entry = _getEntry(key);
		RedBlackTree::getValuesByKeyAndValue(list, entry, key, m_compare, value, value_equals);
		return list;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		List<VT> list;
		NODE* entry = _getEntry(key);
		RedBlackTree::getValuesByKeyAndValue(list, entry, key, m_compare, value, value_equals);
		return list;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::put_NoLock(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableStructBase*>(&m_table)))) {
			return sl_null;
		}
		sl_size capacity = m_table.capacity;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE** nodes = m_table.nodes;
		sl_bool flagInsert;
		NODE* node = RedBlackTree::put(&(nodes[index]), m_table.count, Forward<KEY>(key), m_compare, Forward<VALUE>(value), &flagInsert);
		if (isInsertion) {
			*isInsertion = flagInsert;
		}
		if (flagInsert) {
			_linkNode(node, hash);
			_expand();
		}
		return node;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		ObjectLocker lock(this);
		return put_NoLock(Forward<KEY>(key), Forward<VALUE>(value), isInsertion) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::replace_NoLock(const KEY& key, VALUE&& value) noexcept
	{
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			node->value = Forward<VALUE>(value);
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::replace(const KEY& key, VALUE&& value) noexcept
	{
		ObjectLocker lock(this);
		NODE* entry = _getEntry(key);
		NODE* node = RedBlackTree::find(entry, key, m_compare);
		if (node) {
			node->value = Forward<VALUE>(value);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::add_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableStructBase*>(&m_table)))) {
			return sl_null;
		}
		sl_size capacity = m_table.capacity;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE** nodes = m_table.nodes;
		NODE* node = RedBlackTree::add(&(nodes[index]), m_table.count, Forward<KEY>(key), m_compare, Forward<VALUE_ARGS>(value_args)...);
		if (node) {
			_linkNode(node, hash);
			_expand();
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		ObjectLocker lock(this);
		return add_NoLock(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	MapEmplaceReturn< HashMapNode<KT, VT> > CHashMap<KT, VT, HASH, KEY_COMPARE>::emplace_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableStructBase*>(&m_table)))) {
			return sl_null;
		}
		sl_size capacity = m_table.capacity;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE** nodes = m_table.nodes;
		MapEmplaceReturn<NODE> ret = RedBlackTree::emplace(&(nodes[index]), m_table.count, Forward<KEY>(key), m_compare, Forward<VALUE_ARGS>(value_args)...);
		if (ret.isSuccess) {
			_linkNode(ret.node, hash);
			_expand();
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		ObjectLocker lock(this);
		return emplace_NoLock(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::putAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return sl_true;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return sl_true;
		}
		while (node) {
			if (!(put_NoLock(node->key, node->value))) {
				return sl_false;
			}
			node = node->getNext();
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::putAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return sl_true;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return sl_true;
		}
		MultipleMutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			if (!(put_NoLock(node->key, node->value))) {
				return sl_false;
			}
			node = node->getNext();
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::replaceAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return;
		}
		while (node) {
			replace_NoLock(node->key, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::replaceAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return;
		}
		MultipleMutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			replace_NoLock(node->key, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::addAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return sl_true;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return sl_false;
		}
		while (node) {
			if (!(add_NoLock(node->key, node->value))) {
				return sl_false;
			}
			node = node->getNext();
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::addAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return sl_true;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return sl_false;
		}
		MultipleMutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			if (!(add_NoLock(node->key, node->value))) {
				return sl_false;
			}
			node = node->getNext();
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::emplaceAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return sl_true;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return sl_true;
		}
		while (node) {
			MapEmplaceReturn<NODE> ret = emplace_NoLock(node->key, node->value);
			if (!(ret.node)) {
				return sl_false;
			}
			node = node->getNext();
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::emplaceAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return sl_true;
		}
		if (reinterpret_cast<void*>(m_nodeFirst) == reinterpret_cast<void*>(node)) {
			return sl_true;
		}
		MultipleMutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			MapEmplaceReturn<NODE> ret = emplace_NoLock(node->key, node->value);
			if (!(ret.node)) {
				return sl_false;
			}
			node = node->getNext();
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::removeAt(HashMapNode<KT, VT>* node) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return;
		}
		_unlinkNode(node);
		RedBlackTree::removeNode(m_table.nodes + (node->hash & (capacity - 1)), m_table.count, node);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeAt(HashMapNode<KT, VT>* node, sl_size count) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		NODE** nodes = m_table.nodes;
		sl_size n = 0;
		while (n < count && node) {
			NODE* next = node->next;
			_unlinkNode(node);
			RedBlackTree::removeNode(nodes + (node->hash & (capacity - 1)), m_table.count, node);
			node = next;
			n++;
		}
		return n;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeRange(HashMapNode<KT, VT>* first, HashMapNode<KT, VT>* last) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		NODE* node = first;
		if (!node) {
			node = m_nodeFirst;
			if (!node) {
				return 0;
			}
		}
		NODE** nodes = m_table.nodes;
		sl_size n = 0;
		while (1) {
			n++;
			NODE* next = node->next;
			_unlinkNode(node);
			RedBlackTree::removeNode(nodes + (node->hash & (capacity - 1)), m_table.count, node);
			if (node == last || !next) {
				break;
			}
			node = next;
		}
		return n;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::remove_NoLock(const KT& key, VT* outValue) noexcept
	{
		NODE** pEntry = _getEntryPtr(key);
		if (!pEntry) {
			return sl_false;
		}
		NODE* node = RedBlackTree::find(*pEntry, key, m_compare);
		if (node) {
			if (outValue) {
				*outValue = Move(node->value);
			}
			_unlinkNode(node);
			RedBlackTree::removeNode(pEntry, m_table.count, node);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::remove(const KT& key, VT* outValue) noexcept
	{
		ObjectLocker lock(this);
		return remove_NoLock(key, outValue);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeItems_NoLock(const KT& key) noexcept
	{
		NODE** pEntry = _getEntryPtr(key);
		if (!pEntry) {
			return 0;
		}
		NODE* node;
		NODE* end;
		if (RedBlackTree::getEqualRange(*pEntry, key, m_compare, &node, &end)) {
			sl_size n = 0;
			while (1) {
				n++;
				if (node == end) {
					_unlinkNode(node);
					RedBlackTree::removeNode(pEntry, m_table.count, node);
					break;
				} else {
					NODE* next = reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(reinterpret_cast<RedBlackTreeNode*>(node)));
					_unlinkNode(node);
					RedBlackTree::removeNode(pEntry, m_table.count, node);
					node = next;
				}
			}
			return n;
		} else {
			return 0;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeItems(const KT& key) noexcept
	{
		ObjectLocker lock(this);
		return removeItems_NoLock(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsAndReturnValues_NoLock(const KT& key) noexcept
	{
		List<VT> ret;
		NODE** pEntry = _getEntryPtr(key);
		if (!pEntry) {
			return sl_null;
		}
		NODE* node;
		NODE* end;
		if (RedBlackTree::getEqualRange(*pEntry, key, m_compare, &node, &end)) {
			while (1) {
				ret.add_NoLock(node->value);
				if (node == end) {
					_unlinkNode(node);
					RedBlackTree::removeNode(pEntry, m_table.count, node);
					break;
				} else {
					NODE* next = reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(reinterpret_cast<RedBlackTreeNode*>(node)));
					_unlinkNode(node);
					RedBlackTree::removeNode(pEntry, m_table.count, node);
					node = next;
				}
			}
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsAndReturnValues(const KT& key) noexcept
	{
		ObjectLocker lock(this);
		return removeItemsAndReturnValues_NoLock(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		NODE** pEntry = _getEntryPtr(key);
		if (!pEntry) {
			return sl_false;
		}
		NODE* node;
		NODE* end;
		if (RedBlackTree::getEqualRange(*pEntry, key, m_compare, &node, &end)) {
			while (1) {
				if (value_equals(node->value, value)) {
					_unlinkNode(node);
					RedBlackTree::removeNode(pEntry, m_table.count, node);
					return sl_true;
				}
				if (node == end) {
					break;
				}
				node = reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(reinterpret_cast<RedBlackTreeNode*>(node)));
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return removeKeyAndValue_NoLock(key, value, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		NODE** pEntry = _getEntryPtr(key);
		if (!pEntry) {
			return 0;
		}
		NODE* node;
		NODE* end;
		if (RedBlackTree::getEqualRange(*pEntry, key, m_compare, &node, &end)) {
			sl_size n = 0;
			while (1) {
				if (value_equals(node->value, value)) {
					n++;
					if (node == end) {
						_unlinkNode(node);
						RedBlackTree::removeNode(pEntry, m_table.count, node);
						break;
					} else {
						NODE* next = reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(reinterpret_cast<RedBlackTreeNode*>(node)));
						_unlinkNode(node);
						RedBlackTree::removeNode(pEntry, m_table.count, node);
						node = next;
					}
				} else {
					if (node == end) {
						break;
					}
					node = reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(reinterpret_cast<RedBlackTreeNode*>(node)));
				}
			}
			return n;
		} else {
			return 0;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return removeItemsByKeyAndValue_NoLock(key, value, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeAll_NoLock() noexcept
	{
		if (m_table.capacity == 0) {
			return 0;
		}
		sl_size count = m_table.count;
		_free();
		_priv_HashTable::init(reinterpret_cast<HashTableStructBase*>(&m_table), m_table.capacityMinimum, m_table.capacityMaximum);
		m_nodeFirst = sl_null;
		m_nodeLast = sl_null;
		return count;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size CHashMap<KT, VT, HASH, KEY_COMPARE>::removeAll() noexcept
	{
		NODE* first;
		NODE** nodes;
		sl_size count;
		{
			ObjectLocker lock(this);
			if (m_table.capacity == 0) {
				return 0;
			}
			first = m_nodeFirst;
			nodes = m_table.nodes;
			count = m_table.count;
			_priv_HashTable::init(reinterpret_cast<HashTableStructBase*>(&m_table), m_table.capacityMinimum, m_table.capacityMaximum);
			m_nodeFirst = sl_null;
			m_nodeLast = sl_null;
		}
		NODE* node = first;
		while (node) {
			NODE* next = node->next;
			delete node;
			node = next;
		}
		if (nodes) {
			Base::freeMemory(nodes);
		}
		return count;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::shrink_NoLock() noexcept
	{
		TABLE* table = &m_table;
		sl_size n = Math::roundUpToPowerOfTwo(table->count);
		if (n < table->capacityMinimum) {
			n = table->capacityMinimum;
		}
		if (n < table->capacity) {
			if (_priv_HashTable::reallocNodes(reinterpret_cast<HashTableStructBase*>(table), n)) {
				_rebuildTree(n);
			}
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::shrink() noexcept
	{
		ObjectLocker lock(this);
		shrink_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::copyFrom_NoLock(const CHashMap<KT, VT, HASH, KEY_COMPARE>& other) noexcept
	{
		if (this == &other) {
			return sl_true;
		}
		m_hash = other.m_hash;
		m_compare = other.m_compare;
		_free();
		m_nodeFirst = sl_null;
		m_nodeLast = sl_null;
		_priv_HashTable::init(reinterpret_cast<HashTableStructBase*>(&m_table), other.m_table.capacityMinimum, other.m_table.capacityMaximum);
        return _copyFrom(&other);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::copyFrom(const CHashMap<KT, VT, HASH, KEY_COMPARE>& other) noexcept
	{
		ObjectLocker lock(this);
		return copyFrom_NoLock(other);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	CHashMap<KT, VT, HASH, KEY_COMPARE>* CHashMap<KT, VT, HASH, KEY_COMPARE>::duplicate_NoLock() const noexcept
	{
		CHashMap* map = new CHashMap(m_table.capacityMinimum, m_table.capacityMaximum, m_hash, m_compare);
		if (map) {
			if (map->_copyFrom(this)) {
				return map;
			}
			delete map;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	CHashMap<KT, VT, HASH, KEY_COMPARE>* CHashMap<KT, VT, HASH, KEY_COMPARE>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<KT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getAllKeys_NoLock() const noexcept
	{
		List<KT> ret;
		NODE* node = m_nodeFirst;
		while (node) {
			ret.add_NoLock(node->key);
			node = node->next;
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<KT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getAllKeys() const noexcept
	{
		ObjectLocker lock(this);
		return getAllKeys_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getAllValues_NoLock() const noexcept
	{
		List<VT> ret;
		NODE* node = m_nodeFirst;
		while (node) {
			ret.add_NoLock(node->value);
			node = node->next;
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> CHashMap<KT, VT, HASH, KEY_COMPARE>::getAllValues() const noexcept
	{
		ObjectLocker lock(this);
		return getAllValues_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List< Pair<KT, VT> > CHashMap<KT, VT, HASH, KEY_COMPARE>::toList_NoLock() const noexcept
	{
		List< Pair<KT, VT> > ret;
		NODE* node = m_nodeFirst;
		while (node) {
			ret.add_NoLock(Pair<KT, VT>(node->key, node->value));
			node = node->next;
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List< Pair<KT, VT> > CHashMap<KT, VT, HASH, KEY_COMPARE>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return toList_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE typename CHashMap<KT, VT, HASH, KEY_COMPARE>::POSITION CHashMap<KT, VT, HASH, KEY_COMPARE>::begin() const noexcept
	{
		return m_nodeFirst;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE typename CHashMap<KT, VT, HASH, KEY_COMPARE>::POSITION CHashMap<KT, VT, HASH, KEY_COMPARE>::end() const noexcept
	{
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE CHashMap<KT, VT, HASH, KEY_COMPARE>::EnumLockHelper::EnumLockHelper(const CHashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept
	{
		node = map.m_nodeFirst;
		mutex = map.getLocker();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE CHashMap<KT, VT, HASH, KEY_COMPARE>::EnumHelper::EnumHelper(const CHashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept
	{
		node = map.m_nodeFirst;
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::_free() noexcept
	{
		NODE* node = m_nodeFirst;
		while (node) {
			NODE* next = node->next;
			delete node;
			node = next;
		}
		NODE** nodes = m_table.nodes;
		if (nodes) {
			Base::freeMemory(nodes);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	HashMapNode<KT, VT>* CHashMap<KT, VT, HASH, KEY_COMPARE>::_getEntry(const KT &key) const noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		return m_table.nodes[index];
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	HashMapNode<KT, VT>** CHashMap<KT, VT, HASH, KEY_COMPARE>::_getEntryPtr(const KT &key) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		return m_table.nodes + index;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::_linkNode(HashMapNode<KT, VT>* node, sl_size hash) noexcept
	{
		NODE* last = m_nodeLast;
		node->hash = hash;
		node->next = sl_null;
		node->previous = last;
		if (last) {
			last->next = node;
		} else {
			m_nodeFirst = node;
		}
		m_nodeLast = node;
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::_unlinkNode(HashMapNode<KT, VT>* node) noexcept
	{
		NODE* previous = node->previous;
		NODE* next = node->next;
		if (previous) {
			previous->next = next;
		} else {
			m_nodeFirst = next;
		}
		if (next) {
			next->previous = previous;
		} else {
			m_nodeLast = previous;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::_expand() noexcept
	{
		TABLE* table = &m_table;
		if (table->capacity < table->capacityMaximum && table->count >= table->thresholdUp) {
			// double capacity
			sl_size n = table->capacity;
			n *= 2;
			if (_priv_HashTable::reallocNodes(reinterpret_cast<HashTableStructBase*>(table), n)) {
				_rebuildTree(n);
			}
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void CHashMap<KT, VT, HASH, KEY_COMPARE>::_rebuildTree(sl_size capacity) noexcept
	{
		TABLE* table = &m_table;
		NODE** nodes = table->nodes;
		Base::zeroMemory(nodes, capacity * sizeof(NODE*));
		NODE* node = m_nodeFirst;
		while (node) {
			node->flagRed = sl_false;
			node->parent = sl_null;
			node->left = sl_null;
			node->right = sl_null;
			sl_size index = node->hash & (capacity - 1);
			RedBlackTree::addNode(nodes + index, node, m_compare);
			node = node->next;
		}
	}
    
    template <class KT, class VT, class HASH, class KEY_COMPARE>
    sl_bool CHashMap<KT, VT, HASH, KEY_COMPARE>::_copyFrom(const CHashMap<KT, VT, HASH, KEY_COMPARE>* other) noexcept
    {
        sl_size capacity = other->m_table.capacity;
        if (capacity == 0) {
            return sl_true;
        }
        if (!(_priv_HashTable::reallocNodes(reinterpret_cast<HashTableStructBase*>(&m_table), capacity))) {
            return sl_false;
        }
        NODE** nodes = m_table.nodes;
        Base::zeroMemory(nodes, capacity * sizeof(NODE*));
        NODE* nodeSrc = other->m_nodeFirst;
        while (nodeSrc) {
            sl_size index = nodeSrc->hash & (capacity - 1);
            NODE* node = RedBlackTree::add(&(nodes[index]), m_table.count, nodeSrc->key, m_compare, nodeSrc->value);
            if (node) {
                _linkNode(node, nodeSrc->hash);
            } else {
                removeAll_NoLock();
                return sl_false;
            }
            nodeSrc = nodeSrc->next;
        }
        return sl_true;
    }
	
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE>::HashMap(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : ref(new CMAP(capacityMinimum, capacityMaximum, hash, compare))
	{}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE> HashMap<KT, VT, HASH, KEY_COMPARE>::create(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	{
		return new CMAP(capacityMinimum, capacityMaximum, hash, compare);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE, class OTHER_HASH, class OTHER_COMPARE>
	SLIB_INLINE const HashMap<KT, VT, HASH, KEY_COMPARE>& HashMap<KT, VT, HASH, KEY_COMPARE>::from(const HashMap<KEY, VALUE, OTHER_HASH, OTHER_COMPARE>& other) noexcept
	{
		return *(reinterpret_cast<HashMap const*>(&other));
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::init(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	{
		ref = new CMAP(capacityMinimum, capacityMaximum, hash, compare);
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT HashMap<KT, VT, HASH, KEY_COMPARE>::operator[](const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::getCount() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::isEmpty() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::isNotEmpty() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::getCapacity() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getCapacity();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::getMinimumCapacity() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getMinimumCapacity();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::setMinimumCapacity_NoLock(sl_size capacity) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->setMinimumCapacity_NoLock(capacity);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::setMinimumCapacity(sl_size capacity) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->setMinimumCapacity(capacity);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::getMaximumCapacity() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getMaximumCapacity();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::setMaximumCapacity_NoLock(sl_size capacity) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->setMaximumCapacity_NoLock(capacity);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::setMaximumCapacity(sl_size capacity) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->setMaximumCapacity(capacity);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::getFirstNode() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getFirstNode();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::getLastNode() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getLastNode();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::find_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->find_NoLock(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::find(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->find(key);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::getEqualRange(const KT& key, MapNode<KT, VT>** pStart, MapNode<KT, VT>** pEnd) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getEqualRange(key, pStart, pEnd);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->findKeyAndValue_NoLock(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->findKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT* HashMap<KT, VT, HASH, KEY_COMPARE>::getItemPointer(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getItemPointer(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE VT* HashMap<KT, VT, HASH, KEY_COMPARE>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getItemPointerByKeyAndValue(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->get_NoLock(key, _out);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::get(const KT& key, VT* _out) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->get(key, _out);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::get_NoLock(const KT& key, Nullable<VT>* _out) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->get_NoLock(key, _out);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::get(const KT& key, Nullable<VT>* _out) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT HashMap<KT, VT, HASH, KEY_COMPARE>::getValue_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT HashMap<KT, VT, HASH, KEY_COMPARE>::getValue(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT HashMap<KT, VT, HASH, KEY_COMPARE>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key, def);
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE VT HashMap<KT, VT, HASH, KEY_COMPARE>::getValue(const KT& key, const VT& def) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key, def);
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::getValues_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValues_NoLock(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::getValues(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::getValuesByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValuesByKeyAndValue_NoLock(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValuesByKeyAndValue(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::put_NoLock(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->put_NoLock(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				if (isInsertion) {
					*isInsertion = sl_true;
				}
				return obj->add_NoLock(Forward<KEY>(key), Forward<VALUE>(value));
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->put(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->put(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->put(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::replace_NoLock(const KEY& key, VALUE&& value) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->replace_NoLock(key, Forward<VALUE>(value));
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::replace(const KEY& key, VALUE&& value) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->replace(key, Forward<VALUE>(value));
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	HashMapNode<KT, VT>* HashMap<KT, VT, HASH, KEY_COMPARE>::add_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->add_NoLock(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				return obj->add_NoLock(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->add(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->add(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->add(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	MapEmplaceReturn< HashMapNode<KT, VT> > HashMap<KT, VT, HASH, KEY_COMPARE>::emplace_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->emplace_NoLock(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				NODE* node = obj->add_NoLock(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
				if (node) {
					return MapEmplaceReturn<NODE>(sl_true, node);
				} else {
					return sl_null;
				}
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->emplace(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->emplace(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->emplace(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::putAll_NoLock(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->putAll_NoLock(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				return obj->putAll_NoLock(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::putAll(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->putAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->putAll(other);
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->putAll(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::replaceAll_NoLock(const MAP& other) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->replaceAll_NoLock(other);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::replaceAll(const MAP& other) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->replaceAll(other);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::addAll_NoLock(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->addAll_NoLock(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				return obj->addAll_NoLock(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::addAll(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->addAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->addAll(other);
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->addAll(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::emplaceAll_NoLock(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->emplaceAll_NoLock(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				return obj->emplaceAll_NoLock(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::emplaceAll(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->emplaceAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->emplaceAll(other);
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->emplaceAll(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::removeAt(HashMapNode<KT, VT>* node) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->removeAt(node);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeAt(HashMapNode<KT, VT>* node, sl_size count) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeAt(node, count);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeRange(HashMapNode<KT, VT>* first, HashMapNode<KT, VT>* last) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeRange(first, last);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::remove_NoLock(const KT& key, VT* outValue) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->remove_NoLock(key, outValue);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::remove(const KT& key, VT* outValue) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeItems_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItems_NoLock(key);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeItems(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItems(key);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsAndReturnValues_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsAndReturnValues_NoLock(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsAndReturnValues(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsAndReturnValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeKeyAndValue_NoLock(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool HashMap<KT, VT, HASH, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsByKeyAndValue_NoLock(key, value, value_equals);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsByKeyAndValue(key, value, value_equals);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeAll_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE sl_size HashMap<KT, VT, HASH, KEY_COMPARE>::removeAll() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::shrink_NoLock() noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->shrink_NoLock();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE void HashMap<KT, VT, HASH, KEY_COMPARE>::shrink() noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->shrink();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE> HashMap<KT, VT, HASH, KEY_COMPARE>::duplicate_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE> HashMap<KT, VT, HASH, KEY_COMPARE>::duplicate() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<KT> HashMap<KT, VT, HASH, KEY_COMPARE>::getAllKeys_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys_NoLock();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<KT> HashMap<KT, VT, HASH, KEY_COMPARE>::getAllKeys() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::getAllValues_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues_NoLock();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List<VT> HashMap<KT, VT, HASH, KEY_COMPARE>::getAllValues() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_COMPARE>::toList_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->toList_NoLock();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE List< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_COMPARE>::toList() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->toList();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE NodePosition< HashMapNode<KT, VT> > HashMap<KT, VT, HASH, KEY_COMPARE>::begin() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getFirstNode();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE NodePosition< HashMapNode<KT, VT> > HashMap<KT, VT, HASH, KEY_COMPARE>::end() const noexcept
	{
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE const Mutex* HashMap<KT, VT, HASH, KEY_COMPARE>::getLocker() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getLocker();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE>::EnumLockHelper::EnumLockHelper(const HashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept
	{
		CMAP* obj = map.ref._ptr;
		if (obj) {
			node = obj->getFirstNode();
			mutex = obj->getLocker();
		} else {
			node = sl_null;
			mutex = sl_null;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_COMPARE>::EnumHelper::EnumHelper(const HashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept
	{
		node = map.getFirstNode();
	}
	
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::Atomic(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	 : ref(new CMAP(capacityMinimum, capacityMaximum, hash, compare))
	{}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE, class OTHER_HASH, class OTHER_COMPARE>
	const Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >& Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::from(const Atomic< HashMap<KEY, VALUE, OTHER_HASH, OTHER_COMPARE> >& other) noexcept
	{
		return *(reinterpret_cast<Atomic const*>(&other));
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::init(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_COMPARE& compare) noexcept
	{
		ref = new CMAP(capacityMinimum, capacityMaximum, hash, compare);
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	VT Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::operator[](const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getCount() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::isEmpty() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::isNotEmpty() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getCapacity() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getCapacity();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getMinimumCapacity() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getMinimumCapacity();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::setMinimumCapacity(sl_size capacity) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->setMinimumCapacity(capacity);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getMaximumCapacity() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getMaximumCapacity();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::setMaximumCapacity(sl_size capacity) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->setMaximumCapacity(capacity);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::find(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->find(key);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->findKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::get(const KT& key, VT* _out) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->get(key, _out);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::get(const KT& key, Nullable<VT>* _out) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	VT Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getValue(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	VT Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getValue(const KT& key, const VT& def) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key, def);
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getValues(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValuesByKeyAndValue(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->put(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->put(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->put(Forward<KEY>(key), Forward<VALUE>(value), isInsertion);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::replace(const KEY& key, VALUE&& value) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->replace(key, Forward<VALUE>(value));
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->add(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->add(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->add(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->emplace(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->emplace(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->emplace(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::putAll(const MAP& other) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->putAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->putAll(other);
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->putAll(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	void Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::replaceAll(const MAP& other) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->replaceAll(other);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::addAll(const MAP& other) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->addAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addAll(other);
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addAll(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class MAP>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::emplaceAll(const MAP& other) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->emplaceAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->emplaceAll(other);
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->emplaceAll(other);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::remove(const KT& key, VT* outValue) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::removeItems(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItems(key);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::removeItemsAndReturnValues(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItemsAndReturnValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItemsByKeyAndValue(key, value, value_equals);
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	sl_size Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::removeAll() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::shrink() noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->shrink();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	HashMap<KT, VT, HASH, KEY_COMPARE> Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::duplicate() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<KT> Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getAllKeys() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllKeys();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List<VT> Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::getAllValues() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllValues();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	List< Pair<KT, VT> > Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::toList() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->toList();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	NodePositionWithRef< HashMapNode<KT, VT> > Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::begin() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return POSITION(obj->getFirstNode(), obj._ptr);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	NodePositionWithRef< HashMapNode<KT, VT> > Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::end() const noexcept
	{
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::EnumLockHelper::EnumLockHelper(const HashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept
	{
		CMAP* obj = map.ref._ptr;
		if (obj) {
			node = obj->getFirstNode();
			mutex = obj->getLocker();
			ref = obj;
		} else {
			node = sl_null;
			mutex = sl_null;
		}
	}

	template <class KT, class VT, class HASH, class KEY_COMPARE>
	Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >::EnumHelper::EnumHelper(const HashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept
	 : node(map.getFirstNode()), ref(map.ref)
	{}

}
