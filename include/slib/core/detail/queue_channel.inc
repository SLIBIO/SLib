/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

namespace slib
{

	template <class T>
	QueueChannelArray<T>::QueueChannelArray() noexcept
	{
		m_channelCurrent = 0;
	}
	
	template <class T>
	sl_size QueueChannelArray<T>::getChannelsCount() noexcept
	{
		return m_arr.getCount();
	}
	
	template <class T>
	sl_bool QueueChannelArray<T>::setChannelsCount(sl_size n) noexcept
	{
		Array<Channel> arr = Array<Channel>::create(n);
		if (arr.isNotNull()) {
			arr.copy(m_arr);
			m_arr = arr;
			return sl_true;
		}
		return sl_false;
	}
	
	template <class T>
	sl_size QueueChannelArray<T>::getAllItemsCount() const noexcept
	{
		sl_size count = 0;
		ArrayData<Channel> info;
		m_arr.getData(info);
		for (sl_size i = 0; i < info.count; i++) {
			count += info[i].queue.getCount();
		}
		return count;
	}
	
	template <class T>
	sl_size QueueChannelArray<T>::removeAll() noexcept
	{
		sl_size count = 0;
		ArrayData<Channel> info;
		m_arr.getData(info);
		for (sl_size i = 0; i < info.count; i++) {
			count += info[i].queue.removeAll();
		}
		return count;
	}
	
	template <class T>
	sl_bool QueueChannelArray<T>::pushBack(sl_size channelNo, const T& value, sl_size countLimit) noexcept
	{
		LinkedList<T> queue(_activateChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue.pushBack(value, countLimit);
		}
		return sl_null;
	}
	
	template <class T>
	sl_bool QueueChannelArray<T>::popBack(sl_size channelNo, T* _out) noexcept
	{
		LinkedList<T> queue(_getChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue.popBack(_out);
		}
		return sl_false;
	}
	
	template <class T>
	sl_bool QueueChannelArray<T>::pushFront(sl_size channelNo, const T& value, sl_size countLimit) noexcept
	{
		LinkedList<T> queue(_activateChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue.pushFront(value, countLimit);
		}
		return sl_null;
	}
	
	template <class T>
	sl_bool QueueChannelArray<T>::popFront(sl_size channelNo, T* _out) noexcept
	{
		LinkedList<T> queue(_getChannelQueue(channelNo));
		if (queue.isNotNull()) {
			return queue.popFront(_out);
		}
		return sl_false;
	}

	template <class T>
	sl_bool QueueChannelArray<T>::popBack(T* _out) noexcept
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
			LinkedList<T> queue(info[no].queue);
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

	template <class T>
	sl_bool QueueChannelArray<T>::popFront(T* _out) noexcept
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
			LinkedList<T> queue(info[no].queue);
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

	template <class T>
	LinkedList<T> QueueChannelArray<T>::_getChannelQueue(sl_size no) noexcept
	{
		ArrayData<Channel> info;
		m_arr.getData(info);
		if (no < info.count) {
			return info[no].queue;
		}
		return sl_null;
	}

	template <class T>
	LinkedList<T> QueueChannelArray<T>::_activateChannelQueue(sl_size no) noexcept
	{
		ArrayData<Channel> info;
		m_arr.getData(info);
		if (no < info.count) {
			Channel& channel = info[no];
			MutexLocker lock(&(channel.locker));
			if (channel.queue.isNull()) {
				channel.queue = LinkedList<T>::create();
			}
			return channel.queue;
		}
		return sl_null;
	}

}
