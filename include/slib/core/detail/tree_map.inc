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
	TreeMapPosition<KT, VT>::TreeMapPosition() noexcept
	 : node(sl_null)
	 {}
	
	template <class KT, class VT>
	TreeMapPosition<KT, VT>::TreeMapPosition(RedBlackTreeNode<KT, VT>* _node) noexcept
	 : node(_node)
	 {}
	
	template <class KT, class VT>
	TreeMapPosition<KT, VT>& TreeMapPosition<KT, VT>::operator=(RedBlackTreeNode<KT, VT>* _node) noexcept
	{
		node = _node;
		return *this;
	}
	
	template <class KT, class VT>
	Pair<KT, VT>& TreeMapPosition<KT, VT>::operator*() const noexcept
	{
		return node->data;
	}
	
	template <class KT, class VT>
	sl_bool TreeMapPosition<KT, VT>::operator==(const TreeMapPosition& other) const noexcept
	{
		return node == other.node;
	}
	
	template <class KT, class VT>
	sl_bool TreeMapPosition<KT, VT>::operator!=(const TreeMapPosition& other) const noexcept
	{
		return node != other.node;
	}
	
	template <class KT, class VT>
	TreeMapPosition<KT, VT>::operator RedBlackTreeNode<KT, VT>*() const noexcept
	{
		return node;
	}
	
	template <class KT, class VT>
	TreeMapPosition<KT, VT>& TreeMapPosition<KT, VT>::operator++() noexcept
	{
		node = node->getNext();
		return *this;
	}

	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE TreeMap<KT, VT, KEY_COMPARE>::TreeMap() noexcept
	 {}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE TreeMap<KT, VT, KEY_COMPARE>::TreeMap(const KEY_COMPARE& key_compare) noexcept
	 : tree(key_compare)
	 {}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE TreeMap<KT, VT, KEY_COMPARE>::TreeMap(TreeMap<KT, VT, KEY_COMPARE>&& other) noexcept
	 : tree(Move(other.tree))
	 {}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE TreeMap<KT, VT, KEY_COMPARE>& TreeMap<KT, VT, KEY_COMPARE>::operator=(TreeMap<KT, VT, KEY_COMPARE>&& other) noexcept
	{
		tree = Move(other.tree);
		return *this;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>* TreeMap<KT, VT, KEY_COMPARE>::create(const KEY_COMPARE& key_compare) noexcept
	{
		return new TreeMap<KT, VT, KEY_COMPARE>(key_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT TreeMap<KT, VT, KEY_COMPARE>::operator[](const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return node->data.value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::_getCount() const noexcept
	{
		return tree.getCount();
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::getCount() const noexcept
	{
		return tree.getCount();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::isEmpty() const noexcept
	{
		return tree.getCount() == 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::isNotEmpty() const noexcept
	{
		return tree.getCount() > 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT* TreeMap<KT, VT, KEY_COMPARE>::_getItemPointer(const KT& key) const noexcept
	{
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return &(node->data.value);
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	VT* TreeMap<KT, VT, KEY_COMPARE>::getItemPointer(const KT& key) const noexcept
	{
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return &(node->data.value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			if (_out) {
				*_out = node->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::get(const KT& key, VT* _out) const noexcept
	{
		ObjectLocker lock(this);
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			if (_out) {
				*_out = node->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT TreeMap<KT, VT, KEY_COMPARE>::getValue_NoLock(const KT& key) const noexcept
	{
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return node->data.value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT TreeMap<KT, VT, KEY_COMPARE>::getValue(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return node->data.value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT TreeMap<KT, VT, KEY_COMPARE>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return node->data.value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT TreeMap<KT, VT, KEY_COMPARE>::getValue(const KT& key, const VT& def) const noexcept
	{
		ObjectLocker lock(this);
		RedBlackTreeNode<KT, VT>* node;
		if (tree.find(key, &node)) {
			return node->data.value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::_getValues(const KT& key) const noexcept
	{
		return tree.getValues(key);
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::getValues_NoLock(const KT& key) const noexcept
	{
		return tree.getValues(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::getValues(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return tree.getValues(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::_put(const KT& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			RedBlackTreeNode<KT, VT>* node;
			if (tree.put(key, value, mode, &node)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Tree, node, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return tree.put(key, value, mode);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::_put(const KT& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			RedBlackTreeNode<KT, VT>* node;
			if (tree.put(key, Move(value), mode, &node)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Tree, node, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return tree.put(key, Move(value), mode);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::_put(KT&& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			RedBlackTreeNode<KT, VT>* node;
			if (tree.put(Move(key), value, mode, &node)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Tree, node, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return tree.put(Move(key), value, mode);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::_put(KT&& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			RedBlackTreeNode<KT, VT>* node;
			if (tree.put(Move(key), Move(value), mode, &node)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Tree, node, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return tree.put(Move(key), Move(value), mode);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode, RedBlackTreeNode<KT, VT>** ppNode) noexcept
	{
		return tree.put(Forward<KEY>(key), Forward<VALUE>(value), mode, ppNode);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::put(KEY&& key, VALUE&& value, MapPutMode mode, RedBlackTreeNode<KT, VT>** ppNode) noexcept
	{
		ObjectLocker lock(this);
		return tree.put(Forward<KEY>(key), Forward<VALUE>(value), mode, ppNode);
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::addIfNewKeyAndValue_NoLock(KEY&& key, VALUE&& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_COMPARE& value_compare) noexcept
	{
		return tree.addIfNewKeyAndValue(key, value, ppNode, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::addIfNewKeyAndValue(KEY&& key, VALUE&& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_COMPARE& value_compare) noexcept
	{
		ObjectLocker lock(this);
		return tree.addIfNewKeyAndValue(key, value, ppNode, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::_remove(const KT& key, VT* outValue) noexcept
	{
		return tree.remove(key, outValue);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::remove_NoLock(const KT& key, VT* outValue) noexcept
	{
		return tree.remove(key, outValue);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::remove(const KT& key, VT* outValue) noexcept
	{
		ObjectLocker lock(this);
		return tree.remove(key, outValue);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::_removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		return tree.removeItems(key, outValues);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItems_NoLock(const KT& key, List<VT>* outValues) noexcept
	{
		return tree.removeItems(key, outValues);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		ObjectLocker lock(this);
		return tree.removeItems(key, outValues);
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue, const VALUE_COMPARE& value_compare) noexcept
	{
		return tree.removeKeyAndValue(key, value, outValue, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue, const VALUE_COMPARE& value_compare) noexcept
	{
		ObjectLocker lock(this);
		return tree.removeKeyAndValue(key, value, outValue, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_COMPARE& value_compare) noexcept
	{
		return tree.removeItemsByKeyAndValue(key, value, outValues, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_COMPARE& value_compare) noexcept
	{
		ObjectLocker lock(this);
		return tree.removeItemsByKeyAndValue(key, value, outValues, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::_removeAll() noexcept
	{
		return tree.removeAll();
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeAll_NoLock() noexcept
	{
		return tree.removeAll();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeAll() noexcept
	{
		ObjectLocker lock(this);
		return tree.removeAll();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void TreeMap<KT, VT, KEY_COMPARE>::_removeAt(const MapPosition<KT, VT>& position) noexcept
	{
		tree.removeNode(reinterpret_cast<RedBlackTreeNode<KT, VT>*>(position.node));
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void TreeMap<KT, VT, KEY_COMPARE>::removeAt_NoLock(const RedBlackTreeNode<KT, VT>* node) noexcept
	{
		tree.removeNode(const_cast<RedBlackTreeNode<KT, VT>*>(node));
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void TreeMap<KT, VT, KEY_COMPARE>::removeAt(const RedBlackTreeNode<KT, VT>* node) noexcept
	{
		ObjectLocker lock(this);
		tree.removeNode(const_cast<RedBlackTreeNode<KT, VT>*>(node));
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::_find(const KT& key, MapPosition<KT, VT>* pPosition) const noexcept
	{
		if (pPosition) {
			RedBlackTreeNode<KT, VT>* node;
			if (tree.find(key, &node)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Tree, node, sl_null);
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			return tree.find(key) != sl_null;
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::find_NoLock(const KT& key) const noexcept
	{
		return tree.find(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::find(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return tree.find(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::find_NoLock(const KT& key, RedBlackTreeNode<KT, VT>** ppNode) const noexcept
	{
		return tree.find(key, ppNode);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::find(const KT& key, RedBlackTreeNode<KT, VT>** ppNode) const noexcept
	{
		ObjectLocker lock(this);
		return tree.find(key, ppNode);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	RedBlackTreeNode<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		return tree.findKeyAndValue(key, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	RedBlackTreeNode<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		return tree.findKeyAndValue(key, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::searchKeyAndValue_NoLock(const KT& key, const VALUE& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals) const noexcept
	{
		return tree.findKeyAndValue(key, value, ppNode, value_equals);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::searchKeyAndValue(const KT& key, const VALUE& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		return tree.findKeyAndValue(key, value, ppNode, value_equals);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::contains_NoLock(const KT& key) const noexcept
	{
		return tree.find(key) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::contains(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return tree.find(key) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		return tree.findKeyAndValue(key, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		return tree.findKeyAndValue(key, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::getFirstNode() const noexcept
	{
		return tree.getFirstNode();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::getLastNode() const noexcept
	{
		return tree.getLastNode();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	IMap<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::_duplicate() const noexcept
	{
		return TreeMap::duplicate_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>* TreeMap<KT, VT, KEY_COMPARE>::duplicate_NoLock() const noexcept
	{
		TreeMap<KT, VT, KEY_COMPARE>* ret = new TreeMap<KT, VT, KEY_COMPARE>;
		if (ret) {
			if (ret->tree.copyFrom(&tree)) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>* TreeMap<KT, VT, KEY_COMPARE>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return TreeMap::duplicate_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<KT> TreeMap<KT, VT, KEY_COMPARE>::_getAllKeys() const noexcept
	{
		return TreeMap::getAllKeys_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<KT> TreeMap<KT, VT, KEY_COMPARE>::getAllKeys_NoLock() const noexcept
	{
		if (tree.getCount() == 0) {
			return sl_null;
		}
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			RedBlackTreeNode<KT, VT>* node = tree.getFirstNode();
			while (node) {
				if (!(ret->add_NoLock(node->data.key))) {
					delete ret;
					return sl_null;
				}
				node = node->getNext();
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<KT> TreeMap<KT, VT, KEY_COMPARE>::getAllKeys() const noexcept
	{
		ObjectLocker lock(this);
		return TreeMap::getAllKeys_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::_getAllValues() const noexcept
	{
		return TreeMap::getAllValues_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::getAllValues_NoLock() const noexcept
	{
		if (tree.getCount() == 0) {
			return sl_null;
		}
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			RedBlackTreeNode<KT, VT>* node = tree.getFirstNode();
			while (node) {
				if (!(ret->add_NoLock(node->data.value))) {
					delete ret;
					return sl_null;
				}
				node = node->getNext();
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::getAllValues() const noexcept
	{
		ObjectLocker lock(this);
		return TreeMap::getAllValues_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > TreeMap<KT, VT, KEY_COMPARE>::_toList() const noexcept
	{
		return TreeMap::toList_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > TreeMap<KT, VT, KEY_COMPARE>::toList_NoLock() const noexcept
	{
		if (tree.getCount() == 0) {
			return sl_null;
		}
		CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
		if (ret) {
			RedBlackTreeNode<KT, VT>* node = tree.getFirstNode();
			while (node) {
				if (!(ret->add_NoLock(node->data))) {
					delete ret;
					return sl_null;
				}
				node = node->getNext();
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > TreeMap<KT, VT, KEY_COMPARE>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return TreeMap::toList_NoLock();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	MapPosition<KT, VT> TreeMap<KT, VT, KEY_COMPARE>::_getFirst(Referable* ref) const noexcept
	{
		return MapPosition<KT, VT>(MapContainerType::Tree, tree.getFirstNode(), ref);
	}

	template <class KT, class VT, class KEY_COMPARE>
	TreeMapPosition<KT, VT> TreeMap<KT, VT, KEY_COMPARE>::begin() const noexcept
	{
		return tree.getFirstNode();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMapPosition<KT, VT> TreeMap<KT, VT, KEY_COMPARE>::end() const noexcept
	{
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::copyFrom(const TreeMap<KT, VT, KEY_COMPARE>* other) noexcept
	{
		return tree.copyFrom(&(other->tree));
	}
	
}
