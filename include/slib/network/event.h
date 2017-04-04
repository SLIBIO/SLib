/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_EVENT
#define CHECKHEADER_SLIB_NETWORK_EVENT

#include "definition.h"

#include "socket.h"
#include "../core/event.h"

namespace slib
{

	class SLIB_EXPORT SocketEvent : public Event
	{
		SLIB_DECLARE_OBJECT
		
	public:
		enum
		{
			Read = 1,	// receive, receiveFrom, accept
			Write = 2,	// send, sendTo, connect
			Close = 4	// close, error
		};
		
	public:
		SocketEvent();
		
		~SocketEvent();
		
	public:
		static Ref<SocketEvent> create(const Ref<Socket>& socket);
		
		static Ref<SocketEvent> create(const Ref<Socket>& socket, sl_uint32 events);
		
		static Ref<SocketEvent> createRead(const Ref<Socket>& socket);
		
		static Ref<SocketEvent> createWrite(const Ref<Socket>& socket);
		
		static Ref<SocketEvent> createReadWrite(const Ref<Socket>& socket);
		
	public:
		sl_bool setup(sl_uint32 events);
		
		sl_bool setupRead();
		
		sl_bool setupWrite();
		
		sl_bool setupReadWrite();
		
		sl_uint32 waitEvents(sl_int32 timeout = -1);
		
		// count must be less than 64
		static sl_bool waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout = -1);
		
		const Ref<Socket>& getSocket();
		
	protected:
		sl_bool __wait(sl_int32 timeout);
		
		virtual sl_bool __setup(sl_uint32 events) = 0;
		
		static sl_bool __waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout);
		
	protected:
		Ref<Socket> m_socket;
		
	};

}

#endif
