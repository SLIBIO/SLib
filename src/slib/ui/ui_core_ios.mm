/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
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

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/core.h"
#include "slib/ui/screen.h"
#include "slib/ui/platform.h"
#include "slib/ui/mobile_app.h"
#include "slib/ui/notification.h"

#include "slib/core/log.h"
#include "slib/core/json.h"
#include "slib/core/variant.h"
#include "slib/core/safe_static.h"
#include "slib/core/platform_apple.h"

#include <UserNotifications/UserNotifications.h>

@interface _priv_Slib_iOS_AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

namespace slib
{
	class _iOS_Screen : public Screen
	{
	public:
		UIScreen* m_screen;
		
	public:
		static Ref<_iOS_Screen> create(UIScreen* screen)
		{
			Ref<_iOS_Screen> ret;
			if (screen != nil) {
				ret = new _iOS_Screen();
				if (ret.isNotNull()) {
					ret->m_screen = screen;
				}
			}
			return ret;
		}
		
		static UIScreen* getPrimaryScreen()
		{
			NSArray* arr = [UIScreen screens];
			sl_size n = [arr count];
			if (n == 0) {
				return nil;
			}
			UIScreen* primary = [arr objectAtIndex:0];
			return primary;
		}
		
		UIRect getRegion() override
		{
			CGRect rect = [m_screen bounds];
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			UIRect region;
			region.left = (sl_ui_pos)(rect.origin.x * f);
			region.top = (sl_ui_pos)(rect.origin.y * f);
			region.setWidth((sl_ui_pos)(rect.size.width * f));
			region.setHeight((sl_ui_pos)(rect.size.height * f));
			return region;
		}
		
	};
	
	Ref<Screen> UIPlatform::createScreen(UIScreen* screen)
	{
		return _iOS_Screen::create(screen);
	}
	
	UIScreen* UIPlatform::getScreenHandle(Screen* _screen)
	{
		_iOS_Screen* screen = (_iOS_Screen*)_screen;
		if (screen) {
			return screen->m_screen;
		}
		return nil;
	}
	
	List< Ref<Screen> > UI::getScreens()
	{
		List< Ref<Screen> > ret;
		NSArray* arr = [UIScreen screens];
		sl_size n = [arr count];
		if (n == 0) {
			return ret;
		}
		for (sl_size i = 0; i < n; i++) {
			UIScreen* _screen = [arr objectAtIndex:i];
			ret.add_NoLock(_iOS_Screen::create(_screen));
		}
		return ret;
	}
	
	Ref<Screen> UI::getPrimaryScreen()
	{
		UIScreen* screen = _iOS_Screen::getPrimaryScreen();
		return UIPlatform::createScreen(screen);
	}
	
	Ref<Screen> UI::getFocusedScreen()
	{
		return getPrimaryScreen();
	}
	
	extern UIInterfaceOrientation _g_slib_ui_screen_orientation;
	
	ScreenOrientation UI::getScreenOrientation()
	{
		switch (_g_slib_ui_screen_orientation) {
			case UIInterfaceOrientationPortrait:
				return ScreenOrientation::Portrait;
			case UIInterfaceOrientationPortraitUpsideDown:
				return ScreenOrientation::PortraitUpsideDown;
			case UIInterfaceOrientationLandscapeLeft:
				return ScreenOrientation::LandscapeLeft;
			case UIInterfaceOrientationLandscapeRight:
				return ScreenOrientation::LandscapeRight;
			default:
				break;
		}
		return ScreenOrientation::Portrait;
	}
	
	void _priv_slib_ui_reset_orienation()
	{
		_g_slib_ui_screen_orientation = [[UIApplication sharedApplication] statusBarOrientation];
	}
	
	void UI::openUrl(const String& _url)
	{
		if (_url.isNotEmpty()) {
			NSString* s = Apple::getNSStringFromString(_url);
			NSURL* url = [NSURL URLWithString:s];
			if (![NSThread isMainThread]) {
				dispatch_async(dispatch_get_main_queue(), ^{
					[[UIApplication sharedApplication] openURL:url];
				});
			} else {
				[[UIApplication sharedApplication] openURL:url];
			}
		}
	}
	
	void UI::dismissKeyboard()
	{
		UIWindow* window = UIPlatform::getKeyWindow();
		if (window != nil) {
			if (![NSThread isMainThread]) {
				dispatch_async(dispatch_get_main_queue(), ^{
					[window endEditing:YES];
				});
			} else {
				[window endEditing:YES];
			}
		}
	}
	
	sl_ui_len UI::getScreenStatusBarHeight()
	{
		CGRect rectOfStatusbar = [[UIApplication sharedApplication] statusBarFrame];
		return (sl_ui_len)(rectOfStatusbar.size.height * UIPlatform::getGlobalScaleFactor());
	}
	
	sl_uint32 UI::getBadgeNumber()
	{
		NSInteger number = [[UIApplication sharedApplication] applicationIconBadgeNumber];
		if (number > 0) {
			return (sl_uint32)number;
		}
		return 0;
	}
	
