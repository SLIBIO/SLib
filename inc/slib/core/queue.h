#ifndef CHECKHEADER_SLIB_CORE_QUEUE
#define CHECKHEADER_SLIB_CORE_QUEUE

#include "definition.h"

#include "object.h"
#include "list.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN

template <class T>
struct SLIB_EXPORT Link
{
	Link<T>* before;
	Link<T>* next;
	T value;
};

class SLIB_EXPORT LinkedListBase : public Object
{
	SLIB_DECLARE_OBJECT
};

template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT LinkedList : public LinkedListBase
{
protected:
	Link<T>* m_begin;
	Link<T>* m_end;
	sl_size m_count;
	
public:
	LinkedList();
	
	~LinkedList();
	
public:
	void init(Link<T>* begin, Link<T>* end, sl_size count);
	
	Link<T>* getBegin() const;

	Link<T>* getEnd() const;

	sl_size getCount() const;
	
	sl_bool isEmpty() const;

	sl_bool isNotEmpty() const;
	
public:
	static void freeLink(Link<T>* link);
	
	void removeAll_NoLock();
	
	void removeAll();
	
	sl_bool getFirstElement_NoLock(T* _out) const;
	
	sl_bool getFirstElement(T* _out) const;
	
	sl_bool getLastElement_NoLock(T* _out) const;
	
	sl_bool getLastElement(T* _out) const;
	
	Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushBack(const T& value, sl_size countLimit = 0);
	
	sl_bool popBack_NoLock(T* _out = sl_null);
	
	sl_bool popBack(T* _out = sl_null);
	
	Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushFront(const T& value, sl_size countLimit = 0);
	
	sl_bool popFront_NoLock(T* _out = sl_null);
	
	sl_bool popFront(T* _out = sl_null);
	
	void removeItem_NoLock(Link<T>* item);
	
	void removeItem(Link<T>* item);
	
	Link<T>* insertBefore_NoLock(Link<T>* itemWhere, const T& value);
	
	Link<T>* insertBefore(Link<T>* itemWhere, const T& value);
	
	Link<T>* insertAfter_NoLock(Link<T>* itemWhere, const T& value);
	
	Link<T>* insertAfter(Link<T>* itemWhere, const T& value);
	
	void merge(LinkedList<T, COMPARE>* other);

	template <class _COMPARE>
	void merge(LinkedList<T, _COMPARE>* other);
	
	Array<T> toArray_NoLock() const;
	
	Array<T> toArray() const;
	
	List<T> toList_NoLock() const;
	
	List<T> toList() const;
	
	Link<T>* findValue_NoLock(const T& value) const;
	
	Link<T>* findValue(const T& value) const;
	
	sl_bool removeValue_NoLock(const T& value, sl_bool flagAllValues = sl_false);
	
	sl_bool removeValue(const T& value, sl_bool flagAllValues = sl_false);
	
protected:
	static Link<T>* _createItem(const T& value);
	
	static void _freeItem(Link<T>* item);
	
	Link<T>* _pushBackItem(Link<T>* item, sl_size countLimit);
	
	Link<T>* _popBackItem();
	
	Link<T>* _pushFrontItem(Link<T>* item, sl_size countLimit);
	
	Link<T>* _popFrontItem();
	
	void _removeItem(Link<T>* item);
	
	void _insertBefore(Link<T>* itemWhere, Link<T>* itemNew);
	
	void _insertAfter(Link<T>* itemWhere, Link<T>* itemNew);
	
	void _init();
	
};

template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT Queue : public LinkedList<T, COMPARE>
{
public:
	Link<T>* push(const T& value, sl_size countLimit = 0);

	Link<T>* push_NoLock(const T& value, sl_size countLimit = 0);
	
	sl_bool pop(T* _out = sl_null);
	
	sl_bool pop_NoLock(T* _out = sl_null);

};

template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT Stack : public LinkedList<T, COMPARE>
{
public:
	Link<T>* push(const T& value, sl_size countLimit = 0);
	
	Link<T>* push_NoLock(const T& value, sl_size countLimit = 0);
	
	sl_bool pop(T* _out = sl_null);
	
	sl_bool pop_NoLock(T* _out = sl_null);
	
};


#define SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_QUEUE(...) \
	extern template class LinkedList<__VA_ARGS__>; \
	extern template class Queue<__VA_ARGS__>; \
	extern template class Stack<__VA_ARGS__>;

#define SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_QUEUE(...) \
	template class LinkedList<__VA_ARGS__>; \
	template class Queue<__VA_ARGS__>; \
	template class Stack<__VA_ARGS__>;


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T, class COMPARE>
SLIB_INLINE LinkedList<T, COMPARE>::LinkedList()
{
	m_begin = sl_null;
	m_end = sl_null;
	m_count = 0;
}

