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

#include "slib/crypto/md5.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace wechat
		{
					
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<WeChat>, g_instance)
		
			static String GenerateSign(const Map<String, String>& map, const String& apiKey)
			{
				StringBuffer buf;
				sl_bool flagFirst = sl_true;
				for (auto& item : map) {
					if (item.value.isNotEmpty()) {
						if (flagFirst) {
							flagFirst = sl_false;
						} else {
							buf.addStatic("&");
						}
						buf.add(item.key);
						buf.addStatic("=");
						buf.add(item.value);
					}
				}
				buf.addStatic("&key=");
				buf.add(apiKey);
				return String::makeHexString(MD5::hash(buf.merge()), sl_false);
			}
		
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

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatCreateOrderResult)

	WeChatCreateOrderResult::WeChatCreateOrderResult()
	{
		flagSuccess = sl_false;
		request = sl_null;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatCreateOrderParam)

	WeChatCreateOrderParam::WeChatCreateOrderParam()
	{
		amount = 0;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WeChatParam)

	WeChatParam::WeChatParam()
	{
		authorizeUrl = "https://open.weixin.qq.com/connect/qrconnect";
		accessTokenUrl = "https://api.weixin.qq.com/sns/oauth2/access_token";
		accessTokenMethod = HttpMethod::GET;
		defaultScopes.add_NoLock("snsapi_login");
		flagSupportImplicitGrantType = sl_false;
		clientIdFieldName = "appid";
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

	String WeChat::getOpenId()
	{
		return m_currentOpenId;
	}

	void WeChat::authorizeRequest(UrlRequestParam& param, const OAuthAccessToken& token)
	{
		param.parameters.put_NoLock("access_token", token.token);
	}
	
	String WeChat::getRequestUrl(const String& path)
	{
		return "https://api.weixin.qq.com/" + path;
	}
	
	void WeChat::getUser(const String& openId, const Function<void(WeChatResult&, WeChatUser&)>& onComplete)
	{
		UrlRequestParam rp;
		SLIB_STATIC_STRING(pathUserInfo, "sns/userinfo")
		rp.url = getRequestUrl(pathUserInfo);
		if (openId.isNotEmpty()) {
			rp.parameters.put_NoLock("openid", openId);
		} else {
			rp.parameters.put_NoLock("openid", m_currentOpenId);
		}
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

	void WeChat::getUser(const Function<void(WeChatResult&, WeChatUser&)>& onComplete)
	{
		getUser(String::null(), onComplete);
	}

	void WeChat::createOrder(const WeChatCreateOrderParam& param)
	{
		Map<String, String> map;
		map.put_NoLock("appid", param.appId);
		map.put_NoLock("mch_id", param.businessId);
		if (param.deviceId.isNotNull()) {
			map.put_NoLock("device_info", param.deviceId);
		}
		if (param.nonce.isNotEmpty()) {
			map.put_NoLock("nonce_str", param.nonce);
		} else {
			char s[16];
			Math::randomMemory(s, 16);
			map.put_NoLock("nonce_str", String::makeHexString(s, 16));
		}
		map.put_NoLock("body", param.body);
		if (param.detail.isNotNull()) {
			map.put_NoLock("detail", param.detail);
		}
		if (param.attach.isNotNull()) {
			map.put_NoLock("attach", param.attach);
		}
		map.put_NoLock("out_trade_no", param.orderId);
		if (param.currency.isNotEmpty()) {
			map.put_NoLock("fee_type", param.currency);
		}
		map.put_NoLock("total_fee", String::fromUint64(param.amount));
		map.put_NoLock("spbill_create_ip", param.ip);
		if (param.timeStart.isNotZero()) {
			map.put_NoLock("time_start", param.timeStart.format("%04y%02m%02d%02H%02M%02S"));
		}
		if (param.timeExpire.isNotZero()) {
			map.put_NoLock("time_start", param.timeExpire.format("%04y%02m%02d%02H%02M%02S"));
		}
		map.put_NoLock("notify_url", param.notifyUrl);
		map.put_NoLock("trade_type", "APP");
		String sign = GenerateSign(map, param.apiKey);
		
		UrlRequestParam rp;
		rp.url = "https://api.mch.weixin.qq.com/pay/unifiedorder";
		rp.method = HttpMethod::POST;
		StringBuffer buf;
		buf.addStatic("<xml>");
		for (auto& item : map) {
			buf.addStatic("<");
			buf.add(item.key);
			buf.addStatic(">");
			buf.add(item.value);
			buf.addStatic("</");
			buf.add(item.key);
			buf.addStatic(">");
		}
		buf.addStatic("<sign>");
		buf.add(sign);
		buf.addStatic("</sign>");
		buf.addStatic("</xml>");
		rp.setRequestBodyAsString(buf.merge());
		auto onComplete = param.onComplete;
		String apiKey = param.apiKey;
		rp.onComplete = [onComplete, apiKey](UrlRequest* req) {
			WeChatCreateOrderResult result;
			result.request = req;
			result.responseText = req->getResponseContentAsString();
			if (!(req->isError())) {
				result.response = Xml::parseXml(result.responseText);
				if (result.response.isNotNull()) {
					Ref<XmlElement> root = result.response->getFirstChildElement();
					if (root.isNotNull()) {
						result.returnCode = root->getFirstChildElementText("return_code");
						result.returnMessage = root->getFirstChildElementText("return_msg");
						if (result.returnCode == "SUCCESS") {
							result.resultCode = root->getFirstChildElementText("result_code");
							result.errorCode = root->getFirstChildElementText("err_code");
							result.errorDescription = root->getFirstChildElementText("err_code_des");
							if (result.resultCode == "SUCCESS") {
								result.flagSuccess = sl_true;
								result.order.partnerId = root->getFirstChildElementText("mch_id");
								result.order.prepayId = root->getFirstChildElementText("prepay_id");
								result.order.nonce = root->getFirstChildElementText("nonce_str");
								result.order.package = "Sign = WXPay";
								result.order.timeStamp = Time::now().toUnixTime() + 3600 * 8;
								Map<String, String> map;
								String appId = root->getFirstChildElementText("appid");
								map.put_NoLock("appid", appId);
								map.put_NoLock("partnerid", result.order.partnerId);
								map.put_NoLock("prepayid", result.order.prepayId);
								map.put_NoLock("package", result.order.package);
								map.put_NoLock("noncestr", result.order.nonce);
								map.put_NoLock("timestamp", String::fromUint64(result.order.timeStamp));
								result.order.sign = GenerateSign(map, apiKey);
							}
						}
					}
				}
			}
			onComplete(result);
		};
		UrlRequest::send(rp);
	}

	void WeChat::onCompleteRequestAccessToken(OAuthAccessTokenResult& result)
	{
		m_currentOpenId = result.response["openid"].getString();
	}

}
