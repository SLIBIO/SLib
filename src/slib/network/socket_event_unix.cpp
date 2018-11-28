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

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "slib/network/event.h"
#include "slib/core/pipe.h"
#include "slib/core/thread.h"
#include "slib/core/scoped.h"

#include <unistd.h>
#include <poll.h>

namespace slib
{
	class _priv_Unix_SocketEvent : public SocketEvent
	{
	public:
		Ref<PipeEvent> m_pipe;
		sl_uint32 m_events;
		
	public:
		_priv_Unix_SocketEvent()
		{
			m_events = 0;
		}
		
		~_priv_Unix_SocketEvent()
		{
		}
		
	public:
		static Ref<_priv_Unix_SocketEvent> create(const Ref<Socket>& socket)
		{
			Ref<_priv_Unix_SocketEvent> ret;
			if (socket.isNotNull()) {
				Socket::initializeSocket();
				Ref<PipeEvent> ev = PipeEvent::create();
				if (ev.isNotNull()) {
					ret = new _priv_Unix_SocketEvent;
					if (ret.isNotNull()) {
						ret->m_pipe = ev;
						ret->m_socket = socket;
						return ret;
					}
				}
			}
			return ret;
		}
		
		sl_bool _native_setup(sl_uint32 events) override
		{
			m_events = events;
			return sl_true;
		}
		
		void _native_set() override
		{
			m_pipe->set();
		}
		
		void _native_reset() override
		{
			m_pipe->reset();
		}
	};
	
	Ref<SocketEvent> SocketEvent::create(const Ref<Socket>& socket)
	{
		return Ref<SocketEvent>::from(_priv_Unix_SocketEvent::create(socket));
	}

	sl_bool SocketEvent::_native_waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout)
	{
		SLIB_SCOPED_BUFFER(pollfd, 64, fd, 2*count);
		SLIB_SCOPED_BUFFER(sl_uint32, 64, indexMap, count);
		Base::zeroMemory(fd, sizeof(pollfd)*2*count);
		sl_uint32 cEvents = 0;
		for (sl_uint32 i = 0; i < count; i++) {
			Ref<_priv_Unix_SocketEvent> ev = Ref<_priv_Unix_SocketEvent>::from(events[i]);
			if (ev.isNotNull()) {
				Ref<Socket> sock = ev->getSocket();
				if (sock.isNotNull()) {
					fd[cEvents * 2].fd = (int)(sock->getHandle());
					sl_uint32 evs = 0;
					sl_uint32 sevs = ev->m_events;
					if (sevs & SocketEvent::Read) {
						evs = evs | POLLIN | POLLPRI;
					}
					if (sevs & SocketEvent::Write) {
						evs = evs | POLLOUT;
					}
					if (sevs & SocketEvent::Close) {
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
					ret |= SocketEvent::Read;
				}
				if (revs & POLLOUT) {
					ret |= SocketEvent::Write;
				}
#if defined(SLIB_PLATFORM_IS_LINUX)
				if (revs & (POLLERR | POLLHUP | POLLRDHUP)) {
#else
				if (revs & (POLLERR | POLLHUP)) {
#endif
					ret |= SocketEvent::Close;
				}
				if (status) {
					status[indexMap[k]] = ret;
				}
				if (fd[k*2+1].revents) {
					Ref<_priv_Unix_SocketEvent> ev = Ref<_priv_Unix_SocketEvent>::from(events[indexMap[k]]);
					ev->reset();
				}
			}
			return sl_true;
		}
		return sl_false;
	}
}

#endif
