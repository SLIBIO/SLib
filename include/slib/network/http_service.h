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

#ifndef CHECKHEADER_SLIB_NETWORK_HTTP_SERVICE
#define CHECKHEADER_SLIB_NETWORK_HTTP_SERVICE

#include "definition.h"

#include "http_common.h"
#include "http_io.h"
#include "socket_address.h"

#include "../core/thread_pool.h"

namespace slib
{

	class HttpService;
	class HttpServiceConnection;
	
	class SLIB_EXPORT HttpServiceContext : public Object, public HttpRequest, public HttpResponse, public HttpOutputBuffer
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		HttpServiceContext();
		
		~HttpServiceContext();
		
	public:
		static Ref<HttpServiceContext> create(const Ref<HttpServiceConnection>& connection);
		
	public:
		Memory getRawRequestHeader() const;
		
		sl_uint64 getRequestContentLength() const;
		
		Memory getRequestBody() const;
		
		Variant getRequestBodyAsJson() const;
		
		sl_uint64 getResponseContentLength() const;
		
		Ref<HttpService> getService();
		
		Ref<HttpServiceConnection> getConnection();
		
		Ref<AsyncStream> getIO();
		
		Ref<AsyncIoLoop> getAsyncIoLoop();
		
		const SocketAddress& getLocalAddress();
		
		const SocketAddress& getRemoteAddress();
		
		sl_bool isAsynchronousResponse();
		
		void setAsynchronousResponse(sl_bool flagAsync);
		
		void completeResponse();
		
	public:
		SLIB_BOOLEAN_PROPERTY(ClosingConnection);
		SLIB_BOOLEAN_PROPERTY(ProcessingByThread);
		
	protected:
		HttpHeaderReader m_requestHeaderReader;
		AtomicMemory m_requestHeader;
		sl_uint64 m_requestContentLength;
		MemoryQueue m_requestBodyBuffer;
		AtomicMemory m_requestBody;
		sl_bool m_flagAsynchronousResponse;
		
	private:
		WeakRef<HttpServiceConnection> m_connection;
		
