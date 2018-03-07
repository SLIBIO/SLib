/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	
	class SLIB_EXPORT IUrlRequestListener
	{
	public:
		IUrlRequestListener();

		virtual ~IUrlRequestListener();

	public:
		virtual void onComplete(UrlRequest* request);
		
		virtual void onResponse(UrlRequest* request, HttpStatus status);
		
		virtual void onReceiveContent(UrlRequest* request, const void* data, sl_size len);
		
		virtual void onDownloadContent(UrlRequest* request, sl_uint64 len);
		
		virtual void onUploadBody(UrlRequest* request, sl_uint64 len);
		
	};
	
	class SLIB_EXPORT UrlRequestParam
	{
	public:
		String url;
		HttpMethod method;
		HashMap<String, Variant> parameters;
		HttpHeaderMap requestHeaders;
		HttpHeaderMap additionalRequestHeaders;
		Memory requestBody;
		String downloadFilePath;
		
		Ptr<IUrlRequestListener> listener;
		Function<void(UrlRequest*)> onComplete;
		Function<void(UrlRequest*, const void*, sl_size)> onReceiveContent;
		Ref<Dispatcher> dispatcher;
		
		sl_bool flagUseBackgroundSession;
		sl_bool flagSelfAlive;
		sl_bool flagStoreResponseContent;
		sl_bool flagSynchronous;
		
	public:
		UrlRequestParam();
		
		UrlRequestParam(const UrlRequestParam& param);
		
		~UrlRequestParam();
		
	public:
		void setRequestBodyAsString(const String& str);
		
		void setRequestBodyAsJson(const Json& json);
		
		void setRequestBodyAsXml(const Ref<XmlDocument>& xml);
		
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
		
		static Ref<UrlRequest> send(const String& url, const HashMap<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete);

		static Ref<UrlRequest> send(const String& url, const HashMap<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> post(const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> post(const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> postJson(const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> postJson(const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> sendSynchronous(const String& url);
		
		static Ref<UrlRequest> sendSynchronous(const String& url, const HashMap<String, Variant>& params);
		
		static Ref<UrlRequest> sendSynchronous(HttpMethod method, const String& url);
		
		static Ref<UrlRequest> sendSynchronous(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Variant& body);
		
		static Ref<UrlRequest> sendJsonSynchronous(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Json& json);
		
		static Ref<UrlRequest> postSynchronous(const String& url, const Variant& body);
		
		static Ref<UrlRequest> postSynchronous(const String& url, const HashMap<String, Variant>& params, const Variant& body);
		
		static Ref<UrlRequest> postJsonSynchronous(const String& url, const Json& json);
		
		static Ref<UrlRequest> postJsonSynchronous(const String& url, const HashMap<String, Variant>& params, const Json& json);
		
	public:
		const String& getUrl();
		
		const String& getDownloadingFilePath();
		
		
		HttpMethod getMethod();
		
		const Memory& getRequestBody();
		
		sl_size getRequestBodySize();
		
		HashMap<String, Variant> getParameters();
		
		HttpHeaderMap getRequestHeaders();
		
		HttpHeaderMap getAdditionalRequestHeaders();
		
		
		Memory getResponseContent();
		
		String getResponseContentAsString();
		
		Json getResponseContentAsJson();
		
		Ref<XmlDocument> getResponseContentAsXml();
		
		
		HttpStatus getResponseStatus();
		
		String getResponseMessage();
		
		HttpHeaderMap getResponseHeaders();
		
		String getResponseHeader(const String& name);
		
		
		const Ptr<IUrlRequestListener>& getListener();
		
		const Function<void(UrlRequest*)>& getOnComplete();
		
		const Function<void(UrlRequest*, const void*, sl_size)>& getOnReceiveContent();
		
		const Ref<Dispatcher>& getDispatcher();
		
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
		
		static String _buildParameters(const Map<String, Variant>& params);
		
		static String _buildParameters(const HashMap<String, Variant>& params);
		
		static Memory _buildRequestBody(const Variant& varBody);
		
	protected:
		String m_url;
		String m_downloadFilePath;
		
		HttpMethod m_method;
		Memory m_requestBody;
		HashMap<String, Variant> m_parameters;
		HttpHeaderMap m_requestHeaders;
		HttpHeaderMap m_additionalRequestHeaders;
		
		MemoryQueue m_bufResponseContent;
		HttpStatus m_responseStatus;
		AtomicString m_responseMessage;
		Atomic<HttpHeaderMap> m_responseHeaders;
		
		Ptr<IUrlRequestListener> m_listener;
		Function<void(UrlRequest*)> m_onComplete;
		Function<void(UrlRequest*, const void*, sl_size)> m_onReceiveContent;
		Ref<Dispatcher> m_dispatcher;
		sl_bool m_flagUseBackgroundSession;
		sl_bool m_flagSelfAlive;
		sl_bool m_flagStoreResponseContent;
		
		sl_uint64 m_sizeBodySent;
		sl_uint64 m_sizeContentTotal;
		sl_uint64 m_sizeContentReceived;
		
		sl_bool m_flagClosed;
		sl_bool m_flagError;
		AtomicString m_lastErrorMessage;
		
		Ref<Event> m_eventSync;
		
	};

}

#endif
