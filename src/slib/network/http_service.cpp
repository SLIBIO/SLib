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

#include "slib/network/http_service.h"

#include "slib/network/url.h"
#include "slib/core/app.h"
#include "slib/core/asset.h"
#include "slib/core/file.h"
#include "slib/core/log.h"
#include "slib/core/json.h"
#include "slib/core/content_type.h"

#define SERVICE_TAG "HTTP SERVICE"

namespace slib
{

/**********************************************
			HttpServiceContext
**********************************************/

	SLIB_DEFINE_OBJECT(HttpServiceContext, Object)

	HttpServiceContext::HttpServiceContext()
	{
		m_requestContentLength = 0;
		m_flagAsynchronousResponse = sl_false;

		setClosingConnection(sl_false);
		setProcessingByThread(sl_true);
	}

	HttpServiceContext::~HttpServiceContext()
	{
	}

	Ref<HttpServiceContext> HttpServiceContext::create(const Ref<HttpServiceConnection>& connection)
	{
		Ref<HttpServiceContext> ret;
		if (connection.isNotNull()) {
			ret = new HttpServiceContext;
			if (ret.isNotNull()) {
				ret->m_connection = connection;
			}
		}
		return ret;
	}

	Memory HttpServiceContext::getRawRequestHeader() const
	{
		return m_requestHeader;
	}

	sl_uint64 HttpServiceContext::getRequestContentLength() const
	{
		return m_requestContentLength;
	}

	Memory HttpServiceContext::getRequestBody() const
	{
		return m_requestBody;
	}

	Variant HttpServiceContext::getRequestBodyAsJson() const
	{
		return Json::parseJson16Utf8(m_requestBody);
	}

	sl_uint64 HttpServiceContext::getResponseContentLength() const
	{
		return getOutputLength();
	}

	Ref<HttpService> HttpServiceContext::getService()
	{
		Ref<HttpServiceConnection> connection = getConnection();
		if (connection.isNotNull()) {
			return connection->getService();
		}
		return sl_null;
	}

	Ref<HttpServiceConnection> HttpServiceContext::getConnection()
	{
		return m_connection;
	}

	Ref<AsyncStream> HttpServiceContext::getIO()
	{
		Ref<HttpServiceConnection> connection = m_connection;
		if (connection.isNotNull()) {
			return connection->getIO();
		}
		return sl_null;
	}

	Ref<AsyncIoLoop> HttpServiceContext::getAsyncIoLoop()
	{
		Ref<HttpService> service = getService();
		if (service.isNotNull()) {
			return service->getAsyncIoLoop();
		}
		return sl_null;
	}

	const SocketAddress& HttpServiceContext::getLocalAddress()
	{
		Ref<HttpServiceConnection> connection = getConnection();
		if (connection.isNotNull()) {
			return connection->getLocalAddress();
		} else {
			return SocketAddress::none();
		}
	}

	const SocketAddress& HttpServiceContext::getRemoteAddress()
	{
		Ref<HttpServiceConnection> connection = getConnection();
		if (connection.isNotNull()) {
			return connection->getRemoteAddress();
		} else {
			return SocketAddress::none();
		}
	}

	sl_bool HttpServiceContext::isAsynchronousResponse()
	{
		return m_flagAsynchronousResponse;
	}

	void HttpServiceContext::setAsynchronousResponse(sl_bool flagAsync)
	{
		m_flagAsynchronousResponse = flagAsync;
	}

	void HttpServiceContext::completeResponse()
	{
		Ref<HttpServiceConnection> connection = m_connection;
		if (connection.isNotNull()) {
			connection->_completeResponse(this);
			connection.setNull();
		}
	}

/******************************************************
			HttpServiceConnection
******************************************************/
#define SIZE_READ_BUF 0x10000
#define SIZE_COPY_BUF 0x10000

	HttpServiceConnection::HttpServiceConnection()
	{
		m_flagClosed = sl_true;
		m_flagReading = sl_false;
		m_flagKeepAlive = sl_true;
	}

	HttpServiceConnection::~HttpServiceConnection()
	{
		close();
	}

