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

#include "slib/social/alipay.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace alipay_android
		{

			void OnPayResult(JNIEnv* env, jobject _this, jstring jresult);

			SLIB_JNI_BEGIN_CLASS(JWeChat, "slib/platform/android/alipay/Alipay")
				SLIB_JNI_STATIC_METHOD(pay, "pay", "(Landroid/app/Activity;Ljava/lang/String;)V");
				SLIB_JNI_NATIVE(nativeOnPayResult, "nativeOnPayResult", "(Ljava/lang/String;)V", OnPayResult);
			SLIB_JNI_END_CLASS

			class StaticContext
			{
			public:
				Mutex lock;
				Function<void(AlipayPaymentResult&)> callbackPay;
				
			public:
				void setPayCallback(const Function<void(AlipayPaymentResult&)>& callback)
				{
					MutexLocker locker(&lock);
					if (callbackPay.isNotNull()) {
						AlipayPaymentResult result;
						result.flagCancel = sl_true;
						callbackPay(result);
					}
					callbackPay = callback;
				}
				
				void onPayResult(AlipayPaymentResult& result)
				{
					MutexLocker locker(&lock);
					callbackPay(result);
					callbackPay.setNull();
				}
				
			};
		
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)

			void OnPayResult(JNIEnv* env, jobject _this, jstring jresult)
			{
				AlipayPaymentResult result;
				result.applyAppResponse(Jni::getString(jresult));
				GetStaticContext()->onPayResult(result);
			}

		}
	}
	
	using namespace priv::alipay_android;
	
	void AlipaySDK::initialize(const String& appScheme)
	{
	}

	void AlipaySDK::pay(const AlipayPaymentRequest& param)
	{
		if (!(UI::isUiThread())) {
			void (*func)(const AlipayPaymentRequest&) = &AlipaySDK::pay;
			UI::dispatchToUiThread(Function<void()>::bind(func, param));
			return;
		}

		jobject jactivity = Android::getCurrentActivity();
		if (!jactivity) {
			AlipayPaymentResult result;
			param.onComplete(result);
		}

		GetStaticContext()->setPayCallback(param.onComplete);

		JniLocal<jstring> order(Jni::getJniString(param.order));

		JWeChat::pay.call(sl_null, jactivity, order.get());

	}

}

#endif
