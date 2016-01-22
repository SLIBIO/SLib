#ifndef CHECKHEADER_SLIB_UI_PLATFORM
#define CHECKHEADER_SLIB_UI_PLATFORM

#include "definition.h"

#include "../core/reference.h"
#include "../graphics/color.h"
#include "../graphics/context.h"

#if defined(SLIB_PLATFORM_IS_WIN32)
#include "../core/platform_windows.h"
#include <GdiPlus.h>
#elif defined(SLIB_PLATFORM_IS_ANDROID)
#include "../core/platform_android.h"
#elif defined(SLIB_PLATFORM_IS_APPLE)
#include "../core/platform_apple.h"
#include <CoreText/CoreText.h>
#endif

SLIB_UI_NAMESPACE_BEGIN

class Screen;
class ViewInstance;
class View;
class WindowInstance;
class Window;
class Cursor;

class SLIB_EXPORT UIPlatform
{
public:
#if defined(SLIB_PLATFORM_IS_WIN32)

	static Gdiplus::GraphicsPath* getGraphicsPath(GraphicsPath* path, Ref<GraphicsPathInstance>& instance);

	static Gdiplus::Font* getGdiplusFont(Font* font, Ref<FontInstance>& instance);
	static HFONT getGdiFont(Font* font, Ref<FontInstance>& instance);
	
	static Ref<Canvas> createCanvas(Gdiplus::Graphics* graphics, sl_uint32 width, sl_uint32 height, const Rectangle* rectDirty = sl_null, sl_bool flagFreeOnRelease = sl_true, const Referable* ref = sl_null);
	static Gdiplus::Graphics* getCanvasHandle(Canvas* canvas);
	
	static Ref<Drawable> createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease = sl_true, const Referable* ref = sl_null);
	static Gdiplus::Image* getImageDrawableHandle(Drawable* drawable);

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
	
	static Ref<Cursor> createCursor(HCURSOR hCursor, sl_bool flagDestroyOnRelease = sl_true);
	static HCURSOR getCursorHandle(const Ref<Cursor>& cursor);

	static COLORREF getColorRef(const Color& color);

#elif defined(SLIB_PLATFORM_IS_ANDROID)

    static jobject getGraphicsPath(GraphicsPath* path, Ref<GraphicsPathInstance>& instance);
	static jobject getNativeFont(Font* font, Ref<FontInstance>& instance);
	
	static Ref<Canvas> createCanvas(jobject canvas);
	static jobject getCanvasHandle(Canvas* canvas);
	
	static Ref<Drawable> createImageDrawable(jobject bitmap, sl_bool flagRecycleOnRelease = sl_true, const Referable* ref = sl_null);
	static jobject getImageDrawableHandle(Drawable* drawable);

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
	
	static CGPathRef getGraphicsPath(GraphicsPath* path, Ref<GraphicsPathInstance>& instance);
	static CTFontRef getCoreTextFont(Font* font, Ref<FontInstance>& instance);
	
	static Ref<Canvas> createCanvas(CGContextRef graphics, sl_uint32 width, sl_uint32 height, const Rectangle* rectDirty = sl_null);
	static CGContextRef getCanvasHandle(Canvas* canvas);
	
	static Ref<Drawable> createImageDrawable(CGImageRef image, sl_bool flagFlipped = sl_false);
	static CGImageRef getImageDrawableHandle(Drawable* drawable);

#	if defined(__OBJC__)
#		if defined(SLIB_PLATFORM_IS_OSX)
	
	static NSFont* getNSFont(Font* font, Ref<FontInstance>& instance);
	
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
	
	static Ref<Screen> createScreen(NSScreen* handle);
	static NSScreen* getScreenHandle(Screen* screen);
	static float getDpiForScreen(NSScreen* handle);
	
	static NSColor* getNSColorFromColor(const Color& color);
	static Color getColorFromNSColor(NSColor* color);
	
	static Ref<Cursor> createCursor(NSCursor* handle);
	static NSCursor* getCursorHandle(const Ref<Cursor>& cursor);

#		elif defined(SLIB_PLATFORM_IS_IOS)
	
	static UIFont* getUIFont(Font* font, Ref<FontInstance>& instance);
	
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
	
	static UIColor* getUIColorFromColor(const Color& color);
	static Color getColorFromUIColor(UIColor* color);
	
	static UIWindow* getMainWindow();
	static UIWindow* getKeyWindow();
	static void setKeyWindow(UIWindow* window);
	
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

#define SLIB_UI_MESSAGE_CLOSE (WM_USER+1)
#define SLIB_UI_MESSAGE_DISPATCH (WM_USER+2)

#endif

#endif
