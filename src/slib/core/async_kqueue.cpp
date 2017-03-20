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

#if defined(ASYNC_USE_KQUEUE)

#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/pipe.h"
#include "../../../inc/slib/core/system.h"

#include <sys/types.h>
#include <sys/event.h>
#include <unistd.h>

namespace slib
{

	struct _AsyncIoLoopHandle
	{
		int kq;
		Ref<PipeEvent> eventWake;
	};

	void* AsyncIoLoop::__createHandle()
	{
		Ref<PipeEvent> pipe = PipeEvent::create();
		if (pipe.isNull()) {
			return 0;
		}
		int kq;
		kq = ::kqueue();
		if (kq != -1) {
			_AsyncIoLoopHandle* handle = new _AsyncIoLoopHandle;
			if (handle) {
				handle->kq = kq;
				handle->eventWake = pipe;
				// register wake event
				struct kevent ke;
				EV_SET(&ke, (int)(pipe->getReadPipeHandle()), EVFILT_READ, EV_ADD | EV_CLEAR | EV_ENABLE, 0, 0, sl_null);
				if (-1 != ::kevent(kq, &ke, 1, sl_null, 0, sl_null)) {
					return handle;
				}
				delete handle;
			}
			::close(kq);
		}
		return sl_null;
	}

	void AsyncIoLoop::__closeHandle(void* _handle)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)_handle;
		::close(handle->kq);
		delete handle;
	}

	void AsyncIoLoop::__runLoop()
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;

		struct kevent waitEvents[ASYNC_MAX_WAIT_EVENT];

		while (m_flagRunning) {

			_stepBegin();

			int nEvents = ::kevent(handle->kq, sl_null, 0, waitEvents, ASYNC_MAX_WAIT_EVENT, NULL);
			if (nEvents == 0) {
				m_queueInstancesClosed.removeAll();
			}

			for (int i = 0; m_flagRunning && i < nEvents; i++) {
				struct kevent& ev = waitEvents[i];
				AsyncIoInstance* instance = (AsyncIoInstance*)(ev.udata);
				if (instance) {
					if (!(instance->isClosing())) {
						AsyncIoInstance::EventDesc desc;
						desc.flagIn = sl_false;
						desc.flagOut = sl_false;
						desc.flagError = sl_false;
						int re = ev.filter;
						if (re == EVFILT_READ) {
							desc.flagIn = sl_true;
						}
						if (re == EVFILT_WRITE) {
							desc.flagOut = sl_true;
						}
						int flags = ev.flags;
						if (flags & (EV_EOF | EV_ERROR)) {
							desc.flagError = sl_true;
						}
						instance->onEvent(&desc);
					}
				} else {
					handle->eventWake->reset();
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
		handle->eventWake->set();
	}

	sl_bool AsyncIoLoop::__attachInstance(AsyncIoInstance* instance, AsyncIoMode mode)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;
		int hObject = (int)(instance->getHandle());
		
		struct kevent ke[2];
		int ret = -1;

		switch (mode) {
			case AsyncIoMode::InOut:
				EV_SET(ke, hObject, EVFILT_READ, EV_ADD | EV_CLEAR | EV_ENABLE, 0, 0, instance);
				EV_SET(ke + 1, hObject, EVFILT_WRITE, EV_ADD | EV_CLEAR | EV_ENABLE, 0, 0, instance);
				ret = ::kevent(handle->kq, ke, 2, sl_null, 0, sl_null);
				break;
			case AsyncIoMode::In:
				EV_SET(ke, hObject, EVFILT_READ, EV_ADD | EV_CLEAR | EV_ENABLE, 0, 0, instance);
				ret = ::kevent(handle->kq, ke, 1, sl_null, 0, sl_null);
				break;
			case AsyncIoMode::Out:
				EV_SET(ke, hObject, EVFILT_WRITE, EV_ADD | EV_CLEAR | EV_ENABLE, 0, 0, instance);
				ret = ::kevent(handle->kq, ke, 1, sl_null, 0, sl_null);
				break;
			default:
				break;
		}
		if (ret != -1) {
			instance->setMode(mode);
			return sl_true;
		}
		return sl_false;
	}

	void AsyncIoLoop::__detachInstance(AsyncIoInstance* instance)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;
		int hObject = (int)(instance->getHandle());
		
		AsyncIoMode mode = instance->getMode();
		struct kevent ke[2];
		int ret = -1;
		
		switch (mode) {
			case AsyncIoMode::InOut:
				EV_SET(ke, hObject, EVFILT_READ, EV_DELETE, 0, 0, sl_null);
				EV_SET(ke + 1, hObject, EVFILT_WRITE, EV_DELETE, 0, 0, sl_null);
				ret = ::kevent(handle->kq, ke, 2, sl_null, 0, sl_null);
				break;
			case AsyncIoMode::In:
				EV_SET(ke, hObject, EVFILT_READ, EV_DELETE, 0, 0, sl_null);
				ret = ::kevent(handle->kq, ke, 1, sl_null, 0, sl_null);
				break;
			case AsyncIoMode::Out:
				EV_SET(ke, hObject, EVFILT_WRITE, EV_DELETE, 0, 0, sl_null);
				ret = ::kevent(handle->kq, ke, 1, sl_null, 0, sl_null);
				break;
			default:
				break;
		}
		
		if (ret == -1) {
			SLIB_UNUSED(ret);
		}
	}

}

#endif
