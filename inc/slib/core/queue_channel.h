#ifndef CHECKHEADER_SLIB_CORE_QUEUE_CHANNEL
#define CHECKHEADER_SLIB_CORE_QUEUE_CHANNEL

#include "definition.h"

#include "queue.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN

template <class ITEM>
class SLIB_EXPORT QueueChannelArray : public Object
{
public:
	struct Channel
	{
		AtomicLinkedList<ITEM> queue;
		Mutex locker;		
	};
	
protected:
	AtomicArray<Channel> m_arr;
	sl_size m_channelCurrent;
	
public:
	QueueChannelArray();

	sl_size getChannelsCount();

	sl_bool setChannelsCount(sl_size n);
	
	sl_size getAllItemsCount() const;
	
	void removeAll();
	
	Link<ITEM>* pushBack(sl_size channelNo, const ITEM& value, sl_size countLimit = 0);
	
	sl_bool popBack(sl_size channelNo, ITEM* _out = sl_null);

	Link<ITEM>* pushFront(sl_size channelNo, const ITEM& value, sl_size countLimit = 0);
	
	sl_bool popFront(sl_size channelNo, ITEM* _out = sl_null);
	
	sl_bool popBack(ITEM* _out = sl_null);
	
	sl_bool popFront(ITEM* _out = sl_null);
	
protected:
	LinkedList<ITEM> _getChannelQueue(sl_size no);
	
	LinkedList<ITEM> _activateChannelQueue(sl_size no);
	
};

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class ITEM>
QueueChannelArray<ITEM>::QueueChannelArray()
{
	m_channelCurrent = 0;
}

template <class ITEM>
sl_size QueueChannelArray<ITEM>::getChannelsCount()
{
	return m_arr.getCount();
}

template <class ITEM>
sl_bool QueueChannelArray<ITEM>::setChannelsCount(sl_size n)
{
	Array<Channel> arr = Array<Channel>::create(n);
	if (arr.isNotNull()) {
		arr.copy(m_arr);
		m_arr = arr;
		return sl_true;
	}
	return sl_false;
}

template <class ITEM>
sl_size QueueChannelArray<ITEM>::getAllItemsCount() const
{
	sl_size count = 0;
	ArrayData<Channel> info;
	m_arr.getData(info);
	for (sl_size i = 0; i < info.count; i++) {
		count += info[i].queue.getCount();
	}
	return count;
}

template <class ITEM>
void QueueChannelArray<ITEM>::removeAll()
{
	ArrayData<Channel> info;
	m_arr.getData(info);
	for (sl_size i = 0; i < info.count; i++) {
		info[i].queue.removeAll();
	}
}

template <class ITEM>
Link<ITEM>* QueueChannelArray<ITEM>::pushBack(sl_size channelNo, const ITEM& value, sl_size countLimit)
{
	LinkedList<ITEM> queue(_activateChannelQueue(channelNo));
	if (queue.isNotNull()) {
		return queue.pushBack(value, countLimit);
	}
	return sl_null;
}

template <class ITEM>
sl_bool QueueChannelArray<ITEM>::popBack(sl_size channelNo, ITEM* _out)
{
	LinkedList<ITEM> queue(_getChannelQueue(channelNo));
	if (queue.isNotNull()) {
		return queue.popBack(_out);
	}
	return sl_false;
}

template <class ITEM>
Link<ITEM>* QueueChannelArray<ITEM>::pushFront(sl_size channelNo, const ITEM& value, sl_size countLimit)
{
	LinkedList<ITEM> queue(_activateChannelQueue(channelNo));
	if (queue.isNotNull()) {
		return queue.pushFront(value, countLimit);
	}
	return sl_null;
}

template <class ITEM>
sl_bool QueueChannelArray<ITEM>::popFront(sl_size channelNo, ITEM* _out)
{
	LinkedList<ITEM> queue(_getChannelQueue(channelNo));
	if (queue.isNotNull()) {
		return queue.popFront(_out);
	}
	return sl_false;
}

template <class ITEM>
sl_bool QueueChannelArray<ITEM>::popBack(ITEM* _out)
{
	ArrayData<Channel> info;
	m_arr.getData(info);
	if (info.count == 0) {
		return sl_false;
	}
	sl_size no = m_channelCurrent;
	for (sl_size i = 0; i < info.count; i++) {
		no++;
		if (no >= info.count) {
			no = 0;
		}
		LinkedList<ITEM> queue(info[no].queue);
		if (queue.isNotNull()) {
			if (queue.popBack(_out)) {
				m_channelCurrent = no;
				return sl_true;
			}
		}
	}
	m_channelCurrent = no;
	return sl_false;
}

template <class ITEM>
sl_bool QueueChannelArray<ITEM>::popFront(ITEM* _out)
{
	ArrayData<Channel> info;
	m_arr.getData(info);
	if (info.count == 0) {
		return sl_false;
	}
	sl_size no = m_channelCurrent;
	for (sl_size i = 0; i < info.count; i++) {
		no++;
		if (no >= info.count) {
			no = 0;
		}
		LinkedList<ITEM> queue(info[no].queue);
		if (queue.isNotNull()) {
			if (queue.popFront(_out)) {
				m_channelCurrent = no;
				return sl_true;
			}
		}
	}
	m_channelCurrent = no;
	return sl_false;
}

template <class ITEM>
LinkedList<ITEM> QueueChannelArray<ITEM>::_getChannelQueue(sl_size no)
{
	ArrayData<Channel> info;
	m_arr.getData(info);
	if (no < info.count) {
		return info[no].queue;
	}
	return sl_null;
}

template <class ITEM>
LinkedList<ITEM> QueueChannelArray<ITEM>::_activateChannelQueue(sl_size no)
{
	ArrayData<Channel> info;
	m_arr.getData(info);
	if (no < info.count) {
		Channel& channel = info[no];
		MutexLocker lock(&(channel.locker));
		if (channel.queue.isNull()) {
			channel.queue = LinkedList<ITEM>::create();
		}
		return channel.queue;
	}
	return sl_null;
}

SLIB_NAMESPACE_END

#endif
