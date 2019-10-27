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
		virtual sl_bool validateRedirectUri(String& redirectUri);
		
		virtual sl_bool validateScopes(List<String>& scopes);

	};

	class OAuthUserEntity
	{
	public:
		String userId;
		
	};

	class OAuthServerAuthorizationRequest : public OAuthAuthorizationRequestParam
	{
	public:
		Ref<OAuthClientEntity> client;
		
	};

	class OAuthServer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		OAuthServer();
		
		~OAuthServer();
		
	public:
		sl_bool isSupportTokenGrant();
		
		void setSupportTokenGrant(sl_bool flag);
		
		sl_bool isSupportCodeGrant();
		
		void setSupportCodeGrant(sl_bool flag);
		
		String getDefaultRedirectUri();
		
		void setDefaultRedirectUri(const String uri);
		
		String getRedirectUri(const OAuthServerAuthorizationRequest& request);
		
	public:
		sl_bool validateAuthorizationRequest(HttpServerContext* context, OAuthServerAuthorizationRequest& _out);
		
		void completeAuthorizationRequest(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, const OAuthUserEntity& userEntity);
		
		void completeAuthorizationRequestWithError(HttpServerContext* context, const OAuthServerAuthorizationRequest& request, OAuthErrorCode err, const String& errorDescription = String::null(), const String& errorUri = String::null());
		
		static String getAccessToken(HttpServerContext* context);
		
	public:
		virtual Ref<OAuthClientEntity> getClientEntity(const String& clientId);
		
		virtual sl_bool validateRedirectUri(OAuthClientEntity* client, String& redirectUri);
		
		virtual sl_bool validateScopes(OAuthClientEntity* client, List<String>& scopes);
		
		virtual void issueAccessToken(OAuthAccessToken& outToken, OAuthClientEntity* client, const OAuthUserEntity& user, const List<String>& scopes);
		
	public:
		SLIB_PROPERTY_FUNCTION(Ref<OAuthClientEntity>(const String& clientId), OnResolveClientEntity)
		SLIB_PROPERTY_FUNCTION(void(OAuthAccessToken& outToken, OAuthClientEntity* client, const OAuthUserEntity& user, const List<String>& scopes), OnIssueAccessToken)

	private:
		sl_bool m_flagSupportTokenGrant;
		sl_bool m_flagSupportCodeGrant;
		String m_defaultRedirectUri;
		
	};
	
}

#endif
