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

#if defined(ASYNC_USE_EPOLL)

#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/pipe.h"

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/errno.h>

#if defined(SLIB_PLATFORM_IS_ANDROID)
#define EPOLL_LOW
#endif

namespace slib
{

	struct _AsyncIoLoopHandle
	{
		int fdEpoll;
		Ref<PipeEvent> eventWake;
	};

	void* AsyncIoLoop::__createHandle()
	{
		Ref<PipeEvent> pipe = PipeEvent::create();
		if (pipe.isNull()) {
			return 0;
		}
		int fdEpoll;
#if defined(EPOLL_LOW)
		fdEpoll = ::epoll_create(1024);
#else
		fdEpoll = ::epoll_create1(0);
#endif
		if (fdEpoll >= 0) {
			_AsyncIoLoopHandle* handle = new _AsyncIoLoopHandle;
			if (handle) {
				handle->fdEpoll = fdEpoll;
				handle->eventWake = pipe;
				// register wake event
				epoll_event ev;
				ev.data.ptr = sl_null;
				ev.events = EPOLLIN | EPOLLPRI | EPOLLET;
				if (0 == epoll_ctl(fdEpoll, EPOLL_CTL_ADD, (int)(pipe->getReadPipeHandle()), &ev)) {
					return handle;
				}
				delete handle;
			}
			::close(fdEpoll);
		}
		return 0;
	}

	void AsyncIoLoop::__closeHandle(void* _handle)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)_handle;
		::close(handle->fdEpoll);
		delete handle;
	}

	void AsyncIoLoop::__runLoop()
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;

		epoll_event waitEvents[ASYNC_MAX_WAIT_EVENT];

		while (m_flagRunning) {

			_stepBegin();

			int nEvents = ::epoll_wait(handle->fdEpoll, waitEvents, ASYNC_MAX_WAIT_EVENT, -1);
			if (nEvents == 0) {
				m_queueInstancesClosed.removeAll();
			}
			if (nEvents < 0) {
				int err = errno;
				if (err == EBADF || err == EFAULT || err == EINVAL) {
					//break;
				}
			}

			for (int i = 0; m_flagRunning && i < nEvents; i++) {
				epoll_event& ev = waitEvents[i];
				AsyncIoInstance* instance = (AsyncIoInstance*)(ev.data.ptr);
				if (instance) {
					if (!(instance->isClosing())) {
						AsyncIoInstance::EventDesc desc;
						desc.flagIn = sl_false;
						desc.flagOut = sl_false;
						desc.flagError = sl_false;
						int re = ev.events;
						if (re & (EPOLLIN | EPOLLPRI)) {
							desc.flagIn = sl_true;
						}
						if (re & (EPOLLOUT)) {
							desc.flagOut = sl_true;
						}
#if defined(EPOLL_LOW)
						if (re & (EPOLLERR | EPOLLHUP)) {
#else
						if (re & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
#endif
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
		epoll_event ev;
		ev.data.ptr = (void*)instance;
		
#if defined(EPOLL_LOW)
		ev.events = EPOLLET;
#else
		ev.events = EPOLLRDHUP | EPOLLET;
#endif
		switch (mode) {
			case AsyncIoMode::In:
				ev.events |= EPOLLIN | EPOLLPRI;
				break;
			case AsyncIoMode::Out:
				ev.events |= EPOLLOUT;
				break;
			case AsyncIoMode::InOut:
				ev.events |= EPOLLIN | EPOLLPRI | EPOLLOUT;
				break;
			default:
				return sl_true;
		}
		
		int ret = ::epoll_ctl(handle->fdEpoll, EPOLL_CTL_ADD, hObject, &ev);
		if (ret == 0) {
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void AsyncIoLoop::__detachInstance(AsyncIoInstance* instance)
	{
		_AsyncIoLoopHandle* handle = (_AsyncIoLoopHandle*)m_handle;
		int hObject = (int)(instance->getHandle());
		epoll_event ev;
		int ret = ::epoll_ctl(handle->fdEpoll, EPOLL_CTL_DEL, hObject, &ev);
		SLIB_UNUSED(ret);
	}

}

#endif
