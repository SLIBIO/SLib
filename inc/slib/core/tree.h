#ifndef CHECKHEADER_SLIB_CORE_TREE
#define CHECKHEADER_SLIB_CORE_TREE

#include "definition.h"

#include "constants.h"
#include "compare.h"
#include "search.h"
#include "list.h"
#include "new.h"

#define SLIB_BTREE_DEFAULT_ORDER 16

SLIB_NAMESPACE_BEGIN

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
	
	~BTree();

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

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

SLIB_INLINE sl_bool TreeNode::operator==(const TreeNode& other) const
{
	return position == other.position;
}

SLIB_INLINE sl_bool TreeNode::operator!=(const TreeNode& other) const
{
	return position != other.position;
}

SLIB_INLINE sl_bool TreeNode::isNull() const
{
	return position == 0;
}

SLIB_INLINE sl_bool TreeNode::isNotNull() const
{
	return position != 0;
}

SLIB_INLINE void TreeNode::setNull()
{
	position = 0;
}


SLIB_INLINE sl_bool TreePosition::operator==(const TreePosition& other) const
{
	return node == other.node && item == other.item;
}

SLIB_INLINE sl_bool TreePosition::operator!=(const TreePosition& other) const
{
	return node != other.node || item != other.item;
}

SLIB_INLINE sl_bool TreePosition::isNull() const
{
	return node.isNull();
}

SLIB_INLINE sl_bool TreePosition::isNotNull() const
{
	return node.isNotNull();
}

SLIB_INLINE void TreePosition::setNull()
{
	node.setNull();
}

template <class KT, class VT, class KEY_COMPARE>
BTree<KT, VT, KEY_COMPARE>::BTree(sl_uint32 order)
{
	if (order < 1) {
		order = 1;
	}
	m_order = order;
	m_maxLength = 0;
	m_totalCount = 0;
	init();
}

template <class KT, class VT, class KEY_COMPARE>
BTree<KT, VT, KEY_COMPARE>::BTree(const KEY_COMPARE& compare, sl_uint32 order) : m_compare(compare)
{
	if (order < 1) {
		order = 1;
	}
	m_order = order;
	m_maxLength = 0;
	m_totalCount = 0;
	init();
}

template <class KT, class VT, class KEY_COMPARE>
BTree<KT, VT, KEY_COMPARE>::~BTree()
{
	free();
}

