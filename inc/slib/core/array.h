#ifndef CHECKHEADER_SLIB_CORE_ARRAY
#define CHECKHEADER_SLIB_CORE_ARRAY

#include "definition.h"

#include "algorithm.h"
#include "reference.h"
#include "iterator.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT ArrayInfo
{
public:
	T* data;
	sl_size count;
	Ref<Referable> refer;
	
public:
	SLIB_INLINE T& operator[](sl_reg index) const
	{
		return data[index];
	}
	
};

template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT CArray : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(CArray)
public:
	typedef CArray<T, COMPARE> _Type;

protected:
	T* m_data;
	sl_size m_count;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
protected:
	SLIB_INLINE CArray() {}
	
public:
	SLIB_INLINE CArray(sl_size count)
	{
		_init(count);
	}
	
	template <class _T>
	SLIB_INLINE CArray(const _T* data, sl_size count)
	{
		_init(data, count);
	}
	
	SLIB_INLINE CArray(const T* data, sl_size count, const Referable* refer)
	{
		_initStatic(data, count, refer);
	}

public:
	~CArray()
	{
		_free();
	}
	
public:
	static _Type* create(sl_size count)
	{
		if (count > 0) {
			_Type* ret = new _Type(count);
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
	SLIB_INLINE static _Type* create(const _T* data, sl_size count)
	{
		if (count > 0) {
			_Type* ret = new _Type(data, count);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}
	
	SLIB_INLINE static _Type* createStatic(const T* data, sl_size count, const Referable* refer)
	{
		if (data && count > 0) {
			return new _Type(data, count, refer);
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

	SLIB_INLINE sl_size getCount() const
	{
		return m_count;
	}

	SLIB_INLINE sl_size count() const
	{
		return m_count;
	}

	SLIB_INLINE sl_bool isStatic() const
	{
		return m_flagStatic;
	}

	SLIB_INLINE const Ref<Referable>& getRefer() const
	{
		return m_refer;
	}

public:
	SLIB_INLINE T& operator[](sl_reg index) const
	{
		return m_data[index];
	}

	SLIB_INLINE T* getItemPtr(sl_size index) const
	{
		if (index < m_count) {
			return m_data + index;
		}
		return sl_null;
	}

	SLIB_INLINE sl_bool getItem(sl_size index, T* _out = sl_null) const
	{
		if (index < m_count) {
			*_out = m_data[index];
			return sl_true;
		}
		return sl_false;
	}

	SLIB_INLINE T getItemValue(sl_size index, const T& def) const
	{
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}

	SLIB_INLINE sl_bool setItem(sl_size index, const T& value) const
	{
		if (index < m_count) {
			m_data[index] = value;
			return sl_true;
		}
		return sl_false;
	}

public:
	_Type* sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const
	{
		sl_size countParent = m_count;
		if (start < countParent) {
			if (count > countParent - start) {
				count = countParent - start;
			}
			if (count > 0) {
				if (m_flagStatic) {
					return createStatic(m_data + start, count, m_refer.get());
				} else {
					return createStatic(m_data + start, count, this);
				}
			}
		}
		return sl_null;
	}

	sl_reg indexOf(const T& value, sl_reg start = 0) const
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

	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const
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

	SLIB_INLINE sl_bool contains(const T& value) const
	{
		return indexOf(value) >= 0;
	}

	template <class _T>
	sl_size read(sl_size startSource, sl_size len, _T* dataDst) const
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

	template <class _T>
	sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const
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

	template <class _T, class _COMPARE>
	sl_size copy(sl_size startTarget, const CArray<_T, _COMPARE>* source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		if (source) {
			T* pSrc = source->data();
			if (pSrc) {
				sl_size countSrc = source->count();
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

	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(const CArray<_T, _COMPARE>* source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(0, source, start, len);
	}

	SLIB_INLINE _Type* duplicate() const
	{
		return create(m_data, m_count);
	}
	
protected:
	void _init(sl_size count)
	{
		if (count > 0) {
			sl_size size = count * sizeof(T);
			T* dataNew = (T*)(Base::createMemory(size));
			if (dataNew) {
				T* p = dataNew;
				for (sl_size i = 0; i < count; i++) {
					new (p) T();
					p ++;
				}
				m_flagStatic = sl_false;
				m_data = dataNew;
				m_count = count;
				return;
			}
		}
		m_flagStatic = sl_false;
		m_data = sl_null;
		m_count = 0;
	}
	
	template <class _T>
	void _init(const _T* data, sl_size count)
	{
		if (count > 0) {
			sl_size size = count * sizeof(T);
			T* dataNew = (T*)(Base::createMemory(size));
			if (dataNew) {
				T* p = dataNew;
				const T* q = data;
				for (sl_size i = 0; i < count; i++) {
					new (p) T(*q);
					p ++;
					q ++;
				}
				m_flagStatic = sl_false;
				m_data = dataNew;
				m_count = count;
				return;
			}
		}
		m_flagStatic = sl_false;
		m_data = sl_null;
		m_count = 0;
	}
	
	void _initStatic(const T* data, sl_size count, const Referable* refer)
	{
		if (data && count > 0) {
			m_flagStatic = sl_true;
			m_data = (T*)data;
			m_count = count;
			m_refer = refer;
			return;
		}
		m_flagStatic = sl_false;
		m_data = sl_null;
		m_count = 0;
	}
	
	void _free()
	{
		if (! m_flagStatic) {
			T* p = m_data;
			if (p) {
				for (sl_size i = 0; i < m_count; i++) {
					p->~T();
					p ++;
				}
				Base::freeMemory(m_data);
			}
		}
	}
};

template <class T, class COMPARE>
class SafeArray;

/** auto-referencing object **/
template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT Array
{
	typedef CArray<T, COMPARE> _Obj;
	typedef Array<T, COMPARE> _Type;
	typedef SafeArray<T, COMPARE> _SafeType;
	typedef Ref<_Obj> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(Array, _Obj, _Type, _SafeType)
	
public:
	SLIB_INLINE Array(sl_size count) : m_object(_Obj::create(count))
	{
	}
	
	template <class _T>
	SLIB_INLINE Array(const _T* data, sl_size count) : m_object(_Obj::create(data, count))
	{
	}

	SLIB_INLINE Array(const T* data, sl_size count, const Referable* refer) : m_object(_Obj::createStatic(data, count, refer))
	{
	}

public:
	template <class _COMPARE>
	SLIB_INLINE const _Type& from(const Array<T, _COMPARE>& other)
	{
		return *((_Type*)((void*)&other));
	}

public:
	SLIB_INLINE static _Type create(sl_size count)
	{
		return _Type(count);
	}

	template <class _T>
	SLIB_INLINE static _Type create(const _T* data, sl_size count)
	{
		return _Type(data, count);
	}

	SLIB_INLINE static _Type createStatic(const T* data, sl_size count)
	{
		return _Type(data, count, sl_null);
	}

	SLIB_INLINE static _Type createStatic(const T* data, sl_size count, const Referable* refer)
	{
		return _Type(data, count, refer);
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

	SLIB_INLINE sl_size getCount() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->count();
		}
		return 0;
	}

	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->count() == 0;
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->count() != 0;
		}
		return sl_false;
	}

public:
	SLIB_INLINE T& operator[](sl_reg index) const
	{
		return *(m_object->data() + index);
	}

	SLIB_INLINE T* getItemPtr(sl_size index) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getItemPtr(index);
		}
		return sl_null;
	}

	SLIB_INLINE sl_bool getItem(sl_size index, T* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getItem(index, _out);
		}
		return sl_false;
	}

	SLIB_INLINE T getItemValue(sl_size index, const T& def) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getItemValue(index, def);
		}
		return def;
	}

	SLIB_INLINE sl_bool setItem(sl_size index, const T& value) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->setItem(index, value);
		}
		return sl_false;
	}