	void UI::setBadgeNumber(sl_uint32 number)
	{
		[[UNUserNotificationCenter currentNotificationCenter] requestAuthorizationWithOptions:UNAuthorizationOptionBadge completionHandler:^(BOOL granted, NSError* error) {
			if (granted) {
				dispatch_async(dispatch_get_main_queue(), ^{
					[[UIApplication sharedApplication] setApplicationIconBadgeNumber:number];
				});
			}
		}];
	}
	
	void UIPlatform::runLoop(sl_uint32 level)
	{
		@autoreleasepool {
			CFRunLoopRun();
		}
	}
	
	void UIPlatform::quitLoop()
	{
		CFRunLoopStop(CFRunLoopGetCurrent());
	}
	
	void UIPlatform::runApp()
	{
		char* args[] = {sl_null};
		@autoreleasepool {
			UIApplicationMain(0, args, nil, NSStringFromClass([_priv_Slib_iOS_AppDelegate class]));
		}
	}
	
	void UIPlatform::quitApp()
	{
	}
	
	UIWindow* UIPlatform::getMainWindow()
	{
		_priv_Slib_iOS_AppDelegate* app = (_priv_Slib_iOS_AppDelegate*)([[UIApplication sharedApplication] delegate]);
		if (app != nil) {
			return app.window;
		}
		return nil;
	}
	
	UIWindow* UIPlatform::getKeyWindow()
	{
		UIWindow* window = [[UIApplication sharedApplication] keyWindow];
		if (window != nil) {
			return window;
		}
		return UIPlatform::getMainWindow();
	}
	
	UIView* UIPlatform::findFirstResponder(UIView* root)
	{
		if (root.isFirstResponder) {
			return root;
		}
		for (UIView* subView in root.subviews) {
			UIView* v = findFirstResponder(subView);
			if (v != nil) {
				return v;
			}
		}
		return nil;
	}
	
	CGFloat _g_slib_ios_global_scale_factor = 0;
	
	CGFloat UIPlatform::getGlobalScaleFactor()
	{
		if (_g_slib_ios_global_scale_factor == 0) {
			UIScreen* screen = _iOS_Screen::getPrimaryScreen();
			if (screen != nil) {
				CGFloat f = screen.scale;
				_g_slib_ios_global_scale_factor = f;
				return f;
			} else {
				return 1;
			}
		}
		return _g_slib_ios_global_scale_factor;
	}
	
	void UIPlatform::setGlobalScaleFactor(CGFloat factor)
	{
		_g_slib_ios_global_scale_factor = factor;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<void(NSDictionary*)> >, _g_slib_ios_callbacks_didReceiveRemoteNotification);
	
	void UIPlatform::registerDidReceiveRemoteNotificationCallback(const Function<void(NSDictionary*)>& callback)
	{
		_g_slib_ios_callbacks_didReceiveRemoteNotification.add(callback);
	}
	
}

@implementation _priv_Slib_iOS_AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	
	slib::Log("App", "Finished Launching");
	
	slib::_priv_slib_ui_reset_orienation();
	
	slib::UIApp::dispatchStartToApp();
	
	slib::MobileApp::dispatchCreateActivityToApp();
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	
	slib::Log("App", "Resign Active");

	slib::MobileApp::dispatchPauseToApp();

}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	slib::Log("App", "Enter Background");

}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	
	slib::Log("App", "Enter Foreground");

	slib::_priv_slib_ui_reset_orienation();

}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.

	slib::Log("App", "Become Active");

	slib::_priv_slib_ui_reset_orienation();
	
	slib::MobileApp::dispatchResumeToApp();
}

- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.

	slib::Log("App", "Terminate");
	
	slib::MobileApp::dispatchDestroyActivityToApp();

	slib::UIApp::dispatchExitToApp();
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(nullable UIWindow *)window
{
	UIInterfaceOrientationMask mask = 0;
	for (slib::ScreenOrientation value : slib::UI::getAvailableScreenOrientations()) {
		switch (value) {
			case slib::ScreenOrientation::Portrait:
				mask |= UIInterfaceOrientationMaskPortrait;
				break;
			case slib::ScreenOrientation::LandscapeRight:
				mask |= UIInterfaceOrientationMaskLandscapeRight;
				break;
			case slib::ScreenOrientation::PortraitUpsideDown:
				mask |= UIInterfaceOrientationMaskPortraitUpsideDown;
				break;
			case slib::ScreenOrientation::LandscapeLeft:
				mask |= UIInterfaceOrientationMaskLandscapeLeft;
				break;
		}
	}
	if (mask) {
		return mask;
	} else {
		return UIInterfaceOrientationMaskAll;
	}	
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
	slib::String token = slib::String::makeHexString([deviceToken bytes], [deviceToken length]);
	slib::PushNotification::_onRefreshToken(token);
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	slib::PushNotification::_onRefreshToken(sl_null);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(nonnull void (^)(UIBackgroundFetchResult))completionHandler {
	
	for (auto& callback : slib::_g_slib_ios_callbacks_didReceiveRemoteNotification) {
		callback(userInfo);
	}
	completionHandler(UIBackgroundFetchResultNewData);
}

@end


#endif
