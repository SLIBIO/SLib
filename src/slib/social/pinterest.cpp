/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/social/pinterest.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestUser)
	
	SLIB_DEFINE_JSON(PinterestUser)
	{
		if (isFromJson) {
			this->json = json;
		}
		SLIB_JSON_ADD_MEMBERS(id, url, first_name, last_name)
	}
	
	PinterestUser::PinterestUser()
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestParam)
	
	PinterestParam::PinterestParam()
	{
		authorizeUrl = "https://api.pinterest.com/oauth/";
		accessTokenUrl = "https://api.pinterest.com/v1/oauth/token";
		defaultScopes.add_NoLock("read_public");
	}

	SLIB_DEFINE_OBJECT(Pinterest, OAuth2)
	
	Pinterest::Pinterest(const PinterestParam& param) : OAuth2(param)
	{
	}
	
	Pinterest::~Pinterest()
	{
	}
	
	Ref<Pinterest> Pinterest::create(const PinterestParam& param)
	{
		return new Pinterest(param);
	}
	
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Pinterest>, _g_priv_social_pinterest_instance)
	
	void Pinterest::initialize(const PinterestParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_pinterest_instance)) {
			return;
		}
		_g_priv_social_pinterest_instance = create(param);
	}
	
	void Pinterest::initialize(const String& redirectUri, const String& appId, const String& appSecret)
	{
		PinterestParam param;
		param.preferenceName = "pinterest";
		param.clientId = appId;
		param.clientSecret = appSecret;
		param.loginRedirectUri = redirectUri;
		initialize(param);
	}
	
	Ref<Pinterest> Pinterest::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_pinterest_instance)) {
			return sl_null;
		}
		return _g_priv_social_pinterest_instance;
	}

	void Pinterest::authorizeRequest(UrlRequestParam& param, const OAuthAccessToken& token)
	{
		param.parameters.put("access_token", token.token);
	}
	
	String Pinterest::getRequestUrl(const String& path)
	{
		return "https://api.pinterest.com/v1/" + path;
	}
	
	void Pinterest::getUser(const String& userId, const Function<void(PinterestResult&, PinterestUser&)>& onComplete)
	{
		UrlRequestParam rp;
		if (userId.isNotEmpty()) {
			rp.url = getRequestUrl(String::format("users/%s", userId));
		} else {
			SLIB_STATIC_STRING(me, "me")
			rp.url = getRequestUrl(me);
		}
		rp.onComplete = [onComplete](UrlRequest* request) {
			PinterestResult result(request);
			PinterestUser user;
			if (!(request->isError())) {
				FromJson(result.response["data"], user);
				result.flagSuccess = user.id.isNotEmpty();
			}
			onComplete(result, user);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
}
