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
			url += HttpRequest::buildQuery(param.parameters);
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
			request->_init(param, url);
			request->m_lastErrorMessage = "Failed to create request";
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
	
	Ref<UrlRequest> URL_REQUEST::send(const String& url, const HttpHeaderMap& headers, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.requestHeaders = headers;
		rp.onComplete = onComplete;
		return send(rp);
	}

	Ref<UrlRequest> URL_REQUEST::send(const String& url, const HttpHeaderMap& headers, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.requestHeaders = headers;
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
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestHeaders = headers;
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::send(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestHeaders = headers;
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}

	Ref<UrlRequest> URL_REQUEST::sendJson(HttpMethod method, const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJson(HttpMethod method, const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJson(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestHeaders = headers;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJson(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestHeaders = headers;
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
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestHeaders = headers;
		rp.setRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::post(const String& url, const HttpHeaderMap& headers, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestHeaders = headers;
		rp.setRequestBody(body);
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
	
	Ref<UrlRequest> URL_REQUEST::postJson(const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestHeaders = headers;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postJson(const String& url, const HttpHeaderMap& headers, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestHeaders = headers;
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
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(const String& url, const HttpHeaderMap& headers)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.requestHeaders = headers;
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
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(HttpMethod method, const String& url, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.setRequestBody(body);
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendSynchronous(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestHeaders = headers;
		rp.setRequestBody(body);
		rp.flagSynchronous = sl_true;
		return send(rp);
	}

	Ref<UrlRequest> URL_REQUEST::sendJsonSynchronous(HttpMethod method, const String& url, const Json& json)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestBody = json.toJsonString().toMemory();
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::sendJsonSynchronous(HttpMethod method, const String& url, const HttpHeaderMap& headers, const Json& json)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.requestHeaders = headers;
		rp.requestBody = json.toJsonString().toMemory();
		rp.flagSynchronous = sl_true;
		return send(rp);
	}

	Ref<UrlRequest> URL_REQUEST::postSynchronous(const String& url, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.setRequestBody(body);
		rp.flagSynchronous = sl_true;
		return send(rp);
	}
	
	Ref<UrlRequest> URL_REQUEST::postSynchronous(const String& url, const HttpHeaderMap& headers, const Variant& body)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestHeaders = headers;
		rp.setRequestBody(body);
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
	
	Ref<UrlRequest> URL_REQUEST::postJsonSynchronous(const String& url, const HttpHeaderMap& headers, const Json& json)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestHeaders = headers;
		rp.requestBody = json.toJsonString().toMemory();
		rp.flagSynchronous = sl_true;
		return send(rp);
	}

