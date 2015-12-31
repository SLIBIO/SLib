#include "../../../inc/slib/network/http_client.h"

#include "../../../inc/slib/core/url.h"

SLIB_NETWORK_NAMESPACE_BEGIN
/******************************************************
	HttpClientContext
******************************************************/
void IHttpClientListener::onHttpError(HttpClientContext* context)
{
}

sl_bool IHttpClientListener::onHttpResponseHeader(HttpClientContext* context)
{
	return sl_false;
}

sl_size IHttpClientListener::onHttpResponsePacket(HttpClientContext* context, void* data, sl_size size)
{
	return size;
}

void IHttpClientListener::onHttpResponse(HttpClientContext* context)
{
}

HttpClientContext::HttpClientContext()
{
	m_flagProcessing = sl_false;
	clearResponse();
}

HttpClientContext::~HttpClientContext()
{
}

void HttpClientContext::clearResponse()
{
	m_responseCode = 0;
	m_responseMessage.setNull();
	clearResponseHeaders();

	m_responseHeaderReader.clear();
	m_responseHeader.setNull();
	m_responseContentLength = 0;
	m_responseBodyBuffer.clear();
	m_responseBody.setNull();
}

void HttpClientContext::setUrl(const String& host, const String& path, const String& query)
{
	setHost(host);
	setPath(path);
	setQuery(query);
}

void HttpClientContext::setUrl(const String& strUrl)
{
	URL url;
	url.parse(strUrl);
	if (url.scheme.isNotEmpty()) {
		setProtocol(url.scheme.toLower());
	}
	if (url.host.isNotEmpty()) {
		setHost(url.host);
	}
	setPath(url.path);
	setQuery(url.query);
}

Ref<HttpClientConnection> HttpClientContext::getConnection()
{
	return m_connection;
}

Ref<HttpClientContext> HttpClientContext::create()
{
	Ref<HttpClientContext> ret = new HttpClientContext;
	return ret;
}

/******************************************************
	HttpClientConnection
******************************************************/
#define SIZE_READ_BUF 0x10000
#define SIZE_COPY_BUF 0x10000

HttpClientConnection::HttpClientConnection()
{
	m_flagClosed = sl_false;
	m_flagReading = sl_false;
}

HttpClientConnection::~HttpClientConnection()
{
	close();
}


Ref<AsyncStream> HttpClientConnection::getIO()
{
	return m_io;
}

Ref<HttpClient> HttpClientConnection::getClient()
{
	return m_client;
}

Ref<HttpClientSession> HttpClientConnection::getSession()
{
	return m_session;
}

Ref<HttpClientConnection> HttpClientConnection::create(HttpClient* _client, HttpClientSession* _session, AsyncStream* _io)
{
	Ref<HttpClientConnection> ret;
	Ref<HttpClient> client = _client;
	Ref<HttpClientSession> session = _session;
	Ref<AsyncStream> io = _io;
	Ref<AsyncOutput> output = AsyncOutput::create(io, WeakRef<HttpClientConnection>(ret), SIZE_COPY_BUF);
	if (client.isNotNull() && session.isNotNull() && io.isNotNull() && output.isNotNull()) {
		Memory bufRead = Memory::create(SIZE_READ_BUF);
		if (bufRead.isNotNull()) {
			ret = new HttpClientConnection;
			if (ret.isNotNull()) {
				ret->m_client = client;
				ret->m_session = session;
				ret->m_io = io;
				ret->m_output = output;
				ret->m_bufRead = bufRead;
			}
		}
	}
	return ret;
}

void HttpClientConnection::close()
{
	MutexLocker lock(getLocker());
	if (m_flagClosed) {
		return;
	}
	m_flagClosed = sl_true;
	Ref<HttpClient> client = getClient();
	if (client.isNotNull()) {
		client->closeConnection(this);
	}
	m_client.setNull();
	Ref<AsyncStream> io = m_io;
	if (io.isNotNull()) {
		io->close();
	}
	m_io.setNull();
	Ref<AsyncOutput> output = m_output;
	if (output.isNotNull()) {
		output->close();
	}
	m_output.setNull();
}

void HttpClientConnection::addContext(const Ref<HttpClientContext>& _context)
{
	Ref<HttpClientContext> context = _context;
	if (context.isNull()) {
		return;
	}
	MutexLocker lock(getLocker());
	if (m_flagClosed) {
		return;
	}
	if (!(m_queueContexts.push(context))) {
		_onError(context.get());
		return;
	}	
	if (context->getMethodUppercase() == SLIB_HTTP_METHOD_POST && context->getRequestContentType().isEmpty()) {
		context->setRequestContentType(SLIB_HTTP_CONTENT_TYPE_FORM);
	}
	context->clearResponse();
	Memory memHeader = context->makeRequestPacket();
	m_output->write(memHeader);
	m_output->mergeBuffer(&(context->m_bufferOutput));
	m_output->startWriting();
}

