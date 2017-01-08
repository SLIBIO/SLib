#ifndef CHECKHEADER_SLIB_CORE_LINKED_LIST
#define CHECKHEADER_SLIB_CORE_LINKED_LIST

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
class SLIB_EXPORT LinkPosition
{
public:
	LinkPosition();
	
	LinkPosition(Link<T>* link);
	
	LinkPosition(const LinkPosition<T>& other);
	
public:
	T& operator*();
	
	sl_bool operator!=(const LinkPosition<T>& p);
	
	LinkPosition<T>& operator++();
	
private:
	Link<T>* link;
	
};

class SLIB_EXPORT CLinkedListBase : public Object
{
	SLIB_DECLARE_OBJECT
};

template <class T>
class SLIB_EXPORT CLinkedList : public CLinkedListBase
{
protected:
	Link<T>* m_front;
	Link<T>* m_back;
	sl_size m_count;
	
public:
	CLinkedList();
	
	~CLinkedList();
	
public:
	// not-free existing links
	void init();

	// not-free existing links
	void init(Link<T>* front, Link<T>* back, sl_size count);
	
	// not-free existing links
	void init(const CLinkedList<T>* other);
	
public:
	Link<T>* getFront() const;

	Link<T>* getBack() const;

	sl_size getCount() const;
	
	sl_bool isEmpty() const;

	sl_bool isNotEmpty() const;
	
public:
	sl_bool getFirstItem_NoLock(T* _out) const;
	
	sl_bool getFirstItem(T* _out) const;
	
	sl_bool getLastItem_NoLock(T* _out) const;
	
	sl_bool getLastItem(T* _out) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	Link<T>* search_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	Link<T>* search(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool contains_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushBack(const T& value, sl_size countLimit = 0);
	
	template <class _T>
	void pushBackAll(const CLinkedList<_T>* other);
	
	sl_bool popBack_NoLock(T* _out = sl_null);
	
	sl_bool popBack(T* _out = sl_null);
	
	Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushFront(const T& value, sl_size countLimit = 0);
	
	template <class _T>
	void pushFrontAll(const CLinkedList<_T>* other);

	sl_bool popFront_NoLock(T* _out = sl_null);
	
	sl_bool popFront(T* _out = sl_null);
	
	Link<T>* insertBefore_NoLock(Link<T>* itemWhere, const T& value);
	
	Link<T>* insertBefore(Link<T>* itemWhere, const T& value);
	
	Link<T>* insertAfter_NoLock(Link<T>* itemWhere, const T& value);
	
	Link<T>* insertAfter(Link<T>* itemWhere, const T& value);
	
	void removeItem_NoLock(Link<T>* item);
	
	void removeItem(Link<T>* item);

	void removeAll_NoLock();
	
	void removeAll();

	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool removeValue_NoLock(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS());
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS());
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_size removeItemsByValue_NoLock(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS());
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_size removeItemsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS());
	
	void merge(CLinkedList<T>* other);
	
	Array<T> toArray_NoLock() const;
	
	Array<T> toArray() const;
	
	List<T> toList_NoLock() const;
	
	List<T> toList() const;
	
	CLinkedList<T>* duplicate_NoLock() const;
	
	CLinkedList<T>* duplicate() const;
	
	LinkPosition<T> begin() const;
	
	LinkPosition<T> end() const;

	static void freeLink(Link<T>* link);
	
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
	
};

template <class T>
class SLIB_EXPORT LinkedList
{
public:
	Ref< CLinkedList<T> > ref;
	SLIB_REF_WRAPPER(LinkedList, CLinkedList<T>)
	
public:
	static LinkedList<T> create();

public:
	Link<T>* getFront() const;
	
	Link<T>* getBack() const;
	
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
public:
	sl_bool getFirstItem_NoLock(T* _out) const;
	
	sl_bool getFirstItem(T* _out) const;
	
	sl_bool getLastItem_NoLock(T* _out) const;
	
