/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_LINKED_LIST
#define CHECKHEADER_SLIB_CORE_LINKED_LIST

#include "definition.h"

#include "object.h"
#include "list.h"
#include "array.h"

namespace slib
{
	
	template <class T>
	struct SLIB_EXPORT Link
	{
		Link<T>* before;
		Link<T>* next;
		T value;
	};
	
	template <class T>
	class SLIB_EXPORT LinkPosition
	{
	public:
		LinkPosition();

		LinkPosition(Link<T>* link);
		
		LinkPosition(Link<T>* link, Referable* ref);

		LinkPosition(const LinkPosition& other) = default;
		
		LinkPosition(LinkPosition&& other) = default;
		
	public:
		LinkPosition& operator=(const LinkPosition& other) = default;
		
		LinkPosition& operator=(LinkPosition&& other) = default;

	public:
		T& operator*() const;
		
		sl_bool operator==(const LinkPosition<T>& p) const;

		sl_bool operator!=(const LinkPosition<T>& p) const;

		LinkPosition<T>& operator++();

	private:
		Link<T>* link;
		Ref<Referable> ref;

	};
	
	class SLIB_EXPORT CLinkedListBase : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		CLinkedListBase();

		~CLinkedListBase();

	};
	
	template <class T>
	class SLIB_EXPORT CLinkedList : public CLinkedListBase
	{
	protected:
		Link<T>* m_front;
		Link<T>* m_back;
		sl_size m_count;

	public:
		CLinkedList();

		~CLinkedList();

	public:
		// not-free existing links
		void init();
	
		// not-free existing links
		void init(Link<T>* front, Link<T>* back, sl_size count);

		// not-free existing links
		void init(const CLinkedList<T>* other);

	public:
		Link<T>* getFront() const;
	
		Link<T>* getBack() const;
	
		sl_size getCount() const;

		sl_bool isEmpty() const;
	
		sl_bool isNotEmpty() const;

	public:
		sl_bool getFirstItem_NoLock(T* _out) const;

		sl_bool getFirstItem(T* _out) const;

		sl_bool getLastItem_NoLock(T* _out) const;

		sl_bool getLastItem(T* _out) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		Link<T>* search_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		Link<T>* search(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0);

		Link<T>* pushBack(const T& value, sl_size countLimit = 0);
		
		template <class _T>
		void pushBackAll_NoLock(const CLinkedList<_T>* other);

		template <class _T>
		void pushBackAll(const CLinkedList<_T>* other);

		sl_bool popBack_NoLock(T* _out = sl_null);

		sl_bool popBack(T* _out = sl_null);

		Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0);

		Link<T>* pushFront(const T& value, sl_size countLimit = 0);
		
		template <class _T>
		void pushFrontAll_NoLock(const CLinkedList<_T>* other);

		template <class _T>
		void pushFrontAll(const CLinkedList<_T>* other);
	
		sl_bool popFront_NoLock(T* _out = sl_null);

		sl_bool popFront(T* _out = sl_null);

		Link<T>* insertBefore_NoLock(Link<T>* itemWhere, const T& value);

		Link<T>* insertBefore(Link<T>* itemWhere, const T& value);

		Link<T>* insertAfter_NoLock(Link<T>* itemWhere, const T& value);

		Link<T>* insertAfter(Link<T>* itemWhere, const T& value);

		void removeItem_NoLock(Link<T>* item);

		void removeItem(Link<T>* item);
	
		void removeAll_NoLock();

		void removeAll();
	
		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue_NoLock(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeItemsByValue_NoLock(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeItemsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS());
		
		void merge_NoLock(CLinkedList<T>* other);

		void merge(CLinkedList<T>* other);

		Array<T> toArray_NoLock() const;

		Array<T> toArray() const;

		List<T> toList_NoLock() const;

		List<T> toList() const;

		CLinkedList<T>* duplicate_NoLock() const;

		CLinkedList<T>* duplicate() const;

		LinkPosition<T> begin() const;

		LinkPosition<T> end() const;
	
		static void freeLink(Link<T>* link);

	protected:
		static Link<T>* _createItem(const T& value);

		static void _freeItem(Link<T>* item);

		Link<T>* _pushBackItem(Link<T>* item, sl_size countLimit);

		Link<T>* _popBackItem();

		Link<T>* _pushFrontItem(Link<T>* item, sl_size countLimit);

		Link<T>* _popFrontItem();

		void _removeItem(Link<T>* item);

		void _insertBefore(Link<T>* itemWhere, Link<T>* itemNew);

		void _insertAfter(Link<T>* itemWhere, Link<T>* itemNew);

	};
	
	template <class T>
	class SLIB_EXPORT LinkedList
	{
	public:
		Ref< CLinkedList<T> > ref;
		SLIB_REF_WRAPPER(LinkedList, CLinkedList<T>)

	public:
		static LinkedList<T> create();
	
	public:
		Link<T>* getFront() const;

		Link<T>* getBack() const;

		sl_size getCount() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

	public:
		sl_bool getFirstItem_NoLock(T* _out) const;

		sl_bool getFirstItem(T* _out) const;

		sl_bool getLastItem_NoLock(T* _out) const;

		sl_bool getLastItem(T* _out) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		Link<T>* search_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		Link<T>* search(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;
	
		Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0);

		Link<T>* pushBack(const T& value, sl_size countLimit = 0);

		template <class _T>
		void pushBackAll_NoLock(const LinkedList<_T>& other);
		
		template <class _T>
		void pushBackAll(const LinkedList<_T>& other);

		sl_bool popBack_NoLock(T* _out = sl_null) const;

		sl_bool popBack(T* _out = sl_null) const;

		Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0);

		Link<T>* pushFront(const T& value, sl_size countLimit = 0);
		
		template <class _T>
		void pushFrontAll_NoLock(const LinkedList<_T>& other);

		template <class _T>
		void pushFrontAll(const LinkedList<_T>& other);

		sl_bool popFront_NoLock(T* _out = sl_null) const;

		sl_bool popFront(T* _out = sl_null) const;

		Link<T>* insertBefore_NoLock(Link<T>* itemWhere, const T& value) const;

		Link<T>* insertBefore(Link<T>* itemWhere, const T& value) const;

		Link<T>* insertAfter_NoLock(Link<T>* itemWhere, const T& value) const;

		Link<T>* insertAfter(Link<T>* itemWhere, const T& value) const;

		void removeItem_NoLock(Link<T>* item) const;

		void removeItem(Link<T>* item) const;

		void removeAll_NoLock() const;

		void removeAll() const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue_NoLock(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeItemsByValue_NoLock(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeItemsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;
		
		void merge_NoLock(LinkedList<T>& other);

		void merge(LinkedList<T>& other);

		Array<T> toArray_NoLock() const;

		Array<T> toArray() const;

		List<T> toList_NoLock() const;

		List<T> toList() const;

		LinkedList<T> duplicate_NoLock() const;

		LinkedList<T> duplicate() const;

		LinkPosition<T> begin() const;

		LinkPosition<T> end() const;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< LinkedList<T> >
	{
	public:
		AtomicRef< CLinkedList<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CLinkedList<T>)

	public:
		sl_size getCount() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

	public:
		sl_bool getFirstItem(T* _out) const;

		sl_bool getLastItem(T* _out) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		sl_bool pushBack(const T& value, sl_size countLimit = 0);

		template <class _T>
		void pushBackAll(const LinkedList<_T>& other);

		sl_bool popBack(T* _out = sl_null) const;

		sl_bool pushFront(const T& value, sl_size countLimit = 0);

		template <class _T>
		void pushFrontAll(const LinkedList<_T>& other);

		sl_bool popFront(T* _out = sl_null) const;

		void removeAll() const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeItemsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;

		void merge(LinkedList<T>& other);

		Array<T> toArray() const;

		List<T> toList() const;

		LinkedList<T> duplicate() const;

		LinkPosition<T> begin() const;

		LinkPosition<T> end() const;

	};
	
	template <class T>
	using AtomicLinkedList = Atomic< LinkedList<T> >;

}

#include "detail/linked_list.inc"

#endif
