/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	template <class T>
	SLIB_INLINE LinkedObjectList<T>::LinkedObjectList() noexcept
	{
		m_front.setNull();
		m_back.setNull();
		m_count = 0;
	}
	
	template <class T>
	SLIB_INLINE LinkedObjectList<T>::~LinkedObjectList() noexcept
	{
		removeAll_NoLock();
	}
	
	template <class T>
	SLIB_INLINE const Ref<T>& LinkedObjectList<T>::getFront() const noexcept
	{
		return m_front;
	}
	
	template <class T>
	SLIB_INLINE const Ref<T>& LinkedObjectList<T>::getBack() const noexcept
	{
		return m_back;
	}
	
	template <class T>
	SLIB_INLINE sl_size LinkedObjectList<T>::getCount() const noexcept
	{
		return m_count;
	}
	
	template <class T>
	SLIB_INLINE sl_bool LinkedObjectList<T>::isEmpty() const noexcept
	{
		return m_front.isNull();
	}
	
	template <class T>
	SLIB_INLINE sl_bool LinkedObjectList<T>::isNotEmpty() const noexcept
	{
		return m_front.isNotNull();
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::pushBack_NoLock(const Ref<T>& object, sl_size countLimit) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		_pushBack(object, countLimit);
		return sl_true;
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::pushBack(const Ref<T>& object, sl_size countLimit) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		Ref<T> old;
		{
			ObjectLocker lock(this);
			old = _pushBack(object, countLimit);
		}
		return sl_true;
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::popBack_NoLock(Ref<T>* _out) noexcept
	{
		Ref<T> old = _popBack();
		if (old.isNotNull()) {
			if (_out) {
				*_out = old;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::popBack(Ref<T>* _out) noexcept
	{
		Ref<T> old;
		{
			ObjectLocker lock(this);
			old = _popBack();
		}
		if (old) {
			if (_out) {
				*_out = old;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::pushFront_NoLock(const Ref<T>& object, sl_size countLimit) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		_pushFront(object, countLimit);
		return sl_true;
	}

	template <class T>
	sl_bool LinkedObjectList<T>::pushFront(const Ref<T>& object, sl_size countLimit) noexcept
	{
		if (object.isNull()) {
			return sl_null;
		}
		Ref<T> old;
		{
			ObjectLocker lock(this);
			old = _pushFront(object, countLimit);
		}
		return sl_true;
	}

	template <class T>
	sl_bool LinkedObjectList<T>::popFront_NoLock(Ref<T>* _out) noexcept
	{
		Ref<T> old = _popFront();
		if (old) {
			if (_out) {
				*_out = old;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}

	template <class T>
	sl_bool LinkedObjectList<T>::popFront(Ref<T>* _out) noexcept
	{
		Ref<T> old;
		{
			ObjectLocker lock(this);
			old = _popFront();
		}
		if (old) {
			if (_out) {
				*_out = old;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}

	template <class T>
	sl_bool LinkedObjectList<T>::remove_NoLock(const Ref<T>& object) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		_remove(object);
		return sl_true;
	}

	template <class T>
	sl_bool LinkedObjectList<T>::remove(const Ref<T>& object) noexcept
	{
		ObjectLocker lock(this);
		return remove_NoLock(object);
	}

	template <class T>
	sl_size LinkedObjectList<T>::removeAll_NoLock() noexcept
	{
		sl_size count = m_count;
		_init();
		return count;
	}
	
	template <class T>
	sl_size LinkedObjectList<T>::removeAll() noexcept
	{
		Ref<T> front;
		sl_size count;
		{
			ObjectLocker lock(this);
			front = m_front;
			count = m_count;
			_init();
		}
		return count;
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept
	{
		if (objectWhere.isNotNull()) {
			_insertBefore(objectWhere, objectNew);
			return sl_true;
		} else {
			return pushFront_NoLock(objectNew);
		}
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::insertAfter(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept
	{
		if (objectWhere.isNotNull()) {
			_insertAfter(objectWhere, objectNew);
			return sl_true;
		} else {
			return pushBack_NoLock(objectNew);
		}
	}

	template <class T>
	void LinkedObjectList<T>::merge(LinkedObjectList<T>* other) noexcept
	{
		if ((void*)this == (void*)other) {
			return;
		}
		ObjectLocker lock(this, other);
		Ref<T> _front = other->getFront();
		Ref<T> _back = other->getBack();
		if (_front) {
			sl_size countNew = m_count + other->getCount();
			if (m_back) {
				m_back->next = _front;
				_front->before = m_back;
			} else {
				m_front = _front;
			}
			m_back = _back;
			other->_init();
			m_count = countNew;
		}
	}

	template <class T>
	Array< Ref<T> > LinkedObjectList<T>::toArray_NoLock() const noexcept
	{
		Array< Ref<T> > ret;
		if (m_count) {
			ret = Array< Ref<T> >::create(m_count);
			if (ret.isNotNull()) {
				Ref<T>* buf = ret.getData();
				sl_size index = 0;
				Ref<T> now = m_front;
				while (now) {
					buf[index] = now;
					now = now->next;
					index++;
				}
			}
		}
		return ret;
	}

	template <class T>
	Array< Ref<T> > LinkedObjectList<T>::toArray() const noexcept
	{
		ObjectLocker lock(this);
		return toArray_NoLock();
	}

	template <class T>
	List< Ref<T> > LinkedObjectList<T>::toList_NoLock() const noexcept
	{
		List< Ref<T> > ret;
		if (m_count) {
			ret = List< Ref<T> >::create(m_count);
			if (ret.isNotNull()) {
				ListElements< Ref<T> > list(ret);
				sl_size index = 0;
				Ref<T> now = m_front;
				while (now) {
					list[index] = now;
					now = now->next;
					index++;
				}
			}
		}
		return ret;
	}

	template <class T>
	List< Ref<T> > LinkedObjectList<T>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return toList_NoLock();
	}

	template <class T>
	Ref<T> LinkedObjectList<T>::_pushBack(const Ref<T>& object, sl_size countLimit) noexcept
	{
		Ref<T> old;
		if (countLimit > 0 && m_count >= countLimit) {
			old = _popFront();
		}
		if (m_back.isNotNull()) {
			m_back->next = object;
			object->before = m_back;
			object->next.setNull();
			m_back = object;
		} else {
			m_front = object;
			m_back = object;
		}
		m_count++;
		return old;
	}

	template <class T>
	Ref<T> LinkedObjectList<T>::_popBack() noexcept
	{
		Ref<T> back = m_back;
		if (back.isNotNull()) {
			m_count--;
			Ref<T> before = back->before;
			if (before.isNotNull()) {
				before->next.setNull();
				m_back = before;
			} else {
				m_front.setNull();
				m_back.setNull();
			}
			back->before.setNull();
			back->next.setNull();
		}
		return back;
	}

	template <class T>
	Ref<T> LinkedObjectList<T>::_pushFront(const Ref<T>& object, sl_size countLimit) noexcept
	{
		Ref<T> old;
		if (countLimit > 0 && m_count >= countLimit) {
			old = _popBack();
		}
		if (m_front.isNotNull()) {
			object->next = m_front;
			object->before.setNull();
			m_front->before = object;
			m_front = object;
		} else {
			m_front = object;
			m_back = object;
		}
		m_count++;
		return old;
	}

	template <class T>
	Ref<T> LinkedObjectList<T>::_popFront() noexcept
	{
		Ref<T> front = m_front;
		if (front.isNotNull()) {
			m_count--;
			Ref<T> next = front->next;
			if (next.isNotNull()) {
				next->before.setNull();
				m_front = next;
			} else {
				m_front.setNull();
				m_back.setNull();
			}
			front->before.setNull();
			front->next.setNull();
		}
		return front;
	}

	template <class T>
	void LinkedObjectList<T>::_remove(const Ref<T>& object) noexcept
	{
		Ref<T> before = object->before;
		Ref<T> next = object->next;
		if (m_front == object) {
			m_front = next;
		} else if (before.isNotNull()) {
			before->next = next;
		} else {
			return;
		}
		if (m_back == object) {
			m_back = before;
		} else if (next.isNotNull()) {
			next->before = before;
		} else {
			return;
		}
		m_count--;
		object->before.setNull();
		object->next.setNull();
	}

	template <class T>
	void LinkedObjectList<T>::_insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept
	{
		objectNew->next = objectWhere;
		Ref<T> before = objectWhere->before;
		objectNew->before = before;
		objectWhere->before = objectNew;
		if (m_front == objectWhere) {
			m_front = objectNew;
		} else if (before.isNotNull()) {
			before->next = objectNew;
		} else {
			return;
		}
		m_count++;
	}

	template <class T>
	void LinkedObjectList<T>::_insertAfter(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept
	{
		objectNew->before = objectWhere;
		Ref<T> next = objectWhere->next;
		objectNew->next = next;
		objectWhere->next = objectNew;
		if (m_back == objectWhere) {
			m_back = objectNew;
		} else if (next.isNotNull()) {
			next->before = objectNew;
		} else {
			return;
		}
		m_count++;
	}

	template <class T>
	SLIB_INLINE void LinkedObjectList<T>::_init() noexcept
	{
		m_front.setNull();
		m_back.setNull();
		m_count = 0;
	}

}
