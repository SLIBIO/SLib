/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

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

}
