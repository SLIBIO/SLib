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

template <class T>
class LinkPosition
{
public:
	LinkPosition(Link<T>* link);
	
	LinkPosition(const LinkPosition<T>& other);
	
public:
	T& operator*();
	
	sl_bool operator!=(const LinkPosition<T>& p);
	
	LinkPosition<T>& operator++();
	
private:
	Link<T>* link;
	
};

template <class T>
class LinkReversePosition
{
public:
	LinkReversePosition(Link<T>* link);
	
	LinkReversePosition(const LinkReversePosition<T>& other);
	
public:
	T& operator*();
	
	sl_bool operator!=(const LinkReversePosition<T>& p);
	
	LinkReversePosition<T>& operator++();
	
private:
	Link<T>* link;
	
};

class SLIB_EXPORT LinkedListBase : public Object
{
	SLIB_DECLARE_OBJECT
};

template <class T, class COMPARE>
class Queue;

template <class T, class COMPARE>
class Stack;

template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT LinkedList : public LinkedListBase
{
protected:
	Link<T>* m_front;
	Link<T>* m_back;
	sl_size m_count;
	
public:
	LinkedList();
	
	~LinkedList();
	
public:
	void init(Link<T>* front, Link<T>* back, sl_size count);
	
	Link<T>* getFront() const;

	Link<T>* getBack() const;

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
	
	void pushBackAll(const LinkedList<T, COMPARE>* other);
	
	template <class _T, class _COMPARE>
	void pushBackAll(const LinkedList<_T, _COMPARE>* other);
	
	sl_bool popBack_NoLock(T* _out = sl_null);
	
	sl_bool popBack(T* _out = sl_null);
	
	Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushFront(const T& value, sl_size countLimit = 0);
	
	void pushFrontAll(const LinkedList<T, COMPARE>* other);
	
	template <class _T, class _COMPARE>
	void pushFrontAll(const LinkedList<_T, _COMPARE>* other);

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
	
	LinkedList<T, COMPARE>* duplicate_NoLock() const;
	
	LinkedList<T, COMPARE>* duplicate() const;
	
	Link<T>* findValue_NoLock(const T& value) const;
	
	Link<T>* findValue(const T& value) const;
	
	sl_bool removeValue_NoLock(const T& value, sl_bool flagAllValues = sl_false);
	
	sl_bool removeValue(const T& value, sl_bool flagAllValues = sl_false);
	
	Link<T>* push_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* push(const T& value, sl_size countLimit = 0);
	
	void pushAll(const LinkedList<T, COMPARE>* other);
	
	template <class _T, class _COMPARE>
	void pushAll(const LinkedList<_T, _COMPARE>* other);
	
	Queue<T, COMPARE>* toQueue();
	
	const Queue<T, COMPARE>* toQueue() const;
	
	Stack<T, COMPARE>* toStack();
	
	const Stack<T, COMPARE>* toStack() const;
	
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
	sl_bool pop_NoLock(T* _out = sl_null);
	
	sl_bool pop(T* _out = sl_null);
	
	// range-based for loop
	LinkPosition<T> begin() const;
	
	LinkPosition<T> end() const;

};

template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT Stack : public LinkedList<T, COMPARE>
{
public:
	sl_bool pop_NoLock(T* _out = sl_null);
	
	sl_bool pop(T* _out = sl_null);
	
	// range-based for loop
	LinkReversePosition<T> begin() const;
	
	LinkReversePosition<T> end() const;

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
	m_front = sl_null;
	m_back = sl_null;
	m_count = 0;
}

template <class T, class COMPARE>
SLIB_INLINE LinkedList<T, COMPARE>::~LinkedList()
{
	removeAll_NoLock();
}

template <class T, class COMPARE>
SLIB_INLINE void LinkedList<T, COMPARE>::init(Link<T>* front, Link<T>* back, sl_size count)
{
	m_front = front;
	m_back = back;
	m_count = count;
}

template <class T, class COMPARE>
SLIB_INLINE Link<T>* LinkedList<T, COMPARE>::getFront() const
{
	return m_front;
}

