/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	class HashTableBaseNode
	{
	public:
		HashTableBaseNode* chain;
		HashTableBaseNode* before;
		HashTableBaseNode* next;
		sl_uint32 hash;
	};
	
	struct HashTableBaseTable
	{
		HashTableBaseNode** nodes;
		sl_uint32 capacity;
		
		sl_size count;
		HashTableBaseNode* firstNode;
		HashTableBaseNode* lastNode;

		sl_uint32 capacityMin;
		sl_uint32 thresholdUp;
		sl_uint32 thresholdDown;
	};
	
	class _priv_HashTable
	{
	public:
		typedef HashTableBaseNode Node;
		typedef HashTableBaseTable Table;
		
	public:
		static void init(Table* table) noexcept;
		
		static void init(Table* table, sl_uint32 capacity) noexcept;
		
		static sl_bool validateNodes(Table* table) noexcept;
		
		static sl_bool createNodes(Table* table, sl_uint32 capacity) noexcept;
		
		static void free(Table* table) noexcept;
		
		static void add(Table* table, Node* node, sl_uint32 hash) noexcept;
		
		static void remove(Table* table, Node* node) noexcept;
		
		static void expand(Table* table) noexcept;
		
		static void compact(Table* table) noexcept;
		
	};
	
	template <class KT, class VT>
	void _priv_HashTable_free(HashTableStruct<KT, VT>* table) noexcept;

	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	SLIB_INLINE HashTableNode<KT, VT>::HashTableNode(KEY&& _key, VALUE&& _value) noexcept
	 : data(Forward<KEY>(_key), Forward<VALUE>(_value))
	{
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable() noexcept
	{
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table));
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity) noexcept
	{
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class HASH_ARG>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity, HASH_ARG&& hash) noexcept
	 : m_hash(Forward<HASH_ARG>(hash))
	{
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class HASH_ARG, class KEY_EQUALS_ARG>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity, HASH_ARG&& hash, KEY_EQUALS_ARG&& equals) noexcept
	 : m_hash(Forward<HASH_ARG>(hash)), m_equals(Forward<KEY_EQUALS_ARG>(equals))
	{
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::~HashTable() noexcept
	{
		_priv_HashTable_free(&m_table);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(HashTable<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	 : m_hash(Move(other.m_hash)), m_equals(Move(other.m_equals))
	{
		Base::copyMemory(&m_table, &(other.m_table), sizeof(Table));
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&(other.m_table)));
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>& HashTable<KT, VT, HASH, KEY_EQUALS>::operator=(HashTable<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	{
		_priv_HashTable_free(&m_table);
		Base::copyMemory(&m_table, &(other.m_table), sizeof(Table));
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&(other.m_table)));
		m_hash = Move(other.m_hash);
		m_equals = Move(other.m_equals);
		return *this;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getCount() const noexcept
	{
		return m_table.count;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getCapacity() const noexcept
	{
		return m_table.capacity;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::getFirstNode() const noexcept
	{
		return m_table.firstNode;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::getLastNode() const noexcept
	{
		return m_table.lastNode;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::find(const KT& key) const noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node = m_table.nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->data.key, key)) {
				return node;
			}
			node = node->chain;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node = m_table.nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->data.key, key) && value_equals(node->data.value, value)) {
				return node;
			}
			node = node->chain;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::get(const KT& key, VT* value) const noexcept
	{
		Node* node = find(key);
		if (node) {
			if (value) {
				*value = node->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointer(const KT& key) const noexcept
	{
		Node* node = find(key);
		if (node) {
			return &(node->data.value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Node* node = findKeyAndValue(key, value, value_equals);
		if (node) {
			return &(node->data.value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashTable<KT, VT, HASH, KEY_EQUALS>::getValues(const KT& key) const noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		List<VT> ret;
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node = m_table.nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->data.key, key)) {
				ret.add_NoLock(node->data.value);
			}
			node = node->chain;
		}
		return ret;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	List<VT> HashTable<KT, VT, HASH, KEY_EQUALS>::getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		List<VT> ret;
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node = m_table.nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->data.key, key) && value_equals(node->data.value, value)) {
				ret.add_NoLock(node->data.value);
			}
			node = node->chain;
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::put(KEY&& key, VALUE&& value, MapPutMode mode, HashTableNode<KT, VT>** ppNode) noexcept
	{
		
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableBaseTable*>(&m_table)))) {
			if (ppNode) {
				*ppNode = sl_null;
			}
			return sl_false;
		}

		sl_uint32 capacity = m_table.capacity;
		
		sl_uint32 hash = m_hash(key);

		if (mode != MapPutMode::AddAlways) {
			sl_uint32 index = hash & (capacity - 1);
			Node* node = m_table.nodes[index];
			while (node) {
				if (node->hash == hash && m_equals(node->data.key, key)) {
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
					break;
				}
				node = node->chain;
			}
			if (mode == MapPutMode::ReplaceExisting) {
				if (ppNode) {
					*ppNode = sl_null;
				}
				return sl_false;
			}
		}

		Node* newNode = new Node(Forward<KEY>(key), Forward<VALUE>(value));
		if (ppNode) {
			*ppNode = newNode;
		}
		if (newNode) {
			_priv_HashTable::add(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(newNode), hash);
			_priv_HashTable::expand(reinterpret_cast<HashTableBaseTable*>(&m_table));
			return sl_true;
		}
		return sl_false;

	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE, class VALUE_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue(KEY&& key, VALUE&& value, HashTableNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals) noexcept
	{
		
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableBaseTable*>(&m_table)))) {
			if (ppNode) {
				*ppNode = sl_null;
			}
			return sl_false;
		}
		
		sl_uint32 capacity = m_table.capacity;

		sl_uint32 hash = m_hash(key);
	
		sl_uint32 index = hash & (capacity - 1);
		Node* node = m_table.nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->data.key, key) && value_equals(node->data.value, value)) {
				if (ppNode) {
					*ppNode = node;
				}
				return sl_false;
			}
			node = node->chain;
		}

		Node* newNode = new Node(Forward<KEY>(key), Forward<VALUE>(value));
		if (ppNode) {
			*ppNode = newNode;
		}
		if (newNode) {
			_priv_HashTable::add(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(newNode), hash);
			_priv_HashTable::expand(reinterpret_cast<HashTableBaseTable*>(&m_table));
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::removeNode(const HashTableNode<KT, VT>* nodeRemove) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		
		sl_uint32 hash = nodeRemove->hash;
		sl_uint32 index = hash & (capacity - 1);
		
		Node* node;
		Node** link = m_table.nodes + index;
		while ((node = *link)) {
			if (node == nodeRemove) {
				*link = node->chain;
				_priv_HashTable::remove(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(node));
				_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
				delete node;
				return sl_true;
			} else {
				link = &(node->chain);
			}
		}
		return sl_false;

	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::remove(const KT& key, VT* outValue) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}

		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node;
		Node** link = m_table.nodes + index;
		while ((node = *link)) {
			if (node->hash == hash && m_equals(node->data.key, key)) {
				*link = node->chain;
				_priv_HashTable::remove(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(node));
				if (outValue) {
					*outValue = Move(node->data.value);
				}
				_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
				delete node;
				return sl_true;
			} else {
				link = &(node->chain);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node;
		Node** link = m_table.nodes + index;
		Node* nodeDelete = sl_null;
		Node** linkDelete = &nodeDelete;
		while ((node = *link)) {
			if (node->hash == hash && m_equals(node->data.key, key)) {
				*link = node->chain;
				_priv_HashTable::remove(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(node));
				if (outValues) {
					outValues->add_NoLock(Move(node->data.value));
				}
				*linkDelete = node;
				node->chain = sl_null;
				linkDelete = &(node->chain);
			} else {
				link = &(node->chain);
			}
		}
		if (!nodeDelete) {
			return 0;
		}
		_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
		sl_size nDelete = 0;
		while (nodeDelete) {
			node = nodeDelete;
			nodeDelete = nodeDelete->chain;
			delete node;
			nDelete++;
		}
		return nDelete;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue, const VALUE_EQUALS& value_equals) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node;
		Node** link = m_table.nodes + index;
		while ((node = *link)) {
			if (node->hash == hash && m_equals(node->data.key, key) && value_equals(node->data.value, value)) {
				*link = node->chain;
				_priv_HashTable::remove(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(node));
				if (outValue) {
					*outValue = Move(node->data.value);
				}
				_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
				delete node;
				return sl_true;
			} else {
				link = &(node->chain);
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Node* node;
		Node** link = m_table.nodes + index;
		Node* nodeDelete = sl_null;
		Node** linkDelete = &nodeDelete;
		while ((node = *link)) {
			if (node->hash == hash && m_equals(node->data.key, key) && value_equals(node->data.value, value)) {
				*link = node->chain;
				_priv_HashTable::remove(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(node));
				if (outValues) {
					outValues->add_NoLock(Move(node->data.value));
				}
				*linkDelete = node;
				node->chain = sl_null;
				linkDelete = &(node->chain);
			} else {
				link = &(node->chain);
			}
		}
		if (!nodeDelete) {
			return 0;
		}
		_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
		sl_size nDelete = 0;
		while (nodeDelete) {
			node = nodeDelete;
			nodeDelete = nodeDelete->chain;
			delete node;
			nDelete++;
		}
		return nDelete;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeAll() noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		sl_size count = m_table.count;
		_priv_HashTable_free(&m_table);
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table));
		return count;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other) noexcept
	{
		_priv_HashTable_free(&m_table);
		m_hash = other->m_hash;
		m_equals = other->m_equals;
		sl_uint32 capacity = other->m_table.capacity;
		if (capacity == 0) {
			_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table));
			return sl_true;
		}
		_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table), capacity);
		if (m_table.capacity == 0) {
			return sl_false;
		}
		Node* node = other->m_table.firstNode;
		while (node) {
			Node* newNode = new Node(node->data.key, node->data.value);
			if (!newNode) {
				return sl_false;
			}
			_priv_HashTable::add(reinterpret_cast<HashTableBaseTable*>(&m_table), reinterpret_cast<HashTableBaseNode*>(newNode), newNode->hash);
			node = node->next;
		}
		return sl_true;
	}
	
	
	template <class KT, class VT>
	void _priv_HashTable_free(HashTableStruct<KT, VT>* table) noexcept
	{
		HashTableNode<KT, VT>** nodes = table->nodes;
		sl_uint32 capacity = table->capacity;
		for (sl_uint32 i = 0; i < capacity; i++) {
			HashTableNode<KT, VT>* node = nodes[i];
			while (node) {
				HashTableNode<KT, VT>* chain = node->chain;
				delete node;
				node = chain;
			}
		}
		_priv_HashTable::free(reinterpret_cast<HashTableBaseTable*>(table));
	}

}
