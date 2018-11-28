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

#ifndef CHECKHEADER_SLIB_CORE_RED_BLACK_TREE
#define CHECKHEADER_SLIB_CORE_RED_BLACK_TREE

#include "definition.h"

#include "list.h"
#include "map_common.h"

namespace slib
{
	
	struct SLIB_EXPORT RedBlackTreeNode
	{
		RedBlackTreeNode* parent;
		RedBlackTreeNode* left;
		RedBlackTreeNode* right;
		sl_bool flagRed;
	};

	class SLIB_EXPORT RedBlackTree
	{
	public:
		template <class NODE>
		static NODE* getPreviousNode(NODE* node) noexcept;
		
		template <class NODE>
		static NODE* getNextNode(NODE* node) noexcept;
		
		template <class NODE>
		static NODE* getFirstNode(NODE* root) noexcept;
		
		template <class NODE>
		static NODE* getLastNode(NODE* root) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static NODE* tryFind(NODE* look, const KEY& key, const KEY_COMPARE& key_compare, int& compare_result) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static sl_bool getEqualRange(NODE* look, const KEY& key, const KEY_COMPARE& key_compare, NODE** pStart = sl_null, NODE** pEnd = sl_null) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static void getNearest(NODE* look, const KEY& key, const KEY_COMPARE& key_compare, NODE** pLessEqual = sl_null, NODE** pGreaterEqual = sl_null) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static NODE* getLowerBound(NODE* look, const KEY& key, const KEY_COMPARE& key_compare) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static NODE* getUpperBound(NODE* look, const KEY& key, const KEY_COMPARE& key_compare) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE>
		static NODE* find(NODE* look, const KEY& key, const KEY_COMPARE& key_compare) noexcept;

		template <class NODE, class KEY, class KEY_COMPARE, class VALUE, class VALUE_EQUALS>
		static NODE* findKeyAndValue(NODE* look, const KEY& key, const KEY_COMPARE& key_compare, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept;

		template <class VT, class NODE, class KEY, class KEY_COMPARE>
		static void getValues(List<VT>& list, NODE* look, const KEY& key, const KEY_COMPARE& key_compare) noexcept;

		template <class VT, class NODE, class KEY, class KEY_COMPARE, class VALUE, class VALUE_EQUALS>
		static void getValuesByKeyAndValue(List<VT>& list, NODE* look, const KEY& key, const KEY_COMPARE& key_compare, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept;
		
		template <class NODE>
		static void insertNode(NODE** pRoot, NODE* node, NODE* where, int compare_result) noexcept;
		
		template <class NODE, class KEY_COMPARE>
		static void addNode(NODE** pRoot, NODE* node, const KEY_COMPARE& key_compare) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE, class VALUE>
		static NODE* put(NODE** pRoot, sl_size& count, KEY&& key, const KEY_COMPARE& key_compare, VALUE&& value, sl_bool* isInsertion) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE, class VALUE>
		static NODE* replace(NODE* root, const KEY& key, const KEY_COMPARE& key_compare, VALUE&& value) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE, class... VALUE_ARGS>
		static NODE* add(NODE** pRoot, sl_size& count, KEY&& key, const KEY_COMPARE& key_compare, VALUE_ARGS&&... value_args) noexcept;

		template <class NODE, class KEY, class KEY_COMPARE, class... VALUE_ARGS>
		static MapEmplaceReturn<NODE> emplace(NODE** pRoot, sl_size& count, KEY&& key, const KEY_COMPARE& key_compare, VALUE_ARGS&&... value_args) noexcept;
		
		template <class NODE>
		static void removeNode(NODE** pRoot, sl_size& count, NODE* node) noexcept;

		template <class NODE>
		static sl_size removeNodes(NODE** pRoot, sl_size& count, NODE* node, sl_size countRemove) noexcept;

		template <class NODE>
		static sl_size removeRange(NODE** pRoot, sl_size& count, NODE* node, NODE* last) noexcept;

		template <class NODE, class KEY, class KEY_COMPARE, class VALUE>
		static sl_bool remove(NODE** pRoot, sl_size& count, const KEY& key, const KEY_COMPARE& key_compare, VALUE* outValue) noexcept;

		template <class NODE, class KEY, class KEY_COMPARE>
		static sl_size removeItems(NODE** pRoot, sl_size& count, const KEY& key, const KEY_COMPARE& key_compare) noexcept;
		
		template <class VT, class NODE, class KEY, class KEY_COMPARE>
		static sl_size removeItemsAndReturnValues(List<VT>& list, NODE** pRoot, sl_size& count, const KEY& key, const KEY_COMPARE& key_compare) noexcept;

		template <class NODE, class KEY, class KEY_COMPARE, class VALUE, class VALUE_EQUALS>
		static sl_bool removeKeyAndValue(NODE** pRoot, sl_size& count, const KEY& key, const KEY_COMPARE& key_compare, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept;
		
		template <class NODE, class KEY, class KEY_COMPARE, class VALUE, class VALUE_EQUALS>
		static sl_size removeItemsByKeyAndValue(NODE** pRoot, sl_size& count, const KEY& key, const KEY_COMPARE& key_compare, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept;
		
		template <class NODE>
		static void freeNodes(NODE* node) noexcept;
		
		template <class NODE>
		static NODE* duplicateNode(NODE* other) noexcept;
		
	};
	
}

#include "detail/red_black_tree.inc"

#endif
