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

#include "slib/social/ebay.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EbayUser)
	
	EbayUser::EbayUser()
	{
	}
	
	String EbayUser::getPublicProfileURL(const String& userId)
	{
		if (userId.isNotEmpty()) {
			return "http://www.ebay.com/usr/" + userId;
		}
		return sl_null;
	}
	
	String EbayUser::getPublicProfileURL() const
	{
		return getPublicProfileURL(userId);
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EbayResult)
	
	EbayResult::EbayResult(UrlRequest* _request)
	{
		flagSuccess = sl_false;
		request = _request;
		if (_request) {
			response = _request->getResponseContentAsString();
		}
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EbayParam)
	
	EbayParam::EbayParam(sl_bool _flagProduction)
	{
		flagProduction = _flagProduction;
		if (_flagProduction) {
			authorizeUrl = "https://auth.ebay.com/oauth2/authorize";
			accessTokenUrl = "https://api.ebay.com/identity/v1/oauth2/token";
		} else {
			authorizeUrl = "https://auth.sandbox.ebay.com/oauth2/authorize";
			accessTokenUrl = "https://api.sandbox.ebay.com/identity/v1/oauth2/token";
		}
		defaultScopes.add_NoLock("https://api.ebay.com/oauth/api_scope");
	}
	
	void EbayParam::setRedirectUrl(const String& ruName, const String& _loginRedirectUri)
	{
		redirectUri = ruName;
		loginRedirectUri = _loginRedirectUri;
	}

	SLIB_DEFINE_OBJECT(Ebay, OAuth2)
	
	Ebay::Ebay(const EbayParam& param) : OAuth2(param)
	{
		m_flagProduction = param.flagProduction;
	}
	
	Ebay::~Ebay()
	{
	}
	
	Ref<Ebay> Ebay::create(const EbayParam& param)
	{
		return new Ebay(param);
	}
	
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Ebay>, _g_priv_social_ebay_instance)
	
	void Ebay::initialize(const EbayParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_ebay_instance)) {
			return;
		}
		_g_priv_social_ebay_instance = create(param);
	}
	
	void Ebay::initialize(sl_bool flagProduction, const String& ruName, const String& redirectUri, const String& appId, const String& appSecret)
	{
		EbayParam param(flagProduction);
		param.preferenceName = "ebay";
		param.clientId = appId;
		param.clientSecret = appSecret;
		param.setRedirectUrl(ruName, redirectUri);
		initialize(param);
	}
	
	Ref<Ebay> Ebay::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_ebay_instance)) {
			return sl_null;
		}
		return _g_priv_social_ebay_instance;
	}
	
	String Ebay::getRequestUrl(const String& path)
	{
		if (m_flagProduction) {
			return "https://api.ebay.com/" + path;
		} else {
			return "https://api.sandbox.ebay.com/" + path;
		}
	}
	
	void Ebay::callTraditionalApi(const String& callName, const String& request, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = getRequestUrl("ws/api.dll");
		rp.method = HttpMethod::POST;
		rp.requestHeaders.put_NoLock("X-EBAY-API-COMPATIBILITY-LEVEL", "1085");
		rp.requestHeaders.put_NoLock("X-EBAY-API-SITEID", "0");
		rp.requestHeaders.put_NoLock("X-EBAY-API-CALL-NAME", callName);
		rp.requestHeaders.put_NoLock("X-EBAY-API-IAF-TOKEN", getAccessTokenKey());
		rp.requestHeaders.put_NoLock("Content-Type", "text/xml");
		rp.setRequestBodyAsString(request);
		rp.onComplete = onComplete;
		UrlRequest::send(rp);
	}
	
	static String _priv_Ebay_getSimpleXMLValue(const String& xml, const String& tagName) {
		sl_reg index1 = xml.indexOf("<" + tagName + ">");
		if (index1 < 0) {
			return sl_null;
		}
		sl_reg index2 = xml.indexOf("</" + tagName + ">", index1 + 1);
		if (index2 < 0) {
			return sl_null;
		}
		return xml.substring(index1 + tagName.getLength() + 2, index2);
	}
	
	/*
	static List<String> _priv_Ebay_getSimpleXMLValues(const String& xml, const String& tagName) {
		List<String> ret;
		sl_reg index = 0;
		while (true) {
			sl_reg index1 = xml.indexOf("<" + tagName + ">", index);
			if (index1 < 0) {
				return sl_null;
			}
			sl_reg index2 = xml.indexOf("</" + tagName + ">", index1 + 1);
			if (index2 < 0) {
				return sl_null;
			}
			ret.add(xml.substring(index1 + tagName.getLength() + 2, index2));
			index = index2 + 1;
		}
		return ret;
	}
	*/
	
	void Ebay::getUser(const Function<void(EbayResult&, EbayUser&)>& onComplete)
	{
		String request = SLIB_STRINGIFY(
			<?xml version="1.0" encoding="utf-8"?>
			<GetUserRequest xmlns="urn:ebay:apis:eBLBaseComponents" />
		);
		callTraditionalApi("GetUser", request, [onComplete](UrlRequest* request) {
			EbayResult result(request);
			EbayUser user;
			if (!(request->isError())) {
				user.userId = _priv_Ebay_getSimpleXMLValue(result.response, "UserID");
				user.email = _priv_Ebay_getSimpleXMLValue(result.response, "Email");
				result.flagSuccess = user.userId.isNotEmpty();
			}
			onComplete(result, user);
		});
	}
	
}
