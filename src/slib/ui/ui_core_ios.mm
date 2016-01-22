#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/core/log.h"

@interface _slib_iOS_AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

SLIB_UI_NAMESPACE_BEGIN

class _iOS_Screen : public Screen
{
public:
	UIScreen* m_screen;
	Rectangle m_region;
	
public:
	static Ref<_iOS_Screen> create(UIScreen* screen)
	{
		Ref<_iOS_Screen> ret;
		if (screen != nil) {
			ret = new _iOS_Screen();
			if (ret.isNotNull()) {
				ret->m_screen = screen;
				Rectangle region;
				CGRect rect = [screen bounds];
				region.left = (sl_real)(rect.origin.x);
				region.top = (sl_real)(rect.origin.y);
				region.setWidth((sl_real)(rect.size.width));
				region.setHeight((sl_real)(rect.size.height));
				ret->m_region = region;
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
    Rectangle getRegion()
	{
		return m_region;
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

sl_bool UI::isUIThread()
{
	return [NSThread isMainThread];
}


void UI::runOnUIThread(const Ref<Runnable>& _callback)
{
    Ref<Runnable> callback = _callback;
	if (callback.isNotNull()) {
		dispatch_async(dispatch_get_main_queue(), ^{
			callback->run();
		});
	}
}

void UI::runLoop()
{
	@autoreleasepool {
		UIApplicationMain(0, nil, nil, NSStringFromClass([_slib_iOS_AppDelegate class]));
	}
}

void UI::quitLoop()
{
}

void UI::showAlert(const AlertParam& param)
{
	AlertType type = param.type;
	NSString* caption = Apple::getNSStringFromString(param.caption);
	NSString* text = Apple::getNSStringFromString(param.text);
	NSString* titleOk = Apple::getNSStringFromString(param.titleOk);
	if ([titleOk length] == 0) {
		titleOk = @"OK";
	}
	NSString* titleCancel = Apple::getNSStringFromString(param.titleCancel);
	if ([titleCancel length] == 0) {
		titleCancel = @"Cancel";
	}
	NSString* titleYes = Apple::getNSStringFromString(param.titleYes);
	if ([titleYes length] == 0) {
		titleYes = @"Yes";
	}
	NSString* titleNo = Apple::getNSStringFromString(param.titleNo);
	if ([titleNo length] == 0) {
		titleNo = @"No";
	}
	Ref<Runnable> onOk = param.onOk;
	Ref<Runnable> onCancel = param.onCancel;
	Ref<Runnable> onYes = param.onYes;
	Ref<Runnable> onNo = param.onNo;
	
	dispatch_async(dispatch_get_main_queue(), ^{
		
		UIAlertController* alert = [UIAlertController alertControllerWithTitle:caption message:text preferredStyle:UIAlertControllerStyleAlert];
		if (type == alertType_OkCancel) {
			UIAlertAction* actionOK = [UIAlertAction actionWithTitle:titleOk style:UIAlertActionStyleDefault handler:
									   ^(UIAlertAction *) {
										   [alert dismissViewControllerAnimated:YES completion:nil];
										   if (onOk.isNotNull()) {
											   onOk->run();
										   }
									   }];
			UIAlertAction* actionCancel = [UIAlertAction actionWithTitle:titleCancel style:UIAlertActionStyleCancel handler:
										   ^(UIAlertAction *) {
											   [alert dismissViewControllerAnimated:YES completion:nil];
											   if (onCancel.isNotNull()) {
												   onCancel->run();
											   }
										   }];
			[alert addAction:actionOK];
			[alert addAction:actionCancel];
		} else if (type == alertType_YesNo) {
			UIAlertAction* actionYes = [UIAlertAction actionWithTitle:titleYes style:UIAlertActionStyleDefault handler:
									   ^(UIAlertAction *) {
										   [alert dismissViewControllerAnimated:YES completion:nil];
										   if (onYes.isNotNull()) {
											   onYes->run();
										   }
									   }];
			UIAlertAction* actionNo = [UIAlertAction actionWithTitle:titleNo style:UIAlertActionStyleDestructive handler:
										   ^(UIAlertAction *) {
											   [alert dismissViewControllerAnimated:YES completion:nil];
											   if (onNo.isNotNull()) {
												   onNo->run();
											   }
										   }];
			[alert addAction:actionYes];
			[alert addAction:actionNo];
		} else if (type == alertType_YesNoCancel) {
			UIAlertAction* actionYes = [UIAlertAction actionWithTitle:titleYes style:UIAlertActionStyleDefault handler:
										^(UIAlertAction *) {
											[alert dismissViewControllerAnimated:YES completion:nil];
											if (onYes.isNotNull()) {
												onYes->run();
											}
										}];
			UIAlertAction* actionNo = [UIAlertAction actionWithTitle:titleNo style:UIAlertActionStyleDestructive handler:
									   ^(UIAlertAction *) {
										   [alert dismissViewControllerAnimated:YES completion:nil];
										   if (onNo.isNotNull()) {
											   onNo->run();
										   }
									   }];
			UIAlertAction* actionCancel = [UIAlertAction actionWithTitle:titleCancel style:UIAlertActionStyleCancel handler:
										   ^(UIAlertAction *) {
											   [alert dismissViewControllerAnimated:YES completion:nil];
											   if (onCancel.isNotNull()) {
												   onCancel->run();
											   }
										   }];
			[alert addAction:actionYes];
			[alert addAction:actionNo];
			[alert addAction:actionCancel];
		} else {
			UIAlertAction* actionOK = [UIAlertAction actionWithTitle:titleOk style:UIAlertActionStyleCancel handler:
									   ^(UIAlertAction *) {
										   [alert dismissViewControllerAnimated:YES completion:nil];
										   if (onOk.isNotNull()) {
											   onOk->run();
										   }
									   }];
			[alert addAction:actionOK];
		}
		
		UIWindow* window = UIPlatform::getKeyWindow();
		if (window != nil) {
			UIViewController* rootController = [window rootViewController];
			[rootController presentViewController:alert animated:YES completion:nil];
		}
	});
}

UIColor* UIPlatform::getUIColorFromColor(const Color& color)
{
	return [UIColor colorWithRed:color.getRedF() green:color.getGreenF() blue:color.getBlueF() alpha:color.getAlphaF()];
}

Color UIPlatform::getColorFromUIColor(UIColor* color)
{
	if (color == nil) {
		return Color::zero();
	}
	CGFloat red, green, blue, alpha;
	[color getRed:&red green:&green blue:&blue alpha:&alpha];
	return Color4f((sl_real)red, (sl_real)green, (sl_real)blue, (sl_real)alpha);
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

SLIB_UI_NAMESPACE_END

@implementation _slib_iOS_AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	
	SLIB_LOG("App", "Finished Launching");

	slib::UIApp::dispatchStart();
	
	slib::UIApp::dispatchMobileCreate();
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	
	SLIB_LOG("App", "Resign Active");

	slib::UIApp::dispatchMobilePause();

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

	slib::UIApp::dispatchMobileResume();
}

- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.

	SLIB_LOG("App", "Terminate");

	slib::UIApp::dispatchExit();
}

@end


#endif
