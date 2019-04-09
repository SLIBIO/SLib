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

#ifndef CHECKHEADER_SLIB_NETWORK_URL_REQUEST
#define CHECKHEADER_SLIB_NETWORK_URL_REQUEST

#include "definition.h"

#include "http_common.h"

#include "../core/string.h"
#include "../core/function.h"
#include "../core/dispatch.h"
#include "../core/variant.h"
#include "../core/json.h"
#include "../core/xml.h"

namespace slib
{

	class UrlRequest;
	
	class SLIB_EXPORT UrlRequestParam
	{
	public:
		String url;
		HttpMethod method;
		HashMap<String, Variant> parameters;
		HttpHeaderMap requestHeaders;
		Memory requestBody;
		String downloadFilePath;
		
		Function<void(UrlRequest*)> onComplete;
		Function<void(UrlRequest*, HttpStatus)> onResponse;
		Function<void(UrlRequest*, const void* data, sl_size len)> onReceiveContent;
		Function<void(UrlRequest*, sl_uint64 len)> onDownloadContent;
		Function<void(UrlRequest*, sl_uint64 len)> onUploadBody;
		Ref<Dispatcher> dispatcher;
		
		sl_bool flagUseBackgroundSession;
		sl_bool flagSelfAlive;
		sl_bool flagStoreResponseContent;
		sl_bool flagSynchronous;
		
		sl_uint32 timeout; // In milliseconds
		sl_bool flagAllowInsecureConnection;
		
	public:
		UrlRequestParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(UrlRequestParam)
		
	public:
		void setContentType(const ContentType& contentType);
		
		void setContentType(const String& contentType);
		
		
		void setRequestBodyAsMemory(const Memory& mem);
		
		void setRequestBodyAsString(const String& str);
		
		void setRequestBodyAsJson(const Json& json);
		
		void setRequestBodyAsXml(const Ref<XmlDocument>& xml);
		
		template <class KT, class VT, class KEY_COMPARE>
		void setRequestBodyAsMap(const Map<KT, VT, KEY_COMPARE>& map);
		
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		void setRequestBodyAsHashMap(const HashMap<KT, VT, HASH, KEY_COMPARE>& map);
		
		void setRequestBody(const Variant& var);
		

		template <class KT, class VT, class HASH, class KEY_COMPARE>
		void setFormData(const HashMap<KT, VT, HASH, KEY_COMPARE>& map);
		
		void setMultipartFormData(const HashMap<String, Variant>& params);
		
		void setJsonData(const Json& json);
		
	};
	
	class Event;

	class SLIB_EXPORT UrlRequest : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		UrlRequest();
		
		~UrlRequest();
		
	public:
		static Ref<UrlRequest> send(const UrlRequestParam& param);

