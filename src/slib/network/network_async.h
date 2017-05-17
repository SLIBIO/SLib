/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		
		sl_bool isSupportingConnect();
		
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
		
		sl_bool sendTo(const SocketAddress& address, const Memory& data);
		
	protected:
		void _onReceive(const SocketAddress& address, sl_uint32 size);
		
	protected:
		AtomicRef<Socket> m_socket;

		sl_bool m_flagRunning;
		Memory m_buffer;
		
		struct SendRequest
		{
			SocketAddress addressTo;
			Memory data;
		};
		LinkedQueue<SendRequest> m_queueSendRequests;
		
	};
	
}

#endif
