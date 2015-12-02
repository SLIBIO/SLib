#ifndef CHECKHEADER_SLIB_CORE_ARRAY
#define CHECKHEADER_SLIB_CORE_ARRAY

#include "definition.h"
#include "algorithm.h"
#include "reference.h"
#include "math.h"

SLIB_NAMESPACE_BEGIN
template <class TYPE>
class SLIB_EXPORT ArrayObject : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(ArrayObject)
public:
	TYPE* m_data;
	sl_size m_count;
	sl_size m_size;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
private:
	ArrayObject() {}
	
public:
	~ArrayObject()
	{
		if (! m_flagStatic) {
			if (m_data) {
				TYPE* p = m_data;
				for (sl_size i = 0; i < m_count; i++) {
					p->~TYPE();
					p ++;
				}
				Base::freeMemory(m_data);
			}
		}
	}
	
public:
	static ArrayObject<TYPE>* create(sl_size count, const TYPE* data = sl_null)
	{
		if (count == 0) {
			return sl_null;
		}
		sl_size size = count * sizeof(TYPE);
		TYPE* dataNew = (TYPE*)(Base::createMemory(size));
		if (dataNew == sl_null) {
			return sl_null;
		}
		ArrayObject<TYPE>* ret = new ArrayObject<TYPE>();
		if (ret) {
			if (data) {
				TYPE* p = dataNew;
				const TYPE* q = data;
				for (sl_size i = 0; i < count; i++) {
					new (p) TYPE(*q);
					p ++;
					q ++;
				}
			} else {
				TYPE* p = dataNew;
				for (sl_size i = 0; i < count; i++) {
					new (p) TYPE();
					p ++;
				}
			}
			ret->m_flagStatic = sl_false;
			ret->m_data = dataNew;
			ret->m_count = count;
			ret->m_size = size;
		} else {
			Base::freeMemory(dataNew);
		}
		return ret;
	}
	
	static ArrayObject<TYPE>* createStatic(const void* data, sl_size count, const Ref<Referable>& refer) {
		if (data == sl_null || count == 0) {
			return sl_null;
		}
		ArrayObject<TYPE>* ret = new ArrayObject<TYPE>();
		if (ret) {
			ret->m_flagStatic = sl_true;
			ret->m_data = (TYPE*)data;
			ret->m_count = count;
			ret->m_size = sizeof(TYPE) * count;
			ret->m_refer = refer;
		}
		return ret;
	}
};

template <class TYPE>
class SLIB_EXPORT ArrayInfo
{
public:
	TYPE* data;
	sl_size count;
	Ref<Referable> object;
	
	SLIB_INLINE TYPE& operator[](sl_reg index)
	{
		return data[index];
	}
};

/** auto-referencing object **/
template <class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT Array
{
	typedef Array<TYPE, COMPARE> _Array;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Array, ArrayObject<TYPE>)
	SLIB_DECLARE_OBJECT_WRAPPER(Array, _Array, ArrayObject<TYPE>, Ref< ArrayObject<TYPE> >)
	
