#ifndef CHECKHEADER_SLIB_CORE_ARRAY2D
#define CHECKHEADER_SLIB_CORE_ARRAY2D

#include "definition.h"

#include "ref.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT ArrayData2D
{
public:
	T* data;
	sl_size width;
	sl_size height;
	sl_reg stride;
	Ref<Referable> refer;
	
public:
	T& item(sl_reg x, sl_reg y) const;
	
};

class SLIB_EXPORT CArray2DBase : public Referable
{
	SLIB_DECLARE_OBJECT
};

template <class T>
class SLIB_EXPORT CArray2D : public CArray2DBase
{
protected:
	T* m_data;
	sl_size m_width;
	sl_size m_height;
	sl_reg m_stride;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
protected:
	CArray2D();
	
public:
	CArray2D(sl_size width, sl_size height);
	
	CArray2D(sl_size width, sl_size height, const T* data, sl_size strideSrc = 0);
	
	template <class _T>
	CArray2D(sl_size width, sl_size height, const _T* data, sl_size strideSrc = 0);
	
	CArray2D(const T* data, sl_size width, sl_size height, sl_size stride, const Referable* refer);
	
	~CArray2D();
	
public:
	static CArray2D<T>* create(sl_size width, sl_size height);
	
	static CArray2D<T>* create(sl_size width, sl_size height, const T* data, sl_size strideSrc = 0);
	
	template <class _T>
	static CArray2D<T>* create(sl_size width, sl_size height, const _T* data, sl_size strideSrc = 0);
	
	static CArray2D<T>* createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, const Referable* refer);
	
public:
	T* getData() const;
	
	sl_size getWidth() const;
	
	sl_size getHeight() const;
	
	sl_reg getStride() const;
	
	sl_bool isStatic() const;
	
	const Ref<Referable>& getRefer() const;
	
	sl_bool getItem(sl_reg x, sl_reg y, T* _out = sl_null) const;
	
	T getItemValue(sl_reg x, sl_reg y, const T& def) const;
	
	sl_bool setItem(sl_reg x, sl_reg y, const T& value) const;
	
