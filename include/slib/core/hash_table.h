/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_HASH_TABLE
#define CHECKHEADER_SLIB_CORE_HASH_TABLE

#include "definition.h"

#include "map_common.h"
#include "hash.h"
#include "list.h"

namespace slib
{
	
	template <class KT, class VT>
	class HashTableNode
	{
	public:
		HashTableNode* next;
		sl_size hash;
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
		sl_size capacity;
		sl_size capacityMinimum;
		sl_size capacityMaximum;
		sl_size thresholdDown;
		sl_size thresholdUp;
	};
	
	template <class KT, class VT>
	class SLIB_EXPORT HashTablePosition
	{
	public:
		typedef HashTableNode<KT, VT> NODE;
		
	public:
		HashTablePosition(NODE** entry, NODE** last_entry, NODE* node) noexcept;
		
		HashTablePosition(const HashTablePosition& other) noexcept = default;
		
	public:
		HashTablePosition& operator=(const HashTablePosition& other) noexcept = default;
		
		NODE& operator*() const noexcept;
		
		sl_bool operator==(const HashTablePosition& other) const noexcept;
		
		sl_bool operator!=(const HashTablePosition& other) const noexcept;
		
		HashTablePosition& operator++() noexcept;
		
	public:
		NODE** entry;
		NODE** last_entry;
		NODE* node;
		NODE* next;
		
	};


	template < class KT, class VT, class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT HashTable
	{
	public:
		typedef HashTableNode<KT, VT> NODE;
		
	public:
		HashTable(sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
	
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
		
		sl_size getMinimumCapacity() const noexcept;
		
		void setMinimumCapacity(sl_size capacity) noexcept;
		
		sl_size getMaximumCapacity() const noexcept;
		
		void setMaximumCapacity(sl_size capacity) noexcept;
		
		NODE* find(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		NODE* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
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
		NODE* put(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		NODE* replace(const KEY& key, VALUE&& value) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		NODE* add(KEY&& key, VALUE_ARGS&&... value_args) noexcept;

		template <class KEY, class... VALUE_ARGS>
		MapEmplaceReturn<NODE> emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		sl_bool removeAt(const NODE* node) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_size removeItems(const KT& key) noexcept;

		List<VT> removeItemsAndReturnValues(const KT& key) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_size removeAll() noexcept;
		
		void shrink() noexcept;

		sl_bool copyFrom(const HashTable<KT, VT, HASH, KEY_EQUALS>& other) noexcept;
		
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
