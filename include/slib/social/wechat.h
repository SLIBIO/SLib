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

#ifndef CHECKHEADER_SLIB_SOCIAL_WECHAT
#define CHECKHEADER_SLIB_SOCIAL_WECHAT

#include "definition.h"

#include "oauth.h"

namespace slib
{

	class SLIB_EXPORT WeChatUser
	{
	public:
		String openid;
		String nickname;
		sl_uint32 sex;
		String province;
		String city;
		String country;
		String headimgurl;
		List<String> privilege;
		String unionid;

		Json json;
		
	public:
		WeChatUser();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WeChatUser)
		
		SLIB_DECLARE_JSON
		
	};

	class SLIB_EXPORT WeChatAppResult
	{
	public:
		sl_bool flagSuccess;
		sl_bool flagCancel;
		String error;

	public:
		WeChatAppResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WeChatAppResult)

	};

	typedef OAuthApiResult WeChatResult;

	typedef OAuthLoginResult WeChatLoginResult;

	typedef OAuthLoginParam WeChatLoginParam;

	class SLIB_EXPORT WeChatPaymentOrder
	{
	public:
		String partnerId;
		String prepayId;
		String package;
		String nonceStr;
		sl_uint64 timeStamp;
		String sign;
		
	public:
		WeChatPaymentOrder();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WeChatPaymentOrder)
		
	};

	class SLIB_EXPORT WeChatPaymentResult : public WeChatAppResult
	{
	public:
		WeChatPaymentResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WeChatPaymentResult)

	};

	class SLIB_EXPORT WeChatPaymentRequest : public WeChatPaymentOrder
	{
	public:
		Function<void(WeChatPaymentResult&)> onComplete;
		
	public:
		WeChatPaymentRequest();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WeChatPaymentRequest)
		
	};

	class SLIB_EXPORT WeChatParam : public OAuthParam
	{
	public:
		WeChatParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WeChatParam)
		
	};

	class WeChat : public OAuth2
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		WeChat(const WeChatParam& param);
		
		~WeChat();
		
	public:
		static Ref<WeChat> create(const WeChatParam& param);
		
		static void initialize(const WeChatParam& param);
		
		static void initialize();
		
		static Ref<WeChat> create(const String& appId, const String& appSecret, const String& redirectUrl);

		static void initialize(const String& appId, const String& appSecret, const String& redirectUrl);
		
		static Ref<WeChat> create(const String& appId, const String& redirectUrl);
		
		static void initialize(const String& appId, const String& redirectUrl);
		
		static Ref<WeChat> createWithAccessToken(const String& accessToken);
		
		static Ref<WeChat> getInstance();

	public:
		String getOpenId();
		
		void authorizeRequest(UrlRequestParam& param, const OAuthAccessToken& token) override;
		using OAuth2::authorizeRequest;

		String getRequestUrl(const String& path);
		
		void getUser(const String& openId, const Function<void(WeChatResult&, WeChatUser&)>& onComplete);
		
		void getUser(const Function<void(WeChatResult&, WeChatUser&)>& onComplete);

	protected:
		void onCompleteRequestAccessToken(OAuthAccessTokenResult& result) override;
		
	protected:
		String m_currentOpenId;
		
	};

	class SLIB_EXPORT WeChatSDK
	{
	public:
		static void initialize(const String& appId, const String& universalURL);
		
	public:
		static void login(const WeChatLoginParam& param);
		
		static void login(const Function<void(WeChatLoginResult& result)>& onComplete);

		static void pay(const WeChatPaymentRequest& req);
		
	};
	
}

#endif
