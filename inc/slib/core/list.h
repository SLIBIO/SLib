#ifndef CHECKHEADER_SLIB_CORE_LIST
#define CHECKHEADER_SLIB_CORE_LIST

#include "definition.h"
#include "object.h"
#include "iterator.h"
#include "array.h"
#include "algorithm.h"

/************************************************************************
	List Object
************************************************************************/

#define _SLIB_LIST_DEFAULT_SIZE 5

SLIB_NAMESPACE_BEGIN
template <class TYPE, sl_bool flagThreadSafe = sl_true, class COMPARE=Compare<TYPE> >
class SLIB_EXPORT ListObject : public Object
{
	SLIB_DECLARE_OBJECT(ListObject, Object)
protected:
	sl_size m_count;
	sl_size m_size;
	TYPE* m_data;
	
public:
	SLIB_INLINE ListObject(sl_size size = _SLIB_LIST_DEFAULT_SIZE)
	{
		init(size);
	}

	SLIB_INLINE ListObject(ListObject<TYPE, flagThreadSafe, COMPARE>* other)
	{
		MutexLocker lock(other->getLocker());
		init(other->m_data, other->m_count);
	}

	SLIB_INLINE ListObject(const TYPE* values, sl_size count)
	{
		init(values, count);
	}

	~ListObject()
	{
		release();
	}

public:
	SLIB_INLINE sl_size getCount()
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size getCapacity()
	{
		return m_size;
	}

	sl_bool getItem(sl_size index, TYPE* out = sl_null)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (index < m_count) {
			if (out) {
				*out = m_data[index];
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE TYPE* getItemPtr(sl_size index)
	{
		return m_data + index;
	}
	
	TYPE getItemValue(sl_size index, const TYPE& def)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (index < m_count) {
			return m_data[index];
		} else {
			return def;
		}
	}
	
	sl_bool setItem(sl_size index, const TYPE& value)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (index < m_count) {
			m_data[index] = value;
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE TYPE* getBuffer()
	{
		return m_data;
	}
	
	SLIB_INLINE TYPE& operator[](sl_size index)
	{
		return m_data[index];
	}
	
	SLIB_INLINE sl_bool setCount(sl_size count)
	{
		return setCountInner(count, sl_true);
	}
	
	template <class _TYPE>
	sl_bool insert(sl_size index, const _TYPE* other, sl_size count)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (count > 0) {
			sl_size origCount = m_count;
			if (!(setCountInner(origCount + count, sl_false))) {
				return sl_false;
			}
			if (index < origCount) {
				{
					TYPE* dst = m_data + m_count - 1;
					const TYPE* src = m_data + origCount - 1;
					sl_size n = origCount - index;
					while (n) {
						*(dst--) = *(src--);
						n--;
					}
				}
				{
					TYPE* dst = m_data + index;
					const _TYPE*src = other;
					sl_size n = count;
					while (n) {
						*(dst++) = *(src++);
						n--;
					}
				}
			} else {
				TYPE* dst = m_data + origCount;
				const _TYPE* src = other;
				sl_size n = count;
				while (n) {
					*(dst++) = *(src++);
					n--;
				}
			}
		}
		return sl_true;
	}
	
	template <class _TYPE, sl_bool _flagThreadSafe, class _COMPARE>
	sl_bool insert(sl_size index, ListObject<_TYPE, _flagThreadSafe, _COMPARE>& other)
	{
		MutexLocker lockerThis(flagThreadSafe ? this->getLocker() : sl_null, _flagThreadSafe ? other.getLocker() : sl_null);
		sl_size count = other.getCount();
		if (count > 0) {
			_TYPE* bufOther = other.getBuffer();
			sl_size origCount = m_count;
			if (!(setCountInner(origCount + count, sl_false))) {
				return sl_false;
			}
			if (index < origCount) {
				TYPE* dst = m_data + m_count - 1;
				TYPE* src = m_data + origCount - 1;
				sl_size n = origCount - index;
				while (n) {
					*(dst--) = *(src--);
					n--;
				}
				dst = m_data + index;
				for (sl_size i = 0; i < count; i++) {
					*(dst++) = bufOther[i];
				}
			} else {
				TYPE* dst = m_data + origCount;
				for (sl_size i = 0; i < count; i++) {
					*(dst++) = bufOther[i];
				}
			}
		}
		return sl_true;
	}
	
	void remove(sl_size index, sl_size count)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (index >= m_count) {
			return;
		}
		if (count > m_count - index) {
			count = m_count - index;
		}
		TYPE* dst = m_data + index;
		TYPE* src = dst + count;
		sl_size n = m_count - index - count;
		while (n) {
			*(dst++) = *(src++);
			n--;
		}
		setCountInner(m_count - count, sl_false);
	}
	
