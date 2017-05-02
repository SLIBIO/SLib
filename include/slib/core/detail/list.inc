/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define SLIB_LIST_CAPACITY_MIN 5

namespace slib
{
	
	template <class T>
	CList<T>::CList()
	{
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::CList(sl_size count)
	{
		if (count > 0) {
			T* data = Base::createMemory(count * sizeof(T));
			if (data) {
				for (sl_size i = 0; i < count; i++) {
					new (data + i) T();
				}
				m_data = data;
				m_count = count;
				m_capacity = count;
				return;
			}
		}
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::CList(sl_size count, sl_size capacity)
	{
		if (capacity < count) {
			capacity = count;
		}
		if (capacity > 0) {
			T* data = Base::createMemory(capacity * sizeof(T));
			if (data) {
				for (sl_size i = 0; i < count; i++) {
					new (data + i) T();
				}
				m_data = data;
				m_count = count;
				m_capacity = capacity;
				return;
			}
		}
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::CList(sl_size count, sl_size capacity, const T& initialValue)
	{
		if (capacity < count) {
			capacity = count;
		}
		if (capacity > 0) {
			T* data = Base::createMemory(capacity * sizeof(T));
			if (data) {
				for (sl_size i = 0; i < count; i++) {
					new (data + i) T(initialValue);
				}
				m_data = data;
				m_count = count;
				m_capacity = capacity;
				return;
			}
		}
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}

	template <class T>
	template <class _T>
	CList<T>::CList(const _T* values, sl_size count)
	{
		if (count > 0) {
			T* data = Base::createMemory(count * sizeof(T));
			if (data) {
				for (sl_size i = 0; i < count; i++) {
					new (data + i) T(values[i]);
				}
				m_data = data;
				m_count = count;
				m_capacity = count;
				return;
			}
		}
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::~CList()
	{
		T* data = m_data;
		sl_size count = m_count;
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				(data+i)->~T();
			}
			Base::freeMemory((void*)data);
		}
	}
	
	template <class T>
	CList<T>* CList<T>::create()
	{
		return new CList<T>;
	}
	
	template <class T>
	CList<T>* CList<T>::create(sl_size count)
	{
		if (count > 0) {
			CList<T>* ret = new CList<T>(count);
			if (ret) {
				if (ret->m_count > 0) {
					return ret;
				}
				delete ret;
			}
		} else {
			return new CList<T>;
		}
		return sl_null;
	}
	
	template <class T>
	CList<T>* CList<T>::create(sl_size count, sl_size capacity)
	{
		if (count > 0 || capacity > 0) {
			CList<T>* ret = new CList<T>(count, capacity);
			if (ret) {
				if (ret->m_capacity > 0) {
					return ret;
				}
				delete ret;
			}
		} else {
			return new CList<T>;
		}
		return sl_null;
	}
	
	template <class T>
	CList<T>* CList<T>::create(sl_size count, sl_size capacity, const T& intialValue)
	{
		if (count > 0 || capacity > 0) {
			CList<T>* ret = new CList<T>(count, capacity, intialValue);
			if (ret) {
				if (ret->m_capacity > 0) {
					return ret;
				}
				delete ret;
			}
		} else {
			return new CList<T>;
		}
		return sl_null;
	}

	template <class T>
	template <class _T>
	CList<T>* CList<T>::create(const _T* values, sl_size count)
	{
		if (count > 0) {
			CList<T>* ret = new CList<T>(values, count);
			if (ret) {
				if (ret->m_count > 0) {
					return ret;
				}
				delete ret;
			}
		} else {
			return new CList<T>;
		}
		return sl_null;
	}
	
	template <class T>
	template <class _T>
	CList<T>* CList<T>::create(const Array<_T>& array)
	{
		return create(array.getData(), array.getCount());
	}
	
	template <class T>
	CList<T>* CList<T>::createFromElement(const T& value, sl_size count)
	{
		return create(count, count, value);
	}
	
	template <class T>
	template <class... ARGS>
	CList<T>* CList<T>::createFromElements(ARGS&&... _values)
	{
		T values[] = {Forward<ARGS>(_values)...};
		return create(values, sizeof...(_values));
	}

	template <class T>
	template <class _T>
	CList<T>* CList<T>::createCopy(CList<_T>* other)
	{
		if (other) {
			ObjectLocker lock(other);
			return create(other->getData(), other->getCount());
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_size CList<T>::getCount() const
	{
		return m_count;
	}
	
	template <class T>
	SLIB_INLINE sl_size CList<T>::getCapacity() const
	{
		return m_capacity;
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::getData() const
	{
		return m_data;
	}
	
	template <class T>
	T* CList<T>::getPointerAt(sl_size index) const
	{
		if (index < m_count) {
			return m_data + index;
		}
		return sl_null;
	}
	
	template <class T>
	sl_bool CList<T>::getAt_NoLock(sl_size index, T* _out) const
	{
		if (index < m_count) {
			if (_out) {
				*_out = m_data[index];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::getAt(sl_size index, T* _out) const
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			if (_out) {
				*_out = m_data[index];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	T CList<T>::getValueAt_NoLock(sl_size index) const
	{
		if (index < m_count) {
			return m_data[index];
		} else {
			return T();
		}
	}
	
	template <class T>
	T CList<T>::getValueAt(sl_size index) const
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			return m_data[index];
		} else {
			return T();
		}
	}
	
	template <class T>
	T CList<T>::getValueAt_NoLock(sl_size index, const T& def) const
	{
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}
	
	template <class T>
	T CList<T>::getValueAt(sl_size index, const T& def) const
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::setAt_NoLock(sl_size index, _T&& value) const
	{
		if (index < m_count) {
			m_data[index] = Forward<_T>(value);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::setAt(sl_size index, _T&& value) const
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			m_data[index] = Forward<_T>(value);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T const& CList<T>::operator[](sl_size_t index) const
	{
		return m_data[index];
	}
	
	template <class T>
	SLIB_INLINE T& CList<T>::operator[](sl_size_t index)
	{
		return m_data[index];
	}
	
	template <class T>
	sl_bool CList<T>::setCount_NoLock(sl_size count)
	{
		sl_size oldCount = m_count;
		if (oldCount == count) {
			return sl_true;
		}
		if (count < oldCount) {
			T* data = m_data;
			for (sl_reg i = count; i < (sl_reg)oldCount; i++) {
				(data+i)->~T();
			}
			m_count = count;
			adjustCapacity_NoLock(count);
			return sl_true;
		} else {
			if (adjustCapacity_NoLock(count)) {
				T* data = m_data;
				for (sl_reg i = oldCount; i < (sl_reg)count; i++) {
					new (data + i) T();
				}
				m_count = count;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::setCount(sl_size count)
	{
		ObjectLocker lock(this);
		return setCount_NoLock(count);
	}
	
	template <class T>
	sl_bool CList<T>::setCapacity_NoLock(sl_size capacity)
	{
		if (capacity < m_count) {
			capacity = m_count;
		}
		if (capacity == m_capacity) {
			return sl_true;
		}
		T* data = m_data;
		if (data) {
			data = Base::reallocMemory(data, capacity * sizeof(T));
		} else {
			data = Base::createMemory(capacity * sizeof(T));
		}
		if (data) {
			m_data = data;
			m_capacity = capacity;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::setCapacity(sl_size capacity)
	{
		ObjectLocker lock(this);
		return setCapacity_NoLock(capacity);
	}
	
	template <class T>
	sl_bool CList<T>::adjustCapacity_NoLock(sl_size count)
	{
		if (m_capacity < count) {
			sl_size newCapacity = m_capacity * 3 / 2 + 1;
			if (newCapacity < count) {
				newCapacity = count;
			}
			if (newCapacity < SLIB_LIST_CAPACITY_MIN) {
				newCapacity = SLIB_LIST_CAPACITY_MIN;
			}
			T* newData;
			if (m_data) {
				newData = (T*)(Base::reallocMemory(m_data, newCapacity * sizeof(T)));
			} else {
				newData = (T*)(Base::createMemory(newCapacity * sizeof(T)));
			}
			if (newData) {
				m_data = newData;
				m_capacity = newCapacity;
			} else {
				return sl_false;
			}
		} else if (m_capacity > SLIB_LIST_CAPACITY_MIN && count < m_capacity / 2) {
			sl_size newCapacity = count * 3 / 2 + 1;
			if (newCapacity < SLIB_LIST_CAPACITY_MIN) {
				newCapacity = SLIB_LIST_CAPACITY_MIN;
			}
			if (newCapacity < m_capacity) {
				T* newData = (T*)(Base::reallocMemory(m_data, newCapacity * sizeof(T)));
				if (newData) {
					m_data = newData;
					m_capacity = newCapacity;
				}
			}
		}
		return sl_true;
	}
	
	template <class T>
	sl_bool CList<T>::adjustCapacity(sl_size count)
	{
		ObjectLocker lock(this);
		return adjustCapacity_NoLock(count);
	}

	template <class T>
	sl_bool CList<T>::shrinkToFit_NoLock()
	{
		return setCapacity_NoLock(m_count);
	}
	
	template <class T>
	sl_bool CList<T>::shrinkToFit()
	{
		ObjectLocker lock(this);
		return setCapacity_NoLock(m_count);
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::insert_NoLock(sl_size index, ARGS&&... args)
	{
		sl_size oldCount = m_count;
		if (index > oldCount) {
			index = oldCount;
		}
		sl_size newCount = oldCount + 1;
		if (adjustCapacity_NoLock(newCount)) {
			T* data = m_data + index;
			if (index < oldCount) {
				Base::moveMemory(data + 1, data, (oldCount - index) * sizeof(T));
			}
			new (data) T(Forward<ARGS>(args)...);
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::insert(sl_size index, ARGS&&... args)
	{
		ObjectLocker lock(this);
		return insert_NoLock(index, Forward<ARGS>(args)...);
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::insertElements_NoLock(sl_size index, const _T* values, sl_size nValues)
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		if (index > oldCount) {
			index = oldCount;
		}
		sl_size newCount = oldCount + nValues;
		if (adjustCapacity_NoLock(newCount)) {
			T* data = m_data + index;
			if (index < oldCount) {
				Base::moveMemory(data + nValues, data, (oldCount - index) * sizeof(T));
			}
			for (sl_size i = 0; i < nValues; i++) {
				new (data + i) T(values[i]);
			}
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::insertElements(sl_size index, const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return insertElements_NoLock(index, values, count);
	}
	
	template <class T>
	sl_bool CList<T>::insertElements_NoLock(sl_size index, sl_size nValues, const T& value)
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		if (index > oldCount) {
			index = oldCount;
		}
		sl_size newCount = oldCount + nValues;
		if (adjustCapacity_NoLock(newCount)) {
			T* data = m_data + index;
			if (index < oldCount) {
				Base::moveMemory(data + nValues, data, (oldCount - index) * sizeof(T));
			}
			for (sl_size i = 0; i < nValues; i++) {
				new (data + i) T(value);
			}
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::insertElements(sl_size index, sl_size count, const T& value)
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return insertElements_NoLock(index, count, count);
	}

	template <class T>
	template <class _T>
	sl_bool CList<T>::insertAll_NoLock(sl_size index, const CList<_T>* other)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		return insertElements_NoLock(index, other->getData(), other->getCount());
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::insertAll(sl_size index, const CList<_T>* other)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		ObjectLocker lock(this, other);
		return insertElements_NoLock(index, other->getData(), other->getCount());
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::add_NoLock(ARGS&&... args)
	{
		sl_size oldCount = m_count;
		sl_size newCount = oldCount + 1;
		if (adjustCapacity_NoLock(newCount)) {
			new (m_data + oldCount) T(Forward<ARGS>(args)...);
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::add(ARGS&&... args)
	{
		ObjectLocker lock(this);
		return add_NoLock(Forward<ARGS>(args)...);
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::addElements_NoLock(const _T* values, sl_size nValues)
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		sl_size newCount = oldCount + nValues;
		if (adjustCapacity_NoLock(newCount)) {
			T* data = m_data + oldCount;
			for (sl_size i = 0; i < nValues; i++) {
				new (data + i) T(values[i]);
			}
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::addElements(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return addElements_NoLock(values, count);
	}
	
	template <class T>
	sl_bool CList<T>::addElements_NoLock(sl_size nValues, const T& value)
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		sl_size newCount = oldCount + nValues;
		if (adjustCapacity_NoLock(newCount)) {
			T* data = m_data + oldCount;
			for (sl_size i = 0; i < nValues; i++) {
				new (data + i) T(value);
			}
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::addElements(sl_size count, const T& value)
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return addElements_NoLock(count, value);
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::addAll_NoLock(const CList<_T>* other)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		return addElements_NoLock(other->getData(), other->getCount());
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::addAll(const CList<_T>* other)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		ObjectLocker lock(this, other);
		return addElements_NoLock(other->getData(), other->getCount());
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool CList<T>::addIfNotExist_NoLock(_T&& value, const EQUALS& equals)
	{
		if (indexOf_NoLock(value, 0, equals) < 0) {
			return add_NoLock(value);
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool CList<T>::addIfNotExist(_T&& value, const EQUALS& equals)
	{
		ObjectLocker lock(this);
		if (indexOf_NoLock(value, 0, equals) < 0) {
			return add_NoLock(value);
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::addAll_NoLock(const Iterator<_T>& iterator)
	{
		_T value;
		while (iterator.next(&value)) {
			if (!(add_NoLock(value))) {
				return sl_false;
			}
		}
		return sl_true;
	}
	
	template <class T>
	template <class _T>
	sl_bool CList<T>::addAll(const Iterator<_T>& iterator)
	{
		ObjectLocker lock(this);
		return addAll_NoLock(iterator);
	}
	
	template <class T>
	sl_bool CList<T>::removeAt_NoLock(sl_size index, T* outValue)
	{
		sl_size total = m_count;
		if (index < total) {
			T* m = m_data + index;
			if (outValue) {
				*outValue = *m;
			}
			sl_size n = total - index - 1;
			while (n) {
				*(m) = *(m+1);
				n--;
				m++;
			}
			setCount_NoLock(total - 1);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::removeAt(sl_size index, T* outValue)
	{
		ObjectLocker lock(this);
		return removeAt_NoLock(index, outValue);
	}
	
	template <class T>
	sl_size CList<T>::removeRange_NoLock(sl_size index, sl_size count)
	{
		sl_size total = m_count;
		if (index < total && count > 0) {
			if (count > total - index) {
				count = total - index;
			}
			T* dst = m_data + index;
			T* src = dst + count;
			sl_size n = total - index - count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
			setCount_NoLock(total - count);
			return count;
		}
		return 0;
	}
	
	template <class T>
	sl_size CList<T>::removeRange(sl_size index, sl_size count)
	{
		ObjectLocker lock(this);
		return removeRange_NoLock(index, count);
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool CList<T>::removeValue_NoLock(const _T& value, T* outValue, const EQUALS& equals)
	{
		if (m_data) {
			sl_size n = m_count;
			for (sl_size i = 0; i < n; i++) {
				if (equals(m_data[i], value)) {
					if (outValue) {
						*outValue = m_data[i];
					}
					for (sl_size j = i + 1; j < n; j++) {
						m_data[j - 1] = m_data[j];
					}
					setCount_NoLock(n - 1);
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool CList<T>::removeValue(const _T& value, T* outValue, const EQUALS& equals)
	{
		ObjectLocker lock(this);
		return removeValue_NoLock(value, outValue, equals);
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_size CList<T>::removeElementsByValue_NoLock(const _T& value, List<T>* outValues, const EQUALS& equals)
	{
		if (m_data) {
			sl_size n = m_count;
			for (sl_size i = 0; i < n;) {
				if (equals(m_data[i], value)) {
					if (outValues) {
						outValues->add_NoLock(m_data[i]);
					}
					for (sl_size j = i + 1; j < n; j++) {
						m_data[j - 1] = m_data[j];
					}
					n--;
				} else {
					i++;
				}
			}
			if (n < m_count) {
				setCount_NoLock(n);
				return m_count - n;
			}
		}
		return 0;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_size CList<T>::removeElementsByValue(const _T& value, List<T>* outValues, const EQUALS& equals)
	{
		ObjectLocker lock(this);
		return removeElementsByValue_NoLock(value, outValues, equals);
	}
	
	template <class T>
	sl_size CList<T>::removeAll_NoLock()
	{
		sl_size count = m_count;
		setCount_NoLock(0);
		return count;
	}
	
	template <class T>
	sl_size CList<T>::removeAll()
	{
		ObjectLocker lock(this);
		return removeAll_NoLock();
	}
	
	template <class T>
	sl_bool CList<T>::popFront_NoLock(T* _out)
	{
		sl_size n = m_count;
		if (n > 0) {
			T* dst = m_data;
			T* src = dst + 1;
			if (_out) {
				*_out = *dst;
			}
			sl_size m = n - 1;
			while (m) {
				*(dst++) = *(src++);
				m--;
			}
			return setCount_NoLock(n - 1);
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::popFront(T* _out)
	{
		ObjectLocker lock(this);
		return popFront_NoLock(_out);
	}
	
	template <class T>
	sl_size CList<T>::popFrontElements_NoLock(sl_size count)
	{
		sl_size n = m_count;
		if (n > 0 && count > 0) {
			if (count > n) {
				count = n;
			}
			T* dst = m_data;
			T* src = dst + count;
			sl_size m = n - count;
			while (m) {
				*(dst++) = *(src++);
				m--;
			}
			if (setCount_NoLock(n - count)) {
				return count;
			}
		}
		return 0;
	}
	
	template <class T>
	sl_size CList<T>::popFrontElements(sl_size count)
	{
		ObjectLocker lock(this);
		return popFrontElements_NoLock(count);
	}
	
	template <class T>
	sl_bool CList<T>::popBack_NoLock(T* _out)
	{
		sl_size n = m_count;
		if (n > 0) {
			if (_out) {
				*_out = m_data[n - 1];
			}
			return setCount_NoLock(n - 1);
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::popBack(T* _out)
	{
		ObjectLocker lock(this);
		return popBack_NoLock(_out);
	}
	
	template <class T>
	sl_size CList<T>::popBackElements_NoLock(sl_size count)
	{
		sl_size n = m_count;
		if (n > 0 && count > 0) {
			if (count > n) {
				count = n;
			}
			if (setCount_NoLock(n - count)) {
				return count;
			}
		}
		return 0;
	}
	
	template <class T>
	sl_size CList<T>::popBackElements(sl_size count)
	{
		ObjectLocker lock(this);
		return popBackElements_NoLock(count);
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_reg CList<T>::indexOf_NoLock(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		sl_reg ret = -1;
		if (start < 0) {
			start = 0;
		}
		for (sl_size i = start; i < m_count; i++) {
			if (equals(m_data[i], value)) {
				ret = i;
				break;
			}
		}
		return ret;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_reg CList<T>::indexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		ObjectLocker lock(this);
		return indexOf_NoLock(value, start, equals);
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_reg CList<T>::lastIndexOf_NoLock(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		sl_reg ret = -1;
		if (start < 0 || start >= (sl_reg)m_count) {
			start = m_count - 1;
		}
		for (sl_reg i = start; i >= 0; i--) {
			if (equals(m_data[i], value)) {
				ret = i;
				break;
			}
		}
		return ret;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_reg CList<T>::lastIndexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		ObjectLocker lock(this);
		return lastIndexOf_NoLock(value, start, equals);
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool CList<T>::contains_NoLock(const _T& value, const EQUALS& equals) const
	{
		return indexOf_NoLock(value, 0, equals) >= 0;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool CList<T>::contains(const _T& value, const EQUALS& equals) const
	{
		return indexOf(value, 0, equals) >= 0;
	}
	
	template <class T>
	CList<T>* CList<T>::duplicate_NoLock() const
	{
		if (m_count > 0) {
			return create(m_data, m_count);
		}
		return sl_null;
	}
	
	template <class T>
	CList<T>* CList<T>::duplicate() const
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	template <class T>
	Array<T> CList<T>::toArray_NoLock() const
	{
		return Array<T>::create(m_data, m_count);
	}
	
	template <class T>
	Array<T> CList<T>::toArray() const
	{
		ObjectLocker lock(this);
		return toArray_NoLock();
	}
	
	template <class T>
	template <class COMPARE>
	void CList<T>::sort_NoLock(sl_bool flagAscending, const COMPARE& compare) const
	{
		QuickSort::sort(m_data, m_count, flagAscending, compare);
	}
	
	template <class T>
	template <class COMPARE>
	void CList<T>::sort(sl_bool flagAscending, const COMPARE& compare) const
	{
		ObjectLocker lock(this);
		QuickSort::sort(m_data, m_count, flagAscending, compare);
	}
	
	template <class T>
	Iterator<T> CList<T>::toIterator() const
	{
		return new ListIterator<T>(this, sl_null);
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::begin()
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE T const* CList<T>::begin() const
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::end()
	{
		return m_data + m_count;
	}
	
	template <class T>
	SLIB_INLINE T const* CList<T>::end() const
	{
		return m_data + m_count;
	}
	
	
	template <class T>
	SLIB_INLINE ListPosition<T>::ListPosition() {};
	
	template <class T>
	SLIB_INLINE ListPosition<T>::ListPosition(const Ref< CList<T> >& list) : ref(list)
	{
		if (list.isNotNull()) {
			data = list->getData();
			count = list->getCount();
		} else {
			data = sl_null;
			count = 0;
		}
	}
	
	template <class T>
	ListPosition<T>::ListPosition(const ListPosition<T>& other) = default;
	
	template <class T>
	ListPosition<T>::ListPosition(ListPosition<T>&& other) = default;
	
	template <class T>
	SLIB_INLINE T& ListPosition<T>::operator*()
	{
		return *data;
	}
	
	template <class T>
	SLIB_INLINE sl_bool ListPosition<T>::operator!=(const ListPosition<T>& other)
	{
		return count > 0;
	}
	
	template <class T>
	SLIB_INLINE ListPosition<T>& ListPosition<T>::operator++()
	{
		data++;
		count--;
		return *this;
	}
	
	
	template <class T>
	List<T>::List(sl_size count): ref(CList<T>::create(count))
	{
	}
	
	template <class T>
	List<T>::List(sl_size count, sl_size capacity): ref(CList<T>::create(count, capacity))
	{
	}
	
	template <class T>
	List<T>::List(sl_size count, sl_size capacity, const T& initialValue): ref(CList<T>::create(count, capacity, initialValue))
	{
	}

	template <class T>
	template <class _T>
	List<T>::List(const _T* values, sl_size count): ref(CList<T>::create(values, count))
	{
	}

	template <class T>
	List<T> List<T>::create()
	{
		return CList<T>::create();
	}
	
	template <class T>
	List<T> List<T>::create(sl_size count)
	{
		return CList<T>::create(count);
	}
	
	template <class T>
	List<T> List<T>::create(sl_size count, sl_size capacity)
	{
		return CList<T>::create(count, capacity);
	}
	
	template <class T>
	List<T> List<T>::create(sl_size count, sl_size capacity, const T& initialValue)
	{
		return CList<T>::create(count, capacity, initialValue);
	}

	template <class T>
	template <class _T>
	List<T> List<T>::create(const _T* values, sl_size count)
	{
		return CList<T>::create(values, count);
	}
	
	template <class T>
	template <class _T>
	List<T> List<T>::create(const Array<_T>& array)
	{
		return create(array.getData(), array.getCount());
	}
	
	template <class T>
	List<T> List<T>::createFromElement(const T& e, sl_size count)
	{
		return CList<T>::createFromElement(e, count);
	}
	
	template <class T>
	template <class... ARGS>
	List<T> List<T>::createFromElements(ARGS&&... _values)
	{
		T values[] = {Forward<ARGS>(_values)...};
		return create(values, sizeof...(_values));
	}
	
	template <class T>
	template <class _T>
	List<T> List<T>::createCopy(const List<_T>& other)
	{
		return CList<T>::createCopy(other.ref._ptr);
	}
	
	template <class T>
	template <class _T>
	SLIB_INLINE const List<T>& List<T>::from(const List<_T>& other)
	{
		return *(reinterpret_cast<List<T> const*>(&other));
	}
	
	template <class T>
	SLIB_INLINE sl_size List<T>::getCount() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_size List<T>::getCapacity() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCapacity();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE T* List<T>::getData() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::isEmpty() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount() == 0;
		}
		return sl_true;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::isNotEmpty() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount() != 0;
		}
		return sl_false;
	}
	
	template <class T>
	T* List<T>::getPointerAt(sl_size index) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getPointerAt(index);
		}
		return sl_null;
	}
	
	template <class T>
	sl_bool List<T>::getAt_NoLock(sl_size index, T* _out) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt_NoLock(index, _out);
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::getAt(sl_size index, T* _out) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T List<T>::getValueAt_NoLock(sl_size index) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt_NoLock(index);
		} else {
			return T();
		}
	}

	template <class T>
	T List<T>::getValueAt(sl_size index) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	T List<T>::getValueAt_NoLock(sl_size index, const T& def) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt_NoLock(index, def);
		}
		return def;
	}

	template <class T>
	T List<T>::getValueAt(sl_size index, const T& def) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index, def);
		}
		return def;
	}
	
	template <class T>
	template <class _T>
	sl_bool List<T>::setAt_NoLock(sl_size index, _T&& value) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt_NoLock(index, Forward<_T>(value));
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool List<T>::setAt(sl_size index, _T&& value) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt(index, Forward<_T>(value));
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T& List<T>::operator[](sl_size_t index) const
	{
		return (ref->getData())[index];
	}

	template <class T>
	sl_bool List<T>::setCount_NoLock(sl_size count)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setCount_NoLock(count);
		} else {
			if (count == 0) {
				return sl_true;
			}
			obj = CList<T>::create(count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool List<T>::setCount(sl_size count)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setCount(count);
		} else {
			if (count == 0) {
				return sl_true;
			}
			obj = CList<T>::create(count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::setCapacity_NoLock(sl_size capacity)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setCapacity_NoLock(capacity);
		} else {
			if (capacity == 0) {
				return sl_true;
			}
			obj = CList<T>::create(0, capacity);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::setCapacity(sl_size capacity)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setCount(capacity);
		} else {
			if (capacity == 0) {
				return sl_true;
			}
			obj = CList<T>::create(0, capacity);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::shrinkToFit_NoLock() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->shrinkToFit_NoLock();
		}
	}
	
	template <class T>
	sl_bool List<T>::shrinkToFit() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->shrinkToFit();
		}
	}

	template <class T>
	template <class... ARGS>
	sl_bool List<T>::insert_NoLock(sl_size index, ARGS&&... args)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insert_NoLock(index, Forward<ARGS>(args)...);
		} else {
			obj = CList<T>::createFromElement(T(Forward<ARGS>(args)...));
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class... ARGS>
	sl_bool List<T>::insert(sl_size index, ARGS&&... args)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insert(index, Forward<ARGS>(args)...);
		} else {
			obj = CList<T>::createFromElement(T(Forward<ARGS>(args)...));
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::insertElements_NoLock(sl_size index, const _T* values, sl_size count)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertElements_NoLock(index, values, count);
		} else {
			obj = CList<T>::create(values, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::insertElements(sl_size index, const _T* values, sl_size count)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertElements(index, values, count);
		} else {
			obj = CList<T>::create(values, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::insertElements_NoLock(sl_size index, sl_size count, const T& value)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertElements_NoLock(index, count, value);
		} else {
			obj = CList<T>::createFromElement(value, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::insertElements(sl_size index, sl_size count, const T& value)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertElements(index, count, value);
		} else {
			obj = CList<T>::createFromElement(value, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool List<T>::insertAll_NoLock(sl_size index, const List<_T>& _other)
	{
		CList<_T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertAll_NoLock(index, other);
		} else {
			obj = CList<T>::create(other->getData(), other->getCount());
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool List<T>::insertAll_NoLock(sl_size index, const AtomicList<_T>& other)
	{
		return insertAll_NoLock(index, List<_T>(other));
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::insertAll(sl_size index, const List<_T>& _other)
	{
		CList<_T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertAll(index, other);
		} else {
			{
				ObjectLocker lock(other);
				obj = CList<T>::create(other->getData(), other->getCount());
			}
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::insertAll(sl_size index, const AtomicList<_T>& other)
	{
		return insertAll(index, List<_T>(other));
	}

	template <class T>
	template <class... ARGS>
	sl_bool List<T>::add_NoLock(ARGS&&... args)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->add_NoLock(Forward<ARGS>(args)...);
		} else {
			obj = CList<T>::createFromElement(T(Forward<ARGS>(args)...));
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class... ARGS>
	sl_bool List<T>::add(ARGS&&... args)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->add(Forward<ARGS>(args)...);
		} else {
			obj = CList<T>::createFromElement(T(Forward<ARGS>(args)...));
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addElements_NoLock(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addElements_NoLock(values, count);
		} else {
			obj = CList<T>::create(values, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addElements(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addElements(values, count);
		} else {
			obj = CList<T>::create(values, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::addElements_NoLock(sl_size count, const T& value)
	{
		if (count == 0) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addElements_NoLock(count, value);
		} else {
			obj = CList<T>::createFromElement(value, count);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::addElements(sl_size count, const T& value)
	{
		if (count == 0) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addElements(count, value);
		} else {
			obj = CList<T>::create(count, value);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addAll_NoLock(const List<_T>& _other)
	{
		CList<_T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addAll_NoLock(other);
		} else {
			obj = CList<T>::create(other->getData(), other->getCount());
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addAll_NoLock(const AtomicList<_T>& other)
	{
		return addAll_NoLock(List<_T>(other));
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addAll(const List<_T>& _other)
	{
		CList<_T>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addAll(other);
		} else {
			{
				ObjectLocker lock(other);
				obj = CList<T>::create(other->getData(), other->getCount());
			}
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addAll(const AtomicList<_T>& other)
	{
		return addAll(List<_T>(other));
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool List<T>::addIfNotExist_NoLock(_T&& value, const EQUALS& equals)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addIfNotExist_NoLock(Forward<_T>(value), equals);
		} else {
			obj = CList<T>::createFromElement(value);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool List<T>::addIfNotExist(_T&& value, const EQUALS& equals)
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addIfNotExist(Forward<_T>(value), equals);
		} else {
			obj = CList<T>::createFromElement(value);
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addAll_NoLock(const Iterator<_T>& iterator)
	{
		if (iterator.isNull()) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addAll_NoLock(iterator);
		} else {
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				return obj->addAll_NoLock(iterator);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool List<T>::addAll(const Iterator<_T>& iterator)
	{
		if (iterator.isNull()) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addAll(iterator);
		} else {
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				return obj->addAll_NoLock(iterator);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool List<T>::removeAt_NoLock(sl_size index, T* outValue) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAt_NoLock(index, outValue);
		}
		return sl_false;
	}

	template <class T>
	sl_bool List<T>::removeAt(sl_size index, T* outValue) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAt(index, outValue);
		}
		return 0;
	}

	template <class T>
	sl_size List<T>::removeRange_NoLock(sl_size index, sl_size count) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeRange_NoLock(index, count);
		}
		return 0;
	}

	template <class T>
	sl_size List<T>::removeRange(sl_size index, sl_size count) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeRange(index, count);
		}
		return 0;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool List<T>::removeValue_NoLock(const _T& value, T* outValue, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeValue_NoLock(value, outValue, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool List<T>::removeValue(const _T& value, T* outValue, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeValue(value, outValue, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_size List<T>::removeElementsByValue_NoLock(const _T& value, List<T>* outValues, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeElementsByValue_NoLock(value, outValues, equals);
		}
		return 0;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_size List<T>::removeElementsByValue(const _T& value, List<T>* outValues, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeElementsByValue(value, outValues, equals);
		}
		return 0;
	}

	template <class T>
	sl_size List<T>::removeAll_NoLock() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	template <class T>
	sl_size List<T>::removeAll() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class T>
	sl_bool List<T>::popFront_NoLock(T* _out) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFront_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool List<T>::popFront(T* _out) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size List<T>::popFrontElements_NoLock(sl_size count) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFrontElements_NoLock(count);
		}
		return 0;
	}

	template <class T>
	sl_size List<T>::popFrontElements(sl_size count) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFrontElements(count);
		}
		return 0;
	}
	
	template <class T>
	sl_bool List<T>::popBack_NoLock(T* _out) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBack_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool List<T>::popBack(T* _out) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBack(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size List<T>::popBackElements_NoLock(sl_size count) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBackElements_NoLock(count);
		}
		return 0;
	}

	template <class T>
	sl_size List<T>::popBackElements(sl_size count) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBackElements(count);
		}
		return 0;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg List<T>::indexOf_NoLock(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf_NoLock(value, start, equals);
		}
		return -1;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_reg List<T>::indexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg List<T>::lastIndexOf_NoLock(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf_NoLock(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg List<T>::lastIndexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf(value, start, equals);
		}
		return -1;
	}
	
	template <class T>
	template <class _T, class EQUALS>
	sl_bool List<T>::contains_NoLock(const _T& value, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->contains_NoLock(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool List<T>::contains(const _T& value, const EQUALS& equals) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->contains(value, equals);
		}
		return sl_false;
	}

	template <class T>
	List<T> List<T>::duplicate_NoLock() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}

	template <class T>
	List<T> List<T>::duplicate() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	Array<T> List<T>::toArray_NoLock() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toArray_NoLock();
		}
		return sl_null;
	}

	template <class T>
	Array<T> List<T>::toArray() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toArray();
		}
		return sl_null;
	}

	template <class T>
	template <class COMPARE>
	void List<T>::sort_NoLock(sl_bool flagAscending, const COMPARE& compare) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->sort_NoLock(flagAscending, compare);
		}
	}

	template <class T>
	template <class COMPARE>
	void List<T>::sort(sl_bool flagAscending, const COMPARE& compare) const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->sort(flagAscending, compare);
		}
	}

	template <class T>
	Iterator<T> List<T>::toIterator() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return new ListIterator<T>(obj, obj);
		}
		return sl_null;
	}
	
	template <class T>
	T* List<T>::begin() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->begin();
		}
		return sl_null;
	}

	template <class T>
	T* List<T>::end() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->end();
		}
		return sl_null;
	}

	template <class T>
	const Mutex* List<T>::getLocker() const
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLocker();
		} else {
			return sl_null;
		}
	}


	template <class T>
	Atomic< List<T> >::Atomic(sl_size count): ref(CList<T>::create(count))
	{
	}
	
	template <class T>
	Atomic< List<T> >::Atomic(sl_size count, sl_size capacity): ref(CList<T>::create(count, capacity))
	{
	}
	
	template <class T>
	Atomic< List<T> >::Atomic(sl_size count, sl_size capacity, const T& initialValue): ref(CList<T>::create(count, capacity, initialValue))
	{
	}

	template <class T>
	template <class _T>
	Atomic< List<T> >::Atomic(const _T* values, sl_size count): ref(CList<T>::create(values, count))
	{
	}
	
	template <class T>
	template <class _T>
	const Atomic< List<T> >& Atomic< List<T> >::from(const Atomic< List<_T> >& other)
	{
		return *(reinterpret_cast<Atomic< List<T> > const*>(&other));
	}

	template <class T>
	sl_size Atomic< List<T> >::getCount() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< List<T> >::isEmpty() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount() == 0;
		}
		return sl_true;
	}

	template <class T>
	sl_bool Atomic< List<T> >::isNotEmpty() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount() != 0;
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< List<T> >::getAt(sl_size index, T* _out) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< List<T> >::getValueAt(sl_size index) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	T Atomic< List<T> >::getValueAt(sl_size index, const T& def) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index, def);
		}
		return def;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::setAt(sl_size index, _T&& value) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setAt(index, Forward<_T>(value));
		}
		return sl_false;
	}

	template <class T>
	T Atomic< List<T> >::operator[](sl_size_t index) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	sl_bool Atomic< List<T> >::setCount(sl_size count)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setCount(count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->setCount(count);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->setCount(count);
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::setCapacity(sl_size capacity)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setCapacity(capacity);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->setCapacity(capacity);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->setCapacity(capacity);
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::shrinkToFit() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->shrinkToFit();
		}
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool Atomic< List<T> >::insert(sl_size index, ARGS&&... args)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->insert(index, Forward<ARGS>(args)...);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->insert(index, Forward<ARGS>(args)...);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->insert(index, Forward<ARGS>(args)...);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::insertElements(sl_size index, const _T* values, sl_size count)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->insertElements(index, values, count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->insertElements(index, values, count);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->insertElements(index, values, count);
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::insertElements(sl_size index, sl_size count, const T& value)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->insertElements(index, count, value);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->insertElements(index, count, value);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->insertElements(index, count, value);
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::insertAll(sl_size index, const List<_T>& _other)
	{
		CList<_T>* other = _other.object;
		if (!other) {
			return sl_true;
		}
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->insertAll(index, other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->insertAll(index, other);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->insertAll(index, other);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::insertAll(sl_size index, const AtomicList<_T>& other)
	{
		return insertAll(index, List<_T>(other));
	}

	template <class T>
	template <class... ARGS>
	sl_bool Atomic< List<T> >::add(ARGS&&... args)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->add(Forward<ARGS>(args)...);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->add(Forward<ARGS>(args)...);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->add(Forward<ARGS>(args)...);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::addElements(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->addElements(values, count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addElements(values, count);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addElements(values, count);
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::addElements(sl_size count, const T& value)
	{
		if (count == 0) {
			return sl_true;
		}
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->addElements(count, value);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addElements(count, value);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addElements(count, value);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::addAll(const List<_T>& _other)
	{
		CList<_T>* other = _other.object;
		if (!other) {
			return sl_true;
		}
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->addAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addAll(other);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addAll(other);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::addAll(const AtomicList<_T>& other)
	{
		return addAll(List<_T>(other));
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool Atomic< List<T> >::addIfNotExist(_T&& value, const EQUALS& equals)
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->addIfNotExist(Forward<_T>(value), equals);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addIfNotExist(Forward<_T>(value), equals);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addIfNotExist(Forward<_T>(value), equals);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class _T>
	sl_bool Atomic< List<T> >::addAll(const Iterator<_T>& iterator)
	{
		if (iterator.isNull()) {
			return sl_true;
		}
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->addAll(iterator);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addAll(iterator);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addAll(iterator);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< List<T> >::removeAt(sl_size index, T* outValue) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAt(index, outValue);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< List<T> >::removeRange(sl_size index, sl_size count) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeRange(index, count);
		}
		return 0;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool Atomic< List<T> >::removeValue(const _T& value, T* outValue, const EQUALS& equals) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeValue(value, outValue, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_size Atomic< List<T> >::removeElementsByValue(const _T& value, List<T>* outValues, const EQUALS& equals) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeElementsByValue(value, outValues, equals);
		}
		return 0;
	}

	template <class T>
	sl_size Atomic< List<T> >::removeAll() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< List<T> >::popFront(T* _out) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< List<T> >::popFrontElements(sl_size count) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popFrontElements(count);
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< List<T> >::popBack(T* _out) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popBack(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< List<T> >::popBackElements(sl_size count) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popBackElements(count);
		}
		return 0;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg Atomic< List<T> >::indexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->indexOf(value, start);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg Atomic< List<T> >::lastIndexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->lastIndexOf(value, start);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool Atomic< List<T> >::contains(const _T& value, const EQUALS& equals) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->contains(value);
		}
		return sl_false;
	}

	template <class T>
	List<T> Atomic< List<T> >::duplicate() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	Array<T> Atomic< List<T> >::toArray() const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->toArray();
		}
		return sl_null;
	}

	template <class T>
	template <class COMPARE>
	void Atomic< List<T> >::sort(sl_bool flagAscending, const COMPARE& compare) const
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->sortBy(flagAscending, compare);
		}
	}

	template <class T>
	SLIB_INLINE ListPosition<T> Atomic< List<T> >::begin() const
	{
		return ListPosition<T>(ref);
	}

	template <class T>
	SLIB_INLINE ListPosition<T> Atomic< List<T> >::end() const
	{
		return ListPosition<T>();
	}


	template <class T>
	ListLocker<T>::ListLocker(const List<T>& list) : ObjectLocker(list.ref._ptr)
	{
		m_list = list;
		data = list.getData();
		count = list.getCount();
	}

	template <class T>
	ListLocker<T>::ListLocker(const CList<T>& list) : ObjectLocker(&list)
	{
		data = list.getData();
		count = list.getCount();
	}

	template <class T>
	ListLocker<T>::~ListLocker()
	{
		unlock();
	}

	template <class T>
	SLIB_INLINE T& ListLocker<T>::operator[](sl_reg index)
	{
		return data[index];
	}

	template <class T>
	SLIB_INLINE T* ListLocker<T>::begin()
	{
		return data;
	}

	template <class T>
	SLIB_INLINE T* ListLocker<T>::end()
	{
		return data + count;
	}

	template <class T>
	ListElements<T>::ListElements(const List<T>& list)
	{
		m_list = list;
		data = list.getData();
		count = list.getCount();
	}

	template <class T>
	ListElements<T>::ListElements(const CList<T>& list)
	{
		data = list.getData();
		count = list.getCount();
	}

	template <class T>
	SLIB_INLINE T& ListElements<T>::operator[](sl_reg index)
	{
		return data[index];
	}

	template <class T>
	SLIB_INLINE T* ListElements<T>::begin()
	{
		return data;
	}

	template <class T>
	SLIB_INLINE T* ListElements<T>::end()
	{
		return data + count;
	}


	template <class T>
	ListIterator<T>::ListIterator(const CList<T>* list, Referable* refer)
	{
		m_index = 0;
		m_list = list;
		m_refer = refer;
	}

	template <class T>
	sl_bool ListIterator<T>::hasNext()
	{
		return (m_index < m_list->getCount());
	}

	template <class T>
	sl_bool ListIterator<T>::next(T* _out)
	{
		if (m_list->getAt(m_index, _out)) {
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_reg ListIterator<T>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}

}
