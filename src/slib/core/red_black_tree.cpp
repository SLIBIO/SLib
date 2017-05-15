/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/red_black_tree.h"

namespace slib
{
	
	SLIB_INLINE RedBlackTreeBaseNode* _priv_RedBlackTree_getFirst(RedBlackTreeBaseNode* node) noexcept
	{
		typedef RedBlackTreeBaseNode Node;
		for (;;) {
			Node* left = node->left;
			if (!left) {
				break;
			}
			node = left;
		}
		return node;
	}
	
	SLIB_INLINE RedBlackTreeBaseNode* _priv_RedBlackTree_getLast(RedBlackTreeBaseNode* node) noexcept
	{
		typedef RedBlackTreeBaseNode Node;
		for (;;) {
			Node* right = node->right;
			if (!right) {
				break;
			}
			node = right;
		}
		return node;
	}
	
	SLIB_INLINE RedBlackTreeBaseNode* _priv_RedBlackTree_getPrevious(RedBlackTreeBaseNode* node) noexcept
	{
		typedef RedBlackTreeBaseNode Node;
		Node* left = node->left;
		if (left) {
			return _priv_RedBlackTree_getLast(left);
		}
		for (;;) {
			Node* parent = node->parent;
			if (!parent) {
				break;
			}
			if (parent->right == node) {
				return parent;
			}
			node = parent;
		}
		return sl_null;
	}
	
	SLIB_INLINE RedBlackTreeBaseNode* _priv_RedBlackTree_getNext(RedBlackTreeBaseNode* node) noexcept
	{
		typedef RedBlackTreeBaseNode Node;
		Node* right = node->right;
		if (right) {
			return _priv_RedBlackTree_getFirst(right);
		}
		for (;;) {
			Node* parent = node->parent;
			if (!parent) {
				break;
			}
			if (parent->left == node) {
				return parent;
			}
			node = parent;
		}
		return sl_null;
	}
	
	SLIB_INLINE void _priv_RedBlackTree_setLeft(RedBlackTreeBaseNode* node, RedBlackTreeBaseNode* left) noexcept
	{
		node->left = left;
		if (left) {
			left->parent = node;
		}
	}
	
	SLIB_INLINE void _priv_RedBlackTree_setRight(RedBlackTreeBaseNode* node, RedBlackTreeBaseNode* right) noexcept
	{
		node->right = right;
		if (right) {
			right->parent = node;
		}
	}
	
	SLIB_INLINE void _priv_RedBlackTree_replaceChild(RedBlackTreeBaseNode* parent, RedBlackTreeBaseNode* oldChild, RedBlackTreeBaseNode* newChild, RedBlackTreeBaseNode** pRoot) noexcept
	{
		newChild->parent = parent;
		if (parent) {
			if (parent->left == oldChild) {
				parent->left = newChild;
			} else {
				parent->right = newChild;
			}
		} else {
			*pRoot = newChild;
		}
	}
	
	SLIB_INLINE void _priv_RedBlackTree_removeChild(RedBlackTreeBaseNode* parent, RedBlackTreeBaseNode* child, RedBlackTreeBaseNode** pRoot) noexcept
	{
		if (parent) {
			if (parent->left == child) {
				parent->left = sl_null;
			} else {
				parent->right = sl_null;
			}
		} else {
			*pRoot = sl_null;
		}
	}

	SLIB_INLINE void _priv_RedBlackTree_rotateLeft(RedBlackTreeBaseNode* node, RedBlackTreeBaseNode* right, RedBlackTreeBaseNode* parent, RedBlackTreeBaseNode** pRoot) noexcept
	{
		_priv_RedBlackTree_setRight(node, right->left);
		right->left = node;
		node->parent = right;
		_priv_RedBlackTree_replaceChild(parent, node, right, pRoot);
	}
	
	SLIB_INLINE void _priv_RedBlackTree_rotateRight(RedBlackTreeBaseNode* node, RedBlackTreeBaseNode* left, RedBlackTreeBaseNode* parent, RedBlackTreeBaseNode** pRoot) noexcept
	{
		_priv_RedBlackTree_setLeft(node, left->right);
		left->right = node;
		node->parent = left;
		_priv_RedBlackTree_replaceChild(parent, node, left, pRoot);
	}
	
	RedBlackTreeBaseNode* _priv_RedBlackTree::getPrevious(RedBlackTreeBaseNode* node) noexcept
	{
		return _priv_RedBlackTree_getPrevious(node);
	}
	
