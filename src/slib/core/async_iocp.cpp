/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "async_config.h"

#if defined(ASYNC_USE_IOCP)

#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/platform_windows.h"

namespace slib
{

	struct _AsyncIoLoopHandle
	{
		HANDLE hCompletionPort;
		OVERLAPPED overlappedWake;
	};

	void* AsyncIoLoop::__createHandle()
	{
		HANDLE hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
		if (hCompletionPort) {
			_AsyncIoLoopHandle* handle = new _AsyncIoLoopHandle;
			if (handle) {
				handle->hCompletionPort = hCompletionPort;
				return handle;
			}
			::CloseHandle(hCompletionPort);
		}
		return sl_null;
	}

	void AsyncIoLoop::__closeHandle(void* _handle)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)_handle;
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


	void AsyncIoLoop::__runLoop()
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;

		WINAPI_GetQueuedCompletionStatusEx fGetQueuedCompletionStatusEx = Windows::getAPI_GetQueuedCompletionStatusEx();
		if (!fGetQueuedCompletionStatusEx) {
			fGetQueuedCompletionStatusEx = _GetQueuedCompletionStatusEx_Impl;
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

	void AsyncIoLoop::__wake()
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;
		Base::resetMemory(&(handle->overlappedWake), 0, sizeof(OVERLAPPED));
		::PostQueuedCompletionStatus(handle->hCompletionPort, 0, 0, &(handle->overlappedWake));
	}

	sl_bool AsyncIoLoop::__attachInstance(AsyncIoInstance* instance, AsyncIoMode mode)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;
		HANDLE hObject = (HANDLE)(instance->getHandle());
		HANDLE hPort = ::CreateIoCompletionPort(hObject, handle->hCompletionPort, (ULONG_PTR)instance, 0);
		if (hPort) {
			instance->setMode(mode);
			return sl_true;
		}
		return sl_false;
	}

	void AsyncIoLoop::__detachInstance(AsyncIoInstance* instance)
	{
		HANDLE hObject = (HANDLE)(instance->getHandle());
		::CancelIo(hObject);
	}

}

#endif