public:
	CArray2D<T>* sub(sl_size x, sl_size y, sl_size width, sl_size height) const;
	
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst = 0) const;
	
	template <class _T>
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const;
	
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc = 0) const;
	
	template <class _T>
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const;
	
	sl_size copy(sl_size xTarget, sl_size yTarget, const CArray2D<T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	template <class _T>
	sl_size copy(sl_size xTarget, sl_size yTarget, const CArray2D<_T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	sl_size copy(const CArray2D<T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	template <class _T>
	sl_size copy(const CArray2D<_T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	CArray2D<T>* duplicate() const;
	
};

template <class T>
class SafeArray2D;

#define SLIB_TEMPLATE_PARAMS_CArray2D T
#define SLIB_TEMPLATE_DEF_PARAMS_CArray2D class T

/** auto-referencing object **/
template <class T>
class SLIB_EXPORT Array2D
{
public:
	Ref< CArray2D<T> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(Array2D, SafeArray2D, CArray2D)
	
public:
	Array2D(sl_size width, sl_size height);
	
	Array2D(sl_size width, sl_size height, const T* dataIn, sl_reg strideIn = 0);
	
	template <class _T>
	Array2D(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0);
	
	Array2D(const T* dataIn, sl_size width, sl_size height, sl_reg strideIn, const Referable* refer);
	
public:
	static Array2D<T> create(sl_size width, sl_size height);
	
	static Array2D<T> create(sl_size width, sl_size height, const T* dataIn, sl_reg strideIn = 0);
	
	template <class _T>
	static Array2D<T> create(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0);
	
	static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride = 0);
	
	static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, const Referable* refer);
	
	static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, const Referable* refer);
	
public:
	T* getData() const;
	
	sl_size getWidth() const;
	
	sl_size getHeight() const;
	
	sl_reg getStride() const;
	
	sl_bool getItem(sl_reg x, sl_reg y, T* _out = sl_null) const;
	
	T getItemValue(sl_reg x, sl_reg y, const T& def) const;
	
	sl_bool setItem(sl_reg x, sl_reg y, const T& value) const;
	
public:
	Array2D<T> sub(sl_size x, sl_size y, sl_size width, sl_size height) const;
	
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst = 0) const;
	
	template <class _T>
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const;
	
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc = 0) const;
	
	template <class _T>
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const;
	
	sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const;
	
	template <class _T>
	sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const;
	
	sl_size copy(const Array2D<T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const;
	
	template <class _T>
	sl_size copy(const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const;
	
	Array2D<T> duplicate() const;
	
	sl_bool getData(ArrayData2D<T>& data) const;
	
};

/** auto-referencing object **/
template <class T>
class SLIB_EXPORT SafeArray2D
{
public:
	SafeRef< CArray2D<T> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(SafeArray2D, Array2D, CArray2D)
	
public:
	SafeArray2D(sl_size width, sl_size height);
	
	SafeArray2D(sl_size width, sl_size height, const T* dataIn, sl_reg strideIn = 0);
	
	template <class _T>
	SafeArray2D(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0);
	
	SafeArray2D(const T* dataIn, sl_size width, sl_size height, sl_reg strideIn, const Referable* refer);
	
public:
	sl_size getWidth() const;
	
	sl_size getHeight() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	sl_bool getItem(sl_reg x, sl_reg y, T* _out = sl_null) const;
	
	T getItemValue(sl_reg x, sl_reg y, const T& def) const;
	
	sl_bool setItem(sl_reg x, sl_reg y, const T& value) const;
	
public:
	Array2D<T> sub(sl_size x, sl_size y, sl_size width, sl_size height) const;
	
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst) const;
	
	template <class _T>
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const;
	
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc = 0) const;
	
	template <class _T>
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const;
	
	sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	template <class _T>
	sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	sl_size copy(const Array2D<T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	template <class _T>
	sl_size copy(const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
	Array2D<T> duplicate() const;
	
	sl_bool getData(ArrayData2D<T>& data) const;
	
};

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T>
SLIB_INLINE T& ArrayData2D<T>::item(sl_reg x, sl_reg y) const
{
	return data[y * stride + x];
}

template <class T>
SLIB_INLINE CArray2D<T>::CArray2D()
{
}

template <class T>
CArray2D<T>::CArray2D(sl_size width, sl_size height)
{
	if (width > 0 && height > 0) {
		sl_size size = width * height * sizeof(T);
		T* dataNew = (T*)(Base::createMemory(size));
		if (dataNew) {
			T* rp = dataNew;
			for (sl_size i = 0; i < height; i++) {
				T* p = rp;
				for (sl_size j = 0; j < width; j++) {
					new (p) T();
					p ++;
				}
				rp += width;
			}
			m_flagStatic = sl_false;
			m_data = dataNew;
			m_width = width;
			m_height = height;
			m_stride = width;
			return;
		}
	}
	m_flagStatic = sl_true;
	m_data = sl_null;
	m_width = 0;
	m_height = 0;
	m_stride = 0;
}

template <class T>
CArray2D<T>::CArray2D(sl_size width, sl_size height, const T* data, sl_size strideSrc)
{
	if (width > 0 && height > 0) {
		if (strideSrc == 0) {
			strideSrc = width;
		}
		sl_size size = width * height * sizeof(T);
		T* dataNew = (T*)(Base::createMemory(size));
		if (dataNew) {
			T* rp = dataNew;
			const T* rq = data;
			for (sl_size i = 0; i < height; i++) {
				T* p = rp;
				const T* q = rq;
				for (sl_size j = 0; j < width; j++) {
					new (p) T(*q);
					p ++;
					q ++;
				}
				rp += width;
				rq += strideSrc;
			}
			m_flagStatic = sl_false;
			m_data = dataNew;
			m_width = width;
			m_height = height;
			m_stride = width;
			return;
		}
	}
	m_flagStatic = sl_true;
	m_data = sl_null;
	m_width = 0;
	m_height = 0;
	m_stride = 0;
}

template <class T>
template <class _T>
CArray2D<T>::CArray2D(sl_size width, sl_size height, const _T* data, sl_size strideSrc)
{
	if (width > 0 && height > 0) {
		if (strideSrc == 0) {
			strideSrc = width;
		}
		sl_size size = width * height * sizeof(T);
		T* dataNew = (T*)(Base::createMemory(size));
		if (dataNew) {
			T* rp = dataNew;
			const _T* rq = data;
			for (sl_size i = 0; i < height; i++) {
				T* p = rp;
				const _T* q = rq;
				for (sl_size j = 0; j < width; j++) {
					new (p) T(*q);
					p ++;
					q ++;
				}
				rp += width;
				rq += strideSrc;
			}
			m_flagStatic = sl_false;
			m_data = dataNew;
			m_width = width;
			m_height = height;
			m_stride = width;
			return;
		}
	}
	m_flagStatic = sl_true;
	m_data = sl_null;
	m_width = 0;
	m_height = 0;
	m_stride = 0;
}

template <class T>
CArray2D<T>::CArray2D(const T* data, sl_size width, sl_size height, sl_size stride, const Referable* refer)
{
	if (data && width > 0 && height > 0) {
		if (stride == 0) {
			stride = width;
		}
		m_flagStatic = sl_true;
		m_data = (T*)data;
		m_width = width;
		m_height = height;
		m_stride = stride;
		m_refer = refer;
	}
	m_flagStatic = sl_true;
	m_data = sl_null;
	m_width = 0;
	m_height = 0;
	m_stride = 0;
}

template <class T>
CArray2D<T>::~CArray2D()
{
	if (! m_flagStatic) {
		T* rp = m_data;
		if (rp) {
			for (sl_size i = 0; i < m_height; i++) {
				T* p = rp;
				for (sl_size j = 0; j < m_width; j++) {
					p->~T();
					p ++;
				}
				rp += m_stride;
			}
			Base::freeMemory(m_data);
		}
	}
}

template <class T>
CArray2D<T>* CArray2D<T>::create(sl_size width, sl_size height)
{
	if (width > 0 && height > 0) {
		CArray2D<T>* ret = new CArray2D<T>(width, height);
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
CArray2D<T>* CArray2D<T>::create(sl_size width, sl_size height, const T* data, sl_size strideSrc)
{
	if (width > 0 && height > 0) {
		CArray2D<T>* ret = new CArray2D<T>(width, height, data, strideSrc);
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
CArray2D<T>* CArray2D<T>::create(sl_size width, sl_size height, const _T* data, sl_size strideSrc)
{
	if (width > 0 && height > 0) {
		CArray2D<T>* ret = new CArray2D<T>(width, height, data, strideSrc);
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
CArray2D<T>* CArray2D<T>::createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, const Referable* refer) {
	if (data && width > 0 && height > 0) {
		return new CArray2D<T>(data, width, height, stride, refer);
	}
	return sl_null;
}

template <class T>
SLIB_INLINE T* CArray2D<T>::getData() const
{
	return m_data;
}

template <class T>
SLIB_INLINE sl_size CArray2D<T>::getWidth() const
{
	return m_width;
}

template <class T>
SLIB_INLINE sl_size CArray2D<T>::getHeight() const
{
	return m_height;
}

template <class T>
SLIB_INLINE sl_reg CArray2D<T>::getStride() const
{
	return m_stride;
}

template <class T>
SLIB_INLINE sl_bool CArray2D<T>::isStatic() const
{
	return m_flagStatic;
}

template <class T>
SLIB_INLINE const Ref<Referable>& CArray2D<T>::getRefer() const
{
	return m_refer;
}

template <class T>
sl_bool CArray2D<T>::getItem(sl_reg x, sl_reg y, T* _out) const
{
	if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
		if (_out) {
			*_out = m_data[y * m_stride + x];
		}
		return sl_true;
	}
	return sl_false;
}

template <class T>
T CArray2D<T>::getItemValue(sl_reg x, sl_reg y, const T& def) const
{
	if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
		return m_data[y * m_stride + x];
	}
	return def;
}

template <class T>
sl_bool CArray2D<T>::setItem(sl_reg x, sl_reg y, const T& value) const
{
	if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
		m_data[y * m_stride + x] = value;
		return sl_true;
	}
	return sl_false;
}

template <class T>
CArray2D<T>* CArray2D<T>::sub(sl_size x, sl_size y, sl_size width, sl_size height) const
{
	sl_size widthParent = m_width;
	sl_size heightParent = m_height;
	if (x < widthParent && y < heightParent) {
		if (width > widthParent - x) {
			width = widthParent - x;
		}
		if (height > heightParent - y) {
			height = heightParent - y;
		}
		if (width > 0 && height > 0) {
			if (m_flagStatic) {
				return createStatic(m_data + (x + (sl_reg)y * m_stride), width, height, m_stride, m_refer.ptr);
			} else {
				return createStatic(m_data + (x + (sl_reg)y * m_stride), width, height, m_stride, this);
			}
		}
	}
	return sl_null;
}


template <class T>
sl_size CArray2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst) const
{
	T* pSrc = m_data;
	T* pDst = dataDst;
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	if (strideDst == 0) {
		strideDst = width;
	}
	sl_size widthSrc = m_width;
	sl_size heightSrc = m_height;
	if (x >= widthSrc || y >= heightSrc) {
		return 0;
	}
	if (width > widthSrc - x) {
		return 0;
	}
	if (height > heightSrc - y) {
		return 0;
	}
	sl_reg strideSrc = m_stride;
	T* rp = pSrc + (sl_reg)x + (sl_reg)y * strideSrc;
	T* rq = pDst;
	for (sl_size i = 0; i < height; i++) {
		T* p = rp;
		T* q = rq;
		for (sl_size j = 0; j < width; j++) {
			*(q) = *(p);
			p++;
			q++;
		}
		rp += strideSrc;
		rq += strideDst;
	}
	return width * height;
}

template <class T>
template <class _T>
sl_size CArray2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst) const
{
	T* pSrc = m_data;
	_T* pDst = dataDst;
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	if (strideDst == 0) {
		strideDst = width;
	}
	sl_size widthSrc = m_width;
	sl_size heightSrc = m_height;
	if (x >= widthSrc || y >= heightSrc) {
		return 0;
	}
	if (width > widthSrc - x) {
		return 0;
	}
	if (height > heightSrc - y) {
		return 0;
	}
	sl_reg strideSrc = m_stride;
	T* rp = pSrc + (sl_reg)x + (sl_reg)y * strideSrc;
	_T* rq = pDst;
	for (sl_size i = 0; i < height; i++) {
		T* p = rp;
		_T* q = rq;
		for (sl_size j = 0; j < width; j++) {
			*(q) = *(p);
			p++;
			q++;
		}
		rp += strideSrc;
		rq += strideDst;
	}
	return width * height;
}

template <class T>
sl_size CArray2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc) const
{
	T* pDst = m_data;
	const T* pSrc = dataSrc;
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	if (strideSrc == 0) {
		strideSrc = width;
	}
	sl_size widthDst = m_width;
	sl_size heightDst = m_height;
	if (x >= widthDst || y >= heightDst) {
		return 0;
	}
	if (width > widthDst - x) {
		return 0;
	}
	if (height > heightDst - y) {
		return 0;
	}
	sl_reg strideDst = m_stride;
	T* rp = pDst + ((sl_reg)x + (sl_reg)y * strideDst);
	const T* rq = pSrc;
	for (sl_size i = 0; i < height; i++) {
		T* p = rp;
		const T* q = rq;
		for (sl_size j = 0; j < width; j++) {
			*(p) = *(q);
			p++;
			q++;
		}
		rp += strideDst;
		rq += strideSrc;
	}
	return width * height;
}

