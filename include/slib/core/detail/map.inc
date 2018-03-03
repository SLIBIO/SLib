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
	
	template <class KT, class VT>
	template <class KEY, class... VALUE_ARGS>
	SLIB_INLINE MapNode<KT, VT>::MapNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept
	 : key(Forward<KEY>(_key)), value(Forward<VALUE_ARGS>(value_args)...),
       parent(sl_null), left(sl_null), right(sl_null), flagRed(sl_false)
	{}

	template <class KT, class VT>
	SLIB_INLINE MapNode<KT, VT>* MapNode<KT, VT>::getNext() const noexcept
	{
		return reinterpret_cast<MapNode<KT, VT>*>(_priv_RedBlackTree::getNext(const_cast<RedBlackTreeNode*>(reinterpret_cast<RedBlackTreeNode const*>(this))));
	}
	
	template <class KT, class VT>
	SLIB_INLINE MapNode<KT, VT>* MapNode<KT, VT>::getPrevious() const noexcept
	{
		return reinterpret_cast<MapNode<KT, VT>*>(_priv_RedBlackTree::getPrevious(const_cast<RedBlackTreeNode*>(reinterpret_cast<RedBlackTreeNode const*>(this))));
	}
	
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>::CMap() noexcept
	 : m_root(sl_null), m_count(0)
	{}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>::CMap(const KEY_COMPARE& compare) noexcept
	 : m_root(sl_null), m_count(0), m_compare(compare)
	{}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>::~CMap() noexcept
	{
		NODE* root = m_root;
		if (root) {
			RedBlackTree::freeNodes(root);
			m_root = sl_null;
			m_count = 0;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>::CMap(CMap<KT, VT, KEY_COMPARE>&& other) noexcept
	 : m_root(other.m_root), m_count(other.m_count), m_compare(Move(other.m_compare))
	{
		other.m_root = sl_null;
		other.m_count = 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>& CMap<KT, VT, KEY_COMPARE>::operator=(CMap<KT, VT, KEY_COMPARE>&& other) noexcept
	{
		NODE* root = m_root;
		if (root) {
			RedBlackTree::freeNodes(root);
		}
		m_root = other.m_root;
		m_count = other.m_count;
		m_compare = Move(other.m_compare);
		other.m_root = sl_null;
		other.m_count = 0;
		return *this;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size CMap<KT, VT, KEY_COMPARE>::getCount() const noexcept
	{
		return m_count;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool CMap<KT, VT, KEY_COMPARE>::isEmpty() const noexcept
	{
		return m_count == 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool CMap<KT, VT, KEY_COMPARE>::isNotEmpty() const noexcept
	{
		return m_count != 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::getFirstNode() const noexcept
	{
		return RedBlackTree::getFirstNode(m_root);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::getLastNode() const noexcept
	{
		return RedBlackTree::getLastNode(m_root);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::find_NoLock(const KT& key) const noexcept
	{
		return RedBlackTree::find(m_root, key, m_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::find(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::find(m_root, key, m_compare) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool CMap<KT, VT, KEY_COMPARE>::getEqualRange(const KT& key, MapNode<KT, VT>** pStart, MapNode<KT, VT>** pEnd) const noexcept
	{
		return RedBlackTree::getEqualRange(m_root, key, m_compare, pStart, pEnd);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void CMap<KT, VT, KEY_COMPARE>::getNearest(const KT& key, MapNode<KT, VT>** pLessEqual, MapNode<KT, VT>** pGreaterEqual) const noexcept
	{
		RedBlackTree::getNearest(m_root, key, m_compare, pLessEqual, pGreaterEqual);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::getLowerBound(const KT& key) const noexcept
	{
		return RedBlackTree::getLowerBound(m_root, key, m_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::getUpperBound(const KT& key) const noexcept
	{
		return RedBlackTree::getUpperBound(m_root, key, m_compare);
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		return RedBlackTree::findKeyAndValue(m_root, key, m_compare, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool CMap<KT, VT, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::findKeyAndValue(m_root, key, m_compare, value, value_equals) != sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT* CMap<KT, VT, KEY_COMPARE>::getItemPointer(const KT& key) const noexcept
	{
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE VT* CMap<KT, VT, KEY_COMPARE>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* node = RedBlackTree::findKeyAndValue(m_root, key, m_compare, value, value_equals);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool CMap<KT, VT, KEY_COMPARE>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			if (_out) {
				*_out = node->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::get(const KT& key, VT* _out) const noexcept
	{
		ObjectLocker lock(this);
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			if (_out) {
				*_out = node->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT CMap<KT, VT, KEY_COMPARE>::getValue_NoLock(const KT& key) const noexcept
	{
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT CMap<KT, VT, KEY_COMPARE>::getValue(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT CMap<KT, VT, KEY_COMPARE>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT CMap<KT, VT, KEY_COMPARE>::getValue(const KT& key, const VT& def) const noexcept
	{
		ObjectLocker lock(this);
		NODE* node = RedBlackTree::find(m_root, key, m_compare);
		if (node) {
			return node->value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> CMap<KT, VT, KEY_COMPARE>::getValues_NoLock(const KT& key) const noexcept
	{
		List<VT> list;
		RedBlackTree::getValues(list, m_root, key, m_compare);
		return list;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<VT> CMap<KT, VT, KEY_COMPARE>::getValues(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		List<VT> list;
		RedBlackTree::getValues(list, m_root, key, m_compare);
		return list;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE List<VT> CMap<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		List<VT> list;
		RedBlackTree::getValuesByKeyAndValue(list, m_root, key, m_compare, value, value_equals);
		return list;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> CMap<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		List<VT> list;
		RedBlackTree::getValuesByKeyAndValue(list, m_root, key, m_compare, value, value_equals);
		return list;
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::put_NoLock(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		return RedBlackTree::put(&m_root, m_count, Forward<KEY>(key), m_compare, Forward<VALUE>(value), isInsertion);
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::put(&m_root, m_count, Forward<KEY>(key), m_compare, Forward<VALUE>(value), isInsertion) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::replace_NoLock(const KEY& key, VALUE&& value) noexcept
	{
		return RedBlackTree::replace(m_root, key, m_compare, Forward<VALUE>(value));
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::replace(const KEY& key, VALUE&& value) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::replace(m_root, key, m_compare, Forward<VALUE>(value)) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	SLIB_INLINE MapNode<KT, VT>* CMap<KT, VT, KEY_COMPARE>::add_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		return RedBlackTree::add(&m_root, m_count, Forward<KEY>(key), m_compare, Forward<VALUE_ARGS>(value_args)...);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool CMap<KT, VT, KEY_COMPARE>::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::add(&m_root, m_count, Forward<KEY>(key), m_compare, Forward<VALUE_ARGS>(value_args)...) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	SLIB_INLINE MapEmplaceReturn< MapNode<KT, VT> > CMap<KT, VT, KEY_COMPARE>::emplace_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		return RedBlackTree::emplace(&m_root, m_count, Forward<KEY>(key), m_compare, Forward<VALUE_ARGS>(value_args)...);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool CMap<KT, VT, KEY_COMPARE>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::emplace(&m_root, m_count, Forward<KEY>(key), m_compare, Forward<VALUE_ARGS>(value_args)...);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::putAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		while (node) {
			RedBlackTree::put(&m_root, m_count, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::putAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		MutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			RedBlackTree::put(&m_root, m_count, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::replaceAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		while (node) {
			RedBlackTree::replace(m_root, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::replaceAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		MutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			RedBlackTree::replace(m_root, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::addAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		while (node) {
			RedBlackTree::add(&m_root, m_count, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::addAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		MutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			RedBlackTree::add(&m_root, m_count, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::emplaceAll_NoLock(const MAP& other) noexcept
	{
		typename MAP::EnumHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		while (node) {
			RedBlackTree::emplace(&m_root, m_count, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void CMap<KT, VT, KEY_COMPARE>::emplaceAll(const MAP& other) noexcept
	{
		typename MAP::EnumLockHelper helper(other);
		auto node = helper.node;
		if (!node) {
			return;
		}
		if (reinterpret_cast<void*>(RedBlackTree::getFirstNode(m_root)) == reinterpret_cast<void*>(node)) {
			return;
		}
		MutexLocker lock(getLocker(), helper.mutex);
		while (node) {
			RedBlackTree::emplace(&m_root, m_count, node->key, m_compare, node->value);
			node = node->getNext();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void CMap<KT, VT, KEY_COMPARE>::removeAt(MapNode<KT, VT>* node) noexcept
	{
		RedBlackTree::removeNode(&m_root, m_count, node);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size CMap<KT, VT, KEY_COMPARE>::removeAt(MapNode<KT, VT>* node, sl_size count) noexcept
	{
		return RedBlackTree::removeNodes(&m_root, m_count, node, count);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size CMap<KT, VT, KEY_COMPARE>::removeRange(MapNode<KT, VT>* first, MapNode<KT, VT>* last) noexcept
	{
		return RedBlackTree::removeRange(&m_root, m_count, first, last);
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool CMap<KT, VT, KEY_COMPARE>::remove_NoLock(const KT& key, VT* outValue) noexcept
	{
		return RedBlackTree::remove(&m_root, m_count, key, m_compare, outValue);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::remove(const KT& key, VT* outValue) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::remove(&m_root, m_count, key, m_compare, outValue);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size CMap<KT, VT, KEY_COMPARE>::removeItems_NoLock(const KT& key) noexcept
	{
		return RedBlackTree::removeItems(&m_root, m_count, key, m_compare);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size CMap<KT, VT, KEY_COMPARE>::removeItems(const KT& key) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::removeItems(&m_root, m_count, key, m_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> CMap<KT, VT, KEY_COMPARE>::removeItemsAndReturnValues_NoLock(const KT& key) noexcept
	{
		List<VT> list;
		RedBlackTree::removeItemsAndReturnValues(list, &m_root, m_count, key, m_compare);
		return list;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> CMap<KT, VT, KEY_COMPARE>::removeItemsAndReturnValues(const KT& key) noexcept
	{
		ObjectLocker lock(this);
		List<VT> list;
		RedBlackTree::removeItemsAndReturnValues(list, &m_root, m_count, key, m_compare);
		return list;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool CMap<KT, VT, KEY_COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		return RedBlackTree::removeKeyAndValue(&m_root, m_count, key, m_compare, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool CMap<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::removeKeyAndValue(&m_root, m_count, key, m_compare, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_size CMap<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		return RedBlackTree::removeItemsByKeyAndValue(&m_root, m_count, key, m_compare, value, value_equals);
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size CMap<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return RedBlackTree::removeItemsByKeyAndValue(&m_root, m_count, key, m_compare, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size CMap<KT, VT, KEY_COMPARE>::removeAll_NoLock() noexcept
	{
		NODE* root = m_root;
		if (root) {
			RedBlackTree::freeNodes(root);
			m_root = sl_null;
		}
		sl_size n = m_count;
		m_count = 0;
		return n;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size CMap<KT, VT, KEY_COMPARE>::removeAll() noexcept
	{
		ObjectLocker lock(this);
		return removeAll_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::copyFrom_NoLock(const CMap<KT, VT, KEY_COMPARE>& other) noexcept
	{
		if (this == &other) {
			return sl_true;
		}
		NODE* root = m_root;
		if (root) {
			RedBlackTree::freeNodes(root);
		}
		root = other.m_root;
		if (root) {
			root = RedBlackTree::duplicateNode(root);
			if (root) {
				m_root = root;
				m_count = other.m_count;
                return sl_true;
			} else {
				m_root = sl_null;
				m_count = 0;
                return sl_false;
			}
		} else {
			m_root = sl_null;
			m_count = 0;
            return sl_true;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool CMap<KT, VT, KEY_COMPARE>::copyFrom(const CMap<KT, VT, KEY_COMPARE>& other) noexcept
	{
		if (this == &other) {
			return sl_true;
		}
		ObjectLocker lock(this, &other);
		return copyFrom_NoLock(other);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	CMap<KT, VT, KEY_COMPARE>* CMap<KT, VT, KEY_COMPARE>::duplicate_NoLock() const noexcept
	{
		NODE* root = m_root;
		if (root) {
			NODE* other = RedBlackTree::duplicateNode(root);
			if (other) {
				CMap* ret = new CMap;
				if (ret) {
					ret->m_root = other;
					ret->m_count = m_count;
					return ret;
				} else {
					RedBlackTree::freeNodes(other);
				}
			}
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	CMap<KT, VT, KEY_COMPARE>* CMap<KT, VT, KEY_COMPARE>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<KT> CMap<KT, VT, KEY_COMPARE>::getAllKeys_NoLock() const noexcept
	{
		List<KT> ret;
		NODE* node = RedBlackTree::getFirstNode(m_root);
		while (node) {
			ret.add_NoLock(node->key);
			node = node->getNext();
		}
		return ret;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<KT> CMap<KT, VT, KEY_COMPARE>::getAllKeys() const noexcept
	{
		ObjectLocker lock(this);
		return getAllKeys_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> CMap<KT, VT, KEY_COMPARE>::getAllValues_NoLock() const noexcept
	{
		List<VT> ret;
		NODE* node = RedBlackTree::getFirstNode(m_root);
		while (node) {
			ret.add_NoLock(node->value);
			node = node->getNext();
		}
		return ret;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<VT> CMap<KT, VT, KEY_COMPARE>::getAllValues() const noexcept
	{
		ObjectLocker lock(this);
		return getAllValues_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > CMap<KT, VT, KEY_COMPARE>::toList_NoLock() const noexcept
	{
		List< Pair<KT, VT> > ret;
		NODE* node = RedBlackTree::getFirstNode(m_root);
		while (node) {
			ret.add_NoLock(Pair<KT, VT>(node->key, node->value));
			node = node->getNext();
		}
		return ret;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > CMap<KT, VT, KEY_COMPARE>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return toList_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE typename CMap<KT, VT, KEY_COMPARE>::POSITION CMap<KT, VT, KEY_COMPARE>::begin() const noexcept
	{
		return RedBlackTree::getFirstNode(m_root);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE typename CMap<KT, VT, KEY_COMPARE>::POSITION CMap<KT, VT, KEY_COMPARE>::end() const noexcept
	{
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>::EnumLockHelper::EnumLockHelper(const CMap<KT, VT, KEY_COMPARE>& map) noexcept
	{
		node = map.getFirstNode();
		mutex = map.getLocker();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE CMap<KT, VT, KEY_COMPARE>::EnumHelper::EnumHelper(const CMap<KT, VT, KEY_COMPARE>& map) noexcept
	{
		node = map.getFirstNode();
	}
	
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE> Map<KT, VT, KEY_COMPARE>::create() noexcept
	{
		return new CMAP();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE> Map<KT, VT, KEY_COMPARE>::create(const KEY_COMPARE& compare) noexcept
	{
		return new CMAP(compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE, class OTHER_COMPARE>
	SLIB_INLINE const Map<KT, VT, KEY_COMPARE>& Map<KT, VT, KEY_COMPARE>::from(const Map<KEY, VALUE, OTHER_COMPARE>& other) noexcept
	{
		return *(reinterpret_cast<Map const*>(&other));
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void Map<KT, VT, KEY_COMPARE>::init() noexcept
	{
		ref = new CMAP();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void Map<KT, VT, KEY_COMPARE>::init(const KEY_COMPARE& compare) noexcept
	{
		ref = new CMAP(compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT Map<KT, VT, KEY_COMPARE>::operator[](const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::getCount() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::isEmpty() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::isNotEmpty() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::getFirstNode() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getFirstNode();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::getLastNode() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getLastNode();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::find_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->find_NoLock(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::find(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->find(key);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::getEqualRange(const KT& key, MapNode<KT, VT>** pStart, MapNode<KT, VT>** pEnd) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getEqualRange(key, pStart, pEnd);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void Map<KT, VT, KEY_COMPARE>::getNearest(const KT& key, MapNode<KT, VT>** pLessEqual, MapNode<KT, VT>** pGreaterEqual) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->getNearest(key, pLessEqual, pGreaterEqual);
		} else {
			if (pLessEqual) {
				*pLessEqual = sl_null;
			}
			if (pGreaterEqual) {
				*pGreaterEqual = sl_null;
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::getLowerBound(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getLowerBound(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::getUpperBound(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getUpperBound(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->findKeyAndValue_NoLock(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->findKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT* Map<KT, VT, KEY_COMPARE>::getItemPointer(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getItemPointer(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE VT* Map<KT, VT, KEY_COMPARE>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getItemPointerByKeyAndValue(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->get_NoLock(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::get(const KT& key, VT* _out) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT Map<KT, VT, KEY_COMPARE>::getValue_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT Map<KT, VT, KEY_COMPARE>::getValue(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT Map<KT, VT, KEY_COMPARE>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key, def);
		} else {
			return def;
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE VT Map<KT, VT, KEY_COMPARE>::getValue(const KT& key, const VT& def) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key, def);
		} else {
			return def;
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::getValues_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValues_NoLock(key);
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::getValues(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValuesByKeyAndValue_NoLock(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getValuesByKeyAndValue(key, value, value_equals);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::put_NoLock(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool Map<KT, VT, KEY_COMPARE>::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::replace_NoLock(const KEY& key, VALUE&& value) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->replace_NoLock(key, Forward<VALUE>(value));
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::replace(const KEY& key, VALUE&& value) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->replace(key, Forward<VALUE>(value));
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	MapNode<KT, VT>* Map<KT, VT, KEY_COMPARE>::add_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool Map<KT, VT, KEY_COMPARE>::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	MapEmplaceReturn< MapNode<KT, VT> > Map<KT, VT, KEY_COMPARE>::emplace_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept
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
					return MapEmplaceReturn< MapNode<KT, VT> >(sl_true, node);
				} else {
					return sl_null;
				}
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool Map<KT, VT, KEY_COMPARE>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Map<KT, VT, KEY_COMPARE>::putAll_NoLock(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->putAll_NoLock(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				obj->putAll_NoLock(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Map<KT, VT, KEY_COMPARE>::putAll(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->putAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				obj->putAll(other);
				return;
			}
			obj = new CMAP;
			if (obj) {
				ref = obj;
				lock.unlock();
				obj->putAll(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	SLIB_INLINE void Map<KT, VT, KEY_COMPARE>::replaceAll_NoLock(const MAP& other) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->replaceAll_NoLock(other);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	SLIB_INLINE void Map<KT, VT, KEY_COMPARE>::replaceAll(const MAP& other) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->replaceAll(other);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Map<KT, VT, KEY_COMPARE>::addAll_NoLock(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->addAll_NoLock(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				obj->addAll_NoLock(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Map<KT, VT, KEY_COMPARE>::addAll(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->addAll(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				obj->addAll(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Map<KT, VT, KEY_COMPARE>::emplaceAll_NoLock(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->emplaceAll_NoLock(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				obj->emplaceAll_NoLock(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Map<KT, VT, KEY_COMPARE>::emplaceAll(const MAP& other) noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->emplaceAll(other);
		} else {
			obj = new CMAP;
			if (obj) {
				ref = obj;
				obj->emplaceAll(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE void Map<KT, VT, KEY_COMPARE>::removeAt(MapNode<KT, VT>* node) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			obj->removeAt(node);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeAt(MapNode<KT, VT>* node, sl_size count) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeAt(node, count);
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeRange(MapNode<KT, VT>* first, MapNode<KT, VT>* last) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeRange(first, last);
		}
		return 0;
	}


	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::remove_NoLock(const KT& key, VT* outValue) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->remove_NoLock(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::remove(const KT& key, VT* outValue) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeItems_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItems_NoLock(key);
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeItems(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItems(key);
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::removeItemsAndReturnValues_NoLock(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsAndReturnValues_NoLock(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::removeItemsAndReturnValues(const KT& key) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsAndReturnValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeKeyAndValue_NoLock(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_bool Map<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsByKeyAndValue_NoLock(key, value, value_equals);
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeItemsByKeyAndValue(key, value, value_equals);
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeAll_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_size Map<KT, VT, KEY_COMPARE>::removeAll() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE> Map<KT, VT, KEY_COMPARE>::duplicate_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE> Map<KT, VT, KEY_COMPARE>::duplicate() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<KT> Map<KT, VT, KEY_COMPARE>::getAllKeys_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<KT> Map<KT, VT, KEY_COMPARE>::getAllKeys() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::getAllValues_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List<VT> Map<KT, VT, KEY_COMPARE>::getAllValues() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List< Pair<KT, VT> > Map<KT, VT, KEY_COMPARE>::toList_NoLock() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->toList_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE List< Pair<KT, VT> > Map<KT, VT, KEY_COMPARE>::toList() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE typename Map<KT, VT, KEY_COMPARE>::POSITION Map<KT, VT, KEY_COMPARE>::begin() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getFirstNode();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE typename Map<KT, VT, KEY_COMPARE>::POSITION Map<KT, VT, KEY_COMPARE>::end() const noexcept
	{
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	const Mutex* Map<KT, VT, KEY_COMPARE>::getLocker() const noexcept
	{
		CMAP* obj = ref._ptr;
		if (obj) {
			return obj->getLocker();
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE>::EnumLockHelper::EnumLockHelper(const Map<KT, VT, KEY_COMPARE>& map) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE Map<KT, VT, KEY_COMPARE>::EnumHelper::EnumHelper(const Map<KT, VT, KEY_COMPARE>& map) noexcept
	{
		node = map.getFirstNode();
	}

	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE, class OTHER_COMPARE>
	const Atomic< Map<KT, VT, KEY_COMPARE> >& Atomic< Map<KT, VT, KEY_COMPARE> >::from(const Atomic< Map<KEY, VALUE, OTHER_COMPARE> >& other) noexcept
	{
		return *(reinterpret_cast<Atomic const*>(&other));
	}

	template <class KT, class VT, class KEY_COMPARE>
	void Atomic< Map<KT, VT, KEY_COMPARE> >::init() noexcept
	{
		ref = new CMAP;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void Atomic< Map<KT, VT, KEY_COMPARE> >::init(const KEY_COMPARE& compare) noexcept
	{
		ref = new CMAP;
	}

	template <class KT, class VT, class KEY_COMPARE>
	VT Atomic< Map<KT, VT, KEY_COMPARE> >::operator[](const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size Atomic< Map<KT, VT, KEY_COMPARE> >::getCount() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::isEmpty() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::isNotEmpty() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::find(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->find(key);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->findKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::get(const KT& key, VT* _out) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	VT Atomic< Map<KT, VT, KEY_COMPARE> >::getValue(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	VT Atomic< Map<KT, VT, KEY_COMPARE> >::getValue(const KT& key, const VT& def) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key, def);
		}
		return def;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<VT> Atomic< Map<KT, VT, KEY_COMPARE> >::getValues(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> Atomic< Map<KT, VT, KEY_COMPARE> >::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getValuesByKeyAndValue(key, value, value_equals);
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	SLIB_INLINE sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::replace(const KEY& key, VALUE&& value) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->replace(key, Forward<VALUE>(value));
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
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

	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class... VALUE_ARGS>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
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
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Atomic< Map<KT, VT, KEY_COMPARE> >::putAll(const MAP& other) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->putAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->putAll(other);
				return;
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->putAll(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Atomic< Map<KT, VT, KEY_COMPARE> >::replaceAll(const MAP& other) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->replaceAll(other);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Atomic< Map<KT, VT, KEY_COMPARE> >::addAll(const MAP& other) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->addAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->addAll(other);
				return;
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->addAll(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class MAP>
	void Atomic< Map<KT, VT, KEY_COMPARE> >::emplaceAll(const MAP& other) noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			obj->emplaceAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->emplaceAll(other);
				return;
			}
			obj = new CMAP;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->emplaceAll(other);
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::remove(const KT& key, VT* outValue) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size Atomic< Map<KT, VT, KEY_COMPARE> >::removeItems(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItems(key);
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> Atomic< Map<KT, VT, KEY_COMPARE> >::removeItemsAndReturnValues(const KT& key) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItemsAndReturnValues(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool Atomic< Map<KT, VT, KEY_COMPARE> >::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeKeyAndValue(key, value, value_equals);
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size Atomic< Map<KT, VT, KEY_COMPARE> >::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItemsByKeyAndValue(key, value, value_equals);
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size Atomic< Map<KT, VT, KEY_COMPARE> >::removeAll() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	Map<KT, VT, KEY_COMPARE> Atomic< Map<KT, VT, KEY_COMPARE> >::duplicate() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<KT> Atomic< Map<KT, VT, KEY_COMPARE> >::getAllKeys() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllKeys();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<VT> Atomic< Map<KT, VT, KEY_COMPARE> >::getAllValues() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllValues();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > Atomic< Map<KT, VT, KEY_COMPARE> >::toList() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	typename Atomic< Map<KT, VT, KEY_COMPARE> >::POSITION Atomic< Map<KT, VT, KEY_COMPARE> >::begin() const noexcept
	{
		Ref<CMAP> obj(ref);
		if (obj.isNotNull()) {
			return POSITION(obj->getFirstNode(), obj._ptr);
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE typename Atomic< Map<KT, VT, KEY_COMPARE> >::POSITION Atomic< Map<KT, VT, KEY_COMPARE> >::end() const noexcept
	{
		return sl_null;
	}
	
	
	template <class KT, class VT, class KEY_COMPARE>
	Atomic< Map<KT, VT, KEY_COMPARE> >::EnumLockHelper::EnumLockHelper(const Map<KT, VT, KEY_COMPARE>& map) noexcept
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
	
	template <class KT, class VT, class KEY_COMPARE>
	Atomic< Map<KT, VT, KEY_COMPARE> >::EnumHelper::EnumHelper(const Map<KT, VT, KEY_COMPARE>& map) noexcept
	 : node(map.getFirstNode()), ref(map.ref)
	{}

}

