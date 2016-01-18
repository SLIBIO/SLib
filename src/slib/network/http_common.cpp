#include "../../../inc/slib/network/http_common.h"

#include "../../../inc/slib/core/url.h"

SLIB_NETWORK_NAMESPACE_BEGIN
struct _Http_StaticConsts
{
	Memory memSp;
	Memory memCr;
	Memory memColon;
	Memory memSlash;
	Memory memQuestion;
	String strHttp1_1;
	String strContentLength;
	String strContentType;
	String strTransferEncoding;
	String strContentEncoding;
	String strHost;
	String strGet;
	String strChunked;

	_Http_StaticConsts()
	{
		memSp = Memory::createStatic(" ", 1);
		memCr = Memory::createStatic("\r\n", 2);
		memColon = Memory::createStatic(": ", 2);
		memSlash = Memory::createStatic("/", 1);
		memQuestion = Memory::createStatic("?", 1);
		strHttp1_1 = "HTTP/1.1";
		strContentLength = SLIB_HTTP_HEADER_CONTENT_LENGTH;
		strContentType = SLIB_HTTP_HEADER_CONTENT_TYPE;
		strTransferEncoding = SLIB_HTTP_HEADER_TRANSFER_ENCODING;
		strContentEncoding = SLIB_HTTP_HEADER_CONTENT_ENCODING;
		strHost = SLIB_HTTP_HEADER_HOST;
		strGet = SLIB_HTTP_METHOD_GET;
		strChunked = "chunked";
	}
};
_Http_StaticConsts& _Http_StaticConsts_get()
{
	SLIB_SAFE_STATIC(_Http_StaticConsts, ret);
	return ret;
}

/***********************************************************************
	HttpRequestHeader
***********************************************************************/
HttpRequestHeader::HttpRequestHeader()
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	m_requestVersion = _consts.strHttp1_1;
	m_method = _consts.strGet;
	m_methodUpper = _consts.strGet;
}

void HttpRequestHeader::setMethod(const String& method)
{
	m_method = method;
	m_methodUpper = m_method.toUpper();
}

String HttpRequestHeader::getRequestHeader(String name) const
{
	return m_requestHeaders.getValue(name, String::null());
}

List<String> HttpRequestHeader::getRequestHeaderValues(String name) const
{
	return m_requestHeaders.getValues(name);
}

void HttpRequestHeader::setRequestHeader(String name, String value)
{
	m_requestHeaders.put(name, value);
}

void HttpRequestHeader::addRequestHeader(String name, String value)
{
	m_requestHeaders.add(name, value);
}

sl_bool HttpRequestHeader::containsRequestHeader(String name) const
{
	return m_requestHeaders.containsKey(name);
}

void HttpRequestHeader::removeRequestHeader(String name)
{
	m_requestHeaders.removeAllMatchingKeys(name);
}

void HttpRequestHeader::clearRequestHeaders()
{
	m_requestHeaders.removeAll();
}


sl_uint64 HttpRequestHeader::getRequestContentLengthHeader() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	String headerContentLength = getRequestHeader(_consts.strContentLength);
	if (headerContentLength.isNotEmpty()) {
		return headerContentLength.parseUint64();
	}
	return 0;
}

void HttpRequestHeader::setRequestContentLengthHeader(sl_int64 size)
{
	if (size < 0) {
		size = 0;
	}
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setRequestHeader(_consts.strContentLength, String::fromUint64(size));
}

String HttpRequestHeader::getRequestContentType() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getRequestHeader(_consts.strContentType);
}

void HttpRequestHeader::setRequestContentType(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setRequestHeader(_consts.strContentType, type);
}

String HttpRequestHeader::getRequestContentEncoding() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getRequestHeader(_consts.strContentEncoding);
}

void HttpRequestHeader::setRequestContentEncoding(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setRequestHeader(_consts.strContentEncoding, type);
}

String HttpRequestHeader::getRequestTransferEncoding() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getRequestHeader(_consts.strTransferEncoding);
}

void HttpRequestHeader::setRequestTransferEncoding(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setRequestHeader(_consts.strTransferEncoding, type);
}

sl_bool HttpRequestHeader::isChunkedRequest() const
{
	String te = getRequestTransferEncoding();
	if (te.endsWith("chunked")) {
		return sl_true;
	}
	return sl_false;
}

String HttpRequestHeader::getHost() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getRequestHeader(_consts.strHost);
}

