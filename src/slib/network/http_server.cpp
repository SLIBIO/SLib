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

#include "slib/network/http_server.h"

#include "slib/network/url.h"
#include "slib/core/app.h"
#include "slib/core/asset.h"
#include "slib/core/file.h"
#include "slib/core/log.h"
#include "slib/core/json.h"
#include "slib/core/content_type.h"

#define SERVER_TAG "HTTP SERVER"

namespace slib
{

/**********************************************
			HttpServerContext
**********************************************/

	SLIB_DEFINE_OBJECT(HttpServerContext, Object)

	HttpServerContext::HttpServerContext()
	{
		m_requestContentLength = 0;
		m_flagAsynchronousResponse = sl_false;

		setClosingConnection(sl_false);
		setProcessingByThread(sl_true);
	}

	HttpServerContext::~HttpServerContext()
	{
	}

	Ref<HttpServerContext> HttpServerContext::create(const Ref<HttpServerConnection>& connection)
	{
		Ref<HttpServerContext> ret;
		if (connection.isNotNull()) {
			ret = new HttpServerContext;
			if (ret.isNotNull()) {
				ret->m_connection = connection;
			}
		}
		return ret;
	}

	Memory HttpServerContext::getRawRequestHeader() const
	{
		return m_requestHeader;
	}

	sl_uint64 HttpServerContext::getRequestContentLength() const
	{
		return m_requestContentLength;
	}

	Memory HttpServerContext::getRequestBody() const
	{
		return m_requestBody;
	}

	Variant HttpServerContext::getRequestBodyAsJson() const
	{
		return Json::parseJson16Utf8(m_requestBody);
	}

	sl_uint64 HttpServerContext::getResponseContentLength() const
	{
		return getOutputLength();
	}

	Ref<HttpServer> HttpServerContext::getServer()
	{
		Ref<HttpServerConnection> connection = getConnection();
		if (connection.isNotNull()) {
			return connection->getServer();
		}
		return sl_null;
	}

	Ref<HttpServerConnection> HttpServerContext::getConnection()
	{
		return m_connection;
	}

	Ref<AsyncStream> HttpServerContext::getIO()
	{
		Ref<HttpServerConnection> connection = m_connection;
		if (connection.isNotNull()) {
			return connection->getIO();
		}
		return sl_null;
	}

	Ref<AsyncIoLoop> HttpServerContext::getAsyncIoLoop()
	{
		Ref<HttpServer> server = getServer();
		if (server.isNotNull()) {
			return server->getAsyncIoLoop();
		}
		return sl_null;
	}

	const SocketAddress& HttpServerContext::getLocalAddress()
	{
		Ref<HttpServerConnection> connection = getConnection();
		if (connection.isNotNull()) {
			return connection->getLocalAddress();
		} else {
			return SocketAddress::none();
		}
	}

	const SocketAddress& HttpServerContext::getRemoteAddress()
	{
		Ref<HttpServerConnection> connection = getConnection();
		if (connection.isNotNull()) {
			return connection->getRemoteAddress();
		} else {
			return SocketAddress::none();
		}
	}

	sl_bool HttpServerContext::isAsynchronousResponse()
	{
		return m_flagAsynchronousResponse;
	}

	void HttpServerContext::setAsynchronousResponse(sl_bool flagAsync)
	{
		m_flagAsynchronousResponse = flagAsync;
	}

	void HttpServerContext::completeResponse()
	{
		Ref<HttpServerConnection> connection = m_connection;
		if (connection.isNotNull()) {
			connection->_completeResponse(this);
			connection.setNull();
		}
	}

/******************************************************
			HttpServerConnection
******************************************************/
#define SIZE_READ_BUF 0x10000
#define SIZE_COPY_BUF 0x10000
	
	SLIB_DEFINE_OBJECT(HttpServerConnection, Object)

	HttpServerConnection::HttpServerConnection()
	{
		m_flagClosed = sl_true;
		m_flagReading = sl_false;
		m_flagKeepAlive = sl_true;
	}

	HttpServerConnection::~HttpServerConnection()
	{
		close();
	}

