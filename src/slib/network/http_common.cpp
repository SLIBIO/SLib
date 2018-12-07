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

#include "slib/network/http_common.h"

#include "slib/network/url.h"
#include "slib/core/safe_static.h"
#include "slib/core/variant.h"

namespace slib
{

#define HTTP_STATUS_CASE(name, text) \
	case HttpStatus::name: \
	{ SLIB_STATIC_STRING(s, text); return s; }

	String HttpStatuses::toString(HttpStatus status)
	{
		switch (status) {
			
			HTTP_STATUS_CASE(Unknown, "Unknown");
			
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
		return sl_null;
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
		return sl_null;
	}

	class _priv_HttpMethod_Mapping
	{
	public:
		CHashMap<String, HttpMethod> maps;
		
		_priv_HttpMethod_Mapping()
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
		SLIB_SAFE_STATIC(_priv_HttpMethod_Mapping, t)
		if (SLIB_SAFE_STATIC_CHECK_FREED(t)) {
			return HttpMethod::Unknown;
		}
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

	DEFINE_HTTP_HEADER(Range, "Range")
	DEFINE_HTTP_HEADER(ContentRange, "Content-Range")
	DEFINE_HTTP_HEADER(AcceptRanges, "Accept-Ranges")

	DEFINE_HTTP_HEADER(Origin, "Origin")
	DEFINE_HTTP_HEADER(AccessControlAllowOrigin, "Access-Control-Allow-Origin")

	DEFINE_HTTP_HEADER(SetCookie, "Set-Cookie")
	DEFINE_HTTP_HEADER(Cookie, "Cookie")

	sl_reg HttpHeaders::parseHeaders(HttpHeaderMap& map, const void* _data, sl_size size)
	{
		const sl_char8* data = (const sl_char8*)_data;
		sl_size posCurrent = 0;

		// headers
		for (;;) {
			sl_size posStart = posCurrent;
			sl_size indexSplit = 0;
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
				sl_size startValue = indexSplit + 1;
				sl_size endValue = posCurrent;
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
			map.add_NoLock(name, value);
			posCurrent += 2;
		}
		return posCurrent;
	}


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

	HttpRequest::~HttpRequest()
	{
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

	const HttpHeaderMap& HttpRequest::getRequestHeaders() const
	{
		return m_requestHeaders;
	}

	String HttpRequest::getRequestHeader(String name) const
	{
		return m_requestHeaders.getValue_NoLock(name, String::null());
	}

	List<String> HttpRequest::getRequestHeaderValues(String name) const
	{
		return m_requestHeaders.getValues_NoLock(name);
	}

	void HttpRequest::setRequestHeader(String name, String value)
	{
		m_requestHeaders.put_NoLock(name, value);
	}

	void HttpRequest::addRequestHeader(String name, String value)
	{
		m_requestHeaders.add_NoLock(name, value);
	}

	sl_bool HttpRequest::containsRequestHeader(String name) const
	{
		return m_requestHeaders.find_NoLock(name) != sl_null;
	}

	void HttpRequest::removeRequestHeader(String name)
	{
		m_requestHeaders.removeItems_NoLock(name);
	}

	void HttpRequest::clearRequestHeaders()
	{
		m_requestHeaders.removeAll_NoLock();
	}

	sl_uint64 HttpRequest::getRequestContentLengthHeader() const
	{
		String headerContentLength = getRequestHeader(HttpHeaders::ContentLength);
		if (headerContentLength.isNotEmpty()) {
			return headerContentLength.parseUint64();
		}
		return 0;
	}

	void HttpRequest::setRequestContentLengthHeader(sl_uint64 size)
	{
		setRequestHeader(HttpHeaders::ContentLength, String::fromUint64(size));
	}

	String HttpRequest::getRequestContentType() const
	{
		return getRequestHeader(HttpHeaders::ContentType);
	}

	String HttpRequest::getRequestContentTypeNoParams() const
	{
		String type = getRequestHeader(HttpHeaders::ContentType);
		sl_reg index = type.indexOf(';');
		if (index >= 0) {
			type = type.substring(0, index);
		}
		return type;
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

	String HttpRequest::getRequestRange() const
	{
		return getRequestHeader(HttpHeaders::Range);
	}

	void HttpRequest::setRequestRange(const String& range)
	{
		setRequestHeader(HttpHeaders::Range, range);
	}

	void HttpRequest::setRequestRange(sl_uint64 start, sl_uint64 last)
	{
		setRequestHeader(HttpHeaders::Range, String::format("bytes=%d-%d", start, last));
	}

	void HttpRequest::setRequestRangeFrom(sl_uint64 start)
	{
		setRequestHeader(HttpHeaders::Range, String::format("bytes=%d-", start));
	}

	void HttpRequest::setRequestRangeSuffix(sl_uint64 length)
	{
		setRequestHeader(HttpHeaders::Range, String::format("bytes=-%d", length));
	}

	String HttpRequest::getRequestOrigin() const
	{
		return getRequestHeader(HttpHeaders::Origin);
	}

	void HttpRequest::setRequestOrigin(const String& origin)
	{
		setRequestHeader(HttpHeaders::Origin, origin);
	}

	const HashMap<String, String>& HttpRequest::getParameters() const
	{
		return m_parameters;
	}

	String HttpRequest::getParameter(const String& name) const
	{
		return m_parameters.getValue_NoLock(name, String::null());
	}

	List<String> HttpRequest::getParameterValues(const String& name) const
	{
		return m_parameters.getValues_NoLock(name);
	}

	sl_bool HttpRequest::containsParameter(const String& name) const
	{
		return m_parameters.find_NoLock(name) != sl_null;
	}

	const HashMap<String, String>& HttpRequest::getQueryParameters() const
	{
		return m_queryParameters;
	}

	String HttpRequest::getQueryParameter(String name) const
	{
		return m_queryParameters.getValue_NoLock(name, String::null());
	}

	List<String> HttpRequest::getQueryParameterValues(String name) const
	{
		return m_queryParameters.getValues_NoLock(name);
	}

	sl_bool HttpRequest::containsQueryParameter(String name) const
	{
		return m_queryParameters.find_NoLock(name) != sl_null;
	}

	const HashMap<String, String>& HttpRequest::getPostParameters() const
	{
		return m_postParameters;
	}

	String HttpRequest::getPostParameter(String name) const
	{
		return m_postParameters.getValue_NoLock(name, String::null());
	}

	List<String> HttpRequest::getPostParameterValues(String name) const
	{
		return m_postParameters.getValues_NoLock(name);
	}

	sl_bool HttpRequest::containsPostParameter(String name) const
	{
		return m_postParameters.find_NoLock(name) != sl_null;
	}

	void HttpRequest::applyPostParameters(const void* data, sl_size size)
	{
		HashMap<String, String> params = parseParameters(data, size);
		m_postParameters.putAll_NoLock(params);
		m_parameters.putAll_NoLock(params);
	}

	void HttpRequest::applyPostParameters(const String& str)
	{
		applyPostParameters(str.getData(), str.getLength());
	}

	void HttpRequest::applyQueryToParameters()
	{
		HashMap<String, String> params = parseParameters(m_query);
		m_queryParameters.putAll_NoLock(params);
		m_parameters.putAll_NoLock(params);
	}

	HashMap<String, String> HttpRequest::parseParameters(const String& str)
	{
		return parseParameters(str.getData(), str.getLength());
	}

	HashMap<String, String> HttpRequest::parseParameters(const void* data, sl_size len)
	{
		HashMap<String, String> ret;
		sl_char8* buf = (sl_char8*)data;
		sl_size start = 0;
		sl_size indexSplit = 0;
		sl_size pos = 0;
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
		MemoryBuffer msg;
		String strMethod = m_methodText;
		msg.addStatic(strMethod.getData(), strMethod.getLength());
		msg.addStatic(" ", 1);
		String strPath = m_path;
		if (strPath.isEmpty()) {
			msg.addStatic("/", 1);
		} else {
			msg.addStatic(strPath.getData(), strPath.getLength());
		}
		String strQuery;
		if (m_query.isNotEmpty()) {
			msg.addStatic("?", 1);
			strQuery = m_query;
			msg.addStatic(strQuery.getData(), strQuery.getLength());
		}
		msg.addStatic(" ", 1);
		String strVersion = m_requestVersion;
		msg.addStatic(strVersion.getData(), strVersion.getLength());
		msg.addStatic("\r\n", 2);

		for (auto& pair : m_requestHeaders) {
			String str = pair.key;
			msg.addStatic(str.getData(), str.getLength());
			msg.addStatic(": ", 2);
			str = pair.value;
			msg.addStatic(str.getData(), str.getLength());
			msg.addStatic("\r\n", 2);
		}
		msg.addStatic("\r\n", 2);
		return msg.merge();
	}

	sl_reg HttpRequest::parseRequestPacket(const void* packet, sl_size size)
	{
		const sl_char8* data = (const sl_char8*)packet;
		sl_size posCurrent = 0;
		sl_size posStart = 0;
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
		sl_size posQuery = 0;
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

		sl_reg iRet = HttpHeaders::parseHeaders(m_requestHeaders, data + posCurrent, size - posCurrent);
		if (iRet > 0) {
			return posCurrent + iRet;
		} else {
			return iRet;
		}
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

	HttpResponse::~HttpResponse()
	{
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

	const HttpHeaderMap& HttpResponse::getResponseHeaders() const
	{
		return m_responseHeaders;
	}

	String HttpResponse::getResponseHeader(String name) const
	{
		return m_responseHeaders.getValue_NoLock(name, String::null());
	}

	List<String> HttpResponse::getResponseHeaderValues(String name) const
	{
		return m_responseHeaders.getValues_NoLock(name);
	}

	void HttpResponse::setResponseHeader(String name, String value)
	{
		m_responseHeaders.put_NoLock(name, value);
	}

	void HttpResponse::addResponseHeader(String name, String value)
	{
		m_responseHeaders.add_NoLock(name, value);
	}

	sl_bool HttpResponse::containsResponseHeader(String name) const
	{
		return m_responseHeaders.find_NoLock(name) != sl_null;
	}

	void HttpResponse::removeResponseHeader(String name)
	{
		m_responseHeaders.removeItems_NoLock(name);
	}

	void HttpResponse::clearResponseHeaders()
	{
		m_responseHeaders.removeAll_NoLock();
	}

	sl_uint64 HttpResponse::getResponseContentLengthHeader() const
	{
		String headerContentLength = getResponseHeader(HttpHeaders::ContentLength);
		if (headerContentLength.isNotEmpty()) {
			return headerContentLength.parseUint64();
		}
		return 0;
	}

	void HttpResponse::setResponseContentLengthHeader(sl_uint64 size)
	{
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

	String HttpResponse::getResponseContentRange() const
	{
		return getResponseHeader(HttpHeaders::ContentRange);
	}

	void HttpResponse::setResponseContentRange(const String& range)
	{
		setResponseHeader(HttpHeaders::ContentRange, range);
	}

	void HttpResponse::setResponseContentRange(sl_uint64 start, sl_uint64 last, sl_uint64 total)
	{
		setResponseHeader(HttpHeaders::ContentRange, String::format("bytes %d-%d/%d", start, last, total));
	}

	void HttpResponse::setResponseContentRangeUnknownTotal(sl_uint64 start, sl_uint64 last)
	{
		setResponseHeader(HttpHeaders::ContentRange, String::format("bytes %d-%d/*", start, last));
	}

	void HttpResponse::setResponseContentRangeUnsatisfied(sl_uint64 total)
	{
		setResponseHeader(HttpHeaders::ContentRange, String::format("bytes */%d", total));
	}

	String HttpResponse::getResponseAcceptRanges() const
	{
		return getResponseHeader(HttpHeaders::AcceptRanges);
	}

	void HttpResponse::setResponseAcceptRanges(sl_bool flagAcceptRanges)
	{
		if (flagAcceptRanges) {
			SLIB_STATIC_STRING(s, "bytes")
			setResponseHeader(HttpHeaders::AcceptRanges, s);
		} else {
			SLIB_STATIC_STRING(s, "none")
			setResponseHeader(HttpHeaders::AcceptRanges, s);
		}
	}

	void HttpResponse::setResponseAcceptRangesIfNotDefined(sl_bool flagAcceptRanges)
	{
		if (!(containsResponseHeader(HttpHeaders::AcceptRanges))) {
			setResponseAcceptRanges(flagAcceptRanges);
		}
	}

	String HttpResponse::getResponseAccessControlAllowOrigin() const
	{
		return getResponseHeader(HttpHeaders::AccessControlAllowOrigin);
	}

	void HttpResponse::setResponseAccessControlAllowOrigin(const String& origin)
	{
		setResponseHeader(HttpHeaders::AccessControlAllowOrigin, origin);
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
		MemoryBuffer msg;
		String strVersion = m_responseVersion;
		msg.addStatic(strVersion.getData(), strVersion.getLength());
		msg.addStatic(" ", 1);
		String strCode = String::fromUint32((sl_uint32)m_responseCode);
		msg.addStatic(strCode.getData(), strCode.getLength());
		msg.addStatic(" ", 1);
		String strMessage = m_responseMessage;
		msg.addStatic(strMessage.getData(), strMessage.getLength());
		msg.addStatic("\r\n", 2);

		for (auto& pair : m_responseHeaders) {
			String str = pair.key;
			msg.addStatic(str.getData(), str.getLength());
			msg.addStatic(": ", 2);
			str = pair.value;
			msg.addStatic(str.getData(), str.getLength());
			msg.addStatic("\r\n", 2);
		}
		msg.addStatic("\r\n", 2);
		return msg.merge();
	}

	sl_reg HttpResponse::parseResponsePacket(const void* packet, sl_size size)
	{
		const sl_char8* data = (const sl_char8*)packet;
		sl_size posCurrent = 0;
		sl_size posStart = 0;
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

		sl_reg iRet = HttpHeaders::parseHeaders(m_responseHeaders, data + posCurrent, size - posCurrent);
		if (iRet > 0) {
			return posCurrent + iRet;
		} else {
			return iRet;
		}
	}

}
