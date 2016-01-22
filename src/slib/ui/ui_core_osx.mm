#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/app.h"

@interface _slib_OSX_AppDelegate : NSObject <NSApplicationDelegate>
@end

SLIB_UI_NAMESPACE_BEGIN

class _OSX_Screen : public Screen
{
public:
	NSScreen* m_screen;
	Rectangle m_region;
	
public:
	static Ref<_OSX_Screen> create(NSScreen* screen, NSScreen* primary)
	{
		Ref<_OSX_Screen> ret;
		if (screen != nil) {
			ret = new _OSX_Screen();
			if (ret.isNotNull()) {
				ret->m_screen = screen;
				Rectangle region;
				sl_real leftBottom = 0;
				if (primary == nil) {
					primary = getPrimaryScreen();
				}
				if (primary != nil) {
					NSRect rect = [primary frame];
					leftBottom = (sl_real)(rect.origin.y + rect.size.height);
				}
				NSRect rect = [screen frame];
				region.left = (sl_real)(rect.origin.x);
				region.top = leftBottom - (sl_real)(rect.origin.y + rect.size.height);
				region.setWidth((sl_real)(rect.size.width));
				region.setHeight((sl_real)(rect.size.height));
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
	Rectangle getRegion()
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
	[NSApplication sharedApplication];
	[NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
	_slib_OSX_AppDelegate * delegate = [[_slib_OSX_AppDelegate alloc] init];
	[NSApp setDelegate:delegate];
	[NSApp run];
}

void UI::quitLoop()
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[NSApp terminate:nil];
	});
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
		Ref<Runnable> on2, on3;
		NSAlert* alert = [[NSAlert alloc] init];
		[alert setMessageText:text];
		[alert setAlertStyle:NSInformationalAlertStyle];
		[[alert window] setTitle:caption];
		if (type == alertType_OkCancel) {
			[alert addButtonWithTitle:titleOk];
			NSButton* btnCancel = [alert addButtonWithTitle:titleCancel];
			[btnCancel setKeyEquivalent:@"Cancel"];
			on2 = onCancel;
		} else if (type == alertType_YesNo) {
			[alert addButtonWithTitle:titleYes];
			NSButton* btnNo = [alert addButtonWithTitle:titleNo];
			[btnNo setKeyEquivalent:@"Don't Save"];
			on2 = onNo;
		} else if (type == alertType_YesNoCancel) {
			[alert addButtonWithTitle:titleYes];
			NSButton* btnNo = [alert addButtonWithTitle:titleNo];
			[btnNo setKeyEquivalent:@"Don't Save"];
			NSButton* btnCancel = [alert addButtonWithTitle:titleCancel];
			[btnCancel setKeyEquivalent:@"Cancel"];
			on2 = onNo;
			on3 = onCancel;
		} else {
			[alert addButtonWithTitle:titleOk];
		}
		NSInteger result = [alert runModal];
		if (result == NSAlertFirstButtonReturn) {
			if (onOk.isNotNull()) {
				onOk->run();
			}
		} else if (result == NSAlertSecondButtonReturn) {
			if (on2.isNotNull()) {
				on2->run();
			}
		} else if (result == NSAlertThirdButtonReturn) {
			if (on3.isNotNull()) {
				on3->run();
			}
		}
	});
}

NSColor* UIPlatform::getNSColorFromColor(const Color& color)
{
	return [NSColor colorWithRed:color.getRedF() green:color.getGreenF() blue:color.getBlueF() alpha:color.getAlphaF()];
}

Color UIPlatform::getColorFromNSColor(NSColor* color)
{
	if (color == nil) {
		return Color::zero();
	}
	color = [color colorUsingColorSpaceName:NSDeviceRGBColorSpace];
	CGFloat red, green, blue, alpha;
	[color getRed:&red green:&green blue:&blue alpha:&alpha];
	return Color4f((sl_real)red, (sl_real)green, (sl_real)blue, (sl_real)alpha);
}

SLIB_UI_NAMESPACE_END

@implementation _slib_OSX_AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	slib::UIApp::dispatchStart();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	slib::UIApp::dispatchExit();
}
@end


#endif
