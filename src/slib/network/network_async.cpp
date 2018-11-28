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

#include "slib/network/async.h"

#include "network_async.h"

namespace slib
{

/*******************************************
			AsyncTcpSocket
********************************************/

	AsyncTcpSocketInstance::AsyncTcpSocketInstance()
	{
		m_flagRequestConnect = sl_false;
		m_flagSupportingConnect = sl_true;
	}

	AsyncTcpSocketInstance::~AsyncTcpSocketInstance()
	{
	}

	Ref<Socket> AsyncTcpSocketInstance::getSocket()
	{
		return m_socket;
	}

	sl_bool AsyncTcpSocketInstance::isSupportingConnect()
	{
		return m_flagSupportingConnect;
	}

	sl_bool AsyncTcpSocketInstance::connect(const SocketAddress& address)
	{
		m_flagRequestConnect = sl_true;
		m_addressRequestConnect = address;
		return sl_true;
	}

	void AsyncTcpSocketInstance::_onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
	{
		Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
		if (object.isNotNull()) {
			object->_onReceive(req, size, flagError);
		}
	}

	void AsyncTcpSocketInstance::_onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
	{
		Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
		if (object.isNotNull()) {
			object->_onSend(req, size, flagError);
		}
	}

	void AsyncTcpSocketInstance::_onConnect(sl_bool flagError)
	{
		Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
		if (object.isNotNull()) {
			object->_onConnect(m_addressRequestConnect, flagError);
		}
	}


	IAsyncTcpSocketListener::IAsyncTcpSocketListener()
	{
	}

	IAsyncTcpSocketListener::~IAsyncTcpSocketListener()
	{
	}

	void IAsyncTcpSocketListener::onError(AsyncTcpSocket* socketListen)
	{
	}

	AsyncTcpSocketParam::AsyncTcpSocketParam()
	{
		flagIPv6 = sl_false;
		
		flagLogError = sl_true;
	}

	AsyncTcpSocketParam::~AsyncTcpSocketParam()
	{
	}


	SLIB_DEFINE_OBJECT(AsyncTcpSocket, AsyncStreamBase)

	AsyncTcpSocket::AsyncTcpSocket()
	{
	}

	AsyncTcpSocket::~AsyncTcpSocket()
	{
	}

	Ref<AsyncTcpSocket> AsyncTcpSocket::create(const AsyncTcpSocketParam& param)
	{
		Ref<Socket> socket = param.socket;
		if (socket.isNull()) {
			sl_bool flagIPv6 = param.flagIPv6;
			if (param.bindAddress.ip.isIPv6()) {
				flagIPv6 = sl_true;
			}
			if (flagIPv6) {
				socket = Socket::openTcp_IPv6();
			} else {
				socket = Socket::openTcp();
			}
			if (socket.isNull()) {
				return sl_null;
			}
			if (param.bindAddress.ip.isNotNone() || param.bindAddress.port != 0) {
				if (!(socket->bind(param.bindAddress))) {
					if (param.flagLogError) {
						LogError(TAG, "AsyncTcpSocket bind error: %s, %s", param.bindAddress.toString(), socket->getLastErrorMessage());
					}
					return sl_null;
				}
			}
		}

		Ref<AsyncTcpSocketInstance> instance = _createInstance(socket);
		if (instance.isNotNull()) {
			Ref<AsyncIoLoop> loop = param.ioLoop;
			if (loop.isNull()) {
				loop = AsyncIoLoop::getDefault();
				if (loop.isNull()) {
					return sl_null;
				}
			}
			Ref<AsyncTcpSocket> ret = new AsyncTcpSocket;
			if (ret.isNotNull()) {
				if (ret->_initialize(instance.get(), AsyncIoMode::InOut, loop)) {
					ret->m_listener = param.listener;
					ret->m_onConnect = param.onConnect;
					ret->m_onError = param.onError;
					if (param.connectAddress.isValid()) {
						if (!(ret->connect(param.connectAddress))) {
							if (param.flagLogError) {
								LogError(TAG, "AsyncTcpSocket connect error: %s, %s", param.connectAddress.toString(), socket->getLastErrorMessage());
							}
							return sl_null;
						}
					}
					return ret;
				}
			}
		}

		return sl_null;
	}

