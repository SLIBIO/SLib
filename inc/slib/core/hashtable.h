#ifndef CHECKHEADER_SLIB_CORE_HASHTABLE
#define CHECKHEADER_SLIB_CORE_HASHTABLE

#include "definition.h"

#include "constants.h"
#include "hash.h"
#include "compare.h"
#include "list.h"
#include "math.h"

#define _SLIB_HASHTABLE_MIN_CAPACITY 16
#define _SLIB_HASHTABLE_MAX_CAPACITY 0x10000000
#define _SLIB_HASHTABLE_LOAD_FACTOR_UP 0.75f
#define _SLIB_HASHTABLE_LOAD_FACTOR_DOWN 0.25f

namespace slib
{
	
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
	
	template < class KT, class VT, class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT HashTable
	{
	public:
		HashTable(sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
	
		~HashTable();
	
	public:
		sl_size getCount() const;
	
		sl_size getCapacity() const;
	
		sl_bool search(const KT& key, HashPosition* position = sl_null, VT* outValue = sl_null) const;

		template < class _VT, class VALUE_EQUALS = Equals<VT, _VT> >
		sl_bool searchKeyAndValue(const KT& key, const _VT& value, HashPosition* position = sl_null, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;
	
		sl_bool getAt(const HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null) const;

		sl_bool getFirstPosition(HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null) const;

		sl_bool getNextPosition(HashPosition& position, KT* outKey = sl_null, VT* outValue = sl_null) const;

		sl_bool get(const KT& key, VT* outValue = sl_null) const;

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

		sl_size removeAll();

		sl_bool copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other);

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

		HASH m_hash;
		KEY_EQUALS m_equals;
	
	private:
		void _init();

		void _free();

		sl_bool _createTable(sl_uint32 capacity);

		sl_bool _addEntry(sl_uint32 index, sl_uint32 hash, HashEntry* first, const KT& key, const VT& value);

		void _compact();

	};

}

#include "detail/hashtable.h"

#endif
