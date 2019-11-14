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

#include "slib/core/safe_static.h"

#include "ui_core_common.h"

@interface SLIBAppDelegate : NSObject <NSApplicationDelegate>
@end

namespace slib
{
	
	namespace priv
	{
		namespace ui_core
		{
			
			class ScreenImpl : public Screen
			{
			public:
				NSScreen* m_screen;
				UIRect m_region;
				
			public:
				static Ref<ScreenImpl> create(NSScreen* screen, NSScreen* primary)
				{
					Ref<ScreenImpl> ret;
					if (screen != nil) {
						ret = new ScreenImpl();
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
			
			class StaticContext
			{
			public:
				NSEvent* dispatchEvent;
				Function<void()> customMessageLoop;
				Function<void()> customQuitApp;
				
			public:
				StaticContext()
				{
					dispatchEvent = [NSEvent otherEventWithType:NSEventTypeApplicationDefined location:NSMakePoint(0, 0) modifierFlags:0 timestamp:0 windowNumber:0 context:nil subtype:0 data1:0 data2:0];
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)

			static Function<void()> GetCustomMessageLoop()
			{
				StaticContext* context = GetStaticContext();
				if (context) {
					return context->customMessageLoop;
				}
				return sl_null;
			}
			
			void SetCustomMessageLoop(const Function<void()>& func)
			{
				StaticContext* context = GetStaticContext();
				if (context) {
					context->customMessageLoop = func;
				}
			}
			
			static Function<void()> GetCustomQuitApp()
			{
				StaticContext* context = GetStaticContext();
				if (context) {
					return context->customQuitApp;
				}
				return sl_null;
			}
			
			void SetCustomQuitApp(const Function<void()>& func)
			{
				StaticContext* context = GetStaticContext();
				if (context) {
					context->customQuitApp = func;
				}
			}
			
		}
	}

	using namespace priv::ui_core;

	Ref<Screen> UIPlatform::createScreen(NSScreen* screen)
	{
		return ScreenImpl::create(screen, nil);
	}

	NSScreen* UIPlatform::getScreenHandle(Screen* _screen)
	{
		ScreenImpl* screen = (ScreenImpl*)_screen;
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
			ret.add_NoLock(ScreenImpl::create(_screen, primary));
		}
		return ret;
	}

	Ref<Screen> UI::getPrimaryScreen()
	{
		NSScreen* screen = ScreenImpl::getPrimaryScreen();
		return UIPlatform::createScreen(screen);
	}

	Ref<Screen> UI::getFocusedScreen()
	{
		NSScreen* screen = [NSScreen mainScreen];
		return UIPlatform::createScreen(screen);
	}

	void UI::setBadgeNumber(sl_uint32 number)
	{
		if (number) {
			[[NSApp dockTile] setBadgeLabel:[NSString stringWithFormat:@"%d", number]];
		} else {
			[[NSApp dockTile] setBadgeLabel:nil];
		}
	}
	
	sl_bool UI::isUiThread()
	{
		return [NSThread isMainThread];
	}
	
	void UI::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (callback.isNull()) {
			return;
		}
		StaticContext* context = GetStaticContext();
		if (!context) {
			return;
		}
		if (delayMillis == 0) {
			if (UIDispatcher::addCallback(callback)) {
				if ([NSThread isMainThread]) {
					[NSApp postEvent:context->dispatchEvent atStart:YES];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[NSApp postEvent:context->dispatchEvent atStart:YES];
					});
				}
			}
		} else {
			Function<void()> refCallback(callback);
			dispatch_time_t t = dispatch_time(DISPATCH_TIME_NOW, (sl_int64)(delayMillis) * NSEC_PER_MSEC);
			dispatch_after(t, dispatch_get_main_queue(), ^{
				if (UIDispatcher::addCallback(refCallback)) {
					[NSApp postEvent:context->dispatchEvent atStart:YES];
				}
			});
		}
	}
	
	void UI::dispatchToUiThreadUrgently(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		Function<void()> refCallback(callback);
		if (delayMillis == 0) {
			dispatch_async(dispatch_get_main_queue(), ^{
				refCallback();
			});
		} else {
			dispatch_time_t t = dispatch_time(DISPATCH_TIME_NOW, (sl_int64)(delayMillis) * NSEC_PER_MSEC);
			dispatch_after(t, dispatch_get_main_queue(), ^{
				refCallback();
			});
		}
	}

