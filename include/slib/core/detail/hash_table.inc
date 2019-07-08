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

namespace slib
{
	
	class HashTableNodeBase
	{
	public:
		HashTableNodeBase* next;
		sl_size hash;
	};
	
	struct HashTableStructBase
	{
		HashTableNodeBase** nodes;
		sl_size count;
		sl_size capacity;
		sl_size capacityMinimum;
		sl_size capacityMaximum;
		sl_size thresholdDown;
		sl_size thresholdUp;
	};
	
	namespace priv
	{
		namespace hash_table
		{

			class Helper
			{
			public:
				typedef HashTableNodeBase NODE;
				typedef HashTableStructBase TABLE;
				
			public:
				static void fixCapacityRange(TABLE* table) noexcept;
				
				static void updateThresholds(TABLE* table) noexcept;
				
				static void initialize(TABLE* table, sl_size capacityMinimum, sl_size capacityMaximum) noexcept;
				
				static void move(TABLE* dst, TABLE* src) noexcept;

				static void clear(TABLE* table) noexcept;
				
				static void setMinimumCapacity(TABLE* table, sl_size capacity) noexcept;
				
				static void setMaximumCapacity(TABLE* table, sl_size capacity) noexcept;

				static sl_bool validateNodes(TABLE* table) noexcept;
				
				static sl_bool reallocNodes(TABLE* table, sl_size capacity) noexcept;
				
				static void expand(TABLE* table) noexcept;
				
				static void shrink(TABLE* table) noexcept;
				
				template <class KT, class VT>
				static void free(HashTableStruct<KT, VT>* table) noexcept;
				
			};
			
		}
	}
	
