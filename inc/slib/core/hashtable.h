#ifndef CHECKHEADER_SLIB_CORE_HASHTABLE
#define CHECKHEADER_SLIB_CORE_HASHTABLE

#include "definition.h"

#include "constants.h"
#include "hash.h"
#include "compare.h"
#include "list.h"
#include "math.h"

#define SLIB_HASHTABLE_DEFAULT_CAPACITY 32

#define _SLIB_HASHTABLE_MIN_CAPACITY 16
#define _SLIB_HASHTABLE_MAX_CAPACITY 0x10000000
#define _SLIB_HASHTABLE_LOAD_FACTOR_UP 0.75f
#define _SLIB_HASHTABLE_LOAD_FACTOR_DOWN 0.25f

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT HashPosition
{
public:
	sl_uint32 index;
	void* node;

public:
	HashPosition();
	
	HashPosition(const HashPosition& other);

public:
	HashPosition& operator=(const HashPosition& other);

	sl_bool operator==(const HashPosition& other) const;
	
	sl_bool operator!=(const HashPosition& other) const;
	
	sl_bool isNull() const;

	sl_bool isNotNull() const;

	void setNull();
	
};

template < class KT, class VT, class HASH = Hash<KT>, class COMPARE = Compare<KT> >
class SLIB_EXPORT HashTable
{
public:
	HashTable(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);

	~HashTable();

public:
	sl_size getCount() const;

	sl_size getCapacity() const;

	sl_bool search(const KT& key, HashPosition* position = sl_null) const;
	
	sl_bool search(const KT& key, const VT& value, HashPosition* position = sl_null) const;
	
	sl_bool getFirstPosition(HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null) const;
	
	sl_bool getNextPosition(HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null) const;
	
	sl_bool getAt(const HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null) const;
	
	sl_bool get(const KT& key, VT* outValue = sl_null) const;
	
	VT* getItemPtr(const KT& key) const;
	
	List<VT> getValues(const KT& key) const;
	
	sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
	
	sl_bool addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	sl_bool remove(const KT& key, VT* outValue = sl_null);
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);
	
	sl_bool removeValue(const KT& key, const VT& value);
	
	sl_size removeValues(const KT& key, const VT& value);
	
	sl_size removeAll();
	
	sl_bool copyFrom(const HashTable<KT, VT, HASH, COMPARE>* other);
	
private:
	struct HashEntry
	{
		sl_uint32 hash;
		KT key;
		VT value;
		HashEntry* next;
	};
	
	sl_size m_nSize;
	
	HashEntry** m_table;
	
	sl_uint32 m_nCapacity;
	sl_uint32 m_nCapacityMin;
	sl_uint32 m_nThresholdUp;
	sl_uint32 m_nThresholdDown;

private:
	void _init();
	
	void _free();
	
	sl_bool _createTable(sl_uint32 capacity);
	
	sl_bool _addEntry(sl_uint32 index, sl_uint32 hash, HashEntry* first, const KT& key, const VT& value);
	
	void _compact();
	
};

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

SLIB_INLINE HashPosition::HashPosition()
{
	index = 0;
	node = sl_null;
}

SLIB_INLINE HashPosition::HashPosition(const HashPosition& other)
{
	node = other.node;
	index = other.index;
}

SLIB_INLINE HashPosition& HashPosition::operator=(const HashPosition& other)
{
	node = other.node;
	index = other.index;
	return *this;
}

SLIB_INLINE sl_bool HashPosition::operator==(const HashPosition& other) const
{
	return node == other.node && index == other.index;
}

SLIB_INLINE sl_bool HashPosition::operator!=(const HashPosition& other) const
{
	return node != other.node || index != other.index;
}

SLIB_INLINE sl_bool HashPosition::isNull() const
{
	return node == sl_null;
}

SLIB_INLINE sl_bool HashPosition::isNotNull() const
{
	return node != sl_null;
}

SLIB_INLINE void HashPosition::setNull()
{
	node = sl_null;
}