	sl_bool getLastItem(T* _out) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	Link<T>* search_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	Link<T>* search(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool contains_NoLock(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;

	Link<T>* pushBack_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushBack(const T& value, sl_size countLimit = 0);
	
	template <class _T>
	void pushBackAll(const LinkedList<_T>& other);
	
	sl_bool popBack_NoLock(T* _out = sl_null) const;
	
	sl_bool popBack(T* _out = sl_null) const;
	
	Link<T>* pushFront_NoLock(const T& value, sl_size countLimit = 0);
	
	Link<T>* pushFront(const T& value, sl_size countLimit = 0);
	
	template <class _T>
	void pushFrontAll(const LinkedList<_T>& other);
	
	sl_bool popFront_NoLock(T* _out = sl_null) const;
	
	sl_bool popFront(T* _out = sl_null) const;
	
	Link<T>* insertBefore_NoLock(Link<T>* itemWhere, const T& value) const;
	
	Link<T>* insertBefore(Link<T>* itemWhere, const T& value) const;
	
	Link<T>* insertAfter_NoLock(Link<T>* itemWhere, const T& value) const;
	
	Link<T>* insertAfter(Link<T>* itemWhere, const T& value) const;
	
	void removeItem_NoLock(Link<T>* item) const;
	
	void removeItem(Link<T>* item) const;
	
	void removeAll_NoLock() const;
	
	void removeAll() const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool removeValue_NoLock(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_size removeItemsByValue_NoLock(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_size removeItemsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;
	
	void merge(LinkedList<T>& other);
	
	Array<T> toArray_NoLock() const;
	
	Array<T> toArray() const;
	
	List<T> toList_NoLock() const;
	
	List<T> toList() const;
	
	LinkedList<T> duplicate_NoLock() const;
	
	LinkedList<T> duplicate() const;
	
	LinkPosition<T> begin() const;
	
	LinkPosition<T> end() const;
	
};

template <class T>
class SLIB_EXPORT Atomic< LinkedList<T> >
{
public:
	AtomicRef< CLinkedList<T> > ref;
	SLIB_ATOMIC_REF_WRAPPER(CLinkedList<T>)
	
public:
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
public:
	sl_bool getFirstItem(T* _out) const;
	
	sl_bool getLastItem(T* _out) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool contains(const _T& value, const EQUALS& equals = EQUALS()) const;
	
	sl_bool pushBack(const T& value, sl_size countLimit = 0);
	
	template <class _T>
	void pushBackAll(const LinkedList<_T>& other);
	
	sl_bool popBack(T* _out = sl_null) const;
	
	sl_bool pushFront(const T& value, sl_size countLimit = 0);
	
	template <class _T>
	void pushFrontAll(const LinkedList<_T>& other);
	
	sl_bool popFront(T* _out = sl_null) const;
	
	void removeAll() const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_bool removeValue(const _T& value, T* outValue = sl_null, const EQUALS& equals = EQUALS()) const;
	
	template < class _T, class EQUALS = Equals<T, _T> >
	sl_size removeItemsByValue(const _T& value, List<T>* outValues = sl_null, const EQUALS& equals = EQUALS()) const;
	
	void merge(LinkedList<T>& other);
	
	Array<T> toArray() const;
	
	List<T> toList() const;
	
	LinkedList<T> duplicate() const;
	
	LinkPosition<T> begin() const;
	
	LinkPosition<T> end() const;
	
};

template <class T>
using AtomicLinkedList = Atomic< LinkedList<T> >;


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T>
SLIB_INLINE CLinkedList<T>::CLinkedList()
{
	m_front = sl_null;
	m_back = sl_null;
	m_count = 0;
}

template <class T>
CLinkedList<T>::~CLinkedList()
{
	removeAll_NoLock();
}

template <class T>
SLIB_INLINE void CLinkedList<T>::init()
{
	m_front = sl_null;
	m_back = sl_null;
	m_count = 0;
}

template <class T>
SLIB_INLINE void CLinkedList<T>::init(Link<T>* front, Link<T>* back, sl_size count)
{
	m_front = front;
	m_back = back;
	m_count = count;
}

template <class T>
SLIB_INLINE void CLinkedList<T>::init(const CLinkedList<T>* other)
{
	m_front = other->m_front;
	m_back = other->m_back;
	m_count = other->m_count;
}

template <class T>
SLIB_INLINE Link<T>* CLinkedList<T>::getFront() const
{
	return m_front;
}

template <class T>
SLIB_INLINE Link<T>* CLinkedList<T>::getBack() const
{
	return m_back;
}

template <class T>
SLIB_INLINE sl_size CLinkedList<T>::getCount() const
{
	return m_count;
}

template <class T>
SLIB_INLINE sl_bool CLinkedList<T>::isEmpty() const
{
	return m_front == sl_null;
}

template <class T>
SLIB_INLINE sl_bool CLinkedList<T>::isNotEmpty() const
{
	return m_front != sl_null;
}

template <class T>
sl_bool CLinkedList<T>::getFirstItem_NoLock(T* _out) const
{
	if (m_front) {
		if (_out) {
			*_out = m_front->value;
		}
		return sl_true;
	}
	return sl_false;
}

template <class T>
sl_bool CLinkedList<T>::getFirstItem(T* _out) const
{
	ObjectLocker lock(this);
	return getFirstItem_NoLock(_out);
}

template <class T>
sl_bool CLinkedList<T>::getLastItem_NoLock(T* _out) const
{
	if (m_back) {
		if (_out) {
			*_out = m_back->value;
		}
		return sl_true;
	}
	return sl_false;
}

template <class T>
sl_bool CLinkedList<T>::getLastItem(T* _out) const
{
	ObjectLocker lock(this);
	return getLastItem_NoLock(_out);
}

template <class T>
template <class _T, class EQUALS>
Link<T>* CLinkedList<T>::search_NoLock(const _T& value, const EQUALS& equals) const
{
	Link<T>* now = m_front;
	while (now) {
		if (equals(now->value, value)) {
			return now;
		}
		now = now->next;
	}
	return sl_null;
}

template <class T>
template <class _T, class EQUALS>
Link<T>* CLinkedList<T>::search(const _T& value, const EQUALS& equals) const
{
	ObjectLocker lock(this);
	return findValue_NoLock(value);
}

template <class T>
template <class _T, class EQUALS>
sl_bool CLinkedList<T>::contains_NoLock(const _T& value, const EQUALS& equals) const
{
	Link<T>* now = m_front;
	while (now) {
		if (equals(now->value, value)) {
			return sl_true;
		}
		now = now->next;
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_bool CLinkedList<T>::contains(const _T& value, const EQUALS& equals) const
{
	ObjectLocker lock(this);
	return contains_NoLock(value);
}

template <class T>
Link<T>* CLinkedList<T>::pushBack_NoLock(const T& value, sl_size countLimit)
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

template <class T>
Link<T>* CLinkedList<T>::pushBack(const T& value, sl_size countLimit)
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

template <class T>
template <class _T>
void CLinkedList<T>::pushBackAll(const CLinkedList<_T>* other)
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

template <class T>
sl_bool CLinkedList<T>::popBack_NoLock(T* _out)
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

template <class T>
sl_bool CLinkedList<T>::popBack(T* _out)
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

template <class T>
Link<T>* CLinkedList<T>::pushFront_NoLock(const T& value, sl_size countLimit)
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

template <class T>
Link<T>* CLinkedList<T>::pushFront(const T& value, sl_size countLimit)
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

template <class T>
template <class _T>
void CLinkedList<T>::pushFrontAll(const CLinkedList<_T>* other)
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

template <class T>
sl_bool CLinkedList<T>::popFront_NoLock(T* _out)
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

template <class T>
sl_bool CLinkedList<T>::popFront(T* _out)
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

template <class T>
Link<T>* CLinkedList<T>::insertBefore_NoLock(Link<T>* itemWhere, const T& value)
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

template <class T>
Link<T>* CLinkedList<T>::insertBefore(Link<T>* itemWhere, const T& value)
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

template <class T>
Link<T>* CLinkedList<T>::insertAfter_NoLock(Link<T>* itemWhere, const T& value)
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

template <class T>
Link<T>* CLinkedList<T>::insertAfter(Link<T>* itemWhere, const T& value)
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

template <class T>
void CLinkedList<T>::removeItem_NoLock(Link<T>* item)
{
	if (item) {
		_removeItem(item);
		_freeItem(item);
	}
}

template <class T>
void CLinkedList<T>::removeItem(Link<T>* item)
{
	if (item) {
		{
			ObjectLocker lock(this);
			_removeItem(item);
		}
		_freeItem(item);
	}
}

template <class T>
void CLinkedList<T>::removeAll_NoLock()
{
	Link<T>* now = m_front;
	init();
	freeLink(now);
}

template <class T>
void CLinkedList<T>::removeAll()
{
	Link<T>* now;
	{
		ObjectLocker lock(this);
		now = m_front;
		init();
	}
	freeLink(now);
}

template <class T>
template <class _T, class EQUALS>
sl_bool CLinkedList<T>::removeValue_NoLock(const _T& value, T* outValue, const EQUALS& equals)
{
	Link<T>* now = m_front;
	while (now) {
		Link<T>* next = now->next;
		if (equals(now->value, value)) {
			if (outValue) {
				*outValue = now->value;
			}
			removeItem_NoLock(now);
			return sl_true;
		}
		now = next;
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_bool CLinkedList<T>::removeValue(const _T& value, T* outValue, const EQUALS& equals)
{
	ObjectLocker lock(this);
	return removeValue_NoLock(value, outValue, equals);
}

template <class T>
template <class _T, class EQUALS>
sl_size CLinkedList<T>::removeItemsByValue_NoLock(const _T& value, List<T>* outValues, const EQUALS& equals)
{
	sl_size n = 0;
	Link<T>* now = m_front;
	while (now) {
		Link<T>* next = now->next;
		if (equals(now->value, value)) {
			n++;
			if (outValues) {
				outValues->add_NoLock(now->value);
			}
			removeItem_NoLock(now);
		}
		now = next;
	}
	return n;
}

template <class T>
template <class _T, class EQUALS>
sl_size CLinkedList<T>::removeItemsByValue(const _T& value, List<T>* outValues, const EQUALS& equals)
{
	ObjectLocker lock(this);
	return removeItemsByValue_NoLock(value, outValues, equals);
}

template <class T>
void CLinkedList<T>::merge(CLinkedList<T>* other)
{
	if (!other) {
		return;
	}
	if (this == other) {
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
		other->init();
		m_count = countNew;
	}
}

template <class T>
Array<T> CLinkedList<T>::toArray_NoLock() const
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

template <class T>
Array<T> CLinkedList<T>::toArray() const
{
	ObjectLocker lock(this);
	return toArray_NoLock();
}

template <class T>
List<T> CLinkedList<T>::toList_NoLock() const
{
	List<T> ret;
	if (m_count) {
		ret = List<T>::create(m_count);
		if (ret.isNotNull()) {
			ListElements<T> list(ret);
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

template <class T>
List<T> CLinkedList<T>::toList() const
{
	ObjectLocker lock(this);
	return toList_NoLock();
}

template <class T>
CLinkedList<T>* CLinkedList<T>::duplicate_NoLock() const
{
	CLinkedList<T>* ret = new CLinkedList<T>;
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

template <class T>
CLinkedList<T>* CLinkedList<T>::duplicate() const
{
	ObjectLocker lock(this);
	return duplicate_NoLock();
}

template <class T>
SLIB_INLINE LinkPosition<T> CLinkedList<T>::begin() const
{
	return LinkPosition<T>(m_front);
}

template <class T>
SLIB_INLINE LinkPosition<T> CLinkedList<T>::end() const
{
	return LinkPosition<T>();
}

template <class T>
void CLinkedList<T>::freeLink(Link<T>* link)
{
	while (link) {
		Link<T>* next = link->next;
		_freeItem(link);
		link = next;
	}
}

template <class T>
Link<T>* CLinkedList<T>::_createItem(const T& value)
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

template <class T>
void CLinkedList<T>::_freeItem(Link<T>* item)
{
	item->value.T::~T();
	Base::freeMemory(item);
}


template <class T>
Link<T>* CLinkedList<T>::_pushBackItem(Link<T>* item, sl_size countLimit)
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

template <class T>
Link<T>* CLinkedList<T>::_popBackItem()
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

template <class T>
Link<T>* CLinkedList<T>::_pushFrontItem(Link<T>* item, sl_size countLimit)
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

template <class T>
Link<T>* CLinkedList<T>::_popFrontItem()
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

template <class T>
void CLinkedList<T>::_removeItem(Link<T>* item)
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

template <class T>
void CLinkedList<T>::_insertBefore(Link<T>* itemWhere, Link<T>* itemNew)
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

template <class T>
void CLinkedList<T>::_insertAfter(Link<T>* itemWhere, Link<T>* itemNew)
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


template <class T>
LinkedList<T> LinkedList<T>::create()
{
	return new CLinkedList<T>;
}

template <class T>
SLIB_INLINE Link<T>* LinkedList<T>::getFront() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getFront();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE Link<T>* LinkedList<T>::getBack() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getBack();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE sl_size LinkedList<T>::getCount() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getCount();
	}
	return 0;
}

template <class T>
SLIB_INLINE sl_bool LinkedList<T>::isEmpty() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->isEmpty();
	}
	return sl_true;
}

template <class T>
SLIB_INLINE sl_bool LinkedList<T>::isNotEmpty() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->isNotEmpty();
	}
	return sl_false;
}

template <class T>
sl_bool LinkedList<T>::getFirstItem_NoLock(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getFirstItem_NoLock(_out);
	}
	return sl_false;
}

template <class T>
sl_bool LinkedList<T>::getFirstItem(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getFirstItem(_out);
	}
	return sl_false;
}

template <class T>
sl_bool LinkedList<T>::getLastItem_NoLock(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getLastItem_NoLock(_out);
	}
	return sl_false;
}

template <class T>
sl_bool LinkedList<T>::getLastItem(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->getLastItem(_out);
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
Link<T>* LinkedList<T>::search_NoLock(const _T& value, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->search_NoLock(value, equals);
	}
	return sl_null;
}

template <class T>
template <class _T, class EQUALS>
Link<T>* LinkedList<T>::search(const _T& value, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->search(value, equals);
	}
	return sl_null;
}

template <class T>
template <class _T, class EQUALS>
sl_bool LinkedList<T>::contains_NoLock(const _T& value, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->contains_NoLock(value, equals);
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_bool LinkedList<T>::contains(const _T& value, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->contains(value, equals);
	}
	return sl_false;
}

template <class T>
Link<T>* LinkedList<T>::pushBack_NoLock(const T& value, sl_size countLimit)
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->pushBack_NoLock(value, countLimit);
	} else {
		obj = new CLinkedList<T>;
		if (obj) {
			ref = obj;
			return obj->pushBack_NoLock(value, countLimit);
		}
	}
	return sl_null;
}

