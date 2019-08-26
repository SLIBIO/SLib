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
	
	class Window;
	class WebView;
	
	class SLIB_EXPORT OAuthWebRedirectDialogOptions
	{
	public:
		Ref<Window> parentWindow;
		
	public:
		OAuthWebRedirectDialogOptions();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthWebRedirectDialogOptions)
		
	};
	
	class SLIB_EXPORT OAuthWebRedirectDialogParam
	{
	public:
		String url;
		OAuthWebRedirectDialogOptions options;
		
		Function<void(const String& url)> onRedirect;
		
	public:
		OAuthWebRedirectDialogParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthWebRedirectDialogParam)
		
	};
	
	class SLIB_EXPORT OAuthWebRedirectDialog
	{
	public:
		OAuthWebRedirectDialog();
		
		~OAuthWebRedirectDialog();
		
	public:
		virtual Ref<WebView> getWebView() = 0;
		
		virtual void show(const OAuthWebRedirectDialogParam& param) = 0;
		
		virtual void close() = 0;
		
	public:
		static Ptr<OAuthWebRedirectDialog> getDefault();
		
		static void showDefault(const OAuthWebRedirectDialogParam& param);

	};
	
	class SLIB_EXPORT OAuth1_AccessToken
	{
	public:
		String token;
		String secret;
		
	public:
		OAuth1_AccessToken();
		
		OAuth1_AccessToken(const String& token, const String& tokenSecret);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuth1_AccessToken)
		
		SLIB_DECLARE_JSON
		
	public:
		sl_bool isValid() const;
		
		void setResponse(const HashMap<String, String>& params);
		
	};
	
	class SLIB_EXPORT OAuth1_AuthorizationRequestParam
	{
	public:
		String callbackUrl; // If empty, uses instance's callbackUrl attribute
		HashMap<String, Variant> customParameters;

	public:
		OAuth1_AuthorizationRequestParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuth1_AuthorizationRequestParam)
		
	};
	
	class SLIB_EXPORT OAuth1_AccessTokenResult
	{
	public:
		sl_bool flagSuccess;
		
		OAuth1_AccessToken accessToken;
		
		HashMap<String, String> response;
		
	public:
		OAuth1_AccessTokenResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuth1_AccessTokenResult)
		
	public:
		void setResponse(const HashMap<String, String>& params);
		
	};
	
	class SLIB_EXPORT OAuth1_LoginResult : public OAuth1_AccessTokenResult
	{
	public:
		sl_bool flagCancel;
		sl_bool flagCache; // True if the `access token` is from cache

		String verifier;
		String requestToken;

	public:
		OAuth1_LoginResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuth1_LoginResult)
		
	public:
		void parseRedirectUrl(const String& url);
		
	};
	
	class SLIB_EXPORT OAuth1_LoginParam
	{
	public:
		String url; // Should be defined in uncofidental clients (such as mobile/desktop app). See class description for detail.
		
		OAuth1_AuthorizationRequestParam authorization;
		
		OAuthWebRedirectDialogOptions dialogOptions;
		Ptr<OAuthWebRedirectDialog> dialog;

		sl_bool flagIgnoreExistingAccessToken; // Ignored if `url` is not empty

		Function<void(OAuth1_LoginResult&)> onComplete;
		
	public:
		OAuth1_LoginParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuth1_LoginParam)
		
	};
	
	class SLIB_EXPORT OAuth1_Param
	{
	public:
		String consumerKey;
		String consumerSecret;
		
		OAuth1_AccessToken accessToken;
		
		HttpMethod requestTokenMethod;
		String requestTokenUrl;
		String authenticateUrl;
		HttpMethod accessTokenMethod;
		String accessTokenUrl;
		String callbackUrl;
		
		String preferenceName;
		
	public:
		OAuth1_Param();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuth1_Param)
		
	};
	
	/*
	 	`OAuth1` authentication is not recommended to be used in unconfidental clients
	 	(such as mobile/desktop app), because you can't keep secret the `consumer secret` in such clients.
	 	`TwitterKit`(now discontinued) also saves `consumer secret` in client app, but this is not
	 	good practice.
	 	Instead, you should use the authentication functions in your own API server.
	 	You can get `login url` from your server using `getLoginUrl()`.
	 	After getting `login url`, you can call `login()` function in client app (put the `login url`
	 	in the param), and then you can get `verifier` in callback after user-web-login process.
	 	After getting `verifier`, you should send it to your server, and you can call
	 	`requesteAccessToken` in the server. Your server should return the `access token` to the client,
	 	and then your client app can call Twitter APIs after setting `access token` by `setAccessToken()`.
	 	Btw, `OAuth1` supports complete authentication flow, but you should follow above instruction for security.
	 	your Twitter
	*/
	class SLIB_EXPORT OAuth1 : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuth1(const OAuth1_Param& param);
		
		~OAuth1();
		
	public:
		Ptr<OAuth1_AccessToken> getAccessToken();
		
		void setAccessToken(const OAuth1_AccessToken& accessToken);
		
		void setAccessToken(const String& token, const String& tokenSecret);
		
		void clearAccessToken();
		
		String getAccessTokenKey();
		
		String getAccessTokenSecret();
		
		String getCallbackUrl();
		
		void setCallbackUrl(const String& url);

		void setLoggingErrors(sl_bool flag);
		

		String generateAuthorization(HttpMethod method, const String& url, HashMap<String, String>& parameters, const String& nonce, sl_int64 timestamp, const String& token, const String& tokenSecret, const String& callbackUrl);
		
		virtual void authorizeRequest(UrlRequestParam& param, const String& token, const String& tokenSecret, const String& callbackUrl);
		
		void authorizeRequest(UrlRequestParam& param);

		virtual void getLoginUrl(const OAuth1_AuthorizationRequestParam& param, const Function<void(const String& url, const String& requestToken, const String& requestTokenSecret)>& onComplete);
		
		void getLoginUrl(const Function<void(const String& url, const String& requestToken, const String& requestTokenSecret)>& onComplete);
		
		void requestAccessToken(const String& verifier, const String& requestToken, const String& requestTokenSecret, const Function<void(OAuth1_AccessTokenResult&)>& onComplete);
		
		void login(const OAuth1_LoginParam& param);
		
		void login(const Function<void(OAuth1_LoginResult& result)>& onComplete);
		
	protected:
		void logUrlRequestError(UrlRequest* request);
		
		void save();
		
		void restore();
		
	protected:
		String m_consumerKey;
		String m_consumerSecret;
		
		AtomicPtr<OAuth1_AccessToken> m_accessToken;

		HttpMethod m_requestTokenMethod;
		String m_requestTokenUrl;
		String m_authenticateUrl;
		HttpMethod m_accessTokenMethod;
		String m_accessTokenUrl;
		AtomicString m_callbackUrl;
		
		sl_bool m_flagLogErrors;

		String m_preferenceName;

	};
	
	
	enum class OAuthGrantType
	{
		Token = 0,
		Code = 1
	};
	
	enum class OAuthErrorCode
	{
		None = 0,
		
		InvalidRequest = 0x0001, // The request is missing a required parameter, includes an invalid parameter value, includes a parameter more than once, or is otherwise malformed.
		UnauthorizedClient = 0x0002, // The client is not authorized to request an authorization code using this method.
		AccessDenied = 0x0003, // The resource owner or authorization server denied the request.
		UnsupportedResponseType = 0x0004, // The authorization server does not support obtaining an authorization code using this method.
		InvalidScope = 0x0005, // The requested scope is invalid, unknown, or malformed.
		ServerError = 0x0006, // The authorization server encountered an unexpected condition that prevented it from fulfilling the request. (This error code is needed because a 500 Internal Server Error HTTP status code cannot be returned to the client via an HTTP redirect.)
		TemporarilyUnavailable = 0x0007, // The authorization server is currently unable to handle the request due to a temporary overloading or maintenance of the server. (This error code is needed because a 503 Service Unavailable HTTP status code cannot be returned to the client via an HTTP redirect.)
		
		InvalidClient = 0x0011, // Client authentication failed (e.g., unknown client, no client authentication included, or unsupported authentication method).
		InvalidGrant = 0x0012, // The provided authorization grant (e.g., authorization code, resource owner credentials) or refresh token is invalid, expired, revoked, does not match the redirection URI used in the authorization request, or was issued to another client.
		UnsupportedGrantType = 0x0013 // The authorization grant type is not supported by the authorization server.
	};
	
	class SLIB_EXPORT OAuthAccessToken
	{
	public:
		String token;
		String refreshToken;
		String tokenType;
		List<String> scopes;
		Time expirationTime;
		Time refreshTime;
		
	public:
		OAuthAccessToken();
		
		OAuthAccessToken(const String& token);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthAccessToken)
		
		SLIB_DECLARE_JSON
		
	public:
		sl_bool isValid() const;
		
		sl_bool isValid(const List<String>& requiredScopes) const;
		
		void setResponse(const HashMap<String, String>& params);
		
	};
	
	class SLIB_EXPORT OAuthAuthorizationRequestParam
	{
	public:
		OAuthGrantType grantType;
		List<String> scopes;
		String state;
		String redirectUri; // If empty, uses instance's `redirectUri` attribute
		HashMap<String, Variant> customParameters;
		
	public:
		OAuthAuthorizationRequestParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthAuthorizationRequestParam)
		
	};
	
	class SLIB_EXPORT OAuthResult
	{
	public:
		sl_bool flagSuccess;

		String error;
		String errorDescription;
		String errorUri;
		OAuthErrorCode errorCode;
		
	public:
		OAuthResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthResult)
		
	public:
		void setResponse(const HashMap<String, String>& params);
		
	};
	
	class SLIB_EXPORT OAuthAccessTokenResult : public OAuthResult
	{
	public:
		OAuthAccessToken accessToken;

		Json response;
		
	public:
		OAuthAccessTokenResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthAccessTokenResult)

	public:
		void setResponse(const Json& response);

	};
	
	class SLIB_EXPORT OAuthLoginResult : public OAuthAccessTokenResult
	{
	public:
		sl_bool flagCancel;
		sl_bool flagCache; // Used in `Token` Grant Type. True if the `access token` is from cache
		
		String code; // Used in `Code` Grant Type
		String state;
		
	public:
		OAuthLoginResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthLoginResult)
		
	public:
		void parseRedirectUrl(const String& url);
		
	};
	
	class SLIB_EXPORT OAuthLoginParam
	{
	public:
		String url; // Used in process of `Code` grant type
		
		OAuthAuthorizationRequestParam authorization; // `Code` grant type should not be used in unconfidental clients (such as Mobile/Desktop app)
		
		OAuthWebRedirectDialogOptions dialogOptions;
		Ptr<OAuthWebRedirectDialog> dialog;

		sl_bool flagIgnoreExistingAccessToken; // Ignored if `url` is not empty
		String loginRedirectUri; // If empty, uses `authorization`'s `redirectUri`
		
		Function<void(OAuthLoginResult&)> onComplete;
		
	public:
		OAuthLoginParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthLoginParam)
		
	};
	
	class SLIB_EXPORT OAuthParam
	{
	public:
		String clientId;
		String clientSecret;
		
		OAuthAccessToken accessToken;
		
		String authorizeUrl;
		HttpMethod accessTokenMethod;
		sl_bool flagUseBasicAuthorizationForAccessToken;
		String accessTokenUrl;
		String redirectUri;
		String loginRedirectUri;
		List<String> defaultScopes;
		sl_bool flagSupportTokenGrantType;
		
		String preferenceName;
		
		sl_bool flagLoggingErrors;
		
	public:
		OAuthParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthParam)
		
	};
	
	class SLIB_EXPORT OAuth2 : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuth2(const OAuthParam& param);
		
		~OAuth2();
		
	public:
		Ptr<OAuthAccessToken> getAccessToken();
		
		void setAccessToken(const OAuthAccessToken& accessToken);
		
		void setAccessToken(const String& accessToken);
		
		void clearAccessToken();
		
		String getAccessTokenKey();
		
		List<String> getAccessTokenScopes();
		
		Json getAccessTokenAttribute(const String& key);

		void setLoggingErrors(sl_bool flag);

		
		virtual void authorizeRequest(UrlRequestParam& param, const OAuthAccessToken& token);
		
		void authorizeRequest(UrlRequestParam& param);
		
		String getLoginUrl(const OAuthAuthorizationRequestParam& param);

		String getLoginUrl(OAuthGrantType grantType, const List<String>& scopes = List<String>::null(), const String& state = String::null());

		String getLoginUrl(const List<String>& scopes = List<String>::null(), const String& state = String::null());

		void requestAccessToken(HashMap<String, Variant>& params, const Function<void(OAuthAccessTokenResult&)>& onComplete);
		
		// grant_type=client_credentials
		void requestAccessToken(const Function<void(OAuthAccessTokenResult&)>& onComplete);

		// grant_type=authorization_code
		void requestAccessToken(const String& code, const String& redirectUri, const Function<void(OAuthAccessTokenResult&)>& onComplete);
		
		// grant_type=authorization_code
		void requestAccessToken(const String& code, const Function<void(OAuthAccessTokenResult&)>& onComplete);

		// grant_type=refresh_token
		void requestRefreshToken(const String& refreshToken, const List<String>& scopes, const Function<void(OAuthAccessTokenResult&)>& onComplete);

		// grant_type=refresh_token
		void requestRefreshToken(const String& refreshToken, const Function<void(OAuthAccessTokenResult&)>& onComplete);
		
		void login(const OAuthLoginParam& param);
		
		void login(const Function<void(OAuthLoginResult& result)>& onComplete);
		
	protected:
		void logUrlRequestError(UrlRequest* request);
		
		void save();
		
		void restore();
		
	public:
		String m_clientId;
		String m_clientSecret;
		String m_preferenceName;
		
		AtomicPtr<OAuthAccessToken> m_accessToken;

		String m_authorizeUrl;
		String m_accessTokenUrl;
		HttpMethod m_accessTokenMethod;
		sl_bool m_flagUseBasicAuthorizationForAccessToken;
		String m_redirectUri;
		String m_loginRedirectUri;
		List<String> m_defaultScopes;
		sl_bool m_flagSupportTokenGrantType;
		
		sl_bool m_flagLogErrors;
		
	};
	
	class SLIB_EXPORT OAuthApiResult
	{
	public:
		sl_bool flagSuccess;
		UrlRequest* request;
		Json response;
		
	public:
		OAuthApiResult(UrlRequest*);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthApiResult)
		
	};
	
}

#endif
