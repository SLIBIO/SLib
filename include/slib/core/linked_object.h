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

#ifndef CHECKHEADER_SLIB_CORE_LINKED_OBJECT
#define CHECKHEADER_SLIB_CORE_LINKED_OBJECT

#include "definition.h"

#include "object.h"
#include "array.h"
#include "list.h"

namespace slib
{
	
	template <class T>
	class LinkedObjectList;
	
	template <class T>
	class SLIB_EXPORT LinkedObject : public Object
	{
	public:
		Ref<T> next;
		WeakRef<T> before;

	};
	
	class SLIB_EXPORT LinkedObjectListBase : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		LinkedObjectListBase() noexcept;

		~LinkedObjectListBase() noexcept;

	};
	
	template <class T>
	class SLIB_EXPORT LinkedObjectList : public LinkedObjectListBase
	{
	protected:
		Ref<T> m_front;
		Ref<T> m_back;
		sl_size m_count;

	public:
		LinkedObjectList() noexcept;

		~LinkedObjectList() noexcept;

	public:
		const Ref<T>& getFront() const noexcept;
	
		const Ref<T>& getBack() const noexcept;
	
		sl_size getCount() const noexcept;

		sl_bool isEmpty() const noexcept;
	
		sl_bool isNotEmpty() const noexcept;

		sl_bool pushBack_NoLock(const Ref<T>& object, sl_size countLimit = 0) noexcept;

		sl_bool pushBack(const Ref<T>& object, sl_size countLimit = 0) noexcept;

		sl_bool popBack_NoLock(Ref<T>* _out = sl_null) noexcept;

		sl_bool popBack(Ref<T>* _out = sl_null) noexcept;

		sl_bool pushFront_NoLock(const Ref<T>& object, sl_size countLimit = 0) noexcept;

		sl_bool pushFront(const Ref<T>& object, sl_size countLimit = 0) noexcept;

		sl_bool popFront_NoLock(Ref<T>* _out = sl_null) noexcept;

		sl_bool popFront(Ref<T>* _out = sl_null) noexcept;

		/* unsynchronized function */
		sl_bool insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept;
		
		/* unsynchronized function */
		sl_bool insertAfter(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept;

		sl_bool remove_NoLock(const Ref<T>& object) noexcept;

		sl_bool remove(const Ref<T>& object) noexcept;

		sl_size removeAll_NoLock() noexcept;
		
		sl_size removeAll() noexcept;

		void merge(LinkedObjectList<T>* other) noexcept;

		Array< Ref<T> > toArray_NoLock() const noexcept;

		Array< Ref<T> > toArray() const noexcept;

		List< Ref<T> > toList_NoLock() const noexcept;

		List< Ref<T> > toList() const noexcept;

	protected:
		void _init() noexcept;

		Ref<T> _pushBack(const Ref<T>& object, sl_size countLimit) noexcept;

		Ref<T> _popBack() noexcept;

		Ref<T> _pushFront(const Ref<T>& object, sl_size countLimit) noexcept;

		Ref<T> _popFront() noexcept;

		void _remove(const Ref<T>& object) noexcept;

		void _insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept;

		void _insertAfter(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept;
	
	};

}

#include "detail/linked_object.inc"

#endif