public:
	SLIB_INLINE Array(sl_size count) : m_object(ArrayObject<TYPE>::create(count))
	{
	}
	
	SLIB_INLINE Array(const TYPE* data, sl_size count) : m_object(ArrayObject<TYPE>::create(count, data))
	{
	}
	
	SLIB_INLINE static Array<TYPE> create(sl_size count)
	{
		Array<TYPE> ret(count);
		return ret;
	}

	SLIB_INLINE static Array<TYPE> create(const TYPE* data, sl_size count)
	{
		Array<TYPE> ret(data, count);
		return ret;
	}

	SLIB_INLINE static Array<TYPE> createStatic(const TYPE* data, sl_size count)
	{
		Array<TYPE> ret;
		ret.m_object = ArrayObject<TYPE>::createStatic(data, count, sl_null);
		return ret;
	}

	SLIB_INLINE static Array<TYPE> createStatic(const TYPE* data, sl_size count, const Ref<Referable>& refer)
	{
		Array<TYPE> ret;
		ret.m_object = ArrayObject<TYPE>::createStatic(data, count, refer);
		return ret;
	}

	Array<TYPE> sub(sl_size start, sl_size count)
	{
		Array<TYPE> ret;
		Array<TYPE> arr = *this;
		sl_size countParent = arr.getCount();
		if (start >= countParent) {
			return ret;
		}
		if (count > countParent - start) {
			count = countParent - start;
		}
		if (count == 0) {
			return ret;
		}
		ret.m_object = ArrayObject<TYPE>::createStatic(arr.getBuf() + start, count, arr.getReference());
		return ret;
	}
	
	SLIB_INLINE TYPE* getBuf() const
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->m_data;
		} else {
			return sl_null;
		}
	}

	SLIB_INLINE sl_size getCount() const
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->m_count;
		} else {
			return 0;
		}
	}

	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}

	SLIB_INLINE TYPE& operator[](sl_reg index)
	{
		return *(m_object->m_data + index);
	}
	
	SLIB_INLINE TYPE* getItemPtr(sl_size index) const
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			if (index < object->m_count) {
				return object->m_data + index;
			}
		}
		return sl_null;
	}
	
	SLIB_INLINE sl_bool getItem(sl_size index, TYPE* _out = sl_null) const
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			if (index < object->m_count) {
				*_out = object->m_data[index];
				return sl_true;
			}
		}
		return sl_false;
	}
	
	SLIB_INLINE TYPE getItemValue(sl_size index, const TYPE& def) const
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			if (index < object->m_count) {
				return object->m_data[index];
			}
		}
		return def;
	}
	
	SLIB_INLINE sl_bool setItem(sl_size index, const TYPE& value)
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		if (object.isNotNull()) {
			if (index < object->m_count) {
				object->m_data[index] = value;
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_reg indexOf(const TYPE& value, sl_reg start = 0)
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		sl_reg ret = -1;
		if (object.isNotNull()) {
			sl_reg n = object->m_count;
			TYPE* p = object->m_data;
			if (start < 0) {
				start = 0;
			}
			for (sl_reg i = start; i < n; i++) {
				if (COMPARE::equals(p[i], value)) {
					ret = i;
					break;
				}
			}
		}
		return ret;
	}

	sl_reg lastIndexOf(const TYPE& value, sl_reg start = -1)
	{
		Ref< ArrayObject<TYPE> > object = m_object;
		sl_reg ret = -1;
		if (object.isNotNull()) {
			sl_reg n = object->m_count;
			TYPE* p = object->m_data;
			if (start < 0 || start >= n) {
				start = n - 1;
			}
			for (sl_reg i = start; i >= 0; i--) {
				if (COMPARE::equals(p[i], value)) {
					ret = i;
					break;
				}
			}
		}
		return ret;
	}

	SLIB_INLINE sl_bool contains(const TYPE& value)
	{
		return indexOf(value) >= 0;
	}
	
	template <class T>
	sl_size read(sl_size startSource, sl_size len, T* dataDst)
	{
		Array<TYPE> source = *this;
		TYPE* pSrc = source.getBuf();
		T* pDst = dataDst;
		if (pDst == sl_null || pSrc == sl_null) {
			return 0;
		}
		sl_size countSrc = source.getCount();
		if (startSource >= countSrc) {
			return 0;
		}
		sl_size lenSrc = countSrc - startSource;
		if (len > lenSrc) {
			len = lenSrc;
		}
		pSrc += startSource;
		for (sl_size i = 0; i < len; i++) {
			*(pDst) = *(pSrc);
			pDst ++;
			pSrc ++;
		}
		return len;
	}
	
	template <class T>
	sl_size write(sl_size startTarget, sl_size len, const T* dataSrc)
	{
		Array<TYPE> target = *this;
		TYPE* pDst = target.getBuf();
		const T* pSrc = dataSrc;
		if (pDst == sl_null || pSrc == sl_null) {
			return 0;
		}
		sl_size countDst = target.getCount();
		if (startTarget >= countDst) {
			return 0;
		}
		sl_size lenDst = countDst - startTarget;
		if (len > lenDst) {
			len = lenDst;
		}
		pDst += startTarget;
		for (sl_size i = 0; i < len; i++) {
			*(pDst) = *(pSrc);
			pDst ++;
			pSrc ++;
		}
		return len;
	}

	template <class T, class _COMPARE>
	sl_size copy(sl_size startTarget, Array<T, _COMPARE> source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX)
	{
		TYPE* pSrc = source.getBuf();
		if (pSrc == sl_null) {
			return 0;
		}
		sl_size countSrc = source.getCount();
		if (startSource >= countSrc) {
			return 0;
		}
		sl_size lenSrc = countSrc - startSource;
		if (len > lenSrc) {
			len = lenSrc;
		}
		return write<T>(startTarget, len, pSrc + startSource);
	}
	
	template <class T, class _COMPARE>
	SLIB_INLINE sl_size copy(const Array<T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX)
	{
		return copy(0, source, start, len);
	}
	
	sl_bool getInfo(ArrayInfo<TYPE>& info)
	{
		Ref< ArrayObject<TYPE> > obj = m_object;
		if (obj.isNotNull()) {
			info.data = obj->m_data;
			info.count = obj->m_count;
			if (obj->m_flagStatic) {
				info.object = obj->m_refer;
			} else {
				info.object = obj.get();
			}
			return sl_true;
		} else {
			info.data = sl_null;
			info.count = 0;
			info.object.setNull();
			return sl_false;
		}
	}
	
	SLIB_INLINE Array<TYPE> duplicate() const
	{
		Ref< ArrayObject<TYPE> > obj = m_object;
		if (obj.isNotNull()) {
			return Array<TYPE>::create(obj->m_data, obj->m_count);
		}
		return Array<TYPE>::null();
	}

	Iterator<TYPE> iterator() const;
};

template <class TYPE, class COMPARE>
class SLIB_EXPORT ArrayIterator : public IIterator<TYPE>
{
protected:
	ArrayInfo<TYPE> m_arr;
	sl_size m_index;

public:
	ArrayIterator(const Array<TYPE, COMPARE>& arr)
	{
		m_index = 0;
		arr.getInfo(m_arr);
	}

	~ArrayIterator()
	{
	}

	sl_bool hasNext()
	{
		return (m_index < m_arr.count);
	}

	sl_bool next(TYPE* out)
	{
		sl_uint32 index = m_index;
		if (index < m_arr.count) {
			if (out) {
				*out = m_arr.data[index];
			}
			m_index = index + 1;
			return sl_true;
		}
		return sl_false;
	}

	sl_reg getIndex()
	{
		return (sl_reg)m_index - 1;
	}
};

template <class TYPE, class COMPARE>
Iterator<TYPE> Array<TYPE, COMPARE>::iterator() const
{
	Array<TYPE, COMPARE> o = *this;
	if (o.isNotNull()) {
		return new ArrayIterator<TYPE, COMPARE>(o);
	} else {
		return Iterator<TYPE>::null();
	}
}

SLIB_NAMESPACE_END

#endif
