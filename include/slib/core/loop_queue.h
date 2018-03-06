/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_LOOP_QUEUE
#define CHECKHEADER_SLIB_CORE_LOOP_QUEUE

#include "definition.h"

#include "object.h"
#include "new_helper.h"

namespace slib
{
	
	class SLIB_EXPORT LoopQueueBase : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		LoopQueueBase() noexcept;

		~LoopQueueBase() noexcept;

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
		LoopQueue(sl_size size = 10, sl_size latency = 0) noexcept;

		~LoopQueue() noexcept;
	
	public:
		sl_size getQueueSize() const noexcept;

		sl_bool setQueueSize(sl_size size) noexcept;

		sl_size removeAll() noexcept;

		T* getBuffer() const noexcept;

		sl_size getCount() const noexcept;

		void setLatency(sl_size latency) noexcept;

		sl_size getLatency() const noexcept;

		sl_bool push(const T& data, sl_bool flagShift = sl_true) noexcept;

		sl_bool push(const T* buffer, sl_size count, sl_bool flagShift = sl_true) noexcept;

		sl_bool pop(T& output) noexcept;

		sl_bool pop(T* buffer, sl_size count) noexcept;

		sl_size copy(T* buffer, sl_size count) const noexcept;

	};

}

#include "detail/loop_queue.inc"

#endif
