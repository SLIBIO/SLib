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

#include "slib/social/etsy.h"

#include "slib/core/variant.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EtsyUserFeedbackInfo)
	
	SLIB_DEFINE_JSON_MEMBERS(EtsyUserFeedbackInfo, count, score)
	
	EtsyUserFeedbackInfo::EtsyUserFeedbackInfo()
	{
		count = 0;
		score = 0;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EtsyUser)
	
	SLIB_DEFINE_JSON_MEMBERS(EtsyUser, user_id, login_name, primary_email, creation_tsz, user_pub_key, referred_by_user_id, feedback_info, awaiting_feedback_count, use_new_inventory_endpoints)

	EtsyUser::EtsyUser()
	{
		creation_tsz = 0;
		referred_by_user_id = 0;
		awaiting_feedback_count = 0;
		use_new_inventory_endpoints = sl_false;
	}
	
	String EtsyUser::getUrl() const
	{
		return "https://www.etsy.com/people/" + login_name;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EtsyParam)
	
	EtsyParam::EtsyParam()
	{
		requestTokenUrl = "https://openapi.etsy.com/v2/oauth/request_token";
		authenticateUrl = "https://www.etsy.com/oauth/signin";
		accessTokenUrl = "https://openapi.etsy.com/v2/oauth/access_token";
	}
	
	
	SLIB_DEFINE_OBJECT(Etsy, OAuth1)
	
	Etsy::Etsy(const EtsyParam& param) : OAuth1(param)
	{
	}
	
	Etsy::~Etsy()
	{
	}
	
	Ref<Etsy> Etsy::create(const EtsyParam& param)
	{
		return new Etsy(param);
	}
	
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Etsy>, _g_priv_social_etsy_instance)

	void Etsy::initialize(const EtsyParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_etsy_instance)) {
			return;
		}
		_g_priv_social_etsy_instance = create(param);
	}
	
	void Etsy::initialize(const String& consumerKey, const String& consumerSecret, const String& callbackUrl)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_etsy_instance)) {
			return;
		}
		EtsyParam param;
		param.consumerKey = consumerKey;
		param.consumerSecret = consumerSecret;
		param.callbackUrl = callbackUrl;
		_g_priv_social_etsy_instance = create(param);
	}
	
	Ref<Etsy> Etsy::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_etsy_instance)) {
			return sl_null;
		}
		return _g_priv_social_etsy_instance;
	}

	Ptr<EtsyUser> Etsy::getLoginedUser()
	{
		return m_userLoggined;
	}
	
}
