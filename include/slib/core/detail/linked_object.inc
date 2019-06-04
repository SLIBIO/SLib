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
	sl_bool LinkedObjectList<T>::pushBack_NoLock(const Ref<T>& object) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		_pushBack(object);
		return sl_true;
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::pushBack(const Ref<T>& object) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		ObjectLocker lock(this);
		_pushBack(object);
		return sl_true;
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::popBack_NoLock(Ref<T>* _out) noexcept
	{
		Ref<T> old = _popBack();
		if (old.isNotNull()) {
			if (_out) {
				*_out = Move(old);
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
		if (old.isNotNull()) {
			if (_out) {
				*_out = Move(old);
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	sl_bool LinkedObjectList<T>::pushFront_NoLock(const Ref<T>& object) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		_pushFront(object);
		return sl_true;
	}

	template <class T>
	sl_bool LinkedObjectList<T>::pushFront(const Ref<T>& object) noexcept
	{
		if (object.isNull()) {
			return sl_false;
		}
		ObjectLocker lock(this);
		_pushFront(object);
		return sl_true;
	}

	template <class T>
	sl_bool LinkedObjectList<T>::popFront_NoLock(Ref<T>* _out) noexcept
	{
		Ref<T> old = _popFront();
		if (old.isNotNull()) {
			if (_out) {
				*_out = Move(old);
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
		if (old.isNotNull()) {
			if (_out) {
				*_out = Move(old);
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
			front = Move(m_front);
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
		MultipleObjectsLocker lock(this, other);
		T* _front = other->m_front.get();
		if (_front) {
			sl_size countNew = m_count + other->getCount();
			if (m_back.isNotNull()) {
				m_back->next = Move(other->m_front);
				_front->before = Move(m_back);
			} else {
				m_front = Move(other->m_front);
			}
			m_back = Move(other->m_back);
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
				T* now = m_front.get();
				while (now) {
					buf[index] = now;
					now = now->next.get();
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
				T* now = m_front.get();
				while (now) {
					list[index] = now;
					now = now->next.get();
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
	void LinkedObjectList<T>::_pushBack(const Ref<T>& object) noexcept
	{
		if (m_back.isNotNull()) {
			m_back->next = object;
			object->before = Move(m_back);
			object->next.setNull();
			m_back = object;
		} else {
			m_front = object;
			m_back = object;
		}
		m_count++;
	}

	template <class T>
	Ref<T> LinkedObjectList<T>::_popBack() noexcept
	{
		Ref<T> back = Move(m_back);
		if (back.isNotNull()) {
			m_count--;
			Ref<T>& before = back->before;
			if (before.isNotNull()) {
				before->next.setNull();
				m_back = Move(before);
			} else {
				m_front.setNull();
				m_back.setNull();
			}
			back->next.setNull();
		}
		return back;
	}

	template <class T>
	void LinkedObjectList<T>::_pushFront(const Ref<T>& object) noexcept
	{
		if (m_front.isNotNull()) {
			m_front->before = object;
			object->next = Move(m_front);
			object->before.setNull();
			m_front = object;
		} else {
			m_front = object;
			m_back = object;
		}
		m_count++;
	}

	template <class T>
	Ref<T> LinkedObjectList<T>::_popFront() noexcept
	{
		Ref<T> front = Move(m_front);
		if (front.isNotNull()) {
			m_count--;
			Ref<T>& next = front->next;
			if (next.isNotNull()) {
				next->before.setNull();
				m_front = Move(next);
			} else {
				m_front.setNull();
				m_back.setNull();
			}
			front->before.setNull();
		}
		return front;
	}

	template <class T>
	void LinkedObjectList<T>::_remove(const Ref<T>& object) noexcept
	{
		T* before = object->before.get();
		T* next = object->next.get();
		if (m_front == object) {
			m_front = Move(object->next);
		} else if (before) {
			before->next = Move(object->next);
		} else {
			return;
		}
		if (m_back == object) {
			m_back = Move(object->before);
		} else if (next) {
			next->before = Move(object->before);
		} else {
			return;
		}
		m_count--;
	}

	template <class T>
	void LinkedObjectList<T>::_insertBefore(const Ref<T>& objectWhere, const Ref<T>& objectNew) noexcept
	{
		objectNew->next = objectWhere;
		T* before = objectWhere->before.get();
		objectNew->before = Move(objectWhere->before);
		objectWhere->before = objectNew;
		if (m_front == objectWhere) {
			m_front = objectNew;
		} else if (before) {
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
		T* next = objectWhere->next.get();
		objectNew->next = Move(objectWhere->next);
		objectWhere->next = objectNew;
		if (m_back == objectWhere) {
			m_back = objectNew;
		} else if (next) {
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
