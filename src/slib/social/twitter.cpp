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
#include "slib/core/log.h"

namespace slib
{
	
	namespace priv
	{
		namespace twitter
		{
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Twitter>, g_instance)
		}
	}
	
	using namespace priv::twitter;
	
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
	
	void Twitter::initialize(const TwitterParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return;
		}
		g_instance = create(param);
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
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return sl_null;
		}
		return g_instance;
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

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterShareResult)
	
	TwitterShareResult::TwitterShareResult(UrlRequest* request): TwitterResult(request)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterShareParam)
	
	TwitterShareParam::TwitterShareParam()
	{
	}
	
	void Twitter::share(const TwitterShareParam& _param)
	{
		class Params : public TwitterShareParam
		{
		public:
			Params(const TwitterShareParam& param): TwitterShareParam(param) {}
		public:
			sl_size indexMedia;
			List<String> mediaIds;
			Ref<Twitter> twitter;
		};
		static Function<void(Params param, UrlRequest*)> callback;
		if (callback.isNull()) {
			callback = [](Params param, UrlRequest* request) {
				if (request) {
					TwitterShareResult result(request);
					if (request->isError()) {
						LogError("Twitter Sharing", "%s", result.response);
						param.onComplete(result);
						return;
					}
					String mediaId = result.response["media_id"].getString();
					if (mediaId.isEmpty()) {
						LogError("Twitter Sharing", "%s", result.response);
						param.onComplete(result);
						return;
					}
					param.mediaIds.add(mediaId);
					param.indexMedia++;
				}
				UrlRequestParam rp;
				if (param.indexMedia < param.medias.getCount()) {
					rp.method = HttpMethod::POST;
					rp.url = "https://upload.twitter.com/1.1/media/upload.json";
					VariantHashMap map;
					map.put("media", param.medias.getValueAt(param.indexMedia));
					rp.setMultipartFormData(map);
					rp.onComplete = Function<void(UrlRequest*)>::bind(callback, param);
				} else {
					rp.method = HttpMethod::POST;
					rp.url = getRequestUrl("statuses/update.json");
					VariantHashMap map;
					map.put("status", param.status);
					if (param.mediaIds.isNotEmpty()) {
						map.put("media_ids", StringBuffer::join(",", param.mediaIds));
					}
					rp.setFormData(map);
					rp.onComplete = [param](UrlRequest* request) {
						TwitterShareResult result(request);
						if (request->isError()) {
							LogError("Twitter Sharing", "%s", result.response);
							param.onComplete(result);
							return;
						}
						String id = result.response["id"].getString();
						if (id.isEmpty()) {
							LogError("Twitter Sharing", "%s", result.response);
							param.onComplete(result);
							return;
						}
						result.flagSuccess = sl_true;
						param.onComplete(result);
					};
				}
				param.twitter->authorizeRequest(rp);
				UrlRequest::send(rp);
			};
		}
		
		Params param(_param);
		param.indexMedia = 0;
		param.mediaIds = List<String>::create();
		param.twitter = this;
		callback(param, sl_null);
	}
	
}
