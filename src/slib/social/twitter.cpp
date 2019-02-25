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
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterUser)
	
	SLIB_DEFINE_JSON(TwitterUser)
	{
		if (isFromJson) {
			this->json = json;
		}
		SLIB_JSON_ADD_MEMBERS(id, screen_name, name, location, url, description, followers_count, friends_count, listed_count, favourites_count, statuses_count, created_at, lang, profile_image_url, profile_image_url_https)

	}
	
	TwitterUser::TwitterUser()
	 : id(0), followers_count(0), friends_count(0), listed_count(0), favourites_count(0), statuses_count(0)
	{
	}
	
	String TwitterUser::getPublicProfileURL(const String& screenName)
	{
		if (screenName.isNotEmpty()) {
			return "https://twitter.com/" + screenName;
		}
		return sl_null;
	}
	
	String TwitterUser::getPublicProfileURL() const
	{
		return getPublicProfileURL(screen_name);
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterParam)
	
	TwitterParam::TwitterParam()
	{
		requestTokenUrl = "https://api.twitter.com/oauth/request_token";
		authenticateUrl = "https://api.twitter.com/oauth/authenticate?oauth_token=%s";
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
	
	void Twitter::initialize(const String& callbackUrl, const String& consumerKey, const String& consumerSecret)
	{
		TwitterParam param;
		param.preferenceName = "twitter";
		param.consumerKey = consumerKey;
		param.consumerSecret = consumerSecret;
		param.callbackUrl = callbackUrl;
		initialize(param);
	}
	
	Ref<Twitter> Twitter::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_twitter_instance)) {
			return sl_null;
		}
		return _g_priv_social_twitter_instance;
	}

	String Twitter::getRequestUrl(const String& path)
	{
		return "https://api.twitter.com/1.1/" + path;
	}
	
	void Twitter::getUser(const String& userId, const String& screenName, const Function<void(TwitterResult&, TwitterUser&)>& onComplete)
	{
		UrlRequestParam rp;
		if (userId.isNotEmpty()) {
			rp.url = getRequestUrl("users/show.json");
			rp.parameters.put_NoLock("user_id", userId);
		} else if (screenName.isNotEmpty()) {
			rp.url = getRequestUrl("users/show.json");
			rp.parameters.put_NoLock("screen_name", screenName);
		} else {
			rp.url = getRequestUrl("account/verify_credentials.json");
		}
		rp.onComplete = [onComplete](UrlRequest* request) {
			TwitterResult result(request);
			TwitterUser user;
			if (!(request->isError())) {
				FromJson(result.response, user);
				result.flagSuccess = user.id != 0 && user.screen_name.isNotEmpty();
			}
			onComplete(result, user);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
}
