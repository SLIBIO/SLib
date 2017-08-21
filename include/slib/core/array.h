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

#include "array_traits.h"
#include "ref.h"
#include "compare.h"
#include "null_value.h"
#include "sort.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <initializer_list>
#endif

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
		T& operator[](sl_reg index) const noexcept;

	};
	
	template <class T>
	class Array;
	
	template <class T>
	using AtomicArray = Atomic< Array<T> >;
	
	
	class SLIB_EXPORT CArrayBase : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		CArrayBase() noexcept;

		~CArrayBase() noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT CArray : public CArrayBase
	{
	protected:
		T* m_data;
		sl_size m_count;
		sl_bool m_flagStatic;
		Ref<Referable> m_refer;

	public:
		CArray() noexcept;

		CArray(sl_size count) noexcept;

		template <class VALUE>
		CArray(const VALUE* data, sl_size count) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		CArray(const std::initializer_list<T>& l) noexcept;
#endif

		CArray(const T* data, sl_size count, Referable* refer) noexcept;

		~CArray() noexcept;
		
	public:
		CArray(const CArray& other) = delete;
		
		CArray(CArray&& other) noexcept;
		
		CArray& operator=(const CArray& other) = delete;
		
		CArray& operator=(CArray&& other) noexcept;

	public:
		static CArray<T>* create(sl_size count) noexcept;

		template <class VALUE>
		static CArray<T>* create(const VALUE* data, sl_size count) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		static CArray<T>* create(const std::initializer_list<T>& l) noexcept;
#endif

		static CArray<T>* createStatic(const T* data, sl_size count, Referable* refer) noexcept;

	public:
		T* getData() const noexcept;

		sl_size getCount() const noexcept;

		sl_bool isStatic() const noexcept;

		const Ref<Referable>& getRefer() const noexcept;

	public:
		T* getPointerAt(sl_size index) const noexcept;

		sl_bool getAt(sl_size index, T* _out = sl_null) const noexcept;

		T getValueAt(sl_size index) const noexcept;

		T getValueAt(sl_size index, const T& def) const noexcept;

		sl_bool setAt(sl_size index, const T& value) const noexcept;

		T const& operator[](sl_size_t index) const noexcept;

		T& operator[](sl_size_t index) noexcept;

	public:
		CArray<T>* sub(sl_size start, sl_size count = SLIB_SIZE_MAX) noexcept;
		
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

		template <class VALUE>
		sl_size read(sl_size startSource, sl_size len, VALUE* dataDst) const noexcept;

		template <class VALUE>
		sl_size write(sl_size startTarget, sl_size len, const VALUE* dataSrc) const noexcept;

		template <class VALUE>
		sl_size copy(sl_size startTarget, const CArray<VALUE>* source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const noexcept;

		template <class VALUE>
		sl_size copy(const CArray<VALUE>* source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const noexcept;

		CArray<T>* duplicate() const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sort(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc(const COMPARE& compare = COMPARE()) const noexcept;
		
		void reverse() const noexcept;

		// range-based for loop
		T* begin() noexcept;

		T const* begin() const noexcept;

		T* end() noexcept;

		T const* end() const noexcept;
	
	};
	
	template <class T>
	class SLIB_EXPORT ArrayPosition
	{
	public:
		ArrayPosition() noexcept;
		
		ArrayPosition(T* begin, sl_size count, Referable* ref) noexcept;

		ArrayPosition(const ArrayPosition& other) noexcept = default;

		ArrayPosition(ArrayPosition&& other) noexcept = default;

	public:
		ArrayPosition& operator=(const ArrayPosition& other) noexcept = default;
		
		ArrayPosition& operator=(ArrayPosition&& other) noexcept = default;
		
		T& operator*() const noexcept;
		
		sl_bool operator==(const ArrayPosition& other) const noexcept;

		sl_bool operator!=(const ArrayPosition& other) const noexcept;
		
		explicit operator sl_bool() const noexcept;

		ArrayPosition& operator++() noexcept;

	private:
		T* pos;
		sl_size count;
		Ref<Referable> ref;

	};
	
	template <class T>
	class SLIB_EXPORT Array
	{
	public:
		Ref< CArray<T> > ref;
		SLIB_REF_WRAPPER(Array, CArray<T>)
	
	public:
		Array(sl_size count) noexcept;
		
		template <class VALUE>
		Array(const VALUE* data, sl_size count) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Array(const std::initializer_list<T>& l) noexcept;
#endif
		
		Array(const T* data, sl_size count, Referable* refer) noexcept;
		
	public:
		static Array<T> create(sl_size count) noexcept;

		template <class VALUE>
		static Array<T> create(const VALUE* data, sl_size count) noexcept;

#ifdef SLIB_SUPPORT_STD_TYPES
		static Array<T> create(const std::initializer_list<T>& l) noexcept;
#endif
		
		static Array<T> createStatic(const T* data, sl_size count) noexcept;

		static Array<T> createStatic(const T* data, sl_size count, Referable* refer) noexcept;
		
		template <class VALUE>
		static const Array<T>& from(const Array<VALUE>& other) noexcept;

	public:
		T* getData() const noexcept;

		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

	public:
		T* getPointerAt(sl_size index) const noexcept;

		sl_bool getAt(sl_size index, T* _out = sl_null) const noexcept;

		T getValueAt(sl_size index) const noexcept;

		T getValueAt(sl_size index, const T& def) const noexcept;

		sl_bool setAt(sl_size index, const T& value) const noexcept;

		T& operator[](sl_size_t index) const noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Array<T>& operator=(const std::initializer_list<T>& l) noexcept;
#endif

	public:
		Array<T> sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const noexcept;
		
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

		template <class VALUE>
		sl_size read(sl_size startSource, sl_size len, VALUE* dataDst) const noexcept;

		template <class VALUE>
		sl_size write(sl_size startTarget, sl_size len, const VALUE* dataSrc) const noexcept;

		template <class VALUE>
		sl_size copy(sl_size startTarget, const Array<VALUE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const noexcept;

		template <class VALUE>
		sl_size copy(const Array<VALUE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const noexcept;

		Array<T> duplicate() const noexcept;

		sl_bool getData(ArrayData<T>& data) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sort(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc(const COMPARE& compare = COMPARE()) const noexcept;
		
		void reverse() const noexcept;

		// range-based for loop
		T* begin() const noexcept;

		T* end() const noexcept;
		
	};
	
	
	template <class T>
	class SLIB_EXPORT Atomic< Array<T> >
	{
	public:
		AtomicRef< CArray<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CArray<T>)
		
	public:
		Atomic(sl_size count) noexcept;
		
		template <class VALUE>
		Atomic(const VALUE* data, sl_size count) noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic(const std::initializer_list<T>& l) noexcept;
#endif
		
		Atomic(const T* data, sl_size count, Referable* refer) noexcept;
		
	public:
		template <class VALUE>
		static const Atomic< Array<T> >& from(const Atomic< Array<VALUE> >& other) noexcept;

		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

	public:
		sl_bool getAt(sl_size index, T* _out = sl_null) const noexcept;

		T getValueAt(sl_size index) const noexcept;

		T getValueAt(sl_size index, const T& def) const noexcept;

		sl_bool setAt(sl_size index, const T& value) const noexcept;

		T operator[](sl_size_t index) const noexcept;
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Atomic& operator=(const std::initializer_list<T>& l) noexcept;
#endif

	public:
		Array<T> sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const noexcept;
		
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
	
		template <class VALUE>
		sl_size read(sl_size startSource, sl_size len, VALUE* dataDst) const noexcept;

		template <class VALUE>
		sl_size write(sl_size startTarget, sl_size len, const VALUE* dataSrc) const noexcept;

		template <class VALUE>
		sl_size copy(sl_size startTarget, const Array<VALUE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const noexcept;

		template <class VALUE>
		sl_size copy(const Array<VALUE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const noexcept;

		Array<T> duplicate() const noexcept;

		sl_bool getData(ArrayData<T>& data) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sort(const COMPARE& compare = COMPARE()) const noexcept;
		
		template < class COMPARE = Compare<T> >
		void sortDesc(const COMPARE& compare = COMPARE()) const noexcept;
		
		void reverse() const noexcept;

		// range-based for loop
		ArrayPosition<T> begin() const noexcept;

		ArrayPosition<T> end() const noexcept;

	};
	
}

#include "detail/array.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/array_std.inc"
#endif

#endif
