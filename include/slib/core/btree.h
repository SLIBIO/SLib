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

#include "constants.h"
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
		sl_uint64 position = 0;
	
	public:
		sl_bool operator==(const BTreeNode& other) const;
	
		sl_bool operator!=(const BTreeNode& other) const;
	
		sl_bool isNull() const;
	
		sl_bool isNotNull() const;
	
		void setNull();

	};
	
	class SLIB_EXPORT BTreePosition
	{
	public:
		BTreeNode node;
		sl_uint32 item = 0;
	
	public:
		sl_bool operator==(const BTreePosition& other) const;

		sl_bool operator!=(const BTreePosition& other) const;

		sl_bool isNull() const;
	
		sl_bool isNotNull() const;
	
		void setNull();

	};
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT BTree
	{
	public:
		BTree(sl_uint32 order = SLIB_BTREE_DEFAULT_ORDER);

		BTree(const KEY_COMPARE& compare, sl_uint32 order = SLIB_BTREE_DEFAULT_ORDER);

		virtual ~BTree();
	
	public:
		sl_bool isValid() const;
	
		sl_uint32 getOrder() const;
	
		sl_uint32 getMaxLength() const;

		sl_bool find(const KT& key, BTreePosition* pos = sl_null, VT* outValue = sl_null) const;

		sl_bool findInNode(const KT& key, const BTreeNode& node, BTreePosition* pos = sl_null, VT* outValue = sl_null) const;

		sl_bool findItemInNode(const KT& key, const BTreeNode& node, sl_uint32& pos, BTreeNode& link, VT* outValue = sl_null) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool findKeyAndValue(const KT& key, const VALUE& value, BTreePosition* pos = sl_null, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		sl_bool getRange(const KT& key, BTreePosition* pPosBegin, BTreePosition* pPosEnd) const;
	
		sl_bool getAt(const BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_uint64 getCount() const;

		sl_uint64 getCountInNode(const BTreeNode& node) const;

		sl_bool getFirstPosition(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool getNextPosition(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
	
		sl_bool getLastPosition(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool getPrevPosition(BTreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool get(const KT& key, VT* value = sl_null) const;

		List<VT> getValues(const KT& key) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		sl_bool remove(const KT& key, VT* outValue = sl_null);

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		sl_bool removeAt(const BTreePosition& pos);

		sl_bool removeNode(const BTreeNode& node);

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

		sl_bool _insertItemInNode(const BTreeNode& node, sl_uint32 at, const BTreeNode& after, const KT& key, const VT& value, const BTreeNode& link);

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
