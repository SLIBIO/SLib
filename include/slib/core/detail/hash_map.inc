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
	
	template <class KT, class VT>
	HashMapPosition<KT, VT>::HashMapPosition() noexcept
	 : entry(sl_null)
	 {}
	
	template <class KT, class VT>
	HashMapPosition<KT, VT>::HashMapPosition(HashEntry<KT, VT>* _entry) noexcept
	 : entry(_entry)
	 {}

	template <class KT, class VT>
	HashMapPosition<KT, VT>& HashMapPosition<KT, VT>::operator=(HashEntry<KT, VT>* _entry) noexcept
	{
		entry = _entry;
		return *this;
	}
	
	template <class KT, class VT>
	Pair<KT, VT>& HashMapPosition<KT, VT>::operator*() const noexcept
	{
		return entry->data;
	}
	
	template <class KT, class VT>
	sl_bool HashMapPosition<KT, VT>::operator==(const HashMapPosition& other) const noexcept
	{
		return entry == other.entry;
	}
	
	template <class KT, class VT>
	sl_bool HashMapPosition<KT, VT>::operator!=(const HashMapPosition& other) const noexcept
	{
		return entry != other.entry;
	}
	
	template <class KT, class VT>
	HashMapPosition<KT, VT>::operator HashEntry<KT, VT>*() const noexcept
	{
		return entry;
	}
	
	template <class KT, class VT>
	HashMapPosition<KT, VT>& HashMapPosition<KT, VT>::operator++() noexcept
	{
		entry = entry->next;
		return *this;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap() noexcept
	 {}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap(sl_uint32 capacity) noexcept
	 : table(capacity)
	 {}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap(sl_uint32 capacity, const HASH& hash) noexcept
	 : table(capacity, hash)
	 {}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap(sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals) noexcept
	 : table(capacity, hash, key_equals)
	 {}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap(HashMap<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	 : table(Move(other.table))
	 {}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMap<KT, VT, HASH, KEY_EQUALS>& HashMap<KT, VT, HASH, KEY_EQUALS>::operator=(HashMap<KT, VT, HASH, KEY_EQUALS>&& other) noexcept
	{
		table = Move(other.table);
		return *this;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>* HashMap<KT, VT, HASH, KEY_EQUALS>::create(sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals) noexcept
	{
		HashMap<KT, VT, HASH, KEY_EQUALS>* ret = new HashMap<KT, VT, HASH, KEY_EQUALS>(capacity, hash, key_equals);
		if (ret) {
			if (capacity > 0) {
				if (ret->table.getCapacity() > 0) {
					return ret;
				}
				delete ret;
			} else {
				return ret;
			}
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashMap<KT, VT, HASH, KEY_EQUALS>::operator[](const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return entry->data.value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::_getCount() const noexcept
	{
		return table.getCount();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::getCount() const noexcept
	{
		return table.getCount();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::isEmpty() const noexcept
	{
		return table.getCount() == 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::isNotEmpty() const noexcept
	{
		return table.getCount() > 0;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashMap<KT, VT, HASH, KEY_EQUALS>::_getItemPointer(const KT& key) const noexcept
	{
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return &(entry->data.value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashMap<KT, VT, HASH, KEY_EQUALS>::getItemPointer(const KT& key) const noexcept
	{
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return &(entry->data.value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			if (_out) {
				*_out = entry->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::get(const KT& key, VT* _out) const noexcept
	{
		ObjectLocker lock(this);
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			if (_out) {
				*_out = entry->data.value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashMap<KT, VT, HASH, KEY_EQUALS>::getValue_NoLock(const KT& key) const noexcept
	{
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return entry->data.value;
		} else {
			return VT();
		}
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashMap<KT, VT, HASH, KEY_EQUALS>::getValue(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return entry->data.value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashMap<KT, VT, HASH, KEY_EQUALS>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return entry->data.value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashMap<KT, VT, HASH, KEY_EQUALS>::getValue(const KT& key, const VT& def) const noexcept
	{
		HashEntry<KT, VT>* entry = table.find(key);
		if (entry) {
			return entry->data.value;
		} else {
			return def;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::_getValues(const KT& key) const noexcept
	{
		return table.getValues(key);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getValues_NoLock(const KT& key) const noexcept
	{
		return table.getValues(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getValues(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return table.getValues(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::_put(const KT& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			HashEntry<KT, VT>* entry;
			if (table.put(key, value, mode, &entry)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Hash, entry, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return table.put(key, value, mode);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::_put(const KT& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			HashEntry<KT, VT>* entry;
			if (table.put(key, Move(value), mode, &entry)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Hash, entry, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return table.put(key, Move(value), mode);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::_put(KT&& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			HashEntry<KT, VT>* entry;
			if (table.put(Move(key), value, mode, &entry)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Hash, entry, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return table.put(Move(key), value, mode);
		}
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::_put(KT&& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		if (pPosition) {
			HashEntry<KT, VT>* entry;
			if (table.put(Move(key), Move(value), mode, &entry)) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Hash, entry, sl_null);
				return sl_true;
			}
			return sl_false;
		} else {
			return table.put(Move(key), Move(value), mode);
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode, HashEntry<KT, VT>** ppEntry) noexcept
	{
		return table.put(Forward<KEY>(key), Forward<VALUE>(value), mode, ppEntry);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::put(KEY&& key, VALUE&& value, MapPutMode mode, HashEntry<KT, VT>** ppEntry) noexcept
	{
		ObjectLocker lock(this);
		return table.put(Forward<KEY>(key), Forward<VALUE>(value), mode, ppEntry);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue_NoLock(KEY&& key, VALUE&& value, HashEntry<KT, VT>** ppEntry, const VALUE_EQUALS& value_equals) noexcept
	{
		return table.addIfNewKeyAndValue(Forward<KEY>(key), Forward<VALUE>(value), ppEntry, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class KEY, class VALUE, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue(KEY&& key, VALUE&& value, HashEntry<KT, VT>** ppEntry, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return table.addIfNewKeyAndValue(Forward<KEY>(key), Forward<VALUE>(value), ppEntry, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::_remove(const KT& key, VT* outValue) noexcept
	{
		return table.remove(key, outValue);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::remove_NoLock(const KT& key, VT* outValue) noexcept
	{
		return table.remove(key, outValue);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::remove(const KT& key, VT* outValue) noexcept
	{
		ObjectLocker lock(this);
		return table.remove(key, outValue);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::_removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		return table.removeItems(key, outValues);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItems_NoLock(const KT& key, List<VT>* outValues) noexcept
	{
		return table.removeItems(key, outValues);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		ObjectLocker lock(this);
		return table.removeItems(key, outValues);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue, const VALUE_EQUALS& value_equals) noexcept
	{
		return table.removeKeyAndValue(key, value, outValue, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return table.removeKeyAndValue(key, value, outValue, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals) noexcept
	{
		return table.removeItemsByKeyAndValue(key, value, outValues, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues, const VALUE_EQUALS& value_equals) noexcept
	{
		ObjectLocker lock(this);
		return table.removeItemsByKeyAndValue(key, value, outValues, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::_removeAll() noexcept
	{
		return table.removeAll();
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeAll_NoLock() noexcept
	{
		return table.removeAll();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeAll() noexcept
	{
		ObjectLocker lock(this);
		return table.removeAll();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashMap<KT, VT, HASH, KEY_EQUALS>::_removeAt(const MapPosition<KT, VT>& position) noexcept
	{
		table.removeEntry(reinterpret_cast<HashEntry<KT, VT>*>(position.node));
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashMap<KT, VT, HASH, KEY_EQUALS>::removeAt_NoLock(const HashEntry<KT, VT>* entry) noexcept
	{
		table.removeEntry(const_cast<HashEntry<KT, VT>*>(entry));
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	void HashMap<KT, VT, HASH, KEY_EQUALS>::removeAt(const HashEntry<KT, VT>* entry) noexcept
	{
		ObjectLocker lock(this);
		table.removeEntry(const_cast<HashEntry<KT, VT>*>(entry));
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::_find(const KT& key, MapPosition<KT, VT>* pPosition) const noexcept
	{
		if (pPosition) {
			HashEntry<KT, VT>* entry = table.find(key);
			if (entry) {
				*pPosition = MapPosition<KT, VT>(MapContainerType::Hash, entry, sl_null);
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			return table.find(key) != sl_null;
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashEntry<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::find_NoLock(const KT& key) const noexcept
	{
		return table.find(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashEntry<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::find(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return table.find(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	HashEntry<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		return table.findKeyAndValue(key, value, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	HashEntry<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		return table.findKeyAndValue(key, value, value_equals);
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::contains_NoLock(const KT& key) const noexcept
	{
		return table.find(key) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::contains(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return table.find(key) != sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		return table.findKeyAndValue(key, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class VALUE, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals) const noexcept
	{
		ObjectLocker lock(this);
		return table.findKeyAndValue(key, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashEntry<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::getFirstEntry() const noexcept
	{
		return table.getFirstEntry();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashEntry<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::getLastEntry() const noexcept
	{
		return table.getLastEntry();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	IMap<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::_duplicate() const noexcept
	{
		return HashMap::duplicate_NoLock();
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>* HashMap<KT, VT, HASH, KEY_EQUALS>::duplicate_NoLock() const noexcept
	{
		HashMap<KT, VT, HASH, KEY_EQUALS>* ret = new HashMap<KT, VT, HASH, KEY_EQUALS>;
		if (ret) {
			if (ret->table.copyFrom(&table)) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>* HashMap<KT, VT, HASH, KEY_EQUALS>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return HashMap::duplicate_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<KT> HashMap<KT, VT, HASH, KEY_EQUALS>::_getAllKeys() const noexcept
	{
		return HashMap::getAllKeys_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<KT> HashMap<KT, VT, HASH, KEY_EQUALS>::getAllKeys_NoLock() const noexcept
	{
		if (table.getCount() == 0) {
			return sl_null;
		}
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			HashEntry<KT, VT>* entry = table.getFirstEntry();
			while (entry) {
				if (!(ret->add_NoLock(entry->data.key))) {
					delete ret;
					return sl_null;
				}
				entry = entry->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<KT> HashMap<KT, VT, HASH, KEY_EQUALS>::getAllKeys() const noexcept
	{
		ObjectLocker lock(this);
		return HashMap::getAllKeys_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::_getAllValues() const noexcept
	{
		return HashMap::getAllValues_NoLock();
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getAllValues_NoLock() const noexcept
	{
		if (table.getCount() == 0) {
			return sl_null;
		}
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			HashEntry<KT, VT>* entry = table.getFirstEntry();
			while (entry) {
				if (!(ret->add_NoLock(entry->data.value))) {
					delete ret;
					return sl_null;
				}
				entry = entry->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getAllValues() const noexcept
	{
		ObjectLocker lock(this);
		return HashMap::getAllValues_NoLock();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_EQUALS>::_toList() const noexcept
	{
		return HashMap::toList_NoLock();
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_EQUALS>::toList_NoLock() const noexcept
	{
		if (table.getCount() == 0) {
			return sl_null;
		}
		CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
		if (ret) {
			HashEntry<KT, VT>* entry = table.getFirstEntry();
			while (entry) {
				if (!(ret->add_NoLock(entry->data))) {
					delete ret;
					return sl_null;
				}
				entry = entry->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_EQUALS>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return HashMap::toList_NoLock();
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	MapPosition<KT, VT> HashMap<KT, VT, HASH, KEY_EQUALS>::_getFirst(Referable* ref) const noexcept
	{
		return MapPosition<KT, VT>(MapContainerType::Hash, table.getFirstEntry(), ref);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMapPosition<KT, VT> HashMap<KT, VT, HASH, KEY_EQUALS>::begin() const noexcept
	{
		return table.getFirstEntry();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	SLIB_INLINE HashMapPosition<KT, VT> HashMap<KT, VT, HASH, KEY_EQUALS>::end() const noexcept
	{
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::copyFrom(const HashMap<KT, VT, HASH, KEY_EQUALS>* other) noexcept
	{
		return table.copyFrom(&(other->table));
	}
	
}