void HttpRequestHeader::setHost(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setRequestHeader(_consts.strHost, type);
}


String HttpRequestHeader::getParameter(const String& name) const
{
	return m_parameters.getValue(name, String::null());
}

List<String> HttpRequestHeader::getParameterValues(const String& name) const
{
	return m_parameters.getValues(name);
}

sl_bool HttpRequestHeader::containsParameter(const String& name) const
{
	return m_parameters.containsKey(name);
}

String HttpRequestHeader::getQueryParameter(String name) const
{
	return m_queryParameters.getValue(name, String::null());
}

List<String> HttpRequestHeader::getQueryParameterValues(String name) const
{
	return m_queryParameters.getValues(name);
}

sl_bool HttpRequestHeader::containsQueryParameter(String name) const
{
	return m_queryParameters.containsKey(name);
}

String HttpRequestHeader::getPostParameter(String name) const
{
	return m_postParameters.getValue(name, String::null());
}

List<String> HttpRequestHeader::getPostParameterValues(String name) const
{
	return m_postParameters.getValues(name);
}

sl_bool HttpRequestHeader::containsPostParameter(String name) const
{
	return m_postParameters.containsKey(name);
}


void HttpRequestHeader::applyPostParameters(const void* data, sl_size size)
{
	Map<String, String> params = parseParameters(data, size);
	m_postParameters.put(params.getObject());
	m_parameters.put(params.getObject());
}

void HttpRequestHeader::applyPostParameters(const String& str)
{
	applyPostParameters(str.getBuf(), str.getLength());
}

void HttpRequestHeader::applyQueryToParameters()
{
	Map<String, String> params = parseParameters(m_query);
	m_queryParameters.put(params.getObject());
	m_parameters.put(params.getObject());
}

Map<String, String> HttpRequestHeader::parseParameters(const String& str)
{
	return parseParameters(str.getBuf(), str.getLength());
}

Map<String, String> HttpRequestHeader::parseParameters(const void* data, sl_size _len)
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

Memory HttpRequestHeader::makeRequestPacket() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	String8 str;
	MemoryBuffer msg;
	str = m_method;
	msg.add(str.toMemory());
	msg.add(_consts.memSp);
	str = m_path;
	if (str.isEmpty()) {
		msg.add(_consts.memSlash);
	} else {
		msg.add(str.toMemory());
	}
	if (m_query.isNotEmpty()) {
		msg.add(_consts.memQuestion);
		str = m_query;
		msg.add(str.toMemory());
	}
	msg.add(_consts.memSp);
	str = m_requestVersion;
	msg.add(str.toMemory());
	msg.add(_consts.memCr);

	Iterator< Pair<String, String> > iterator = m_requestHeaders.iterator();
	Pair<String, String> pair;
	while (iterator.next(&pair)) {
		str = pair.key;
		msg.add(str.toMemory());
		msg.add(_consts.memColon);
		str = pair.value;
		msg.add(str.toMemory());
		msg.add(_consts.memCr);
	}
	msg.add(_consts.memCr);
	return msg.merge();
}

sl_reg HttpRequestHeader::parseRequestPacket(const void* packet, sl_size _size)
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
	HttpResponseHeader
***********************************************************************/
HttpResponseHeader::HttpResponseHeader()
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	m_responseVersion = _consts.strHttp1_1;
	m_responseCode = SLIB_HTTP_STATUS_200_OK;
	SLIB_STATIC_STRING(s, SLIB_HTTP_MSG_200_OK);
	m_responseMessage = s;
}

#define _RESPONSE_CASE(CODE, MSG) \
	case CODE: \
{ \
	SLIB_STATIC_STRING(s, MSG); \
	m_responseMessage = s; \
	break; \
}

