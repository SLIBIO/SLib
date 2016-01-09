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

SLIB_NAMESPACE_BEGIN
struct _AsyncLoopHandle
{
	int fdEpoll;
	Ref<PipeEvent> eventWake;
};

void* AsyncLoop::__createHandle()
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
		_AsyncLoopHandle* handle = new _AsyncLoopHandle;
		if (handle) {
			handle->fdEpoll = fdEpoll;
			handle->eventWake = pipe;
			// register wake event
			epoll_event ev;
			ev.data.ptr = sl_null;
			ev.events = EPOLLIN | EPOLLPRI;
			if (0 == epoll_ctl(fdEpoll, EPOLL_CTL_ADD, (int)(pipe->getReadPipeHandle()), &ev)) {
				return handle;
			}
			delete handle;
		}
		::close(fdEpoll);
	}
	return 0;
}

void AsyncLoop::__closeHandle(void* _handle)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)_handle;
	::close(handle->fdEpoll);
	delete handle;
}

void AsyncLoop::__runLoop()
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;

	epoll_event waitEvents[ASYNC_MAX_WAIT_EVENT];

	while (m_flagRunning) {

		_stepBegin();

		int nEvents = ::epoll_wait(handle->fdEpoll, waitEvents, ASYNC_MAX_WAIT_EVENT, 0);
		if (nEvents == 0) {
			m_queueInstancesClosed.removeAll();
			int timeout = _getTimeout();
			nEvents = ::epoll_wait(handle->fdEpoll, waitEvents, ASYNC_MAX_WAIT_EVENT, timeout);
		}

		for (int i = 0; m_flagRunning && i < nEvents; i++) {
			epoll_event& ev = waitEvents[i];
			AsyncInstance* instance = (AsyncInstance*)(ev.data.ptr);
			if (instance && !(instance->isClosing())) {
				AsyncInstance::EventDesc desc;
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
			} else {
				handle->eventWake->reset();
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
	handle->eventWake->set();
}

sl_bool AsyncLoop::__attachInstance(AsyncInstance* instance)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;
	int hObject = (int)(instance->getHandle());
	epoll_event ev;
	ev.data.ptr = (void*)instance;
#if defined(EPOLL_LOW)
	ev.events = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLET;
#else
	ev.events = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLRDHUP | EPOLLET;
#endif
	int ret = ::epoll_ctl(handle->fdEpoll, EPOLL_CTL_ADD, hObject, &ev);
	if (ret == 0) {
		return sl_true;
	} else {
		return sl_false;
	}
}

void AsyncLoop::__detachInstance(AsyncInstance* instance)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;
	int hObject = (int)(instance->getHandle());
	epoll_event ev;
	int ret = ::epoll_ctl(handle->fdEpoll, EPOLL_CTL_DEL, hObject, &ev);
	SLIB_UNUSED(ret);
}
SLIB_NAMESPACE_END

#endif
