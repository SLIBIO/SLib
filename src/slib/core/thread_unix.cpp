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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_UNIX) && !defined(SLIB_PLATFORM_IS_APPLE)

#include <pthread.h>
#include <time.h>

#include "slib/core/thread.h"

namespace slib
{

	namespace priv
	{
		namespace thread
		{
			SLIB_THREAD Thread* g_currentThread = sl_null;
			SLIB_THREAD sl_uint64 g_uniqueId = 0;
		}
	}

	Thread* Thread::_nativeGetCurrentThread()
	{
		return priv::thread::g_currentThread;
	}

	void Thread::_nativeSetCurrentThread(Thread* thread)
	{
		priv::thread::g_currentThread = thread;
	}

	sl_uint64 Thread::_nativeGetCurrentThreadUniqueId()
	{
		return priv::thread::g_uniqueId;
	}

	void Thread::_nativeSetCurrentThreadUniqueId(sl_uint64 n)
	{
		priv::thread::g_uniqueId = n;
	}

	namespace priv
	{
		namespace thread
		{
			static void* ThreadProc(void* lpParam)
			{
				Thread* pThread = (Thread*)lpParam;
				pThread->_run();
				pThread->decreaseReference();
				pthread_exit(0);
				return 0;
			}
		}
	}

	void Thread::_nativeStart(sl_uint32 stackSize)
	{
		pthread_attr_t attr;
		pthread_t threadId;
		int result = pthread_attr_init(&attr);
		if (result) {
			return;
		}
		result = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		result |= pthread_attr_setstacksize(&attr, stackSize);
		if (result) {
			return;
		}
		this->increaseReference();
		result = pthread_create(&threadId, &attr, &(priv::thread::ThreadProc), this);
		if (result == 0) {
			m_handle = (void*)(threadId);
		} else {
			this->decreaseReference();
		}
		pthread_attr_destroy(&attr);
	}

	void Thread::_nativeClose()
	{
	}

	namespace priv
	{
		namespace thread
		{
#define PRIV_UNIX_SCHED_POLICY SCHED_FIFO
			static int getUnixPriority(ThreadPriority priority)
			{
				int min = sched_get_priority_min(PRIV_UNIX_SCHED_POLICY);
				int max = sched_get_priority_max(PRIV_UNIX_SCHED_POLICY);
				if (min < 0 || max < 0) {
					return -1;
				}
				switch (priority) {
				case ThreadPriority::Lowest:
					return min;
				case ThreadPriority::BelowNormal:
					return (min * 3 + max) / 4;
				case ThreadPriority::Normal:
					return (min + max) / 2;
				case ThreadPriority::AboveNormal:
					return (min + max * 3) / 4;
				case ThreadPriority::Highest:
					return max;
				}
				return -1;
			}
		}
	}

	void Thread::_nativeSetPriority()
	{
		pthread_t thread = (pthread_t)m_handle;
		if (thread) {
			int p = priv::thread::getUnixPriority(m_priority);
			if (p >= 0) {
				sched_param param;
				param.sched_priority = p;
				pthread_setschedparam(thread, PRIV_UNIX_SCHED_POLICY, &param);
			}
		}
	}

}

#endif
