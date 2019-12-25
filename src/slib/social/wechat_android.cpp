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

#include "slib/social/wechat.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace wechat_android
		{

			void OnLoginResult(JNIEnv* env, jobject _this, jboolean flagSuccess, jboolean flagCancel, jstring code, jstring errStr);
			void OnPayResult(JNIEnv* env, jobject _this, jboolean flagSuccess, jboolean flagCancel, jstring errStr);
			
			SLIB_JNI_BEGIN_CLASS(JWeChat, "slib/platform/android/wechat/WeChat")
				SLIB_JNI_STATIC_METHOD(initialize, "initialize", "(Landroid/app/Activity;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(login, "login", "()V");
				SLIB_JNI_STATIC_METHOD(pay, "pay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V");
				SLIB_JNI_NATIVE(nativeOnLoginResult, "nativeOnLoginResult", "(ZZLjava/lang/String;Ljava/lang/String;)V", OnLoginResult);
				SLIB_JNI_NATIVE(nativeOnPayResult, "nativeOnPayResult", "(ZZLjava/lang/String;)V", OnPayResult);
			SLIB_JNI_END_CLASS

			class StaticContext
			{
			public:
				Mutex lock;
				Function<void(WeChatLoginResult&)> callbackLogin;
				Function<void(WeChatPaymentResult&)> callbackPay;
				
			public:
				void setLoginCallback(const Function<void(WeChatLoginResult&)>& callback)
				{
					MutexLocker locker(&lock);
					if (callbackPay.isNotNull()) {
						WeChatLoginResult result;
						result.flagCancel = sl_true;
						callbackLogin(result);
					}
					callbackLogin = callback;
				}
				
				void onLoginResult(WeChatLoginResult& result)
				{
					MutexLocker locker(&lock);
					callbackLogin(result);
					callbackLogin.setNull();
				}
				
				void setPayCallback(const Function<void(WeChatPaymentResult&)>& callback)
				{
					MutexLocker locker(&lock);
					if (callbackPay.isNotNull()) {
						WeChatPaymentResult result;
						result.flagCancel = sl_true;
						callbackPay(result);
					}
					callbackPay = callback;
				}
				
				void onPayResult(WeChatPaymentResult& result)
				{
					MutexLocker locker(&lock);
					callbackPay(result);
					callbackPay.setNull();
				}
				
			};
		
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)

			void OnLoginResult(JNIEnv* env, jobject _this, jboolean flagSuccess, jboolean flagCancel, jstring code, jstring errStr)
			{
				WeChatLoginResult result;
				result.flagSuccess = flagSuccess;
				result.flagCancel = flagCancel;
				result.code = Jni::getString(code);
				result.error = Jni::getString(errStr);
				GetStaticContext()->onLoginResult(result);
			}

			void OnPayResult(JNIEnv* env, jobject _this, jboolean flagSuccess, jboolean flagCancel, jstring errStr)
			{
				WeChatPaymentResult result;
				result.flagSuccess = flagSuccess;
				result.flagCancel = flagCancel;
				result.error = Jni::getString(errStr);
				GetStaticContext()->onPayResult(result);
			}

		}
	}
	
	using namespace priv::wechat_android;
	
	void WeChatSDK::initialize(const String& appId, const String& universalLink)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jappId = Jni::getJniString(appId);
			JWeChat::initialize.call(sl_null, jactivity, jappId.get());
		}
	}

	void WeChatSDK::login(const WeChatLoginParam& param)
	{
		if (!(UI::isUiThread())) {
			void (*f)(const WeChatLoginParam&) = &WeChatSDK::login;
			UI::dispatchToUiThread(Function<void()>::bind(f, param));
			return;
		}
		
		GetStaticContext()->setLoginCallback(param.onComplete);
		
		JWeChat::login.call(sl_null);
	}

	void WeChatSDK::pay(const WeChatPaymentRequest& param)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&WeChatSDK::pay, param));
			return;
		}
		
		GetStaticContext()->setPayCallback(param.onComplete);
		
		JniLocal<jstring> partnerId = Jni::getJniString(param.order.partnerId);
		JniLocal<jstring> prepayId = Jni::getJniString(param.order.prepayId);
		JniLocal<jstring> package = Jni::getJniString(param.order.package);
		JniLocal<jstring> nonce = Jni::getJniString(param.order.nonce);
		jlong timeStamp = (jlong)(param.order.timeStamp);
		JniLocal<jstring> sign = Jni::getJniString(param.order.sign);

		JWeChat::pay.call(sl_null, partnerId.get(), prepayId.get(), package.get(), nonce.get(), timeStamp, sign.get());
	}
	
}

#endif
