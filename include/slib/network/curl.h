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

#ifndef CHECKHEADER_SLIB_NETWORK_CURL
#define CHECKHEADER_SLIB_NETWORK_CURL

#include "definition.h"

#include "url_request.h"

namespace slib
{

	class SLIB_EXPORT CurlRequest
	{
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
		
	protected:
		static Ref<UrlRequest> _create(const UrlRequestParam& param, const String& url);
		
	};

}

#endif