public:
	SLIB_INLINE _Type sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->sub(start, count);
		}
		return _Type::null();
	}

	SLIB_INLINE sl_reg indexOf(const T& value, sl_reg start = 0) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->indexOf(value, start);
		}
		return -1;
	}

	SLIB_INLINE sl_reg lastIndexOf(const T& value, sl_reg start = -1) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->lastIndexOf(value, start);
		}
		return -1;
	}

	SLIB_INLINE sl_bool contains(const T& value) const
	{
		return indexOf(value) >= 0;
	}
	
	template <class _T>
	SLIB_INLINE sl_size read(sl_size startSource, sl_size len, _T* dataDst) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->read(startSource, len, dataDst);
		}
		return 0;
	}
	
	template <class _T>
	SLIB_INLINE sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->write(startTarget, len, dataSrc);
		}
		return 0;
	}

	template <class _T, class _COMPARE>
	sl_size copy(sl_size startTarget, const Array<_T, _COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->copy(startTarget, source.getObject(), startSource, len);
		}
		return 0;
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(const Array<_T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(0, source, start, len);
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(sl_size startTarget, const SafeArray<_T, _COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(startTarget, Array<_T, _COMPARE>(source), startSource, len);
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(const SafeArray<_T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(Array<_T, _COMPARE>(source), start, len);
	}
	
	SLIB_INLINE _Type duplicate() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate();
		}
		return _Type::null();
	}

