#ifndef CHECKHEADER_SLIB_CORE_QUEUE_CHANNEL
#define CHECKHEADER_SLIB_CORE_QUEUE_CHANNEL

#include "definition.h"
#include "queue.h"
#include "array.h"

SLIB_NAMESPACE_BEGIN
template <class ITEM>
class SLIB_EXPORT QueueChannelArray : public Object
{
protected:
	typedef Ref< Queue<ITEM> > Channel;
	Array<Channel> m_arr;
	sl_size m_channelCurrent;

public:
	QueueChannelArray()
	{
		m_channelCurrent = 0;
	}

	sl_size getChannelsCount()
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
			Channel channel = info.data[i];
			if (channel.isNotNull()) {
				count += channel->getCount();
			}
		}
		return count;
	}

	void clear()
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		for (sl_size i = 0; i < info.count; i++) {
			Channel channel = info.data[i];
			if (channel.isNotNull()) {
				channel->clear();
			}
		}
	}

	Channel getChannel(sl_size no)
	{
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		if (no < info.count) {
			Channel channel = info.data[no];
			return channel;
		} else {
			return Channel::null();
		}
	}

	Channel activateChannel(sl_size no)
	{
		ObjectLocker lock(this);
		ArrayInfo<Channel> info;
		m_arr.getInfo(info);
		if (no < info.count) {
			Channel channel = info.data[no];
			if (channel.isNull()) {
				channel = new Queue<ITEM>;
				info.data[no] = channel;
			}
			return channel;
		} else {
			return Channel::null();
		}
	}

	Link<ITEM>* pushBack(sl_size channelNo, const ITEM& value, sl_size countLimit = 0)
	{
		Channel channel = activateChannel(channelNo);
		if (channel.isNotNull()) {
			return channel->pushBack(value, countLimit);
		}
		return sl_null;
	}

	sl_bool popBack(sl_size channelNo, ITEM* out = sl_null)
	{
		Channel channel = getChannel(channelNo);
		if (channel.isNotNull()) {
			return channel->popBack(out);
		}
		return sl_false;
	}

	Link<ITEM>* pushFront(sl_size channelNo, const ITEM& value, sl_size countLimit = 0)
	{
		Channel channel = activateChannel(channelNo);
		if (channel.isNotNull()) {
			return channel->pushFront(value, countLimit);
		}
		return sl_null;
	}

	sl_bool popFront(sl_size channelNo, ITEM* out = sl_null)
	{
		Channel channel = getChannel(channelNo);
		if (channel.isNotNull()) {
			return channel->popFront(out);
		}
		return sl_false;
	}

	sl_bool popBack(ITEM* out = sl_null)
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
			Channel channel = info.data[no];
			if (channel.isNotNull()) {
				if (channel->popBack(out)) {
					m_channelCurrent = no;
					return sl_true;
				}
			}
		}
		m_channelCurrent = no;
		return sl_false;
	}

	sl_bool popFront(ITEM* out = sl_null)
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
			Channel channel = info.data[no];
			if (channel.isNotNull()) {
				if (channel->popFront(out)) {
					m_channelCurrent = no;
					return sl_true;
				}
			}
		}
		m_channelCurrent = no;
		return sl_false;
	}
};

SLIB_NAMESPACE_END

#endif
