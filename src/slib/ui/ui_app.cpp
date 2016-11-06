#include "../../../inc/slib/ui/app.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(UIApp, Application)

Ref<UIApp> UIApp::getApp()
{
	Ref<Application> app(Application::getApp());
	if (UIApp::checkInstance(app.ptr)) {
		return Ref<UIApp>::from(app);
	}
	return Ref<UIApp>::null();
}

AppType UIApp::getAppType()
{
	return AppType::UI;
}

void UIApp::quit()
{
	UI::quitApp();
}

Ref<Window> UIApp::getMainWindow()
{
	return m_mainWindow;
}

void UIApp::setMainWindow(const Ref<Window>& window)
{
	void* _thiz = this;
	if (_thiz) {
		m_mainWindow = window;
	}
}

Ref<Menu> UIApp::getMenu()
{
	return m_mainMenu;
}

#if !defined(SLIB_PLATFORM_IS_OSX)

void UIApp::setMenu(const Ref<Menu>& menu)
{
	m_mainMenu = menu;
}

#endif

void UIApp::onStart()
{
}

void UIApp::onExit()
{
}

void UIApp::onRunApp()
{
	UI::runApp();
}

void UIApp::dispatchStart()
{
	onStart();
}

void UIApp::dispatchStartToApp()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchStart();
	}
}

void UIApp::dispatchExit()
{
	onExit();
}

void UIApp::dispatchExitToApp()
{
	Ref<UIApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchExit();
	}
}

SLIB_UI_NAMESPACE_END

