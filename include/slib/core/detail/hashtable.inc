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
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::HashTable(sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& equals) : m_hash(hash), m_equals(equals)
	{
		if (capacity < _SLIB_HASHTABLE_MIN_CAPACITY) {
			capacity = _SLIB_HASHTABLE_MIN_CAPACITY;
		} else if (capacity > _SLIB_HASHTABLE_MAX_CAPACITY) {
			capacity = _SLIB_HASHTABLE_MAX_CAPACITY;
		} else {
			capacity = Math::roundUpToPowerOfTwo32(capacity);
		}
		m_nCapacityMin = capacity;
		_init();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashTable<KT, VT, HASH, KEY_EQUALS>::~HashTable()
	{
		_free();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getCount() const
	{
		return m_nSize;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::getCapacity() const
	{
		return m_nCapacity;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::getFirstEntry() const
	{
		return m_firstEntry;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::getLastEntry() const
	{
		return m_lastEntry;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::search(const KT& key) const
	{
		if (m_nCapacity == 0) {
			return sl_null;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->key, key)) {
				return entry;
			}
			entry = entry->chain;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	HashEntry<KT, VT>* HashTable<KT, VT, HASH, KEY_EQUALS>::searchKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		if (m_nCapacity == 0) {
			return sl_null;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->key, key) && value_equals(entry->value, value)) {
				return entry;
			}
			entry = entry->chain;
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::get(const KT& key, VT* value) const
	{
		Entry* entry = search(key);
		if (entry) {
			if (value) {
				*value = entry->value;
			}
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointer(const KT& key) const
	{
		Entry* entry = search(key);
		if (entry) {
			return &(entry->value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	VT* HashTable<KT, VT, HASH, KEY_EQUALS>::getItemPointerByKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		Entry* entry = searchKeyAndValue(key, value, value_equals);
		if (entry) {
			return &(entry->value);
		}
		return sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashTable<KT, VT, HASH, KEY_EQUALS>::getValues(const KT& key) const
	{
		List<VT> ret;
		if (m_nCapacity == 0) {
			return ret;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->key, key)) {
				ret.add_NoLock(entry->value);
			}
			entry = entry->chain;
		}
		return ret;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	List<VT> HashTable<KT, VT, HASH, KEY_EQUALS>::getValuesByKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		List<VT> ret;
		if (m_nCapacity == 0) {
			return ret;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->key, key) && value_equals(entry->value, value)) {
				ret.add_NoLock(entry->value);
			}
			entry = entry->chain;
		}
		return ret;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::_addEntry(sl_uint32 hash, const KT& key, const VT& value)
	{
		sl_uint32 index = hash & (m_nCapacity - 1);
		
		Entry* entry = new Entry;
		if (!entry) {
			return sl_false;
		}
		entry->key = key;
		entry->value = value;
		entry->hash = hash;
		entry->chain = m_table[index];
		Entry* last = m_lastEntry;
		if (last) {
			last->next = entry;
		}
		m_lastEntry = entry;
		entry->before = last;
		if (!m_firstEntry) {
			m_firstEntry = entry;
		}
		entry->next = sl_null;

		m_table[index] = entry;
		m_nSize++;
	
		if (m_nSize >= m_nThresholdUp) {
			// double capacity
			Entry** tableOld = m_table;
			sl_uint32 n = m_nCapacity;
			if (_createTable(n + n)) {
				Entry** table = m_table;
				for (sl_uint32 i = 0; i < n; i++) {
					Entry* entry = tableOld[i];
					table[i] = sl_null;
					table[i | n] = sl_null;
					if (entry) {
						sl_uint32 highBitBefore = entry->hash & n;
						Entry* broken = sl_null;
						table[i | highBitBefore] = entry;
						while (Entry* chain = entry->chain) {
							sl_uint32 highBitChain = chain->hash & n;
							if (highBitBefore != highBitChain) {
								if (broken) {
									broken->chain = chain;
								} else {
									table[i | highBitChain] = chain;
								}
								broken = entry;
								highBitBefore = highBitChain;
							}
							entry = chain;
						}
						if (broken) {
							broken->chain = sl_null;
						}
					}
				}
				Base::freeMemory(tableOld);
			}
		}
		return sl_true;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
	{
		if (pFlagExist) {
			*pFlagExist = sl_false;
		}
		if (m_nCapacity == 0) {
			return sl_false;
		}

		sl_uint32 hash = m_hash(key);

		if (mode != MapPutMode::AddAlways) {
			sl_uint32 index = hash & (m_nCapacity - 1);
			Entry* entry = m_table[index];
			while (entry) {
				if (entry->hash == hash && m_equals(entry->key, key)) {
					if (pFlagExist) {
						*pFlagExist = sl_true;
					}
					if (mode == MapPutMode::AddNew) {
						return sl_false;
					}
					entry->value = value;
					return sl_true;
				}
				entry = entry->chain;
			}
			if (mode == MapPutMode::ReplaceExisting) {
				return sl_false;
			}
		}

		return _addEntry(hash, key, value);

	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_EQUALS& value_equals)
	{
		if (pFlagExist) {
			*pFlagExist = sl_false;
		}
		if (m_nCapacity == 0) {
			return sl_false;
		}

		sl_uint32 hash = m_hash(key);
	
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && m_equals(entry->key, key) && value_equals(entry->value, value)) {
				if (pFlagExist) {
					*pFlagExist = sl_true;
				}
				return sl_false;
			}
			entry = entry->chain;
		}

		return _addEntry(hash, key, value);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashTable<KT, VT, HASH, KEY_EQUALS>::_compact()
	{
		Entry* entry;
		Entry** link;
		if (m_nSize <= m_nThresholdDown) {
			// half capacity
			Entry** tableOld = m_table;
			sl_uint32 n = m_nCapacity >> 1;
			if (_createTable(n)) {
				Entry** table = m_table;
				for (sl_uint32 i = 0; i < n; i++) {
					table[i] = tableOld[i];
					link = table + i;
					while ((entry = *link)) {
						link = &(entry->chain);
					}
					*link = tableOld[i | n];
				}
				Base::freeMemory(tableOld);
			}
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashTable<KT, VT, HASH, KEY_EQUALS>::_removeEntry(Entry* entry)
	{
		m_nSize--;
		Entry* before = entry->before;
		Entry* next = entry->next;
		if (before) {
			before->next = next;
		} else {
			m_firstEntry = next;
		}
		if (next) {
			next->before = before;
		} else {
			m_lastEntry = before;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::remove(const KT& key, VT* outValue)
	{
		if (m_nCapacity == 0) {
			return 0;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry;
		Entry** link = m_table + index;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->key, key)) {
				*link = entry->chain;
				_removeEntry(entry);
				if (outValue) {
					*outValue = entry->value;
				}
				_compact();
				delete entry;
				return sl_true;
			} else {
				link = &(entry->chain);
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItems(const KT& key, List<VT>* outValues)
	{
		if (m_nCapacity == 0) {
			return 0;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry;
		Entry** link = m_table + index;
		Entry* entryDelete = sl_null;
		Entry** linkDelete = &entryDelete;
		sl_size oldSize = m_nSize;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->key, key)) {
				*link = entry->chain;
				_removeEntry(entry);
				if (outValues) {
					outValues->add_NoLock(entry->value);
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
		_compact();
		while (entryDelete) {
			entry = entryDelete;
			entryDelete = entryDelete->chain;
			delete entry;
		}
		return oldSize - m_nSize;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue(const KT& key, const _VT& value, VT* outValue, const VALUE_EQUALS& value_equals)
	{
		if (m_nCapacity == 0) {
			return 0;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry;
		Entry** link = m_table + index;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->key, key) && value_equals(entry->value, value)) {
				*link = entry->chain;
				_removeEntry(entry);
				if (outValue) {
					*outValue = entry->value;
				}
				_compact();
				delete entry;
				return sl_true;
			} else {
				link = &(entry->chain);
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
	{
		if (m_nCapacity == 0) {
			return 0;
		}
		sl_uint32 hash = m_hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		Entry* entry;
		Entry** link = m_table + index;
		Entry* entryDelete = sl_null;
		Entry** linkDelete = &entryDelete;
		sl_size oldSize = m_nSize;
		while ((entry = *link)) {
			if (entry->hash == hash && m_equals(entry->key, key) && value_equals(entry->value, value)) {
				*link = entry->chain;
				_removeEntry(entry);
				if (outValues) {
					outValues->add_NoLock(entry->value);
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
		_compact();
		while (entryDelete) {
			entry = entryDelete;
			entryDelete = entryDelete->chain;
			delete entry;
		}
		return oldSize - m_nSize;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashTable<KT, VT, HASH, KEY_EQUALS>::removeAll()
	{
		if (m_nCapacity == 0) {
			return 0;
		}
		sl_size oldSize = m_nSize;
		_free();
		_init();
		return oldSize;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other)
	{
		_free();
		m_nCapacityMin = other->m_nCapacityMin;
		_init();
		if (other->m_nCapacity == 0) {
			return sl_false;
		}
		if (m_nCapacity == 0) {
			return sl_true;
		}
		
		Entry* entry = other->m_firstEntry;
		while (entry) {
			if (!(_addEntry(entry->hash, entry->key, entry->value))) {
				return sl_false;
			}
			entry = entry->next;
		}
		
		return sl_true;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashTable<KT, VT, HASH, KEY_EQUALS>::_init()
	{
		m_nSize = 0;
		sl_uint32 capacity = m_nCapacityMin;
		if (_createTable(capacity)) {
			for (sl_uint32 i = 0; i < capacity; i++) {
				m_table[i] = sl_null;
			}
		} else {
			m_table = sl_null;
			m_nCapacity = 0;
			m_nThresholdUp = 0;
			m_nThresholdDown = 0;
		}
		m_firstEntry = sl_null;
		m_lastEntry = sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashTable<KT, VT, HASH, KEY_EQUALS>::_free()
	{
		Entry** table = m_table;
		sl_uint32 nCapacity = m_nCapacity;
		m_table = sl_null;
		m_nCapacity = 0;
		m_firstEntry = sl_null;
		m_lastEntry = sl_null;
		if (table) {
			for (sl_uint32 i = 0; i < nCapacity; i++) {
				Entry* entry = table[i];
				while (entry) {
					Entry* chain = entry->chain;
					delete entry;
					entry = chain;
				}
			}
			Base::freeMemory(table);
		}
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashTable<KT, VT, HASH, KEY_EQUALS>::_createTable(sl_uint32 capacity)
	{
		if (capacity > _SLIB_HASHTABLE_MAX_CAPACITY || capacity < m_nCapacityMin) {
			return sl_false;
		}
		Entry** table = (Entry**)(Base::createMemory(sizeof(Entry*)*capacity));
		if (table) {
			m_table = table;
			m_nCapacity = capacity;
			m_nThresholdUp = (sl_uint32)(_SLIB_HASHTABLE_LOAD_FACTOR_UP * capacity);
			m_nThresholdDown = (sl_uint32)(_SLIB_HASHTABLE_LOAD_FACTOR_DOWN * capacity);
			return sl_true;
		} else {
			return sl_false;
		}
	}

}
