/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <new>

namespace slib
{
	
	template <class T>
	SLIB_INLINE T& ArrayData<T>::operator[](sl_reg index) const noexcept
	{
		return data[index];
	}

	template <class T>
	SLIB_INLINE CArray<T>::CArray() noexcept
	 : m_flagStatic(sl_true), m_data(sl_null), m_count(0)
	 {}

	template <class T>
	CArray<T>::CArray(sl_size count) noexcept
	{
		if (count > 0) {
			T* dataNew = (T*)(Base::createMemory(count * sizeof(T)));
			if (dataNew) {
				for (sl_size i = 0; i < count; i++) {
					new (dataNew + i) T();
				}
				m_flagStatic = sl_false;
				m_data = dataNew;
				m_count = count;
				return;
			}
		}
		m_flagStatic = sl_true;
		m_data = sl_null;
		m_count = 0;
	}

	template <class T>
	template <class VALUE>
	CArray<T>::CArray(const VALUE* data, sl_size count) noexcept
	{
		if (count > 0) {
			T* dataNew = (T*)(Base::createMemory(count * sizeof(T)));
			if (dataNew) {
				for (sl_size i = 0; i < count; i++) {
					new (dataNew + i) T(data[i]);
				}
				m_flagStatic = sl_false;
				m_data = dataNew;
				m_count = count;
				return;
			}
		}
		m_flagStatic = sl_true;
		m_data = sl_null;
		m_count = 0;
	}

	template <class T>
	CArray<T>::CArray(const T* data, sl_size count, Referable* refer) noexcept
	{
		if (data && count > 0) {
			m_flagStatic = sl_true;
			m_data = const_cast<T*>(data);
			m_count = count;
			m_refer = refer;
		} else {
			m_flagStatic = sl_true;
			m_data = sl_null;
			m_count = 0;
		}
	}

	template <class T>
	CArray<T>::~CArray() noexcept
	{
		if (! m_flagStatic) {
			T* data = m_data;
			if (data) {
				sl_size count = m_count;
				for (sl_size i = 0; i < count; i++) {
					(data+i)->~T();
				}
			}
			Base::freeMemory((void*)data);
		}
	}


	template <class T>
	CArray<T>::CArray(CArray<T>&& other) noexcept
	 : m_refer(Move(other.m_refer))
	{
		m_flagStatic = other.m_flagStatic;
		m_data = other.m_data;
		m_count = other.m_count;
		other.m_flagStatic = sl_true;
		other.m_data = sl_null;
		other.m_count = 0;
	}

	template <class T>
	CArray<T>& CArray<T>::operator=(CArray<T>&& other) noexcept
	{
		if (!m_flagStatic) {
			T* data = m_data;
			if (data) {
				sl_size count = m_count;
				for (sl_size i = 0; i < count; i++) {
					(data+i)->~T();
				}
			}
			Base::freeMemory((void*)data);
		}
		m_flagStatic = other.m_flagStatic;
		m_data = other.m_data;
		m_count = other.m_count;
		other.m_flagStatic = sl_true;
		other.m_data = sl_null;
		other.m_count = 0;
		m_refer = Move(other.m_refer);
		return *this;
	}