	sl_bool removeValue(const TYPE& value, sl_bool flagAllValues = sl_false)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
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
				setCountInner(n, sl_false);
				return sl_true;
			} else {
				return sl_false;
			}
		}
		return sl_false;
	}

	sl_bool pop(TYPE* output = sl_null)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		bool ret = sl_false;
		if (m_count > 0) {
			if (output) {
				*output = m_data[m_count - 1];
			}
			setCountInner(m_count - 1, sl_false);
			ret = sl_true;
		}
		return ret;
	}

	void pop(sl_size count)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (m_count >= count) {
			setCountInner(m_count - count, sl_false);
		}
	}
	
	sl_reg indexOf(const TYPE& value, sl_reg start = 0)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
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
	
	sl_reg lastIndexOf(const TYPE& value, sl_reg start = -1)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
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
	
	Ref< ListObject<TYPE, flagThreadSafe, COMPARE> > duplicate()
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		Ref< ListObject<TYPE, flagThreadSafe, COMPARE> > ret = new ListObject<TYPE, flagThreadSafe, COMPARE>(m_count);
		if (ret.isNotNull()) {
			if (ret->getCapacity() == 0) {
				ret.setNull();
				return ret;
			}
		} else {
			return ret;
		}
		if (!(ret->setCount(m_count))) {
			ret.setNull();
			return ret;
		}
		TYPE* src = m_data;
		TYPE* dst = ret->m_data;
		sl_size n = m_count;
		while (n) {
			*(dst++) = *(src++);
			n--;
		}
		return ret;
	}
	
	SLIB_INLINE sl_bool insert(sl_size index, const TYPE& value)
	{
		return insert(index, &value, 1);
	}
	
	sl_bool add(const TYPE& value)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		sl_size origCount = m_count;
		if (origCount < m_size) {
			m_count++;
			new (m_data + origCount) TYPE(value);
		} else {
			if (!(setCountInner(origCount + 1, sl_false))) {
				return sl_false;
			}
			m_data[origCount] = value;
		}
		return sl_true;
	}
	
	sl_bool addIfNotExist(const TYPE& value)
	{
		MutexLocker locker(flagThreadSafe ? this->getLocker() : sl_null);
		if (indexOf(value) >= 0) {
			return sl_false;
		} else {
			return add(value);
		}
	}
	
	template <class _TYPE, sl_bool _flagThreadSafe, class _COMPARE>
	SLIB_INLINE sl_bool add(ListObject<_TYPE, _flagThreadSafe, _COMPARE>& other)
	{
		return insert(-1, other);
	}
	
	template <class _TYPE>
	SLIB_INLINE sl_bool add(const _TYPE* listValue, sl_size countValue)
	{
		return insert(-1, listValue, countValue);
	}
	
	SLIB_INLINE void removeAll()
	{
		remove(0, -1);
	}
	
	SLIB_INLINE void clear()
	{
		remove(0, -1);
	}
	
	SLIB_INLINE sl_bool contains(const TYPE& value)
	{
		return indexOf(value) >= 0;
	}
	
