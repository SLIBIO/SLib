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

void IAsyncTcpSocketListener::onReceive(AsyncTcpSocket* socket, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError)
{
}

void IAsyncTcpSocketListener::onSend(AsyncTcpSocket* socket, void* data, sl_uint32 sizeSent, Referable* ref, sl_bool flagError)
{
}

void IAsyncTcpSocketListener::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError)
{
	onReceive((AsyncTcpSocket*)stream, data, sizeRead, ref, flagError);
}

void IAsyncTcpSocketListener::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError)
{
	onSend((AsyncTcpSocket*)stream, data, sizeWritten, ref, flagError);
}

AsyncTcpSocketInstance::AsyncTcpSocketInstance()
{
	m_flagRequestConnect = sl_false;
	m_flagSupportingConnect = sl_true;
}

AsyncTcpSocketInstance::~AsyncTcpSocketInstance()
{
}

sl_bool AsyncTcpSocketInstance::connect(const SocketAddress& address, const Ptr<IAsyncTcpSocketListener>& listener)
{
	m_flagRequestConnect = sl_true;
	m_addressRequestConnect = address;
	m_listenerConnect = listener;
	return sl_true;
}

sl_bool AsyncTcpSocketInstance::receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData)
{
	if (size == 0) {
		return sl_false;
	}
	Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, refData, listener);
	if (req.isNotNull()) {
		m_requestsRead.push(req);
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncTcpSocketInstance::send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData)
{
	if (size == 0) {
		return sl_false;
	}
	Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, refData, listener);
	if (req.isNotNull()) {
		m_requestsWrite.push(req);
		return sl_true;
	}
	return sl_false;
}

void AsyncTcpSocketInstance::_onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
{
	Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	PtrLocker<IAsyncStreamListener> listener(req->listener);
	if (listener.isNotNull()) {
		listener->onRead(object.get(), req->data, size, req->refData.get(), flagError);
	}
}

void AsyncTcpSocketInstance::_onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
{
	Ref<AsyncTcpSocket> object = Ref<AsyncTcpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	if (!flagError && req->size != size) {
		flagError = sl_true;
	}
	PtrLocker<IAsyncStreamListener> listener(req->listener);
	if (listener.isNotNull()) {
		listener->onWrite(object.get(), req->data, size, req->refData.get(), flagError);
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

AsyncTcpSocket::AsyncTcpSocket()
{
}

AsyncTcpSocket::~AsyncTcpSocket()
{
}

Ref<Socket> AsyncTcpSocket::getSocket()
{
	Ref<AsyncTcpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		return instance->getSocket();
	}
	return Ref<Socket>::null();
}

sl_bool AsyncTcpSocket::connect(const SocketAddress& _address, const Ptr<IAsyncTcpSocketListener>& listener)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	SocketAddress address = _address;
	if (address.isInvalid()) {
		return sl_false;
	}
	Ref<AsyncTcpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		Ref<Socket> socket = instance->getSocket();
		if (socket.isNotNull()) {
			if (instance->isSupportingConnect()) {
				Socket::Type type = socket->getType();
				SocketAddress addr = address;
				if (addr.ip.isIPv4() && type == Socket::typeTcp_IPv4) {
				} else if ((addr.ip.isIPv4() || addr.ip.isIPv6()) && type == Socket::typeTcp_IPv6) {
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
				Ref<AsyncLoop> loop = getLoop();
				if (loop.isNotNull()) {
					loop->addTask(SLIB_CALLBACK_WEAKREF(AsyncTcpSocket, _connectCallback, this, address, listener));
				}
			}	
		}
	}
	return sl_false;
}

void AsyncTcpSocket::_connectCallback(SocketAddress address, Ptr<IAsyncTcpSocketListener> _listener)
{
	Ref<Socket> socket = getSocket();
	if (socket.isNotNull()) {
		PtrLocker<IAsyncTcpSocketListener> listener(_listener);
		if (socket->connectAndWait(address)) {
			listener->onConnect(this, address, sl_false);
		} else {
			listener->onConnect(this, address, sl_true);
		}
	}
}

