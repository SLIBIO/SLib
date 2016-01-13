#ifndef CHECKHEADER_SLIB_CORE_QUEUE
#define CHECKHEADER_SLIB_CORE_QUEUE

#include "definition.h"

#include "object.h"
#include "list.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN

template <class TYPE>
struct SLIB_EXPORT Link
{
	TYPE value;
	Link<TYPE>* before;
	Link<TYPE>* next;
};

template <class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT LinkedList : public Object
{
	SLIB_DECLARE_OBJECT(LinkedList, Object)
public:
	SLIB_INLINE LinkedList()
	{
		m_begin = sl_null;
		m_end = sl_null;
		m_count = 0;
	}

	~LinkedList()
	{
		removeAll();
	}

public:
	SLIB_INLINE void init(Link<TYPE>* begin, Link<TYPE>* end, sl_size count)
	{
		m_begin = begin;
		m_end = end;
		m_count = count;
	}

	SLIB_INLINE Link<TYPE>* getBegin() const
	{
		return m_begin;
	}

	SLIB_INLINE Link<TYPE>* getEnd() const
	{
		return m_end;
	}

	SLIB_INLINE sl_size getCount() const
	{
		return m_count;
	}
	
	SLIB_INLINE sl_size count() const
	{
		return m_count;
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		return m_begin == sl_null;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return m_begin != sl_null;
	}
	
public:
	static void freeLink(Link<TYPE>* link)
	{
		while (link) {
			Link<TYPE>* next = link->next;
			_freeItem(link);
			link = next;
		}
	}

	void removeAll_NoLock()
	{
		Link<TYPE>* now = m_begin;
		_init();
		freeLink(now);
	}

	void removeAll()
	{
		Link<TYPE>* now;
		{
			ObjectLocker lock(this);
			now = m_begin;
			_init();
		}
		freeLink(now);
	}
	
	
	SLIB_INLINE sl_bool getFirstElement_NoLock(TYPE* _out) const
	{
		if (m_begin) {
			if (_out) {
				*_out = m_begin->value;
			}
			return sl_true;
		}
		return sl_false;
	}

	sl_bool getFirstElement(TYPE* _out) const
	{
		ObjectLocker lock(this);
		return getFirstElement_NoLock(_out);
	}
	

	SLIB_INLINE sl_bool getLastElement_NoLock(TYPE* _out) const
	{
		if (m_end) {
			if (_out) {
				*_out = m_end->value;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool getLastElement(TYPE* _out) const
	{
		ObjectLocker lock(this);
		return getLastElement_NoLock(_out);
	}
	
	
	Link<TYPE>* pushBack_NoLock(const TYPE& value, sl_size countLimit = 0)
	{
		Link<TYPE>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		Link<TYPE>* old = _pushBackItem(item, countLimit);
		if (old) {
			_freeItem(old);
		}
		return item;
	}

	Link<TYPE>* pushBack(const TYPE& value, sl_size countLimit = 0)
	{
		Link<TYPE>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		Link<TYPE>* old;
		{
			ObjectLocker lock(this);
			old = _pushBackItem(item, countLimit);
		}
		if (old) {
			_freeItem(old);
		}
		return item;
	}
	
	
	sl_bool popBack_NoLock(TYPE* _out = sl_null)
	{
		Link<TYPE>* old = _popBackItem();
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

	sl_bool popBack(TYPE* _out = sl_null)
	{
		Link<TYPE>* old;
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
	
	
	Link<TYPE>* pushFront_NoLock(const TYPE& value, sl_size countLimit = 0)
	{
		Link<TYPE>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		Link<TYPE>* old = _pushFrontItem(item, countLimit);
		if (old) {
			_freeItem(old);
		}
		return item;
	}

	Link<TYPE>* pushFront(const TYPE& value, sl_size countLimit = 0)
	{
		Link<TYPE>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		Link<TYPE>* old;
		{
			ObjectLocker lock(this);
			old = _pushFrontItem(item, countLimit);
		}
		if (old) {
			_freeItem(old);
		}
		return item;
	}
	
	
	sl_bool popFront_NoLock(TYPE* _out = sl_null)
	{
		Link<TYPE>* old = _popFrontItem();
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

	sl_bool popFront(TYPE* _out = sl_null)
	{
		Link<TYPE>* old;
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
	
	
	void removeItem_NoLock(Link<TYPE>* item)
	{
		if (item) {
			_removeItem(item);
			_freeItem(item);
		} else {
			SLIB_ABORT("Trying to free null item");
		}
	}

	void removeItem(Link<TYPE>* item)
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


	Link<TYPE>* insertBefore_NoLock(Link<TYPE>* itemWhere, const TYPE& value)
	{
		if (itemWhere) {
			Link<TYPE>* itemNew = _createItem(value);
			if (itemNew) {
				_insertBefore(itemWhere, itemNew);
			}
			return itemNew;
		} else {
			return pushFront_NoLock(value);
		}
	}

	Link<TYPE>* insertBefore(Link<TYPE>* itemWhere, const TYPE& value)
	{
		if (itemWhere) {
			Link<TYPE>* itemNew = _createItem(value);
			if (itemNew) {
				ObjectLocker lock(this);
				_insertBefore(itemWhere, itemNew);
			}
			return itemNew;
		} else {
			return pushFront(value);
		}
	}
	

	Link<TYPE>* insertAfter_NoLock(Link<TYPE>* itemWhere, const TYPE& value)
	{
		if (itemWhere) {
			Link<TYPE>* itemNew = _createItem(value);
			if (itemNew) {
				_insertAfter(itemWhere, itemNew);
			}
			return itemNew;
		} else {
			return pushBack_NoLock(value);
		}
	}
	
	Link<TYPE>* insertAfter(Link<TYPE>* itemWhere, const TYPE& value)
	{
		if (itemWhere) {
			Link<TYPE>* itemNew = _createItem(value);
			if (itemNew) {
				ObjectLocker lock(this);
				_insertAfter(itemWhere, itemNew);
			}
			return itemNew;
		} else {
			return pushBack_NoLock(value);
		}
	}


	template <class _COMPARE>
	void merge(LinkedList<TYPE, _COMPARE>* other)
	{
		if ((void*)this == (void*)other) {
			return;
		}
		ObjectLocker lock(this, other);
		Link<TYPE>* _begin = other->getBegin();
		Link<TYPE>* _end = other->getEnd();
		if (_begin) {
			sl_size countNew = m_count + other->getCount();
			if (m_end) {
				m_end->next = _begin;
				_begin->before = m_end;
			} else {
				m_begin = _begin;
			}
			m_end = _end;
			((LinkedList<TYPE, COMPARE>*)((void*)other))->_init();
			m_count = countNew;
		}
	}
	
	
	Array<TYPE> toArray_NoLock() const
	{
		Array<TYPE> ret;
		if (m_count) {
			ret = Array<TYPE>::create(m_count);
			if (ret.isNotNull()) {
				sl_size index = 0;
				Link<TYPE>* now = m_begin;
				while (now) {
					ret[index] = now->value;
					now = now->next;
					index++;
				}
			}
		}
		return ret;
	}
	
	Array<TYPE> toArray() const
	{
		ObjectLocker lock(this);
		return toArray_NoLock();
	}

	List<TYPE> toList_NoLock() const
	{
		List<TYPE> ret;
		if (m_count) {
			ret = List<TYPE>::create(m_count);
			if (ret.isNotNull()) {
				ListItems<TYPE> list(ret);
				sl_size index = 0;
				Link<TYPE>* now = m_begin;
				while (now) {
					list[index] = now->value;
					now = now->next;
					index++;
				}
			}
		}
		return ret;
	}
	
	List<TYPE> toList() const
	{
		ObjectLocker lock(this);
		return toList_NoLock();
	}


	Link<TYPE>* findValue_NoLock(const TYPE& value) const
	{
		Link<TYPE>* now = m_begin;
		while (now) {
			if (COMPARE::equals(value, now->value)) {
				return now;
			}
			now = now->next;
		}
		return sl_null;
	}
	
	Link<TYPE>* findValue(const TYPE& value) const
	{
		ObjectLocker lock(this);
		return findValue_NoLock(value);
	}


	sl_bool removeValue_NoLock(const TYPE& value, sl_bool flagAllValues = sl_false)
	{
		Link<TYPE>* now = m_begin;
		sl_bool bRet = sl_false;
		while (now) {
			Link<TYPE>* next = now->next;
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
	
	sl_bool removeValue(const TYPE& value, sl_bool flagAllValues = sl_false)
	{
		ObjectLocker lock(this);
		return removeValue_NoLock(value, flagAllValues);
	}
	
protected:
	SLIB_INLINE static Link<TYPE>* _createItem(const TYPE& value)
	{
		Link<TYPE>* item = (Link<TYPE>*)(Base::createMemory(sizeof(Link<TYPE>)));
		if (!item) {
			return sl_null;
		}
		item->next = sl_null;
		item->before = sl_null;
		new (&(item->value)) TYPE(value);
		return item;
	}

	SLIB_INLINE static void _freeItem(Link<TYPE>* item)
	{
		item->value.TYPE::~TYPE();
		Base::freeMemory(item);
	}
	
	
	SLIB_INLINE Link<TYPE>* _pushBackItem(Link<TYPE>* item, sl_size countLimit)
	{
		Link<TYPE>* old;
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
	
	SLIB_INLINE Link<TYPE>* _popBackItem()
	{
		Link<TYPE>* end = m_end;
		if (end) {
			m_count--;
			Link<TYPE>* before = end->before;
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
	
	SLIB_INLINE Link<TYPE>* _pushFrontItem(Link<TYPE>* item, sl_size countLimit)
	{
		Link<TYPE>* old;
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

	SLIB_INLINE Link<TYPE>* _popFrontItem()
	{
		Link<TYPE>* begin = m_begin;
		if (begin) {
			m_count--;
			Link<TYPE>* next = begin->next;
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
	
	SLIB_INLINE void _removeItem(Link<TYPE>* item)
	{
		m_count--;
		Link<TYPE>* before = item->before;
		Link<TYPE>* next = item->next;
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
	
	SLIB_INLINE void _insertBefore(Link<TYPE>* itemWhere, Link<TYPE>* itemNew)
	{
		itemNew->next = itemWhere;
		Link<TYPE>* before = itemWhere->before;
		itemNew->before = before;
		itemWhere->before = itemNew;
		if (before) {
			before->next = itemNew;
		} else {
			m_begin = itemNew;
		}
		m_count++;
	}
	
	SLIB_INLINE void _insertAfter(Link<TYPE>* itemWhere, Link<TYPE>* itemNew)
	{
		itemNew->before = itemWhere;
		Link<TYPE>* next = itemWhere->next;
		itemNew->next = next;
		itemWhere->next = itemNew;
		if (next) {
			next->before = itemNew;
		} else {
			m_end = itemNew;
		}
		m_count++;
	}

	SLIB_INLINE void _init()
	{
		m_begin = sl_null;
		m_end = sl_null;
	}
	
protected:
	Link<TYPE>* m_begin;
	Link<TYPE>* m_end;
	sl_size m_count;

};

template <class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT Queue : public LinkedList<TYPE, COMPARE>
{
public:
	SLIB_INLINE Link<TYPE>* push(const TYPE& value, sl_size countLimit = 0)
	{
		return this->pushBack(value, countLimit);
	}

	SLIB_INLINE Link<TYPE>* push_NoLock(const TYPE& value, sl_size countLimit = 0)
	{
		return this->pushBack_NoLock(value, countLimit);
	}
	
	SLIB_INLINE sl_bool pop(TYPE* _out = sl_null)
	{
		return this->popFront(_out);
	}
	
	SLIB_INLINE sl_bool pop_NoLock(TYPE* _out = sl_null)
	{
		return this->popFront_NoLock(_out);
	}

};

template <class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT Stack : public LinkedList<TYPE, COMPARE>
{
public:
	SLIB_INLINE Link<TYPE>* push(const TYPE& value, sl_size countLimit = 0)
	{
		return this->pushBack(value, countLimit);
	}
	
	SLIB_INLINE Link<TYPE>* push_NoLock(const TYPE& value, sl_size countLimit = 0)
	{
		return this->pushBack_NoLock(value, countLimit);
	}
	
	SLIB_INLINE sl_bool pop(TYPE* _out = sl_null)
	{
		return this->popBack(_out);
	}
	
	SLIB_INLINE sl_bool pop_NoLock(TYPE* _out = sl_null)
	{
		return this->popBack_NoLock(_out);
	}
};

template <class TYPE>
class SLIB_EXPORT LoopQueue : public Object
{
	SLIB_DECLARE_OBJECT(LoopQueue, Object)
public:
	LoopQueue(sl_size size = 10, sl_size latency = 0)
	{
		m_first = 0;
		m_count = 0;
		m_latency = latency;
		m_data = new TYPE[size];
		if (m_data) {
			m_size = size;
		} else {
			m_size = 0;
		}
	}

	~LoopQueue()
	{
		_release();
	}

public:
	SLIB_INLINE sl_size getQueueSize()
	{
		return m_size;
	}
	
	sl_bool setQueueSize(sl_size size)
	{
		ObjectLocker lock(this);
		_release();
		m_data = new TYPE[size];
		if (m_data) {
			m_first = 0;
			m_count = 0;
			m_size = size;
			return sl_true;
		}
		return sl_false;
	}
    
    void removeAll()
    {
		ObjectLocker lock(this);
		m_first = 0;
		m_count = 0;
    }

	TYPE* getBuffer()
	{
		return m_data;
	}
	
	sl_size getCount()
	{
		return m_count;
	}
	
	sl_size count()
	{
		return m_count;
	}
	
	void setLatency(sl_size latency)
	{
		m_latency = latency;
	}
	
	sl_size getLatency()
	{
		return m_latency;
	}
	
	sl_bool add(const TYPE& data, sl_bool flagPush = sl_true)
	{
		ObjectLocker lock(this);
		if (m_size == 0) {
			return sl_false;
		}
		if (m_count == m_size && ! flagPush) {
			return sl_false;
		}
		sl_size last = m_first + m_count;
		m_data[last % m_size] = data;
		m_count ++;
		if (m_count > m_size) {
			m_count = m_size;
			m_first = (last + 1) % m_size;
		}
		return sl_true;
	}

	sl_bool add(const TYPE* buffer, sl_size count, sl_bool flagPush = sl_true)
	{
		ObjectLocker lock(this);
		if (m_size == 0) {
			return sl_false;
		}
		if (m_count + count > m_size && !flagPush) {
			return sl_false;
		}
		sl_size last = m_first + m_count;
		if (count > m_size) {
			buffer += (count - m_size);
			last += (count - m_size);
			m_count += (count - m_size);
			count = m_size;
		}
		sl_size i = last % m_size;
		sl_size n = 0;
		while (i < m_size && n < count) {
			m_data[i] = buffer[n];
			i ++;
			n ++;
		}
		i = 0;
		while (n < count) {
			m_data[i] = buffer[n];
			i ++;
			n ++;
		}
		m_count += count;
		if (m_count > m_size) {
			m_first = (m_first + m_count) % m_size;
			m_count = m_size;
		}
		return sl_true;
	}

	
	sl_bool get(TYPE& output)
	{
		ObjectLocker lock(this);
		sl_bool ret = sl_false;
		if (m_count > m_latency) {
			output = m_data[m_first % m_size];
			m_first = (m_first + 1) % m_size;
			m_count --;
			ret = sl_true;
		}
		return ret;
	}
		
	sl_bool get(TYPE* buffer, sl_size count)
	{
		ObjectLocker lock(this);
		sl_bool ret = sl_false;
		if (count <= m_count && m_count > m_latency) {
			sl_size n = 0;
			sl_size i = m_first;
			while (i < m_size && n < count) {
				buffer[n] = m_data[i];
				i ++;
				n ++;
			}
			i = 0;
			while (n < count) {
				buffer[n] = m_data[i];
				i ++;
				n ++;
			}
			m_first = (m_first + count) % m_size;
			m_count -= count;
			ret = sl_true;
		}
		return ret;
	}

	sl_size copy(TYPE* buffer, sl_size count)
	{
		ObjectLocker lock(this);
		if (count > m_count) {
			count = m_count;
		}
		{
			sl_size n = 0;
			sl_size i = m_first;
			while (i < m_size && n < count) {
				buffer[n] = m_data[i];
				i++;
				n++;
			}
			i = 0;
			while (n < count) {
				buffer[n] = m_data[i];
				i++;
				n++;
			}
		}
		return count;
	}

protected:
	void _release() {
		if (m_data) {
			delete[] m_data;
			m_data = sl_null;
		}
	}
	
protected:
	TYPE* m_data;
	sl_size m_size;
	sl_size m_first;
	sl_size m_count;
	sl_size m_latency;

};

SLIB_NAMESPACE_END

#endif
