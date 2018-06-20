/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
