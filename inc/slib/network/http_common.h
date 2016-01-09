#ifndef CHECKHEADER_SLIB_NETWORK_HTTP_COMMON
#define CHECKHEADER_SLIB_NETWORK_HTTP_COMMON

#include "definition.h"

/****************************************************************************
	
				Hypertext Transfer Protocol -- HTTP/1.1

	http://ietf.org/rfc/rfc2616.txt

	and replaced by

	http://tools.ietf.org/html/rfc7230 (Message Syntax and Routing)
	http://tools.ietf.org/html/rfc7231 (Semantics and Content)
	http://tools.ietf.org/html/rfc7232 (Conditional Requests)
	http://tools.ietf.org/html/rfc7233 (Range Requests)
	http://tools.ietf.org/html/rfc7234 (Caching)
	http://tools.ietf.org/html/rfc7235 (Authentication)


				Hypertext Transfer Protocol Version 2 (HTTP/2)

	https://tools.ietf.org/html/rfc7540 (HTTP/2)
	https://tools.ietf.org/html/rfc7541 (HPACK: Header Compression for HTTP/2)

*****************************************************************************/

// Informational
#define SLIB_HTTP_STATUS_100_CONTINUE							100
#define SLIB_HTTP_MSG_100_CONTINUE								"Continue"
#define SLIB_HTTP_STATUS_101_SWITCHING_PROTOCOLS				101
#define SLIB_HTTP_MSG_101_SWITCHING_PROTOCOLS					"Switching Protocols"
// Success 
#define SLIB_HTTP_STATUS_200_OK									200
#define SLIB_HTTP_MSG_200_OK									"OK"
#define SLIB_HTTP_STATUS_201_CREATED							201
#define SLIB_HTTP_MSG_201_CREATED								"Created"
#define SLIB_HTTP_STATUS_202_ACCEPTED							202
#define SLIB_HTTP_MSG_202_ACCEPTED								"Accepted"
#define SLIB_HTTP_STATUS_203_NON_AUTH_INFO						203
#define SLIB_HTTP_MSG_203_NON_AUTH_INFO							"Non-Authoritative Information"
#define SLIB_HTTP_STATUS_204_NO_CONTENT							204
#define SLIB_HTTP_MSG_204_NO_CONTENT							"No Content"
#define SLIB_HTTP_STATUS_205_RESET_CONTENT						205
#define SLIB_HTTP_MSG_205_RESET_CONTENT							"Reset Content"
#define SLIB_HTTP_STATUS_206_PARTIAL_CONTENT					206
#define SLIB_HTTP_MSG_206_PARTIAL_CONTENT						"Partial Content"
// Redirection
#define SLIB_HTTP_STATUS_300_MULTIPLE_CHOICES					300
#define SLIB_HTTP_MSG_300_MULTIPLE_CHOICES						"Multiple Choices"
#define SLIB_HTTP_STATUS_301_MOVED_PERMANENTLY					301
#define SLIB_HTTP_MSG_301_MOVED_PERMANENTLY						"Moved Permanently"
#define SLIB_HTTP_STATUS_302_FOUND								302
#define SLIB_HTTP_MSG_302_FOUND									"Found"
#define SLIB_HTTP_STATUS_303_SEE_OTHER							303
#define SLIB_HTTP_MSG_303_SEE_OTHER								"See Other"
#define SLIB_HTTP_STATUS_304_NOT_MODIFIED						304
#define SLIB_HTTP_MSG_304_NOT_MODIFIED							"Not Modified"
#define SLIB_HTTP_STATUS_305_USE_PROXY							305
#define SLIB_HTTP_MSG_305_USE_PROXY								"Use Proxy"
#define SLIB_HTTP_STATUS_307_TEMP_REDIRECT						307
#define SLIB_HTTP_MSG_307_TEMP_REDIRECT							"Temporary Redirect"
// Client Error
#define SLIB_HTTP_STATUS_400_BAD_REQUEST						400
#define SLIB_HTTP_MSG_400_BAD_REQUEST							"Bad Request"
#define SLIB_HTTP_STATUS_401_UNAUTHROIZED						401
#define SLIB_HTTP_MSG_401_UNAUTHROIZED							"Unauthorized"
#define SLIB_HTTP_STATUS_402_PAYMENT_REQUIRED					402
#define SLIB_HTTP_MSG_402_PAYMENT_REQUIRED						"Payment Required"
#define SLIB_HTTP_STATUS_403_FORBIDDEN							403
#define SLIB_HTTP_MSG_403_FORBIDDEN								"Forbidden"
#define SLIB_HTTP_STATUS_404_NOT_FOUND							404
#define SLIB_HTTP_MSG_404_NOT_FOUND								"Not Found"
#define SLIB_HTTP_STATUS_405_METHOD_NOT_ALLOWED					405
#define SLIB_HTTP_MSG_405_METHOD_NOT_ALLOWED					"Method Not Allowed"
#define SLIB_HTTP_STATUS_406_NOT_ACCEPTABLE						406
#define SLIB_HTTP_MSG_406_NOT_ACCEPTABLE						"Not Acceptable"
#define SLIB_HTTP_STATUS_407_PROXY_AUTH_REQUIRED				407
#define SLIB_HTTP_MSG_407_PROXY_AUTH_REQUIRED					"Proxy Authentication Required"
#define SLIB_HTTP_STATUS_408_REQUEST_TIMEOUT					408
#define SLIB_HTTP_MSG_408_REQUEST_TIMEOUT						"Request Time-out"
#define SLIB_HTTP_STATUS_409_CONFLICT							409
#define SLIB_HTTP_MSG_409_CONFLICT								"Conflict"
#define SLIB_HTTP_STATUS_410_GONE								410
#define SLIB_HTTP_MSG_410_GONE									"Gone"
#define SLIB_HTTP_STATUS_411_LENGTH_REQUIRED					411
#define SLIB_HTTP_MSG_411_LENGTH_REQUIRED						"Length Required"
#define SLIB_HTTP_STATUS_412_PRECONDITION_FAILED				412
#define SLIB_HTTP_MSG_412_PRECONDITION_FAILED					"Precondition Failed"
#define SLIB_HTTP_STATUS_413_REQUEST_ENTITY_LARGE				413
#define SLIB_HTTP_MSG_413_REQUEST_ENTITY_LARGE					"Request Entity Too Large"
#define SLIB_HTTP_STATUS_414_REQUEST_URI_LARGE					414
#define SLIB_HTTP_MSG_414_REQUEST_URI_LARGE						"Request-URI Too Large"
#define SLIB_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE				415
#define SLIB_HTTP_MSG_415_UNSUPPORTED_MEDIA_TYPE				"Unsupported Media Type"
#define SLIB_HTTP_STATUS_416_REQUESTED_RANGE_NOT_SATISFIABLE	416
#define SLIB_HTTP_MSG_416_REQUESTED_RANGE_NOT_SATISFIABLE		"Requested range not satisfiable"
#define SLIB_HTTP_STATUS_417_EXPECTATION_FAILED					417
#define SLIB_HTTP_MSG_417_EXPECTATION_FAILED					"Expectation Failed"
// Server Error
#define SLIB_HTTP_STATUS_500_INTERNAL_SERVER_ERROR				500
#define SLIB_HTTP_MSG_500_INTERNAL_SERVER_ERROR					"Internal Server Error"
#define SLIB_HTTP_STATUS_501_NOT_IMPLEMENTED					501
#define SLIB_HTTP_MSG_501_NOT_IMPLEMENTED						"Not Implemented"
#define SLIB_HTTP_STATUS_502_BAD_GATEWAY						502
#define SLIB_HTTP_MSG_502_BAD_GATEWAY							"Bad Gateway"
#define SLIB_HTTP_STATUS_503_SERVICE_UNAVAILABLE				503
#define SLIB_HTTP_MSG_503_SERVICE_UNAVAILABLE					"Service Unavailable"
#define SLIB_HTTP_STATUS_504_GATEWAY_TIMEOUT					504
#define SLIB_HTTP_MSG_504_GATEWAY_TIMEOUT						"Gateway Time-out"
#define SLIB_HTTP_STATUS_505_HTTP_VERSION_NOT_SUPPORTED			505
#define SLIB_HTTP_MSG_505_HTTP_VERSION_NOT_SUPPORTED			"HTTP Version not supported"