	template <class T>
	CArray<T>* CArray<T>::create(sl_size count) noexcept
	{
		if (count > 0) {
			CArray<T>* ret = new CArray<T>(count);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}
	
	template <class T>
	template <class VALUE>
	CArray<T>* CArray<T>::create(const VALUE* data, sl_size count) noexcept
	{
		if (count > 0) {
			CArray<T>* ret = new CArray<T>(data, count);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}
	
	template <class T>
	CArray<T>* CArray<T>::createStatic(const T* data, sl_size count, Referable* refer) noexcept
	{
		if (data && count > 0) {
			return new CArray<T>(data, count, refer);
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE T* CArray<T>::getData() const noexcept
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE sl_size CArray<T>::getCount() const noexcept
	{
		return m_count;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray<T>::isStatic() const noexcept
	{
		return m_flagStatic;
	}
	
	template <class T>
	SLIB_INLINE const Ref<Referable>& CArray<T>::getRefer() const noexcept
	{
		return m_refer;
	}
	
	template <class T>
	SLIB_INLINE T* CArray<T>::getPointerAt(sl_size index) const noexcept
	{
		if (index < m_count) {
			return m_data + index;
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray<T>::getAt(sl_size index, T* _out) const noexcept
	{
		if (index < m_count) {
			*_out = m_data[index];
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T CArray<T>::getValueAt(sl_size index) const noexcept
	{
		if (index < m_count) {
			return m_data[index];
		} else {
			return T();
		}
	}
	
	template <class T>
	SLIB_INLINE T CArray<T>::getValueAt(sl_size index, const T& def) const noexcept
	{
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray<T>::setAt(sl_size index, const T& value) const noexcept
	{
		if (index < m_count) {
			m_data[index] = value;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T const& CArray<T>::operator[](sl_size_t index) const noexcept
	{
		return m_data[index];
	}
	
	template <class T>
	SLIB_INLINE T& CArray<T>::operator[](sl_size_t index) noexcept
	{
		return m_data[index];
	}
	
	template <class T>
	CArray<T>* CArray<T>::sub(sl_size start, sl_size count) noexcept
	{
		sl_size countParent = m_count;
		if (start < countParent) {
			if (count > countParent - start) {
				count = countParent - start;
			}
			if (count > 0) {
				if (start == 0 && countParent == count) {
					return this;
				}
				if (m_flagStatic) {
					return createStatic(m_data + start, count, m_refer._ptr);
				} else {
					return createStatic(m_data + start, count, this);
				}
			}
		}
		return sl_null;
	}
	
	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg CArray<T>::indexOf(const VALUE& value, sl_reg start, const EQUALS& equals) const noexcept
	{
		sl_reg ret = -1;
		sl_reg n = m_count;
		T* p = m_data;
		if (start < 0) {
			start = 0;
		}
		for (sl_reg i = start; i < n; i++) {
			if (equals(p[i], value)) {
				ret = i;
				break;
			}
		}
		return ret;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg CArray<T>::lastIndexOf(const VALUE& value, sl_reg start, const EQUALS& equals) const noexcept
	{
		sl_reg ret = -1;
		sl_reg n = m_count;
		T* p = m_data;
		if (start < 0 || start >= n) {
			start = n - 1;
		}
		for (sl_reg i = start; i >= 0; i--) {
			if (equals(p[i], value)) {
				ret = i;
				break;
			}
		}
		return ret;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool CArray<T>::contains(const VALUE& value, const EQUALS& equals) const noexcept
	{
		return indexOf(value, 0, equals) >= 0;
	}

	template <class T>
	template <class VALUE>
	sl_size CArray<T>::read(sl_size startSource, sl_size len, VALUE* dataDst) const noexcept
	{
		T* pSrc = m_data;
		VALUE* pDst = dataDst;
		if (pDst && pSrc) {
			sl_size countSrc = m_count;
			if (startSource < countSrc) {
				sl_size lenSrc = countSrc - startSource;
				if (len > lenSrc) {
					len = lenSrc;
				}
				pSrc += startSource;
				for (sl_size i = 0; i < len; i++) {
					*(pDst) = *(pSrc);
					pDst++;
					pSrc++;
				}
				return len;
			}
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size CArray<T>::write(sl_size startTarget, sl_size len, const VALUE* dataSrc) const noexcept
	{
		T* pDst = m_data;
		const VALUE* pSrc = dataSrc;
		if (pSrc && pDst) {
			sl_size countDst = m_count;
			if (startTarget < countDst) {
				sl_size lenDst = countDst - startTarget;
				if (len > lenDst) {
					len = lenDst;
				}
				pDst += startTarget;
				for (sl_size i = 0; i < len; i++) {
					*(pDst) = *(pSrc);
					pDst++;
					pSrc++;
				}
				return len;
			}
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size CArray<T>::copy(sl_size startTarget, const CArray<VALUE>* source, sl_size startSource, sl_size len) const noexcept
	{
		if (source) {
			VALUE* pSrc = source->getData();
			if (pSrc) {
				sl_size countSrc = source->getCount();
				if (startSource < countSrc) {
					sl_size lenSrc = countSrc - startSource;
					if (len > lenSrc) {
						len = lenSrc;
					}
					return write<VALUE>(startTarget, len, pSrc + startSource);
				}
			}
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size CArray<T>::copy(const CArray<VALUE>* source, sl_size start, sl_size len) const noexcept
	{
		return copy(0, source, start, len);
	}

	template <class T>
	CArray<T>* CArray<T>::duplicate() const noexcept
	{
		return create(m_data, m_count);
	}

	template <class T>
	SLIB_INLINE T* CArray<T>::begin() noexcept
	{
		return m_data;
	}

	template <class T>
	SLIB_INLINE T const* CArray<T>::begin() const noexcept
	{
		return m_data;
	}

	template <class T>
	SLIB_INLINE T* CArray<T>::end() noexcept
	{
		return m_data + m_count;
	}

	template <class T>
	SLIB_INLINE T const* CArray<T>::end() const noexcept
	{
		return m_data + m_count;
	}


	template <class T>
	SLIB_INLINE ArrayPosition<T>::ArrayPosition() noexcept
	{
		pos = sl_null;
	}

	template <class T>
	SLIB_INLINE ArrayPosition<T>::ArrayPosition(T* _pos, sl_size _count, Referable* _ref) noexcept
	 : ref(_ref)
	{
		count = _count;
		if (_count > 0) {
			pos = _pos;
		} else {
			pos = sl_null;
		}
	}

	template <class T>
	SLIB_INLINE T& ArrayPosition<T>::operator*() const noexcept
	{
		return *pos;
	}
	
	template <class T>
	SLIB_INLINE sl_bool ArrayPosition<T>::operator==(const ArrayPosition<T>& other) const noexcept
	{
		return pos == other.pos;
	}

	template <class T>
	SLIB_INLINE sl_bool ArrayPosition<T>::operator!=(const ArrayPosition<T>& other) const noexcept
	{
		return pos != other.pos;
	}
	
	template <class T>
	SLIB_INLINE ArrayPosition<T>::operator sl_bool() const noexcept
	{
		return count > 0;
	}

	template <class T>
	SLIB_INLINE ArrayPosition<T>& ArrayPosition<T>::operator++() noexcept
	{
		pos++;
		count--;
		if (count == 0) {
			pos = sl_null;
		}
		return *this;
	}


	template <class T>
	Array<T>::Array(sl_size count) noexcept : ref(CArray<T>::create(count))
	{
	}
	
	template <class T>
	template <class VALUE>
	Array<T>::Array(const VALUE* data, sl_size count) noexcept : ref(CArray<T>::create(data, count))
	{
	}
	
	template <class T>
	Array<T>::Array(const T* data, sl_size count, Referable* refer) noexcept : ref(CArray<T>::createStatic(data, count, refer))
	{
	}

	template <class T>
	Array<T> Array<T>::create(sl_size count) noexcept
	{
		return CArray<T>::create(count);
	}

	template <class T>
	template <class VALUE>
	Array<T> Array<T>::create(const VALUE* data, sl_size count) noexcept
	{
		return CArray<T>::create(data, count);
	}

	template <class T>
	Array<T> Array<T>::createStatic(const T* data, sl_size count) noexcept
	{
		return CArray<T>::createStatic(data, count, sl_null);
	}

	template <class T>
	Array<T> Array<T>::createStatic(const T* data, sl_size count, Referable* refer) noexcept
	{
		return CArray<T>::createStatic(data, count, refer);
	}
	
	template <class T>
	template <class VALUE>
	const Array<T>& Array<T>::from(const Array<VALUE>& other) noexcept
	{
		return *(reinterpret_cast<Array<T> const*>(&other));
	}
	
	template <class T>
	SLIB_INLINE T* Array<T>::getData() const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE sl_size Array<T>::getCount() const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_bool Array<T>::isEmpty() const noexcept
	{
		return ref.isNull();
	}

	template <class T>
	SLIB_INLINE sl_bool Array<T>::isNotEmpty() const noexcept
	{
		return ref.isNotNull();
	}

	template <class T>
	T* Array<T>::getPointerAt(sl_size index) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getPointerAt(index);
		}
		return sl_null;
	}

	template <class T>
	sl_bool Array<T>::getAt(sl_size index, T* _out) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T Array<T>::getValueAt(sl_size index) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	T Array<T>::getValueAt(sl_size index, const T& def) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index, def);
		}
		return def;
	}

	template <class T>
	sl_bool Array<T>::setAt(sl_size index, const T& value) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt(index, value);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE T& Array<T>::operator[](sl_size_t index) const noexcept
	{
		return (ref->getData())[index];
	}

	template <class T>
	Array<T> Array<T>::sub(sl_size start, sl_size count) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->sub(start, count);
		}
		return sl_null;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg Array<T>::indexOf(const VALUE& value, sl_reg start, const EQUALS& equals) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg Array<T>::lastIndexOf(const VALUE& value, sl_reg start, const EQUALS& equals) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Array<T>::contains(const VALUE& value, const EQUALS& equals) const noexcept
	{
		return indexOf(value, equals) >= 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Array<T>::read(sl_size startSource, sl_size len, VALUE* dataDst) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->read(startSource, len, dataDst);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Array<T>::write(sl_size startTarget, sl_size len, const VALUE* dataSrc) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->write(startTarget, len, dataSrc);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Array<T>::copy(sl_size startTarget, const Array<VALUE>& source, sl_size startSource, sl_size len) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->copy(startTarget, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Array<T>::copy(const Array<VALUE>& source, sl_size start, sl_size len) const noexcept
	{
		return copy(0, source, start, len);
	}

	template <class T>
	Array<T> Array<T>::duplicate() const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}


	template <class T>
	sl_bool Array<T>::getData(ArrayData<T>& data) const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			data.data = obj->getData();
			data.count = obj->getCount();
			if (obj->isStatic()) {
				data.refer = obj->getRefer();
			} else {
				data.refer = obj;
			}
			return sl_true;
		} else {
			data.data = sl_null;
			data.count = 0;
			data.refer.setNull();
			return sl_false;
		}
	}

	template <class T>
	T* Array<T>::begin() const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return sl_null;
	}

	template <class T>
	T* Array<T>::end() const noexcept
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData() + obj->getCount();
		}
		return sl_null;
	}


	template <class T>
	Atomic< Array<T> >::Atomic(sl_size count) noexcept : ref(CArray<T>::create(count))
	{
	}
	
	template <class T>
	template <class VALUE>
	Atomic< Array<T> >::Atomic(const VALUE* data, sl_size count) noexcept : ref(CArray<T>::create(data, count))
	{
	}
	
	template <class T>
	Atomic< Array<T> >::Atomic(const T* data, sl_size count, Referable* refer) noexcept : ref(CArray<T>::createStatic(data, count, refer))
	{
	}
	
	template <class T>
	template <class VALUE>
	const Atomic< Array<T> >& Atomic< Array<T> >::from(const Atomic< Array<VALUE> >& other) noexcept
	{
		return *(reinterpret_cast<Atomic< Array<T> > const*>(&other));
	}

	template <class T>
	sl_size Atomic< Array<T> >::getCount() const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< Array<T> >::isEmpty() const noexcept
	{
		return ref.isNull();
	}

	template <class T>
	sl_bool Atomic< Array<T> >::isNotEmpty() const noexcept
	{
		return ref.isNotNull();
	}

	template <class T>
	sl_bool Atomic< Array<T> >::getAt(sl_size index, T* _out) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< Array<T> >::getValueAt(sl_size index) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	T Atomic< Array<T> >::getValueAt(sl_size index, const T& def) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index, def);
		}
		return def;
	}

	template <class T>
	sl_bool Atomic< Array<T> >::setAt(sl_size index, const T& value) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setAt(index, value);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< Array<T> >::operator[](sl_size_t index) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	Array<T> Atomic< Array<T> >::sub(sl_size start, sl_size count) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->sub(start, count);
		}
		return sl_null;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg Atomic< Array<T> >::indexOf(const VALUE& value, sl_reg start, const EQUALS& equals) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->indexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_reg Atomic< Array<T> >::lastIndexOf(const VALUE& value, sl_reg start, const EQUALS& equals) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->lastIndexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class VALUE, class EQUALS>
	sl_bool Atomic< Array<T> >::contains(const VALUE& value, const EQUALS& equals) const noexcept
	{
		return indexOf(value, equals) >= 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array<T> >::read(sl_size startSource, sl_size len, VALUE* dataDst) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->read(startSource, len, dataDst);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array<T> >::write(sl_size startTarget, sl_size len, const VALUE* dataSrc) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->write(startTarget, len, dataSrc);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array<T> >::copy(sl_size startTarget, const Array<VALUE>& source, sl_size startSource, sl_size len) const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->copy(startTarget, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array<T> >::copy(const Array<VALUE>& source, sl_size start, sl_size len) const noexcept
	{
		return copy(0, source, start, len);
	}

	template <class T>
	Array<T> Atomic< Array<T> >::duplicate() const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	sl_bool Atomic< Array<T> >::getData(ArrayData<T>& data) const noexcept
	{
		Array<T> obj(*this);
		return obj.getData(data);
	}

	template <class T>
	ArrayPosition<T> Atomic< Array<T> >::begin() const noexcept
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return ArrayPosition<T>(obj->getData(), obj->getCount(), obj.get());
		}
		return ArrayPosition<T>();
	}

	template <class T>
	ArrayPosition<T> Atomic< Array<T> >::end() const noexcept
	{
		return ArrayPosition<T>();
	}

}
