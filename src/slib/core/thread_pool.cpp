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

#include "slib/core/thread_pool.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(ThreadPool, Dispatcher)

	ThreadPool::ThreadPool()
	{
		setThreadStackSize(SLIB_THREAD_DEFAULT_STACK_SIZE);
		m_flagRunning = sl_true;
	}

	ThreadPool::~ThreadPool()
	{
		release();
	}

	Ref<ThreadPool> ThreadPool::create(sl_uint32 minThreads, sl_uint32 maxThreads)
	{
		Ref<ThreadPool> ret = new ThreadPool();
		if (ret.isNotNull()) {
			ret->setMinimumThreadsCount(minThreads);
			ret->setMaximumThreadsCount(maxThreads);
		}
		return ret;
	}

	void ThreadPool::release()
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		
		ListElements< Ref<Thread> > threads(m_threadWorkers);
		sl_size i;
		for (i = 0; i < threads.count; i++) {
			threads[i]->finish();
		}
		for (i = 0; i < threads.count; i++) {
			threads[i]->finishAndWait();
		}
	}

	sl_bool ThreadPool::isRunning()
	{
		return m_flagRunning;
	}

	sl_uint32 ThreadPool::getThreadsCount()
	{
		return (sl_uint32)(m_threadWorkers.getCount());
	}

	sl_bool ThreadPool::addTask(const Function<void()>& task)
	{
		if (task.isNull()) {
			return sl_false;
		}
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return sl_false;
		}
		// add task
		if (!(m_tasks.push(task))) {
			return sl_false;
		}

		// wake a sleeping worker
		{
			Ref<Thread> thread;
			if (m_threadSleeping.pop_NoLock(&thread)) {
				thread->wakeSelfEvent();
				return sl_true;
			}
		}

		// increase workers
		{
			sl_size nThreads = m_threadWorkers.getCount();
			if (nThreads == 0 || (nThreads < getMaximumThreadsCount())) {
				Ref<Thread> worker = Thread::start(SLIB_FUNCTION_MEMBER(ThreadPool, onRunWorker, this), getThreadStackSize());
				if (worker.isNotNull()) {
					m_threadWorkers.add_NoLock(worker);
				}
			}
		}
		return sl_true;
	}

	sl_bool ThreadPool::dispatch(const Function<void()>& callback, sl_uint64 delay_ms)
	{
		return addTask(callback);
	}

	void ThreadPool::onRunWorker()
	{
		Ref<Thread> thread = Thread::getCurrent();
		if (thread.isNull()) {
			return;
		}
		while (m_flagRunning && thread->isNotStopping()) {
			Function<void()> task;
			if (m_tasks.pop(&task)) {
				task();
			} else {
				ObjectLocker lock(this);
				sl_size nThreads = m_threadWorkers.getCount();
				if (nThreads > getMinimumThreadsCount()) {
					m_threadWorkers.remove_NoLock(thread);
					return;
				} else {
					m_threadSleeping.push_NoLock(thread);
					lock.unlock();
					thread->wait();
				}
			}
		}
	}

}
