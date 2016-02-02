#include "../../../inc/slib/network/secure_stream.h"

SLIB_NETWORK_NAMESPACE_BEGIN

AsyncTcpSecureStreamServer::AsyncTcpSecureStreamServer()
{
}

AsyncTcpSecureStreamServer::~AsyncTcpSecureStreamServer()
{
	close();
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, const SocketAddress& addressListen, const Ref<AsyncIoLoop>& loop)
{
	Ref<AsyncTcpSecureStreamServer> ret = new AsyncTcpSecureStreamServer;
	if (ret.isNotNull()) {
		ret->m_param = param;
		Ref<AsyncTcpServer> server = AsyncTcpServer::create(addressListen, WeakRef<AsyncTcpSecureStreamServer>(ret), loop);
		if (server.isNotNull()) {
			ret->m_server = server;
			return ret;
		}
	}
	return ret;
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, const SocketAddress& addressListen)
{
	return create(param, addressListen, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, sl_uint32 portListen, const Ref<AsyncIoLoop>& loop)
{
	return create(param, SocketAddress(IPv4Address::any(), portListen), loop);
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, sl_uint32 portListen)
{
	return create(param, portListen, AsyncIoLoop::getDefault());
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::createIPv6(const AsyncSecureStreamServerParam& param, sl_uint32 portListen, const Ref<AsyncIoLoop>& loop)
{
	return create(param, SocketAddress(IPv6Address::any(), portListen), loop);
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::createIPv6(const AsyncSecureStreamServerParam& param, sl_uint32 portListen)
{
	return createIPv6(param, portListen, AsyncIoLoop::getDefault());
}

void AsyncTcpSecureStreamServer::close()
{
	ObjectLocker lock(this);
	if (m_server.isNotNull()) {
		m_server->close();
		m_server.setNull();
	}
}

void AsyncTcpSecureStreamServer::onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address)
{
	Ref<AsyncTcpSocket> socket = AsyncTcpSocket::create(socketAccept);
	if (socket.isNotNull()) {
		AsyncSecureStreamServerParam param = m_param;
		param.listener = (WeakRef<AsyncTcpSecureStreamServer>)(this);
		Ref<AsyncSecureStream> stream = SecureStreamServer::createAsyncStream(socket.get(), param);
		if (stream.isNotNull()) {
			m_streams.put(stream.get(), stream);
		}
	}
}

void AsyncTcpSecureStreamServer::onConnectedSecureStream(AsyncSecureStream* securedStream, sl_bool flagError)
{
	if (!flagError) {
		PtrLocker<IAsyncSecureStreamListener> listener(m_param.listener);
		if (listener.isNotNull()) {
			listener->onConnectedSecureStream(securedStream, flagError);
		}
	}
	m_streams.remove(securedStream);
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(const Ref<AsyncTcpSocket>& socket, const AsyncSecureStreamClientParam& param, sl_bool flagConnect)
{
	Ref<AsyncSecureStream> ret;
	if (socket.isNull()) {
		return ret;
	}
	return SecureStreamClient::createAsyncStream(socket.get(), param, flagConnect);
}

class _AsyncTcpSecureStreamClient_Container : public Referable, public IAsyncTcpSocketListener
{
public:
	WeakRef<AsyncSecureStream> m_stream;

public:
	void onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError)
	{
		Ref<AsyncSecureStream> stream = m_stream;
		if (stream.isNull()) {
			return;
		}
		if (flagError) {
			PtrLocker<IAsyncSecureStreamListener> listener(stream->getListener());
			if (listener.isNotNull()) {
				listener->onConnectedSecureStream(stream.get(), sl_true);
			}
		} else {
			if (!(stream->connect())) {
				PtrLocker<IAsyncSecureStreamListener> listener(stream->getListener());
				if (listener.isNotNull()) {
					listener->onConnectedSecureStream(stream.get(), sl_true);
				}
			}
		}
	}
};

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param,
	const SocketAddress& addressBind, const SocketAddress& addressConnect,
	const Ref<AsyncIoLoop>& loop)
{
	Ref<AsyncTcpSocket> socket = AsyncTcpSocket::create(addressBind, loop);
	Ref<AsyncSecureStream> stream = create(socket, param, sl_false);
	Ref<_AsyncTcpSecureStreamClient_Container> container = new _AsyncTcpSecureStreamClient_Container;
	if (container.isNotNull()) {
		container->m_stream = stream;
		if (socket->connect(addressConnect, container)) {
			return stream;
		}
	}
	return Ref<AsyncSecureStream>::null();
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param,
	const SocketAddress& addressBind, const SocketAddress& addressConnect)
{
	return create(param, addressBind, addressConnect, AsyncIoLoop::getDefault());
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param,
	const SocketAddress& addressConnect,
	const Ref<AsyncIoLoop>& loop)
{
	return create(param, SocketAddress::none(), addressConnect, loop);
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param,
	const SocketAddress& addressConnect)
{
	return create(param, SocketAddress::none(), addressConnect, AsyncIoLoop::getDefault());
}

SLIB_NETWORK_NAMESPACE_END
