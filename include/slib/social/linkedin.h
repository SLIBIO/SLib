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

#ifndef CHECKHEADER_SLIB_SOCIAL_LINKEDIN
#define CHECKHEADER_SLIB_SOCIAL_LINKEDIN

#include "definition.h"

#include "oauth.h"

namespace slib
{
	
	class SLIB_EXPORT LinkedInUser
	{
	public:
		String id;
		String firstName;
		String lastName;
		
		Json json;
		
	public:
		LinkedInUser();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedInUser)
		
		SLIB_DECLARE_JSON
		
	public:
		static String getNameFromLocalized(const Json& localizedName);
		
	};
	
	typedef OAuthApiResult LinkedInResult;
	
	typedef OAuthLoginResult LinkedInLoginResult;
	
	class SLIB_EXPORT LinkedInLoginParam: public OAuthLoginParam
	{
	public:
		LinkedInLoginParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedInLoginParam)
		
	public:
		void addScopeForSharing();
		
	};
	
	class SLIB_EXPORT LinkedInResolveUserUrlParam
	{
	public:
		OAuthWebRedirectDialogOptions dialogOptions;
		Ptr<OAuthWebRedirectDialog> dialog;
		
		Function<void(const String& url)> onComplete;
		
	public:
		LinkedInResolveUserUrlParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedInResolveUserUrlParam)
		
	};
	
	class SLIB_EXPORT LinkedInShareResult : public LinkedInResult
	{
	public:
		LinkedInShareResult(UrlRequest* request);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedInShareResult)
		
	};
	
	class SLIB_EXPORT LinkedInShareParam
	{
	public:
		String text;
		String contentTitle;
		String contentDescription;
		List<String> contentEntities;
		
		Function<void(LinkedInShareResult& result)> onComplete;
		
	public:
		LinkedInShareParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedInShareParam)
		
	};
	
	class SLIB_EXPORT LinkedInParam : public OAuthParam
	{
	public:
		LinkedInParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedInParam)
		
	};
	
	class LinkedIn : public OAuth2
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		LinkedIn(const LinkedInParam& param);
		
		~LinkedIn();
		
	public:
		static Ref<LinkedIn> create(const LinkedInParam& param);
		
		static void initialize(const LinkedInParam& param);
		
		static void initialize();
		
		static Ref<LinkedIn> create(const String& clientId, const String& clientSecret, const String& redirectUri);
		
		static void initialize(const String& clientId, const String& clientSecret, const String& redirectUri);
		
		static Ref<LinkedIn> createWithAccessToken(const String& accessToken);
		
		static Ref<LinkedIn> getInstance();
		
	public:
		static void resolveUserUrl(const LinkedInResolveUserUrlParam& param);
		
		static void resolveUserUrl(const Function<void(const String& url)>& onComplete);
		
	public:
		static String getRequestUrl(const String& path);
		
		void getUser(const String& userId, const String& fields, const Function<void(LinkedInResult&, LinkedInUser&)>& onComplete);
		
		void getUser(const String& userId, const List<String>& fields, const Function<void(LinkedInResult&, LinkedInUser&)>& onComplete);

		void getUser(const String& userId, const Function<void(LinkedInResult&, LinkedInUser&)>& onComplete);

		void share(const LinkedInShareParam& param);
		
	};
	
}

#endif
