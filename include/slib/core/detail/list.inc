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
	
	namespace priv
	{
		namespace list
		{

			sl_bool setCapacity(void* pData, sl_size elementSize, sl_size* pCapacity, sl_size* pCount, sl_size newCapacity) noexcept;
			
			sl_bool adjustCapacity(void* pData, sl_size elementSize, sl_size* pCapacity, sl_size* pCount, sl_size newCount) noexcept;		

			sl_bool growCapacity(void* pData, sl_size elementSize, sl_size* pCapacity, sl_size newCount) noexcept;		

			sl_bool shrinkCapacity(void* pData, sl_size elementSize, sl_size* pCapacity, sl_size newCount) noexcept;		

		}
	}

	template <class T>
	SLIB_INLINE CList<T>::CList() noexcept
	 : m_data(sl_null), m_count(0), m_capacity(0)
	 {}
	
	template <class T>
	CList<T>::CList(sl_size count) noexcept
	{
		if (count > 0) {
			T* data = (T*)(Base::createMemory(count * sizeof(T)));
			if (data) {
				ArrayTraits<T>::construct(data, count);
				m_data = data;
				m_count = count;
				m_capacity = count;
				return;
			}
		}
		m_data = sl_null;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::CList(sl_size count, sl_size capacity) noexcept
	{
		if (capacity < count) {
			capacity = count;
		}
		if (capacity > 0) {
			T* data = (T*)(Base::createMemory(capacity * sizeof(T)));
			if (data) {
				ArrayTraits<T>::construct(data, count);
				m_data = data;
				m_count = count;
				m_capacity = capacity;
				return;
			}
		}
		m_data = sl_null;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::CList(sl_size count, sl_size capacity, const T& initialValue) noexcept
	{
		if (capacity < count) {
			capacity = count;
		}
		if (capacity > 0) {
			T* data = (T*)(Base::createMemory(capacity * sizeof(T)));
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
		m_data = sl_null;
		m_count = 0;
		m_capacity = 0;
	}

	template <class T>
	template <class VALUE>
	CList<T>::CList(const VALUE* values, sl_size count) noexcept
	{
		if (count > 0) {
			T* data = (T*)(Base::createMemory(count * sizeof(T)));
			if (data) {
				ArrayTraits<T>::copy_construct(data, values, count);
				m_data = data;
				m_count = count;
				m_capacity = count;
				return;
			}
		}
		m_data = sl_null;
		m_count = 0;
		m_capacity = 0;
	}
	
	template <class T>
	CList<T>::~CList() noexcept
	{
		T* data = m_data;
		if (data) {
			ArrayTraits<T>::free(data, m_count);
			Base::freeMemory((void*)data);
		}
	}
	
	template <class T>
	SLIB_INLINE CList<T>::CList(CList<T>&& other) noexcept
	{
		m_data = other.m_data;
		m_count = other.m_count;
		m_capacity = other.m_capacity;
		other.m_data = sl_null;
		other.m_count = 0;
		other.m_capacity = 0;
	}
	
	template <class T>
	CList<T>& CList<T>::operator=(CList<T>&& other) noexcept
	{
		T* data = m_data;
		if (data) {
			ArrayTraits<T>::free(data, m_count);
			Base::freeMemory((void*)data);
		}
		m_data = other.m_data;
		m_count = other.m_count;
		m_capacity = other.m_capacity;
		other.m_data = sl_null;
		other.m_count = 0;
		other.m_capacity = 0;
		return *this;
	}

	
	template <class T>
	SLIB_INLINE CList<T>* CList<T>::create() noexcept
	{
		return new CList<T>;
	}
	
	template <class T>
	CList<T>* CList<T>::create(sl_size count) noexcept
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
	CList<T>* CList<T>::create(sl_size count, sl_size capacity) noexcept
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
	CList<T>* CList<T>::create(sl_size count, sl_size capacity, const T& intialValue) noexcept
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
	template <class VALUE>
	CList<T>* CList<T>::create(const VALUE* values, sl_size count) noexcept
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
	template <class VALUE>
	CList<T>* CList<T>::create(const Array<VALUE>& array) noexcept
	{
		return create(array.getData(), array.getCount());
	}
	
	template <class T>
	template <class VALUE>
	CList<T>* CList<T>::createFromElement(VALUE&& value) noexcept
	{
		CList<T>* ret = new CList<T>();
		if (ret) {
			T* data = (T*)(Base::createMemory(sizeof(T)));
			if (data) {
				new (data) T(Forward<VALUE>(value));
				ret->m_data = data;
				ret->m_count = 1;
				ret->m_capacity = 1;
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class T>
	CList<T>* CList<T>::createFromElement(const T& value, sl_size count) noexcept
	{
		return create(count, count, value);
	}
	
	template <class T>
	template <class... ARGS>
	CList<T>* CList<T>::createFromElements(ARGS&&... _values) noexcept
	{
		T values[] = {Forward<ARGS>(_values)...};
		return create(values, sizeof...(_values));
	}

	template <class T>
	template <class VALUE>
	CList<T>* CList<T>::createCopy(CList<VALUE>* other) noexcept
	{
		if (other) {
			ObjectLocker lock(other);
			return create(other->getData(), other->getCount());
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_size CList<T>::getCount() const noexcept
	{
		return m_count;
	}
	
	template <class T>
	SLIB_INLINE sl_size CList<T>::getCapacity() const noexcept
	{
		return m_capacity;
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::getData() const noexcept
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::getPointerAt(sl_size index) const noexcept
	{
		if (index < m_count) {
			return m_data + index;
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::getAt_NoLock(sl_size index, T* _out) const noexcept
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
	sl_bool CList<T>::getAt(sl_size index, T* _out) const noexcept
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
	SLIB_INLINE T CList<T>::getValueAt_NoLock(sl_size index) const noexcept
	{
		if (index < m_count) {
			return m_data[index];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T CList<T>::getValueAt(sl_size index) const noexcept
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			return m_data[index];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T CList<T>::getValueAt_NoLock(sl_size index, const T& def) const noexcept
	{
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}
	
	template <class T>
	T CList<T>::getValueAt(sl_size index, const T& def) const noexcept
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::getFirst_NoLock(T* _out) const noexcept
	{
		if (m_count) {
			if (_out) {
				*_out = m_data[0];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::getFirst(T* _out) const noexcept
	{
		ObjectLocker lock(this);
		if (m_count) {
			if (_out) {
				*_out = m_data[0];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T CList<T>::getFirstValue_NoLock() const noexcept
	{
		if (m_count) {
			return m_data[0];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T CList<T>::getFirstValue() const noexcept
	{
		ObjectLocker lock(this);
		if (m_count) {
			return m_data[0];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T CList<T>::getFirstValue_NoLock(const T& def) const noexcept
	{
		if (m_count) {
			return m_data[0];
		}
		return def;
	}
	
	template <class T>
	T CList<T>::getFirstValue(const T& def) const noexcept
	{
		ObjectLocker lock(this);
		if (m_count) {
			return m_data[0];
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::getLast_NoLock(T* _out) const noexcept
	{
		sl_size count = m_count;
		if (count) {
			if (_out) {
				*_out = m_data[count - 1];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::getLast(T* _out) const noexcept
	{
		ObjectLocker lock(this);
		sl_size count = m_count;
		if (count) {
			if (_out) {
				*_out = m_data[count - 1];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T CList<T>::getLastValue_NoLock() const noexcept
	{
		sl_size count = m_count;
		if (count) {
			return m_data[count - 1];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T CList<T>::getLastValue() const noexcept
	{
		ObjectLocker lock(this);
		sl_size count = m_count;
		if (count) {
			return m_data[count - 1];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T CList<T>::getLastValue_NoLock(const T& def) const noexcept
	{
		sl_size count = m_count;
		if (count) {
			return m_data[count - 1];
		}
		return def;
	}
	
	template <class T>
	T CList<T>::getLastValue(const T& def) const noexcept
	{
		ObjectLocker lock(this);
		sl_size count = m_count;
		if (count) {
			return m_data[count - 1];
		}
		return def;
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE sl_bool CList<T>::setAt_NoLock(sl_size index, VALUE&& value) const noexcept
	{
		if (index < m_count) {
			m_data[index] = Forward<VALUE>(value);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CList<T>::setAt(sl_size index, VALUE&& value) const noexcept
	{
		ObjectLocker lock(this);
		if (index < m_count) {
			m_data[index] = Forward<VALUE>(value);
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T const& CList<T>::operator[](sl_size_t index) const noexcept
	{
		return m_data[index];
	}
	
	template <class T>
	SLIB_INLINE T& CList<T>::operator[](sl_size_t index) noexcept
	{
		return m_data[index];
	}
	
	template <class T>
	sl_bool CList<T>::setCount_NoLock(sl_size count) noexcept
	{
		sl_size oldCount = m_count;
		if (oldCount == count) {
			return sl_true;
		}
		if (count < oldCount) {
			ArrayTraits<T>::free(m_data + count, oldCount - count);
			m_count = count;
			adjustCapacity_NoLock(count);
			return sl_true;
		} else {
			if (adjustCapacity_NoLock(count)) {
				ArrayTraits<T>::construct(m_data + oldCount, count - oldCount);
				m_count = count;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::setCount(sl_size count) noexcept
	{
		ObjectLocker lock(this);
		return setCount_NoLock(count);
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::setCapacity_NoLock(sl_size capacity) noexcept
	{
		return priv::list::setCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, &m_count, capacity);
	}
	
	template <class T>
	sl_bool CList<T>::setCapacity(sl_size capacity) noexcept
	{
		ObjectLocker lock(this);
		return priv::list::setCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, &m_count, capacity);
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::adjustCapacity_NoLock(sl_size count) noexcept
	{
		return priv::list::adjustCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, &m_count, count);
	}
	
	template <class T>
	sl_bool CList<T>::adjustCapacity(sl_size count) noexcept
	{
		ObjectLocker lock(this);
		return priv::list::adjustCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, &m_count, count);
	}

	template <class T>
	SLIB_INLINE sl_bool CList<T>::growCapacity_NoLock(sl_size newCount) noexcept
	{
		return priv::list::growCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, newCount);
	}

	template <class T>
	SLIB_INLINE sl_bool CList<T>::growCapacity(sl_size newCount) noexcept
	{
		ObjectLocker lock(this);
		return priv::list::growCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, newCount);
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::shrinkCapacity_NoLock() noexcept
	{
		return priv::list::shrinkCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, m_count);
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::shrinkCapacity() noexcept
	{
		ObjectLocker lock(this);
		return priv::list::shrinkCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, m_count);
	}
	
	template <class T>
	SLIB_INLINE sl_bool CList<T>::shrinkToFit_NoLock() noexcept
	{
		return priv::list::setCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, &m_count, m_count);
	}
	
	template <class T>
	sl_bool CList<T>::shrinkToFit() noexcept
	{
		ObjectLocker lock(this);
		return priv::list::setCapacity(reinterpret_cast<void*>(&m_data), sizeof(T), &m_capacity, &m_count, m_count);
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::insert_NoLock(sl_size index, ARGS&&... args) noexcept
	{
		sl_size oldCount = m_count;
		if (index > oldCount) {
			index = oldCount;
		}
		sl_size newCount = oldCount + 1;
		if (growCapacity_NoLock(newCount)) {
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
	sl_bool CList<T>::insert(sl_size index, ARGS&&... args) noexcept
	{
		ObjectLocker lock(this);
		return insert_NoLock(index, Forward<ARGS>(args)...);
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CList<T>::insertElements_NoLock(sl_size index, const VALUE* values, sl_size nValues) noexcept
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		if (index > oldCount) {
			index = oldCount;
		}
		sl_size newCount = oldCount + nValues;
		if (growCapacity_NoLock(newCount)) {
			T* data = m_data + index;
			if (index < oldCount) {
				Base::moveMemory(data + nValues, data, (oldCount - index) * sizeof(T));
			}
			ArrayTraits<T>::copy_construct(data, values, nValues);
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CList<T>::insertElements(sl_size index, const VALUE* values, sl_size count) noexcept
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return insertElements_NoLock(index, values, count);
	}
	
	template <class T>
	sl_bool CList<T>::insertElements_NoLock(sl_size index, sl_size nValues, const T& value) noexcept
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		if (index > oldCount) {
			index = oldCount;
		}
		sl_size newCount = oldCount + nValues;
		if (growCapacity_NoLock(newCount)) {
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
	sl_bool CList<T>::insertElements(sl_size index, sl_size count, const T& value) noexcept
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return insertElements_NoLock(index, count, value);
	}

	template <class T>
	template <class VALUE>
	sl_bool CList<T>::insertAll_NoLock(sl_size index, const CList<VALUE>* other) noexcept
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
	template <class VALUE>
	sl_bool CList<T>::insertAll(sl_size index, const CList<VALUE>* other) noexcept
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		MultipleObjectsLocker lock(this, other);
		return insertElements_NoLock(index, other->getData(), other->getCount());
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::add_NoLock(ARGS&&... args) noexcept
	{
		sl_size oldCount = m_count;
		sl_size newCount = oldCount + 1;
		if (growCapacity_NoLock(newCount)) {
			new (m_data + oldCount) T(Forward<ARGS>(args)...);
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool CList<T>::add(ARGS&&... args) noexcept
	{
		ObjectLocker lock(this);
		return add_NoLock(Forward<ARGS>(args)...);
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CList<T>::addElements_NoLock(const VALUE* values, sl_size nValues) noexcept
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		sl_size newCount = oldCount + nValues;
		if (growCapacity_NoLock(newCount)) {
			ArrayTraits<T>::copy_construct(m_data + oldCount, values, nValues);
			m_count = newCount;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CList<T>::addElements(const VALUE* values, sl_size count) noexcept
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return addElements_NoLock(values, count);
	}
	
	template <class T>
	sl_bool CList<T>::addElements_NoLock(sl_size nValues, const T& value) noexcept
	{
		if (nValues == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		sl_size newCount = oldCount + nValues;
		if (growCapacity_NoLock(newCount)) {
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
	sl_bool CList<T>::addElements(sl_size count, const T& value) noexcept
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return addElements_NoLock(count, value);
	}
	
	template <class T>
	template <class VALUE>
	sl_bool CList<T>::addAll_NoLock(const CList<VALUE>* other) noexcept
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
	template <class VALUE>
	sl_bool CList<T>::addAll(const CList<VALUE>* other) noexcept
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		MultipleObjectsLocker lock(this, other);
		return addElements_NoLock(other->getData(), other->getCount());
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CList<T>::addIfNotExist_NoLock(VALUE&& value, const EQUALS& equals) noexcept
	{
		if (indexOf_NoLock(value, equals) < 0) {
			return add_NoLock(value);
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CList<T>::addIfNotExist(VALUE&& value, const EQUALS& equals) noexcept
	{
		ObjectLocker lock(this);
		if (indexOf_NoLock(value, equals) < 0) {
			return add_NoLock(value);
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::removeAt_NoLock(sl_size index, T* outValue) noexcept
	{
		sl_size count = m_count;
		if (index < count) {
			T* m = m_data + index;
			if (outValue) {
				*outValue = Move(*m);
			}
			m->~T();
			if (index + 1 < count) {
				Base::moveMemory(m, m+1, (count - index - 1) * sizeof(T));
			}
			m_count = count - 1;
			shrinkCapacity_NoLock();
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::removeAt(sl_size index, T* outValue) noexcept
	{
		ObjectLocker lock(this);
		return removeAt_NoLock(index, outValue);
	}
	
	template <class T>
	sl_size CList<T>::removeRange_NoLock(sl_size index, sl_size nValues) noexcept
	{
		sl_size count = m_count;
		if (nValues > 0 && index < count) {
			if (nValues > count - index) {
				nValues = count - index;
			}
			T* dst = m_data + index;
			ArrayTraits<T>::free(dst, nValues);
			if (index + nValues < count) {
				Base::moveMemory(dst, dst + nValues, (count - index - nValues) * sizeof(T));
			}
			m_count = count - nValues;
			shrinkCapacity_NoLock();
			return nValues;
		}
		return 0;
	}
	
	template <class T>
	sl_size CList<T>::removeRange(sl_size index, sl_size count) noexcept
	{
		ObjectLocker lock(this);
		return removeRange_NoLock(index, count);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CList<T>::remove_NoLock(const VALUE& value, const EQUALS& equals) noexcept
	{
		if (m_data) {
			T* data = m_data;
			sl_size count = m_count;
			for (sl_size i = 0; i < count; i++) {
				if (equals(data[i], value)) {
					(data + i)->~T();
					if (i + 1 < count) {
						Base::moveMemory(data + i, data + i + 1, (count - i - 1) * sizeof(T));
					}
					m_count = count - 1;
					shrinkCapacity_NoLock();
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CList<T>::remove(const VALUE& value, const EQUALS& equals) noexcept
	{
		ObjectLocker lock(this);
		return remove_NoLock(value, equals);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_size CList<T>::removeValues_NoLock(const VALUE& value, const EQUALS& equals) noexcept
	{
		if (m_data) {
			T* data = m_data;
			sl_size count = m_count;
			sl_size t = 0;
			for (sl_size i = 0; i < count; i++) {
				if (!(equals(data[i], value))) {
					if (t != i) {
						data[t] = Move(data[i]);
					}
					t++;
				}
			}
			if (t < count) {
				setCount_NoLock(t);
				return count - t;
			}
		}
		return 0;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_size CList<T>::removeValues(const VALUE& value, const EQUALS& equals) noexcept
	{
		ObjectLocker lock(this);
		return removeValues_NoLock(value, equals);
	}
	
	template <class T>
	template <class PREDICATE>
	sl_bool CList<T>::removeIf_NoLock(const PREDICATE& p) noexcept
	{
		if (m_data) {
			T* data = m_data;
			sl_size count = m_count;
			for (sl_size i = 0; i < count; i++) {
				if (p(data[i])) {
					(data + i)->~T();
					if (i + 1 < count) {
						Base::moveMemory(data + i, data + i + 1, (count - i - 1) * sizeof(T));
					}
					m_count = count - 1;
					shrinkCapacity_NoLock();
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class PREDICATE>
	sl_bool CList<T>::removeIf(const PREDICATE& p) noexcept
	{
		ObjectLocker lock(this);
		return removeIf_NoLock(p);
	}
	
	template <class T>
	template <class PREDICATE>
	sl_size CList<T>::removeElementsIf_NoLock(const PREDICATE& p) noexcept
	{
		if (m_data) {
			T* data = m_data;
			sl_size count = m_count;
			sl_size t = 0;
			for (sl_size i = 0; i < count; i++) {
				if (!(p(data[i]))) {
					if (t != i) {
						data[t] = Move(data[i]);
					}
					t++;
				}
			}
			if (t < count) {
				setCount_NoLock(t);
				return count - t;
			}
		}
		return 0;
	}
	
	template <class T>
	template <class PREDICATE>
	sl_size CList<T>::removeElementsIf(const PREDICATE& p) noexcept
	{
		ObjectLocker lock(this);
		return removeElementsIf_NoLock(p);
	}

	template <class T>
	sl_size CList<T>::removeAll_NoLock() noexcept
	{
		T* data = m_data;
		sl_size count = m_count;
		if (data) {
			ArrayTraits<T>::free(data, count);
			Base::freeMemory((void*)data);
			m_data = sl_null;
		}
		m_count = 0;
		m_capacity = 0;
		return count;
	}
	
	template <class T>
	sl_size CList<T>::removeAll() noexcept
	{
		T* data;
		sl_size count;
		{
			ObjectLocker lock(this);
			data = m_data;
			count = m_count;
			m_data = sl_null;
			m_count = 0;
			m_capacity = 0;
		}
		if (data) {
			ArrayTraits<T>::free(data, count);
			Base::freeMemory((void*)data);
		}
		return count;
	}
	
	template <class T>
	sl_bool CList<T>::popFront_NoLock(T* _out) noexcept
	{
		sl_size count = m_count;
		if (count > 0) {
			T* data = m_data;
			if (_out) {
				*_out = Move(*data);
			}
			data->~T();
			count--;
			Base::moveMemory(data, data + 1, count * sizeof(T));
			m_count = count;
			shrinkCapacity_NoLock();
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::popFront(T* _out) noexcept
	{
		ObjectLocker lock(this);
		return popFront_NoLock(_out);
	}
	
	template <class T>
	sl_size CList<T>::popFrontElements_NoLock(sl_size nElements) noexcept
	{
		sl_size count = m_count;
		if (nElements > 0 && count > 0) {
			if (nElements > count) {
				nElements = count;
			}
			T* data = m_data;
			ArrayTraits<T>::free(data, nElements);
			count -= nElements;
			Base::moveMemory(data, data + nElements, count * sizeof(T));
			m_count = count;
			shrinkCapacity_NoLock();
			return nElements;
		}
		return 0;
	}
	
	template <class T>
	sl_size CList<T>::popFrontElements(sl_size count) noexcept
	{
		ObjectLocker lock(this);
		return popFrontElements_NoLock(count);
	}
	
	template <class T>
	sl_bool CList<T>::popBack_NoLock(T* _out) noexcept
	{
		sl_size count = m_count;
		if (count > 0) {
			T* data = m_data + count - 1;
			if (_out) {
				*_out = Move(*data);
			}
			data->~T();
			m_count = count - 1;
			shrinkCapacity_NoLock();
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool CList<T>::popBack(T* _out) noexcept
	{
		ObjectLocker lock(this);
		return popBack_NoLock(_out);
	}
	
	template <class T>
	sl_size CList<T>::popBackElements_NoLock(sl_size nElements) noexcept
	{
		sl_size count = m_count;
		if (nElements > 0 && count > 0) {
			if (nElements > count) {
				nElements = count;
			}
			ArrayTraits<T>::free(m_data + count - nElements, nElements);
			m_count = count - nElements;
			shrinkCapacity_NoLock();
			return nElements;
		}
		return 0;
	}
	
	template <class T>
	sl_size CList<T>::popBackElements(sl_size count) noexcept
	{
		ObjectLocker lock(this);
		return popBackElements_NoLock(count);
	}
	
	template <class T>
	template <class VALUE, class ARG>
	SLIB_INLINE sl_reg CList<T>::indexOf_NoLock(const VALUE& value, const ARG& arg) const noexcept
	{
		return ArrayTraits<T>::indexOf(m_data, m_count, value, arg);
	}
	
	template <class T>
	template <class VALUE, class ARG>
	sl_reg CList<T>::indexOf(const VALUE& value, const ARG& arg) const noexcept
	{
		ObjectLocker lock(this);
		return ArrayTraits<T>::indexOf(m_data, m_count, value, arg);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_reg CList<T>::indexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		return ArrayTraits<T>::indexOf(m_data, m_count, value, equals, startIndex);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg CList<T>::indexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		ObjectLocker lock(this);
		return ArrayTraits<T>::indexOf(m_data, m_count, value, equals, startIndex);
	}
	
	template <class T>
	template <class VALUE, class ARG>
	SLIB_INLINE sl_reg CList<T>::lastIndexOf_NoLock(const VALUE& value, const ARG& arg) const noexcept
	{
		return ArrayTraits<T>::lastIndexOf(m_data, m_count, value, arg);
	}
	
	template <class T>
	template <class VALUE, class ARG>
	sl_reg CList<T>::lastIndexOf(const VALUE& value, const ARG& arg) const noexcept
	{
		ObjectLocker lock(this);
		return ArrayTraits<T>::lastIndexOf(m_data, m_count, value, arg);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_reg CList<T>::lastIndexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		return ArrayTraits<T>::lastIndexOf(m_data, m_count, value, equals, startIndex);
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg CList<T>::lastIndexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		ObjectLocker lock(this);
		return ArrayTraits<T>::lastIndexOf(m_data, m_count, value, equals, startIndex);
	}

	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_bool CList<T>::contains_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		return ArrayTraits<T>::indexOf(m_data, m_count, value, equals) >= 0;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CList<T>::contains(const VALUE& value, const EQUALS& equals) const noexcept
	{
		ObjectLocker lock(this);
		return ArrayTraits<T>::indexOf(m_data, m_count, value, equals) >= 0;
	}
	
	template <class T>
	SLIB_INLINE CList<T>* CList<T>::duplicate_NoLock() const noexcept
	{
		if (m_count > 0) {
			return create(m_data, m_count);
		}
		return sl_null;
	}
	
	template <class T>
	CList<T>* CList<T>::duplicate() const noexcept
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	template <class T>
	SLIB_INLINE Array<T> CList<T>::toArray_NoLock() const noexcept
	{
		return Array<T>::create(m_data, m_count);
	}
	
	template <class T>
	Array<T> CList<T>::toArray() const noexcept
	{
		ObjectLocker lock(this);
		return toArray_NoLock();
	}
	
	template <class T>
	template <class COMPARE>
	SLIB_INLINE void CList<T>::sort_NoLock(const COMPARE& compare) const noexcept
	{
		QuickSort::sortAsc(m_data, m_count, compare);
	}
	
	template <class T>
	template <class COMPARE>
	void CList<T>::sort(const COMPARE& compare) const noexcept
	{
		ObjectLocker lock(this);
		QuickSort::sortAsc(m_data, m_count, compare);
	}
	
	template <class T>
	template <class COMPARE>
	SLIB_INLINE void CList<T>::sortDesc_NoLock(const COMPARE& compare) const noexcept
	{
		QuickSort::sortDesc(m_data, m_count, compare);
	}
	
	template <class T>
	template <class COMPARE>
	void CList<T>::sortDesc(const COMPARE& compare) const noexcept
	{
		ObjectLocker lock(this);
		QuickSort::sortDesc(m_data, m_count, compare);
	}
	
	template <class T>
	SLIB_INLINE void CList<T>::reverse_NoLock() const noexcept
	{
		ArrayTraits<T>::reverse(m_data, m_count);
	}
	
	template <class T>
	void CList<T>::reverse() const noexcept
	{
		ObjectLocker lock(this);
		ArrayTraits<T>::reverse(m_data, m_count);
	}
	
	template <class T>
	CList<T>* CList<T>::slice_NoLock(sl_size index, sl_size count) const noexcept
	{
		if (count > 0 && index < m_count) {
			sl_size n = m_count - index;
			if (count > n) {
				count = n;
			}
			return create(m_data + index, count);
		}
		return sl_null;
	}
	
	template <class T>
	CList<T>* CList<T>::slice(sl_size index, sl_size count) const noexcept
	{
		ObjectLocker lock(this);
		return slice_NoLock(index, count);
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::begin() noexcept
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE T const* CList<T>::begin() const noexcept
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE T* CList<T>::end() noexcept
	{
		return m_data + m_count;
	}
	
	template <class T>
	SLIB_INLINE T const* CList<T>::end() const noexcept
	{
		return m_data + m_count;
	}
	
	
	template <class T>
	SLIB_INLINE List<T>::List(sl_size count) noexcept
	 : ref(CList<T>::create(count))
	{}
	
	template <class T>
	SLIB_INLINE List<T>::List(sl_size count, sl_size capacity) noexcept
	 : ref(CList<T>::create(count, capacity))
	{}
	
	template <class T>
	SLIB_INLINE List<T>::List(sl_size count, sl_size capacity, const T& initialValue) noexcept
	 : ref(CList<T>::create(count, capacity, initialValue))
	{}

	template <class T>
	template <class VALUE>
	SLIB_INLINE List<T>::List(const VALUE* values, sl_size count) noexcept
	 : ref(CList<T>::create(values, count))
	{}

	template <class T>
	SLIB_INLINE List<T> List<T>::create() noexcept
	{
		return CList<T>::create();
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::create(sl_size count) noexcept
	{
		return CList<T>::create(count);
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::create(sl_size count, sl_size capacity) noexcept
	{
		return CList<T>::create(count, capacity);
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::create(sl_size count, sl_size capacity, const T& initialValue) noexcept
	{
		return CList<T>::create(count, capacity, initialValue);
	}

	template <class T>
	template <class VALUE>
	SLIB_INLINE List<T> List<T>::create(const VALUE* values, sl_size count) noexcept
	{
		return CList<T>::create(values, count);
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE List<T> List<T>::create(const Array<VALUE>& array) noexcept
	{
		return CList<T>::create(array.getData(), array.getCount());
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE List<T> List<T>::createFromElement(VALUE&& e) noexcept
	{
		return CList<T>::createFromElement(Forward<VALUE>(e));
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::createFromElement(const T& e, sl_size count) noexcept
	{
		return CList<T>::createFromElement(e, count);
	}
	
	template <class T>
	template <class... ARGS>
	SLIB_INLINE List<T> List<T>::createFromElements(ARGS&&... args) noexcept
	{
		T values[] = {Forward<ARGS>(args)...};
		return CList<T>::create(values, sizeof...(args));
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE List<T> List<T>::createCopy(const List<VALUE>& other) noexcept
	{
		return CList<T>::createCopy(other.ref._ptr);
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE List<T>& List<T>::from(const List<VALUE>& other) noexcept
	{
		return *(const_cast<List<T>*>(reinterpret_cast<List<T> const*>(&other)));
	}
	
	template <class T>
	SLIB_INLINE sl_size List<T>::getCount() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_size List<T>::getCapacity() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCapacity();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE T* List<T>::getData() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::isEmpty() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount() == 0;
		}
		return sl_true;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::isNotEmpty() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount() != 0;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T* List<T>::getPointerAt(sl_size index) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getPointerAt(index);
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::getAt_NoLock(sl_size index, T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt_NoLock(index, _out);
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::getAt(sl_size index, T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE T List<T>::getValueAt_NoLock(sl_size index) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt_NoLock(index);
		} else {
			return NullValue<T>::get();
		}
	}

	template <class T>
	SLIB_INLINE T List<T>::getValueAt(sl_size index) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index);
		} else {
			return NullValue<T>::get();
		}
	}

	template <class T>
	SLIB_INLINE T List<T>::getValueAt_NoLock(sl_size index, const T& def) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt_NoLock(index, def);
		}
		return def;
	}

	template <class T>
	SLIB_INLINE T List<T>::getValueAt(sl_size index, const T& def) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index, def);
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::getFirst_NoLock(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFirst_NoLock(_out);
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::getFirst(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFirst(_out);
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getFirstValue_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFirstValue_NoLock();
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getFirstValue() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFirstValue();
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getFirstValue_NoLock(const T& def) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFirstValue_NoLock(def);
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getFirstValue(const T& def) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getFirstValue(def);
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::getLast_NoLock(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLast_NoLock(_out);
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::getLast(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLast(_out);
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getLastValue_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLastValue_NoLock();
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getLastValue() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLastValue();
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getLastValue_NoLock(const T& def) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLastValue_NoLock(def);
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE T List<T>::getLastValue(const T& def) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLastValue(def);
		}
		return def;
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE sl_bool List<T>::setAt_NoLock(sl_size index, VALUE&& value) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt_NoLock(index, Forward<VALUE>(value));
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	SLIB_INLINE sl_bool List<T>::setAt(sl_size index, VALUE&& value) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt(index, Forward<VALUE>(value));
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T& List<T>::operator[](sl_size_t index) const noexcept
	{
		return (ref->getData())[index];
	}

	template <class T>
	sl_bool List<T>::setCount_NoLock(sl_size count) noexcept
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
	sl_bool List<T>::setCount(sl_size count) noexcept
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
	sl_bool List<T>::setCapacity_NoLock(sl_size capacity) noexcept
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
	sl_bool List<T>::setCapacity(sl_size capacity) noexcept
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
	SLIB_INLINE sl_bool List<T>::shrinkToFit_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->shrinkToFit_NoLock();
		}
		return sl_true;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::shrinkToFit() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->shrinkToFit();
		}
		return sl_true;
	}

	template <class T>
	template <class... ARGS>
	sl_bool List<T>::insert_NoLock(sl_size index, ARGS&&... args) noexcept
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
	sl_bool List<T>::insert(sl_size index, ARGS&&... args) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insert(index, Forward<ARGS>(args)...);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->insert(index, Forward<ARGS>(args)...);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->insert(index, Forward<ARGS>(args)...);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	sl_bool List<T>::insertElements_NoLock(sl_size index, const VALUE* values, sl_size count) noexcept
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
	template <class VALUE>
	sl_bool List<T>::insertElements(sl_size index, const VALUE* values, sl_size count) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertElements(index, values, count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->insertElements(index, values, count);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->insertElements(index, values, count);
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::insertElements_NoLock(sl_size index, sl_size count, const T& value) noexcept
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
	sl_bool List<T>::insertElements(sl_size index, sl_size count, const T& value) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertElements(index, count, value);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->insertElements(index, count, value);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->insertElements(index, count, value);
			}
		}
		return sl_false;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool List<T>::insertAll_NoLock(sl_size index, const List<VALUE>& _other) noexcept
	{
		CList<VALUE>* other = _other.ref._ptr;
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
	template <class VALUE>
	SLIB_INLINE sl_bool List<T>::insertAll_NoLock(sl_size index, const AtomicList<VALUE>& other) noexcept
	{
		return insertAll_NoLock(index, List<VALUE>(other));
	}

	template <class T>
	template <class VALUE>
	sl_bool List<T>::insertAll(sl_size index, const List<VALUE>& _other) noexcept
	{
		CList<VALUE>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->insertAll(index, other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->insertAll(index, other);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->insertAll(index, other);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	SLIB_INLINE sl_bool List<T>::insertAll(sl_size index, const AtomicList<VALUE>& other) noexcept
	{
		return insertAll(index, List<VALUE>(other));
	}

	template <class T>
	template <class... ARGS>
	sl_bool List<T>::add_NoLock(ARGS&&... args) noexcept
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
	sl_bool List<T>::add(ARGS&&... args) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->add(Forward<ARGS>(args)...);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->add(Forward<ARGS>(args)...);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->add(Forward<ARGS>(args)...);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	sl_bool List<T>::addElements_NoLock(const VALUE* values, sl_size count) noexcept
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
	template <class VALUE>
	sl_bool List<T>::addElements(const VALUE* values, sl_size count) noexcept
	{
		if (count == 0) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addElements(values, count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->addElements(values, count);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->addElements(values, count);
			}
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool List<T>::addElements_NoLock(sl_size count, const T& value) noexcept
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
	sl_bool List<T>::addElements(sl_size count, const T& value) noexcept
	{
		if (count == 0) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addElements(count, value);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->addElements(count, value);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->addElements(count, value);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	sl_bool List<T>::addAll_NoLock(const List<VALUE>& _other) noexcept
	{
		CList<VALUE>* other = _other.ref._ptr;
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
	template <class VALUE>
	SLIB_INLINE sl_bool List<T>::addAll_NoLock(const AtomicList<VALUE>& other) noexcept
	{
		return addAll_NoLock(List<VALUE>(other));
	}

	template <class T>
	template <class VALUE>
	sl_bool List<T>::addAll(const List<VALUE>& _other) noexcept
	{
		CList<VALUE>* other = _other.ref._ptr;
		if (!other) {
			return sl_true;
		}
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addAll(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->addAll(other);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->addAll(other);
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE>
	SLIB_INLINE sl_bool List<T>::addAll(const AtomicList<VALUE>& other) noexcept
	{
		return addAll(List<VALUE>(other));
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool List<T>::addIfNotExist_NoLock(VALUE&& value, const EQUALS& equals) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addIfNotExist_NoLock(Forward<VALUE>(value), equals);
		} else {
			obj = CList<T>::createFromElement(Forward<VALUE>(value));
			if (obj) {
				ref = obj;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool List<T>::addIfNotExist(VALUE&& value, const EQUALS& equals) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->addIfNotExist(Forward<VALUE>(value), equals);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref._ptr;
			if (obj) {
				lock.unlock();
				return obj->addIfNotExist(Forward<VALUE>(value), equals);
			}
			obj = CList<T>::create();
			if (obj) {
				ref = obj;
				lock.unlock();
				return obj->addIfNotExist(Forward<VALUE>(value), equals);
			}
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_bool List<T>::removeAt_NoLock(sl_size index, T* outValue) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAt_NoLock(index, outValue);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_bool List<T>::removeAt(sl_size index, T* outValue) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAt(index, outValue);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::removeRange_NoLock(sl_size index, sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeRange_NoLock(index, count);
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_size List<T>::removeRange(sl_size index, sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeRange(index, count);
		}
		return 0;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_bool List<T>::remove_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->remove_NoLock(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_bool List<T>::remove(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->remove(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_size List<T>::removeValues_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeValues_NoLock(value, equals);
		}
		return 0;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_size List<T>::removeValues(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeValues(value, equals);
		}
		return 0;
	}
	
	template <class T>
	template <class PREDICATE>
	SLIB_INLINE sl_bool List<T>::removeIf_NoLock(const PREDICATE& p) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeIf_NoLock(p);
		}
		return sl_false;
	}
	
	template <class T>
	template <class PREDICATE>
	SLIB_INLINE sl_bool List<T>::removeIf(const PREDICATE& p) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeIf(p);
		}
		return sl_false;
	}
	
	template <class T>
	template <class PREDICATE>
	SLIB_INLINE sl_size List<T>::removeElementsIf_NoLock(const PREDICATE& p) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeElementsIf_NoLock(p);
		}
		return 0;
	}
	
	template <class T>
	template <class PREDICATE>
	SLIB_INLINE sl_size List<T>::removeElementsIf(const PREDICATE& p) noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeElementsIf(p);
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::removeAll_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::removeAll() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_bool List<T>::popFront_NoLock(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFront_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_bool List<T>::popFront(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::popFrontElements_NoLock(sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFrontElements_NoLock(count);
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::popFrontElements(sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popFrontElements(count);
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_bool List<T>::popBack_NoLock(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBack_NoLock(_out);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_bool List<T>::popBack(T* _out) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBack(_out);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::popBackElements_NoLock(sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBackElements_NoLock(count);
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_size List<T>::popBackElements(sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->popBackElements(count);
		}
		return 0;
	}
	
	template <class T>
	template <class VALUE, class ARG>
	SLIB_INLINE sl_reg List<T>::indexOf_NoLock(const VALUE& value, const ARG& arg) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf_NoLock(value, arg);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class ARG>
	SLIB_INLINE sl_reg List<T>::indexOf(const VALUE& value, const ARG& arg) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf(value, arg);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_reg List<T>::indexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf_NoLock(value, equals, startIndex);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_reg List<T>::indexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf(value, equals, startIndex);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class ARG>
	SLIB_INLINE sl_reg List<T>::lastIndexOf_NoLock(const VALUE& value, const ARG& arg) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf_NoLock(value, arg);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class ARG>
	SLIB_INLINE sl_reg List<T>::lastIndexOf(const VALUE& value, const ARG& arg) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf(value, arg);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_reg List<T>::lastIndexOf_NoLock(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf_NoLock(value, equals, startIndex);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_reg List<T>::lastIndexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf(value, equals, startIndex);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_bool List<T>::contains_NoLock(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->contains_NoLock(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	SLIB_INLINE sl_bool List<T>::contains(const VALUE& value, const EQUALS& equals) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->contains(value, equals);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE List<T> List<T>::duplicate_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::duplicate() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE Array<T> List<T>::toArray_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toArray_NoLock();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE Array<T> List<T>::toArray() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->toArray();
		}
		return sl_null;
	}

	template <class T>
	template <class COMPARE>
	SLIB_INLINE void List<T>::sort_NoLock(const COMPARE& compare) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->sort_NoLock(compare);
		}
	}

	template <class T>
	template <class COMPARE>
	SLIB_INLINE void List<T>::sort(const COMPARE& compare) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->sort(compare);
		}
	}
	
	template <class T>
	template <class COMPARE>
	SLIB_INLINE void List<T>::sortDesc_NoLock(const COMPARE& compare) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->sortDesc_NoLock(compare);
		}
	}
	
	template <class T>
	template <class COMPARE>
	SLIB_INLINE void List<T>::sortDesc(const COMPARE& compare) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->sortDesc(compare);
		}
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::slice_NoLock(sl_size index, sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->slice_NoLock(index, count);
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE List<T> List<T>::slice(sl_size index, sl_size count) const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->slice(index, count);
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE void List<T>::reverse_NoLock() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->reverse_NoLock();
		}
	}
	
	template <class T>
	SLIB_INLINE void List<T>::reverse() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			obj->reverse();
		}
	}
	
	template <class T>
	SLIB_INLINE const Mutex* List<T>::getLocker() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->getLocker();
		} else {
			return sl_null;
		}
	}

	template <class T>
	SLIB_INLINE T* List<T>::begin() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->begin();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE T* List<T>::end() const noexcept
	{
		CList<T>* obj = ref._ptr;
		if (obj) {
			return obj->end();
		}
		return sl_null;
	}


	template <class T>
	Atomic< List<T> >::Atomic(sl_size count) noexcept
	 : ref(CList<T>::create(count))
	{}
	
	template <class T>
	Atomic< List<T> >::Atomic(sl_size count, sl_size capacity) noexcept
	 : ref(CList<T>::create(count, capacity))
	{}
	
	template <class T>
	Atomic< List<T> >::Atomic(sl_size count, sl_size capacity, const T& initialValue) noexcept
	 : ref(CList<T>::create(count, capacity, initialValue))
	{}

	template <class T>
	template <class VALUE>
	Atomic< List<T> >::Atomic(const VALUE* values, sl_size count) noexcept
	 : ref(CList<T>::create(values, count))
	{}
	
	template <class T>
	template <class VALUE>
	Atomic< List<T> >& Atomic< List<T> >::from(const Atomic< List<VALUE> >& other) noexcept
	{
		return *(const_cast<Atomic< List<T> >*>(reinterpret_cast<Atomic< List<T> > const*>(&other)));
	}

	template <class T>
	sl_size Atomic< List<T> >::getCount() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< List<T> >::isEmpty() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount() == 0;
		}
		return sl_true;
	}

	template <class T>
	sl_bool Atomic< List<T> >::isNotEmpty() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount() != 0;
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< List<T> >::getAt(sl_size index, T* _out) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< List<T> >::getValueAt(sl_size index) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return NullValue<T>::get();
		}
	}

	template <class T>
	T Atomic< List<T> >::getValueAt(sl_size index, const T& def) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index, def);
		}
		return def;
	}

	template <class T>
	sl_bool Atomic< List<T> >::getFirst(T* _out) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getFirst(_out);
		}
		return sl_false;
	}
	
	template <class T>
	T Atomic< List<T> >::getFirstValue() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getFirstValue();
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T Atomic< List<T> >::getFirstValue(const T& def) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getFirstValue(def);
		}
		return def;
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::getLast(T* _out) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getLast(_out);
		}
		return sl_false;
	}
	
	template <class T>
	T Atomic< List<T> >::getLastValue() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getLastValue();
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T Atomic< List<T> >::getLastValue(const T& def) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getLastValue(def);
		}
		return def;
	}
	
	template <class T>
	template <class VALUE>
	sl_bool Atomic< List<T> >::setAt(sl_size index, VALUE&& value) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setAt(index, Forward<VALUE>(value));
		}
		return sl_false;
	}

	template <class T>
	T Atomic< List<T> >::operator[](sl_size_t index) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return NullValue<T>::get();
		}
	}

	template <class T>
	sl_bool Atomic< List<T> >::setCount(sl_size count) noexcept
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
	sl_bool Atomic< List<T> >::setCapacity(sl_size capacity) noexcept
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
	sl_bool Atomic< List<T> >::shrinkToFit() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->shrinkToFit();
		}
		return sl_true;
	}
	
	template <class T>
	template <class... ARGS>
	sl_bool Atomic< List<T> >::insert(sl_size index, ARGS&&... args) noexcept
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
	template <class VALUE>
	sl_bool Atomic< List<T> >::insertElements(sl_size index, const VALUE* values, sl_size count) noexcept
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
	sl_bool Atomic< List<T> >::insertElements(sl_size index, sl_size count, const T& value) noexcept
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
	template <class VALUE>
	sl_bool Atomic< List<T> >::insertAll(sl_size index, const List<VALUE>& _other) noexcept
	{
		CList<VALUE>* other = _other.ref._ptr;
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
	template <class VALUE>
	sl_bool Atomic< List<T> >::insertAll(sl_size index, const AtomicList<VALUE>& other) noexcept
	{
		return insertAll(index, List<VALUE>(other));
	}

	template <class T>
	template <class... ARGS>
	sl_bool Atomic< List<T> >::add(ARGS&&... args) noexcept
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
	template <class VALUE>
	sl_bool Atomic< List<T> >::addElements(const VALUE* values, sl_size count) noexcept
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
	sl_bool Atomic< List<T> >::addElements(sl_size count, const T& value) noexcept
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
	template <class VALUE>
	sl_bool Atomic< List<T> >::addAll(const List<VALUE>& _other) noexcept
	{
		CList<VALUE>* other = _other.ref._ptr;
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
	template <class VALUE>
	sl_bool Atomic< List<T> >::addAll(const AtomicList<VALUE>& other) noexcept
	{
		return addAll(List<VALUE>(other));
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Atomic< List<T> >::addIfNotExist(VALUE&& value, const EQUALS& equals) noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->addIfNotExist(Forward<VALUE>(value), equals);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addIfNotExist(Forward<VALUE>(value), equals);
			}
			obj = CList<T>::create();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->addIfNotExist(Forward<VALUE>(value), equals);
			}
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< List<T> >::removeAt(sl_size index, T* outValue) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAt(index, outValue);
		}
		return sl_false;
	}
	
	template <class T>
	sl_size Atomic< List<T> >::removeRange(sl_size index, sl_size count) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeRange(index, count);
		}
		return 0;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Atomic< List<T> >::remove(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->remove(value, equals);
		}
		return sl_false;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_size Atomic< List<T> >::removeValues(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeValues(value, equals);
		}
		return 0;
	}
	
	template <class T>
	template <class PREDICATE>
	sl_bool Atomic< List<T> >::removeIf(const PREDICATE& p) noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeIf(p);
		}
		return sl_false;
	}
	
	template <class T>
	template <class PREDICATE>
	sl_size Atomic< List<T> >::removeElementsIf(const PREDICATE& p) noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeElementsIf(p);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< List<T> >::removeAll() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< List<T> >::popFront(T* _out) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< List<T> >::popFrontElements(sl_size count) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popFrontElements(count);
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< List<T> >::popBack(T* _out) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popBack(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_size Atomic< List<T> >::popBackElements(sl_size count) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->popBackElements(count);
		}
		return 0;
	}

	template <class T>
	template <class VALUE, class ARG>
	sl_reg Atomic< List<T> >::indexOf(const VALUE& value, const ARG& arg) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->indexOf(value, arg);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg Atomic< List<T> >::indexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->indexOf(value, equals, startIndex);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class ARG>
	sl_reg Atomic< List<T> >::lastIndexOf(const VALUE& value, const ARG& arg) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->lastIndexOf(value, arg);
		}
		return -1;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg Atomic< List<T> >::lastIndexOf(const VALUE& value, const EQUALS& equals, sl_reg startIndex) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->lastIndexOf(value, equals, startIndex);
		}
		return -1;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Atomic< List<T> >::contains(const VALUE& value, const EQUALS& equals) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->contains(value);
		}
		return sl_false;
	}

	template <class T>
	List<T> Atomic< List<T> >::duplicate() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	Array<T> Atomic< List<T> >::toArray() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->toArray();
		}
		return sl_null;
	}

	template <class T>
	template <class COMPARE>
	void Atomic< List<T> >::sort(const COMPARE& compare) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->sort(compare);
		}
	}
	
	template <class T>
	template <class COMPARE>
	void Atomic< List<T> >::sortDesc(const COMPARE& compare) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->sortDesc(compare);
		}
	}
	
	template <class T>
	void Atomic< List<T> >::reverse() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->reverse();
		}
	}
	
	template <class T>
	List<T> Atomic< List<T> >::slice(sl_size index, sl_size count) const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->slice(index, count);
		}
		return sl_null;
	}

	template <class T>
	ArrayPosition<T> Atomic< List<T> >::begin() const noexcept
	{
		Ref< CList<T> > obj(ref);
		if (obj.isNotNull()) {
			return ArrayPosition<T>(obj->getData(), obj->getCount(), obj.get());
		}
		return ArrayPosition<T>();
	}

	template <class T>
	SLIB_INLINE ArrayPosition<T> Atomic< List<T> >::end() const noexcept
	{
		return ArrayPosition<T>();
	}

}
