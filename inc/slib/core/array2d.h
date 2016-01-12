#ifndef CHECKHEADER_SLIB_CORE_ARRAY2D
#define CHECKHEADER_SLIB_CORE_ARRAY2D

#include "definition.h"

#include "reference.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT ArrayInfo2D
{
public:
	T* data;
	sl_size width;
	sl_size height;
	sl_reg stride;
	Ref<Referable> refer;

public:
	SLIB_INLINE T& item(sl_reg x, sl_reg y) const
	{
		return data[y * stride + x];
	}

};

template <class T>
class SLIB_EXPORT CArray2D : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(CArray2D)
public:
	typedef CArray2D<T> _Type;

protected:
	T* m_data;
	sl_size m_width;
	sl_size m_height;
	sl_reg m_stride;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
protected:
	SLIB_INLINE CArray2D() {}
	
public:
	SLIB_INLINE CArray2D(sl_size width, sl_size height)
	{
		_init(width, height);
	}
	
	template <class _T>
	SLIB_INLINE CArray2D(sl_size width, sl_size height, const _T* data, sl_size strideSrc = 0)
	{
		_init(width, height, data, strideSrc);
	}
	
	SLIB_INLINE CArray2D(const T* data, sl_size width, sl_size height, sl_size stride, const Referable* refer)
	{
		_initStatic(data, width, height, stride, refer);
	}

public:
	~CArray2D()
	{
		_free();
	}
	
public:
	static _Type* create(sl_size width, sl_size height)
	{
		if (width > 0 && height > 0) {
			_Type* ret = new _Type(width, height);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}
	
	template <class _T>
	static _Type* create(sl_size width, sl_size height, const _T* data, sl_size strideSrc = 0)
	{
		if (width > 0 && height > 0) {
			_Type* ret = new _Type(width, height, data, strideSrc);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}

	static _Type* createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer) {
		if (data && width > 0 && height > 0) {
			return new _Type(data, width, height, stride, refer);
		}
		return sl_null;
	}
	
public:
	SLIB_INLINE T* getData() const
	{
		return m_data;
	}

	SLIB_INLINE T* data() const
	{
		return m_data;
	}

	SLIB_INLINE sl_size getWidth() const
	{
		return m_width;
	}

	SLIB_INLINE sl_size width() const
	{
		return m_width;
	}

	SLIB_INLINE sl_size getHeight() const
	{
		return m_height;
	}

	SLIB_INLINE sl_size height() const
	{
		return m_height;
	}

	SLIB_INLINE sl_reg getStride() const
	{
		return m_stride;
	}

	SLIB_INLINE sl_reg stride() const
	{
		return m_stride;
	}

	SLIB_INLINE sl_bool isStatic() const
	{
		return m_flagStatic;
	}

	SLIB_INLINE const Ref<Referable>& getRefer() const
	{
		return m_refer;
	}

	SLIB_INLINE T& item(sl_reg x, sl_reg y) const
	{
		return m_data[y * m_stride + x];
	}

	SLIB_INLINE sl_bool getItem(sl_reg x, sl_reg y, T* _out = sl_null) const
	{
		if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
			if (_out) {
				*_out = m_data[y * m_stride + x];
			}
			return sl_true;
		}
		return sl_false;
	}

	SLIB_INLINE T getItemValue(sl_reg x, sl_reg y, const T& def) const
	{
		if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
			return m_data[y * m_stride + x];
		}
		return def;
	}

	SLIB_INLINE sl_bool setItem(sl_reg x, sl_reg y, const T& value) const
	{
		if (x >= 0 && x < (sl_reg)m_width && y >= 0 && y < (sl_reg)m_height) {
			m_data[y * m_stride + x] = value;
			return sl_true;
		}
		return sl_false;
	}

