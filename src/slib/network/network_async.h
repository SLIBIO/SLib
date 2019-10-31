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

#ifndef CHECKHEADER_SLIB_NETWORK_NETWORK_ASYNC_CONFIG
#define CHECKHEADER_SLIB_NETWORK_NETWORK_ASYNC_CONFIG

#include "slib/network/async.h"

#include "slib/core/log.h"

#define TAG "AsyncSocket"

#define ASYNC_UDP_PACKET_SIZE 65535

namespace slib
{

	class SLIB_EXPORT AsyncTcpSocketInstance : public AsyncStreamInstance
	{
	protected:
		AsyncTcpSocketInstance();
		
		~AsyncTcpSocketInstance();
		
	public:
		Ref<Socket> getSocket();
		
		sl_bool isSupportedConnect();
		
	public:
		sl_bool connect(const SocketAddress& address);
		
	protected:
		void _onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
		
		void _onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
		
		void _onConnect(sl_bool flagError);
		
	protected:
		AtomicRef<Socket> m_socket;
		
		sl_bool m_flagSupportingConnect;
		sl_bool m_flagRequestConnect;
		SocketAddress m_addressRequestConnect;
		
	};

	class SLIB_EXPORT AsyncTcpServerInstance : public AsyncIoInstance
	{
	public:
		AsyncTcpServerInstance();
		
		~AsyncTcpServerInstance();
		
	public:
		void close() override;
		
		void start();
		
		sl_bool isRunning();
		
		Ref<Socket> getSocket();
			
	protected:
		void _onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address);
		
		void _onError();
		
	protected:
		AtomicRef<Socket> m_socket;
		
		sl_bool m_flagRunning;
		
	};

	class SLIB_EXPORT AsyncUdpSocketInstance : public AsyncIoInstance
	{
	public:
		AsyncUdpSocketInstance();
		
		~AsyncUdpSocketInstance();
		
	public:
		void close() override;
		
		void start();
		
		sl_bool isRunning();
		
		Ref<Socket> getSocket();
		
	protected:
		void _onReceive(const SocketAddress& address, sl_uint32 size);
		
		void _onError();
		
	protected:
		AtomicRef<Socket> m_socket;

		sl_bool m_flagRunning;
		Memory m_buffer;
		
	};
	
}

#endif
