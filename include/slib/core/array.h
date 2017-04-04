/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ARRAY
#define CHECKHEADER_SLIB_CORE_ARRAY

#include "definition.h"

#include "ref.h"
#include "iterator.h"
#include "new_helper.h"
#include "compare.h"

namespace std
{
	template <class T>
	class initializer_list;
}

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT ArrayData
	{
	public:
		T* data;
		sl_size count;
		Ref<Referable> refer;

	public:
		T& operator[](sl_reg index) const;

	};
	
	template <class T>
	class Array;
	
	template <class T>
	using AtomicArray = Atomic< Array<T> >;
	
	
	class SLIB_EXPORT CArrayBase : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		CArrayBase();

		~CArrayBase();

	};
	
	template <class T>
	class SLIB_EXPORT CArray : public CArrayBase
	{
	protected:
		T* m_data;
		sl_size m_count;
		sl_bool m_flagStatic;
		Ref<Referable> m_refer;

	protected:
		CArray();

	public:
		CArray(sl_size count);

		template <class _T>
		CArray(const _T* data, sl_size count);
		
		CArray(const std::initializer_list<T>& l);

		CArray(const T* data, sl_size count, Referable* refer);

		~CArray();

	public:
		static CArray<T>* create(sl_size count);

		template <class _T>
		static CArray<T>* create(const _T* data, sl_size count);
		
		static CArray<T>* create(const std::initializer_list<T>& l);

		static CArray<T>* createStatic(const T* data, sl_size count, Referable* refer);

	public:
		T* getData() const;

		sl_size getCount() const;

		sl_bool isStatic() const;

		const Ref<Referable>& getRefer() const;

	public:
		T* getPointerAt(sl_size index) const;

		sl_bool getAt(sl_size index, T* _out = sl_null) const;

		T getValueAt(sl_size index) const;

		T getValueAt(sl_size index, const T& def) const;

		sl_bool setAt(sl_size index, const T& value) const;

		T const& operator[](sl_size_t index) const;

		T& operator[](sl_size_t index);

	public:
		CArray<T>* sub(sl_size start, sl_size count = SLIB_SIZE_MAX);

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		template <class _T>
		sl_size read(sl_size startSource, sl_size len, _T* dataDst) const;

		template <class _T>
		sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const;

		template <class _T>
		sl_size copy(sl_size startTarget, const CArray<_T>* source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;

		template <class _T>
		sl_size copy(const CArray<_T>* source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;

		CArray<T>* duplicate() const;

		// range-based for loop
		T* begin();

		T const* begin() const;

		T* end();

		T const* end() const;
	
	};
	
	template <class T>
	class SLIB_EXPORT ArrayPosition
	{
	public:
		ArrayPosition();

		ArrayPosition(const Ref< CArray<T> >& array);

		ArrayPosition(const ArrayPosition<T>& other);

		ArrayPosition(ArrayPosition<T>&& other);

	public:
		T& operator*();

		sl_bool operator!=(const ArrayPosition<T>& other);

		ArrayPosition<T>& operator++();

	private:
		Ref< CArray<T> > ref;
		T* data;
		sl_size count;

	};
	
	template <class T>
	class SLIB_EXPORT Array
	{
	public:
		Ref< CArray<T> > ref;
		SLIB_REF_WRAPPER(Array, CArray<T>)
	
	public:
		Array(sl_size count);
		
		template <class _T>
		Array(const _T* data, sl_size count);
		
		Array(const std::initializer_list<T>& l);
		
		Array(const T* data, sl_size count, Referable* refer);
		
	public:
		static Array<T> create(sl_size count);

		template <class _T>
		static Array<T> create(const _T* data, sl_size count);

		static Array<T> create(const std::initializer_list<T>& l);
		
		static Array<T> createStatic(const T* data, sl_size count);

		static Array<T> createStatic(const T* data, sl_size count, Referable* refer);
		
		template <class _T>
		static const Array<T>& from(const Array<_T>& other);

	public:
		T* getData() const;

		sl_size getCount() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

	public:
		T* getPointerAt(sl_size index) const;

		sl_bool getAt(sl_size index, T* _out = sl_null) const;

		T getValueAt(sl_size index) const;

		T getValueAt(sl_size index, const T& def) const;

		sl_bool setAt(sl_size index, const T& value) const;

		T& operator[](sl_size_t index) const;
		
		Array<T>& operator=(const std::initializer_list<T>& l);

	public:
		Array<T> sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

		template <class _T>
		sl_size read(sl_size startSource, sl_size len, _T* dataDst) const;

		template <class _T>
		sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const;

		template <class _T>
		sl_size copy(sl_size startTarget, const Array<_T>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;

		template <class _T>
		sl_size copy(const Array<_T>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;

		Array<T> duplicate() const;

		Iterator<T> toIterator() const;

		sl_bool getData(ArrayData<T>& data) const;

		// range-based for loop
		T* begin() const;

		T* end() const;

	};
	
	
	template <class T>
	class SLIB_EXPORT Atomic< Array<T> >
	{
	public:
		AtomicRef< CArray<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CArray<T>)
		
	public:
		Atomic(sl_size count);
		
		template <class _T>
		Atomic(const _T* data, sl_size count);
		
		Atomic(const std::initializer_list<T>& l);
		
		Atomic(const T* data, sl_size count, Referable* refer);
		
	public:
		template <class _T>
		static const Atomic< Array<T> >& from(const Atomic< Array<_T> >& other);

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
		Array<T> sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg indexOf(const _T& value, sl_reg start = 0, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_reg lastIndexOf(const _T& value, sl_reg start = -1, const EQUALS& equals = EQUALS()) const;

		template < class _T, class EQUALS = Equals<T, _T> >
		sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;
	
		template <class _T>
		sl_size read(sl_size startSource, sl_size len, _T* dataDst) const;

		template <class _T>
		sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const;

		template <class _T>
		sl_size copy(sl_size startTarget, const Array<_T>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;

		template <class _T>
		sl_size copy(const Array<_T>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;

		Array<T> duplicate() const;

		sl_bool getData(ArrayData<T>& data) const;

		// range-based for loop
		ArrayPosition<T> begin() const;

		ArrayPosition<T> end() const;

	};
	
	
	template <class T>
	class SLIB_EXPORT ArrayIterator : public IIterator<T>
	{
	protected:
		ArrayData<T> m_arr;
		sl_size m_index;

	public:
		ArrayIterator(const Array<T>& arr);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(T* _out);

		// override
		sl_reg getIndex();

	};

}

#include "detail/array.h"

#endif
