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

#include "../graphics/image.h"

namespace slib
{
	
	class SLIB_EXPORT TwitterUser
	{
	public:
		sl_uint64 id;
		String screen_name; // The screen name, handle, or alias that this user identifies themselves with. screen_names are unique but subject to change.
		String name; // The name of the user, as they’ve defined it. Not necessarily a person’s name. Typically capped at 20 characters, but subject to change.
		String location; // The user-defined location for this account’s profile. Not necessarily a location, nor machine-parseable. This field will occasionally be fuzzily interpreted by the Search service.
		String url; // A URL provided by the user in association with their profile.
		String description; // The user-defined UTF-8 string describing their account.
		sl_uint32 followers_count; // The number of followers this account currently has. Under certain conditions of duress, this field will temporarily indicate “0”.
		sl_uint32 friends_count; // The number of users this account is following (AKA their “followings”). Under certain conditions of duress, this field will temporarily indicate “0”.
		sl_uint32 listed_count; // The number of public lists that this user is a member of.
		sl_uint32 favourites_count; // The number of Tweets this user has liked in the account’s lifetime. British spelling used in the field name for historical reasons.
		sl_uint32 statuses_count; // The number of Tweets (including retweets) issued by the user.
		String created_at; // The UTC datetime that the user account was created on Twitter.
		String lang; // The BCP 47 code for the user’s self-declared user interface language. May or may not have anything to do with the content of their Tweets.
		String profile_image_url; // A HTTP-based URL pointing to the user’s profile image.
		String profile_image_url_https; // A HTTPS-based URL pointing to the user’s profile image.
		
		Json json; // raw-response data from API
		
	public:
		TwitterUser();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterUser)
		
		SLIB_DECLARE_JSON
		
	public:
		static String getPublicProfileURL(const String& screenName);
		
		// Url for User Home Page
		String getPublicProfileURL() const;
		
	};
	
	typedef OAuthApiResult TwitterResult;
	
	typedef OAuth1_LoginResult TwitterLoginResult;
	
	typedef OAuth1_LoginParam TwitterLoginParam;
	
	class SLIB_EXPORT TwitterShareResult : public TwitterResult
	{
	public:
		TwitterShareResult(UrlRequest* request);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterShareResult)
		
	};
	
	class SLIB_EXPORT TwitterShareParam
	{
	public:
		String status;
		List<Memory> medias;
		
		Function<void(TwitterShareResult& result)> onComplete;
		
	public:
		TwitterShareParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterShareParam)
		
	};
	
	class SLIB_EXPORT TwitterParam : public OAuth1_Param
	{
	public:
		TwitterParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TwitterParam)
		
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
		
		static void initialize();
		
		static Ref<Twitter> create(const String& consumerKey, const String& consumerSecret, const String& callbackUrl);

		static void initialize(const String& consumerKey, const String& consumerSecret, const String& callbackUrl);
		
		static Ref<Twitter> createWithAccessToken(const String& token, const String tokenSecret);
		
		static Ref<Twitter> getInstance();

	public:
		static String getRequestUrl(const String& path);
		
		// get user by userId or screenName (if two arguments are empty, then the loginned user will be returned)
		void getUser(const String& userId, const String& screenName, const Function<void(TwitterResult&, TwitterUser&)>& callback);
		
		void share(const TwitterShareParam& param);
		
	};
	
}

#endif