private:
	void init(sl_size size)
	{
		if (size == 0) {
			size = _SLIB_LIST_DEFAULT_SIZE;
		}
		TYPE* data = (TYPE*)(Base::createMemory(size * sizeof(TYPE)));
		if (data) {
			m_count = 0;
			m_size = size;
			m_data = data;
			for (sl_size i = 0; i < size; i++) {
				new (data + i) TYPE();
			}
		} else {
			m_count = 0;
			m_size = 0;
			m_data = 0;
		}
	}

	void init(const TYPE* items, sl_size count)
	{
		sl_size size = count;
		if (size < _SLIB_LIST_DEFAULT_SIZE) {
			size = _SLIB_LIST_DEFAULT_SIZE;
		}
		TYPE* data = (TYPE*)(Base::createMemory(size * sizeof(TYPE)));
		if (data) {
			m_count = count;
			m_size = size;
			m_data = data;
			sl_size i;
			for (i = 0; i < count; i++) {
				new (data + i) TYPE(items[i]);
			}
			for (i = count; i < size; i++) {
				new (data + i) TYPE();
			}
		} else {
			m_count = 0;
			m_size = 0;
			m_data = 0;
		}
	}

	void release()
	{
		if (m_data) {
			sl_size n = m_count;
			for (sl_size i = 0; i < n; i++) {
				(m_data + i)->~TYPE();
			}
			Base::freeMemory(m_data);
			m_data = sl_null;
		}
		m_count = 0;
		m_size = 0;
	}
	
	sl_bool setCountInner(sl_size count, sl_bool bLock)
	{
		MutexLocker locker(flagThreadSafe && bLock ? this->getLocker() : sl_null);
		sl_size origCount = m_count;
		if (origCount == count) {
			return sl_true;
		}
		if (count < origCount) {
			for (sl_size i = count; i < origCount; i++) {
				(m_data + i)->~TYPE();
			}
			m_count = count;
		}
		if (m_size < count) {
			sl_size newSize = m_size * 3 / 2 + 1;
			if (newSize < count) {
				newSize = count;
			}
			TYPE* newData = (TYPE*)(Base::reallocMemory(m_data, newSize * sizeof(TYPE)));
			if (newData) {
				m_data = newData;
				m_size = newSize;
			} else {
				return sl_false;
			}
		} else if (m_size > _SLIB_LIST_DEFAULT_SIZE && count < m_size / 2) {
			sl_size newSize = count * 3 / 2 + 1;
			TYPE* newData = (TYPE*)(Base::reallocMemory(m_data, newSize * sizeof(TYPE)));
			if (newData) {
				m_data = newData;
				m_size = newSize;
			}
		}
		if (count > origCount) {
			for (sl_size i = origCount; i < count; i++) {
				new (m_data + i) TYPE();
			}
			m_count = count;
		}
		return sl_true;
	}
};
SLIB_NAMESPACE_END


/************************************************************************/
/* List class Definition                                                */
/************************************************************************/
SLIB_NAMESPACE_BEGIN

/** auto-referencing object **/
template <class TYPE, sl_bool flagThreadSafe = sl_true, class COMPARE=Compare<TYPE> >
class SLIB_EXPORT List
{
	typedef ListObject<TYPE, flagThreadSafe, COMPARE> _ListObj;
	typedef List<TYPE, flagThreadSafe, COMPARE> _ListType;
	typedef Ref< ListObject<TYPE, flagThreadSafe, COMPARE> > _ListRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_ListType, _ListObj)
	SLIB_DECLARE_OBJECT_WRAPPER(List, _ListType, _ListObj, _ListRef)
	