		static Ref<UrlRequest> send(const String& url, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(const String& url, const HttpHeaderMap& headers, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(const String& url, const HttpHeaderMap& headers, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> post(const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> post(const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> postJson(const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> postJson(const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> sendSynchronous(const String& url);
		
		static Ref<UrlRequest> sendSynchronous(const String& url, const HttpHeaderMap& headers);
		
		static Ref<UrlRequest> sendSynchronous(HttpMethod method, const String& url);
		
		static Ref<UrlRequest> sendSynchronous(HttpMethod method, const String& url, const Variant& body);

		static Ref<UrlRequest> sendSynchronous(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Variant& body);
		
		static Ref<UrlRequest> sendJsonSynchronous(HttpMethod method, const String& url, const Json& json);
		
		static Ref<UrlRequest> sendJsonSynchronous(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Json& json);
		
		static Ref<UrlRequest> postSynchronous(const String& url, const Variant& body);
		
		static Ref<UrlRequest> postSynchronous(const String& url, const HttpHeaderMap& headers, const Variant& body);
		
		static Ref<UrlRequest> postJsonSynchronous(const String& url, const Json& json);
		
		static Ref<UrlRequest> postJsonSynchronous(const String& url, const HttpHeaderMap& headers, const Json& json);
		
		// In milliseconds
		static sl_uint32 getDefaultTimeout();
		
		// In milliseconds
		static void setDefaultTimeout(sl_uint32 ms);
		
		static sl_bool isDefaultAllowInsecureConnection();
		
		static void setDefaultAllowInsecureConnection(sl_bool flag);
		
		static Ref<Dispatcher> getDefaultDispatcher();
		
		static void setDefaultDispatcher(const Ref<Dispatcher>& dispatcher);
		
	public:
		const String& getUrl();
		
		const String& getDownloadingFilePath();
		
		
		HttpMethod getMethod();
		
		const Memory& getRequestBody();
		
		sl_size getRequestBodySize();
		
		HashMap<String, Variant> getParameters();
		
		HttpHeaderMap getRequestHeaders();
		
		
		Memory getResponseContent();
		
		String getResponseContentAsString();
		
		Json getResponseContentAsJson();
		
		Ref<XmlDocument> getResponseContentAsXml();
		
		
		HttpStatus getResponseStatus();
		
		String getResponseMessage();
		
		HttpHeaderMap getResponseHeaders();
		
		String getResponseHeader(const String& name);
		
		
		sl_bool isUsingBackgroundSession();
		
		sl_bool isSelfAlive();
		
		sl_bool isStoringResponseContent();
		
		
		sl_uint64 getSentRequestBodySize();
		
		sl_uint64 getResponseContentSize();
		
		sl_uint64 getExpectedResponseContentSize();
		
		
		void cancel();
		
		sl_bool isError();
		
		String getLastErrorMessage();

		sl_bool isClosed();
		
		
	protected:
		static Ref<UrlRequest> _create(const UrlRequestParam& param, const String& url);
		
		virtual void _sendSync();
		
		void _sendSync_call();
		
		virtual void _sendAsync();
		
		void _init(const UrlRequestParam& param, const String& url);
		
		void _removeFromMap();
		
		virtual void _cancel();
		
		void onComplete();
		
		void onError();
		
		void onResponse();
		
		void onReceiveContent(const void* data, sl_size len, const Memory& mem);
		
		void onDownloadContent(sl_uint64 size);
		
		void onUploadBody(sl_uint64 size);
		
		void _runCallback(const Function<void(UrlRequest*)>& callback);
		
	protected:
		String m_url;
		String m_downloadFilePath;
		
		HttpMethod m_method;
		Memory m_requestBody;
		HashMap<String, Variant> m_parameters;
		HttpHeaderMap m_requestHeaders;
		
		MemoryQueue m_bufResponseContent;
		HttpStatus m_responseStatus;
		AtomicString m_responseMessage;
		Atomic<HttpHeaderMap> m_responseHeaders;
		
		Function<void(UrlRequest*)> m_onComplete;
		Function<void(UrlRequest*, HttpStatus)> m_onResponse;
		Function<void(UrlRequest*, const void* data, sl_size len)> m_onReceiveContent;
		Function<void(UrlRequest*, sl_uint64 len)> m_onDownloadContent;
		Function<void(UrlRequest*, sl_uint64 len)> m_onUploadBody;
		Ref<Dispatcher> m_dispatcher;
		
		sl_bool m_flagUseBackgroundSession;
		sl_bool m_flagSelfAlive;
		sl_bool m_flagStoreResponseContent;
		
		sl_uint32 m_timeout;
		sl_bool m_flagAllowInsecureConnection;
		
		sl_uint64 m_sizeBodySent;
		sl_uint64 m_sizeContentTotal;
		sl_uint64 m_sizeContentReceived;
		
		sl_bool m_flagClosed;
		sl_bool m_flagError;
		AtomicString m_lastErrorMessage;
		
		Ref<Event> m_eventSync;
		
		friend class CurlRequest;
	};

}

#include "detail/url_request.inc"

#endif