	Ref<HttpServiceConnection> HttpServiceConnection::create(HttpService* service, AsyncStream* io)
	{
		if (service && io) {
			Memory bufRead = Memory::create(SIZE_READ_BUF);
			if (bufRead.isNotNull()) {
				Ref<HttpServiceConnection> ret = new HttpServiceConnection;
				if (ret.isNotNull()) {
					AsyncOutputParam op;
					op.stream = io;
					op.onEnd = SLIB_FUNCTION_WEAKREF(HttpServiceConnection, onAsyncOutputEnd, ret);
					op.bufferSize = SIZE_COPY_BUF;
					Ref<AsyncOutput> output = AsyncOutput::create(op);
					if (output.isNotNull()) {
						ret->m_service = service;
						ret->m_io = io;
						ret->m_output = output;
						ret->m_bufRead = bufRead;
						ret->m_flagClosed = sl_false;
						return ret;
					}
				}
			}
		}
		return sl_null;
	}

	void HttpServiceConnection::close()
	{
		ObjectLocker lock(this);
		if (m_flagClosed) {
			return;
		}
		m_flagClosed = sl_true;
		
		Ref<HttpService> service = m_service;
		if (service.isNotNull()) {
			service->closeConnection(this);
		}
		m_io->close();
		m_output->close();
	}

	void HttpServiceConnection::start(const void* data, sl_uint32 size)
	{
		m_contextCurrent.setNull();
		if (data && size > 0) {
			_processInput(data, size);
		} else {
			_read();
		}
	}

	Ref<AsyncStream> HttpServiceConnection::getIO()
	{
		return m_io;
	}

	Ref<HttpService> HttpServiceConnection::getService()
	{
		return m_service;
	}

	Ref<HttpServiceContext> HttpServiceConnection::getCurrentContext()
	{
		return m_contextCurrent;
	}

	void HttpServiceConnection::_read()
	{
		ObjectLocker lock(this);
		if (m_flagClosed) {
			return;
		}
		if (m_flagReading) {
			return;
		}
		m_flagReading = sl_true;
		if (!(m_io->readToMemory(m_bufRead, SLIB_FUNCTION_WEAKREF(HttpServiceConnection, onReadStream, this)))) {
			m_flagReading = sl_false;
			close();
		}
	}

