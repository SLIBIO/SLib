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
	
	SLIB_INLINE BTreeNode::BTreeNode() noexcept
	 : position(0)
	 {}
	
	SLIB_INLINE BTreeNode::BTreeNode(sl_null_t) noexcept
	 : position(0)
	 {}
	
	SLIB_INLINE BTreeNode::BTreeNode(sl_uint64 _position) noexcept
	 : position(_position)
	 {}

	SLIB_INLINE BTreeNode::BTreeNode(const BTreeNode& other) noexcept
	 : position(other.position)
	 {}

	SLIB_INLINE BTreeNode& BTreeNode::operator=(const BTreeNode& other) noexcept
	{
		position = other.position;
		return *this;
	}
	
	SLIB_INLINE BTreeNode& BTreeNode::operator=(sl_null_t) noexcept
	{
		position = 0;
		return *this;
	}

	SLIB_INLINE sl_bool BTreeNode::operator==(const BTreeNode& other) const noexcept
	{
		return position == other.position;
	}
	
	SLIB_INLINE sl_bool BTreeNode::operator!=(const BTreeNode& other) const noexcept
	{
		return position != other.position;
	}
	
	SLIB_INLINE sl_bool BTreeNode::isNull() const noexcept
	{
		return position == 0;
	}
	
	SLIB_INLINE sl_bool BTreeNode::isNotNull() const noexcept
	{
		return position != 0;
	}
	
	SLIB_INLINE void BTreeNode::setNull() noexcept
	{
		position = 0;
	}
	
	SLIB_INLINE BTreeNode::operator sl_bool() const noexcept
	{
		return position != 0;
	}
	
	
	SLIB_INLINE BTreePosition::BTreePosition() noexcept
	 : item(0)
	 {}
	
	SLIB_INLINE BTreePosition::BTreePosition(sl_null_t) noexcept
	 : item(0)
	 {}
	
	SLIB_INLINE BTreePosition::BTreePosition(const BTreeNode& _node, sl_uint32 _item) noexcept
	 : node(_node), item(_item)
	 {}
	
	SLIB_INLINE BTreePosition::BTreePosition(const BTreePosition& other) noexcept
	 : node(other.node), item(other.item)
	 {}
	
	SLIB_INLINE BTreePosition& BTreePosition::operator=(const BTreePosition& other) noexcept
	{
		node = other.node;
		item = other.item;
		return *this;
	}
	
	SLIB_INLINE BTreePosition& BTreePosition::operator=(sl_null_t) noexcept
	{
		node.setNull();
		item = 0;
		return *this;
	}

	SLIB_INLINE sl_bool BTreePosition::operator==(const BTreePosition& other) const noexcept
	{
		return node == other.node && item == other.item;
	}
	
	SLIB_INLINE sl_bool BTreePosition::operator!=(const BTreePosition& other) const noexcept
	{
		return node != other.node || item != other.item;
	}
	
	SLIB_INLINE sl_bool BTreePosition::isNull() const noexcept
	{
		return node.isNull();
	}
	
	SLIB_INLINE sl_bool BTreePosition::isNotNull() const noexcept
	{
		return node.isNotNull();
	}
	
	SLIB_INLINE void BTreePosition::setNull() noexcept
	{
		node.setNull();
	}
	
	SLIB_INLINE BTreePosition::operator sl_bool() const noexcept
	{
		return node.isNotNull();
	}

	
	template <class KT, class VT, class KEY_COMPARE>
	BTree<KT, VT, KEY_COMPARE>::BTree(sl_uint32 order)
	{
		if (order < 1) {
			order = 1;
		}
		m_order = order;
		m_maxLength = 0;
		m_totalCount = 0;
		init();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	BTree<KT, VT, KEY_COMPARE>::BTree(const KEY_COMPARE& compare, sl_uint32 order) : m_compare(compare)
	{
		if (order < 1) {
			order = 1;
		}
		m_order = order;
		m_maxLength = 0;
		m_totalCount = 0;
		init();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	BTree<KT, VT, KEY_COMPARE>::~BTree()
	{
		free();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool BTree<KT, VT, KEY_COMPARE>::isValid() const noexcept
	{
		return m_rootNode != sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_uint32 BTree<KT, VT, KEY_COMPARE>::getOrder() const noexcept
	{
		return m_order;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_uint32 BTree<KT, VT, KEY_COMPARE>::getMaxLength() const noexcept
	{
		return m_maxLength;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_uint64 BTree<KT, VT, KEY_COMPARE>::getCountInNode(const BTreeNode& node) const
	{
		if (node.isNull()) {
			return 0;
		}
		NodeDataScope data(this, node);
		if (data.isNotNull()) {
			sl_uint64 ret = data->countTotal;
			return ret;
		} else {
			return 0;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_uint64 BTree<KT, VT, KEY_COMPARE>::getCount() const
	{
		return getCountInNode(getRootNode());
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::isEmpty() const
	{
		return getCountInNode(getRootNode()) == 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::isNotEmpty() const
	{
		return getCountInNode(getRootNode()) > 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::getAt(const BTreePosition& pos, KT* key, VT* value) const
	{
		NodeDataScope data(this, pos.node);
		if (data.isNotNull()) {
			if (pos.item < data->countItems) {
				if (key) {
					*key = data->keys[pos.item];
				}
				if (value) {
					*value = data->values[pos.item];
				}
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::moveToFirstInNode(const BTreeNode& _node, BTreePosition& pos, KT* key, VT* value) const
	{
		BTreeNode node(_node);
		while (1) {
			NodeDataScope data(this, node);
			if (data.isNull()) {
				return sl_false;
			}
			BTreeNode first = data->linkFirst;
			if (first.isNotNull()) {
				node = first;
			} else {
				if (data->countItems == 0) {
					return sl_false;
				} else {
					if (key) {
						*key = data->keys[0];
					}
					if (value) {
						*value = data->values[0];
					}
					pos.node = node;
					pos.item = 0;
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::moveToFirst(BTreePosition& pos, KT* key, VT* value ) const
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			return sl_false;
		}
		return moveToFirstInNode(root, pos, key, value);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::moveToLastInNode(const BTreeNode& _node, BTreePosition& pos, KT* key, VT* value) const
	{
		BTreeNode node(_node);
		while (1) {
			NodeDataScope data(this, node);
			if (data.isNull()) {
				return sl_false;
			}
			sl_uint32 n = data->countItems;
			if (n == 0) {
				BTreeNode first = data->linkFirst;
				if (first.isNotNull()) {
					node = first;
				} else {
					return sl_false;
				}
			} else {
				BTreeNode last = data->links[n - 1];
				if (last.isNotNull()) {
					node = last;
				} else {
					if (key) {
						*key = data->keys[n - 1];
					}
					if (value) {
						*value = data->values[n - 1];
					}
					pos.node = node;
					pos.item = n - 1;
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::moveToLast(BTreePosition& pos, KT* key, VT* value) const
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			return sl_false;
		}
		return moveToLastInNode(root, pos, key, value);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::moveToPrevious(BTreePosition& pos, KT* key, VT* value) const
	{
		if (pos.isNull()) {
			return moveToLast(pos, key, value);
		}
		BTreeNode nodeStart = pos.node;
		sl_uint32 itemStart = pos.item;
		NodeDataScope dataStart(this, nodeStart);
		if (dataStart.isNull()) {
			return sl_false;
		}
		if (itemStart >= dataStart->countItems) {
			return sl_false;
		}
		BTreeNode node;
		if (itemStart == 0) {
			node = dataStart->linkFirst;
		} else {
			node = dataStart->links[itemStart - 1];
		}
		if (node.isNotNull()) {
			return moveToLastInNode(node, pos, key, value);
		} else {
			if (itemStart == 0) {
				node = nodeStart;
				BTreeNode parent = dataStart->linkParent;
				while (1) {
					if (parent.isNull()) {
						return sl_false;
					}
					NodeDataScope data(this, parent);
					if (data.isNull()) {
						return sl_false;
					}
					if (data->linkFirst == node) {
						node = parent;
						parent = data->linkParent;
					} else {
						sl_uint32 n = data->countItems;
						sl_uint32 i = 0;
						for (; i < n; i++) {
							if (data->links[i] == node) {
								pos.node = parent;
								pos.item = i;
								if (key) {
									*key = data->keys[i];
								}
								if (value) {
									*value = data->values[i];
								}
								return sl_true;
							}
						}
						return sl_false;
					}
				}
			} else {
				itemStart--;
				pos.item = itemStart;
				if (key) {
					*key = dataStart->keys[itemStart];
				}
				if (value) {
					*value = dataStart->values[itemStart];
				}
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::moveToNext(BTreePosition& pos, KT* key, VT* value) const
	{
		if (pos.isNull()) {
			return moveToFirst(pos, key, value);
		}
		BTreeNode nodeStart = pos.node;
		sl_uint32 itemStart = pos.item;
		NodeDataScope dataStart(this, nodeStart);
		if (dataStart.isNull()) {
			return sl_false;
		}
		if (itemStart >= dataStart->countItems) {
			return sl_false;
		}
		BTreeNode node = dataStart->links[itemStart];
		if (node.isNotNull()) {
			return moveToFirst(pos, key, value);
		} else {
			if (itemStart == dataStart->countItems - 1) {
				node = nodeStart;
				BTreeNode parent = dataStart->linkParent;
				while (1) {
					if (parent.isNull()) {
						return sl_false;
					}
					NodeDataScope data(this, parent);
					if (data.isNull()) {
						return sl_false;
					}
					if (data->linkFirst == node) {
						pos.node = parent;
						pos.item = 0;
						if (key) {
							*key = data->keys[0];
						}
						if (value) {
							*value = data->values[0];
						}
						return sl_true;
					}
					sl_uint32 n = data->countItems;
					for (sl_uint32 i = 1; i < n; i++) {
						if (data->links[i - 1] == node) {
							pos.node = parent;
							pos.item = i;
							if (key) {
								*key = data->keys[i];
							}
							if (value) {
								*value = data->values[i];
							}
							return sl_true;
						}
					}
					if (data->links[n - 1] == node) {
						node = parent;
						parent = data->linkParent;
					} else {
						return sl_false;
					}
				}
			} else {
				itemStart++;
				pos.item = itemStart;
				if (key) {
					*key = dataStart->keys[itemStart];
				}
				if (value) {
					*value = dataStart->values[itemStart];
				}
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::findItemInNode(const BTreeNode& node, const KT& key, sl_uint32& pos, BTreeNode& link, VT* outValue, sl_uint32* pCountItemsInNode) const
	{
		pos = 0;
		NodeDataScope data(this, node);
		if (data.isNull()) {
			link.setNull();
			if (pCountItemsInNode) {
				*pCountItemsInNode = 0;
			}
			return sl_false;
		}
		sl_uint32 n = data->countItems;
		if (pCountItemsInNode) {
			*pCountItemsInNode = n;
		}
		if (n == 0) {
			link.setNull();
			return sl_false;
		}
		sl_size _pos = 0;
		if (BinarySearch::search(data->keys, n, key, &_pos, m_compare)) {
			pos = (sl_uint32)_pos;
			if (outValue) {
				*outValue = data->values[pos];
			}
			link = data->links[pos];
			return sl_true;
		} else {
			pos = (sl_uint32)_pos;
			if (pos) {
				link = data->links[pos - 1];
			} else {
				link = data->linkFirst;
			}
			return sl_false;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::findInNode(const BTreeNode& node, const KT& key, BTreePosition* pos, VT* outValue) const
	{
		BTreeNode link;
		sl_uint32 item;
		if (findItemInNode(node, key, item, link, outValue)) {
			if (pos) {
				pos->node = node;
				pos->item = item;
			}
			return sl_true;
		} else {
			if (link.isNotNull()) {
				return findInNode(link, key, pos, outValue);
			} else {
				if (pos) {
					pos->node = node;
					pos->item = item;
				}
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::find(const KT& key, BTreePosition* pos, VT* outValue) const
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			return sl_false;
		}
		return findInNode(root, key, pos, outValue);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	BTreePosition BTree<KT, VT, KEY_COMPARE>::findInsertPositionInNode(const BTreeNode& node, const KT& key) const
	{
		BTreeNode link;
		sl_uint32 item;
		findItemInNode(node, key, item, link, sl_null);
		if (link.isNotNull()) {
			return findInsertPositionInNode(link, key);
		} else {
			return BTreePosition(node, item);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::getBoundsInNode(const BTreeNode& node, const KT& key, BTreePosition* pLessEqual, BTreePosition* pGreaterEqual) const
	{
		BTreeNode link;
		sl_uint32 item;
		sl_uint32 n;
		if (findItemInNode(node, key, item, link, sl_null, &n)) {
			if (pLessEqual) {
				pLessEqual->node = node;
				pLessEqual->item = item;
			}
			if (pGreaterEqual) {
				pGreaterEqual->node = node;
				pGreaterEqual->item = item;
			}
			return sl_true;
		} else {
			if (link.isNotNull()) {
				return getBoundsInNode(link, key, pLessEqual, pGreaterEqual);
			} else {
				if (n == 0) {
					if (pLessEqual) {
						pLessEqual->setNull();
					}
					if (pGreaterEqual) {
						pGreaterEqual->setNull();
					}
				} else {
					if (pLessEqual) {
						pLessEqual->node = node;
						pLessEqual->item = item;
						if (!(moveToPrevious(*pLessEqual))) {
							pLessEqual->setNull();
						}
					}
					if (pGreaterEqual) {
						pGreaterEqual->node = node;
						if (item == n) {
							pGreaterEqual->item = item - 1;
							if (!(moveToNext(*pGreaterEqual))) {
								pGreaterEqual->setNull();
							}
						} else {
							pGreaterEqual->item = item;
						}
					}
				}
				return sl_false;
			}
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::getBounds(const KT& key, BTreePosition* pLessEqual, BTreePosition* pGreaterEqual) const
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			if (pLessEqual) {
				pLessEqual->setNull();
			}
			if (pGreaterEqual) {
				pGreaterEqual->setNull();
			}
			return sl_false;
		}
		return getBoundsInNode(root, key, pLessEqual, pGreaterEqual);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::getEqualRange(const KT& key, BTreePosition* pBegin, BTreePosition* pEnd) const
	{
		BTreePosition pos;
		if (find(key, &pos)) {
			BTreePosition posMid = pos;
			KT keyItem;
			if (pBegin) {
				*pBegin = posMid;
				BTreePosition pos = posMid;
				while (moveToPrevious(pos, &keyItem)) {
					if (m_compare(keyItem, key) == 0) {
						*pBegin = pos;
					} else {
						break;
					}
				}
			}
			if (pEnd) {
				*pEnd = posMid;
				BTreePosition pos = posMid;
				while (moveToNext(pos, &keyItem)) {
					if (m_compare(keyItem, key) == 0) {
						*pEnd = pos;
					} else {
						break;
					}
				}
			}
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool BTree<KT, VT, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, BTreePosition* pPos, VT* outValue, const VALUE_EQUALS& value_equals) const
	{
		VT v;
		BTreePosition pos;
		if (find(key, &pos, &v)) {
			if (value_equals(value, v)) {
				if (pPos) {
					*pPos = pos;
				}
				if (outValue) {
					*outValue = v;
				}
				return sl_true;
			}
			BTreePosition posMid = pos;
			KT keyItem;
			while (moveToPrevious(pos, &keyItem, &v)) {
				if (m_compare(keyItem, key) == 0) {
					if (value_equals(value, v)) {
						if (pPos) {
							*pPos = pos;
						}
						if (outValue) {
							*outValue = v;
						}
						return sl_true;
					}
				} else {
					break;
				}
			}
			pos = posMid;
			while (moveToNext(pos, &keyItem, &v)) {
				if (m_compare(keyItem, key) == 0) {
					if (value_equals(value, v)) {
						if (pPos) {
							*pPos = pos;
						}
						if (outValue) {
							*outValue = v;
						}
						return sl_true;
					}
				} else {
					break;
				}
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::get(const KT& key, VT* value) const
	{
		return find(key, sl_null, value);
	}

	template <class KT, class VT, class KEY_COMPARE>
	List<VT> BTree<KT, VT, KEY_COMPARE>::getValues(const KT& key) const
	{
		List<VT> ret;
		BTreePosition pos;
		if (find(key, &pos)) {
			VT value;
			if (!(getAt(pos, sl_null, &value))) {
				return ret;
			}
			ret.add_NoLock(value);
			BTreePosition posMid = pos;
			KT keyItem;
			while (moveToPrevious(pos, &keyItem, &value)) {
				if (m_compare(keyItem, key) == 0) {
					ret.add_NoLock(value);
				} else {
					break;
				}
			}
			pos = posMid;
			while (moveToNext(pos, &keyItem, &value)) {
				if (m_compare(keyItem, key) == 0) {
					ret.add_NoLock(value);
				} else {
					break;
				}
			}
		}
		return ret;
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> BTree<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const
	{
		List<VT> ret;
		BTreePosition pos;
		VT v;
		if (find(key, &pos, &v)) {
			if (value_equals(v, value)) {
				ret.add_NoLock(v);
			}
			BTreePosition posMid = pos;
			KT keyItem;
			while (getPrevPosition(pos, &keyItem, &v)) {
				if (m_compare(keyItem, key) == 0) {
					if (value_equals(v, value)) {
						ret.add_NoLock(v);
					}
				} else {
					break;
				}
			}
			pos = posMid;
			while (getNextPosition(pos, &keyItem, &v)) {
				if (m_compare(keyItem, key) == 0) {
					if (value_equals(v, value)) {
						ret.add_NoLock(v);
					}
				} else {
					break;
				}
			}
		}
		return ret;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::put(const KT& key, const VT& value, BTreePosition* pPos, sl_bool* isInsertion) noexcept
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			if (isInsertion) {
				*isInsertion = sl_true;
			}
			return sl_false;
		}
		BTreePosition pos;
		if (findInNode(root, key, &pos)) {
			if (isInsertion) {
				*isInsertion = sl_false;
			}
			if (pPos) {
				*pPos = pos;
			}
			NodeDataScope data(this, pos.node);
			if (data.isNotNull()) {
				if (pos.item < data->countItems) {
					data->values[pos.item] = value;
					return writeNodeData(pos.node, data.data);
				}
			}
			return sl_false;
		}
		BTreeNode link;
		if (_insertItemInNode(pos.node, pos.item, link, key, value, link, pPos)) {
			if (isInsertion) {
				*isInsertion = sl_true;
			}
			return sl_true;
		}
		if (isInsertion) {
			*isInsertion = sl_false;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::replace(const KT& key, const VT& value, BTreePosition* pPos) noexcept
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			return sl_false;
		}
		BTreePosition pos;
		if (findInNode(root, key, &pos)) {
			if (pPos) {
				*pPos = pos;
			}
			NodeDataScope data(this, pos.node);
			if (data.isNotNull()) {
				if (pos.item < data->countItems) {
					data->values[pos.item] = value;
					return writeNodeData(pos.node, data.data);
				}
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::add(const KT& key, const VT& value, BTreePosition* pPos) noexcept
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			return sl_false;
		}
		BTreePosition pos = findInsertPositionInNode(root, key);
		BTreeNode link;
		if (_insertItemInNode(pos.node, pos.item, link, key, value, link, pPos)) {
			return sl_true;
		}
		if (pPos) {
			pPos->setNull();
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::emplace(const KT& key, const VT& value, BTreePosition* pPos) noexcept
	{
		BTreeNode root = getRootNode();
		if (root.isNull()) {
			return sl_false;
		}
		BTreePosition pos;
		if (findInNode(root, key, &pos)) {
			return sl_false;
		}
		BTreeNode link;
		if (_insertItemInNode(pos.node, pos.item, link, key, value, link, pPos)) {
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::removeNode(const BTreeNode& node)
	{
		if (node.isNull()) {
			return sl_false;
		}
		if (node == getRootNode()) {
			return sl_false;
		}
		return _removeNode(node, sl_true);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::removeAt(const BTreePosition& pos)
	{
		if (pos.node.isNull()) {
			return sl_false;
		}
		NodeDataScope data(this, pos.node);
		if (data.isNull()) {
			return sl_false;
		}
		sl_uint32 n = data->countItems;
		if (n == 0) {
			return sl_false;
		}
		if (pos.item >= n) {
			return sl_false;
		}
		BTreeNode left;
		if (pos.item == 0) {
			left = data->linkFirst;
		} else {
			left = data->links[pos.item - 1];
		}
		BTreeNode right = data->links[pos.item];
		if (left.isNull()) {
			if (pos.item == 0) {
				data->linkFirst = data->links[0];
			} else {
				data->links[pos.item - 1] = data->links[pos.item];
			}
		} else {
			if (right.isNotNull()) {
				BTreePosition nextPos = pos;
				KT keyNext;
				VT valueNext;
				if (!(moveToNext(nextPos, &keyNext, &valueNext))) {
					return sl_false;
				}
				if (nextPos.node.isNull()) {
					return sl_false;
				}
				if (nextPos.node == pos.node) {
					return sl_false;
				}
				data->keys[pos.item] = keyNext;
				data->values[pos.item] = valueNext;
				if (!writeNodeData(pos.node, data.data)) {
					return sl_false;
				}
				return removeAt(nextPos);
			}
		}
		if (n <= 1 && pos.node != getRootNode()) {
			return _removeNode(pos.node, sl_true);
		}
		for (sl_uint32 i = pos.item; i < n - 1; i++) {
			data->keys[i] = data->keys[i + 1];
			data->values[i] = data->values[i + 1];
			data->links[i] = data->links[i + 1];
		}
		data->countTotal--;
		data->countItems = n - 1;
		if (!writeNodeData(pos.node, data.data)) {
			return sl_false;
		}
		_changeParentTotalCount(data.data, -1);
		return sl_true;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::remove(const KT& key, VT* outValue)
	{
		BTreePosition pos;
		if (find(key, &pos, outValue)) {
			if (removeAt(pos)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size BTree<KT, VT, KEY_COMPARE>::removeItems(const KT& key, List<VT>* outValues)
	{
		BTreePosition pos;
		VT v;
		if (find(key, &pos, &v)) {
			if (removeAt(pos)) {
				if (outValues) {
					outValues->add_NoLock(v);			
				}
				sl_size n = 1;
				while (find(key, &pos, &v)) {
					if (removeAt(pos)) {
						if (outValues) {
							outValues->add_NoLock(v);
						}
						n++;
					} else {
						break;
					}
				}
				return n;
			}
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool BTree<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue, const VALUE_EQUALS& value_equals)
	{
		BTreePosition pos;
		if (findKeyAndValue(key, value, &pos, outValue, value_equals)) {
			if (removeAt(pos)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size BTree<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
	{
		BTreePosition pos;
		VT v;
		if (findKeyAndValue(key, value, &pos, &v, value_equals)) {
			if (removeAt(pos)) {
				if (outValues) {
					outValues->add_NoLock(v);
				}
				sl_size n = 1;
				while (searchKeyAndValue(key, value, &pos, &v, value_equals)) {
					if (removeAt(pos)) {
						if (outValues) {
							outValues->add_NoLock(v);
						}
						n++;
					} else {
						break;
					}
				}
				return n;
			}
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size BTree<KT, VT, KEY_COMPARE>::removeAll()
	{
		BTreeNode node = getRootNode();
		NodeDataScope data(this, node);
		sl_size countTotal = (sl_size)(data->countTotal);
		if (data.isNotNull()) {
			_removeNode(data->linkFirst, sl_false);
			sl_uint32 n = data->countItems;
			for (sl_uint32 i = 0; i < n; i++) {
				if (data->links[i].isNotNull()) {
					_removeNode(data->links[i], sl_false);
				}
			}
			data->countTotal = 0;
			data->countItems = 0;
			data->linkFirst.setNull();
			m_totalCount = 0;
			if (writeNodeData(node, data.data)) {
				return countTotal;
			}
		}
		return 0;
	}


	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE BTree<KT, VT, KEY_COMPARE>::NodeDataScope::NodeDataScope(const BTree* tree, const BTreeNode& node)
	{
		this->tree = (BTree*)tree;
		this->data = tree->readNodeData(node);
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE BTree<KT, VT, KEY_COMPARE>::NodeDataScope::~NodeDataScope()
	{
		tree->releaseNodeData(data);
	}

	template <class KT, class VT, class KEY_COMPARE>
	typename BTree<KT, VT, KEY_COMPARE>::NodeData* BTree<KT, VT, KEY_COMPARE>::NodeDataScope::operator->()
	{
		return data;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool BTree<KT, VT, KEY_COMPARE>::NodeDataScope::isNull() {
		return data == sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE sl_bool BTree<KT, VT, KEY_COMPARE>::NodeDataScope::isNotNull() {
		return data != sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	typename BTree<KT, VT, KEY_COMPARE>::NodeData* BTree<KT, VT, KEY_COMPARE>::_createNodeData()
	{
		NodeData* data = new NodeData;
		if (data) {
			data->countTotal = 0;
			data->countItems = 0;
			data->linkParent.setNull();
			data->linkFirst.setNull();
			data->keys = NewHelper<KT>::create(m_order);
			if (data->keys) {
				data->values = NewHelper<VT>::create(m_order);
				if (data->values) {
					data->links = NewHelper<BTreeNode>::create(m_order);
					if (data->links) {
						return data;
					}
					NewHelper<VT>::free(data->values, m_order);
				}
				NewHelper<KT>::free(data->keys, m_order);
			}
			delete data;
		}
		return sl_null;
	}

	template <class KT, class VT, class KEY_COMPARE>
	void BTree<KT, VT, KEY_COMPARE>::_freeNodeData(NodeData* data)
	{
		if (data) {
			NewHelper<KT>::free(data->keys, m_order);
			NewHelper<VT>::free(data->values, m_order);
			NewHelper<BTreeNode>::free(data->links, m_order);
			delete data;
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::_insertItemInNode(const BTreeNode& node, sl_uint32 at, const BTreeNode& after, const KT& key, const VT& value, const BTreeNode& link, BTreePosition* pPosition)
	{
		NodeDataScope data(this, node);
		if (data.isNull()) {
			return sl_false;
		}
		sl_uint32 n = data->countItems;
		if (n > m_order) {
			return sl_false;
		}
		if (after.isNotNull()) {
			if (after == data->linkFirst) {
				at = 0;
			} else {
				for (sl_uint32 i = 0; i < n; i++) {
					if (data->links[i] == after) {
						at = i + 1;
					}
				}
			}
		}
		if (at > n) {
			return sl_false;
		}
		if (n < m_order) {
			for (sl_uint32 i = n; i > at; i--) {
				data->keys[i] = Move(data->keys[i - 1]);
				data->values[i] = Move(data->values[i - 1]);
				data->links[i] = Move(data->links[i - 1]);
			}
			data->keys[at] = key;
			data->values[at] = value;
			data->links[at] = link;
			data->countItems = n + 1;
			data->countTotal += 1;
			if (writeNodeData(node, data.data)) {
				_changeParentTotalCount(data.data, 1);
				if (pPosition) {
					pPosition->node = node;
					pPosition->item = at;
				}
				return sl_true;
			}
		} else {
			sl_uint32 half = n / 2; // equals to  "m_order / 2"
			NodeData* newData = _createNodeData();
			if (newData) {
				KT keyTop;
				VT valueTop;
				sl_bool flagInsertAtTop = sl_false;
				sl_bool flagInsertAtNewNode = sl_false;
				sl_uint32 i;
				if (at > half) {
					sl_uint32 m = at - half - 1;
					newData->linkFirst = data->links[half];
					for (i = 0; i < m; i++) {
						newData->keys[i] = Move(data->keys[i + half + 1]);
						newData->values[i] = Move(data->values[i + half + 1]);
						newData->links[i] = Move(data->links[i + half + 1]);
					}
					newData->keys[m] = key;
					newData->values[m] = value;
					newData->links[m] = link;
					if (pPosition) {
						pPosition->item = m;
					}
					for (i = m + 1; i < n - half; i++) {
						newData->keys[i] = Move(data->keys[i + half]);
						newData->values[i] = Move(data->values[i + half]);
						newData->links[i] = Move(data->links[i + half]);
					}
					keyTop = data->keys[half];
					valueTop = data->values[half];
					flagInsertAtNewNode = sl_true;
				} else if (at < half) {
					newData->linkFirst = data->links[half - 1];
					for (i = 0; i < n - half; i++) {
						newData->keys[i] = Move(data->keys[i + half]);
						newData->values[i] = Move(data->values[i + half]);
						newData->links[i] = Move(data->links[i + half]);
					}
					keyTop = data->keys[half - 1];
					valueTop = data->values[half - 1];
					for (i = half - 1; i > at; i--) {
						data->keys[i] = Move(data->keys[i - 1]);
						data->values[i] = Move(data->values[i - 1]);
						data->links[i] = Move(data->links[i - 1]);
					}
					data->keys[at] = key;
					data->values[at] = value;
					data->links[at] = link;
					if (pPosition) {
						pPosition->node = node;
						pPosition->item = at;
					}
				} else {
					newData->linkFirst = link;
					for (i = 0; i < n - half; i++) {
						newData->keys[i] = Move(data->keys[i + half]);
						newData->values[i] = Move(data->values[i + half]);
						newData->links[i] = Move(data->links[i + half]);
					}
					keyTop = key;
					valueTop = value;
					flagInsertAtTop = sl_true;
				}
				BTreeNode parent = data->linkParent;
				sl_bool flagCreateRoot = sl_false;
				if (parent.isNull()) {
					parent = createNode(sl_null);
					if (parent.isNull()) {
						_freeNodeData(newData);
						return sl_false;
					}
					flagCreateRoot = sl_true;
					if (! setRootNode(parent)) {
						_freeNodeData(newData);
						return sl_false;
					}
					m_maxLength++;
				}
				data->linkParent = parent;
				data->countItems = half;
				data->countTotal = _getTotalCountInData(data.data);
				newData->linkParent = parent;
				newData->countItems = n - half;
				newData->countTotal = _getTotalCountInData(newData);
				BTreeNode newNode = createNode(newData);
				if (newNode.isNull()) {
					_freeNodeData(newData);
					return sl_false;
				}
				if (flagInsertAtNewNode) {
					if (pPosition) {
						pPosition->node = newNode;
					}
				}
				NodeDataScope newNodeData(this, newNode);
				sl_bool flagSuccess = sl_false;
				if (writeNodeData(node, data.data)) {
					if (newNodeData->linkFirst.isNotNull()) {
						NodeDataScope dataChild(this, newNodeData->linkFirst);
						if (dataChild.isNotNull()) {
							dataChild->linkParent = newNode;
							writeNodeData(newNodeData->linkFirst, dataChild.data);
						}
					}
					for (i = 0; i < n - half; i++) {
						BTreeNode child = newNodeData->links[i];
						if (child.isNotNull()) {
							NodeDataScope dataChild(this, child);
							if (dataChild.isNotNull()) {
								dataChild->linkParent = newNode;
								writeNodeData(child, dataChild.data);
							}
						}
					}
					if (flagCreateRoot) {
						NodeDataScope dataRoot(this, parent);
						if (dataRoot.isNotNull()) {
							dataRoot->countTotal = data->countTotal + newNodeData->countTotal + 1;
							m_totalCount = dataRoot->countTotal;
							dataRoot->countItems = 1;
							dataRoot->linkFirst = node;
							dataRoot->keys[0] = Move(keyTop);
							dataRoot->values[0] = Move(valueTop);
							dataRoot->links[0] = Move(newNode);
							flagSuccess = writeNodeData(parent, dataRoot.data);
						}
						if (flagInsertAtTop) {
							if (pPosition) {
								pPosition->node = parent;
								pPosition->item = 0;
							}
						}
					} else {
						if (flagInsertAtTop) {
							flagSuccess = _insertItemInNode(parent, -1, node, keyTop, valueTop, newNode, pPosition);
						} else {
							flagSuccess = _insertItemInNode(parent, -1, node, keyTop, valueTop, newNode, sl_null);
						}
					}
				}
				return flagSuccess;
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	void BTree<KT, VT, KEY_COMPARE>::_changeTotalCount(const BTreeNode& node, sl_int64 n)
	{
		NodeDataScope data(this, node);
		if (data.isNotNull()) {
			data->countTotal += n;
			writeNodeData(node, data.data);
			BTreeNode parent = data->linkParent;
			if (parent.isNotNull()) {
				_changeTotalCount(parent, n);
			}
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	void BTree<KT, VT, KEY_COMPARE>::_changeParentTotalCount(NodeData* data, sl_int64 n)
	{
		BTreeNode parent = data->linkParent;
		if (parent.isNotNull()) {
			_changeTotalCount(parent, n);
		} else {
			m_totalCount = data->countTotal;
		}
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_uint64 BTree<KT, VT, KEY_COMPARE>::_getTotalCountInData(NodeData* data) const
	{
		sl_uint32 n = data->countItems;
		sl_uint64 m = n + getCountInNode(data->linkFirst);
		for (sl_uint32 i = 0; i < n; i++) {
			m += getCountInNode(data->links[i]);
		}
		return m;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::_removeNode(const BTreeNode& node, sl_bool flagUpdateParent)
	{
		if (node.isNull()) {
			return sl_false;
		}
		NodeDataScope data(this, node);
		if (data.isNull()) {
			return sl_false;
		}
		if (flagUpdateParent) {
			BTreeNode parent = data->linkParent;
			if (parent.isNull()) {
				return sl_false;
			}
			NodeDataScope parentData(this, parent);
			if (parentData.isNotNull()) {
				if (node == parentData->linkFirst) {
					parentData->linkFirst.setNull();
				} else {
					sl_uint32 i;
					sl_uint32 n = parentData->countItems;
					for (i = 0; i < n; i++) {
						if (parentData->links[i] == node) {
							parentData->links[i].setNull();
							break;
						}
					}
					if (i == n) {
						return sl_false;
					}
				}
				parentData->countTotal -= data->countTotal;
				if (!writeNodeData(parent, parentData.data)) {
					return sl_false;
				}
				_changeParentTotalCount(parentData.data, -((sl_int64)(data->countTotal)));
			}
		}
		{
			_removeNode(data->linkFirst, sl_false);
			sl_uint32 n = data->countItems;
			for (sl_uint32 i = 0; i < n; i++) {
				if (data->links[i].isNotNull()) {
					_removeNode(data->links[i], sl_false);
				}
			}
		}
		return deleteNode(node);
	}

	template <class KT, class VT, class KEY_COMPARE>
	void BTree<KT, VT, KEY_COMPARE>::init()
	{
		m_rootNode = _createNodeData();
	}

	template <class KT, class VT, class KEY_COMPARE>
	void BTree<KT, VT, KEY_COMPARE>::free()
	{
		_removeNode(getRootNode(), sl_false);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	BTreeNode BTree<KT, VT, KEY_COMPARE>::getRootNode() const
	{
		return (sl_size)(m_rootNode);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::setRootNode(BTreeNode node)
	{
		if (node.isNull()) {
			return sl_false;
		}
		m_rootNode = (NodeData*)(void*)(sl_size)(node.position);
		return sl_true;
	}

	template <class KT, class VT, class KEY_COMPARE>
	BTreeNode BTree<KT, VT, KEY_COMPARE>::createNode(NodeData* data)
	{
		sl_size position;
		if (data) {
			position = (sl_size)data;
		} else {
			position = (sl_size)(_createNodeData());
		}
		// important for file storage
		{
			// _freeNodeData(data);
		}
		return position;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::deleteNode(BTreeNode node)
	{
		if (node.isNull()) {
			return sl_false;
		}
		NodeData* data = (NodeData*)(void*)(sl_size)(node.position);
		_freeNodeData(data);
		return sl_true;
	}

	template <class KT, class VT, class KEY_COMPARE>
	typename BTree<KT, VT, KEY_COMPARE>::NodeData* BTree<KT, VT, KEY_COMPARE>::readNodeData(const BTreeNode& node) const
	{
		NodeData* data = (NodeData*)(void*)(sl_size)(node.position);
		return data;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool BTree<KT, VT, KEY_COMPARE>::writeNodeData(const BTreeNode& node, NodeData* data)
	{
		if (node.isNull()) {
			return sl_false;
		}
		if (!data) {
			return sl_false;
		}
		NodeData* o = (NodeData*)(void*)(sl_size)(node.position);
		if (o != data) {
			sl_uint32 n = o->countItems = data->countItems;
			o->countTotal = data->countTotal;
			o->linkParent = data->linkParent;
			o->linkFirst = data->linkFirst;
			for (sl_uint32 i = 0; i < n; i++) {
				o->keys[i] = data->keys[i];
				o->values[i] = data->values[i];
				o->links[i] = data->links[i];
			}
		}
		return sl_true;
	}

	template <class KT, class VT, class KEY_COMPARE>
	void BTree<KT, VT, KEY_COMPARE>::releaseNodeData(NodeData* data)
	{
	}

}