template <class T>
template <class _T>
sl_size CArray2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc) const
{
	T* pDst = m_data;
	const _T* pSrc = dataSrc;
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	if (strideSrc == 0) {
		strideSrc = width;
	}
	sl_size widthDst = m_width;
	sl_size heightDst = m_height;
	if (x >= widthDst || y >= heightDst) {
		return 0;
	}
	if (width > widthDst - x) {
		return 0;
	}
	if (height > heightDst - y) {
		return 0;
	}
	sl_reg strideDst = m_stride;
	T* rp = pDst + ((sl_reg)x + (sl_reg)y * strideDst);
	const _T* rq = pSrc;
	for (sl_size i = 0; i < height; i++) {
		T* p = rp;
		const _T* q = rq;
		for (sl_size j = 0; j < width; j++) {
			*(p) = *(q);
			p++;
			q++;
		}
		rp += strideDst;
		rq += strideSrc;
	}
	return width * height;
}

template <class T>
sl_size CArray2D<T>::copy(sl_size xTarget, sl_size yTarget, const CArray2D<T>* source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	T* pSrc = source->getData();
	if (pSrc == sl_null) {
		return 0;
	}
	sl_size widthSrc = source->getWidth();
	sl_size heightSrc = source->getHeight();
	if (xSource >= widthSrc || ySource >= heightSrc) {
		return 0;
	}
	if (width > widthSrc - xSource) {
		width = widthSrc - xSource;
	}
	if (height > heightSrc - ySource) {
		height = heightSrc - ySource;
	}
	sl_reg strideSrc = source->getStride();
	return write(xTarget, yTarget, width, height, pSrc + ((sl_reg)xSource + (sl_reg)ySource * strideSrc), strideSrc);
}

