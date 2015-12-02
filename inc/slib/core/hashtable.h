#ifndef CHECKHEADER_SLIB_HASHTABLE
#define CHECKHEADER_SLIB_HASHTABLE

#include "definition.h"
#include "algorithm.h"
#include "list.h"
#include "math.h"

/*************************************************
Notice - This Data Structure is not thread-safe
**************************************************/

#define SLIB_HASHTABLE_DEFAULT_CAPACITY 32

#define _SLIB_HASHTABLE_MIN_CAPACITY 16
#define _SLIB_HASHTABLE_MAX_CAPACITY 0x10000000
#define _SLIB_HASHTABLE_LOAD_FACTOR_UP 0.75f
#define _SLIB_HASHTABLE_LOAD_FACTOR_DOWN 0.25f

SLIB_NAMESPACE_BEGIN

struct SLIB_EXPORT HashPosition
{
	sl_uint32 index;
	void* node;

	SLIB_INLINE HashPosition()
	{
		index = 0;
		node = sl_null;
	}

	SLIB_INLINE HashPosition(const HashPosition& other)
	{
		node = other.node;
		index = other.index;
	}

	SLIB_INLINE HashPosition& operator=(const HashPosition& other)
	{
		node = other.node;
		index = other.index;
		return *this;
	}

	SLIB_INLINE sl_bool operator!=(const HashPosition& other) const
	{
		return node != other.node || index != other.index;
	}

	SLIB_INLINE sl_bool operator==(const HashPosition& other) const
	{
		return node == other.node && index == other.index;
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return node == sl_null;
	}

	SLIB_INLINE sl_bool isNotNull() const
	{
		return node != sl_null;
	}

	SLIB_INLINE void setNull()
	{
		node = sl_null;
	}
};

template <class KT, class VT, class HASH = Hash<KT> >
class SLIB_EXPORT HashTable
{
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

public:
	HashTable(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
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

	~HashTable()
	{
		_free();
	}

	SLIB_INLINE sl_size getCount()
	{
		return m_nSize;
	}

	SLIB_INLINE sl_size getCapacity()
	{
		return m_nCapacity;
	}

	sl_bool search(const KT& key, HashPosition* position = sl_null)
	{
		if (m_nCapacity == 0) {
			return sl_false;
		}
		sl_uint32 hash = HASH::hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		HashEntry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && entry->key == key) {
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

	sl_bool getFirstPosition(HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null)
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

	sl_bool getNextPosition(HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null)
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

	sl_bool getAt(const HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null)
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

	sl_bool get(const KT& key, VT* value = sl_null)
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

	VT* getItemPtr(const KT& key)
	{
		HashPosition pos;
		if (search(key, &pos)) {
			HashEntry* entry = (HashEntry*)(pos.node);
			return &(entry->value);
		} else {
			return sl_null;
		}
	}

	List<VT> getValues(const KT& key)
	{
		List<VT> ret;
		if (m_nCapacity == 0) {
			return ret;
		}
		sl_uint32 hash = HASH::hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		HashEntry* entry = m_table[index];
		while (entry) {
			if (entry->hash == hash && entry->key == key) {
				ret.add(entry->value);
			}
			entry = entry->next;
		}
		return ret;
	}

	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true, sl_bool* pFlagReplaced = sl_null)
	{
		if (m_nCapacity == 0) {
			return sl_false;
		}
		sl_uint32 hash = HASH::hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		HashEntry* first = m_table[index];
		HashEntry* entry;
		if (flagReplace) {
			entry = first;
			while (entry) {
				if (entry->hash == hash && entry->key == key) {
					entry->value = value;
					if (pFlagReplaced) {
						*pFlagReplaced = sl_true;
					}
					return sl_true;
				}
				entry = entry->next;
			}
		}
		// insert new entry
		{
			entry = new HashEntry;
			if (!entry) {
				return sl_false;
			}
			if (pFlagReplaced) {
				*pFlagReplaced = sl_false;
			}
			entry->hash = hash;
			entry->key = key;
			entry->value = value;
			entry->next = first;
			m_table[index] = entry;
			m_nSize++;
		}
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
				delete[] tableOld;
			}
		}
		return sl_true;
	}

	sl_bool remove(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		if (m_nCapacity == 0) {
			return sl_false;
		}
		sl_uint32 hash = HASH::hash(key);
		sl_uint32 index = hash & (m_nCapacity - 1);
		HashEntry* entry;
		HashEntry** link = m_table + index;
		HashEntry* entryDelete = sl_null;
		HashEntry** linkDelete = &entryDelete;
		while ((entry = *link)) {
			if (entry->hash == hash && entry->key == key) {
				*link = entry->next;
				m_nSize--;
				*linkDelete = entry;
				entry->next = sl_null;
				linkDelete = &(entry->next);
				if (!flagAllKeys) {
					break;
				}
			} else {
				link = &(entry->next);
			}
		}
		if (!entryDelete) {
			return sl_false;
		}
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
				delete[] tableOld;
			}
		}
		while (entryDelete) {
			entry = entryDelete;
			entryDelete = entryDelete->next;
			delete entry;
		}
		return sl_true;
	}

	void clear()
	{
		if (m_nCapacity == 0) {
			return;
		}
		_free();
		_init();
	}

	sl_bool copyFrom(HashTable<KT, VT, HASH>* other)
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

private:
	void _init()
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

	void _free()
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
			delete[] table;
		}
	}

	sl_bool _createTable(sl_uint32 capacity)
	{
		if (capacity > _SLIB_HASHTABLE_MAX_CAPACITY || capacity < m_nCapacityMin) {
			return sl_false;
		}
		HashEntry** table = new HashEntry*[capacity];
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

};
SLIB_NAMESPACE_END

#endif
