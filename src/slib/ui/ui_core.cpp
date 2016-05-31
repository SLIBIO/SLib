#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/common_dialogs.h"

#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/graphics/font.h"

SLIB_UI_NAMESPACE_BEGIN

/**************************************
	UI Core
***************************************/

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

void UI::runOnUiThread(const Ref<Runnable> &callback)
{
	if (UI::isUiThread()) {
		callback->run();
		return;
	}
	UI::dispatchToUiThread(callback);
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

Ref<UIApp> UI::getApp()
{
	return UIApp::getApp();
}

Ref<Window> UI::getMainWindow()
{
	return UIApp::getMainWindow();
}

void UI::setMainWindow(const Ref<Window>& window)
{
	UIApp::setMainWindow(window);
}

/**************************************
	UIApplication
***************************************/

sl_bool UIApp::m_flagMobilePaused = sl_false;

SLIB_DEFINE_OBJECT(UIApp, Application)

Ref<UIApp> UIApp::getApp()
{
	Ref<Application> app(Application::getApp());
	if (app.isNotNull() && app->getAppType() == AppType::UI) {
		return Ref<UIApp>::from(app);
	}
	return Ref<UIApp>::null();
}

AppType UIApp::getAppType()
{
	return AppType::UI;
}

void UIApp::run(const String& param)
{
	Application::run(param);
	UI::runApp();
}

void UIApp::run()
{
	run(String::null());
}

void UIApp::quit()
{
	UI::quitApp();
}

void UIApp::onStart()
{
}

void UIApp::dispatchStart()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->onStart();
	}
}

void UIApp::onExit()
{
}

void UIApp::dispatchExit()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->onExit();
	}
}

void UIApp::onMobileCreate()
{
}

void UIApp::dispatchMobileCreate()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->onMobileCreate();
	}
}

void UIApp::onMobilePause()
{
}

void UIApp::dispatchMobilePause()
{
	m_flagMobilePaused = sl_true;
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->onMobilePause();
	}
}

void UIApp::onMobileResume()
{
}

void UIApp::dispatchMobileResume()
{
	m_flagMobilePaused = sl_false;
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->onMobileResume();
	}
}

void UIApp::onMobileDestroy()
{
}

void UIApp::dispatchMobileDestroy()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->onMobileDestroy();
	}
}

sl_bool UIApp::onMobileBack()
{
	return sl_true;
}

sl_bool UIApp::dispatchMobileBack()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		return app->onMobileBack();
	}
	return sl_true;
}

sl_bool UIApp::isMobilePaused()
{
	return m_flagMobilePaused;
}

SLIB_SAFE_STATIC_REF(WeakRef<Window>, _UIApp_mainWindow);

Ref<Window> UIApp::getMainWindow()
{
	return _UIApp_mainWindow;
}

void UIApp::setMainWindow(const Ref<Window>& window)
{
	_UIApp_mainWindow = window;
}

Ref<Menu> UI::getApplicationMenu()
{
	return UIApp::getMenu();
}

void UI::setApplicationMenu(const Ref<Menu>& menu)
{
	UIApp::setMenu(menu);
}

/**************************************
	UIPlatform
***************************************/

typedef HashMap<const void*, WeakRef<ViewInstance> > _UiViewInstanceMap;
SLIB_SAFE_STATIC_GETTER(_UiViewInstanceMap, _UI_getViewInstances);

#if defined(SLIB_PLATFORM_IS_WIN32)
#define CHECK_FREE_STATIC(v) if (!(&v)) return;
#define CHECK_FREE_STATIC_RET(v, r) if (!(&v)) return r;
#else
#define CHECK_FREE_STATIC(v)
#define CHECK_FREE_STATIC_RET(v, r)
#endif

void UIPlatform::_registerViewInstance(const void* handle, ViewInstance* instance)
{
	_UiViewInstanceMap& map = _UI_getViewInstances();
	CHECK_FREE_STATIC(map);
	map.put(handle, instance);
}

Ref<ViewInstance> UIPlatform::_getViewInstance(const void* handle)
{
	_UiViewInstanceMap& map = _UI_getViewInstances();
	CHECK_FREE_STATIC_RET(map, Ref<ViewInstance>::null());
	return map.getValue(handle, WeakRef<ViewInstance>::null());
}

void UIPlatform::_removeViewInstance(const void* handle)
{
	_UiViewInstanceMap& map = _UI_getViewInstances();
	CHECK_FREE_STATIC(map);
	map.remove(handle);
}

typedef HashMap<const void*, WeakRef<WindowInstance> > _UiWindowInstanceMap;
SLIB_SAFE_STATIC_GETTER(_UiWindowInstanceMap, _UI_getWindowInstances);

void UIPlatform::_registerWindowInstance(const void* handle, WindowInstance* instance)
{
	_UiWindowInstanceMap& map = _UI_getWindowInstances();
	CHECK_FREE_STATIC(map);
	map.put(handle, instance);
}

Ref<WindowInstance> UIPlatform::_getWindowInstance(const void* handle)
{
	_UiWindowInstanceMap& map = _UI_getWindowInstances();
	CHECK_FREE_STATIC_RET(map, Ref<WindowInstance>::null());
	return map.getValue(handle, WeakRef<WindowInstance>::null());
}

void UIPlatform::_removeWindowInstance(const void* handle)
{
	_UiWindowInstanceMap& map = _UI_getWindowInstances();
	CHECK_FREE_STATIC(map);
	map.remove(handle);
}

SLIB_UI_NAMESPACE_END

