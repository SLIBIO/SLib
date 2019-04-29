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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/social/facebook.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"

#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidFacebookToken, "slib/platform/android/social/FacebookToken")
		SLIB_JNI_STRING_FIELD(token)
		SLIB_JNI_STRING_FIELD(scopes)
		SLIB_JNI_LONG_FIELD(expirationTime)
		SLIB_JNI_LONG_FIELD(refreshTime)
	SLIB_JNI_END_CLASS

	void _priv_Android_Facebook_onLoginResult(JNIEnv* env, jobject _this, jobject token, jboolean flagCancel);
	void _priv_Android_Facebook_onShareResult(JNIEnv* env, jobject _this, jboolean flagSuccess, jboolean flagCancel);

	SLIB_JNI_BEGIN_CLASS(JAndroidFacebook, "slib/platform/android/social/Facebook")
		SLIB_JNI_STATIC_METHOD(initialize, "initialize", "()V");
		SLIB_JNI_STATIC_METHOD(getCurrentToken, "getCurrentToken", "()Lslib/platform/android/social/FacebookToken;");
		SLIB_JNI_STATIC_METHOD(clearAccessToken, "clearAccessToken", "()V");

		SLIB_JNI_STATIC_METHOD(login, "login", "(Landroid/app/Activity;ZLjava/lang/String;)V");
		SLIB_JNI_NATIVE(nativeOnLoginResult, "nativeOnLoginResult", "(Lslib/platform/android/social/FacebookToken;Z)V", _priv_Android_Facebook_onLoginResult);

		SLIB_JNI_STATIC_METHOD(share, "share", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		SLIB_JNI_NATIVE(nativeOnShareResult, "nativeOnShareResult", "(ZZ)V", _priv_Android_Facebook_onShareResult);
	SLIB_JNI_END_CLASS

	
	void FacebookSDK::initialize()
	{
		JAndroidFacebook::initialize.call(sl_null);
	}

	class _priv_FacebookSDK
	{
	public:
		Function<void(FacebookLoginResult&)> onLoginResult;
		Function<void(FacebookShareResult&)> onShareResult;

	public:
		static _priv_FacebookSDK* get()
		{
			SLIB_SAFE_STATIC(_priv_FacebookSDK, s)
			if (SLIB_SAFE_STATIC_CHECK_FREED(s)) {
				return sl_null;
			}
			return &s;
		}

		static void getToken(OAuthAccessToken& _out, jobject _in)
		{
			_out.token = JAndroidFacebookToken::token.get(_in);
			_out.scopes = JAndroidFacebookToken::scopes.get(_in).split(",");
			_out.expirationTime = Time::withMilliseconds(JAndroidFacebookToken::expirationTime.get(_in));
			_out.refreshTime = Time::withMilliseconds(JAndroidFacebookToken::refreshTime.get(_in));
		}
		
	};

	void FacebookSDK::_updateCurrentToken(Facebook* instance)
	{
		auto p = _priv_FacebookSDK::get();
		if (p) {
			JniLocal<jobject> token = JAndroidFacebook::getCurrentToken.callObject(sl_null);
			if (token.get()) {
				OAuthAccessToken oauthToken;
				_priv_FacebookSDK::getToken(oauthToken, token.get());
				instance->setAccessToken(oauthToken);
			}
		}
	}

	void FacebookSDK::login(const FacebookLoginParam& param)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread([param]() {
				FacebookSDK::login(param);
			});
			return;
		}
		auto p = _priv_FacebookSDK::get();
		if (p) {
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				if (p->onLoginResult.isNotNull()) {
					FacebookLoginResult result;
					p->onLoginResult(result);
				}
				p->onLoginResult = param.onComplete;
				String scopes = StringBuffer::join(",", param.authorization.scopes);
				JniLocal<jstring> jscopes = Jni::getJniString(scopes);
				JAndroidFacebook::login.call(sl_null, jactivity, param.flagPublishPermissions, jscopes.get());
				return;
			}
		}
		FacebookLoginResult result;
		param.onComplete(result);
	}

	void _priv_Android_Facebook_onLoginResult(JNIEnv* env, jobject _this, jobject token, jboolean flagCancel)
	{
		auto p = _priv_FacebookSDK::get();
		if (p) {
			FacebookLoginResult result;
			if (token) {
				_priv_FacebookSDK::getToken(result.accessToken, token);
				result.flagSuccess = sl_true;
			} else {
				if (flagCancel) {
					result.flagCancel = sl_true;
				}
			}
			p->onLoginResult(result);
			p->onLoginResult.setNull();
		}
	}

	void FacebookSDK::share(const FacebookShareParam& param)
	{
		auto p = _priv_FacebookSDK::get();
		if (!p) {
			FacebookShareResult result;
			param.onComplete(result);
			return;
		}
		if (param.url.isEmpty()) {
			FacebookShareResult result;
			param.onComplete(result);
			return;
		}
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread([param]() {
				FacebookSDK::share(param);
			});
			return;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (!jactivity) {
			FacebookShareResult result;
			param.onComplete(result);
			return;
		}
		if (p->onShareResult.isNotNull()) {
			FacebookShareResult result;
			p->onShareResult(result);
		}
		p->onShareResult = param.onComplete;

		JniLocal<jstring> jurl = Jni::getJniString(param.url);
		JniLocal<jstring> jquote = Jni::getJniString(param.quote);
		JniLocal<jstring> jhashTag = Jni::getJniString(param.hashTag);
		JAndroidFacebook::share.call(sl_null, jactivity, jurl.get(), jquote.get(), jhashTag.get());
	}

	void _priv_Android_Facebook_onShareResult(JNIEnv* env, jobject _this, jboolean flagSuccess, jboolean flagCancel)
	{
		auto p = _priv_FacebookSDK::get();
		if (p) {
			FacebookShareResult result;
			if (flagSuccess) {
				result.flagSuccess = flagSuccess;
			} else {
				if (flagCancel) {
					result.flagCancel = sl_true;
				}
			}
			p->onShareResult(result);
			p->onShareResult.setNull();
		}
	}

	void FacebookSDK::clearAccessToken()
	{
		JAndroidFacebook::clearAccessToken.call(sl_null);
	}
	
}

#endif