#define SLIB_HTTP_METHOD_GET			"GET"
#define SLIB_HTTP_METHOD_HEAD			"HEAD"
#define SLIB_HTTP_METHOD_POST			"POST"
#define SLIB_HTTP_METHOD_PUT			"PUT"
#define SLIB_HTTP_METHOD_DELETE			"DELETE"
#define SLIB_HTTP_METHOD_CONNECT		"CONNECT"
#define SLIB_HTTP_METHOD_OPTIONS		"OPTIONS"
#define SLIB_HTTP_METHOD_TRACE			"TRACE"

#define SLIB_HTTP_CONTENT_TYPE_FORM					"application/x-www-form-urlencoded"
#define SLIB_HTTP_CONTENT_TYPE_TEXT_HTML_UTF8		"text/html; charset=UTF-8"

#define SLIB_HTTP_HEADER_CONTENT_LENGTH			"Content-Length"
#define SLIB_HTTP_HEADER_CONTENT_TYPE			"Content-Type"
#define SLIB_HTTP_HEADER_HOST					"Host"
#define SLIB_HTTP_HEADER_ACCEPT_ENCODING		"Accept-Encoding"
#define SLIB_HTTP_HEADER_TRANSFER_ENCODING		"Transfer-Encoding"
#define SLIB_HTTP_HEADER_CONTENT_ENCODING		"Content-Encoding"