void HttpClientConnection::_onError(HttpClientContext* context)
{
	Ref<HttpClient> client;
}

/*
void HttpClientConnection::_processInput(const void* _data, sl_uint32 size)
{
	Ref<HttpClient> client = getClient();
	if (client.isNull()) {
		return;
	}
	if (m_flagClosed) {
		return;
	}

	Ref<HttpClientContext> _context = m_currentContext;
	if (_context.isNull()) {
		if (m_queueContexts.pop(&_context)) {
			m_currentContext = _context;
		} else {
			client->returnConnection(this);
			return;
		}
	}
	HttpClientContext* context = _context.get();

	char* data = (char*)_data;

	if (context->m_responseHeader.isNull()) {
		sl_size posBody;
		if (context->m_responseHeaderReader.add(data, size, posBody)) {
			context->m_responseHeader = context->m_responseHeaderReader.mergeHeader();
			if (context->m_responseHeader.isNull()) {
				_onError(context);
				return;
			}
			if (posBody > size) {
				_onError(context);
				return;
			}
			context->m_responseHeaderReader.clear();
			sl_reg iRet = context->parseResponsePacket(context->m_responseHeader.getBuf(), context->m_responseHeader.getSize());
			if (iRet != context->m_responseHeader.getSize()) {
				_onError(context);
				return;
			}
			
			sl_uint32 code = context->getResponseCode();
			if (
				(code >= 100 && code <= 199) // 1xx
				|| code == 204 // Not Content
				|| code == 304 // Not Modified
				|| context->getMethod() == SLIB_HTTP_METHOD_HEAD
				|| context->containsResponseHeader(SLIB_HTTP_HEADER_TRANSFER_ENCODING)
				) // Content Length is not allowed
			{
				if (context->containsResponseHeader(SLIB_HTTP_HEADER_CONTENT_LENGTH)) {
					_onError(context);
					return;
				}
				m_currentConnectionType = connectionTypePersistent;
				context->m_responseContentLength = 0;
			} else {
				if (context->containsResponseHeader(SLIB_HTTP_HEADER_CONTENT_LENGTH)) {
					context->m_responseContentLength = context->getResponseContentLengthHeader();
					m_currentConnectionType = connectionTypePersistent;
				} else {
					context->m_responseContentLength = -1;
					m_currentConnectionType = connectionTypeTearDown;
				}
			}
			
			if (context->get)
			context->m_requestBody = Memory::create(data + posBody, size - (sl_uint32)posBody);
			if (!(context->m_requestBody.isEmpty())) {
				sendResponse_ServerError();
				return;
			}
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
		if (!(context->m_requestBodyBuffer.add(data, size))) {
			sendResponse_ServerError();
			return;
		}
	}
	if (context->m_requestHeader.isNotNull()) {
		if (context->m_requestBodyBuffer.getSize() >= context->m_requestContentLength) {

			m_contextCurrent.setNull();

			context->m_requestBody = context->m_requestBodyBuffer.merge();
			if (context->m_requestBody.isNotEmpty()) {
				sendResponse_ServerError();
				return;
			}
			context->m_requestBodyBuffer.clear();

			if (context->getMethodUppercase() == "POST") {
				if (context->getRequestContentType() == "application/x-www-form-urlencoded") {
					context->applyPostParameters(context->m_requestBody.getBuf(), context->m_requestBody.getSize());
				}
			}
			SLIB_STATIC_STRING(s, "text/html; charset=UTF-8");
			context->setResponseContentType(s);

			if (context->isProcessingByThread()) {
				Ref<AsyncLoop> loop = service->getAsyncLoop();
				if (loop.isNotNull()) {
					loop->addTask(SLIB_CALLBACK_WEAKREF(HttpServiceConnection, _processContext, this, _context));
					return;
				} else {
					sendResponse_ServerError();
					return;
				}
			} else {
				_processContext(context);
				return;
			}
		}
	}
	_read();
}

void HttpServiceConnection::_read()
{
	MutexLocker lock(getLocker());
	if (m_flagClosed) {
		return;
	}
	if (m_flagReading) {
		return;
	}
	m_flagReading = sl_true;
	if (!(m_io->readToMemory(m_bufRead, (WeakRef<HttpServiceConnection>)this))) {
		m_flagReading = sl_false;
		close();
	}
}

void HttpServiceConnection::onRead(AsyncStream* stream, void* data, sl_uint32 size, Referable* ref, sl_bool flagError)
{
	m_flagReading = sl_false;
	if (flagError) {
		close();
	} else {
		_processInput(data, size);
	}
}
*/

/******************************************************
	HttpClient
******************************************************/
void HttpClient::sendRequest(const Ref<HttpClientContext>& request, const SocketAddress& proxy)
{

}

void HttpClient::sendRequest(const Ref<HttpClientContext>& request)
{

}

SLIB_NETWORK_NAMESPACE_END