template <class T>
template <class _T>
sl_size CArray2D<T>::copy(sl_size xTarget, sl_size yTarget, const CArray2D<_T>* source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	_T* pSrc = source->getData();
	if (pSrc == sl_null) {
		return 0;
	}
	sl_size widthSrc = source->getWidth();
	sl_size heightSrc = source->getHeight();
	if (xSource >= widthSrc || ySource >= heightSrc) {
		return 0;
	}
	if (width > widthSrc - xSource) {
		width = widthSrc - xSource;
	}
	if (height > heightSrc - ySource) {
		height = heightSrc - ySource;
	}
	sl_reg strideSrc = source->getStride();
	return write<_T>(xTarget, yTarget, width, height, pSrc + ((sl_reg)xSource + (sl_reg)ySource * strideSrc), strideSrc);
}

template <class T>
sl_size CArray2D<T>::copy(const CArray2D<T>* source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	return copy(0, 0, source, xSource, ySource, width, height);
}

template <class T>
template <class _T>
sl_size CArray2D<T>::copy(const CArray2D<_T>* source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	return copy<_T>(0, 0, source, xSource, ySource, width, height);
}

template <class T>
CArray2D<T>* CArray2D<T>::duplicate() const
{
	return create(m_width, m_height, m_data, m_stride);
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(Array2D, SafeArray2D, CArray2D, ref)

template <class T>
Array2D<T>::Array2D(sl_size width, sl_size height) : ref(CArray2D<T>::create(width, height))
{
}

template <class T>
Array2D<T>::Array2D(sl_size width, sl_size height, const T* dataIn, sl_reg strideIn) : ref(CArray2D<T>::create(width, height, dataIn, strideIn))
{
}

template <class T>
template <class _T>
Array2D<T>::Array2D(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn) : ref(CArray2D<T>::create(width, height, dataIn, strideIn))
{
}

template <class T>
Array2D<T>::Array2D(const T* dataIn, sl_size width, sl_size height, sl_reg strideIn, const Referable* refer) : ref(CArray2D<T>::createStatic(dataIn, width, height, strideIn, refer))
{
}

template <class T>
Array2D<T> Array2D<T>::create(sl_size width, sl_size height)
{
	return Array2D<T>(width, height);
}

template <class T>
Array2D<T> Array2D<T>::create(sl_size width, sl_size height, const T* dataIn, sl_reg strideIn)
{
	return Array2D<T>(width, height, dataIn, strideIn);
}

template <class T>
template <class _T>
Array2D<T> Array2D<T>::create(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn)
{
	return Array2D<T>(width, height, dataIn, strideIn);
}

template <class T>
Array2D<T> Array2D<T>::createStatic(const T* data, sl_size width, sl_size height, sl_reg stride)
{
	return Array2D<T>(data, width, height, stride, sl_null);
}

template <class T>
Array2D<T> Array2D<T>::createStatic(const T* data, sl_size width, sl_size height, const Referable* refer)
{
	return Array2D<T>(data, width, height, width, refer);
}

template <class T>
Array2D<T> Array2D<T>::createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, const Referable* refer)
{
	return Array2D<T>(data, width, height, stride, refer);
}

