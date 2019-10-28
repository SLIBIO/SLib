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

	SLIB_DEFINE_OBJECT(OAuthClientEntity, Object)

	OAuthClientEntity::OAuthClientEntity()
	{
	}

	OAuthClientEntity::~OAuthClientEntity()
	{
	}

	sl_bool OAuthClientEntity::validateRedirectUri(String& redirectUri)
	{
		return sl_true;
	}

	sl_bool OAuthClientEntity::validateScopes(List<String>& scopes)
	{
		return sl_true;
	}

	SLIB_DEFINE_OBJECT(OAuthServer, Object)
	
	OAuthServer::OAuthServer()
	{
		m_flagSupportTokenGrant = sl_true;
		m_flagSupportCodeGrant = sl_true;
	}

	OAuthServer::~OAuthServer()
	{
	}

	sl_bool OAuthServer::isSupportTokenGrant()
	{
		return m_flagSupportTokenGrant;
	}

	void OAuthServer::setSupportTokenGrant(sl_bool flag)
	{
		m_flagSupportTokenGrant = flag;
	}

	sl_bool OAuthServer::isSupportCodeGrant()
	{
		return m_flagSupportCodeGrant;
	}

	void OAuthServer::setSupportCodeGrant(sl_bool flag)
	{
		m_flagSupportCodeGrant = flag;
	}

	String OAuthServer::getDefaultRedirectUri()
	{
		return m_defaultRedirectUri;
	}

	void OAuthServer::setDefaultRedirectUri(const String uri)
	{
		m_defaultRedirectUri = uri;
	}

	String OAuthServer::getRedirectUri(const OAuthServerAuthorizationRequest& request)
	{
		if (request.redirectUri.isNotEmpty()) {
			return request.redirectUri;
		} else {
			return m_defaultRedirectUri;
		}
	}

	sl_bool OAuthServer::validateAuthorizationRequest(HttpServerContext* context, OAuthServerAuthorizationRequest& request)
	{
		String strGrantType = context->getParameter("response_type");
		sl_bool flagGrantTypeError = sl_false;
		if (strGrantType == "token") {
			request.grantType = OAuthGrantType::Token;
		} else if (strGrantType == "code") {
			request.grantType = OAuthGrantType::Code;
		} else {
			flagGrantTypeError = sl_true;
		}
		request.redirectUri = context->getParameter("redirect_uri");
		request.clientId = context->getParameter("client_id");
		request.scopes = context->getParameter("scope").split(" ");
		request.state = context->getParameter("state");
		
		if (request.clientId.isEmpty()) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidRequest, "client_id is missing");
			return sl_false;
		}
		if (flagGrantTypeError) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidRequest, "response_type is invalid");
			return sl_false;
		}
		if (request.grantType == OAuthGrantType::Code) {
			if (!m_flagSupportTokenGrant) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::UnsupportedGrantType);
				return sl_false;
			}
		} else {
			if (!m_flagSupportCodeGrant) {
				completeAuthorizationRequestWithError(context, request, OAuthErrorCode::UnsupportedGrantType);
				return sl_false;
			}
		}
		
		request.client = getClientEntity(request.clientId);
		OAuthClientEntity* client = request.client.get();
		if (!client) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::InvalidClient, "Cannot find the client entity");
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

	void OAuthServer::completeAuthorizationRequest(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, const OAuthUserEntity& userEntity)
	{
		OAuthAccessToken token;
		token.tokenType = "Bearer";
		issueAccessToken(token, request.client.get(), userEntity, request.scopes);
		if (token.token.isEmpty()) {
			completeAuthorizationRequestWithError(context, request, OAuthErrorCode::ServerError, "Cannot generate access token");
			return;
		}
		CHashMap<String, String> params;
		params.add_NoLock("access_token", token.token);
		params.add_NoLock("token_type", token.tokenType);
		sl_int64 exp = (token.expirationTime - Time::now()).getSecondsCount();
		if (exp < 0) {
			exp = 0;
		}
		params.add_NoLock("expires_in", String::fromInt64(exp));
		if (token.refreshToken.isNotEmpty()) {
			params.add_NoLock("refresh_token", token.refreshToken);
		}
		if (request.state.isNotEmpty()) {
			params.add_NoLock("state", request.state);
		}
		params.add_NoLock("scope", StringBuffer::join(" ", request.scopes));
		context->setResponseRedirect(request.redirectUri, params);
	}

	void OAuthServer::completeAuthorizationRequestWithError(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, OAuthErrorCode err, const String& errorDescription, const String& errorUri)
	{
		String redirectUri = getRedirectUri(request);
		if (request.redirectUri.isNotEmpty()) {
			CHashMap<String, String> params;
			String strError;
			switch (err) {
				case OAuthErrorCode::InvalidRequest:
					strError = "invalid_request";
					break;
				case OAuthErrorCode::UnauthorizedClient:
					strError = "unauthorized_client";
					break;
				case OAuthErrorCode::AccessDenied:
					strError = "access_denied";
					break;
				case OAuthErrorCode::UnsupportedResponseType:
					strError = "unsupported_response_type";
					break;
				case OAuthErrorCode::InvalidScope:
					strError = "invalid_scope";
					break;
				case OAuthErrorCode::ServerError:
					strError = "server_error";
					break;
				case OAuthErrorCode::TemporarilyUnavailable:
					strError = "temporarily_unavailable";
					break;
				case OAuthErrorCode::InvalidClient:
					strError = "invalid_client";
					break;
				case OAuthErrorCode::InvalidGrant:
					strError = "invalid_grant";
				case OAuthErrorCode::UnsupportedGrantType:
					strError = "unsupported_grant_type";
				default:
					strError = "unknown";
					break;
			}
			params.add_NoLock("error", strError);
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
		if (auth.substring(0, 7).equalsIgnoreCase("Bearer ")) {
			return auth.substring(7);
		}
		return sl_null;
	}

	Ref<OAuthClientEntity> OAuthServer::getClientEntity(const String& clientId)
	{
		auto func = getOnResolveClientEntity();
		if (func.isNotNull()) {
			return func(clientId);
		}
		Ref<OAuthClientEntity> client = new OAuthClientEntity;
		if (client.isNotNull()) {
			client->clientId = clientId;
		}
		return client;
	}

	sl_bool OAuthServer::validateRedirectUri(OAuthClientEntity* client, String& redirectUri)
	{
		return client->validateRedirectUri(redirectUri);
	}

	sl_bool OAuthServer::validateScopes(OAuthClientEntity* client, List<String>& scopes)
	{
		return client->validateScopes(scopes);
	}

	void OAuthServer::issueAccessToken(OAuthAccessToken& outToken, OAuthClientEntity* client, const OAuthUserEntity& user, const List<String>& scopes)
	{
		auto func = getOnIssueAccessToken();
		if (func.isNotNull()) {
			func(outToken, client, user, scopes);
		}
	}

}
