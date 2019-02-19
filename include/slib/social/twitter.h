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

#ifndef CHECKHEADER_SLIB_SOCIAL_TWITTER
#define CHECKHEADER_SLIB_SOCIAL_TWITTER

#include "definition.h"

#include "oauth.h"

namespace slib
{
	
	class SLIB_EXPORT TwitterParam : public OAuthParam
	{
	public:
		TwitterParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterParam)
		
	};
	
	class SLIB_EXPORT TwitterLoginResult : public OAuthLoginResult
	{
	public:
		String userId;
		String userScreenName;
		
	public:
		TwitterLoginResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterLoginResult)
		
	};

	class SLIB_EXPORT TwitterLoginParam : public OAuthLoginParam
	{
	public:
		Function<void(TwitterLoginResult& result)> onComplete;
		
	public:
		TwitterLoginParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterLoginParam)
		
	};
	
	class Twitter : public OAuth1
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Twitter(const TwitterParam& param);
		
		~Twitter();
		
	public:
		static Ref<Twitter> create(const TwitterParam& param);
		
		static void initialize(const TwitterParam& param);
		
		static void initialize(const String& consumerKey, const String& consumerSecret, const String& callbackUrl);

		static Ref<Twitter> getInstance();

	public:
		void login(const TwitterLoginParam& param);
		
		void login(const Function<void(TwitterLoginResult& result)>& onComplete);
		
	public:
		String getUserId();
		
		void setUserId(const String& userId);
		
		String getUserScreenName();
		
		void setUserScreenName(const String& name);
		
	protected:
		AtomicString m_userId;
		AtomicString m_userScreenName;
		
	};
	
}

#endif
