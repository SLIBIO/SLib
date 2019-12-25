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

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/social/alipay.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/safe_static.h"

#import "alipay/iOS/AlipaySDK.h"

typedef AlipaySDK SDK;

namespace slib
{

	namespace priv
	{
		namespace alipay_ios
		{
		
			String g_appScheme;
		
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
		
		}
	}
	
	using namespace priv::alipay_ios;
	
	void AlipaySDK::initialize(const String& appScheme)
	{
		g_appScheme = appScheme;
		UIPlatform::registerOpenUrlCallback([](NSURL* url, NSDictionary*) {
			[[SDK defaultService] processOrderWithPaymentResult:url standbyCallback:^(NSDictionary *resultDic) {
			}];
			return YES;
		});
	}

	void AlipaySDK::pay(const AlipayPaymentRequest& param)
	{
		if (!(UI::isUiThread())) {
			void (*func)(const AlipayPaymentRequest&) = &AlipaySDK::pay;
			UI::dispatchToUiThread(Function<void()>::bind(func, param));
			return;
		}
		
		GetStaticContext()->setPayCallback(param.onComplete);
		
		NSString* order = Apple::getNSStringFromString(param.order);
		NSString* scheme = Apple::getNSStringFromString(g_appScheme);
		
		static auto callback = ^(NSDictionary *resultDic) {
			NSLog(@"Alipay result = %@", resultDic);
			HashMap<String, String> map;
			for (NSString* key in resultDic) {
				NSString* value = resultDic[key];
				map.put_NoLock(Apple::getStringFromNSString(key), Apple::getStringFromNSString(value));
			}
			AlipayPaymentResult result;
			result.response = map;
			GetStaticContext()->onPayResult(result);
		};
		
		[[SDK defaultService] payOrder:order fromScheme:scheme callback:callback];
		
	}
	
}

#endif
