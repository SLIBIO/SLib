#include "../../../inc/slib/ui/mobile_app.h"
#include "../../../inc/slib/ui/view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(MobileApp, UIApp)

MobileApp::MobileApp()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setMainWindow(new MobileMainWindow);
}

Ref<MobileApp> MobileApp::getApp()
{
	Ref<Application> app(Application::getApp());
	if (MobileApp::checkInstance(app.ptr)) {
		return Ref<MobileApp>::from(app);
	}
	return Ref<MobileApp>::null();
}

Ref<MobileMainWindow> MobileApp::getMainWindow()
{
	Ref<Window> window = UIApp::getMainWindow();
	if (MobileMainWindow::checkInstance(window.ptr)) {
		return Ref<MobileMainWindow>::from(window);
	}
	return Ref<MobileMainWindow>::null();
}

sl_bool MobileApp::m_flagPaused = sl_false;

sl_bool MobileApp::isPaused()
{
	return m_flagPaused;
}

void MobileApp::addView(const Ref<View>& view)
{
	if (this) {
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->addView(view);
		}
	}
}

void MobileApp::removeView(const Ref<View>& view)
{
	if (this) {
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->removeView(view);
		}
	}
}

void MobileApp::onPause()
{
}

void MobileApp::onResume()
{
}

void MobileApp::onBack(UIEvent* ev)
{
}

void MobileApp::onCreateActivity()
{
}

void MobileApp::onDestroyActivity()
{
}

void MobileApp::dispatchPause()
{
	onPause();
}

void MobileApp::dispatchPauseToApp()
{
	m_flagPaused = sl_true;
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchPause();
	}
}

void MobileApp::dispatchResume()
{
	onResume();
}

void MobileApp::dispatchResumeToApp()
{
	m_flagPaused = sl_false;
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchResume();
	}
}

void MobileApp::dispatchBack(UIEvent* ev)
{
	onBack(ev);
}

sl_bool MobileApp::dispatchBackToApp()
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			app->dispatchBack(ev.ptr);
			if (ev->isPreventedDefault()) {
				return sl_false;
			}
		}
	}
	return sl_true;
}

void MobileApp::dispatchCreateActivity()
{
	Ref<MobileMainWindow> window = getMainWindow();
	if (window.isNotNull()) {
		window->create();
	}
	
	onCreateActivity();
}

void MobileApp::dispatchCreateActivityToApp()
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchCreateActivity();
	}
}

void MobileApp::dispatchDestroyActivity()
{
	onDestroyActivity();
}

void MobileApp::dispatchDestroyActivityToApp()
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchDestroyActivity();
	}
}


SLIB_DEFINE_OBJECT(MobileMainWindow, Window)

MobileMainWindow::MobileMainWindow()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
}

SLIB_UI_NAMESPACE_END

