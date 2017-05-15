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
	
	class HashBaseEntry
	{
	public:
		HashBaseEntry* chain;
		HashBaseEntry* before;
		HashBaseEntry* next;
		sl_uint32 hash;
	};
	
	struct HashBaseTable
	{
		HashBaseEntry** entries;
		sl_uint32 capacity;
		
		sl_size count;
		HashBaseEntry* firstEntry;
		HashBaseEntry* lastEntry;
		
		sl_uint32 thresholdUp;
		sl_uint32 thresholdDown;
	};
	
	class _priv_HashTable
	{
	public:
		typedef HashBaseEntry Entry;
		typedef HashBaseTable Table;
		
	public:
		static void init(Table* table) noexcept;
		
		static void init(Table* table, sl_uint32 capacity) noexcept;
		
		static sl_bool validateEntries(Table* table) noexcept;
		
		static sl_bool createEntries(Table* table, sl_uint32 capacity) noexcept;
		
		static void free(Table* table) noexcept;
		
		static void add(Table* table, Entry* entry, sl_uint32 hash) noexcept;
		
		static void remove(Table* table, Entry* entry) noexcept;
		
		static void expand(Table* table) noexcept;
		
		static void compact(Table* table) noexcept;
		
	};
	
	template <class KT, class VT>
	void _priv_HashTable_free(HashTableStruct<KT, VT>* table) noexcept;

	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	SLIB_INLINE HashEntry<KT, VT>::HashEntry(KEY&& _key, VALUE&& _value) noexcept
	 : data(Forward<KEY>(_key), Forward<VALUE>(_value))
	{
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable() noexcept
	{
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&m_table));
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity) noexcept
	{
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class HASH_ARG>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity, HASH_ARG&& hash) noexcept
	 : m_hash(Forward<HASH_ARG>(hash))
	{
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&m_table), capacity);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class HASH_ARG, class KEY_EQUALS_ARG>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity, HASH_ARG&& hash, KEY_EQUALS_ARG&& equals) noexcept
	 : m_hash(Forward<HASH_ARG>(hash)), m_equals(Forward<KEY_EQUALS_ARG>(equals))
	{
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&m_table), capacity);
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
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&(other.m_table)));
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>& HashTable<KT, VT, HASH, KEY_EQUALS>::operator=(HashTable<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	{
		Base::copyMemory(&m_table, &(other.m_table), sizeof(Table));
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&(other.m_table)));
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
	SLIB_INLINE HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::getFirstEntry() const noexcept
	{
		return m_table.firstEntry;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::getLastEntry() const noexcept
	{
		return m_table.lastEntry;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::search(const KT& key) const noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Entry* entry = m_table.entries[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->data.key, key)) {
				return entry;
			}
			entry = entry->chain;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::searchKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_null;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (capacity - 1);
		Entry* entry = m_table.entries[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->data.key, key) && value_equals(entry->data.value, value)) {
				return entry;
			}
			entry = entry->chain;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::get(const KT& key, VT* value) const noexcept
	{
		Entry* entry = search(key);
		if (entry) {
			if (value) {
				*value = entry->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointer(const KT& key) const noexcept
	{
		Entry* entry = search(key);
		if (entry) {
			return &(entry->data.value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		Entry* entry = searchKeyAndValue(key, value, value_equals);
		if (entry) {
			return &(entry->data.value);
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
		Entry* entry = m_table.entries[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->data.key, key)) {
				ret.add_NoLock(entry->data.value);
			}
			entry = entry->chain;
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
		Entry* entry = m_table.entries[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->data.key, key) && value_equals(entry->data.value, value)) {
				ret.add_NoLock(entry->data.value);
			}
			entry = entry->chain;
		}
		return ret;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::put(KEY&& key, VALUE&& value, MapPutMode mode, HashEntry<KT, VT>* ppEntry) noexcept
	{
		
		if (!(_priv_HashTable::validateEntries(reinterpret_cast<HashBaseTable*>(&m_table)))) {
			if (ppEntry) {
				*ppEntry = sl_null;
			}
			return sl_false;
		}

		sl_uint32 capacity = m_table.capacity;
		
		sl_uint32 hash = m_hash(key);

		if (mode != MapPutMode::AddAlways) {
			sl_uint32 index = hash & (capacity - 1);
			Entry* entry = m_table.entries[index];
			while (entry) {
				if (entry->hash == hash && m_equals(entry->data.key, key)) {
					if (mode != MapPutMode::AddAlways) {
						if (ppEntry) {
							*ppEntry = entry;
						}
						if (mode == MapPutMode::AddNew) {
							return sl_false;
						}
						entry->data.value = Forward<VALUE>(value);
						return sl_true;
					}
					break;
				}
				entry = entry->chain;
			}
			if (mode == MapPutMode::ReplaceExisting) {
				if (ppEntry) {
					*ppEntry = sl_null;
				}
				return sl_false;
			}
		}

		Entry* newEntry = new Entry(Forward<KEY>(key), Forward<VALUE>(value));
		if (ppEntry) {
			*ppEntry = newEntry;
		}
		if (newEntry) {
			_priv_HashTable::add(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(newEntry), hash);
			_priv_HashTable::expand(reinterpret_cast<HashBaseTable*>(&m_table));
			return sl_true;
		}
		return sl_false;

	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE, class VALUE_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue(KEY&& key, VALUE&& value, HashEntry<KT, VT>* ppEntry, const VALUE_EQUALS& value_equals) noexcept
	{
		
		if (!(_priv_HashTable::validateEntries(reinterpret_cast<HashBaseTable*>(&m_table)))) {
			if (ppEntry) {
				*ppEntry = sl_null;
			}
			return sl_false;
		}
		
		sl_uint32 capacity = m_table.capacity;

		sl_uint32 hash = m_hash(key);
	
		sl_uint32 index = hash & (capacity - 1);
		Entry* entry = m_table.entries[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->data.key, key) && value_equals(entry->data.value, value)) {
				if (ppEntry) {
					*ppEntry = entry;
				}
				return sl_false;
			}
			entry = entry->chain;
		}

		Entry* newEntry = new Entry(Forward<KEY>(key), Forward<VALUE>(value));
		if (ppEntry) {
			*ppEntry = newEntry;
		}
		if (newEntry) {
			_priv_HashTable::add(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(newEntry), hash);
			_priv_HashTable::expand(reinterpret_cast<HashBaseTable*>(&m_table));
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::removeEntry(const HashEntry<KT, VT>* entryRemove) noexcept
	{
		sl_uint32 capacity = m_table.capacity;
		if (capacity == 0) {
			return sl_false;
		}
		
		sl_uint32 hash = entryRemove->hash;
		sl_uint32 index = hash & (capacity - 1);
		
		Entry* entry;
		Entry** link = m_table.entries + index;
		while ((entry = *link)) {
			if (entry == entryRemove) {
				*link = entry->chain;
				_priv_HashTable::remove(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(entry));
				_priv_HashTable::compact(reinterpret_cast<HashBaseTable*>(&m_table));
				delete entry;
				return sl_true;
			} else {
				link = &(entry->chain);
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
		Entry* entry;
		Entry** link = m_table.entries + index;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->data.key, key)) {
				*link = entry->chain;
				_priv_HashTable::remove(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(entry));
				if (outValue) {
					*outValue = Move(entry->data.value);
				}
				_priv_HashTable::compact(reinterpret_cast<HashBaseTable*>(&m_table));
				delete entry;
				return sl_true;
			} else {
				link = &(entry->chain);
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
		Entry* entry;
		Entry** link = m_table.entries + index;
		Entry* entryDelete = sl_null;
		Entry** linkDelete = &entryDelete;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->data.key, key)) {
				*link = entry->chain;
				_priv_HashTable::remove(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(entry));
				if (outValues) {
					outValues->add_NoLock(Move(entry->data.value));
				}
				*linkDelete = entry;
				entry->chain = sl_null;
				linkDelete = &(entry->chain);
			} else {
				link = &(entry->chain);
			}
		}
		if (!entryDelete) {
			return 0;
		}
		_priv_HashTable::compact(reinterpret_cast<HashBaseTable*>(&m_table));
		sl_size nDelete = 0;
		while (entryDelete) {
			entry = entryDelete;
			entryDelete = entryDelete->chain;
			delete entry;
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
		Entry* entry;
		Entry** link = m_table.entries + index;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->data.key, key) && value_equals(entry->data.value, value)) {
				*link = entry->chain;
				_priv_HashTable::remove(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(entry));
				if (outValue) {
					*outValue = Move(entry->data.value);
				}
				_priv_HashTable::compact(reinterpret_cast<HashBaseTable*>(&m_table));
				delete entry;
				return sl_true;
			} else {
				link = &(entry->chain);
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
		Entry* entry;
		Entry** link = m_table.entries + index;
		Entry* entryDelete = sl_null;
		Entry** linkDelete = &entryDelete;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->data.key, key) && value_equals(entry->data.value, value)) {
				*link = entry->chain;
				_priv_HashTable::remove(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(entry));
				if (outValues) {
					outValues->add_NoLock(Move(entry->data.value));
				}
				*linkDelete = entry;
				entry->chain = sl_null;
				linkDelete = &(entry->chain);
			} else {
				link = &(entry->chain);
			}
		}
		if (!entryDelete) {
			return 0;
		}
		_priv_HashTable::compact(reinterpret_cast<HashBaseTable*>(&m_table));
		sl_size nDelete = 0;
		while (entryDelete) {
			entry = entryDelete;
			entryDelete = entryDelete->chain;
			delete entry;
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
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&m_table));
		return count;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other) noexcept
	{
		_priv_HashTable_free(&m_table);
		m_hash = other->m_hash;
		m_equals = other->m_equals;
		_priv_HashTable::init(reinterpret_cast<HashBaseTable*>(&m_table), other->m_table.capacity);
		if (m_table.capacity == 0) {
			return sl_false;
		}
		Entry* entry = other->m_table.firstEntry;
		while (entry) {
			Entry* newEntry = new Entry(entry->data.key, entry->data.value);
			if (!newEntry) {
				return sl_false;
			}
			_priv_HashTable::add(reinterpret_cast<HashBaseTable*>(&m_table), reinterpret_cast<HashBaseEntry*>(entry), entry->hash);
			entry = entry->next;
		}
		return sl_true;
	}
	
	
	template <class KT, class VT>
	void _priv_HashTable_free(HashTableStruct<KT, VT>* table) noexcept
	{
		HashEntry<KT, VT>** entries = table->entries;
		sl_uint32 capacity = table->capacity;
		for (sl_uint32 i = 0; i < capacity; i++) {
			HashEntry<KT, VT>* entry = entries[i];
			while (entry) {
				HashEntry<KT, VT>* chain = entry->chain;
				delete entry;
				entry = chain;
			}
		}
		_priv_HashTable::free(reinterpret_cast<HashBaseTable*>(table));
	}

}
