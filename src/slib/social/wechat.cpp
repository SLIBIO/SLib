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

#include "slib/social/wechat.h"

#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace wechat
		{
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<WeChat>, g_instance)
		}
	}

	using namespace priv::wechat;


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatUser)

	SLIB_DEFINE_JSON_MEMBERS(WeChatUser, openid, nickname, sex, province, city, country, headimgurl, privilege, unionid)

	WeChatUser::WeChatUser()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatAppResult)

	WeChatAppResult::WeChatAppResult()
	{
		flagSuccess = sl_false;
		flagCancel = sl_false;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatPaymentOrder)

	WeChatPaymentOrder::WeChatPaymentOrder()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatPaymentResult)

	WeChatPaymentResult::WeChatPaymentResult()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatPaymentRequest)

	WeChatPaymentRequest::WeChatPaymentRequest()
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatParam)

	WeChatParam::WeChatParam()
	{
		authorizeUrl = "https://open.weixin.qq.com/connect/qrconnect";
		accessTokenUrl = "https://api.weixin.qq.com/sns/oauth2/access_token";
		accessTokenMethod = HttpMethod::GET;
		defaultScopes.add_NoLock("snsapi_login");
		flagSupportImplicitGrantType = sl_false;
		clientIdFieldName = "app_id";
		clientSecretFieldName = "secret";
	}


	SLIB_DEFINE_OBJECT(WeChat, OAuth2)
	
	WeChat::WeChat(const WeChatParam& param) : OAuth2(param)
	{
	}
	
	WeChat::~WeChat()
	{
	}
	
	Ref<WeChat> WeChat::create(const WeChatParam& param)
	{
		return new WeChat(param);
	}
	
	void WeChat::initialize(const WeChatParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return;
		}
		g_instance = create(param);
	}
	
	void WeChat::initialize()
	{
		WeChatParam param;
		param.preferenceName = "wechat";
		initialize(param);
	}
	
	Ref<WeChat> WeChat::create(const String& appId, const String& appSecret, const String& redirectUri)
	{
		WeChatParam param;
		param.clientId = appId;
		param.clientSecret = appSecret;
		param.redirectUri = redirectUri;
		return create(param);
	}
	
	void WeChat::initialize(const String& appId, const String& appSecret, const String& redirectUri)
	{
		WeChatParam param;
		param.preferenceName = "wechat";
		param.clientId = appId;
		param.clientSecret = appSecret;
		param.redirectUri = redirectUri;
		initialize(param);
	}
	
	Ref<WeChat> WeChat::create(const String& appId, const String& redirectUri)
	{
		return create(appId, String::null(), redirectUri);
	}
	
	void WeChat::initialize(const String& appId, const String& redirectUri)
	{
		initialize(appId, String::null(), redirectUri);
	}
	
	Ref<WeChat> WeChat::createWithAccessToken(const String& accessToken)
	{
		WeChatParam param;
		param.accessToken.token = accessToken;
		return create(param);
	}
	
	Ref<WeChat> WeChat::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return sl_null;
		}
		return g_instance;
	}

	String WeChat::getRequestUrl(const String& path)
	{
		return "https://api.weixin.qq.com/" + path;
	}
	
	void WeChat::getUser(const Function<void(WeChatResult&, WeChatUser&)>& onComplete)
	{
		UrlRequestParam rp;
		SLIB_STATIC_STRING(pathUserInfo, "sns/userinfo")
		rp.url = getRequestUrl(pathUserInfo);
		rp.onComplete = [onComplete](UrlRequest* request) {
			WeChatResult result(request);
			WeChatUser user;
			if (!(request->isError())) {
				FromJson(result.response, user);
				result.flagSuccess = user.unionid.isNotEmpty();
			}
			onComplete(result, user);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}

}
