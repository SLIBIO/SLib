#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/common_dialogs.h"

#include "../../../inc/slib/graphics/font.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Screen, Object)

SLIB_SAFE_STATIC_REF(SafeRef<Font>, _g_ui_core_default_font)

Ref<Font> UI::getDefaultFont()
{
	Ref<Font> font = _g_ui_core_default_font;
	if (font.isNotNull()) {
		return font;
	}
	return Font::getDefault();
}

void UI::setDefaultFont(const Ref<Font>& font)
{
	_g_ui_core_default_font = font;
}

Rectangle UI::getScreenRegion()
{
	return UI::getScreenRegion(Ref<Screen>::null());
}

Rectangle UI::getScreenRegion(const Ref<Screen>& _screen)
{
	Ref<Screen> screen = _screen;
	if (screen.isNull()) {
		screen = getPrimaryScreen();
		if (screen.isNull()) {
			return Rectangle::zero();
		}
	}
	return screen->getRegion();
}

Rectangle UI::getScreenBounds()
{
	return UI::getScreenBounds(Ref<Screen>::null());
}

Rectangle UI::getScreenBounds(const Ref<Screen>& _screen)
{
	Ref<Screen> screen = _screen;
	if (screen.isNull()) {
		screen = getPrimaryScreen();
		if (screen.isNull()) {
			return Rectangle::zero();
		}
	}
	Rectangle region = screen->getRegion();
	return Rectangle(0, 0, region.getWidth(), region.getHeight());
}

Size UI::getScreenSize()
{
	return UI::getScreenSize(Ref<Screen>::null());
}

Size UI::getScreenSize(const Ref<Screen>& _screen)
{
	Ref<Screen> screen = _screen;
	if (screen.isNull()) {
		screen = getPrimaryScreen();
		if (screen.isNull()) {
			return Size::zero();
		}
	}
	Rectangle region = screen->getRegion();
	return Size(region.getWidth(), region.getHeight());
}

void UI::alert(const String& text)
{
	AlertDialog::run(text);
}

void UI::alert(const String& caption, const String& text)
{
	AlertDialog::run(caption, text);
}

void UI::showAlert(const String& text, const Ref<Runnable>& onOk)
{
	AlertDialog::show(text, onOk);
}

void UI::showAlert(const String& caption, const String& text, const Ref<Runnable>& onOk)
{
	AlertDialog::show(caption, text, onOk);
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
		UI::dispatchToUiThread(SLIB_CALLBACK(UI::quitLoop));
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
		UI::dispatchToUiThread(SLIB_CALLBACK(UI::quitApp));
		return;
	}
	_g_ui_flag_quit_app = sl_true;
	if (_g_ui_run_loop_level > 0) {
		UIPlatform::quitLoop();
	} else {
		UIPlatform::quitApp();
	}
}

SLIB_UI_NAMESPACE_END