template <class T>
SLIB_INLINE T* Array2D<T>::getData() const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->getData();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE sl_size Array2D<T>::getWidth() const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->getWidth();
	}
	return 0;
}

template <class T>
SLIB_INLINE sl_size Array2D<T>::getHeight() const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->getHeight();
	}
	return 0;
}

template <class T>
SLIB_INLINE sl_reg Array2D<T>::getStride() const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->getStride();
	}
	return 0;
}

template <class T>
sl_bool Array2D<T>::getItem(sl_reg x, sl_reg y, T* _out) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->getItem(x, y, _out);
	}
	return sl_false;
}

template <class T>
T Array2D<T>::getItemValue(sl_reg x, sl_reg y, const T& def) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->getItemValue(x, y, def);
	}
	return def;
}

template <class T>
sl_bool Array2D<T>::setItem(sl_reg x, sl_reg y, const T& value) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->setItem(x, y, value);
	}
	return sl_false;
}

template <class T>
Array2D<T> Array2D<T>::sub(sl_size x, sl_size y, sl_size width, sl_size height) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->sub(x, y, width, height);
	}
	return Array2D<T>::null();
}

template <class T>
sl_size Array2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->read(x, y, width, height, dataDst, strideDst);
	}
	return 0;
}

template <class T>
template <class _T>
sl_size Array2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->read(x, y, width, height, dataDst, strideDst);
	}
	return 0;
}

