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

#ifndef CHECKHEADER_SLIB_SOCIAL_OAUTH
#define CHECKHEADER_SLIB_SOCIAL_OAUTH

/**************************************************************************
 
 							OAuth 1.0
 				https://tools.ietf.org/html/rfc5849
 
 							OAuth 2.0
				https://tools.ietf.org/html/rfc6749
				https://tools.ietf.org/html/rfc6750
 
**************************************************************************/

#include "definition.h"

#include "../network/url_request.h"

namespace slib
{
	
	class SLIB_EXPORT OAuthParam
	{
	public:
		String consumerKey;
		String consumerSecret;
		
		String token;
		String tokenSecret;
		
		String requestTokenUrl;
		String authenticateUrl;
		String accessTokenUrl;
		String callbackUrl;
		
	public:
		OAuthParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthParam)
		
	};
	
	class SLIB_EXPORT OAuthLoginResult
	{
	public:
		sl_bool flagError;		
		sl_bool flagCancel;
		
		String token;
		String tokenSecret;
		
		HashMap<String, String> parameters;
		
	public:
		OAuthLoginResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthLoginResult)
		
	};
	
	class SLIB_EXPORT OAuthLoginParam
	{
	public:
		Function<void(OAuthLoginResult& result)> onComplete;
		
	public:
		OAuthLoginParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthLoginParam)
		
	};
	
	class SLIB_EXPORT OAuth1 : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuth1(const OAuthParam& param);
		
		~OAuth1();
		
	public:
		String getToken();
		
		void setToken(const String& token);
		
		String getTokenSecret();
		
		void setTokenSecret(const String& tokenSecret);
		
		String getCallbackUrl();
		
		void setCallbackUrl(const String& url);
		
		String generateAuthorization(HttpMethod method, const String& url, HashMap<String, String>& parameters, const String& nonce, sl_int64 timestamp, const String& token, const String& tokenSecret, const String& callbackUrl);
		
		void authorizeRequest(UrlRequestParam& param, const String& token, const String& tokenSecret, const String& callbackUrl);
		
		void authorizeRequest(UrlRequestParam& param);

		void getLoginUrl(const Function<void(const String& url, const String& token, const String& tokenSecret)>& callback);
		
		void login(const OAuthLoginParam& param);
		
		static void logUrlRequestError(UrlRequest* request);
		
	protected:
		String m_consumerKey;
		String m_consumerSecret;
		
		AtomicString m_token;
		AtomicString m_tokenSecret;

		String m_requestTokenUrl;
		String m_authenticateUrl;
		String m_accessTokenUrl;
		AtomicString m_callbackUrl;
		
	};
	
}

#endif