sl_bool AsyncTcpSocket::receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncTcpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		if (instance->receive(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncTcpSocket::send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncTcpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		if (instance->send(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(AsyncTcpSocketInstance* instance, const Ref<AsyncLoop>& loop)
{
	Ref<AsyncTcpSocket> ret;
	if (instance) {
		ret = new AsyncTcpSocket;
		if (ret.isNotNull()) {
			if (!(ret->initialize(instance, loop))) {
				ret.setNull();
			}
		}
	}
	return ret;
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<Socket>& socket)
{
	return AsyncTcpSocket::create(socket, AsyncLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<AsyncLoop>& loop, sl_bool flagIPv6)
{
	Ref<Socket> socket;
	if (flagIPv6) {
		return AsyncTcpSocket::create(SocketAddress(IPv6Address::getAny(), 0), loop);
	} else {
		return AsyncTcpSocket::create(SocketAddress(IPv4Address::getAny(), 0), loop);
	}
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(sl_bool flagIPv6)
{
	return AsyncTcpSocket::create(AsyncLoop::getDefault(), flagIPv6);
}

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const SocketAddress& _addressBind, const Ref<AsyncLoop>& loop)
{
	SocketAddress addressBind = _addressBind;
	Ref<Socket> socket;
	if (addressBind.ip.isIPv6()) {
		socket = Socket::openTcp_IPv6();
	} else {
		socket = Socket::openTcp_IPv4();
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
	return AsyncTcpSocket::create(addressBind, AsyncLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncLoop>& loop)
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
	return AsyncTcpSocket::createAndConnect(addressBind, addressConnect, listener, AsyncLoop::getDefault());
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncLoop>& loop)
{
	return AsyncTcpSocket::createAndConnect(SocketAddress::none(), addressConnect, listener, loop);
}

Ref<AsyncTcpSocket> AsyncTcpSocket::createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener)
{
	return AsyncTcpSocket::createAndConnect(addressConnect, listener, AsyncLoop::getDefault());
}

/*******************************************
	AsyncTcpServer
********************************************/
void IAsyncTcpServerListener::onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address)
{
}

void IAsyncTcpServerListener::onError(AsyncTcpServer* socketListen)
{
}

AsyncTcpServerInstance::AsyncTcpServerInstance()
{
}

AsyncTcpServerInstance::~AsyncTcpServerInstance()
{
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

AsyncTcpServer::AsyncTcpServer()
{
	m_flagStarted = sl_false;
}

AsyncTcpServer::~AsyncTcpServer()
{
}

Ref<Socket> AsyncTcpServer::getSocket()
{
	Ref<AsyncTcpServerInstance> instance = getInstance();
	if (instance.isNotNull()) {
		return instance->getSocket();
	}
	return Ref<Socket>::null();
}

Ptr<IAsyncTcpServerListener> AsyncTcpServer::getListener()
{
	Ref<AsyncTcpServerInstance> instance = getInstance();
	if (instance.isNotNull()) {
		return instance->getListener();
	}
	return Ptr<IAsyncTcpServerListener>::null();
}

void AsyncTcpServer::setListener(const Ptr<IAsyncTcpServerListener>& listener)
{
	Ref<AsyncTcpServerInstance> instance = getInstance();
	if (instance.isNotNull()) {
		instance->setListener(listener);
	}
}

void AsyncTcpServer::close()
{
	closeInstance();
}

sl_bool AsyncTcpServer::isOpened()
{
	return getInstance().isNotNull();
}

void AsyncTcpServer::start()
{
	MutexLocker lock(getLocker());
	if (m_flagStarted) {
		return;
	}
	m_flagStarted = sl_true;
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNotNull()) {
		Ref<AsyncTcpServerInstance> instance = getInstance();
		if (instance.isNotNull()) {
			loop->requestOrder(instance.get());
		}
	}
}

Ref<AsyncTcpServer> AsyncTcpServer::create(AsyncTcpServerInstance* instance, const Ref<AsyncLoop>& _loop)
{
	Ref<AsyncTcpServer> ret;
	if (!instance) {
		return ret;
	}
	Ref<AsyncLoop> loop = _loop;
	if (loop.isNull()) {
		loop = AsyncLoop::getDefault();
		if (loop.isNull()) {
			return ret;
		}
	}
	ret = new AsyncTcpServer;
	if (ret.isNotNull()) {
		instance->setObject(ret.get());
		ret->setInstance(instance);
		ret->setLoop(loop);
		if (!(loop->attachInstance(instance))) {
			
			ret.setNull();
		}
	}
	return ret;
}

Ref<AsyncTcpServer> AsyncTcpServer::create(const Ref<Socket>& socket)
{
	return AsyncTcpServer::create(socket, AsyncLoop::getDefault());
}

Ref<AsyncTcpServer> AsyncTcpServer::create(const SocketAddress& _addressListen, const Ref<AsyncLoop>& loop)
{
	SocketAddress addressListen = _addressListen;
	Ref<Socket> socket;
	if (addressListen.ip.isIPv6()) {
		socket = Socket::openTcp_IPv6();
	} else {
		socket = Socket::openTcp_IPv4();
	}
	if (socket.isNotNull()) {
		
		// Sometimes, the previously bound port is remaining used state even after exit on Unix system.
		// So, we set ReuseAddress flag on Server sockets to avoid this issue
		socket->setOption_ReuseAddress(sl_true);
		
		if (socket->bind(addressListen)) {
			if (socket->listen()) {
				return AsyncTcpServer::create(socket, loop);
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

Ref<AsyncTcpServer> AsyncTcpServer::create(const SocketAddress& addressListen)
{
	return AsyncTcpServer::create(addressListen, AsyncLoop::getDefault());
}

Ref<AsyncTcpServer> AsyncTcpServer::create(sl_uint32 portListen, const Ref<AsyncLoop>& loop, sl_bool flagIPv6)
{
	SocketAddress address;
	address.port = portListen;
	if (flagIPv6) {
		address.ip = IPv6Address::getAny();
	} else {
		address.ip = IPv4Address::getAny();
	}
	return AsyncTcpServer::create(address, loop);
}

Ref<AsyncTcpServer> AsyncTcpServer::create(sl_uint32 portListen, sl_bool flagIPv6)
{
	return AsyncTcpServer::create(portListen, AsyncLoop::getDefault(), flagIPv6);
}

/*******************************************
	AsyncUdpSocket
********************************************/
void IAsyncUdpSocketListener::onReceiveFrom(AsyncUdpSocket* socket, void* data, sl_uint32 sizeReceive, const SocketAddress& address, sl_bool flagError)
{
}

void IAsyncUdpSocketListener::onSendTo(AsyncUdpSocket* socket, void* data, sl_uint32 sizeSent, const SocketAddress& address, sl_bool flagError)
{
}

AsyncUdpSocketInstance::AsyncUdpSocketInstance()
{
}

AsyncUdpSocketInstance::~AsyncUdpSocketInstance()
{
}

sl_bool AsyncUdpSocketInstance::receiveFrom(void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, Referable* refData)
{
	if (size == 0) {
		return sl_false;
	}
	Ref<ReceiveRequest> req = new ReceiveRequest;
	if (req.isNotNull()) {
		req->data = data;
		req->size = size;
		req->listener = listener;
		req->refData = refData;
		m_requestsReceive.push(req);
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncUdpSocketInstance::sendTo(const SocketAddress& address, void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, Referable* refData)
{
	if (size == 0) {
		return sl_false;
	}
	Ref<SendRequest> req = new SendRequest;
	if (req.isNotNull()) {
		req->address = address;
		req->data = data;
		req->size = size;
		req->listener = listener;
		req->refData = refData;
		m_requestsSend.push(req);
		return sl_true;
	}
	return sl_false;
}

void AsyncUdpSocketInstance::_onReceive(AsyncUdpSocketInstance::ReceiveRequest* req, sl_uint32 size, const SocketAddress& address, sl_bool flagError)
{
	Ref<AsyncUdpSocket> object = Ref<AsyncUdpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	PtrLocker<IAsyncUdpSocketListener> listener(req->listener);
	if (listener.isNotNull()) {
		listener->onReceiveFrom(object.get(), req->data, size, address, flagError);
	}
}

void AsyncUdpSocketInstance::_onSend(AsyncUdpSocketInstance::SendRequest* req, sl_uint32 size, sl_bool flagError)
{
	Ref<AsyncUdpSocket> object = Ref<AsyncUdpSocket>::from(getObject());
	if (object.isNull()) {
		return;
	}
	PtrLocker<IAsyncUdpSocketListener> listener(req->listener);
	if (listener.isNotNull()) {
		listener->onSendTo(object.get(), req->data, size, req->address, flagError);
	}
}

AsyncUdpSocket::AsyncUdpSocket()
{
}

AsyncUdpSocket::~AsyncUdpSocket()
{
}

Ref<Socket> AsyncUdpSocket::getSocket()
{
	Ref<AsyncUdpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		return instance->getSocket();
	}
	return Ref<Socket>::null();
}

void AsyncUdpSocket::close()
{
	closeInstance();
}

sl_bool AsyncUdpSocket::isOpened()
{
	return getInstance().isNotNull();
}

void AsyncUdpSocket::setBroadcast(sl_bool flag)
{
	Ref<Socket> socket = getSocket();
	if (socket.isNotNull()) {
		socket->setOption_Broadcast(flag);
	}
}

sl_bool AsyncUdpSocket::receiveFrom(void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, Referable* refData)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncUdpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		if (instance->receiveFrom(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncUdpSocket::sendTo(const SocketAddress& address, void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, Referable* refData)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncUdpSocketInstance> instance = getInstance();
	if (instance.isNotNull()) {
		if (instance->sendTo(address, data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(AsyncUdpSocketInstance* instance, const Ref<AsyncLoop>& _loop)
{
	Ref<AsyncUdpSocket> ret;
	if (!instance) {
		return ret;
	}
	Ref<AsyncLoop> loop = _loop;
	if (loop.isNull()) {
		loop = AsyncLoop::getDefault();
		if (loop.isNull()) {
			return ret;
		}
	}
	ret = new AsyncUdpSocket;
	if (ret.isNotNull()) {
		instance->setObject(ret.get());
		ret->setInstance(instance);
		ret->setLoop(loop);
		if (!(loop->attachInstance(instance))) {
			ret.setNull();
		}
	}
	return ret;
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ref<Socket>& socket)
{
	return AsyncUdpSocket::create(socket, AsyncLoop::getDefault());
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const SocketAddress& _addressBind, const Ref<AsyncLoop>& loop, sl_bool flagBroadcast)
{
	SocketAddress addressBind = _addressBind;
	Ref<Socket> socket;
	if (addressBind.ip.isIPv6()) {
		socket = Socket::openUdp_IPv6();
	} else {
		socket = Socket::openUdp_IPv4();
	}
	if (socket.isNotNull()) {
		if (addressBind.ip.isNotNone() || addressBind.port != 0) {
			if (!(socket->bind(addressBind))) {
				return Ref<AsyncUdpSocket>::null();
			}
		}
		if (flagBroadcast) {
			socket->setOption_Broadcast(sl_true);
		}
		return AsyncUdpSocket::create(socket, loop);
	}
	return Ref<AsyncUdpSocket>::null();
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const SocketAddress& addressBind, sl_bool flagBroadcast)
{
	return AsyncUdpSocket::create(addressBind, AsyncLoop::getDefault(), flagBroadcast);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(sl_uint32 portBind, const Ref<AsyncLoop>& loop, sl_bool flagBroadcast, sl_bool flagIPv6)
{
	SocketAddress address;
	if (flagIPv6) {
		address.ip = IPv6Address::getAny();
	} else {
		address.ip = IPv4Address::getAny();
	}
	address.port = portBind;
	return AsyncUdpSocket::create(address, loop, flagBroadcast);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(sl_uint32 portBind, sl_bool flagBroadcast, sl_bool flagIPv6)
{
	return AsyncUdpSocket::create(portBind, AsyncLoop::getDefault(), flagBroadcast, flagIPv6);
}

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ref<AsyncLoop>& loop, sl_bool flagBroadcast, sl_bool flagIPv6)
{
	return AsyncUdpSocket::create(0, loop, flagBroadcast, flagIPv6);
}

SLIB_NETWORK_NAMESPACE_END