	template <class KT, class VT>
	template <class KEY, class... VALUE_ARGS>
	SLIB_INLINE HashTableNode<KT, VT>::HashTableNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept
	 : key(Forward<KEY>(_key)), value(Forward<VALUE_ARGS>(value_args)...)
	 {}
	
	
	template <class KT, class VT>
	SLIB_INLINE HashTablePosition<KT, VT>::HashTablePosition(HashTableNode<KT, VT>** _entry, HashTableNode<KT, VT>** _last_entry, HashTableNode<KT, VT>* _node) noexcept
	{
		entry = _entry;
		last_entry = _last_entry;
		next = _node;
		++(*this);
	}
	
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
		node = next;
		if (node) {
			next = node->next;
			while (!next) {
				entry++;
				if (entry == last_entry) {
					break;
				}
				next = *entry;
			}
		} else {
			next = sl_null;
		}
		return *this;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_size capacityMinimum, sl_size capacityMaximum, const HASH& hash, const KEY_EQUALS& equals) noexcept
	 : m_hash(hash), m_equals(equals)
	{
		priv::hash_table::Helper::initialize(reinterpret_cast<HashTableStructBase*>(&m_table), capacityMinimum, capacityMaximum);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(HashTable<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	: m_hash(Move(other.m_hash)), m_equals(Move(other.m_equals))
	{
		priv::hash_table::Helper::move(reinterpret_cast<HashTableStructBase*>(&m_table), reinterpret_cast<HashTableStructBase*>(&(other.m_table)));
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::~HashTable() noexcept
	{
		priv::hash_table::Helper::free(&m_table);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>& HashTable<KT, VT, HASH, KEY_EQUALS>::operator=(HashTable<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	{
		priv::hash_table::Helper::free(&m_table);
		priv::hash_table::Helper::move(reinterpret_cast<HashTableStructBase*>(&m_table), reinterpret_cast<HashTableStructBase*>(&(other.m_table)));
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
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getMinimumCapacity() const noexcept
	{
		return m_table.capacityMinimum;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE void HashTable<KT, VT, HASH, KEY_EQUALS>::setMinimumCapacity(sl_size capacity) noexcept
	{
		priv::hash_table::Helper::setMinimumCapacity(reinterpret_cast<HashTableStructBase*>(&m_table), capacity);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getMaximumCapacity() const noexcept
	{
		return m_table.capacityMaximum;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE void HashTable<KT, VT, HASH, KEY_EQUALS>::setMaximumCapacity(sl_size capacity) noexcept
	{
		priv::hash_table::Helper::setMaximumCapacity(reinterpret_cast<HashTableStructBase*>(&m_table), capacity);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTableNode<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::find(const KT& key) const noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE* node = m_table.nodes[index];
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
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE* node = m_table.nodes[index];
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
		NODE* node = find(key);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		NODE* node = findKeyAndValue(key, value, value_equals);
		if (node) {
			return &(node->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::get(const KT& key, VT* value) const noexcept
	{
		NODE* node = find(key);
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
		NODE* node = find(key);
		if (node) {
			return node->value;
		} else {
			return NullValue<VT>::get();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashTable<KT, VT, HASH, KEY_EQUALS>::getValue(const KT& key, const VT& def) const noexcept
	{
		NODE* node = find(key);
		if (node) {
			return node->value;
		}
		return def;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashTable<KT, VT, HASH, KEY_EQUALS>::getValues(const KT& key) const noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		List<VT> ret;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE* node = m_table.nodes[index];
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
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		List<VT> ret;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		NODE* node = m_table.nodes[index];
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
		if (!(priv::hash_table::Helper::validateNodes(reinterpret_cast<HashTableStructBase*>(&m_table)))) {
			return sl_null;
		}
		
		sl_size capacity = m_table.capacity;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		
		NODE** nodes = m_table.nodes;
		NODE* node = nodes[index];
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
		
		node = new NODE(Forward<KEY>(key), Forward<VALUE>(value));
		if (node) {
			(m_table.count)++;
			node->hash = hash;
			node->next = nodes[index];
			nodes[index] = node;
			priv::hash_table::Helper::expand(reinterpret_cast<HashTableStructBase*>(&m_table));
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
		NODE* node = find(key);
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
		if (!(priv::hash_table::Helper::validateNodes(reinterpret_cast<HashTableStructBase*>(&m_table)))) {
			return sl_null;
		}
		NODE* node = new NODE(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		if (node) {
			sl_size capacity = m_table.capacity;
			sl_size hash = m_hash(key);
			sl_size index = hash & (capacity - 1);
			NODE** nodes = m_table.nodes;
			(m_table.count)++;
			node->hash = hash;
			node->next = nodes[index];
			nodes[index] = node;
			priv::hash_table::Helper::expand(reinterpret_cast<HashTableStructBase*>(&m_table));
			return node;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class... VALUE_ARGS>
	MapEmplaceReturn< HashTableNode<KT, VT> > HashTable<KT, VT, HASH, KEY_EQUALS>::emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept
	{
		if (!(priv::hash_table::Helper::validateNodes(reinterpret_cast<HashTableStructBase*>(&m_table)))) {
			return sl_null;
		}
		
		sl_size capacity = m_table.capacity;
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		
		NODE** nodes = m_table.nodes;
		NODE* node = nodes[index];
		while (node) {
			if (node->hash == hash && m_equals(node->key, key)) {
				return MapEmplaceReturn< HashTableNode<KT, VT> >(sl_false, node);
			}
			node = node->next;
		}
		
		node = new NODE(Forward<KEY>(key), Forward<VALUE_ARGS>(value_args)...);
		if (node) {
			(m_table.count)++;
			node->hash = hash;
			node->next = nodes[index];
			nodes[index] = node;
			priv::hash_table::Helper::expand(reinterpret_cast<HashTableStructBase*>(&m_table));
			return MapEmplaceReturn< HashTableNode<KT, VT> >(sl_true, node);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::removeAt(const HashTableNode<KT, VT>* nodeRemove) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		
		sl_size hash = nodeRemove->hash;
		sl_size index = hash & (capacity - 1);
		
		NODE** link = m_table.nodes + index;
		NODE* node = *link;
		while (node) {
			NODE* next = node->next;
			if (node == nodeRemove) {
				*link = next;
				(m_table.count)--;
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
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::remove(const KT& key, VT* outValue) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}

		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		
		NODE** link = m_table.nodes + index;
		NODE* node = *link;
		while (node) {
			NODE* next = node->next;
			if (node->hash == hash && m_equals(node->key, key)) {
				*link = next;
				(m_table.count)--;
				if (outValue) {
					*outValue = Move(node->value);
				}
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
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItems(const KT& key) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		
		NODE** link = m_table.nodes + index;
		NODE* node = *link;
		NODE* nodeDelete = sl_null;
		NODE** linkDelete = &nodeDelete;
		while ((node = *link)) {
			NODE* next = node->next;
			if (node->hash == hash && m_equals(node->key, key)) {
				*link = next;
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
	List<VT> HashTable<KT, VT, HASH, KEY_EQUALS>::removeItemsAndReturnValues(const KT& key) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		
		List<VT> ret;
		NODE** link = m_table.nodes + index;
		NODE* node = *link;
		NODE* nodeDelete = sl_null;
		NODE** linkDelete = &nodeDelete;
		while ((node = *link)) {
			NODE* next = node->next;
			if (node->hash == hash && m_equals(node->key, key)) {
				*link = next;
				ret.add_NoLock(Move(node->value));
				*linkDelete = node;
				node->next = sl_null;
				linkDelete = &(node->next);
			} else {
				link = &(node->next);
			}
			node = next;
		}
		if (!nodeDelete) {
			return sl_null;
		}
		sl_size nDelete = 0;
		while (nodeDelete) {
			node = nodeDelete;
			nodeDelete = nodeDelete->next;
			delete node;
			nDelete++;
		}
		(m_table.count) -= nDelete;
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);
		
		NODE** link = m_table.nodes + index;
		NODE* node = *link;
		while (node) {
			NODE* next = node->next;
			if (node->hash == hash && m_equals(node->key, key) && value_equals(node->value, value)) {
				*link = next;
				(m_table.count)--;
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
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) noexcept
	{
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		
		sl_size hash = m_hash(key);
		sl_size index = hash & (capacity - 1);

		NODE** link = m_table.nodes + index;
		NODE* node = *link;
		NODE* nodeDelete = sl_null;
		NODE** linkDelete = &nodeDelete;
		while (node) {
			NODE* next = node->next;
			if (node->hash == hash && m_equals(node->key, key) && value_equals(node->value, value)) {
				*link = next;
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
		sl_size capacity = m_table.capacity;
		if (capacity == 0) {
			return 0;
		}
		sl_size count = m_table.count;
		priv::hash_table::Helper::free(&m_table);
		priv::hash_table::Helper::initialize(reinterpret_cast<HashTableStructBase*>(&m_table), m_table.capacityMinimum, m_table.capacityMaximum);
		return count;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashTable<KT, VT, HASH, KEY_EQUALS>::shrink() noexcept
	{
		priv::hash_table::Helper::shrink(reinterpret_cast<HashTableStructBase*>(&m_table));
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>& other) noexcept
	{
		priv::hash_table::Helper::free(&m_table);
		m_hash = other.m_hash;
		m_equals = other.m_equals;
		sl_size capacity = other.m_table.capacity;
		priv::hash_table::Helper::initialize(reinterpret_cast<HashTableStructBase*>(&m_table), other.m_table.capacityMinimum, other.m_table.capacityMaximum);
		if (capacity == 0) {
			return sl_true;
		}
		if (!(priv::hash_table::Helper::reallocNodes(reinterpret_cast<HashTableStructBase*>(&m_table), capacity))) {
			return sl_false;
		}
		NODE** nodesTarget = m_table.nodes;
		Base::zeroMemory(nodesTarget, capacity*sizeof(NODE*));
		NODE** nodesSource = other.m_table.nodes;
		for (sl_size i = 0; i < capacity; i++) {
			NODE* nodeSource = nodesSource[i];
			if (nodeSource) {
				NODE** link = &(nodesTarget[i]);
				do {
					NODE* nodeTarget = new NODE(nodeSource->key, nodeSource->value);
					*link = nodeTarget;
					if (!nodeTarget) {
						priv::hash_table::Helper::free(&m_table);
						priv::hash_table::Helper::initialize(reinterpret_cast<HashTableStructBase*>(&m_table), other.m_table.capacityMinimum, other.m_table.capacityMaximum);
						return sl_false;
					}
					nodeTarget->hash = nodeSource->hash;
					link = &(nodeTarget->next);
					nodeSource = nodeSource->next;
				} while (nodeSource);
			}
		}
		m_table.count = other.m_table.count;
		return sl_true;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashTablePosition<KT, VT> HashTable<KT, VT, HASH, KEY_EQUALS>::begin() const noexcept
	{
		NODE** nodes = m_table.nodes;
		sl_size capacity = m_table.capacity;
		for (sl_size i = 0; i < capacity; i++) {
			NODE* node = nodes[i];
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
	void priv::hash_table::Helper::free(HashTableStruct<KT, VT>* table) noexcept
	{
		HashTableNode<KT, VT>** nodes = table->nodes;
		if (nodes) {
			sl_size capacity = table->capacity;
			for (sl_size i = 0; i < capacity; i++) {
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
