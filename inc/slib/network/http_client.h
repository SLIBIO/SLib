#ifndef CHECKHEADER_SLIB_NETWORK_HTTP_CLIENT
#define CHECKHEADER_SLIB_NETWORK_HTTP_CLIENT

#include "http_common.h"

#include "socket_address.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class HttpClientManager;

class HttpClient;
class HttpClientSession;
class HttpClientContext;
class HttpClientConnection;

class SLIB_EXPORT IHttpClientListener
{
public:
	virtual void onHttpError(HttpClientContext* context);
	
	// returns sl_true if the user want to process response stream himself
	virtual sl_bool onHttpResponseHeader(HttpClientContext* context);

	// returns the size of buffer that should be added to context body buffer
	virtual sl_size onHttpResponsePacket(HttpClientContext* context, void* data, sl_size size);

	virtual void onHttpResponse(HttpClientContext* context);
	
};

class SLIB_EXPORT HttpClientContext : public Object, public HttpRequestHeader, public HttpResponseHeader, public HttpOutputBuffer
{
	SLIB_DECLARE_OBJECT(HttpClientContext, Object)
protected:
	HttpClientContext();
	
public:
	static Ref<HttpClientContext> create();

public:
	void clearResponse();
	
	
	void setUrl(const String& host, const String& path, const String& query);
	
	void setUrl(const String& url);


	Ref<HttpClientConnection> getConnection();

	SLIB_STRING_PROPERTY_INLINE(Protocol)

public:
	SLIB_INLINE const Memory& getRawResponseHeader() const
	{
		return m_responseHeader;
	}

	SLIB_INLINE sl_uint64 getResponseContentLength() const
	{
		return m_responseContentLength;
	}

	SLIB_INLINE const Memory& getResponseBody() const
	{
		return m_responseBody;
	}

protected:
	HttpHeaderReader m_responseHeaderReader;
	Memory m_responseHeader;
	sl_uint64 m_responseContentLength;
	MemoryBuffer m_responseBodyBuffer;
	Memory m_responseBody;

	sl_bool m_flagProcessing;
	WeakRef<HttpClientConnection> m_connection;

public:
	SLIB_INLINE sl_uint64 getRequestContentLength() const
	{
		return getOutputLength();
	}

public:
	SLIB_PROPERTY_INLINE(Ptr<IHttpClientListener>, Listener);
	SLIB_PROPERTY_INLINE(Ref<Event>, CompletionEvent);

	friend class HttpClientConnection;
};

class SLIB_EXPORT HttpClientConnection : public Object, public IAsyncStreamListener, public IAsyncOutputListener, public IClosable
{
protected:
	HttpClientConnection();
public:
	~HttpClientConnection();

public:
	Ref<AsyncStream> getIO();

	Ref<HttpClient> getClient();
	Ref<HttpClientSession> getSession();

	void close();
	void addContext(const Ref<HttpClientContext>& context);

public:
	static Ref<HttpClientConnection> create(HttpClient* client, HttpClientSession* session, AsyncStream* io);

protected:
	WeakRef<HttpClient> m_client;
	WeakRef<HttpClientSession> m_session;
	Ref<AsyncStream> m_io;
	Ref<AsyncOutput> m_output;

	Queue< Ref<HttpClientContext> > m_queueContexts;
	Ref<HttpClientContext> m_currentContext;

	sl_bool m_flagClosed;
	Memory m_bufRead;
	sl_bool m_flagReading;

	Time m_timeReturned;

protected:
	void _read();
	void _processInput(const void* data, sl_uint32 size);
	void _onError(HttpClientContext* context);

protected:
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError);
	void onAsyncOutputComplete(AsyncOutput* output);
	void onAsyncOutputError(AsyncOutput* output);

	friend class HttpClient;
};

class SLIB_EXPORT HttpClientSession : public Object
{
protected:
	HttpClientSession();
public:
	~HttpClientSession();

public:
	SLIB_INLINE const String& getProtocol()
	{
		return m_protocol;
	}

	SLIB_INLINE const String& getHost()
	{
		return m_host;
	}

public:
	static Ref<HttpClientSession> create(const String& protocol, const String& host);

protected:
	String m_protocol;
	String m_host;
	Queue< Ref<HttpClientConnection> > m_connectionsWorking;
	Queue< Ref<HttpClientConnection> > m_connectionsFree;

	friend class HttpClient;
};

class SLIB_EXPORT HttpClientParam
{
public:
	sl_uint32 maxConnectionsPerSession;

public:
	HttpClientParam()
	{
		maxConnectionsPerSession = 128;
	}
};

class SLIB_EXPORT HttpClient : public Object
{
protected:
	HttpClient();
public:
	~HttpClient();

public:
	void sendRequest(const Ref<HttpClientContext>& request, const SocketAddress& proxy);
	void sendRequest(const Ref<HttpClientContext>& request);

	void closeConnection(HttpClientConnection* connection);
	void returnConnection(HttpClientConnection* connection);

public:
	static Ref<HttpClient> create(const HttpClientParam& param);
};
SLIB_NETWORK_NAMESPACE_END

#endif

