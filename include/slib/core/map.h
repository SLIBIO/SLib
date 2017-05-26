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

#include "pair.h"
#include "object.h"
#include "list.h"
#include "hash_table.h"
#include "red_black_tree.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <initializer_list>
#endif

namespace slib
{
	
	template <class KT, class VT>
	class IMap;
	
	template <class KT, class VT>
	class Map;
	
	template <class KT, class VT>
	using AtomicMap = Atomic< Map<KT, VT> >;

	
	template <class KT, class VT>
	class SLIB_EXPORT MapPosition
	{
	public:
		MapPosition() noexcept;
		
		MapPosition(sl_null_t) noexcept;
		
		MapPosition(MapContainerType containerType, void* node, Referable* ref) noexcept;
		
		MapPosition(const MapPosition& other) noexcept = default;
		
		MapPosition(MapPosition&& other) noexcept = default;
		
	public:
		MapPosition& operator=(const MapPosition& other) noexcept = default;
		
		MapPosition& operator=(MapPosition&& other) noexcept = default;
		
		Pair<KT, VT>& operator*() const noexcept;
		
		sl_bool operator==(const MapPosition& other) const noexcept;
		
		sl_bool operator!=(const MapPosition& other) const noexcept;
		
		explicit operator sl_bool() const noexcept;
		
		MapPosition<KT, VT>& operator++() noexcept;
		
	public:
		void* node;
		MapContainerType containerType;
		Ref<Referable> ref;
		
	};

	
	extern const char _priv_Map_ClassID[];
	
	template <class KT, class VT>
	class SLIB_EXPORT IMap : public Object
	{
		SLIB_TEMPLATE_OBJECT(Object, _priv_Map_ClassID)
	
	public:
		IMap() noexcept;
		
	public:
		static IMap* createDefault() noexcept;
		
		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;
	
		sl_bool isNotEmpty() const noexcept;
	
		VT* getItemPointer(const KT& key) const noexcept;

		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const noexcept;

		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;

		VT getValue_NoLock(const KT& key) const noexcept;

		VT getValue(const KT& key) const noexcept;

		VT getValue_NoLock(const KT& key, const VT& def) const noexcept;

		VT getValue(const KT& key, const VT& def) const noexcept;

		List<VT> getValues_NoLock(const KT& key) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, MapPosition<KT, VT>* pPosition = sl_null) noexcept;

		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, MapPosition<KT, VT>* pPosition = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		void putAll_NoLock(IMap<KEY, VALUE>* other, MapPutMode mode = MapPutMode::Default) noexcept;

		template <class KEY, class VALUE>
		void putAll(IMap<KEY, VALUE>* other, MapPutMode mode = MapPutMode::Default) noexcept;

		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;

		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		sl_size removeAll_NoLock() noexcept;

		sl_size removeAll() noexcept;
		
		void removeAt_NoLock(const MapPosition<KT, VT>& position) noexcept;
		
		void removeAt(const MapPosition<KT, VT>& position) noexcept;
		
		MapPosition<KT, VT> find_NoLock(const KT& key) noexcept;
		
		MapPosition<KT, VT> find(const KT& key) noexcept;
		
		sl_bool find_NoLock(const KT& key, MapPosition<KT, VT>* pPosition) noexcept;
		
		sl_bool find(const KT& key, MapPosition<KT, VT>* pPosition) noexcept;

		sl_bool contains_NoLock(const KT& key) const noexcept;

		sl_bool contains(const KT& key) const noexcept;

		IMap* duplicate_NoLock() const noexcept;

		IMap* duplicate() const noexcept;

		List<KT> getAllKeys_NoLock() const noexcept;

		List<KT> getAllKeys() const noexcept;

		List<VT> getAllValues_NoLock() const noexcept;

		List<VT> getAllValues() const noexcept;

		List< Pair<KT, VT> > toList_NoLock() const noexcept;

		List< Pair<KT, VT> > toList() const noexcept;

		// range-based for loop
		MapPosition<KT, VT> begin() const noexcept;

		MapPosition<KT, VT> end() const noexcept;
		
		MapPosition<KT, VT> getFirst(Referable* ref = sl_null) const noexcept;
		
	protected:
		virtual sl_size _getCount() const noexcept = 0;
		
		virtual VT* _getItemPointer(const KT& key) const noexcept = 0;
		
		virtual List<VT> _getValues(const KT& key) const noexcept = 0;
		
