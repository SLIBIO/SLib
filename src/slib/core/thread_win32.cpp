#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <windows.h>

#include "../../../inc/slib/core/thread.h"

SLIB_NAMESPACE_BEGIN

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

static DWORD CALLBACK _ThreadProc(LPVOID lpParam)
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
	m_handle = (void*)(CreateThread(NULL, stackSize, _ThreadProc, (LPVOID)this, 0, &threadID));
	if (!m_handle) {
		this->decreaseReference();
	}
}

void Thread::_nativeSetPriority()
{
	HANDLE hThread = (HANDLE)m_handle;
	if (hThread) {
		switch (m_priority) {
		case threadPriority_Normal:
			SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
			break;
		case threadPriority_AboveNormal:
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			break;
		case threadPriority_Highest:
			SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
			break;
		case threadPriority_BelowNormal:
			SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
			break;
		case threadPriority_Lowest:
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

SLIB_NAMESPACE_END

#endif
