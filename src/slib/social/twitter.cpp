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

#include "slib/social/twitter.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterParam)
	
	TwitterParam::TwitterParam()
	{
		requestTokenUrl = "https://api.twitter.com/oauth/request_token";
		authenticateUrl = "https://api.twitter.com/oauth/authenticate?force_login=true&oauth_token=%s";
		accessTokenUrl = "https://api.twitter.com/oauth/access_token";
	}
	
	
	SLIB_DEFINE_OBJECT(Twitter, OAuth1)
	
	Twitter::Twitter(const TwitterParam& param) : OAuth1(param)
	{
	}
	
	Twitter::~Twitter()
	{
	}
	
	Ref<Twitter> Twitter::create(const TwitterParam& param)
	{
		return new Twitter(param);
	}
	
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Twitter>, _g_priv_social_twitter_instance)

	void Twitter::initialize(const TwitterParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_twitter_instance)) {
			return;
		}
		_g_priv_social_twitter_instance = create(param);
	}
	
	void Twitter::initialize(const String& consumerKey, const String& consumerSecret, const String& callbackUrl)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_twitter_instance)) {
			return;
		}
		TwitterParam param;
		param.consumerKey = consumerKey;
		param.consumerSecret = consumerSecret;
		param.callbackUrl = callbackUrl;
		_g_priv_social_twitter_instance = create(param);
	}
	
	Ref<Twitter> Twitter::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_twitter_instance)) {
			return sl_null;
		}
		return _g_priv_social_twitter_instance;
	}

	String Twitter::getUserId()
	{
		return m_userId;
	}
	
	void Twitter::setUserId(const String& userId)
	{
		m_userId = userId;
	}
	
	String Twitter::getUserScreenName()
	{
		return m_userScreenName;
	}
	
	void Twitter::setUserScreenName(const String& name)
	{
		m_userScreenName = name;
	}
	
}