template <class T>
Link<T>* LinkedList<T>::pushBack(const T& value, sl_size countLimit)
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->pushBack(value, countLimit);
	} else {
		obj = new CLinkedList<T>;
		if (obj) {
			ref = obj;
			return obj->pushBack_NoLock(value, countLimit);
		}
	}
	return sl_null;
}

template <class T>
template <class _T>
void LinkedList<T>::pushBackAll(const LinkedList<_T>& _other)
{
	CLinkedList<T>* other = _other.ref._ptr;
	if (other) {
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->pushBackAll(other);
		} else {
			ref = other->duplicate();
		}
	}
}

template <class T>
sl_bool LinkedList<T>::popBack_NoLock(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->popBack_NoLock(_out);
	}
	return sl_false;
}

template <class T>
sl_bool LinkedList<T>::popBack(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->popBack(_out);
	}
	return sl_false;
}

template <class T>
Link<T>* LinkedList<T>::pushFront_NoLock(const T& value, sl_size countLimit)
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->pushFront_NoLock(value, countLimit);
	} else {
		obj = new CLinkedList<T>;
		if (obj) {
			ref = obj;
			return obj->pushFront_NoLock(value, countLimit);
		}
	}
	return sl_null;
}

template <class T>
Link<T>* LinkedList<T>::pushFront(const T& value, sl_size countLimit)
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->pushFront(value, countLimit);
	} else {
		obj = new CLinkedList<T>;
		if (obj) {
			ref = obj;
			return obj->pushFront_NoLock(value, countLimit);
		}
	}
	return sl_null;
}

