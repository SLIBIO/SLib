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

#include "slib/social/oauth_server.h"

#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace oauth_server
		{

			SLIB_STATIC_STRING(g_string_space, " ")

			SLIB_STATIC_STRING(g_field_tokenType, "t")
			SLIB_STATIC_STRING(g_field_clientId, "cid")
			SLIB_STATIC_STRING(g_field_scope, "scp")
			SLIB_STATIC_STRING(g_field_user, "usr")
			SLIB_STATIC_STRING(g_field_redirectUri, "dir")
			SLIB_STATIC_STRING(g_field_codeChallenge, "cc")
			SLIB_STATIC_STRING(g_field_codeChallengeMode, "cm")

			SLIB_STATIC_STRING(g_tokenType_access, "access")
			SLIB_STATIC_STRING(g_tokenType_refresh, "refresh")
			SLIB_STATIC_STRING(g_tokenType_code, "code")

		}
	}

	using namespace priv::oauth_server;

	SLIB_DEFINE_OBJECT(OAuthClientEntity, Object)

	OAuthClientEntity::OAuthClientEntity()
	{
	}

	OAuthClientEntity::~OAuthClientEntity()
	{
	}

	sl_bool OAuthClientEntity::validateSecret(const String& clientSecret)
	{
		return sl_true;
	}

	sl_bool OAuthClientEntity::validateRedirectUri(String& redirectUri)
	{
		return sl_true;
	}

	sl_bool OAuthClientEntity::validateScopes(List<String>& scopes)
	{
		return sl_true;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(OAuthTokenPayload)

	OAuthTokenPayload::OAuthTokenPayload()
	{
		grantType = OAuthGrantType::None;
		accessTokenExpirationTime.setZero();
		refreshTokenExpirationTime.setZero();
		authorizationCodeExpirationTime.setZero();
		codeChallengeMethod = OAuthCodeChallengeMethod::Plain;
	}

	SLIB_DEFINE_OBJECT(OAuthTokenRepository, Object)

	OAuthTokenRepository::OAuthTokenRepository()
	{
	}

	OAuthTokenRepository::~OAuthTokenRepository()
	{
	}

	SLIB_DEFINE_OBJECT(OAuthTokenMemoryRepository, OAuthTokenRepository)

	OAuthTokenMemoryRepository::OAuthTokenMemoryRepository()
	{
	}

	OAuthTokenMemoryRepository::~OAuthTokenMemoryRepository()
	{
	}

	void OAuthTokenMemoryRepository::registerToken(const String& code, const Json& data)
	{
		m_repo.put(code, data);
	}

	void OAuthTokenMemoryRepository::revokeToken(const String& code)
	{
		m_repo.remove(code);
	}

	sl_bool OAuthTokenMemoryRepository::isValid(const String& code)
	{
		return m_repo.find(code);
	}

	Json OAuthTokenMemoryRepository::getTokenData(const String& code)
	{
		return m_repo.getValue(code);
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(OAuthServerAuthorizationRequest)

	OAuthServerAuthorizationRequest::OAuthServerAuthorizationRequest()
	{
	}

	SLIB_DEFINE_OBJECT(OAuthServer, Object)
	
	OAuthServer::OAuthServer()
	{
		setSupportImplicitGrant(sl_true);
		setSupportAuthorizationCodeGrant(sl_true);
		setSupportClientCredentialsGrant(sl_true);
		setSupportPasswordGrant(sl_true);
		setSupportRefreshToken(sl_true);

		setAccessTokenExpirySeconds(50000);
		setRefreshTokenExpirySeconds(1000000);
		setAuthorizationCodeExpirySeconds(3600);
	}

	OAuthServer::~OAuthServer()
	{
	}

	sl_bool OAuthServer::validateAuthorizationRequest(HttpServerContext* context, OAuthServerAuthorizationRequest& request)
	{
		String strResponseType = context->getParameter("response_type");
		sl_bool flagResponseTypeError = sl_false;
		if (strResponseType == "token") {
			request.responseType = OAuthResponseType::Token;
		} else if (strResponseType == "code") {
			request.responseType = OAuthResponseType::Code;
		} else {
			flagResponseTypeError = sl_true;
		}
		request.redirectUri = context->getParameter("redirect_uri");
		request.clientId = context->getParameter("client_id");
		request.scopes = context->getParameter("scope").split(" ");
		request.state = context->getParameter("state");
		
		request.codeChallenge = context->getParameter("code_challenge");
		String strCodeChallengeMethod = context->getParameter("code_challenge_method");
		sl_bool flagCodeChallengeMethodError = sl_false;
		if (strCodeChallengeMethod.isEmpty() || strCodeChallengeMethod == "plain") {
			request.codeChallengeMethod = OAuthCodeChallengeMethod::Plain;
		} else if (strCodeChallengeMethod == "S256") {
			request.codeChallengeMethod = OAuthCodeChallengeMethod::S256;
		} else {
			flagCodeChallengeMethodError = sl_true;
		}
		
		if (request.clientId.isEmpty()) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidRequest, "client_id is not found");
			return sl_false;
		}
		if (flagResponseTypeError) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidRequest, "response_type is invalid");
			return sl_false;
		}
		if (flagCodeChallengeMethodError) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidRequest, "code_challenge_method is invalid");
			return sl_false;
		}
		if (request.codeChallenge.isNotEmpty()) {
			if (!(OAuth2::checkCodeVerifier(request.codeChallenge))) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidRequest, "Code challenge must follow the specifications of RFC-7636.");
				return sl_false;
			}
		}
		
		if (request.responseType == OAuthResponseType::Code) {
			if (!(isSupportAuthorizationCodeGrant())) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::UnsupportedGrantType);
				return sl_false;
			}
		} else {
			if (!(isSupportImplicitGrant())) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::UnsupportedGrantType);
				return sl_false;
			}
		}
		
		request.client = getClientEntity(request.clientId);
		OAuthClientEntity* client = request.client.get();
		if (!client) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidClient, "Client entity is not found");
			return sl_false;
		}
		
		if (!(validateRedirectUri(client, request.redirectUri))) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidClient, "redirect_uri is not authorized for the client entity");
			return sl_false;
		}
		if (!(validateScopes(client, request.scopes))) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidScope);
			return sl_false;
		}
		
		return sl_true;
	}

	void OAuthServer::completeAuthorizationRequest(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, const Json& userEntity)
	{
		if (request.responseType == OAuthResponseType::Token) {

			OAuthTokenPayload payload;
			payload.grantType = OAuthGrantType::Implicit;
			payload.client = request.client;
			payload.clientId = request.clientId;
			payload.user = userEntity;
			payload.scopes = request.scopes;

			setExpirySeconds(payload);
			issueAccessToken(payload);

			if (payload.accessToken.isEmpty()) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::ServerError, "Failed to generate access token");
				return;
			}

			registerAccessToken(payload);
			if (payload.refreshToken.isNotEmpty()) {
				registerRefreshToken(payload);
			}

			HashMap<String, String> params = generateAccessTokenResponseParams(payload);
			if (request.state.isNotEmpty()) {
				params.add_NoLock("state", request.state);
			}
			context->setResponseRedirect(request.redirectUri, params);

		} else if (request.responseType == OAuthResponseType::Code) {

			OAuthTokenPayload payload;
			payload.client = request.client;
			payload.clientId = request.clientId;
			payload.user = userEntity;
			payload.scopes = request.scopes;
			payload.redirectUri = request.redirectUri;
			payload.codeChallenge = request.codeChallenge;
			payload.codeChallengeMethod = request.codeChallengeMethod;

			setExpirySeconds(payload);
			issueAuthorizationCode(payload);
			
			if (payload.authorizationCode.isEmpty()) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::ServerError, "Failed to generate authorization code");
				return;
			}

			registerAuthorizationCode(payload);

			CHashMap<String, String> params;
			params.add_NoLock("code", payload.authorizationCode);
			if (request.state.isNotEmpty()) {
				params.add_NoLock("state", request.state);
			}
			context->setResponseRedirect(request.redirectUri, params);
		}
	}

	void OAuthServer::completeAuthorizationRequestWithError(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, OAuthErrorCode err, const String& errorDescription, const String& errorUri)
	{
		String redirectUri = getRedirectUri(request);
		if (request.redirectUri.isNotEmpty()) {
			CHashMap<String, String> params;
			params.add_NoLock("error", getErrorCodeText(err));
			if (errorDescription.isNotEmpty()) {
				params.add_NoLock("error_description", errorDescription);
			}
			if (errorUri.isNotEmpty()) {
				params.add_NoLock("error_uri", errorUri);
			}
			if (request.state.isNotEmpty()) {
				params.add_NoLock("state", request.state);
			}
			context->setResponseRedirect(request.redirectUri, params);
		} else {
			respondError(context, err, errorDescription, errorUri, request.state);
		}
	}

	void OAuthServer::respondToAccessTokenRequest(HttpServerContext* context)
	{
		String grantType = context->getParameter("grant_type");
		String clientId = getParameter(context, "client_id");
		String clientSecret = getParameter(context, "client_secret");
		List<String> scopes = context->getParameter("scope").split(" ");
		
		if (clientId.isEmpty()) {
			respondError(context, OAuthErrorCode::InvalidRequest, "client_id is not found");
			return;
		}
		Ref<OAuthClientEntity> refClient = getClientEntity(clientId);
		OAuthClientEntity* client = refClient.get();
		if (!client) {
			respondError(context, OAuthErrorCode::InvalidClient, "Client entity is not found");
			return;
		}
		if (clientSecret.isEmpty() || !(validateClientSecret(client, clientSecret))) {
			respondError(context, OAuthErrorCode::InvalidClient, "Client authentication failed");
			return;
		}
		if (!(validateScopes(client, scopes))) {
			respondError(context, OAuthErrorCode::InvalidScope);
			return;
		}

		OAuthTokenPayload payload;
		payload.client = client;
		payload.clientId = clientId;

		if (grantType == "authorization_code") {

			if (!(isSupportAuthorizationCodeGrant())) {
				respondError(context, OAuthErrorCode::UnsupportedGrantType);
				return;
			}

			String code = context->getParameter("code");
			if (code.isEmpty()) {
				respondError(context, OAuthErrorCode::InvalidRequest, "Authorization code is not found");
				return;
			}

			Ref<OAuthTokenRepository> repo = getAuthorizationCodeRepository();
			if (repo.isNotNull()) {
				if (!(repo->isValid(code))) {
					respondError(context, OAuthErrorCode::InvalidRequest, "Authorization code is revoked or invalid");
					return;
				}
			}

			payload.grantType = OAuthGrantType::AuthorizationCode;
			payload.authorizationCode = code;
			
			if (!(getAuthorizationCodePayload(payload))) {
				respondError(context, OAuthErrorCode::InvalidRequest, "Authorization code is invalid");
				return;
			}

			if (payload.authorizationCodeExpirationTime.isNotZero() && payload.authorizationCodeExpirationTime < Time::now()) {
				respondError(context, OAuthErrorCode::InvalidRequest, "Authorization code has expired");
				revokeAuthorizationCode(payload);
				return;
			}
			if (payload.clientId != clientId) {
				respondError(context, OAuthErrorCode::InvalidRequest, "Authorization code was not issued to this client");
				return;
			}
			if (payload.redirectUri != context->getParameter("redirect_uri")) {
				respondError(context, OAuthErrorCode::InvalidRequest, "redirect_uri is invalid");
				return;
			}
			ListElements<String> requiredScopes(scopes);
			for (sl_size i = 0; i < requiredScopes.count; i++) {
				if (!(payload.scopes.contains(requiredScopes[i]))) {
					respondError(context, OAuthErrorCode::InvalidScope);
					return;
				}
			}
			
			String codeVerifier = context->getParameter("code_verifier");
			if (payload.codeChallenge.isNotEmpty() || codeVerifier.isNotEmpty()) {
				if (codeVerifier.isEmpty()) {
					respondError(context, OAuthErrorCode::InvalidRequest, "code_verifier is not found");
					return;
				}
				if (payload.codeChallenge.isEmpty()) {
					respondError(context, OAuthErrorCode::InvalidRequest, "Failed to verify code_verifier");
					return;
				}
				if (!(OAuth2::checkCodeVerifier(codeVerifier))) {
					respondError(context, OAuthErrorCode::InvalidRequest, "code_verifier must follow the specifications of RFC-7636.");
					return;
				}
				if (payload.codeChallenge != OAuth2::generateCodeChallenge(codeVerifier, payload.codeChallengeMethod)) {
					respondError(context, OAuthErrorCode::InvalidRequest, "Failed to verify code_verifier");
					return;
				}

			}

		} else if (grantType == "client_credentials") {			

			if (!(isSupportClientCredentialsGrant())) {
				respondError(context, OAuthErrorCode::UnsupportedGrantType);
				return;
			}
			payload.grantType = OAuthGrantType::ClientCredentials;
			if (!(onClientCredentialsGrant(payload))) {
				respondError(context, OAuthErrorCode::AccessDenied);
				return;
			}

		} else if (grantType == "password") {

			if (!(isSupportPasswordGrant())) {
				respondError(context, OAuthErrorCode::UnsupportedGrantType);
				return;
			}
			payload.grantType = OAuthGrantType::Password;
			String username = context->getParameter("username");
			if (username.isEmpty()) {
				respondError(context, OAuthErrorCode::InvalidRequest, "username is not found");
				return;
			}
			String password = context->getParameter("password");
			if (password.isEmpty()) {
				respondError(context, OAuthErrorCode::InvalidRequest, "password is not found");
				return;
			}
			if (!(onPasswordGrant(username, password, payload))) {
				respondError(context, OAuthErrorCode::AccessDenied, "username or password is not match");
				return;
			}

		} else if (grantType == "refresh_token") {
			
			if (!(isSupportRefreshToken())) {
				respondError(context, OAuthErrorCode::UnsupportedGrantType);
				return;
			}

			payload.grantType = OAuthGrantType::RefreshToken;
			payload.refreshToken = context->getParameter("refresh_token");
			if (payload.refreshToken.isEmpty()) {
				respondError(context, OAuthErrorCode::InvalidRequest, "refresh_token is not found");
				return;
			}

			Ref<OAuthTokenRepository> repo = getRefreshTokenRepository();
			if (repo.isNotNull()) {
				if (!(repo->isValid(payload.refreshToken))) {
					respondError(context, OAuthErrorCode::InvalidRequest, "refresh_token is revoked or invalid");
					return;
				}
			}

			if (!(getRefreshTokenPayload(payload))) {
				respondError(context, OAuthErrorCode::InvalidRequest, "refresh_token is invalid");
				return;
			}

			if (payload.refreshTokenExpirationTime.isNotZero() && payload.refreshTokenExpirationTime < Time::now()) {
				respondError(context, OAuthErrorCode::InvalidRequest, "refresh_token has expired");
				revokeRefreshToken(payload);
				return;
			}
			if (payload.clientId != clientId) {
				respondError(context, OAuthErrorCode::InvalidRequest, "refresh_token was not issued to this client");
				return;
			}
			ListElements<String> requiredScopes(scopes);
			for (sl_size i = 0; i < requiredScopes.count; i++) {
				if (!(payload.scopes.contains(requiredScopes[i]))) {
					respondError(context, OAuthErrorCode::InvalidScope);
					return;
				}
			}

		} else {
			respondError(context, OAuthErrorCode::InvalidGrant);
			return;
		}

		payload.scopes = scopes;

		setExpirySeconds(payload);
		issueAccessToken(payload);

		if (payload.accessToken.isEmpty()) {
			respondError(context, OAuthErrorCode::ServerError, "Failed to generate access token");
			return;
		}

		registerAccessToken(payload);
		if (payload.refreshToken.isNotEmpty()) {
			registerRefreshToken(payload);
		}

		HashMap<String, String> params = generateAccessTokenResponseParams(payload);
		context->write(Json(params).toJsonString());
	}

	sl_bool OAuthServer::validateAccessToken(HttpServerContext* context, OAuthTokenPayload& payload)
	{
		String token = getAccessToken(context);
		if (token.isNotEmpty()) {
			Ref<OAuthTokenRepository> repo = getAccessTokenRepository();
			if (repo.isNotNull()) {
				if (!(repo->isValid(token))) {
					return sl_false;
				}
			}
			payload.accessToken = token;
			if (getAccessTokenPayload(payload)) {
				if (payload.accessTokenExpirationTime.isNotZero() && payload.accessTokenExpirationTime < Time::now()) {
					revokeAccessToken(payload);
					return sl_false;
				}
				return sl_true;
			}
		}
		return sl_false;
	}

	void OAuthServer::respondError(HttpServerContext* context, OAuthErrorCode err, const String& errorDescription, const String& errorUri, const String& state)
	{
		HttpStatus status = HttpStatus::BadRequest;
		if (err == OAuthErrorCode::InvalidClient || err == OAuthErrorCode::UnauthorizedClient || err == OAuthErrorCode::AccessDenied) {
			status = HttpStatus::Unauthorized;
		} else if (err == OAuthErrorCode::ServerError) {
			status = HttpStatus::InternalServerError;
		} else if (err == OAuthErrorCode::TemporarilyUnavailable) {
			status = HttpStatus::ServiceUnavailable;
		}
		
		context->setResponseCode(status);
		context->setResponseContentType(ContentType::Json);

		Json json;
		json.putItem("error", getErrorCodeText(err));
		if (errorDescription.isNotEmpty()) {
			json.putItem("error_description", errorDescription);
		}
		if (errorUri.isNotEmpty()) {
			json.putItem("error_uri", errorUri);
		}
		if (state.isNotEmpty()) {
			json.putItem("state", state);
		}
		context->write(json.toJsonString());
	}

	Ref<OAuthClientEntity> OAuthServer::getClientEntity(const String& clientId)
	{
		Ref<OAuthClientEntity> client = new OAuthClientEntity;
		if (client.isNotNull()) {
			client->clientId = clientId;
		}
		return client;
	}

	sl_bool OAuthServer::validateClientSecret(OAuthClientEntity* client, const String& clientSecret)
	{
		return client->validateSecret(clientSecret);
	}

	sl_bool OAuthServer::validateRedirectUri(OAuthClientEntity* client, String& redirectUri)
	{
		return client->validateRedirectUri(redirectUri);
	}

	sl_bool OAuthServer::validateScopes(OAuthClientEntity* client, List<String>& scopes)
	{
		return client->validateScopes(scopes);
	}

	void OAuthServer::registerAccessToken(OAuthTokenPayload& payload)
	{
		if (payload.accessToken.isEmpty()) {
			return;
		}
		Ref<OAuthTokenRepository> repo = getAccessTokenRepository();
		if (repo.isNotNull()) {
			repo->registerToken(payload.accessToken, sl_null);
		}
	}

	void OAuthServer::revokeAccessToken(OAuthTokenPayload& payload)
	{
		if (payload.accessToken.isEmpty()) {
			return;
		}
		Ref<OAuthTokenRepository> repo = getAccessTokenRepository();
		if (repo.isNotNull()) {
			repo->revokeToken(payload.accessToken);
		}
	}

	void OAuthServer::registerRefreshToken(OAuthTokenPayload& payload)
	{
		if (payload.refreshToken.isEmpty()) {
			return;
		}
		Ref<OAuthTokenRepository> repo = getRefreshTokenRepository();
		if (repo.isNotNull()) {
			repo->registerToken(payload.refreshToken, sl_null);
		}
	}

	void OAuthServer::revokeRefreshToken(OAuthTokenPayload& payload)
	{
		if (payload.refreshToken.isEmpty()) {
			return;
		}
		Ref<OAuthTokenRepository> repo = getRefreshTokenRepository();
		if (repo.isNotNull()) {
			repo->revokeToken(payload.refreshToken);
		}
	}

	void OAuthServer::registerAuthorizationCode(OAuthTokenPayload& payload)
	{
		if (payload.authorizationCode.isEmpty()) {
			return;
		}
		Ref<OAuthTokenRepository> repo = getAuthorizationCodeRepository();
		if (repo.isNotNull()) {
			repo->registerToken(payload.authorizationCode, sl_null);
		}
	}

	void OAuthServer::revokeAuthorizationCode(OAuthTokenPayload& payload)
	{
		if (payload.authorizationCode.isEmpty()) {
			return;
		}
		Ref<OAuthTokenRepository> repo = getAuthorizationCodeRepository();
		if (repo.isNotNull()) {
			repo->revokeToken(payload.authorizationCode);
		}
	}

	sl_bool OAuthServer::onClientCredentialsGrant(OAuthTokenPayload& payload)
	{
		return sl_false;
	}

	sl_bool OAuthServer::onPasswordGrant(const String& username, const String& password, OAuthTokenPayload& payload)
	{
		return sl_false;
	}

	String OAuthServer::getRedirectUri(const OAuthServerAuthorizationRequest& request)
	{
		if (request.redirectUri.isNotEmpty()) {
			return request.redirectUri;
		} else {
			return getDefaultRedirectUri();
		}
	}

	void OAuthServer::setExpirySeconds(OAuthTokenPayload& payload)
	{
		payload.accessTokenExpirationTime = getExpiryTime(getAccessTokenExpirySeconds());
		payload.refreshTokenExpirationTime = getExpiryTime(getRefreshTokenExpirySeconds());
		payload.authorizationCodeExpirationTime = getExpiryTime(getAuthorizationCodeExpirySeconds());
	}

	HashMap<String, String> OAuthServer::generateAccessTokenResponseParams(const OAuthTokenPayload& payload)
	{
		HashMap<String, String> params;
		params.add_NoLock("access_token", payload.accessToken);
		params.add_NoLock("token_type", "Bearer");
		sl_int64 exp = (payload.accessTokenExpirationTime - Time::now()).getSecondsCount();
		if (exp < 0) {
			exp = 0;
		}
		params.add_NoLock("expires_in", String::fromInt64(exp));
		if (payload.refreshToken.isNotEmpty()) {
			params.add_NoLock("refresh_token", payload.refreshToken);
		}
		params.add_NoLock("scope", StringBuffer::join(" ", payload.scopes));
		return Move(params);
	}

	String OAuthServer::getErrorCodeText(OAuthErrorCode err)
	{
		switch (err) {
		case OAuthErrorCode::InvalidRequest:
			return "invalid_request";
		case OAuthErrorCode::UnauthorizedClient:
			return "unauthorized_client";
		case OAuthErrorCode::AccessDenied:
			return "access_denied";
		case OAuthErrorCode::UnsupportedResponseType:
			return "unsupported_response_type";
		case OAuthErrorCode::InvalidScope:
			return "invalid_scope";
		case OAuthErrorCode::ServerError:
			return "server_error";
		case OAuthErrorCode::TemporarilyUnavailable:
			return "temporarily_unavailable";
		case OAuthErrorCode::InvalidClient:
			return "invalid_client";
		case OAuthErrorCode::InvalidGrant:
			return "invalid_grant";
		case OAuthErrorCode::UnsupportedGrantType:
			return "unsupported_grant_type";
		default:
			return "unknown";
		}
	}

	String OAuthServer::getParameter(HttpServerContext* context, const String& name)
	{
		if (context->containsRequestHeader(name)) {
			return context->getRequestHeader(name);
		}
		return context->getParameter(name);
	}

	String OAuthServer::getAccessToken(HttpServerContext* context)
	{
		String auth = getParameter(context, "Authorization");
		if (auth.startsWith("Bearer ")) {
			return auth.substring(7);
		}
		return sl_null;
	}

	Time OAuthServer::getExpiryTime(sl_uint32 seconds)
	{
		if (seconds) {
			return Time::now() + Time::withSeconds(seconds);
		} else {
			return Time::zero();
		}
	}


	SLIB_DEFINE_OBJECT(OAuthServerWithJwt, OAuthServer)

	OAuthServerWithJwt::OAuthServerWithJwt()
	{
		setAlgorithm(JwtAlgorithm::HS256);
	}

	OAuthServerWithJwt::~OAuthServerWithJwt()
	{
	}

	Memory& OAuthServerWithJwt::getMasterKey()
	{
		return m_masterKey;
	}

	void OAuthServerWithJwt::setMasterKey(const Memory& key)
	{
		m_masterKey = key;
	}

	void OAuthServerWithJwt::setMasterKey(const void* key, sl_size len)
	{
		m_masterKey = Memory::create(key, len);
	}

	void OAuthServerWithJwt::issueAccessToken(OAuthTokenPayload& payload)
	{
		payload.accessToken = generateToken(TokenType::Access, payload);
		if (isSupportRefreshToken()) {
			payload.refreshToken = generateToken(TokenType::Refresh, payload);
		}
	}

	sl_bool OAuthServerWithJwt::getAccessTokenPayload(OAuthTokenPayload& payload)
	{
		return parseToken(payload.accessToken, payload) == TokenType::Access;
	}

	sl_bool OAuthServerWithJwt::getRefreshTokenPayload(OAuthTokenPayload& payload)
	{
		return parseToken(payload.refreshToken, payload) == TokenType::Refresh;
	}

	void OAuthServerWithJwt::issueAuthorizationCode(OAuthTokenPayload& payload)
	{
		payload.authorizationCode = generateToken(TokenType::AuthorizationCode, payload);
	}

	sl_bool OAuthServerWithJwt::getAuthorizationCodePayload(OAuthTokenPayload& payload)
	{
		return parseToken(payload.authorizationCode, payload) == TokenType::AuthorizationCode;
	}

	String OAuthServerWithJwt::generateToken(OAuthServerWithJwt::TokenType type, OAuthTokenPayload& payload)
	{
		Jwt jwt;
		jwt.setAlgorithm(getAlgorithm());
		if (type == TokenType::Refresh) {
			jwt.payload.putItem(g_field_tokenType, g_tokenType_refresh);
			jwt.setExpirationTime(payload.refreshTokenExpirationTime);
		} else if (type == TokenType::AuthorizationCode) {
			jwt.payload.putItem(g_field_tokenType, g_tokenType_code);
			jwt.setExpirationTime(payload.authorizationCodeExpirationTime);
			if (payload.codeChallenge.isNotEmpty()) {
				jwt.payload.putItem(g_field_codeChallenge, payload.codeChallenge);
				jwt.payload.putItem(g_field_codeChallengeMode, (int)(payload.codeChallengeMethod));
			}
			if (payload.redirectUri.isNotEmpty()) {
				jwt.payload.putItem(g_field_redirectUri, payload.redirectUri);
			}
		} else {
			jwt.payload.putItem(g_field_tokenType, g_tokenType_access);
			jwt.setExpirationTime(payload.accessTokenExpirationTime);
		}
		jwt.payload.putItem(g_field_clientId, payload.clientId);
		jwt.payload.putItem(g_field_user, payload.user);
		if (payload.scopes.isNotEmpty()) {
			jwt.payload.putItem(g_field_scope, StringBuffer::join(g_string_space, payload.scopes));
		}
		return encrypt(jwt);
	}

	OAuthServerWithJwt::TokenType OAuthServerWithJwt::parseToken(const String& token, OAuthTokenPayload& payload)
	{
		Jwt jwt;
		if (decrypt(token, jwt)) {
			String strType = jwt.payload.getItem(g_field_tokenType).getString();
			payload.clientId = jwt.payload.getItem(g_field_clientId).getString();
			payload.user = jwt.payload.getItem(g_field_user);
			payload.scopes = jwt.payload.getItem(g_field_scope).getString().split(g_string_space);
			if (strType == g_tokenType_access) {
				payload.accessTokenExpirationTime = jwt.getExpirationTime();
				return TokenType::Access;
			} else if (strType == g_tokenType_refresh) {
				payload.refreshTokenExpirationTime = jwt.getExpirationTime();
				return TokenType::Refresh;
			} else if (strType == g_tokenType_code) {
				payload.authorizationCodeExpirationTime = jwt.getExpirationTime();
				payload.redirectUri = jwt.payload.getItem(g_field_redirectUri).getString();
				payload.codeChallenge = jwt.payload.getItem(g_field_codeChallenge).getString();
				payload.codeChallengeMethod = (OAuthCodeChallengeMethod)(jwt.payload.getItem(g_field_codeChallengeMode).getUint32());
				return TokenType::AuthorizationCode;
			}
		}
		return TokenType::None;
	}

	String OAuthServerWithJwt::encrypt(const Jwt& jwt)
	{
		return jwt.encode(m_masterKey);
	}

	sl_bool OAuthServerWithJwt::decrypt(const String& str, Jwt& jwt)
	{
		return jwt.decode(m_masterKey, str);
	}

}
