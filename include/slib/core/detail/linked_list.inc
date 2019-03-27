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
	SLIB_INLINE CLinkedList<T>::CLinkedList() noexcept
	{
		m_front = sl_null;
		m_back = sl_null;
		m_count = 0;
	}
	
	template <class T>
	CLinkedList<T>::~CLinkedList() noexcept
	{
		removeAll_NoLock();
	}
	
	template <class T>
	SLIB_INLINE void CLinkedList<T>::initialize() noexcept
	{
		m_front = sl_null;
		m_back = sl_null;
		m_count = 0;
	}
	
	template <class T>
	SLIB_INLINE void CLinkedList<T>::initialize(Link<T>* front, Link<T>* back, sl_size count) noexcept
	{
		m_front = front;
		m_back = back;
		m_count = count;
	}
	
	template <class T>
	SLIB_INLINE void CLinkedList<T>::initialize(const CLinkedList<T>* other) noexcept
	{
		m_front = other->m_front;
		m_back = other->m_back;
		m_count = other->m_count;
	}
	
	template <class T>
	SLIB_INLINE Link<T>* CLinkedList<T>::getFront() const noexcept
	{
		return m_front;
	}
	
	template <class T>
	SLIB_INLINE Link<T>* CLinkedList<T>::getBack() const noexcept
	{
		return m_back;
	}
	
	template <class T>
	SLIB_INLINE sl_size CLinkedList<T>::getCount() const noexcept
	{
		return m_count;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CLinkedList<T>::isEmpty() const noexcept
	{
		return m_front == sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CLinkedList<T>::isNotEmpty() const noexcept
	{
		return m_front != sl_null;
	}
	
	template <class T>
	sl_bool CLinkedList<T>::getFrontValue_NoLock(T* _out) const noexcept
	{
		if (m_front) {
			if (_out) {
				*_out = m_front->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CLinkedList<T>::getFrontValue(T* _out) const noexcept
	{
		ObjectLocker lock(this);
		return getFrontValue_NoLock(_out);
	}
	
	template <class T>
	sl_bool CLinkedList<T>::getBackValue_NoLock(T* _out) const noexcept
	{
		if (m_back) {
			if (_out) {
				*_out = m_back->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CLinkedList<T>::getBackValue(T* _out) const noexcept
	{
		ObjectLocker lock(this);
		return getBackValue_NoLock(_out);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	Link<T>* CLinkedList<T>::find_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Link<T>* now = m_front;
		while (now) {
			if (equals(now->value, value)) {
				return now;
			}
			now = now->next;
		}
		return sl_null;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CLinkedList<T>::find(const VALUE& value, const EQUALS& equals) const noexcept
	{
		ObjectLocker lock(this);
		return find_NoLock(value) != sl_null;
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::pushBack_NoLock(const T& value, sl_size countLimit) noexcept
	{
		Link<T>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		Link<T>* old = _pushBackItem(item, countLimit);
		if (old) {
			_freeItem(old);
		}
		return item;
	}
	
	template <class T>
	sl_bool CLinkedList<T>::pushBack(const T& value, sl_size countLimit) noexcept
	{
		Link<T>* item = _createItem(value);
		if (!item) {
			return sl_false;
		}
		Link<T>* old;
		{
			ObjectLocker lock(this);
			old = _pushBackItem(item, countLimit);
		}
		if (old) {
			_freeItem(old);
		}
		return sl_true;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CLinkedList<T>::pushBackAll_NoLock(const CLinkedList<VALUE>* other) noexcept
	{
		Link<VALUE>* link = other->getFront();
		while (link) {
			if (!(pushBack_NoLock(link->value))) {
				return sl_false;
			}
			link = link->next;
		}
		return sl_true;
	}

	template <class T>
	template <class VALUE>
	sl_bool CLinkedList<T>::pushBackAll(const CLinkedList<VALUE>* other) noexcept
	{
		MultipleObjectsLocker lock(this, other);
		return pushBackAll_NoLock(other);
	}
	
	template <class T>
	sl_bool CLinkedList<T>::popBack_NoLock(T* _out) noexcept
	{
		Link<T>* old = _popBackItem();
		if (old) {
			if (_out) {
				*_out = old->value;
			}
			_freeItem(old);
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	sl_bool CLinkedList<T>::popBack(T* _out) noexcept
	{
		Link<T>* old;
		{
			ObjectLocker lock(this);
			old = _popBackItem();
		}
		if (old) {
			if (_out) {
				*_out = old->value;
			}
			_freeItem(old);
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::pushFront_NoLock(const T& value, sl_size countLimit) noexcept
	{
		Link<T>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		Link<T>* old = _pushFrontItem(item, countLimit);
		if (old) {
			_freeItem(old);
		}
		return item;
	}
	
	template <class T>
	sl_bool CLinkedList<T>::pushFront(const T& value, sl_size countLimit) noexcept
	{
		Link<T>* item = _createItem(value);
		if (!item) {
			return sl_false;
		}
		Link<T>* old;
		{
			ObjectLocker lock(this);
			old = _pushFrontItem(item, countLimit);
		}
		if (old) {
			_freeItem(old);
		}
		return sl_true;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CLinkedList<T>::pushFrontAll_NoLock(const CLinkedList<VALUE>* other) noexcept
	{
		Link<VALUE>* link = other->getBack();
		while (link) {
			if (!(pushFront_NoLock(link->value))) {
				return sl_false;
			}
			link = link->before;
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	sl_bool CLinkedList<T>::pushFrontAll(const CLinkedList<VALUE>* other) noexcept
	{
		MultipleObjectsLocker lock(this, other);
		return pushFrontAll_NoLock(other);
	}
	
	template <class T>
	sl_bool CLinkedList<T>::popFront_NoLock(T* _out) noexcept
	{
		Link<T>* old = _popFrontItem();
		if (old) {
			if (_out) {
				*_out = old->value;
			}
			_freeItem(old);
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	sl_bool CLinkedList<T>::popFront(T* _out) noexcept
	{
		Link<T>* old;
		{
			ObjectLocker lock(this);
			old = _popFrontItem();
		}
		if (old) {
			if (_out) {
				*_out = old->value;
			}
			_freeItem(old);
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::insertBefore(Link<T>* itemWhere, const T& value) noexcept
	{
		if (itemWhere) {
			Link<T>* itemNew = _createItem(value);
			if (itemNew) {
				_insertBefore(itemWhere, itemNew);
			}
			return itemNew;
		} else {
			return pushFront_NoLock(value);
		}
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::insertAfter(Link<T>* itemWhere, const T& value) noexcept
	{
		if (itemWhere) {
			Link<T>* itemNew = _createItem(value);
			if (itemNew) {
				_insertAfter(itemWhere, itemNew);
			}
			return itemNew;
		} else {
			return pushBack_NoLock(value);
		}
	}
	
	template <class T>
	void CLinkedList<T>::removeAt(Link<T>* item) noexcept
	{
		if (item) {
			_removeItem(item);
			_freeItem(item);
		}
	}
	
	template <class T>
	sl_size CLinkedList<T>::removeAll_NoLock() noexcept
	{
		Link<T>* front = m_front;
		sl_size count = m_count;
		initialize();
		freeLink(front);
		return count;
	}
	
	template <class T>
	sl_size CLinkedList<T>::removeAll() noexcept
	{
		Link<T>* front;
		sl_size count;
		{
			ObjectLocker lock(this);
			front = m_front;
			count = m_count;
			initialize();
		}
		freeLink(front);
		return count;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CLinkedList<T>::remove_NoLock(const VALUE& value, const EQUALS& equals) noexcept
	{
		Link<T>* now = m_front;
		while (now) {
			Link<T>* next = now->next;
			if (equals(now->value, value)) {
				removeAt(now);
				return sl_true;
			}
			now = next;
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CLinkedList<T>::remove(const VALUE& value, const EQUALS& equals) noexcept
	{
		ObjectLocker lock(this);
		return remove_NoLock(value, equals);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_size CLinkedList<T>::removeValues_NoLock(const VALUE& value, const EQUALS& equals) noexcept
	{
		sl_size n = 0;
		Link<T>* now = m_front;
		while (now) {
			Link<T>* next = now->next;
			if (equals(now->value, value)) {
				n++;
				removeAt(now);
			}
			now = next;
		}
		return n;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_size CLinkedList<T>::removeValues(const VALUE& value, const EQUALS& equals) noexcept
	{
		ObjectLocker lock(this);
		return removeValues_NoLock(value, equals);
	}
	
	template <class T>
	void CLinkedList<T>::merge_NoLock(CLinkedList<T>* other) noexcept
	{
		if (!other) {
			return;
		}
		if (this == other) {
			return;
		}
		Link<T>* _front = other->getFront();
		Link<T>* _back = other->getBack();
		if (_front) {
			sl_size countNew = m_count + other->getCount();
			if (m_back) {
				m_back->next = _front;
				_front->before = m_back;
			} else {
				m_front = _front;
			}
			m_back = _back;
			other->initialize();
			m_count = countNew;
		}
	}

	template <class T>
	void CLinkedList<T>::merge(CLinkedList<T>* other) noexcept
	{
		if (!other) {
			return;
		}
		if (this == other) {
			return;
		}
		MultipleObjectsLocker lock(this, other);
		merge_NoLock(other);
	}
	
	template <class T>
	Array<T> CLinkedList<T>::toArray_NoLock() const noexcept
	{
		Array<T> ret;
		if (m_count) {
			ret = Array<T>::create(m_count);
			if (ret.isNotNull()) {
				T* buf = ret.getData();
				sl_size index = 0;
				Link<T>* now = m_front;
				while (now) {
					buf[index] = now->value;
					now = now->next;
					index++;
				}
			}
		}
		return ret;
	}
	
	template <class T>
	Array<T> CLinkedList<T>::toArray() const noexcept
	{
		ObjectLocker lock(this);
		return toArray_NoLock();
	}
	
	template <class T>
	List<T> CLinkedList<T>::toList_NoLock() const noexcept
	{
		List<T> ret;
		if (m_count) {
			ret = List<T>::create(m_count);
			if (ret.isNotNull()) {
				ListElements<T> list(ret);
				sl_size index = 0;
				Link<T>* now = m_front;
				while (now) {
					list[index] = now->value;
					now = now->next;
					index++;
				}
			}
		}
		return ret;
	}
	
	template <class T>
	List<T> CLinkedList<T>::toList() const noexcept
	{
		ObjectLocker lock(this);
		return toList_NoLock();
	}
	
	template <class T>
	CLinkedList<T>* CLinkedList<T>::duplicate_NoLock() const noexcept
	{
		CLinkedList<T>* ret = new CLinkedList<T>;
		if (ret) {
			Link<T>* now = m_front;
			while (now) {
				if (!(ret->pushBack_NoLock(now->value))) {
					delete ret;
					return sl_null;
				}
				now = now->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class T>
	CLinkedList<T>* CLinkedList<T>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	template <class T>
	SLIB_INLINE LinkPosition<T> CLinkedList<T>::begin() const noexcept
	{
		return LinkPosition<T>(m_front);
	}
	
	template <class T>
	SLIB_INLINE LinkPosition<T> CLinkedList<T>::end() const noexcept
	{
		return LinkPosition<T>();
	}
	
	template <class T>
	void CLinkedList<T>::freeLink(Link<T>* link) noexcept
	{
		while (link) {
			Link<T>* next = link->next;
			_freeItem(link);
			link = next;
		}
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::_createItem(const T& value) noexcept
	{
		Link<T>* item = (Link<T>*)(Base::createMemory(sizeof(Link<T>)));
		if (!item) {
			return sl_null;
		}
		item->next = sl_null;
		item->before = sl_null;
		new (&(item->value)) T(value);
		return item;
	}
	
	template <class T>
	void CLinkedList<T>::_freeItem(Link<T>* item) noexcept
	{
		item->value.T::~T();
		Base::freeMemory(item);
	}
	
	
	template <class T>
	Link<T>* CLinkedList<T>::_pushBackItem(Link<T>* item, sl_size countLimit) noexcept
	{
		Link<T>* old;
		if (countLimit > 0 && m_count >= countLimit) {
			old = _popFrontItem();
		} else {
			old = sl_null;
		}
		if (m_back) {
			m_back->next = item;
			item->before = m_back;
			m_back = item;
		} else {
			m_front = item;
			m_back = item;
		}
		m_count++;
		return old;
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::_popBackItem() noexcept
	{
		Link<T>* back = m_back;
		if (back) {
			m_count--;
			Link<T>* before = back->before;
			if (before) {
				before->next = sl_null;
				m_back = before;
			} else {
				m_front = sl_null;
				m_back = sl_null;
			}
		}
		return back;
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::_pushFrontItem(Link<T>* item, sl_size countLimit) noexcept
	{
		Link<T>* old;
		if (countLimit > 0 && m_count >= countLimit) {
			old = _popBackItem();
		} else {
			old = sl_null;
		}
		if (m_front) {
			item->next = m_front;
			m_front->before = item;
			m_front = item;
		} else {
			m_front = item;
			m_back = item;
		}
		m_count++;
		return old;
	}
	
	template <class T>
	Link<T>* CLinkedList<T>::_popFrontItem() noexcept
	{
		Link<T>* front = m_front;
		if (front) {
			m_count--;
			Link<T>* next = front->next;
			if (next) {
				next->before = sl_null;
				m_front = next;
			} else {
				m_front = sl_null;
				m_back = sl_null;
			}
		}
		return front;
	}
	
	template <class T>
	void CLinkedList<T>::_removeItem(Link<T>* item) noexcept
	{
		m_count--;
		Link<T>* before = item->before;
		Link<T>* next = item->next;
		if (before) {
			before->next = next;
		} else {
			m_front = next;
		}
		if (next) {
			next->before = before;
		} else {
			m_back = before;
		}
	}
	
	template <class T>
	void CLinkedList<T>::_insertBefore(Link<T>* itemWhere, Link<T>* itemNew) noexcept
	{
		itemNew->next = itemWhere;
		Link<T>* before = itemWhere->before;
		itemNew->before = before;
		itemWhere->before = itemNew;
		if (before) {
			before->next = itemNew;
		} else {
			m_front = itemNew;
		}
		m_count++;
	}

	template <class T>
	void CLinkedList<T>::_insertAfter(Link<T>* itemWhere, Link<T>* itemNew) noexcept
	{
		itemNew->before = itemWhere;
		Link<T>* next = itemWhere->next;
		itemNew->next = next;
		itemWhere->next = itemNew;
		if (next) {
			next->before = itemNew;
		} else {
			m_back = itemNew;
		}
		m_count++;
	}


	template <class T>
	LinkedList<T> LinkedList<T>::create() noexcept
	{
		return new CLinkedList<T>;
	}

	template <class T>
	SLIB_INLINE Link<T>* LinkedList<T>::getFront() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFront();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE Link<T>* LinkedList<T>::getBack() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getBack();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE sl_size LinkedList<T>::getCount() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_bool LinkedList<T>::isEmpty() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->isEmpty();
		}
		return sl_true;
	}

	template <class T>
	SLIB_INLINE sl_bool LinkedList<T>::isNotEmpty() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->isNotEmpty();
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::getFrontValue_NoLock(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFrontValue_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::getFrontValue(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFrontValue(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::getBackValue_NoLock(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getBackValue_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::getBackValue(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getBackValue(_out);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	Link<T>* LinkedList<T>::find_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->find_NoLock(value, equals);
		}
		return sl_null;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool LinkedList<T>::find(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->find(value, equals);
		}
		return sl_false;
	}

	template <class T>
	Link<T>* LinkedList<T>::pushBack_NoLock(const T& value, sl_size countLimit) noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushBack_NoLock(value, countLimit);
		} else {
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				return obj->pushBack_NoLock(value, countLimit);
			}
		}
		return sl_null;
	}

	template <class T>
	sl_bool LinkedList<T>::pushBack(const T& value, sl_size countLimit) noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushBack(value, countLimit);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->pushBack(value, countLimit);
			}
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->pushBack(value, countLimit);
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool LinkedList<T>::pushBackAll_NoLock(const LinkedList<VALUE>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushBackAll_NoLock(other);
		} else {
			ref = other->duplicate_NoLock();
			return ref.isNotNull();
		}
	}

	template <class T>
	template <class VALUE>
	sl_bool LinkedList<T>::pushBackAll(const LinkedList<VALUE>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushBackAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->pushBackAll(other);
			}
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->pushBackAll(other);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::popBack_NoLock(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBack_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::popBack(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBack(_out);
		}
		return sl_false;
	}

	template <class T>
	Link<T>* LinkedList<T>::pushFront_NoLock(const T& value, sl_size countLimit) noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushFront_NoLock(value, countLimit);
		} else {
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				return obj->pushFront_NoLock(value, countLimit);
			}
		}
		return sl_null;
	}

	template <class T>
	sl_bool LinkedList<T>::pushFront(const T& value, sl_size countLimit) noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushFront(value, countLimit);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->pushFront(value, countLimit);
			}
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->pushFront(value, countLimit);
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool LinkedList<T>::pushFrontAll_NoLock(const LinkedList<VALUE>& _other) noexcept
	{
		LinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushFrontAll_NoLock(other);
		} else {
			ref = other->duplicate_NoLock();
			return ref.isNotNull();
		}
	}

	template <class T>
	template <class VALUE>
	sl_bool LinkedList<T>::pushFrontAll(const LinkedList<VALUE>& _other) noexcept
	{
		LinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->pushFrontAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->pushFrontAll(other);
			}
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->pushFrontAll(other);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::popFront_NoLock(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFront_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool LinkedList<T>::popFront(T* _out) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	Link<T>* LinkedList<T>::insertBefore(Link<T>* itemWhere, const T& value) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->insertBefore(itemWhere, value);
		}
		return sl_null;
	}

	template <class T>
	Link<T>* LinkedList<T>::insertAfter(Link<T>* itemWhere, const T& value) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->insertAfter(itemWhere, value);
		}
		return sl_null;
	}

	template <class T>
	void LinkedList<T>::removeAt(Link<T>* item) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->removeAt(item);
		}
	}

	template <class T>
	sl_size LinkedList<T>::removeAll_NoLock() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	template <class T>
	sl_size LinkedList<T>::removeAll() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool LinkedList<T>::remove_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->remove_NoLock(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool LinkedList<T>::remove(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->remove(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_size LinkedList<T>::removeValues_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeValues_NoLock(value, equals);
		}
		return 0;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_size LinkedList<T>::removeValues(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeValues(value, equals);
		}
		return 0;
	}
	
	template <class T>
	void LinkedList<T>::merge_NoLock(LinkedList<T>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return;
		}
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->merge_NoLock(other);
		} else {
			obj = new CLinkedList<T>;
			if (obj) {
				obj->initialize(other);
				other->initialize();
				ref = obj;
			}
		}
	}

	template <class T>
	void LinkedList<T>::merge(LinkedList<T>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return;
		}
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->merge(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				obj->merge(other);
				return;
			}
			obj = new CLinkedList<T>;
			if (obj) {
				ref = obj;
				lock.unlock();
				obj->merge(other);
				return;
			}
		}
	}

	template <class T>
	Array<T> LinkedList<T>::toArray_NoLock() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toArray_NoLock();
		}
		return sl_null;
	}

	template <class T>
	Array<T> LinkedList<T>::toArray() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toArray();
		}
		return sl_null;
	}

	template <class T>
	List<T> LinkedList<T>::toList_NoLock() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toList_NoLock();
		}
		return sl_null;
	}

	template <class T>
	List<T> LinkedList<T>::toList() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class T>
	LinkedList<T> LinkedList<T>::duplicate_NoLock() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}

	template <class T>
	LinkedList<T> LinkedList<T>::duplicate() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE LinkPosition<T> LinkedList<T>::begin() const noexcept
	{
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			return LinkPosition<T>(obj->getFront());
		}
		return LinkPosition<T>();
	}

	template <class T>
	SLIB_INLINE LinkPosition<T> LinkedList<T>::end() const noexcept
	{
		return LinkPosition<T>();
	}


	template <class T>
	SLIB_INLINE sl_size Atomic< LinkedList<T> >::getCount() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< LinkedList<T> >::isEmpty() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->isEmpty();
		}
		return sl_true;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< LinkedList<T> >::isNotEmpty() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->isNotEmpty();
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< LinkedList<T> >::getFrontValue(T* _out) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getFrontValue(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< LinkedList<T> >::getBackValue(T* _out) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getBackValue(_out);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Atomic< LinkedList<T> >::find(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->find(value, equals);
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< LinkedList<T> >::pushBack(const T& value, sl_size countLimit) noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->pushBack(value, countLimit);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->pushBack(value, countLimit);
			}
			obj = new CLinkedList<T>;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->pushBack(value, countLimit);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	sl_bool Atomic< LinkedList<T> >::pushBackAll(const LinkedList<VALUE>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->pushBackAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->pushBackAll(other);
			}
			obj = new CLinkedList<T>;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->pushBackAll(other);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< LinkedList<T> >::popBack(T* _out) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popBack(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< LinkedList<T> >::pushFront(const T& value, sl_size countLimit) noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->pushFront(value, countLimit);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->pushFront(value, countLimit);
			}
			obj = new CLinkedList<T>;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->pushFront(value, countLimit);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	sl_bool Atomic< LinkedList<T> >::pushFrontAll(const LinkedList<VALUE>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->pushFrontAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->pushFrontAll(other);
			}
			obj = new CLinkedList<T>;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->pushFrontAll(other);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< LinkedList<T> >::popFront(T* _out) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< LinkedList<T> >::removeAll() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Atomic< LinkedList<T> >::remove(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeValue(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_size Atomic< LinkedList<T> >::removeValues(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeValues(value, equals);
		}
		return 0;
	}

	template <class T>
	void Atomic< LinkedList<T> >::merge(LinkedList<T>& _other) noexcept
	{
		CLinkedList<T>* other = _other.ref._ptr;
		if (!other) {
			return;
		}
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->merge(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->merge(other);
				return;
			}
			obj = new CLinkedList<T>;
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->merge(other);
				return;
			}
		}
	}

	template <class T>
	Array<T> Atomic< LinkedList<T> >::toArray() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->toArray();
		}
		return sl_null;
	}

	template <class T>
	List<T> Atomic< LinkedList<T> >::toList() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class T>
	LinkedList<T> Atomic< LinkedList<T> >::duplicate() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE LinkPosition<T> Atomic< LinkedList<T> >::begin() const noexcept
	{
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			return LinkPosition<T>(obj->getFront(), obj.get());
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE LinkPosition<T> Atomic< LinkedList<T> >::end() const noexcept
	{
		return LinkPosition<T>();
	}


	template <class T>
	SLIB_INLINE LinkPosition<T>::LinkPosition() noexcept : link(sl_null) {}

	template <class T>
	SLIB_INLINE LinkPosition<T>::LinkPosition(Link<T>* _link) noexcept : link(_link) {}
	
	template <class T>
	SLIB_INLINE LinkPosition<T>::LinkPosition(Link<T>* _link, Referable* _ref) noexcept : link(_link), ref(_ref) {}

	template <class T>
	SLIB_INLINE T& LinkPosition<T>::operator*() const noexcept
	{
		return link->value;
	}
	
	template <class T>
	SLIB_INLINE sl_bool LinkPosition<T>::operator==(const LinkPosition<T>& p) const noexcept
	{
		return link == p.link;
	}

	template <class T>
	SLIB_INLINE sl_bool LinkPosition<T>::operator!=(const LinkPosition<T>& p) const noexcept
	{
		return link != p.link;
	}
	
	template <class T>
	SLIB_INLINE LinkPosition<T>::operator Link<T>*() const noexcept
	{
		return link;
	}

	template <class T>
	SLIB_INLINE LinkPosition<T>& LinkPosition<T>::operator++() noexcept
	{
		link = link->next;
		return *this;
	}

}