template <class T>
sl_size Array2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->write(x, y, width, height, dataSrc, strideSrc);
	}
	return 0;
}

template <class T>
template <class _T>
sl_size Array2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->write(x, y, width, height, dataSrc, strideSrc);
	}
	return 0;
}

template <class T>
sl_size Array2D<T>::copy(sl_size xTarget, sl_size yTarget, const Array2D<T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->copy(xTarget, yTarget, source.ref.ptr, xSource, ySource, width, height);
	}
	return 0;
}

template <class T>
template <class _T>
sl_size Array2D<T>::copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->copy(xTarget, yTarget, source.ref.ptr, xSource, ySource, width, height);
	}
	return 0;
}

template <class T>
sl_size Array2D<T>::copy(const Array2D<T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	return copy(0, 0, source, xSource, ySource, width, height);
}

template <class T>
template <class _T>
sl_size Array2D<T>::copy(const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	return copy<_T>(0, 0, source, xSource, ySource, width, height);
}

template <class T>
Array2D<T> Array2D<T>::duplicate() const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		return obj->duplicate();
	}
	return Array2D<T>::null();
}

template <class T>
sl_bool Array2D<T>::getData(ArrayData2D<T>& data) const
{
	CArray2D<T>* obj = ref.ptr;
	if (obj) {
		data.data = obj->getData();
		data.width = obj->getWidth();
		data.height = obj->getHeight();
		data.stride = obj->getStride();
		if (obj->isStatic()) {
			data.refer = obj->getRefer();
		} else {
			data.refer = obj;
		}
		return sl_true;
	} else {
		data.data = sl_null;
		data.width = 0;
		data.height = 0;
		data.stride = 0;
		data.refer.setNull();
		return sl_false;
	}
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(SafeArray2D, Array2D, CArray2D, ref)

template <class T>
SafeArray2D<T>::SafeArray2D(sl_size width, sl_size height) : ref(CArray2D<T>::create(width, height))
{
}

template <class T>
SafeArray2D<T>::SafeArray2D(sl_size width, sl_size height, const T* dataIn, sl_reg strideIn) : ref(CArray2D<T>::create(width, height, dataIn, strideIn))
{
}

template <class T>
template <class _T>
SafeArray2D<T>::SafeArray2D(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn) : ref(CArray2D<T>::create(width, height, dataIn, strideIn))
{
}

template <class T>
SafeArray2D<T>::SafeArray2D(const T* dataIn, sl_size width, sl_size height, sl_reg strideIn, const Referable* refer) : ref(CArray2D<T>::createStatic(dataIn, width, height, strideIn, refer))
{
}

template <class T>
sl_size SafeArray2D<T>::getWidth() const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getWidth();
	}
	return 0;
}