	Ref<Socket> AsyncTcpSocket::getSocket()
	{
		Ref<AsyncTcpSocketInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			return instance->getSocket();
		}
		return sl_null;
	}

	sl_bool AsyncTcpSocket::connect(const SocketAddress& _address)
	{
		Ref<AsyncIoLoop> loop = getIoLoop();
		if (loop.isNull()) {
			return sl_false;
		}
		SocketAddress address = _address;
		if (address.isInvalid()) {
			return sl_false;
		}
		Ref<AsyncTcpSocketInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			Ref<Socket> socket = instance->getSocket();
			if (socket.isNotNull()) {
				if (instance->isSupportingConnect()) {
					SocketType type = socket->getType();
					SocketAddress addr = address;
					if (addr.ip.isIPv4() && type == SocketType::Stream) {
					} else if ((addr.ip.isIPv4() || addr.ip.isIPv6()) && type == SocketType::StreamIPv6) {
						if (addr.ip.isIPv4()) {
							addr.ip = IPv6Address(addr.ip.getIPv4());
						}
					} else {
						return sl_false;
					}
					if (instance->connect(addr)) {
						loop->requestOrder(instance.get());
						return sl_true;
					}
				} else {
					if (socket->connectAndWait(address)) {
						_onConnect(address, sl_false);
						return sl_true;
					} else {
						_onConnect(address, sl_true);
					}
				}
			}
		}
		return sl_false;
	}

	sl_bool AsyncTcpSocket::receive(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* userObject)
	{
		return AsyncStreamBase::read(data, size, callback, userObject);
	}

	sl_bool AsyncTcpSocket::receive(const Memory& mem, const Function<void(AsyncStreamResult*)>& callback)
	{
		return AsyncStreamBase::read(mem.getData(), (sl_uint32)(mem.getSize()), callback, mem.ref.get());
	}

	sl_bool AsyncTcpSocket::send(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* userObject)
	{
		return AsyncStreamBase::write(data, size, callback, userObject);
	}

	sl_bool AsyncTcpSocket::send(const Memory& mem, const Function<void(AsyncStreamResult*)>& callback)
	{
		return AsyncStreamBase::write(mem.getData(), (sl_uint32)(mem.getSize()), callback, mem.ref.get());
	}

	Ref<AsyncTcpSocketInstance> AsyncTcpSocket::_getIoInstance()
	{
		return Ref<AsyncTcpSocketInstance>::from(AsyncStreamBase::getIoInstance());
	}

	void AsyncTcpSocket::_onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
	{
		req->runCallback(this, size, flagError);
		if (flagError) {
			_onError();
		}
	}

	void AsyncTcpSocket::_onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
	{
		req->runCallback(this, size, flagError);
		if (flagError) {
			_onError();
		}
	}

	void AsyncTcpSocket::_onConnect(const SocketAddress& address, sl_bool flagError)
	{
		PtrLocker<IAsyncTcpSocketListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onConnect(this, address, flagError);
		}
		m_onConnect(this, address, flagError);
	}

	void AsyncTcpSocket::_onError()
	{
		PtrLocker<IAsyncTcpSocketListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onError(this);
		}
		m_onError(this);
	}


