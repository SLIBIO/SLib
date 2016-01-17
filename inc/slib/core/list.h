#ifndef CHECKHEADER_SLIB_CORE_LIST
#define CHECKHEADER_SLIB_CORE_LIST

#include "definition.h"

#include "object.h"
#include "iterator.h"
#include "array.h"
#include "algorithm.h"

#define SLIB_LIST_CAPACITY_MIN 5

SLIB_NAMESPACE_BEGIN

template < class T, class COMPARE=Compare<T> >
class SLIB_EXPORT CList : public Object
{
	SLIB_DECLARE_OBJECT(CList, Object)
public:
	typedef CList<T, COMPARE> _Type;

protected:
	T* m_data;
	sl_size m_count;
	sl_size m_capacity;
	
public:
	SLIB_INLINE CList()
	{
		_init();
	}

	SLIB_INLINE CList(sl_size count)
	{
		_init(count);
	}
	
	template <class _T>
	SLIB_INLINE CList(const _T* values, sl_size count)
	{
		_init(values, count);
	}
	
	~CList()
	{
		_free();
	}
	
public:
	SLIB_INLINE static _Type* create()
	{
		return new _Type;
	}
	
	static _Type* create(sl_size count)
	{
		if (count > 0) {
			_Type* ret = new _Type(count);
			if (ret) {
				if (ret->m_count > 0) {
					return ret;
				}
				delete ret;
			}
		} else {
			return new _Type;
		}
		return sl_null;
	}
	
	template <class _T>
	static _Type* create(const _T* values, sl_size count)
	{
		if (count > 0) {
			_Type* ret = new _Type(values, count);
			if (ret) {
				if (ret->m_count > 0) {
					return ret;
				}
				delete ret;
			}
		} else {
			return new _Type;
		}
		return sl_null;
	}
	
	template <class _T, class _COMPARE>
	static _Type* createCopy(CList<_T, _COMPARE>* other)
	{
		if (other) {
			return create(other->data(), other->count());
		}
		return sl_null;
	}
	
