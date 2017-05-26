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
		
		static void rebalanceAfterInsert(Node* node, Node** pRoot) noexcept;
		
		static void removeNode(Node* node, Node** pRoot) noexcept;
		
	};
	
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	SLIB_INLINE RedBlackTreeNode<KT, VT>::RedBlackTreeNode(KEY&& _key, VALUE&& _value) noexcept
	 : parent(sl_null), left(sl_null), right(sl_null), flagRed(sl_false), data(Forward<KEY>(_key), Forward<VALUE>(_value))
	{
	}
	
	template <class KT, class VT>
	SLIB_INLINE RedBlackTreeNode<KT, VT>* RedBlackTreeNode<KT, VT>::getNext() const noexcept
	{
		return reinterpret_cast<Node*>(_priv_RedBlackTree::getNext(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class KT, class VT>
	SLIB_INLINE RedBlackTreeNode<KT, VT>* RedBlackTreeNode<KT, VT>::getPrevious() const noexcept
	{
		return reinterpret_cast<Node*>(_priv_RedBlackTree::getPrevious(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class KT, class VT>
	SLIB_INLINE RedBlackTreeNode<KT, VT>* RedBlackTreeNode<KT, VT>::getFirst() const noexcept
	{
		return reinterpret_cast<Node*>(_priv_RedBlackTree::getFirst(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}
	
	template <class KT, class VT>
	SLIB_INLINE RedBlackTreeNode<KT, VT>* RedBlackTreeNode<KT, VT>::getLast() const noexcept
	{
		return reinterpret_cast<Node*>(_priv_RedBlackTree::getLast(const_cast<RedBlackTreeBaseNode*>(reinterpret_cast<RedBlackTreeBaseNode const*>(this))));
	}

	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTree<KT, VT, KEY_COMPARE>::RedBlackTree() noexcept
	{
		m_rootNode = sl_null;
		m_count = 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY_COMPARE_ARG>
	RedBlackTree<KT, VT, KEY_COMPARE>::RedBlackTree(KEY_COMPARE_ARG&& key_compare) noexcept
	 : m_compare(Forward<KEY_COMPARE_ARG>(key_compare))
	{
		m_rootNode = sl_null;
		m_count = 0;
	}
	
	template <class KT, class VT>
	void _priv_RedBlackTree_freeNode(RedBlackTreeNode<KT, VT>* node) noexcept;
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTree<KT, VT, KEY_COMPARE>::~RedBlackTree() noexcept
	{
		Node* node = m_rootNode;
		if (node) {
			_priv_RedBlackTree_freeNode(node);
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTree<KT, VT, KEY_COMPARE>::RedBlackTree(RedBlackTree<KT, VT, KEY_COMPARE>&& other) noexcept
	 : m_compare(Move(other.m_compare))
	{
		m_rootNode = other.m_rootNode;
		other.m_rootNode = sl_null;
		m_count = other.m_count;
		other.m_count = 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTree<KT, VT, KEY_COMPARE>& RedBlackTree<KT, VT, KEY_COMPARE>::operator=(RedBlackTree<KT, VT, KEY_COMPARE>&& other) noexcept
	{
		Node* node = m_rootNode;
		if (node) {
			_priv_RedBlackTree_freeNode(node);
		}
		m_rootNode = other.m_rootNode;
		other.m_rootNode = sl_null;
		m_count = other.m_count;
		other.m_count = 0;
		m_compare = Move(other.m_compare);
		return *this;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE>::getCount() const noexcept
	{
		return m_count;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::find(const KT& key, RedBlackTreeNode<KT, VT>** ppNode) const noexcept
	{
		Node* look = m_rootNode;
		if (!look) {
			if (ppNode) {
				*ppNode = sl_null;
			}
			return sl_false;
		}
		for (;;) {
			int comp = m_compare(key, look->data.key);
			if (comp == 0) {
				if (ppNode) {
					*ppNode = look;
				}
				return sl_true;
			} else if (comp < 0) {
				Node* left = look->left;
				if (left) {
					look = left;
				} else {
					if (ppNode) {
						*ppNode = look->getPrevious();
					}
					break;
				}
			} else {
				Node* right = look->right;
				if (right) {
					look = right;
				} else {
					if (ppNode) {
						*ppNode = look;
					}
					break;
				}
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* RedBlackTree<KT, VT, KEY_COMPARE>::find(const KT& key) const noexcept
	{
		Node* node;
		if (find(key, &node)) {
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals) const noexcept
	{
		Node* node;
		if (find(key, &node)) {
			if (value_equals(node->data.value, value)) {
				if (ppNode) {
					*ppNode = node;
				}
				return sl_true;
			}
			Node* nodeMid = node;
			while ((node = node->getPrevious())) {
				if (m_compare(node->data.key, key) == 0) {
					if (value_equals(node->data.value, value)) {
						if (ppNode) {
							*ppNode = node;
						}
						return sl_true;
					}
				} else {
					break;
				}
			}
			node = nodeMid;
			while ((node = node->getNext())) {
				if (m_compare(node->data.key, key) == 0) {
					if (value_equals(node->data.value, value)) {
						if (ppNode) {
							*ppNode = node;
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
	template <class VALUE, class VALUE_EQUALS>
	RedBlackTreeNode<KT, VT>* RedBlackTree<KT, VT, KEY_COMPARE>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Node* node;
		if (findKeyAndValue(key, value, &node, value_equals)) {
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	SLIB_INLINE RedBlackTreeNode<KT, VT>* RedBlackTree<KT, VT, KEY_COMPARE>::getRootNode() const noexcept
	{
		return m_rootNode;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* RedBlackTree<KT, VT, KEY_COMPARE>::getFirstNode() const noexcept
	{
		Node* node = m_rootNode;
		if (node) {
			return node->getFirst();
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	RedBlackTreeNode<KT, VT>* RedBlackTree<KT, VT, KEY_COMPARE>::getLastNode() const noexcept
	{
		Node* node = m_rootNode;
		if (node) {
			return node->getLast();
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::get(const KT& key, VT* value) const noexcept
	{
		Node* node;
		if (find(key, &node)) {
			if (value) {
				*value = node->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT* RedBlackTree<KT, VT, KEY_COMPARE>::getItemPointer(const KT& key) const noexcept
	{
		Node* node;
		if (find(key, &node)) {
			return &(node->data.value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	VT* RedBlackTree<KT, VT, KEY_COMPARE>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Node* node;
		if (findKeyAndValue(key, value, &node, value_equals)) {
			return &(node->data.value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> RedBlackTree<KT, VT, KEY_COMPARE>::getValues(const KT& key) const noexcept
	{
		Node* node;
		if (find(key, &node)) {
			List<VT> ret(1, 1, node->data.value);
			Node* nodeMid = node;
			while ((node = node->getPrevious())) {
				if (m_compare(node->data.key, key) == 0) {
					ret.add_NoLock(node->data.value);
				} else {
					break;
				}
			}
			node = nodeMid;
			while ((node = node->getNext())) {
				if (m_compare(node->data.key, key) == 0) {
					ret.add_NoLock(node->data.value);
				} else {
					break;
				}
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> RedBlackTree<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Node* node;
		if (find(key, &node)) {
			List<VT> ret;
			if (value_equals(node->data.value, value)) {
				ret.add_NoLock(node->data.value);
			}
			Node* nodeMid = node;
			while ((node = node->getPrevious())) {
				if (m_compare(node->data.key, key) == 0) {
					if (value_equals(node->data.value, value)) {
						ret.add_NoLock(node->data.value);
					}
				} else {
					break;
				}
			}
			node = nodeMid;
			while ((node = node->getNext())) {
				if (m_compare(node->data.key, key) == 0) {
					if (value_equals(node->data.value, value)) {
						ret.add_NoLock(node->data.value);
					}
				} else {
					break;
				}
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::put(KEY&& key, VALUE&& value, MapPutMode mode, RedBlackTreeNode<KT, VT>** ppNode) noexcept
	{
		Node* node;
		if (find(key, &node)) {
			if (mode != MapPutMode::AddAlways) {
				if (ppNode) {
					*ppNode = node;
				}
				if (mode == MapPutMode::AddNew) {
					return sl_false;
				}
				node->data.value = Forward<VALUE>(value);
				return sl_true;
			}
		}
		if (mode == MapPutMode::ReplaceExisting) {
			if (ppNode) {
				*ppNode = sl_null;
			}
			return sl_false;
		}
		node = new Node(Forward<KEY>(key), Forward<VALUE>(value));
		if (ppNode) {
			*ppNode = node;
		}
		if (node) {
			insertNode(node);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class KEY, class VALUE, class VALUE_EQUALS>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::addIfNewKeyAndValue(KEY&& key, VALUE&& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals) noexcept
	{
		Node* node;
		if (findKeyAndValue(key, value, &node, value_equals)) {
			if (ppNode) {
				*ppNode = node;
			}
			return sl_false;
		}
		node = new Node(Forward<KEY>(key), Forward<VALUE>(value));
		if (ppNode) {
			*ppNode = node;
		}
		if (node) {
			insertNode(node);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	void RedBlackTree<KT, VT, KEY_COMPARE>::insertNode(Node* node) noexcept
	{
		Node* root = m_rootNode;
		
		m_count++;
		
		// Init the root when the tree is empty
		if (!root) {
			m_rootNode = node;
			return;
		}
		
		// Insert `node` to the binary tree
		{
			Node* look = root;
			for (;;) {
				int comp = m_compare(node->data.key, look->data.key);
				if (comp < 0) {
					Node* left = look->left;
					if (left) {
						look = left;
					} else {
						look->left = node;
						node->parent = look;
						break;
					}
				} else {
					Node* right = look->right;
					if (right) {
						look = right;
					} else {
						look->right = node;
						node->parent = look;
						break;
					}
				}
			}
		}
		
		_priv_RedBlackTree::rebalanceAfterInsert(reinterpret_cast<RedBlackTreeBaseNode*>(node), reinterpret_cast<RedBlackTreeBaseNode**>(&m_rootNode));
		
	}

	template <class KT, class VT, class KEY_COMPARE>
	void RedBlackTree<KT, VT, KEY_COMPARE>::removeNode(Node* node) noexcept
	{
		m_count--;
		_priv_RedBlackTree::removeNode(reinterpret_cast<RedBlackTreeBaseNode*>(node), reinterpret_cast<RedBlackTreeBaseNode**>(&m_rootNode));
		delete node;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::remove(const KT& key, VT* outValue) noexcept
	{
		Node* node;
		if (find(key, &node)) {
			if (outValue) {
				*outValue = Move(node->data.value);
			}
			removeNode(node);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		Node* node;
		if (find(key, &node)) {
			if (outValues) {
				outValues->add_NoLock(Move(node->data.value));
			}
			removeNode(node);
			sl_size n = 1;
			while (find(key, &node)) {
				if (outValues) {
					outValues->add_NoLock(Move(node->data.value));
				}
				removeNode(node);
				n++;
			}
			return n;
		}
		return 0;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue, const VALUE_EQUALS& value_equals) noexcept
	{
		Node* node;
		if (findKeyAndValue(key, value, &node, value_equals)) {
			if (outValue) {
				*outValue = Move(node->data.value);
			}
			removeNode(node);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class VALUE, class VALUE_EQUALS>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals) noexcept
	{
		Node* node;
		if (findKeyAndValue(key, value, &node, value_equals)) {
			if (outValues) {
				outValues->add_NoLock(Move(node->data.value));
			}
			removeNode(node);
			sl_size n = 1;
			while (findKeyAndValue(key, value, &node, value_equals)) {
				if (outValues) {
					outValues->add_NoLock(Move(node->data.value));
				}
				removeNode(node);
				n++;
			}
			return n;
		}
		return 0;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_size RedBlackTree<KT, VT, KEY_COMPARE>::removeAll() noexcept
	{
		Node* node = m_rootNode;
		if (node) {
			_priv_RedBlackTree_freeNode(node);
			m_rootNode = sl_null;
		}
		sl_size n = m_count;
		m_count = 0;
		return n;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool RedBlackTree<KT, VT, KEY_COMPARE>::copyFrom(const RedBlackTree<KT, VT, KEY_COMPARE>* other) noexcept
	{
		Node* nodeTarget = m_rootNode;
		if (nodeTarget) {
			_priv_RedBlackTree_freeNode(nodeTarget);
		}

		m_compare = other->m_compare;
		
		Node* nodeSource = other->m_rootNode;
		if (!nodeSource) {
			m_rootNode = sl_null;
			m_count = 0;
			return sl_true;
		}
		
		nodeTarget = new Node(nodeSource->data.key, nodeSource->data.value);
		if (!nodeTarget) {
			m_rootNode = sl_null;
			m_count = 0;
			return sl_false;
		}

		Node* nodeTargetRoot = nodeTarget;
		Node* stackSource[PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE];
		Node* stackTarget[PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE];
		sl_size nStack = 0;
		
		for(;;) {
			Node* leftSource = nodeSource->left;
			Node* leftTarget;
			if (leftSource) {
				leftTarget = new Node(leftSource->data.key, leftSource->data.value);
				if (!leftTarget) {
					_priv_RedBlackTree_freeNode(nodeTargetRoot);
					m_rootNode = sl_null;
					m_count = 0;
					return sl_false;
				}
				leftTarget->flagRed = leftSource->flagRed;
				leftTarget->parent = nodeTarget;
				nodeTarget->left = leftTarget;
			} else {
				leftTarget = sl_null;
			}
			

			Node* rightSource = nodeSource->right;
			Node* rightTarget;
			if (rightSource) {
				rightTarget = new Node(rightSource->data.key, rightSource->data.value);
				if (!rightTarget) {
					_priv_RedBlackTree_freeNode(nodeTargetRoot);
					m_rootNode = sl_null;
					m_count = 0;
					return sl_false;
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
		
		m_rootNode = nodeTargetRoot;
		m_count = other->m_count;
		
		return sl_true;
		
	}
	
	template <class KT, class VT>
	void _priv_RedBlackTree_freeNode(RedBlackTreeNode<KT, VT>* node) noexcept
	{
		typedef RedBlackTreeNode<KT, VT> Node;
		Node* stack[PRIV_SLIB_RED_BLACK_TREE_MAX_DISTANCE];
		sl_size nStack = 0;
		for(;;) {
			Node* left = node->left;
			Node* right = node->right;
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

}
