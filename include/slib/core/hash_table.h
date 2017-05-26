/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_HASH_TABLE
#define CHECKHEADER_SLIB_CORE_HASH_TABLE

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
	class HashTableNode
	{
	public:
		HashTableNode* chain;
		HashTableNode* before;
		HashTableNode* next;
		sl_uint32 hash;
		
		Pair<KT, VT> data;

	public:
		template <class KEY, class VALUE>
		HashTableNode(KEY&& _key, VALUE&& _value) noexcept;

	public:
		SLIB_INLINE constexpr HashTableNode<KT, VT>* getNext() const noexcept
		{
			return next;
		}
		
		SLIB_INLINE constexpr HashTableNode<KT, VT>* getPrevious() const noexcept
		{
			return before;
		}
		
	};
	
	template <class KT, class VT>
	struct HashTableStruct
	{
		HashTableNode<KT, VT>** nodes;
		sl_uint32 capacity;
		
		sl_size count;
		HashTableNode<KT, VT>* firstNode;
		HashTableNode<KT, VT>* lastNode;
		
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
	
		HashTable(const HashTable& other) = delete;
		
		HashTable(HashTable&& other) noexcept;
		
		~HashTable() noexcept;
		
	public:
		HashTable& operator=(const HashTable& other) = delete;
		
		HashTable& operator=(HashTable&& other) noexcept;
		
	public:
		sl_size getCount() const noexcept;
	
		sl_size getCapacity() const noexcept;
	
		HashTableNode<KT, VT>* getFirstNode() const noexcept;
		
		HashTableNode<KT, VT>* getLastNode() const noexcept;
		
		HashTableNode<KT, VT>* find(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		HashTableNode<KT, VT>* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool get(const KT& key, VT* outValue = sl_null) const noexcept;

		VT* getItemPointer(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, HashTableNode<KT, VT>** ppNode = sl_null) noexcept;

		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue(KEY&& key, VALUE&& value, HashTableNode<KT, VT>** ppNode = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_bool removeNode(const HashTableNode<KT, VT>* node) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_size removeAll() noexcept;

		sl_bool copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other) noexcept;

	private:
		typedef HashTableNode<KT, VT> Node;
		typedef HashTableStruct<KT, VT> Table;
		
		Table m_table;
		HASH m_hash;
		KEY_EQUALS m_equals;
		
	};

}

#include "detail/hash_table.inc"

#endif