#include "../core/string.h"
#include "../core/map.h"
#include "../core/queue.h"
#include "../crypto/zlib.h"

#include "async.h"

SLIB_NETWORK_NAMESPACE_BEGIN
class SLIB_EXPORT HttpRequestHeader
{
public:
	HttpRequestHeader();

public:
	SLIB_INLINE const String& getMethod() const
	{
		return m_method;
	}
	SLIB_INLINE const String& getMethodUppercase() const
	{
		return m_methodUpper;
	}
	void setMethod(const String& method);

	SLIB_INLINE const String& getPath() const
	{
		return m_path;
	}
	SLIB_INLINE void setPath(const String& path)
	{
		m_path = path;
	}

	SLIB_INLINE const String& getQuery() const
	{
		return m_query;
	}
	SLIB_INLINE void setQuery(String query)
	{
		m_query = query;
	}

	SLIB_INLINE const String& getRequestVersion() const
	{
		return m_requestVersion;
	}
	SLIB_INLINE void setRequestVersion(const String& version)
	{
		m_requestVersion = version;
	}

protected:
	String m_method;
	String m_methodUpper;

	String m_path;
	String m_query;
	
	String m_requestVersion;

public:
	SLIB_INLINE const Map<String, String>& getRequestHeaders() const
	{
		return m_requestHeaders;
	}
	SLIB_INLINE String getRequestHeader(String name) const
	{
		return m_requestHeaders.getValue(name, String::null());
	}
	SLIB_INLINE void setRequestHeader(String name, String value)
	{
		m_requestHeaders.put(name, value);
	}
	SLIB_INLINE void addRequestHeader(String name, String value)
	{
		m_requestHeaders.add(name, value);
	}
	SLIB_INLINE sl_bool containsRequestHeader(String name) const
	{
		return m_requestHeaders.containsKey(name);
	}
	SLIB_INLINE void removeRequestHeader(String name)
	{
		m_requestHeaders.removeAllMatchingKeys(name);
	}
	SLIB_INLINE void clearRequestHeaders()
	{
		m_requestHeaders.removeAll();
	}

