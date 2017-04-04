/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		AtomicRef<T> next;
		AtomicWeakRef<T> before;

	};
	
	class SLIB_EXPORT LinkedObjectListBase : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		LinkedObjectListBase();

		~LinkedObjectListBase();

	};
	
	template <class T>
	class SLIB_EXPORT LinkedObjectList : public LinkedObjectListBase
	{
	protected:
		Ref<T> m_front;
		Ref<T> m_back;
		sl_size m_count;

	public:
		LinkedObjectList();

		~LinkedObjectList();

	public:
		const Ref<T>& getFront() const;
	
		const Ref<T>& getBack() const;
	
		sl_size getCount() const;

		sl_bool isEmpty() const;
	
		sl_bool isNotEmpty() const;

	public:
		void removeAll_NoLock();

		void removeAll();

		sl_bool pushBack_NoLock(const Ref<T>& object, sl_size countLimit = 0);

		sl_bool pushBack(const Ref<T>& object, sl_size countLimit = 0);

		sl_bool popBack_NoLock(Ref<T>* _out = sl_null);

		sl_bool popBack(Ref<T>* _out = sl_null);

		sl_bool pushFront_NoLock(const Ref<T>& object, sl_size countLimit = 0);

		sl_bool pushFront(const Ref<T>& object, sl_size countLimit = 0);

		sl_bool popFront_NoLock(Ref<T>* _out = sl_null);

		sl_bool popFront(Ref<T>* _out = sl_null);

		void remove_NoLock(const Ref<T>& object);

		void remove(const Ref<T>& object);

		sl_bool insertBefore_NoLock(const Ref<T>& objectWhere, const Ref<T>& objectNew);

		sl_bool insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew);

		sl_bool insertAfter_NoLock(const Ref<T>& objectWhere, const Ref<T>& objectNew);

		sl_bool insertAfter(const Ref<T>& objectWhere, const Ref<T>& objectNew);

		void merge(LinkedObjectList<T>* other);

		Array< Ref<T> > toArray_NoLock() const;

		Array< Ref<T> > toArray() const;

		List< Ref<T> > toList_NoLock() const;

		List< Ref<T> > toList() const;

	protected:
		void _init();

		Ref<T> _pushBack(const Ref<T>& object, sl_size countLimit);

		Ref<T> _popBack();

		Ref<T> _pushFront(const Ref<T>& object, sl_size countLimit);

		Ref<T> _popFront();

		void _remove(const Ref<T>& object);

		void _insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew);

		void _insertAfter(const Ref<T>& objectWhere, const Ref<T>& objectNew);
	
	};

}

#include "detail/linked_object.h"

#endif
