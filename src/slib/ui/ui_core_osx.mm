#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/app.h"

@interface _slib_OSX_AppDelegate : NSObject <NSApplicationDelegate>
@end

SLIB_UI_NAMESPACE_BEGIN

class _OSX_Screen : public Screen
{
public:
	NSScreen* m_screen;
	UIRect m_region;
	
public:
	static Ref<_OSX_Screen> create(NSScreen* screen, NSScreen* primary)
	{
		Ref<_OSX_Screen> ret;
		if (screen != nil) {
			ret = new _OSX_Screen();
			if (ret.isNotNull()) {
				ret->m_screen = screen;
				UIRect region;
				sl_ui_pos leftBottom = 0;
				if (primary == nil) {
					primary = getPrimaryScreen();
				}
				if (primary != nil) {
					NSRect rect = [primary frame];
					leftBottom = (sl_ui_pos)(rect.origin.y + rect.size.height);
				}
				NSRect rect = [screen frame];
				region.left = (sl_ui_pos)(rect.origin.x);
				region.top = leftBottom - (sl_ui_pos)(rect.origin.y + rect.size.height);
				region.setWidth((sl_ui_pos)(rect.size.width));
				region.setHeight((sl_ui_pos)(rect.size.height));
				ret->m_region = region;
			}
		}
		return ret;
	}
	
	static NSScreen* getPrimaryScreen()
	{
		NSArray* arr = [NSScreen screens];
		sl_size n = [arr count];
		if (n == 0) {
			return nil;
		}
		NSScreen* primary = [arr objectAtIndex:0];
		return primary;
	}
	
public:
	// override
	UIRect getRegion()
	{
		return m_region;
	}

};

Ref<Screen> UIPlatform::createScreen(NSScreen* screen)
{
	return _OSX_Screen::create(screen, nil);
}

NSScreen* UIPlatform::getScreenHandle(Screen* _screen)
{
	_OSX_Screen* screen = (_OSX_Screen*)_screen;
	if (screen) {
		return screen->m_screen;
	}
	return nil;
}

float UIPlatform::getDpiForScreen(NSScreen* screen)
{
	NSDictionary *description = [screen deviceDescription];
	NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
	CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);
	return displayPixelSize.width / displayPhysicalSize.width * 25.4f;
}

List< Ref<Screen> > UI::getScreens()
{
	List< Ref<Screen> > ret;
	NSArray* arr = [NSScreen screens];
	sl_size n = [arr count];
	if (n == 0) {
		return ret;
	}
	NSScreen* primary = [arr objectAtIndex:0];
	for (sl_size i = 0; i < n; i++) {
		NSScreen* _screen = [arr objectAtIndex:i];
		ret.add_NoLock(_OSX_Screen::create(_screen, primary));
	}
	return ret;
}

Ref<Screen> UI::getPrimaryScreen()
{
	NSScreen* screen = _OSX_Screen::getPrimaryScreen();
	return UIPlatform::createScreen(screen);
}

Ref<Screen> UI::getFocusedScreen()
{
	NSScreen* screen = [NSScreen mainScreen];
	return UIPlatform::createScreen(screen);
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
	while (1) {
		NSEvent* ev = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
		if ([ev type] == NSApplicationDefined) {
			if ([ev subtype] == NSPowerOffEventType) {
				break;
			}
		}
		[NSApp sendEvent:ev];
	}
}

void UIPlatform::quitLoop()
{
	NSEvent* ev = [NSEvent otherEventWithType: NSApplicationDefined
										location: NSMakePoint(0,0)
								   modifierFlags: 0
									   timestamp: 0.0
									windowNumber: 0
										 context: nil
										 subtype: NSPowerOffEventType
										   data1: 0
										   data2: 0];
	[NSApp postEvent:ev atStart:YES];
}

void UIPlatform::runApp()
{
	[NSApplication sharedApplication];
	[NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
	_slib_OSX_AppDelegate * delegate = [[_slib_OSX_AppDelegate alloc] init];
	[NSApp setDelegate:delegate];
	
	[NSApp run];
}

void UIPlatform::quitApp()
{
	Ref<Application> app = Application::getApp();
	if (app.isNotNull()) {
		app->dispatchQuitApp();
	}
	dispatch_async(dispatch_get_main_queue(), ^{
		[NSApp terminate:nil];
	});
}

SLIB_UI_NAMESPACE_END

@implementation _slib_OSX_AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	slib::UIApp::dispatchStartToApp();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	slib::UIApp::dispatchExitToApp();
}
@end


#endif