template <class KT, class VT, class HASH, class COMPARE>
HashTable<KT, VT, HASH, COMPARE>::HashTable(sl_uint32 capacity)
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

template <class KT, class VT, class HASH, class COMPARE>
HashTable<KT, VT, HASH, COMPARE>::~HashTable()
{
	_free();
}

template <class KT, class VT, class HASH, class COMPARE>
SLIB_INLINE sl_size HashTable<KT, VT, HASH, COMPARE>::getCount() const
{
	return m_nSize;
}

template <class KT, class VT, class HASH, class COMPARE>
SLIB_INLINE sl_size HashTable<KT, VT, HASH, COMPARE>::getCapacity() const
{
	return m_nCapacity;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::search(const KT& key, HashPosition* position) const
{
	if (m_nCapacity == 0) {
		return sl_false;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry = m_table[index];
	while (entry) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key)) {
			if (position) {
				position->node = entry;
				position->index = index;
			}
			return sl_true;
		}
		entry = entry->next;
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::search(const KT& key, const VT& value, HashPosition* position) const
{
	if (m_nCapacity == 0) {
		return sl_false;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry = m_table[index];
	while (entry) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key) && entry->value == value) {
			if (position) {
				position->node = entry;
				position->index = index;
			}
			return sl_true;
		}
		entry = entry->next;
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::getFirstPosition(HashPosition& position, KT* outKey, VT* outValue) const
{
	if (m_nCapacity == 0) {
		return sl_false;
	}
	if (m_nSize == 0) {
		return sl_false;
	}
	for (sl_uint32 i = 0; i < m_nCapacity; i++) {
		if (HashEntry* entry = m_table[i]) {
			position.index = i;
			position.node = (void*)entry;
			if (outKey) {
				*outKey = entry->key;
			}
			if (outValue) {
				*outValue = entry->value;
			}
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::getNextPosition(HashPosition& position, KT* outKey, VT* outValue) const
{
	if (m_nCapacity == 0) {
		return sl_false;
	}
	if (position.isNull()) {
		return getFirstPosition(position, outKey, outValue);
	}
	HashEntry* entry = (HashEntry*)(position.node);
	entry = entry->next;
	if (entry) {
		position.node = (void*)entry;
		if (outKey) {
			*outKey = entry->key;
		}
		if (outValue) {
			*outValue = entry->value;
		}
		return sl_true;
	}
	for (sl_uint32 i = position.index + 1; i < m_nCapacity; i++) {
		if (HashEntry* entry = m_table[i]) {
			position.index = i;
			position.node = (void*)entry;
			if (outKey) {
				*outKey = entry->key;
			}
			if (outValue) {
				*outValue = entry->value;
			}
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::getAt(const HashPosition& position, KT* outKey, VT* outValue) const
{
	if (m_nCapacity == 0) {
		return sl_false;
	}
	if (position.isNull()) {
		return sl_false;
	}
	HashEntry* entry = (HashEntry*)(position.node);
	if (outKey) {
		*outKey = entry->key;
	}
	if (outValue) {
		*outValue = entry->value;
	}
	return sl_true;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::get(const KT& key, VT* value) const
{
	HashPosition pos;
	if (search(key, &pos)) {
		if (value) {
			HashEntry* entry = (HashEntry*)(pos.node);
			*value = entry->value;
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

template <class KT, class VT, class HASH, class COMPARE>
VT* HashTable<KT, VT, HASH, COMPARE>::getItemPtr(const KT& key) const
{
	HashPosition pos;
	if (search(key, &pos)) {
		HashEntry* entry = (HashEntry*)(pos.node);
		return &(entry->value);
	} else {
		return sl_null;
	}
}

template <class KT, class VT, class HASH, class COMPARE>
List<VT> HashTable<KT, VT, HASH, COMPARE>::getValues(const KT& key) const
{
	List<VT> ret;
	if (m_nCapacity == 0) {
		return ret;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry = m_table[index];
	while (entry) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key)) {
			ret.add_NoLock(entry->value);
		}
		entry = entry->next;
	}
	return ret;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::_addEntry(sl_uint32 index, sl_uint32 hash, HashEntry* first, const KT& key, const VT& value)
{
	HashEntry* entry = new HashEntry;
	if (!entry) {
		return sl_false;
	}
	entry->hash = hash;
	entry->key = key;
	entry->value = value;
	entry->next = first;
	
	m_table[index] = entry;
	m_nSize++;

	if (m_nSize >= m_nThresholdUp) {
		// double capacity
		HashEntry** tableOld = m_table;
		sl_uint32 n = m_nCapacity;
		if (_createTable(n + n)) {
			HashEntry** table = m_table;
			for (sl_uint32 i = 0; i < n; i++) {
				HashEntry* entry = tableOld[i];
				table[i] = sl_null;
				table[i | n] = sl_null;
				if (entry) {
					sl_uint32 highBitBefore = entry->hash & n;
					HashEntry* broken = sl_null;
					table[i | highBitBefore] = entry;
					while (HashEntry* next = entry->next) {
						sl_uint32 highBitNext = next->hash & n;
						if (highBitBefore != highBitNext) {
							if (broken) {
								broken->next = next;
							} else {
								table[i | highBitNext] = next;
							}
							broken = entry;
							highBitBefore = highBitNext;
						}
						entry = next;
					}
					if (broken) {
						broken->next = sl_null;
					}
				}
			}
			Base::freeMemory(tableOld);
		}
	}
	return sl_true;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	if (pFlagExist) {
		*pFlagExist = sl_false;
	}
	if (m_nCapacity == 0) {
		return sl_false;
	}
	
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* first = m_table[index];
	
	if (mode != MapPutMode::AddAlways) {
		HashEntry* entry = first;
		while (entry) {
			if (entry->hash == hash && COMPARE::equals(entry->key, key)) {
				if (pFlagExist) {
					*pFlagExist = sl_true;
				}
				if (mode == MapPutMode::AddNew) {
					return sl_false;
				}
				entry->value = value;
				return sl_true;
			}
			entry = entry->next;
		}
		if (mode == MapPutMode::ReplaceExisting) {
			return sl_false;
		}
	}
	
	return _addEntry(index, hash, first, key, value);
	
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	if (pFlagExist) {
		*pFlagExist = sl_false;
	}
	if (m_nCapacity == 0) {
		return sl_false;
	}
	
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* first = m_table[index];

	HashEntry* entry = first;
	while (entry) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key) && entry->value == value) {
			if (pFlagExist) {
				*pFlagExist = sl_true;
			}
			return sl_false;
		}
		entry = entry->next;
	}
	
	return _addEntry(index, hash, first, key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
void HashTable<KT, VT, HASH, COMPARE>::_compact()
{
	HashEntry* entry;
	HashEntry** link;
	if (m_nSize <= m_nThresholdDown) {
		// half capacity
		HashEntry** tableOld = m_table;
		sl_uint32 n = m_nCapacity >> 1;
		if (_createTable(n)) {
			HashEntry** table = m_table;
			for (sl_uint32 i = 0; i < n; i++) {
				table[i] = tableOld[i];
				link = table + i;
				while ((entry = *link)) {
					link = &(entry->next);
				}
				*link = tableOld[i | n];
			}
			Base::freeMemory(tableOld);
		}
	}
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::remove(const KT& key, VT* outValue)
{
	if (m_nCapacity == 0) {
		return 0;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry;
	HashEntry** link = m_table + index;
	while ((entry = *link)) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key)) {
			*link = entry->next;
			m_nSize--;
			if (outValue) {
				*outValue = entry->value;
			}
			delete entry;
			_compact();
			return sl_true;
		} else {
			link = &(entry->next);
		}
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashTable<KT, VT, HASH, COMPARE>::removeItems(const KT& key, List<VT>* outValues)
{
	if (m_nCapacity == 0) {
		return 0;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry;
	HashEntry** link = m_table + index;
	HashEntry* entryDelete = sl_null;
	HashEntry** linkDelete = &entryDelete;
	sl_size oldSize = m_nSize;
	while ((entry = *link)) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key)) {
			*link = entry->next;
			m_nSize--;
			if (outValues) {
				outValues->add_NoLock(entry->value);
			}
			*linkDelete = entry;
			entry->next = sl_null;
			linkDelete = &(entry->next);
		} else {
			link = &(entry->next);
		}
	}
	if (!entryDelete) {
		return 0;
	}
	_compact();
	while (entryDelete) {
		entry = entryDelete;
		entryDelete = entryDelete->next;
		delete entry;
	}
	return oldSize - m_nSize;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::removeValue(const KT& key, const VT& value)
{
	if (m_nCapacity == 0) {
		return 0;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry;
	HashEntry** link = m_table + index;
	while ((entry = *link)) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key) && entry->value == value) {
			*link = entry->next;
			m_nSize--;
			delete entry;
			_compact();
			return sl_true;
		} else {
			link = &(entry->next);
		}
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashTable<KT, VT, HASH, COMPARE>::removeValues(const KT& key, const VT& value)
{
	if (m_nCapacity == 0) {
		return 0;
	}
	sl_uint32 hash = HASH::hash(key);
	sl_uint32 index = hash & (m_nCapacity - 1);
	HashEntry* entry;
	HashEntry** link = m_table + index;
	HashEntry* entryDelete = sl_null;
	HashEntry** linkDelete = &entryDelete;
	sl_size oldSize = m_nSize;
	while ((entry = *link)) {
		if (entry->hash == hash && COMPARE::equals(entry->key, key) && entry->value == value) {
			*link = entry->next;
			m_nSize--;
			*linkDelete = entry;
			entry->next = sl_null;
			linkDelete = &(entry->next);
		} else {
			link = &(entry->next);
		}
	}
	if (!entryDelete) {
		return 0;
	}
	_compact();
	while (entryDelete) {
		entry = entryDelete;
		entryDelete = entryDelete->next;
		delete entry;
	}
	return oldSize - m_nSize;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashTable<KT, VT, HASH, COMPARE>::removeAll()
{
	if (m_nCapacity == 0) {
		return 0;
	}
	sl_size oldSize = m_nSize;
	_free();
	_init();
	return oldSize;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::copyFrom(const HashTable<KT, VT, HASH, COMPARE>* other)
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
	for (sl_uint32 i = 0; i < m_nCapacity; i++) {
		HashEntry* entry = other->m_table[i];
		HashEntry** link = m_table + i;
		while (entry) {
			HashEntry* dst = new HashEntry;
			if (dst) {
				dst->hash = entry->hash;
				dst->key = entry->key;
				dst->value = entry->value;
				dst->next = sl_null;
				*link = dst;
				link = &(dst->next);
				entry = entry->next;
			} else {
				return sl_false;
			}
		}
	}
	return sl_true;
}

template <class KT, class VT, class HASH, class COMPARE>
void HashTable<KT, VT, HASH, COMPARE>::_init()
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
}

template <class KT, class VT, class HASH, class COMPARE>
void HashTable<KT, VT, HASH, COMPARE>::_free()
{
	HashEntry** table = m_table;
	sl_uint32 nCapacity = m_nCapacity;
	m_table = sl_null;
	m_nCapacity = 0;
	if (table) {
		for (sl_uint32 i = 0; i < nCapacity; i++) {
			HashEntry* entry = table[i];
			while (entry) {
				HashEntry* next = entry->next;
				delete entry;
				entry = next;
			}
		}
		Base::freeMemory(table);
	}
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashTable<KT, VT, HASH, COMPARE>::_createTable(sl_uint32 capacity)
{
	if (capacity > _SLIB_HASHTABLE_MAX_CAPACITY || capacity < m_nCapacityMin) {
		return sl_false;
	}
	HashEntry** table = (HashEntry**)(Base::createMemory(sizeof(HashEntry*)*capacity));
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

SLIB_NAMESPACE_END

#endif