template <class T>
template <class _T>
void LinkedList<T>::pushFrontAll(const LinkedList<_T>& _other)
{
	LinkedList<T>* other = _other.ref._ptr;
	if (other) {
		CLinkedList<T>* obj = ref._ptr;
		if (obj) {
			obj->pushFrontAll(other);
		} else {
			ref = other->duplicate();
		}
	}
}

template <class T>
sl_bool LinkedList<T>::popFront_NoLock(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->popFront_NoLock(_out);
	}
	return sl_false;
}

template <class T>
sl_bool LinkedList<T>::popFront(T* _out) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->popFront(_out);
	}
	return sl_false;
}

template <class T>
Link<T>* LinkedList<T>::insertBefore_NoLock(Link<T>* itemWhere, const T& value) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->insertBefore_NoLock(itemWhere, value);
	}
	return sl_null;
}

template <class T>
Link<T>* LinkedList<T>::insertBefore(Link<T>* itemWhere, const T& value) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->insertBefore(itemWhere, value);
	}
	return sl_null;
}

template <class T>
Link<T>* LinkedList<T>::insertAfter_NoLock(Link<T>* itemWhere, const T& value) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->insertAfter_NoLock(itemWhere, value);
	}
	return sl_null;
}

template <class T>
Link<T>* LinkedList<T>::insertAfter(Link<T>* itemWhere, const T& value) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->insertAfter_NoLock(itemWhere, value);
	}
	return sl_null;
}

