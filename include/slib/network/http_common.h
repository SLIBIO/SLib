/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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

#include "../core/string.h"
#include "../core/content_type.h"
#include "../core/map.h"

namespace slib
{

	enum class HttpStatus
	{
		Unknown = 0,
		
		// Informational
		Continue = 100,
		SwitchingProtocols = 101,
		
		// Success
		OK = 200,
		Created = 201,
		Accepted = 202,
		NonAuthInfo = 203,
		NoContent = 204,
		ResetContent = 205,
		PartialContent = 206,
		
		// Redirection
		MultipleChoices = 300,
		MovedPermanently = 301,
		Found = 302,
		SeeOther = 303,
		NotModified = 304,
		UseProxy = 305,
		TemporaryRedirect = 307,
		
		// Client Error
		BadRequest = 400,
		Unauthorized = 401,
		PaymentRequired = 402,
		Forbidden = 403,
		NotFound = 404,
		MethodNotAllowed = 405,
		NotAcceptable = 406,
		ProxyAuthenticationRequired = 407,
		RequestTimeout = 408,
		Conflict = 409,
		Gone = 410,
		LengthRequired = 411,
		PreconditionFailed = 412,
		RequestEntityTooLarge = 413,
		RequestUriTooLarge = 414,
		UnsupportedMediaType = 415,
		RequestRangeNotSatisfiable = 416,
		ExpectationFailed = 417,
		
		// Server Error
		InternalServerError = 500,
		NotImplemented = 501,
		BadGateway = 502,
		ServiceUnavailable = 503,
		GatewayTimeout = 504,
		HttpVersionNotSupported = 505
		
	};
	
	class SLIB_EXPORT HttpStatuses
	{
	public:
		static String toString(HttpStatus status);
		
	};
	
	
#ifdef DELETE
#undef DELETE
#endif
	
	enum class HttpMethod
	{
		Unknown = 0,
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE
	};
	
	class SLIB_EXPORT HttpMethods
	{
	public:
		static String toString(HttpMethod method);
		
		static HttpMethod fromString(const String& method);
		
	};
	
	
	class SLIB_EXPORT HttpHeaders
	{
	public:
		static const String& ContentLength;
		static const String& ContentType;
		static const String& Host;
		static const String& AcceptEncoding;
		static const String& TransferEncoding;
		static const String& ContentEncoding;
		
		static const String& Range;
		static const String& ContentRange;
		static const String& AcceptRanges;
		
		static const String& Origin;
		static const String& AccessControlAllowOrigin;
		
	public:
		
		/*
		 Returns
		 <0: error
		 =0: incomplete packet
		 >0: size of the headers (ending with [CR][LF][CR][LF])
		 */
		static sl_reg parseHeaders(Map<String, String>& outMap, const void* headers, sl_size size);
		
	};
	
	
	class SLIB_EXPORT HttpRequest
	{
	public:
		HttpRequest();
		
		~HttpRequest();
		
	public:
		HttpMethod getMethod() const;
		
		String getMethodText() const;
		
		String getMethodTextUppercase() const;
		
		void setMethod(HttpMethod method);
		
		void setMethod(const String& method);
		
		String getPath() const;
		
		void setPath(const String& path);
		
		String getQuery() const;
		
		void setQuery(String query);
		
		String getRequestVersion() const;
		
		void setRequestVersion(const String& version);
		
		
		const Map<String, String>& getRequestHeaders() const;
		
		String getRequestHeader(String name) const;
		
		List<String> getRequestHeaderValues(String name) const;
		
		void setRequestHeader(String name, String value);
		
		void addRequestHeader(String name, String value);
		
		sl_bool containsRequestHeader(String name) const;
		
		void removeRequestHeader(String name);
		
		void clearRequestHeaders();
		
		
		sl_uint64 getRequestContentLengthHeader() const;
		
		void setRequestContentLengthHeader(sl_uint64 size);
		
		String getRequestContentType() const;
		
		String getRequestContentTypeNoParams() const;
		
		void setRequestContentType(const String& type);
		
		void setRequestContentType(ContentType type);
		
		String getRequestContentEncoding() const;
		
		void setRequestContentEncoding(const String& type);
		
		String getRequestTransferEncoding() const;
		
		void setRequestTransferEncoding(const String& type);
		
		sl_bool isChunkedRequest() const;
		
		String getHost() const;
		
		void setHost(const String& type);
		
		String getRequestRange() const;
		
