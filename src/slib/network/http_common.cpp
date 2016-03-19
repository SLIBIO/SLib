#include "../../../inc/slib/network/http_common.h"

#include "../../../inc/slib/core/url.h"

SLIB_NETWORK_NAMESPACE_BEGIN

#define HTTP_STATUS_CASE(name, text) \
	case HttpStatus::name: \
	{ SLIB_STATIC_STRING(s, text); return s; }

String HttpStatuses::toString(HttpStatus status)
{
	switch (status) {
		
		HTTP_STATUS_CASE(Continue, "Continue");
		HTTP_STATUS_CASE(SwitchingProtocols, "Switching Protocols");
		
		HTTP_STATUS_CASE(OK, "OK");
		HTTP_STATUS_CASE(Created, "Created");
		HTTP_STATUS_CASE(Accepted, "Accepted");
		HTTP_STATUS_CASE(NonAuthInfo, "Non-Authoritative Information");
		HTTP_STATUS_CASE(NoContent, "No Content");
		HTTP_STATUS_CASE(ResetContent, "Reset Content");
		HTTP_STATUS_CASE(PartialContent, "Partial Content");
		
		HTTP_STATUS_CASE(MultipleChoices, "Multiple Choices");
		HTTP_STATUS_CASE(MovedPermanently, "Moved Permanently");
		HTTP_STATUS_CASE(Found, "Found");
		HTTP_STATUS_CASE(SeeOther, "See Other");
		HTTP_STATUS_CASE(NotModified, "Not Modified");
		HTTP_STATUS_CASE(UseProxy, "Use Proxy");
		HTTP_STATUS_CASE(TemporaryRedirect, "Temporary Redirect");
		
		HTTP_STATUS_CASE(BadRequest, "Bad Request");
		HTTP_STATUS_CASE(Unauthorized, "Unauthorized");
		HTTP_STATUS_CASE(PaymentRequired, "Payment Required");
		HTTP_STATUS_CASE(Forbidden, "Forbidden");
		HTTP_STATUS_CASE(NotFound, "Not Found");
		HTTP_STATUS_CASE(MethodNotAllowed, "Method Not Allowed");
		HTTP_STATUS_CASE(NotAcceptable, "Not Acceptable");
		HTTP_STATUS_CASE(ProxyAuthenticationRequired, "Proxy Authentication Required");
		HTTP_STATUS_CASE(RequestTimeout, "Request Time-out");
		HTTP_STATUS_CASE(Conflict, "Conflict");
		HTTP_STATUS_CASE(Gone, "Gone");
		HTTP_STATUS_CASE(LengthRequired, "Length Required");
		HTTP_STATUS_CASE(PreconditionFailed, "Precondition Failed");
		HTTP_STATUS_CASE(RequestEntityTooLarge, "Request Entity Too Large");
		HTTP_STATUS_CASE(RequestUriTooLarge, "Request-URI Too Large");
		HTTP_STATUS_CASE(UnsupportedMediaType, "Unsupported Media Type");
		HTTP_STATUS_CASE(RequestRangeNotSatisfiable, "Requested range not satisfiable");
		HTTP_STATUS_CASE(ExpectationFailed, "Expectation Failed");
		
		HTTP_STATUS_CASE(InternalServerError, "Internal Server Error");
		HTTP_STATUS_CASE(NotImplemented, "Not Implemented");
		HTTP_STATUS_CASE(BadGateway, "Bad Gateway");
		HTTP_STATUS_CASE(ServiceUnavailable, "Service Unavailable");
		HTTP_STATUS_CASE(GatewayTimeout, "Gateway Time-out");
		HTTP_STATUS_CASE(HttpVersionNotSupported, "HTTP Version not supported");
		
	}
	return String::null();
}


SLIB_STATIC_STRING(_g_sz_http_method_GET, "GET");
SLIB_STATIC_STRING(_g_sz_http_method_HEAD, "HEAD");
SLIB_STATIC_STRING(_g_sz_http_method_POST, "POST");
SLIB_STATIC_STRING(_g_sz_http_method_PUT, "PUT");
SLIB_STATIC_STRING(_g_sz_http_method_DELETE, "DELETE");
SLIB_STATIC_STRING(_g_sz_http_method_CONNECT, "CONNECT");
SLIB_STATIC_STRING(_g_sz_http_method_OPTIONS, "OPTIONS");
SLIB_STATIC_STRING(_g_sz_http_method_TRACE, "TRACE");

String HttpMethods::toString(HttpMethod method)
{
	switch (method) {
		case HttpMethod::GET:
			return _g_sz_http_method_GET;
		case HttpMethod::HEAD:
			return _g_sz_http_method_HEAD;
		case HttpMethod::POST:
			return _g_sz_http_method_POST;
		case HttpMethod::PUT:
			return _g_sz_http_method_PUT;
		case HttpMethod::DELETE:
			return _g_sz_http_method_DELETE;
		case HttpMethod::CONNECT:
			return _g_sz_http_method_CONNECT;
		case HttpMethod::OPTIONS:
			return _g_sz_http_method_OPTIONS;
		case HttpMethod::TRACE:
			return _g_sz_http_method_TRACE;
		default:
			break;
	}
	return String::null();
}