	sl_uint64 getRequestContentLengthHeader() const;
	void setRequestContentLengthHeader(sl_int64 size);

	String getRequestContentType() const;
	void setRequestContentType(const String& type);

	String getRequestContentEncoding() const;
	void setRequestContentEncoding(const String& type);

	String getRequestTransferEncoding() const;
	void setRequestTransferEncoding(const String& type);

	sl_bool isChunkedRequest() const;

	String getHost() const;
	void setHost(const String& type);

protected:
	Map<String, String> m_requestHeaders;

public:
	SLIB_INLINE const Map<String, String>& getParameters() const
	{
		return m_parameters;
	}
	SLIB_INLINE String getParameter(const String& name) const
	{
		return m_parameters.getValue(name, String::null());
	}
	SLIB_INLINE sl_bool containsParameter(const String& name) const
	{
		return m_parameters.containsKey(name);
	}

	SLIB_INLINE const Map<String, String>& getQueryParameters() const
	{
		return m_queryParameters;
	}
	SLIB_INLINE String getQueryParameter(String name) const
	{
		return m_queryParameters.getValue(name, String::null());
	}
	SLIB_INLINE sl_bool containsQueryParameter(String name) const
	{
		return m_queryParameters.containsKey(name);
	}

	SLIB_INLINE const Map<String, String>& getPostParameters() const
	{
		return m_postParameters;
	}
	SLIB_INLINE String getPostParameter(String name) const
	{
		return m_postParameters.getValue(name, String::null());
	}
	SLIB_INLINE sl_bool containsPostParameter(String name) const
	{
		return m_postParameters.containsKey(name);
	}

protected:
	Map<String, String> m_parameters;
	Map<String, String> m_queryParameters;
	Map<String, String> m_postParameters;

public:
	void applyPostParameters(const void* data, sl_size size);
	void applyPostParameters(const String& str);
	void applyQueryToParameters();

public:
	static Map<String, String> parseParameters(const void* data, sl_size size);
	static Map<String, String> parseParameters(const String& str);

public:
	Memory makeRequestPacket() const;

	/*
	Returns
		<0: error
		=0: incomplete packet
		>0: size of the HTTP header section (ending with [CR][LF][CR][LF])
	*/
	sl_reg parseRequestPacket(const void* packet, sl_size size);
};

class SLIB_EXPORT HttpResponseHeader
{
public:
	HttpResponseHeader();

public:
	SLIB_INLINE sl_uint32 getResponseCode() const
	{
		return m_responseCode;
	}
	void setResponseCode(sl_uint32 code);

	SLIB_INLINE const String& getResponseMessage() const
	{
		return m_responseMessage;
	}
	SLIB_INLINE void setResponseMessage(String message)
	{
		m_responseMessage = message;
	}

	SLIB_INLINE const String& getResponseVersion() const
	{
		return m_responseVersion;
	}
	SLIB_INLINE void setResponseVersion(const String& version)
	{
		m_responseVersion = version;
	}

protected:
	sl_uint32 m_responseCode;
	String m_responseMessage;
	String m_responseVersion;

public:
	SLIB_INLINE const Map<String, String>& getResponseHeaders() const
	{
		return m_responseHeaders;
	}
	SLIB_INLINE String getResponseHeader(String name) const
	{
		return m_responseHeaders.getValue(name, String::null());
	}
	SLIB_INLINE void setResponseHeader(String name, String value)
	{
		m_responseHeaders.put(name, value);
	}
	SLIB_INLINE void addResponseHeader(String name, String value)
	{
		m_responseHeaders.add(name, value);
	}
	SLIB_INLINE sl_bool containsResponseHeader(String name) const
	{
		return m_responseHeaders.containsKey(name);
	}
	SLIB_INLINE void removeResponseHeader(String name)
	{
		m_responseHeaders.removeAllMatchingKeys(name);
	}
	SLIB_INLINE void clearResponseHeaders()
	{
		m_responseHeaders.removeAll();
	}