template <class T>
void LinkedList<T>::removeItem_NoLock(Link<T>* item) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->removeItem_NoLock(item);
	}
}

template <class T>
void LinkedList<T>::removeItem(Link<T>* item) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->removeItem(item);
	}
}

template <class T>
void LinkedList<T>::removeAll_NoLock() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->removeAll_NoLock();
	}
}

template <class T>
void LinkedList<T>::removeAll() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->removeAll();
	}
}

template <class T>
template <class _T, class EQUALS>
sl_bool LinkedList<T>::removeValue_NoLock(const _T& value, T* outValue, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->removeValue_NoLock(value, outValue, equals);
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_bool LinkedList<T>::removeValue(const _T& value, T* outValue, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->removeValue(value, outValue, equals);
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_size LinkedList<T>::removeItemsByValue_NoLock(const _T& value, List<T>* outValues, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->removeItemsByValue_NoLock(value, outValues, equals);
	}
	return 0;
}

template <class T>
template <class _T, class EQUALS>
sl_size LinkedList<T>::removeItemsByValue(const _T& value, List<T>* outValues, const EQUALS& equals) const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->removeItemsByValue(value, outValues, equals);
	}
	return 0;
}

template <class T>
void LinkedList<T>::merge(LinkedList<T>& _other)
{
	CLinkedList<T>* other = _other.ref._ptr;
	if (!other) {
		return;
	}
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		obj->merge(other);
	} else {
		obj = new CLinkedList<T>;
		if (obj) {
			obj->init(other);
			other->init();
			ref = obj;
		}
	}
}