void HttpResponseHeader::setResponseCode(sl_uint32 code)
{
	m_responseCode = code;
	switch (code) {
		_RESPONSE_CASE(SLIB_HTTP_STATUS_100_CONTINUE, SLIB_HTTP_MSG_100_CONTINUE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_101_SWITCHING_PROTOCOLS, SLIB_HTTP_MSG_101_SWITCHING_PROTOCOLS);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_200_OK, SLIB_HTTP_MSG_200_OK);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_201_CREATED, SLIB_HTTP_MSG_201_CREATED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_202_ACCEPTED, SLIB_HTTP_MSG_202_ACCEPTED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_203_NON_AUTH_INFO, SLIB_HTTP_MSG_203_NON_AUTH_INFO);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_204_NO_CONTENT, SLIB_HTTP_MSG_204_NO_CONTENT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_205_RESET_CONTENT, SLIB_HTTP_MSG_205_RESET_CONTENT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_206_PARTIAL_CONTENT, SLIB_HTTP_MSG_206_PARTIAL_CONTENT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_300_MULTIPLE_CHOICES, SLIB_HTTP_MSG_300_MULTIPLE_CHOICES);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_301_MOVED_PERMANENTLY, SLIB_HTTP_MSG_301_MOVED_PERMANENTLY);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_302_FOUND, SLIB_HTTP_MSG_302_FOUND);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_303_SEE_OTHER, SLIB_HTTP_MSG_303_SEE_OTHER);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_304_NOT_MODIFIED, SLIB_HTTP_MSG_304_NOT_MODIFIED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_305_USE_PROXY, SLIB_HTTP_MSG_305_USE_PROXY);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_307_TEMP_REDIRECT, SLIB_HTTP_MSG_307_TEMP_REDIRECT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_400_BAD_REQUEST, SLIB_HTTP_MSG_400_BAD_REQUEST);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_401_UNAUTHROIZED, SLIB_HTTP_MSG_401_UNAUTHROIZED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_402_PAYMENT_REQUIRED, SLIB_HTTP_MSG_402_PAYMENT_REQUIRED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_403_FORBIDDEN, SLIB_HTTP_MSG_403_FORBIDDEN);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_404_NOT_FOUND, SLIB_HTTP_MSG_404_NOT_FOUND);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_405_METHOD_NOT_ALLOWED, SLIB_HTTP_MSG_405_METHOD_NOT_ALLOWED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_406_NOT_ACCEPTABLE, SLIB_HTTP_MSG_406_NOT_ACCEPTABLE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_407_PROXY_AUTH_REQUIRED, SLIB_HTTP_MSG_407_PROXY_AUTH_REQUIRED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_408_REQUEST_TIMEOUT, SLIB_HTTP_MSG_408_REQUEST_TIMEOUT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_409_CONFLICT, SLIB_HTTP_MSG_409_CONFLICT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_410_GONE, SLIB_HTTP_MSG_410_GONE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_411_LENGTH_REQUIRED, SLIB_HTTP_MSG_411_LENGTH_REQUIRED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_412_PRECONDITION_FAILED, SLIB_HTTP_MSG_412_PRECONDITION_FAILED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_413_REQUEST_ENTITY_LARGE, SLIB_HTTP_MSG_413_REQUEST_ENTITY_LARGE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_414_REQUEST_URI_LARGE, SLIB_HTTP_MSG_414_REQUEST_URI_LARGE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE, SLIB_HTTP_MSG_415_UNSUPPORTED_MEDIA_TYPE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_416_REQUESTED_RANGE_NOT_SATISFIABLE, SLIB_HTTP_MSG_416_REQUESTED_RANGE_NOT_SATISFIABLE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_417_EXPECTATION_FAILED, SLIB_HTTP_MSG_417_EXPECTATION_FAILED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_500_INTERNAL_SERVER_ERROR, SLIB_HTTP_MSG_500_INTERNAL_SERVER_ERROR);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_501_NOT_IMPLEMENTED, SLIB_HTTP_MSG_501_NOT_IMPLEMENTED);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_502_BAD_GATEWAY, SLIB_HTTP_MSG_502_BAD_GATEWAY);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_503_SERVICE_UNAVAILABLE, SLIB_HTTP_MSG_503_SERVICE_UNAVAILABLE);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_504_GATEWAY_TIMEOUT, SLIB_HTTP_MSG_504_GATEWAY_TIMEOUT);
		_RESPONSE_CASE(SLIB_HTTP_STATUS_505_HTTP_VERSION_NOT_SUPPORTED, SLIB_HTTP_MSG_505_HTTP_VERSION_NOT_SUPPORTED);
	}
}

sl_uint64 HttpResponseHeader::getResponseContentLengthHeader() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	String headerContentLength = getResponseHeader(_consts.strContentLength);
	if (headerContentLength.isNotEmpty()) {
		return headerContentLength.parseUint64();
	}
	return 0;
}

void HttpResponseHeader::setResponseContentLengthHeader(sl_int64 size)
{
	if (size < 0) {
		size = 0;
	}
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setResponseHeader(_consts.strContentLength, String::fromUint64(size));
}

