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

#include "slib/ui/core.h"

#include "slib/ui/platform.h"
#include "slib/ui/window.h"
#include "slib/ui/screen.h"
#include "slib/ui/common_dialogs.h"
#include "slib/graphics/font.h"
#include "slib/device/device.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	namespace priv
	{
		namespace ui_core
		{
			
			class DefaultContext
			{
			public:
				sl_real fontSize;
				AtomicString fontFamily;
				AtomicRef<Font> font;
				sl_ui_len scrollBarWidth;
				
			public:
				DefaultContext()
				{
#if defined(SLIB_PLATFORM_IS_DESKTOP)
					scrollBarWidth = 12;
					fontSize = 12;
#else
					scrollBarWidth = SLIB_MIN(UI::getScreenWidth(), UI::getScreenHeight()) / 60;
					fontSize = (sl_real)(SLIB_MIN(UI::getScreenWidth(), UI::getScreenHeight()) / 40);
#endif
					fontFamily = "Arial";
					font = Font::create(fontFamily, fontSize);
				}
			};
			
			SLIB_SAFE_STATIC_GETTER(DefaultContext, getDefaultContext)

			class UICallback : public Callable<void()>
			{
			public:
				Function<void()> m_callback;
				
			public:
				SLIB_INLINE UICallback(const Function<void()>& callback) noexcept
				 : m_callback(callback)
				{}
				
			public:
				void invoke() noexcept override
				{
					if (UI::isUiThread()) {
						m_callback();
					} else {
						UI::dispatchToUiThread(m_callback);
					}
				}
				
			};
			
			class DispatcherImpl : public Dispatcher
			{
			public:
				sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms) override
				{
					if (delay_ms > 0x7fffffff) {
						delay_ms = 0x7fffffff;
					}
					UI::dispatchToUiThread(callback, (sl_uint32)delay_ms);
					return sl_true;
				}
			};
			
			static sl_int32 g_nLevelRunLoop = 0;
			static sl_bool g_flagQuitApp = 0;
			
			static void QuitLoop()
			{
				if (g_nLevelRunLoop > 0) {
					UIPlatform::quitLoop();
				} else {
					UIPlatform::quitApp();
				}
			}
			
			static void QuitApp()
			{
				if (g_flagQuitApp) {
					return;
				}
				g_flagQuitApp = sl_true;
				QuitLoop();
			}
			
			SLIB_STATIC_ZERO_INITIALIZED(AtomicList<ScreenOrientation>, g_listAvailableScreenOrientations)

			UIKeyboardAdjustMode g_keyboardAdjustMode = UIKeyboardAdjustMode::Pan;
			
#if defined(SLIB_UI_IS_ANDROID)
			void UpdateKeyboardAdjustMode(UIKeyboardAdjustMode mode);
#endif
			
		}
	}
	
	using namespace priv::ui_core;

	SLIB_DEFINE_OBJECT(Screen, Object)

	Screen::Screen()
	{
	}

	Screen::~Screen()
	{
	}
	
	sl_real UI::getDefaultFontSize()
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return 0;
		}
		return def->fontSize;
	}

	void UI::setDefaultFontSize(sl_real fontSize)
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return;
		}
		def->fontSize = fontSize;
		Ref<Font> font = def->font;
		if (font.isNull() || font->getSize() != fontSize) {
			FontDesc desc;
			font->getDesc(desc);
			desc.size = fontSize;
			font = Font::create(desc);
			def->font = font;
		}
	}

	String UI::getDefaultFontFamily()
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return sl_null;
		}
		return def->fontFamily;
	}

	void UI::setDefaultFontFamily(const String& fontFamily)
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return;
		}
		def->fontFamily = fontFamily;
		Ref<Font> font = def->font;
		if (font.isNull() || font->getFamilyName() != fontFamily) {
			FontDesc desc;
			font->getDesc(desc);
			desc.familyName = fontFamily;
			font = Font::create(desc);
			def->font = font;
		}
	}

	Ref<Font> UI::getDefaultFont()
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return sl_null;
		}
		return def->font;
	}

	void UI::setDefaultFont(const Ref<Font>& font)
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return;
		}
		if (font.isNotNull()) {
			def->fontFamily = font->getFamilyName();
			def->fontSize = font->getSize();
			def->font = font;
		}
	}

	sl_ui_len UI::getDefaultScrollBarWidth()
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return 0;
		}
		return def->scrollBarWidth;
	}

	void UI::setDefaultScrollBarWidth(sl_ui_len len)
	{
		DefaultContext* def = getDefaultContext();
		if (!def) {
			return;
		}
		def->scrollBarWidth = len;
	}

	UIRect UI::getScreenRegion()
	{
		return UI::getScreenRegion(Ref<Screen>::null());
	}

	UIRect UI::getScreenRegion(const Ref<Screen>& _screen)
	{
		Ref<Screen> screen = _screen;
		if (screen.isNull()) {
			screen = getPrimaryScreen();
			if (screen.isNull()) {
				return UIRect::zero();
			}
		}
		return screen->getRegion();
	}

	UIRect UI::getScreenBounds()
	{
		return UI::getScreenBounds(Ref<Screen>::null());
	}

	UIRect UI::getScreenBounds(const Ref<Screen>& _screen)
	{
		Ref<Screen> screen = _screen;
		if (screen.isNull()) {
			screen = getPrimaryScreen();
			if (screen.isNull()) {
				return UIRect::zero();
			}
		}
		UIRect region = screen->getRegion();
		return UIRect(0, 0, region.getWidth(), region.getHeight());
	}

	UISize UI::getScreenSize()
	{
		return UI::getScreenSize(Ref<Screen>::null());
	}

	UISize UI::getScreenSize(const Ref<Screen>& _screen)
	{
		Ref<Screen> screen = _screen;
		if (screen.isNull()) {
			screen = getPrimaryScreen();
			if (screen.isNull()) {
				return UISize::zero();
			}
		}
		UIRect region = screen->getRegion();
		return UISize(region.getWidth(), region.getHeight());
	}

	sl_ui_len UI::getScreenWidth()
	{
		return getScreenSize().x;
	}

	sl_ui_len UI::getScreenHeight()
	{
		return getScreenSize().y;
	}

	List<ScreenOrientation> UI::getAvailableScreenOrientations()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_listAvailableScreenOrientations)) {
			return sl_null;
		}
		return g_listAvailableScreenOrientations;
	}
	
	void UI::setAvailableScreenOrientations(const List<ScreenOrientation>& orientations)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_listAvailableScreenOrientations)) {
			return;
		}
		g_listAvailableScreenOrientations = orientations;
		attemptRotateScreenOrientation();
	}
	
	void UI::setAvailableScreenOrientation(const ScreenOrientation& orientation)
	{
		setAvailableScreenOrientations(List<ScreenOrientation>::createFromElements(orientation));
	}
	
	void UI::setAvailableScreenOrientationsPortrait()
	{
		setAvailableScreenOrientations(List<ScreenOrientation>::createFromElements(ScreenOrientation::Portrait, ScreenOrientation::PortraitUpsideDown));
	}
	
	void UI::setAvailableScreenOrientationsLandscape()
	{
		setAvailableScreenOrientations(List<ScreenOrientation>::createFromElements(ScreenOrientation::LandscapeRight, ScreenOrientation::LandscapeLeft));
	}
	
	void UI::setAvailableScreenOrientationsAll()
	{
		setAvailableScreenOrientations(sl_null);
	}
	