		void setRequestRange(const String& range);
		
		void setRequestRange(sl_uint64 start, sl_uint64 last);
		
		void setRequestRangeFrom(sl_uint64 start);
		
		void setRequestRangeSuffix(sl_uint64 length);
		
		String getRequestOrigin() const;
		
		void setRequestOrigin(const String& origin);
		
		
		const Map<String, String>& getParameters() const;
		
		String getParameter(const String& name) const;
		
		List<String> getParameterValues(const String& name) const;
		
		sl_bool containsParameter(const String& name) const;
		
		const Map<String, String>& getQueryParameters() const;
		
		String getQueryParameter(String name) const;
		
		List<String> getQueryParameterValues(String name) const;
		
		sl_bool containsQueryParameter(String name) const;
		
		const Map<String, String>& getPostParameters() const;
		
		String getPostParameter(String name) const;
		
		List<String> getPostParameterValues(String name) const;
		
		sl_bool containsPostParameter(String name) const;
		
		void applyPostParameters(const void* data, sl_size size);
		
		void applyPostParameters(const String& str);
		
		void applyQueryToParameters();
		
		static Map<String, String> parseParameters(const void* data, sl_size size);
		
		static Map<String, String> parseParameters(const String& str);
		
		Memory makeRequestPacket() const;
		
		/*
		 Returns
		 <0: error
		 =0: incomplete packet
		 >0: size of the HTTP header section (ending with [CR][LF][CR][LF])
		 */
		sl_reg parseRequestPacket(const void* packet, sl_size size);
		
	protected:
		HttpMethod m_method;
		String m_methodText;
		String m_methodTextUpper;
		String m_path;
		String m_query;
		String m_requestVersion;
		
		Map<String, String> m_requestHeaders;
		Map<String, String> m_parameters;
		Map<String, String> m_queryParameters;
		Map<String, String> m_postParameters;
		
	};
	
	class SLIB_EXPORT HttpResponse
	{
	public:
		HttpResponse();
		
		~HttpResponse();
		
	public:
		HttpStatus getResponseCode() const;
		
		void setResponseCode(HttpStatus code);
		
		String getResponseMessage() const;
		
		void setResponseMessage(String message);
		
		String getResponseVersion() const;
		
		void setResponseVersion(const String& version);
		
		
		const Map<String, String>& getResponseHeaders() const;
		
		String getResponseHeader(String name) const;
		
		List<String> getResponseHeaderValues(String name) const;
		
		void setResponseHeader(String name, String value);
		
		void addResponseHeader(String name, String value);
		
		sl_bool containsResponseHeader(String name) const;
		
		void removeResponseHeader(String name);
		
		void clearResponseHeaders();
		
		
		sl_uint64 getResponseContentLengthHeader() const;
		
		void setResponseContentLengthHeader(sl_uint64 size);
		
		String getResponseContentType() const;
		
		void setResponseContentType(const String& type);
		
		void setResponseContentType(ContentType type);
		
		String getResponseContentEncoding() const;
		
		void setResponseContentEncoding(const String& type);
		
		String getResponseTransferEncoding() const;
		
		void setResponseTransferEncoding(const String& type);
		
		sl_bool isChunkedResponse() const;
		
		String getResponseContentRange() const;
		
		void setResponseContentRange(const String& range);
		
		void setResponseContentRange(sl_uint64 start, sl_uint64 last, sl_uint64 total);
		
		void setResponseContentRangeUnknownTotal(sl_uint64 start, sl_uint64 last);
		
		void setResponseContentRangeUnsatisfied(sl_uint64 total);
		
		String getResponseAcceptRanges() const;
		
		void setResponseAcceptRanges(sl_bool flagAcceptRanges);
		
		void setResponseAcceptRangesIfNotDefined(sl_bool flagAcceptRanges);
		
		String getResponseAccessControlAllowOrigin() const;
		
		void setResponseAccessControlAllowOrigin(const String& origin);
		
		
		Memory makeResponsePacket() const;
		
		/*
		 Returns
		 <0: error
		 =0: incomplete packet
		 >0: size of the HTTP header section (ending with [CR][LF][CR][LF])
		 */
		sl_reg parseResponsePacket(const void* packet, sl_size size);
		
	protected:
		HttpStatus m_responseCode;
		String m_responseMessage;
		String m_responseVersion;
		
		Map<String, String> m_responseHeaders;
		
	};
	
}

#endif

