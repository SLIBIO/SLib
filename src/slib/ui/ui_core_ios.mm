#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/mobile_app.h"
#include "../../../inc/slib/ui/notification.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/variant.h"
#include "../../../inc/slib/core/platform_apple.h"

@interface _slib_iOS_AppDelegate : UIResponder <UIApplicationDelegate>
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
		
		// override
		UIRect getRegion()
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
	
	void UI::openUrl(const String& _url)
	{
		if (![NSThread isMainThread]) {
			String __url = _url;
			dispatch_async(dispatch_get_main_queue(), ^{
				openUrl(__url);
			});
			return;
		}
		
		if (_url.isNotEmpty()) {
			NSString* s = Apple::getNSStringFromString(_url);
			NSURL* url = [NSURL URLWithString:s];
			[[UIApplication sharedApplication] openURL:url];
		}
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
		@autoreleasepool {
			UIApplicationMain(0, nil, nil, NSStringFromClass([_slib_iOS_AppDelegate class]));
		}
	}
	
	void UIPlatform::quitApp()
	{
	}
	
	UIWindow* UIPlatform::getMainWindow()
	{
		_slib_iOS_AppDelegate* app = (_slib_iOS_AppDelegate*)([[UIApplication sharedApplication] delegate]);
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
}

@implementation _slib_iOS_AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	
	slib::Log("App", "Finished Launching");
	
	slib::UIApp::dispatchStartToApp();
	
	[application registerForRemoteNotifications];
	
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

}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.

	slib::Log("App", "Become Active");

	slib::MobileApp::dispatchResumeToApp();
}

- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.

	slib::Log("App", "Terminate");
	
	slib::MobileApp::dispatchDestroyActivityToApp();

	slib::UIApp::dispatchExitToApp();
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
	const sl_uint8* bytes = (const sl_uint8* )[deviceToken bytes];
	slib::String tokenString = "";
	for (sl_uint32 i = 0; i < [deviceToken length]; i++) {
		tokenString += slib::String::format("%02x", bytes[i]);
	}
	
	slib::Function<void (slib::String)> callback = slib::PushNotification::getTokenRefreshCallback();
	if (callback.isNotNull()) {
		callback(tokenString);
	}
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	slib::Function<void (slib::String)> callback = slib::PushNotification::getTokenRefreshCallback();
	if (callback.isNotNull()) {
		callback(slib::String::null());
	}
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(nonnull void (^)(UIBackgroundFetchResult))completionHandler {
    NSError *err;
	NSData* jsonData = [NSJSONSerialization dataWithJSONObject:userInfo options:0 error:&err];
	NSString* jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
	if (jsonString) {
        NSDictionary* aps = userInfo[@"aps"];
        id alert;
        if (aps != nil) {
            alert = aps[@"alert"];
        }
        
		slib::Variant _userInfo = slib::Json::parseJson(slib::Apple::getStringFromNSString(jsonString));
        slib::PushNotificationMessage message;
        message.data = _userInfo.getVariantMap();
        
        if (alert) {
            if ([alert isKindOfClass:[NSString class]]) {
                message.title = slib::Apple::getStringFromNSString(alert);
            } else {
                NSString* title = alert[@"title"];
                NSString* body = alert[@"body"];
                message.title = slib::Apple::getStringFromNSString(title);
                message.body = slib::Apple::getStringFromNSString(body);
            }
        }
        
		slib::Function<void(slib::PushNotificationMessage&)> callback = slib::PushNotification::getNotificationReceivedCallback();
		if (callback.isNotNull()) {
			callback(message);
		}
	}
}

@end


#endif