	void HttpServiceConnection::_processInput(const void* _data, sl_uint32 size)
	{
		Ref<HttpService> service = m_service;
		if (service.isNull()) {
			return;
		}
		if (m_flagClosed) {
			return;
		}
		
		const HttpServiceParam& param = service->getParam();
		sl_uint64 maxRequestHeadersSize = param.maxRequestHeadersSize;
		sl_uint64 maxRequestBodySize = param.maxRequestBodySize;

		char* data = (char*)_data;
		Ref<HttpServiceContext> _context = m_contextCurrent;
		if (_context.isNull()) {
			_context = HttpServiceContext::create(this);
			if (_context.isNull()) {
				sendResponse_ServerError();
				return;
			}
			m_contextCurrent = _context;
			_context->setProcessingByThread(param.flagProcessByThreads);
		}
		HttpServiceContext* context = _context.get();
		if (context->m_requestHeader.isNull()) {
			sl_size posBody;
			if (context->m_requestHeaderReader.add(data, size, posBody)) {
				context->m_requestHeader = context->m_requestHeaderReader.mergeHeader();
				if (context->m_requestHeader.isNull()) {
					sendResponse_ServerError();
					return;
				}
				if (posBody > size) {
					sendResponse_ServerError();
					return;
				}
				context->m_requestHeaderReader.clear();
				Memory header = context->getRawRequestHeader();
				sl_reg iRet = context->parseRequestPacket(header.getData(), header.getSize());
				if (iRet != (sl_reg)(context->m_requestHeader.getSize())) {
					sendResponse_BadRequest();
					return;
				}
				context->m_requestContentLength = context->getRequestContentLengthHeader();
				if (context->m_requestContentLength > maxRequestBodySize) {
					sendResponse_BadRequest();
					return;
				}
				context->m_requestBody = Memory::create(data + posBody, size - (sl_uint32)posBody);
				if (!(context->m_requestBodyBuffer.add(context->m_requestBody))) {
					sendResponse_ServerError();
					return;
				}
				context->applyQueryToParameters();
				if (service->preprocessRequest(context)) {
					return;
				}
			} else {
				if (context->m_requestHeaderReader.getHeaderSize() > maxRequestHeadersSize) {
					sendResponse_BadRequest();
					return;
				}
			}
		} else {
			if (!(context->m_requestBodyBuffer.add(Memory::create(data, size)))) {
				sendResponse_ServerError();
				return;
			}
		}
		
		if (context->m_requestHeader.isNotNull()) {
			
			if (context->m_requestBodyBuffer.getSize() >= context->m_requestContentLength) {

				m_contextCurrent.setNull();

				context->m_requestBody = context->m_requestBodyBuffer.merge();
				if (context->m_requestContentLength > 0 && context->m_requestBody.isNull()) {
					sendResponse_ServerError();
					return;
				}
				context->m_requestBodyBuffer.clear();

				String multipartBoundary = context->getRequestMultipartFormDataBoundary();
				if (multipartBoundary.isNotEmpty()) {
					Memory body = context->getRequestBody();
					context->applyMultipartFormData(multipartBoundary, body);
				} else if (context->getMethod() == HttpMethod::POST) {
					String reqContentType = context->getRequestContentTypeNoParams();
					if (reqContentType == ContentTypes::WebForm) {
						Memory body = context->getRequestBody();
						context->applyPostParameters(body.getData(), body.getSize());
					}
				}
				
				if (context->isProcessingByThread()) {
					Ref<ThreadPool> threadPool = service->getThreadPool();
					if (threadPool.isNotNull()) {
						threadPool->addTask(SLIB_BIND_WEAKREF(void(), HttpServiceConnection, _processContext, this, _context));
					} else {
						sendResponse_ServerError();
					}
				} else {
					_processContext(context);
				}
				return;
			}
		}
		_read();
	}

	void HttpServiceConnection::_processContext(const Ref<HttpServiceContext>& context)
	{
		Ref<HttpService> service = getService();
		if (service.isNull()) {
			return;
		}
		if (context->getMethod() == HttpMethod::CONNECT) {
			sendConnectResponse_Failed();
			return;
		}
		service->processRequest(context.get());
		if (!(context->isAsynchronousResponse())) {
			context->completeResponse();
		}
	}

	void HttpServiceConnection::_completeResponse(HttpServiceContext* context)
	{
		context->setResponseHeader(HttpHeaders::ContentLength, String::fromUint64(context->getResponseContentLength()));
		String oldResponseContentType = context->getResponseContentType();
		if (oldResponseContentType.isEmpty()) {
			context->setResponseContentType(ContentTypes::TextHtml_Utf8);
		}
		Memory header = context->makeResponsePacket();
		if (header.isNull()) {
			close();
			return;
		}
		if (!(m_output->write(header))) {
			close();
			return;
		}
		m_output->mergeBuffer(&(context->m_bufferOutput));
		if (context->isKeepAlive()) {
			m_output->startWriting();
			start();
		} else {
			m_contextCurrent.setNull();
			m_flagKeepAlive = sl_false;
			m_output->startWriting();
		}
	}

	void HttpServiceConnection::onReadStream(AsyncStreamResult* result)
	{
		m_flagReading = sl_false;
		if (result->flagError) {
			close();
		} else {
			_processInput(result->data, result->size);
		}
	}

	void HttpServiceConnection::onAsyncOutputEnd(AsyncOutput* output, sl_bool flagError)
	{
		if (flagError || !m_flagKeepAlive) {
			close();
		}
	}

	void HttpServiceConnection::sendResponse(const Memory& mem)
	{
		if (mem.isNotNull()) {
			if (m_io->writeFromMemory(mem, sl_null)) {
				return;
			}
		}
		close();
	}

	void HttpServiceConnection::sendResponseAndRestart(const Memory& mem)
	{
		if (mem.isNotNull()) {
			if (m_io->writeFromMemory(mem, sl_null)) {
				start();
				return;
			}
		}
		close();
	}

