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

#ifndef CHECKHEADER_SLIB_CORE_THREAD_POOL
#define CHECKHEADER_SLIB_CORE_THREAD_POOL

#include "definition.h"

#include "queue.h"
#include "thread.h"
#include "dispatch.h"

namespace slib
{
	
	class SLIB_EXPORT ThreadPool : public Dispatcher
	{
		SLIB_DECLARE_OBJECT

	private:
		ThreadPool();

		~ThreadPool();

	public:
		static Ref<ThreadPool> create(sl_uint32 minThreads = 0, sl_uint32 maxThreads = 30);
	
	public:
		void release();

		sl_bool isRunning();

		sl_uint32 getThreadsCount();
	
		sl_bool addTask(const Function<void()>& task);

		sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms = 0) override;
	
	public:
		SLIB_PROPERTY(sl_uint32, MinimumThreadsCount)
		SLIB_PROPERTY(sl_uint32, MaximumThreadsCount)
		SLIB_PROPERTY(sl_uint32, ThreadStackSize)
	
	protected:
		void onRunWorker();
	
	protected:
		CList< Ref<Thread> > m_threadWorkers;
		LinkedQueue< Ref<Thread> > m_threadSleeping;
		LinkedQueue< Function<void()> > m_tasks;

		sl_bool m_flagRunning;

	};

}

#endif
