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

#include "slib/social/facebook.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
#if !defined(SLIB_PLATFORM_IS_IOS) && !defined(SLIB_PLATFORM_IS_ANDROID)
	
	void FacebookSDK::initialize()
	{
	}
	
	void FacebookSDK::login(const FacebookLoginParam& param)
	{
		Ref<Facebook> instance = Facebook::getInstance();
		if (instance.isNotNull()) {
			instance->login(param);
		}
	}
	
	void FacebookSDK::clearAccessToken()
	{
		Ref<Facebook> instance = Facebook::getInstance();
		if (instance.isNotNull()) {
			instance->clearAccessToken();
		}
	}

	Ref<Facebook> FacebookSDK::getInstance()
	{
		return Facebook::getInstance();
	}

#else

	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Facebook>, _g_priv_social_facebook_sdk_instance)

	Ref<Facebook> FacebookSDK::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_social_facebook_sdk_instance)) {
			return sl_null;
		}
		Ref<Facebook> instance = _g_priv_social_facebook_sdk_instance;
		if (instance.isNull()) {
			FacebookParam param;
			instance = new Facebook(param);
			_g_priv_social_facebook_sdk_instance = instance;
		}
		if (instance.isNotNull()) {
			_updateCurrentToken(instance.get());
		}
		return instance;
	}

#endif
	
	void FacebookSDK::login(const Function<void(FacebookLoginResult& result)>& onComplete)
	{
		FacebookLoginParam param;
		param.onComplete = onComplete;
		login(param);
	}
	
}