public:
	template <sl_bool _flagThreadSafe, class _COMPARE>
	SLIB_INLINE List(const List<TYPE, _flagThreadSafe, _COMPARE>& other)
	: m_object(*((_ListRef*)&(other.getReference())))
	{
	}

	template <sl_bool _flagThreadSafe, class _COMPARE>
	SLIB_INLINE _ListType& operator=(const List<TYPE, _flagThreadSafe, _COMPARE>& other)
	{
		m_object = *((_ListRef*)&(other.getReference()));
		return *this;
	}

	SLIB_INLINE TYPE& operator[](sl_size index) const
	{
		return *(m_object->getItemPtr(index));
	}
	
	SLIB_INLINE static _ListType create(sl_size size = _SLIB_LIST_DEFAULT_SIZE)
	{
		_ListType ret;
		ret.m_object = new ListObject<TYPE>(size);
		if (ret.m_object.isNotNull()) {
			if (ret.m_object->getCapacity() == 0) {
				ret.setNull();
			}
		}
		return ret;
	}
	
	SLIB_INLINE static _ListType fromElement(const TYPE& e)
	{
		_ListType ret;
		ret.add(e);
		return ret;
	}
	
	_ListType duplicate() const
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			_ListType ret;
			ret.m_object = object->duplicate();
			return ret;
		} else {
			return null();
		}
	}
	
	SLIB_INLINE sl_bool getItem(sl_size index, TYPE* _out = sl_null) const
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->getItem(index, _out);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE TYPE* getItemPtr(sl_size index) const
	{
		return m_object->getItemPtr(index);
	}
	
	SLIB_INLINE TYPE getItemValue(sl_size index, const TYPE& def) const
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->getItemValue(index, def);
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool setItem(sl_size index, const TYPE& value)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->setItem(index, value);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_size getCount() const
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->getCount();
		} else {
			return 0;
		}
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
	
	sl_bool setCount(sl_size count)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->setCount(count);
		} else {
			_ListType list(create(count));
			if (list.isNotNull()) {
				if (list.setCount(count)) {
					*this = list;
					return sl_true;
				}
			}
			return sl_false;
		}
	}
	
	template <class _TYPE>
	sl_bool insert(sl_size index, const _TYPE* listValue, sl_size countValue)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->insert(index, listValue, countValue);
		} else {
			_ListType list(create());
			if (list.isNotNull()) {
				if (list.add(listValue, countValue)) {
					*this = list;
					return sl_true;
				}
			}
			return sl_false;
		}
	}
	
	template <class _TYPE, sl_bool _flagThreadSafe, class _COMPARE>
	sl_bool insert(sl_size index, const List<_TYPE, _flagThreadSafe, _COMPARE>& other)
	{
		Ref< ListObject<_TYPE, _flagThreadSafe, _COMPARE>  > object2 = other.getReference();
		if (object2.isNull())
			return sl_true;
		_ListRef object1(m_object);
		if (object1.isNotNull()) {
			if ((void*)(object1.get()) != (void*)(object2.get())) {
				return object1->insert(index, *(object2.get()));
			} else {
				_ListType list(duplicate());
				if (list.isNotNull()) {
					if (list.insert(index, other)) {
						*this = list;
						return sl_true;
					}
				}
				return sl_false;
			}
		} else {
			_ListType list(create());
			if (list.isNotNull()) {
				if (list.insert(index, other)) {
					*this = list;
					return sl_true;
				}
			}
			return sl_false;
		}

	}
	
	SLIB_INLINE void remove(sl_size index, sl_size count = 1)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			object->remove(index, count);
		}
	}
	
	SLIB_INLINE sl_bool removeValue(const TYPE& value, sl_bool flagAllValues = sl_false)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->removeValue(value, flagAllValues);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_bool pop(TYPE* out = sl_null)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->pop(out);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE TYPE* getBuffer()
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->getBuffer();
		} else {
			return sl_null;
		}
	}
	
	sl_bool insert(sl_size index, const TYPE& value)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->insert(index, value);
		} else {
			_ListType list(create());
			if (list.isNotNull()) {
				if (list.add(value)) {
					*this = list;
					return sl_true;
				}
			}
			return sl_false;
		}
	}
	
	sl_bool push(const TYPE& value)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->add(value);
		} else {
			_ListType list(create());
			if (list.isNotNull()) {
				if (list.m_object->add(value)) {
					*this = list;
					return sl_true;
				}
			}
			return sl_false;
		}
	}
	
	template <class _TYPE>
	sl_bool push(const _TYPE* listValue, sl_size countValue)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->add(listValue, countValue);
		} else {
			_ListType list = create();
			if (list.isNotNull()) {
				if (list.add(listValue, countValue)) {
					*this = list;
					return sl_true;
				}
			}
			return sl_false;
		}
	}
	
	template <class _TYPE, sl_bool _flagThreadSafe, class _COMPARE>
	SLIB_INLINE sl_bool push(const List<_TYPE, _flagThreadSafe, _COMPARE>& other)
	{
		return insert(-1, other);
	}
	
	SLIB_INLINE sl_bool add(const TYPE& value)
	{
		return push(value);
	}
	
	template <class _TYPE>
	SLIB_INLINE sl_bool add(const _TYPE* listValue, sl_size countValue)
	{
		return push(listValue, countValue);
	}
	
	template <class _TYPE, sl_bool _flagThreadSafe, class _COMPARE>
	SLIB_INLINE sl_bool add(const List<_TYPE, _flagThreadSafe, _COMPARE>& other)
	{
		return push(other);
	}
	
	SLIB_INLINE sl_bool addIfNotExist(const TYPE& value)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->addIfNotExist(value);
		} else {
			return add(value);
		}
	}
	
	template <class _TYPE>
	sl_bool addIterator(const Iterator<_TYPE>& iterator)
	{
		TYPE value;
		while (iterator.next(&value)) {
			if (!(add(value))) {
				return sl_false;
			}
		}
		return sl_true;
	}
	
	SLIB_INLINE void removeAll()
	{
		remove(0, -1);
	}

	SLIB_INLINE void removeFirst()
	{
		remove(0, 1);
	}

	SLIB_INLINE void removeLast()
	{
		pop(1);
	}

	SLIB_INLINE void clear()
	{
		remove(0, -1);
	}
	
	SLIB_INLINE void pop(sl_size count)
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			object->pop(count);
		}
	}
	
	SLIB_INLINE sl_reg indexOf(const TYPE& value, sl_reg start = 0) const
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->indexOf(value, start);
		} else {
			return -1;
		}
	}

	SLIB_INLINE sl_reg lastIndexOf(const TYPE& value, sl_reg start = -1) const
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->lastIndexOf(value, start);
		} else {
			return -1;
		}
	}

	SLIB_INLINE sl_bool contains(const TYPE& value) const
	{
		return indexOf(value) >= 0;
	}
	
	Iterator<TYPE> iterator() const;
	
	SLIB_INLINE const Mutex* getLocker()
	{
		_ListRef object(m_object);
		if (object.isNotNull()) {
			return object->getLocker();
		} else {
			return sl_null;
		}
	}
	
	Array<TYPE> toArray() const
	{
		Array<TYPE> ret;
		_ListRef object(m_object);
		if (object.isNotNull()) {
			MutexLocker locker(flagThreadSafe ? object->getLocker() : sl_null);
			ret = Array<TYPE>::create(object->getBuffer(), object->getCount());
		}
		return ret;
	}
	
	template <class _COMPARE>
	void sortBy(sl_bool flagAscending = sl_true) const
	{
		_ListType ret;
		_ListRef object(m_object);
		if (object.isNotNull()) {
			MutexLocker locker(flagThreadSafe ? object->getLocker() : sl_null);
			TYPE* buf = object->getBuffer();
			sl_size n = object->getCount();
			QuickSort<TYPE, _COMPARE>::sort(buf, n, flagAscending);
		}
	}
	
	SLIB_INLINE void sort(sl_bool flagAscending = sl_true)
	{
		sortBy<COMPARE>(flagAscending);
	}

};
SLIB_NAMESPACE_END


