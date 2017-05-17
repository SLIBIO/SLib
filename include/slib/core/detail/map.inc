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
	SLIB_INLINE MapPosition<KT, VT>::MapPosition() noexcept
	 : node(sl_null), containerType(MapContainerType::Hash)
	 {}
	
	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT>::MapPosition(sl_null_t) noexcept
	 : node(sl_null), containerType(MapContainerType::Hash)
	 {}
	
	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT>::MapPosition(MapContainerType _containerType, void* _node, Referable* _ref) noexcept
	 : node(_node), containerType(_containerType), ref(_ref)
	 {}

	template <class KT, class VT>
	SLIB_INLINE Pair<KT, VT>& MapPosition<KT, VT>::operator*() const noexcept
	{
		if (containerType == MapContainerType::Hash) {
			return (reinterpret_cast<HashEntry<KT, VT>*>(node))->data;
		} else {
			return (reinterpret_cast<RedBlackTreeNode<KT, VT>*>(node))->data;
		}
	}
	
	template <class KT, class VT>
	SLIB_INLINE sl_bool MapPosition<KT, VT>::operator==(const MapPosition& other) const noexcept
	{
		return node == other.node;
	}
	
	template <class KT, class VT>
	SLIB_INLINE sl_bool MapPosition<KT, VT>::operator!=(const MapPosition& other) const noexcept
	{
		return node != other.node;
	}
	
	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT>::operator sl_bool() const noexcept
	{
		return node != sl_null;
	}
	
	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT>& MapPosition<KT, VT>::operator++() noexcept
	{
		if (containerType == MapContainerType::Hash) {
			node = (reinterpret_cast<HashEntry<KT, VT>*>(node))->next;
		} else {
			node = (reinterpret_cast<RedBlackTreeNode<KT, VT>*>(node))->getNext();
		}
		return *this;
	}
	
	
	template <class KT, class VT>
	SLIB_INLINE IMap<KT, VT>::IMap() noexcept
	 {}

	template <class KT, class VT>
	IMap<KT, VT>* IMap<KT, VT>::createDefault() noexcept
	{
		return HashMap<KT, VT>::create();
	}
	
	template <class KT, class VT>
	sl_size IMap<KT, VT>::getCount() const noexcept
	{
		return _getCount();
	}

	template <class KT, class VT>
	sl_bool IMap<KT, VT>::isEmpty() const noexcept
	{
		return _getCount() == 0;
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::isNotEmpty() const noexcept
	{
		return _getCount() != 0;
	}
	
	template <class KT, class VT>
	VT* IMap<KT, VT>::getItemPointer(const KT& key) const noexcept
	{
		return _getItemPointer(key);
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		VT* p = _getItemPointer(key);
		if (p) {
			if (_out) {
				*_out = *p;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::get(const KT& key, VT* _out) const noexcept
	{
		ObjectLocker lock(this);
		VT* p = _getItemPointer(key);
		if (p) {
			if (_out) {
				*_out = *p;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue_NoLock(const KT& key) const noexcept
	{
		VT* p = _getItemPointer(key);
		if (p) {
			return *p;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		VT* p = _getItemPointer(key);
		if (p) {
			return *p;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		VT* p = _getItemPointer(key);
		if (p) {
			return *p;
		}
		return def;
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue(const KT& key, const VT& def) const noexcept
	{
		ObjectLocker lock(this);
		VT* p = _getItemPointer(key);
		if (p) {
			return *p;
		}
		return def;
	}
	
	template <class KT, class VT>
	List<VT> IMap<KT, VT>::getValues_NoLock(const KT& key) const noexcept
	{
		return _getValues(key);
	}

	template <class KT, class VT>
	List<VT> IMap<KT, VT>::getValues(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return _getValues(key);
	}
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	sl_bool IMap<KT, VT>::put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		return _put(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	sl_bool IMap<KT, VT>::put(KEY&& key, VALUE&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		ObjectLocker lock(this);
		return _put(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
	}
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	void IMap<KT, VT>::putAll_NoLock(IMap<KEY, VALUE>* other, MapPutMode mode) noexcept
	{
		if (!other) {
			return;
		}
		if (this == other) {
			return;
		}
		for (auto& pair : *other) {
			_put(pair.key, pair.value, mode, sl_null);
		}
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	void IMap<KT, VT>::putAll(IMap<KEY, VALUE>* other, MapPutMode mode) noexcept
	{
		ObjectLocker lock(this);
		putAll_NoLock(other, mode);
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::remove_NoLock(const KT& key, VT* outValue) noexcept
	{
		return _remove(key, outValue);
	}

	template <class KT, class VT>
	sl_bool IMap<KT, VT>::remove(const KT& key, VT* outValue) noexcept
	{
		ObjectLocker lock(this);
		return _remove(key, outValue);
	}
	
	template <class KT, class VT>
	sl_size IMap<KT, VT>::removeItems_NoLock(const KT& key, List<VT>* outValues) noexcept
	{
		return _removeItems(key, outValues);
	}

	template <class KT, class VT>
	sl_size IMap<KT, VT>::removeItems(const KT& key, List<VT>* outValues) noexcept
	{
		ObjectLocker lock(this);
		return _removeItems(key, outValues);
	}
	
	template <class KT, class VT>
	sl_size IMap<KT, VT>::removeAll_NoLock() noexcept
	{
		return _removeAll();
	}

	template <class KT, class VT>
	sl_size IMap<KT, VT>::removeAll() noexcept
	{
		ObjectLocker lock(this);
		return _removeAll();
	}
	
	template <class KT, class VT>
	void IMap<KT, VT>::removeAt_NoLock(const MapPosition<KT, VT>& position) noexcept
	{
		_removeAt(position);
	}
	
	template <class KT, class VT>
	void IMap<KT, VT>::removeAt(const MapPosition<KT, VT>& position) noexcept
	{
		ObjectLocker lock(this);
		_removeAt(position);
	}
	
	template <class KT, class VT>
	MapPosition<KT, VT> IMap<KT, VT>::find_NoLock(const KT& key) noexcept
	{
		MapPosition<KT, VT> pos;
		if (_find(key, &pos)) {
			return pos;
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT>
	MapPosition<KT, VT> IMap<KT, VT>::find(const KT& key) noexcept
	{
		ObjectLocker lock(this);
		MapPosition<KT, VT> pos;
		if (_find(key, &pos)) {
			return pos;
		} else {
			return sl_null;
		}
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::find_NoLock(const KT& key, MapPosition<KT, VT>* pPosition) noexcept
	{
		return _find(key, pPosition);
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::find(const KT& key, MapPosition<KT, VT>* pPosition) noexcept
	{
		ObjectLocker lock(this);
		return _find(key, pPosition);
	}
	
	template <class KT, class VT>
	sl_bool IMap<KT, VT>::contains_NoLock(const KT& key) const noexcept
	{
		return _find(key, sl_null);
	}

	template <class KT, class VT>
	sl_bool IMap<KT, VT>::contains(const KT& key) const noexcept
	{
		ObjectLocker lock(this);
		return _find(key, sl_null);
	}
	
	template <class KT, class VT>
	IMap<KT, VT>* IMap<KT, VT>::duplicate_NoLock() const noexcept
	{
		return _duplicate();
	}

	template <class KT, class VT>
	IMap<KT, VT>* IMap<KT, VT>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return _duplicate();
	}
	
	template <class KT, class VT>
	List<KT> IMap<KT, VT>::getAllKeys_NoLock() const noexcept
	{
		return _getAllKeys();
	}

	template <class KT, class VT>
	List<KT> IMap<KT, VT>::getAllKeys() const noexcept
	{
		ObjectLocker lock(this);
		return _getAllKeys();
	}
	
	template <class KT, class VT>
	List<VT> IMap<KT, VT>::getAllValues_NoLock() const noexcept
	{
		return _getAllValues();
	}

	template <class KT, class VT>
	List<VT> IMap<KT, VT>::getAllValues() const noexcept
	{
		ObjectLocker lock(this);
		return _getAllValues();
	}
	
	template <class KT, class VT>
	List< Pair<KT, VT> > IMap<KT, VT>::toList_NoLock() const noexcept
	{
		return _toList();
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > IMap<KT, VT>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return _toList();
	}
	
	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT> IMap<KT, VT>::begin() const noexcept
	{
		return _getFirst(sl_null);
	}
	
	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT> IMap<KT, VT>::end() const noexcept
	{
		return sl_null;
	}
	
	template <class KT, class VT>
	MapPosition<KT, VT> IMap<KT, VT>::getFirst(Referable* ref) const noexcept
	{
		return _getFirst(ref);
	}

	
	template <class KT, class VT>
	Map<KT, VT> Map<KT, VT>::create() noexcept
	{
		return IMap<KT, VT>::createDefault();
	}
	
	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	Map<KT, VT> Map<KT, VT>::createHash(sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals) noexcept
	{
		return HashMap<KT, VT, HASH, KEY_EQUALS>::create(initialCapacity, hash, key_equals);
	}
	
	template <class KT, class VT>
	template <class KEY_COMPARE>
	Map<KT, VT> Map<KT, VT>::createTree(const KEY_COMPARE& key_compare) noexcept
	{
		return TreeMap<KT, VT, KEY_COMPARE>::create(key_compare);
	}
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	const Map<KT, VT>& Map<KT, VT>::from(const Map<KEY, VALUE>& other) noexcept
	{
		return *(reinterpret_cast<Map<KT, VT> const*>(&other));
	}

	template <class KT, class VT>
	void Map<KT, VT>::init() noexcept
	{
		ref = IMap<KT, VT>::createDefault();
	}
	
	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	void Map<KT, VT>::initHash(sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals) noexcept
	{
		ref = HashMap<KT, VT, HASH, KEY_EQUALS>::create(initialCapacity, hash, key_equals);
	}
	
	template <class KT, class VT>
	template <class KEY_COMPARE>
	void Map<KT, VT>::initTree(const KEY_COMPARE& key_compare) noexcept
	{
		ref = TreeMap<KT, VT, KEY_COMPARE>::create(key_compare);
	}
	
	template <class KT, class VT>
	VT Map<KT, VT>::operator[](const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT>
	sl_size Map<KT, VT>::getCount() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class KT, class VT>
	sl_bool Map<KT, VT>::isEmpty() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::isNotEmpty() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}

	template <class KT, class VT>
	VT* Map<KT, VT>::getItemPointer(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getItemPointer(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT>
	sl_bool Map<KT, VT>::get_NoLock(const KT& key, VT* _out) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->get_NoLock(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::get(const KT& key, VT* _out) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue_NoLock(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue_NoLock(const KT& key, const VT& def) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key, def);
		}
		return def;
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue(const KT& key, const VT& def) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key, def);
		}
		return def;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getValues_NoLock(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValues_NoLock(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getValues(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValues(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	sl_bool Map<KT, VT>::put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->put_NoLock(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				obj = IMap<KT, VT>::createDefault();
				if (obj) {
					ref = obj;
					return obj->put_NoLock(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
				}
			}
		}
		return sl_false;
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	sl_bool Map<KT, VT>::put(KEY&& key, VALUE&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->put(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				obj = IMap<KT, VT>::createDefault();
				if (obj) {
					ref = obj;
					return obj->put_NoLock(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
				}
			}
		}
		return sl_false;
	}
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	void Map<KT, VT>::putAll_NoLock(const Map<KEY, VALUE>& other, MapPutMode mode) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			obj->putAll_NoLock(other.ref._ptr, mode);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				obj = IMap<KT, VT>::createDefault();
				if (obj) {
					ref = obj;
					obj->putAll_NoLock(other.ref._ptr, mode);
				}
			}
		}
	}
	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	void Map<KT, VT>::putAll_NoLock(const AtomicMap<KEY, VALUE>& other, MapPutMode mode) noexcept
	{
		putAll_NoLock(Map<KEY, VALUE>(other), mode);
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	void Map<KT, VT>::putAll(const Map<KEY, VALUE>& other, MapPutMode mode) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			obj->putAll(other.ref._ptr, mode);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				obj = IMap<KT, VT>::createDefault();
				if (obj) {
					ref = obj;
					obj->putAll(other.ref._ptr, mode);
				}
			}
		}
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	void Map<KT, VT>::putAll(const AtomicMap<KEY, VALUE>& other, MapPutMode mode) noexcept
	{
		putAll(Map<KEY, VALUE>(other), mode);
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::remove_NoLock(const KT& key, VT* outValue) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->remove_NoLock(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::remove(const KT& key, VT* outValue) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeItems_NoLock(const KT& key, List<VT>* outValues) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeItems_NoLock(key, outValues);
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeItems(const KT& key, List<VT>* outValues) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeItems(key, outValues);
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeAll_NoLock() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeAll() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}
	
	template <class KT, class VT>
	void Map<KT, VT>::removeAt_NoLock(const MapPosition<KT, VT>& position) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			obj->removeAt_NoLock(position);
		}
	}
	
	template <class KT, class VT>
	void Map<KT, VT>::removeAt(const MapPosition<KT, VT>& position) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			obj->removeAt(position);
		}
	}
	
	template <class KT, class VT>
	MapPosition<KT, VT> Map<KT, VT>::find_NoLock(const KT& key) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->find_NoLock(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT>
	MapPosition<KT, VT> Map<KT, VT>::find(const KT& key) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->find(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT>
	sl_bool Map<KT, VT>::find_NoLock(const KT& key, MapPosition<KT, VT>* pPosition) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->find_NoLock(key, pPosition);
		}
		return sl_false;
	}
	
	template <class KT, class VT>
	sl_bool Map<KT, VT>::find(const KT& key, MapPosition<KT, VT>* pPosition) noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->find(key, pPosition);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::contains_NoLock(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->contains_NoLock(key);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::contains(const KT& key) const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->contains(key);
		}
		return sl_false;
	}

	template <class KT, class VT>
	Map<KT, VT> Map<KT, VT>::duplicate_NoLock() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Map<KT, VT> Map<KT, VT>::duplicate() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<KT> Map<KT, VT>::getAllKeys_NoLock() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<KT> Map<KT, VT>::getAllKeys() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getAllValues_NoLock() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getAllValues() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > Map<KT, VT>::toList_NoLock() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->toList_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > Map<KT, VT>::toList() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT> Map<KT, VT>::begin() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getFirst(obj);
		}
		return sl_null;
	}

	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT> Map<KT, VT>::end() const noexcept
	{
		return sl_null;
	}

	template <class KT, class VT>
	const Mutex* Map<KT, VT>::getLocker() const noexcept
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getLocker();
		}
		return sl_null;
	}

	
	template <class KT, class VT>
	template <class KEY, class VALUE>
	const Atomic< Map<KT, VT> >& Atomic< Map<KT, VT> >::from(const Atomic< Map<KEY, VALUE> >& other) noexcept
	{
		return *(reinterpret_cast<Atomic< Map<KT, VT> > const*>(&other));
	}

	template <class KT, class VT>
	void Atomic< Map<KT, VT> >::init() noexcept
	{
		ref = IMap<KT, VT>::createDefault();
	}

	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	void Atomic< Map<KT, VT> >::initHash(sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals) noexcept
	{
		ref = HashMap<KT, VT, HASH, KEY_EQUALS>::create(initialCapacity, hash, key_equals);
	}

	template <class KT, class VT>
	template <class KEY_COMPARE>
	void Atomic< Map<KT, VT> >::initTree(const KEY_COMPARE& key_compare) noexcept
	{
		ref = TreeMap<KT, VT, KEY_COMPARE>::create(key_compare);
	}

	template <class KT, class VT>
	VT Atomic< Map<KT, VT> >::operator[](const KT& key) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	sl_size Atomic< Map<KT, VT> >::getCount() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::isEmpty() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::isNotEmpty() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::get(const KT& key, VT* _out) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT>
	VT Atomic< Map<KT, VT> >::getValue(const KT& key) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	VT Atomic< Map<KT, VT> >::getValue(const KT& key, const VT& def) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key, def);
		}
		return def;
	}

	template <class KT, class VT>
	List<VT> Atomic< Map<KT, VT> >::getValues(const KT& key) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValues(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	sl_bool Atomic< Map<KT, VT> >::put(KEY&& key, VALUE&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->put(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				SpinLocker lock(SpinLockPoolForMap::get(this));
				obj = ref;
				if (obj.isNotNull()) {
					lock.unlock();
					return obj->put(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
				}
				obj = IMap<KT, VT>::createDefault();
				if (obj.isNotNull()) {
					ref = obj;
					lock.unlock();
					return obj->put(Forward<KEY>(key), Forward<VALUE>(value), mode, pPosition);
				}
			}
		}
		return sl_false;
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	void Atomic< Map<KT, VT> >::putAll(const Map<KEY, VALUE>& other, MapPutMode mode) noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			obj->putAll(other.ref._ptr, mode);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				SpinLocker lock(SpinLockPoolForMap::get(this));
				obj = ref;
				if (obj.isNotNull()) {
					lock.unlock();
					obj->putAll(other.ref._ptr, mode);
				}
				obj = IMap<KT, VT>::createDefault();
				if (obj.isNotNull()) {
					ref = obj;
					lock.unlock();
					obj->putAll(other.ref._ptr, mode);
				}
			}
		}
	}

	template <class KT, class VT>
	template <class KEY, class VALUE>
	void Atomic< Map<KT, VT> >::putAll(const AtomicMap<KEY, VALUE>& other, MapPutMode mode) noexcept
	{
		putAll(Map<KEY, VALUE>(other), mode);
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::remove(const KT& key, VT* outValue) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_size Atomic< Map<KT, VT> >::removeItems(const KT& key, List<VT>* outValues) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItems(key, outValues);
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Atomic< Map<KT, VT> >::removeAll() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}
	
	template <class KT, class VT>
	void Atomic< Map<KT, VT> >::removeAt(const MapPosition<KT, VT>& position) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			obj->removeAt(position);
		}
	}
	
	template <class KT, class VT>
	MapPosition<KT, VT> Atomic< Map<KT, VT> >::find(const KT& key) noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->find(key);
		}
		return sl_null;
	}
	
	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::find(const KT& key, MapPosition<KT, VT>* pPosition) noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->find(key, pPosition);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::contains(const KT& key) const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->contains(key);
		}
		return sl_false;
	}

	template <class KT, class VT>
	Map<KT, VT> Atomic< Map<KT, VT> >::duplicate() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<KT> Atomic< Map<KT, VT> >::getAllKeys() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllKeys();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Atomic< Map<KT, VT> >::getAllValues() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllValues();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > Atomic< Map<KT, VT> >::toList() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT> Atomic< Map<KT, VT> >::begin() const noexcept
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getFirst(obj.get());
		}
		return sl_null;
	}

	template <class KT, class VT>
	SLIB_INLINE MapPosition<KT, VT> Atomic< Map<KT, VT> >::end() const noexcept
	{
		return sl_null;
	}

}
