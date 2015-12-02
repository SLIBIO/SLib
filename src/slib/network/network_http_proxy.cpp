#include "../../../inc/slib/network/http_proxy.h"

#include "../../../inc/slib/core/log.h"

#define SERVICE_TAG "HTTP PROXY"

#define CONNECT_BUFFER_SIZE 65536
#define CONNECT_BUFFER_COUNT 8

SLIB_NETWORK_NAMESPACE_BEGIN
HttpProxy::HttpProxy()
{
	m_flagRunning = sl_false;
}

HttpProxy::~HttpProxy()
{
	release();
}

void HttpProxy::release()
{
	MutexLocker lock(getLocker());
	HttpService::release();
}

sl_bool HttpProxy::start(const HttpProxyParam& param)
{
	MutexLocker lock(getLocker());
	if (m_flagRunning) {
		return sl_false;
	}
	if (HttpService::start(param)) {
		m_flagRunning = sl_true;
		return sl_true;
	}
	return sl_false;
}

sl_bool HttpProxy::preprocessRequest(HttpServiceContext* context)
{
	Ref<HttpServiceConnection> connection = context->getConnection();
	if (connection.isNotNull()) {
		String method = context->getMethod();
		if (method.toUpper() == SLIB_HTTP_METHOD_CONNECT) {
			if (m_param.flagLogDebug) {
				SLIB_LOG(SERVICE_TAG, "[" + String::fromPointerValue(connection.getObject()) + "] PROXY CONNECT - Host: " + context->getPath());
			}
			Ref<AsyncLoop> loop = getAsyncLoop();
			if (loop.isNotNull()) {
				loop->addTask(SLIB_CALLBACK_WEAKREF(HttpProxy, _processConnect, this, Ref<HttpServiceContext>(context)));
			}
			return sl_true;
		}
		SLIB_STATIC_STRING(schemaHttp, "http://");
		SLIB_STATIC_STRING(schemaFtp, "ftp://");
		sl_uint32 portProxy = 0;
		sl_bool flagProxy = sl_false;
		String path = context->getPath();
		if (path.startsWith(schemaHttp)) {
			flagProxy = sl_true;
			portProxy = 80;
		} else if (path.startsWith(schemaFtp)) {
			flagProxy = sl_true;
			portProxy = 21;
		}
		if (flagProxy) {
			String host = context->getHost();
			if (m_param.flagLogDebug) {
				String method = context->getMethod();
				SLIB_LOG(SERVICE_TAG, "[" + String::fromPointerValue(connection.getObject()) + "] PROXY " + method + " - Host: " + host);
			}
			Ref<AsyncLoop> loop = getAsyncLoop();
			if (loop.isNotNull()) {
				loop->addTask(SLIB_CALLBACK_WEAKREF(HttpProxy, _processProxy, this, Ref<HttpServiceContext>(context), host, portProxy));
			}
			return sl_true;
		}
	}
	return sl_false;
}

class _HttpProxy_ConnectContext : public Object, public IHttpProxyConnectListener, public IAsyncCopyListener
{
public:
	WeakRef<HttpServiceConnection> m_connection;
	Memory m_memoryInitialReceived;

	Ref<AsyncCopy> m_copyLocalToRemote;
	Ref<AsyncCopy> m_copyRemoteToLocal;
	sl_int32 m_nCountCopy;

	_HttpProxy_ConnectContext(HttpServiceConnection* connection, const Memory& memReceived)
	{
		m_connection = connection;
		m_memoryInitialReceived = memReceived;
		m_nCountCopy = 0;
	}

