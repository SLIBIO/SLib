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
