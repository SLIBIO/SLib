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

#include "slib/core/red_black_tree.h"

namespace slib
{
	
	namespace priv
	{
		namespace rb_tree
		{
			
			SLIB_INLINE static RedBlackTreeNode* GetFirst(RedBlackTreeNode* node) noexcept
			{
				typedef RedBlackTreeNode Node;
				for (;;) {
					Node* left = node->left;
					if (!left) {
						break;
					}
					node = left;
				}
				return node;
			}
			
			SLIB_INLINE static RedBlackTreeNode* GetLast(RedBlackTreeNode* node) noexcept
			{
				typedef RedBlackTreeNode Node;
				for (;;) {
					Node* right = node->right;
					if (!right) {
						break;
					}
					node = right;
				}
				return node;
			}
			
			SLIB_INLINE static RedBlackTreeNode* GetPrevious(RedBlackTreeNode* node) noexcept
			{
				typedef RedBlackTreeNode Node;
				Node* left = node->left;
				if (left) {
					return GetLast(left);
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
			
			SLIB_INLINE static RedBlackTreeNode* GetNext(RedBlackTreeNode* node) noexcept
			{
				typedef RedBlackTreeNode Node;
				Node* right = node->right;
				if (right) {
					return GetFirst(right);
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
			
			SLIB_INLINE static void SetLeft(RedBlackTreeNode* node, RedBlackTreeNode* left) noexcept
			{
				node->left = left;
				if (left) {
					left->parent = node;
				}
			}
			
			SLIB_INLINE static void SetRight(RedBlackTreeNode* node, RedBlackTreeNode* right) noexcept
			{
				node->right = right;
				if (right) {
					right->parent = node;
				}
			}
			
			SLIB_INLINE static void ReplaceChild(RedBlackTreeNode* parent, RedBlackTreeNode* oldChild, RedBlackTreeNode* newChild, RedBlackTreeNode** pRoot) noexcept
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
			
			SLIB_INLINE static void RotateLeft(RedBlackTreeNode* node, RedBlackTreeNode* right, RedBlackTreeNode* parent, RedBlackTreeNode** pRoot) noexcept
			{
				SetRight(node, right->left);
				right->left = node;
				node->parent = right;
				ReplaceChild(parent, node, right, pRoot);
			}
			
			SLIB_INLINE static void RotateRight(RedBlackTreeNode* node, RedBlackTreeNode* left, RedBlackTreeNode* parent, RedBlackTreeNode** pRoot) noexcept
			{
				SetLeft(node, left->right);
				left->right = node;
				node->parent = left;
				ReplaceChild(parent, node, left, pRoot);
			}
			
			RedBlackTreeNode* Helper::getPrevious(RedBlackTreeNode* node) noexcept
			{
				return GetPrevious(node);
			}
			
			RedBlackTreeNode* Helper::getNext(RedBlackTreeNode* node) noexcept
			{
				return GetNext(node);
			}
			
			RedBlackTreeNode* Helper::getFirst(RedBlackTreeNode* node) noexcept
			{
				return GetFirst(node);
			}
			
			RedBlackTreeNode* Helper::getLast(RedBlackTreeNode* node) noexcept
			{
				return GetLast(node);
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
			
			void Helper::rebalanceAfterInsert(RedBlackTreeNode* newNode, RedBlackTreeNode** pRoot) noexcept
			{
				typedef RedBlackTreeNode Node;
				
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
											SetRight(parent, current->left);
											current->left = parent;
											parent->parent = current;
											// We don't need to link `current` to `grand_parent`, because comming rotation will update it.
											parent = current;
										}
										// parent is always left child of the grand_parent
										RotateRight(grand_parent, parent, grand_parent->parent, pRoot);
									} else {
										if (current == parent->left) {
											// rotate right
											SetLeft(parent, current->right);
											current->right = parent;
											parent->parent = current;
											// We don't need to link `current` to `grand_parent`, because comming rotation will update it.
											parent = current;
										}
										// parent is always right child of the grand_parent
										RotateLeft(grand_parent, parent, grand_parent->parent, pRoot);
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
			
			void Helper::removeNode(RedBlackTreeNode* node, RedBlackTreeNode** pRoot) noexcept
			{
				typedef RedBlackTreeNode Node;
				
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
						ReplaceChild(parent, node, successor, pRoot);
						SetLeft(successor, left);
						
						if (right != successor) {
							// so that, successor->parent != node
							SetRight(successor, right);
							// successor is always left child of the parent
							SetLeft(successor_parent, successor_right);
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
						ReplaceChild(parent, node, right, pRoot);
						right->flagRed = sl_false;
						return;
					}
				} else {
					if (left) {
						// In this case, `node` is BLACK and `left` is RED, because the other child `right` is NIL.
						SLIB_ASSERT(left->flagRed);
						ReplaceChild(parent, node, left, pRoot);
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
				// `node` is always NIL, here
				node = sl_null;
				
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
							RotateLeft(parent, sibling, parent->parent, pRoot);
							sibling = parent->right; // sibling's left child
						} else {
							RotateRight(parent, sibling, parent->parent, pRoot);
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
								RotateRight(sibling, sibling_left, parent, pRoot);
								sibling = sibling_left;
								sibling_right = sibling->right;
							}
							if (sibling_right) {
								sibling_right->flagRed = sl_false;
							}
							// Parent has always two non-leaf children
							SLIB_ASSERT(parent->right);
							RotateLeft(parent, parent->right, parent->parent, pRoot);
						} else {
							if(!sibling_left_flagRed){
								// sibling's right is always RED, because of above checking  (!sibling_left_flagRed && !sibling_right_flagRed)
								sibling->flagRed = sl_true;
								sibling_right->flagRed = sl_false;
								RotateLeft(sibling, sibling_right, parent, pRoot);
								sibling = sibling_right;
								sibling_left = sibling->left;
							}
							if (sibling_left) {
								sibling_left->flagRed = sl_false;
							}
							// Parent has always two non-leaf children
							SLIB_ASSERT(parent->left);
							RotateRight(parent, parent->left, parent->parent, pRoot);
						}
						sibling->flagRed = parent->flagRed;
						parent->flagRed = sl_false;
						break;
					}
					
				}
				
			}
			
		}
	}
	
}
