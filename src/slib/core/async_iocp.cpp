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

#include "async_config.h"

#if defined(ASYNC_USE_IOCP)

#include "slib/core/async.h"
#include "slib/core/platform_windows.h"

namespace slib
{

	namespace priv
	{
		namespace async_iocp
		{
			struct AsyncIoLoopHandle
			{
				HANDLE hCompletionPort;
				OVERLAPPED overlappedWake;
			};
		}
	}

	using namespace priv::async_iocp;
	
	void* AsyncIoLoop::_native_createHandle()
	{
		HANDLE hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
		if (hCompletionPort) {
			AsyncIoLoopHandle* handle = new AsyncIoLoopHandle;
			if (handle) {
				handle->hCompletionPort = hCompletionPort;
				return handle;
			}
			::CloseHandle(hCompletionPort);
		}
		return sl_null;
	}

	void AsyncIoLoop::_native_closeHandle(void* _handle)
	{
		AsyncIoLoopHandle* handle = (AsyncIoLoopHandle*)_handle;
		::CloseHandle(handle->hCompletionPort);
		delete handle;
	}


	BOOL WINAPI _priv_GetQueuedCompletionStatusEx_Impl(
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


	void AsyncIoLoop::_native_runLoop()
	{
		AsyncIoLoopHandle* handle = (AsyncIoLoopHandle*)m_handle;

		WINAPI_GetQueuedCompletionStatusEx fGetQueuedCompletionStatusEx = Windows::getAPI_GetQueuedCompletionStatusEx();
		if (!fGetQueuedCompletionStatusEx) {
			fGetQueuedCompletionStatusEx = _priv_GetQueuedCompletionStatusEx_Impl;
		}

		OVERLAPPED_ENTRY entries[ASYNC_MAX_WAIT_EVENT];

		while (m_flagRunning) {

			_stepBegin();

			DWORD nCount = 0;
			
			if (!fGetQueuedCompletionStatusEx(handle->hCompletionPort, entries, ASYNC_MAX_WAIT_EVENT, &nCount, INFINITE, FALSE)) {
				nCount = 0;
			}
			if (nCount == 0) {
				m_queueInstancesClosed.removeAll();
			}

			for (DWORD i = 0; m_flagRunning && i < nCount; i++) {
				OVERLAPPED_ENTRY& entry = entries[i];
				AsyncIoInstance* instance = (AsyncIoInstance*)(entry.lpCompletionKey);
				if (instance && !(instance->isClosing())) {
					AsyncIoInstance::EventDesc desc;
					desc.pOverlapped = entry.lpOverlapped;
					instance->onEvent(&desc);
				}
			}

			if (m_flagRunning) {
				_stepEnd();
			}
		}

	}

	void AsyncIoLoop::_native_wake()
	{
		AsyncIoLoopHandle* handle = (AsyncIoLoopHandle*)m_handle;
		Base::resetMemory(&(handle->overlappedWake), 0, sizeof(OVERLAPPED));
		::PostQueuedCompletionStatus(handle->hCompletionPort, 0, 0, &(handle->overlappedWake));
	}

	sl_bool AsyncIoLoop::_native_attachInstance(AsyncIoInstance* instance, AsyncIoMode mode)
	{
		AsyncIoLoopHandle* handle = (AsyncIoLoopHandle*)m_handle;
		HANDLE hObject = (HANDLE)(instance->getHandle());
		HANDLE hPort = ::CreateIoCompletionPort(hObject, handle->hCompletionPort, (ULONG_PTR)instance, 0);
		if (hPort) {
			instance->setMode(mode);
			return sl_true;
		}
		return sl_false;
	}

	void AsyncIoLoop::_native_detachInstance(AsyncIoInstance* instance)
	{
		HANDLE hObject = (HANDLE)(instance->getHandle());
		::CancelIo(hObject);
	}

}

#endif
