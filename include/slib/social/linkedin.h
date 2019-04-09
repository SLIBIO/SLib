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
	
	class SLIB_EXPORT LinkedinUser
	{
	public:
		String id;
		String firstName;
		String lastName;
		
		Json json;
		
	public:
		LinkedinUser();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedinUser)
		
		SLIB_DECLARE_JSON
		
	public:
		static String getNameFromLocalized(const Json& localizedName);
		
	};
	
	typedef OAuthLoginResult LinkedinLoginResult;
	
	typedef OAuthApiResult LinkedinResult;
	
	class SLIB_EXPORT LinkedinLoginParam: public OAuthLoginParam
	{
	public:
		LinkedinLoginParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedinLoginParam)
		
	public:
		void addScopeForSharing();
		
	};
	
	class SLIB_EXPORT LinkedinResolveUserUrlParam
	{
	public:
		OAuthWebRedirectDialogOptions dialogOptions;
		Ptr<OAuthWebRedirectDialog> dialog;
		
		Function<void(const String& url)> onComplete;
		
	public:
		LinkedinResolveUserUrlParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedinResolveUserUrlParam)
		
	};
	
	class SLIB_EXPORT LinkedinShareResult : public LinkedinResult
	{
	public:
		LinkedinShareResult(UrlRequest* request);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedinShareResult)
		
	};
	
	class SLIB_EXPORT LinkedinShareParam
	{
	public:
		String text;
		String contentTitle;
		String contentDescription;
		List<String> contentEntities;
		
		Function<void(LinkedinShareResult& result)> onComplete;
		
	public:
		LinkedinShareParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedinShareParam)
		
	};
	
	class SLIB_EXPORT LinkedinParam : public OAuthParam
	{
	public:
		LinkedinParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(LinkedinParam)
		
	};
	
	class Linkedin : public OAuth2
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Linkedin(const LinkedinParam& param);
		
		~Linkedin();
		
	public:
		static Ref<Linkedin> create(const LinkedinParam& param);
		
		static void initialize(const LinkedinParam& param);
		
		static void initialize(const String& redirectUri, const String& clientId, const String& clientSecret);

		static Ref<Linkedin> getInstance();
		
	public:
		static void resolveUserUrl(const LinkedinResolveUserUrlParam& param);
		
		static void resolveUserUrl(const Function<void(const String& url)>& onComplete);
		
	public:
		static String getRequestUrl(const String& path);
		
		void getUser(const String& userId, const String& fields, const Function<void(LinkedinResult&, LinkedinUser&)>& onComplete);
		
		void getUser(const String& userId, const List<String>& fields, const Function<void(LinkedinResult&, LinkedinUser&)>& onComplete);

		void getUser(const String& userId, const Function<void(LinkedinResult&, LinkedinUser&)>& onComplete);

		void share(const LinkedinShareParam& param);
		
	};
	
}

#endif
