#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/common_dialogs.h"
#include "../../../inc/slib/graphics/font.h"
#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Screen, Object)

	Screen::Screen()
	{
	}

	Screen::~Screen()
	{
	}


	class _UI_Core_Default
	{
	public:
		sl_real fontSize;
		AtomicString fontFamily;
		AtomicRef<Font> font;
		sl_ui_len scrollBarWidth;

	public:
		_UI_Core_Default()
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

	SLIB_SAFE_STATIC_GETTER(_UI_Core_Default, _UI_Core_getDefault)

	sl_real UI::getDefaultFontSize()
	{
		_UI_Core_Default* def = _UI_Core_getDefault();
		if (!def) {
			return 0;
		}
		return def->fontSize;
	}

	void UI::setDefaultFontSize(sl_real fontSize)
	{
		_UI_Core_Default* def = _UI_Core_getDefault();
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
		_UI_Core_Default* def = _UI_Core_getDefault();
		if (!def) {
			return sl_null;
		}
		return def->fontFamily;
	}

	void UI::setDefaultFontFamily(const String& fontFamily)
	{
		_UI_Core_Default* def = _UI_Core_getDefault();
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
		_UI_Core_Default* def = _UI_Core_getDefault();
		if (!def) {
			return sl_null;
		}
		return def->font;
	}

	void UI::setDefaultFont(const Ref<Font>& font)
	{
		_UI_Core_Default* def = _UI_Core_getDefault();
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
		_UI_Core_Default* def = _UI_Core_getDefault();
		if (!def) {
			return 0;
		}
		return def->scrollBarWidth;
	}

	void UI::setDefaultScrollBarWidth(sl_ui_len len)
	{
		_UI_Core_Default* def = _UI_Core_getDefault();
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

	void UI::alert(const String& text)
	{
		AlertDialog::run(text);
	}

	void UI::alert(const Ref<Window>& parent, const String& text)
	{
		AlertDialog::run(parent, text);
	}

	void UI::alert(const String& caption, const String& text)
	{
		AlertDialog::run(caption, text);
	}

	void UI::alert(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog::run(parent, caption, text);
	}

	void UI::showAlert(const String& text)
	{
		AlertDialog::show(text);
	}

	void UI::showAlert(const String& text, const Function<void()>& onOk)
	{
		AlertDialog::show(text, onOk);
	}

	void UI::showAlert(const String& caption, const String& text, const Function<void()>& onOk)
	{
		AlertDialog::show(caption, text, onOk);
	}

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

	class _UiCallback : public Callable<void()>
	{
	public:
		Function<void()> m_callback;

	public:
		SLIB_INLINE _UiCallback(const Function<void()>& callback) : m_callback(callback)
		{
		}

	public:
		// override
		void invoke()
		{
			if (UI::isUiThread()) {
				m_callback();
			} else {
				UI::dispatchToUiThread(m_callback);
			}
		}

	};

	Function<void()> UI::getCallbackOnUiThread(const Function<void()>& callback)
	{
		if (callback.isNotNull()) {
			return (Callable<void()>*)(new _UiCallback(callback));
		}
		return sl_null;
	}

	class _UiDispatcher : public Dispatcher
	{
	public:
		// override
		sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms)
		{
			if (delay_ms > 0x7fffffff) {
				delay_ms = 0x7fffffff;
			}
			UI::dispatchToUiThread(callback, (sl_uint32)delay_ms);
			return sl_true;
		}
	};

	Ref<Dispatcher> UI::getDispatcher()
	{
		return new _UiDispatcher();
	}

	static sl_int32 _g_ui_run_loop_level = 0;
	static sl_bool _g_ui_flag_quit_app = 0;

	void UI::runLoop()
	{
		if (!(UI::isUiThread())) {
			return;
		}
		_g_ui_run_loop_level++;
		UIPlatform::runLoop(_g_ui_run_loop_level);
		_g_ui_run_loop_level--;
		if (_g_ui_flag_quit_app) {
			if (_g_ui_run_loop_level > 0) {
				UIPlatform::quitLoop();			
			} else {
				UIPlatform::quitApp();
			}
		}
	}

	void UI::quitLoop()
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(&(UI::quitLoop));
			return;
		}
		if (_g_ui_run_loop_level > 0) {
			UIPlatform::quitLoop();
		} else {
			quitApp();
		}
	}

	void UI::runApp()
	{
		UIPlatform::runApp();
	}

	void UI::quitApp()
	{
		if (_g_ui_flag_quit_app) {
			return;
		}
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(&(UI::quitApp));
			return;
		}
		_g_ui_flag_quit_app = sl_true;
		if (_g_ui_run_loop_level > 0) {
			UIPlatform::quitLoop();
		} else {
			UIPlatform::quitApp();
		}
	}

}