#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	ScreenOrientation UI::getScreenOrientation()
	{
		return ScreenOrientation::Portrait;
	}

	void UI::attemptRotateScreenOrientation()
	{
	}
#endif

	double UI::getScreenPPI()
	{
		double ppi = Device::getScreenPPI();
		if (ppi < 1) {
			return 1;
		} else {
			return ppi;
		}
	}
	
	sl_real UI::pixelToInch(sl_real px)
	{
		return (sl_real)(px / getScreenPPI());
	}
	
	sl_real UI::inchToPixel(sl_real inch)
	{
		return (sl_real)(inch * getScreenPPI());
	}
	
	sl_real UI::pixelToMeter(sl_real px)
	{
		return (sl_real)(px / getScreenPPI() * 0.0254);
	}
	
	sl_real UI::meterToPixel(sl_real meters)
	{
		return (sl_real)(meters * 39.3701 * getScreenPPI());
	}

	sl_real UI::pixelToCentimeter(sl_real px)
	{
		return (sl_real)(px * 2.54 / getScreenPPI());
	}
	
	sl_real UI::centimeterToPixel(sl_real cm)
	{
		return (sl_real)(cm * getScreenPPI() * 0.393701);
	}
	
	sl_real UI::pixelToMillimeter(sl_real px)
	{
		return (sl_real)(px * 25.4 / getScreenPPI());
	}
	
	sl_real UI::millimeterToPixel(sl_real mm)
	{
		return (sl_real)(mm * getScreenPPI() * 0.0393701);
	}
	
	sl_real UI::pixelToPoint(sl_real px)
	{
		return (sl_real)(px * 72 / getScreenPPI());
	}
	
	sl_real UI::pointToPixel(sl_real pt)
	{
		return (sl_real)(pt * getScreenPPI() / 72);
	}
	
	sl_real UI::pixelToDp(sl_real px)
	{
		return (sl_real)(px * 160 / getScreenPPI());
	}
	
	sl_real UI::dpToPixel(sl_real dp)
	{
		return (sl_real)(dp * getScreenPPI() / 160);
	}
	
	void UI::alert(const String& text)
	{
		AlertDialog alert;
		alert.text = text;
		alert.run();
	}

	void UI::alert(const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.caption = caption;
		alert.text = text;
		alert.run();
	}

	void UI::alert(const Ref<Window>& parent, const String& text)
	{
		AlertDialog alert;
		alert.parent = parent;
		alert.text = text;
		alert.run();
	}

	void UI::alert(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.parent = parent;
		alert.caption = caption;
		alert.text = text;
		alert.run();
	}

	void UI::showAlert(const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	void UI::showAlert(const String& caption, const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.caption = caption;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	void UI::showAlert(const Ref<Window>& parent, const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.parent = parent;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}
	
	void UI::showAlert(const Ref<Window>& parent, const String& caption, const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.parent = parent;
		alert.caption = caption;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	sl_bool UI::confirm(const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.text = text;
		return alert.run() == DialogResult::Ok;
	}
	
	sl_bool UI::confirm(const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.caption = caption;
		alert.text = text;
		return alert.run() == DialogResult::Ok;
	}
	
	sl_bool UI::confirm(const Ref<Window>& parent, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.parent = parent;
		alert.text = text;
		return alert.run() == DialogResult::Ok;
	}
	
	sl_bool UI::confirm(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.parent = parent;
		alert.caption = caption;
		alert.text = text;
		return alert.run() == DialogResult::Ok;
	}
	
	void UI::showConfirm(const String& text, const Function<void(sl_bool)>& onResult)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.text = text;
		alert.onComplete = [onResult](DialogResult result) {
			if (result == DialogResult::Ok) {
				onResult(sl_true);
			} else {
				onResult(sl_false);
			}
		};
		alert.show();
	}
	
	void UI::showConfirm(const String& caption, const String& text, const Function<void(sl_bool)>& onResult)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.caption = caption;
		alert.text = text;
		alert.onComplete = [onResult](DialogResult result) {
			if (result == DialogResult::Ok) {
				onResult(sl_true);
			} else {
				onResult(sl_false);
			}
		};
		alert.show();
	}
	
	void UI::showConfirm(const Ref<Window>& parent, const String& text, const Function<void(sl_bool)>& onResult)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.parent = parent;
		alert.text = text;
		alert.onComplete = [onResult](DialogResult result) {
			if (result == DialogResult::Ok) {
				onResult(sl_true);
			} else {
				onResult(sl_false);
			}
		};
		alert.show();
	}
	
	void UI::showConfirm(const Ref<Window>& parent, const String& caption, const String& text, const Function<void(sl_bool)>& onResult)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.parent = parent;
		alert.caption = caption;
		alert.text = text;
		alert.onComplete = [onResult](DialogResult result) {
			if (result == DialogResult::Ok) {
				onResult(sl_true);
			} else {
				onResult(sl_false);
			}
		};
		alert.show();
	}
	
