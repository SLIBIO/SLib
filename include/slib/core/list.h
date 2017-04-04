/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_LIST
#define CHECKHEADER_SLIB_CORE_LIST

#include "definition.h"

#include "new_helper.h"
#include "object.h"
#include "iterator.h"
#include "array.h"
#include "compare.h"
#include "sort.h"

namespace std
{
	template <class T>
	class initializer_list;
}

namespace slib
{
	
	template <class T>
	class List;
	
	template <class T>
	using AtomicList = Atomic< List<T> >;
	
	extern const char _List_ClassID[];
	
	template <class T>
	class SLIB_EXPORT CList : public Object
	{
		SLIB_TEMPLATE_OBJECT(Object, _List_ClassID)

	protected:
		T* m_data;
		sl_size m_count;
		sl_size m_capacity;

	public:
		CList();

		CList(sl_size count);

		CList(sl_size count, sl_size capacity);

		template <class _T>
		CList(const _T* values, sl_size count);
		
		CList(const std::initializer_list<T>& l);

		~CList();

	public:
		static CList<T>* create();

		static CList<T>* create(sl_size count);

		static CList<T>* create(sl_size count, sl_size capacity);
		
		static CList<T>* create(const std::initializer_list<T>& l);

		template <class _T>
		static CList<T>* createFromArray(const _T* values, sl_size count);

		template <class _T>
		static CList<T>* createFromArray(const Array<_T>& array);

		static CList<T>* createFromElement(const T& value);
		
		template <class... ARGS>
		static CList<T>* createFromElements(ARGS&&... values);

		template <class _T>
		static CList<T>* createCopy(CList<_T>* other);

	public:
		sl_size getCount() const;

		sl_size getCapacity() const;

		T* getData() const;

	public:
		T* getPointerAt(sl_size index) const;

		sl_bool getAt_NoLock(sl_size index, T* _out = sl_null) const;

		sl_bool getAt(sl_size index, T* _out = sl_null) const;

		T getValueAt_NoLock(sl_size index) const;

		T getValueAt(sl_size index) const;

		T getValueAt_NoLock(sl_size index, const T& def) const;

		T getValueAt(sl_size index, const T& def) const;

		sl_bool setAt_NoLock(sl_size index, const T& value) const;

		sl_bool setAt(sl_size index, const T& value) const;

		T const& operator[](sl_size_t index) const;

		T& operator[](sl_size_t index);

	public:
		sl_bool setCount_NoLock(sl_size count);

		sl_bool setCount(sl_size count);

		sl_bool insert_NoLock(sl_size index, const T& value);

		sl_bool insert(sl_size index, const T& value);

		template <class _T>
		sl_bool insertElements_NoLock(sl_size index, const _T* values, sl_size count);

		template <class _T>
		sl_bool insertElements(sl_size index, const _T* values, sl_size count);
		
		sl_bool insertElements_NoLock(sl_size index, const std::initializer_list<T>& l);
		
		sl_bool insertElements(sl_size index, const std::initializer_list<T>& l);
		
		template <class _T>
		sl_bool insertAll(sl_size index, const CList<_T>* other);

		sl_bool add_NoLock(const T& value);

		sl_bool add(const T& value);
	
		template <class _T>
		sl_bool addElements_NoLock(const _T* values, sl_size count);

		template <class _T>
		sl_bool addElements(const _T* values, sl_size count);
		
		sl_bool addElements_NoLock(const std::initializer_list<T>& l);
		
		sl_bool addElements(const std::initializer_list<T>& l);
		
		template <class _T>
		sl_bool addAll_NoLock(const CList<_T>* other);
	
