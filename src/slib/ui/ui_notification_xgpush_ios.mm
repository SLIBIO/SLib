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

#include "slib/ui/xgpush.h"

#include "slib/core/safe_static.h"
#include "slib/core/log.h"

#include "slib/ui/platform.h"

#include "xgpush/iOS/XGPush.h"

@interface SLIBXGPush : NSObject<XGPushDelegate>
{
}
@end

namespace slib
{

	namespace priv
	{
		namespace xgpush
		{
			sl_uint32 g_accessId = 0;
			String g_accessKey;
			SLIBXGPush* g_instance = nil;
			sl_bool g_flagStarted = sl_false;
		}
	}

	using namespace priv::xgpush;

	void XgPush::initialize(sl_uint32 accessId, const String& key)
	{
		g_accessId = accessId;
		g_accessKey = key;
	}
	
	Ref<XgPush> XgPush::getInstance()
	{
		SLIB_SAFE_STATIC(Mutex, lock)
		MutexLocker locker(&lock);
		
		SLIB_STATIC_ZERO_INITIALIZED(Ref<XgPush>, instance);
		if (instance.isNotNull()) {
			return instance;
		}

		instance = new XgPush;
		g_instance = [SLIBXGPush new];
		
		UIPlatform::registerDidReceiveRemoteNotificationCallback([](NSDictionary* userInfo) {
			[[XGPush defaultManager] reportXGNotificationInfo:userInfo];
		});
		UIPlatform::registerDidFinishLaunchingCallback([](NSDictionary* userInfo) {
			[[XGPush defaultManager] reportXGNotificationInfo:userInfo];
		});
		return instance;
	}

	void XgPush::onStart()
	{
		sl_uint32 accessId = g_accessId;
		NSString* accessKey = Apple::getNSStringFromString(g_accessKey);
		[[XGPush defaultManager] startXGWithAppID:accessId appKey:accessKey delegate:g_instance];
	}
	
	void XgPush::setEnableDebug(sl_bool flag)
	{
		[[XGPush defaultManager] setEnableDebug:(flag ? YES : NO)];
	}
	
}

using namespace slib;

@implementation SLIBXGPush

- (void)xgPushDidFinishStart:(BOOL)isSuccess error:(nullable NSError *)error
{
	if (error != nil) {
		LogError("XgPush", "Error: %s", Apple::getStringFromNSString([error localizedDescription]));
	}
}

- (void)xgPushDidRegisteredDeviceToken:(nullable NSString *)deviceToken error:(nullable NSError *)error
{
	if (error != nil) {
		LogError("XgPush", "Error: %s", Apple::getStringFromNSString([error localizedDescription]));
	} else {
		String token = Apple::getStringFromNSString(deviceToken);
		XgPush::getInstance()->dispatchRefreshToken(token);
	}
}

- (void)xgPushDidReceiveRemoteNotification:(id)notification withCompletionHandler:(void (^)(NSUInteger))completionHandler {
	if ([notification isKindOfClass:[NSDictionary class]]) {
		[[XGPush defaultManager] reportXGNotificationInfo:(NSDictionary *)notification];
		completionHandler(UIBackgroundFetchResultNewData);
	} else if ([notification isKindOfClass:[UNNotification class]]) {
		NSDictionary* userInfo = ((UNNotification *)notification).request.content.userInfo;
		[[XGPush defaultManager] reportXGNotificationInfo:userInfo];
		PushNotificationMessage message;
		if (UIPlatform::parseRemoteNotificationInfo(userInfo, message)) {
			message.flagClicked = sl_false;
			XgPush::getInstance()->dispatchReceiveMessage(message);
		}
		completionHandler(UNNotificationPresentationOptionBadge | UNNotificationPresentationOptionSound | UNNotificationPresentationOptionAlert);
	}
}

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
- (void)xgPushUserNotificationCenter:(UNUserNotificationCenter *)center
	  didReceiveNotificationResponse:(UNNotificationResponse *)response
			   withCompletionHandler:(void (^)(void))completionHandler
{
	[[XGPush defaultManager] reportXGNotificationResponse:response];
	PushNotificationMessage message;
	NSDictionary* userInfo = response.notification.request.content.userInfo;
	if (UIPlatform::parseRemoteNotificationInfo(userInfo, message)) {
		message.flagClicked = sl_true;
		XgPush::getInstance()->dispatchReceiveMessage(message);
	}
	completionHandler();
}
#endif

@end

#endif