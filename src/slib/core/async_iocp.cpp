#include "async_config.h"

#if defined(ASYNC_USE_IOCP)

#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/platform_windows.h"

SLIB_NAMESPACE_BEGIN
struct _AsyncLoopHandle
{
	HANDLE hCompletionPort;
	OVERLAPPED overlappedWake;
};

void* AsyncLoop::__createHandle()
{
	HANDLE hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (hCompletionPort) {
		_AsyncLoopHandle* handle = new _AsyncLoopHandle;
		if (handle) {
			handle->hCompletionPort = hCompletionPort;
			return handle;
		}
		::CloseHandle(hCompletionPort);
	}
	return sl_null;
}

void AsyncLoop::__closeHandle(void* _handle)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)_handle;
	::CloseHandle(handle->hCompletionPort);
	delete handle;
}


BOOL WINAPI _GetQueuedCompletionStatusEx_Impl(
	HANDLE CompletionPort,
	LPOVERLAPPED_ENTRY lpCompletionPortEntries,
	ULONG ulCount,
	PULONG ulNumEntriesRemoved,
	DWORD dwMilliseconds,
	BOOL fAlertable
)
{
	::GetQueuedCompletionStatus(CompletionPort
		, &(lpCompletionPortEntries[0].dwNumberOfBytesTransferred)
		, &(lpCompletionPortEntries[0].lpCompletionKey)
		, &(lpCompletionPortEntries[0].lpOverlapped), dwMilliseconds);
	if (lpCompletionPortEntries[0].lpCompletionKey) {
		*ulNumEntriesRemoved = 1;
	} else {
		*ulNumEntriesRemoved = 0;
	}
	return TRUE;
}


void AsyncLoop::__runLoop()
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;

	WINAPI_GetQueuedCompletionStatusEx fGetQueuedCompletionStatusEx = Windows::getAPI_GetQueuedCompletionStatusEx();
	if (!fGetQueuedCompletionStatusEx) {
		fGetQueuedCompletionStatusEx = _GetQueuedCompletionStatusEx_Impl;
	}

	OVERLAPPED_ENTRY entries[ASYNC_MAX_WAIT_EVENT];

	while (m_flagRunning) {

		_stepBegin();

		DWORD nCount = 0;
		
		if (!fGetQueuedCompletionStatusEx(handle->hCompletionPort, entries, ASYNC_MAX_WAIT_EVENT, &nCount, 0, FALSE)) {
			nCount = 0;
		}
		if (nCount == 0) {
			m_queueInstancesClosed.removeAll();
			sl_int32 _t = _getTimeout();
			DWORD timeout;
			if (_t >= 0) {
				timeout = _t;
			} else {
				timeout = INFINITE;
			}
			if (!fGetQueuedCompletionStatusEx(handle->hCompletionPort, entries, ASYNC_MAX_WAIT_EVENT, &nCount, timeout, FALSE)) {
				nCount = 0;
			}
		}

		for (DWORD i = 0; m_flagRunning && i < nCount; i++) {
			OVERLAPPED_ENTRY& entry = entries[i];
			AsyncInstance* instance = (AsyncInstance*)(entry.lpCompletionKey);
			if (instance && !(instance->isClosing())) {
				AsyncInstance::EventDesc desc;
				desc.pOverlapped = entry.lpOverlapped;
				instance->onEvent(&desc);
			}
		}

		if (m_flagRunning) {
			_stepEnd();
		}
	}

}

void AsyncLoop::__wake()
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;
	Base::resetMemory(&(handle->overlappedWake), 0, sizeof(OVERLAPPED));
	::PostQueuedCompletionStatus(handle->hCompletionPort, 0, 0, &(handle->overlappedWake));
}

sl_bool AsyncLoop::__attachInstance(AsyncInstance* instance)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;
	HANDLE hObject = (HANDLE)(instance->getHandle());
	HANDLE hPort = ::CreateIoCompletionPort(hObject, handle->hCompletionPort, (ULONG_PTR)instance, 0);
	if (hPort) {
		return sl_true;
	}
	return sl_false;
}

void AsyncLoop::__detachInstance(AsyncInstance* instance)
{
	HANDLE hObject = (HANDLE)(instance->getHandle());
	::CancelIo(hObject);
}
SLIB_NAMESPACE_END

#endif