public:
	_Type* sub(sl_size x, sl_size y, sl_size width, sl_size height) const
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
					return createStatic(m_data + (x + (sl_reg)y * m_stride), width, height, m_stride, m_refer.get());
				} else {
					return createStatic(m_data + (x + (sl_reg)y * m_stride), width, height, m_stride, this);
				}
			}
		}
		return sl_null;
	}

	template <class _T>
	sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const
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

	template <class _T>
	sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const
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
			_T* q = rq;
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

	template <class _T>
	sl_size copy(sl_size xTarget, sl_size yTarget, const CArray2D<_T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const
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

	template <class _T>
	SLIB_INLINE sl_size copy(const CArray2D<_T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const
	{
		return copy<_T>(0, 0, source, xSource, ySource, width, height);
	}

	SLIB_INLINE _Type* duplicate() const
	{
		return create(m_width, m_height, m_data, m_stride);
	}
	
protected:
	void _init(sl_size width, sl_size height)
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
		m_flagStatic = sl_false;
		m_data = sl_null;
		m_width = 0;
		m_height = 0;
		m_stride = 0;
	}
	
	template <class _T>
	void _init(sl_size width, sl_size height, const _T* data, sl_reg strideSrc = 0)
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
		m_flagStatic = sl_false;
		m_data = sl_null;
		m_width = 0;
		m_height = 0;
		m_stride = 0;
	}
	
	void _initStatic(const T* data, sl_size width, sl_size height, sl_reg stride, const Referable* refer)
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
		m_flagStatic = sl_false;
		m_data = sl_null;
		m_width = 0;
		m_height = 0;
		m_stride = 0;
	}
	
	void _free()
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
};

template <class T>
class SafeArray2D;

/** auto-referencing object **/
template <class T>
class SLIB_EXPORT Array2D
{
	typedef CArray2D<T> _Obj;
	typedef Array2D<T> _Type;
	typedef SafeArray2D<T> _SafeType;
	typedef Ref<_Obj> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(Array2D, _Obj, _Type, _SafeType)
	
public:
	SLIB_INLINE Array2D(sl_size width, sl_size height) : m_object(_Obj::create(width, height))
	{
	}

	template <class _T>
	SLIB_INLINE Array2D(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0) : m_object(_Obj::create(width, height, dataIn, strideIn))
	{
	}

	SLIB_INLINE Array2D(const T* dataIn, sl_size width, sl_size height, sl_reg strideIn, const Referable* refer) : m_object(_Obj::createStatic(strideIn, width, height, dataIn, refer))
	{
	}

public:
	SLIB_INLINE static _Type create(sl_size width, sl_size height)
	{
		return _Type(width, height);
	}

	template <class _T>
	SLIB_INLINE static _Type create(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0)
	{
		return _Type(width, height, dataIn, strideIn);
	}

	SLIB_INLINE static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride = 0)
	{
		return _Type(data, width, height, stride, sl_null);
	}

	SLIB_INLINE static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, const Referable* refer)
	{
		return _Type(data, width, height, width, refer);
	}

	SLIB_INLINE static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, const Referable* refer)
	{
		return _Type(data, width, height, stride, refer);
	}

public:
	SLIB_INLINE T* getData() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->data();
		}
		return sl_null;
	}

	SLIB_INLINE T* data() const
	{
		return getData();
	}

	SLIB_INLINE sl_size getWidth() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->width();
		}
		return 0;
	}

	SLIB_INLINE sl_reg width() const
	{
		return getWidth();
	}

	SLIB_INLINE sl_size getHeight() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->height();
		}
		return 0;
	}

	SLIB_INLINE sl_size height() const
	{
		return getHeight();
	}

	SLIB_INLINE sl_reg getStride() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->stride();
		}
		return 0;
	}

	SLIB_INLINE sl_reg stride() const
	{
		return getStride();
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->width() == 0 || obj->height() == 0;
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->width() != 0 && obj->height() != 0;
		}
		return sl_false;
	}

	SLIB_INLINE T& item(sl_reg x, sl_reg y) const
	{
		return (m_object->data())[y * m_object->stride() + x];
	}

	SLIB_INLINE sl_bool getItem(sl_reg x, sl_reg y, T* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getItem(x, y, _out);
		}
		return sl_false;
	}

	SLIB_INLINE T getItemValue(sl_reg x, sl_reg y, const T& def) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getItemValue(x, y, def);
		}
		return def;
	}

	SLIB_INLINE sl_bool setItem(sl_reg x, sl_reg y, const T& value) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->setItem(x, y, value);
		}
		return sl_false;
	}

