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
protected:
	Link<TYPE>* m_begin;
	Link<TYPE>* m_end;
	sl_size m_count;
	
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

	void removeAll()
	{
		ObjectLocker lock(this);
		Link<TYPE>* now = m_begin;
		_clear();
		lock.unlock();
		while (now) {
			Link<TYPE>* next = now->next;
			_freeItem(now);
			now = next;
		}
	}

	sl_bool getFirstElement(TYPE* out) const
	{
		ObjectLocker lock(this);
		if (m_begin) {
			*out = m_begin->value;
			return sl_true;
		} else {
			return sl_false;
		}
	}

	sl_bool getLastElement(TYPE* out) const
	{
		ObjectLocker lock(this);
		if (m_end) {
			*out = m_end->value;
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	Link<TYPE>* pushBack(const TYPE& value, sl_size countLimit = 0)
	{
		Link<TYPE>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		ObjectLocker lock(this);
		Link<TYPE>* old = _pushBackItem(item, countLimit);
		lock.unlock();
		if (old) {
			_freeItem(old);
		}
		return item;
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

	sl_bool popBack(TYPE* _out = sl_null)
	{
		ObjectLocker lock(this);
		Link<TYPE>* old = _popBackItem();
		lock.unlock();
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

	Link<TYPE>* pushFront(const TYPE& value, sl_size countLimit = 0)
	{
		Link<TYPE>* item = _createItem(value);
		if (!item) {
			return sl_null;
		}
		ObjectLocker lock(this);
		Link<TYPE>* old = _pushFrontItem(item, countLimit);
		lock.unlock();
		if (old) {
			_freeItem(old);
		}
		return item;
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
	
	sl_bool popFront(TYPE* _out = sl_null)
	{
		ObjectLocker lock(this);
		Link<TYPE>* old = _popFrontItem();
		lock.unlock();
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

	void removeItem(Link<TYPE>* item)
	{
		if (item) {
			ObjectLocker lock(this);
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
			lock.unlock();
			_freeItem(item);
		} else {
			SLIB_ABORT("Trying to free null item");
		}
	}

	Link<TYPE>* insertBefore(Link<TYPE>* item, const TYPE& value)
	{
		if (item) {
			Link<TYPE>* newItem = _createItem(value);
			if (!newItem) {
				return sl_null;
			}
			ObjectLocker lock(this);
			newItem->next = item;
			Link<TYPE>* before = item->before;
			newItem->before = before;
			item->before = newItem;
			if (before) {
				before->next = newItem;
			} else {
				m_begin = newItem;
			}
			m_count++;
			return newItem;
		} else {
			return pushFront(value);
		}
	}

	Link<TYPE>* insertAfter(Link<TYPE>* item, const TYPE& value)
	{
		if (item) {
			Link<TYPE>* newItem = _createItem(value);
			if (!newItem) {
				return sl_null;
			}
			ObjectLocker lock(this);
			newItem->before = item;
			Link<TYPE>* next = item->next;
			newItem->next = next;
			item->next = newItem;
			if (next) {
				next->before = newItem;
			} else {
				m_end = newItem;
			}
			m_count++;
			return newItem;
		} else {
			return pushBack(value);
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
			other->_clear();
			m_count = countNew;
		}
	}
	
	Array<TYPE> toArray() const
	{
		Array<TYPE> ret;
		ObjectLocker lock(this);
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

	List<TYPE> toList() const
	{
		List<TYPE> ret;
		ObjectLocker lock(this);
		if (m_count) {
			ret = List<TYPE>::create(m_count);
			if (ret.isNotNull()) {
				ListLocker<TYPE> list(ret);
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

	Link<TYPE>* findValue(const TYPE& value) const
	{
		ObjectLocker lock(this);
		Link<TYPE>* now = m_begin;
		while (now) {
			if (COMPARE::equals(value, now->value)) {
				return now;
			}
			now = now->next;
		}
		return sl_null;
	}

	sl_bool removeValue(const TYPE& value, sl_bool flagAllValues = sl_false)
	{
		ObjectLocker lock(this);
		Link<TYPE>* now = m_begin;
		sl_bool bRet = sl_false;
		while (now) {
			Link<TYPE>* next = now->next;
			if (COMPARE::equals(value, now->value)) {
				removeItem(now);
				if (!flagAllValues) {
					return sl_true;
				}
				bRet = sl_true;
			}
			now = next;
		}
		return bRet;
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
	
public:
	SLIB_INLINE void _clear()
	{
		m_begin = sl_null;
		m_end = sl_null;
	}
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
private:
	TYPE* m_data;
	sl_size m_size;
	sl_size m_first;
	sl_size m_count;
	sl_size m_latency;
	
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
		removeAll();
	}

	void release() {
		ObjectLocker lock(this);
		if (m_data) {
			delete[] m_data;
			m_data = sl_null;
		}
		m_size = 0;
		m_first = 0;
		m_count = 0;
	}
	
	SLIB_INLINE sl_size getQueueSize()
	{
		return m_size;
	}
	
	sl_bool setQueueSize(sl_size size)
	{
		ObjectLocker lock(this);
		release();
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
};
SLIB_NAMESPACE_END

#endif
