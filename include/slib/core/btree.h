/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_BTREE
#define CHECKHEADER_SLIB_CORE_BTREE

#include "definition.h"

#include "compare.h"
#include "search.h"
#include "list.h"
#include "new_helper.h"

#define SLIB_BTREE_DEFAULT_ORDER 16

namespace slib
{
	
	class SLIB_EXPORT BTreeNode
	{
	public:
		sl_uint64 position;
		
	public:
		BTreeNode() noexcept;
		
		BTreeNode(sl_null_t) noexcept;
		
		BTreeNode(sl_uint64 position) noexcept;
		
		BTreeNode(const BTreeNode& other) noexcept;
	
	public:
		BTreeNode& operator=(const BTreeNode& other) noexcept;
		
		BTreeNode& operator=(sl_null_t) noexcept;
		
		sl_bool operator==(const BTreeNode& other) const noexcept;
	
		sl_bool operator!=(const BTreeNode& other) const noexcept;
	
		sl_bool isNull() const noexcept;
	
		sl_bool isNotNull() const noexcept;
	
		void setNull() noexcept;
		
		explicit operator sl_bool() const noexcept;

	};
	
	class SLIB_EXPORT BTreePosition
	{
	public:
		BTreeNode node;
		sl_uint32 item;
		
	public:
		BTreePosition() noexcept;
		
		BTreePosition(sl_null_t) noexcept;
		
		BTreePosition(const BTreeNode& node, sl_uint32 item) noexcept;
		
		BTreePosition(const BTreePosition& other) noexcept;
	
	public:
		BTreePosition& operator=(const BTreePosition& other) noexcept;
		
		BTreePosition& operator=(sl_null_t) noexcept;
		
		sl_bool operator==(const BTreePosition& other) const noexcept;

		sl_bool operator!=(const BTreePosition& other) const noexcept;

		sl_bool isNull() const noexcept;
	
		sl_bool isNotNull() const noexcept;
	
		void setNull() noexcept;
		
		explicit operator sl_bool() const noexcept;

	};
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT BTree
	{
	public:
		BTree(sl_uint32 order = SLIB_BTREE_DEFAULT_ORDER);

		BTree(const KEY_COMPARE& compare, sl_uint32 order = SLIB_BTREE_DEFAULT_ORDER);

		virtual ~BTree();
	
	public:
		sl_bool isValid() const noexcept;
	
		sl_uint32 getOrder() const noexcept;
	
		sl_uint32 getMaxLength() const noexcept;
		
		sl_uint64 getCountInNode(const BTreeNode& node) const;
		
		sl_uint64 getCount() const;
		
		sl_bool isEmpty() const;
		
		sl_bool isNotEmpty() const;
		
		sl_bool getAt(const BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
				
		sl_bool moveToFirstInNode(const BTreeNode& node, BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
		
		sl_bool moveToFirst(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
		
		sl_bool moveToLastInNode(const BTreeNode& node, BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
		
		sl_bool moveToLast(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool moveToPrevious(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool moveToNext(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
		
		sl_bool findItemInNode(const BTreeNode& node, const KT& key, sl_uint32& pos, BTreeNode& link, VT* outValue = sl_null, sl_uint32* pCountItemsInNode = sl_null) const;

		sl_bool findInNode(const BTreeNode& node, const KT& key, BTreePosition* pos = sl_null, VT* outValue = sl_null) const;

		sl_bool find(const KT& key, BTreePosition* pos = sl_null, VT* outValue = sl_null) const;
		
		BTreePosition findInsertPositionInNode(const BTreeNode& node, const KT& key) const;

		sl_bool getNearestInNode(const BTreeNode& node, const KT& key, BTreePosition* pLessEqual = sl_null, BTreePosition* pGreaterEqual = sl_null) const;

		sl_bool getNearest(const KT& key, BTreePosition* pLessEqual = sl_null, BTreePosition* pGreaterEqual = sl_null) const;
		
		sl_bool getEqualRangeInNode(const BTreeNode& node, const KT& key, BTreePosition* pLowerBound = sl_null, BTreePosition* pUpperBound = sl_null) const;

		sl_bool getEqualRange(const KT& key, BTreePosition* pLowerBound = sl_null, BTreePosition* pUpperBound = sl_null) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		BTreePosition findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;
	
		sl_bool get(const KT& key, VT* value = sl_null) const;

		List<VT> getValues(const KT& key) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		sl_bool put(const KT& key, const VT& value, BTreePosition* pos = sl_null, sl_bool* isInsertion = sl_null) noexcept;
		
		sl_bool replace(const KT& key, const VT& value, BTreePosition* pos = sl_null) noexcept;
		
		sl_bool add(const KT& key, const VT& value, BTreePosition* pos = sl_null) noexcept;
		
		sl_bool emplace(const KT& key, const VT& value, BTreePosition* pos = sl_null) noexcept;

		sl_bool removeNode(const BTreeNode& node);

		sl_bool removeAt(const BTreePosition& pos);

		sl_bool remove(const KT& key, VT* outValue = sl_null);
		
		sl_size removeItems(const KT& key);

		sl_size removeItems(const KT& key, List<VT>& removed_values);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		sl_size removeAll();

	protected:
		struct NodeData
		{
			sl_uint64 countTotal;
			sl_uint32 countItems;

			BTreeNode linkParent;
			BTreeNode linkFirst;
			KT* keys;
			VT* values;
			BTreeNode* links;
		};

		class NodeDataScope
		{
		public:
			NodeData* data;
			BTree* tree;

		public:
			NodeDataScope(const BTree* tree, const BTreeNode& node);

			~NodeDataScope();

		public:
			NodeData* operator->();

			sl_bool isNull();

			sl_bool isNotNull();

		};

		friend class NodeDataScope;

	private:
		sl_uint32 m_order;
		sl_uint32 m_maxLength;
		sl_uint64 m_totalCount;
		KEY_COMPARE m_compare;
	
	private:
		NodeData* _createNodeData();

		void _freeNodeData(NodeData* data);

		sl_bool _insertItemInNode(const BTreeNode& node, sl_uint32 at, const BTreeNode& after, const KT& key, const VT& value, const BTreeNode& link, BTreePosition* pPosition);

		void _changeTotalCount(const BTreeNode& node, sl_int64 n);

		void _changeParentTotalCount(NodeData* data, sl_int64 n);

		sl_uint64 _getTotalCountInData(NodeData* data) const;

		sl_bool _removeNode(const BTreeNode& node, sl_bool flagUpdateParent);

		// container-specific implementation
	private:
		NodeData* m_rootNode;
	
	protected:
		virtual void init();

		virtual void free();
		
		virtual BTreeNode getRootNode() const;
		
		virtual sl_bool setRootNode(BTreeNode node);
		
		// here, "data" must be newly created node data. this function should manage the memory of "data" parameter
		virtual BTreeNode createNode(NodeData* data);
		
		virtual sl_bool deleteNode(BTreeNode node);
		
		virtual NodeData* readNodeData(const BTreeNode& node) const;
		
		virtual sl_bool writeNodeData(const BTreeNode& node, NodeData* data);
		
		virtual void releaseNodeData(NodeData* data);
		
	};

}

#include "detail/btree.inc"

#endif