	RedBlackTreeBaseNode* _priv_RedBlackTree::getNext(RedBlackTreeBaseNode* node) noexcept
	{
		return _priv_RedBlackTree_getNext(node);
	}
	
	RedBlackTreeBaseNode* _priv_RedBlackTree::getFirst(RedBlackTreeBaseNode* node) noexcept
	{
		return _priv_RedBlackTree_getFirst(node);
	}
	
	RedBlackTreeBaseNode* _priv_RedBlackTree::getLast(RedBlackTreeBaseNode* node) noexcept
	{
		return _priv_RedBlackTree_getLast(node);
	}
	
	
	/*
				Properties of Red-Black Tree
		
	 https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Properties
	 
	 1. Each node is either red or black.
	 2. The root is black.
	 3. All leaves (NIL) are black.
	 4. If a node is red, then both its children are black.
	 5. Every path from a given node to any of its descendant NIL nodes contains the same number of black nodes.
	 
	 */

	void _priv_RedBlackTree::rebalanceAfterInsert(RedBlackTreeBaseNode* newNode, RedBlackTreeBaseNode** pRoot) noexcept
	{
		
		newNode->flagRed = sl_true;
		
		Node* current = newNode;
		
		for (;;) {
			
			Node* parent = current->parent;
			
			if (parent) {
				
				if (parent->flagRed) {
					
					Node* grand_parent = parent->parent;
					
					if (grand_parent) {
						
						grand_parent->flagRed = sl_true;
						
						Node* uncle = grand_parent->left;
						if (uncle == parent) {
							uncle = grand_parent->right;
						}
						
						if (uncle && uncle->flagRed) {
							
							// the parent and the uncle are red
							parent->flagRed = sl_false;
							uncle->flagRed = sl_false;
							current = grand_parent;
							
						} else {
							
							if (parent == grand_parent->left) {
								if (current == parent->right) {
									// rotate left
									_priv_RedBlackTree_setRight(parent, current->left);
									current->left = parent;
									parent->parent = current;
									// We don't need to link `current` to `grand_parent`, because comming rotation will update it.
									parent = current;
								}
								// parent is always left child of the grand_parent
								_priv_RedBlackTree_rotateRight(grand_parent, parent, grand_parent->parent, pRoot);
							} else {
								if (current == parent->left) {
									// rotate right
									_priv_RedBlackTree_setLeft(parent, current->right);
									current->right = parent;
									parent->parent = current;
									// We don't need to link `current` to `grand_parent`, because comming rotation will update it.
									parent = current;
								}
								// parent is always right child of the grand_parent
								_priv_RedBlackTree_rotateLeft(grand_parent, parent, grand_parent->parent, pRoot);
							}
							
							parent->flagRed = sl_false;
							
							break;
						}
						
					} else {
						// The root is always black. This is the error situation.
						SLIB_ASSERT(true);
						parent->flagRed = sl_false;
						break;
					}
				} else {
					// The current node's parent is black. So, now the tree is valid state.
					break;
				}
			} else {
				// The current node is at the root of the tree
				current->flagRed = sl_false;
				break;
			}
		}
		
	}
		
