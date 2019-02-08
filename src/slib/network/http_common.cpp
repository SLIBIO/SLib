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
#include "slib/core/file.h"
#include "slib/core/scoped.h"

namespace slib
{

#define HTTP_STATUS_CASE(name, text) \
	case HttpStatus::name: SLIB_RETURN_STRING(text);

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


	SLIB_STATIC_STRING(_g_priv_http_method_GET, "GET");
	SLIB_STATIC_STRING(_g_priv_http_method_HEAD, "HEAD");
	SLIB_STATIC_STRING(_g_priv_http_method_POST, "POST");
	SLIB_STATIC_STRING(_g_priv_http_method_PUT, "PUT");
	SLIB_STATIC_STRING(_g_priv_http_method_DELETE, "DELETE");
	SLIB_STATIC_STRING(_g_priv_http_method_CONNECT, "CONNECT");
	SLIB_STATIC_STRING(_g_priv_http_method_OPTIONS, "OPTIONS");
	SLIB_STATIC_STRING(_g_priv_http_method_TRACE, "TRACE");
	SLIB_STATIC_STRING(_g_priv_http_method_PATCH, "PATCH");

	String HttpMethods::toString(HttpMethod method)
	{
		switch (method) {
			case HttpMethod::GET:
				return _g_priv_http_method_GET;
			case HttpMethod::HEAD:
				return _g_priv_http_method_HEAD;
			case HttpMethod::POST:
				return _g_priv_http_method_POST;
			case HttpMethod::PUT:
				return _g_priv_http_method_PUT;
			case HttpMethod::DELETE:
				return _g_priv_http_method_DELETE;
			case HttpMethod::CONNECT:
				return _g_priv_http_method_CONNECT;
			case HttpMethod::OPTIONS:
				return _g_priv_http_method_OPTIONS;
			case HttpMethod::TRACE:
				return _g_priv_http_method_TRACE;
			case HttpMethod::PATCH:
				return _g_priv_http_method_PATCH;
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
			maps.put_NoLock(_g_priv_http_method_GET, HttpMethod::GET);
			maps.put_NoLock(_g_priv_http_method_HEAD, HttpMethod::HEAD);
			maps.put_NoLock(_g_priv_http_method_POST, HttpMethod::POST);
			maps.put_NoLock(_g_priv_http_method_PUT, HttpMethod::PUT);
			maps.put_NoLock(_g_priv_http_method_DELETE, HttpMethod::DELETE);
			maps.put_NoLock(_g_priv_http_method_CONNECT, HttpMethod::CONNECT);
			maps.put_NoLock(_g_priv_http_method_OPTIONS, HttpMethod::OPTIONS);
			maps.put_NoLock(_g_priv_http_method_TRACE, HttpMethod::TRACE);
			maps.put_NoLock(_g_priv_http_method_PATCH, HttpMethod::PATCH);
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

	DEFINE_HTTP_HEADER(Connection, "Connection")
	DEFINE_HTTP_HEADER(CacheControl, "Cache-Control")
	DEFINE_HTTP_HEADER(ContentDisposition, "Content-Disposition")

	DEFINE_HTTP_HEADER(ContentLength, "Content-Length")
	DEFINE_HTTP_HEADER(ContentType, "Content-Type")
	DEFINE_HTTP_HEADER(ContentEncoding, "Content-Encoding")

	DEFINE_HTTP_HEADER(Host, "Host")
	DEFINE_HTTP_HEADER(AcceptEncoding, "Accept-Encoding")
	DEFINE_HTTP_HEADER(Origin, "Origin")
	DEFINE_HTTP_HEADER(Cookie, "Cookie")
	DEFINE_HTTP_HEADER(Range, "Range")
	DEFINE_HTTP_HEADER(IfModifiedSince, "If-Modified-Since")

	DEFINE_HTTP_HEADER(TransferEncoding, "Transfer-Encoding")
	DEFINE_HTTP_HEADER(AccessControlAllowOrigin, "Access-Control-Allow-Origin")
	DEFINE_HTTP_HEADER(SetCookie, "Set-Cookie")
	DEFINE_HTTP_HEADER(AcceptRanges, "Accept-Ranges")
	DEFINE_HTTP_HEADER(ContentRange, "Content-Range")
	DEFINE_HTTP_HEADER(LastModified, "Last-Modified")

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
				return SLIB_PARSE_ERROR;
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

	void HttpHeaders::splitValue(const String& value, List<String>* list, HttpHeaderValueMap* map, HashMap<String, String>* mapCaseSensitive, sl_char8 delimiter)
	{
		sl_char8* sz = value.getData();
		sl_size len = value.getLength();
		if (!len) {
			return;
		}
		sl_size i = 0;
		for (;;) {
			while (i < len && SLIB_CHAR_IS_WHITE_SPACE(sz[i])) {
				i++;
			}
			if (i >= len) {
				return;
			}
			sl_size indexName = i;
			sl_size indexEndName = i;
			sl_size indexEndValue = i;
			sl_size indexValue = i;
			sl_bool flagQuot = sl_false;
			if (sz[i] == '\"') {
				i++;
				if (i >= len) {
					return;
				}
				indexName = i;
				indexValue = i;
				flagQuot = sl_true;
			} else {
				for (;;) {
					if (sz[i] == delimiter) {
						break;
					} else if (sz[i] == '=') {
						indexEndName = indexEndValue;
						i++;
						while (i < len && SLIB_CHAR_IS_WHITE_SPACE(sz[i])) {
							i++;
						}
						if (i < len) {
							if (sz[i] == '\"') {
								i++;
								if (i < len) {
									flagQuot = sl_true;
								}
							}
						}
						indexValue = i;
						break;
					} else {
						if (!SLIB_CHAR_IS_WHITE_SPACE(sz[i])) {
							indexEndValue = i + 1;
						}
						i++;
						if (i >= len) {
							break;
						}
					}
				}
			}
			if (flagQuot) {
				for (;;) {
					if (i >= len || sz[i] == '\"') {
						if (i > indexName) {
							if (list) {
								list->add_NoLock(String(sz + indexName, i - indexName));
							}
							if (map) {
								if (indexEndName > indexName) {
									map->add_NoLock(String(sz + indexName, indexEndName - indexName), String(sz + indexValue, i - indexValue));
								} else {
									map->add_NoLock(String(sz + indexName, i - indexName), sl_null);
								}
							}
							if (mapCaseSensitive) {
								if (indexEndName > indexName) {
									mapCaseSensitive->add_NoLock(String(sz + indexName, indexEndName - indexName), String(sz + indexValue, i - indexValue));
								} else {
									mapCaseSensitive->add_NoLock(String(sz + indexName, i - indexName), sl_null);
								}
							}
						}
						i++;
						while (i < len && SLIB_CHAR_IS_WHITE_SPACE(sz[i])) {
							i++;
						}
						if (i >= len) {
							return;
						}
						if (sz[i] == delimiter) {
							i++;
						}
						break;
					} else {
						i++;
					}
				}
			} else {
				for (;;) {
					if (i >= len || (sz[i] == delimiter && !(i >= 3 && Time::parseHttpDate(sl_null, sz, i - 3, len) != SLIB_PARSE_ERROR))) {
						if (indexEndValue > indexName) {
							if (list) {
								list->add_NoLock(String(sz + indexName, indexEndValue - indexName));
							}
							if (map) {
								if (indexEndName > indexName) {
									map->add_NoLock(String(sz + indexName, indexEndName - indexName), String(sz + indexValue, indexEndValue - indexValue));
								} else {
									map->add_NoLock(String(sz + indexName, indexEndValue - indexName), sl_null);
								}
							}
							if (mapCaseSensitive) {
								if (indexEndName > indexName) {
									mapCaseSensitive->add_NoLock(String(sz + indexName, indexEndName - indexName), String(sz + indexValue, indexEndValue - indexValue));
								} else {
									mapCaseSensitive->add_NoLock(String(sz + indexName, indexEndValue - indexName), sl_null);
								}
							}
						}
						i++;
						if (i >= len) {
							return;
						}
						break;
					} else {
						if (!SLIB_CHAR_IS_WHITE_SPACE(sz[i])) {
							indexEndValue = i + 1;
						}
						i++;
					}
				}
			}
		}
	}

	List<String> HttpHeaders::splitValueToList(const String& value, sl_char8 delimiter)
	{
		List<String> list;
		splitValue(value, &list, sl_null, sl_null, delimiter);
		return list;
	}
	
	HttpHeaderValueMap HttpHeaders::splitValueToMap(const String& value, sl_char8 delimiter)
	{
		HttpHeaderValueMap map;
		splitValue(value, sl_null, &map, sl_null, delimiter);
		return map;
	}
	
	String HttpHeaders::makeSafeValue(const String& value, sl_char8 delimiter)
	{
		sl_char8* sz = value.getData();
		sl_size len = value.getLength();
		sl_bool flagNeedRemoveQuot = sl_false;
		sl_bool flagNeedQuot = sl_false;
		for (sl_size i = 0; i < len; i++) {
			if (sz[i] == '\"') {
				flagNeedRemoveQuot = sl_true;
			} else if (sz[i] == delimiter && !flagNeedQuot) {
				if (!(i >= 3 && Time::parseHttpDate(sl_null, sz, i - 3, len) != SLIB_PARSE_ERROR)) {
					flagNeedQuot = sl_true;
				}
			}
		}
		String s = value;
		if (flagNeedRemoveQuot) {
			s = s.replaceAll("\"", sl_null);
		}
		if (flagNeedQuot) {
			StringBuffer sb;
			sb.addStatic("\"", 1);
			sb.add(s);
			sb.addStatic("\"", 1);
			s = sb.merge();
		}
		return s;
	}
	
	String HttpHeaders::mergeValues(const List<String>& list, sl_char8 delimiter)
	{
		StringBuffer sb;
		ListLocker<String> values(list);
		for (sl_size i = 0; i < values.count; i++) {
			if (values[i].isNotEmpty()) {
				if (sb.getLength() > 0) {
					sb.addStatic(&delimiter, 1);
					sb.addStatic(" ", 1);
				}
				sb.add(makeSafeValue(values[i], delimiter));
			}
		}
		return sb.merge();
	}
	
	String HttpHeaders::mergeValueMap(const HttpHeaderMap& map, sl_char8 delimiter)
	{
		MutexLocker lock(map.getLocker());
		StringBuffer sb;
		for (auto& item: map) {
			if (item.key.isNotEmpty()) {
				if (sb.getLength() > 0) {
					sb.addStatic(&delimiter, 1);
					sb.addStatic(" ", 1);
				}
				if (item.value.isNull()) {
					sb.add(makeSafeValue(item.key));
				} else {
					sb.add(makeSafeValue(item.key));
					sb.addStatic("=", 1);
					sb.add(makeSafeValue(item.value));
				}
			}
		}
		return sb.merge();
	}

	String HttpHeaders::mergeValueMapCaseSensitive(const HashMap<String, String>& map, sl_char8 delimiter)
	{
		MutexLocker lock(map.getLocker());
		StringBuffer sb;
		for (auto& item: map) {
			if (item.key.isNotEmpty()) {
				if (sb.getLength() > 0) {
					sb.addStatic(&delimiter, 1);
					sb.addStatic(" ", 1);
				}
				if (item.value.isNull()) {
					sb.add(makeSafeValue(item.key));
				} else {
					sb.add(makeSafeValue(item.key));
					sb.addStatic("=", 1);
					sb.add(makeSafeValue(item.value));
				}
			}
		}
		return sb.merge();
	}
	
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_expires, "Expires")
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_max_age, "Max-Age")
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_domain, "Domain")
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_path, "Path")
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_secure, "Secure")
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_http_only, "HttpOnly")
	SLIB_STATIC_STRING(_g_priv_http_set_cookie_same_site, "SameSite")
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(HttpCookie)
	
	HttpCookie::HttpCookie()
	{
	}
	
	String HttpCookie::toHeaderValue() const
	{
		HttpHeaderValueMap map;
		map.put_NoLock(name, value);
		if (expires.isNotZero()) {
			map.put_NoLock(_g_priv_http_set_cookie_expires, expires.toHttpDate());
		}
		if (max_age.isNotNull()) {
			map.put_NoLock(_g_priv_http_set_cookie_max_age, String::fromInt32(max_age));
		}
		if (domain.isNotNull()) {
			map.put_NoLock(_g_priv_http_set_cookie_domain, domain);
		}
		if (path.isNotNull()) {
			map.put_NoLock(_g_priv_http_set_cookie_path, path);
		}
		if (secure) {
			map.put_NoLock(_g_priv_http_set_cookie_path, sl_null);
		}
		if (http_only) {
			map.put_NoLock(_g_priv_http_set_cookie_http_only, sl_null);
		}
		if (same_site.isNotNull()) {
			map.put_NoLock(_g_priv_http_set_cookie_same_site, same_site);
		}
		return HttpHeaders::mergeValueMap(map, ';');
	}
	
	void HttpCookie::parseHeaderValue(const String& headerValue)
	{
		HttpHeaderValueMap map;
		HttpHeaders::splitValue(headerValue, sl_null, &map, sl_null, ';');
		HashMapNode<String, String>* node = map.getFirstNode();
		if (node) {
			name = node->key;
			value = node->value;
		} else {
			name.setNull();
			value.setNull();
		}
		Time t;
		if (t.parseHttpDate(map.getValue_NoLock(_g_priv_http_set_cookie_expires))) {
			expires = t;
		} else {
			expires.setZero();
		}
		sl_int32 n;
		if (map.getValue_NoLock(_g_priv_http_set_cookie_max_age).parseInt32(10, &n)) {
			max_age = n;
		} else {
			max_age.setNull();
		}
		domain = map.getValue_NoLock(_g_priv_http_set_cookie_domain);
		path = map.getValue_NoLock(_g_priv_http_set_cookie_path);
		secure = map.find_NoLock(_g_priv_http_set_cookie_secure);
		http_only = map.find_NoLock(_g_priv_http_set_cookie_http_only);
		same_site = map.getValue_NoLock(_g_priv_http_set_cookie_same_site);
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(HttpUploadFile)
	
	HttpUploadFile::HttpUploadFile(const String& fileName, const HttpHeaderMap& headers, void* data, sl_size size, const Ref<Referable>& ref)
	 : m_fileName(fileName), m_headers(headers), m_data(data), m_size(size), m_ref(ref)
	{
	}
	
	String HttpUploadFile::getFileName()
	{
		return m_fileName;
	}
	
	const HttpHeaderMap& HttpUploadFile::getHeaders()
	{
		return m_headers;
	}
	
	String HttpUploadFile::getHeader(const String& name)
	{
		return m_headers.getValue_NoLock(name);
	}
	
	String HttpUploadFile::getContentType()
	{
		return getHeader(HttpHeaders::ContentType);
	}
	
	void* HttpUploadFile::getData()
	{
		return m_data;
	}
	
	sl_size HttpUploadFile::getSize()
	{
		return m_size;
	}
	
	sl_bool HttpUploadFile::saveToFile(const String& path)
	{
		return File::writeAllBytes(path, m_data, m_size);
	}
	
	
/***********************************************************************
							HttpRequest
***********************************************************************/

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(HttpRequest)
	
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

	void HttpRequest::setQuery(const String& query)
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

	String HttpRequest::getRequestHeader(const String& name) const
	{
		String value = m_requestHeaders.getValue_NoLock(name, String::null());
		sl_size len = value.getLength();
		if (len >= 2 && value.startsWith('\"') && value.endsWith('\"')) {
			return value.substring(1, len - 1);
		}
		return value;
	}
	
	void HttpRequest::setRequestHeader(const String& name, const String& value)
	{
		m_requestHeaders.put_NoLock(name, value);
	}

	void HttpRequest::addRequestHeader(const String& name, const String& value)
	{
		m_requestHeaders.add_NoLock(name, value);
	}

	sl_bool HttpRequest::containsRequestHeader(const String& name) const
	{
		return m_requestHeaders.find_NoLock(name) != sl_null;
	}

	void HttpRequest::removeRequestHeader(const String& name)
	{
		m_requestHeaders.removeItems_NoLock(name);
	}

	List<String> HttpRequest::getRequestHeaderValues(const String& name) const
	{
		List<String> list;
		MapNode<String, String>* node;
		MapNode<String, String>* nodeEnd;
		if (m_requestHeaders.getEqualRange(name, &node, &nodeEnd)) {
			for (;;) {
				HttpHeaders::splitValue(node->value, &list, sl_null, sl_null);
				if (node == nodeEnd) {
					break;
				}
				node = node->getNext();
			}
		}
		return list;
	}
	
	void HttpRequest::setRequestHeaderValues(const String& name, const List<String>& list)
	{
		m_requestHeaders.put_NoLock(name, HttpHeaders::mergeValues(list));
	}
	
	void HttpRequest::addRequestHeaderValues(const String& name, const List<String>& list)
	{
		m_requestHeaders.add_NoLock(name, HttpHeaders::mergeValues(list));
	}
	
	HttpHeaderValueMap HttpRequest::getRequestHeaderValueMap(const String& name) const
	{
		HttpHeaderValueMap map;
		MapNode<String, String>* node;
		MapNode<String, String>* nodeEnd;
		if (m_requestHeaders.getEqualRange(name, &node, &nodeEnd)) {
			for (;;) {
				HttpHeaders::splitValue(node->value, sl_null, &map, sl_null);
				if (node == nodeEnd) {
					break;
				}
				node = node->getNext();
			}
		}
		return map;
	}

	void HttpRequest::setRequestHeaderValueMap(const String& name, const HttpHeaderValueMap& map)
	{
		m_requestHeaders.put_NoLock(name, HttpHeaders::mergeValueMap(map));
	}
	
	void HttpRequest::addRequestHeaderValueMap(const String& name, const HttpHeaderValueMap& map)
	{
		m_requestHeaders.add_NoLock(name, HttpHeaders::mergeValueMap(map));
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

	sl_bool HttpRequest::isRequestMultipartFormData() const
	{
		return getRequestContentTypeNoParams().trim().equalsIgnoreCase(ContentTypes::MultipartFormData);
	}
	
	String HttpRequest::getRequestMultipartFormDataBoundary() const
	{
		String value = getRequestContentType();
		HttpHeaderValueMap map = HttpHeaders::splitValueToMap(value, ';');
		if (map.find_NoLock(ContentTypes::MultipartFormData)) {
			SLIB_STATIC_STRING(t, "boundary")
			return map.getValue_NoLock(t);
		}
		return sl_null;
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
	
	sl_bool HttpRequest::isKeepAlive() const
	{
		SLIB_STATIC_STRING(str, "Keep-Alive");
		return getRequestHeader(HttpHeaders::Connection).equalsIgnoreCase(str);
	}
	
	void HttpRequest::setKeepAlive()
	{
		SLIB_STATIC_STRING(str, "Keep-Alive");
		setRequestHeader(HttpHeaders::Connection, str);
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
	
	Time HttpRequest::getRequestIfModifiedSince() const
	{
		Time time;
		if (time.parseHttpDate(getRequestHeader(HttpHeaders::IfModifiedSince))) {
			return time;
		}
		return Time::zero();
	}
	
	void HttpRequest::setRequestIfModifiedSince(const Time& time)
	{
		if (time.isNotZero()) {
			setRequestHeader(HttpHeaders::IfModifiedSince, time.toHttpDate());
		} else {
			removeRequestHeader(HttpHeaders::IfModifiedSince);
		}
	}
	
	SLIB_STATIC_STRING(_g_priv_http_cache_control_max_age, "max-age")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_max_stale, "max-stale")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_min_fresh, "min-fresh")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_no_cache, "no-cache")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_no_store, "no-store")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_no_transform, "no-transform")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_only_if_cached, "only-if-cached")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_must_revalidate, "must-revalidate")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_public, "public")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_private, "private")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_proxy_revalidate, "proxy-revalidate")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_s_maxage, "s-maxage")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_immutable, "immutable")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_stale_while_revalidate, "stale-while-revalidate")
	SLIB_STATIC_STRING(_g_priv_http_cache_control_stale_if_error, "stale-if-error")

	HttpCacheControlRequest HttpRequest::getRequestCacheControl() const
	{
		HttpCacheControlRequest cc;
		HttpHeaderValueMap map = getRequestHeaderValueMap(HttpHeaders::CacheControl);
		sl_int32 n;
		if (map.getValue_NoLock(_g_priv_http_cache_control_max_age).parseInt32(10, &n)) {
			cc.max_age = n;
		}
		if (map.getValue_NoLock(_g_priv_http_cache_control_max_stale).parseInt32(10, &n)) {
			cc.max_stale = n;
		}
		if (map.getValue_NoLock(_g_priv_http_cache_control_min_fresh).parseInt32(10, &n)) {
			cc.min_fresh = n;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_no_cache)) {
			cc.no_cache = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_no_store)) {
			cc.no_store = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_no_transform)) {
			cc.no_transform = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_only_if_cached)) {
			cc.only_if_cached = sl_true;
		}
		return cc;
	}
	
	void HttpRequest::setRequestCacheControl(const HttpCacheControlRequest& cc)
	{
		HttpHeaderValueMap map;
		if (cc.max_age.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_max_age, String::fromInt32(cc.max_age));
		}
		if (cc.max_stale.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_max_stale, String::fromInt32(cc.max_stale));
		}
		if (cc.min_fresh.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_min_fresh, String::fromInt32(cc.min_fresh));
		}
		if (cc.no_cache) {
			map.put_NoLock(_g_priv_http_cache_control_no_cache, sl_null);
		}
		if (cc.no_store) {
			map.put_NoLock(_g_priv_http_cache_control_no_store, sl_null);
		}
		if (cc.no_transform) {
			map.put_NoLock(_g_priv_http_cache_control_no_transform, sl_null);
		}
		if (cc.only_if_cached) {
			map.put_NoLock(_g_priv_http_cache_control_only_if_cached, sl_null);
		}
		if (map.isNotEmpty()) {
			setRequestHeaderValueMap(HttpHeaders::CacheControl, map);
		} else {
			removeRequestHeader(HttpHeaders::CacheControl);
		}
	}
	
	HashMap<String, String> HttpRequest::getRequestCookies() const
	{
		HashMap<String, String> map;
		MapNode<String, String>* node;
		MapNode<String, String>* nodeEnd;
		if (m_requestHeaders.getEqualRange(HttpHeaders::Cookie, &node, &nodeEnd)) {
			for (;;) {
				HttpHeaders::splitValue(node->value, sl_null, sl_null, &map, ';');
				if (node == nodeEnd) {
					break;
				}
				node = node->getNext();
			}
		}
		return map;
	}
	
	void HttpRequest::setRequestCookies(const HashMap<String, String>& cookies)
	{
		String value = HttpHeaders::mergeValueMapCaseSensitive(cookies);
		setRequestHeader(HttpHeaders::Cookie, value);
	}
	
	String HttpRequest::getRequestCookie(const String& cookie) const
	{
		return getRequestCookies().getValue_NoLock(cookie);
	}
	
	void HttpRequest::setRequestCookie(const String& name, const String& value)
	{
		HashMap<String, String> cookies = getRequestCookies();
		cookies.put_NoLock(name, value);
		setRequestCookies(cookies);
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

	String HttpRequest::getQueryParameter(const String& name) const
	{
		return m_queryParameters.getValue_NoLock(name, String::null());
	}

	List<String> HttpRequest::getQueryParameterValues(const String& name) const
	{
		return m_queryParameters.getValues_NoLock(name);
	}

	sl_bool HttpRequest::containsQueryParameter(const String& name) const
	{
		return m_queryParameters.find_NoLock(name) != sl_null;
	}

	const HashMap<String, String>& HttpRequest::getPostParameters() const
	{
		return m_postParameters;
	}

	String HttpRequest::getPostParameter(const String& name) const
	{
		return m_postParameters.getValue_NoLock(name, String::null());
	}

	List<String> HttpRequest::getPostParameterValues(const String& name) const
	{
		return m_postParameters.getValues_NoLock(name);
	}

	sl_bool HttpRequest::containsPostParameter(const String& name) const
	{
		return m_postParameters.find_NoLock(name) != sl_null;
	}

	void HttpRequest::applyPostParameters(const void* data, sl_size size)
	{
		HashMap<String, String> params = parseParameters(data, size);
		m_postParameters.addAll_NoLock(params);
		m_parameters.addAll_NoLock(params);
	}

	void HttpRequest::applyPostParameters(const String& str)
	{
		applyPostParameters(str.getData(), str.getLength());
	}

	void HttpRequest::applyQueryToParameters()
	{
		HashMap<String, String> params = parseParameters(m_query);
		m_queryParameters.addAll_NoLock(params);
		m_parameters.addAll_NoLock(params);
	}

	HashMap<String, String> HttpRequest::parseParameters(const String& str)
	{
		return parseParameters(str.getData(), str.getLength());
	}

	HashMap<String, String> HttpRequest::parseParameters(const void* data, sl_size len)
	{
		if (!data) {
			return sl_null;
		}
		if (!len) {
			return sl_null;
		}
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
					ret.add_NoLock(name, value);
				} else {
					String name = String::fromUtf8(buf + start, pos - start);
					ret.add_NoLock(name, String::null());
				}
				start = pos + 1;
				indexSplit = start;
			}
		}
		return ret;
	}
	
	const HashMap< String, Ref<HttpUploadFile> >& HttpRequest::getUploadFiles() const
	{
		return m_uploadFiles;
	}
	
	Ref<HttpUploadFile> HttpRequest::getUploadFile(const String& name) const
	{
		return m_uploadFiles.getValue_NoLock(name);
	}
	
	List< Ref<HttpUploadFile> > HttpRequest::getUploadFiles(const String& name) const
	{
		return m_uploadFiles.getValues_NoLock(name);
	}
	
	sl_bool HttpRequest::containsUploadFile(const String& name) const
	{
		return m_uploadFiles.find_NoLock(name);
	}

	void HttpRequest::applyMultipartFormData(const String& _boundary, const Memory& body)
	{
		SLIB_STATIC_STRING(s1, "name")
		SLIB_STATIC_STRING(s2, "filename")
		sl_char8* data = (sl_char8*)(body.getData());
		sl_size size = body.getSize();
		sl_size sizeBoundary = 2 + _boundary.getLength();
		SLIB_SCOPED_BUFFER(sl_char8, 512, boundary, sizeBoundary)
		boundary[0] = '-';
		boundary[1] = '-';
		Base::copyMemory(boundary + 2, _boundary.getData(), sizeBoundary - 2);
		if (sizeBoundary + 2 > size) {
			return;
		}
		if (!(Base::equalsMemory(data, boundary, sizeBoundary))) {
			return;
		}
		if (data[sizeBoundary] != '\r' || data[sizeBoundary + 1] != '\n') {
			return;
		}
		sl_size i = sizeBoundary + 2;
		for (;;) {
			HttpHeaderMap map;
			sl_reg lenHeader = HttpHeaders::parseHeaders(map, data + i, size - i);
			if (lenHeader <= 0) {
				return;
			}
			i += lenHeader;
			if (i > size) {
				return;
			}
			sl_size posStart = i;
			sl_size posEnd = size;
			for (; i + 2 + sizeBoundary <= size;) {
				if (data[i] == '\r' && data[i + 1] == '\n' && Base::equalsMemory(data + i + 2, boundary, sizeBoundary)) {
					posEnd = i;
					break;
				} else {
					i++;
				}
			}
			String header = map.getValue_NoLock(HttpHeaders::ContentDisposition);
			HttpHeaderMap fields = HttpHeaders::splitValueToMap(header, ';');
			String name = fields.getValue_NoLock(s1);
			String fileName = fields.getValue_NoLock(s2);
			if (fileName.isNull()) {
				String value(data + posStart, posEnd - posStart);
				m_postParameters.add_NoLock(name, value);
				m_parameters.add_NoLock(name, value);
			} else {
				Ref<HttpUploadFile> file = new HttpUploadFile(fileName, map, data + posStart, posEnd - posStart, body.ref);
				m_uploadFiles.add_NoLock(name, file);
			}
			if (i + 2 + sizeBoundary >= size) {
				return;
			}
			i += 2 + sizeBoundary;
			if (i + 2 >= size) {
				return;
			}
			if (data[i] != '\r' || data[i + 1] != '\n') {
				return;
			}
			i += 2;
		}
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

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(HttpResponse)
	
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

	void HttpResponse::setResponseMessage(const String& message)
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

	String HttpResponse::getResponseHeader(const String& name) const
	{
		String value = m_responseHeaders.getValue_NoLock(name, String::null());
		sl_size len = value.getLength();
		if (len >= 2 && value.startsWith('\"') && value.endsWith('\"')) {
			return value.substring(1, len - 1);
		}
		return value;
	}

	void HttpResponse::setResponseHeader(const String& name, const String& value)
	{
		m_responseHeaders.put_NoLock(name, value);
	}

	void HttpResponse::addResponseHeader(const String& name, const String& value)
	{
		m_responseHeaders.add_NoLock(name, value);
	}

	sl_bool HttpResponse::containsResponseHeader(const String& name) const
	{
		return m_responseHeaders.find_NoLock(name) != sl_null;
	}

	void HttpResponse::removeResponseHeader(const String& name)
	{
		m_responseHeaders.removeItems_NoLock(name);
	}

	List<String> HttpResponse::getResponseHeaderValues(const String& name) const
	{
		List<String> list;
		MapNode<String, String>* node;
		MapNode<String, String>* nodeEnd;
		if (m_responseHeaders.getEqualRange(name, &node, &nodeEnd)) {
			for (;;) {
				HttpHeaders::splitValue(node->value, &list, sl_null, sl_null);
				if (node == nodeEnd) {
					break;
				}
				node = node->getNext();
			}
		}
		return list;
	}
	
	void HttpResponse::setResponseHeaderValues(const String& name, const List<String>& list)
	{
		m_responseHeaders.put_NoLock(name, HttpHeaders::mergeValues(list));
	}
	
	void HttpResponse::addResponseHeaderValues(const String& name, const List<String>& list)
	{
		m_responseHeaders.add_NoLock(name, HttpHeaders::mergeValues(list));
	}

	HttpHeaderValueMap HttpResponse::getResponseHeaderValueMap(const String& name) const
	{
		HttpHeaderValueMap map;
		MapNode<String, String>* node;
		MapNode<String, String>* nodeEnd;
		if (m_responseHeaders.getEqualRange(name, &node, &nodeEnd)) {
			for (;;) {
				HttpHeaders::splitValue(node->value, sl_null, &map, sl_null);
				if (node == nodeEnd) {
					break;
				}
				node = node->getNext();
			}
		}
		return map;
	}

	void HttpResponse::setResponseHeaderValueMap(const String& name, const HttpHeaderValueMap& map)
	{
		m_responseHeaders.put_NoLock(name, HttpHeaders::mergeValueMap(map));
	}
	
	void HttpResponse::addResponseHeaderValueMap(const String& name, const HttpHeaderValueMap& map)
	{
		m_responseHeaders.add_NoLock(name, HttpHeaders::mergeValueMap(map));
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

	sl_bool HttpResponse::isChunkedResponse() const
	{
		String te = getResponseTransferEncoding();
		if (te.endsWith("chunked")) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool HttpResponse::isAttachmentResponse() const
	{
		String value = getResponseHeader(HttpHeaders::ContentDisposition);
		if (value.startsWith("attachment")) {
			return sl_true;
		}
		return sl_false;
	}
	
	String HttpResponse::getResponseAttachmentFileName() const
	{
		HttpHeaderValueMap map = getResponseHeaderValueMap(HttpHeaders::ContentDisposition);
		SLIB_STATIC_STRING(s, "filename")
		return map.getValue_NoLock(s);
	}
	
	void HttpResponse::setResponseInline()
	{
		SLIB_STATIC_STRING(s, "inline")
		setResponseHeader(HttpHeaders::ContentDisposition, s);
	}
	
	void HttpResponse::setResponseAttachment(const String& fileName)
	{
		if (fileName.isEmpty()) {
			SLIB_STATIC_STRING(s, "attachment")
			setResponseHeader(HttpHeaders::ContentDisposition, s);
		} else {
			setResponseHeader(HttpHeaders::ContentDisposition, String::format("attachment; filename=\"%s\"", fileName));
		}
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
	
	Time HttpResponse::getResponseLastModified() const
	{
		Time time;
		if (time.parseHttpDate(getResponseHeader(HttpHeaders::LastModified))) {
			return time;
		}
		return Time::zero();
	}
	
	void HttpResponse::setResponseLastModified(const Time& time)
	{
		if (time.isNotZero()) {
			setResponseHeader(HttpHeaders::LastModified, time.toHttpDate());
		} else {
			removeResponseHeader(HttpHeaders::LastModified);
		}
	}
	
	HttpCacheControlResponse HttpResponse::getResponseCacheControl() const
	{
		HttpCacheControlResponse cc;
		HttpHeaderValueMap map = getResponseHeaderValueMap(HttpHeaders::CacheControl);
		sl_int32 n;
		if (map.find_NoLock(_g_priv_http_cache_control_must_revalidate)) {
			cc.must_revalidate = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_no_cache)) {
			cc.no_cache = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_no_store)) {
			cc.no_store = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_no_transform)) {
			cc.no_transform = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_public)) {
			cc.public_ = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_private)) {
			cc.private_ = sl_true;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_proxy_revalidate)) {
			cc.proxy_revalidate = sl_true;
		}
		if (map.getValue_NoLock(_g_priv_http_cache_control_max_age).parseInt32(10, &n)) {
			cc.max_age = n;
		}
		if (map.getValue_NoLock(_g_priv_http_cache_control_s_maxage).parseInt32(10, &n)) {
			cc.s_maxage = n;
		}
		if (map.find_NoLock(_g_priv_http_cache_control_immutable)) {
			cc.immutable = sl_true;
		}
		if (map.getValue_NoLock(_g_priv_http_cache_control_stale_while_revalidate).parseInt32(10, &n)) {
			cc.stale_while_revalidate = n;
		}
		if (map.getValue_NoLock(_g_priv_http_cache_control_stale_if_error).parseInt32(10, &n)) {
			cc.stale_if_error = n;
		}
		return cc;
	}
	
	void HttpResponse::setResponseCacheControl(const HttpCacheControlResponse& cc)
	{
		HttpHeaderValueMap map;
		if (cc.must_revalidate) {
			map.put_NoLock(_g_priv_http_cache_control_must_revalidate, sl_null);
		}
		if (cc.no_cache) {
			map.put_NoLock(_g_priv_http_cache_control_no_cache, sl_null);
		}
		if (cc.no_store) {
			map.put_NoLock(_g_priv_http_cache_control_no_store, sl_null);
		}
		if (cc.no_transform) {
			map.put_NoLock(_g_priv_http_cache_control_no_transform, sl_null);
		}
		if (cc.public_) {
			map.put_NoLock(_g_priv_http_cache_control_public, sl_null);
		}
		if (cc.private_) {
			map.put_NoLock(_g_priv_http_cache_control_private, sl_null);
		}
		if (cc.proxy_revalidate) {
			map.put_NoLock(_g_priv_http_cache_control_proxy_revalidate, sl_null);
		}
		if (cc.max_age.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_max_age, String::fromInt32(cc.max_age));
		}
		if (cc.s_maxage.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_s_maxage, String::fromInt32(cc.s_maxage));
		}
		if (cc.immutable) {
			map.put_NoLock(_g_priv_http_cache_control_immutable, sl_null);
		}
		if (cc.stale_while_revalidate.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_stale_while_revalidate, String::fromInt32(cc.stale_while_revalidate));
		}
		if (cc.stale_if_error.isNotNull()) {
			map.put_NoLock(_g_priv_http_cache_control_stale_if_error, String::fromInt32(cc.stale_if_error));
		}
		if (map.isNotEmpty()) {
			setResponseHeaderValueMap(HttpHeaders::CacheControl, map);
		} else {
			removeResponseHeader(HttpHeaders::CacheControl);
		}
	}

	List<HttpCookie> HttpResponse::getResponseCookies() const
	{
		List<HttpCookie> list;
		ListElements<String> values(getResponseHeaderValues(HttpHeaders::SetCookie));
		for (sl_size i = 0; i < values.count; i++) {
			HttpCookie cookie;
			cookie.parseHeaderValue(values[i]);
			list.add_NoLock(cookie);
		}
		return list;
	}
	
	void HttpResponse::setResponseCookies(const List<HttpCookie>& _cookies)
	{
		m_responseHeaders.removeItems_NoLock(HttpHeaders::SetCookie);
		ListLocker<HttpCookie> cookies(_cookies);
		for (sl_size i = 0; i < cookies.count; i++) {
			HttpCookie& cookie = cookies[i];
			m_responseHeaders.add_NoLock(HttpHeaders::SetCookie, cookie.toHeaderValue());
		}
	}
	
	HashMap<String, HttpCookie> HttpResponse::getResponseCookieMap() const
	{
		HashMap<String, HttpCookie> map;
		ListElements<String> values(getResponseHeaderValues(HttpHeaders::SetCookie));
		for (sl_size i = 0; i < values.count; i++) {
			HttpCookie cookie;
			cookie.parseHeaderValue(values[i]);
			map.add_NoLock(cookie.name, cookie);
		}
		return map;
	}
	
	void HttpResponse::setResponseCookieMap(const HashMap<String, HttpCookie>& map)
	{
		m_responseHeaders.removeItems_NoLock(HttpHeaders::SetCookie);
		MutexLocker lock(map.getLocker());
		for (auto& item : map) {
			m_responseHeaders.add_NoLock(HttpHeaders::SetCookie, item.value.toHeaderValue());
		}
	}
	
	sl_bool HttpResponse::getResponseCookie(const String& name, HttpCookie* cookie)
	{
		return getResponseCookieMap().get_NoLock(name, cookie);
	}
	
	String HttpResponse::getResponseCookie(const String& name)
	{
		HttpCookie cookie;
		if (getResponseCookieMap().get_NoLock(name, &cookie)) {
			return cookie.value;
		}
		return String::null();
	}
	
	void HttpResponse::setResponseCookie(const HttpCookie& cookie)
	{
		HashMap<String, HttpCookie> map = getResponseCookieMap();
		map.put_NoLock(cookie.name, cookie);
		setResponseCookieMap(map);
	}
	
	void HttpResponse::addResponseCookie(const HttpCookie& cookie)
	{
		m_responseHeaders.add_NoLock(HttpHeaders::SetCookie, cookie.toHeaderValue());
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
