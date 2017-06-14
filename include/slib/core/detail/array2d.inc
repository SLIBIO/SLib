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
	SLIB_INLINE T& ArrayData2D<T>::element(sl_reg x, sl_reg y) const noexcept
	{
		return data[y * stride + x];
	}
	
	template <class T>
	CArray2D<T>::CArray2D() noexcept
	{
		m_flagStatic = sl_true;
		m_data = sl_null;
		m_width = 0;
		m_height = 0;
		m_stride = 0;
	}
	
	template <class T>
	CArray2D<T>::CArray2D(sl_size width, sl_size height) noexcept
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
	template <class VALUE>
	CArray2D<T>::CArray2D(sl_size width, sl_size height, const VALUE* data, sl_size strideSrc) noexcept
	{
		if (width > 0 && height > 0) {
			if (strideSrc == 0) {
				strideSrc = width;
			}
			sl_size size = width * height * sizeof(T);
			T* dataNew = (T*)(Base::createMemory(size));
			if (dataNew) {
				T* rp = dataNew;
				const VALUE* rq = data;
				for (sl_size i = 0; i < height; i++) {
					T* p = rp;
					const VALUE* q = rq;
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
	CArray2D<T>::CArray2D(const T* data, sl_size width, sl_size height, sl_size stride, Referable* refer) noexcept
	{
		if (data && width > 0 && height > 0) {
			if (stride == 0) {
				stride = width;
			}
			m_flagStatic = sl_true;
			m_data = const_cast<T*>(data);
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
	CArray2D<T>::~CArray2D() noexcept
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
	CArray2D<T>* CArray2D<T>::create(sl_size width, sl_size height) noexcept
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
	template <class VALUE>
	CArray2D<T>* CArray2D<T>::create(sl_size width, sl_size height, const VALUE* data, sl_size strideSrc) noexcept
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
	CArray2D<T>* CArray2D<T>::createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer) noexcept
	{
		if (data && width > 0 && height > 0) {
			return new CArray2D<T>(data, width, height, stride, refer);
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE T* CArray2D<T>::getData() const noexcept
	{
		return m_data;
	}
	
	template <class T>
	SLIB_INLINE sl_size CArray2D<T>::getWidth() const noexcept
	{
		return m_width;
	}
	
	template <class T>
	SLIB_INLINE sl_size CArray2D<T>::getHeight() const noexcept
	{
		return m_height;
	}
	
	template <class T>
	SLIB_INLINE sl_reg CArray2D<T>::getStride() const noexcept
	{
		return m_stride;
	}
	
	template <class T>
	SLIB_INLINE sl_bool CArray2D<T>::isStatic() const noexcept
	{
		return m_flagStatic;
	}
	
	template <class T>
	SLIB_INLINE const Ref<Referable>& CArray2D<T>::getRefer() const noexcept
	{
		return m_refer;
	}
	
	template <class T>
	sl_bool CArray2D<T>::getAt(sl_reg x, sl_reg y, T* _out) const noexcept
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
	T CArray2D<T>::getValueAt(sl_reg x, sl_reg y) const noexcept
	{
		if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
			return m_data[y * m_stride + x];
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T CArray2D<T>::getValueAt(sl_reg x, sl_reg y, const T& def) const noexcept
	{
		if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
			return m_data[y * m_stride + x];
		}
		return def;
	}
	
	template <class T>
	sl_bool CArray2D<T>::setAt(sl_reg x, sl_reg y, const T& value) const noexcept
	{
		if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
			m_data[y * m_stride + x] = value;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	CArray2D<T>* CArray2D<T>::sub(sl_size x, sl_size y, sl_size width, sl_size height) noexcept
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
					return createStatic(m_data + (x + (sl_reg)y * m_stride), width, height, m_stride, m_refer._ptr);
				} else {
					return createStatic(m_data + (x + (sl_reg)y * m_stride), width, height, m_stride, this);
				}
			}
		}
		return sl_null;
	}
	
	template <class T>
	template <class VALUE>
	sl_size CArray2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, VALUE* dataDst, sl_reg strideDst) const noexcept
	{
		T* pSrc = m_data;
		VALUE* pDst = dataDst;
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
		VALUE* rq = pDst;
		for (sl_size i = 0; i < height; i++) {
			T* p = rp;
			VALUE* q = rq;
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
	template <class VALUE>
	sl_size CArray2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const VALUE* dataSrc, sl_reg strideSrc) const noexcept
	{
		T* pDst = m_data;
		const VALUE* pSrc = dataSrc;
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
		const VALUE* rq = pSrc;
		for (sl_size i = 0; i < height; i++) {
			T* p = rp;
			const VALUE* q = rq;
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
	template <class VALUE>
	sl_size CArray2D<T>::copy(sl_size xTarget, sl_size yTarget, const CArray2D<VALUE>* source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept
	{
		VALUE* pSrc = source->getData();
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
		return write<VALUE>(xTarget, yTarget, width, height, pSrc + ((sl_reg)xSource + (sl_reg)ySource * strideSrc), strideSrc);
	}
	
	template <class T>
	template <class VALUE>
	sl_size CArray2D<T>::copy(const CArray2D<VALUE>* source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept
	{
		return copy<VALUE>(0, 0, source, xSource, ySource, width, height);
	}
	
	template <class T>
	CArray2D<T>* CArray2D<T>::duplicate() const noexcept
	{
		return create(m_width, m_height, m_data, m_stride);
	}
	
	
	template <class T>
	Array2D<T> Array2D<T>::create(sl_size width, sl_size height) noexcept
	{
		return CArray2D<T>::create(width, height);
	}
	
	template <class T>
	template <class VALUE>
	Array2D<T> Array2D<T>::create(sl_size width, sl_size height, const VALUE* dataIn, sl_reg strideIn) noexcept
	{
		return CArray2D<T>::create(width, height, dataIn, strideIn);
	}
	
	template <class T>
	Array2D<T> Array2D<T>::createStatic(const T* data, sl_size width, sl_size height, sl_reg stride) noexcept
	{
		return CArray2D<T>::createStatic(data, width, height, stride, sl_null);
	}
	
	template <class T>
	Array2D<T> Array2D<T>::createStatic(const T* data, sl_size width, sl_size height, Referable* refer) noexcept
	{
		return CArray2D<T>::createStatic(data, width, height, width, refer);
	}
	
	template <class T>
	Array2D<T> Array2D<T>::createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer) noexcept
	{
		return CArray2D<T>::createStatic(data, width, height, stride, refer);
	}
	
	template <class T>
	SLIB_INLINE T* Array2D<T>::getData() const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_size Array2D<T>::getWidth() const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getWidth();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_size Array2D<T>::getHeight() const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getHeight();
		}
		return 0;
	}
	
	template <class T>
	SLIB_INLINE sl_reg Array2D<T>::getStride() const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getStride();
		}
		return 0;
	}
	
	template <class T>
	sl_bool Array2D<T>::getAt(sl_reg x, sl_reg y, T* _out) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getAt(x, y, _out);
		}
		return sl_false;
	}
	
	template <class T>
	T Array2D<T>::getValueAt(sl_reg x, sl_reg y) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(x, y);
		} else {
			return NullValue<T>::get();
		}
	}
	
	template <class T>
	T Array2D<T>::getValueAt(sl_reg x, sl_reg y, const T& def) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->getValueAt(x, y, def);
		}
		return def;
	}
	
	template <class T>
	sl_bool Array2D<T>::setAt(sl_reg x, sl_reg y, const T& value) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->setAt(x, y, value);
		}
		return sl_false;
	}
	
	template <class T>
	Array2D<T> Array2D<T>::sub(sl_size x, sl_size y, sl_size width, sl_size height) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->sub(x, y, width, height);
		}
		return sl_null;
	}
	
	template <class T>
	template <class VALUE>
	sl_size Array2D<T>::read(sl_size x, sl_size y, sl_size width, sl_size height, VALUE* dataDst, sl_reg strideDst) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->read(x, y, width, height, dataDst, strideDst);
		}
		return 0;
	}
	
	template <class T>
	template <class VALUE>
	sl_size Array2D<T>::write(sl_size x, sl_size y, sl_size width, sl_size height, const VALUE* dataSrc, sl_reg strideSrc) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->write(x, y, width, height, dataSrc, strideSrc);
		}
		return 0;
	}
	
	template <class T>
	template <class VALUE>
	sl_size Array2D<T>::copy(sl_size xTarget, sl_size yTarget, const Array2D<VALUE>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->copy(xTarget, yTarget, source.ref._ptr, xSource, ySource, width, height);
		}
		return 0;
	}
	
	template <class T>
	template <class VALUE>
	sl_size Array2D<T>::copy(const Array2D<VALUE>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept
	{
		return copy<VALUE>(0, 0, source, xSource, ySource, width, height);
	}
	
	template <class T>
	Array2D<T> Array2D<T>::duplicate() const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}
	
	template <class T>
	sl_bool Array2D<T>::getData(ArrayData2D<T>& data) const noexcept
	{
		CArray2D<T>* obj = ref._ptr;
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


	template <class T>
	sl_size Atomic< Array2D<T> >::getWidth() const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getWidth();
		}
		return 0;
	}

	template <class T>
	sl_size Atomic< Array2D<T> >::getHeight() const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getHeight();
		}
		return 0;
	}

	template <class T>
	sl_bool Atomic< Array2D<T> >::isEmpty() const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getWidth() == 0 || obj->getHeight() == 0;
		}
		return sl_true;
	}

	template <class T>
	sl_bool Atomic< Array2D<T> >::isNotEmpty() const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getWidth() != 0 && obj->getHeight() != 0;
		}
		return sl_false;
	}

	template <class T>
	sl_bool Atomic< Array2D<T> >::getAt(sl_reg x, sl_reg y, T* _out) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAt(x, y, _out);
		}
		return sl_false;
	}

	template <class T>
	T Atomic< Array2D<T> >::getValueAt(sl_reg x, sl_reg y) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(x, y);
		} else {
			return NullValue<T>::get();
		}
	}

	template <class T>
	T Atomic< Array2D<T> >::getValueAt(sl_reg x, sl_reg y, const T& def) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValueAt(x, y, def);
		}
		return def;
	}

	template <class T>
	sl_bool Atomic< Array2D<T> >::setAt(sl_reg x, sl_reg y, const T& value) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->setAt(x, y, value);
		}
		return sl_false;
	}

	template <class T>
	Array2D<T> Atomic< Array2D<T> >::sub(sl_size x, sl_size y, sl_size width, sl_size height) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->sub(x, y, width, height);
		}
		return sl_null;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array2D<T> >::read(sl_size x, sl_size y, sl_size width, sl_size height, VALUE* dataDst, sl_reg strideDst) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->read(x, y, width, height, dataDst, strideDst);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array2D<T> >::write(sl_size x, sl_size y, sl_size width, sl_size height, const VALUE* dataSrc, sl_reg strideSrc) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->write(x, y, width, height, dataSrc, strideSrc);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array2D<T> >::copy(sl_size xTarget, sl_size yTarget, const Array2D<VALUE>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->copy(xTarget, yTarget, source.getObject(), xSource, ySource, width, height);
		}
		return 0;
	}

	template <class T>
	template <class VALUE>
	sl_size Atomic< Array2D<T> >::copy(const Array2D<VALUE>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const noexcept
	{
		return copy<VALUE>(0, 0, source, xSource, ySource, width, height);
	}

	template <class T>
	Array2D<T> Atomic< Array2D<T> >::duplicate() const noexcept
	{
		Ref< CArray2D<T> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< Array2D<T> >::getData(ArrayData2D<T>& data) const noexcept
	{
		Array2D<T> obj(ref);
		return obj.getData(data);
	}

}
