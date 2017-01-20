#ifndef CHECKHEADER_SLIB_CORE_LOOP_QUEUE
#define CHECKHEADER_SLIB_CORE_LOOP_QUEUE

#include "definition.h"

#include "object.h"
#include "new_helper.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT LoopQueueBase : public Object
{
	SLIB_DECLARE_OBJECT
};

template <class T>
class SLIB_EXPORT LoopQueue : public LoopQueueBase
{
protected:
	T* m_data;
	sl_size m_size;
	sl_size m_first;
	sl_size m_count;
	sl_size m_latency;

public:
	LoopQueue(sl_size size = 10, sl_size latency = 0);
	
	~LoopQueue();

public:
	sl_size getQueueSize();
	
	sl_bool setQueueSize(sl_size size);
	
	void removeAll();
	
	T* getBuffer();
	
	sl_size getCount();
	
	void setLatency(sl_size latency);
	
	sl_size getLatency();
	
	sl_bool add(const T& data, sl_bool flagPush = sl_true);
	
	sl_bool add(const T* buffer, sl_size count, sl_bool flagPush = sl_true);
	
	sl_bool get(T& output);
	
	sl_bool get(T* buffer, sl_size count);
	
	sl_size copy(T* buffer, sl_size count);
	
};

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T>
LoopQueue<T>::LoopQueue(sl_size size, sl_size latency)
{
	m_first = 0;
	m_count = 0;
	m_latency = latency;
	m_data = NewHelper<T>::create(size);
	if (m_data) {
		m_size = size;
	} else {
		m_size = 0;
	}
}

template <class T>
LoopQueue<T>::~LoopQueue()
{
	if (m_data) {
		NewHelper<T>::free(m_data, m_size);
	}
}

template <class T>
sl_size LoopQueue<T>::getQueueSize()
{
	return m_size;
}

template <class T>
sl_bool LoopQueue<T>::setQueueSize(sl_size size)
{
	ObjectLocker lock(this);
	if (m_data) {
		NewHelper<T>::free(m_data, m_size);
		m_data = sl_null;
	}
	m_data = NewHelper<T>::create(size);
	if (m_data) {
		m_first = 0;
		m_count = 0;
		m_size = size;
		return sl_true;
	}
	return sl_false;
}

template <class T>
void LoopQueue<T>::removeAll()
{
	ObjectLocker lock(this);
	m_first = 0;
	m_count = 0;
}

template <class T>
T* LoopQueue<T>::getBuffer()
{
	return m_data;
}

template <class T>
sl_size LoopQueue<T>::getCount()
{
	return m_count;
}

template <class T>
void LoopQueue<T>::setLatency(sl_size latency)
{
	m_latency = latency;
}

template <class T>
sl_size LoopQueue<T>::getLatency()
{
	return m_latency;
}

template <class T>
sl_bool LoopQueue<T>::add(const T& data, sl_bool flagPush)
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

template <class T>
sl_bool LoopQueue<T>::add(const T* buffer, sl_size count, sl_bool flagPush)
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

template <class T>
sl_bool LoopQueue<T>::get(T& output)
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

template <class T>
sl_bool LoopQueue<T>::get(T* buffer, sl_size count)
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

template <class T>
sl_size LoopQueue<T>::copy(T* buffer, sl_size count)
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

SLIB_NAMESPACE_END

#endif
