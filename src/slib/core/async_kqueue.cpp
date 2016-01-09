#include "async_config.h"

#if defined(ASYNC_USE_KQUEUE)

#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/pipe.h"

#include <sys/types.h>
#include <sys/event.h>
#include <unistd.h>

SLIB_NAMESPACE_BEGIN
struct _AsyncLoopHandle
{
	int kq;
	Ref<PipeEvent> eventWake;
};

void* AsyncLoop::__createHandle()
{
	Ref<PipeEvent> pipe = PipeEvent::create();
	if (pipe.isNull()) {
		return 0;
	}
	int kq;
	kq = ::kqueue();
	if (kq != -1) {
		_AsyncLoopHandle* handle = new _AsyncLoopHandle;
		if (handle) {
			handle->kq = kq;
			handle->eventWake = pipe;
			// register wake event
			struct kevent ke;
			EV_SET(&ke, (int)(pipe->getReadPipeHandle()), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, sl_null);
			if (-1 != ::kevent(kq, &ke, 1, sl_null, 0, sl_null)) {
				return handle;
			}
			delete handle;
		}
		::close(kq);
	}
	return sl_null;
}

void AsyncLoop::__closeHandle(void* _handle)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)_handle;
	::close(handle->kq);
	delete handle;
}

void AsyncLoop::__runLoop()
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;

	struct kevent waitEvents[ASYNC_MAX_WAIT_EVENT];

	while (m_flagRunning) {

		_stepBegin();

		int nEvents = ::kevent(handle->kq, sl_null, 0, waitEvents, ASYNC_MAX_WAIT_EVENT, 0);
		
		if (nEvents == 0) {

			m_queueInstancesClosed.removeAll();

			timespec* timeout;
			timespec _timeout;
			sl_int32 _t = _getTimeout();
			if (_t > 0) {
				_timeout.tv_sec = _t / 1000;
				_t = _t % 1000;
				_timeout.tv_nsec = _t * 1000000;
				timeout = &_timeout;
			} else {
				timeout = NULL;
			}

			nEvents = ::kevent(handle->kq, sl_null, 0, waitEvents, ASYNC_MAX_WAIT_EVENT, timeout);
		}

		for (int i = 0; m_flagRunning && i < nEvents; i++) {
			struct kevent& ev = waitEvents[i];
			AsyncInstance* instance = (AsyncInstance*)(ev.udata);
			if (instance && !(instance->isClosing())) {
				AsyncInstance::EventDesc desc;
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
	struct kevent ke[2];
	EV_SET(ke, hObject, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, instance);
	EV_SET(ke + 1, hObject, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, instance);
	if (-1 != ::kevent(handle->kq, ke, 2, sl_null, 0, sl_null)) {
		return sl_true;
	} else {
		return sl_false;
	}
}

void AsyncLoop::__detachInstance(AsyncInstance* instance)
{
	_AsyncLoopHandle* handle = (_AsyncLoopHandle*)m_handle;
	int hObject = (int)(instance->getHandle());
	struct kevent ke[2];
	EV_SET(ke, hObject, EVFILT_READ, EV_DELETE, 0, 0, sl_null);
	EV_SET(ke + 1, hObject, EVFILT_WRITE, EV_DELETE, 0, 0, sl_null);
	int ret = ::kevent(handle->kq, ke, 2, sl_null, 0, sl_null);
	if (ret == -1) {
		SLIB_UNUSED(ret);
	}
}
SLIB_NAMESPACE_END

#endif
