/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ARRAY2D
#define CHECKHEADER_SLIB_CORE_ARRAY2D

#include "definition.h"

#include "ref.h"
#include "array.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT ArrayData2D
	{
	public:
		T* data;
		sl_size width;
		sl_size height;
		sl_reg stride;
		Ref<Referable> refer;

	public:
		T& element(sl_reg x, sl_reg y) const noexcept;

	};
	
	template <class T>
	class Array2D;
	
	template <class T>
	using AtomicArray2D = Atomic< Array2D<T> >;
	
	
	class SLIB_EXPORT CArray2DBase : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		CArray2DBase() noexcept;

		~CArray2DBase() noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT CArray2D : public CArray2DBase
	{
	protected:
		T* m_data;
		sl_size m_width;
		sl_size m_height;
		sl_reg m_stride;
		sl_bool m_flagStatic;
		Ref<Referable> m_refer;

	protected:
		CArray2D() noexcept;

	public:
		CArray2D(sl_size width, sl_size height) noexcept;

		template <class VALUE>
		CArray2D(sl_size width, sl_size height, const VALUE* data, sl_size strideSrc = 0) noexcept;

		CArray2D(const T* data, sl_size width, sl_size height, sl_size stride, Referable* refer) noexcept;

		~CArray2D() noexcept;

	public:
		static CArray2D<T>* create(sl_size width, sl_size height) noexcept;

		template <class VALUE>
		static CArray2D<T>* create(sl_size width, sl_size height, const VALUE* data, sl_size strideSrc = 0) noexcept;

		static CArray2D<T>* createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer) noexcept;

	public:
		T* getData() const noexcept;

		sl_size getWidth() const noexcept;

		sl_size getHeight() const noexcept;

		sl_reg getStride() const noexcept;

		sl_bool isStatic() const noexcept;

		const Ref<Referable>& getRefer() const noexcept;

		sl_bool getAt(sl_reg x, sl_reg y, T* _out = sl_null) const noexcept;

		T getValueAt(sl_reg x, sl_reg y) const noexcept;

		T getValueAt(sl_reg x, sl_reg y, const T& def) const noexcept;

		sl_bool setAt(sl_reg x, sl_reg y, const T& value) const noexcept;

	public:
		CArray2D<T>* sub(sl_size x, sl_size y, sl_size width, sl_size height) noexcept;

		template <class VALUE>
		sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, VALUE* dataDst, sl_reg strideDst = 0) const noexcept;

		template <class VALUE>
		sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const VALUE* dataSrc, sl_reg strideSrc = 0) const noexcept;

		template <class VALUE>
		sl_size copy(sl_size xTarget, sl_size yTarget, const CArray2D<VALUE>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const noexcept;

		template <class VALUE>
		sl_size copy(const CArray2D<VALUE>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const noexcept;

		CArray2D<T>* duplicate() const noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT Array2D
	{
	public:
		Ref< CArray2D<T> > ref;
		SLIB_REF_WRAPPER(Array2D, CArray2D<T>)
	
	public:
		static Array2D<T> create(sl_size width, sl_size height) noexcept;
	
		template <class VALUE>
		static Array2D<T> create(sl_size width, sl_size height, const VALUE* dataIn, sl_reg strideIn = 0) noexcept;

		static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride = 0) noexcept;

		static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, Referable* refer) noexcept;

		static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer) noexcept;

	public:
		T* getData() const noexcept;

		sl_size getWidth() const noexcept;

		sl_size getHeight() const noexcept;

		sl_reg getStride() const noexcept;

		sl_bool getAt(sl_reg x, sl_reg y, T* _out = sl_null) const noexcept;

		T getValueAt(sl_reg x, sl_reg y) const noexcept;
	
		T getValueAt(sl_reg x, sl_reg y, const T& def) const noexcept;

		sl_bool setAt(sl_reg x, sl_reg y, const T& value) const noexcept;

	public:
		Array2D<T> sub(sl_size x, sl_size y, sl_size width, sl_size height) const noexcept;

		template <class VALUE>
		sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, VALUE* dataDst, sl_reg strideDst = 0) const noexcept;

		template <class VALUE>
		sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const VALUE* dataSrc, sl_reg strideSrc = 0) const noexcept;

		template <class VALUE>
		sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<VALUE>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept;

		template <class VALUE>
		sl_size copy(const Array2D<VALUE>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept;

		Array2D<T> duplicate() const noexcept;

		sl_bool getData(ArrayData2D<T>& data) const noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< Array2D<T> >
	{
	public:
		AtomicRef< CArray2D<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CArray2D<T>)
	
	public:
		sl_size getWidth() const noexcept;

		sl_size getHeight() const noexcept;

		sl_bool isEmpty() const noexcept;

		sl_bool isNotEmpty() const noexcept;

		sl_bool getAt(sl_reg x, sl_reg y, T* _out = sl_null) const noexcept;

		T getValueAt(sl_reg x, sl_reg y) const noexcept;
	
		T getValueAt(sl_reg x, sl_reg y, const T& def) const noexcept;

		sl_bool setAt(sl_reg x, sl_reg y, const T& value) const noexcept;

	public:
		Array2D<T> sub(sl_size x, sl_size y, sl_size width, sl_size height) const noexcept;

		template <class VALUE>
		sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, VALUE* dataDst, sl_reg strideDst = 0) const noexcept;

		template <class VALUE>
		sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const VALUE* dataSrc, sl_reg strideSrc = 0) const noexcept;

		template <class VALUE>
		sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<VALUE>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const noexcept;
	
		template <class VALUE>
		sl_size copy(const Array2D<VALUE>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const noexcept;

		Array2D<T> duplicate() const noexcept;

		sl_bool getData(ArrayData2D<T>& data) const noexcept;

	};

}

#include "detail/array2d.inc"

#endif