template <class T>
sl_size SafeArray2D<T>::getHeight() const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getHeight();
	}
	return 0;
}

template <class T>
sl_bool SafeArray2D<T>::isEmpty() const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getWidth() == 0 || obj->getHeight() == 0;
	}
	return sl_true;
}

template <class T>
sl_bool SafeArray2D<T>::isNotEmpty() const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getWidth() != 0 && obj->getHeight() != 0;
	}
	return sl_false;
}

template <class T>
sl_bool SafeArray2D<T>::getItem(sl_reg x, sl_reg y, T* _out) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getItem(x, y, _out);
	}
	return sl_false;
}

template <class T>
T SafeArray2D<T>::getItemValue(sl_reg x, sl_reg y, const T& def) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getItemValue(x, y, def);
	}
	return def;
}

template <class T>
sl_bool SafeArray2D<T>::setItem(sl_reg x, sl_reg y, const T& value) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->setItem(x, y, value);
	}
	return sl_false;
}

template <class T>
Array2D<T> SafeArray2D<T>::sub(sl_size x, sl_size y, sl_size width, sl_size height) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->sub(x, y, width, height);
	}
	return Array2D<T>::null();
}

template <class T>
sl_size SafeArray2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->read(x, y, width, height, dataDst, strideDst);
	}
	return 0;
}

template <class T>
template <class _T>
sl_size SafeArray2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->read(x, y, width, height, dataDst, strideDst);
	}
	return 0;
}

template <class T>
sl_size SafeArray2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->write(x, y, width, height, dataSrc, strideSrc);
	}
	return 0;
}

template <class T>
template <class _T>
sl_size SafeArray2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->write(x, y, width, height, dataSrc, strideSrc);
	}
	return 0;
}

template <class T>
sl_size SafeArray2D<T>::copy(sl_size xTarget, sl_size yTarget, const Array2D<T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->copy(xTarget, yTarget, source.getObject(), xSource, ySource, width, height);
	}
	return 0;
}

template <class T>
template <class _T>
sl_size SafeArray2D<T>::copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->copy(xTarget, yTarget, source.getObject(), xSource, ySource, width, height);
	}
	return 0;
}

template <class T>
sl_size SafeArray2D<T>::copy(const Array2D<T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	return copy(0, 0, source, xSource, ySource, width, height);
}

template <class T>
template <class _T>
sl_size SafeArray2D<T>::copy(const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const
{
	return copy<_T>(0, 0, source, xSource, ySource, width, height);
}

template <class T>
Array2D<T> SafeArray2D<T>::duplicate() const
{
	Ref< CArray2D<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->duplicate();
	}
	return Array2D<T>::null();
}

template <class T>
SLIB_INLINE sl_bool SafeArray2D<T>::getData(ArrayData2D<T>& data) const
{
	Array2D<T> obj(*this);
	return obj.getData(data);
}

SLIB_NAMESPACE_END

#endif