	static _Type* createFromElement(const T& value)
	{
		_Type* ret = create(1);
		if (ret) {
			if (ret->add_NoLock(value)) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}

public:
	SLIB_INLINE sl_size getCount() const
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size count() const
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size getCapacity() const
	{
		return m_capacity;
	}
	
	SLIB_INLINE sl_size capacity() const
	{
		return m_capacity;
	}
	
	SLIB_INLINE T* getData() const
	{
		return m_data;
	}
	
	SLIB_INLINE T* data() const
	{
		return m_data;
	}

public:
	SLIB_INLINE T& operator[](sl_size index) const
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
	
	SLIB_INLINE sl_bool getItem_NoLock(sl_size index, T* _out = sl_null) const
	{
		if (index < m_count) {
			if (_out) {
				*_out = m_data[index];
			}
			return sl_true;
		}
		return sl_false;
	}

	sl_bool getItem(sl_size index, T* _out = sl_null) const
	{
		ObjectLocker lock(this);
		return getItem_NoLock(index, _out);
	}
	
	SLIB_INLINE T getItemValue_NoLock(sl_size index, const T& def) const
	{
		if (index < m_count) {
			return m_data[index];
		}
		return def;
	}

	T getItemValue(sl_size index, const T& def) const
	{
		ObjectLocker lock(this);
		return getItemValue_NoLock(index, def);
	}
	
	SLIB_INLINE sl_bool setItem_NoLock(sl_size index, const T& value) const
	{
		if (index < m_count) {
			m_data[index] = value;
			return sl_true;
		}
		return sl_false;
	}

	sl_bool setItem(sl_size index, const T& value) const
	{
		ObjectLocker lock(this);
		return setItem_NoLock(index, value);
	}
	
public:
	SLIB_INLINE sl_bool setCount_NoLock(sl_size count)
	{
		return _setCountInner(count);
	}
	
	sl_bool setCount(sl_size count)
	{
		ObjectLocker lock(this);
		return setCount_NoLock(count);
	}
	
	template <class _T>
	sl_bool insert_NoLock(sl_size index, const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		if (_setCountInner(oldCount + count)) {
			if (index < oldCount) {
				{
					T* dst = m_data + m_count - 1;
					const T* src = m_data + oldCount - 1;
					sl_size n = oldCount - index;
					while (n) {
						*(dst--) = *(src--);
						n--;
					}
				}
				{
					T* dst = m_data + index;
					const _T* src = values;
					sl_size n = count;
					while (n) {
						*(dst++) = *(src++);
						n--;
					}
				}
			} else {
				T* dst = m_data + oldCount;
				const _T* src = values;
				sl_size n = count;
				while (n) {
					*(dst++) = *(src++);
					n--;
				}
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class _T>
	sl_bool insert(sl_size index, const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return insert_NoLock(index, values, count);
	}
	
	template <class _T, class _COMPARE>
	sl_bool insert(sl_size index, const CList<_T, _COMPARE>* other)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		ObjectLocker lock(this, other);
		sl_size count = other->count();
		if (count == 0) {
			return sl_true;
		}
		_T* otherData = other->data();
		sl_size oldCount = m_count;
		if (_setCountInner(oldCount + count)) {
			if (index < oldCount) {
				T* dst = m_data + m_count - 1;
				T* src = m_data + oldCount - 1;
				sl_size n = oldCount - index;
				while (n) {
					*(dst--) = *(src--);
					n--;
				}
				dst = m_data + index;
				for (sl_size i = 0; i < count; i++) {
					*(dst++) = otherData[i];
				}
			} else {
				T* dst = m_data + oldCount;
				const _T* src = otherData;
				sl_size n = count;
				while (n) {
					*(dst++) = *(src++);
					n--;
				}
			}
			return sl_true;
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool insert_NoLock(sl_size index, const T& value)
	{
		return insert_NoLock(index, &value, 1);
	}
	
	SLIB_INLINE sl_bool insert(sl_size index, const T& value)
	{
		return insert(index, &value, 1);
	}
	
	template <class _T>
	SLIB_INLINE sl_bool add_NoLock(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		sl_size oldCount = m_count;
		if (_setCountInner(oldCount + count)) {
			T* dst = m_data + oldCount;
			const _T* src = values;
			sl_size n = count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class _T>
	SLIB_INLINE sl_bool add(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		ObjectLocker lock(this);
		return add_NoLock(values, count);
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_bool add(const CList<_T, _COMPARE>* other)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		ObjectLocker lock(this, other);
		sl_size count = other->count();
		if (count == 0) {
			return sl_true;
		}
		_T* otherData = other->data();
		sl_size oldCount = m_count;
		if (_setCountInner(oldCount + count)) {
			T* dst = m_data + oldCount;
			for (sl_size i = 0; i < count; i++) {
				*(dst++) = otherData[i];
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool add_NoLock(const T& value)
	{
		sl_size oldCount = m_count;
		if (oldCount < m_capacity) {
			m_count++;
			new (m_data + oldCount) T(value);
		} else {
			if (!(_setCountInner(oldCount + 1))) {
				return sl_false;
			}
			m_data[oldCount] = value;
		}
		return sl_true;
	}

	sl_bool add(const T& value)
	{
		ObjectLocker lock(this);
		return add_NoLock(value);
	}
	
	SLIB_INLINE sl_bool addIfNotExist_NoLock(const T& value)
	{
		if (indexOf_NoLock(value) < 0) {
			return add_NoLock(value);
		}
		return sl_false;
	}

	sl_bool addIfNotExist(const T& value)
	{
		ObjectLocker lock(this);
		return addIfNotExist_NoLock(value);
	}
	
	template <class _T>
	sl_bool add_NoLock(const Iterator<_T>& iterator)
	{
		_T value;
		while (iterator.next(&value)) {
			if (!(add_NoLock(value))) {
				return sl_false;
			}
		}
		return sl_true;
	}

	template <class _T>
	sl_bool add(const Iterator<_T>& iterator)
	{
		ObjectLocker lock(this);
		return add_NoLock(iterator);
	}
	
	sl_size remove_NoLock(sl_size index, sl_size count)
	{
		if (index < m_count && count > 0) {
			if (count > m_count - index) {
				count = m_count - index;
			}
			T* dst = m_data + index;
			T* src = dst + count;
			sl_size n = m_count - index - count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
			_setCountInner(m_count - count);
			return count;
		}
		return 0;
	}

	sl_size remove(sl_size index, sl_size count)
	{
		ObjectLocker lock(this);
		return remove_NoLock(index, count);
	}
	
	sl_size removeValue_NoLock(const T& value, sl_bool flagAllValues = sl_false)
	{
		if (m_data) {
			sl_size n = m_count;
			for (sl_size i = 0; i < n;) {
				if (COMPARE::equals(m_data[i], value)) {
					for (sl_size j = i + 1; j < n; j++) {
						m_data[j - 1] = m_data[j];
					}
					n--;
					if (!flagAllValues) {
						break;
					}
				} else {
					i++;
				}
			}
			if (n < m_count) {
				_setCountInner(n);
				return m_count - n;
			}
		}
		return 0;
	}
	
	sl_size removeValue(const T& value, sl_bool flagAllValues = sl_false)
	{
		ObjectLocker lock(this);
		return removeValue_NoLock(value, flagAllValues);
	}
	
	sl_size removeAll_NoLock()
	{
		sl_size count = m_count;
		_setCountInner(0);
		return count;
	}

	sl_size removeAll()
	{
		ObjectLocker lock(this);
		return removeAll_NoLock();
	}
	
	sl_bool popFront_NoLock(T* _out = sl_null)
	{
		sl_size n = m_count;
		if (n > 0) {
			T* dst = m_data;
			T* src = dst + 1;
			if (_out) {
				*_out = *dst;
			}
			sl_size m = n - 1;
			while (m) {
				*(dst++) = *(src++);
				m--;
			}
			return _setCountInner(n - 1);
		}
		return sl_false;
	}
	
	sl_bool popFront(T* _out = sl_null)
	{
		ObjectLocker lock(this);
		return popFront_NoLock(_out);
	}
	
	sl_size popFront_NoLock(sl_size count)
	{
		sl_size n = m_count;
		if (n > 0 && count > 0) {
			if (count > n) {
				count = n;
			}
			T* dst = m_data;
			T* src = dst + count;
			sl_size m = n - count;
			while (m) {
				*(dst++) = *(src++);
				m--;
			}
			if (_setCountInner(n - count)) {
				return count;
			}
		}
		return 0;
	}

	sl_size popFront(sl_size count)
	{
		ObjectLocker lock(this);
		return popFront_NoLock(count);
	}
	
	sl_bool popBack_NoLock(T* _out = sl_null)
	{
		sl_size n = m_count;
		if (n > 0) {
			if (_out) {
				*_out = m_data[n - 1];
			}
			return _setCountInner(n - 1);
		}
		return sl_false;
	}
	
	sl_bool popBack(T* _out = sl_null)
	{
		ObjectLocker lock(this);
		return popBack_NoLock(_out);
	}

	sl_size popBack_NoLock(sl_size count)
	{
		sl_size n = m_count;
		if (n > 0 && count > 0) {
			if (count > n) {
				count = n;
			}
			if (_setCountInner(n - count)) {
				return count;
			}
		}
		return 0;
	}
	
	sl_size popBack(sl_size count)
	{
		ObjectLocker lock(this);
		return popBack_NoLock(count);
	}
	
	sl_reg indexOf_NoLock(const T& value, sl_reg start = 0) const
	{
		sl_reg ret = -1;
		if (start < 0) {
			start = 0;
		}
		for (sl_size i = start; i < m_count; i++) {
			if (COMPARE::equals(m_data[i], value)) {
				ret = i;
				break;
			}
		}
		return ret;
	}
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const
	{
		ObjectLocker lock(this);
		return indexOf_NoLock(value, start);
	}
	
	sl_reg lastIndexOf_NoLock(const T& value, sl_reg start = -1) const
	{
		sl_reg ret = -1;
		if (start < 0 || start >= (sl_reg)m_count) {
			start = m_count - 1;
		}
		for (sl_reg i = start; i >= 0; i--) {
			if (COMPARE::equals(m_data[i], value)) {
				ret = i;
				break;
			}
		}
		return ret;
	}
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const
	{
		ObjectLocker lock(this);
		return lastIndexOf_NoLock(value, start);
	}

	SLIB_INLINE sl_bool contains_NoLock(const T& value) const
	{
		return indexOf_NoLock(value) >= 0;
	}
	
	SLIB_INLINE sl_bool contains(const T& value) const
	{
		return indexOf(value) >= 0;
	}
	
	SLIB_INLINE _Type* duplicate_NoLock() const
	{
		if (m_count > 0) {
			return create(m_data, m_count);
		}
		return sl_null;
	}

	_Type* duplicate() const
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	SLIB_INLINE Array<T, COMPARE> toArray_NoLock() const
	{
		return Array<T, COMPARE>::create(m_data, m_count);
	}
	
	Array<T, COMPARE> toArray() const
	{
		ObjectLocker lock(this);
		return toArray_NoLock();
	}
	
	template <class _COMPARE>
	SLIB_INLINE void sortBy_NoLock(sl_bool flagAscending = sl_true) const
	{
		QuickSort<T, _COMPARE>::sort(m_data, m_count, flagAscending);
	}
	
	template <class _COMPARE>
	void sortBy(sl_bool flagAscending = sl_true) const
	{
		ObjectLocker lock(this);
		sortBy_NoLock<_COMPARE>(flagAscending);
	}
	
	SLIB_INLINE void sort_NoLock(sl_bool flagAscending = sl_true) const
	{
		sortBy_NoLock<COMPARE>(flagAscending);
	}

	SLIB_INLINE void sort(sl_bool flagAscending = sl_true) const
	{
		sortBy<COMPARE>(flagAscending);
	}
	
	Iterator<T> iterator() const;

protected:
	void _init()
	{
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}
	
	void _init(sl_size count)
	{
		if (count > 0) {
			T* data = (T*)(Base::createMemory(count * sizeof(T)));
			if (data) {
				for (sl_size i = 0; i < count; i++) {
					new (data + i) T();
				}
				m_data = data;
				m_count = count;
				m_capacity = count;
				return;
			}
		}
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}

	template <class _T>
	void _init(const _T* items, sl_size count)
	{
		if (count > 0) {
			T* data = (T*)(Base::createMemory(count * sizeof(T)));
			if (data) {
				for (sl_size i = 0; i < count; i++) {
					new (data + i) T(items[i]);
				}
				m_data = data;
				m_count = count;
				m_capacity = count;
				return;
			}
		}
		m_data = 0;
		m_count = 0;
		m_capacity = 0;
	}

	void _free()
	{
		if (m_data) {
			sl_size n = m_count;
			for (sl_size i = 0; i < n; i++) {
				(m_data + i)->~T();
			}
			Base::freeMemory(m_data);
		}
	}
	
	sl_bool _setCountInner(sl_size count)
	{
		sl_size oldCount = m_count;
		if (oldCount == count) {
			return sl_true;
		}
		if (count < oldCount) {
			T* data = m_data + count;
			sl_size n = oldCount - count;
			while (n) {
				data->~T();
				data++;
				n--;
			}
			m_count = count;
		}
		if (m_capacity < count) {
			sl_size newCapacity = m_capacity * 3 / 2 + 1;
			if (newCapacity < count) {
				newCapacity = count;
			}
			if (newCapacity < SLIB_LIST_CAPACITY_MIN) {
				newCapacity = SLIB_LIST_CAPACITY_MIN;
			}
			T* newData;
			sl_size size = newCapacity * sizeof(T);
			if (m_data) {
				newData = (T*)(Base::reallocMemory(m_data, size));
				if (!newData) {
					newData = (T*)(Base::createMemory(size));
					if (newData) {
						Base::copyMemory(newData, m_data, m_capacity * sizeof(T));
						Base::freeMemory(m_data);
					}
				}
			} else {
				newData = (T*)(Base::createMemory(size));
			}
			if (newData) {
				m_data = newData;
				m_capacity = newCapacity;
			} else {
				return sl_false;
			}
		} else if (m_capacity > SLIB_LIST_CAPACITY_MIN && count < m_capacity / 2) {
			sl_size newCapacity = count * 3 / 2 + 1;
			if (newCapacity < SLIB_LIST_CAPACITY_MIN) {
				newCapacity = SLIB_LIST_CAPACITY_MIN;
			}
			T* newData = (T*)(Base::reallocMemory(m_data, newCapacity * sizeof(T)));
			if (newData) {
				m_data = newData;
				m_capacity = newCapacity;
			}
		}
		if (count > oldCount) {
			T* data = m_data + oldCount;
			sl_size n = count - oldCount;
			while (n) {
				new (data) T();
				data++;
				n--;
			}
			m_count = count;
		}
		return sl_true;
	}
};

template <class T, class COMPARE>
class SLIB_EXPORT ListIterator : public IIterator<T>
{
protected:
	const CList<T, COMPARE>* m_list;
	Ref<Referable> m_refer;
	sl_size m_index;
	
public:
	SLIB_INLINE ListIterator(const CList<T, COMPARE>* list, const Referable* refer)
	{
		m_index = 0;
		m_list = list;
		m_refer = refer;
	}
		
public:
	// override
	sl_bool hasNext()
	{
		return (m_index < m_list->count());
	}
	
	// override
	sl_bool next(T* _out)
	{
		if (m_list->getItem(m_index, _out)) {
			m_index++;
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
SLIB_INLINE Iterator<T> CList<T, COMPARE>::iterator() const
{
	return new ListIterator<T, COMPARE>(this, sl_null);
}

template <class T, class COMPARE>
class SafeList;

/** auto-referencing object **/
template < class T, class COMPARE=Compare<T> >
class SLIB_EXPORT List
{
	typedef CList<T, COMPARE> _Obj;
	typedef List<T, COMPARE> _Type;
	typedef SafeList<T, COMPARE> _SafeType;
	typedef Ref<_Obj> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(List, _Obj, _Type, _SafeType)
	
public:
	SLIB_INLINE List(sl_size count) : m_object(_Obj::create(count))
	{
	}

	template <class _T>
	SLIB_INLINE List(const _T* data, sl_size count) : m_object(_Obj::create(data, count))
	{
	}

public:
	template <class _COMPARE>
	SLIB_INLINE const _Type& from(const List<T, _COMPARE>& other)
	{
		return *((_Type*)((void*)&other));
	}
	
public:
	SLIB_INLINE static _Type create()
	{
		return _Obj::create();
	}
	
	SLIB_INLINE static _Type create(sl_size count)
	{
		return _Obj::create(count);
	}
	
	template <class _T>
	SLIB_INLINE static _Type create(const _T* values, sl_size count)
	{
		return _Obj::create(values, count);
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE static _Type createCopy(const List<_T, _COMPARE>& other)
	{
		return _Obj::createCopy(other.getObject());
	}

	SLIB_INLINE static _Type createFromElement(const T& e)
	{
		return _Obj::createFromElement(e);
	}

public:
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
	
	SLIB_INLINE sl_size getCapacity() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->capacity();
		}
		return 0;
	}
	
	SLIB_INLINE sl_size capacity() const
	{
		return getCapacity();
	}
	
	SLIB_INLINE T* getData() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->data();
		}
		return 0;
	}
	
	SLIB_INLINE T* data() const
	{
		return getData();
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
	SLIB_INLINE T& operator[](sl_size index) const
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
	sl_bool setCount_NoLock(sl_size count)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->setCount_NoLock(count);
		} else {
			if (count == 0) {
				return sl_true;
			}
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->setCount_NoLock(count);
			}
		}
		return sl_false;
	}
	
	sl_bool setCount(sl_size count)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->setCount(count);
		} else {
			if (count == 0) {
				return sl_true;
			}
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->setCount_NoLock(count);
			}
		}
		return sl_false;
	}
	
	
	template <class _T>
	SLIB_INLINE sl_bool insert_NoLock(sl_size index, const _T* values, sl_size count) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->insert_NoLock(index, values, count);
		}
		return sl_false;
	}
	
	template <class _T>
	SLIB_INLINE sl_bool insert(sl_size index, const _T* values, sl_size count) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->insert(index, values, count);
		}
		return sl_false;
	}
	
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_bool insert(sl_size index, const List<_T, _COMPARE>& other) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->insert(index, other.getObject());
		}
		return sl_false;
	}
	
	
	SLIB_INLINE sl_bool insert_NoLock(sl_size index, const T& value) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->insert_NoLock(index, value);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool insert(sl_size index, const T& value) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->insert(index, value);
		}
		return sl_false;
	}
	
	
	template <class _T>
	sl_bool add_NoLock(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add_NoLock(values, count);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add_NoLock(values, count);
			}
		}
		return sl_false;
	}
	
	template <class _T>
	sl_bool add(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add(values, count);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add_NoLock(values, count);
			}
		}
		return sl_false;
	}
	
	
	template <class _T, class _COMPARE>
	sl_bool add(const List<_T, _COMPARE>& _other)
	{
		CList<_T, _COMPARE>* other = _other.getObject();
		if (!other) {
			return sl_true;
		}
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add(other);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add(other);
			}
		}
		return sl_false;
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_bool add(const SafeList<_T, _COMPARE>& other)
	{
		return add(List<_T, _COMPARE>(other));
	}

	
	sl_bool add_NoLock(const T& value)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add_NoLock(value);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add_NoLock(value);
			}
		}
		return sl_false;
	}
	
	sl_bool add(const T& value)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add(value);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add_NoLock(value);
			}
		}
		return sl_false;
	}
	
	
	sl_bool addIfNotExist_NoLock(const T& value)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->addIfNotExist_NoLock(value);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->addIfNotExist_NoLock(value);
			}
		}
		return sl_false;
	}
	
	sl_bool addIfNotExist(const T& value)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->addIfNotExist(value);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->addIfNotExist_NoLock(value);
			}
		}
		return sl_false;
	}
	
	
	template <class _T>
	sl_bool add_NoLock(const Iterator<_T>& iterator)
	{
		if (iterator.isNull()) {
			return sl_true;
		}
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add_NoLock(iterator);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add_NoLock(iterator);
			}
		}
		return sl_false;
	}
	
	template <class _T>
	sl_bool add(const Iterator<_T>& iterator)
	{
		if (iterator.isNull()) {
			return sl_true;
		}
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->add(iterator);
		} else {
			obj = _Obj::create();
			if (obj) {
				m_object = obj;
				return obj->add_NoLock(iterator);
			}
		}
		return sl_false;
	}
	

	SLIB_INLINE sl_size remove_NoLock(sl_size index, sl_size count = 1) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->remove_NoLock(index, count);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size remove(sl_size index, sl_size count = 1) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->remove(index, count);
		}
		return 0;
	}
	
	
	SLIB_INLINE sl_size removeValue_NoLock(const T& value, sl_bool flagAllValues = sl_false) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->removeValue_NoLock(value, flagAllValues);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size removeValue(const T& value, sl_bool flagAllValues = sl_false) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->removeValue(value, flagAllValues);
		}
		return 0;
	}
	
	
	SLIB_INLINE sl_size removeAll_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}
	
	SLIB_INLINE sl_size removeAll() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}
	
	
	SLIB_INLINE sl_bool popFront_NoLock(T* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popFront_NoLock(_out);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool popFront(T* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popFront(_out);
		}
		return sl_false;
	}
	
	
	SLIB_INLINE sl_size popFront_NoLock(sl_size count) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popFront_NoLock(count);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size popFront(sl_size count) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popFront(count);
		}
		return 0;
	}
	
	
	SLIB_INLINE sl_bool popBack_NoLock(T* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popBack_NoLock(_out);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool popBack(T* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popBack(_out);
		}
		return sl_false;
	}
	
	
	SLIB_INLINE sl_size popBack_NoLock(sl_size count) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popBack_NoLock(count);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size popBack(sl_size count) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->popBack(count);
		}
		return 0;
	}
	

	SLIB_INLINE sl_reg indexOf_NoLock(const T& value, sl_reg start = 0) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->indexOf_NoLock(value, start);
		}
		return -1;
	}
	
	SLIB_INLINE sl_reg indexOf(const T& value, sl_reg start = 0) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->indexOf(value, start);
		}
		return -1;
	}
	
	
	SLIB_INLINE sl_reg lastIndexOf_NoLock(const T& value, sl_reg start = -1) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->lastIndexOf_NoLock(value, start);
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
	
	
	SLIB_INLINE sl_bool contains_NoLock(const T& value) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->contains_NoLock(value);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool contains(const T& value) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->contains(value);
		}
		return sl_false;
	}
	
	
	SLIB_INLINE _Type duplicate_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return _Type::null();
	}
	
	SLIB_INLINE _Type duplicate() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate();
		}
		return _Type::null();
	}
	
	
	SLIB_INLINE Array<T, COMPARE> toArray_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->toArray_NoLock();
		}
		return Array<T, COMPARE>::null();
	}
	
	SLIB_INLINE Array<T, COMPARE> toArray() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->toArray();
		}
		return Array<T, COMPARE>::null();
	}
	
	
	template <class _COMPARE>
	SLIB_INLINE void sortBy(sl_bool flagAscending = sl_true) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			obj->template sortBy<_COMPARE>(flagAscending);
		}
	}
	
	template <class _COMPARE>
	SLIB_INLINE void sortBy_NoLock(sl_bool flagAscending = sl_true) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			obj->template sortBy_NoLock<_COMPARE>(flagAscending);
		}
	}

	
	SLIB_INLINE void sort_NoLock(sl_bool flagAscending = sl_true) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			obj->sort_NoLock(flagAscending);
		}
	}
	
	SLIB_INLINE void sort(sl_bool flagAscending = sl_true) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			obj->sort(flagAscending);
		}
	}
	
	
	SLIB_INLINE Iterator<T> iterator() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return new ListIterator<T, COMPARE>(obj, obj);
		}
		return Iterator<T>::null();
	}