template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE sl_bool BTree<KT, VT, KEY_COMPARE>::isValid() const
{
	return m_rootNode != sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE sl_uint32 BTree<KT, VT, KEY_COMPARE>::getOrder() const
{
	return m_order;
}

template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE sl_uint32 BTree<KT, VT, KEY_COMPARE>::getMaxLength() const
{
	return m_maxLength;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::search(const KT& key, TreePosition* pos, VT* outValue) const
{
	return searchInNode(key, getRootNode(), pos, outValue);
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::searchInNode(const KT& key, const TreeNode& node, TreePosition* pos, VT* outValue) const
{
	TreeNode link;
	sl_uint32 item;
	if (searchItemInNode(key, node, item, link, outValue)) {
		if (pos) {
			pos->node = node;
			pos->item = item;
		}
		return sl_true;
	} else {
		if (link.isNotNull()) {
			return searchInNode(key, link, pos, outValue);
		} else {
			if (pos) {
				pos->node = node;
				pos->item = item;
			}
			return sl_false;
		}
	}
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::searchItemInNode(const KT& key, const TreeNode& node, sl_uint32& pos, TreeNode& link, VT* outValue) const
{
	NodeDataScope data(this, node);
	if (data.isNull()) {
		link.setNull();
		return sl_false;
	}
	pos = 0;
	sl_uint32 n = data->countItems;
	if (n == 0) {
		link.setNull();
		return sl_false;
	}
	sl_size _pos = 0;
	if (BinarySearch::search(data->keys, n, key, &_pos, m_compare)) {
		pos = (sl_uint32)_pos;
		if (outValue) {
			*outValue = data->values[pos];
		}
		link.setNull();
		return sl_true;
	} else {
		pos = (sl_uint32)_pos;
		if (pos) {
			link = data->links[pos - 1];
		} else {
			link = data->linkFirst;
		}
		return sl_false;
	}
}

template <class KT, class VT, class KEY_COMPARE>
template <class _VT, class VALUE_EQUALS>
sl_bool BTree<KT, VT, KEY_COMPARE>::searchKeyAndValue(const KT& key, const _VT& value, TreePosition* pPos, VT* outValue, const VALUE_EQUALS& value_equals) const
{
	List<VT> ret;
	TreePosition pos;
	if (search(key, &pos)) {
		VT v;
		if (!(getAt(pos, sl_null, &v))) {
			if (pPos) {
				*pPos = pos;
			}
			return sl_false;
		}
		if (value_equals(value, v)) {
			if (pPos) {
				*pPos = pos;
			}
			if (outValue) {
				*outValue = v;
			}
			return sl_true;
		}
		TreePosition posMid = pos;
		KT keyItem;
		while (getPrevPosition(pos, &keyItem, &v)) {
			if (m_compare(keyItem, key) == 0) {
				if (value_equals(value, v)) {
					if (pPos) {
						*pPos = pos;
					}
					if (outValue) {
						*outValue = v;
					}
					return sl_true;
				}
			} else {
				break;
			}
		}
		pos = posMid;
		while (getNextPosition(pos, &keyItem, &v)) {
			if (m_compare(keyItem, key) == 0) {
				if (value_equals(value, v)) {
					if (pPos) {
						*pPos = pos;
					}
					if (outValue) {
						*outValue = v;
					}
					return sl_true;
				}
			} else {
				break;
			}
		}
		if (pPos) {
			*pPos = posMid;
		}
	} else {
		if (pPos) {
			*pPos = pos;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::getRange(const KT& key, TreePosition* pPosBegin, TreePosition* pPosEnd) const
{
	TreePosition pos;
	if (search(key, &pos)) {
		TreePosition posBegin = pos;
		TreePosition posEnd = pos;
		KT keyItem;
		while (getPrevPosition(pos, &keyItem)) {
			if (m_compare(keyItem, key) == 0) {
				posBegin = pos;
			} else {
				break;
			}
		}
		pos = posEnd;
		while (getNextPosition(pos, &keyItem)) {
			if (m_compare(keyItem, key) == 0) {
				posEnd = pos;
			} else {
				break;
			}
		}
		return sl_true;
	}
	if (pPosBegin) {
		*pPosBegin = pos;
	}
	if (pPosEnd) {
		pPosEnd->setNull();
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::getAt(const TreePosition& pos, KT* key, VT* value) const
{
	NodeDataScope data(this, pos.node);
	if (data.isNotNull()) {
		if (pos.item < data->countItems) {
			if (key) {
				*key = data->keys[pos.item];
			}
			if (value) {
				*value = data->values[pos.item];
			}
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
VT* BTree<KT, VT, KEY_COMPARE>::getValuePointerAt(const TreePosition& pos) const
{
	NodeDataScope data(this, pos.node);
	if (data.isNotNull()) {
		if (pos.item < data->countItems) {
			return &(data->values[pos.item]);
		}
	}
	return sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
sl_uint64 BTree<KT, VT, KEY_COMPARE>::getCount() const
{
	return getCountInNode(getRootNode());
}

template <class KT, class VT, class KEY_COMPARE>
sl_uint64 BTree<KT, VT, KEY_COMPARE>::getCountInNode(const TreeNode& node) const
{
	if (node.isNull()) {
		return 0;
	}
	NodeDataScope data(this, node);
	if (data.isNotNull()) {
		sl_uint64 ret = data->countTotal;
		return ret;
	} else {
		return 0;
	}
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::getFirstPosition(TreePosition& pos, KT* key, VT* value ) const
{
	TreeNode node = getRootNode();
	if (node.isNull()) {
		return sl_false;
	}
	while (1) {
		NodeDataScope data(this, node);
		if (data.isNull()) {
			return sl_false;
		}
		TreeNode first = data->linkFirst;
		if (first.isNotNull()) {
			node = first;
		} else {
			if (data->countItems == 0) {
				return sl_false;
			} else {
				pos.node = node;
				pos.item = 0;
				if (key) {
					*key = data->keys[0];
				}
				if (value) {
					*value = data->values[0];
				}
				return sl_true;
			}
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::getNextPosition(TreePosition& pos, KT* key, VT* value) const
{
	if (pos.isNull()) {
		return getFirstPosition(pos, key, value);
	}
	TreeNode nodeStart = pos.node;
	sl_uint32 itemStart = pos.item;
	NodeDataScope dataStart(this, nodeStart);
	if (dataStart.isNull()) {
		return sl_false;
	}
	if (itemStart >= dataStart->countItems) {
		return sl_false;
	}
	TreeNode node = dataStart->links[itemStart];
	if (node.isNotNull()) {
		while (1) {
			NodeDataScope data(this, node);
			if (data.isNull()) {
				return sl_false;
			}
			TreeNode first = data->linkFirst;
			if (first.isNotNull()) {
				node = first;
			} else {
				if (data->countItems == 0) {
					return sl_false;
				} else {
					pos.node = node;
					pos.item = 0;
					if (key) {
						*key = data->keys[0];
					}
					if (value) {
						*value = data->values[0];
					}
					return sl_true;
				}
			}
		}
	} else {
		if (itemStart == dataStart->countItems - 1) {
			node = nodeStart;
			TreeNode parent = dataStart->linkParent;
			while (1) {
				if (parent.isNull()) {
					return sl_false;
				}
				NodeDataScope data(this, parent);
				if (data.isNull()) {
					return sl_false;
				}
				if (data->linkFirst == node) {
					pos.node = parent;
					pos.item = 0;
					if (key) {
						*key = data->keys[0];
					}
					if (value) {
						*value = data->values[0];
					}
					return sl_true;
				}
				sl_uint32 n = data->countItems;
				for (sl_uint32 i = 1; i <= n; i++) {
					if (data->links[i - 1] == node) {
						if (i == n) {
							node = parent;
							parent = data->linkParent;
						} else {
							pos.node = parent;
							pos.item = i;
							if (key) {
								*key = data->keys[i];
							}
							if (value) {
								*value = data->values[i];
							}
							return sl_true;
						}
						break;
					}
				}
			}
		} else {
			itemStart++;
			pos.item = itemStart;
			if (key) {
				*key = dataStart->keys[itemStart];
			}
			if (value) {
				*value = dataStart->values[itemStart];
			}
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::getLastPosition(TreePosition& pos, KT* key, VT* value) const
{
	TreeNode node = getRootNode();
	if (node.isNull()) {
		return sl_false;
	}
	while (1) {
		NodeDataScope data(this, node);
		if (data.isNull()) {
			return sl_false;
		}
		sl_uint32 n = data->countItems;
		if (n == 0) {
			TreeNode first = data->linkFirst;
			if (first.isNotNull()) {
				node = first;
			} else {
				return sl_false;
			}
		} else {
			TreeNode last = data->links[n - 1];
			if (last.isNotNull()) {
				node = last;
			} else {
				pos.node = node;
				pos.item = n - 1;
				if (key) {
					*key = data->keys[n - 1];
				}
				if (value) {
					*value = data->values[n - 1];
				}
				return sl_true;
			}
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::getPrevPosition(TreePosition& pos, KT* key, VT* value) const
{
	if (pos.isNull()) {
		return getLastPosition(pos, key, value);
	}
	TreeNode nodeStart = pos.node;
	sl_uint32 itemStart = pos.item;
	NodeDataScope dataStart(this, nodeStart);
	if (dataStart.isNull()) {
		return sl_false;
	}
	if (itemStart >= dataStart->countItems) {
		return sl_false;
	}
	TreeNode node;
	if (itemStart == 0) {
		node = dataStart->linkFirst;
	} else {
		node = dataStart->links[itemStart - 1];
	}
	if (node.isNotNull()) {
		while (1) {
			NodeDataScope data(this, node);
			if (data.isNull()) {
				return sl_false;
			}
			sl_uint32 n = data->countItems;
			if (n == 0) {
				TreeNode first = data->linkFirst;
				if (first.isNotNull()) {
					node = first;
				} else {
					return sl_false;
				}
			} else {
				TreeNode last = data->links[n - 1];
				if (last.isNotNull()) {
					node = last;
				} else {
					pos.node = node;
					pos.item = n - 1;
					if (key) {
						*key = data->keys[n-1];
					}
					if (value) {
						*value = data->values[n-1];
					}
					return sl_true;
				}
			}
		}
	} else {
		if (itemStart == 0) {
			node = nodeStart;
			TreeNode parent = dataStart->linkParent;
			while (1) {
				if (parent.isNull()) {
					return sl_false;
				}
				NodeDataScope data(this, parent);
				if (data.isNull()) {
					return sl_false;
				}
				if (data->linkFirst == node) {
					node = parent;
					parent = data->linkParent;
				} else {
					sl_uint32 n = data->countItems;
					for (sl_uint32 i = 0; i < n; i++) {
						if (data->links[i] == node) {
							pos.node = parent;
							pos.item = i;
							if (key) {
								*key = data->keys[i];
							}
							if (value) {
								*value = data->values[i];
							}
							return sl_true;
						}
					}
				}
			}
		} else {
			itemStart--;
			pos.item = itemStart;
			if (key) {
				*key = dataStart->keys[itemStart];
			}
			if (value) {
				*value = dataStart->values[itemStart];
			}
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::get(const KT& key, VT* value) const
{
	return search(key, sl_null, value);
}

template <class KT, class VT, class KEY_COMPARE>
VT* BTree<KT, VT, KEY_COMPARE>::getItemPointer(const KT& key) const
{
	TreePosition pos;
	if (search(key, &pos)) {
		return getValuePointerAt(pos);
	}
	return sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
template <class _VT, class VALUE_EQUALS>
VT* BTree<KT, VT, KEY_COMPARE>::getItemPointerByKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
{
	TreePosition pos;
	if (searchKeyAndValue(key, value, &pos, sl_null, value_equals)) {
		return getValuePointerAt(pos);
	}
	return sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
List<VT> BTree<KT, VT, KEY_COMPARE>::getValues(const KT& key) const
{
	List<VT> ret;
	TreePosition pos;
	if (search(key, &pos)) {
		VT value;
		if (!(getAt(pos, sl_null, &value))) {
			return ret;
		}
		ret.add_NoLock(value);
		TreePosition posMid = pos;
		KT keyItem;
		while (getPrevPosition(pos, &keyItem, &value)) {
			if (m_compare(keyItem, key) == 0) {
				ret.add_NoLock(value);
			} else {
				break;
			}
		}
		pos = posMid;
		while (getNextPosition(pos, &keyItem, &value)) {
			if (m_compare(keyItem, key) == 0) {
				ret.add_NoLock(value);
			} else {
				break;
			}
		}
	}
	return ret;
}

template <class KT, class VT, class KEY_COMPARE>
template <class _VT, class VALUE_EQUALS>
List<VT> BTree<KT, VT, KEY_COMPARE>::getValuesByKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
{
	List<VT> ret;
	TreePosition pos;
	VT v;
	if (searchKeyAndValue(key, value, &pos, &v, value_equals)) {
		if (value_equals(v, value)) {
			ret.add_NoLock(v);
		}
		TreePosition posMid = pos;
		KT keyItem;
		while (getPrevPosition(pos, &keyItem, &v)) {
			if (m_compare(keyItem, key) == 0) {
				if (value_equals(v, value)) {
					ret.add_NoLock(v);
				}
			} else {
				break;
			}
		}
		pos = posMid;
		while (getNextPosition(pos, &keyItem, &v)) {
			if (m_compare(keyItem, key) == 0) {
				if (value_equals(v, value)) {
					ret.add_NoLock(v);
				}
			} else {
				break;
			}
		}
	}
	return ret;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	if (pFlagExist) {
		*pFlagExist = sl_false;
	}
	TreePosition pos;
	if (search(key, &pos)) {
		if (pFlagExist) {
			*pFlagExist = sl_true;
		}
		if (mode != MapPutMode::AddAlways) {
			if (mode == MapPutMode::AddNew) {
				return sl_false;
			}
			NodeDataScope data(this, pos.node);
			if (data.isNotNull()) {
				if (pos.item < data->countItems) {
					data->values[pos.item] = value;
					return writeNodeData(pos.node, data.data);
				}
				return sl_false;
			}
		}
	}
	if (mode == MapPutMode::ReplaceExisting) {
		return sl_false;
	}
	TreeNode link;
	return _insertItemInNode(pos.node, pos.item, link, key, value, link);
}

template <class KT, class VT, class KEY_COMPARE>
template <class _VT, class VALUE_EQUALS>
sl_bool BTree<KT, VT, KEY_COMPARE>::addIfNewKeyAndValue(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_EQUALS& value_equals)
{
	if (pFlagExist) {
		*pFlagExist = sl_false;
	}
	TreePosition pos;
	if (searchKeyAndValue(key, value, &pos, sl_null, value_equals)) {
		if (pFlagExist) {
			*pFlagExist = sl_true;
		}
		return sl_false;
	}
	TreeNode link;
	return _insertItemInNode(pos.node, pos.item, link, key, value, link);
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::remove(const KT& key, VT* outValue)
{
	TreePosition pos;
	if (search(key, &pos, outValue)) {
		if (removeAt(pos)) {
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
sl_size BTree<KT, VT, KEY_COMPARE>::removeItems(const KT& key, List<VT>* outValues)
{
	TreePosition pos;
	if (outValues) {
		VT v;
		if (search(key, &pos, &v)) {
			if (removeAt(pos)) {
				outValues->add_NoLock(v);
				sl_size n = 1;
				while (search(key, &pos, &v)) {
					if (removeAt(pos)) {
						outValues->add_NoLock(v);
						n++;
					} else {
						break;
					}
				}
				return n;
			}
		}
	} else {
		if (search(key, &pos)) {
			if (removeAt(pos)) {
				sl_size n = 1;
				while (search(key, &pos)) {
					if (removeAt(pos)) {
						n++;
					} else {
						break;
					}
				}
				return n;
			}
		}
	}
	return 0;
}

template <class KT, class VT, class KEY_COMPARE>
template <class _VT, class VALUE_EQUALS>
sl_bool BTree<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const _VT& value, VT* outValue, const VALUE_EQUALS& value_equals)
{
	TreePosition pos;
	if (searchKeyAndValue(key, value, &pos, outValue, value_equals)) {
		if (removeAt(pos)) {
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
template <class _VT, class VALUE_EQUALS>
sl_size BTree<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
{
	TreePosition pos;
	if (outValues) {
		VT v;
		if (searchKeyAndValue(key, value, &pos, &v, value_equals)) {
			if (removeAt(pos)) {
				outValues->add_NoLock(v);
				sl_size n = 1;
				while (searchKeyAndValue(key, value, &pos, &v, value_equals)) {
					if (removeAt(pos)) {
						outValues->add_NoLock(v);
						n++;
					} else {
						break;
					}
				}
				return n;
			}
		}
	} else {
		if (searchKeyAndValue(key, value, &pos, sl_null, value_equals)) {
			if (removeAt(pos)) {
				sl_size n = 1;
				while (searchKeyAndValue(key, value, &pos, sl_null, value_equals)) {
					if (removeAt(pos)) {
						n++;
					} else {
						break;
					}
				}
				return n;
			}
		}
	}
	return 0;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::removeAt(const TreePosition& pos)
{
	if (pos.node.isNull()) {
		return sl_false;
	}
	NodeDataScope data(this, pos.node);
	if (data.isNull()) {
		return sl_false;
	}
	sl_uint32 n = data->countItems;
	if (n == 0) {
		return sl_false;
	}
	if (pos.item >= n) {
		return sl_false;
	}
	TreeNode left;
	if (pos.item == 0) {
		left = data->linkFirst;
	} else {
		left = data->links[pos.item - 1];
	}
	TreeNode right = data->links[pos.item];
	if (left.isNull()) {
		if (pos.item == 0) {
			data->linkFirst = data->links[0];
		} else {
			data->links[pos.item - 1] = data->links[pos.item];
		}
	} else {
		if (right.isNotNull()) {
			TreePosition nextPos = pos;
			KT keyNext;
			VT valueNext;
			if (!getNextPosition(nextPos, &keyNext, &valueNext)) {
				return sl_false;
			}
			if (nextPos.node.isNull()) {
				return sl_false;
			}
			if (nextPos.node == pos.node) {
				return sl_false;
			}
			data->keys[pos.item] = keyNext;
			data->values[pos.item] = valueNext;
			if (!writeNodeData(pos.node, data.data)) {
				return sl_false;
			}
			return removeAt(nextPos);
		}
	}
	if (n <= 1 && pos.node != getRootNode()) {
		return removeNode(pos.node);
	}
	for (sl_uint32 i = pos.item; i < n - 1; i++) {
		data->keys[i] = data->keys[i + 1];
		data->values[i] = data->values[i + 1];
		data->links[i] = data->links[i + 1];
	}
	data->countTotal--;
	data->countItems = n - 1;
	if (!writeNodeData(pos.node, data.data)) {
		return sl_false;
	}
	_changeParentTotalCount(data.data, -1);
	return sl_true;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::removeNode(const TreeNode& node)
{
	if (node.isNull()) {
		return sl_false;
	}
	if (node == getRootNode()) {
		return sl_false;
	}
	return _removeNode(node, sl_true);
}

template <class KT, class VT, class KEY_COMPARE>
sl_size BTree<KT, VT, KEY_COMPARE>::removeAll()
{
	TreeNode node = getRootNode();
	NodeDataScope data(this, node);
	sl_size countTotal = (sl_size)(data->countTotal);
	if (data.isNotNull()) {
		_removeNode(data->linkFirst, sl_false);
		sl_uint32 n = data->countItems;
		for (sl_uint32 i = 0; i < n; i++) {
			if (data->links[i].isNotNull()) {
				_removeNode(data->links[i], sl_false);
			}
		}
		data->countTotal = 0;
		data->countItems = 0;
		data->linkFirst.setNull();
		m_totalCount = 0;
		if (writeNodeData(node, data.data)) {
			return countTotal;
		}
	}
	return 0;
}


template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE BTree<KT, VT, KEY_COMPARE>::NodeDataScope::NodeDataScope(const BTree* tree, const TreeNode& node)
{
	this->tree = (BTree*)tree;
	this->data = tree->readNodeData(node);
}

template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE BTree<KT, VT, KEY_COMPARE>::NodeDataScope::~NodeDataScope()
{
	tree->releaseNodeData(data);
}

template <class KT, class VT, class KEY_COMPARE>
typename BTree<KT, VT, KEY_COMPARE>::NodeData* BTree<KT, VT, KEY_COMPARE>::NodeDataScope::operator->()
{
	return data;
}

template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE sl_bool BTree<KT, VT, KEY_COMPARE>::NodeDataScope::isNull() {
	return data == sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
SLIB_INLINE sl_bool BTree<KT, VT, KEY_COMPARE>::NodeDataScope::isNotNull() {
	return data != sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
typename BTree<KT, VT, KEY_COMPARE>::NodeData* BTree<KT, VT, KEY_COMPARE>::_createNodeData()
{
	NodeData* data = new NodeData;
	if (data) {
		data->countTotal = 0;
		data->countItems = 0;
		data->linkParent.setNull();
		data->linkFirst.setNull();
		data->keys = New<KT>::create(m_order);
		if (data->keys) {
			data->values = New<VT>::create(m_order);
			if (data->values) {
				data->links = New<TreeNode>::create(m_order);
				if (data->links) {
					return data;
				}
				New<VT>::free(data->values, m_order);
			}
			New<KT>::free(data->keys, m_order);
		}
		delete data;
	}
	return sl_null;
}

template <class KT, class VT, class KEY_COMPARE>
void BTree<KT, VT, KEY_COMPARE>::_freeNodeData(NodeData* data)
{
	if (data) {
		New<KT>::free(data->keys, m_order);
		New<VT>::free(data->values, m_order);
		New<TreeNode>::free(data->links, m_order);
		delete data;
	}
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::_insertItemInNode(const TreeNode& node, sl_uint32 at, const TreeNode& after, const KT& key, const VT& value, const TreeNode& link)
{
	NodeDataScope data(this, node);
	if (data.isNull()) {
		return sl_false;
	}
	sl_uint32 n = data->countItems;
	if (n > m_order) {
		return sl_false;
	}
	if (after.isNotNull()) {
		if (after == data->linkFirst) {
			at = 0;
		} else {
			for (sl_uint32 i = 0; i < n; i++) {
				if (data->links[i] == after) {
					at = i + 1;
				}
			}
		}
	}
	if (at > n) {
		return sl_false;
	}
	if (n < m_order) {
		for (sl_uint32 i = n; i > at; i--) {
			data->keys[i] = data->keys[i - 1];
			data->values[i] = data->values[i - 1];
			data->links[i] = data->links[i - 1];
		}
		data->keys[at] = key;
		data->values[at] = value;
		data->links[at] = link;
		data->countItems = n + 1;
		data->countTotal += 1;
		if (writeNodeData(node, data.data)) {
			_changeParentTotalCount(data.data, 1);
			return sl_true;
		}
	} else {
		sl_uint32 half = n / 2; // equals to  "m_order / 2"
		NodeData* newData = _createNodeData();
		if (newData) {
			KT keyTop;
			VT valueTop;
			sl_uint32 i;
			if (at > half) {
				sl_uint32 m = at - half - 1;
				newData->linkFirst = data->links[half];
				for (i = 0; i < m; i++) {
					newData->keys[i] = data->keys[i + half + 1];
					newData->values[i] = data->values[i + half + 1];
					newData->links[i] = data->links[i + half + 1];
				}
				newData->keys[m] = key;
				newData->values[m] = value;
				newData->links[m] = link;
				for (i = m + 1; i < n - half; i++) {
					newData->keys[i] = data->keys[i + half];
					newData->values[i] = data->values[i + half];
					newData->links[i] = data->links[i + half];
				}
				keyTop = data->keys[half];
				valueTop = data->values[half];
			} else if (at < half) {
				newData->linkFirst = data->links[half - 1];
				for (i = 0; i < n - half; i++) {
					newData->keys[i] = data->keys[i + half];
					newData->values[i] = data->values[i + half];
					newData->links[i] = data->links[i + half];
				}
				keyTop = data->keys[half - 1];
				valueTop = data->values[half - 1];
				for (i = half - 1; i > at; i--) {
					data->keys[i] = data->keys[i - 1];
					data->values[i] = data->values[i - 1];
					data->links[i] = data->links[i - 1];
				}
				data->keys[at] = key;
				data->values[at] = value;
				data->links[at] = link;
			} else {
				newData->linkFirst = link;
				for (i = 0; i < n - half; i++) {
					newData->keys[i] = data->keys[i + half];
					newData->values[i] = data->values[i + half];
					newData->links[i] = data->links[i + half];
				}
				keyTop = key;
				valueTop = value;
			}
			TreeNode parent = data->linkParent;
			sl_bool flagCreateRoot = sl_false;
			if (parent.isNull()) {
				parent = createNode(sl_null);
				if (parent.isNull()) {
					_freeNodeData(newData);
					return sl_false;
				}
				flagCreateRoot = sl_true;
				if (! setRootNode(parent)) {
					_freeNodeData(newData);
					return sl_false;
				}
				m_maxLength++;
			}
			data->linkParent = parent;
			data->countItems = half;
			data->countTotal = _getTotalCountInData(data.data);
			newData->linkParent = parent;
			newData->countItems = n - half;
			newData->countTotal = _getTotalCountInData(newData);
			TreeNode newNode = createNode(newData);
			if (newNode.isNull()) {
				_freeNodeData(newData);
				return sl_false;
			}
			NodeDataScope newNodeData(this, newNode);
			sl_bool flagSuccess = sl_false;
			if (writeNodeData(node, data.data)) {
				if (newNodeData->linkFirst.isNotNull()) {
					NodeDataScope dataChild(this, newNodeData->linkFirst);
					if (dataChild.isNotNull()) {
						dataChild->linkParent = newNode;
						writeNodeData(newNodeData->linkFirst, dataChild.data);
					}
				}
				for (i = 0; i < n - half; i++) {
					TreeNode child = newNodeData->links[i];
					if (child.isNotNull()) {
						NodeDataScope dataChild(this, child);
						if (dataChild.isNotNull()) {
							dataChild->linkParent = newNode;
							writeNodeData(child, dataChild.data);
						}
					}
				}
				if (flagCreateRoot) {
					NodeDataScope dataRoot(this, parent);
					if (dataRoot.isNotNull()) {
						dataRoot->countTotal = data->countTotal + newNodeData->countTotal + 1;
						m_totalCount = dataRoot->countTotal;
						dataRoot->countItems = 1;
						dataRoot->linkFirst = node;
						dataRoot->keys[0] = keyTop;
						dataRoot->values[0] = valueTop;
						dataRoot->links[0] = newNode;
						flagSuccess = writeNodeData(parent, dataRoot.data);
					}
				} else {
					flagSuccess = _insertItemInNode(parent, -1, node, keyTop, valueTop, newNode);
				}
			}
			return flagSuccess;
		}
	}
	return sl_false;
}

template <class KT, class VT, class KEY_COMPARE>
void BTree<KT, VT, KEY_COMPARE>::_changeTotalCount(const TreeNode& node, sl_int64 n)
{
	NodeDataScope data(this, node);
	if (data.isNotNull()) {
		data->countTotal += n;
		writeNodeData(node, data.data);
		TreeNode parent = data->linkParent;
		if (parent.isNotNull()) {
			_changeTotalCount(parent, n);
		}
	}
}

template <class KT, class VT, class KEY_COMPARE>
void BTree<KT, VT, KEY_COMPARE>::_changeParentTotalCount(NodeData* data, sl_int64 n)
{
	TreeNode parent = data->linkParent;
	if (parent.isNotNull()) {
		_changeTotalCount(parent, n);
	} else {
		m_totalCount = data->countTotal;
	}
}

template <class KT, class VT, class KEY_COMPARE>
sl_uint64 BTree<KT, VT, KEY_COMPARE>::_getTotalCountInData(NodeData* data) const
{
	sl_uint32 n = data->countItems;
	sl_uint64 m = n + getCountInNode(data->linkFirst);
	for (sl_uint32 i = 0; i < n; i++) {
		m += getCountInNode(data->links[i]);
	}
	return m;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::_removeNode(const TreeNode& node, sl_bool flagUpdateParent)
{
	if (node.isNull()) {
		return sl_false;
	}
	NodeDataScope data(this, node);
	if (data.isNull()) {
		return sl_false;
	}
	if (flagUpdateParent) {
		TreeNode parent = data->linkParent;
		if (parent.isNull()) {
			return sl_false;
		}
		NodeDataScope parentData(this, parent);
		if (parentData.isNotNull()) {
			if (node == parentData->linkFirst) {
				parentData->linkFirst.setNull();
			} else {
				sl_uint32 i;
				sl_uint32 n = parentData->countItems;
				for (i = 0; i < n; i++) {
					if (parentData->links[i] == node) {
						parentData->links[i].setNull();
						break;
					}
				}
				if (i == n) {
					return sl_false;
				}
			}
			parentData->countTotal -= data->countTotal;
			if (!writeNodeData(parent, parentData.data)) {
				return sl_false;
			}
			_changeParentTotalCount(parentData.data, -((sl_int64)(data->countTotal)));
		}
	}
	{
		_removeNode(data->linkFirst, sl_false);
		sl_uint32 n = data->countItems;
		for (sl_uint32 i = 0; i < n; i++) {
			if (data->links[i].isNotNull()) {
				_removeNode(data->links[i], sl_false);
			}
		}
	}
	return deleteNode(node);
}

template <class KT, class VT, class KEY_COMPARE>
void BTree<KT, VT, KEY_COMPARE>::init()
{
	m_rootNode = _createNodeData();
}

template <class KT, class VT, class KEY_COMPARE>
void BTree<KT, VT, KEY_COMPARE>::free()
{
	_removeNode(getRootNode(), sl_false);
}

template <class KT, class VT, class KEY_COMPARE>
TreeNode BTree<KT, VT, KEY_COMPARE>::getRootNode() const
{
	TreeNode node;
	node.position = (sl_size)m_rootNode;
	return node;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::setRootNode(TreeNode node)
{
	if (node.isNull()) {
		return sl_false;
	}
	m_rootNode = (NodeData*)(void*)(sl_size)(node.position);
	return sl_true;
}

template <class KT, class VT, class KEY_COMPARE>
TreeNode BTree<KT, VT, KEY_COMPARE>::createNode(NodeData* data)
{
	TreeNode node;
	if (data) {
		node.position = (sl_size)data;
	} else {
		node.position = (sl_size)(_createNodeData());
	}
	// important for file storage
	{
		// _freeNodeData(data);
	}
	return node;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::deleteNode(TreeNode node)
{
	if (node.isNull()) {
		return sl_false;
	}
	NodeData* data = (NodeData*)(void*)(sl_size)(node.position);
	_freeNodeData(data);
	return sl_true;
}

template <class KT, class VT, class KEY_COMPARE>
typename BTree<KT, VT, KEY_COMPARE>::NodeData* BTree<KT, VT, KEY_COMPARE>::readNodeData(const TreeNode& node) const
{
	NodeData* data = (NodeData*)(void*)(sl_size)(node.position);
	return data;
}

template <class KT, class VT, class KEY_COMPARE>
sl_bool BTree<KT, VT, KEY_COMPARE>::writeNodeData(const TreeNode& node, NodeData* data)
{
	if (node.isNull()) {
		return sl_false;
	}
	if (!data) {
		return sl_false;
	}
	NodeData* o = (NodeData*)(void*)(sl_size)(node.position);
	if (o != data) {
		sl_uint32 n = o->countItems = data->countItems;
		o->countTotal = data->countTotal;
		o->linkParent = data->linkParent;
		o->linkFirst = data->linkFirst;
		for (sl_uint32 i = 0; i < n; i++) {
			o->keys[i] = data->keys[i];
			o->values[i] = data->values[i];
			o->links[i] = data->links[i];
		}
	}
	return sl_true;
}

template <class KT, class VT, class KEY_COMPARE>
void BTree<KT, VT, KEY_COMPARE>::releaseNodeData(NodeData* data)
{
}

SLIB_NAMESPACE_END

#endif