	Ref<HttpServerConnection> HttpServerConnection::create(HttpServer* server, AsyncStream* io)
	{
		if (server && io) {
			Memory bufRead = Memory::create(SIZE_READ_BUF);
			if (bufRead.isNotNull()) {
				Ref<HttpServerConnection> ret = new HttpServerConnection;
				if (ret.isNotNull()) {
					AsyncOutputParam op;
					op.stream = io;
					op.onEnd = SLIB_FUNCTION_WEAKREF(HttpServerConnection, onAsyncOutputEnd, ret);
					op.bufferSize = SIZE_COPY_BUF;
					Ref<AsyncOutput> output = AsyncOutput::create(op);
					if (output.isNotNull()) {
						ret->m_server = server;
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

	void HttpServerConnection::close()
	{
		ObjectLocker lock(this);
		if (m_flagClosed) {
			return;
		}
		m_flagClosed = sl_true;
		
		Ref<HttpServer> server = m_server;
		if (server.isNotNull()) {
			server->closeConnection(this);
		}
		m_io->close();
		m_output->close();
	}

	void HttpServerConnection::start(const void* data, sl_uint32 size)
	{
		m_contextCurrent.setNull();
		if (data && size > 0) {
			_processInput(data, size);
		} else {
			_read();
		}
	}

	Ref<AsyncStream> HttpServerConnection::getIO()
	{
		return m_io;
	}

	Ref<HttpServer> HttpServerConnection::getServer()
	{
		return m_server;
	}

	Ref<HttpServerContext> HttpServerConnection::getCurrentContext()
	{
		return m_contextCurrent;
	}

	void HttpServerConnection::_read()
	{
		ObjectLocker lock(this);
		if (m_flagClosed) {
			return;
		}
		if (m_flagReading) {
			return;
		}
		m_flagReading = sl_true;
		if (!(m_io->readToMemory(m_bufRead, SLIB_FUNCTION_WEAKREF(HttpServerConnection, onReadStream, this)))) {
			m_flagReading = sl_false;
			close();
		}
	}

	void HttpServerConnection::_processInput(const void* _data, sl_uint32 size)
	{
		Ref<HttpServer> server = m_server;
		if (server.isNull()) {
			return;
		}
		if (m_flagClosed) {
			return;
		}
		
		const HttpServerParam& param = server->getParam();
		sl_uint64 maxRequestHeadersSize = param.maxRequestHeadersSize;
		sl_uint64 maxRequestBodySize = param.maxRequestBodySize;

		char* data = (char*)_data;
		Ref<HttpServerContext> _context = m_contextCurrent;
		if (_context.isNull()) {
			_context = HttpServerContext::create(this);
			if (_context.isNull()) {
				sendResponse_ServerError();
				return;
			}
			m_contextCurrent = _context;
			_context->setProcessingByThread(param.flagProcessByThreads);
		}
		HttpServerContext* context = _context.get();
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
				if (server->preprocessRequest(context)) {
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
					Ref<ThreadPool> threadPool = server->getThreadPool();
					if (threadPool.isNotNull()) {
						threadPool->addTask(SLIB_BIND_WEAKREF(void(), HttpServerConnection, _processContext, this, _context));
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

	void HttpServerConnection::_processContext(const Ref<HttpServerContext>& context)
	{
		Ref<HttpServer> server = getServer();
		if (server.isNull()) {
			return;
		}
		if (context->getMethod() == HttpMethod::CONNECT) {
			sendConnectResponse_Failed();
			return;
		}
		server->processRequest(context.get());
		if (!(context->isAsynchronousResponse())) {
			context->completeResponse();
		}
	}

	void HttpServerConnection::_completeResponse(HttpServerContext* context)
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

	void HttpServerConnection::onReadStream(AsyncStreamResult* result)
	{
		m_flagReading = sl_false;
		if (result->flagError) {
			close();
		} else {
			_processInput(result->data, result->size);
		}
	}

	void HttpServerConnection::onAsyncOutputEnd(AsyncOutput* output, sl_bool flagError)
	{
		if (flagError || !m_flagKeepAlive) {
			close();
		}
	}

	void HttpServerConnection::sendResponse(const Memory& mem)
	{
		if (mem.isNotNull()) {
			if (m_io->writeFromMemory(mem, sl_null)) {
				return;
			}
		}
		close();
	}

	void HttpServerConnection::sendResponseAndRestart(const Memory& mem)
	{
		if (mem.isNotNull()) {
			if (m_io->writeFromMemory(mem, sl_null)) {
				start();
				return;
			}
		}
		close();
	}

	class _priv_HttpServerConnection_SendResponseAndCloseListener : public Referable
	{
	public:
		WeakRef<HttpServerConnection> m_connection;

		_priv_HttpServerConnection_SendResponseAndCloseListener(HttpServerConnection* connection)
		{
			m_connection = connection;
		}

		void onWriteStream(AsyncStreamResult* result)
		{
			Ref<HttpServerConnection> connection = m_connection;
			if (connection.isNotNull()) {
				connection->close();
			}
		}
	};

	void HttpServerConnection::sendResponseAndClose(const Memory& mem)
	{
		if (mem.isNotNull()) {
			Ref<_priv_HttpServerConnection_SendResponseAndCloseListener> listener(new _priv_HttpServerConnection_SendResponseAndCloseListener(this));
			if (m_io->writeFromMemory(mem, SLIB_FUNCTION_REF(_priv_HttpServerConnection_SendResponseAndCloseListener, onWriteStream, listener))) {
				return;
			}
		}
		close();
	}

	void HttpServerConnection::sendResponse_BadRequest()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
		sendResponseAndRestart(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServerConnection::sendResponse_ServerError()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n");
		sendResponseAndRestart(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServerConnection::sendConnectResponse_Successed()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 200 Connection established\r\n\r\n");
		sendResponse(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServerConnection::sendConnectResponse_Failed()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 500 Tunneling is not supported\r\n\r\n");
		sendResponseAndClose(Memory::create(s.getData(), s.getLength()));
	}

	void HttpServerConnection::sendProxyResponse_Failed()
	{
		SLIB_STATIC_STRING(s, "HTTP/1.1 500 Internal Error\r\nContent-Length: 0\r\n\r\n");
		sendResponseAndRestart(Memory::create(s.getData(), s.getLength()));
	}

/******************************************************
			HttpServerConnectionProvider
******************************************************/
	
	SLIB_DEFINE_OBJECT(HttpServerConnectionProvider, Object)

	HttpServerConnectionProvider::HttpServerConnectionProvider()
	{
	}

	HttpServerConnectionProvider::~HttpServerConnectionProvider()
	{
	}

	Ref<HttpServer> HttpServerConnectionProvider::getServer()
	{
		return m_server;
	}

	void HttpServerConnectionProvider::setServer(const Ref<HttpServer>& server)
	{
		m_server = server;
	}

	class _priv_DefaultHttpServerConnectionProvider : public HttpServerConnectionProvider
	{
	public:
		Ref<AsyncTcpServer> m_server;
		Ref<AsyncIoLoop> m_loop;

	public:
		_priv_DefaultHttpServerConnectionProvider()
		{
		}

		~_priv_DefaultHttpServerConnectionProvider()
		{
			release();
		}

	public:
		static Ref<HttpServerConnectionProvider> create(HttpServer* server, const SocketAddress& addressListen)
		{
			Ref<AsyncIoLoop> loop = server->getAsyncIoLoop();
			if (loop.isNotNull()) {
				Ref<_priv_DefaultHttpServerConnectionProvider> ret = new _priv_DefaultHttpServerConnectionProvider;
				if (ret.isNotNull()) {
					ret->m_loop = loop;
					ret->setServer(server);
					AsyncTcpServerParam sp;
					sp.bindAddress = addressListen;
					sp.onAccept = SLIB_FUNCTION_WEAKREF(_priv_DefaultHttpServerConnectionProvider, onAccept, ret);
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
			Ref<HttpServer> server = getServer();
			if (server.isNotNull()) {
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
					server->addConnection(stream.get(), address, addrLocal);
				}
			}
		}

	};

/******************************************************
					HttpServer
******************************************************/
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(HttpServerParam)

	HttpServerParam::HttpServerParam()
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

	void HttpServerParam::setJson(const Json& conf)
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
	
	sl_bool HttpServerParam::parseJsonFile(const String& filePath)
	{
		Json json = Json::parseJsonFromTextFile(filePath);
		if (json.isNotNull()) {
			setJson(json);
			return sl_true;
		} else {
			return sl_false;
		}
	}


	SLIB_DEFINE_OBJECT(HttpServer, Object)

	HttpServer::HttpServer()
	{
		m_flagRunning = sl_true;
	}

	HttpServer::~HttpServer()
	{
		release();
	}

	sl_bool HttpServer::_init(const HttpServerParam& param)
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
					if (! (addHttpServer(param.addressBind, param.port))) {
						return sl_false;
					}
				}
				
				ioLoop->start();

				return sl_true;
			}
		}
		return sl_false;
	}

	Ref<HttpServer> HttpServer::create(const HttpServerParam& param)
	{
		Ref<HttpServer> ret = new HttpServer;
		if (ret.isNotNull()) {
			if (ret->_init(param)) {
				return ret;
			}
		}
		return sl_null;
	}

	void HttpServer::release()
	{
		ObjectLocker lock(this);
		
		if (!m_flagRunning) {
			return;
		}
		
		m_flagRunning = sl_false;
		
		{
			ListLocker< Ref<HttpServerConnectionProvider> > cp(m_connectionProviders);
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

	sl_bool HttpServer::isRunning()
	{
		return m_flagRunning;
	}

	Ref<AsyncIoLoop> HttpServer::getAsyncIoLoop()
	{
		return m_ioLoop;
	}

	Ref<ThreadPool> HttpServer::getThreadPool()
	{
		return m_threadPool;
	}

	const HttpServerParam& HttpServer::getParam()
	{
		return m_param;
	}

	sl_bool HttpServer::preprocessRequest(const Ref<HttpServerContext>& context)
	{
		return sl_false;
	}

	void HttpServer::processRequest(const Ref<HttpServerContext>& context)
	{
		Ref<HttpServerConnection> connection = context->getConnection();
		if (connection.isNull()) {
			return;
		}
		if (m_param.flagLogDebug) {
			Log(SERVER_TAG, "[%s] Method=%s Path=%s Query=%s Host=%s",
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

	sl_bool HttpServer::processAsset(const Ref<HttpServerContext>& context, const String& path)
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

	sl_bool HttpServer::processFile(const Ref<HttpServerContext>& context, const String& path)
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
	
	void HttpServer::_processCacheControl(const Ref<HttpServerContext>& context)
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

	sl_bool HttpServer::processRangeRequest(const Ref<HttpServerContext>& context, sl_uint64 totalLength, const String& range, sl_uint64& outStart, sl_uint64& outLength)
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
	
	sl_bool HttpServer::onRequest(HttpServerContext* context)
	{
		return sl_false;
	}
	
	sl_bool HttpServer::dispatchRequest(HttpServerContext* context)
	{
		if (m_param.onRequest.isNotNull()) {
			if (m_param.onRequest(this, context)) {
				return sl_true;
			}
		}
		return onRequest(context);
	}
	
	void HttpServer::onPostRequest(HttpServerContext* context, sl_bool flagProcessed)
	{
	}
	
	void HttpServer::dispatchPostRequest(HttpServerContext* context, sl_bool flagProcessed)
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

	Ref<HttpServerConnection> HttpServer::addConnection(const Ref<AsyncStream>& stream, const SocketAddress& remoteAddress, const SocketAddress& localAddress)
	{
		Ref<HttpServerConnection> connection = HttpServerConnection::create(this, stream.get());
		if (connection.isNotNull()) {
			if (m_param.flagLogDebug) {
				Log(SERVER_TAG, "[%s] Connection Created - Address: %s",
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

	void HttpServer::closeConnection(HttpServerConnection* connection)
	{
		if (m_param.flagLogDebug) {
			Log(SERVER_TAG, "[%s] Connection Closed", String::fromPointerValue(connection));
		}
		m_connections.remove(connection);
	}

	void HttpServer::addConnectionProvider(const Ref<HttpServerConnectionProvider>& provider)
	{
		m_connectionProviders.add(provider);
	}

	void HttpServer::removeConnectionProvider(const Ref<HttpServerConnectionProvider>& provider)
	{
		m_connectionProviders.remove(provider);
	}

	sl_bool HttpServer::addHttpServer(const SocketAddress& addr)
	{
		Ref<HttpServerConnectionProvider> provider = _priv_DefaultHttpServerConnectionProvider::create(this, addr);
		if (provider.isNotNull()) {
			addConnectionProvider(provider);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool HttpServer::addHttpServer(sl_uint32 port)
	{
		return addHttpServer(SocketAddress(port));
	}

	sl_bool HttpServer::addHttpServer(const IPAddress& addr, sl_uint32 port)
	{
		return addHttpServer(SocketAddress(addr, port));
	}

}
