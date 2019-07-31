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

@interface SLIBAppDelegate : UIResponder <UIApplicationDelegate>
	@property (strong, nonatomic) UIWindow *window;
@end

namespace slib
{
	
	namespace priv
	{

		namespace window
		{
			void resetOrientation();
		}

		using namespace window;

		namespace ui_core
		{
			
			CGFloat g_fGlobalScaleFactor = 0;
			
			SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void(NSDictionary*)>, g_callbackDidFinishLaunching);
			SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void(NSData*, NSError*)>, g_callbackDidRegisterForRemoteNotifications);
			SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void(NSDictionary*)>, g_callbackDidReceiveRemoteNotification);
			SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<BOOL(NSURL*, NSDictionary*)> >, g_callbackOpenURL);

			class ScreenImpl : public Screen
			{
			public:
				UIScreen* m_screen;
				
			public:
				static Ref<ScreenImpl> create(UIScreen* screen)
				{
					Ref<ScreenImpl> ret;
					if (screen != nil) {
						ret = new ScreenImpl();
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
			
			static BOOL UIPlatform_onOpenUrl(NSURL* url, NSDictionary* options)
			{
				for (auto& callback : g_callbackOpenURL) {
					if (callback(url, options)) {
						return YES;
					}
				}
				return MobileApp::dispatchOpenUrlToApp(Apple::getStringFromNSString(url.absoluteString));
			}
			
		}
	}
	
	using namespace priv::ui_core;
	
	Ref<Screen> UIPlatform::createScreen(UIScreen* screen)
	{
		return ScreenImpl::create(screen);
	}
	
	UIScreen* UIPlatform::getScreenHandle(Screen* _screen)
	{
		ScreenImpl* screen = (ScreenImpl*)_screen;
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
			ret.add_NoLock(ScreenImpl::create(_screen));
		}
		return ret;
	}
	
	Ref<Screen> UI::getPrimaryScreen()
	{
		UIScreen* screen = ScreenImpl::getPrimaryScreen();
		return UIPlatform::createScreen(screen);
	}
	
	Ref<Screen> UI::getFocusedScreen()
	{
		return getPrimaryScreen();
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
	
	UIEdgeInsets UI::getSafeAreaInsets()
	{
		if (@available(iOS 12.0, *)) {
			UIWindow* window = UIPlatform::getMainWindow();
			if (window != nil) {
				::UIEdgeInsets insets = window.safeAreaInsets;
				UIEdgeInsets ret;
				ret.left = (sl_ui_len)(insets.left * UIPlatform::getGlobalScaleFactor());
				ret.top = (sl_ui_len)(insets.top * UIPlatform::getGlobalScaleFactor());
				ret.right = (sl_ui_len)(insets.right * UIPlatform::getGlobalScaleFactor());
				ret.bottom = (sl_ui_len)(insets.bottom * UIPlatform::getGlobalScaleFactor());
				return ret;
			}
		}
		UIEdgeInsets ret;
		ret.left = 0;
		ret.top = getStatusBarHeight();
		ret.right = 0;
		ret.bottom = 0;
		return ret;
	}

	sl_ui_len UI::getStatusBarHeight()
	{
		CGRect rectOfStatusbar = [[UIApplication sharedApplication] statusBarFrame];
		return (sl_ui_len)(rectOfStatusbar.size.height * UIPlatform::getGlobalScaleFactor());
	}
	
	sl_bool UI::isUiThread()
	{
		return [NSThread isMainThread];
	}
	
	void UI::dispatchToUiThread(const Function<void()>& _callback, sl_uint32 delayMillis)
	{
		Function<void()> callback = _callback;
		if (callback.isNotNull()) {
			if (delayMillis == 0) {
				dispatch_async(dispatch_get_main_queue(), ^{
					callback();
				});
			} else {
				dispatch_time_t t = dispatch_time(DISPATCH_TIME_NOW, (sl_int64)(delayMillis) * NSEC_PER_MSEC);
				dispatch_after(t, dispatch_get_main_queue(), ^{
					callback();
				});
			}
		}
	}
	
	void UIPlatform::runLoop(sl_uint32 level)
	{
		CFRunLoopRun();
	}
	
	void UIPlatform::quitLoop()
	{
		CFRunLoopStop(CFRunLoopGetCurrent());
	}
	
	void UIPlatform::runApp()
	{		
		char* args[] = {sl_null};
		@autoreleasepool {
			UIApplicationMain(0, args, nil, NSStringFromClass([SLIBAppDelegate class]));
		}
	}
	
	void UIPlatform::quitApp()
	{
	}
	
	UIWindow* UIPlatform::getMainWindow()
	{
		SLIBAppDelegate* app = (SLIBAppDelegate*)([[UIApplication sharedApplication] delegate]);
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
	
	UIViewController* UIPlatform::getCurrentViewController()
	{
		return getCurrentViewController(Ref<Window>::null());
	}
	
	UIViewController* UIPlatform::getCurrentViewController(const Ref<Window>& _parentWindow)
	{
		Ref<Window> parentWindow = _parentWindow;
		UIWindow* window = nil;
		if (parentWindow.isNull()) {
			Ref<MobileApp> app = MobileApp::getApp();
			if (app.isNotNull()) {
				parentWindow = app->getMainWindow();
			}
		}
		if (parentWindow.isNotNull()) {
			Ref<WindowInstance> instance = parentWindow->getWindowInstance();
			UIView* view = UIPlatform::getWindowHandle(instance.get());
			if ([view isKindOfClass:[UIWindow class]]) {
				window = (UIWindow*)view;
			}
		}
		if (window == nil) {
			window = UIPlatform::getKeyWindow();
		}
		if (window != nil) {
			return [window rootViewController];
		}
		return nil;
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
	
	
	CGFloat UIPlatform::getGlobalScaleFactor()
	{
		if (g_fGlobalScaleFactor == 0) {
			UIScreen* screen = ScreenImpl::getPrimaryScreen();
			if (screen != nil) {
				CGFloat f = screen.scale;
				g_fGlobalScaleFactor = f;
				return f;
			} else {
				return 1;
			}
		}
		return g_fGlobalScaleFactor;
	}
	
	void UIPlatform::setGlobalScaleFactor(CGFloat factor)
	{
		g_fGlobalScaleFactor = factor;
	}
	
	void UIPlatform::registerDidFinishLaunchingCallback(const Function<void(NSDictionary*)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackDidFinishLaunching)) {
			return;
		}
		g_callbackDidFinishLaunching.add(callback);
	}
	

	void UIPlatform::registerDidRegisterForRemoteNotifications(const Function<void(NSData*, NSError*)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackDidRegisterForRemoteNotifications)) {
			return;
		}
		g_callbackDidRegisterForRemoteNotifications.add(callback);
	}
	
	void UIPlatform::registerDidReceiveRemoteNotificationCallback(const Function<void(NSDictionary*)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackDidReceiveRemoteNotification)) {
			return;
		}
		g_callbackDidReceiveRemoteNotification.add(callback);
	}
	
	void UIPlatform::registerOpenUrlCallback(const Function<BOOL(NSURL*, NSDictionary*)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackOpenURL)) {
			return;
		}
		g_callbackOpenURL.add(callback);
	}
	
}

using namespace slib::priv::ui_core;
using namespace slib::priv::window;

@implementation SLIBAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	
	slib::Log("App", "Finished Launching");
	
	resetOrientation();
	
	slib::UIApp::dispatchStartToApp();
	
	g_callbackDidFinishLaunching(launchOptions);
	
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

	resetOrientation();

}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.

	slib::Log("App", "Become Active");

	resetOrientation();
	
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
	g_callbackDidRegisterForRemoteNotifications(deviceToken, nil);
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	g_callbackDidRegisterForRemoteNotifications(nil, error);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(nonnull void (^)(UIBackgroundFetchResult))completionHandler {
	
	g_callbackDidReceiveRemoteNotification(userInfo);
	completionHandler(UIBackgroundFetchResultNewData);
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url options:(NSDictionary *)options {
	return UIPlatform_onOpenUrl(url, options);
}

- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> * _Nullable))restorationHandler
{
	if ([userActivity.activityType isEqualToString:NSUserActivityTypeBrowsingWeb]) {
		NSURL* url = userActivity.webpageURL;
		return UIPlatform_onOpenUrl(url, @{});
	}
	return NO;
}

@end


#endif
