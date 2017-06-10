/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_RED_BLACK_TREE
#define CHECKHEADER_SLIB_CORE_RED_BLACK_TREE

#include "definition.h"

#include "map_common.h"
#include "node_position.h"
#include "compare.h"
#include "list.h"

namespace slib
{
	
	template <class NODE>
	class SLIB_EXPORT RedBlackTreeNodeBase
	{
	public:
		NODE* parent;
		NODE* left;
		NODE* right;
		sl_bool flagRed;
		
	public:
		RedBlackTreeNodeBase();

	public:
		NODE* getNext() const noexcept;
		
		NODE* getPrevious() const noexcept;
		
		NODE* getFirst() const noexcept;
		
		NODE* getLast() const noexcept;
		
	private:
		typedef RedBlackTreeNodeBase Node;
		
	};
	
	template <class KT, class VT>
	class SLIB_EXPORT RedBlackTreeNode : public RedBlackTreeNodeBase< RedBlackTreeNode<KT, VT> >
	{
	public:
		KT key;
		VT value;
		
	public:
		template <class KEY, class VALUE>
		RedBlackTreeNode(KEY&& key, VALUE&& value) noexcept;
		
	};
	
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT>, class NODE = RedBlackTreeNode<KT, VT> >
	class SLIB_EXPORT RedBlackTree
	{
	public:
		typedef NODE Node;
		
	public:
		RedBlackTree() noexcept;

		template <class KEY_COMPARE_ARG>
		RedBlackTree(KEY_COMPARE_ARG&& key_compare) noexcept;
		
		RedBlackTree(const RedBlackTree& other) = delete;
		
		RedBlackTree(RedBlackTree&& other) noexcept;
		
		~RedBlackTree() noexcept;
		
	public:
		RedBlackTree& operator=(const RedBlackTree& other) = delete;
		
		RedBlackTree& operator=(RedBlackTree&& other) noexcept;
	
	public:
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		NODE* getRootNode() const noexcept;
		
		NODE* getFirstNode() const noexcept;
		
		NODE* getLastNode() const noexcept;
		
		NODE* tryFind(const KT& key, int* pCompareResult) const noexcept;

		NODE* find(const KT& key) const noexcept;
		
		void getBounds(const KT& key, NODE** pLessEqual = sl_null, NODE** pGreaterEqual = sl_null) const noexcept;
		
		sl_bool getEqualRange(const KT& key, NODE** pStart = sl_null, NODE** pEnd = sl_null) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		NODE* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		VT* getItemPointer(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		sl_bool get(const KT& key, VT* value = sl_null) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		void insertNode(NODE* node, NODE* where, int compare_result) noexcept;
		
		void insertNode(NODE* node) noexcept;
		
		template <class KEY, class VALUE>
		NODE* put(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		NODE* replace(const KEY& key, VALUE&& value) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		NODE* add(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		MapEmplaceReturn<NODE> emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		void removeNode(NODE* node) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		sl_size removeAll() noexcept;
		
		sl_bool copyFrom(const RedBlackTree<KT, VT, KEY_COMPARE, NODE>* other) noexcept;
		
		// range-based for loop
		NodePosition<NODE> begin() const noexcept;
		
		NodePosition<NODE> end() const noexcept;
		
	private:
		NODE* m_rootNode;
		KEY_COMPARE m_compare;
		
		sl_size m_count;
		
	};
	
}

#include "detail/red_black_tree.inc"

#endif
