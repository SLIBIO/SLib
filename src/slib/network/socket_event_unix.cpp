#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/pipe.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/scoped_pointer.h"

#include <unistd.h>
#include <poll.h>

SLIB_NETWORK_NAMESPACE_BEGIN
class _Unix_SocketEvent : public SocketEvent
{
public:
	Ref<PipeEvent> m_pipe;
	sl_uint32 m_events;

	_Unix_SocketEvent()
	{
		m_events = 0;
	}

	~_Unix_SocketEvent()
	{
	}

	static Ref<_Unix_SocketEvent> create(const Ref<Socket>& socket)
	{
		Ref<_Unix_SocketEvent> ret;
		if (socket.isNotNull()) {
			Socket::initializeSocket();
			Ref<PipeEvent> ev = PipeEvent::create();
			if (ev.isNotNull()) {
				ret = new _Unix_SocketEvent;
				if (ret.isNotNull()) {
					ret->m_pipe = ev;
					ret->m_socket = socket;
					return ret;
				}
			}
		}
		return ret;
	}

	sl_bool __setup(sl_uint32 events)
	{
		m_events = events;
		return sl_true;
	}

	void __set()
	{
		m_pipe->set();
	}

	void __reset()
	{
		m_pipe->reset();
	}
};

Ref<SocketEvent> SocketEvent::create(const Ref<Socket>& socket)
{
	Ref<_Unix_SocketEvent> ret = _Unix_SocketEvent::create(socket);
	return Ref<SocketEvent>::from(ret);
}

sl_bool SocketEvent::__waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout)
{
	SLIB_SCOPED_BUFFER(pollfd, 64, fd, 2*count);
	SLIB_SCOPED_BUFFER(sl_uint32, 64, indexMap, count);
	Base::zeroMemory(fd, sizeof(pollfd)*2*count);
	sl_uint32 cEvents = 0;
	for (sl_uint32 i = 0; i < count; i++) {
		Ref<_Unix_SocketEvent> ev = Ref<_Unix_SocketEvent>::from(events[i]);
		if (ev.isNotNull()) {
			Ref<Socket> sock = ev->getSocket();
			if (sock.isNotNull()) {
				fd[cEvents * 2].fd = (int)(sock->getHandle());
				sl_uint32 evs = 0;
				sl_uint32 sevs = ev->m_events;
				if (sevs & eventRead) {
					evs = evs | POLLIN | POLLPRI;
				}
				if (sevs & eventWrite) {
					evs = evs | POLLOUT;
				}
				if (sevs & eventClose) {
#if defined(SLIB_PLATFORM_IS_LINUX)
					evs = evs | POLLERR | POLLHUP | POLLRDHUP;
#else
					evs = evs | POLLERR | POLLHUP;
#endif
				}
				fd[cEvents * 2].events = evs;
				fd[cEvents * 2 + 1].fd = (int)(ev->m_pipe->getReadPipeHandle());
				fd[cEvents * 2 + 1].events = POLLIN | POLLPRI | POLLERR | POLLHUP;
				indexMap[cEvents] = i;
				cEvents++;
			}
			if (status) {
				status[i] = 0;
			}
		}
	}
	if (cEvents == 0) {
		return sl_false;
	}
		
	int t = timeout >= 0 ? (int)timeout : -1;
	int ret = poll(fd, 2 * (int)cEvents, t);
	if (ret > 0) {
		for (sl_uint32 k = 0; k < cEvents; k++) {
			sl_uint32 ret = 0;
			sl_uint32 revs = fd[k*2].revents;
			if (revs & (POLLIN | POLLPRI)) {
				ret |= eventRead;
			}
			if (revs & POLLOUT) {
				ret |= eventWrite;
			}
#if defined(SLIB_PLATFORM_IS_LINUX)
			if (revs & (POLLERR | POLLHUP | POLLRDHUP)) {
#else
			if (revs & (POLLERR | POLLHUP)) {
#endif
				ret |= eventClose;
			}
			if (status) {
				status[indexMap[k]] = ret;
			}
			if (fd[k*2+1].revents) {
				Ref<_Unix_SocketEvent> ev = Ref<_Unix_SocketEvent>::from(events[indexMap[k]]);
				ev->reset();
			}
		}
		return sl_true;
	}
    return sl_false;
}
SLIB_NETWORK_NAMESPACE_END
#endif
