/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

		sl_bool pushBack(sl_size channelNo, const T& value, sl_size countLimit = 0) noexcept;

		sl_bool popBack(sl_size channelNo, T* _out = sl_null) noexcept;
	
		sl_bool pushFront(sl_size channelNo, const T& value, sl_size countLimit = 0) noexcept;

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
