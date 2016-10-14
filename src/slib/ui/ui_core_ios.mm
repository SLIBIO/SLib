#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/mobile_app.h"

#include "../../../inc/slib/core/log.h"

@interface _slib_iOS_AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

SLIB_UI_NAMESPACE_BEGIN

class _iOS_Screen : public Screen
{
public:
	UIScreen* m_screen;
	CGRect m_rect;
	
public:
	static Ref<_iOS_Screen> create(UIScreen* screen)
	{
		Ref<_iOS_Screen> ret;
		if (screen != nil) {
			ret = new _iOS_Screen();
			if (ret.isNotNull()) {
				ret->m_screen = screen;
				ret->m_rect = [screen bounds];
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
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		UIRect region;
		region.left = (sl_ui_pos)(m_rect.origin.x * f);
		region.top = (sl_ui_pos)(m_rect.origin.y * f);
		region.setWidth((sl_ui_pos)(m_rect.size.width * f));
		region.setHeight((sl_ui_pos)(m_rect.size.height * f));
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

sl_bool UI::isUiThread()
{
	return [NSThread isMainThread];
}

void UI::dispatchToUiThread(const Ref<Runnable>& _callback)
{
    Ref<Runnable> callback = _callback;
	if (callback.isNotNull()) {
		dispatch_async(dispatch_get_main_queue(), ^{
			callback->run();
		});
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
	@autoreleasepool {
		UIApplicationMain(0, nil, nil, NSStringFromClass([_slib_iOS_AppDelegate class]));
	}
}

void UIPlatform::quitApp()
{
}

__weak UIWindow* _g_slib_ios_keyWindow = nil;
UIWindow* UIPlatform::getMainWindow()
{
	_slib_iOS_AppDelegate* app = [[UIApplication sharedApplication] delegate];
	if (app != nil) {
		return app.window;
	}
	return nil;
}

UIWindow* UIPlatform::getKeyWindow()
{
	UIWindow* window = _g_slib_ios_keyWindow;
	if (window == nil) {
		UIApplication* app = [UIApplication sharedApplication];
		for (UIWindow* win in app.windows) {
			if (win.windowLevel > UIWindowLevelNormal) {
				window = win;
				[win makeKeyAndVisible];
				break;
			}
		}
		return UIPlatform::getMainWindow();
	}
	return window;
}

void UIPlatform::setKeyWindow(UIWindow *window)
{
	_g_slib_ios_keyWindow = window;
}

CGFloat _g_slib_ios_global_scale_factor = 0;

CGFloat UIPlatform::getGlobalScaleFactor()
{
	if (_g_slib_ios_global_scale_factor == 0) {
		UIScreen* screen = _iOS_Screen::getPrimaryScreen();
		if (screen != nil) {
			CGFloat f = screen.nativeScale;
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

SLIB_UI_NAMESPACE_END

@implementation _slib_iOS_AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	
	SLIB_LOG("App", "Finished Launching");

	slib::UIApp::dispatchStartToApp();
	
	slib::MobileApp::dispatchCreateActivityToApp();
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	
	SLIB_LOG("App", "Resign Active");

	slib::MobileApp::dispatchPauseToApp();

}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	
	SLIB_LOG("App", "Enter Background");

}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	
	SLIB_LOG("App", "Enter Foreground");

}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.

	SLIB_LOG("App", "Become Active");

	slib::MobileApp::dispatchResumeToApp();
}

- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.

	SLIB_LOG("App", "Terminate");
	
	slib::MobileApp::dispatchDestroyActivityToApp();

	slib::UIApp::dispatchExitToApp();
}

@end


#endif
