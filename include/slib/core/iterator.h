/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ITERATOR
#define CHECKHEADER_SLIB_CORE_ITERATOR

#include "definition.h"

#include "ref.h"

namespace slib
{
	
	class IIteratorBase : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		IIteratorBase();

		~IIteratorBase();

	};
	
	template <class T>
	class SLIB_EXPORT IIterator : public IIteratorBase
	{
	public:
		virtual sl_bool hasNext() = 0;

		virtual sl_bool next(T* _out = sl_null) = 0;

		virtual sl_reg getIndex() = 0;

	};
	
	template <class T>
	class SLIB_EXPORT IteratorPosition
	{
	public:
		IteratorPosition();

		IteratorPosition(const Ref< IIterator<T> >& iterator);

		IteratorPosition(const IteratorPosition<T>& other);

		IteratorPosition(IteratorPosition<T>&& other);

	public:
		T& operator*();

		sl_bool operator!=(const IteratorPosition<T>& p);

		IteratorPosition<T>& operator++();

	private:
		Ref< IIterator<T> > ref;
		T value;
		sl_bool flagValid;

	};
	
	template <class T>
	class SLIB_EXPORT Iterator
	{
	public:
		Ref< IIterator<T> > ref;
		SLIB_REF_WRAPPER(Iterator, IIterator<T>)

	public:
		sl_bool hasNext() const;

		sl_reg getIndex() const;

		sl_bool next(T* _out) const;

		// range-based for loop
		IteratorPosition<T> begin() const;

		IteratorPosition<T> end() const;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< Iterator<T> >
	{
	public:
		AtomicRef< IIterator<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(IIterator<T>)
	
	public:
		sl_bool hasNext() const;

		sl_reg getIndex() const;

		sl_bool next(T* _out) const;

		// range-based for loop
		IteratorPosition<T> begin() const;

		IteratorPosition<T> end() const;

	};
	
	template <class T>
	using AtomicIterator = Atomic< Iterator<T> >;

}

#include "detail/iterator.inc"

#endif