template <class T>
Array<T> LinkedList<T>::toArray_NoLock() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->toArray_NoLock();
	}
	return sl_null;
}

template <class T>
Array<T> LinkedList<T>::toArray() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->toArray();
	}
	return sl_null;
}

template <class T>
List<T> LinkedList<T>::toList_NoLock() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->toList_NoLock();
	}
	return sl_null;
}

template <class T>
List<T> LinkedList<T>::toList() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->toList();
	}
	return sl_null;
}

template <class T>
LinkedList<T> LinkedList<T>::duplicate_NoLock() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->duplicate_NoLock();
	}
	return sl_null;
}

template <class T>
LinkedList<T> LinkedList<T>::duplicate() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->duplicate();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE LinkPosition<T> LinkedList<T>::begin() const
{
	CLinkedList<T>* obj = ref._ptr;
	if (obj) {
		return obj->begin();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE LinkPosition<T> LinkedList<T>::end() const
{
	return LinkPosition<T>();
}


template <class T>
SLIB_INLINE sl_size Atomic< LinkedList<T> >::getCount() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount();
	}
	return 0;
}

template <class T>
SLIB_INLINE sl_bool Atomic< LinkedList<T> >::isEmpty() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->isEmpty();
	}
	return sl_true;
}

template <class T>
SLIB_INLINE sl_bool Atomic< LinkedList<T> >::isNotEmpty() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->isNotEmpty();
	}
	return sl_false;
}

template <class T>
sl_bool Atomic< LinkedList<T> >::getFirstItem(T* _out) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getFirstItem(_out);
	}
	return sl_false;
}