template <class T, class COMPARE>
SLIB_INLINE Link<T>* LinkedList<T, COMPARE>::getBack() const
{
	return m_back;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size LinkedList<T, COMPARE>::getCount() const
{
	return m_count;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool LinkedList<T, COMPARE>::isEmpty() const
{
	return m_front == sl_null;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool LinkedList<T, COMPARE>::isNotEmpty() const
{
	return m_front != sl_null;
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
	Link<T>* now = m_front;
	_init();
	freeLink(now);
}

template <class T, class COMPARE>
void LinkedList<T, COMPARE>::removeAll()
{
	Link<T>* now;
	{
		ObjectLocker lock(this);
		now = m_front;
		_init();
	}
	freeLink(now);
}

template <class T, class COMPARE>
sl_bool LinkedList<T, COMPARE>::getFirstElement_NoLock(T* _out) const
{
	if (m_front) {
		if (_out) {
			*_out = m_front->value;
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
	if (m_back) {
		if (_out) {
			*_out = m_back->value;
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
void LinkedList<T, COMPARE>::pushBackAll(const LinkedList<T, COMPARE>* other)
{
	ObjectLocker lock(this, other);
	Link<T>* link = other->getFront();
	while (link) {
		if (!(pushBack_NoLock(link->value))) {
			return;
		}
		link = link->next;
	}
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
void LinkedList<T, COMPARE>::pushBackAll(const LinkedList<_T, _COMPARE>* other)
{
	ObjectLocker lock(this, other);
	Link<_T>* link = other->getFront();
	while (link) {
		if (!(pushBack_NoLock(link->value))) {
			return;
		}
		link = link->next;
	}
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
void LinkedList<T, COMPARE>::pushFrontAll(const LinkedList<T, COMPARE>* other)
{
	ObjectLocker lock(this, other);
	Link<T>* link = other->getBack();
	while (link) {
		if (!(pushFront_NoLock(link->value))) {
			return;
		}
		link = link->before;
	}
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
void LinkedList<T, COMPARE>::pushFrontAll(const LinkedList<_T, _COMPARE>* other)
{
	ObjectLocker lock(this, other);
	Link<_T>* link = other->getBack();
	while (link) {
		if (!(pushFront_NoLock(link->value))) {
			return;
		}
		link = link->before;
	}
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
	Link<T>* _front = other->getFront();
	Link<T>* _back = other->getBack();
	if (_front) {
		sl_size countNew = m_count + other->getCount();
		if (m_back) {
			m_back->next = _front;
			_front->before = m_back;
		} else {
			m_front = _front;
		}
		m_back = _back;
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
	Link<T>* _front = other->getFront();
	Link<T>* _back = other->getBack();
	if (_front) {
		sl_size countNew = m_count + other->getCount();
		if (m_back) {
			m_back->next = _front;
			_front->before = m_back;
		} else {
			m_front = _front;
		}
		m_back = _back;
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
			Link<T>* now = m_front;
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
			Link<T>* now = m_front;
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
LinkedList<T, COMPARE>* LinkedList<T, COMPARE>::duplicate_NoLock() const
{
	LinkedList<T, COMPARE>* ret = new LinkedList<T, COMPARE>;
	if (ret) {
		Link<T>* now = m_front;
		while (now) {
			if (!(ret->pushBack_NoLock(now->value))) {
				delete ret;
				return sl_null;
			}
			now = now->next;
		}
		return ret;
	}
	return sl_null;
}

template <class T, class COMPARE>
LinkedList<T, COMPARE>* LinkedList<T, COMPARE>::duplicate() const
{
	ObjectLocker lock(this);
	return duplicate_NoLock();
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::findValue_NoLock(const T& value) const
{
	Link<T>* now = m_front;
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
	Link<T>* now = m_front;
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
SLIB_INLINE Queue<T, COMPARE>* LinkedList<T, COMPARE>::toQueue()
{
	return (Queue<T, COMPARE>*)this;
}

template <class T, class COMPARE>
SLIB_INLINE const Queue<T, COMPARE>* LinkedList<T, COMPARE>::toQueue() const
{
	return (const Queue<T, COMPARE>*)this;
}

template <class T, class COMPARE>
SLIB_INLINE Stack<T, COMPARE>* LinkedList<T, COMPARE>::toStack()
{
	return (Stack<T, COMPARE>*)this;
}

template <class T, class COMPARE>
SLIB_INLINE const Stack<T, COMPARE>* LinkedList<T, COMPARE>::toStack() const
{
	return (const Stack<T, COMPARE>*)this;
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
	if (m_back) {
		m_back->next = item;
		item->before = m_back;
		m_back = item;
	} else {
		m_front = item;
		m_back = item;
	}
	m_count++;
	return old;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_popBackItem()
{
	Link<T>* back = m_back;
	if (back) {
		m_count--;
		Link<T>* before = back->before;
		if (before) {
			before->next = sl_null;
			m_back = before;
		} else {
			m_front = sl_null;
			m_back = sl_null;
		}
	}
	return back;
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
	if (m_front) {
		item->next = m_front;
		m_front->before = item;
		m_front = item;
	} else {
		m_front = item;
		m_back = item;
	}
	m_count++;
	return old;
}

template <class T, class COMPARE>
Link<T>* LinkedList<T, COMPARE>::_popFrontItem()
{
	Link<T>* front = m_front;
	if (front) {
		m_count--;
		Link<T>* next = front->next;
		if (next) {
			next->before = sl_null;
			m_front = next;
		} else {
			m_front = sl_null;
			m_back = sl_null;
		}
	}
	return front;
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
		m_front = next;
	}
	if (next) {
		next->before = before;
	} else {
		m_back = before;
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
		m_front = itemNew;
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
		m_back = itemNew;
	}
	m_count++;
}

template <class T, class COMPARE>
SLIB_INLINE void LinkedList<T, COMPARE>::_init()
{
	m_front = sl_null;
	m_back = sl_null;
	m_count = 0;
}

template <class T, class COMPARE>
SLIB_INLINE Link<T>* LinkedList<T, COMPARE>::push_NoLock(const T& value, sl_size countLimit)
{
	return pushBack_NoLock(value, countLimit);
}

template <class T, class COMPARE>
SLIB_INLINE Link<T>* LinkedList<T, COMPARE>::push(const T& value, sl_size countLimit)
{
	return pushBack(value, countLimit);
}

template <class T, class COMPARE>
SLIB_INLINE void LinkedList<T, COMPARE>::pushAll(const LinkedList<T, COMPARE>* other)
{
	pushBackAll(other);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
SLIB_INLINE void LinkedList<T, COMPARE>::pushAll(const LinkedList<_T, _COMPARE>* other)
{
	pushBackAll(other);
}


template <class T, class COMPARE>
SLIB_INLINE sl_bool Queue<T, COMPARE>::pop_NoLock(T* _out)
{
	return this->popFront_NoLock(_out);
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool Queue<T, COMPARE>::pop(T* _out)
{
	return this->popFront(_out);
}

template <class T, class COMPARE>
SLIB_INLINE LinkPosition<T> Queue<T, COMPARE>::begin() const
{
	return LinkPosition<T>(this->m_front);
}

template <class T, class COMPARE>
SLIB_INLINE LinkPosition<T> Queue<T, COMPARE>::end() const
{
	return LinkPosition<T>(sl_null);
}


template <class T, class COMPARE>
SLIB_INLINE sl_bool Stack<T, COMPARE>::pop_NoLock(T* _out)
{
	return this->popBack_NoLock(_out);
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool Stack<T, COMPARE>::pop(T* _out)
{
	return this->popBack(_out);
}

template <class T, class COMPARE>
SLIB_INLINE LinkReversePosition<T> Stack<T, COMPARE>::begin() const
{
	return LinkReversePosition<T>(this->m_back);
}

template <class T, class COMPARE>
SLIB_INLINE LinkReversePosition<T> Stack<T, COMPARE>::end() const
{
	return LinkReversePosition<T>(sl_null);
}

template <class T>
SLIB_INLINE LinkPosition<T>::LinkPosition(Link<T>* _link) : link(_link) {}

template <class T>
SLIB_INLINE LinkPosition<T>::LinkPosition(const LinkPosition<T>& other) : link(other.link) {};

template <class T>
SLIB_INLINE T& LinkPosition<T>::operator*()
{
	return link->value;
}

template <class T>
SLIB_INLINE sl_bool LinkPosition<T>::operator!=(const LinkPosition<T>& p)
{
	return link != p.link;
}

template <class T>
SLIB_INLINE LinkPosition<T>& LinkPosition<T>::operator++()
{
	link = link->next;
	return *this;
}


template <class T>
SLIB_INLINE LinkReversePosition<T>::LinkReversePosition(Link<T>* _link) : link(_link) {}

template <class T>
SLIB_INLINE LinkReversePosition<T>::LinkReversePosition(const LinkReversePosition<T>& other) : link(other.link) {};

template <class T>
SLIB_INLINE T& LinkReversePosition<T>::operator*()
{
	return link->value;
}

template <class T>
SLIB_INLINE sl_bool LinkReversePosition<T>::operator!=(const LinkReversePosition<T>& p)
{
	return link != p.link;
}

template <class T>
SLIB_INLINE LinkReversePosition<T>& LinkReversePosition<T>::operator++()
{
	link = link->before;
	return *this;
}

SLIB_NAMESPACE_END

#endif
