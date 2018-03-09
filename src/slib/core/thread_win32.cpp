/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <windows.h>

#include "slib/core/thread.h"

namespace slib
{

	SLIB_THREAD Thread* _gt_threadCurrent = sl_null;
	Thread* Thread::_nativeGetCurrentThread()
	{
		return _gt_threadCurrent;
	}

	void Thread::_nativeSetCurrentThread(Thread* thread)
	{
		_gt_threadCurrent = thread;
	}

	SLIB_THREAD sl_uint64 _gt_threadUniqueId = 0;
	sl_uint64 Thread::_nativeGetCurrentThreadUniqueId()
	{
		return _gt_threadUniqueId;
	}

	void Thread::_nativeSetCurrentThreadUniqueId(sl_uint64 n)
	{
		_gt_threadUniqueId = n;
	}

	static DWORD CALLBACK _priv_ThreadProc(LPVOID lpParam)
	{
		Thread* pThread = (Thread*)lpParam;
		pThread->_run();
		pThread->decreaseReference();
		return 0;
	}

	void Thread::_nativeStart(sl_uint32 stackSize)
	{
		DWORD threadID = 0;
		this->increaseReference();
		m_handle = (void*)(CreateThread(NULL, stackSize, _priv_ThreadProc, (LPVOID)this, 0, &threadID));
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

}

#endif
