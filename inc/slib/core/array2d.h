#ifndef CHECKHEADER_SLIB_CORE_ARRAY2D
#define CHECKHEADER_SLIB_CORE_ARRAY2D

#include "definition.h"
#include "reference.h"
#include "math.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN
template <class TYPE>
class SLIB_EXPORT Array2DObject : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(Array2DObject)
public:
	TYPE* m_data;
	sl_size m_width;
	sl_size m_height;
	sl_reg m_stride;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
private:
	Array2DObject()
	{
	}

public:
	~Array2DObject()
	{
		if (! m_flagStatic) {
			if (m_data) {
				TYPE* rp = m_data;
				for (sl_size i = 0; i < m_height; i++) {
					TYPE* p = rp;
					for (sl_size j = 0; j < m_width; j++) {
						p->~TYPE();
						p ++;
					}
					rp += m_stride;
				}
				Base::freeMemory(m_data);
			}
		}
	}
	
public:
	static Array2DObject<TYPE>* create(sl_size width, sl_size height, const TYPE* data = sl_null, sl_reg strideSrc = 0)
	{
		if (width == 0 || height == 0) {
			return sl_null;
		}
		if (strideSrc == 0) {
			strideSrc = width;
		}
		sl_size size = width * height * sizeof(TYPE);
		TYPE* dataNew = (TYPE*)(Base::createMemory(size));
		if (dataNew == sl_null) {
			return sl_null;
		}
		Array2DObject<TYPE>* ret = new Array2DObject<TYPE>;
		if (ret) {
			if (data) {
				TYPE* rp = dataNew;
				const TYPE* rq = data;
				for (sl_size i = 0; i < height; i++) {
					TYPE* p = rp;
					const TYPE* q = rq;
					for (sl_size j = 0; j < width; j++) {
						new (p) TYPE(*q);
						p ++;
						q ++;
					}
					rp += width;
					rq += strideSrc;
				}
			} else {
				TYPE* rp = dataNew;
				for (sl_size i = 0; i < height; i++) {
					TYPE* p = rp;
					for (sl_size j = 0; j < width; j++) {
						new (p) TYPE();
						p ++;
					}
					rp += width;
				}
			}
			ret->m_flagStatic = sl_false;
			ret->m_data = dataNew;
			ret->m_width = width;
			ret->m_height = height;
			ret->m_stride = width;
		} else {
			Base::freeMemory(dataNew);
		}
		return ret;
	}
	
	static Array2DObject<TYPE>* createStatic(const TYPE* data, sl_size width, sl_size height, sl_reg stride, const Ref<Referable>& refer) {
		if (data == sl_null || width == 0 || height == 0) {
			return sl_null;
		}
		if (stride == 0) {
			stride = width;
		}
		ArrayObject<TYPE>* ret = new ArrayObject<TYPE>();
		if (ret) {
			ret->m_flagStatic = sl_true;
			ret->m_data = (TYPE*)data;
			ret->m_width = width;
			ret->m_height = height;
			ret->m_stride = stride;
			ret->m_refer = refer;
		}
		return ret;
	}
	
};


template <class TYPE>
class SLIB_EXPORT ArrayInfo2D
{
public:
	TYPE* data;
	sl_size width;
	sl_size height;
	sl_reg stride;
	Ref<Referable> object;
	
	SLIB_INLINE TYPE& item(sl_reg x, sl_reg y)
	{
		return data[y * stride + x];
	}

};


/** auto-referencing object **/
template <class TYPE>
class SLIB_EXPORT Array2D
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(Array2D<TYPE>, Array2DObject<TYPE>)
	SLIB_DECLARE_OBJECT_WRAPPER(Array2D, Array2D<TYPE>, Array2DObject<TYPE>, Ref< Array2DObject<TYPE> >)
	
