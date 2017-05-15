/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_HASHTABLE
#define CHECKHEADER_SLIB_CORE_HASHTABLE

#include "definition.h"

#include "constants.h"
#include "pair.h"
#include "hash.h"
#include "compare.h"
#include "list.h"
#include "math.h"

namespace slib
{
	
	template <class KT, class VT>
	class HashEntry
	{
	public:
		HashEntry* chain;
		HashEntry* before;
		HashEntry* next;
		sl_uint32 hash;
		
		Pair<KT, VT> data;

	public:
		template <class KEY, class VALUE>
		HashEntry(KEY&& _key, VALUE&& _value) noexcept;
		
	};
	
	template <class KT, class VT>
	struct HashTableStruct
	{
		HashEntry<KT, VT>** entries;
		sl_uint32 capacity;
		
		sl_size count;
		HashEntry<KT, VT>* firstEntry;
		HashEntry<KT, VT>* lastEntry;
		
		sl_uint32 capacityMin;
		sl_uint32 thresholdUp;
		sl_uint32 thresholdDown;
	};
	

	template < class KT, class VT, class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT HashTable
	{
	public:
		HashTable() noexcept;
		
		HashTable(sl_uint32 capacity) noexcept;
		
		template <class HASH_ARG>
		HashTable(sl_uint32 capacity, HASH_ARG&& hash) noexcept;

		template <class HASH_ARG, class KEY_EQUALS_ARG>
		HashTable(sl_uint32 capacity, HASH_ARG&& hash, KEY_EQUALS_ARG&& key_equals) noexcept;
	
		~HashTable() noexcept;
	
	public:
		sl_size getCount() const noexcept;
	
		sl_size getCapacity() const noexcept;
	
		HashEntry<KT, VT>* getFirstEntry() const noexcept;
		
		HashEntry<KT, VT>* getLastEntry() const noexcept;
		
		HashEntry<KT, VT>* search(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		HashEntry<KT, VT>* searchKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool get(const KT& key, VT* outValue = sl_null) const noexcept;

		VT* getItemPointer(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, HashEntry<KT, VT>* ppEntry = sl_null) noexcept;

		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue(KEY&& key, VALUE&& value, HashEntry<KT, VT>* ppEntry = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_bool removeEntry(const HashEntry<KT, VT>* entry) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_size removeAll() noexcept;

		sl_bool copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other) noexcept;

	private:
		typedef HashEntry<KT, VT> Entry;
		typedef HashTableStruct<KT, VT> Table;
		
		Table m_table;
		HASH m_hash;
		KEY_EQUALS m_equals;
		
	};

}

#include "detail/hashtable.inc"

#endif