public:
	SLIB_INLINE const Mutex* getLocker() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getLocker();
		} else {
			return sl_null;
		}
	}
 
};

/** auto-referencing object **/
template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT SafeList
{
	typedef CList<T, COMPARE> _Obj;
	typedef SafeList<T, COMPARE> _Type;
	typedef List<T, COMPARE> _LocalType;
	typedef SafeRef<_Obj> _Ref;
	typedef Ref<_Obj> _LocalRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_SAFE_WRAPPER(SafeList, _Obj, _Type, _LocalType)
	
public:
	SLIB_INLINE SafeList(sl_size count) : m_object(_Obj::create(count))
	{
	}
	
	template <class _T>
	SLIB_INLINE SafeList(const _T* data, sl_size count) : m_object(_Obj::create(data, count))
	{
	}
	
public:
	template <class _COMPARE>
	SLIB_INLINE const _Type& from(const SafeList<T, _COMPARE>& other)
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
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->count() == 0;
		}
		return sl_true;
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->count() != 0;
		}
		return sl_false;
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
	sl_bool setCount(sl_size count)
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->setCount(count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->setCount(count);
			}
			obj = _Obj::create();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->setCount(count);
			}
		}
		return sl_false;
	}
	
	template <class _T>
	SLIB_INLINE sl_bool insert(sl_size index, const _T* values, sl_size count) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->insert(index, values, count);
		}
		return sl_false;
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_bool insert(sl_size index, const List<_T, _COMPARE>& other) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->insert(index, other.getObject());
		}
		return sl_false;
	}
	
	
	SLIB_INLINE sl_bool insert(sl_size index, const T& value) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->insert(index, value);
		}
		return sl_false;
	}
	
	template <class _T>
	sl_bool add(const _T* values, sl_size count)
	{
		if (count == 0) {
			return sl_true;
		}
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->add(values, count);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->add(values, count);
			}
			obj = _Obj::create();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->add(values, count);
			}
		}
		return sl_false;
	}
	
	template <class _T, class _COMPARE>
	sl_bool add(const List<_T, _COMPARE>& _other)
	{
		CList<_T, _COMPARE>* other = _other.getObject();
		if (!other) {
			return sl_true;
		}
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->add(other);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->add(other);
			}
			obj = _Obj::create();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->add(other);
			}
		}
		return sl_false;
	}
	
	template <class _T, class _COMPARE>
	SLIB_INLINE sl_bool add(const SafeList<_T, _COMPARE>& other)
	{
		return add(List<_T, _COMPARE>(other));
	}

	sl_bool add(const T& value)
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->add(value);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->add(value);
			}
			obj = _Obj::create();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->add(value);
			}
		}
		return sl_false;
	}
	
	sl_bool addIfNotExist(const T& value)
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->addIfNotExist(value);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->addIfNotExist(value);
			}
			obj = _Obj::create();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->addIfNotExist(value);
			}
		}
		return sl_false;
	}
	
	template <class _T>
	sl_bool add(const Iterator<_T>& iterator)
	{
		if (iterator.isNull()) {
			return sl_true;
		}
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->add(iterator);
		} else {
			SpinLocker lock(SpinLockPoolForList::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->add(iterator);
			}
			obj = _Obj::create();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->add(iterator);
			}
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_size remove(sl_size index, sl_size count = 1) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->remove(index, count);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size removeValue(const T& value, sl_bool flagAllValues = sl_false) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->removeValue(value, flagAllValues);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size removeAll() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}
	
	SLIB_INLINE sl_bool popFront(T* _out = sl_null) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->popFront(_out);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_size popFront(sl_size count) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->popFront(count);
		}
		return 0;
	}
	
	SLIB_INLINE sl_bool popBack(T* _out = sl_null) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->popBack(_out);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_size popBack(sl_size count) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->popBack(count);
		}
		return 0;
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
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->contains(value);
		}
		return sl_false;
	}
	
	SLIB_INLINE _LocalType duplicate() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return _LocalType::null();
	}
	
	SLIB_INLINE Array<T, COMPARE> toArray() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->toArray();
		}
		return Array<T, COMPARE>::null();
	}
	
	template <class _COMPARE>
	SLIB_INLINE void sortBy(sl_bool flagAscending = sl_true) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			obj->template sortBy<_COMPARE>(flagAscending);
		}
	}
	
	SLIB_INLINE void sort(sl_bool flagAscending = sl_true) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			obj->sort(flagAscending);
		}
	}
};