		friend class HttpServiceConnection;
		
	};
	
	class SLIB_EXPORT HttpServiceConnection : public Object, public IClosable
	{
	protected:
		HttpServiceConnection();
		
		~HttpServiceConnection();
		
	public:
		static Ref<HttpServiceConnection> create(HttpService* service, AsyncStream* io);
		
	public:
		void close() override;
		
		void start(const void* data = sl_null, sl_uint32 size = 0);
		
		Ref<AsyncStream> getIO();
		
		Ref<HttpService> getService();
		
		Ref<HttpServiceContext> getCurrentContext();
		
		void sendResponse(const Memory& mem);
		
		void sendResponseAndRestart(const Memory& mem);
		
		void sendResponseAndClose(const Memory& mem);
		
		void sendResponse_BadRequest();
		
		void sendResponse_ServerError();
		
		void sendConnectResponse_Successed();
		
		void sendConnectResponse_Failed();
		
		void sendProxyResponse_Failed();
		
	public:
		SLIB_PROPERTY(SocketAddress, LocalAddress)
		SLIB_PROPERTY(SocketAddress, RemoteAddress)
		SLIB_PROPERTY(AtomicRef<Referable>, ProxyObject)
		SLIB_PROPERTY(AtomicRef<Referable>, UserObject)
		
	protected:
		WeakRef<HttpService> m_service;
		Ref<AsyncStream> m_io;
		Ref<AsyncOutput> m_output;
		
		AtomicRef<HttpServiceContext> m_contextCurrent;
		
		sl_bool m_flagClosed;
		Memory m_bufRead;
		sl_bool m_flagReading;
		sl_bool m_flagKeepAlive;
		
	protected:
		void _read();
		
		void _processInput(const void* data, sl_uint32 size);
		
		void _processContext(const Ref<HttpServiceContext>& context);
		
		void _completeResponse(HttpServiceContext* context);
		
	protected:
		void onReadStream(AsyncStreamResult* result);

		void onAsyncOutputEnd(AsyncOutput* output, sl_bool flagError);
		
		friend class HttpServiceContext;
		
	};
	
	
	class SLIB_EXPORT HttpServiceConnectionProvider : public Object
	{
	public:
		HttpServiceConnectionProvider();
		
		~HttpServiceConnectionProvider();
		
	public:
		virtual void release() = 0;
		
	public:
		Ref<HttpService> getService();
		
		void setService(const Ref<HttpService>& service);
		
	private:
		WeakRef<HttpService> m_service;
		
	};
	
	class SLIB_EXPORT HttpServiceParam
	{
	public:
		IPAddress addressBind;
		sl_uint16 port;
		
		sl_uint32 maxThreadsCount;
		sl_bool flagProcessByThreads;
		
		sl_bool flagUseWebRoot;
		String webRootPath;

		sl_bool flagUseAsset;
		String prefixAsset;
		
		sl_uint64 maxRequestHeadersSize;
		sl_uint64 maxRequestBodySize;
		
		sl_bool flagAllowCrossOrigin;
		
		List<String> allowedFileExtensions;
		List<String> blockedFileExtensions;
		
		sl_bool flagUseCacheControl;
		sl_bool flagCacheControlNoCache;
		sl_uint32 cacheControlMaxAge;
		
		sl_bool flagLogDebug;
		
		Function<sl_bool(HttpService*, HttpServiceContext*)> onRequest;
		Function<void(HttpService*, HttpServiceContext*, sl_bool flagProcessed)> onPostRequest;

	public:
		HttpServiceParam();
		
		HttpServiceParam(const HttpServiceParam& other);
		
		~HttpServiceParam();
		
	public:
		void setJson(const Json& json);
		
		sl_bool parseJsonFile(const String& filePath);
		
	};
	
	class SLIB_EXPORT HttpService : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		HttpService();
		
		~HttpService();
		
	public:
		static Ref<HttpService> create(const HttpServiceParam& param);
		
	public:
		void release();
		
		sl_bool isRunning();
		
		Ref<AsyncIoLoop> getAsyncIoLoop();
		
		Ref<ThreadPool> getThreadPool();
		
		const HttpServiceParam& getParam();
		
	public:
		// called before processing body, returns true if the service is trying to process the connection itself.
		virtual sl_bool preprocessRequest(const Ref<HttpServiceContext>& context);
		
		// called after inputing body
		virtual void processRequest(const Ref<HttpServiceContext>& context);
		
		virtual sl_bool processAsset(const Ref<HttpServiceContext>& context, const String& path);
		
		sl_bool processFile(const Ref<HttpServiceContext>& context, const String& path);
		
		sl_bool processRangeRequest(const Ref<HttpServiceContext>& context, sl_uint64 totalLength, const String& range, sl_uint64& outStart, sl_uint64& outLength);
		
		virtual Ref<HttpServiceConnection> addConnection(const Ref<AsyncStream>& stream, const SocketAddress& remoteAddress, const SocketAddress& localAddress);
		
		virtual void closeConnection(HttpServiceConnection* connection);
		
	protected:
		virtual sl_bool onRequest(HttpServiceContext* context);
		
		sl_bool dispatchRequest(HttpServiceContext* context);
		
		virtual void onPostRequest(HttpServiceContext* context, sl_bool flagProcessed);
		
		void dispatchPostRequest(HttpServiceContext* context, sl_bool flagProcessed);
		
	public:
		void addConnectionProvider(const Ref<HttpServiceConnectionProvider>& provider);
		
		void removeConnectionProvider(const Ref<HttpServiceConnectionProvider>& provider);
		
		
		sl_bool addHttpService(const SocketAddress& addr);
		
		sl_bool addHttpService(sl_uint32 port = 80);
		
		sl_bool addHttpService(const IPAddress& addr, sl_uint32 port = 80);
		
		
	protected:
		sl_bool _init(const HttpServiceParam& param);
		
		void _processCacheControl(const Ref<HttpServiceContext>& context);
		
	protected:
		AtomicRef<AsyncIoLoop> m_ioLoop;
		AtomicRef<ThreadPool> m_threadPool;
		sl_bool m_flagRunning;
		
		CHashMap< HttpServiceConnection*, Ref<HttpServiceConnection> > m_connections;
		
		CList< Ref<HttpServiceConnectionProvider> > m_connectionProviders;
		
		HttpServiceParam m_param;
		
	};

}

#endif

