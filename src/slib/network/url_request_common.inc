/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


	Ref<UrlRequest> URL_REQUEST::send(const UrlRequestParam& param)
	{
		String url = param.url;
		if (url.isNotEmpty()) {
			if (param.parameters.isNotEmpty()) {
				if (url.contains('?')) {
					url += "&";
				} else {
					url += "?";
				}
			}
			url += UrlRequest::_buildParameters(param.parameters);
			Ref<UrlRequest> request = _create(param, url);
			if (request.isNotNull()) {
				if (param.flagSynchronous) {
					request->_sendSync();
				} else {
					request->_sendAsync();
				}
				return request;
			}
		}
		Ref<UrlRequest> request = new UrlRequest;
		if (request.isNotNull()) {
			LogError("UrlRequest", "Failed to create request on %s", url);
			request->_init(param, url);
			request->onError();
			return request;
		}
		return sl_null;
	}
	
	Ref<UrlRequest> URL_REQUEST::send(const String& url, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(const String& url, const HashMap<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.parameters = params;
		rp.onComplete = onComplete;
		return send(rp);
	}

	Ref<UrlRequest> URL_REQUEST::send(const String& url, const HashMap<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.parameters = params;
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJson(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJson(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const HashMap<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJson(const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJson(const String& url, const HashMap<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(const String& url)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(const String& url, const HashMap<String, Variant>& params)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.parameters = params;
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(HttpMethod method, const String& url)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJsonSynchronous(HttpMethod method, const String& url, const HashMap<String, Variant>& params, const Json& json)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postSynchronous(const String& url, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postSynchronous(const String& url, const HashMap<String, Variant>& params, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = UrlRequest::_buildRequestBody(body);
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJsonSynchronous(const String& url, const Json& json)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = json.toJsonString().toMemory();
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJsonSynchronous(const String& url, const HashMap<String, Variant>& params, const Json& json)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.flagSynchronous = sl_true;
		return send(rp);
	}

