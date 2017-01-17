#ifndef CHECKHEADER_SLIB_UI_PLATFORM
#define CHECKHEADER_SLIB_UI_PLATFORM

#include "definition.h"

#include "../core/ref.h"
#include "../graphics/platform.h"

#if defined(SLIB_PLATFORM_IS_WIN32)
#include "../core/platform_windows.h"
#include <GdiPlus.h>
#elif defined(SLIB_PLATFORM_IS_ANDROID)
#include "../core/platform_android.h"
#elif defined(SLIB_PLATFORM_IS_APPLE)
#include "../core/platform_apple.h"
#include <CoreText/CoreText.h>
#endif

#include "event.h"

SLIB_UI_NAMESPACE_BEGIN

class Screen;
class ViewInstance;
class View;
class WindowInstance;
class Window;
class Cursor;
class Menu;
class MenuItem;

class SLIB_EXPORT UIPlatform
{
public:
	static void runLoop(sl_uint32 level);
	static void quitLoop();
	static void runApp();
	static void quitApp();
	
#if defined(SLIB_PLATFORM_IS_WIN32)
	
	static Ref<ViewInstance> createViewInstance(HWND hWnd, sl_bool flagDestroyOnRelease = sl_true);
	static void registerViewInstance(HWND hWnd, ViewInstance* instance);
	static Ref<ViewInstance> getViewInstance(HWND hWnd);
	static void removeViewInstance(HWND hWnd);
	static HWND getViewHandle(ViewInstance* instance);
	static HWND getViewHandle(View* view);

	static Ref<WindowInstance> createWindowInstance(HWND hWnd, sl_bool flagDestroyOnRelease = sl_true);
	static Ref<WindowInstance> getWindowInstance(HWND hWnd);
	static void removeWindowInstance(HWND hWnd);
	static HWND getWindowHandle(WindowInstance* instance);
	static HWND getWindowHandle(Window* window);

	static Ref<Cursor> createCursor(HCURSOR hCursor, sl_bool flagDestroyOnRelease = sl_true);
	static HCURSOR getCursorHandle(const Ref<Cursor>& cursor);

	static HMENU getMenuHandle(const Ref<Menu>& menu);
	static Ref<Menu> getMenu(HMENU hMenu);

#elif defined(SLIB_PLATFORM_IS_ANDROID)
	
	static Ref<ViewInstance> createViewInstance(jobject jhandle);
	static void registerViewInstance(jobject jhandle, ViewInstance* instance);
	static Ref<ViewInstance> getViewInstance(jobject jhandle);
	static void removeViewInstance(jobject jhandle);
	static jobject getViewHandle(ViewInstance* instance);
	static jobject getViewHandle(View* view);

	static Ref<WindowInstance> createWindowInstance(jobject window);
	static Ref<WindowInstance> getWindowInstance(jobject window);
	static void removeWindowInstance(jobject window);
	static jobject getWindowHandle(WindowInstance* instance);

#elif defined(SLIB_PLATFORM_IS_APPLE)
	
#	if defined(__OBJC__)
#		if defined(SLIB_PLATFORM_IS_OSX)
	
	static Ref<ViewInstance> createViewInstance(NSView* handle, sl_bool flagFreeOnRelease = sl_true);
	static void registerViewInstance(NSView* handle, ViewInstance* instance);
	static Ref<ViewInstance> getViewInstance(NSView* handle);
	static void removeViewInstance(NSView* handle);
	static NSView* getViewHandle(ViewInstance* instance);
	static NSView* getViewHandle(View* view);

	static Ref<WindowInstance> createWindowInstance(NSWindow* handle);
	static Ref<WindowInstance> getWindowInstance(NSWindow* handle);
	static void removeWindowInstance(NSWindow* handle);
	static NSWindow* getWindowHandle(WindowInstance* instance);
	static NSWindow* getWindowHandle(Window* window);
	
	static Ref<Screen> createScreen(NSScreen* handle);
	static NSScreen* getScreenHandle(Screen* screen);
	static float getDpiForScreen(NSScreen* handle);
	
	static Ref<Cursor> createCursor(NSCursor* handle);
	static NSCursor* getCursorHandle(const Ref<Cursor>& cursor);
	
	static NSMenu* getMenuHandle(const Ref<Menu>& menu);
	static NSMenuItem* getMenuItemHandle(const Ref<MenuItem>& menu);

	static NSString* getKeyEquivalent(const KeycodeAndModifiers& km, NSUInteger& outMask);
	
#		elif defined(SLIB_PLATFORM_IS_IOS)
	
	static Ref<ViewInstance> createViewInstance(UIView* handle, sl_bool flagFreeOnRelease = sl_true);
	static void registerViewInstance(UIView* handle, ViewInstance* instance);
	static Ref<ViewInstance> getViewInstance(UIView* handle);
	static void removeViewInstance(UIView* handle);
	static UIView* getViewHandle(ViewInstance* instance);
	static UIView* getViewHandle(View* view);
	
	static Ref<WindowInstance> createWindowInstance(UIView* handle);
	static Ref<WindowInstance> getWindowInstance(UIView* handle);
	static void removeWindowInstance(UIView* handle);
	static UIView* getWindowHandle(WindowInstance* instance);
	
	static Ref<Screen> createScreen(UIScreen* handle);
	static UIScreen* getScreenHandle(Screen* screen);
	
	static UIWindow* getMainWindow();
	static UIWindow* getKeyWindow();
	static void setKeyWindow(UIWindow* window);
	
	static CGFloat getGlobalScaleFactor();
	static void setGlobalScaleFactor(CGFloat factor);
	
#		endif
#	endif
#endif

private:
	static Ref<ViewInstance> _getViewInstance(const void* handle);
	static void _registerViewInstance(const void* handle, ViewInstance* instance);
	static void _removeViewInstance(const void* handle);
	static Ref<WindowInstance> _getWindowInstance(const void* handle);
	static void _registerWindowInstance(const void* handle, WindowInstance* instance);
	static void _removeWindowInstance(const void* handle);
};

SLIB_UI_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_WIN32)

#define SLIB_UI_MESSAGE_BEGIN 0x7100
#define SLIB_UI_MESSAGE_CLOSE SLIB_UI_MESSAGE_BEGIN
#define SLIB_UI_MESSAGE_DISPATCH (SLIB_UI_MESSAGE_BEGIN+1)
#define SLIB_UI_MESSAGE_DISPATCH_DELAYED (SLIB_UI_MESSAGE_BEGIN+2)
#define SLIB_UI_MESSAGE_CUSTOM_MSGBOX (SLIB_UI_MESSAGE_BEGIN+3)

#endif

#endif