class _HttpMethod_Mapping
{
public:
	HashMap<String, HttpMethod> maps;
	
	_HttpMethod_Mapping()
	{
		maps.put(_g_sz_http_method_GET, HttpMethod::GET);
		maps.put(_g_sz_http_method_HEAD, HttpMethod::HEAD);
		maps.put(_g_sz_http_method_POST, HttpMethod::POST);
		maps.put(_g_sz_http_method_PUT, HttpMethod::PUT);
		maps.put(_g_sz_http_method_DELETE, HttpMethod::DELETE);
		maps.put(_g_sz_http_method_CONNECT, HttpMethod::CONNECT);
		maps.put(_g_sz_http_method_OPTIONS, HttpMethod::OPTIONS);
		maps.put(_g_sz_http_method_TRACE, HttpMethod::TRACE);
	}
};

HttpMethod HttpMethods::fromString(const String& method)
{
	SLIB_SAFE_STATIC(_HttpMethod_Mapping, t);
	return t.maps.getValue(method, HttpMethod::Unknown);
}


#define DEFINE_HTTP_HEADER(name, value) \
	SLIB_STATIC_STRING(static_##name, value); \
	const String& HttpHeaders::name = static_##name;

DEFINE_HTTP_HEADER(ContentLength, "Content-Length")
DEFINE_HTTP_HEADER(ContentType, "Content-Type")
DEFINE_HTTP_HEADER(Host, "Host")
DEFINE_HTTP_HEADER(AcceptEncoding, "Accept-Encoding")
DEFINE_HTTP_HEADER(TransferEncoding, "Transfer-Encoding")
DEFINE_HTTP_HEADER(ContentEncoding, "Content-Encoding")


/***********************************************************************
	HttpRequest
***********************************************************************/
HttpRequest::HttpRequest()
{
	SLIB_STATIC_STRING(s1, "HTTP/1.1");
	m_requestVersion = s1;
	m_method = HttpMethod::GET;
	SLIB_STATIC_STRING(s2, "GET");
	m_methodText = s2;
	m_methodTextUpper = s2;
}

HttpMethod HttpRequest::getMethod() const
{
	return m_method;
}

String HttpRequest::getMethodText() const
{
	return m_methodText;
}

String HttpRequest::getMethodTextUppercase() const
{
	return m_methodTextUpper;
}

void HttpRequest::setMethod(HttpMethod method)
{
	m_method = method;
	m_methodText = HttpMethods::toString(method);
	m_methodTextUpper = m_methodText;
}

void HttpRequest::setMethod(const String& method)
{
	m_methodText = method;
	m_methodTextUpper = method.toUpper();
	m_method = HttpMethods::fromString(m_methodTextUpper);
}

String HttpRequest::getPath() const
{
	return m_path;
}

void HttpRequest::setPath(const String& path)
{
	m_path = path;
}

String HttpRequest::getQuery() const
{
	return m_query;
}

void HttpRequest::setQuery(String query)
{
	m_query = query;
}

String HttpRequest::getRequestVersion() const
{
	return m_requestVersion;
}

void HttpRequest::setRequestVersion(const String& version)
{
	m_requestVersion = version;
}

const IMap<String, String>& HttpRequest::getRequestHeaders() const
{
	return m_requestHeaders;
}

String HttpRequest::getRequestHeader(String name) const
{
	return m_requestHeaders.getValue(name, String::null());
}

List<String> HttpRequest::getRequestHeaderValues(String name) const
{
	return m_requestHeaders.getValues(name);
}

void HttpRequest::setRequestHeader(String name, String value)
{
	m_requestHeaders.put(name, value);
}

void HttpRequest::addRequestHeader(String name, String value)
{
	m_requestHeaders.add(name, value);
}

sl_bool HttpRequest::containsRequestHeader(String name) const
{
	return m_requestHeaders.containsKey(name);
}

void HttpRequest::removeRequestHeader(String name)
{
	m_requestHeaders.removeAllMatchingKeys(name);
}

void HttpRequest::clearRequestHeaders()
{
	m_requestHeaders.removeAll();
}

sl_uint64 HttpRequest::getRequestContentLengthHeader() const
{
	String headerContentLength = getRequestHeader(HttpHeaders::ContentLength);
	if (headerContentLength.isNotEmpty()) {
		return headerContentLength.parseUint64();
	}
	return 0;
}

void HttpRequest::setRequestContentLengthHeader(sl_int64 size)
{
	if (size < 0) {
		size = 0;
	}
	setRequestHeader(HttpHeaders::ContentLength, String::fromUint64(size));
}

String HttpRequest::getRequestContentType() const
{
	return getRequestHeader(HttpHeaders::ContentType);
}

void HttpRequest::setRequestContentType(const String& type)
{
	setRequestHeader(HttpHeaders::ContentType, type);
}

void HttpRequest::setRequestContentType(ContentType type)
{
	setRequestHeader(HttpHeaders::ContentType, ContentTypes::toString(type));
}

String HttpRequest::getRequestContentEncoding() const
{
	return getRequestHeader(HttpHeaders::ContentEncoding);
}

void HttpRequest::setRequestContentEncoding(const String& type)
{
	setRequestHeader(HttpHeaders::ContentEncoding, type);
}

String HttpRequest::getRequestTransferEncoding() const
{
	return getRequestHeader(HttpHeaders::TransferEncoding);
}

void HttpRequest::setRequestTransferEncoding(const String& type)
{
	setRequestHeader(HttpHeaders::TransferEncoding, type);
}

sl_bool HttpRequest::isChunkedRequest() const
{
	String te = getRequestTransferEncoding();
	if (te.endsWith("chunked")) {
		return sl_true;
	}
	return sl_false;
}

String HttpRequest::getHost() const
{
	return getRequestHeader(HttpHeaders::Host);
}

void HttpRequest::setHost(const String& type)
{
	setRequestHeader(HttpHeaders::Host, type);
}

const IMap<String, String>& HttpRequest::getParameters() const
{
	return m_parameters;
}

String HttpRequest::getParameter(const String& name) const
{
	return m_parameters.getValue(name, String::null());
}

List<String> HttpRequest::getParameterValues(const String& name) const
{
	return m_parameters.getValues(name);
}

sl_bool HttpRequest::containsParameter(const String& name) const
{
	return m_parameters.containsKey(name);
}

const IMap<String, String>& HttpRequest::getQueryParameters() const
{
	return m_queryParameters;
}

String HttpRequest::getQueryParameter(String name) const
{
	return m_queryParameters.getValue(name, String::null());
}

List<String> HttpRequest::getQueryParameterValues(String name) const
{
	return m_queryParameters.getValues(name);
}

sl_bool HttpRequest::containsQueryParameter(String name) const
{
	return m_queryParameters.containsKey(name);
}

const IMap<String, String>& HttpRequest::getPostParameters() const
{
	return m_postParameters;
}

String HttpRequest::getPostParameter(String name) const
{
	return m_postParameters.getValue(name, String::null());
}

List<String> HttpRequest::getPostParameterValues(String name) const
{
	return m_postParameters.getValues(name);
}

sl_bool HttpRequest::containsPostParameter(String name) const
{
	return m_postParameters.containsKey(name);
}

void HttpRequest::applyPostParameters(const void* data, sl_size size)
{
	Map<String, String> params = parseParameters(data, size);
	m_postParameters.put(params.ref.ptr);
	m_parameters.put(params.ref.ptr);
}

void HttpRequest::applyPostParameters(const String& str)
{
	applyPostParameters(str.getData(), str.getLength());
}

void HttpRequest::applyQueryToParameters()
{
	Map<String, String> params = parseParameters(m_query);
	m_queryParameters.put(params.ref.ptr);
	m_parameters.put(params.ref.ptr);
}

Map<String, String> HttpRequest::parseParameters(const String& str)
{
	return parseParameters(str.getData(), str.getLength());
}

Map<String, String> HttpRequest::parseParameters(const void* data, sl_size _len)
{
	Map<String, String> ret;
	sl_char8* buf = (sl_char8*)data;
	sl_uint32 start = 0;
	sl_uint32 indexSplit = 0;
	sl_uint32 pos = 0;
	if (_len > SLIB_STR_MAX_LEN) {
		_len = SLIB_STR_MAX_LEN;		
	}
	sl_uint32 len = (sl_uint32)_len;
	for (; pos <= len; pos++) {
		sl_char8 ch;
		if (pos == len) {
			ch = '&';
		} else {
			ch = buf[pos];
		}
		if (ch == '=') {
			indexSplit = pos;
		} else if (ch == '&') {
			if (indexSplit > start) {
				String name = String::fromUtf8(buf + start, indexSplit - start);
				indexSplit++;
				String value = String::fromUtf8(buf + indexSplit, pos - indexSplit);
				value = Url::decodeUriComponentByUTF8(value);
				ret.put_NoLock(name, value);
			} else {
				String name = String::fromUtf8(buf + start, pos - start);
				ret.put_NoLock(name, String::null());
			}
			start = pos + 1;
			indexSplit = start;
		}
	}
	return ret;
}

Memory HttpRequest::makeRequestPacket() const
{
	String8 str;
	MemoryBuffer msg;
	str = m_methodText;
	msg.addStatic(str.getData(), str.getLength());
	msg.addStatic(" ", 1);
	str = m_path;
	if (str.isEmpty()) {
		msg.addStatic("/", 1);
	} else {
		msg.addStatic(str.getData(), str.getLength());
	}
	if (m_query.isNotEmpty()) {
		msg.addStatic("?", 1);
		str = m_query;
		msg.addStatic(str.getData(), str.getLength());
	}
	msg.addStatic(" ", 1);
	str = m_requestVersion;
	msg.addStatic(str.getData(), str.getLength());
	msg.addStatic("\r\n", 2);

	Iterator< Pair<String, String> > iterator = m_requestHeaders.iterator();
	Pair<String, String> pair;
	while (iterator.next(&pair)) {
		str = pair.key;
		msg.addStatic(str.getData(), str.getLength());
		msg.addStatic(": ", 2);
		str = pair.value;
		msg.addStatic(str.getData(), str.getLength());
		msg.addStatic("\r\n", 2);
	}
	msg.addStatic("\r\n", 2);
	return msg.merge();
}

sl_reg HttpRequest::parseRequestPacket(const void* packet, sl_size _size)
{
	const sl_char8* data = (const sl_char8*)packet;
	if (_size > SLIB_STR_MAX_LEN) {
		_size = SLIB_STR_MAX_LEN;
	}
	sl_uint32 size = (sl_uint32)(_size);
	sl_uint32 posCurrent = 0;
	sl_uint32 posStart = 0;
	// method
	while (posCurrent < size) {
		sl_char8 ch = data[posCurrent];
		if (ch == ' ') {
			break;
		}
		if (ch == '\r' || ch == '\n') {
			return -1;
		}
		posCurrent++;
	}
	if (posCurrent == size) {
		return 0;
	}
	setMethod(String::fromUtf8(data + posStart, posCurrent - posStart));
	posCurrent++;

	// uri
	posStart = posCurrent;
	sl_uint32 posQuery = 0;
	while (posCurrent < size) {
		sl_char8 ch = data[posCurrent];
		if (ch == ' ') {
			break;
		}
		if (ch == '\r' || ch == '\n') {
			return -1;
		}
		if (posQuery == 0 && ch == '?') {
			posQuery = posCurrent + 1;
		}
		posCurrent++;
	}
	if (posCurrent == size) {
		return 0;
	}
	if (posQuery > 0) {
		setPath(String::fromUtf8(data + posStart, posQuery - 1 - posStart));
		setQuery(String::fromUtf8(data + posQuery, posCurrent - posQuery));
	} else {
		setPath(String::fromUtf8(data + posStart, posCurrent - posStart));
		setQuery(String::null());
	}
	posCurrent++;

	// version
	posStart = posCurrent;
	while (posCurrent < size) {
		sl_char8 ch = data[posCurrent];
		if (ch == '\r') {
			break;
		}
		posCurrent++;
	}
	if (posCurrent >= size - 1) {
		return 0;
	}
	if (data[posCurrent + 1] != '\n') {
		return -1;
	}
	setRequestVersion(String::fromUtf8(data + posStart, posCurrent - posStart));
	posCurrent += 2;

	// headers
	for (;;) {
		posStart = posCurrent;
		sl_uint32 indexSplit = 0;
		while (posCurrent < size) {
			sl_char8 ch = data[posCurrent];
			if (ch == '\r') {
				break;
			}
			if (indexSplit == 0) {
				if (ch == ':') {
					indexSplit = posCurrent;
				}
			}
			posCurrent++;
		}
		if (posCurrent >= size - 1) {
			return 0;
		}
		if (data[posCurrent + 1] != '\n') {
			return -1;
		}

		if (posCurrent == posStart) {
			posCurrent += 2;
			break;
		}

		String name;
		String value;
		if (indexSplit != 0) {
			name = String::fromUtf8(data + posStart, indexSplit - posStart);
			sl_uint32 startValue = indexSplit + 1;
			sl_uint32 endValue = posCurrent;
			while (startValue < endValue) {
				if (data[startValue] != ' ' && data[startValue] != '\t') {
					break;
				}
				startValue++;
			}
			while (startValue < endValue) {
				if (data[endValue - 1] != ' ' && data[endValue - 1] != '\t') {
					break;
				}
				endValue--;
			}
			value = Url::decodeUriComponentByUTF8(String::fromUtf8(data + startValue, endValue - startValue));
		} else {
			name = String::fromUtf8(data + posStart, posCurrent - posStart);
		}
		m_requestHeaders.add(name, value);
		posCurrent += 2;
	}
	return posCurrent;
}


/***********************************************************************
	HttpResponse
***********************************************************************/

HttpResponse::HttpResponse()
{
	SLIB_STATIC_STRING(s1, "HTTP/1.1");
	m_responseVersion = s1;
	m_responseCode = HttpStatus::OK;
	SLIB_STATIC_STRING(s2, "OK");
	m_responseMessage = s2;
}

HttpStatus HttpResponse::getResponseCode() const
{
	return m_responseCode;
}

void HttpResponse::setResponseCode(HttpStatus code)
{
	m_responseCode = code;
	m_responseMessage = HttpStatuses::toString(code);
}

String HttpResponse::getResponseMessage() const
{
	return m_responseMessage;
}

void HttpResponse::setResponseMessage(String message)
{
	m_responseMessage = message;
}

String HttpResponse::getResponseVersion() const
{
	return m_responseVersion;
}

void HttpResponse::setResponseVersion(const String& version)
{
	m_responseVersion = version;
}

const IMap<String, String>& HttpResponse::getResponseHeaders() const
{
	return m_responseHeaders;
}

String HttpResponse::getResponseHeader(String name) const
{
	return m_responseHeaders.getValue(name, String::null());
}

List<String> HttpResponse::getResponseHeaderValues(String name) const
{
	return m_responseHeaders.getValues(name);
}

void HttpResponse::setResponseHeader(String name, String value)
{
	m_responseHeaders.put(name, value);
}

void HttpResponse::addResponseHeader(String name, String value)
{
	m_responseHeaders.add(name, value);
}

sl_bool HttpResponse::containsResponseHeader(String name) const
{
	return m_responseHeaders.containsKey(name);
}

void HttpResponse::removeResponseHeader(String name)
{
	m_responseHeaders.removeAllMatchingKeys(name);
}

void HttpResponse::clearResponseHeaders()
{
	m_responseHeaders.removeAll();
}

sl_uint64 HttpResponse::getResponseContentLengthHeader() const
{
	String headerContentLength = getResponseHeader(HttpHeaders::ContentLength);
	if (headerContentLength.isNotEmpty()) {
		return headerContentLength.parseUint64();
	}
	return 0;
}

void HttpResponse::setResponseContentLengthHeader(sl_int64 size)
{
	if (size < 0) {
		size = 0;
	}
	setResponseHeader(HttpHeaders::ContentLength, String::fromUint64(size));
}

String HttpResponse::getResponseContentType() const
{
	return getResponseHeader(HttpHeaders::ContentType);
}

void HttpResponse::setResponseContentType(const String& type)
{
	setResponseHeader(HttpHeaders::ContentType, type);
}

void HttpResponse::setResponseContentType(ContentType type)
{
	setResponseHeader(HttpHeaders::ContentType, ContentTypes::toString(type));
}

String HttpResponse::getResponseContentEncoding() const
{
	return getResponseHeader(HttpHeaders::ContentEncoding);
}

void HttpResponse::setResponseContentEncoding(const String& type)
{
	setResponseHeader(HttpHeaders::ContentEncoding, type);
}

String HttpResponse::getResponseTransferEncoding() const
{
	return getResponseHeader(HttpHeaders::TransferEncoding);
}

void HttpResponse::setResponseTransferEncoding(const String& type)
{
	setResponseHeader(HttpHeaders::TransferEncoding, type);
}

sl_bool HttpResponse::isChunkedResponse() const
{
	String te = getResponseTransferEncoding();
	if (te.endsWith("chunked")) {
		return sl_true;
	}
	return sl_false;
}

Memory HttpResponse::makeResponsePacket() const
{
	String8 str;
	MemoryBuffer msg;
	str = m_responseVersion;
	msg.addStatic(str.getData(), str.getLength());
	msg.addStatic(" ", 1);
	str = String8::fromUint32((sl_uint32)m_responseCode);
	msg.addStatic(str.getData(), str.getLength());
	msg.addStatic(" ", 1);
	str = m_responseMessage;
	msg.addStatic(str.getData(), str.getLength());
	msg.addStatic("\r\n", 2);

	Iterator< Pair<String, String> > iterator = m_responseHeaders.iterator();
	Pair<String, String> pair;
	while (iterator.next(&pair)) {
		str = pair.key;
		msg.addStatic(str.getData(), str.getLength());
		msg.addStatic(": ", 2);
		str = pair.value;
		msg.addStatic(str.getData(), str.getLength());
		msg.addStatic("\r\n", 2);
	}
	msg.addStatic("\r\n", 2);
	return msg.merge();
}

sl_reg HttpResponse::parseResponsePacket(const void* packet, sl_size _size)
{
	const sl_char8* data = (const sl_char8*)packet;
	if (_size > SLIB_STR_MAX_LEN) {
		_size = SLIB_STR_MAX_LEN;
	}
	sl_uint32 size = (sl_uint32)(_size);
	sl_uint32 posCurrent = 0;
	sl_uint32 posStart = 0;
	// version
	while (posCurrent < size) {
		sl_char8 ch = data[posCurrent];
		if (ch == ' ') {
			break;
		}
		if (ch == '\r' || ch == '\n') {
			return -1;
		}
		posCurrent++;
	}
	if (posCurrent == size) {
		return 0;
	}
	setResponseVersion(String::fromUtf8(data + posStart, posCurrent - posStart));
	posCurrent++;

	// response code
	posStart = posCurrent;
	while (posCurrent < size) {
		sl_char8 ch = data[posCurrent];
		if (ch == ' ') {
			break;
		}
		if (ch == '\r' || ch == '\n') {
			return -1;
		}
		posCurrent++;
	}
	if (posCurrent == size) {
		return 0;
	}
	String strResponseCode = String::fromUtf8(data + posStart, posCurrent - posStart);
	sl_uint32 code;
	if (!(strResponseCode.parseUint32(10, &code))) {
		return -1;
	}
	m_responseCode = (HttpStatus)code;
	posCurrent++;

	// response message
	posStart = posCurrent;
	while (posCurrent < size) {
		sl_char8 ch = data[posCurrent];
		if (ch == '\r') {
			break;
		}
		posCurrent++;
	}
	if (posCurrent >= size - 1) {
		return 0;
	}
	if (data[posCurrent + 1] != '\n') {
		return -1;
	}
	setResponseMessage(String::fromUtf8(data + posStart, posCurrent - posStart));
	posCurrent += 2;

	// headers
	for (;;) {
		posStart = posCurrent;
		sl_uint32 indexSplit = 0;
		while (posCurrent < size) {
			sl_char8 ch = data[posCurrent];
			if (ch == '\r') {
				break;
			}
			if (indexSplit == 0) {
				if (ch == ':') {
					indexSplit = posCurrent;
				}
			}
			posCurrent++;
		}
		if (posCurrent >= size - 1) {
			return 0;
		}
		if (data[posCurrent + 1] != '\n') {
			return -1;
		}

		if (posCurrent == posStart) {
			posCurrent += 2;
			break;
		}

		String name;
		String value;
		if (indexSplit != 0) {
			name = String::fromUtf8(data + posStart, indexSplit - posStart);
			sl_uint32 startValue = indexSplit + 1;
			sl_uint32 endValue = posCurrent;
			while (startValue < endValue) {
				if (data[startValue] != ' ' && data[startValue] != '\t') {
					break;
				}
				startValue++;
			}
			while (startValue < endValue) {
				if (data[endValue - 1] != ' ' && data[endValue - 1] != '\t') {
					break;
				}
				endValue--;
			}
			value = Url::decodeUriComponentByUTF8(String::fromUtf8(data + startValue, endValue - startValue));
		} else {
			name = String::fromUtf8(data + posStart, posCurrent - posStart);
		}
		m_responseHeaders.add(name, value);
		posCurrent += 2;
	}
	return posCurrent;
}

/***********************************************************************
	HttpOutputBuffer
***********************************************************************/
HttpOutputBuffer::HttpOutputBuffer()
{
}

void HttpOutputBuffer::clearOutput()
{
	m_bufferOutput.clearOutput();
}

void HttpOutputBuffer::write(const void* buf, sl_size size)
{
	m_bufferOutput.write(buf, size);
}

void HttpOutputBuffer::write(const String& str)
{
	write(str.getData(), str.getLength());
}

void HttpOutputBuffer::write(const Memory& mem)
{
	m_bufferOutput.write(mem);
}

void HttpOutputBuffer::copyFrom(AsyncStream* stream, sl_uint64 size)
{
	m_bufferOutput.copyFrom(stream, size);
}

void HttpOutputBuffer::copyFromFile(const String& path)
{
	m_bufferOutput.copyFromFile(path);
}

sl_uint64 HttpOutputBuffer::getOutputLength() const
{
	return m_bufferOutput.getOutputLength();
}

/***********************************************************************
	HttpHeaderReader
***********************************************************************/
HttpHeaderReader::HttpHeaderReader()
{
	m_last[0] = 0;
	m_last[1] = 0;
	m_last[2] = 0;
}

sl_bool HttpHeaderReader::add(const void* _buf, sl_size size, sl_size& posBody)
{
	if (size == 0) {
		return sl_false;
	}
	sl_bool flagFound = sl_false;
	const sl_uint8* buf = (const sl_uint8*)(_buf);
	if (size > 0 && m_last[0] == '\r' && m_last[1] == '\n' && m_last[2] == '\r' && buf[0] == '\n') {
		posBody = 1;
		flagFound = sl_true;
	}
	if (size > 1 && m_last[1] == '\r' && m_last[2] == '\n' && buf[0] == '\r' && buf[1] == '\n') {
		posBody = 2;
		flagFound = sl_true;
	}
	if (size > 2 && m_last[2] == '\r' && buf[0] == '\n' && buf[1] == '\r' && buf[2] == '\n') {
		posBody = 3;
		flagFound = sl_true;
	}
	if (size > 3) {
		for (sl_size i = 0; i <= size - 4; i++) {
			if (buf[i] == '\r' && buf[i + 1] == '\n' && buf[i + 2] == '\r' && buf[i + 3] == '\n') {
				posBody = 4 + i;
				flagFound = sl_true;
				break;
			}
		}
	}
	if (flagFound) {
		m_buffer.add(Memory::create(buf, posBody));
		m_last[0] = 0;
		m_last[1] = 0;
		m_last[2] = 0;
	} else {
		m_buffer.add(Memory::create(buf, size));
		if (size < 3) {
			if (size == 1) {
				m_last[0] = m_last[1];
				m_last[1] = m_last[2];
				m_last[2] = buf[0];
			} else {
				m_last[0] = m_last[2];
				m_last[1] = buf[0];
				m_last[2] = buf[1];
			}
		} else {
			m_last[0] = buf[size - 3];
			m_last[1] = buf[size - 2];
			m_last[2] = buf[size - 1];
		}
	}
	return flagFound;
}

sl_size HttpHeaderReader::getHeaderSize()
{
	return m_buffer.getSize();
}

Memory HttpHeaderReader::mergeHeader()
{
	return m_buffer.merge();
}

void HttpHeaderReader::clear()
{
	m_last[0] = 0;
	m_last[1] = 0;
	m_last[2] = 0;
	m_buffer.clear();
}

/***********************************************************************
	HttpContentReader
***********************************************************************/
HttpContentReader::HttpContentReader()
{
	m_flagDecompressing = sl_false;
}

class _HttpContentReader_Persistent : public HttpContentReader
{
public:
	sl_uint64 m_sizeTotal;
	sl_uint64 m_sizeRead;

public:
	_HttpContentReader_Persistent()
	{
		m_sizeRead = 0;
	}

	Memory filterRead(void* data, sl_uint32 size, const Referable* refData)
	{
		sl_uint64 sizeRemain = m_sizeTotal - m_sizeRead;
		if (size < sizeRemain) {
			m_sizeRead += size;
			return decompressData(data, size, refData);
		} else {
			m_sizeRead = m_sizeTotal;
			sl_uint32 sizeRead = (sl_uint32)sizeRemain;
			Memory ret = decompressData(data, sizeRead, refData);
			setCompleted((char*)data + sizeRead, size - sizeRead);
			return ret;
		}
	}
};

Ref<HttpContentReader> HttpContentReader::createPersistent(const Ref<AsyncStream>& io,
														   const Ptr<IHttpContentReaderListener>& listener,
														   sl_uint64 contentLength,
														   sl_uint32 bufferSize,
														   sl_bool flagDecompress)
{
	Ref<_HttpContentReader_Persistent> ret = new _HttpContentReader_Persistent;
	if (io.isNull()) {
		return ret;
	}
	if (contentLength == 0 || bufferSize == 0) {
		return ret;
	}
	if (ret.isNotNull()) {
		ret->m_sizeTotal = contentLength;
		ret->m_listener = listener;
		ret->setReadingBufferSize(bufferSize);
		ret->setSourceStream(io);
		if (flagDecompress) {
			if (!(ret->setDecompressing())) {
				ret.setNull();
			}
		}
	}
	return ret;
}

/*

							Chunked Transfer Coding

	chunked-body   = *chunk
                      last-chunk
                      trailer-part
                      CRLF

     chunk          = chunk-size [ chunk-ext ] CRLF
                      chunk-data CRLF
     chunk-size     = 1*HEXDIG
     last-chunk     = 1*("0") [ chunk-ext ] CRLF

     chunk-data     = 1*OCTET ; a sequence of chunk-size octets

*/

class _HttpContentReader_Chunked : public HttpContentReader
{
public:
	sl_uint32 m_state;
	sl_uint64 m_sizeCurrentChunk;
	sl_uint64 m_sizeCurrentChunkRead;
	sl_uint32 m_sizeTrailerField;

public:
	_HttpContentReader_Chunked()
	{
		m_state = 0;
		m_sizeCurrentChunk = 0;
		m_sizeCurrentChunkRead = 0;
		m_sizeTrailerField = 0;
	}

	Memory filterRead(void* _data, sl_uint32 size, const Referable* refData)
	{
		sl_uint8* data = (sl_uint8*)_data;
		sl_uint32 pos = 0;

		sl_uint8* output = data;
		sl_uint32 sizeOutput = 0;
		
		sl_uint32 v;

		while (pos < size) {
			sl_uint8 ch = data[pos];
			switch (m_state) {
			case 0: // chunk-size
				v = SLIB_CHAR_HEX_TO_INT(ch);
				if (v < 16) {
					m_sizeCurrentChunk = (m_sizeCurrentChunk << 4) | v;
					pos++;
				} else {
					m_state = 1;
				}
				break;
			case 1: // chunk-ext
				if (ch == '\r') {
					m_state = 2;
				}
				pos++;
				break;
			case 2: // CRLF
				if (ch == '\n') {
					if (m_sizeCurrentChunk > 0) {
						m_state = 3;
					} else {
						// last chunk
						m_state = 10;
						m_sizeTrailerField = 0;
					}
				} else {
					m_state = -1;
					setError();
					return Memory::null();
				}
				pos++;
				break;
			case 3: // chunk-data
				if (m_sizeCurrentChunkRead < m_sizeCurrentChunk) {
					output[sizeOutput] = ch;
					m_sizeCurrentChunkRead++;
					sizeOutput++;
				} else {
					if (ch == '\r') {
						m_state = 4;
					} else {
						m_state = -1;
						setError();
						return Memory::null();
					}
				}
				pos++;
				break;
			case 4: // CRLF
				if (ch == '\n') {
					m_sizeCurrentChunk = 0;
					m_sizeCurrentChunkRead = 0;
					m_state = 0;
				} else {
					m_state = -1;
					setError();
					return Memory::null();
				}
				pos++;
				break;
			case 10: // trailer-part
				if (ch == '\r') {
					m_state = 11;
				} else {
					m_sizeTrailerField++;
				}
				pos++;
				break;
			case 11: // CRLF
				if (ch == '\n') {
					if (m_sizeTrailerField > 0) {
						m_state = 10;
						m_sizeTrailerField = 0;
					} else {
						pos++;
						m_state = -1;
						Memory mem = decompressData(output, sizeOutput, refData);
						setCompleted(data + pos, size - pos);
						return mem;
					}
				} else {
					m_state = -1;
					setError();
					return Memory::null();
				}
				pos++;
				break;
			default:
				return Memory::null();
			}
		}
		return decompressData(output, sizeOutput, refData);
	}
};

Ref<HttpContentReader> HttpContentReader::createChunked(const Ref<AsyncStream>& io,
														const Ptr<IHttpContentReaderListener>& listener,
														sl_uint32 bufferSize,
														sl_bool flagDecompress)
{
	Ref<_HttpContentReader_Chunked> ret = new _HttpContentReader_Chunked;
	if (io.isNull()) {
		return ret;
	}
	if (bufferSize == 0) {
		return ret;
	}
	if (ret.isNotNull()) {
		ret->m_listener = listener;
		ret->setReadingBufferSize(bufferSize);
		ret->setSourceStream(io);
		if (flagDecompress) {
			if (!(ret->setDecompressing())) {
				ret.setNull();
			}
		}
	}
	return ret;
}

class _HttpContentReader_TearDown : public HttpContentReader
{
public:
	Memory filterRead(void* data, sl_uint32 size, const Referable* refData)
	{
		return decompressData(data, size, refData);
	}
};

Ref<HttpContentReader> HttpContentReader::createTearDown(const Ref<AsyncStream>& io,
														 const Ptr<IHttpContentReaderListener>& listener,
														 sl_uint32 bufferSize,
														 sl_bool flagDecompress)
{
	Ref<_HttpContentReader_TearDown> ret = new _HttpContentReader_TearDown;
	if (io.isNull()) {
		return ret;
	}
	if (bufferSize == 0) {
		return ret;
	}
	if (ret.isNotNull()) {
		ret->m_listener = listener;
		ret->setReadingBufferSize(bufferSize);
		ret->setSourceStream(io);
		if (flagDecompress) {
			if (!(ret->setDecompressing())) {
				ret.setNull();
			}
		}
	}
	return ret;
}

sl_bool HttpContentReader::isDecompressing()
{
	return m_flagDecompressing;
}

void HttpContentReader::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
	if (flagError) {
		setReadingEnded();
	}
	AsyncStreamFilter::onRead(stream, data, sizeRead, ref, flagError);
	if (flagError) {
		setError();
	}
}

void HttpContentReader::setCompleted(void* dataRemain, sl_uint32 size)
{
	setReadingEnded();
	PtrLocker<IHttpContentReaderListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onCompleteReadHttpContent(dataRemain, size, isReadingError());
	}
	setReadingError();
}

void HttpContentReader::setError()
{
	setReadingEnded();
	PtrLocker<IHttpContentReaderListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onCompleteReadHttpContent(sl_null, 0, sl_true);
	}
	setReadingError();
}

sl_bool HttpContentReader::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref)
{
	return sl_false;
}

sl_bool HttpContentReader::setDecompressing()
{
	if (m_zlib.start()) {
		m_flagDecompressing = sl_true;
		return sl_true;
	} else {
		m_flagDecompressing = sl_false;
		return sl_false;
	}
}

Memory HttpContentReader::decompressData(void* data, sl_uint32 size, const Referable* refData)
{
	if (m_flagDecompressing) {
		return m_zlib.decompress(data, size);
	} else {
		return Memory::createStatic(data, size, refData);
	}
}

SLIB_NETWORK_NAMESPACE_END
