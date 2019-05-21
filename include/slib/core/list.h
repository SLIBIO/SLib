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

#ifndef CHECKHEADER_SLIB_CORE_LIST
#define CHECKHEADER_SLIB_CORE_LIST

#include "definition.h"

#include "object.h"
#include "array.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <initializer_list>
#endif

namespace slib
{
	
	template <class T>
	class List;
	
	template <class T>
	using AtomicList = Atomic< List<T> >;
	
	namespace priv
	{
		namespace list
		{
			extern const char g_classID[];
		}
	}
	
	template <class T>
	class SLIB_EXPORT CList : public Object
	{
		SLIB_TEMPLATE_OBJECT(Object, priv::list::g_classID)

	protected:
		T* m_data;
		sl_size m_count;
		sl_size m_capacity;

	public:
		CList() noexcept;

		CList(sl_size count) noexcept;

		CList(sl_size count, sl_size capacity) noexcept;
		
		CList(sl_size count, sl_size capacity, const T& initialValue) noexcept;

		template <class VALUE>
		CList(const VALUE* values, sl_size count) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		CList(const std::initializer_list<T>& l) noexcept;
#endif

		~CList() noexcept;
		
	public:
		CList(const CList& other) = delete;
		
		CList(CList&& other) noexcept;
		
		CList& operator=(const CList& other) = delete;
		
		CList& operator=(CList&& other) noexcept;

	public:
		static CList<T>* create() noexcept;

		static CList<T>* create(sl_size count) noexcept;

		static CList<T>* create(sl_size count, sl_size capacity) noexcept;

		static CList<T>* create(sl_size count, sl_size capacity, const T& initialValue) noexcept;
		
		template <class VALUE>
		static CList<T>* create(const VALUE* values, sl_size count) noexcept;
		
		template <class VALUE>
		static CList<T>* create(const Array<VALUE>& array) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		static CList<T>* create(const std::initializer_list<T>& l) noexcept;
#endif
		
		static CList<T>* createFromElement(const T& value, sl_size count = 1) noexcept;
		
		template <class... ARGS>
		static CList<T>* createFromElements(ARGS&&... values) noexcept;

		template <class VALUE>
		static CList<T>* createCopy(CList<VALUE>* other) noexcept;

	public:
		sl_size getCount() const noexcept;

		sl_size getCapacity() const noexcept;

		T* getData() const noexcept;

	public:
		/* unsynchronized function */
		T* getPointerAt(sl_size index) const noexcept;

		sl_bool getAt_NoLock(sl_size index, T* _out = sl_null) const noexcept;

		sl_bool getAt(sl_size index, T* _out = sl_null) const noexcept;

		T getValueAt_NoLock(sl_size index) const noexcept;

		T getValueAt(sl_size index) const noexcept;

		T getValueAt_NoLock(sl_size index, const T& def) const noexcept;

		T getValueAt(sl_size index, const T& def) const noexcept;

		template <class VALUE>
		sl_bool setAt_NoLock(sl_size index, VALUE&& value) const noexcept;

		template <class VALUE>
		sl_bool setAt(sl_size index, VALUE&& value) const noexcept;

		/* unsynchronized function */
		T const& operator[](sl_size_t index) const noexcept;

		/* unsynchronized function */
		T& operator[](sl_size_t index) noexcept;

	public:
		sl_bool setCount_NoLock(sl_size count) noexcept;

		sl_bool setCount(sl_size count) noexcept;

		sl_bool setCapacity_NoLock(sl_size capacity) noexcept;
		
		sl_bool setCapacity(sl_size capacity) noexcept;
		
		sl_bool adjustCapacity_NoLock(sl_size newCount) noexcept;
		
		sl_bool adjustCapacity(sl_size newCount) noexcept;
		
		sl_bool shrinkToFit_NoLock() noexcept;
		
		sl_bool shrinkToFit() noexcept;

		template <class... ARGS>
		sl_bool insert_NoLock(sl_size index, ARGS&&... args) noexcept;
		
		template <class... ARGS>
		sl_bool insert(sl_size index, ARGS&&... args) noexcept;

		template <class VALUE>
		sl_bool insertElements_NoLock(sl_size index, const VALUE* values, sl_size count) noexcept;

		template <class VALUE>
		sl_bool insertElements(sl_size index, const VALUE* values, sl_size count) noexcept;
		