public:
	Iterator<T> iterator() const;

	sl_bool getInfo(ArrayInfo<T>& info) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			info.data = obj->data();
			info.count = obj->count();
			if (obj->isStatic()) {
				info.refer = obj->getRefer();
			} else {
				info.refer = obj;
			}
			return sl_true;
		} else {
			info.data = sl_null;
			info.count = 0;
			info.refer.setNull();
			return sl_false;
		}
	}

};

template <class T, class COMPARE>
class SLIB_EXPORT ArrayIterator : public IIterator<T>
{
protected:
	ArrayInfo<T> m_arr;
	sl_size m_index;

public:
	SLIB_INLINE ArrayIterator(const Array<T, COMPARE>& arr)
	{
		m_index = 0;
		arr.getInfo(m_arr);
	}

public:
	// override
	sl_bool hasNext()
	{
		return (m_index < m_arr.count);
	}

	// override
	sl_bool next(T* _out)
	{
		sl_uint32 index = m_index;
		if (index < m_arr.count) {
			if (_out) {
				*_out = m_arr.data[index];
			}
			m_index = index + 1;
			return sl_true;
		}
		return sl_false;
	}

	// override
	sl_reg getIndex()
	{
		return (sl_reg)m_index - 1;
	}
};

template <class T, class COMPARE>
Iterator<T> Array<T, COMPARE>::iterator() const
{
	_Obj* obj = m_object.get();
	if (obj) {
		return new ArrayIterator<T, COMPARE>(obj);
	}
	return Iterator<T>::null();
}


/** auto-referencing object **/
template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT SafeArray
{
	typedef CArray<T, COMPARE> _Obj;
	typedef SafeArray<T, COMPARE> _Type;
	typedef Array<T, COMPARE> _LocalType;
	typedef SafeRef<_Obj> _Ref;
	typedef Ref<_Obj> _LocalRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_SAFE_WRAPPER(SafeArray, _Obj, _Type, _LocalType)

public:
	SLIB_INLINE SafeArray(sl_size count) : m_object(_Obj::create(count))
	{
	}

	template <class _T>
	SLIB_INLINE SafeArray(const _T* data, sl_size count) : m_object(_Obj::create(data, count))
	{
	}

	SLIB_INLINE SafeArray(const T* data, sl_size count, const Referable* refer) : m_object(_Obj::createStatic(data, count, refer))
	{
	}

public:
	template <class _COMPARE>
	SLIB_INLINE const _Type& from(const SafeArray<T, _COMPARE>& other)
	{
		return *((_Type*)((void*)&other));
	}

public:
	SLIB_INLINE sl_size getCount() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->count();
		}
		return 0;
	}

	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		return getCount() == 0;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return getCount() != 0;
	}

public:
	SLIB_INLINE sl_bool getItem(sl_size index, T* _out = sl_null) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getItem(index, _out);
		}
		return sl_false;
	}

	SLIB_INLINE T getItemValue(sl_size index, const T& def) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getItemValue(index, def);
		}
		return def;
	}

	SLIB_INLINE sl_bool setItem(sl_size index, const T& value) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->setItem(index, value);
		}
		return sl_false;
	}

public:
	SLIB_INLINE _LocalType sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->sub(start, count);
		}
		return _LocalType::null();
	}

	SLIB_INLINE sl_reg indexOf(const T& value, sl_reg start = 0) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->indexOf(value, start);
		}
		return -1;
	}

	SLIB_INLINE sl_reg lastIndexOf(const T& value, sl_reg start = -1) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->lastIndexOf(value, start);
		}
		return -1;
	}

	SLIB_INLINE sl_bool contains(const T& value) const
	{
		return indexOf(value) >= 0;
	}

	template <class _T>
	SLIB_INLINE sl_size read(sl_size startSource, sl_size len, _T* dataDst) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->read(startSource, len, dataDst);
		}
		return 0;
	}

	template <class _T>
	SLIB_INLINE sl_size write(sl_size startTarget, sl_size len, const _T* dataSrc) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->write(startTarget, len, dataSrc);
		}
		return 0;
	}

	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(sl_size startTarget, const Array<_T, _COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->copy(startTarget, source.getObject(), startSource, len);
		}
		return 0;
	}

	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(const Array<_T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(0, source, start, len);
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(sl_size startTarget, const SafeArray<_T, _COMPARE>& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(startTarget, Array<_T, _COMPARE>(source), startSource, len);
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_size copy(const SafeArray<_T, _COMPARE>& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(Array<_T, _COMPARE>(source), start, len);
	}

	SLIB_INLINE _LocalType duplicate() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return _LocalType::null();
	}

public:
	sl_bool getInfo(ArrayInfo<T>& info) const;

};

template <class T, class COMPARE>
SLIB_INLINE sl_bool SafeArray<T, COMPARE>::getInfo(ArrayInfo<T>& info) const
{
	_LocalType obj(*this);
	return obj.getInfo(info);
}

SLIB_NAMESPACE_END

#endif