/*******************************************
			AsyncTcpServer
********************************************/

	AsyncTcpServerInstance::AsyncTcpServerInstance()
	{
		m_flagRunning = sl_false;
	}

	AsyncTcpServerInstance::~AsyncTcpServerInstance()
	{
	}

	void AsyncTcpServerInstance::close()
	{
		m_flagRunning = sl_false;
	}

	void AsyncTcpServerInstance::start()
	{
		ObjectLocker lock(this);
		if (m_flagRunning) {
			return;
		}
		m_flagRunning = sl_true;
		requestOrder();
	}

	sl_bool AsyncTcpServerInstance::isRunning()
	{
		return m_flagRunning;
	}

	Ref<Socket> AsyncTcpServerInstance::getSocket()
	{
		return m_socket;
	}

	void AsyncTcpServerInstance::_onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address)
	{
		Ref<AsyncTcpServer> server = Ref<AsyncTcpServer>::from(getObject());
		if (server.isNotNull()) {
			server->_onAccept(socketAccept, address);
		}
	}

	void AsyncTcpServerInstance::_onError()
	{
		Ref<AsyncTcpServer> server = Ref<AsyncTcpServer>::from(getObject());
		if (server.isNotNull()) {
			server->_onError();
		}
	}


	IAsyncTcpServerListener::IAsyncTcpServerListener()
	{
	}

	IAsyncTcpServerListener::~IAsyncTcpServerListener()
	{
	}

	void IAsyncTcpServerListener::onError(AsyncTcpServer* socketListen)
	{
	}

	AsyncTcpServerParam::AsyncTcpServerParam()
	{
		flagIPv6 = sl_false;
		
		flagAutoStart = sl_true;
		flagLogError = sl_true;
	}

	AsyncTcpServerParam::~AsyncTcpServerParam()
	{
	}

	SLIB_DEFINE_OBJECT(AsyncTcpServer, AsyncIoObject)

	AsyncTcpServer::AsyncTcpServer()
	{
	}

	AsyncTcpServer::~AsyncTcpServer()
	{
	}

	Ref<AsyncTcpServer> AsyncTcpServer::create(const AsyncTcpServerParam& param)
	{
		Ref<Socket> socket = param.socket;
		if (socket.isNull()) {
			if (param.bindAddress.port == 0) {
				return sl_null;
			}
			sl_bool flagIPv6 = param.flagIPv6;
			if (param.bindAddress.ip.isIPv6()) {
				flagIPv6 = sl_true;
			}
			if (flagIPv6) {
				socket = Socket::openTcp_IPv6();
			} else {
				socket = Socket::openTcp();
			}
			if (socket.isNull()) {
				return sl_null;
			}
			
#if defined(SLIB_PLATFORM_IS_UNIX)
			/*
			 * SO_REUSEADDR option allows the server applications to listen on the port that is still
			 * bound by some TIME_WAIT sockets.
			 *
			 * http://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t
			 */
			socket->setOption_ReuseAddress(sl_true);
#endif

			if (!(socket->bind(param.bindAddress))) {
				if (param.flagLogError) {
					LogError(TAG, "AsyncTcpServer bind error: %s, %s", param.bindAddress.toString(), socket->getLastErrorMessage());
				}
				return sl_null;
			}
		}
		
		if (socket->listen()) {
			Ref<AsyncTcpServerInstance> instance = _createInstance(socket);
			if (instance.isNotNull()) {
				Ref<AsyncIoLoop> loop = param.ioLoop;
				if (loop.isNull()) {
					loop = AsyncIoLoop::getDefault();
					if (loop.isNull()) {
						return sl_null;
					}
				}
				Ref<AsyncTcpServer> ret = new AsyncTcpServer;
				if (ret.isNotNull()) {
					ret->m_listener = param.listener;
					ret->m_onAccept = param.onAccept;
					ret->m_onError = param.onError;
					instance->setObject(ret.get());
					ret->setIoInstance(instance.get());
					ret->setIoLoop(loop);
					if (loop->attachInstance(instance.get(), AsyncIoMode::In)) {
						if (param.flagAutoStart) {
							instance->start();
						}
						return ret;
					}
				}
			}
		} else {
			if (param.flagLogError) {
				LogError(TAG, "AsyncTcpServer listen error: %s, %s", param.bindAddress.toString(), socket->getLastErrorMessage());
			}
		}
		return sl_null;
	}


	void AsyncTcpServer::close()
	{
		closeIoInstance();
	}

	sl_bool AsyncTcpServer::isOpened()
	{
		return getIoInstance().isNotNull();
	}

	void AsyncTcpServer::start()
	{
		Ref<AsyncTcpServerInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			instance->start();
		}
	}

	sl_bool AsyncTcpServer::isRunning()
	{
		Ref<AsyncTcpServerInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			return instance->isRunning();
		}
		return sl_false;
	}

	Ref<Socket> AsyncTcpServer::getSocket()
	{
		Ref<AsyncTcpServerInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			return instance->getSocket();
		}
		return sl_null;
	}

	Ref<AsyncTcpServerInstance> AsyncTcpServer::_getIoInstance()
	{
		return Ref<AsyncTcpServerInstance>::from(AsyncIoObject::getIoInstance());
	}

	void AsyncTcpServer::_onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address)
	{
		PtrLocker<IAsyncTcpServerListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onAccept(this, socketAccept, address);
		}
		m_onAccept(this, socketAccept.get(), address);
	}

	void AsyncTcpServer::_onError()
	{
		PtrLocker<IAsyncTcpServerListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onError(this);
		}
		m_onError(this);
	}


