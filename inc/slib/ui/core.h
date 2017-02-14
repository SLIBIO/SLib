#ifndef CHECKHEADER_SLIB_UI_CORE
#define CHECKHEADER_SLIB_UI_CORE

#include "definition.h"

#include "event.h"

#include "../core/string.h"
#include "../core/thread.h"
#include "../core/function.h"
#include "../core/dispatch.h"

namespace slib
{

	class Window;
	class Screen;
	class Cursor;
	class Menu;
	class UIApp;
	class Font;

	class SLIB_EXPORT UI
	{
	public:
		static sl_real getDefaultFontSize();
		
		static void setDefaultFontSize(sl_real fontSize);
		
		static String getDefaultFontFamily();
		
		static void setDefaultFontFamily(const String& fontFamily);
		
		static Ref<Font> getDefaultFont();

		static void setDefaultFont(const Ref<Font>& font);

		static sl_ui_len getDefaultScrollBarWidth();
		
		static void setDefaultScrollBarWidth(sl_ui_len width);
		
		// Screens
		static List< Ref<Screen> > getScreens();
		
		static Ref<Screen> getPrimaryScreen();
		
		static Ref<Screen> getFocusedScreen();
		
		static UIRect getScreenRegion();
		
		static UIRect getScreenRegion(const Ref<Screen>& screen);
		
		static UIRect getScreenBounds();
		
		static UIRect getScreenBounds(const Ref<Screen>& screen);
		
		static UISize getScreenSize();
		
		static UISize getScreenSize(const Ref<Screen>& screen);
		
		static sl_ui_len getScreenWidth();
		
		static sl_ui_len getScreenHeight();
		
		
		// Message Box
		// Run on UI thread
		static void alert(const String& text);

		// Run on UI thread
		static void alert(const Ref<Window>& parent, const String& text);

		// Run on UI thread
		static void alert(const String& caption, const String& text);
		
		// Run on UI thread
		static void alert(const Ref<Window>& parent, const String& caption, const String& text);
		
		static void showAlert(const String& text);
		
		static void showAlert(const String& text, const Function<void()>& onOk);
		
		static void showAlert(const String& caption, const String& text, const Function<void()>& onOk);
		
		
		// HID related functions (Platform Specific, only for desktop apps)
		static sl_bool checkKeyPressed(Keycode key);
		
		static sl_bool checkCapsLockOn();
		
		static sl_bool checkScrollLockOn();
		
		static sl_bool checkNumLockOn();
		
		static UIPoint getCursorPos();
		
		static sl_bool checkLeftButtonPressed();
		
		static sl_bool checkRightButtonPressed();
		
		static sl_bool checkMiddleButtonPressed();
		
		static String getStatesDescription();
		
		
		// HID related functions
		static String getKeyName(Keycode key, sl_bool flagShort = sl_false);

		static Keycode getKeycodeFromName(const String& keyName);
		
		// UI Thread
		static sl_bool isUiThread();
		
		static void dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis = 0);
		
		static void runOnUiThread(const Function<void()>& callback);
		
		static Function<void()> getCallbackOnUiThread(const Function<void()>& callback);

		static Ref<Dispatcher> getDispatcher();
		
		// Run Loop
		static void runLoop();
		
		static void quitLoop();
		
		static void runApp();
		
		static void quitApp();
		
		
		static void openUrl(const String& url);

	};

}

#define SLIB_UI_CALLBACK(...) slib::UI::getCallbackOnUiThread(SLIB_BIND(void(), __VA_ARGS__))
#define SLIB_UI_CALLBACK_CLASS(...) slib::UI::getCallbackOnUiThread(SLIB_BIND_CLASS(void(), __VA_ARGS__))
#define SLIB_UI_CALLBACK_REF(...) slib::UI::getCallbackOnUiThread(SLIB_BIND_REF(void(), __VA_ARGS__))
#define SLIB_UI_CALLBACK_WEAKREF(...) slib::UI::getCallbackOnUiThread(SLIB_BIND_WEAKREF(void(), __VA_ARGS__))

#endif
