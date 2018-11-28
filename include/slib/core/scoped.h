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

#include "detail/scoped.inc"

#endif
