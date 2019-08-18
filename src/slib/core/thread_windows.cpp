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

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

#include "slib/core/thread.h"

namespace slib
{

	namespace priv
	{
		namespace thread
		{

			SLIB_THREAD Thread* g_currentThread = sl_null;
			SLIB_THREAD sl_uint64 g_uniqueId = 0;

			static DWORD CALLBACK ThreadProc(LPVOID lpParam)
			{
				Thread* pThread = (Thread*)lpParam;
				pThread->_run();
				pThread->decreaseReference();
				return 0;
			}

		}
	}

	using namespace priv::thread;

	Thread* Thread::_nativeGetCurrentThread()
	{
		return g_currentThread;
	}

	void Thread::_nativeSetCurrentThread(Thread* thread)
	{
		g_currentThread = thread;
	}

	sl_uint64 Thread::_nativeGetCurrentThreadUniqueId()
	{
		return g_uniqueId;
	}

	void Thread::_nativeSetCurrentThreadUniqueId(sl_uint64 n)
	{
		g_uniqueId = n;
	}

	void Thread::_nativeStart(sl_uint32 stackSize)
	{
		DWORD threadID = 0;
		this->increaseReference();
		m_handle = (void*)(CreateThread(NULL, stackSize, ThreadProc, (LPVOID)this, 0, &threadID));
		if (!m_handle) {
			this->decreaseReference();
		}
	}

	void Thread::_nativeSetPriority()
	{
		HANDLE hThread = (HANDLE)m_handle;
		if (hThread) {
			switch (m_priority) {
			case ThreadPriority::Normal:
				SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
				break;
			case ThreadPriority::AboveNormal:
				SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
				break;
			case ThreadPriority::Highest:
				SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
				break;
			case ThreadPriority::BelowNormal:
				SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
				break;
			case ThreadPriority::Lowest:
				SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
				break;
			}
		}
	}

	void Thread::_nativeClose()
	{
		if (m_handle) {
			CloseHandle((HANDLE)m_handle);
		}
	}

	sl_uint32 Thread::getCurrentThreadId()
	{
		return (sl_uint32)(GetCurrentThreadId());
	}

}

#endif
