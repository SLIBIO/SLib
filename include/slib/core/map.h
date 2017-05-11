/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_MAP
#define CHECKHEADER_SLIB_CORE_MAP

#include "definition.h"

#include "object.h"
#include "iterator.h"
#include "list.h"
#include "hashtable.h"
#include "red_black_tree.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <initializer_list>
#endif

namespace slib
{
	
	template <class KT, class VT>
	class Map;
	
	template <class KT, class VT>
	using AtomicMap = Atomic< Map<KT, VT> >;

	
	template <class KT, class VT>
	class SLIB_EXPORT Pair
	{
	public:
		KT key;
		VT value;

	public:
		SLIB_INLINE Pair() {}

		SLIB_INLINE Pair(const Pair<KT, VT>& other) = default;

		SLIB_INLINE Pair(Pair<KT, VT>&& other) = default;

		template <class T1, class T2>
		SLIB_INLINE Pair(T1&& _key, T2&& _value) : key(Forward<T1>(_key)), value(Forward<T2>(_value)) {}

		SLIB_INLINE Pair<KT, VT>& operator=(const Pair<KT, VT>& other) = default;

		SLIB_INLINE Pair<KT, VT>& operator=(Pair<KT, VT>&& other) = default;

	};

	extern const char _priv_Map_ClassID[];
	
	template <class KT, class VT>
	class SLIB_EXPORT IMap : public Object
	{
		SLIB_TEMPLATE_OBJECT(Object, _priv_Map_ClassID)
	
	public:
		static IMap<KT, VT>* createDefault();

		virtual sl_size getCount() const = 0;

		sl_bool isEmpty() const;
	
		sl_bool isNotEmpty() const;
	
		virtual VT* getItemPointer(const KT& key) const = 0;

		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const;

		sl_bool get(const KT& key, VT* _out = sl_null) const;

		VT getValue_NoLock(const KT& key) const;

		VT getValue(const KT& key) const;

		VT getValue_NoLock(const KT& key, const VT& def) const;

		VT getValue(const KT& key, const VT& def) const;

		virtual List<VT> getValues_NoLock(const KT& key) const = 0;

		List<VT> getValues(const KT& key) const;

		virtual sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null) = 0;

		sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
		
		template <class KEY, class VALUE>
		void putAll_NoLock(IMap<KEY, VALUE>* other, MapPutMode mode = MapPutMode::Default);

		template <class KEY, class VALUE>
		void putAll(IMap<KEY, VALUE>* other, MapPutMode mode = MapPutMode::Default);

		virtual sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) = 0;

		sl_bool remove(const KT& key, VT* outValue = sl_null);

		virtual sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) = 0;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);

		virtual sl_size removeAll_NoLock() = 0;

		sl_size removeAll();

		virtual sl_bool contains_NoLock(const KT& key) const = 0;

		sl_bool contains(const KT& key) const;

		virtual IMap<KT, VT>* duplicate_NoLock() const = 0;

		IMap<KT, VT>* duplicate() const;

		virtual Iterator<KT> getKeyIteratorWithRefer(Referable* refer) const = 0;

		Iterator<KT> getKeyIterator() const;

		virtual List<KT> getAllKeys_NoLock() const = 0;

		List<KT> getAllKeys() const;

		virtual Iterator<VT> getValueIteratorWithRefer(Referable* refer) const = 0;

		Iterator<VT> getValueIterator() const;

		virtual List<VT> getAllValues_NoLock() const = 0;

		List<VT> getAllValues() const;

		virtual Iterator< Pair<KT, VT> > toIteratorWithRefer(Referable* refer) const = 0;

		Iterator< Pair<KT, VT> > toIterator() const;

		virtual List< Pair<KT, VT> > toList_NoLock() const = 0;

		List< Pair<KT, VT> > toList() const;

		// range-based for loop
		IteratorPosition< Pair<KT, VT> > begin() const;

		IteratorPosition< Pair<KT, VT> > end() const;

	};
	
	
	template < class KT, class VT, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT ListMap : public IMap<KT, VT>
	{
	public:
		class PairKeyCompare
		{
		public:
			KEY_EQUALS key_equals;

		public:
			PairKeyCompare(const KEY_EQUALS& key_equals);

			sl_bool operator()(const Pair<KT, VT>& a, const KT& b) const;
		};

		template <class VALUE, class VALUE_EQUALS>
		class PairCompare
		{
		public:
			KEY_EQUALS key_equals;
			VALUE_EQUALS value_equals;

		public:
			PairCompare(const KEY_EQUALS& key_equals, const VALUE_EQUALS& value_equals);

			sl_bool operator()(const Pair<KT, VT>& a, const Pair<KT, VALUE>& b) const;
		};
	
		CList< Pair<KT, VT> > list;

	public:
		ListMap(const KEY_EQUALS& equals = KEY_EQUALS());
		
#ifdef SLIB_SUPPORT_STD_TYPES
		ListMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_EQUALS& equals = KEY_EQUALS());