/*******************************************
			AsyncUdpSocket
********************************************/

	AsyncUdpSocketInstance::AsyncUdpSocketInstance()
	{
		m_flagRunning = sl_false;
	}

	AsyncUdpSocketInstance::~AsyncUdpSocketInstance()
	{
	}

	void AsyncUdpSocketInstance::close()
	{
		m_flagRunning = sl_false;
	}

	void AsyncUdpSocketInstance::start()
	{
		ObjectLocker lock(this);
		if (m_flagRunning) {
			return;
		}
		m_flagRunning = sl_true;
		requestOrder();
	}

	sl_bool AsyncUdpSocketInstance::isRunning()
	{
		return m_flagRunning;
	}

	Ref<Socket> AsyncUdpSocketInstance::getSocket()
	{
		return m_socket;
	}

#define UDP_QUEUE_MAX_SIZE 1024000

	sl_bool AsyncUdpSocketInstance::sendTo(const SocketAddress& addressTo, const Memory& data)
	{
		if (isOpened()) {
			if (data.isNotNull()) {
				SendRequest request;
				request.addressTo = addressTo;
				request.data = data;
				if (m_queueSendRequests.getCount() < UDP_QUEUE_MAX_SIZE) {
					if (m_queueSendRequests.push(request)) {
						return sl_true;
					}
				}
			}
		}
		return sl_false;
	}

	void AsyncUdpSocketInstance::_onReceive(const SocketAddress& address, sl_uint32 size)
	{
		Ref<AsyncUdpSocket> object = Ref<AsyncUdpSocket>::from(getObject());
		if (object.isNotNull()) {
			object->_onReceive(address, m_buffer.getData(), size);
		}
	}


	IAsyncUdpSocketListener::IAsyncUdpSocketListener()
	{
	}

	IAsyncUdpSocketListener::~IAsyncUdpSocketListener()
	{
	}

	AsyncUdpSocketParam::AsyncUdpSocketParam()
	{
		flagIPv6 = sl_false;
		flagBroadcast = sl_false;
		flagAutoStart = sl_false;
		flagLogError = sl_false;
		packetSize = 65536;
	}

	AsyncUdpSocketParam::~AsyncUdpSocketParam()
	{
	}


	SLIB_DEFINE_OBJECT(AsyncUdpSocket, AsyncIoObject)

	AsyncUdpSocket::AsyncUdpSocket()
	{
	}

	AsyncUdpSocket::~AsyncUdpSocket()
	{
	}

	Ref<AsyncUdpSocket> AsyncUdpSocket::create(const AsyncUdpSocketParam& param)
	{
		if (param.packetSize < 1) {
			return sl_null;
		}
		
		Ref<Socket> socket = param.socket;
		if (socket.isNull()) {
			sl_bool flagIPv6 = param.flagIPv6;
			if (param.bindAddress.ip.isIPv6()) {
				flagIPv6 = sl_true;
			}
			if (flagIPv6) {
				socket = Socket::openUdp_IPv6();
			} else {
				socket = Socket::openUdp();
			}
			if (socket.isNull()) {
				return sl_null;
			}
#if defined(SLIB_PLATFORM_IS_UNIX)
			/*
			 * SO_REUSEADDR option allows the server applications to listen on the port that is still
			 * bound by some TIME_WAIT sockets.
			 *
			 * http://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t
			 */
			socket->setOption_ReuseAddress(sl_true);
#endif
			if (param.bindAddress.ip.isNotNone() || param.bindAddress.port != 0) {
				if (!(socket->bind(param.bindAddress))) {
					if (param.flagLogError) {
						LogError(TAG, "AsyncTcpSocket bind error: %s, %s", param.bindAddress.toString(), socket->getLastErrorMessage());
					}
					return sl_null;
				}
			}
		}
		if (param.flagBroadcast) {
			socket->setOption_Broadcast(sl_true);
		}
		
		Ref<AsyncUdpSocketInstance> instance = _createInstance(socket, param.packetSize);
		if (instance.isNotNull()) {
			Ref<AsyncIoLoop> loop = param.ioLoop;
			if (loop.isNull()) {
				loop = AsyncIoLoop::getDefault();
				if (loop.isNull()) {
					return sl_null;
				}
			}
			Ref<AsyncUdpSocket> ret = new AsyncUdpSocket;
			if (ret.isNotNull()) {
				ret->m_listener = param.listener;
				ret->m_onReceiveFrom = param.onReceiveFrom;
				instance->setObject(ret.get());
				ret->setIoInstance(instance.get());
				ret->setIoLoop(loop);
				if (loop->attachInstance(instance.get(), AsyncIoMode::In)) {
					if (param.flagAutoStart) {
						ret->start();
					}
					return ret;
				}
			}
		}
		
		return sl_null;
	}

	void AsyncUdpSocket::close()
	{
		closeIoInstance();
	}

	sl_bool AsyncUdpSocket::isOpened()
	{
		return getIoInstance().isNotNull();
	}

	void AsyncUdpSocket::start()
	{
		Ref<AsyncUdpSocketInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			instance->start();
		}
	}

	sl_bool AsyncUdpSocket::isRunning()
	{
		Ref<AsyncUdpSocketInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			return instance->isRunning();
		}
		return sl_false;
	}

	Ref<Socket> AsyncUdpSocket::getSocket()
	{
		Ref<AsyncUdpSocketInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			return instance->getSocket();
		}
		return sl_null;
	}

	void AsyncUdpSocket::setBroadcast(sl_bool flag)
	{
		Ref<Socket> socket = getSocket();
		if (socket.isNotNull()) {
			socket->setOption_Broadcast(flag);
		}
	}

	void AsyncUdpSocket::setSendBufferSize(sl_uint32 size)
	{
		Ref<Socket> socket = getSocket();
		if (socket.isNotNull()) {
			socket->setOption_SendBufferSize(size);
		}
	}

	void AsyncUdpSocket::setReceiveBufferSize(sl_uint32 size)
	{
		Ref<Socket> socket = getSocket();
		if (socket.isNotNull()) {
			socket->setOption_ReceiveBufferSize(size);
		}
	}

	sl_bool AsyncUdpSocket::sendTo(const SocketAddress& addressTo, const void* data, sl_uint32 size)
	{
		return sendTo(addressTo, Memory::create(data, size));
	}

	sl_bool AsyncUdpSocket::sendTo(const SocketAddress& addressTo, const Memory& mem)
	{
		Ref<AsyncIoLoop> loop = getIoLoop();
		if (loop.isNull()) {
			return sl_false;
		}
		Ref<AsyncUdpSocketInstance> instance = _getIoInstance();
		if (instance.isNotNull()) {
			if (instance->sendTo(addressTo, mem)) {
				loop->requestOrder(instance.get());
				return sl_true;
			}
		}
		return sl_false;
	}

	Ref<AsyncUdpSocketInstance> AsyncUdpSocket::_getIoInstance()
	{
		return Ref<AsyncUdpSocketInstance>::from(AsyncIoObject::getIoInstance());
	}

	void AsyncUdpSocket::_onReceive(const SocketAddress& address, void* data, sl_uint32 sizeReceived)
	{
		PtrLocker<IAsyncUdpSocketListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onReceiveFrom(this, address, data, sizeReceived);
		}
		m_onReceiveFrom(this, address, data, sizeReceived);
	}

}
