/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_SCOPED
#define CHECKHEADER_SLIB_CORE_SCOPED

#include "definition.h"

#include "base.h"
#include "new_helper.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT ScopedPtr
	{
	public:
		ScopedPtr();

		ScopedPtr(T* ptr);

		~ScopedPtr();

	public:
		void release();

		sl_bool isNull();

		sl_bool isNotNull();

		void setNull();

		T& operator*();

		T* operator->();

	public:
		T* ptr;

	};

	template <class T>
	class SLIB_EXPORT ScopedPtrNew : public ScopedPtr<T>
	{
	public:
		ScopedPtrNew();

	public:
		T& operator*();

		T* operator->();

	};

	template <class T>
	class SLIB_EXPORT ScopedArray
	{
	public:
		ScopedArray();

		ScopedArray(T* data, sl_size count);

		ScopedArray(sl_size count);
	
		~ScopedArray();
	
	public:
		void release();
	
		sl_bool isNull();
	
		sl_bool isNotNull();
	
		void setNull();
	
		T& operator[](sl_size index);

		T* operator+(sl_size offset);

	public:
		T* data;
		sl_size count;

	};

#define SLIB_SCOPED_ARRAY(TYPE, NAME, COUNT) \
	ScopedArray<TYPE> _scoped_array__##NAME(COUNT); \
	TYPE* NAME = _scoped_array__##NAME.data;

	
	template <class T, sl_size countStack = 0>
	class SLIB_EXPORT ScopedBuffer
	{
	public:
		ScopedBuffer(sl_size _count);

		~ScopedBuffer();

	public:
		void release();

		sl_bool isNull();

		sl_bool isNotNull();

		T& operator[](sl_size index);

		T* operator+(sl_size offset);

	public:
		T* data;
		sl_size count;

	private:
		T stack[countStack];

	};

#define SLIB_SCOPED_BUFFER(TYPE, STACK, NAME, COUNT) \
	ScopedBuffer<TYPE, STACK> _scoped_buf__##NAME(COUNT); \
	TYPE* NAME = _scoped_buf__##NAME.data;
	
	
	class ScopedCounter
	{
	public:
		ScopedCounter(sl_reg* p)
		{
			count = p;
			Base::interlockedIncrement(p);
		}

		~ScopedCounter()
		{
			Base::interlockedDecrement(count);
		}

	public:
		sl_reg* count;

	};

}

#include "detail/scoped.h"

#endif
