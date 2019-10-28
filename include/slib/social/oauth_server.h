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

#ifndef CHECKHEADER_SLIB_SOCIAL_OAUTH_SERVER
#define CHECKHEADER_SLIB_SOCIAL_OAUTH_SERVER

#include "definition.h"

#include "oauth.h"

#include "../network/http_server.h"
#include "../crypto/jwt.h"

namespace slib
{

	class OAuthClientEntity : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuthClientEntity();
		
		~OAuthClientEntity();
		
	public:
		String clientId;
		
	public:
		virtual sl_bool validateSecret(const String& clientSecret);
		
		virtual sl_bool validateRedirectUri(String& redirectUri);
		
		virtual sl_bool validateScopes(List<String>& scopes);

	};

	class OAuthTokenPayload
	{
	public:
		OAuthGrantType grantType;
		Ref<OAuthClientEntity> client;
		String clientId;

		Json user;
		
		List<String> scopes;

		String accessToken;
		Time accessTokenExpirationTime;

		String refreshToken;
		Time refreshTokenExpirationTime;

		// payload for authorization code
		String redirectUri;
		String codeChallenge;
		OAuthCodeChallengeMethod codeChallengeMethod;

		String authorizationCode;
		Time authorizationCodeExpirationTime;

	public:
		OAuthTokenPayload();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthTokenPayload)
		
	};

	class OAuthTokenRepository : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuthTokenRepository();
		
		~OAuthTokenRepository();
		
	public:
		virtual void registerToken(const String& token, const Json& data) = 0;
		
		virtual void revokeToken(const String& token) = 0;
		
		virtual sl_bool isValid(const String& token) = 0;
		
		virtual Json getTokenData(const String& token) = 0;

	};

	class OAuthTokenMemoryRepository : public OAuthTokenRepository
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuthTokenMemoryRepository();
		
		~OAuthTokenMemoryRepository();
		
	public:
		void registerToken(const String& token, const Json& data) override;
		
		void revokeToken(const String& token) override;
		
		sl_bool isValid(const String& token) override;

		Json getTokenData(const String& token) override;
		
	protected:
		HashMap<String, Json> m_repo;
		
	};

	class OAuthServerAuthorizationRequest : public OAuthAuthorizationRequestParam
	{
	public:
		Ref<OAuthClientEntity> client;

	public:
		OAuthServerAuthorizationRequest();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OAuthServerAuthorizationRequest)

	};

	class OAuthServer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuthServer();
		
		~OAuthServer();
		
	public:
		sl_bool validateAuthorizationRequest(HttpServerContext* context, OAuthServerAuthorizationRequest& _out);
		
		void completeAuthorizationRequest(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, const Json& userEntity);
		
		void completeAuthorizationRequestWithError(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, OAuthErrorCode err, const String& errorDescription = String::null(), const String& errorUri = String::null());
		
		void respondToAccessTokenRequest(HttpServerContext* context);

		sl_bool validateAccessToken(HttpServerContext* context, OAuthTokenPayload& payload);

		static void respondError(HttpServerContext* context, HttpStatus status, OAuthErrorCode err, const String& errorDescription = String::null(), const String& errorUri = String::null(), const String& state = String::null());

	public:
		virtual Ref<OAuthClientEntity> getClientEntity(const String& clientId);
		
		virtual sl_bool validateClientSecret(OAuthClientEntity* client, const String& clientSecret);
		
		virtual sl_bool validateRedirectUri(OAuthClientEntity* client, String& redirectUri);
		
		virtual sl_bool validateScopes(OAuthClientEntity* client, List<String>& scopes);
		

		virtual void issueAccessToken(OAuthTokenPayload& payload) = 0;

		virtual sl_bool getAccessTokenPayload(OAuthTokenPayload& payload) = 0;

		virtual sl_bool getRefreshTokenPayload(OAuthTokenPayload& payload) = 0;

		virtual void issueAuthorizationCode(OAuthTokenPayload& payload) = 0;

		virtual sl_bool getAuthorizationCodePayload(OAuthTokenPayload& payload) = 0;


		virtual void registerAccessToken(OAuthTokenPayload& payload);

		virtual void revokeAccessToken(OAuthTokenPayload& payload);

		virtual void registerRefreshToken(OAuthTokenPayload& payload);

		virtual void revokeRefreshToken(OAuthTokenPayload& payload);

		virtual void registerAuthorizationCode(OAuthTokenPayload& payload);

		virtual void revokeAuthorizationCode(OAuthTokenPayload& payload);


		virtual sl_bool onClientCredentialsGrant(OAuthTokenPayload& payload);

		virtual sl_bool onPasswordGrant(const String& username, const String& password, OAuthTokenPayload& payload);
		
	public:
		String getRedirectUri(const OAuthServerAuthorizationRequest& request);

		void setExpirySeconds(OAuthTokenPayload& payload);

		static HashMap<String, String> generateAccessTokenResponseParams(const OAuthTokenPayload& payload);

		static String getErrorCodeText(OAuthErrorCode err);

		static String getParameter(HttpServerContext* context, const String& name);

		static String getAccessToken(HttpServerContext* context);

		static Time getExpiryTime(sl_uint32 seconds);

	public:
		SLIB_PROPERTY(AtomicString, DefaultRedirectUri)

		SLIB_BOOLEAN_PROPERTY(SupportImplicitGrant)
		SLIB_BOOLEAN_PROPERTY(SupportAuthorizationCodeGrant)
		SLIB_BOOLEAN_PROPERTY(SupportClientCredentialsGrant)
		SLIB_BOOLEAN_PROPERTY(SupportPasswordGrant)
		SLIB_BOOLEAN_PROPERTY(SupportRefreshToken)

		SLIB_PROPERTY(sl_uint32, AccessTokenExpirySeconds)
		SLIB_PROPERTY(sl_uint32, RefreshTokenExpirySeconds)
		SLIB_PROPERTY(sl_uint32, AuthorizationCodeExpirySeconds)

		SLIB_PROPERTY(AtomicRef<OAuthTokenRepository>, AccessTokenRepository)
		SLIB_PROPERTY(AtomicRef<OAuthTokenRepository>, AuthorizationCodeRepository)
		SLIB_PROPERTY(AtomicRef<OAuthTokenRepository>, RefreshTokenRepository)

	};
	
	class OAuthServerWithJwt : public OAuthServer
	{
		SLIB_DECLARE_OBJECT

	public:
		OAuthServerWithJwt();

		~OAuthServerWithJwt();

	public:
		void issueAccessToken(OAuthTokenPayload& payload) override;

		sl_bool getAccessTokenPayload(OAuthTokenPayload& payload) override;

		sl_bool getRefreshTokenPayload(OAuthTokenPayload& payload) override;

		void issueAuthorizationCode(OAuthTokenPayload& payload) override;

		sl_bool getAuthorizationCodePayload(OAuthTokenPayload& payload) override;

	protected:
		enum class TokenType {
			None, Access, Refresh, AuthorizationCode
		};

		String generateToken(TokenType type, OAuthTokenPayload& payload);

		TokenType parseToken(const String& token, OAuthTokenPayload& payload);

		virtual String encrypt(const Jwt& jwt);

		virtual sl_bool decrypt(const String& str, Jwt& jwt);

	public:
		SLIB_PROPERTY(AtomicMemory, MasterKey)
		SLIB_PROPERTY(JwtAlgorithm, Algorithm)

	};

}

#endif