template <class T>
sl_bool Atomic< LinkedList<T> >::getLastItem(T* _out) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getLastItem(_out);
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_bool Atomic< LinkedList<T> >::contains(const _T& value, const EQUALS& equals) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->contains(value, equals);
	}
	return sl_false;
}

template <class T>
sl_bool Atomic< LinkedList<T> >::pushBack(const T& value, sl_size countLimit)
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->pushBack(value, countLimit) != sl_null;
	} else {
		obj = new CLinkedList<T>;
		if (obj.isNotNull()) {
			if (obj->pushBack_NoLock(value, countLimit)) {
				ref = obj;
				return sl_true;
			}
		}
	}
	return sl_false;
}

template <class T>
template <class _T>
void Atomic< LinkedList<T> >::pushBackAll(const LinkedList<_T>& _other)
{
	CLinkedList<T>* other = _other.ref._ptr;
	if (other) {
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->pushBackAll(other);
		} else {
			ref = other->duplicate();
		}
	}
}

template <class T>
sl_bool Atomic< LinkedList<T> >::popBack(T* _out) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->popBack(_out);
	}
	return sl_false;
}

template <class T>
sl_bool Atomic< LinkedList<T> >::pushFront(const T& value, sl_size countLimit)
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->pushFront(value, countLimit) != sl_null;
	} else {
		obj = new CLinkedList<T>;
		if (obj.isNotNull()) {
			if (obj->pushFront_NoLock(value, countLimit)) {
				ref = obj;
				return sl_true;
			}
		}
	}
	return sl_null;
}

template <class T>
template <class _T>
void Atomic< LinkedList<T> >::pushFrontAll(const LinkedList<_T>& _other)
{
	CLinkedList<T>* other = _other.ref._ptr;
	if (other) {
		Ref< CLinkedList<T> > obj(ref);
		if (obj.isNotNull()) {
			obj->pushFrontAll(other);
		} else {
			ref = other->duplicate();
		}
	}
}

template <class T>
sl_bool Atomic< LinkedList<T> >::popFront(T* _out) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->popFront(_out);
	}
	return sl_false;
}

template <class T>
void Atomic< LinkedList<T> >::removeAll() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		obj->removeAll();
	}
}

template <class T>
template <class _T, class EQUALS>
sl_bool Atomic< LinkedList<T> >::removeValue(const _T& value, T* outValue, const EQUALS& equals) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->removeValue(value, outValue, equals);
	}
	return sl_false;
}

template <class T>
template <class _T, class EQUALS>
sl_size Atomic< LinkedList<T> >::removeItemsByValue(const _T& value, List<T>* outValues, const EQUALS& equals) const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->removeItemsByValue(value, outValues, equals);
	}
	return 0;
}

template <class T>
void Atomic< LinkedList<T> >::merge(LinkedList<T>& _other)
{
	CLinkedList<T>* other = _other.ref._ptr;
	if (!other) {
		return;
	}
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		obj->merge(other);
	} else {
		obj = new CLinkedList<T>;
		if (obj.isNotNull()) {
			obj->init(other);
			other->init();
			ref = obj;
		}
	}
}

template <class T>
Array<T> Atomic< LinkedList<T> >::toArray() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->toArray();
	}
	return sl_null;
}

template <class T>
List<T> Atomic< LinkedList<T> >::toList() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->toList();
	}
	return sl_null;
}

template <class T>
LinkedList<T> Atomic< LinkedList<T> >::duplicate() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->duplicate();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE LinkPosition<T> Atomic< LinkedList<T> >::begin() const
{
	Ref< CLinkedList<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->begin();
	}
	return sl_null;
}

template <class T>
SLIB_INLINE LinkPosition<T> Atomic< LinkedList<T> >::end() const
{
	return LinkPosition<T>();
}


template <class T>
SLIB_INLINE LinkPosition<T>::LinkPosition() : link(sl_null) {}

template <class T>
SLIB_INLINE LinkPosition<T>::LinkPosition(Link<T>* _link) : link(_link) {}

template <class T>
SLIB_INLINE LinkPosition<T>::LinkPosition(const LinkPosition<T>& other) : link(other.link) {}

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

SLIB_NAMESPACE_END

#endif
