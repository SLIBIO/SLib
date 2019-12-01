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

#include "slib/social/wechat.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/time.h"
#include "slib/core/safe_static.h"

#import "wechat/iOS/WXApi.h"

@interface SLIBWechatSDKDelegate : NSObject<WXApiDelegate>
{
}
@end

namespace slib
{

	namespace priv
	{
		namespace wechat_ios
		{
						
			class StaticContext
			{
			public:
				SLIBWechatSDKDelegate* delegate;
				Mutex lock;
				Function<void(WechatLoginResponse&)> callbackLogin;
				Function<void(WechatPaymentResponse&)> callbackPay;
				
			public:
				StaticContext()
				{
					SLIBWechatSDKDelegate* delegate = [SLIBWechatSDKDelegate new];
					this->delegate = delegate;
					UIPlatform::registerOpenUrlCallback([delegate](NSURL* url, NSDictionary*) {
						return [WXApi handleOpenURL:url delegate:delegate];
					});
				}
				
			public:
				void setLoginCallback(const Function<void(WechatLoginResponse&)>& callback)
				{
					MutexLocker locker(&lock);
					if (callbackPay.isNotNull()) {
						WechatLoginResponse response;
						callbackLogin(response);
					}
					callbackLogin = callback;
				}
				
				void onLoginResponse(WechatLoginResponse& response)
				{
					MutexLocker locker(&lock);
					callbackLogin(response);
					callbackLogin.setNull();
				}
				
				void setPayCallback(const Function<void(WechatPaymentResponse&)>& callback)
				{
					MutexLocker locker(&lock);
					if (callbackPay.isNotNull()) {
						WechatPaymentResponse response;
						callbackPay(response);
					}
					callbackPay = callback;
				}
				
				void onPayResponse(WechatPaymentResponse& response)
				{
					MutexLocker locker(&lock);
					callbackPay(response);
					callbackPay.setNull();
				}
				
			};
		
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
		
		}
	}
	
	using namespace priv::wechat_ios;
	
	void WechatSDK::initialize(const String& appId, const String& universalLink)
	{
		GetStaticContext();
		[WXApi registerApp:Apple::getNSStringFromString(appId) universalLink:Apple::getNSStringFromString(universalLink)];
	}

	void WechatSDK::login(const WechatLoginParam& param)
	{
		if (!(UI::isUiThread())) {
			void (*f)(const WechatLoginParam&) = &WechatSDK::login;
			UI::dispatchToUiThread(Function<void()>::bind(f, param));
			return;
		}
		
		GetStaticContext()->setLoginCallback(param.onComplete);
		
		SendAuthReq* req = [SendAuthReq new];
		req.scope = @"snsapi_userinfo";
		req.state = [NSString stringWithFormat:@"%d", (int)(Time::now().toInt())];
		[WXApi sendReq:req completion:^(BOOL success) {
			if (!success) {
				WechatLoginResponse response;
				GetStaticContext()->onLoginResponse(response);
			}
		}];
	}

	void WechatSDK::pay(const WechatPaymentRequest& param)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&WechatSDK::pay, param));
			return;
		}
		
		GetStaticContext()->setPayCallback(param.onComplete);
		
		PayReq* req = [PayReq new];
		req.partnerId = Apple::getNSStringFromString(param.partnerId);
		req.prepayId = Apple::getNSStringFromString(param.prepayId);
		req.nonceStr = Apple::getNSStringFromString(param.nonceStr);
		req.timeStamp = (UInt32)(param.timeStamp);
		req.package = Apple::getNSStringFromString(param.package);
		req.sign = Apple::getNSStringFromString(param.sign);
		[WXApi sendReq:req completion:^(BOOL success) {
			if (!success) {
				WechatPaymentResponse response;
				GetStaticContext()->onPayResponse(response);
			}
		}];
	}
	
}

using namespace slib;
using namespace slib::priv::wechat_ios;

@implementation SLIBWechatSDKDelegate

-(void)onResp:(BaseResp*)resp
{
	if ([resp isKindOfClass:[SendAuthResp class]]) {
		SendAuthResp* loginResponse = (SendAuthResp*)resp;
		WechatLoginResponse response;
		switch(loginResponse.errCode){
			case WXSuccess:
				response.flagSuccess = sl_true;
				break;
			case WXErrCodeUserCancel:
				response.flagCancel = sl_true;
			default:
				break;
		}
		response.code = Apple::getStringFromNSString(loginResponse.code);
		response.error = Apple::getStringFromNSString(loginResponse.errStr);
		GetStaticContext()->onLoginResponse(response);
	} else if ([resp isKindOfClass:[PayResp class]]) {
		PayResp* paymentResponse = (PayResp*)resp;
		WechatPaymentResponse response;
		switch(paymentResponse.errCode){
			case WXSuccess:
				response.flagSuccess = sl_true;
				break;
			case WXErrCodeUserCancel:
				response.flagCancel = sl_true;
			default:
				break;
		}
		response.error = Apple::getStringFromNSString(paymentResponse.errStr);
		GetStaticContext()->onPayResponse(response);
	}
}

@end

#endif