		virtual sl_bool _put(const KT& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept = 0;
		
		virtual sl_bool _put(const KT& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept = 0;
		
		virtual sl_bool _put(KT&& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept = 0;
		
		virtual sl_bool _put(KT&& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept = 0;
		
		virtual sl_bool _remove(const KT& key, VT* outValue) noexcept = 0;
		
		virtual sl_size _removeItems(const KT& key, List<VT>* outValues) noexcept = 0;
		
		virtual sl_size _removeAll() noexcept = 0;
		
		virtual void _removeAt(const MapPosition<KT, VT>& position) noexcept = 0;
		
		virtual sl_bool _find(const KT& key, MapPosition<KT, VT>* pPosition) const noexcept = 0;
		
		virtual IMap* _duplicate() const noexcept = 0;
		
		virtual List<KT> _getAllKeys() const noexcept = 0;
		
		virtual List<VT> _getAllValues() const noexcept = 0;
		
		virtual List< Pair<KT, VT> > _toList() const noexcept = 0;
		
		virtual MapPosition<KT, VT> _getFirst(Referable* ref) const noexcept = 0;
		
	};
	

	template <class KT, class VT>
	using HashMapNode = HashTableNode<KT, VT>;
	
	template <class KT, class VT>
	class SLIB_EXPORT HashMapPosition
	{
	public:
		HashMapPosition() noexcept;
		
		HashMapPosition(HashMapNode<KT, VT>* node) noexcept;
		
		HashMapPosition(const HashMapPosition& other) noexcept = default;
		
		HashMapPosition(HashMapPosition&& other) noexcept = default;

	public:
		HashMapPosition& operator=(const HashMapPosition& other) noexcept = default;
		
		HashMapPosition& operator=(HashMapPosition&& other) noexcept = default;
		
		HashMapPosition& operator=(HashMapNode<KT, VT>* node) noexcept;

		Pair<KT, VT>& operator*() const noexcept;
		
		sl_bool operator==(const HashMapPosition& other) const noexcept;
		
		sl_bool operator!=(const HashMapPosition& other) const noexcept;
		
		operator HashMapNode<KT, VT>*() const noexcept;
		
		HashMapPosition<KT, VT>& operator++() noexcept;
		
	public:
		HashMapNode<KT, VT>* node;
		
	};

	
	template < class KT, class VT, class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
	class SLIB_EXPORT HashMap final : public IMap<KT, VT>
	{
	public:
		HashTable<KT, VT, HASH, KEY_EQUALS> table;

	public:
		HashMap() noexcept;
		
		HashMap(sl_uint32 capacity) noexcept;
		
		HashMap(sl_uint32 capacity, const HASH& hash) noexcept;
		
		HashMap(sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		HashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
#endif
		
	public:
		HashMap(const HashMap& other) = delete;
		
		HashMap(HashMap&& other) noexcept;
		
		HashMap& operator=(const HashMap& other) = delete;
		
		HashMap& operator=(HashMap&& other) noexcept;
		
	public:
		static HashMap* create(sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		static HashMap* create(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 capacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
#endif
		
		VT operator[](const KT& key) const noexcept;
	
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		VT* getItemPointer(const KT& key) const noexcept;

		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const noexcept;
		
		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;
		
		VT getValue_NoLock(const KT& key) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue_NoLock(const KT& key, const VT& def) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;
		
		List<VT> getValues_NoLock(const KT& key) const noexcept;
		
		List<VT> getValues(const KT& key) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, HashMapNode<KT, VT>** ppNode = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, HashMapNode<KT, VT>** ppNode = sl_null) noexcept;

		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue_NoLock(KEY&& key, VALUE&& value, HashMapNode<KT, VT>** ppNode = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue(KEY&& key, VALUE&& value, HashMapNode<KT, VT>** ppNode = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) noexcept;
		
		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;

		sl_size removeAll_NoLock() noexcept;
		
		sl_size removeAll() noexcept;
		
		void removeAt_NoLock(const HashMapNode<KT, VT>* node) noexcept;
		
		void removeAt(const HashMapNode<KT, VT>* node) noexcept;
		
		HashMapNode<KT, VT>* find_NoLock(const KT& key) const noexcept;
		
		HashMapNode<KT, VT>* find(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		HashMapNode<KT, VT>* findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		HashMapNode<KT, VT>* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool contains_NoLock(const KT& key) const noexcept;
		
		sl_bool contains(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		HashMapNode<KT, VT>* getFirstNode() const noexcept;
		
		HashMapNode<KT, VT>* getLastNode() const noexcept;
		
		HashMap* duplicate_NoLock() const noexcept;
		
		HashMap* duplicate() const noexcept;

		List<KT> getAllKeys_NoLock() const noexcept;
		
		List<KT> getAllKeys() const noexcept;

		List<VT> getAllValues_NoLock() const noexcept;
		
		List<VT> getAllValues() const noexcept;

		List< Pair<KT, VT> > toList_NoLock() const noexcept;
		
		List< Pair<KT, VT> > toList() const noexcept;
		
		// range-based for loop
		HashMapPosition<KT, VT> begin() const noexcept;
		
		HashMapPosition<KT, VT> end() const noexcept;
		
		sl_bool copyFrom(const HashMap* other) noexcept;

	protected:
		sl_size _getCount() const noexcept override;
		
		VT* _getItemPointer(const KT& key) const noexcept override;
		
		List<VT> _getValues(const KT& key) const noexcept override;
		
		sl_bool _put(const KT& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _put(const KT& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _put(KT&& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _put(KT&& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _remove(const KT& key, VT* outValue) noexcept override;
		
		sl_size _removeItems(const KT& key, List<VT>* outValues) noexcept override;
		
		sl_size _removeAll() noexcept override;
		
		void _removeAt(const MapPosition<KT, VT>& position) noexcept override;
		
		sl_bool _find(const KT& key, MapPosition<KT, VT>* pPosition) const noexcept override;
		
		IMap<KT, VT>* _duplicate() const noexcept override;
		
		List<KT> _getAllKeys() const noexcept override;
		
		List<VT> _getAllValues() const noexcept override;
		
		List< Pair<KT, VT> > _toList() const noexcept override;
		
		MapPosition<KT, VT> _getFirst(Referable* ref) const noexcept override;
		
	};
	

	template <class KT, class VT>
	using TreeMapNode = RedBlackTreeNode<KT, VT>;
	
	template <class KT, class VT>
	class SLIB_EXPORT TreeMapPosition
	{
	public:
		TreeMapPosition() noexcept;
		
		TreeMapPosition(TreeMapNode<KT, VT>* node) noexcept;
		
		TreeMapPosition(const TreeMapPosition& other) noexcept = default;
		
		TreeMapPosition(TreeMapPosition&& other) noexcept = default;
		
	public:
		TreeMapPosition& operator=(const TreeMapPosition& other) noexcept = default;
		
		TreeMapPosition& operator=(TreeMapPosition&& other) noexcept = default;
		
		TreeMapPosition& operator=(TreeMapNode<KT, VT>* node) noexcept;
		
		Pair<KT, VT>& operator*() const noexcept;
		
		sl_bool operator==(const TreeMapPosition& other) const noexcept;
		
		sl_bool operator!=(const TreeMapPosition& other) const noexcept;
		
		operator TreeMapNode<KT, VT>*() const noexcept;
		
		TreeMapPosition<KT, VT>& operator++() noexcept;
		
	public:
		TreeMapNode<KT, VT>* node;
		
	};
	

	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT TreeMap final : public IMap<KT, VT>
	{
	public:
		RedBlackTree<KT, VT, KEY_COMPARE> tree;

	public:
		TreeMap() noexcept;
		
		TreeMap(const KEY_COMPARE& key_compare) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		TreeMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		TreeMap(const TreeMap& other) = delete;
		
		TreeMap(TreeMap&& other) noexcept;
		
		TreeMap& operator=(const TreeMap& other) = delete;
		
		TreeMap& operator=(TreeMap&& other) noexcept;

	public:
		static TreeMap* create(const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		static TreeMap* create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;
#endif
		
		VT operator[](const KT& key) const noexcept;
		
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		VT* getItemPointer(const KT& key) const noexcept;
		
		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const noexcept;
		
		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;
		
		VT getValue_NoLock(const KT& key) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue_NoLock(const KT& key, const VT& def) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;
		
		List<VT> getValues_NoLock(const KT& key) const noexcept;
		
		List<VT> getValues(const KT& key) const noexcept;
		
		template <class KEY, class VALUE>
		sl_bool put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, TreeMapNode<KT, VT>** ppNode = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, TreeMapNode<KT, VT>** ppNode = sl_null) noexcept;
		
		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue_NoLock(KEY&& key, VALUE&& value, TreeMapNode<KT, VT>** ppNode = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class KEY, class VALUE, class VALUE_EQUALS = Equals<VT, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNewKeyAndValue(KEY&& key, VALUE&& value, TreeMapNode<KT, VT>** ppNode = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) noexcept;
		
		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, VT* outValue = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, List<VT>* outValues = sl_null, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		sl_size removeAll_NoLock() noexcept;
		
		sl_size removeAll() noexcept;
		
		void removeAt_NoLock(const TreeMapNode<KT, VT>* node) noexcept;
		
		void removeAt(const TreeMapNode<KT, VT>* node) noexcept;

		TreeMapNode<KT, VT>* find_NoLock(const KT& key) const noexcept;
		
		TreeMapNode<KT, VT>* find(const KT& key) const noexcept;
		
		sl_bool find_NoLock(const KT& key, TreeMapNode<KT, VT>** ppNode) const noexcept;
		
		sl_bool find(const KT& key, TreeMapNode<KT, VT>** ppNode) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		TreeMapNode<KT, VT>* findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		TreeMapNode<KT, VT>* findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool searchKeyAndValue_NoLock(const KT& key, const VALUE& value, TreeMapNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool searchKeyAndValue(const KT& key, const VALUE& value, TreeMapNode<KT, VT>** ppNode, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;

		sl_bool contains_NoLock(const KT& key) const noexcept;
		
		sl_bool contains(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool containsKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		TreeMapNode<KT, VT>* getFirstNode() const noexcept;
		
		TreeMapNode<KT, VT>* getLastNode() const noexcept;
		
		TreeMap* duplicate_NoLock() const noexcept;
		
		TreeMap* duplicate() const noexcept;
		
		List<KT> getAllKeys_NoLock() const noexcept;
		
		List<KT> getAllKeys() const noexcept;
		
		List<VT> getAllValues_NoLock() const noexcept;
		
		List<VT> getAllValues() const noexcept;
		
		List< Pair<KT, VT> > toList_NoLock() const noexcept;
		
		List< Pair<KT, VT> > toList() const noexcept;
		
		// range-based for loop
		TreeMapPosition<KT, VT> begin() const noexcept;
		
		TreeMapPosition<KT, VT> end() const noexcept;
		
		sl_bool copyFrom(const TreeMap* other) noexcept;
		
	protected:
		sl_size _getCount() const noexcept override;
		
		VT* _getItemPointer(const KT& key) const noexcept override;
		
		List<VT> _getValues(const KT& key) const noexcept override;
		
		sl_bool _put(const KT& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _put(const KT& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _put(KT&& key, const VT& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _put(KT&& key, VT&& value, MapPutMode mode, MapPosition<KT, VT>* pPosition) noexcept override;
		
		sl_bool _remove(const KT& key, VT* outValue) noexcept override;
		
		sl_size _removeItems(const KT& key, List<VT>* outValues) noexcept override;
		
		sl_size _removeAll() noexcept override;
		
		void _removeAt(const MapPosition<KT, VT>& position) noexcept override;
		
		sl_bool _find(const KT& key, MapPosition<KT, VT>* pPosition) const noexcept override;

		IMap<KT, VT>* _duplicate() const noexcept override;
		
		List<KT> _getAllKeys() const noexcept override;
		
		List<VT> _getAllValues() const noexcept override;
		
		List< Pair<KT, VT> > _toList() const noexcept override;
		
		MapPosition<KT, VT> _getFirst(Referable* ref) const noexcept override;
		
	};
	
	
	template <class KT, class VT>
	class SLIB_EXPORT Map
	{
	public:
		Ref< IMap<KT, VT> > ref;
		SLIB_REF_WRAPPER(Map, IMap<KT, VT>)
	
	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Map(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif
		
	public:
		static Map create() noexcept;

		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		static Map createHash(sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		static Map createHash(const std::initializer_list< Pair<KT, VT> >& l, sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
#endif
	
		template < class KEY_COMPARE = Compare<KT> >
		static Map createTree(const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;
	
#ifdef SLIB_SUPPORT_STD_TYPES
		template < class KEY_COMPARE = Compare<KT> >
		static Map createTree(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;
#endif
		
		template <class KEY, class VALUE>
		static const Map& from(const Map<KEY, VALUE>& other) noexcept;
		
	public:
		void init() noexcept;
		
		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		void initHash(sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;
		
		template < class KEY_COMPARE = Compare<KT> >
		void initTree(const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Map& operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif

		VT operator[](const KT& key) const noexcept;

		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		VT* getItemPointer(const KT& key) const noexcept;

		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const noexcept;

		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;

		VT getValue_NoLock(const KT& key) const noexcept;

		VT getValue(const KT& key) const noexcept;

		VT getValue_NoLock(const KT& key, const VT& def) const noexcept;

		VT getValue(const KT& key, const VT& def) const noexcept;

		List<VT> getValues_NoLock(const KT& key) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put_NoLock(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, MapPosition<KT, VT>* pPosition = sl_null) noexcept;

		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, MapPosition<KT, VT>* pPosition = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		void putAll_NoLock(const Map<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default) noexcept;
		
		template <class KEY, class VALUE>
		void putAll_NoLock(const AtomicMap<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default) noexcept;

		template <class KEY, class VALUE>
		void putAll(const Map<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default) noexcept;

		template <class KEY, class VALUE>
		void putAll(const AtomicMap<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default) noexcept;

		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) const noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) const noexcept;

		sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) const noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) const noexcept;

		sl_size removeAll_NoLock() const noexcept;

		sl_size removeAll() const noexcept;
		
		void removeAt_NoLock(const MapPosition<KT, VT>& position) const noexcept;
		
		void removeAt(const MapPosition<KT, VT>& position) const noexcept;
		
		MapPosition<KT, VT> find_NoLock(const KT& key) noexcept;
		
		MapPosition<KT, VT> find(const KT& key) noexcept;
		
		sl_bool find_NoLock(const KT& key, MapPosition<KT, VT>* pPosition) noexcept;
		
		sl_bool find(const KT& key, MapPosition<KT, VT>* pPosition) noexcept;

		sl_bool contains_NoLock(const KT& key) const noexcept;

		sl_bool contains(const KT& key) const noexcept;

		Map duplicate_NoLock() const noexcept;

		Map duplicate() const noexcept;

		List<KT> getAllKeys_NoLock() const noexcept;

		List<KT> getAllKeys() const noexcept;

		List<VT> getAllValues_NoLock() const noexcept;

		List<VT> getAllValues() const noexcept;

		List< Pair<KT, VT> > toList_NoLock() const noexcept;

		List< Pair<KT, VT> > toList() const noexcept;

		// range-based for loop
		MapPosition<KT, VT> begin() const noexcept;

		MapPosition<KT, VT> end() const noexcept;
	
		const Mutex* getLocker() const noexcept;

	};
	
	template <class KT, class VT>
	class SLIB_EXPORT Atomic< Map<KT, VT> >
	{
	public:
		AtomicRef< IMap<KT, VT> > ref;
		SLIB_ATOMIC_REF_WRAPPER(IMap<KT, VT>)

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif
		
	public:
		template <class KEY, class VALUE>
		static const Atomic< Map<KT, VT> >& from(const Atomic< Map<KEY, VALUE> >& other) noexcept;
		
		void init() noexcept;

		template < class HASH = Hash<KT>, class KEY_EQUALS = Equals<KT> >
		void initHash(sl_uint32 initialCapacity = 0, const HASH& hash = HASH(), const KEY_EQUALS& key_equals = KEY_EQUALS()) noexcept;

		template < class KEY_COMPARE = Compare<KT> >
		void initTree(const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic< Map<KT, VT> >& operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif

		VT operator[](const KT& key) const noexcept;

		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;

		VT getValue(const KT& key) const noexcept;

		VT getValue(const KT& key, const VT& def) const noexcept;

		List<VT> getValues(const KT& key) const noexcept;

		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, MapPutMode mode = MapPutMode::Default, MapPosition<KT, VT>* pPosition = sl_null) noexcept;

		template <class KEY, class VALUE>
		void putAll(const Map<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default) noexcept;

		template <class KEY, class VALUE>
		void putAll(const AtomicMap<KEY, VALUE>& other, MapPutMode mode = MapPutMode::Default) noexcept;

		sl_bool remove(const KT& key, VT* outValue = sl_null) const noexcept;

		sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) const noexcept;

		sl_size removeAll() const noexcept;
		
		void removeAt(const MapPosition<KT, VT>& position) const noexcept;
		
		MapPosition<KT, VT> find(const KT& key) noexcept;

		sl_bool find(const KT& key, MapPosition<KT, VT>* pPosition) noexcept;
		
		sl_bool contains(const KT& key) const noexcept;
	
		Map<KT, VT> duplicate() const noexcept;

		List<KT> getAllKeys() const noexcept;

		List<VT> getAllValues() const noexcept;

		List< Pair<KT, VT> > toList() const noexcept;

		// range-based for loop
		MapPosition<KT, VT> begin() const noexcept;
		
		MapPosition<KT, VT> end() const noexcept;
	
	};

}

#include "detail/map.inc"
#include "detail/hash_map.inc"
#include "detail/tree_map.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/map_std.inc"
#endif

#endif
