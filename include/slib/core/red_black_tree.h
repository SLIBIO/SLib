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

#include "constants.h"
#include "pair.h"
#include "compare.h"
#include "list.h"

namespace slib
{
	
	template <class KT, class VT>
	class SLIB_EXPORT RedBlackTreeNode
	{
	public:
		RedBlackTreeNode<KT, VT>* parent;
		RedBlackTreeNode<KT, VT>* left;
		RedBlackTreeNode<KT, VT>* right;
		sl_bool flagRed;
		
		Pair<KT, VT> data;
		
	public:
		template <class KEY, class VALUE>
		RedBlackTreeNode(KEY&& key, VALUE&& value) noexcept;
		
	public:
		RedBlackTreeNode<KT, VT>* getNext() noexcept;
		
		RedBlackTreeNode<KT, VT>* getPrevious() noexcept;
		
		RedBlackTreeNode<KT, VT>* getFirst() noexcept;
		
		RedBlackTreeNode<KT, VT>* getLast() noexcept;
		
	private:
		typedef RedBlackTreeNode<KT, VT> Node;
				
	};
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT RedBlackTree
	{
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
		
		sl_bool find(const KT& key, RedBlackTreeNode<KT, VT>** ppNode) const noexcept;
		
		RedBlackTreeNode<KT, VT>* find(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool findKeyAndValue(const KT& key, const VALUE& value, RedBlackTreeNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		RedBlackTreeNode<KT, VT>* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		RedBlackTreeNode<KT, VT>* getFirstNode() const noexcept;

		RedBlackTreeNode<KT, VT>* getLastNode() const noexcept;

		sl_bool get(const KT& key, VT* value = sl_null) const noexcept;

		VT* getItemPointer(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, RedBlackTreeNode<KT, VT>** ppNode = sl_null) noexcept;

		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue(KEY&& key, VALUE&& value, RedBlackTreeNode<KT, VT>** ppNode = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		void insertNode(RedBlackTreeNode<KT, VT>* node) noexcept;

		void removeNode(RedBlackTreeNode<KT, VT>* node) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		sl_size removeAll() noexcept;
		
		sl_bool copyFrom(const RedBlackTree<KT, VT, KEY_COMPARE>* other) noexcept;
		
	private:
		typedef RedBlackTreeNode<KT, VT> Node;
		
		Node* m_rootNode;
		KEY_COMPARE m_compare;
		
		sl_size m_count;
		
	};

}

#include "detail/red_black_tree.inc"

#endif
