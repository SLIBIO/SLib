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
		Ref< Queue<ITEM> > queue;
		Mutex locker;		
	};

public:
	SLIB_INLINE QueueChannelArray()
	{
		m_channelCurrent = 0;
	}

	SLIB_INLINE sl_size getChannelsCount()
	{
		return m_arr.getCount();
	}

	sl_bool setChannelsCount(sl_size n)
	{
		Array<Channel> arr = Array<Channel>::create(n);
		if (arr.isNotNull()) {
			arr.copy(m_arr);
			m_arr = arr;
			return sl_true;
		}
		return sl_false;
	}

	sl_size getAllItemsCount() const
	{
		sl_size count = 0;
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		for (sl_size i = 0; i < info.count; i++) {
			Ref< Queue<ITEM> > queue(info[i].queue);
			if (queue.isNotNull()) {
				count += queue->getCount();
			}
		}
		return count;
	}

	void removeAll()
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		for (sl_size i = 0; i < info.count; i++) {
			Ref< Queue<ITEM> > queue(info[i].queue);
			if (queue.isNotNull()) {
				queue->removeAll();
			}
		}
	}

	Link<ITEM>* pushBack(sl_size channelNo, const ITEM& value, sl_size countLimit = 0)
	{
		Ref< Queue<ITEM> > queue(_activateChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue->pushBack(value, countLimit);
		}
		return sl_null;
	}

	sl_bool popBack(sl_size channelNo, ITEM* _out = sl_null)
	{
		Ref< Queue<ITEM> > queue(_getChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue->popBack(_out);
		}
		return sl_false;
	}

	Link<ITEM>* pushFront(sl_size channelNo, const ITEM& value, sl_size countLimit = 0)
	{
		Ref< Queue<ITEM> > queue(_activateChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue->pushFront(value, countLimit);
		}
		return sl_null;
	}

	sl_bool popFront(sl_size channelNo, ITEM* _out = sl_null)
	{
		Ref< Queue<ITEM> > queue(_getChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue->popFront(_out);
		}
		return sl_false;
	}

	sl_bool popBack(ITEM* _out = sl_null)
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		if (info.count == 0) {
			return sl_false;
		}
		sl_size no = m_channelCurrent;
		for (sl_size i = 0; i < info.count; i++) {
			no++;
			if (no >= info.count) {
				no = 0;
			}
			Ref< Queue<ITEM> > queue(info[no].queue);
			if (queue.isNotNull()) {
				if (queue->popBack(_out)) {
					m_channelCurrent = no;
					return sl_true;
				}
			}
		}
		m_channelCurrent = no;
		return sl_false;
	}

	sl_bool popFront(ITEM* _out = sl_null)
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		if (info.count == 0) {
			return sl_false;
		}
		sl_size no = m_channelCurrent;
		for (sl_size i = 0; i < info.count; i++) {
			no++;
			if (no >= info.count) {
				no = 0;
			}
			Ref< Queue<ITEM> > queue(info[no].queue);
			if (queue.isNotNull()) {
				if (queue->popFront(_out)) {
					m_channelCurrent = no;
					return sl_true;
				}
			}
		}
		m_channelCurrent = no;
		return sl_false;
	}

protected:
	Ref< Queue<ITEM> > _getChannelQueue(sl_size no)
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		if (no < info.count) {
			return info[no].queue;
		}
		return Ref< Queue<ITEM> >::null();
	}
	
	Ref< Queue<ITEM> > _activateChannelQueue(sl_size no)
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		if (no < info.count) {
			Channel& channel = info[no];
			MutexLocker lock(&(channel.locker));
			if (channel.queue.isNull()) {
				channel.queue = new Queue<ITEM>;
			}
			return channel.queue;
		}
		return Ref< Queue<ITEM> >::null();
	}
	
protected:
	SafeArray<Channel> m_arr;
	sl_size m_channelCurrent;

};

SLIB_NAMESPACE_END

#endif
