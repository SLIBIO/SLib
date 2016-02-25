#include "network_async_config.h"
#include "../../../inc/slib/network/async.h"
#include "../../../inc/slib/core/log.h"

SLIB_NETWORK_NAMESPACE_BEGIN

/*******************************************
	AsyncSocket
********************************************/
void IAsyncTcpSocketListener::onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError)
{
}

void IAsyncTcpSocketListener::onReceive(AsyncTcpSocket* socket, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
}

void IAsyncTcpSocketListener::onSend(AsyncTcpSocket* socket, void* data, sl_uint32 sizeSent, const Referable* ref, sl_bool flagError)
{
}

void IAsyncTcpSocketListener::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
	onReceive((AsyncTcpSocket*)stream, data, sizeRead, ref, flagError);
}

void IAsyncTcpSocketListener::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError)
{
	onSend((AsyncTcpSocket*)stream, data, sizeWritten, ref, flagError);
}

AsyncTcpSocketInstance::AsyncTcpSocketInstance()
{
	m_flagRequestConnect = sl_false;
	m_flagSupportingConnect = sl_true;
}

Ref<Socket> AsyncTcpSocketInstance::getSocket()
{
	return m_socket;
}

sl_bool AsyncTcpSocketInstance::isSupportingConnect()
{
	return m_flagSupportingConnect;
}

sl_bool AsyncTcpSocketInstance::connect(const SocketAddress& address, const Ptr<IAsyncTcpSocketListener>& listener)
{
	m_flagRequestConnect = sl_true;
	m_addressRequestConnect = address;
	m_listenerConnect = listener;
	return sl_true;
}

sl_bool AsyncTcpSocketInstance::receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData)
{
	if (isOpened()) {
		if (size > 0) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, refData, listener);
			if (req.isNotNull()) {
				if (m_requestsRead.push(req)) {
					return sl_true;
				}
			}
		}
	}
	return sl_false;
}

sl_bool AsyncTcpSocketInstance::send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData)
{
	if (isOpened()) {
		if (size > 0) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, refData, listener);
			if (req.isNotNull()) {
				if (m_requestsWrite.push(req)) {
					return sl_true;
				}
			}
		}
	}
	return sl_false;
}

void AsyncTcpSocketInstance::_onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
{
	Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	PtrLocker<IAsyncStreamListener> listener(req->getListener());
	if (listener.isNotNull()) {
		listener->onRead(object.get(), req->data(), size, req->getDataReference(), flagError);
	}
}

void AsyncTcpSocketInstance::_onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
{
	Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	if (!flagError && req->size() != size) {
		flagError = sl_true;
	}
	PtrLocker<IAsyncStreamListener> listener(req->getListener());
	if (listener.isNotNull()) {
		listener->onWrite(object.get(), req->data(), size, req->getDataReference(), flagError);
	}
}

