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
		LinkPosition() noexcept;

		LinkPosition(Link<T>* link) noexcept;
		
		LinkPosition(Link<T>* link, Referable* ref) noexcept;

		LinkPosition(const LinkPosition& other) = default;
		
		LinkPosition(LinkPosition&& other) = default;
		
	public:
		LinkPosition& operator=(const LinkPosition& other) = default;
		
		LinkPosition& operator=(LinkPosition&& other) = default;

		T& operator*() const noexcept;
		
		sl_bool operator==(const LinkPosition<T>& p) const noexcept;

		sl_bool operator!=(const LinkPosition<T>& p) const noexcept;
		
		operator Link<T>*() const noexcept;

		LinkPosition<T>& operator++() noexcept;

	public:
		Link<T>* link;
		Ref<Referable> ref;

	};
	
	class SLIB_EXPORT CLinkedListBase : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		CLinkedListBase() noexcept;

		~CLinkedListBase() noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT CLinkedList : public CLinkedListBase
	{
	protected:
		Link<T>* m_front;
		Link<T>* m_back;
		sl_size m_count;

	public:
		CLinkedList() noexcept;

		~CLinkedList() noexcept;

	public:
		// not-free existing links
		void initialize() noexcept;
	
		// not-free existing links
		void initialize(Link<T>* front, Link<T>* back, sl_size count) noexcept;

		// not-free existing links
		void initialize(const CLinkedList<T>* other) noexcept;

	public:
		Link<T>* getFront() const noexcept;

		Link<T>* getBack() const noexcept;
	
		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;
	
		sl_bool isNotEmpty() const noexcept;

	public:
		sl_bool getFrontValue_NoLock(T* _out) const noexcept;

		sl_bool getFrontValue(T* _out) const noexcept;

		sl_bool getBackValue_NoLock(T* _out) const noexcept;

		sl_bool getBackValue(T* _out) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		Link<T>* find_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool find(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool pushBack(const T& value, sl_size countLimit = 0) noexcept;
		
		template <class VALUE>
		sl_bool pushBackAll_NoLock(const CLinkedList<VALUE>* other) noexcept;

		template <class VALUE>
		sl_bool pushBackAll(const CLinkedList<VALUE>* other) noexcept;

		sl_bool popBack_NoLock(T* _out = sl_null) noexcept;

		sl_bool popBack(T* _out = sl_null) noexcept;

		Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool pushFront(const T& value, sl_size countLimit = 0) noexcept;
		
		template <class VALUE>
		sl_bool pushFrontAll_NoLock(const CLinkedList<VALUE>* other) noexcept;

		template <class VALUE>
		sl_bool pushFrontAll(const CLinkedList<VALUE>* other) noexcept;
	
		sl_bool popFront_NoLock(T* _out = sl_null) noexcept;

		sl_bool popFront(T* _out = sl_null) noexcept;

		/* unsynchronized function */
		Link<T>* insertBefore(Link<T>* itemWhere, const T& value) noexcept;

		/* unsynchronized function */
		Link<T>* insertAfter(Link<T>* itemWhere, const T& value) noexcept;

		/* unsynchronized function */
		void removeAt(Link<T>* item) noexcept;

		sl_size removeAll_NoLock() noexcept;

		sl_size removeAll() noexcept;
	
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;
		
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;
		
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;
		
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;
		
		void merge_NoLock(CLinkedList<T>* other) noexcept;

		void merge(CLinkedList<T>* other) noexcept;

		Array<T> toArray_NoLock() const noexcept;

		Array<T> toArray() const noexcept;

		List<T> toList_NoLock() const noexcept;

		List<T> toList() const noexcept;

		CLinkedList<T>* duplicate_NoLock() const noexcept;

		CLinkedList<T>* duplicate() const noexcept;

		LinkPosition<T> begin() const noexcept;

		LinkPosition<T> end() const noexcept;
	
		static void freeLink(Link<T>* link) noexcept;

	protected:
		static Link<T>* _createItem(const T& value) noexcept;

		static void _freeItem(Link<T>* item) noexcept;

		Link<T>* _pushBackItem(Link<T>* item, sl_size countLimit) noexcept;

		Link<T>* _popBackItem() noexcept;

		Link<T>* _pushFrontItem(Link<T>* item, sl_size countLimit) noexcept;

		Link<T>* _popFrontItem() noexcept;

		void _removeItem(Link<T>* item) noexcept;

		void _insertBefore(Link<T>* itemWhere, Link<T>* itemNew) noexcept;

		void _insertAfter(Link<T>* itemWhere, Link<T>* itemNew) noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT LinkedList
	{
	public:
		Ref< CLinkedList<T> > ref;
		SLIB_REF_WRAPPER(LinkedList, CLinkedList<T>)

	public:
		static LinkedList<T> create() noexcept;
	
	public:
		Link<T>* getFront() const noexcept;

		Link<T>* getBack() const noexcept;

		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

	public:
		sl_bool getFrontValue_NoLock(T* _out) const noexcept;

		sl_bool getFrontValue(T* _out) const noexcept;

		sl_bool getBackValue_NoLock(T* _out) const noexcept;

		sl_bool getBackValue(T* _out) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		Link<T>* find_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool find(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool pushBack(const T& value, sl_size countLimit = 0) noexcept;

		template <class VALUE>
		sl_bool pushBackAll_NoLock(const LinkedList<VALUE>& other) noexcept;
		
		template <class VALUE>
		sl_bool pushBackAll(const LinkedList<VALUE>& other) noexcept;

		sl_bool popBack_NoLock(T* _out = sl_null) const noexcept;

		sl_bool popBack(T* _out = sl_null) const noexcept;

		Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool pushFront(const T& value, sl_size countLimit = 0) noexcept;
		
		template <class VALUE>
		sl_bool pushFrontAll_NoLock(const LinkedList<VALUE>& other) noexcept;

		template <class VALUE>
		sl_bool pushFrontAll(const LinkedList<VALUE>& other) noexcept;

		sl_bool popFront_NoLock(T* _out = sl_null) const noexcept;

		sl_bool popFront(T* _out = sl_null) const noexcept;

		/* unsynchronized function */
		Link<T>* insertBefore(Link<T>* itemWhere, const T& value) const noexcept;
		
		/* unsynchronized function */
		Link<T>* insertAfter(Link<T>* itemWhere, const T& value) const noexcept;
		
		/* unsynchronized function */
		void removeAt(Link<T>* item) const noexcept;

		sl_size removeAll_NoLock() const noexcept;

		sl_size removeAll() const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;
		
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;
		
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;
		
		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;
		
		void merge_NoLock(LinkedList<T>& other) noexcept;

		void merge(LinkedList<T>& other) noexcept;

		Array<T> toArray_NoLock() const noexcept;

		Array<T> toArray() const noexcept;

		List<T> toList_NoLock() const noexcept;

		List<T> toList() const noexcept;

		LinkedList<T> duplicate_NoLock() const noexcept;

		LinkedList<T> duplicate() const noexcept;

		LinkPosition<T> begin() const noexcept;

		LinkPosition<T> end() const noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< LinkedList<T> >
	{
	public:
		AtomicRef< CLinkedList<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CLinkedList<T>)

	public:
		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

	public:
		sl_bool getFrontValue(T* _out) const noexcept;

		sl_bool getBackValue(T* _out) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool find(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		sl_bool pushBack(const T& value, sl_size countLimit = 0) noexcept;

		template <class VALUE>
		sl_bool pushBackAll(const LinkedList<VALUE>& other) noexcept;

		sl_bool popBack(T* _out = sl_null) const noexcept;

		sl_bool pushFront(const T& value, sl_size countLimit = 0) noexcept;

		template <class VALUE>
		sl_bool pushFrontAll(const LinkedList<VALUE>& other) noexcept;

		sl_bool popFront(T* _out = sl_null) const noexcept;

		sl_size removeAll() const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		void merge(LinkedList<T>& other) noexcept;

		Array<T> toArray() const noexcept;

		List<T> toList() const noexcept;

		LinkedList<T> duplicate() const noexcept;

		LinkPosition<T> begin() const noexcept;

		LinkPosition<T> end() const noexcept;

	};
	
	template <class T>
	using AtomicLinkedList = Atomic< LinkedList<T> >;

}

#include "detail/linked_list.inc"

#endif
