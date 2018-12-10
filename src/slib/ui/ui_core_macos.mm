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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/core.h"
#include "slib/ui/screen.h"
#include "slib/ui/window.h"
#include "slib/ui/platform.h"
#include "slib/ui/app.h"

@interface _priv_Slib_macOS_AppDelegate : NSObject <NSApplicationDelegate>
@end

namespace slib
{

	class _priv_macOS_Screen : public Screen
	{
	public:
		NSScreen* m_screen;
		UIRect m_region;
		
	public:
		static Ref<_priv_macOS_Screen> create(NSScreen* screen, NSScreen* primary)
		{
			Ref<_priv_macOS_Screen> ret;
			if (screen != nil) {
				ret = new _priv_macOS_Screen();
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
		UIRect getRegion() override
		{
			return m_region;
		}

	};

	Ref<Screen> UIPlatform::createScreen(NSScreen* screen)
	{
		return _priv_macOS_Screen::create(screen, nil);
	}

	NSScreen* UIPlatform::getScreenHandle(Screen* _screen)
	{
		_priv_macOS_Screen* screen = (_priv_macOS_Screen*)_screen;
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
			ret.add_NoLock(_priv_macOS_Screen::create(_screen, primary));
		}
		return ret;
	}

	Ref<Screen> UI::getPrimaryScreen()
	{
		NSScreen* screen = _priv_macOS_Screen::getPrimaryScreen();
		return UIPlatform::createScreen(screen);
	}

	Ref<Screen> UI::getFocusedScreen()
	{
		NSScreen* screen = [NSScreen mainScreen];
		return UIPlatform::createScreen(screen);
	}

	void UI::openUrl(const String& _url)
	{
		if (_url.isNotEmpty()) {
			NSString* s = Apple::getNSStringFromString(_url);
			NSURL* url = [NSURL URLWithString:s];
			[[NSWorkspace sharedWorkspace] openURL:url];
		}
	}
	
	sl_uint32 UI::getBadgeNumber()
	{
		int number = [[[NSApp dockTile] badgeLabel] intValue];
		if (number > 0) {
			return (sl_uint32)number;
		}
		return number;
	}
	
	void UI::setBadgeNumber(sl_uint32 number)
	{
		[[NSApp dockTile] setBadgeLabel:[NSString stringWithFormat:@"%d", number]];
	}
	
	void UIPlatform::runLoop(sl_uint32 level)
	{
		while (1) {
			@autoreleasepool {
				NSDate* date = [NSDate dateWithTimeIntervalSinceNow:1000];
				NSEvent* ev = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:date inMode:NSDefaultRunLoopMode dequeue:YES];
				if (ev != nil) {
					if ([ev type] == NSApplicationDefined) {
						if ([ev subtype] == NSPowerOffEventType) {
							break;
						}
					}
					[NSApp sendEvent:ev];
				}
			}
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
		[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:NSApp topLevelObjects:nil];
		_priv_Slib_macOS_AppDelegate * delegate = [[_priv_Slib_macOS_AppDelegate alloc] init];
		[NSApp setDelegate:delegate];
		
		@autoreleasepool {
			[NSApp run];
		}
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

}

@implementation _priv_Slib_macOS_AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	slib::UIApp::dispatchStartToApp();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	slib::UIApp::dispatchExitToApp();
}
@end


#endif
