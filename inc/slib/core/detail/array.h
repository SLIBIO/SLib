#ifndef CHECKHEADER_SLIB_CORE_DETAIL_ARRAY
#define CHECKHEADER_SLIB_CORE_DETAIL_ARRAY

#include "../array.h"

namespace slib
{
	
	template <class T>
	SLIB_INLINE T& ArrayData<T>::operator[](sl_reg index) const
	{
		return data[index];
	}

	template <class T>
	CArray<T>::CArray()
	{
		m_flagStatic = sl_true;
		m_data = sl_null;
		m_count = 0;
	}

	template <class T>
	CArray<T>::CArray(sl_size count)
	{
		if (count > 0) {
			T* dataNew = NewHelper<T>::create(count);
			if (dataNew) {
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
	template <class _T>
	CArray<T>::CArray(const _T* data, sl_size count)
	{
		if (count > 0) {
			T* dataNew = NewHelper<T>::create(data, count);
			if (dataNew) {
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
	CArray<T>::CArray(const T* data, sl_size count, Referable* refer)
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
	CArray<T>::~CArray()
	{
		if (! m_flagStatic) {
			NewHelper<T>::free(m_data, m_count);
		}
	}

	template <class T>
	CArray<T>* CArray<T>::create(sl_size count)
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
	template <class _T>
	CArray<T>* CArray<T>::create(const _T* data, sl_size count)
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
	CArray<T>* CArray<T>::createStatic(const T* data, sl_size count, Referable* refer)
	{
		if (data && count > 0) {
			return new CArray<T>(data, count, refer);
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE T* CArray<T>::getData() const
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE sl_size CArray<T>::getCount() const
	{
		return m_count;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray<T>::isStatic() const
	{
		return m_flagStatic;
	}
	
	template <class T>
	SLIB_INLINE const Ref<Referable>& CArray<T>::getRefer() const
	{
		return m_refer;
	}
	
	template <class T>
	SLIB_INLINE T* CArray<T>::getPointerAt(sl_size index) const
	{
		if (index < m_count) {
			return m_data + index;
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray<T>::getAt(sl_size index, T* _out) const
	{
		if (index < m_count) {
			*_out = m_data[index];
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T CArray<T>::getValueAt(sl_size index) const
	{
		if (index < m_count) {
			return m_data[index];
		} else {
			return T();
		}
	}
	
	template <class T>
	SLIB_INLINE T CArray<T>::getValueAt(sl_size index, const T& def) const
	{
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray<T>::setAt(sl_size index, const T& value) const
	{
		if (index < m_count) {
			m_data[index] = value;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	SLIB_INLINE T const& CArray<T>::operator[](sl_size_t index) const
	{
		return m_data[index];
	}
	
	template <class T>
	SLIB_INLINE T& CArray<T>::operator[](sl_size_t index)
	{
		return m_data[index];
	}
	
	template <class T>
	CArray<T>* CArray<T>::sub(sl_size start, sl_size count)
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
	template <class _T, class EQUALS>
	sl_reg CArray<T>::indexOf(const _T& value, sl_reg start, const EQUALS& equals) const
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
	template <class _T, class EQUALS>
	sl_reg CArray<T>::lastIndexOf(const _T& value, sl_reg start, const EQUALS& equals) const
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
	template <class _T, class EQUALS>
	sl_bool CArray<T>::contains(const _T& value, const EQUALS& equals) const
	{
		return indexOf(value, 0, equals) >= 0;
	}

	template <class T>
	template <class _T>
	sl_size CArray<T>::read(sl_size startSource, sl_size len, _T* dataDst) const
	{
		T* pSrc = m_data;
		_T* pDst = dataDst;
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
	template <class _T>
	sl_size CArray<T>::write(sl_size startTarget, sl_size len, const _T* dataSrc) const
	{
		T* pDst = m_data;
		const _T* pSrc = dataSrc;
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
	template <class _T>
	sl_size CArray<T>::copy(sl_size startTarget, const CArray<_T>* source, sl_size startSource, sl_size len) const
	{
		if (source) {
			_T* pSrc = source->getData();
			if (pSrc) {
				sl_size countSrc = source->getCount();
				if (startSource < countSrc) {
					sl_size lenSrc = countSrc - startSource;
					if (len > lenSrc) {
						len = lenSrc;
					}
					return write<_T>(startTarget, len, pSrc + startSource);
				}
			}
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size CArray<T>::copy(const CArray<_T>* source, sl_size start, sl_size len) const
	{
		return copy(0, source, start, len);
	}

	template <class T>
	CArray<T>* CArray<T>::duplicate() const
	{
		return create(m_data, m_count);
	}

	template <class T>
	SLIB_INLINE T* CArray<T>::begin()
	{
		return m_data;
	}

	template <class T>
	SLIB_INLINE T const* CArray<T>::begin() const
	{
		return m_data;
	}

	template <class T>
	SLIB_INLINE T* CArray<T>::end()
	{
		return m_data + m_count;
	}

	template <class T>
	SLIB_INLINE T const* CArray<T>::end() const
	{
		return m_data + m_count;
	}


	template <class T>
	SLIB_INLINE ArrayPosition<T>::ArrayPosition()
	{
		data = sl_null;
		count = 0;
	}

	template <class T>
	SLIB_INLINE ArrayPosition<T>::ArrayPosition(const Ref< CArray<T> >& array) : ref(array)
	{
		if (array.isNotNull()) {
			data = array->getData();
			count = array->getCount();
		} else {
			data = sl_null;
			count = 0;
		}
	}

	template <class T>
	ArrayPosition<T>::ArrayPosition(const ArrayPosition<T>& other) = default;

	template <class T>
	ArrayPosition<T>::ArrayPosition(ArrayPosition<T>&& other) = default;

	template <class T>
	SLIB_INLINE T& ArrayPosition<T>::operator*()
	{
		return *data;
	}

	template <class T>
	SLIB_INLINE sl_bool ArrayPosition<T>::operator!=(const ArrayPosition<T>& other)
	{
		return count > 0;
	}

	template <class T>
	SLIB_INLINE ArrayPosition<T>& ArrayPosition<T>::operator++()
	{
		data++;
		count--;
		return *this;
	}


	template <class T>
	Array<T>::Array(sl_size count): ref(CArray<T>::create(count))
	{
	}
	
	template <class T>
	template <class _T>
	Array<T>::Array(const _T* data, sl_size count): ref(CArray<T>::create(data, count))
	{
	}
	
	template <class T>
	Array<T>::Array(const T* data, sl_size count, Referable* refer): ref(CArray<T>::createStatic(data, count, refer))
	{
	}

	template <class T>
	Array<T> Array<T>::create(sl_size count)
	{
		return CArray<T>::create(count);
	}

	template <class T>
	template <class _T>
	Array<T> Array<T>::create(const _T* data, sl_size count)
	{
		return CArray<T>::create(data, count);
	}

	template <class T>
	Array<T> Array<T>::createStatic(const T* data, sl_size count)
	{
		return CArray<T>::createStatic(data, count, sl_null);
	}

	template <class T>
	Array<T> Array<T>::createStatic(const T* data, sl_size count, Referable* refer)
	{
		return CArray<T>::createStatic(data, count, refer);
	}

	template <class T>
	SLIB_INLINE T* Array<T>::getData() const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE sl_size Array<T>::getCount() const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	SLIB_INLINE sl_bool Array<T>::isEmpty() const
	{
		return ref.isNull();
	}

	template <class T>
	SLIB_INLINE sl_bool Array<T>::isNotEmpty() const
	{
		return ref.isNotNull();
	}

	template <class T>
	T* Array<T>::getPointerAt(sl_size index) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getPointerAt(index);
		}
		return sl_null;
	}

	template <class T>
	sl_bool Array<T>::getAt(sl_size index, T* _out) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T Array<T>::getValueAt(sl_size index) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	T Array<T>::getValueAt(sl_size index, const T& def) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(index, def);
		}
		return def;
	}

	template <class T>
	sl_bool Array<T>::setAt(sl_size index, const T& value) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt(index, value);
		}
		return sl_false;
	}

	template <class T>
	SLIB_INLINE T& Array<T>::operator[](sl_size_t index) const
	{
		return (ref->getData())[index];
	}

	template <class T>
	Array<T> Array<T>::sub(sl_size start, sl_size count) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->sub(start, count);
		}
		return sl_null;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg Array<T>::indexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->indexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg Array<T>::lastIndexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->lastIndexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool Array<T>::contains(const _T& value, const EQUALS& equals) const
	{
		return indexOf(value, equals) >= 0;
	}

	template <class T>
	template <class _T>
	sl_size Array<T>::read(sl_size startSource, sl_size len, _T* dataDst) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->read(startSource, len, dataDst);
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size Array<T>::write(sl_size startTarget, sl_size len, const _T* dataSrc) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->write(startTarget, len, dataSrc);
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size Array<T>::copy(sl_size startTarget, const Array<_T>& source, sl_size startSource, sl_size len) const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->copy(startTarget, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size Array<T>::copy(const Array<_T>& source, sl_size start, sl_size len) const
	{
		return copy(0, source, start, len);
	}

	template <class T>
	Array<T> Array<T>::duplicate() const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	Iterator<T> Array<T>::toIterator() const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return new ArrayIterator<T>(obj);
		}
		return sl_null;
	}

	template <class T>
	sl_bool Array<T>::getData(ArrayData<T>& data) const
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
	T* Array<T>::begin() const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return sl_null;
	}

	template <class T>
	T* Array<T>::end() const
	{
		CArray<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData() + obj->getCount();
		}
		return sl_null;
	}


	template <class T>
	Atomic< Array<T> >::Atomic(sl_size count): ref(CArray<T>::create(count))
	{
	}
	
	template <class T>
	template <class _T>
	Atomic< Array<T> >::Atomic(const _T* data, sl_size count): ref(CArray<T>::create(data, count))
	{
	}
	
	template <class T>
	Atomic< Array<T> >::Atomic(const T* data, sl_size count, Referable* refer): ref(CArray<T>::createStatic(data, count, refer))
	{
	}
	
	template <class T>
	sl_size Atomic< Array<T> >::getCount() const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< Array<T> >::isEmpty() const
	{
		return ref.isNull();
	}

	template <class T>
	sl_bool Atomic< Array<T> >::isNotEmpty() const
	{
		return ref.isNotNull();
	}

	template <class T>
	sl_bool Atomic< Array<T> >::getAt(sl_size index, T* _out) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAt(index, _out);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< Array<T> >::getValueAt(sl_size index) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	T Atomic< Array<T> >::getValueAt(sl_size index, const T& def) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index, def);
		}
		return def;
	}

	template <class T>
	sl_bool Atomic< Array<T> >::setAt(sl_size index, const T& value) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setAt(index, value);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< Array<T> >::operator[](sl_size_t index) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(index);
		} else {
			return T();
		}
	}

	template <class T>
	Array<T> Atomic< Array<T> >::sub(sl_size start, sl_size count) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->sub(start, count);
		}
		return sl_null;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg Atomic< Array<T> >::indexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->indexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_reg Atomic< Array<T> >::lastIndexOf(const _T& value, sl_reg start, const EQUALS& equals) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->lastIndexOf(value, start, equals);
		}
		return -1;
	}

	template <class T>
	template <class _T, class EQUALS>
	sl_bool Atomic< Array<T> >::contains(const _T& value, const EQUALS& equals) const
	{
		return indexOf(value, equals) >= 0;
	}

	template <class T>
	template <class _T>
	sl_size Atomic< Array<T> >::read(sl_size startSource, sl_size len, _T* dataDst) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->read(startSource, len, dataDst);
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size Atomic< Array<T> >::write(sl_size startTarget, sl_size len, const _T* dataSrc) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->write(startTarget, len, dataSrc);
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size Atomic< Array<T> >::copy(sl_size startTarget, const Array<_T>& source, sl_size startSource, sl_size len) const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->copy(startTarget, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	template <class T>
	template <class _T>
	sl_size Atomic< Array<T> >::copy(const Array<_T>& source, sl_size start, sl_size len) const
	{
		return copy(0, source, start, len);
	}

	template <class T>
	Array<T> Atomic< Array<T> >::duplicate() const
	{
		Ref< CArray<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	sl_bool Atomic< Array<T> >::getData(ArrayData<T>& data) const
	{
		Array<T> obj(*this);
		return obj.getData(data);
	}

	template <class T>
	ArrayPosition<T> Atomic< Array<T> >::begin() const
	{
		return ArrayPosition<T>(ref);
	}

	template <class T>
	ArrayPosition<T> Atomic< Array<T> >::end() const
	{
		return ArrayPosition<T>();
	}


	template <class T>
	ArrayIterator<T>::ArrayIterator(const Array<T>& arr)
	{
		m_index = 0;
		arr.getData(m_arr);
	}

	template <class T>
	sl_bool ArrayIterator<T>::hasNext()
	{
		return (m_index < m_arr.count);
	}

	template <class T>
	sl_bool ArrayIterator<T>::next(T* _out)
	{
		sl_size index = m_index;
		if (index < m_arr.count) {
			if (_out) {
				*_out = m_arr.data[index];
			}
			m_index = index + 1;
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_reg ArrayIterator<T>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}

}

#endif
