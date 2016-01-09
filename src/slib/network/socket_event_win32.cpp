#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_NETWORK_NAMESPACE_BEGIN
class _Win_SocketEvent : public SocketEvent
{
public:
	WSAEVENT m_hEvent;

	_Win_SocketEvent()
	{
	}

	~_Win_SocketEvent()
	{
		::WSACloseEvent(m_hEvent);
	}

	static Ref<_Win_SocketEvent> create(const Ref<Socket>& socket)
	{
		Ref<_Win_SocketEvent> ret;
		if (socket.isNotNull()) {
			Socket::initializeSocket();
			WSAEVENT hEvent = ::WSACreateEvent();
			if (hEvent != WSA_INVALID_EVENT) {
				ret = new _Win_SocketEvent;
				if (ret.isNotNull()) {
					ret->m_hEvent = hEvent;
					ret->m_socket = socket;
					return ret;
				}
				::WSACloseEvent(hEvent);
			}
		}
		return ret;
	}

	sl_bool __setup(sl_uint32 events)
	{
		sl_uint32 ev = 0;
		if (events & eventRead) {
			ev = ev | FD_READ | FD_ACCEPT;
		}
		if (events & eventWrite) {
			ev = ev | FD_WRITE | FD_CONNECT;
		}
		if (events & eventClose) {
			ev = ev | FD_CLOSE;
		}
		int ret = ::WSAEventSelect((SOCKET)(m_socket->getHandle()), m_hEvent, ev);
		if (ret) {
			return sl_false;
		}
		return sl_true;
	}

	void __set()
	{
		::WSASetEvent(m_hEvent);
	}

	void __reset()
	{
		::WSAResetEvent(m_hEvent);
	}
};

Ref<SocketEvent> SocketEvent::create(const Ref<Socket>& socket)
{
	Ref<_Win_SocketEvent> ret = _Win_SocketEvent::create(socket);
	return Ref<SocketEvent>::from(ret);
}

sl_bool SocketEvent::__waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout)
{
	DWORD t = timeout >= 0 ? timeout : WSA_INFINITE;
	SLIB_SCOPED_BUFFER(WSAEVENT, 64, hEvents, count);
	SLIB_SCOPED_BUFFER(sl_uint32, 64, indexMap, count);
	sl_uint32 cEvents = 0;
	for (sl_uint32 i = 0; i < count; i++) {
		Ref<_Win_SocketEvent> ev = Ref<_Win_SocketEvent>::from(events[i]);
		if (ev.isNotNull()) {
			Ref<Socket> sock = ev->getSocket();
			if (sock.isNotNull() && sock->isOpened()) {
				hEvents[cEvents] = ev->m_hEvent;
				indexMap[cEvents] = i;
				cEvents++;
			}
		}
		if (status) {
			status[i] = 0;
		}
	}
	if (cEvents == 0) {
		return sl_false;
	}
	
	DWORD dwRet = ::WSAWaitForMultipleEvents(cEvents, hEvents, FALSE, t, TRUE);
	if (dwRet >= WSA_WAIT_EVENT_0 && dwRet < WSA_WAIT_EVENT_0 + cEvents) {
		sl_uint32 indexHandle = dwRet - WSA_WAIT_EVENT_0;
		sl_uint32 index = indexMap[indexHandle];
		WSANETWORKEVENTS ne;
		ZeroMemory(&ne, sizeof(ne));
		Ref<_Win_SocketEvent> ev = Ref<_Win_SocketEvent>::from(events[index]);
		if (ev.isNotNull()) {
			Ref<Socket> sock = ev->getSocket();
			if (sock.isNotNull() && 0 == ::WSAEnumNetworkEvents((SOCKET)(sock->getHandle()), hEvents[indexHandle], &ne)) {
				sl_uint32 st = 0;
				sl_uint32 le = ne.lNetworkEvents;
				if (le & (FD_CONNECT | FD_WRITE)) {
					st |= eventWrite;
				}
				if (le & (FD_ACCEPT | FD_READ)) {
					st |= eventRead;
				}
				if (le & FD_CLOSE) {
					st |= eventClose;
				}
				if (status) {
					status[index] = st;
				}
				return sl_true;
			}
		}
	}
    return sl_false;
}
SLIB_NETWORK_NAMESPACE_END
#endif