public:
	SLIB_INLINE Array2D(sl_size width, sl_size height) : m_object(Array2DObject<TYPE>::create(width, height))
	{
	}

	SLIB_INLINE Array2D(const TYPE* dataIn, sl_size width, sl_size height, sl_reg strideIn = 0) : m_object(Array2DObject<TYPE>::create(width, height, dataIn, strideIn))
	{
	}
	
	SLIB_INLINE static Array2D<TYPE> create(sl_size width, sl_size height)
	{
		Array2D<TYPE> ret(width, height);
		return ret;
	}

	SLIB_INLINE static Array2D<TYPE> create(const TYPE* dataIn, sl_size width, sl_size height, sl_reg strideIn = 0)
	{
		Array2D<TYPE> ret(dataIn, width, height, strideIn);
		return ret;
	}

	SLIB_INLINE static Array2D<TYPE> createStatic(const TYPE* data, sl_size width, sl_size height, sl_reg stride = 0)
	{
		Array2D<TYPE> ret;
		ret.m_object = Array2DObject<TYPE>::createStatic(data, width, height, stride, stride);
		return ret;
	}

	SLIB_INLINE static Array2D<TYPE> createStatic(const TYPE* data, sl_size width, sl_size height, const Ref<Referable>& refer)
	{
		Array2D<TYPE> ret;
		ret.m_object = Array2DObject<TYPE>::createStatic(data, width, height, width, refer);
		return ret;
	}

	SLIB_INLINE static Array2D<TYPE> createStatic(const TYPE* data, sl_size width, sl_size height, sl_reg stride, const Ref<Referable>& refer)
	{
		Array2D<TYPE> ret;
		ret.m_object = Array2DObject<TYPE>::createStatic(data, width, height, stride, refer);
		return ret;
	}

	Array2D<TYPE> sub(sl_size x, sl_size y, sl_size width, sl_size height)
	{
		Array2D<TYPE> ret;
		Array2D<TYPE> arr = *this;
		sl_size widthParent = arr.getWidth();
		sl_size heightParent = arr.getHeight();
		if (x >= widthParent || y >= heightParent) {
			return ret;
		}
		if (width > widthParent - x) {
			width = widthParent - x;
		}
		if (height > heightParent - y) {
			height = heightParent - y;
		}
		if (width == 0 || height == 0) {
			return ret;
		}
		sl_reg stride = arr.getStride();
		ret.m_object = Array2DObject<TYPE>::createStatic(arr.getBuf() + (x + (sl_reg)y * stride), width, height, stride, arr.getReference());
		return ret;
	}
	
	SLIB_INLINE TYPE* getBuf() const
	{
		Ref< Array2DObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->m_data;
		} else {
			return sl_null;
		}
	}

	SLIB_INLINE sl_reg getStride() const
	{
		Ref< Array2DObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->m_stride;
		} else {
			return sl_null;
		}
	}

	SLIB_INLINE sl_reg stride() const
	{
		return getStride();
	}

	SLIB_INLINE sl_size getWidth() const
	{
		Ref< Array2DObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->m_width;
		} else {
			return 0;
		}
	}

	SLIB_INLINE sl_reg width() const
	{
		return getWidth();
	}

	SLIB_INLINE sl_size getHeight() const
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->m_height;
		} else {
			return 0;
		}
	}

	SLIB_INLINE sl_size height() const
	{
		return getHeight();
	}

	SLIB_INLINE sl_size getSize() const
	{
		Ref< Array2DObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return Math::abs(object->m_stride) * object->m_height;
		} else {
			return 0;
		}
	}

	SLIB_INLINE sl_size size() const
	{
		return getSize();
	}

	SLIB_INLINE TYPE& item(sl_reg x, sl_reg y)
	{
		return m_object->m_data[y * m_object->m_stride + x];
	}

	Array<TYPE> getRow(sl_size y) const
	{
		Ref< Array2DObject<TYPE> > object = m_object;
		if (object.isNotNull() && y < object.m_object->m_height) {
			return Array<TYPE>::createStatic(object.m_object->m_data + ((sl_reg)y * object.m_object->m_stride), object.m_object->m_width, object.getReference());
		} else {
			return Array<TYPE>::null();
		}
	}
	
	template <class T>
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, T* dataDst, sl_reg strideDst = 0)
	{
		Array2D<TYPE> source = *this;
		TYPE* pSrc = source.getBuf();
		T* pDst = dataDst;
		if (pDst == sl_null || pSrc == sl_null) {
			return 0;
		}
		if (strideDst == 0) {
			strideDst = width;
		}
		sl_size widthSrc = source.getWidth();
		sl_size heightSrc = source.getHeight();
		if (x >= widthSrc || y >= heightSrc) {
			return 0;
		}
		if (width > widthSrc - x) {
			return 0;
		}
		if (height > heightSrc - y) {
			return 0;
		}
		sl_reg strideSrc = source.getStride();
		TYPE* rp = pSrc + (sl_reg)x + (sl_reg)y * strideSrc;
		T* rq = pDst;
		for (sl_size i = 0; i < height; i++) {
			TYPE* p = rp;
			T* q = rq;
			for (sl_size j = 0; j < width; j++) {
				*(q) = *(p);
				p ++;
				q ++;
			}
			rp += strideSrc;
			rq += strideDst;
		}
		return width * height;
	}
	
	template <class T>
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const T* dataSrc, sl_reg strideSrc = 0)
	{
		Array2D<TYPE> target = *this;
		TYPE* pDst = target.getBuf();
		const T* pSrc = dataSrc;
		if (pDst == sl_null || pSrc == sl_null) {
			return 0;
		}
		if (strideSrc == 0) {
			strideSrc = width;
		}
		sl_size widthDst = target.getWidth();
		sl_size heightDst = target.getHeight();
		if (x >= widthDst || y >= heightDst) {
			return 0;
		}
		if (width > widthDst - x) {
			return 0;
		}
		if (height > heightDst - y) {
			return 0;
		}
		sl_reg strideDst = target.getStride();
		TYPE* rp = pDst + ((sl_reg)x + (sl_reg)y * strideDst);
		const T* rq = pSrc;
		for (sl_size i = 0; i < height; i++) {
			TYPE* p = rp;
			T* q = rq;
			for (sl_size j = 0; j < width; j++) {
				*(p) = *(q);
				p ++;
				q ++;
			}
			rp += strideDst;
			rq += strideSrc;
		}
		return width * height;
	}
	
	template <class T>
	sl_size copy(sl_size xTarget, sl_size yTarget, Array2D<T> source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX)
	{
		T* pSrc = source.getBuf();
		if (pSrc == sl_null) {
			return 0;
		}
		sl_size widthSrc = source.getWidth();
		sl_size heightSrc = source.getHeight();
		if (xSource >= widthSrc || ySource >= heightSrc) {
			return 0;
		}
		if (width > widthSrc - xSource) {
			width = widthSrc - xSource;
		}
		if (height > heightSrc - ySource) {
			height = heightSrc - ySource;
		}
		sl_reg strideSrc = source.getStride();
		return write<T>(xTarget, yTarget, width, height, pSrc + ((sl_reg)xSource + (sl_reg)ySource * strideSrc), strideSrc);
	}
	
	template <class T>
	SLIB_INLINE sl_size copy(const Array2D<T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX)
	{
		return copy<T>(0, 0, source, xSource, ySource, width, height);
	}
	
	sl_bool getInfo(ArrayInfo2D<TYPE>& info)
	{
		Ref< Array2DObject<TYPE> > obj = m_object;
		if (obj.isNotNull()) {
			info.data = obj->m_data;
			info.stride = obj->m_stride;
			info.width = obj->m_width;
			info.height = obj->m_height;
			if (obj.m_flagStatic) {
				info.object = obj->m_refer;
			} else {
				info.object = obj.get();
			}
			return sl_true;
		} else {
			info.data = sl_null;
			info.stride = 0;
			info.width = 0;
			info.height = 0;
			info.object.setNull();
			return sl_false;
		}
	}
	
	Array2D<TYPE> duplicate() const
	{
		Array<TYPE> obj = *this;
		if (obj.isNotNull()) {
			return Array2D<TYPE>::create(obj->m_data, obj->m_width, obj->m_height, obj->m_stride);
		}
		return Array2D<TYPE>::null();
	}
};
SLIB_NAMESPACE_END

#endif
