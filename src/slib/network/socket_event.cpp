/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/event.h"
#include "slib/core/thread.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(SocketEvent, Event)
	
	SocketEvent::SocketEvent()
	{
	}
	
	SocketEvent::~SocketEvent()
	{
	}
	
	Ref<SocketEvent> SocketEvent::create(const Ref<Socket>& socket, sl_uint32 events)
	{
		Ref<SocketEvent> ev = SocketEvent::create(socket);
		if (ev.isNotNull()) {
			if (ev->setup(events)) {
				return ev;
			}
		}
		return sl_null;
	}
	
	Ref<SocketEvent> SocketEvent::createRead(const Ref<Socket>& socket)
	{
		return SocketEvent::create(socket, SocketEvent::Read | SocketEvent::Close);
	}
	
	Ref<SocketEvent> SocketEvent::createWrite(const Ref<Socket>& socket)
	{
		return SocketEvent::create(socket, SocketEvent::Write | SocketEvent::Close);
	}
	
	Ref<SocketEvent> SocketEvent::createReadWrite(const Ref<Socket>& socket)
	{
		return SocketEvent::create(socket, SocketEvent::Read | SocketEvent::Write | SocketEvent::Close);
	}
	
	sl_bool SocketEvent::setup(sl_uint32 events)
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNotNull() && socket->isOpened()) {
			if (socket->setNonBlockingMode(sl_true)) {
				return _native_setup(events);
			}
		}
		return sl_false;
	}
	
	sl_bool SocketEvent::setupRead()
	{
		return setup(SocketEvent::Read | SocketEvent::Close);
	}
	
	sl_bool SocketEvent::setupWrite()
	{
		return setup(SocketEvent::Write | SocketEvent::Close);
	}
	
	sl_bool SocketEvent::setupReadWrite()
	{
		return setup(SocketEvent::Read | SocketEvent::Write | SocketEvent::Close);
	}
	
	sl_uint32 SocketEvent::waitEvents(sl_int32 timeout)
	{
		sl_uint32 events;
		Ref<SocketEvent> ev = this;
		if (waitMultipleEvents(&ev, &events, 1, timeout)) {
			return events;
		}
		return 0;
	}
	
	sl_bool SocketEvent::_native_wait(sl_int32 timeout)
	{
		if (waitEvents(timeout) != 0) {
			return sl_true;
		}
		return sl_false;
	}
	
#define _MAX_WAIT_EVENTS 64
	sl_bool SocketEvent::waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout)
	{
		if (count == 0 || count > _MAX_WAIT_EVENTS) { // WSA_MAXIMUM_WAIT_EVENTS
			return sl_false;
		}
		Ref<Thread> thread = Thread::getCurrent();
		if (thread.isNotNull()) {
			if (thread->isStopping()) {
				return sl_false;
			}
			thread->setWaitingEvent(events[0].get());
		}
		sl_bool ret = _native_waitMultipleEvents(events, status, count, timeout);
		if (thread.isNotNull()) {
			thread->clearWaitingEvent();
		}
		return ret;
	}
	
	const Ref<Socket>& SocketEvent::getSocket()
	{
		return m_socket;
	}

	sl_bool Socket::connectAndWait(const SocketAddress& address, sl_int32 timeout)
	{
		setNonBlockingMode(sl_true);
		if (connect(address)) {
			Ref<SocketEvent> ev = SocketEvent::createWrite(this);
			if (ev.isNotNull()) {
				if (ev->waitEvents(timeout) & SocketEvent::Write) {
					if (getOption_Error() == 0) {
						return sl_true;
					}
				}
			}
		}
		return sl_false;
	}

}
