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

#ifndef CHECKHEADER_SLIB_SOCIAL_PINTEREST
#define CHECKHEADER_SLIB_SOCIAL_PINTEREST

#include "definition.h"

#include "oauth.h"

namespace slib
{
	
	class SLIB_EXPORT PinterestUser
	{
	public:
		String id;
		String url;
		String first_name;
		String last_name;
		
		Json json;
		
	public:
		PinterestUser();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PinterestUser)
		
		SLIB_DECLARE_JSON
		
	};
	
	typedef OAuthLoginResult PinterestLoginResult;
	
	typedef OAuthApiResult PinterestResult;
	
	typedef OAuthLoginParam PinterestLoginParam;
	
	class SLIB_EXPORT PinterestParam : public OAuthParam
	{
	public:
		PinterestParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PinterestParam)
		
	};
	
	class Pinterest : public OAuth2
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Pinterest(const PinterestParam& param);
		
		~Pinterest();
		
	public:
		static Ref<Pinterest> create(const PinterestParam& param);
		
		static void initialize(const PinterestParam& param);
		
		static void initialize(const String& redirectUrl, const String& appId, const String& appSecret = String::null());

		static Ref<Pinterest> getInstance();
		
	public:
		using OAuth2::authorizeRequest;
		
		void authorizeRequest(UrlRequestParam& param, const OAuthAccessToken& token) override;
		
	public:
		String getRequestUrl(const String& path);
		
		void getUser(const String& userId, const Function<void(PinterestResult&, PinterestUser&)>& onComplete);
		
	protected:
		String m_version;
		
	};
	
}

#endif
