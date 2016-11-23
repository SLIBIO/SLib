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

sl_bool HttpProxy::_init(const HttpProxyParam& param)
{
	if (HttpService::_init(param)) {
		return sl_true;
	}
	return sl_false;
}

Ref<HttpProxy> HttpProxy::create(const HttpProxyParam& param)
{
	Ref<HttpProxy> ret = new HttpProxy;
	if (ret.isNotNull()) {
		if (ret->_init(param)) {
			return ret;
		}
	}
	return Ref<HttpProxy>::null();
}

void HttpProxy::release()
{
	ObjectLocker lock(this);
	HttpService::release();
}


sl_bool HttpProxy::preprocessRequest(HttpServiceContext* context)
{
	Ref<HttpServiceConnection> connection = context->getConnection();
	if (connection.isNotNull()) {
		HttpMethod method = context->getMethod();
		if (method == HttpMethod::CONNECT) {
			if (m_param.flagLogDebug) {
				SLIB_LOG(SERVICE_TAG, "[" + String::fromPointerValue(connection.ptr) + "] PROXY CONNECT - Host: " + context->getPath());
			}
			Ref<ThreadPool> pool = getThreadPool();
			if (pool.isNotNull()) {
				pool->addTask(SLIB_CALLBACK_WEAKREF(HttpProxy, _processConnect, this, Ref<HttpServiceContext>(context)));
			}
			return sl_true;
		}
		sl_uint32 portProxy = 0;
		sl_bool flagProxy = sl_false;
		String path = context->getPath();
		if (path.startsWith("http://")) {
			flagProxy = sl_true;
			portProxy = 80;
		} else if (path.startsWith("ftp://")) {
			flagProxy = sl_true;
			portProxy = 21;
		}
		if (flagProxy) {
			String host = context->getHost();
			if (m_param.flagLogDebug) {
				SLIB_LOG(SERVICE_TAG, "[" + String::fromPointerValue(connection.ptr) + "] PROXY " + context->getMethodText() + " - Host: " + host);
			}
			Ref<ThreadPool> pool = getThreadPool();
			if (pool.isNotNull()) {
				pool->addTask(SLIB_CALLBACK_WEAKREF(HttpProxy, _processProxy, this, Ref<HttpServiceContext>(context), host, portProxy));
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

public:
	_HttpProxy_ConnectContext(HttpServiceConnection* connection, const Memory& memReceived)
	{
		m_connection = connection;
		m_memoryInitialReceived = memReceived;
		m_nCountCopy = 0;
	}

public:
	// override
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
		m_copyLocalToRemote = AsyncCopy::create(streamLocal, streamRemote, SLIB_SIZE_MAX, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
		m_copyRemoteToLocal = AsyncCopy::create(streamRemote, streamLocal, SLIB_SIZE_MAX, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
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

	// override
	void onAsyncCopyExit(AsyncCopy* task)
	{
		ObjectLocker lock(this);
		m_nCountCopy--;
		if (m_nCountCopy == 0) {
			lock.unlock();
			Ref<HttpServiceConnection> connection = m_connection;
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
	Ref<_HttpProxy_ConnectContext> connectContext = new _HttpProxy_ConnectContext(connection.ptr, context->getRequestBody());
	if (connectContext.isNotNull()) {
		if (connectTo(connection.ptr, host, WeakRef<_HttpProxy_ConnectContext>(connectContext))) {
			connection->setProxyObject(connectContext.ptr);
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

public:
	_HttpProxy_ProxyContext(HttpServiceConnection* connection, HttpServiceContext* context)
	{
		m_connection = connection;
		m_context = context;
		m_nCountCopy = 0;
		m_sizeResponse = 0;
	}

public:
	// override
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
			m_copyLocalToRemote = AsyncCopy::create(streamLocal, streamRemote, sizeRequest, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
			if (m_copyLocalToRemote.isNull()) {
				connection->sendProxyResponse_Failed();
				return;
			}
		}
		m_copyRemoteToLocal = AsyncCopy::create(streamRemote, streamLocal, SLIB_SIZE_MAX, _this, CONNECT_BUFFER_SIZE, CONNECT_BUFFER_COUNT, sl_false);
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

	// override
	Memory onAsyncCopyRead(AsyncCopy* task, const Memory& input)
	{
		if (task == m_copyRemoteToLocal.ptr) {
			if (m_sizeResponse == 0) {

			} else if (task->getWrittenSize() >= m_sizeResponse) {
				m_copyLocalToRemote.setNull();
				m_copyRemoteToLocal.setNull();
				Ref<HttpServiceConnection> connection = m_connection;
				if (connection.isNotNull()) {
					connection->setProxyObject(Ref<Referable>::null());
					connection->start();
				}
			}
		}
		return input;
	}

	// override
	void onAsyncCopyExit(AsyncCopy* task)
	{
		ObjectLocker lock(this);
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
	Ref<_HttpProxy_ProxyContext> proxyContext = new _HttpProxy_ProxyContext(connection.ptr, context.ptr);
	if (proxyContext.isNotNull()) {
		if (connectTo(connection.ptr, host, WeakRef<_HttpProxy_ProxyContext>(proxyContext))) {
			connection->setProxyObject(proxyContext.ptr);
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

public:
	_HttpProxy_ConnectListener(HttpServiceConnection* connection, const Ptr<IHttpProxyConnectListener>& listener)
	{
		m_connection = connection;
		m_listener = listener;
	}

public:
	// override
	void onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError)
	{
		Ref<HttpServiceConnection> connection = m_connection;
		if (connection.isNull()) {
			return;
		}
		connection->setUserObject(Ref<Referable>::null());
		PtrLocker<IHttpProxyConnectListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onConnect(connection.ptr, socket, flagError);
		}
	}
};

sl_bool HttpProxy::connectTo(HttpServiceConnection* connection, const String& hostAddress, const Ptr<IHttpProxyConnectListener>& listener)
{
	SocketAddress address;
	address.setHostAddress(hostAddress);
	if (address.isValid()) {
		Ref<AsyncTcpSocket> socket = AsyncTcpSocket::create(getAsyncIoLoop());
		if (socket.isNotNull()) {
			Ref<_HttpProxy_ConnectListener> listenerConnect = new _HttpProxy_ConnectListener(connection, listener);
			if (socket->connect(address, listenerConnect)) {
				connection->setUserObject(socket.ptr);
				return sl_true;
			}
		}
	}
	return sl_false;
}

SLIB_NETWORK_NAMESPACE_END
