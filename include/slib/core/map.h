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
#include "map_common.h"
#include "node_position.h"
#include "pair.h"
#include "red_black_tree.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <initializer_list>
#endif

namespace slib
{
	
	template <class KT, class VT, class KEY_COMPARE>
	class CMap;
	
	template <class KT, class VT, class KEY_COMPARE>
	class Map;
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	using AtomicMap = Atomic< Map<KT, VT, KEY_COMPARE> >;
	
	
	template <class KT, class VT>
	class SLIB_EXPORT MapNode
	{
	public:
		MapNode* parent;
		MapNode* left;
		MapNode* right;
		sl_bool flagRed;
		
		KT key;
		VT value;
		
	public:
		MapNode(const MapNode& other) = delete;
		
#ifdef SLIB_COMPILER_IS_GCC
		MapNode(MapNode& other) = delete;
#endif
		
		template <class KEY, class... VALUE_ARGS>
		MapNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept;
		
	public:
		MapNode* getNext() const noexcept;
		
		MapNode* getPrevious() const noexcept;
		
	};
	
	extern const char _priv_Map_ClassID[];
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT CMap : public Object
	{
		SLIB_TEMPLATE_OBJECT(Object, _priv_Map_ClassID)
		
	public:
		typedef MapNode<KT, VT> NODE;
		typedef NodePosition<NODE> POSITION;
		
	protected:
		NODE* m_root;
		sl_size m_count;
		KEY_COMPARE m_compare;
		
	public:
		CMap() noexcept;

		CMap(const KEY_COMPARE& compare) noexcept;
		
		~CMap() noexcept;
		
	public:
		CMap(const CMap& other) = delete;
		
		CMap& operator=(const CMap& other) = delete;
		
		CMap(CMap&& other) noexcept;
		
		CMap& operator=(CMap&& other) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		CMap(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		NODE* getFirstNode() const noexcept;
		
		NODE* getLastNode() const noexcept;
		
		NODE* find_NoLock(const KT& key) const noexcept;
		
		sl_bool find(const KT& key) const noexcept;
		
		/* unsynchronized function */
		sl_bool getEqualRange(const KT& key, NODE** pStart = sl_null, NODE** pEnd = sl_null) const noexcept;
		
		/* unsynchronized function */
		void getNearest(const KT& key, NODE** pLessEqual = sl_null, NODE** pGreaterEqual = sl_null) const noexcept;
		
		/* unsynchronized function */
		NODE* getLowerBound(const KT& key) const noexcept;
		
		/* unsynchronized function */
		NODE* getUpperBound(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		NODE* findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		/* unsynchronized function */
		VT* getItemPointer(const KT& key) const noexcept;
		
		/* unsynchronized function */
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const noexcept;
		
		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;
		
		VT getValue_NoLock(const KT& key) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue_NoLock(const KT& key, const VT& def) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;
		
		List<VT> getValues_NoLock(const KT& key) const noexcept;
		
		List<VT> getValues(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template <class KEY, class VALUE>
		NODE* put_NoLock(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		NODE* replace_NoLock(const KEY& key, VALUE&& value) noexcept;
		
		template <class KEY, class VALUE>
		sl_bool replace(const KEY& key, VALUE&& value) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		NODE* add_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		sl_bool add(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		MapEmplaceReturn<NODE> emplace_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		sl_bool emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class MAP>
		sl_bool putAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool putAll(const MAP& other) noexcept;
		
		template <class MAP>
		void replaceAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		void replaceAll(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool addAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool addAll(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool emplaceAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool emplaceAll(const MAP& other) noexcept;
		
		/* unsynchronized function */
		void removeAt(NODE* node) noexcept;
		
		/* unsynchronized function */
		sl_size removeAt(NODE* node, sl_size count) noexcept;
		
		/* unsynchronized function */
		sl_size removeRange(NODE* first, NODE* last) noexcept;
		
		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_bool remove(const KT& key, VT* outValue = sl_null) noexcept;
		
		sl_size removeItems_NoLock(const KT& key) noexcept;
		
		sl_size removeItems(const KT& key) noexcept;
		
		List<VT> removeItemsAndReturnValues_NoLock(const KT& key) noexcept;
		
		List<VT> removeItemsAndReturnValues(const KT& key) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) noexcept;
		
		sl_size removeAll_NoLock() noexcept;
		
		sl_size removeAll() noexcept;
		
		sl_bool copyFrom_NoLock(const CMap& other) noexcept;

		sl_bool copyFrom(const CMap& other) noexcept;
		
		CMap* duplicate_NoLock() const noexcept;
		
		CMap* duplicate() const noexcept;
		
		List<KT> getAllKeys_NoLock() const noexcept;
		
		List<KT> getAllKeys() const noexcept;
		
		List<VT> getAllValues_NoLock() const noexcept;
		
		List<VT> getAllValues() const noexcept;
		
		List< Pair<KT, VT> > toList_NoLock() const noexcept;
		
		List< Pair<KT, VT> > toList() const noexcept;
		
		// range-based for loop
		POSITION begin() const noexcept;
		
		POSITION end() const noexcept;
		
	public:
		class EnumLockHelper
		{
		public:
			EnumLockHelper(const CMap& map) noexcept;
		public:
			NODE* node;
			Mutex* mutex;
		};
		
		class EnumHelper
		{
		public:
			EnumHelper(const CMap& map) noexcept;
		public:
			NODE* node;
		};
		
	};
	
	
	template < class KT, class VT, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT Map
	{
	public:
		Ref< CMap<KT, VT, KEY_COMPARE> > ref;
		SLIB_REF_WRAPPER(Map, CMap<KT, VT, KEY_COMPARE>)
		
	public:
		typedef MapNode<KT, VT> NODE;
		typedef NodePosition<NODE> POSITION;
		typedef CMap<KT, VT, KEY_COMPARE> CMAP;
		
	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Map(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		static Map create() noexcept;
		
		static Map create(const KEY_COMPARE& compare) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		static Map create(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& key_compare = KEY_COMPARE()) noexcept;
#endif
		
		template <class KEY, class VALUE, class OTHER_COMPARE>
		static const Map& from(const Map<KEY, VALUE, OTHER_COMPARE>& other) noexcept;
		
		void init() noexcept;
		
		void init(const KEY_COMPARE& compare) noexcept;

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Map& operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif
		
		VT operator[](const KT& key) const noexcept;
		
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		NODE* getFirstNode() const noexcept;
		
		NODE* getLastNode() const noexcept;
		
		NODE* find_NoLock(const KT& key) const noexcept;
		
		sl_bool find(const KT& key) const noexcept;
		
		/* unsynchronized function */
		sl_bool getEqualRange(const KT& key, NODE** pStart = sl_null, NODE** pEnd = sl_null) const noexcept;
		
		/* unsynchronized function */
		void getNearest(const KT& key, NODE** pLessEqual = sl_null, NODE** pGreaterEqual = sl_null) const noexcept;
		
		/* unsynchronized function */
		NODE* getLowerBound(const KT& key) const noexcept;
		
		/* unsynchronized function */
		NODE* getUpperBound(const KT& key) const noexcept;

		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		NODE* findKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		/* unsynchronized function */
		VT* getItemPointer(const KT& key) const noexcept;
		
		/* unsynchronized function */
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		VT* getItemPointerByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const noexcept;
		
		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;
		
		VT getValue_NoLock(const KT& key) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue_NoLock(const KT& key, const VT& def) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;
		
		List<VT> getValues_NoLock(const KT& key) const noexcept;
		
		List<VT> getValues(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template <class KEY, class VALUE>
		NODE* put_NoLock(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		NODE* replace_NoLock(const KEY& key, VALUE&& value) const noexcept;
		
		template <class KEY, class VALUE>
		sl_bool replace(const KEY& key, VALUE&& value) const noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		NODE* add_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		sl_bool add(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		MapEmplaceReturn<NODE> emplace_NoLock(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		sl_bool emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class MAP>
		sl_bool putAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool putAll(const MAP& other) noexcept;
		
		template <class MAP>
		void replaceAll_NoLock(const MAP& other) const noexcept;
		
		template <class MAP>
		void replaceAll(const MAP& other) const noexcept;
		
		template <class MAP>
		sl_bool addAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool addAll(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool emplaceAll_NoLock(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool emplaceAll(const MAP& other) noexcept;
		
		/* unsynchronized function */
		void removeAt(NODE* node) const noexcept;
		
		/* unsynchronized function */
		sl_size removeAt(NODE* node, sl_size count) const noexcept;
		
		/* unsynchronized function */
		sl_size removeRange(NODE* first, NODE* last) const noexcept;
		
		sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) const noexcept;
		
		sl_bool remove(const KT& key, VT* outValue = sl_null) const noexcept;
		
		sl_size removeItems_NoLock(const KT& key) const noexcept;
		
		sl_size removeItems(const KT& key) const noexcept;
		
		List<VT> removeItemsAndReturnValues_NoLock(const KT& key) const noexcept;
		
		List<VT> removeItemsAndReturnValues(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue_NoLock(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_size removeAll_NoLock() const noexcept;
		
		sl_size removeAll() const noexcept;
		
		Map duplicate_NoLock() const noexcept;
		
		Map duplicate() const noexcept;
		
		List<KT> getAllKeys_NoLock() const noexcept;
		
		List<KT> getAllKeys() const noexcept;
		
		List<VT> getAllValues_NoLock() const noexcept;
		
		List<VT> getAllValues() const noexcept;
		
		List< Pair<KT, VT> > toList_NoLock() const noexcept;
		
		List< Pair<KT, VT> > toList() const noexcept;
		
		// range-based for loop
		POSITION begin() const noexcept;
		
		POSITION end() const noexcept;
		
		const Mutex* getLocker() const noexcept;
		
	public:
		class EnumLockHelper
		{
		public:
			EnumLockHelper(const Map& map) noexcept;
		public:
			NODE* node;
			Mutex* mutex;
		};
		
		class EnumHelper
		{
		public:
			EnumHelper(const Map& map) noexcept;
		public:
			NODE* node;
		};
		
	};
	
	template <class KT, class VT, class KEY_COMPARE>
	class SLIB_EXPORT Atomic< Map<KT, VT, KEY_COMPARE> >
	{
	public:
		AtomicRef< CMap<KT, VT, KEY_COMPARE> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CMap<KT, VT, KEY_COMPARE>)
		
	public:
		typedef MapNode<KT, VT> NODE;
		typedef NodePositionWithRef<NODE> POSITION;
		typedef CMap<KT, VT, KEY_COMPARE> CMAP;
		
	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::initializer_list< Pair<KT, VT> >& l, const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		template <class KEY, class VALUE, class OTHER_COMPARE>
		static const Atomic& from(const Atomic< Map<KEY, VALUE, OTHER_COMPARE> >& other) noexcept;
		
		void init() noexcept;
		
		void init(const KEY_COMPARE& compare) noexcept;

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic< Map<KT, VT, KEY_COMPARE> >& operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif
		
		VT operator[](const KT& key) const noexcept;
		
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		sl_bool find(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;
		
		VT getValue(const KT& key) const noexcept;
		
		VT getValue(const KT& key, const VT& def) const noexcept;
		
		List<VT> getValues(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		List<VT> getValuesByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template <class KEY, class VALUE>
		sl_bool put(KEY&& key, VALUE&& value, sl_bool* isInsertion = sl_null) noexcept;
		
		template <class KEY, class VALUE>
		sl_bool replace(const KEY& key, VALUE&& value) const noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		sl_bool add(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class KEY, class... VALUE_ARGS>
		sl_bool emplace(KEY&& key, VALUE_ARGS&&... value_args) noexcept;
		
		template <class MAP>
		sl_bool putAll(const MAP& other) noexcept;
		
		template <class MAP>
		void replaceAll(const MAP& other) const noexcept;
		
		template <class MAP>
		sl_bool addAll(const MAP& other) noexcept;
		
		template <class MAP>
		sl_bool emplaceAll(const MAP& other) noexcept;
		
		sl_bool remove(const KT& key, VT* outValue = sl_null) const noexcept;
		
		sl_size removeItems(const KT& key) const noexcept;
		
		List<VT> removeItemsAndReturnValues(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool removeKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_size removeItemsByKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_size removeAll() const noexcept;
		
		Map<KT, VT, KEY_COMPARE> duplicate() const noexcept;
		
		List<KT> getAllKeys() const noexcept;
		
		List<VT> getAllValues() const noexcept;
		
		List< Pair<KT, VT> > toList() const noexcept;
		
		// range-based for loop
		POSITION begin() const noexcept;
		
		POSITION end() const noexcept;
		
	public:
		class EnumLockHelper
		{
		public:
			EnumLockHelper(const Map<KT, VT, KEY_COMPARE>& map) noexcept;
		public:
			NODE* node;
			Mutex* mutex;
			Ref<Referable> ref;
		};
		
		class EnumHelper
		{
		public:
			EnumHelper(const Map<KT, VT, KEY_COMPARE>& map) noexcept;
		public:
			NODE* node;
			Ref<Referable> ref;
		};
		
	};
	
}

#include "detail/map.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/map_std.inc"
#endif

#endif