template <class T>
class SLIB_EXPORT ListLocker : public ObjectLocker
{
private:
	T* m_data;
	sl_size m_count;
	List<T> m_list;
	
public:
	template <class _COMPARE>
	SLIB_INLINE ListLocker(const List<T, _COMPARE>& list) : ObjectLocker(list.getObject())
	{
		m_list = list;
		m_data = list.data();
		m_count = list.count();
	}
	
	template <class _COMPARE>
	SLIB_INLINE ListLocker(const SafeList<T, _COMPARE>& list) : ListLocker(List<T, _COMPARE>(list))
	{
	}

	template <class _COMPARE>
	SLIB_INLINE ListLocker(const CList<T, _COMPARE>& list) : ObjectLocker(&list)
	{
		m_data = list.data();
		m_count = list.count();
	}
	
	SLIB_INLINE sl_size getCount()
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size count()
	{
		return m_count;
	}
	
	SLIB_INLINE T* getData()
	{
		return m_data;
	}
	
	SLIB_INLINE T* data()
	{
		return m_data;
	}

	SLIB_INLINE T& operator[](sl_size index)
	{
		return m_data[index];
	}
	
};


template <class T>
class SLIB_EXPORT ListItems
{
private:
	T* m_data;
	sl_size m_count;
	List<T> m_list;
	
public:
	template <class _COMPARE>
	SLIB_INLINE ListItems(const List<T, _COMPARE>& list)
	{
		m_list = list;
		m_data = list.data();
		m_count = list.count();
	}

	template <class _COMPARE>
	SLIB_INLINE ListItems(const SafeList<T, _COMPARE>& list) : ListItems(List<T, _COMPARE>(list))
	{
	}

	template <class _COMPARE>
	SLIB_INLINE ListItems(const CList<T, _COMPARE>& list)
	{
		m_data = list.data();
		m_count = list.count();
	}

	SLIB_INLINE sl_size getCount()
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size count()
	{
		return m_count;
	}
	
	SLIB_INLINE T* getData()
	{
		return m_data;
	}
	
	SLIB_INLINE T* data()
	{
		return m_data;
	}
	
	SLIB_INLINE T& operator[](sl_size index)
	{
		return m_data[index];
	}
	
};


SLIB_NAMESPACE_END

#endif
