/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	
	class SLIB_EXPORT IAsyncTcpSocketListener
	{
	public:
		IAsyncTcpSocketListener();

		virtual ~IAsyncTcpSocketListener();

	public:
		virtual void onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError) = 0;
		
		virtual void onError(AsyncTcpSocket* socketListen);
		
	};
	
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
		
		Ptr<IAsyncTcpSocketListener> listener;
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
		Ptr<IAsyncTcpSocketListener> m_listener;
		Function<void(AsyncTcpSocket*, const SocketAddress&, sl_bool)> m_onConnect;
		Function<void(AsyncTcpSocket*)> m_onError;
		
		friend class AsyncTcpSocketInstance;
		
	};
	
	
	class AsyncTcpServer;
	class AsyncTcpServerInstance;
	
	class SLIB_EXPORT IAsyncTcpServerListener
	{
	public:
		IAsyncTcpServerListener();

		virtual ~IAsyncTcpServerListener();

	public:
		virtual void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address) = 0;
		
		virtual void onError(AsyncTcpServer* socketListen);
		
	};
	
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
		
		Ptr<IAsyncTcpServerListener> listener;
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
		Ptr<IAsyncTcpServerListener> m_listener;
		Function<void(AsyncTcpServer*, Socket*, const SocketAddress&)> m_onAccept;
		Function<void(AsyncTcpServer*)> m_onError;
		
		friend class AsyncTcpServerInstance;
		
	};
	
	
	class AsyncUdpSocket;
	class AsyncUdpSocketInstance;
	
	class SLIB_EXPORT IAsyncUdpSocketListener
	{
	public:
		IAsyncUdpSocketListener();

		virtual ~IAsyncUdpSocketListener();

	public:
		virtual void onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& address, void* data, sl_uint32 sizeReceived) = 0;
		
	};
	
	
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
		
		Ptr<IAsyncUdpSocketListener> listener;
		Function<void(AsyncUdpSocket*, const SocketAddress&, void*, sl_uint32)> onReceiveFrom;
		
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
		Ptr<IAsyncUdpSocketListener> m_listener;
		Function<void(AsyncUdpSocket*, const SocketAddress&, void*, sl_uint32)> m_onReceiveFrom;
		
		friend class AsyncUdpSocketInstance;
		
	};

}

#endif
