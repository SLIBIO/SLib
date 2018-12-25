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

#ifndef CHECKHEADER_SLIB_NETWORK_ASYNC
#define CHECKHEADER_SLIB_NETWORK_ASYNC

#include "definition.h"

#include "socket.h"

#include "../core/async.h"

namespace slib
{

	class AsyncTcpSocket;
	class AsyncTcpSocketInstance;
	
	class SLIB_EXPORT AsyncTcpSocketParam
	{
	public:
		// optional
		Ref<Socket> socket;
		SocketAddress bindAddress;
		SocketAddress connectAddress;
		sl_bool flagIPv6; // default: false
		sl_bool flagLogError; // default: true
		Ref<AsyncIoLoop> ioLoop;
		
		Function<void(AsyncTcpSocket*, const SocketAddress&, sl_bool)> onConnect;
		Function<void(AsyncTcpSocket*)> onError;
		
	public:
		AsyncTcpSocketParam();
		
		~AsyncTcpSocketParam();
		
	};
	
	class SLIB_EXPORT AsyncTcpSocket : public AsyncStreamBase
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AsyncTcpSocket();
		
		~AsyncTcpSocket();
		
	public:
		static Ref<AsyncTcpSocket> create(const AsyncTcpSocketParam& param);
		
	public:
		Ref<Socket> getSocket();
		
		sl_bool connect(const SocketAddress& address);
		
		sl_bool receive(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* userObject = sl_null);
		
		sl_bool receive(const Memory& mem, const Function<void(AsyncStreamResult*)>& callback);
		
		sl_bool send(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* userObject = sl_null);
		
		sl_bool send(const Memory& mem, const Function<void(AsyncStreamResult*)>& callback);
		
	protected:
		Ref<AsyncTcpSocketInstance> _getIoInstance();
		
		void _onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
		
		void _onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
		
		void _onConnect(const SocketAddress& address, sl_bool flagError);
		
		void _onError();
		
	private:
		static Ref<AsyncTcpSocketInstance> _createInstance(const Ref<Socket>& socket);
		
	protected:
		Function<void(AsyncTcpSocket*, const SocketAddress&, sl_bool flagError)> m_onConnect;
		Function<void(AsyncTcpSocket*)> m_onError;
		
		friend class AsyncTcpSocketInstance;
		
	};
	
	
	class AsyncTcpServer;
	class AsyncTcpServerInstance;
	
	class SLIB_EXPORT AsyncTcpServerParam
	{
	public:
		Ref<Socket> socket;
		SocketAddress bindAddress;
		
		// optional
		sl_bool flagIPv6; // default: false
		sl_bool flagAutoStart; // default: true
		sl_bool flagLogError; // default: true
		Ref<AsyncIoLoop> ioLoop;
		
		Function<void(AsyncTcpServer*, Socket*, const SocketAddress&)> onAccept;
		Function<void(AsyncTcpServer*)> onError;
		
	public:
		AsyncTcpServerParam();
		
		~AsyncTcpServerParam();
		
	};
	
	class SLIB_EXPORT AsyncTcpServer : public AsyncIoObject
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AsyncTcpServer();
		
		~AsyncTcpServer();
		
	public:
		static Ref<AsyncTcpServer> create(const AsyncTcpServerParam& param);
		
	public:
		void close();
		
		sl_bool isOpened();
		
		void start();
		
		sl_bool isRunning();
		
		Ref<Socket> getSocket();
		
	protected:
		Ref<AsyncTcpServerInstance> _getIoInstance();
		
		void _onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address);
		
		void _onError();
		
	protected:
		static Ref<AsyncTcpServerInstance> _createInstance(const Ref<Socket>& socket);
		
	protected:
		Function<void(AsyncTcpServer*, Socket*, const SocketAddress&)> m_onAccept;
		Function<void(AsyncTcpServer*)> m_onError;
		
		friend class AsyncTcpServerInstance;
		
	};
	
	
	class AsyncUdpSocket;
	class AsyncUdpSocketInstance;

	class SLIB_EXPORT AsyncUdpSocketParam
	{
	public:
		// optional
		Ref<Socket> socket;
		SocketAddress bindAddress;
		sl_bool flagIPv6; // default: false
		sl_bool flagBroadcast; // default: false
		sl_bool flagAutoStart; // default: true
		sl_bool flagLogError; // default: true
		sl_uint32 packetSize; // default: 65536
		Ref<AsyncIoLoop> ioLoop;
		
		Function<void(AsyncUdpSocket*, const SocketAddress&, void* data, sl_uint32 sizeReceived)> onReceiveFrom;
		
	public:
		AsyncUdpSocketParam();
		
		~AsyncUdpSocketParam();
		
	};
	
	class SLIB_EXPORT AsyncUdpSocket : public AsyncIoObject
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AsyncUdpSocket();
		
		~AsyncUdpSocket();
		
	public:
		static Ref<AsyncUdpSocket> create(const AsyncUdpSocketParam& param);
		
	public:
		void close();
		
		sl_bool isOpened();
		
		void start();
		
		sl_bool isRunning();
		
	public:
		Ref<Socket> getSocket();
		
		void setBroadcast(sl_bool flag);
		
		void setSendBufferSize(sl_uint32 size);
		
		void setReceiveBufferSize(sl_uint32 size);
		
		sl_bool sendTo(const SocketAddress& addressTo, const void* data, sl_uint32 size);
		
		sl_bool sendTo(const SocketAddress& addressTo, const Memory& mem);
		
	protected:
		Ref<AsyncUdpSocketInstance> _getIoInstance();
		
		void _onReceive(const SocketAddress& address, void* data, sl_uint32 sizeReceived);
		
	protected:
		static Ref<AsyncUdpSocketInstance> _createInstance(const Ref<Socket>& socket, sl_uint32 packetSize);
		
	protected:
		Function<void(AsyncUdpSocket*, const SocketAddress&, void* data, sl_uint32 sizeReceived)> m_onReceiveFrom;
		
		friend class AsyncUdpSocketInstance;
		
	};

}

#endif
