#ifndef CHECKHEADER_SLIB_UI_CORE
#define CHECKHEADER_SLIB_UI_CORE

#include "definition.h"
#include "event.h"

#include "../core/app.h"
#include "../core/string.h"
#include "../core/thread.h"

#include "../graphics/context.h"

SLIB_UI_NAMESPACE_BEGIN

class Window;
class Screen;
class Cursor;

class AlertParam;

class SLIB_EXPORT UI
{
public:
	// Graphics (Platform Specific)
	static void setDefaultFontName(const String& fontName);

	static Size getFontTextSize(const Ref<Font>& font, const String& text);
	
	static Rectangle getPathBounds(const Ref<GraphicsPath>& path);
	static sl_bool checkPointInPath(const Ref<GraphicsPath>& path, const Point& pt);
	
	static Ref<Drawable> createDrawableFromImage(const ImageDesc& desc);
	static Ref<Drawable> loadDrawableFromMemory(const void* mem, sl_size size);
	
	static Ref<Bitmap> createBitmap(sl_uint32 width, sl_uint32 height);
	static Ref<Drawable> createDrawableFromBitmap(const Ref<Bitmap>& bitmap);
	static Ref<Bitmap> loadBitmapFromMemory(const void* mem, sl_size size);

	// Graphics
	static Ref<GraphicsContext> getGraphicsContext();
	
	static Ref<Drawable> createDrawableFromImage(const Ref<Image>& image);
	static Ref<Drawable> loadDrawableFromMemory(const Memory& mem);
	static Ref<Drawable> loadDrawableFromFile(const String& filePath);
	static Ref<Drawable> loadDrawableFromResource(const String& path);
	
	static Ref<Bitmap> createBitmapFromImage(const ImageDesc& desc);
	static Ref<Bitmap> createBitmapFromImage(const Ref<Image>& image);
	static Ref<Bitmap> loadBitmapFromMemory(const Memory& mem);
	static Ref<Bitmap> loadBitmapFromFile(const String& filePath);
	static Ref<Bitmap> loadBitmapFromResource(const String& path);

	
	// Screens (Platform Specific)
	static List< Ref<Screen> > getScreens();
	static Ref<Screen> getPrimaryScreen();
	static Ref<Screen> getFocusedScreen();
	
	// Screens
	static Rectangle getScreenRegion();
	static Rectangle getScreenRegion(const Ref<Screen>& screen);
	static Rectangle getScreenBounds();
	static Rectangle getScreenBounds(const Ref<Screen>& screen);
	static Size getScreenSize();
	static Size getScreenSize(const Ref<Screen>& screen);
	
	// UI Thread (Platform Specific)
	static sl_bool isUIThread();
	static void runOnUIThread(const Ref<Runnable>& callback);
	
	// Run Loop (Platform Specific, only for desktop apps)
	static void runLoop();
	static void quitLoop();
	
	// Message Box (Platform Specific)
	static void showAlert(const AlertParam& param);
	
	// Message Box
	static void showAlert(const String& text);
	static void showAlert(const String& text, const Ref<Runnable>& onOk);
	static void showAlert(const String& text, const String& caption);
	static void showAlert(const String& text, const String& caption, const Ref<Runnable>& onOk);
	
	// HID related functions (Platform Specific, only for desktop apps)
	static sl_bool checkKeyPressed(Keycode key);
	static sl_bool checkCapsLockOn();
	static sl_bool checkScrollLockOn();
	static sl_bool checkNumLockOn();
	static Point getCursorPos();
	static sl_bool checkLeftButtonPressed();
	static sl_bool checkRightButtonPressed();
	static sl_bool checkMiddleButtonPressed();
	static String getStatusDescription();
	
	// HID related functions
	static String getKeyName(Keycode key);

};

class SLIB_EXPORT UIApp : public Application
{
	SLIB_DECLARE_OBJECT(UIApp, Application)
public:
	UIApp();
	~UIApp();

public:
	static Ref<UIApp> getApp();

public:
	// override
	AppType getAppType();
	
	// override
	void run(const String& param);
	
	void run();

public:
	virtual void onStart() {}
	virtual void onExit() {}

	static void dispatchStart();
	static void dispatchExit();

	// mobile
	virtual void onMobileCreate() {}
	virtual void onMobileResume() {}
	virtual void onMobilePause() {}
	virtual void onMobileDestroy() {}
	virtual sl_bool onMobileBack()
	{
		return sl_true;
	}

	static void dispatchMobileCreate();
	static void dispatchMobileResume();
	static void dispatchMobilePause();
	static void dispatchMobileDestroy();
	static sl_bool dispatchMobileBack();

	static void quit();

	SLIB_INLINE static sl_bool isMobilePaused()
	{
		return m_flagMobilePaused;
	}

private:
	static sl_bool m_flagMobilePaused;
};

enum AlertType {
    alertType_Ok = 0,
    alertType_OkCancel = 1,
    alertType_YesNo = 2,
    alertType_YesNoCancel = 3
};

class SLIB_EXPORT AlertParam
{
public:
	AlertType type;
	String caption;
	String text;

	String titleOk;
	String titleCancel;
	String titleYes;
	String titleNo;

	Ref<Runnable> onOk;
	Ref<Runnable> onCancel;
	Ref<Runnable> onYes;
	Ref<Runnable> onNo;

public:
	AlertParam()
	{
		type = alertType_Ok;
	}
};

SLIB_UI_NAMESPACE_END

#endif