public:
	SLIB_INLINE _Type sub(sl_size x, sl_size y, sl_size width, sl_size height) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->sub(x, y, width, height);
		}
		return _Type::null();
	}

	template <class _T>
	SLIB_INLINE sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->read(x, y, width, height, dataDst, strideDst);
		}
		return 0;
	}
	
	template <class _T>
	SLIB_INLINE sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->write(x, y, width, height, dataSrc, strideSrc);
		}
		return 0;
	}
	
	template <class _T>
	SLIB_INLINE sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->copy(xTarget, yTarget, source.getObject(), xSource, ySource, width, height);
		}
		return 0;
	}
	
	template <class _T>
	SLIB_INLINE sl_size copy(const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const
	{
		return copy<_T>(0, 0, source, xSource, ySource, width, height);
	}

	SLIB_INLINE _Type duplicate() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate();
		}
		return _Type::null();
	}

	sl_bool getInfo(ArrayInfo2D<T>& info) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			info.data = obj->data();
			info.width = obj->width();
			info.height = obj->height();
			info.stride = obj->stride();
			if (obj->isStatic()) {
				info.refer = obj->getRefer();
			} else {
				info.refer = obj;
			}
			return sl_true;
		} else {
			info.data = sl_null;
			info.width = 0;
			info.height = 0;
			info.stride = 0;
			info.refer.setNull();
			return sl_false;
		}
	}
	
};

/** auto-referencing object **/
template <class T>
class SLIB_EXPORT SafeArray2D
{
	typedef CArray2D<T> _Obj;
	typedef SafeArray2D<T> _Type;
	typedef Array2D<T> _LocalType;
	typedef SafeRef<_Obj> _Ref;
	typedef Ref<_Obj> _LocalRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_SAFE_WRAPPER(SafeArray2D, _Obj, _Type, _LocalType)

public:
	SLIB_INLINE SafeArray2D(sl_size width, sl_size height) : m_object(_Obj::create(width, height))
	{
	}
	
	template <class _T>
	SLIB_INLINE SafeArray2D(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0) : m_object(_Obj::create(width, height, dataIn, strideIn))
	{
	}
	
	SLIB_INLINE SafeArray2D(const T* dataIn, sl_size width, sl_size height, sl_reg strideIn, const Referable* refer) : m_object(_Obj::createStatic(strideIn, width, height, dataIn, refer))
	{
	}

public:
	SLIB_INLINE sl_size getWidth() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->width();
		}
		return 0;
	}

	SLIB_INLINE sl_reg width() const
	{
		return getWidth();
	}

	SLIB_INLINE sl_size getHeight() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->height();
		}
		return 0;
	}

	SLIB_INLINE sl_size height() const
	{
		return getHeight();
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->width() == 0 || obj->height() == 0;
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->width() != 0 && obj->height() != 0;
		}
		return sl_false;
	}

	SLIB_INLINE sl_bool getItem(sl_reg x, sl_reg y, T* _out = sl_null) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getItem(x, y, _out);
		}
		return sl_false;
	}

	SLIB_INLINE T getItemValue(sl_reg x, sl_reg y, const T& def) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getItemValue(x, y, def);
		}
		return def;
	}

	SLIB_INLINE sl_bool setItem(sl_reg x, sl_reg y, const T& value) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->setItem(x, y, value);
		}
		return sl_false;
	}

public:
	SLIB_INLINE _LocalType sub(sl_size x, sl_size y, sl_size width, sl_size height) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->sub(x, y, width, height);
		}
		return _LocalType::null();
	}

	template <class _T>
	SLIB_INLINE sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->read(x, y, width, height, dataDst, strideDst);
		}
		return 0;
	}

	template <class _T>
	SLIB_INLINE sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->write(x, y, width, height, dataSrc, strideSrc);
		}
		return 0;
	}

	template <class _T>
	SLIB_INLINE sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->copy(xTarget, yTarget, source.getObject(), xSource, ySource, width, height);
		}
		return 0;
	}

	template <class _T>
	SLIB_INLINE sl_size copy(const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const
	{
		return copy<_T>(0, 0, source, xSource, ySource, width, height);
	}

	SLIB_INLINE _LocalType duplicate() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return _LocalType::null();
	}

	sl_bool getInfo(ArrayInfo2D<T>& info) const;
};

template <class T>
SLIB_INLINE sl_bool SafeArray2D<T>::getInfo(ArrayInfo2D<T>& info) const
{
	_LocalType obj(*this);
	return obj.getInfo(info);
}

SLIB_NAMESPACE_END

#endif
