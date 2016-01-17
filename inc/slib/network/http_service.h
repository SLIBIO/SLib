#ifndef CHECKHEADER_SLIB_NETWORK_HTTP_SERVICE
#define CHECKHEADER_SLIB_NETWORK_HTTP_SERVICE

#include "http_common.h"

#include "socket_address.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class HttpService;
class HttpServiceConnection;

class SLIB_EXPORT HttpServiceContext : public Object, public HttpRequestHeader, public HttpResponseHeader, public HttpOutputBuffer
{
	SLIB_DECLARE_OBJECT(HttpServiceContext, Object)
protected:
	HttpServiceContext();
public:
	~HttpServiceContext();

public:
	SLIB_INLINE const Memory& getRawRequestHeader() const
	{
		return m_requestHeader;
	}

	SLIB_INLINE sl_uint64 getRequestContentLength() const
	{
		return m_requestContentLength;
	}

	SLIB_INLINE const Memory& getRequestBody() const
	{
		return m_requestBody;
	}

protected:
	HttpHeaderReader m_requestHeaderReader;
	Memory m_requestHeader;
	sl_uint64 m_requestContentLength;
	MemoryBuffer m_requestBodyBuffer;
	Memory m_requestBody;

public:
	SLIB_INLINE sl_uint64 getResponseContentLength() const
	{
		return getOutputLength();
	}

public:
	SLIB_BOOLEAN_PROPERTY_INLINE(ClosingConnection);
	SLIB_BOOLEAN_PROPERTY_INLINE(ProcessingByThread);

public:
	Ref<HttpService> getService();
	Ref<HttpServiceConnection> getConnection();
	Ref<AsyncStream> getIO();
	Ref<AsyncLoop> getAsyncLoop();

	const SocketAddress& getLocalAddress();
	const SocketAddress& getRemoteAddress();

public:
	static Ref<HttpServiceContext> create(const Ref<HttpServiceConnection>& connection);

private:
	WeakRef<HttpServiceConnection> m_connection;

	friend class HttpServiceConnection;
};

class SLIB_EXPORT HttpServiceConnection : public Object, public IAsyncStreamListener, public IAsyncOutputListener, public IClosable
{
protected:
	HttpServiceConnection();
public:
	~HttpServiceConnection();

public:
	Ref<AsyncStream> getIO();

	Ref<HttpService> getService();

	void close();
	void start(const void* data = sl_null, sl_uint32 size = 0);

	void sendResponse(const Memory& mem);
	void sendResponseAndRestart(const Memory& mem);
	void sendResponseAndClose(const Memory& mem);

	void sendResponse_BadRequest();
	void sendResponse_ServerError();
	void sendConnectResponse_Successed();
	void sendConnectResponse_Failed();
	void sendProxyResponse_Failed();

public:
	SLIB_PROPERTY_INLINE(SocketAddress, LocalAddress)
	SLIB_PROPERTY_INLINE(SocketAddress, RemoteAddress)
	SLIB_PROPERTY_INLINE(Ref<Referable>, ProxyObject)
	SLIB_PROPERTY_INLINE(Ref<Referable>, UserObject)

	SLIB_INLINE const Ref<HttpServiceContext>& getCurrentContext()
	{
		return m_contextCurrent;
	}

public:
	static Ref<HttpServiceConnection> create(HttpService* service, AsyncStream* io);

protected:
	WeakRef<HttpService> m_service;
	Ref<AsyncStream> m_io;
	Ref<AsyncOutput> m_output;

	Ref<HttpServiceContext> m_contextCurrent;

	sl_bool m_flagClosed;
	Memory m_bufRead;
	sl_bool m_flagReading;

protected:
	void _read();
	void _processInput(const void* data, sl_uint32 size);
	void _processContext(Ref<HttpServiceContext> context);

protected:
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError);
	void onAsyncOutputComplete(AsyncOutput* output);
	void onAsyncOutputError(AsyncOutput* output);
};

