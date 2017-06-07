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
		HashTableBaseNode* next;
		sl_uint32 hash;
	};
	
	struct HashTableBaseTable
	{
		HashTableBaseNode** nodes;
		sl_size count;
		sl_uint32 capacity;
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
		
		static void expand(Table* table) noexcept;
		
		static void compact(Table* table) noexcept;
		
	};
	
	template <class KT, class VT>
	void _priv_HashTable_free(HashTableStruct<KT, VT>* table) noexcept;
	
	
	template <class KT, class VT>
	template <class KEY, class... VALUE_ARGS>
	SLIB_INLINE HashTableNode<KT, VT>::HashTableNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept
	 : key(Forward<KEY>(_key)), value(Forward<VALUE_ARGS>(value_args)...)
	 {}
	
	
	template <class KT, class VT>
	SLIB_INLINE HashTablePosition<KT, VT>::HashTablePosition(HashTableNode<KT, VT>** _entry, HashTableNode<KT, VT>** _last_entry, HashTableNode<KT, VT>* _node) noexcept
	 : entry(_entry), last_entry(_last_entry), node(_node)
	 {}
	
	template <class KT, class VT>
	SLIB_INLINE HashTableNode<KT, VT>& HashTablePosition<KT, VT>::operator*() const noexcept
	{
		return *node;
	}
	
	template <class KT, class VT>
	SLIB_INLINE sl_bool HashTablePosition<KT, VT>::operator==(const HashTablePosition<KT, VT>& other) const noexcept
	{
		return node == other.node;
	}
	
	template <class KT, class VT>
	SLIB_INLINE sl_bool HashTablePosition<KT, VT>::operator!=(const HashTablePosition<KT, VT>& other) const noexcept
	{
		return node != other.node;
	}
	
	template <class KT, class VT>
	SLIB_INLINE HashTablePosition<KT, VT>& HashTablePosition<KT, VT>::operator++() noexcept
	{
		node = node->next;
		while (!node) {
			entry++;
			if (entry == last_entry) {
				break;
			}
			node = *entry;
		}
		return *this;
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
	SLIB_INLINE sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::isEmpty() const noexcept
	{
		return m_table.count == 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::isNotEmpty() const noexcept
	{
		return m_table.count > 0;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getCapacity() const noexcept
	{
		return m_table.capacity;
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
			if (node->hash == hash && m_equals(node->key, key)) {
				return node;
			}
			node = node->next;
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
			if (node->hash == hash && m_equals(node->key, key) && value_equals(node->value, value)) {
				return node;
			}
			node = node->next;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointer(const KT& key) const noexcept
	{
		Node* node = find(key);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Node* node = findKeyAndValue(key, value, value_equals);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::get(const KT& key, VT* value) const noexcept
	{
		Node* node = find(key);
		if (node) {
			if (value) {
				*value = node->value;
			}
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashTable<KT, VT, HASH, KEY_EQUALS>::getValue(const KT& key) const noexcept
	{
		Node* node = find(key);
		if (node) {
			return node->value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashTable<KT, VT, HASH, KEY_EQUALS>::getValue(const KT& key, const VT& def) const noexcept
	{
		Node* node = find(key);
		if (node) {
			return node->value;
		}
		return def;
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
			if (node->hash == hash && m_equals(node->key, key)) {
				ret.add_NoLock(node->value);
			}
			node = node->next;
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
			if (node->hash == hash && m_equals(node->key, key) && value_equals(node->value, value)) {
				ret.add_NoLock(node->value);
			}
			node = node->next;
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::put(KEY&& key, VALUE&& value, sl_bool* isInsertion) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		
		Node** nodes = m_table.nodes;
		Node* node = nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->key, key)) {
				node->value = Forward<VALUE>(value);
				if (isInsertion) {
					*isInsertion = sl_false;
				}
				return node;
			}
			node = node->next;
		}
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableBaseTable*>(&m_table)))) {
			return sl_null;
		}
		node = new Node(Forward<KEY>(key), Forward<VALUE>(value));
		if (node) {
			(m_table.count)++;
			node->hash = hash;
			node->next = nodes[index];
			nodes[index] = node;
			_priv_HashTable::expand(reinterpret_cast<HashTableBaseTable*>(&m_table));
			if (isInsertion) {
				*isInsertion = sl_true;
			}
			return node;
		}
		if (isInsertion) {
			*isInsertion = sl_false;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::replace(const KEY& key, VALUE&& value) noexcept
	{
		Node* node = find(key);
		if (node) {
			node->value = Forward<VALUE>(value);
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class... VALUE_ARGS>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::add(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableBaseTable*>(&m_table)))) {
			return sl_null;
		}
		Node* node = new Node(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		if (node) {
			sl_uint32 capacity = m_table.capacity;
			sl_uint32 hash = m_hash(key);
			sl_uint32 index = hash & (capacity - 1);
			Node** nodes = m_table.nodes;
			(m_table.count)++;
			node->hash = hash;
			node->next = nodes[index];
			nodes[index] = node;
			_priv_HashTable::expand(reinterpret_cast<HashTableBaseTable*>(&m_table));
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class... VALUE_ARGS>
	MapEmplaceReturn< HashTableNode<KT, VT> > HashTable<KT, VT, HASH, KEY_EQUALS>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		
		Node** nodes = m_table.nodes;
		Node* node = nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->key, key)) {
				return MapEmplaceReturn< HashTableNode<KT, VT> >(sl_false, node);
			}
			node = node->next;
		}
		if (!(_priv_HashTable::validateNodes(reinterpret_cast<HashTableBaseTable*>(&m_table)))) {
			return sl_null;
		}
		node = new Node(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		if (node) {
			(m_table.count)++;
			node->hash = hash;
			node->next = nodes[index];
			nodes[index] = node;
			_priv_HashTable::expand(reinterpret_cast<HashTableBaseTable*>(&m_table));
			return MapEmplaceReturn< HashTableNode<KT, VT> >(sl_true, node);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::removeNode(const HashTableNode<KT, VT>* nodeRemove) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		
		sl_uint32 hash = nodeRemove->hash;
		sl_uint32 index = hash & (capacity - 1);
		
		Node** link = m_table.nodes + index;
		Node* node = *link;
		while (node) {
			Node* next = node->next;
			if (node == nodeRemove) {
				*link = next;
				(m_table.count)--;
				_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
				delete node;
				return next;
			} else {
				link = &(node->next);
			}
			node = next;
		}
		return sl_null;
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
		
		Node** link = m_table.nodes + index;
		Node* node = *link;
		while (node) {
			Node* next = node->next;
			if (node->hash == hash && m_equals(node->key, key)) {
				*link = next;
				(m_table.count)--;
				if (outValue) {
					*outValue = Move(node->value);
				}
				_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
				delete node;
				return sl_true;
			} else {
				link = &(node->next);
			}
			node = next;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		
		Node** link = m_table.nodes + index;
		Node* node = *link;
		Node* nodeDelete = sl_null;
		Node** linkDelete = &nodeDelete;
		while ((node = *link)) {
			Node* next = node->next;
			if (node->hash == hash && m_equals(node->key, key)) {
				*link = next;
				if (outValues) {
					outValues->add_NoLock(Move(node->value));
				}
				*linkDelete = node;
				node->next = sl_null;
				linkDelete = &(node->next);
			} else {
				link = &(node->next);
			}
			node = next;
		}
		if (!nodeDelete) {
			return 0;
		}
		_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
		sl_size nDelete = 0;
		while (nodeDelete) {
			node = nodeDelete;
			nodeDelete = nodeDelete->next;
			delete node;
			nDelete++;
		}
		(m_table.count) -= nDelete;
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
		
		Node** link = m_table.nodes + index;
		Node* node = *link;
		while (node) {
			Node* next = node->next;
			if (node->hash == hash && m_equals(node->key, key) && value_equals(node->value, value)) {
				*link = next;
				(m_table.count)--;
				if (outValue) {
					*outValue = Move(node->value);
				}
				_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
				delete node;
				return sl_true;
			} else {
				link = &(node->next);
			}
			node = next;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);

		Node** link = m_table.nodes + index;
		Node* node = *link;
		Node* nodeDelete = sl_null;
		Node** linkDelete = &nodeDelete;
		while (node) {
			Node* next = node->next;
			if (node->hash == hash && m_equals(node->key, key) && value_equals(node->value, value)) {
				*link = next;
				if (outValues) {
					outValues->add_NoLock(Move(node->value));
				}
				*linkDelete = node;
				node->next = sl_null;
				linkDelete = &(node->next);
			} else {
				link = &(node->next);
			}
			node = next;
		}
		if (!nodeDelete) {
			return 0;
		}
		_priv_HashTable::compact(reinterpret_cast<HashTableBaseTable*>(&m_table));
		sl_size nDelete = 0;
		while (nodeDelete) {
			node = nodeDelete;
			nodeDelete = nodeDelete->next;
			delete node;
			nDelete++;
		}
		(m_table.count) -= nDelete;
		return nDelete;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeAll() noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
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
		Node** nodesTarget = m_table.nodes;
		Node** nodesSource = other->m_table.nodes;
		for (sl_uint32 i = 0; i < capacity; i++) {
			Node* nodeSource = nodesSource[i];
			if (nodeSource) {
				Node** link = &(nodesTarget[i]);
				do {
					Node* nodeTarget = new Node(nodeSource->key, nodeSource->value);
					*link = nodeTarget;
					if (!nodeTarget) {
						_priv_HashTable_free(&m_table);
						_priv_HashTable::init(reinterpret_cast<HashTableBaseTable*>(&m_table));
						return sl_false;
					}
					nodeTarget->hash = nodeSource->hash;
					link = &(nodeTarget->next);
					nodeSource = nodeSource->next;
				} while (nodeSource);
			}
		}
		m_table.count = other->m_table.count;
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashTablePosition<KT, VT> HashTable<KT, VT, HASH, KEY_EQUALS>::begin() const noexcept
	{
		Node** nodes = m_table.nodes;
		sl_uint32 capacity = m_table.capacity;
		for (sl_uint32 i = 0; i < capacity; i++) {
			Node* node = nodes[i];
			if (node) {
				return HashTablePosition<KT, VT>(nodes + i, nodes + capacity, node);
			}
		}
		return HashTablePosition<KT, VT>(sl_null, sl_null, sl_null);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashTablePosition<KT, VT> HashTable<KT, VT, HASH, KEY_EQUALS>::end() const noexcept
	{
		return HashTablePosition<KT, VT>(sl_null, sl_null, sl_null);
	}
	
	template <class KT, class VT>
	void _priv_HashTable_free(HashTableStruct<KT, VT>* table) noexcept
	{
		typedef HashTableNode<KT, VT> Node;
		Node** nodes = table->nodes;
		if (nodes) {
			sl_uint32 capacity = table->capacity;
			for (sl_uint32 i = 0; i < capacity; i++) {
				HashTableNode<KT, VT>* node = nodes[i];
				while (node) {
					HashTableNode<KT, VT>* next = node->next;
					delete node;
					node = next;
				}
			}
			Base::freeMemory(nodes);
		}
	}

}