#if !defined(SLIB_UI_IS_MACOS)
	void UI::dispatchToUiThreadUrgently(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		dispatchToUiThread(callback, delayMillis);
	}
#endif
	
	void UI::runOnUiThread(const Function<void()>& callback)
	{
		if (callback.isNotNull()) {
			if (isUiThread()) {
				callback();
			} else {
				dispatchToUiThread(callback);
			}
		}
	}

	Function<void()> UI::getCallbackOnUiThread(const Function<void()>& callback)
	{
		if (callback.isNotNull()) {
			return static_cast<Callable<void()>*>(new UICallback(callback));
		}
		return sl_null;
	}

	Ref<Dispatcher> UI::getDispatcher()
	{
		return new DispatcherImpl();
	}

	void UI::runLoop()
	{
		if (!(UI::isUiThread())) {
			return;
		}
		g_nLevelRunLoop++;
		UIPlatform::runLoop(g_nLevelRunLoop);
		g_nLevelRunLoop--;
		if (g_flagQuitApp) {
			QuitLoop();
		}
	}

	void UI::quitLoop()
	{
		if (UI::isUiThread()) {
			QuitLoop();
		} else {
			UI::dispatchToUiThread(&QuitLoop);
		}
	}

	void UI::runApp()
	{
		UIPlatform::runApp();
	}

	void UI::quitApp()
	{
		if (UI::isUiThread()) {
			QuitApp();
		} else {
			UI::dispatchToUiThread(&QuitApp);
		}
	}
	
	void UI::openUrl(const String& url)
	{
		Device::openUrl(url);
	}
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	void UI::dismissKeyboard()
	{
	}
#endif
	
	UIKeyboardAdjustMode UI::getKeyboardAdjustMode()
	{
		return g_keyboardAdjustMode;
	}
	
	
	void UI::setKeyboardAdjustMode(UIKeyboardAdjustMode mode)
	{
		g_keyboardAdjustMode = mode;
#if defined(SLIB_UI_IS_ANDROID)
		UpdateKeyboardAdjustMode(mode);
#endif
	}
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	UIEdgeInsets UI::getSafeAreaInsets()
	{
		UIEdgeInsets ret;
		ret.left = 0;
		ret.top = getStatusBarHeight();
		ret.right = 0;
		ret.bottom = 0;
		return ret;
	}

	sl_ui_len UI::getStatusBarHeight()
	{
		return 0;
	}
	
	void UI::setStatusBarStyle(StatusBarStyle style)
	{
	}
#endif
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_ANDROID) && !defined(SLIB_UI_IS_WIN32)
	void UI::setBadgeNumber(sl_uint32 number)
	{
	}
#endif
	
}