String HttpResponseHeader::getResponseHeader(String name) const
{
	return m_responseHeaders.getValue(name, String::null());
}

List<String> HttpResponseHeader::getResponseHeaderValues(String name) const
{
	return m_responseHeaders.getValues(name);
}

void HttpResponseHeader::setResponseHeader(String name, String value)
{
	m_responseHeaders.put(name, value);
}

void HttpResponseHeader::addResponseHeader(String name, String value)
{
	m_responseHeaders.add(name, value);
}

sl_bool HttpResponseHeader::containsResponseHeader(String name) const
{
	return m_responseHeaders.containsKey(name);
}

void HttpResponseHeader::removeResponseHeader(String name)
{
	m_responseHeaders.removeAllMatchingKeys(name);
}

void HttpResponseHeader::clearResponseHeaders()
{
	m_responseHeaders.removeAll();
}

Memory HttpResponseHeader::makeResponsePacket() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	String8 str;
	MemoryBuffer msg;
	str = m_responseVersion;
	msg.add(str.toMemory());
	msg.add(_consts.memSp);
	msg.add(String8::fromUint32(m_responseCode).toMemory());
	msg.add(_consts.memSp);
	str = m_responseMessage;
	msg.add(str.toMemory());
	msg.add(_consts.memCr);

	Iterator< Pair<String, String> > iterator = m_responseHeaders.iterator();
	Pair<String, String> pair;
	while (iterator.next(&pair)) {
		str = pair.key;
		msg.add(str.toMemory());
		msg.add(_consts.memColon);
		str = pair.value;
		msg.add(str.toMemory());
		msg.add(_consts.memCr);
	}
	msg.add(_consts.memCr);
	return msg.merge();
}

sl_reg HttpResponseHeader::parseResponsePacket(const void* packet, sl_size _size)
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
	if (!(strResponseCode.parseUint32(&m_responseCode))) {
		return -1;
	}
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

String HttpResponseHeader::getResponseContentType() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getResponseHeader(_consts.strContentType);
}

void HttpResponseHeader::setResponseContentType(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setResponseHeader(_consts.strContentType, type);
}

String HttpResponseHeader::getResponseContentEncoding() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getResponseHeader(_consts.strContentEncoding);
}

void HttpResponseHeader::setResponseContentEncoding(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setResponseHeader(_consts.strContentEncoding, type);
}

String HttpResponseHeader::getResponseTransferEncoding() const
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	return getResponseHeader(_consts.strTransferEncoding);
}

void HttpResponseHeader::setResponseTransferEncoding(const String& type)
{
	_Http_StaticConsts& _consts = _Http_StaticConsts_get();
	setResponseHeader(_consts.strTransferEncoding, type);
}

sl_bool HttpResponseHeader::isChunkedResponse() const
{
	String te = getResponseTransferEncoding();
	if (te.endsWith("chunked")) {
		return sl_true;
	}
	return sl_false;
}

/***********************************************************************
	HttpOutputBuffer
***********************************************************************/
HttpOutputBuffer::HttpOutputBuffer()
{
}

void HttpOutputBuffer::write(const String& str)
{
	write(str.getBuf(), str.getLength());
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
		m_buffer.add(buf, posBody);
		m_last[0] = 0;
		m_last[1] = 0;
		m_last[2] = 0;
	} else {
		m_buffer.add(buf, size);
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

Ref<HttpContentReader> HttpContentReader::createPersistent(
	const Ref<AsyncStream>& io
	, const Ptr<IHttpContentReaderListener>& listener
	, sl_uint64 contentLength
	, sl_uint32 bufferSize
	, sl_bool flagDecompress)
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
		ret->setReadingStream(io);
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

Ref<HttpContentReader> HttpContentReader::createChunked(
	const Ref<AsyncStream>& io
	, const Ptr<IHttpContentReaderListener>& listener
	, sl_uint32 bufferSize
	, sl_bool flagDecompress)
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
		ret->setReadingStream(io);
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

Ref<HttpContentReader> HttpContentReader::createTearDown(
	const Ref<AsyncStream>& io
	, const Ptr<IHttpContentReaderListener>& listener
	, sl_uint32 bufferSize
	, sl_bool flagDecompress)
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
		ret->setReadingStream(io);
		if (flagDecompress) {
			if (!(ret->setDecompressing())) {
				ret.setNull();
			}
		}
	}
	return ret;
}



SLIB_NETWORK_NAMESPACE_END