	void UIPlatform::runLoop(sl_uint32 level)
	{
		StaticContext* context = GetStaticContext();
		if (!context) {
			return;
		}
		while (1) {
			@autoreleasepool {
				NSDate* date = [NSDate dateWithTimeIntervalSinceNow:1000];
				NSEvent* ev = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:date inMode:NSDefaultRunLoopMode dequeue:YES];
				if (ev != nil) {
					if ([ev type] == NSApplicationDefined && [ev subtype] == NSPowerOffEventType) {
						break;
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
		
		SLIBAppDelegate * delegate = [[SLIBAppDelegate alloc] init];
		[NSApp setDelegate:delegate];

		[NSEvent addLocalMonitorForEventsMatchingMask:(NSApplicationDefinedMask | NSKeyDownMask) handler:^(NSEvent* event){
			NSEventType type = [event type];
			if (type == NSKeyDown) {
				NSEventModifierFlags modifiers = [event modifierFlags] & NSDeviceIndependentModifierFlagsMask;
				NSString* key = [event charactersIgnoringModifiers];
				if (modifiers == NSCommandKeyMask) {
					if ([key isEqualToString:@"x"]) {
						if ([NSApp sendAction:NSSelectorFromString(@"cut:") to:nil from:NSApp]) {
							return (NSEvent*)nil;
						}
					}
					else if ([key isEqualToString:@"c"]) {
						if ([NSApp sendAction:NSSelectorFromString(@"copy:") to:nil from:NSApp]) {
							return (NSEvent*)nil;
						}
					}
					else if ([key isEqualToString:@"v"]) {
						if ([NSApp sendAction:NSSelectorFromString(@"paste:") to:nil from:NSApp]) {
							return (NSEvent*)nil;
						}
					}
					else if ([key isEqualToString:@"z"]) {
						if ([NSApp sendAction:NSSelectorFromString(@"undo:") to:nil from:NSApp]) {
							return (NSEvent*)nil;
						}
					}
					else if ([key isEqualToString:@"a"]) {
						if ([NSApp sendAction:NSSelectorFromString(@"selectAll:") to:nil from:NSApp]) {
							return (NSEvent*)nil;
						}
					}
				}
				else if (modifiers == (NSCommandKeyMask | NSShiftKeyMask)) {
					if ([key isEqualToString:@"Z"]) {
						if ([NSApp sendAction:NSSelectorFromString(@"redo:") to:nil from:NSApp]) {
							return (NSEvent*)nil;
						}
					}
				}
			} else if (type == NSApplicationDefined) {
				UIDispatcher::processCallbacks();
				return (NSEvent*)nil;
			}
			return event;
		}];
		
		Function<void()> customMessageLoop = GetCustomMessageLoop();
		if (customMessageLoop.isNotNull()) {
			customMessageLoop();
		} else {
			@autoreleasepool {
				[NSApp run];
			}
		}
	}

	void UIPlatform::quitApp()
	{
		Ref<Application> app = Application::getApp();
		if (app.isNotNull()) {
			app->dispatchQuitApp();
		}
		dispatch_async(dispatch_get_main_queue(), ^{
			Function<void()> customQuitApp = GetCustomQuitApp();
			if (customQuitApp.isNotNull()) {
				customQuitApp();
			} else {
				[NSApp terminate:nil];
			}
		});
	}

	void UIApp::onExistingInstance()
	{
		String uid = getUniqueInstanceId();
		if (uid.isEmpty()) {
			return;
		}
		NSArray* arr = [NSRunningApplication runningApplicationsWithBundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
		if (arr.count > 0) {
			NSRunningApplication* app = [arr objectAtIndex:0];
			if ([app.bundleURL.absoluteString isEqualToString:([NSRunningApplication currentApplication].bundleURL.absoluteString)]) {
				[app activateWithOptions:NSApplicationActivateIgnoringOtherApps];
			} else {
				[[NSWorkspace sharedWorkspace] launchApplication:app.bundleURL.path];
			}
		} else {
			NSLog(@"Running application is not found! bundleId=%@", [[NSBundle mainBundle] bundleIdentifier]);
		}
	}

	sl_bool UIApp::isMenuBarVisible()
	{
		return [NSMenu menuBarVisible] == YES;
	}

	void UIApp::setMenuBarVisible(sl_bool flagVisible)
	{
		[NSMenu setMenuBarVisible:flagVisible ? YES : NO];
	}

	void UIApp::setVisibleOnDock(sl_bool flagVisible)
	{
		ProcessSerialNumber psn = { 0, kCurrentProcess };
		if (flagVisible) {
			TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		} else {
			TransformProcessType(&psn, kProcessTransformToUIElementApplication);
		}
	}

	void UIApp::activate(sl_bool flagIgnoreOtherApps)
	{
		[NSApp activateIgnoringOtherApps:(flagIgnoreOtherApps ? YES : NO)];
	}

}

using namespace slib;
using namespace slib::priv::ui_core;

@implementation SLIBAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	UIApp::dispatchStartToApp();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	UIApp::dispatchExitToApp();
}

- (void)application:(NSApplication *)application openURLs:(NSArray<NSURL*>*)urls
{
	List<String> list;
	for (NSURL* url : urls) {
		list.add_NoLock(Apple::getStringFromNSString(url.absoluteString));
	}
	if (list.getCount() == 1) {
		UIApp::dispatchOpenUrlToApp(list[0]);
	} else {
		UIApp::dispatchOpenUrlsToApp(list);
	}
}

- (BOOL)application:(NSApplication *)sender openFile:(NSString *)filename
{
	return UIApp::dispatchOpenFileToApp(Apple::getStringFromNSString(filename));
}

- (void)application:(NSApplication *)sender openFiles:(NSArray<NSString*>*)filenames
{
	List<String> list;
	for (NSString* path : filenames) {
		list.add_NoLock(Apple::getStringFromNSString(path));
	}
	UIApp::dispatchOpenUrlsToApp(list);
}

- (BOOL)application:(NSApplication *)sender openTempFile:(NSString *)filename
{
	return UIApp::dispatchOpenTempFileToApp(Apple::getStringFromNSString(filename));
}

- (BOOL)applicationOpenUntitledFile:(NSApplication *)sender
{
	return UIApp::dispatchOpenUntitledFileToApp();
}

- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender
{
	return UIApp::getApp()->shouldOpenUntitledFile();
}

- (BOOL)applicationShouldHandleReopen:(NSApplication *)sender hasVisibleWindows:(BOOL)flag
{
	return UIApp::dispatchReopenToApp(sl_null, flag);
}

@end

#endif