	sl_uint64 getResponseContentLengthHeader() const;
	void setResponseContentLengthHeader(sl_int64 size);

	String getResponseContentType() const;
	void setResponseContentType(const String& size);

	String getResponseContentEncoding() const;
	void setResponseContentEncoding(const String& type);

	String getResponseTransferEncoding() const;
	void setResponseTransferEncoding(const String& type);

	sl_bool isChunkedResponse() const;

protected:
	Map<String, String> m_responseHeaders;

public:
	Memory makeResponsePacket() const;

	/*
	Returns
		<0: error
		=0: incomplete packet
		>0: size of the HTTP header section (ending with [CR][LF][CR][LF])
	*/
	sl_reg parseResponsePacket(const void* packet, sl_size size);
};

class SLIB_EXPORT HttpOutputBuffer
{
public:
	HttpOutputBuffer();

public:
	SLIB_INLINE void clearOutput()
	{
		m_bufferOutput.clearOutput();
	}

	SLIB_INLINE void write(const void* buf, sl_size size)
	{
		m_bufferOutput.write(buf, size);
	}
	
	void write(const String& str);

	SLIB_INLINE void write(const Memory& mem)
	{
		m_bufferOutput.write(mem);
	}
	
	SLIB_INLINE void copyFrom(AsyncStream* stream, sl_uint64 size)
	{
		m_bufferOutput.copyFrom(stream, size);
	}

	SLIB_INLINE void copyFromFile(const String& path)
	{
		m_bufferOutput.copyFromFile(path);
	}

	SLIB_INLINE sl_uint64 getOutputLength() const
	{
		return m_bufferOutput.getOutputLength();
	}

protected:
	AsyncOutputBuffer m_bufferOutput;
};

class SLIB_EXPORT HttpHeaderReader
{
public:
	HttpHeaderReader();

public:
	// return sl_true when body section (\r\n\r\n) is detected
	sl_bool add(const void* buf, sl_size size, sl_size& posBody);

	Memory mergeHeader();
	sl_size getHeaderSize();
	void clear();

protected:
	sl_char16 m_last[3];
	MemoryBuffer m_buffer;
};

class SLIB_EXPORT IHttpContentReaderListener
{
public:
	virtual void onCompleteReadHttpContent(void* dataRemained, sl_uint32 sizeRemained, sl_bool flagError) = 0;
};

class SLIB_EXPORT HttpContentReader : public AsyncStreamFilter
{
protected:
	HttpContentReader();
public:
	~HttpContentReader();

public:
	SLIB_PROPERTY_INLINE(Ptr<IHttpContentReaderListener>, Listener);

	SLIB_INLINE sl_bool isDecompressing()
	{
		return m_flagDecompressing;
	}

protected:
	void setError();
	void setCompleted(void* dataRemain, sl_uint32 size);
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);
	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref);

	sl_bool setDecompressing();
	Memory decompressData(void* data, sl_uint32 size, Referable* refData);

protected:
	sl_bool m_flagDecompressing;
	ZlibDecompress m_zlib;

public:
	static Ref<HttpContentReader> createPersistent(
		const Ref<AsyncStream>& io
		, const Ptr<IHttpContentReaderListener>& listener
		, sl_uint64 contentLength
		, sl_uint32 bufferSize
		, sl_bool flagDecompress);

	static Ref<HttpContentReader> createChunked(
		const Ref<AsyncStream>& io
		, const Ptr<IHttpContentReaderListener>& listener
		, sl_uint32 bufferSize
		, sl_bool flagDecompress);

	static Ref<HttpContentReader> createTearDown(
		const Ref<AsyncStream>& io
		, const Ptr<IHttpContentReaderListener>& listener
		, sl_uint32 bufferSize
		, sl_bool flagDecompress);

};
SLIB_NETWORK_NAMESPACE_END

#endif

