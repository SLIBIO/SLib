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

#ifndef CHECKHEADER_SLIB_CORE_HASH_MAP
#define CHECKHEADER_SLIB_CORE_HASH_MAP

#include "definition.h"

#include "map.h"

#include "hash.h"
#include "hash_table.h"
#include "math.h"

namespace slib
{
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	class CHashMap;
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	class HashMap;
	
	template < class KT, class VT, class HASH = Hash<KT>, class KEY_COMPARE = Compare<KT> >
	using AtomicHashMap = Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >;
	
	
	template <class KT, class VT>
	class SLIB_EXPORT HashMapNode
	{
	public:
		HashMapNode* parent;
		HashMapNode* left;
		HashMapNode* right;
		sl_bool flagRed;
		
		KT key;
		VT value;
		
		sl_size hash;
		HashMapNode* previous;
		HashMapNode* next;
		
	public:
		HashMapNode(const HashMapNode& other) = delete;

#ifdef SLIB_COMPILER_IS_GCC
		HashMapNode(HashMapNode& other) = delete;
#endif

		template <class KEY, class... VALUE_ARGS>
		HashMapNode(KEY&& _key, VALUE_ARGS&&... value_args) noexcept;
		
	public:
		HashMapNode* getNext() const noexcept;
		
		HashMapNode* getPrevious() const noexcept;
		
	};
	
	namespace priv
	{
		namespace hash_map
		{
			extern const char g_classID[];
		}
	}
	
