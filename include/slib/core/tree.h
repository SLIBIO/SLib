/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_TREE
#define CHECKHEADER_SLIB_CORE_TREE

#include "definition.h"

#include "constants.h"
#include "compare.h"
#include "search.h"
#include "list.h"
#include "new_helper.h"

#define SLIB_BTREE_DEFAULT_ORDER 16

namespace slib
{
	
	class SLIB_EXPORT TreeNode
	{
	public:
		sl_uint64 position = 0;
	
	public:
		sl_bool operator==(const TreeNode& other) const;
	
		sl_bool operator!=(const TreeNode& other) const;
	
		sl_bool isNull() const;
	
		sl_bool isNotNull() const;
	
		void setNull();

	};
	
	class SLIB_EXPORT TreePosition
	{
	public:
		TreeNode node;
		sl_uint32 item = 0;
	
	public:
		sl_bool operator==(const TreePosition& other) const;

		sl_bool operator!=(const TreePosition& other) const;

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

		sl_bool search(const KT& key, TreePosition* pos = sl_null, VT* outValue = sl_null) const;

		sl_bool searchInNode(const KT& key, const TreeNode& node, TreePosition* pos = sl_null, VT* outValue = sl_null) const;

		sl_bool searchItemInNode(const KT& key, const TreeNode& node, sl_uint32& pos, TreeNode& link, VT* outValue = sl_null) const;

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		sl_bool searchKeyAndValue(const KT& key, const _VT& value, TreePosition* pos = sl_null, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		sl_bool getRange(const KT& key, TreePosition* pPosBegin, TreePosition* pPosEnd) const;
	
		sl_bool getAt(const TreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		VT* getValuePointerAt(const TreePosition& pos) const;

		sl_uint64 getCount() const;

		sl_uint64 getCountInNode(const TreeNode& node) const;

		sl_bool getFirstPosition(TreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool getNextPosition(TreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;
	
		sl_bool getLastPosition(TreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool getPrevPosition(TreePosition& pos, KT* key = sl_null, VT* value = sl_null) const;

		sl_bool get(const KT& key, VT* value = sl_null) const;

		VT* getItemPointer(const KT& key) const;

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		VT* getItemPointerByKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		List<VT> getValues(const KT& key) const;

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		List<VT> getValuesByKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		sl_bool addIfNewKeyAndValue(const KT& key, const _VT& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		sl_bool remove(const KT& key, VT* outValue = sl_null);

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		sl_bool removeKeyAndValue(const KT& key, const _VT& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		sl_size removeItemsByKeyAndValue(const KT& key, const _VT& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		sl_bool removeAt(const TreePosition& pos);

		sl_bool removeNode(const TreeNode& node);

		sl_size removeAll();

	protected:
		struct NodeData
		{
			sl_uint64 countTotal;
			sl_uint32 countItems;

			TreeNode linkParent;
			TreeNode linkFirst;
			KT* keys;
			VT* values;
			TreeNode* links;
		};

		class NodeDataScope
		{
		public:
			NodeData* data;
			BTree* tree;

		public:
			NodeDataScope(const BTree* tree, const TreeNode& node);

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

		sl_bool _insertItemInNode(const TreeNode& node, sl_uint32 at, const TreeNode& after, const KT& key, const VT& value, const TreeNode& link);

		void _changeTotalCount(const TreeNode& node, sl_int64 n);

		void _changeParentTotalCount(NodeData* data, sl_int64 n);

		sl_uint64 _getTotalCountInData(NodeData* data) const;

		sl_bool _removeNode(const TreeNode& node, sl_bool flagUpdateParent);

		// container-specific implementation
	private:
		NodeData* m_rootNode;
	
	protected:
		virtual void init();

		virtual void free();

		virtual TreeNode getRootNode() const;
		
		virtual sl_bool setRootNode(TreeNode node);
		
		// here, "data" must be newly created node data. this function should manage the memory of "data" parameter
		virtual TreeNode createNode(NodeData* data);
		
		virtual sl_bool deleteNode(TreeNode node);
		
		virtual NodeData* readNodeData(const TreeNode& node) const;
		
		virtual sl_bool writeNodeData(const TreeNode& node, NodeData* data);
		
		virtual void releaseNodeData(NodeData* data);
		
	};

}

#include "detail/tree.inc"

#endif
