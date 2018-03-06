/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_QUEUE
#define CHECKHEADER_SLIB_CORE_QUEUE

#include "definition.h"

#include "linked_list.h"

namespace slib
{
	
	template < class T, class CONTAINER = CLinkedList<T> >
	class SLIB_EXPORT Queue : public CONTAINER
	{
	public:
		sl_bool push_NoLock(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool push(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool pushAll(const Queue<T, CONTAINER>* other) noexcept;

		sl_bool pop_NoLock(T* _out = sl_null) noexcept;

		sl_bool pop(T* _out = sl_null) noexcept;

		Queue<T, CONTAINER>* duplicate_NoLock() const noexcept;

		Queue<T, CONTAINER>* duplicate() const noexcept;

	};
	
	template < class T, class CONTAINER = CLinkedList<T> >
	class SLIB_EXPORT Stack : public CONTAINER
	{
	public:
		sl_bool push_NoLock(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool push(const T& value, sl_size countLimit = 0) noexcept;

		sl_bool pushAll(const Stack<T, CONTAINER>* other) noexcept;

		sl_bool pop_NoLock(T* _out = sl_null) noexcept;

		sl_bool pop(T* _out = sl_null) noexcept;

		Stack<T, CONTAINER>* duplicate_NoLock() const noexcept;

		Stack<T, CONTAINER>* duplicate() const noexcept;

	};
	
	template <class T>
	using LinkedQueue = Queue< T, CLinkedList<T> >;
	
	template <class T>
	using LinkedStack = Stack< T, CLinkedList<T> >;

}

#include "detail/queue.inc"

#endif