	void _priv_RedBlackTree::rebalanceAfterRemove(RedBlackTreeBaseNode* node, RedBlackTreeBaseNode** pRoot) noexcept
	{
		sl_bool flagRed = node->flagRed;
		Node* parent = node->parent;
		Node* left = node->left;
		Node* right = node->right;
		
		if (right) {
			if (left) {
				// in this case, we replace deleting node with the successor (the minimum element in its right subtree)
				Node* successor = getFirst(right);
				Node* successor_parent = successor->parent;
				// successor's left is always null. and also successor's right is NIL or RED
				SLIB_ASSERT(!(successor->left));
				Node* successor_right = successor->right;
				sl_bool successor_flagRed = successor->flagRed;
				
				// copy node values to the successor
				successor->flagRed = flagRed;
				_priv_RedBlackTree_replaceChild(parent, node, successor, pRoot);
				_priv_RedBlackTree_setLeft(successor, left);
				
				if (right != successor) {
					// so that, successor->parent != node
					_priv_RedBlackTree_setRight(successor, right);
					// successor is always left child of the parent
					_priv_RedBlackTree_setLeft(successor_parent, successor_right);
					parent = successor_parent;
				} else {
					parent = successor;
				}

				if (successor_flagRed) {
					return;
				}
				
				if (successor_right) {
					// In this case, successor_right is 'RED', because successor's left is always NIL
					SLIB_ASSERT(successor_right->flagRed);
					successor_right->flagRed = sl_false;
					return;
				}
			} else {
				// In this case, `node` is BLACK and `right` is RED, because the other child `left` is NIL.
				SLIB_ASSERT(right->flagRed);
				_priv_RedBlackTree_replaceChild(parent, node, right, pRoot);
				right->flagRed = sl_false;
				return;
			}
		} else {
			if (left) {
				// In this case, `node` is BLACK and `left` is RED, because the other child `right` is NIL.
				SLIB_ASSERT(left->flagRed);
				_priv_RedBlackTree_replaceChild(parent, node, left, pRoot);
				left->flagRed = sl_false;
				return;
			} else {
				// Deleting Leaf node
				if (parent) {
					if (parent->left == node) {
						parent->left = sl_null;
					} else {
						parent->right = sl_null;
					}
					if (flagRed) {
						return;
					}
				} else {
					// deleting Root node
					*pRoot = sl_null;
					return;
				}
			}
		}
		
		// rebalance red-black tree.
		// `node` is always BLACK, here
		SLIB_ASSERT(!(node && node->flagRed));
		
		for (;;) {
			
			Node* sibling;
			sl_bool flagLeft;
			if (parent->left == node) {
				flagLeft = sl_true;
				sibling = parent->right;
			} else {
				flagLeft = sl_false;
				sibling = parent->left;
			}
			
			// `sibling` is not null, because `node` was BLACK
			SLIB_ASSERT(sibling);
			if (sibling->flagRed) {
				// In this case, `sibling' has two non-NIL black children, because `node` was BLACK
				parent->flagRed = sl_true;
				sibling->flagRed = sl_false;
				if (flagLeft) {
					_priv_RedBlackTree_rotateLeft(parent, sibling, parent->parent, pRoot);
					sibling = parent->right; // sibling's left child
				} else {
					_priv_RedBlackTree_rotateRight(parent, sibling, parent->parent, pRoot);
					sibling = parent->left; // sibling's right child
				}
				SLIB_ASSERT(sibling);
				// Here, sibling is always BLACK
				SLIB_ASSERT(!(sibling->flagRed));
			}
			Node* sibling_left = sibling->left;
			Node* sibling_right = sibling->right;
			sl_bool sibling_left_flagRed = sibling_left && sibling_left->flagRed;
			sl_bool sibling_right_flagRed = sibling_right && sibling_right->flagRed;
			if (!sibling_left_flagRed && !sibling_right_flagRed) {
				sibling->flagRed = sl_true;
				if (parent->flagRed) {
					parent->flagRed = sl_false;
					break;
				}
				node = parent;
				parent = node->parent;
				if (!parent) {
					break;
				}
			} else {
				// Here, sibling is always BLACK. See above.
				if (flagLeft) {
					if (!sibling_right_flagRed){
						// sibling's left is always RED, because of above checking  (!sibling_left_flagRed && !sibling_right_flagRed)
						sibling->flagRed = sl_true;
						sibling_left->flagRed = sl_false;
						_priv_RedBlackTree_rotateRight(sibling, sibling_left, parent, pRoot);
						sibling = sibling_left;
						sibling_right = sibling->right;
					}
					if (sibling_right) {
						sibling_right->flagRed = sl_false;
					}
					// Parent has always two non-leaf children
					SLIB_ASSERT(parent->right);
					_priv_RedBlackTree_rotateLeft(parent, parent->right, parent->parent, pRoot);
				} else {
					if(!sibling_left_flagRed){
						// sibling's right is always RED, because of above checking  (!sibling_left_flagRed && !sibling_right_flagRed)
						sibling->flagRed = sl_true;
						sibling_right->flagRed = sl_false;
						_priv_RedBlackTree_rotateLeft(sibling, sibling_right, parent, pRoot);
						sibling = sibling_right;
						sibling_left = sibling->left;
					}
					if (sibling_left) {
						sibling_left->flagRed = sl_false;
					}
					// Parent has always two non-leaf children
					SLIB_ASSERT(parent->left);
					_priv_RedBlackTree_rotateRight(parent, parent->left, parent->parent, pRoot);
				}
				sibling->flagRed = parent->flagRed;
				parent->flagRed = sl_false;
				break;
			}
			
 		}
		
	}
	
}
