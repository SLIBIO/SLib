#include "../../../inc/slib/network/secure_stream.h"

SLIB_NETWORK_NAMESPACE_BEGIN
AsyncTcpSecureStreamServer::AsyncTcpSecureStreamServer()
{
}

AsyncTcpSecureStreamServer::~AsyncTcpSecureStreamServer()
{
	close();
}

void AsyncTcpSecureStreamServer::start()
{
	ObjectLocker lock(this);
	if (m_server.isNotNull()) {
		m_server->start();
	}
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

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const Ref<AsyncTcpServer>& server, const AsyncSecureStreamServerParam& param, sl_bool flagStart)
{
	Ref<AsyncTcpSecureStreamServer> ret;
	if (server.isNull()) {
		return ret;
	}
	ret = new AsyncTcpSecureStreamServer();
	if (ret.isNotNull()) {
		ret->m_server = server;
		ret->m_param = param;
		server->setListener(WeakRef<AsyncTcpSecureStreamServer>(ret));
		if (flagStart) {
			server->start();
		}
		return ret;
	}
	return ret;
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, const SocketAddress& addressListen, const Ref<AsyncLoop>& loop, sl_bool flagStart)
{
	return create(AsyncTcpServer::create(addressListen, loop), param, flagStart);
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, const SocketAddress& addressListen, sl_bool flagStart)
{
	return create(AsyncTcpServer::create(addressListen), param, flagStart);
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, sl_uint32 portListen, const Ref<AsyncLoop>& loop, sl_bool flagIPv6, sl_bool flagStart)
{
	return create(AsyncTcpServer::create(portListen, loop, flagIPv6), param, flagStart);
}

Ref<AsyncTcpSecureStreamServer> AsyncTcpSecureStreamServer::create(const AsyncSecureStreamServerParam& param, sl_uint32 portListen, sl_bool flagIPv6, sl_bool flagStart)
{
	return create(AsyncTcpServer::create(portListen, flagIPv6), param, flagStart);
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(const Ref<AsyncTcpSocket>& socket, const AsyncSecureStreamClientParam& param, sl_bool flagConnect)
{
	Ref<AsyncSecureStream> ret;
	if (socket.isNull()) {
		return ret;
	}
	ret = SecureStreamClient::createAsyncStream(socket.get(), param, flagConnect);
	return ret;
}

class _AsyncTcpSecureStreamClient_Container : public Referable, public IAsyncTcpSocketListener
{
public:
	WeakRef<AsyncSecureStream> m_stream;

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
	const AsyncSecureStreamClientParam& param
	, const SocketAddress& addressBind, const SocketAddress& addressConnect
	, const Ref<AsyncLoop>& loop)
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
	stream.setNull();
	return stream;
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param
	, const SocketAddress& addressBind, const SocketAddress& addressConnect)
{
	return create(param, addressBind, addressConnect, AsyncLoop::getDefault());
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param
	, const SocketAddress& addressConnect
	, const Ref<AsyncLoop>& loop)
{
	return create(param, SocketAddress::none(), addressConnect, loop);
}

Ref<AsyncSecureStream> AsyncTcpSecureStreamClient::create(
	const AsyncSecureStreamClientParam& param
	, const SocketAddress& addressConnect)
{
	return create(param, SocketAddress::none(), addressConnect, AsyncLoop::getDefault());
}
SLIB_NETWORK_NAMESPACE_END
