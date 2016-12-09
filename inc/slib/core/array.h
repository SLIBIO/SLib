#ifndef CHECKHEADER_SLIB_CORE_ARRAY
#define CHECKHEADER_SLIB_CORE_ARRAY

#include "definition.h"

#include "new.h"
#include "compare.h"
#include "ref.h"
#include "iterator.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT ArrayData
{
public:
	T* data;
	sl_size count;
	Ref<Referable> refer;
	
public:
	T& operator[](sl_reg index) const;
	
};

class SLIB_EXPORT CArrayBase : public Referable
{
	SLIB_DECLARE_OBJECT
};

template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT CArray : public CArrayBase
{
protected:
	T* m_data;
	sl_size m_count;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
protected:
	CArray();
	
public:
	CArray(sl_size count);
	
	CArray(const T* data, sl_size count);
	
	template <class _T>
	CArray(const _T* data, sl_size count);
	
	CArray(const T* data, sl_size count, const Referable* refer);
	
	~CArray();
	
public:
	static CArray<T, COMPARE>* create(sl_size count);
	
	static CArray<T, COMPARE>* create(const T* data, sl_size count);
	
	template <class _T>
	static CArray<T, COMPARE>* create(const _T* data, sl_size count);
	
	static CArray<T, COMPARE>* createStatic(const T* data, sl_size count, const Referable* refer);
	
public:
	T* getData() const;
	
	sl_size getCount() const;
	
	sl_bool isStatic() const;
	
	const Ref<Referable>& getRefer() const;
	
public:
	T* getItemPtr(sl_size index) const;
	
	sl_bool getItem(sl_size index, T* _out = sl_null) const;
	
	T getItemValue(sl_size index, const T& def) const;
	
	sl_bool setItem(sl_size index, const T& value) const;
	
public:
	CArray<T, COMPARE>* sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const;
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const;
	
	sl_bool contains(const T& value) const;
	
	sl_size read(sl_size startSource, sl_size len, T* dataDst) const;
	
	template <class _T>
	sl_size read(sl_size startSource, sl_size len, _T* dataDst) const;
	
	sl_size write(sl_size startTarget, sl_size len, const T* dataSrc) const;
	
	template <class _T>
	sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const;
	
	sl_size copy(sl_size startTarget, const CArray<T, COMPARE>* source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	template <class _T, class _COMPARE>
	sl_size copy(sl_size startTarget, const CArray<_T, _COMPARE>* source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	sl_size copy(const CArray<T, COMPARE>* source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	template <class _T, class _COMPARE>
	sl_size copy(const CArray<_T, _COMPARE>* source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	CArray<T, COMPARE>* duplicate() const;
	
	// range-based for loop
	T* begin();
	
	T const* begin() const;
	
	T* end();
	
	T const* end() const;

};

template <class T, class COMPARE>
class SLIB_EXPORT ArrayPosition
{
public:
	ArrayPosition();
	
	ArrayPosition(const Ref< CArray<T, COMPARE> >& array);
	
	ArrayPosition(const ArrayPosition<T, COMPARE>& other);
	
	ArrayPosition(ArrayPosition<T, COMPARE>&& other);
	
public:
	T& operator*();
	
	sl_bool operator!=(const ArrayPosition<T, COMPARE>& other);
	
	ArrayPosition<T, COMPARE>& operator++();
	
private:
	Ref< CArray<T, COMPARE> > ref;
	T* data;
	sl_size count;
	
};

#define SLIB_TEMPLATE_PARAMS_CArray T, COMPARE
#define SLIB_TEMPLATE_DEF_PARAMS_CArray class T, class COMPARE

template <class T, class COMPARE>
class SafeArray;

/** auto-referencing object **/
template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT Array
{
public:
	Ref< CArray<T, COMPARE> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(Array, SafeArray, CArray)
	
public:
	Array(sl_size count);
	
	Array(const T* data, sl_size count);
	
	template <class _T>
	Array(const _T* data, sl_size count);
	
	Array(const T* data, sl_size count, const Referable* refer);
	
public:
	template <class _COMPARE>
	const Array<T, COMPARE>& from(const Array<T, _COMPARE>& other);
	
public:
	static Array<T, COMPARE> create(sl_size count);
	
	static Array<T, COMPARE> create(const T* data, sl_size count);
	
	template <class _T>
	static Array<T, COMPARE> create(const _T* data, sl_size count);
	
	static Array<T, COMPARE> createStatic(const T* data, sl_size count);
	
	static Array<T, COMPARE> createStatic(const T* data, sl_size count, const Referable* refer);
	
public:
	T* getData() const;
	
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
public:
	T* getItemPtr(sl_size index) const;
	
	sl_bool getItem(sl_size index, T* _out = sl_null) const;
	
	T getItemValue(sl_size index, const T& def) const;
	
	sl_bool setItem(sl_size index, const T& value) const;
	
public:
	Array<T, COMPARE> sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const;
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const;
	
	sl_bool contains(const T& value) const;
	
	
	sl_size read(sl_size startSource, sl_size len, T* dataDst) const;
	
	template <class _T>
	sl_size read(sl_size startSource, sl_size len, _T* dataDst) const;
	
	sl_size write(sl_size startTarget, sl_size len, const T* dataSrc) const;
	
	template <class _T>
	sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const;
	
	sl_size copy(sl_size startTarget, const Array<T, COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	template <class _T, class _COMPARE>
	sl_size copy(sl_size startTarget, const Array<_T, _COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	sl_size copy(const Array<T, COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	template <class _T, class _COMPARE>
	sl_size copy(const Array<_T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	Array<T, COMPARE> duplicate() const;
	
	Iterator<T> iterator() const;
	
	sl_bool getData(ArrayData<T>& data) const;
	
	// range-based for loop
	T* begin() const;
	
	T* end() const;
	
};


/** auto-referencing object **/
template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT SafeArray
{
public:
	SafeRef< CArray<T, COMPARE> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(SafeArray, Array, CArray)
	
public:
	SafeArray(sl_size count);
	
	SafeArray(const T* data, sl_size count);
	
	template <class _T>
	SafeArray(const _T* data, sl_size count);
	
	SafeArray(const T* data, sl_size count, const Referable* refer);
	
public:
	template <class _COMPARE>
	const SafeArray<T, COMPARE>& from(const SafeArray<T, _COMPARE>& other);
	
public:
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
public:
	sl_bool getItem(sl_size index, T* _out = sl_null) const;
	
	T getItemValue(sl_size index, const T& def) const;
	
	sl_bool setItem(sl_size index, const T& value) const;
	
public:
	Array<T, COMPARE> sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const;
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const;
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const;
	
	sl_bool contains(const T& value) const;
	
	
	sl_size read(sl_size startSource, sl_size len, T* dataDst) const;
	
	template <class _T>
	sl_size read(sl_size startSource, sl_size len, _T* dataDst) const;
	
	sl_size write(sl_size startTarget, sl_size len, const T* dataSrc) const;
	
	template <class _T>
	sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const;
	
	sl_size copy(sl_size startTarget, const Array<T, COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	template <class _T, class _COMPARE>
	sl_size copy(sl_size startTarget, const Array<_T, _COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	sl_size copy(const Array<T, COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	template <class _T, class _COMPARE>
	sl_size copy(const Array<_T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const;
	
	Array<T, COMPARE> duplicate() const;
	
	sl_bool getData(ArrayData<T>& data) const;
	
	// range-based for loop
	ArrayPosition<T, COMPARE> begin() const;
	
	ArrayPosition<T, COMPARE> end() const;
	
};


template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT ArrayIterator : public IIterator<T>
{
protected:
	ArrayData<T> m_arr;
	sl_size m_index;
	
public:
	ArrayIterator(const Array<T, COMPARE>& arr);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(T* _out);
	
	// override
	sl_reg getIndex();
	
};


#define SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_ARRAY(...) \
	extern template class CArray<__VA_ARGS__>; \
	extern template class Array<__VA_ARGS__>; \
	extern template class SafeArray<__VA_ARGS__>; \
	extern template class ArrayIterator<__VA_ARGS__>;

#define SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_ARRAY(...) \
	template class CArray<__VA_ARGS__>; \
	template class Array<__VA_ARGS__>; \
	template class SafeArray<__VA_ARGS__>; \
	template class ArrayIterator<__VA_ARGS__>;

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_ARRAY(sl_uint8)


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T>
SLIB_INLINE T& ArrayData<T>::operator[](sl_reg index) const
{
	return data[index];
}

template <class T, class COMPARE>
SLIB_INLINE CArray<T, COMPARE>::CArray()
{
}

template <class T, class COMPARE>
CArray<T, COMPARE>::CArray(sl_size count)
{
	if (count > 0) {
		T* dataNew = New<T>::create(count);
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

template <class T, class COMPARE>
CArray<T, COMPARE>::CArray(const T* data, sl_size count)
{
	if (count > 0) {
		T* dataNew = New<T>::create(data, count);
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

template <class T, class COMPARE>
template <class _T>
CArray<T, COMPARE>::CArray(const _T* data, sl_size count)
{
	if (count > 0) {
		T* dataNew = New<T>::create(data, count);
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

template <class T, class COMPARE>
CArray<T, COMPARE>::CArray(const T* data, sl_size count, const Referable* refer)
{
	if (data && count > 0) {
		m_flagStatic = sl_true;
		m_data = (T*)data;
		m_count = count;
		m_refer = refer;
	} else {
		m_flagStatic = sl_true;
		m_data = sl_null;
		m_count = 0;
	}
}

template <class T, class COMPARE>
CArray<T, COMPARE>::~CArray()
{
	if (! m_flagStatic) {
		New<T>::free(m_data, m_count);
	}
}

template <class T, class COMPARE>
CArray<T, COMPARE>* CArray<T, COMPARE>::create(sl_size count)
{
	if (count > 0) {
		CArray<T, COMPARE>* ret = new CArray<T, COMPARE>(count);
		if (ret) {
			if (ret->m_data) {
				return ret;
			}
			delete ret;
		}
	}
	return sl_null;
}

template <class T, class COMPARE>
CArray<T, COMPARE>* CArray<T, COMPARE>::create(const T* data, sl_size count)
{
	if (count > 0) {
		CArray<T, COMPARE>* ret = new CArray<T, COMPARE>(data, count);
		if (ret) {
			if (ret->m_data) {
				return ret;
			}
			delete ret;
		}
	}
	return sl_null;
}

template <class T, class COMPARE>
template <class _T>
CArray<T, COMPARE>* CArray<T, COMPARE>::create(const _T* data, sl_size count)
{
	if (count > 0) {
		CArray<T, COMPARE>* ret = new CArray<T, COMPARE>(data, count);
		if (ret) {
			if (ret->m_data) {
				return ret;
			}
			delete ret;
		}
	}
	return sl_null;
}

template <class T, class COMPARE>
CArray<T, COMPARE>* CArray<T, COMPARE>::createStatic(const T* data, sl_size count, const Referable* refer)
{
	if (data && count > 0) {
		return new CArray<T, COMPARE>(data, count, refer);
	}
	return sl_null;
}

template <class T, class COMPARE>
SLIB_INLINE T* CArray<T, COMPARE>::getData() const
{
	return m_data;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size CArray<T, COMPARE>::getCount() const
{
	return m_count;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool CArray<T, COMPARE>::isStatic() const
{
	return m_flagStatic;
}

template <class T, class COMPARE>
SLIB_INLINE const Ref<Referable>& CArray<T, COMPARE>::getRefer() const
{
	return m_refer;
}

template <class T, class COMPARE>
SLIB_INLINE T* CArray<T, COMPARE>::getItemPtr(sl_size index) const
{
	if (index < m_count) {
		return m_data + index;
	}
	return sl_null;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool CArray<T, COMPARE>::getItem(sl_size index, T* _out) const
{
	if (index < m_count) {
		*_out = m_data[index];
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
SLIB_INLINE T CArray<T, COMPARE>::getItemValue(sl_size index, const T& def) const
{
	if (index < m_count) {
		return m_data[index];
	}
	return def;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool CArray<T, COMPARE>::setItem(sl_size index, const T& value) const
{
	if (index < m_count) {
		m_data[index] = value;
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
CArray<T, COMPARE>* CArray<T, COMPARE>::sub(sl_size start, sl_size count) const
{
	sl_size countParent = m_count;
	if (start < countParent) {
		if (count > countParent - start) {
			count = countParent - start;
		}
		if (count > 0) {
			if (start == 0 && countParent == count) {
				return (CArray<T, COMPARE>*)this;
			}
			if (m_flagStatic) {
				return createStatic(m_data + start, count, m_refer.ptr);
			} else {
				return createStatic(m_data + start, count, this);
			}
		}
	}
	return sl_null;
}

template <class T, class COMPARE>
sl_reg CArray<T, COMPARE>::indexOf(const T& value, sl_reg start) const
{
	sl_reg ret = -1;
	sl_reg n = m_count;
	T* p = m_data;
	if (start < 0) {
		start = 0;
	}
	for (sl_reg i = start; i < n; i++) {
		if (COMPARE::equals(p[i], value)) {
			ret = i;
			break;
		}
	}
	return ret;
}

template <class T, class COMPARE>
sl_reg CArray<T, COMPARE>::lastIndexOf(const T& value, sl_reg start) const
{
	sl_reg ret = -1;
	sl_reg n = m_count;
	T* p = m_data;
	if (start < 0 || start >= n) {
		start = n - 1;
	}
	for (sl_reg i = start; i >= 0; i--) {
		if (COMPARE::equals(p[i], value)) {
			ret = i;
			break;
		}
	}
	return ret;
}

template <class T, class COMPARE>
sl_bool CArray<T, COMPARE>::contains(const T& value) const
{
	return indexOf(value) >= 0;
}

template <class T, class COMPARE>
sl_size CArray<T, COMPARE>::read(sl_size startSource, sl_size len, T* dataDst) const
{
	T* pSrc = m_data;
	T* pDst = dataDst;
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

template <class T, class COMPARE>
template <class _T>
sl_size CArray<T, COMPARE>::read(sl_size startSource, sl_size len, _T* dataDst) const
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

template <class T, class COMPARE>
sl_size CArray<T, COMPARE>::write(sl_size startTarget, sl_size len, const T* dataSrc) const
{
	T* pDst = m_data;
	const T* pSrc = dataSrc;
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

template <class T, class COMPARE>
template <class _T>
sl_size CArray<T, COMPARE>::write(sl_size startTarget, sl_size len, const _T* dataSrc) const
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

template <class T, class COMPARE>
sl_size CArray<T, COMPARE>::copy(sl_size startTarget, const CArray<T, COMPARE>* source, sl_size startSource, sl_size len) const
{
	if (source) {
		T* pSrc = source->getData();
		if (pSrc) {
			sl_size countSrc = source->getCount();
			if (startSource < countSrc) {
				sl_size lenSrc = countSrc - startSource;
				if (len > lenSrc) {
					len = lenSrc;
				}
				return write(startTarget, len, pSrc + startSource);
			}
		}
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size CArray<T, COMPARE>::copy(sl_size startTarget, const CArray<_T, _COMPARE>* source, sl_size startSource, sl_size len) const
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

template <class T, class COMPARE>
sl_size CArray<T, COMPARE>::copy(const CArray<T, COMPARE>* source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size CArray<T, COMPARE>::copy(const CArray<_T, _COMPARE>* source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

template <class T, class COMPARE>
CArray<T, COMPARE>* CArray<T, COMPARE>::duplicate() const
{
	return create(m_data, m_count);
}

template <class T, class COMPARE>
SLIB_INLINE T* CArray<T, COMPARE>::begin()
{
	return m_data;
}

template <class T, class COMPARE>
SLIB_INLINE T const* CArray<T, COMPARE>::begin() const
{
	return m_data;
}

template <class T, class COMPARE>
SLIB_INLINE T* CArray<T, COMPARE>::end()
{
	return m_data + m_count;
}

template <class T, class COMPARE>
SLIB_INLINE T const* CArray<T, COMPARE>::end() const
{
	return m_data + m_count;
}


template <class T, class COMPARE>
SLIB_INLINE ArrayPosition<T, COMPARE>::ArrayPosition() {};

template <class T, class COMPARE>
SLIB_INLINE ArrayPosition<T, COMPARE>::ArrayPosition(const Ref< CArray<T, COMPARE> >& array) : ref(array)
{
	if (array.isNotNull()) {
		data = array->getData();
		count = array->getCount();
	} else {
		data = sl_null;
		count = 0;
	}
}

template <class T, class COMPARE>
ArrayPosition<T, COMPARE>::ArrayPosition(const ArrayPosition<T, COMPARE>& other) = default;

template <class T, class COMPARE>
ArrayPosition<T, COMPARE>::ArrayPosition(ArrayPosition<T, COMPARE>&& other) = default;

template <class T, class COMPARE>
SLIB_INLINE T& ArrayPosition<T, COMPARE>::operator*()
{
	return *data;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool ArrayPosition<T, COMPARE>::operator!=(const ArrayPosition<T, COMPARE>& other)
{
	return count > 0;
}

template <class T, class COMPARE>
SLIB_INLINE ArrayPosition<T, COMPARE>& ArrayPosition<T, COMPARE>::operator++()
{
	data++;
	count--;
	return *this;
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(Array, SafeArray, CArray, ref)

template <class T, class COMPARE>
Array<T, COMPARE>::Array(sl_size count) : ref(CArray<T, COMPARE>::create(count))
{
}

template <class T, class COMPARE>
Array<T, COMPARE>::Array(const T* data, sl_size count) : ref(CArray<T, COMPARE>::create(data, count))
{
}

template <class T, class COMPARE>
template <class _T>
Array<T, COMPARE>::Array(const _T* data, sl_size count) : ref(CArray<T, COMPARE>::create(data, count))
{
}

template <class T, class COMPARE>
Array<T, COMPARE>::Array(const T* data, sl_size count, const Referable* refer) : ref(CArray<T, COMPARE>::createStatic(data, count, refer))
{
}

template <class T, class COMPARE>
template <class _COMPARE>
SLIB_INLINE const Array<T, COMPARE>& Array<T, COMPARE>::from(const Array<T, _COMPARE>& other)
{
	return *((Array<T, COMPARE>*)((void*)&other));
}

template <class T, class COMPARE>
Array<T, COMPARE> Array<T, COMPARE>::create(sl_size count)
{
	return Array<T, COMPARE>(count);
}

template <class T, class COMPARE>
Array<T, COMPARE> Array<T, COMPARE>::create(const T* data, sl_size count)
{
	return Array<T, COMPARE>(data, count);
}

template <class T, class COMPARE>
template <class _T>
Array<T, COMPARE> Array<T, COMPARE>::create(const _T* data, sl_size count)
{
	return Array<T, COMPARE>(data, count);
}

template <class T, class COMPARE>
Array<T, COMPARE> Array<T, COMPARE>::createStatic(const T* data, sl_size count)
{
	return Array<T, COMPARE>(data, count, sl_null);
}

template <class T, class COMPARE>
Array<T, COMPARE> Array<T, COMPARE>::createStatic(const T* data, sl_size count, const Referable* refer)
{
	return Array<T, COMPARE>(data, count, refer);
}

template <class T, class COMPARE>
SLIB_INLINE T* Array<T, COMPARE>::getData() const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getData();
	}
	return sl_null;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size Array<T, COMPARE>::getCount() const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getCount();
	}
	return 0;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool Array<T, COMPARE>::isEmpty() const
{
	return isNull();
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool Array<T, COMPARE>::isNotEmpty() const
{
	return isNotNull();
}

template <class T, class COMPARE>
T* Array<T, COMPARE>::getItemPtr(sl_size index) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getItemPtr(index);
	}
	return sl_null;
}

template <class T, class COMPARE>
sl_bool Array<T, COMPARE>::getItem(sl_size index, T* _out) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getItem(index, _out);
	}
	return sl_false;
}

template <class T, class COMPARE>
T Array<T, COMPARE>::getItemValue(sl_size index, const T& def) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getItemValue(index, def);
	}
	return def;
}

template <class T, class COMPARE>
sl_bool Array<T, COMPARE>::setItem(sl_size index, const T& value) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->setItem(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
Array<T, COMPARE> Array<T, COMPARE>::sub(sl_size start, sl_size count) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->sub(start, count);
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
sl_reg Array<T, COMPARE>::indexOf(const T& value, sl_reg start) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->indexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg Array<T, COMPARE>::lastIndexOf(const T& value, sl_reg start) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->lastIndexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_bool Array<T, COMPARE>::contains(const T& value) const
{
	return indexOf(value) >= 0;
}

template <class T, class COMPARE>
sl_size Array<T, COMPARE>::read(sl_size startSource, sl_size len, T* dataDst) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->read(startSource, len, dataDst);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T>
sl_size Array<T, COMPARE>::read(sl_size startSource, sl_size len, _T* dataDst) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->read(startSource, len, dataDst);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size Array<T, COMPARE>::write(sl_size startTarget, sl_size len, const T* dataSrc) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->write(startTarget, len, dataSrc);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T>
sl_size Array<T, COMPARE>::write(sl_size startTarget, sl_size len, const _T* dataSrc) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->write(startTarget, len, dataSrc);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size Array<T, COMPARE>::copy(sl_size startTarget, const Array<T, COMPARE>& source, sl_size startSource, sl_size len) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->copy(startTarget, source.ref.ptr, startSource, len);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size Array<T, COMPARE>::copy(sl_size startTarget, const Array<_T, _COMPARE>& source, sl_size startSource, sl_size len) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->copy(startTarget, source.ref.ptr, startSource, len);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size Array<T, COMPARE>::copy(const Array<T, COMPARE>& source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size Array<T, COMPARE>::copy(const Array<_T, _COMPARE>& source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

template <class T, class COMPARE>
Array<T, COMPARE> Array<T, COMPARE>::duplicate() const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->duplicate();
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
Iterator<T> Array<T, COMPARE>::iterator() const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return new ArrayIterator<T, COMPARE>(obj);
	}
	return Iterator<T>::null();
}

template <class T, class COMPARE>
sl_bool Array<T, COMPARE>::getData(ArrayData<T>& data) const
{
	CArray<T, COMPARE>* obj = ref.ptr;
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

template <class T, class COMPARE>
T* Array<T, COMPARE>::begin() const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getData();
	}
	return sl_null;
}

template <class T, class COMPARE>
T* Array<T, COMPARE>::end() const
{
	CArray<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getData() + obj->getCount();
	}
	return sl_null;
}

SLIB_DEFINE_TEMPLATE_REF_WRAPPER(SafeArray, Array, CArray, ref)

template <class T, class COMPARE>
SafeArray<T, COMPARE>::SafeArray(sl_size count) : ref(CArray<T, COMPARE>::create(count))
{
}

template <class T, class COMPARE>
SafeArray<T, COMPARE>::SafeArray(const T* data, sl_size count) : ref(CArray<T, COMPARE>::create(data, count))
{
}

template <class T, class COMPARE>
template <class _T>
SafeArray<T, COMPARE>::SafeArray(const _T* data, sl_size count) : ref(CArray<T, COMPARE>::create(data, count))
{
}

template <class T, class COMPARE>
SafeArray<T, COMPARE>::SafeArray(const T* data, sl_size count, const Referable* refer) : ref(CArray<T, COMPARE>::createStatic(data, count, refer))
{
}

template <class T, class COMPARE>
template <class _COMPARE>
const SafeArray<T, COMPARE>& SafeArray<T, COMPARE>::from(const SafeArray<T, _COMPARE>& other)
{
	return *((SafeArray<T, COMPARE>*)((void*)&other));
}

template <class T, class COMPARE>
sl_size SafeArray<T, COMPARE>::getCount() const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount();
	}
	return 0;
}

template <class T, class COMPARE>
sl_bool SafeArray<T, COMPARE>::isEmpty() const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount() == 0;
	}
	return sl_true;
}

template <class T, class COMPARE>
sl_bool SafeArray<T, COMPARE>::isNotEmpty() const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount() != 0;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeArray<T, COMPARE>::getItem(sl_size index, T* _out) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getItem(index, _out);
	}
	return sl_false;
}

template <class T, class COMPARE>
T SafeArray<T, COMPARE>::getItemValue(sl_size index, const T& def) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getItemValue(index, def);
	}
	return def;
}

template <class T, class COMPARE>
sl_bool SafeArray<T, COMPARE>::setItem(sl_size index, const T& value) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->setItem(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
Array<T, COMPARE> SafeArray<T, COMPARE>::sub(sl_size start, sl_size count) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->sub(start, count);
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
sl_reg SafeArray<T, COMPARE>::indexOf(const T& value, sl_reg start) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->indexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg SafeArray<T, COMPARE>::lastIndexOf(const T& value, sl_reg start) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->lastIndexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_bool SafeArray<T, COMPARE>::contains(const T& value) const
{
	return indexOf(value) >= 0;
}

template <class T, class COMPARE>
sl_size SafeArray<T, COMPARE>::read(sl_size startSource, sl_size len, T* dataDst) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->read(startSource, len, dataDst);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T>
sl_size SafeArray<T, COMPARE>::read(sl_size startSource, sl_size len, _T* dataDst) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->read(startSource, len, dataDst);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size SafeArray<T, COMPARE>::write(sl_size startTarget, sl_size len, const T* dataSrc) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->write(startTarget, len, dataSrc);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T>
sl_size SafeArray<T, COMPARE>::write(sl_size startTarget, sl_size len, const _T* dataSrc) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->write(startTarget, len, dataSrc);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size SafeArray<T, COMPARE>::copy(sl_size startTarget, const Array<T, COMPARE>& source, sl_size startSource, sl_size len) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->copy(startTarget, source.ref.ptr, startSource, len);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size SafeArray<T, COMPARE>::copy(sl_size startTarget, const Array<_T, _COMPARE>& source, sl_size startSource, sl_size len) const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->copy(startTarget, source.ref.ptr, startSource, len);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size SafeArray<T, COMPARE>::copy(const Array<T, COMPARE>& source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size SafeArray<T, COMPARE>::copy(const Array<_T, _COMPARE>& source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

template <class T, class COMPARE>
Array<T, COMPARE> SafeArray<T, COMPARE>::duplicate() const
{
	Ref< CArray<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->duplicate();
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
sl_bool SafeArray<T, COMPARE>::getData(ArrayData<T>& data) const
{
	Array<T, COMPARE> obj(*this);
	return obj.getData(data);
}

template <class T, class COMPARE>
ArrayPosition<T, COMPARE> SafeArray<T, COMPARE>::begin() const
{
	return ArrayPosition<T, COMPARE>(ref);
}

template <class T, class COMPARE>
ArrayPosition<T, COMPARE> SafeArray<T, COMPARE>::end() const
{
	return ArrayPosition<T, COMPARE>();
}


template <class T, class COMPARE>
ArrayIterator<T, COMPARE>::ArrayIterator(const Array<T, COMPARE>& arr)
{
	m_index = 0;
	arr.getData(m_arr);
}

template <class T, class COMPARE>
sl_bool ArrayIterator<T, COMPARE>::hasNext()
{
	return (m_index < m_arr.count);
}

template <class T, class COMPARE>
sl_bool ArrayIterator<T, COMPARE>::next(T* _out)
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

template <class T, class COMPARE>
sl_reg ArrayIterator<T, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


SLIB_NAMESPACE_END

#endif
