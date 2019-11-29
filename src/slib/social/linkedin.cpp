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
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Linkedin>, g_instance)
		}
	}
	
	using namespace priv::linkedin;
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedinUser)
	
	SLIB_DEFINE_JSON(LinkedinUser)
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
	
	LinkedinUser::LinkedinUser()
	{
	}
	
	String LinkedinUser::getNameFromLocalized(const Json& localizedName)
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

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedinParam)
	
	LinkedinParam::LinkedinParam()
	{
		authorizeUrl = "https://www.linkedin.com/oauth/v2/authorization";
		accessTokenUrl = "https://www.linkedin.com/oauth/v2/accessToken";
		defaultScopes.add_NoLock("r_liteprofile");
		defaultScopes.add_NoLock("r_emailaddress");
		flagSupportImplicitGrantType = sl_false;
	}
	
	SLIB_DEFINE_OBJECT(Linkedin, OAuth2)
	
	Linkedin::Linkedin(const LinkedinParam& param) : OAuth2(param)
	{
	}
	
	Linkedin::~Linkedin()
	{
	}
	
	Ref<Linkedin> Linkedin::create(const LinkedinParam& param)
	{
		return new Linkedin(param);
	}
		
	void Linkedin::initialize(const LinkedinParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return;
		}
		g_instance = create(param);
	}
	
	void Linkedin::initialize(const String& redirectUri, const String& clientId, const String& clientSecret)
	{
		LinkedinParam param;
		param.preferenceName = "linkedin";
		param.clientId = clientId;
		param.clientSecret = clientSecret;
		param.redirectUri = redirectUri;
		initialize(param);
	}
	
	Ref<Linkedin> Linkedin::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return sl_null;
		}
		return g_instance;
	}
	
	String Linkedin::getRequestUrl(const String& path)
	{
		return "https://api.linkedin.com/v2/" + path;
	}
	
	void Linkedin::getUser(const String& userId, const String& fields, const Function<void(LinkedinResult&, LinkedinUser&)>& onComplete)
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
			LinkedinResult result(request);
			LinkedinUser user;
			if (!(request->isError())) {
				FromJson(result.response, user);
				result.flagSuccess = user.id.isNotEmpty();
			}
			onComplete(result, user);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
	void Linkedin::getUser(const String& userId, const List<String>& fields, const Function<void(LinkedinResult&, LinkedinUser&)>& onComplete)
	{
		getUser(userId, String::join(fields, ","), onComplete);
	}
	
	void Linkedin::getUser(const String& userId, const Function<void(LinkedinResult&, LinkedinUser&)>& onComplete)
	{
		getUser(userId, String::null(), onComplete);
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedinShareResult)
	
	LinkedinShareResult::LinkedinShareResult(UrlRequest* request): LinkedinResult(request)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(LinkedinShareParam)
	
	LinkedinShareParam::LinkedinShareParam()
	{
	}
	
	void Linkedin::share(const LinkedinShareParam& param)
	{
		auto linkedin = ToRef(this);
		getUser(sl_null, [linkedin, param](LinkedinResult& result, LinkedinUser& user) {
			if (user.id.isEmpty()) {
				LogError("Linkedin Share", "%s", result.response);
				LinkedinShareResult r(sl_null);
				*((LinkedinResult*)&r) = result;
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
				LinkedinShareResult result(request);
				String id = result.response["id"].getString();
				if (id.isEmpty()) {
					LogError("Linkedin Share", "%s", result.response);
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
