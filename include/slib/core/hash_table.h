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

#include "map_common.h"
#include "hash.h"
#include "compare.h"
#include "list.h"

namespace slib
{
	
	template <class KT, class VT>
	class HashTableNode
	{
	public:
		HashTableNode* next;
		sl_uint32 hash;
		KT key;
		VT value;
		
	public:
		template <class KEY, class... VALUE_ARGS>
		HashTableNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept;
		
	};
	
	template <class KT, class VT>
	struct HashTableStruct
	{
		HashTableNode<KT, VT>** nodes;
		sl_size count;
		sl_uint32 capacity;
		sl_uint32 capacityMin;
		sl_uint32 thresholdUp;
		sl_uint32 thresholdDown;
	};
	
	template <class KT, class VT>
	class SLIB_EXPORT HashTablePosition
	{
	public:
		typedef HashTableNode<KT, VT> Node;
		
	public:
		HashTablePosition(Node** entry, Node** last_entry, Node* node) noexcept;
		
		HashTablePosition(const HashTablePosition& other) noexcept = default;
		
	public:
		HashTablePosition& operator=(const HashTablePosition& other) noexcept = default;
		
		Node& operator*() const noexcept;
		
		sl_bool operator==(const HashTablePosition& other) const noexcept;
		
		sl_bool operator!=(const HashTablePosition& other) const noexcept;
		
		HashTablePosition& operator++() noexcept;
		
	public:
		Node** entry;
		Node** last_entry;
		Node* node;
		Node* next;
		
	};


	template < class KT, class VT, class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT HashTable
	{
	public:
		typedef HashTableNode<KT, VT> Node;
		
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
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
	
		sl_size getCapacity() const noexcept;
		
		Node* find(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		Node* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		VT* getItemPointer(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		sl_bool get(const KT& key, VT* outValue = sl_null) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;
		
		List<VT> getValues(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		template <class KEY, class VALUE>
		Node* put(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		Node* replace(const KEY& key, VALUE&& value) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		Node* add(KEY&& key, VALUE_ARGS&&... value_args) noexcept;

		template <class KEY, class... VALUE_ARGS>
		MapEmplaceReturn<Node> emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		void removeAt(const Node* node) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_size removeAll() noexcept;
		
		void shrink() noexcept;

		sl_bool copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>* other) noexcept;
		
		// range-based for loop
		HashTablePosition<KT, VT> begin() const noexcept;
		
		HashTablePosition<KT, VT> end() const noexcept;

	private:
		typedef HashTableStruct<KT, VT> Table;
		
		Table m_table;
		HASH m_hash;
		KEY_EQUALS m_equals;
		
	};

}

#include "detail/hash_table.inc"

#endif