#endif
		
	public:
		static ListMap<KT, VT, KEY_EQUALS>* create(const KEY_EQUALS& key_equals = KEY_EQUALS());

#ifdef SLIB_SUPPORT_STD_TYPES
		static ListMap<KT, VT, KEY_EQUALS>* create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_EQUALS& key_equals = KEY_EQUALS());
#endif
		
		VT operator[](const KT& key) const;

		// override
		sl_size getCount() const;

		// override
		VT* getItemPointer(const KT& key) const;

		// override
		List<VT> getValues_NoLock(const KT& key) const;

		// override
		sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue_NoLock(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		// override
		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null);

		// override
		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		// override
		sl_size removeAll_NoLock();

		// override
		sl_bool contains_NoLock(const KT& key) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		// override
		IMap<KT, VT>* duplicate_NoLock() const;

		// override
		Iterator<KT> getKeyIteratorWithRefer(Referable* refer) const;

		// override
		List<KT> getAllKeys_NoLock() const;

		// override
		Iterator<VT> getValueIteratorWithRefer(Referable* refer) const;

		// override
		List<VT> getAllValues_NoLock() const;

		// override
		Iterator< Pair<KT, VT> > toIteratorWithRefer(Referable* refer) const;

		// override
		List< Pair<KT, VT> > toList_NoLock() const;

	protected:
		KEY_EQUALS m_equals;
		
	};
	

	template < class KT, class VT, class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT HashMap : public IMap<KT, VT>
	{
	public:
		HashTable<KT, VT, HASH, KEY_EQUALS> table;

	public:
		HashMap(sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
		
#ifdef SLIB_SUPPORT_STD_TYPES
		HashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
#endif
		
	public:
		static HashMap<KT, VT, HASH, KEY_EQUALS>* create(sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
		
#ifdef SLIB_SUPPORT_STD_TYPES
		static HashMap<KT, VT, HASH, KEY_EQUALS>* create(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
#endif
		
		VT operator[](const KT& key) const;
	
		// override
		sl_size getCount() const;

		// override
		VT* getItemPointer(const KT& key) const;

		// override
		List<VT> getValues_NoLock(const KT& key) const;

		// override
		sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue_NoLock(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		// override
		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null);

		// override
		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		// override
		sl_size removeAll_NoLock();

		// override
		sl_bool contains_NoLock(const KT& key) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		// override
		IMap<KT, VT>* duplicate_NoLock() const;

		// override
		Iterator<KT> getKeyIteratorWithRefer(Referable* refer) const;

		// override
		List<KT> getAllKeys_NoLock() const;

		// override
		Iterator<VT> getValueIteratorWithRefer(Referable* refer) const;

		// override
		List<VT> getAllValues_NoLock() const;

		// override
		Iterator< Pair<KT, VT> > toIteratorWithRefer(Referable* refer) const;

		// override
		List< Pair<KT, VT> > toList_NoLock() const;
	
	};
	
	
/*
 TreeMap class Definition                                             
	Now TreeMap is based on BTree, but should be changed to Red-Black
	Tree implementation which is a little better for in-memory structure.
*/
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT TreeMap : public IMap<KT, VT>
	{
	public:
		RedBlackTree<KT, VT, KEY_COMPARE> tree;

	public:
		TreeMap(const KEY_COMPARE& key_compare = KEY_COMPARE());
	
#ifdef SLIB_SUPPORT_STD_TYPES
		TreeMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE());
#endif
		
	public:
		static TreeMap<KT, VT, KEY_COMPARE>* create(const KEY_COMPARE& key_compare = KEY_COMPARE());

#ifdef SLIB_SUPPORT_STD_TYPES
		static TreeMap<KT, VT, KEY_COMPARE>* create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE());
#endif
		
		VT operator[](const KT& key) const;
	
		// override
		sl_size getCount() const;

		// override
		VT* getItemPointer(const KT& key) const;

		// override
		List<VT> getValues_NoLock(const KT& key) const;

		// override
		sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue_NoLock(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool addIfNewKeyAndValue(const KT& key, const VALUE& value, sl_bool* pFlagExist = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		// override
		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null);

		// override
		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null);

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS());

		// override
		sl_size removeAll_NoLock();

		// override
		sl_bool contains_NoLock(const KT& key) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const;

		// override
		IMap<KT, VT>* duplicate_NoLock() const;

		// override
		Iterator<KT> getKeyIteratorWithRefer(Referable* refer) const;

		// override
		List<KT> getAllKeys_NoLock() const;

		// override
		Iterator<VT> getValueIteratorWithRefer(Referable* refer) const;

		// override
		List<VT> getAllValues_NoLock() const;

		// override
		Iterator< Pair<KT, VT> > toIteratorWithRefer(Referable* refer) const;

		// override
		List< Pair<KT, VT> > toList_NoLock() const;

	};
	
	
	template <class KT, class VT>
	class SLIB_EXPORT Map
	{
	public:
		Ref< IMap<KT, VT> > ref;
		SLIB_REF_WRAPPER(Map, IMap<KT, VT>)
	
	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Map(const std::initializer_list< Pair<KT, VT> >& l);
#endif
		
	public:
		static Map<KT, VT> create();

		template < class KEY_EQUALS = Equals<KT> >
		static Map<KT, VT> createList(const KEY_EQUALS& key_equals = KEY_EQUALS());
	
#ifdef SLIB_SUPPORT_STD_TYPES
		template < class KEY_EQUALS = Equals<KT> >
		static Map<KT, VT> createList(const std::initializer_list< Pair<KT, VT> >& l, const KEY_EQUALS& key_equals = KEY_EQUALS());
#endif
		
		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		static Map<KT, VT> createHash(sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
		
#ifdef SLIB_SUPPORT_STD_TYPES
		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		static Map<KT, VT> createHash(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());
#endif
	
		template < class KEY_COMPARE = Compare<KT> >
		static Map<KT, VT> createTree(const KEY_COMPARE& key_compare = KEY_COMPARE());
	
#ifdef SLIB_SUPPORT_STD_TYPES
		template < class KEY_COMPARE = Compare<KT> >
		static Map<KT, VT> createTree(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE());
#endif
		
		template <class KEY, class VALUE>
		static const Map<KT, VT>& from(const Map<KEY, VALUE>& other);
		
	public:
		void init();
		
		template < class KEY_EQUALS = Equals<KT> >
		void initList(const KEY_EQUALS& key_equals = KEY_EQUALS());

		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		void initHash(sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());

		template < class KEY_COMPARE = Compare<KT> >
		void initTree(const KEY_COMPARE& key_compare = KEY_COMPARE());

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Map<KT, VT>& operator=(const std::initializer_list< Pair<KT, VT> >& l);
#endif

		VT operator[](const KT& key) const;

		sl_size getCount() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

		VT* getItemPointer(const KT& key) const;

		VT* getNewItemPointer(const KT& key);

		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const;

		sl_bool get(const KT& key, VT* _out = sl_null) const;

		VT getValue_NoLock(const KT& key) const;

		VT getValue(const KT& key) const;

		VT getValue_NoLock(const KT& key, const VT& def) const;

		VT getValue(const KT& key, const VT& def) const;

		List<VT> getValues_NoLock(const KT& key) const;

		List<VT> getValues(const KT& key) const;

		sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
		
		template <class KEY, class VALUE>
		void putAll_NoLock(const Map<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default);
		
		template <class KEY, class VALUE>
		void putAll_NoLock(const AtomicMap<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default);

		template <class KEY, class VALUE>
		void putAll(const Map<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default);

		template <class KEY, class VALUE>
		void putAll(const AtomicMap<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default);

		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) const;

		sl_bool remove(const KT& key, VT* outValue = sl_null) const;

		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) const;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) const;

		sl_size removeAll_NoLock() const;

		sl_size removeAll() const;

		sl_bool contains_NoLock(const KT& key) const;

		sl_bool contains(const KT& key) const;

		Map<KT, VT> duplicate_NoLock() const;

		Map<KT, VT> duplicate() const;

		Iterator<KT> getKeyIterator() const;

		List<KT> getAllKeys_NoLock() const;

		List<KT> getAllKeys() const;

		Iterator<VT> getValueIterator() const;

		List<VT> getAllValues_NoLock() const;

		List<VT> getAllValues() const;

		Iterator< Pair<KT, VT> > toIterator() const;

		List< Pair<KT, VT> > toList_NoLock() const;

		List< Pair<KT, VT> > toList() const;

		// range-based for loop
		IteratorPosition< Pair<KT, VT> > begin() const;

		IteratorPosition< Pair<KT, VT> > end() const;
	
		const Mutex* getLocker() const;

	};
	
	template <class KT, class VT>
	class SLIB_EXPORT Atomic< Map<KT, VT> >
	{
	public:
		AtomicRef< IMap<KT, VT> > ref;
		SLIB_ATOMIC_REF_WRAPPER(IMap<KT, VT>)

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::initializer_list< Pair<KT, VT> >& l);
#endif
		
	public:
		template <class KEY, class VALUE>
		static const Atomic< Map<KT, VT> >& from(const Atomic< Map<KEY, VALUE> >& other);
		
		void init();
		
		template < class KEY_EQUALS = Equals<KT> >
		void initList(const KEY_EQUALS& key_equals = KEY_EQUALS());

		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		void initHash(sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS());

		template < class KEY_COMPARE = Compare<KT> >
		void initTree(const KEY_COMPARE& key_compare = KEY_COMPARE());

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic< Map<KT, VT> >& operator=(const std::initializer_list< Pair<KT, VT> >& l);
#endif

		VT operator[](const KT& key) const;

		sl_size getCount() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

		sl_bool get(const KT& key, VT* _out = sl_null) const;

		VT getValue(const KT& key) const;

		VT getValue(const KT& key, const VT& def) const;

		List<VT> getValues(const KT& key) const;

		sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

		template <class KEY, class VALUE>
		void putAll(const Map<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default);

		template <class KEY, class VALUE>
		void putAll(const AtomicMap<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default);

		sl_bool remove(const KT& key, VT* outValue = sl_null) const;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) const;

		sl_size removeAll() const;

		sl_bool contains(const KT& key) const;
	
		Map<KT, VT> duplicate() const;

		Iterator<KT> getKeyIterator() const;

		List<KT> getAllKeys() const;

		Iterator<VT> getValueIterator() const;

		List<VT> getAllValues() const;

		Iterator< Pair<KT, VT> > toIterator() const;

		List< Pair<KT, VT> > toList() const;

		// range-based for loop
		IteratorPosition< Pair<KT, VT> > begin() const;

		IteratorPosition< Pair<KT, VT> > end() const;
	
	};

}

#include "detail/map.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/map_std.inc"
#endif

#endif
