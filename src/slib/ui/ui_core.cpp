#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

/**************************************
	UI Core
***************************************/

SLIB_DEFINE_OBJECT(Screen, Object)

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

void UI::showAlert(const String& text)
{
	AlertParam param;
	param.text = text;
	param.caption = "Alert";
	showAlert(param);
}

void UI::showAlert(const String& text, const Ref<Runnable>& onOk)
{
	AlertParam param;
	param.text = text;
	param.caption = "Alert";
	param.onOk = onOk;
	showAlert(param);
}

void UI::showAlert(const String& text, const String& caption)
{
	AlertParam param;
	param.text = text;
	param.caption = caption;
	showAlert(param);
}

void UI::showAlert(const String& text, const String& caption, const Ref<Runnable>& onOk)
{
	AlertParam param;
	param.text = text;
	param.caption = caption;
	param.onOk = onOk;
	showAlert(param);
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
	UI::runLoop();
}

void UIApp::run()
{
	run(String::null());
}

void UIApp::quit()
{
	UI::quitLoop();
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


AlertParam::AlertParam()
{
	type = AlertType::Ok;
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

