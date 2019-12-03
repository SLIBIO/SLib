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

#include "slib/social/linkedin.h"

#include "slib/core/safe_static.h"
#include "slib/core/log.h"

namespace slib
{
	
	namespace priv
	{
		namespace linkedin
		{
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<LinkedIn>, g_instance)
		}
	}
	
	using namespace priv::linkedin;
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedInUser)
	
	SLIB_DEFINE_JSON(LinkedInUser)
	{
		if (isFromJson) {
			this->json = json;
		}
		SLIB_JSON_ADD_MEMBER(id, "id")
		SLIB_JSON_ADD_MEMBER(firstName, "localizedFirstName")
		SLIB_JSON_ADD_MEMBER(lastName, "localizedLastName")
		if (isFromJson) {
			if (firstName.isNull()) {
				firstName = getNameFromLocalized(json["firstName"]);
			}
			if (lastName.isNull()) {
				lastName = getNameFromLocalized(json["lastName"]);
			}
		}
	}
	
	LinkedInUser::LinkedInUser()
	{
	}
	
	String LinkedInUser::getNameFromLocalized(const Json& localizedName)
	{
		Json name = localizedName["localized"];
		if (name.isNotNull()) {
			Json preferedLocale = localizedName["preferredLocale"];
			if (preferedLocale.isNotNull()) {
				String language = preferedLocale["language"].getString();
				String country = preferedLocale["country"].getString();
				return name[language + "_" + country].getString();
			}
		}
		return sl_null;
	}

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedInParam)
	
	LinkedInParam::LinkedInParam()
	{
		authorizeUrl = "https://www.linkedin.com/oauth/v2/authorization";
		accessTokenUrl = "https://www.linkedin.com/oauth/v2/accessToken";
		defaultScopes.add_NoLock("r_liteprofile");
		defaultScopes.add_NoLock("r_emailaddress");
		flagSupportImplicitGrantType = sl_false;
	}
	
	SLIB_DEFINE_OBJECT(LinkedIn, OAuth2)
	
	LinkedIn::LinkedIn(const LinkedInParam& param) : OAuth2(param)
	{
	}
	
	LinkedIn::~LinkedIn()
	{
	}
	
	Ref<LinkedIn> LinkedIn::create(const LinkedInParam& param)
	{
		return new LinkedIn(param);
	}
	
	void LinkedIn::initialize(const LinkedInParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return;
		}
		g_instance = create(param);
	}
	
	void LinkedIn::initialize()
	{
		LinkedInParam param;
		param.preferenceName = "linkedin";
		initialize(param);
	}
	
	Ref<LinkedIn> LinkedIn::create(const String& clientId, const String& clientSecret, const String& redirectUri)
	{
		LinkedInParam param;
		param.clientId = clientId;
		param.clientSecret = clientSecret;
		param.redirectUri = redirectUri;
		return create(param);
	}
	
	void LinkedIn::initialize(const String& clientId, const String& clientSecret, const String& redirectUri)
	{
		LinkedInParam param;
		param.preferenceName = "linkedin";
		param.clientId = clientId;
		param.clientSecret = clientSecret;
		param.redirectUri = redirectUri;
		initialize(param);
	}
	
	Ref<LinkedIn> LinkedIn::createWithAccessToken(const String& accessKey)
	{
		LinkedInParam param;
		param.accessToken.token = accessKey;
		return create(param);
	}
	
	Ref<LinkedIn> LinkedIn::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return sl_null;
		}
		return g_instance;
	}
	
	String LinkedIn::getRequestUrl(const String& path)
	{
		return "https://api.linkedin.com/v2/" + path;
	}
	
	void LinkedIn::getUser(const String& userId, const String& fields, const Function<void(LinkedInResult&, LinkedInUser&)>& onComplete)
	{
		UrlRequestParam rp;
		if (userId.isNotEmpty()) {
			rp.url = getRequestUrl(String::format("people/(id:%s)", userId));
		} else {
			SLIB_STATIC_STRING(me, "me")
			rp.url = getRequestUrl(me);
		}
		if (fields.isNotEmpty()) {
			rp.parameters.put_NoLock("projection", String::format("(%s)", fields));
		}
		rp.requestHeaders.put_NoLock("X-RestLi-Protocol-Version", "2.0.0");
		rp.onComplete = [onComplete](UrlRequest* request) {
			LinkedInResult result(request);
			LinkedInUser user;
			if (!(request->isError())) {
				FromJson(result.response, user);
				result.flagSuccess = user.id.isNotEmpty();
			}
			onComplete(result, user);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
	void LinkedIn::getUser(const String& userId, const List<String>& fields, const Function<void(LinkedInResult&, LinkedInUser&)>& onComplete)
	{
		getUser(userId, String::join(fields, ","), onComplete);
	}
	
	void LinkedIn::getUser(const String& userId, const Function<void(LinkedInResult&, LinkedInUser&)>& onComplete)
	{
		getUser(userId, String::null(), onComplete);
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedInShareResult)
	
	LinkedInShareResult::LinkedInShareResult(UrlRequest* request): LinkedInResult(request)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedInShareParam)
	
	LinkedInShareParam::LinkedInShareParam()
	{
	}
	
	void LinkedIn::share(const LinkedInShareParam& param)
	{
		auto linkedin = ToRef(this);
		getUser(sl_null, [linkedin, param](LinkedInResult& result, LinkedInUser& user) {
			if (user.id.isEmpty()) {
				LogError("LinkedIn Share", "%s", result.response);
				LinkedInShareResult r(sl_null);
				*((LinkedInResult*)&r) = result;
				param.onComplete(r);
				return;
			}
			UrlRequestParam rp;
			rp.method = HttpMethod::POST;
			rp.url = getRequestUrl("shares");
			rp.requestHeaders.put_NoLock("X-RestLi-Protocol-Version", "2.0.0");
			Json json;
			json.putItem("owner", "urn:li:person:" + user.id);
			if (param.text.isNotEmpty()) {
				Json text;
				text.putItem("text", param.text);
				json.putItem("text", text);
			}
			if (param.contentEntities.isNotEmpty()) {
				Json entities;
				for (auto& entity: param.contentEntities) {
					Json location;
					location.putItem("entityLocation", entity);
					entities.addElement(location);
				}
				Json content;
				content.putItem("contentEntities", entities);
				if (param.contentTitle.isNotEmpty()) {
					content.putItem("title", param.contentTitle);
				}
				if (param.contentDescription.isNotEmpty()) {
					content.putItem("description", param.contentDescription);
				}
				json.putItem("content", content);
			}
			rp.setJsonData(json);
			rp.onComplete = [param](UrlRequest* request) {
				LinkedInShareResult result(request);
				String id = result.response["id"].getString();
				if (id.isEmpty()) {
					LogError("LinkedIn Share", "%s", result.response);
					param.onComplete(result);
					return;
				}
				result.flagSuccess = sl_true;
				param.onComplete(result);
			};
			linkedin->authorizeRequest(rp);
			UrlRequest::send(rp);
		});
	}

}
