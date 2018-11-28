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
		sl_bool _native_wait(sl_int32 timeout) override;
		
		virtual sl_bool _native_setup(sl_uint32 events) = 0;
		
		static sl_bool _native_waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout);
		
	protected:
		Ref<Socket> m_socket;
		
	};

}

#endif
