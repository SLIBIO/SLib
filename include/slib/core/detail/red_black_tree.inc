/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	#define PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE 128
	
	class SLIB_EXPORT RedBlackTreeBaseNode
	{
	public:
		RedBlackTreeBaseNode* parent;
		RedBlackTreeBaseNode* left;
		RedBlackTreeBaseNode* right;
		sl_bool flagRed;
	};
	
	class SLIB_EXPORT _priv_RedBlackTree
	{
	public:
		typedef RedBlackTreeBaseNode Node;
		
		static Node* getPrevious(Node* node) noexcept;
		
		static Node* getNext(Node* node) noexcept;
		
		static Node* getFirst(Node* node) noexcept;
		
		static Node* getLast(Node* node) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static NODE* tryFind(NODE* look, const KEY& key, const KEY_COMPARE& key_compare, int* pCompareResult) noexcept;
		
		static void rebalanceAfterInsert(Node* node, Node** pRoot) noexcept;
		
		template <class NODE>
		static void insertNode(NODE** pRoot, NODE* node, NODE* where, int compare_result) noexcept;
		
		template <class NODE, class KEY_COMPARE>
		static void addNode(NODE** pRoot, NODE* node, const KEY_COMPARE& key_compare) noexcept;
		
		static void removeNode(Node* node, Node** pRoot) noexcept;
		
		template <class NODE>
		static void freeNodes(NODE* node) noexcept;
		
		template <class NODE>
		static NODE* duplicateNode(NODE* other) noexcept;
		
	};
	
	template <class NODE>
	SLIB_INLINE RedBlackTreeNodeBase<NODE>::RedBlackTreeNodeBase()
	 : parent(sl_null), left(sl_null), right(sl_null), flagRed(sl_false)
	 {}
	
	template <class NODE>
	SLIB_INLINE NODE* RedBlackTreeNodeBase<NODE>::getNext() const noexcept
	{
		return reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class NODE>
	SLIB_INLINE NODE* RedBlackTreeNodeBase<NODE>::getPrevious() const noexcept
	{
		return reinterpret_cast<NODE*>(_priv_RedBlackTree::getPrevious(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class NODE>
	SLIB_INLINE NODE* RedBlackTreeNodeBase<NODE>::getFirst() const noexcept
	{
		return reinterpret_cast<NODE*>(_priv_RedBlackTree::getFirst(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class NODE>
	SLIB_INLINE NODE* RedBlackTreeNodeBase<NODE>::getLast() const noexcept
	{
		return reinterpret_cast<NODE*>(_priv_RedBlackTree::getLast(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	SLIB_INLINE RedBlackTreeNode<KT, VT>::RedBlackTreeNode(KEY&& _key, VALUE&& _value) noexcept
	 : key(Forward<KEY>(_key)), value(Forward<VALUE>(_value))
	 {}
	
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	RedBlackTree<KT, VT, KEY_COMPARE, NODE>::RedBlackTree() noexcept
	{
		m_rootNode = sl_null;
		m_count = 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class KEY_COMPARE_ARG>
	RedBlackTree<KT, VT, KEY_COMPARE, NODE>::RedBlackTree(KEY_COMPARE_ARG&& key_compare) noexcept
	 : m_compare(Forward<KEY_COMPARE_ARG>(key_compare))
	{
		m_rootNode = sl_null;
		m_count = 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	RedBlackTree<KT, VT, KEY_COMPARE, NODE>::~RedBlackTree() noexcept
	{
		if (m_rootNode) {
			_priv_RedBlackTree::freeNodes(m_rootNode);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	RedBlackTree<KT, VT, KEY_COMPARE, NODE>::RedBlackTree(RedBlackTree<KT, VT, KEY_COMPARE, NODE>&& other) noexcept
	: m_compare(Move(other.m_compare))
	{
		m_rootNode = other.m_rootNode;
		other.m_rootNode = sl_null;
		m_count = other.m_count;
		other.m_count = 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	RedBlackTree<KT, VT, KEY_COMPARE, NODE>& RedBlackTree<KT, VT, KEY_COMPARE, NODE>::operator=(RedBlackTree<KT, VT, KEY_COMPARE, NODE>&& other) noexcept
	{
		if (m_rootNode) {
			_priv_RedBlackTree::freeNodes(m_rootNode);
		}
		m_rootNode = other.m_rootNode;
		other.m_rootNode = sl_null;
		m_count = other.m_count;
		other.m_count = 0;
		m_compare = Move(other.m_compare);
		return *this;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE sl_size RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getCount() const noexcept
	{
		return m_count;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::isEmpty() const noexcept
	{
		return m_count == 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::isNotEmpty() const noexcept
	{
		return m_count > 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getRootNode() const noexcept
	{
		return m_rootNode;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getFirstNode() const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			return reinterpret_cast<NODE*>(_priv_RedBlackTree::getFirst(reinterpret_cast<RedBlackTreeBaseNode*>(node)));
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getLastNode() const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			return reinterpret_cast<NODE*>(_priv_RedBlackTree::getLast(reinterpret_cast<RedBlackTreeBaseNode*>(node)));
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::tryFind(const KT& key, int* pCompareResult) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			return _priv_RedBlackTree::tryFind(node, key, m_compare, pCompareResult);
		} else {
			return sl_null;
		}
	}

	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::find(const KT& key) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				return node;
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	void RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getBounds(const KT& key, NODE** pLessEqual, NODE** pGreaterEqual) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				if (pLessEqual) {
					*pLessEqual = node;
				}
				if (pGreaterEqual) {
					*pGreaterEqual = node;
				}
			} else if (compare_result > 0) {
				if (pLessEqual) {
					*pLessEqual = reinterpret_cast<NODE*>(_priv_RedBlackTree::getPrevious(reinterpret_cast<RedBlackTreeBaseNode*>(node)));
				}
				if (pGreaterEqual) {
					*pGreaterEqual = node;
				}
			} else {
				if (pLessEqual) {
					*pLessEqual = node;
				}
				if (pGreaterEqual) {
					*pGreaterEqual = reinterpret_cast<NODE*>(_priv_RedBlackTree::getNext(reinterpret_cast<RedBlackTreeBaseNode*>(node)));
				}
			}
		} else {
			if (pLessEqual) {
				*pLessEqual = sl_null;
			}
			if (pGreaterEqual) {
				*pGreaterEqual = sl_null;
			}
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getEqualRange(const KT& key, NODE** pStart, NODE** pEnd) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			NODE* mid = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				if (pStart) {
					NODE* start = mid;
					node = mid;
					while ((node = node->getPrevious())) {
						if (m_compare(node->key, key) == 0) {
							start = node;
						} else {
							break;
						}
					}
					*pStart = start;
				}
				if (pEnd) {
					NODE* end = mid;
					node = mid;
					while ((node = node->getNext())) {
						if (m_compare(node->key, key) == 0) {
							end = node;
						} else {
							break;
						}
					}
					*pEnd = end;
				}
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class VALUE, class VALUE_EQUALS>
	NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			NODE* mid = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				while ((node = node->getPrevious())) {
					if (m_compare(node->key, key) == 0) {
						if (value_equals(node->value, value)) {
							return node;
						}
					} else {
						break;
					}
				}
				node = mid;
				while ((node = node->getNext())) {
					if (m_compare(node->key, key) == 0) {
						if (value_equals(node->value, value)) {
							return node;
						}
					} else {
						break;
					}
				}
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE VT* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getItemPointer(const KT& key) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				return &(node->value);
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class VALUE, class VALUE_EQUALS>
	SLIB_INLINE VT* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* node = findKeyAndValue(key, value, value_equals);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::get(const KT& key, VT* value) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				if (value) {
					*value = node->value;
				}
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE VT RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getValue(const KT& key) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				return node->value;
			}
		}
		return VT();
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE VT RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getValue(const KT& key, const VT& def) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				return node->value;
			}
		}
		return def;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	List<VT> RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getValues(const KT& key) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				List<VT> ret(1, 1, node->value);
				NODE* mid = node;
				while ((node = node->getPrevious())) {
					if (m_compare(node->key, key) == 0) {
						ret.add_NoLock(node->value);
					} else {
						break;
					}
				}
				node = mid;
				while ((node = node->getNext())) {
					if (m_compare(node->key, key) == 0) {
						ret.add_NoLock(node->value);
					} else {
						break;
					}
				}
				return ret;
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> RedBlackTree<KT, VT, KEY_COMPARE, NODE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				List<VALUE> ret;
				if (value_equals(node->value, value)) {
					ret.add_NoLock(node->value);
				}
				NODE* mid = node;
				while ((node = node->getPrevious())) {
					if (m_compare(node->key, key) == 0) {
						if (value_equals(node->value, value)) {
							ret.add_NoLock(node->value);
						}
					} else {
						break;
					}
				}
				node = mid;
				while ((node = node->getNext())) {
					if (m_compare(node->key, key) == 0) {
						if (value_equals(node->value, value)) {
							ret.add_NoLock(node->value);
						}
					} else {
						break;
					}
				}
				return ret;
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE void RedBlackTree<KT, VT, KEY_COMPARE, NODE>::insertNode(NODE* node, NODE* where, int compare_result) noexcept
	{
		m_count++;
		if (where) {
			_priv_RedBlackTree::insertNode(&m_rootNode, node, where, compare_result);
		} else {
			m_rootNode = node;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE void RedBlackTree<KT, VT, KEY_COMPARE, NODE>::insertNode(NODE* node) noexcept
	{
		m_count++;
		_priv_RedBlackTree::addNode(&m_rootNode, node, m_compare);
	}

	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class KEY, class VALUE>
	NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		NODE* root = m_rootNode;
		if (root) {
			int compare_result;
			NODE* where = _priv_RedBlackTree::tryFind(m_rootNode, key, m_compare, &compare_result);
			if (compare_result == 0) {
				where->value = Forward<VALUE>(value);
				if (isInsertion) {
					*isInsertion = sl_false;
				}
				return where;
			}
			NODE* node = new NODE(Forward<KEY>(key), Forward<VALUE>(value));
			if (node) {
				_priv_RedBlackTree::insertNode(&m_rootNode, node, where, compare_result);
				m_count++;
				if (isInsertion) {
					*isInsertion = sl_true;
				}
				return node;
			}
		} else {
			NODE* node = new NODE(Forward<KEY>(key), Forward<VALUE>(value));
			if (node) {
				m_rootNode = node;
				m_count++;
				if (isInsertion) {
					*isInsertion = sl_true;
				}
				return node;
			}
		}
		if (isInsertion) {
			*isInsertion = sl_false;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class KEY, class VALUE>
	NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::replace(const KEY& key, VALUE&& value) noexcept
	{
		NODE* root = m_rootNode;
		if (root) {
			int compare_result;
			NODE* where = _priv_RedBlackTree::tryFind(m_rootNode, key, m_compare, &compare_result);
			if (compare_result == 0) {
				where->value = Forward<VALUE>(value);
				return where;
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class KEY, class... VALUE_ARGS>
	NODE* RedBlackTree<KT, VT, KEY_COMPARE, NODE>::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		NODE* node = new NODE(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		if (node) {
			m_count++;
			NODE* root = m_rootNode;
			if (root) {
				_priv_RedBlackTree::addNode(&m_rootNode, node, m_compare);
			} else {
				m_rootNode = node;
			}
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class KEY, class... VALUE_ARGS>
	MapEmplaceReturn<NODE> RedBlackTree<KT, VT, KEY_COMPARE, NODE>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		NODE* root = m_rootNode;
		if (root) {
			int compare_result;
			NODE* where = _priv_RedBlackTree::tryFind(m_rootNode, key, m_compare, &compare_result);
			if (compare_result == 0) {
				return MapEmplaceReturn<NODE>(sl_false, where);
			}
			NODE* node = new NODE(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			if (node) {
				_priv_RedBlackTree::insertNode(&m_rootNode, node, where, compare_result);
				m_count++;
				return MapEmplaceReturn<NODE>(sl_true, node);
			}
		} else {
			NODE* node = new NODE(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
			if (node) {
				m_rootNode = node;
				m_count++;
				return MapEmplaceReturn<NODE>(sl_true, node);
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE void RedBlackTree<KT, VT, KEY_COMPARE, NODE>::removeNode(NODE* node) noexcept
	{
		m_count--;
		_priv_RedBlackTree::removeNode(reinterpret_cast<RedBlackTreeBaseNode*>(node), reinterpret_cast<RedBlackTreeBaseNode**>(&m_rootNode));
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::remove(const KT& key, VT* outValue) noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			int compare_result;
			node = _priv_RedBlackTree::tryFind(node, key, m_compare, &compare_result);
			if (compare_result == 0) {
				if (outValue) {
					*outValue = Move(node->value);
				}
				removeNode(node);
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE, NODE>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		NODE* node = find(key);
		if (node) {
			if (outValues) {
				outValues->add_NoLock(Move(node->value));
			}
			removeNode(node);
			sl_size n = 1;
			while ((node = find(key))) {
				if (outValues) {
					outValues->add_NoLock(Move(node->value));
				}
				removeNode(node);
				n++;
			}
			return n;
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue, const VALUE_EQUALS& value_equals) noexcept
	{
		NODE* node = findKeyAndValue(key, value, value_equals);
		if (node) {
			if (outValue) {
				*outValue = Move(node->value);
			}
			removeNode(node);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE, NODE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals) noexcept
	{
		NODE* node = findKeyAndValue(key, value, value_equals);
		if (node) {
			if (outValues) {
				outValues->add_NoLock(Move(node->value));
			}
			removeNode(node);
			sl_size n = 1;
			while ((node = findKeyAndValue(key, value, value_equals))) {
				if (outValues) {
					outValues->add_NoLock(Move(node->value));
				}
				removeNode(node);
				n++;
			}
			return n;
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE, NODE>::removeAll() noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			_priv_RedBlackTree::freeNodes(node);
			m_rootNode = sl_null;
		}
		sl_size n = m_count;
		m_count = 0;
		return n;
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE, NODE>::copyFrom(const RedBlackTree<KT, VT, KEY_COMPARE, NODE>* other) noexcept
	{
		if (m_rootNode) {
			_priv_RedBlackTree::freeNodes(m_rootNode);
		}
		m_compare = other->m_compare;
		NODE* otherRoot = other->m_rootNode;
		if (!otherRoot) {
			m_rootNode = sl_null;
			m_count = 0;
			return sl_true;
		}
		NODE* newNode = _priv_RedBlackTree::duplicateNode(otherRoot);
		if (newNode) {
			m_rootNode = newNode;
			m_count = other->m_count;
			return sl_true;
		} else {
			m_rootNode = sl_null;
			m_count = 0;
			return sl_false;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NodePosition<NODE> RedBlackTree<KT, VT, KEY_COMPARE, NODE>::begin() const noexcept
	{
		NODE* node = m_rootNode;
		if (node) {
			return reinterpret_cast<NODE*>(_priv_RedBlackTree::getFirst(reinterpret_cast<RedBlackTreeBaseNode*>(node)));
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE, class NODE>
	SLIB_INLINE NodePosition<NODE> RedBlackTree<KT, VT, KEY_COMPARE, NODE>::end() const noexcept
	{
		return sl_null;
	}
	
	
	template <class NODE, class KEY, class KEY_COMPARE>
	NODE* _priv_RedBlackTree::tryFind(NODE* look, const KEY& key, const KEY_COMPARE& key_compare, int* pCompareResult) noexcept
	{
		int comp;
		for (;;) {
			comp = key_compare(look->key, key);
			if (comp == 0) {
				break;
			} else if (comp > 0) {
				NODE* left = look->left;
				if (left) {
					look = left;
				} else {
					break;
				}
			} else {
				NODE* right = look->right;
				if (right) {
					look = right;
				} else {
					break;
				}
			}
		}
		*pCompareResult = comp;
		return look;
	}
	
	template <class NODE>
	SLIB_INLINE void _priv_RedBlackTree::insertNode(NODE** pRoot, NODE* node, NODE* where, int compare_result) noexcept
	{
		if (compare_result > 0) {
			where->left = node;
		} else {
			where->right = node;
		}
		node->parent = where;
		_priv_RedBlackTree::rebalanceAfterInsert(reinterpret_cast<RedBlackTreeBaseNode*>(node), reinterpret_cast<RedBlackTreeBaseNode**>(pRoot));
	}
	
	template <class NODE, class KEY_COMPARE>
	void _priv_RedBlackTree::addNode(NODE** pRoot, NODE* node, const KEY_COMPARE& key_compare) noexcept
	{
		NODE* look = *pRoot;
		if (look) {
			for (;;) {
				if (key_compare(look->key, node->key) > 0) {
					NODE* left = look->left;
					if (left) {
						look = left;
					} else {
						look->left = node;
						break;
					}
				} else {
					NODE* right = look->right;
					if (right) {
						look = right;
					} else {
						look->right = node;
						break;
					}
				}
			}
			node->parent = look;
			_priv_RedBlackTree::rebalanceAfterInsert(reinterpret_cast<RedBlackTreeBaseNode*>(node), reinterpret_cast<RedBlackTreeBaseNode**>(pRoot));
		} else {
			*pRoot = node;
		}
	}
	
	template <class NODE>
	void _priv_RedBlackTree::freeNodes(NODE* node) noexcept
	{
		NODE* stack[PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE];
		sl_size nStack = 0;
		for(;;) {
			NODE* left = node->left;
			NODE* right = node->right;
			delete node;
			if (left) {
				if (right) {
					node = left;
					SLIB_ASSERT(nStack < PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE);
					stack[nStack] = right;
					nStack++;
				} else {
					node = left;
				}
			} else {
				if (right) {
					node = right;
				} else {
					if (nStack > 0) {
						node = stack[nStack-1];
						nStack--;
					} else {
						break;
					}
				}
			}
		}
	}
	
	template <class NODE>
	NODE* _priv_RedBlackTree::duplicateNode(NODE* nodeSource) noexcept
	{
		NODE* nodeTarget = new NODE(nodeSource->key, nodeSource->value);
		if (!nodeTarget) {
			return sl_null;
		}
		
		NODE* nodeTargetRoot = nodeTarget;
		NODE* stackSource[PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE];
		NODE* stackTarget[PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE];
		sl_size nStack = 0;
		
		for(;;) {
			NODE* leftSource = nodeSource->left;
			NODE* leftTarget;
			if (leftSource) {
				leftTarget = new NODE(leftSource->key, leftSource->value);
				if (!leftTarget) {
					_priv_RedBlackTree::freeNodes(nodeTargetRoot);
					return sl_null;
				}
				leftTarget->flagRed = leftSource->flagRed;
				leftTarget->parent = nodeTarget;
				nodeTarget->left = leftTarget;
			} else {
				leftTarget = sl_null;
			}
			
			NODE* rightSource = nodeSource->right;
			NODE* rightTarget;
			if (rightSource) {
				rightTarget = new NODE(rightSource->key, rightSource->value);
				if (!rightTarget) {
					_priv_RedBlackTree::freeNodes(nodeTargetRoot);
					return sl_null;
				}
				rightTarget->flagRed = rightSource->flagRed;
				rightTarget->parent = nodeTarget;
				nodeTarget->right = rightTarget;
			} else {
				rightTarget = sl_null;
			}
			
			if (leftSource) {
				if (rightSource) {
					nodeSource = leftSource;
					nodeTarget = leftTarget;
					SLIB_ASSERT(nStack < PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE);
					stackSource[nStack] = rightSource;
					stackTarget[nStack] = rightTarget;
					nStack++;
				} else {
					nodeSource = leftSource;
					nodeTarget = leftTarget;
				}
			} else {
				if (rightSource) {
					nodeSource = rightSource;
					nodeTarget = rightTarget;
				} else {
					if (nStack > 0) {
						nodeSource = stackSource[nStack-1];
						nodeTarget = stackTarget[nStack-1];
						nStack--;
					} else {
						break;
					}
				}
			}
		}
		
		return nodeTargetRoot;
	}
	
}