void AsyncTcpSocketInstance::_onConnect(sl_bool flagError)
{
	Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	PtrLocker<IAsyncTcpSocketListener> listener(m_listenerConnect);
	if (listener.isNotNull()) {
		listener->onConnect(object.get(), m_addressRequestConnect, flagError);
	}
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<Socket>& socket)
{
	return AsyncTcpSocket::create(socket, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const SocketAddress& _addressBind, const Ref<AsyncIoLoop>& loop)
{
	SocketAddress addressBind = _addressBind;
	Ref<Socket> socket;
	if (addressBind.ip.isIPv6()) {
		socket = Socket::openTcp_IPv6();
	} else {
		socket = Socket::openTcp();
	}
	if (socket.isNotNull()) {
		if (addressBind.ip.isNotNone() || addressBind.port != 0) {
			if (!(socket->bind(addressBind))) {
				return Ref<AsyncTcpSocket>::null();
			}
		}
		return AsyncTcpSocket::create(socket, loop);
	}
	return Ref<AsyncTcpSocket>::null();
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const SocketAddress& addressBind)
{
	return AsyncTcpSocket::create(addressBind, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(sl_uint32 port, const Ref<AsyncIoLoop>& loop)
{
	return AsyncTcpSocket::create(SocketAddress(IPv4Address::any(), port), loop);
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(sl_uint32 port)
{
	return AsyncTcpSocket::create(port, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createIPv6(sl_uint32 port, const Ref<AsyncIoLoop>& loop)
{
	return AsyncTcpSocket::create(SocketAddress(IPv6Address::any(), port), loop);
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createIPv6(sl_uint32 port)
{
	return AsyncTcpSocket::createIPv6(port, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<AsyncIoLoop>& loop)
{
	return AsyncTcpSocket::create(SocketAddress::none(), loop);
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create()
{
	return AsyncTcpSocket::create(AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createIPv6(const Ref<AsyncIoLoop>& loop)
{
	return AsyncTcpSocket::create(SocketAddress(IPv6Address::any(), 0), loop);
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createIPv6()
{
	return AsyncTcpSocket::createIPv6(AsyncIoLoop::getDefault());
}


Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncIoLoop>& loop)
{
	Ref<AsyncTcpSocket> async = create(addressBind, loop);
	if (async.isNotNull()) {
		if (async->connect(addressConnect, listener)) {
			return async;
		}
	}
	return Ref<AsyncTcpSocket>::null();
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener)
{
	return AsyncTcpSocket::createAndConnect(addressBind, addressConnect, listener, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncIoLoop>& loop)
{
	if (addressConnect.ip.isIPv6()) {
		return AsyncTcpSocket::createAndConnect(SocketAddress(IPv6Address::any(), 0), addressConnect, listener, loop);
	} else {
		return AsyncTcpSocket::createAndConnect(SocketAddress::none(), addressConnect, listener, loop);
	}
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener)
{
	return AsyncTcpSocket::createAndConnect(addressConnect, listener, AsyncIoLoop::getDefault());
}

Ref<Socket> AsyncTcpSocket::getSocket()
{
	Ref<AsyncTcpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->getSocket();
	}
	return Ref<Socket>::null();
}

sl_bool AsyncTcpSocket::connect(const SocketAddress& _address, const Ptr<IAsyncTcpSocketListener>& listener)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	SocketAddress address = _address;
	if (address.isInvalid()) {
		return sl_false;
	}
	Ref<AsyncTcpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		Ref<Socket> socket = instance->getSocket();
		if (socket.isNotNull()) {
			if (instance->isSupportingConnect()) {
				SocketType type = socket->getType();
				SocketAddress addr = address;
				if (addr.ip.isIPv4() && type == socketType_Tcp) {
				} else if ((addr.ip.isIPv4() || addr.ip.isIPv6()) && type == socketType_TcpIPv6) {
					if (addr.ip.isIPv4()) {
						addr.ip = IPv6Address(addr.ip.getIPv4());
					}
				} else {
					return sl_false;
				}
				if (instance->connect(addr, listener)) {
					loop->requestOrder(instance.get());
					return sl_true;
				}
			} else {
				if (socket->connectAndWait(address)) {
					if (listener.isNotNull()) {
						listener->onConnect(this, address, sl_false);
					}
					return sl_true;
				} else {
					if (listener.isNotNull()) {
						listener->onConnect(this, address, sl_true);
					}
				}
			}
		}
	}
	return sl_false;
}

sl_bool AsyncTcpSocket::receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncTcpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		if (instance->receive(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncTcpSocket::receive(const Memory& mem, const Ptr<IAsyncTcpSocketListener>& listener)
{
	return receive(mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
}

sl_bool AsyncTcpSocket::send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncTcpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		if (instance->send(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncTcpSocket::send(const Memory& mem, const Ptr<IAsyncTcpSocketListener>& listener)
{
	return send(mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
}

Ref<AsyncTcpSocketInstance> AsyncTcpSocket::getIoInstance()
{
	return Ref<AsyncTcpSocketInstance>::from(AsyncStreamBase::getIoInstance());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(AsyncTcpSocketInstance* instance, const Ref<AsyncIoLoop>& loop)
{
	if (instance) {
		Ref<AsyncTcpSocket> ret = new AsyncTcpSocket;
		if (ret.isNotNull()) {
			if (ret->_initialize(instance, asyncIoMode_InOut, loop)) {
				return ret;
			}
		}
	}
	return Ref<AsyncTcpSocket>::null();
}

/*******************************************
	AsyncTcpServer
********************************************/
void IAsyncTcpServerListener::onError(AsyncTcpServer* socketListen)
{
}

AsyncTcpServerInstance::AsyncTcpServerInstance()
{
	m_flagRunning = sl_false;
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

Ptr<IAsyncTcpServerListener> AsyncTcpServerInstance::getListener()
{
	return m_listener;
}

void AsyncTcpServerInstance::_onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address)
{
	Ref<AsyncTcpServer> server = Ref<AsyncTcpServer>::from(getObject());
	if (server.isNull()) {
		return;
	}
	PtrLocker<IAsyncTcpServerListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onAccept(server.get(), socketAccept, address);
	}
}

void AsyncTcpServerInstance::_onError()
{
	Ref<AsyncTcpServer> server = Ref<AsyncTcpServer>::from(getObject());
	if (server.isNull()) {
		return;
	}
	PtrLocker<IAsyncTcpServerListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onError(server.get());
	}
}

Ref<AsyncTcpServer> AsyncTcpServer::create(const Ref<Socket>& socket, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart)
{
	return AsyncTcpServer::create(socket, listener, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncTcpServer> AsyncTcpServer::create(const SocketAddress& _addressListen, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	SocketAddress addressListen = _addressListen;
	Ref<Socket> socket;
	if (addressListen.ip.isIPv6()) {
		socket = Socket::openTcp_IPv6();
	} else {
		socket = Socket::openTcp();
	}
	if (socket.isNotNull()) {
		
		// Sometimes, the previously bound port is remaining used state even after exit on Unix system.
		// So, we set ReuseAddress flag on Server sockets to avoid this issue
		socket->setOption_ReuseAddress(sl_true);
		
		if (socket->bind(addressListen)) {
			if (socket->listen()) {
				return AsyncTcpServer::create(socket, listener, loop, flagAutoStart);
			} else {
				SLIB_STATIC_STRING(s, "Can not listen on address - ");
				LOG_ERROR(s + addressListen.toString());
			}
		} else {
			SLIB_STATIC_STRING(s, "Can not bind to address - ");
			LOG_ERROR(s + addressListen.toString());
		}
	}
	return Ref<AsyncTcpServer>::null();
}

Ref<AsyncTcpServer> AsyncTcpServer::create(const SocketAddress& addressListen, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart)
{
	return AsyncTcpServer::create(addressListen, listener, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncTcpServer> AsyncTcpServer::create(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	SocketAddress address;
	address.ip = IPv4Address::getAny();
	address.port = portListen;
	return AsyncTcpServer::create(address, listener, loop, flagAutoStart);
}

Ref<AsyncTcpServer> AsyncTcpServer::create(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart)
{
	return AsyncTcpServer::create(portListen, listener, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncTcpServer> AsyncTcpServer::createIPv6(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	SocketAddress address;
	address.ip = IPv6Address::getAny();
	address.port = portListen;
	return AsyncTcpServer::create(address, listener, loop, flagAutoStart);
}

Ref<AsyncTcpServer> AsyncTcpServer::createIPv6(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart)
{
	return AsyncTcpServer::createIPv6(portListen, listener, AsyncIoLoop::getDefault(), flagAutoStart);
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
	Ref<AsyncTcpServerInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		instance->start();
	}
}

sl_bool AsyncTcpServer::isRunning()
{
	Ref<AsyncTcpServerInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->isRunning();
	}
	return sl_false;
}

Ref<Socket> AsyncTcpServer::getSocket()
{
	Ref<AsyncTcpServerInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->getSocket();
	}
	return Ref<Socket>::null();
}

Ref<AsyncTcpServerInstance> AsyncTcpServer::getIoInstance()
{
	return Ref<AsyncTcpServerInstance>::from(AsyncIoObject::getIoInstance());
}

Ref<AsyncTcpServer> AsyncTcpServer::create(AsyncTcpServerInstance* instance, const Ref<AsyncIoLoop>& _loop, sl_bool flagAutoStart)
{
	if (instance) {
		Ref<AsyncIoLoop> loop = _loop;
		if (loop.isNull()) {
			loop = AsyncIoLoop::getDefault();
			if (loop.isNull()) {
				return Ref<AsyncTcpServer>::null();
			}
		}
		Ref<AsyncTcpServer> ret = new AsyncTcpServer;
		if (ret.isNotNull()) {
			instance->setObject(ret.get());
			ret->setIoInstance(instance);
			ret->setIoLoop(loop);
			if (loop->attachInstance(instance, asyncIoMode_In)) {
				if (flagAutoStart) {
					instance->start();
				}
				return ret;
			}
		}
	}
	return Ref<AsyncTcpServer>::null();
}

/*******************************************
	AsyncUdpSocket
********************************************/
AsyncUdpSocketInstance::AsyncUdpSocketInstance()
{
	m_flagRunning = sl_false;
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

Ptr<IAsyncUdpSocketListener> AsyncUdpSocketInstance::getListener()
{
	return m_listener;
}

#define UDP_QUEUE_MAX_SIZE 1024000

sl_bool AsyncUdpSocketInstance::sendTo(const SocketAddress& addressTo, const Memory& data)
{
	if (isOpened()) {
		if (data.isNotEmpty()) {
			SendRequest request;
			request.addressTo = addressTo;
			request.data = data;
			if (m_queueSendRequests.count() < UDP_QUEUE_MAX_SIZE) {
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
	if (object.isNull()) {
		return;
	}
	PtrLocker<IAsyncUdpSocketListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onReceiveFrom(object.get(), address, m_buffer.getBuf(), size);
	}
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ref<Socket>& socket, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(socket, listener, packetSize, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const SocketAddress& addressBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	if (packetSize > 0) {
		Ref<Socket> socket;
		if (addressBind.ip.isIPv6()) {
			socket = Socket::openUdp_IPv6();
		} else {
			socket = Socket::openUdp();
		}
		if (socket.isNotNull()) {
			if (addressBind.ip.isNotNone() || addressBind.port != 0) {
				if (!(socket->bind(addressBind))) {
					return Ref<AsyncUdpSocket>::null();
				}
			}
			return AsyncUdpSocket::create(socket, listener, packetSize, loop, flagAutoStart);
		}
	}
	return Ref<AsyncUdpSocket>::null();
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const SocketAddress& addressBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(addressBind, listener, packetSize, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(SocketAddress(IPv4Address::any(), portBind), listener, packetSize, loop, flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(portBind, listener, packetSize, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::createIPv6(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(SocketAddress(IPv6Address::any(), portBind), listener, packetSize, loop, flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::createIPv6(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::createIPv6(portBind, listener, packetSize, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(SocketAddress::none(), listener, packetSize, loop, flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(listener, packetSize, AsyncIoLoop::getDefault(), flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::createIPv6(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::create(SocketAddress(IPv6Address::any(), 0), listener, packetSize, loop, flagAutoStart);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::createIPv6(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart)
{
	return AsyncUdpSocket::createIPv6(listener, packetSize, AsyncIoLoop::getDefault(), flagAutoStart);
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
	Ref<AsyncUdpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		instance->start();
	}
}

sl_bool AsyncUdpSocket::isRunning()
{
	Ref<AsyncUdpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->isRunning();
	}
	return sl_false;
}

Ref<Socket> AsyncUdpSocket::getSocket()
{
	Ref<AsyncUdpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->getSocket();
	}
	return Ref<Socket>::null();
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

sl_bool AsyncUdpSocket::sendTo(const SocketAddress& addressTo, void* data, sl_uint32 size)
{
	return sendTo(addressTo, Memory::create(data, size));
}

sl_bool AsyncUdpSocket::sendTo(const SocketAddress& addressTo, const Memory& mem)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncUdpSocketInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		if (instance->sendTo(addressTo, mem)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

Ref<AsyncUdpSocketInstance> AsyncUdpSocket::getIoInstance()
{
	return Ref<AsyncUdpSocketInstance>::from(AsyncIoObject::getIoInstance());
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(AsyncUdpSocketInstance* instance, const Ref<AsyncIoLoop>& _loop, sl_bool flagAutoStart)
{
	if (instance) {
		Ref<AsyncIoLoop> loop = _loop;
		if (loop.isNull()) {
			loop = AsyncIoLoop::getDefault();
			if (loop.isNull()) {
				return Ref<AsyncUdpSocket>::null();
			}
		}
		Ref<AsyncUdpSocket> ret = new AsyncUdpSocket;
		if (ret.isNotNull()) {
			instance->setObject(ret.get());
			ret->setIoInstance(instance);
			ret->setIoLoop(loop);
			if (loop->attachInstance(instance, asyncIoMode_In)) {
				if (flagAutoStart) {
					ret->start();
				}
				return ret;
			}
		}
	}
	return Ref<AsyncUdpSocket>::null();
}

SLIB_NETWORK_NAMESPACE_END