		template <class _T>
		sl_bool addAll(const CList<_T>* other);

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool addIfNotExist_NoLock(const _T& value, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool addIfNotExist(const _T& value, const EQUALS& equals = EQUALS());

		template <class _T>
		sl_bool addAll_NoLock(const Iterator<_T>& iterator);

		template <class _T>
		sl_bool addAll(const Iterator<_T>& iterator);

		sl_bool removeAt_NoLock(sl_size index, T* outValue = sl_null);

		sl_bool removeAt(sl_size index, T* outValue = sl_null);
	
		sl_size removeRange_NoLock(sl_size index, sl_size count);

		sl_size removeRange(sl_size index, sl_size count);

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue_NoLock(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeElementsByValue_NoLock(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeElementsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS());

		sl_size removeAll_NoLock();

		sl_size removeAll();

		sl_bool popFront_NoLock(T* _out = sl_null);

		sl_bool popFront(T* _out = sl_null);

		sl_size popFrontElements_NoLock(sl_size count);

		sl_size popFrontElements(sl_size count);

		sl_bool popBack_NoLock(T* _out = sl_null);

		sl_bool popBack(T* _out = sl_null);

		sl_size popBackElements_NoLock(sl_size count);

		sl_size popBackElements(sl_size count);

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf_NoLock(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf_NoLock(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		CList<T>* duplicate_NoLock() const;

		CList<T>* duplicate() const;

		Array<T> toArray_NoLock() const;

		Array<T> toArray() const;

		template < class COMPARE = Compare<T> >
		void sort_NoLock(sl_bool flagAscending = sl_true, const COMPARE& compare = COMPARE()) const;

		template < class COMPARE = Compare<T> >
		void sort(sl_bool flagAscending = sl_true, const COMPARE& compare = COMPARE()) const;

		Iterator<T> toIterator() const;

		// range-based for loop
		T* begin();

		T const* begin() const;

		T* end();

		T const* end() const;

	protected:
		sl_bool _setCountInner(sl_size count);

	};
	
	template <class T>
	class SLIB_EXPORT ListPosition
	{
	public:
		ListPosition();

		ListPosition(const Ref< CList<T> >& list);

		ListPosition(const ListPosition<T>& other);

		ListPosition(ListPosition<T>&& other);

	public:
		T& operator*();

		sl_bool operator!=(const ListPosition<T>& other);

		ListPosition<T>& operator++();

	private:
		Ref< CList<T> > ref;
		T* data;
		sl_size count;

	};
	
	template <class T>
	class SLIB_EXPORT List
	{
	public:
		Ref< CList<T> > ref;
		SLIB_REF_WRAPPER(List, CList<T>)

	public:
		List(sl_size count);
		
		List(sl_size count, sl_size capacity);
		
		template <class _T>
		List(const _T* values, sl_size count);

		List(const std::initializer_list<T>& l);
		
	public:
		static List<T> create();

		static List<T> create(sl_size count);

		static List<T> create(sl_size count, sl_size capacity);
		
		static List<T> create(const std::initializer_list<T>& l);

		template <class _T>
		static List<T> createFromArray(const _T* values, sl_size count);

		template <class _T>
		static List<T> createFromArray(const Array<_T>& array);

		static List<T> createFromElement(const T& e);
		
		template <class... ARGS>
		static List<T> createFromElements(ARGS&&... args);
		
		template <class _T>
		static List<T> createCopy(const List<_T>& other);
		
		template <class _T>
		static const List<T>& from(const List<_T>& other);

	public:
		sl_size getCount() const;

		sl_size getCapacity() const;

		T* getData() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;
		
	public:
		T* getPointerAt(sl_size index) const;

		sl_bool getAt_NoLock(sl_size index, T* _out = sl_null) const;

		sl_bool getAt(sl_size index, T* _out = sl_null) const;

		T getValueAt_NoLock(sl_size index) const;

		T getValueAt(sl_size index) const;
	
		T getValueAt_NoLock(sl_size index, const T& def) const;

		T getValueAt(sl_size index, const T& def) const;

		sl_bool setAt_NoLock(sl_size index, const T& value) const;

		sl_bool setAt(sl_size index, const T& value) const;

		T& operator[](sl_size_t index) const;

		List<T>& operator=(const std::initializer_list<T>& l);

	public:
		sl_bool setCount_NoLock(sl_size count);

		sl_bool setCount(sl_size count);

		sl_bool insert_NoLock(sl_size index, const T& value) const;

		sl_bool insert(sl_size index, const T& value) const;
	
		template <class _T>
		sl_bool insertElements_NoLock(sl_size index, const _T* values, sl_size count) const;

		template <class _T>
		sl_bool insertElements(sl_size index, const _T* values, sl_size count) const;
		
		sl_bool insertElements_NoLock(sl_size index, const std::initializer_list<T>& l) const;
		
		sl_bool insertElements(sl_size index, const std::initializer_list<T>& l) const;

		template <class _T>
		sl_bool insertAll(sl_size index, const List<_T>& other) const;

		template <class _T>
		sl_bool insertAll(sl_size index, const AtomicList<_T>& other) const;

		sl_bool add_NoLock(const T& value);

		sl_bool add(const T& value);
	
		template <class _T>
		sl_bool addElements_NoLock(const _T* values, sl_size count);

		template <class _T>
		sl_bool addElements(const _T* values, sl_size count);
		
		sl_bool addElements_NoLock(const std::initializer_list<T>& l);
		
		sl_bool addElements(const std::initializer_list<T>& l);

		template <class _T>
		sl_bool addAll_NoLock(const List<_T>& _other);

		template <class _T>
		sl_bool addAll_NoLock(const AtomicList<_T>& _other);

		template <class _T>
		sl_bool addAll(const List<_T>& _other);

		template <class _T>
		sl_bool addAll(const AtomicList<_T>& _other);

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool addIfNotExist_NoLock(const _T& value, const EQUALS& equals = EQUALS());

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool addIfNotExist(const _T& value, const EQUALS& equals = EQUALS());

		template <class _T>
		sl_bool addAll_NoLock(const Iterator<_T>& iterator);

		template <class _T>
		sl_bool addAll(const Iterator<_T>& iterator);

		sl_bool removeAt_NoLock(sl_size index, T* outValue = sl_null) const;

		sl_bool removeAt(sl_size index, T* outValue = sl_null) const;

		sl_size removeRange_NoLock(sl_size index, sl_size count) const;

		sl_size removeRange(sl_size index, sl_size count) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue_NoLock(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeElementsByValue_NoLock(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeElementsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;

		sl_size removeAll_NoLock() const;

		sl_size removeAll() const;

		sl_bool popFront_NoLock(T* _out = sl_null) const;

		sl_bool popFront(T* _out = sl_null) const;

		sl_size popFrontElements_NoLock(sl_size count) const;

		sl_size popFrontElements(sl_size count) const;

		sl_bool popBack_NoLock(T* _out = sl_null) const;

		sl_bool popBack(T* _out = sl_null) const;

		sl_size popBackElements_NoLock(sl_size count) const;

		sl_size popBackElements(sl_size count) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf_NoLock(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf_NoLock(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		List<T> duplicate_NoLock() const;

		List<T> duplicate() const;

		Array<T> toArray_NoLock() const;

		Array<T> toArray() const;

		template < class COMPARE = Compare<T> >
		void sort(sl_bool flagAscending = sl_true, const COMPARE& compare = COMPARE()) const;

		template < class COMPARE = Compare<T> >
		void sort_NoLock(sl_bool flagAscending = sl_true, const COMPARE& compare = COMPARE()) const;

		Iterator<T> toIterator() const;

		const Mutex* getLocker() const;

		// range-based for loop
		T* begin() const;

		T* end() const;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< List<T> >
	{
	public:
		AtomicRef< CList<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CList<T>)
		
	public:
		Atomic(sl_size count);
		
		Atomic(sl_size count, sl_size capacity);
		
		template <class _T>
		Atomic(const _T* values, sl_size count);

		Atomic(const std::initializer_list<T>& l);

	public:
		template <class _T>
		static const Atomic< List<T> >& from(const Atomic< List<_T> >& other);

		sl_size getCount() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

	public:
		sl_bool getAt(sl_size index, T* _out = sl_null) const;

		T getValueAt(sl_size index) const;

		T getValueAt(sl_size index, const T& def) const;

		sl_bool setAt(sl_size index, const T& value) const;

		T operator[](sl_size_t index) const;
		
		Atomic& operator=(const std::initializer_list<T>& l);

	public:
		sl_bool setCount(sl_size count);

		sl_bool insert(sl_size index, const T& value) const;

		template <class _T>
		sl_bool insertElements(sl_size index, const _T* values, sl_size count) const;

		sl_bool insertElements(sl_size index, const std::initializer_list<T>& l) const;
		
		template <class _T>
		sl_bool insertAll(sl_size index, const List<_T>& other) const;

		template <class _T>
		sl_bool insertAll(sl_size index, const AtomicList<_T>& other) const;

		sl_bool add(const T& value);

		template <class _T>
		sl_bool addElements(const _T* values, sl_size count);
		
		sl_bool addElements(const std::initializer_list<T>& l);

		template <class _T>
		sl_bool addAll(const List<_T>& _other);

		template <class _T>
		sl_bool addAll(const AtomicList<_T>& _other);

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool addIfNotExist(const _T& value, const EQUALS& equals = EQUALS());

		template <class _T>
		sl_bool addAll(const Iterator<_T>& iterator);

		sl_bool removeAt(sl_size index, T* outValue = sl_null) const;

		sl_size removeRange(sl_size index, sl_size count) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_size removeElementsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;

		sl_size removeAll() const;

		sl_bool popFront(T* _out = sl_null) const;

		sl_size popFrontElements(sl_size count) const;

		sl_bool popBack(T* _out = sl_null) const;

		sl_size popBackElements(sl_size count) const;
	
		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		List<T> duplicate() const;

		Array<T> toArray() const;

		template < class COMPARE = Compare<T> >
		void sort(sl_bool flagAscending = sl_true, const COMPARE& compare = COMPARE()) const;
	
		// range-based for loop
		ListPosition<T> begin() const;

		ListPosition<T> end() const;

	};
	
	
	template <class T>
	class SLIB_EXPORT ListLocker : public ObjectLocker
	{
	public:
		T* data;
		sl_size count;

	private:
		List<T> m_list;

	public:
		ListLocker(const List<T>& list);

		ListLocker(const CList<T>& list);

		~ListLocker();

	public:
		T& operator[](sl_reg index);

		// range-based for loop
		T* begin();

		T* end();

	};
	
	
	template <class T>
	class SLIB_EXPORT ListElements
	{
	public:
		T* data;
		sl_size count;

	private:
		List<T> m_list;

	public:
		ListElements(const List<T>& list);

		ListElements(const CList<T>& list);

	public:
		T& operator[](sl_reg index);

		// range-based for loop
		T* begin();

		T* end();

	};
	
	
	template <class T>
	class SLIB_EXPORT ListIterator : public IIterator<T>
	{
	protected:
		const CList<T>* m_list;
		Ref<Referable> m_refer;
		sl_size m_index;

	public:
		ListIterator(const CList<T>* list, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(T* _out);

		// override
		sl_reg getIndex();

	};
	
}

#include "detail/list.h"

#endif