class SLIB_EXPORT IHttpServiceProcessor
{
public:
	virtual sl_bool onHttpRequest(HttpServiceContext* context) = 0;
};

class SLIB_EXPORT HttpServiceParam
{
public:
	IPAddress addressBind;
	sl_uint16 port;

	sl_uint32 maxThreadsCount;
	sl_bool flagProcessByThreads;

	sl_bool flagUseResource;
	String prefixResource;

	sl_uint64 maxRequestHeadersSize;
	sl_uint64 maxRequestBodySize;

	sl_bool flagLogDebug;

public:
	HttpServiceParam()
	{
		port = 80;

		maxThreadsCount = 32;
		flagProcessByThreads = sl_true;

		flagUseResource = sl_false;

		maxRequestHeadersSize = 0x10000; // 64KB
		maxRequestBodySize = 0x2000000; // 32MB

		flagLogDebug = sl_false;
	}
};

class SLIB_EXPORT HttpServiceConnectionProvider : public Object
{
public:
	virtual void release() = 0;

	Ref<HttpService> getService();
	void setService(const Ref<HttpService>& service);

private:
	WeakRef<HttpService> m_service;
};

class SLIB_EXPORT HttpService : public Object
{
	SLIB_DECLARE_OBJECT(HttpService, Object)
protected:
	HttpService();
public:
	~HttpService();

public:
	sl_bool start(const HttpServiceParam& param);
	void release();
	
	virtual Ref<HttpServiceConnection> addConnection(const Ref<AsyncStream>& stream, const SocketAddress& remoteAddress, const SocketAddress& localAddress);
	virtual void closeConnection(HttpServiceConnection* connection);

	// called before processing body, returns true if the service is trying to process the connection itself.
	virtual sl_bool preprocessRequest(HttpServiceContext* context);

	// called after inputing body
	virtual void processRequest(HttpServiceContext* context);

	virtual sl_bool processResource(HttpServiceContext* context, String path);

public:
	SLIB_INLINE sl_bool isRunning()
	{
		return m_flagRunning;
	}

	SLIB_INLINE const Ref<AsyncLoop>& getAsyncLoop()
	{
		return m_loop;
	}

	SLIB_INLINE void addProcessor(const Ptr<IHttpServiceProcessor>& processor)
	{
		m_processors.add(processor);
	}

	SLIB_INLINE void removeProcessor(const Ptr<IHttpServiceProcessor>& processor)
	{
		m_processors.removeValue(processor);
	}

	SLIB_INLINE List< Ptr<IHttpServiceProcessor> > getProcessors()
	{
		return m_processors;
	}

	SLIB_INLINE void addConnectionProvider(const Ref<HttpServiceConnectionProvider>& provider)
	{
		m_connectionProviders.add(provider);
	}

	SLIB_INLINE void removeService(const Ref<HttpServiceConnectionProvider>& provider)
	{
		m_connectionProviders.removeValue(provider);
	}

	sl_bool addHttpService(const SocketAddress& addr);

	SLIB_INLINE sl_bool addHttpService(sl_uint32 port = 80)
	{
		return addHttpService(SocketAddress(port));
	}

	SLIB_INLINE sl_bool addHttpService(const IPAddress& addr, sl_uint32 port = 80)
	{
		return addHttpService(SocketAddress(addr, port));
	}

	SLIB_INLINE const HttpServiceParam& getParam()
	{
		return m_param;
	}

public:
	static Ref<HttpService> create(const HttpServiceParam& param);

protected:
	Ref<AsyncLoop> m_loop;
	sl_bool m_flagRunning;
	Map< HttpServiceConnection*, Ref<HttpServiceConnection> > m_connections;

	List< Ptr<IHttpServiceProcessor> > m_processors;
	List< Ref<HttpServiceConnectionProvider> > m_connectionProviders;

	HttpServiceParam m_param;
};

SLIB_NETWORK_NAMESPACE_END

#endif

