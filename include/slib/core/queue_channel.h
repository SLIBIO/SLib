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

#ifndef CHECKHEADER_SLIB_CORE_QUEUE_CHANNEL
#define CHECKHEADER_SLIB_CORE_QUEUE_CHANNEL

#include "definition.h"

#include "queue.h"
#include "array.h"

namespace slib
{

	template <class T>
	class SLIB_EXPORT QueueChannelArray : public Object
	{
	public:
		struct Channel
		{
			AtomicLinkedList<T> queue;
			Mutex locker;
		};

	protected:
		AtomicArray<Channel> m_arr;
		sl_size m_channelCurrent;

	public:
		QueueChannelArray() noexcept;
	
		sl_size getChannelsCount() noexcept;
	
		sl_bool setChannelsCount(sl_size n) noexcept;

		sl_size getAllItemsCount() const noexcept;

		sl_size removeAll() noexcept;

		template <class... ARGS>
		sl_bool pushBack(sl_size channelNo, ARGS&&... args) noexcept;

		sl_bool popBack(sl_size channelNo, T* _out = sl_null) noexcept;

		template <class... ARGS>
		sl_bool pushFront(sl_size channelNo, ARGS&&... args) noexcept;

		sl_bool popFront(sl_size channelNo, T* _out = sl_null) noexcept;

		sl_bool popBack(T* _out = sl_null) noexcept;

		sl_bool popFront(T* _out = sl_null) noexcept;

	protected:
		LinkedList<T> _getChannelQueue(sl_size no) noexcept;

		LinkedList<T> _activateChannelQueue(sl_size no) noexcept;

	};

}

#include "detail/queue_channel.inc"

#endif