	void onConnect(HttpServiceConnection* connection, AsyncStream* streamRemote, sl_bool flagError)
	{
		Ref<AsyncStream> streamLocal = connection->getIO();
		if (streamLocal.isNull()) {
			return;
		}
		if (flagError) {
			connection->sendConnectResponse_Failed();
			return;
		}

		WeakRef<_HttpProxy_ConnectContext> _this(this);
		m_copyLocalToRemote = AsyncCopy::create(streamLocal.get(), streamRemote, SLIB_SIZE_MAX, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
		m_copyRemoteToLocal = AsyncCopy::create(streamRemote, streamLocal.get(), SLIB_SIZE_MAX, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
		if (m_copyLocalToRemote.isNull() || m_copyRemoteToLocal.isNull()) {
			connection->sendConnectResponse_Failed();
			return;
		}

		connection->sendConnectResponse_Successed();
		streamRemote->writeFromMemory(m_memoryInitialReceived, Ptr<IAsyncStreamListener>::null());
		m_memoryInitialReceived.setNull();

		m_copyLocalToRemote->start();
		m_copyRemoteToLocal->start();
		m_nCountCopy = 2;
	}

	void onAsyncCopyExit(AsyncCopy* task)
	{
		MutexLocker lock(getLocker());
		m_nCountCopy--;
		if (m_nCountCopy == 0) {
			lock.unlock();
			Ref<HttpServiceConnection> connection = m_connection.lock();
			if (connection.isNotNull()) {
				connection->close();
				connection->setProxyObject(Ref<Referable>::null());
			}
		}
	}
};

void HttpProxy::_processConnect(Ref<HttpServiceContext> context)
{
	Ref<HttpServiceConnection> connection = context->getConnection();
	if (connection.isNull()) {
		return;
	}
	String host = context->getPath();
	Ref<_HttpProxy_ConnectContext> connectContext = new _HttpProxy_ConnectContext(connection.get(), context->getRequestBody());
	if (connectContext.isNotNull()) {
		if (connectTo(connection.get(), host, WeakRef<_HttpProxy_ConnectContext>(connectContext))) {
			connection->setProxyObject(connectContext.getObject());
			return;
		}
	}
	connection->sendConnectResponse_Failed();
}

class _HttpProxy_ProxyContext : public Object, public IHttpProxyConnectListener, public IAsyncCopyListener
{
public:
	WeakRef<HttpServiceConnection> m_connection;
	Ref<HttpServiceContext> m_context;

	Ref<AsyncCopy> m_copyLocalToRemote;
	Ref<AsyncCopy> m_copyRemoteToLocal;
	sl_int32 m_nCountCopy;
	sl_uint64 m_sizeResponse;

	_HttpProxy_ProxyContext(HttpServiceConnection* connection, HttpServiceContext* context)
	{
		m_connection = connection;
		m_context = context;
		m_nCountCopy = 0;
		m_sizeResponse = 0;
	}

	void onConnect(HttpServiceConnection* connection, AsyncStream* streamRemote, sl_bool flagError)
	{
		Ref<AsyncStream> streamLocal = connection->getIO();
		if (streamLocal.isNull()) {
			return;
		}
		if (flagError) {
			connection->sendProxyResponse_Failed();
			return;
		}

		WeakRef<_HttpProxy_ProxyContext> _this(this);

		sl_int64 sizeRequest = m_context->getRequestContentLength();
		sizeRequest -= m_context->getRequestBody().getSize();
		if (sizeRequest > 0) {
			m_copyLocalToRemote = AsyncCopy::create(streamLocal.get(), streamRemote, sizeRequest, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
			if (m_copyLocalToRemote.isNull()) {
				connection->sendProxyResponse_Failed();
				return;
			}
		}
		m_copyRemoteToLocal = AsyncCopy::create(streamRemote, streamLocal.get(), SLIB_SIZE_MAX, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
		if (m_copyRemoteToLocal.isNull()) {
			connection->sendProxyResponse_Failed();
			return;
		}

		streamRemote->writeFromMemory(m_context->getRawRequestHeader(), Ptr<IAsyncStreamListener>::null());
		streamRemote->writeFromMemory(m_context->getRequestBody(), Ptr<IAsyncStreamListener>::null());

		if (m_copyLocalToRemote.isNotNull()) {
			m_copyLocalToRemote->start();
			m_nCountCopy++;
		}
		m_copyRemoteToLocal->start();
		m_nCountCopy++;
	}

	Memory m_memHeader;
	Memory onRead(AsyncCopy* task, const Memory& _input)
	{
		if (task == m_copyRemoteToLocal.get()) {
			Memory input = _input;
			if (m_sizeResponse == 0) {

			} else if (task->getWrittenSize() >= m_sizeResponse) {
				m_copyLocalToRemote.setNull();
				m_copyRemoteToLocal.setNull();
				Ref<HttpServiceConnection> connection = m_connection.lock();
				if (connection.isNotNull()) {
					connection->setProxyObject(Ref<Referable>::null());
					connection->start();
				}
			}
		}
		return _input;
	}

	void onAsyncCopyExit(AsyncCopy* task)
	{
		MutexLocker lock(getLocker());
		m_nCountCopy--;
	}
};

void HttpProxy::_processProxy(Ref<HttpServiceContext> context, String host, sl_uint32 port)
{
	Ref<HttpServiceConnection> connection = context->getConnection();
	if (connection.isNull()) {
		return;
	}
	if (host.indexOf(':') < 0) {
		host += ":";
		host += port;
	}
	Ref<_HttpProxy_ProxyContext> proxyContext = new _HttpProxy_ProxyContext(connection.get(), context.get());
	if (proxyContext.isNotNull()) {
		if (connectTo(connection.get(), host, WeakRef<_HttpProxy_ProxyContext>(proxyContext))) {
			connection->setProxyObject(proxyContext.getObject());
			return;
		}
	}
	connection->sendProxyResponse_Failed();
}

class _HttpProxy_ConnectListener : public Referable, public IAsyncTcpSocketListener
{
public:
	WeakRef<HttpServiceConnection> m_connection;
	Ptr<IHttpProxyConnectListener> m_listener;

	_HttpProxy_ConnectListener(HttpServiceConnection* connection, const Ptr<IHttpProxyConnectListener>& listener)
	{
		m_connection = connection;
		m_listener = listener;
	}

	void onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError)
	{
		Ref<HttpServiceConnection> connection = m_connection.lock();
		if (connection.isNull()) {
			return;
		}
		connection->setUserObject(Ref<Referable>::null());
		PtrLocker<IHttpProxyConnectListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onConnect(connection.get(), socket, flagError);
		}
	}
};

sl_bool HttpProxy::connectTo(HttpServiceConnection* connection, const String& hostAddress, const Ptr<IHttpProxyConnectListener>& listener)
{
	SocketAddress address;
	address.setHostAddress(hostAddress);
	if (address.isValid()) {
		Ref<AsyncTcpSocket> socket = AsyncTcpSocket::create(getAsyncLoop());
		if (socket.isNotNull()) {
			Ref<_HttpProxy_ConnectListener> listenerConnect = new _HttpProxy_ConnectListener(connection, listener);
			if (socket->connect(address, listenerConnect)) {
				connection->setUserObject(socket.getObject());
				return sl_true;
			}
		}
	}
	return sl_false;
}

Ref<HttpProxy> HttpProxy::create(const HttpProxyParam& param)
{
	Ref<HttpProxy> ret = new HttpProxy;
	if (ret.isNotNull()) {
		if (ret->start(param)) {
			return ret;
		}
		ret.setNull();
	}
	return ret;
}
SLIB_NETWORK_NAMESPACE_END