		sl_bool insertElements_NoLock(sl_size index, sl_size count, const T& value) noexcept;
		
		sl_bool insertElements(sl_size index, sl_size count, const T& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool insertElements_NoLock(sl_size index, const std::initializer_list<T>& l) noexcept;
		
		sl_bool insertElements(sl_size index, const std::initializer_list<T>& l) noexcept;
#endif
		
		template <class VALUE>
		sl_bool insertAll_NoLock(sl_size index, const CList<VALUE>* other) noexcept;
		
		template <class VALUE>
		sl_bool insertAll(sl_size index, const CList<VALUE>* other) noexcept;

		template <class... ARGS>
		sl_bool add_NoLock(ARGS&&... args) noexcept;

		template <class... ARGS>
		sl_bool add(ARGS&&... args) noexcept;
	
		template <class VALUE>
		sl_bool addElements_NoLock(const VALUE* values, sl_size count) noexcept;

		template <class VALUE>
		sl_bool addElements(const VALUE* values, sl_size count) noexcept;
		
		sl_bool addElements_NoLock(sl_size count, const T& value) noexcept;
		
		sl_bool addElements(sl_size count, const T& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool addElements_NoLock(const std::initializer_list<T>& l) noexcept;
		
		sl_bool addElements(const std::initializer_list<T>& l) noexcept;
#endif
		
		template <class VALUE>
		sl_bool addAll_NoLock(const CList<VALUE>* other) noexcept;
	
		template <class VALUE>
		sl_bool addAll(const CList<VALUE>* other) noexcept;

		template < class VALUE, class EQUALS = Equals<T, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNotExist_NoLock(VALUE&& value, const EQUALS& equals = EQUALS()) noexcept;

		template < class VALUE, class EQUALS = Equals<T, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNotExist(VALUE&& value, const EQUALS& equals = EQUALS()) noexcept;

		sl_bool removeAt_NoLock(sl_size index, T* outValue = sl_null) noexcept;

		sl_bool removeAt(sl_size index, T* outValue = sl_null) noexcept;
	
		sl_size removeRange_NoLock(sl_size index, sl_size count) noexcept;

		sl_size removeRange(sl_size index, sl_size count) noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues(const VALUE& value, const EQUALS& equals = EQUALS()) noexcept;
		
		template <class PREDICATE>
		sl_bool removeIf_NoLock(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_bool removeIf(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_size removeElementsIf_NoLock(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_size removeElementsIf(const PREDICATE& p) noexcept;

		sl_size removeAll_NoLock() noexcept;

		sl_size removeAll() noexcept;

		sl_bool popFront_NoLock(T* _out = sl_null) noexcept;

		sl_bool popFront(T* _out = sl_null) noexcept;

		sl_size popFrontElements_NoLock(sl_size count) noexcept;

		sl_size popFrontElements(sl_size count) noexcept;

		sl_bool popBack_NoLock(T* _out = sl_null) noexcept;

		sl_bool popBack(T* _out = sl_null) noexcept;

		sl_size popBackElements_NoLock(sl_size count) noexcept;

		sl_size popBackElements(sl_size count) noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg indexOf_NoLock(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg indexOf(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg indexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg indexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg lastIndexOf_NoLock(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg lastIndexOf(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg lastIndexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg lastIndexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool contains_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool contains(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		CList<T>* duplicate_NoLock() const noexcept;

		CList<T>* duplicate() const noexcept;

		Array<T> toArray_NoLock() const noexcept;

		Array<T> toArray() const noexcept;

		template < class COMPARE = Compare<T> >
		void sort_NoLock(const COMPARE& compare = COMPARE()) const noexcept;

		template < class COMPARE = Compare<T> >
		void sort(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc_NoLock(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc(const COMPARE& compare = COMPARE()) const noexcept;
		
		void reverse_NoLock() const noexcept;
		
		void reverse() const noexcept;

		CList<T>* slice_NoLock(sl_size index, sl_size count = SLIB_SIZE_MAX) const noexcept;
		
		CList<T>* slice(sl_size index, sl_size count = SLIB_SIZE_MAX) const noexcept;

		// range-based for loop
		T* begin() noexcept;

		T const* begin() const noexcept;

		T* end() noexcept;

		T const* end() const noexcept;
		
	};
	
	
	template <class T>
	class SLIB_EXPORT List
	{
	public:
		Ref< CList<T> > ref;
		SLIB_REF_WRAPPER(List, CList<T>)

	public:
		List(sl_size count) noexcept;
		
		List(sl_size count, sl_size capacity) noexcept;
		
		List(sl_size count, sl_size capacity, const T& initialValue) noexcept;
		
		template <class VALUE>
		List(const VALUE* values, sl_size count) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		List(const std::initializer_list<T>& l) noexcept;
#endif
		
	public:
		static List<T> create() noexcept;

		static List<T> create(sl_size count) noexcept;

		static List<T> create(sl_size count, sl_size capacity) noexcept;
		
		static List<T> create(sl_size count, sl_size capacity, const T& initialValue) noexcept;
		
		template <class VALUE>
		static List<T> create(const VALUE* values, sl_size count) noexcept;

		template <class VALUE>
		static List<T> create(const Array<VALUE>& array) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		static List<T> create(const std::initializer_list<T>& l) noexcept;
#endif

		static List<T> createFromElement(const T& e, sl_size count = 1) noexcept;
		
		template <class... ARGS>
		static List<T> createFromElements(ARGS&&... args) noexcept;
		
		template <class VALUE>
		static List<T> createCopy(const List<VALUE>& other) noexcept;
		
		template <class VALUE>
		static List<T>& from(const List<VALUE>& other) noexcept;
		
	public:
		sl_size getCount() const noexcept;

		sl_size getCapacity() const noexcept;

		T* getData() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;
		
	public:
		/* unsynchronized function */
		T* getPointerAt(sl_size index) const noexcept;

		sl_bool getAt_NoLock(sl_size index, T* _out = sl_null) const noexcept;

		sl_bool getAt(sl_size index, T* _out = sl_null) const noexcept;

		T getValueAt_NoLock(sl_size index) const noexcept;

		T getValueAt(sl_size index) const noexcept;
	
		T getValueAt_NoLock(sl_size index, const T& def) const noexcept;

		T getValueAt(sl_size index, const T& def) const noexcept;

		template <class VALUE>
		sl_bool setAt_NoLock(sl_size index, VALUE&& value) const noexcept;

		template <class VALUE>
		sl_bool setAt(sl_size index, VALUE&& value) const noexcept;

		/* unsynchronized function */
		T& operator[](sl_size_t index) const noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		List<T>& operator=(const std::initializer_list<T>& l) noexcept;
#endif

	public:
		sl_bool setCount_NoLock(sl_size count) noexcept;

		sl_bool setCount(sl_size count) noexcept;
		
		sl_bool setCapacity_NoLock(sl_size capacity) noexcept;
		
		sl_bool setCapacity(sl_size capacity) noexcept;
				
		sl_bool shrinkToFit_NoLock() const noexcept;
		
		sl_bool shrinkToFit() const noexcept;

		template <class... ARGS>
		sl_bool insert_NoLock(sl_size index, ARGS&&... args) noexcept;

		template <class... ARGS>
		sl_bool insert(sl_size index, ARGS&&... args) noexcept;
	
		template <class VALUE>
		sl_bool insertElements_NoLock(sl_size index, const VALUE* values, sl_size count) noexcept;

		template <class VALUE>
		sl_bool insertElements(sl_size index, const VALUE* values, sl_size count) noexcept;
		
		sl_bool insertElements_NoLock(sl_size index, sl_size count, const T& value) noexcept;
		
		sl_bool insertElements(sl_size index, sl_size count, const T& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool insertElements_NoLock(sl_size index, const std::initializer_list<T>& l) noexcept;
		
		sl_bool insertElements(sl_size index, const std::initializer_list<T>& l) noexcept;
#endif
		
		template <class VALUE>
		sl_bool insertAll_NoLock(sl_size index, const List<VALUE>& other) noexcept;
		
		template <class VALUE>
		sl_bool insertAll_NoLock(sl_size index, const AtomicList<VALUE>& other) noexcept;

		template <class VALUE>
		sl_bool insertAll(sl_size index, const List<VALUE>& other) noexcept;

		template <class VALUE>
		sl_bool insertAll(sl_size index, const AtomicList<VALUE>& other) noexcept;

		template <class... ARGS>
		sl_bool add_NoLock(ARGS&&... args) noexcept;

		template <class... ARGS>
		sl_bool add(ARGS&&... args) noexcept;
	
		template <class VALUE>
		sl_bool addElements_NoLock(const VALUE* values, sl_size count) noexcept;

		template <class VALUE>
		sl_bool addElements(const VALUE* values, sl_size count) noexcept;
		
		sl_bool addElements_NoLock(sl_size count, const T& value) noexcept;
		
		sl_bool addElements(sl_size count, const T& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool addElements_NoLock(const std::initializer_list<T>& l) noexcept;
		
		sl_bool addElements(const std::initializer_list<T>& l) noexcept;
#endif

		template <class VALUE>
		sl_bool addAll_NoLock(const List<VALUE>& _other) noexcept;

		template <class VALUE>
		sl_bool addAll_NoLock(const AtomicList<VALUE>& _other) noexcept;

		template <class VALUE>
		sl_bool addAll(const List<VALUE>& _other) noexcept;

		template <class VALUE>
		sl_bool addAll(const AtomicList<VALUE>& _other) noexcept;

		template < class VALUE, class EQUALS = Equals<T, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNotExist_NoLock(VALUE&& value, const EQUALS& equals = EQUALS()) noexcept;

		template < class VALUE, class EQUALS = Equals<T, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNotExist(VALUE&& value, const EQUALS& equals = EQUALS()) noexcept;

		sl_bool removeAt_NoLock(sl_size index, T* outValue = sl_null) const noexcept;

		sl_bool removeAt(sl_size index, T* outValue = sl_null) const noexcept;

		sl_size removeRange_NoLock(sl_size index, sl_size count) const noexcept;

		sl_size removeRange(sl_size index, sl_size count) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template <class PREDICATE>
		sl_bool removeIf_NoLock(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_bool removeIf(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_size removeElementsIf_NoLock(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_size removeElementsIf(const PREDICATE& p) noexcept;

		sl_size removeAll_NoLock() const noexcept;

		sl_size removeAll() const noexcept;

		sl_bool popFront_NoLock(T* _out = sl_null) const noexcept;

		sl_bool popFront(T* _out = sl_null) const noexcept;

		sl_size popFrontElements_NoLock(sl_size count) const noexcept;

		sl_size popFrontElements(sl_size count) const noexcept;

		sl_bool popBack_NoLock(T* _out = sl_null) const noexcept;

		sl_bool popBack(T* _out = sl_null) const noexcept;

		sl_size popBackElements_NoLock(sl_size count) const noexcept;

		sl_size popBackElements(sl_size count) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg indexOf_NoLock(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg indexOf(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg indexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg indexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg lastIndexOf_NoLock(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg lastIndexOf(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg lastIndexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg lastIndexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool contains_NoLock(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool contains(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		List<T> duplicate_NoLock() const noexcept;

		List<T> duplicate() const noexcept;

		Array<T> toArray_NoLock() const noexcept;

		Array<T> toArray() const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sort_NoLock(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sort(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc_NoLock(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc(const COMPARE& compare = COMPARE()) const noexcept;
		
		void reverse_NoLock() const noexcept;
		
		void reverse() const noexcept;
		
		List<T> slice_NoLock(sl_size index, sl_size count = SLIB_SIZE_MAX) const noexcept;
		
		List<T> slice(sl_size index, sl_size count = SLIB_SIZE_MAX) const noexcept;

		const Mutex* getLocker() const noexcept;

		// range-based for loop
		T* begin() const noexcept;

		T* end() const noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< List<T> >
	{
	public:
		AtomicRef< CList<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CList<T>)
		
	public:
		Atomic(sl_size count) noexcept;
		
		Atomic(sl_size count, sl_size capacity) noexcept;
		
		Atomic(sl_size count, sl_size capacity, const T& initialValue) noexcept;
		
		template <class VALUE>
		Atomic(const VALUE* values, sl_size count) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::initializer_list<T>& l) noexcept;
#endif

	public:
		template <class VALUE>
		static Atomic< List<T> >& from(const Atomic< List<VALUE> >& other) noexcept;

		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

	public:
		sl_bool getAt(sl_size index, T* _out = sl_null) const noexcept;

		T getValueAt(sl_size index) const noexcept;

		T getValueAt(sl_size index, const T& def) const noexcept;

		template <class VALUE>
		sl_bool setAt(sl_size index, VALUE&& value) const noexcept;

		T operator[](sl_size_t index) const noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic& operator=(const std::initializer_list<T>& l) noexcept;
#endif

	public:
		sl_bool setCount(sl_size count) noexcept;
		
		sl_bool setCapacity(sl_size capacity) noexcept;
		
		sl_bool shrinkToFit() const noexcept;

		template <class... ARGS>
		sl_bool insert(sl_size index, ARGS&&... args) noexcept;

		template <class VALUE>
		sl_bool insertElements(sl_size index, const VALUE* values, sl_size count) noexcept;
		
		sl_bool insertElements(sl_size index, sl_size count, const T& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool insertElements(sl_size index, const std::initializer_list<T>& l) noexcept;
#endif
		
		template <class VALUE>
		sl_bool insertAll(sl_size index, const List<VALUE>& other) noexcept;

		template <class VALUE>
		sl_bool insertAll(sl_size index, const AtomicList<VALUE>& other) noexcept;

		template <class... ARGS>
		sl_bool add(ARGS&&... args) noexcept;

		template <class VALUE>
		sl_bool addElements(const VALUE* values, sl_size count) noexcept;
		
		sl_bool addElements(sl_size count, const T& value) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		sl_bool addElements(const std::initializer_list<T>& l) noexcept;
#endif

		template <class VALUE>
		sl_bool addAll(const List<VALUE>& _other) noexcept;

		template <class VALUE>
		sl_bool addAll(const AtomicList<VALUE>& _other) noexcept;

		template < class VALUE, class EQUALS = Equals<T, typename RemoveConstReference<VALUE>::Type> >
		sl_bool addIfNotExist(VALUE&& value, const EQUALS& equals = EQUALS()) noexcept;

		sl_bool removeAt(sl_size index, T* outValue = sl_null) const noexcept;

		sl_size removeRange(sl_size index, sl_size count) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool remove(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_size removeValues(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		template <class PREDICATE>
		sl_bool removeIf(const PREDICATE& p) noexcept;
		
		template <class PREDICATE>
		sl_size removeElementsIf(const PREDICATE& p) noexcept;
		
		sl_size removeAll() const noexcept;

		sl_bool popFront(T* _out = sl_null) const noexcept;

		sl_size popFrontElements(sl_size count) const noexcept;

		sl_bool popBack(T* _out = sl_null) const noexcept;

		sl_size popBackElements(sl_size count) const noexcept;
		
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg indexOf(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg indexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;
		
		template < class VALUE, class ARG = Equals<T, VALUE> >
		sl_reg lastIndexOf(const VALUE& value, const ARG& arg = ARG()) const noexcept;
		
		template <class VALUE, class EQUALS>
		sl_reg lastIndexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept;

		template < class VALUE, class EQUALS = Equals<T, VALUE> >
		sl_bool contains(const VALUE& value, const EQUALS& equals = EQUALS()) const noexcept;

		List<T> duplicate() const noexcept;

		Array<T> toArray() const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sort(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc(const COMPARE& compare = COMPARE()) const noexcept;
		
		void reverse() const noexcept;
		
		List<T> slice(sl_size index, sl_size count = SLIB_SIZE_MAX) const noexcept;

		// range-based for loop
		ArrayPosition<T> begin() const noexcept;

		ArrayPosition<T> end() const noexcept;

	};
	
	
	template <class T>
	class SLIB_EXPORT ListLocker : public ObjectLocker
	{
	public:
		T* data;
		sl_size count;
		List<T> list;

	public:
		ListLocker(const List<T>& list) noexcept;

		ListLocker(const CList<T>& list) noexcept;

		~ListLocker() noexcept;

	public:
		T& operator[](sl_reg index) noexcept;

		// range-based for loop
		T* begin() noexcept;

		T* end() noexcept;

	};
	
	
	template <class T>
	class SLIB_EXPORT ListElements
	{
	public:
		T* data;
		sl_size count;
		List<T> list;

	public:
		ListElements(const List<T>& list) noexcept;

		ListElements(const CList<T>& list) noexcept;

	public:
		T& operator[](sl_reg index) noexcept;

		// range-based for loop
		T* begin() noexcept;

		T* end() noexcept;

	};
	
}

#include "detail/list.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/list_std.inc"
#endif

#endif