/************************************************************************/
/* ListIterator class Definition                                        */
/************************************************************************/
SLIB_NAMESPACE_BEGIN
template <class TYPE, sl_bool flagThreadSafe, class COMPARE>
class SLIB_EXPORT ListIterator : public IIterator<TYPE>
{
protected:
	List<TYPE, flagThreadSafe, COMPARE> m_list;
	sl_size m_index;

public:
	ListIterator(const List<TYPE, flagThreadSafe, COMPARE>& list)
	{
		m_index = 0;
		m_list = list;
	}
	
	~ListIterator()
	{
	}
	
	sl_bool hasNext()
	{
		return (m_index < m_list.getCount());
	}
	
	sl_bool next(TYPE* out)
	{
		if (m_list.getItem(m_index, out)) {
			m_index++;
			return sl_true;
		}
		return sl_false;
	}
	
	sl_reg getIndex()
	{
		return (sl_reg)m_index - 1;
	}
};

template <class TYPE, sl_bool flagThreadSafe, class COMPARE>
Iterator<TYPE> List<TYPE, flagThreadSafe, COMPARE>::iterator() const
{
	List<TYPE, flagThreadSafe, COMPARE> o = *this;
	if (o.isNotNull()) {
		return new ListIterator<TYPE, flagThreadSafe, COMPARE>(o);
	} else {
		return Iterator<TYPE>::null();
	}
}
SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN
template <class TYPE>
class SLIB_EXPORT ListLocker
{
private:
	sl_size m_count;
	TYPE* m_buffer;
	List<TYPE, sl_false> m_list;
	MutexLocker m_locker;
	
public:
	template <sl_bool _flagThreadSafe, class _COMPARE>
	ListLocker(const List<TYPE, _flagThreadSafe, _COMPARE>& list)
	{
		m_list = list;
		if (_flagThreadSafe) {
			m_locker.lock(m_list.getLocker());
		}
		m_count = m_list.getCount();
		m_buffer = m_list.getBuffer();
	}
	
	~ListLocker()
	{
	}
	
	SLIB_INLINE sl_size getCount()
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size count()
	{
		return m_count;
	}
	
	SLIB_INLINE TYPE* getBuffer()
	{
		return m_buffer;
	}
	
	SLIB_INLINE TYPE& operator[](sl_size index)
	{
		return m_buffer[index];
	}
	
	SLIB_INLINE void unlock()
	{
		m_locker.unlock();
	}
};
SLIB_NAMESPACE_END

#endif// CHECKHEADER_SLIB_CORE_LIST