	class _priv_HttpServiceConnection_SendResponseAndCloseListener : public Referable
	{
	public:
		WeakRef<HttpServiceConnection> m_connection;

		_priv_HttpServiceConnection_SendResponseAndCloseListener(HttpServiceConnection* connection)
		{
			m_connection = connection;
		}

		void onWriteStream(AsyncStreamResult* result)
		{
			Ref<HttpServiceConnection> connection = m_connection;
			if (connection.isNotNull()) {
				connection->close();
			}
		}
	};

	void HttpServiceConnection::sendResponseAndClose(const Memory& mem)
	{
		if (mem.isNotNull()) {
			Ref<_priv_HttpServiceConnection_SendResponseAndCloseListener> listener(new _priv_HttpServiceConnection_SendResponseAndCloseListener(this));
			if (m_io->writeFromMemory(mem, SLIB_FUNCTION_REF(_priv_HttpServiceConnection_SendResponseAndCloseListener, onWriteStream, listener))) {
				return;
			}
		}
		close();
	}

	void HttpServiceConnection::sendResponse_BadRequest()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
		sendResponseAndRestart(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServiceConnection::sendResponse_ServerError()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n");
		sendResponseAndRestart(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServiceConnection::sendConnectResponse_Successed()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 200 Connection established\r\n\r\n");
		sendResponse(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServiceConnection::sendConnectResponse_Failed()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 500 Tunneling is not supported\r\n\r\n");
		sendResponseAndClose(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServiceConnection::sendProxyResponse_Failed()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 500 Internal Error\r\nContent-Length: 0\r\n\r\n");
		sendResponseAndRestart(Memory::create(s.getData(), s.getLength()));
	}

/******************************************************
			HttpServiceConnectionProvider
******************************************************/

	HttpServiceConnectionProvider::HttpServiceConnectionProvider()
	{
	}

	HttpServiceConnectionProvider::~HttpServiceConnectionProvider()
	{
	}

	Ref<HttpService> HttpServiceConnectionProvider::getService()
	{
		return m_service;
	}

	void HttpServiceConnectionProvider::setService(const Ref<HttpService>& service)
	{
		m_service = service;
	}

	class _priv_DefaultHttpServiceConnectionProvider : public HttpServiceConnectionProvider
	{
	public:
		Ref<AsyncTcpServer> m_server;
		Ref<AsyncIoLoop> m_loop;

	public:
		_priv_DefaultHttpServiceConnectionProvider()
		{
		}

		~_priv_DefaultHttpServiceConnectionProvider()
		{
			release();
		}

	public:
		static Ref<HttpServiceConnectionProvider> create(HttpService* service, const SocketAddress& addressListen)
		{
			Ref<AsyncIoLoop> loop = service->getAsyncIoLoop();
			if (loop.isNotNull()) {
				Ref<_priv_DefaultHttpServiceConnectionProvider> ret = new _priv_DefaultHttpServiceConnectionProvider;
				if (ret.isNotNull()) {
					ret->m_loop = loop;
					ret->setService(service);
					AsyncTcpServerParam sp;
					sp.bindAddress = addressListen;
					sp.onAccept = SLIB_FUNCTION_WEAKREF(_priv_DefaultHttpServiceConnectionProvider, onAccept, ret);
					sp.ioLoop = loop;
					Ref<AsyncTcpServer> server = AsyncTcpServer::create(sp);
					if (server.isNotNull()) {
						ret->m_server = server;
						return ret;
					}
				}
			}
			return sl_null;
		}
		
		void release()
		{
			ObjectLocker lock(this);
			if (m_server.isNotNull()) {
				m_server->close();
			}
		}

		void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address)
		{
			Ref<HttpService> service = getService();
			if (service.isNotNull()) {
				Ref<AsyncIoLoop> loop = m_loop;
				if (loop.isNull()) {
					return;
				}
				AsyncTcpSocketParam cp;
				cp.socket = socketAccept;
				cp.ioLoop = loop;
				Ref<AsyncTcpSocket> stream = AsyncTcpSocket::create(cp);
				if (stream.isNotNull()) {
					SocketAddress addrLocal;
					socketAccept->getLocalAddress(addrLocal);
					service->addConnection(stream.get(), address, addrLocal);
				}
			}
		}

	};

/******************************************************
					HttpService
******************************************************/

	HttpServiceParam::HttpServiceParam()
	{
		port = 8080;
		
		maxThreadsCount = 32;
		flagProcessByThreads = sl_true;
		
		flagUseWebRoot = sl_false;
		flagUseAsset = sl_false;
		
		maxRequestHeadersSize = 0x10000; // 64KB
		maxRequestBodySize = 0x2000000; // 32MB
		
		flagAllowCrossOrigin = sl_false;
		
		flagUseCacheControl = sl_true;
		flagCacheControlNoCache = sl_false;
		cacheControlMaxAge = 600;
		
		flagLogDebug = sl_false;
	}

	HttpServiceParam::HttpServiceParam(const HttpServiceParam& other) = default;

	HttpServiceParam::~HttpServiceParam()
	{
	}
	
	void HttpServiceParam::setJson(const Json& conf)
	{
		port = (sl_uint16)(conf["port"].getUint32(port));
		{
			String s = conf["root"].getString();
			if (s.isNotNull()) {
				webRootPath = s;
				flagUseWebRoot = sl_true;
			}
		}
		{
			List<String> s;
			conf["allowed_file_extensions"].get(s);
			if (s.isNotNull()) {
				allowedFileExtensions = s;
			}
		}
		{
			List<String> s;
			conf["blocked_file_extensions"].get(s);
			if (s.isNotNull()) {
				blockedFileExtensions = s;
			}
		}
		
		Json cacheControl = conf["cache_control"];
		if (cacheControl.isNotNull()) {
			flagUseCacheControl = sl_true;
			flagCacheControlNoCache = cacheControl["no_cache"].getBoolean(flagCacheControlNoCache);
			cacheControlMaxAge = cacheControl["max_age"].getUint32(cacheControlMaxAge);
		}
		
		{
			sl_uint32 n;
			if (conf["max_request_body"].getString().parseUint32(10, &n)) {
				maxRequestBodySize = n * 1024 * 1024;
			}
		}
	}
	
	sl_bool HttpServiceParam::parseJsonFile(const String& filePath)
	{
		Json json = Json::parseJsonFromTextFile(filePath);
		if (json.isNotNull()) {
			setJson(json);
			return sl_true;
		} else {
			return sl_false;
		}
	}


	SLIB_DEFINE_OBJECT(HttpService, Object)

	HttpService::HttpService()
	{
		m_flagRunning = sl_true;
	}

	HttpService::~HttpService()
	{
		release();
	}

	sl_bool HttpService::_init(const HttpServiceParam& param)
	{
		Ref<AsyncIoLoop> ioLoop = AsyncIoLoop::create(sl_false);
		
		if (ioLoop.isNotNull()) {
			
			Ref<ThreadPool> threadPool = ThreadPool::create();
			
			if (threadPool.isNotNull()) {
				
				threadPool->setMaximumThreadsCount(param.maxThreadsCount);
				
				m_ioLoop = ioLoop;
				m_threadPool = threadPool;
				m_param = param;
				if (param.port) {
					if (! (addHttpService(param.addressBind, param.port))) {
						return sl_false;
					}
				}
				
				ioLoop->start();

				return sl_true;
			}
		}
		return sl_false;
	}

	Ref<HttpService> HttpService::create(const HttpServiceParam& param)
	{
		Ref<HttpService> ret = new HttpService;
		if (ret.isNotNull()) {
			if (ret->_init(param)) {
				return ret;
			}
		}
		return sl_null;
	}

	void HttpService::release()
	{
		ObjectLocker lock(this);
		
		if (!m_flagRunning) {
			return;
		}
		
		m_flagRunning = sl_false;
		
		{
			ListLocker< Ref<HttpServiceConnectionProvider> > cp(m_connectionProviders);
			for (sl_size i = 0; i < cp.count; i++) {
				cp[i]->release();
			}
		}
		m_connectionProviders.removeAll();
		
		Ref<AsyncIoLoop> ioLoop = m_ioLoop;
		if (ioLoop.isNotNull()) {
			ioLoop->release();
			m_ioLoop.setNull();
		}
		Ref<ThreadPool> threadPool = m_threadPool;
		if (threadPool.isNotNull()) {
			threadPool->release();
			m_threadPool.setNull();
		}
		
		m_connections.removeAll();
	}

	sl_bool HttpService::isRunning()
	{
		return m_flagRunning;
	}

	Ref<AsyncIoLoop> HttpService::getAsyncIoLoop()
	{
		return m_ioLoop;
	}

	Ref<ThreadPool> HttpService::getThreadPool()
	{
		return m_threadPool;
	}

	const HttpServiceParam& HttpService::getParam()
	{
		return m_param;
	}

	sl_bool HttpService::preprocessRequest(const Ref<HttpServiceContext>& context)
	{
		return sl_false;
	}

	void HttpService::processRequest(const Ref<HttpServiceContext>& context)
	{
		Ref<HttpServiceConnection> connection = context->getConnection();
		if (connection.isNull()) {
			return;
		}
		if (m_param.flagLogDebug) {
			Log(SERVICE_TAG, "[%s] Method=%s Path=%s Query=%s Host=%s",
				String::fromPointerValue(connection.get()),
				context->getMethodText(),
				context->getPath(),
				context->getQuery(),
				context->getHost());
		}
		
		do {
			
			if (dispatchRequest(context.get())) {
				break;
			}
		
			sl_bool flagProcessAsResource = m_param.flagUseWebRoot || m_param.flagUseAsset;
			if (flagProcessAsResource && (m_param.allowedFileExtensions.isNotEmpty() || m_param.blockedFileExtensions.isNotEmpty())) {
				String ext = File::getFileExtension(context->getPath()).trim();
				if (m_param.blockedFileExtensions.isNotEmpty()) {
					if (m_param.blockedFileExtensions.contains(ext)) {
						flagProcessAsResource = sl_false;
					}
				} else if (m_param.allowedFileExtensions.isNotEmpty()) {
					if (!(m_param.allowedFileExtensions.contains(ext))) {
						flagProcessAsResource = sl_false;
					}
				}
			}
			
			if (flagProcessAsResource) {
				if (m_param.flagUseWebRoot) {
					if (context->getMethod() == HttpMethod::GET) {
						String path = context->getPath();
						if (path.startsWith('/')) {
							path = path.substring(1);
						}
						FilePathSegments seg;
						seg.parsePath(path);
						if (seg.parentLevel == 0) {
							String webRootPath = m_param.webRootPath;
							if (webRootPath.isEmpty()) {
								webRootPath = Application::getApplicationDirectory();
							}
							path = webRootPath + "/" + path;
							if (processFile(context, path)) {
								break;
							}
							if (path.endsWith('/')) {
								if (processFile(context, path + "index.html")) {
									break;
								}
								if (processFile(context, path + "index.htm")) {
									break;
								}
							}
						}
					}
				}
				
				if (m_param.flagUseAsset) {
					if (context->getMethod() == HttpMethod::GET) {
						String path = context->getPath();
						if (path.startsWith('/')) {
							path = path.substring(1);
						}
						path = m_param.prefixAsset + path;
						if (processAsset(context, path)) {
							break;
						}
						if (path.endsWith('/')) {
							if (processAsset(context, path + "index.html")) {
								break;
							}
							if (processAsset(context, path + "index.htm")) {
								break;
							}
						}
					}
				}
			}
			
			dispatchPostRequest(context.get(), sl_false);
			return;
		
		} while (0);
		
		dispatchPostRequest(context.get(), sl_true);
		
	}

	sl_bool HttpService::processAsset(const Ref<HttpServiceContext>& context, const String& path)
	{
		FilePathSegments seg;
		seg.parsePath(path);
		if (seg.parentLevel == 0) {
			String ext = File::getFileExtension(path);
			if (Assets::isBasedOnFileSystem()) {
				String filePath = Assets::getFilePath(path);
				return processFile(context, filePath);
			} else {
				Memory mem = Assets::readAllBytes(path);
				if (mem.isNotNull()) {
					String oldResponseContentType = context->getResponseContentType();
					if (oldResponseContentType.isEmpty()) {
						ContentType contentType = ContentTypes::getFromFileExtension(ext);
						if (contentType == ContentType::Unknown) {
							contentType = ContentType::OctetStream;
						}
						context->setResponseContentType(contentType);
					}
					_processCacheControl(context);
					context->write(mem);
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	sl_bool HttpService::processFile(const Ref<HttpServiceContext>& context, const String& path)
	{
		if (File::exists(path) && !(File::isDirectory(path))) {

			sl_uint64 totalSize = File::getSize(path);

			String ext = File::getFileExtension(path);
			
			String oldResponseContentType = context->getResponseContentType();
			if (oldResponseContentType.isEmpty()) {
				ContentType contentType = ContentTypes::getFromFileExtension(ext);
				if (contentType == ContentType::Unknown) {
					contentType = ContentType::OctetStream;
				}
				context->setResponseContentType(contentType);
			}

			context->setResponseAcceptRanges(sl_true);
			
			_processCacheControl(context);

			Time lastModifiedTime = File::getModifiedTime(path);
			context->setResponseLastModified(lastModifiedTime);
			Time ifModifiedSince = context->getRequestIfModifiedSince();
			if (ifModifiedSince.isNotZero() && ifModifiedSince == lastModifiedTime) {
				context->setResponseCode(HttpStatus::NotModified);
				return sl_true;
			}
			
			String rangeHeader = context->getRequestRange();
			
			if (rangeHeader.isNotEmpty()) {
				
				sl_uint64 start;
				sl_uint64 len;
				
				if (processRangeRequest(context, totalSize, rangeHeader, start, len)) {

					Ref<AsyncFile> file = AsyncFile::openForRead(path, m_threadPool);
					if (file.isNotNull()) {
						file->seek(start);
						context->copyFrom(file.get(), len);
						return sl_true;
					}
					
				} else {
					return sl_true;
				}
				
			} else {
				if (totalSize > 100000) {
					context->copyFromFile(path, m_threadPool);
					return sl_true;
				} else {
					Memory mem = File::readAllBytes(path);
					if (mem.isNotNull()) {
						context->write(mem);
						return sl_true;
					}
				}
			}
			
		}
		return sl_false;
	}
	
	void HttpService::_processCacheControl(const Ref<HttpServiceContext>& context)
	{
		if (m_param.flagUseCacheControl) {
			HttpCacheControlResponse cc;
			if (m_param.flagCacheControlNoCache) {
				cc.no_cache = sl_true;
			} else {
				cc.max_age = m_param.cacheControlMaxAge;
			}
			context->setResponseCacheControl(cc);
		}
	}

	sl_bool HttpService::processRangeRequest(const Ref<HttpServiceContext>& context, sl_uint64 totalLength, const String& range, sl_uint64& outStart, sl_uint64& outLength)
	{
		if (range.getLength() < 2 || !(range.startsWith("bytes="))) {
			context->setResponseCode(HttpStatus::BadRequest);
			return sl_false;
		}
		sl_reg indexSplit = range.indexOf('-');
		if (indexSplit < 0) {
			context->setResponseCode(HttpStatus::BadRequest);
			return sl_false;
		}
		String s1 = range.substring(6, indexSplit);
		String s2 = range.substring(indexSplit+1);
		sl_uint64 n1 = 0;
		sl_uint64 n2 = 0;
		if (s1.isNotEmpty()) {
			if (!(s1.parseUint64(10, &n1))) {
				context->setResponseCode(HttpStatus::BadRequest);
				return sl_false;
			}
		}
		if (s2.isNotEmpty()) {
			if (!(s2.parseUint64(10, &n2))) {
				context->setResponseCode(HttpStatus::BadRequest);
				return sl_false;
			}
		}
		if (indexSplit == 0) {
			if (n2 == 0) {
				context->setResponseCode(HttpStatus::NoContent);
				return sl_false;
			}
			if (n2 > totalLength) {
				context->setResponseCode(HttpStatus::RequestRangeNotSatisfiable);
				context->setResponseContentRangeUnsatisfied(totalLength);
				return sl_false;
			}
			outStart = totalLength - n2;
			outLength = totalLength - 1;
		} else {
			if (n1 >= totalLength) {
				context->setResponseCode(HttpStatus::RequestRangeNotSatisfiable);
				context->setResponseContentRangeUnsatisfied(totalLength);
				return sl_false;
			}
			if (indexSplit == (sl_reg)(range.getLength()) - 1) {
				outLength = totalLength - n1;
			} else {
				if (n2 >= totalLength) {
					context->setResponseCode(HttpStatus::RequestRangeNotSatisfiable);
					context->setResponseContentRangeUnsatisfied(totalLength);
					return sl_false;
				}
				outLength = n2 - n1 + 1;
			}
			outStart = n1;
		}
		context->setResponseContentRange(outStart, outStart + outLength - 1, totalLength);
		context->setResponseCode(HttpStatus::PartialContent);
		return sl_true;
	}
	
	sl_bool HttpService::onRequest(HttpServiceContext* context)
	{
		return sl_false;
	}
	
	sl_bool HttpService::dispatchRequest(HttpServiceContext* context)
	{
		if (m_param.onRequest.isNotNull()) {
			if (m_param.onRequest(this, context)) {
				return sl_true;
			}
		}
		return onRequest(context);
	}
	
	void HttpService::onPostRequest(HttpServiceContext* context, sl_bool flagProcessed)
	{
	}
	
	void HttpService::dispatchPostRequest(HttpServiceContext* context, sl_bool flagProcessed)
	{
		if (!flagProcessed) {
			context->setResponseCode(HttpStatus::NotFound);
		}
		m_param.onPostRequest(this, context, flagProcessed);
		onPostRequest(context, flagProcessed);
		if (m_param.flagAllowCrossOrigin) {
			context->setResponseAccessControlAllowOrigin("*");
			if (!flagProcessed && context->getMethod() == HttpMethod::OPTIONS) {
				context->setResponseCode(HttpStatus::OK);
			}
		}
	}

	Ref<HttpServiceConnection> HttpService::addConnection(const Ref<AsyncStream>& stream, const SocketAddress& remoteAddress, const SocketAddress& localAddress)
	{
		Ref<HttpServiceConnection> connection = HttpServiceConnection::create(this, stream.get());
		if (connection.isNotNull()) {
			if (m_param.flagLogDebug) {
				Log(SERVICE_TAG, "[%s] Connection Created - Address: %s",
					String::fromPointerValue(connection.get()),
					remoteAddress.toString());
			}
			connection->setRemoteAddress(remoteAddress);
			connection->setLocalAddress(localAddress);
			m_connections.put(connection.get(), connection);
			connection->start();
		}
		return connection;
	}

	void HttpService::closeConnection(HttpServiceConnection* connection)
	{
		if (m_param.flagLogDebug) {
			Log(SERVICE_TAG, "[%s] Connection Closed", String::fromPointerValue(connection));
		}
		m_connections.remove(connection);
	}

	void HttpService::addConnectionProvider(const Ref<HttpServiceConnectionProvider>& provider)
	{
		m_connectionProviders.add(provider);
	}

	void HttpService::removeConnectionProvider(const Ref<HttpServiceConnectionProvider>& provider)
	{
		m_connectionProviders.remove(provider);
	}

	sl_bool HttpService::addHttpService(const SocketAddress& addr)
	{
		Ref<HttpServiceConnectionProvider> provider = _priv_DefaultHttpServiceConnectionProvider::create(this, addr);
		if (provider.isNotNull()) {
			addConnectionProvider(provider);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool HttpService::addHttpService(sl_uint32 port)
	{
		return addHttpService(SocketAddress(port));
	}

	sl_bool HttpService::addHttpService(const IPAddress& addr, sl_uint32 port)
	{
		return addHttpService(SocketAddress(addr, port));
	}

}