	template < class KT, class VT, class HASH = Hash<KT>, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT CHashMap : public Object
	{
		SLIB_TEMPLATE_OBJECT(Object, priv::hash_map::g_classID)
		
	public:
		typedef KT KEY_TYPE;
		typedef VT VALUE_TYPE;
		typedef HashMapNode<KT, VT> NODE;
		typedef NodePosition<NODE> POSITION;
		
		struct TABLE
		{
			NODE** nodes;
			sl_size count;
			sl_size capacity;
			sl_size capacityMinimum;
			sl_size capacityMaximum;
			sl_size thresholdDown;
			sl_size thresholdUp;
		};
		
	protected:
		TABLE m_table;
		NODE* m_nodeFirst;
		NODE* m_nodeLast;
		HASH m_hash;
		KEY_COMPARE m_compare;
		
	public:
		CHashMap(sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
		
		~CHashMap() noexcept;
		
	public:
		CHashMap(const CHashMap& other) = delete;
		
		CHashMap& operator=(const CHashMap& other) = delete;
		
		CHashMap(CHashMap&& other) noexcept;
		
		CHashMap& operator=(CHashMap&& other) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		CHashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		sl_size getCapacity() const noexcept;
		
		sl_size getMinimumCapacity() const noexcept;
		
		void setMinimumCapacity_NoLock(sl_size capacity) noexcept;
		
		void setMinimumCapacity(sl_size capacity) noexcept;
		
		sl_size getMaximumCapacity() const noexcept;
		
		void setMaximumCapacity_NoLock(sl_size capacity) noexcept;
		
		void setMaximumCapacity(sl_size capacity) noexcept;
		
		NODE* getFirstNode() const noexcept;
		
		NODE* getLastNode() const noexcept;
		
		NODE* find_NoLock(const KT& key) const noexcept;
		
		sl_bool find(const KT& key) const noexcept;
		
		/* unsynchronized function */
		sl_bool getEqualRange(const KT& key, MapNode<KT, VT>** pStart = sl_null, MapNode<KT, VT>** pEnd = sl_null) const noexcept;
		
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
		
		sl_bool get_NoLock(const KT& key, Nullable<VT>* _out) const noexcept;
		
		sl_bool get(const KT& key, Nullable<VT>* _out) const noexcept;
		
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
		
		void shrink_NoLock() noexcept;
		
		void shrink() noexcept;
		
		sl_bool copyFrom_NoLock(const CHashMap& other) noexcept;
		
		sl_bool copyFrom(const CHashMap& other) noexcept;
		
		CHashMap* duplicate_NoLock() const noexcept;
		
		CHashMap* duplicate() const noexcept;
		
		List<KT> getAllKeys_NoLock() const noexcept;
		
		List<KT> getAllKeys() const noexcept;
		
		List<VT> getAllValues_NoLock() const noexcept;
		
		List<VT> getAllValues() const noexcept;
		
		List< Pair<KT, VT> > toList_NoLock() const noexcept;
		
		List< Pair<KT, VT> > toList() const noexcept;
		
		// range-based for loop
		POSITION begin() const noexcept;
		
		POSITION end() const noexcept;
		
	protected:
		void _free() noexcept;
		
		NODE* _getEntry(const KT& key) const noexcept;
		
		NODE** _getEntryPtr(const KT& key) noexcept;
		
        void _linkNode(NODE* node, sl_size hash) noexcept;
		
		void _unlinkNode(NODE* node) noexcept;
        
		void _expand() noexcept;
		
        void _rebuildTree(sl_size capacity) noexcept;
		
		sl_bool _copyFrom(const CHashMap* other) noexcept;
		
	public:
		class EnumLockHelper
		{
		public:
			EnumLockHelper(const CHashMap& map) noexcept;
		public:
			NODE* node;
			Mutex* mutex;
		};
		
		class EnumHelper
		{
		public:
			EnumHelper(const CHashMap& map) noexcept;
		public:
			NODE* node;
		};
		
	};
	
	
	template < class KT, class VT, class HASH = Hash<KT>, class KEY_COMPARE = Compare<KT> >
	class SLIB_EXPORT HashMap
	{
	public:
		Ref< CHashMap<KT, VT, HASH, KEY_COMPARE> > ref;
		SLIB_REF_WRAPPER(HashMap, CHashMap<KT, VT, HASH, KEY_COMPARE>)
		
	public:
		typedef KT KEY_TYPE;
		typedef VT VALUE_TYPE;
		typedef HashMapNode<KT, VT> NODE;
		typedef NodePosition<NODE> POSITION;
		typedef CHashMap<KT, VT, HASH, KEY_COMPARE> CMAP;
		
	public:
		HashMap(sl_size capacityMinimum, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		HashMap(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		static HashMap create(sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		static HashMap create(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
		template <class KEY, class VALUE, class OTHER_HASH, class OTHER_COMPARE>
		static const HashMap& from(const HashMap<KEY, VALUE, OTHER_HASH, OTHER_COMPARE>& other) noexcept;
		
		void initialize(sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;

	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		HashMap& operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif
		
		VT operator[](const KT& key) const noexcept;
		
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		sl_size getCapacity() const noexcept;
		
		sl_size getMinimumCapacity() const noexcept;
		
		void setMinimumCapacity_NoLock(sl_size capacity) noexcept;
		
		void setMinimumCapacity(sl_size capacity) noexcept;
		
		sl_size getMaximumCapacity() const noexcept;
		
		void setMaximumCapacity_NoLock(sl_size capacity) noexcept;
		
		void setMaximumCapacity(sl_size capacity) noexcept;
		
		NODE* getFirstNode() const noexcept;
		
		NODE* getLastNode() const noexcept;
		
		NODE* find_NoLock(const KT& key) const noexcept;
		
		sl_bool find(const KT& key) const noexcept;
		
		/* unsynchronized function */
		sl_bool getEqualRange(const KT& key, MapNode<KT, VT>** pStart = sl_null, MapNode<KT, VT>** pEnd = sl_null) const noexcept;
		
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
		
		sl_bool get_NoLock(const KT& key, Nullable<VT>* _out) const noexcept;
		
		sl_bool get(const KT& key, Nullable<VT>* _out) const noexcept;
		
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
		
		void shrink_NoLock() noexcept;
		
		void shrink() noexcept;
		
		HashMap duplicate_NoLock() const noexcept;
		
		HashMap duplicate() const noexcept;
		
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
			EnumLockHelper(const HashMap& map) noexcept;
		public:
			NODE* node;
			Mutex* mutex;
		};
		
		class EnumHelper
		{
		public:
			EnumHelper(const HashMap& map) noexcept;
		public:
			NODE* node;
		};
		
	};
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	class SLIB_EXPORT Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >
	{
	public:
		AtomicRef< CHashMap<KT, VT, HASH, KEY_COMPARE> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CHashMap<KT, VT, HASH, KEY_COMPARE>)
		
	public:
		typedef KT KEY_TYPE;
		typedef VT VALUE_TYPE;
		typedef HashMapNode<KT, VT> NODE;
		typedef NodePositionWithRef<NODE> POSITION;
		typedef CHashMap<KT, VT, HASH, KEY_COMPARE> CMAP;
		
	public:
		Atomic(sl_size capacityMinimum, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::initializer_list< Pair<KT, VT> >& l, sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
#endif
		
	public:
		template <class KEY, class VALUE, class OTHER_HASH, class OTHER_COMPARE>
		static const Atomic& from(const Atomic< HashMap<KEY, VALUE, OTHER_HASH, OTHER_COMPARE> >& other) noexcept;
		
		void initialize(sl_size capacityMinimum = 0, sl_size capacityMaximum = 0, const HASH& hash = HASH(), const KEY_COMPARE& compare = KEY_COMPARE()) noexcept;
		
	public:
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic< HashMap<KT, VT, HASH, KEY_COMPARE> >& operator=(const std::initializer_list< Pair<KT, VT> >& l) noexcept;
#endif
		
		VT operator[](const KT& key) const noexcept;
		
		sl_size getCount() const noexcept;
		
		sl_bool isEmpty() const noexcept;
		
		sl_bool isNotEmpty() const noexcept;
		
		sl_size getCapacity() const noexcept;
		
		sl_size getMinimumCapacity() const noexcept;
		
		void setMinimumCapacity(sl_size capacity) noexcept;
		
		sl_size getMaximumCapacity() const noexcept;
		
		void setMaximumCapacity(sl_size capacity) noexcept;
		
		sl_bool find(const KT& key) const noexcept;
		
		template < class VALUE, class VALUE_EQUALS = Equals<VT, VALUE> >
		sl_bool findKeyAndValue(const KT& key, const VALUE& value, const VALUE_EQUALS& value_equals = VALUE_EQUALS()) const noexcept;
		
		sl_bool get(const KT& key, VT* _out = sl_null) const noexcept;
		
		sl_bool get(const KT& key, Nullable<VT>* _out) const noexcept;

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
		
		void shrink() noexcept;
		
		HashMap<KT, VT, HASH, KEY_COMPARE> duplicate() const noexcept;
		
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
			EnumLockHelper(const HashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept;
		public:
			NODE* node;
			Mutex* mutex;
			Ref<Referable> ref;
		};
		
		class EnumHelper
		{
		public:
			EnumHelper(const HashMap<KT, VT, HASH, KEY_COMPARE>& map) noexcept;
		public:
			NODE* node;
			Ref<Referable> ref;
		};
		
	};
	
}

#include "detail/hash_map.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/hash_map_std.inc"
#endif

#endif