template <class T, class COMPARE>
SLIB_INLINE LinkedList<T, COMPARE>::~LinkedList()
{
	removeAll();
}

template <class T, class COMPARE>
SLIB_INLINE void LinkedList<T, COMPARE>::init(Link<T>* begin, Link<T>* end, sl_size count)
{
	m_begin = begin;
	m_end = end;
	m_count = count;
}

template <class T, class COMPARE>
SLIB_INLINE Link<T>* LinkedList<T, COMPARE>::getBegin() const
{
	return m_begin;
}

template <class T, class COMPARE>
SLIB_INLINE Link<T>* LinkedList<T, COMPARE>::getEnd() const
{
	return m_end;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size LinkedList<T, COMPARE>::getCount() const
{
	return m_count;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool LinkedList<T, COMPARE>::isEmpty() const
{
	return m_begin == sl_null;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool LinkedList<T, COMPARE>::isNotEmpty() const
{
	return m_begin != sl_null;
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::freeLink(Link<T>* link)
{
	while (link) {
		Link<T>* next = link->next;
		_freeItem(link);
		link = next;
	}
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::removeAll_NoLock()
{
	Link<T>* now = m_begin;
	_init();
	freeLink(now);
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::removeAll()
{
	Link<T>* now;
	{
		ObjectLocker lock(this);
		now = m_begin;
		_init();
	}
	freeLink(now);
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::getFirstElement_NoLock(T* _out) const
{
	if (m_begin) {
		if (_out) {
			*_out = m_begin->value;
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::getFirstElement(T* _out) const
{
	ObjectLocker lock(this);
	return getFirstElement_NoLock(_out);
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::getLastElement_NoLock(T* _out) const
{
	if (m_end) {
		if (_out) {
			*_out = m_end->value;
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::getLastElement(T* _out) const
{
	ObjectLocker lock(this);
	return getLastElement_NoLock(_out);
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::pushBack_NoLock(const T& value, sl_size countLimit)
{
	Link<T>* item = _createItem(value);
	if (!item) {
		return sl_null;
	}
	Link<T>* old = _pushBackItem(item, countLimit);
	if (old) {
		_freeItem(old);
	}
	return item;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::pushBack(const T& value, sl_size countLimit)
{
	Link<T>* item = _createItem(value);
	if (!item) {
		return sl_null;
	}
	Link<T>* old;
	{
		ObjectLocker lock(this);
		old = _pushBackItem(item, countLimit);
	}
	if (old) {
		_freeItem(old);
	}
	return item;
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::popBack_NoLock(T* _out)
{
	Link<T>* old = _popBackItem();
	if (old) {
		if (_out) {
			*_out = old->value;
		}
		_freeItem(old);
		return sl_true;
	} else {
		return sl_false;
	}
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::popBack(T* _out)
{
	Link<T>* old;
	{
		ObjectLocker lock(this);
		old = _popBackItem();
	}
	if (old) {
		if (_out) {
			*_out = old->value;
		}
		_freeItem(old);
		return sl_true;
	} else {
		return sl_false;
	}
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::pushFront_NoLock(const T& value, sl_size countLimit)
{
	Link<T>* item = _createItem(value);
	if (!item) {
		return sl_null;
	}
	Link<T>* old = _pushFrontItem(item, countLimit);
	if (old) {
		_freeItem(old);
	}
	return item;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::pushFront(const T& value, sl_size countLimit)
{
	Link<T>* item = _createItem(value);
	if (!item) {
		return sl_null;
	}
	Link<T>* old;
	{
		ObjectLocker lock(this);
		old = _pushFrontItem(item, countLimit);
	}
	if (old) {
		_freeItem(old);
	}
	return item;
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::popFront_NoLock(T* _out)
{
	Link<T>* old = _popFrontItem();
	if (old) {
		if (_out) {
			*_out = old->value;
		}
		_freeItem(old);
		return sl_true;
	} else {
		return sl_false;
	}
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::popFront(T* _out)
{
	Link<T>* old;
	{
		ObjectLocker lock(this);
		old = _popFrontItem();
	}
	if (old) {
		if (_out) {
			*_out = old->value;
		}
		_freeItem(old);
		return sl_true;
	} else {
		return sl_false;
	}
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::removeItem_NoLock(Link<T>* item)
{
	if (item) {
		_removeItem(item);
		_freeItem(item);
	} else {
		SLIB_ABORT("Trying to free null item");
	}
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::removeItem(Link<T>* item)
{
	if (item) {
		{
			ObjectLocker lock(this);
			_removeItem(item);
		}
		_freeItem(item);
	} else {
		SLIB_ABORT("Trying to free null item");
	}
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::insertBefore_NoLock(Link<T>* itemWhere, const T& value)
{
	if (itemWhere) {
		Link<T>* itemNew = _createItem(value);
		if (itemNew) {
			_insertBefore(itemWhere, itemNew);
		}
		return itemNew;
	} else {
		return pushFront_NoLock(value);
	}
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::insertBefore(Link<T>* itemWhere, const T& value)
{
	if (itemWhere) {
		Link<T>* itemNew = _createItem(value);
		if (itemNew) {
			ObjectLocker lock(this);
			_insertBefore(itemWhere, itemNew);
		}
		return itemNew;
	} else {
		return pushFront(value);
	}
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::insertAfter_NoLock(Link<T>* itemWhere, const T& value)
{
	if (itemWhere) {
		Link<T>* itemNew = _createItem(value);
		if (itemNew) {
			_insertAfter(itemWhere, itemNew);
		}
		return itemNew;
	} else {
		return pushBack_NoLock(value);
	}
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::insertAfter(Link<T>* itemWhere, const T& value)
{
	if (itemWhere) {
		Link<T>* itemNew = _createItem(value);
		if (itemNew) {
			ObjectLocker lock(this);
			_insertAfter(itemWhere, itemNew);
		}
		return itemNew;
	} else {
		return pushBack_NoLock(value);
	}
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::merge(LinkedList<T, COMPARE>* other)
{
	if ((void*)this == (void*)other) {
		return;
	}
	ObjectLocker lock(this, other);
	Link<T>* _begin = other->getBegin();
	Link<T>* _end = other->getEnd();
	if (_begin) {
		sl_size countNew = m_count + other->getCount();
		if (m_end) {
			m_end->next = _begin;
			_begin->before = m_end;
		} else {
			m_begin = _begin;
		}
		m_end = _end;
		((LinkedList<T, COMPARE>*)((void*)other))->_init();
		m_count = countNew;
	}
}

template <class T, class COMPARE>
template <class _COMPARE>
void LinkedList<T, COMPARE>::merge(LinkedList<T, _COMPARE>* other)
{
	if ((void*)this == (void*)other) {
		return;
	}
	ObjectLocker lock(this, other);
	Link<T>* _begin = other->getBegin();
	Link<T>* _end = other->getEnd();
	if (_begin) {
		sl_size countNew = m_count + other->getCount();
		if (m_end) {
			m_end->next = _begin;
			_begin->before = m_end;
		} else {
			m_begin = _begin;
		}
		m_end = _end;
		((LinkedList<T, COMPARE>*)((void*)other))->_init();
		m_count = countNew;
	}
}

template <class T, class COMPARE>
Array<T> LinkedList<T, COMPARE>::toArray_NoLock() const
{
	Array<T> ret;
	if (m_count) {
		ret = Array<T>::create(m_count);
		if (ret.isNotNull()) {
			T* buf = ret.getData();
			sl_size index = 0;
			Link<T>* now = m_begin;
			while (now) {
				buf[index] = now->value;
				now = now->next;
				index++;
			}
		}
	}
	return ret;
}

template <class T, class COMPARE>
Array<T> LinkedList<T, COMPARE>::toArray() const
{
	ObjectLocker lock(this);
	return toArray_NoLock();
}

template <class T, class COMPARE>
List<T> LinkedList<T, COMPARE>::toList_NoLock() const
{
	List<T> ret;
	if (m_count) {
		ret = List<T>::create(m_count);
		if (ret.isNotNull()) {
			ListItems<T> list(ret);
			sl_size index = 0;
			Link<T>* now = m_begin;
			while (now) {
				list[index] = now->value;
				now = now->next;
				index++;
			}
		}
	}
	return ret;
}

template <class T, class COMPARE>
List<T> LinkedList<T, COMPARE>::toList() const
{
	ObjectLocker lock(this);
	return toList_NoLock();
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::findValue_NoLock(const T& value) const
{
	Link<T>* now = m_begin;
	while (now) {
		if (COMPARE::equals(value, now->value)) {
			return now;
		}
		now = now->next;
	}
	return sl_null;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::findValue(const T& value) const
{
	ObjectLocker lock(this);
	return findValue_NoLock(value);
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::removeValue_NoLock(const T& value, sl_bool flagAllValues)
{
	Link<T>* now = m_begin;
	sl_bool bRet = sl_false;
	while (now) {
		Link<T>* next = now->next;
		if (COMPARE::equals(value, now->value)) {
			removeItem_NoLock(now);
			if (!flagAllValues) {
				return sl_true;
			}
			bRet = sl_true;
		}
		now = next;
	}
	return bRet;
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::removeValue(const T& value, sl_bool flagAllValues)
{
	ObjectLocker lock(this);
	return removeValue_NoLock(value, flagAllValues);
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_createItem(const T& value)
{
	Link<T>* item = (Link<T>*)(Base::createMemory(sizeof(Link<T>)));
	if (!item) {
		return sl_null;
	}
	item->next = sl_null;
	item->before = sl_null;
	new (&(item->value)) T(value);
	return item;
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::_freeItem(Link<T>* item)
{
	item->value.T::~T();
	Base::freeMemory(item);
}


template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_pushBackItem(Link<T>* item, sl_size countLimit)
{
	Link<T>* old;
	if (countLimit > 0 && m_count >= countLimit) {
		old = _popFrontItem();
	} else {
		old = sl_null;
	}
	if (m_end) {
		m_end->next = item;
		item->before = m_end;
		m_end = item;
	} else {
		m_begin = item;
		m_end = item;
	}
	m_count++;
	return old;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_popBackItem()
{
	Link<T>* end = m_end;
	if (end) {
		m_count--;
		Link<T>* before = end->before;
		if (before) {
			before->next = sl_null;
			m_end = before;
		} else {
			m_begin = sl_null;
			m_end = sl_null;
		}
	}
	return end;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_pushFrontItem(Link<T>* item, sl_size countLimit)
{
	Link<T>* old;
	if (countLimit > 0 && m_count >= countLimit) {
		old = _popBackItem();
	} else {
		old = sl_null;
	}
	if (m_begin) {
		item->next = m_begin;
		m_begin->before = item;
		m_begin = item;
	} else {
		m_begin = item;
		m_end = item;
	}
	m_count++;
	return old;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_popFrontItem()
{
	Link<T>* begin = m_begin;
	if (begin) {
		m_count--;
		Link<T>* next = begin->next;
		if (next) {
			next->before = sl_null;
			m_begin = next;
		} else {
			m_begin = sl_null;
			m_end = sl_null;
		}
	}
	return begin;
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::_removeItem(Link<T>* item)
{
	m_count--;
	Link<T>* before = item->before;
	Link<T>* next = item->next;
	if (before) {
		before->next = next;
	} else {
		m_begin = next;
	}
	if (next) {
		next->before = before;
	} else {
		m_end = before;
	}
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::_insertBefore(Link<T>* itemWhere, Link<T>* itemNew)
{
	itemNew->next = itemWhere;
	Link<T>* before = itemWhere->before;
	itemNew->before = before;
	itemWhere->before = itemNew;
	if (before) {
		before->next = itemNew;
	} else {
		m_begin = itemNew;
	}
	m_count++;
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::_insertAfter(Link<T>* itemWhere, Link<T>* itemNew)
{
	itemNew->before = itemWhere;
	Link<T>* next = itemWhere->next;
	itemNew->next = next;
	itemWhere->next = itemNew;
	if (next) {
		next->before = itemNew;
	} else {
		m_end = itemNew;
	}
	m_count++;
}

template <class T, class COMPARE>
SLIB_INLINE void LinkedList<T, COMPARE>::_init()
{
	m_begin = sl_null;
	m_end = sl_null;
}


template <class T, class COMPARE>
Link<T>* Queue<T, COMPARE>::push(const T& value, sl_size countLimit)
{
	return this->pushBack(value, countLimit);
}

template <class T, class COMPARE>
Link<T>* Queue<T, COMPARE>::push_NoLock(const T& value, sl_size countLimit)
{
	return this->pushBack_NoLock(value, countLimit);
}

template <class T, class COMPARE>
sl_bool Queue<T, COMPARE>::pop(T* _out)
{
	return this->popFront(_out);
}

template <class T, class COMPARE>
sl_bool Queue<T, COMPARE>::pop_NoLock(T* _out)
{
	return this->popFront_NoLock(_out);
}


template <class T, class COMPARE>
Link<T>* Stack<T, COMPARE>::push(const T& value, sl_size countLimit)
{
	return this->pushBack(value, countLimit);
}

template <class T, class COMPARE>
Link<T>* Stack<T, COMPARE>::push_NoLock(const T& value, sl_size countLimit)
{
	return this->pushBack_NoLock(value, countLimit);
}

template <class T, class COMPARE>
sl_bool Stack<T, COMPARE>::pop(T* _out)
{
	return this->popBack(_out);
}

template <class T, class COMPARE>
sl_bool Stack<T, COMPARE>::pop_NoLock(T* _out)
{
	return this->popBack_NoLock(_out);
}


SLIB_NAMESPACE_END

#endif
