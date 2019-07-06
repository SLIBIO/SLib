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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "slib/network/event.h"
#include "slib/core/scoped.h"

namespace slib
{

	namespace priv
	{
		namespace socket_event
		{

			class SocketEventImpl : public SocketEvent
			{
			public:
				WSAEVENT m_hEvent;

			public:
				SocketEventImpl()
				{
				}

				~SocketEventImpl()
				{
					::WSACloseEvent(m_hEvent);
				}

			public:
				static Ref<SocketEventImpl> create(const Ref<Socket>& socket)
				{
					Ref<SocketEventImpl> ret;
					if (socket.isNotNull()) {
						Socket::initializeSocket();
						WSAEVENT hEvent = ::WSACreateEvent();
						if (hEvent != WSA_INVALID_EVENT) {
							ret = new SocketEventImpl;
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

				sl_bool _native_setup(sl_uint32 events)
				{
					sl_uint32 ev = 0;
					if (events & SocketEvent::Read) {
						ev = ev | FD_READ | FD_ACCEPT;
					}
					if (events & SocketEvent::Write) {
						ev = ev | FD_WRITE | FD_CONNECT;
					}
					if (events & SocketEvent::Close) {
						ev = ev | FD_CLOSE;
					}
					int ret = ::WSAEventSelect((SOCKET)(m_socket->getHandle()), m_hEvent, ev);
					if (ret) {
						return sl_false;
					}
					return sl_true;
				}

				void _native_set()
				{
					::WSASetEvent(m_hEvent);
				}

				void _native_reset()
				{
					::WSAResetEvent(m_hEvent);
				}
			};

		}
	}

	using namespace priv::socket_event;

	Ref<SocketEvent> SocketEvent::create(const Ref<Socket>& socket)
	{
		return SocketEventImpl::create(socket);
	}

	sl_bool SocketEvent::_native_waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout)
	{
		DWORD t = timeout >= 0 ? timeout : WSA_INFINITE;
		SLIB_SCOPED_BUFFER(WSAEVENT, 64, hEvents, count);
		SLIB_SCOPED_BUFFER(sl_uint32, 64, indexMap, count);
		sl_uint32 cEvents = 0;
		for (sl_uint32 i = 0; i < count; i++) {
			Ref<SocketEventImpl> ev = Ref<SocketEventImpl>::from(events[i]);
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
			Ref<SocketEventImpl> ev = Ref<SocketEventImpl>::from(events[index]);
			if (ev.isNotNull()) {
				Ref<Socket> sock = ev->getSocket();
				if (sock.isNotNull() && 0 == ::WSAEnumNetworkEvents((SOCKET)(sock->getHandle()), hEvents[indexHandle], &ne)) {
					sl_uint32 st = 0;
					sl_uint32 le = ne.lNetworkEvents;
					if (le & (FD_CONNECT | FD_WRITE)) {
						st |= SocketEvent::Write;
					}
					if (le & (FD_ACCEPT | FD_READ)) {
						st |= SocketEvent::Read;
					}
					if (le & FD_CLOSE) {
						st |= SocketEvent::Close;
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

}

#endif
