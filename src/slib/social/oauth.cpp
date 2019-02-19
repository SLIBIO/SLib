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

#include "slib/social/oauth.h"

#include "slib/crypto/base64.h"
#include "slib/crypto/hmac.h"
#include "slib/crypto/sha1.h"
#include "slib/core/log.h"

#define TAG "OAuth"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(OAuthParam)
	
	OAuthParam::OAuthParam()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(OAuth1, Object)
	
	OAuth1::OAuth1(const OAuthParam& param)
	{
		m_consumerKey = param.consumerKey;
		m_consumerSecret = param.consumerSecret;
		m_token = param.token;
		m_tokenSecret = param.tokenSecret;
		m_requestTokenUrl = param.requestTokenUrl;
		m_authenticateUrl = param.authenticateUrl;
		m_accessTokenUrl = param.accessTokenUrl;
		m_callbackUrl = param.callbackUrl;
	}
	
	OAuth1::~OAuth1()
	{
	}
	
	String OAuth1::getToken()
	{
		return m_token;
	}
	
	void OAuth1::setToken(const String& token)
	{
		m_token = token;
	}
	
	String OAuth1::getTokenSecret()
	{
		return m_tokenSecret;
	}
	
	void OAuth1::setTokenSecret(const String& tokenSecret)
	{
		m_tokenSecret = tokenSecret;
	}
	
	String OAuth1::getCallbackUrl()
	{
		return m_callbackUrl;
	}
	
	void OAuth1::setCallbackUrl(const String& url)
	{
		m_callbackUrl = url;
	}
	
	String OAuth1::generateAuthorization(HttpMethod _method, const String& _url, HashMap<String, String>& parameters, const String& nonce, sl_int64 _timestamp, const String& token, const String& tokenSecret, const String& callbackUrl)
	{
		String consumerKey = Url::encodePercentByUTF8(m_consumerKey);
		String timestamp = String::fromInt64(_timestamp);

		Map< Pair<String, String>, sl_bool > mapParams;
		String method = HttpMethods::toString(_method);
		String url = _url;
		sl_reg indexQuery = url.indexOf('?');
		if (indexQuery > 0) {
			for (auto& item : HttpRequest::parseParameters(url.substring(indexQuery + 1))) {
				mapParams.add_NoLock(Pair<String, String>(Url::encodePercentByUTF8(item.key), Url::encodePercentByUTF8(item.value)), sl_true);
			}
			url = url.substring(0, indexQuery);
		}
		for (auto& item : parameters) {
			mapParams.add_NoLock(Pair<String, String>(Url::encodePercentByUTF8(item.key), Url::encodePercentByUTF8(item.value)), sl_true);
		}
		mapParams.add_NoLock(Pair<String, String>("oauth_consumer_key", consumerKey), sl_true);
		mapParams.add_NoLock(Pair<String, String>("oauth_nonce", nonce), sl_true);
		mapParams.add_NoLock(Pair<String, String>("oauth_signature_method", "HMAC-SHA1"), sl_true);
		mapParams.add_NoLock(Pair<String, String>("oauth_timestamp", timestamp), sl_true);
		mapParams.add_NoLock(Pair<String, String>("oauth_version", "1.0"), sl_true);
		if (token.isNotEmpty()) {
			mapParams.add_NoLock(Pair<String, String>("oauth_token", Url::encodePercentByUTF8(token)), sl_true);
		}
		if (callbackUrl.isNotEmpty()) {
			mapParams.add_NoLock(Pair<String, String>("oauth_callback", Url::encodePercentByUTF8(callbackUrl)), sl_true);
		}
		
		StringBuffer sbParams;
		{
			sl_bool flagFirstParam = sl_true;
			for (auto& item : mapParams) {
				if (flagFirstParam) {
					flagFirstParam = sl_false;
				} else {
					sbParams.addStatic("&", 1);
				}
				sbParams.add(item.key.first);
				sbParams.addStatic("=", 1);
				sbParams.add(item.key.second);
			}
		}
		
		StringBuffer sbSignature;
		sbSignature.add(method);
		sbSignature.addStatic("&", 1);
		sbSignature.add(Url::encodePercentByUTF8(url));
		sbSignature.addStatic("&", 1);
		sbSignature.add(Url::encodePercentByUTF8(sbParams.merge()));
		String signatureBase = sbSignature.merge();
		String signKey = Url::encodePercentByUTF8(m_consumerSecret) + "&";
		if (tokenSecret.isNotEmpty()) {
			signKey += Url::encodePercentByUTF8(tokenSecret);
		}
		char bufSignature[SHA1::HashSize];
		HMAC<SHA1>::execute(signKey.getData(), signKey.getLength(), signatureBase.getData(), signatureBase.getLength(), bufSignature);
		String signature = Url::encodePercentByUTF8(Base64::encode(bufSignature, SHA1::HashSize));

		mapParams.add_NoLock(Pair<String, String>("oauth_signature", signature), sl_true);
		StringBuffer sbAuthorization;
		{
			sbAuthorization.addStatic("OAuth ", 6);
			sl_bool flagFirstParam = sl_true;
			for (auto& item : mapParams) {
				if (item.key.first.startsWith("oauth_")) {
					if (flagFirstParam) {
						flagFirstParam = sl_false;
					} else {
						sbAuthorization.addStatic(", ", 2);
					}
					sbAuthorization.add(item.key.first);
					sbAuthorization.addStatic("=\"", 2);
					sbAuthorization.add(item.key.second);
					sbAuthorization.addStatic("\"", 1);
				}
			}
		}
		return sbAuthorization.merge();
	}
	
	void OAuth1::authorizeRequest(UrlRequestParam& param, const String& token, const String& tokenSecret, const String& callbackUrl)
	{
		char bufNonce[32];
		Math::randomMemory(bufNonce, 32);
		String nonce = String::makeHexString(bufNonce, 32);
		sl_int64 timestamp = Time::now().toUnixTime();

		HashMap<String, String> parameters;
		for (auto& item : param.parameters) {
			parameters.add_NoLock(item.key, item.value.getString());
		}
		
		if (param.method == HttpMethod::POST) {
			String type = param.requestHeaders.getValue(HttpHeaders::ContentType);
			sl_reg index = type.indexOf(';');
			if (index >= 0) {
				type = type.substring(0, index);
			}
			if (!(type.trim().equalsIgnoreCase(ContentTypes::MultipartFormData))) {
				if (param.requestBody.isNotNull()) {
					for (auto& item : HttpRequest::parseParameters(param.requestBody.getData(), param.requestBody.getSize())) {
						parameters.add_NoLock(item.key, item.value);
					}
				}
			}
		}
		
		String authorization = generateAuthorization(param.method, param.url, parameters, nonce, timestamp, token, tokenSecret, callbackUrl);
		param.requestHeaders.add_NoLock("Authorization", authorization);
	}
	
	void OAuth1::authorizeRequest(UrlRequestParam& param)
	{
		authorizeRequest(param, m_token, m_tokenSecret, sl_null);
	}
	
	void OAuth1::getLoginUrl(const HashMap<String, String>& requestParams, const Function<void(const String& url, const String& token, const String& tokenSecret)>& callback)
	{
		UrlRequestParam param;
		param.method = HttpMethod::POST;
		param.url = m_requestTokenUrl;
		param.parameters.putAll(requestParams);
		String authenticateUrl = m_authenticateUrl;
		auto weak = ToWeakRef(this);
		param.onComplete = [weak, callback](UrlRequest* request) {
			auto _this = ToRef(weak);
			if (_this.isNull()) {
				callback(sl_null, sl_null, sl_null);
				return;
			}
			if (request->isError()) {
				logUrlRequestError(request);
				callback(sl_null, sl_null, sl_null);
				return;
			}
			String response = request->getResponseContentAsString();
			Log(TAG, "RequestToken Response: %s", response);
			auto params = HttpRequest::parseParameters(response);
			String token = params["oauth_token"];
			String tokenSecret = params["oauth_token_secret"];
			String loginUrl = params["login_url"];
			if (loginUrl.isEmpty()) {
				loginUrl = _this->m_authenticateUrl.arg(Url::encodePercentByUTF8(token));
			}
			callback(loginUrl, token, tokenSecret);
		};
		authorizeRequest(param, sl_null, sl_null, m_callbackUrl);
		UrlRequest::send(param);
	}
	
	void OAuth1::logUrlRequestError(UrlRequest* request)
	{
		LogError(TAG, "Error: %s, Status: %s, Response: %s", request->getLastErrorMessage(), HttpStatuses::toString(request->getResponseStatus()), request->getResponseContentAsString());
	}
	
}
