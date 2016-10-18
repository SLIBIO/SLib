#include "../../../inc/slib/ui/mobile_app.h"
#include "../../../inc/slib/ui/view.h"

#include "../../../inc/slib/core/animation.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(MobileApp, UIApp)

MobileApp::MobileApp()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	Ref<MobileMainWindow> window = new MobileMainWindow;
	setMainWindow(window);
	
	m_pageStack = new ViewStack;
	window->addView(m_pageStack);
	
	m_flagStartedPage = sl_false;
	
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
	void* _thiz = this;
	if (_thiz) {
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->addView(view);
		}
	}
}

void MobileApp::removeView(const Ref<View>& view)
{
	void* _thiz = this;
	if (_thiz) {
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->removeView(view);
		}
	}
}

Ref<ViewStack> MobileApp::getPageStack()
{
	return m_pageStack;
}

void MobileApp::openPage(const Ref<View>& page, const Transition& transition)
{
	m_pageStack->push(page, transition);
}

void MobileApp::openPage(const Ref<View>& page)
{
	m_pageStack->push(page);
}

void MobileApp::openHomePage(const Ref<View>& page, const Transition& transition)
{
	m_pageStack->push(page, transition, sl_true);
}

void MobileApp::openHomePage(const Ref<View>& page)
{
	m_pageStack->push(page, sl_true);
}

void MobileApp::closePage(const Ref<View>& page, const Transition& transition)
{
	m_pageStack->pop(page, transition);
}

void MobileApp::closePage(const Ref<View>& page)
{
	m_pageStack->pop(page);
}

void MobileApp::closePage(const Transition& transition)
{
	m_pageStack->pop(transition);
}

void MobileApp::closePage()
{
	m_pageStack->pop();
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

void MobileApp::onResize(sl_ui_len width, sl_ui_len height)
{
}

void MobileApp::onStartPage()
{
}

void MobileApp::dispatchPause()
{
	Animation::pauseAnimationCenter();
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
	Animation::resumeAnimationCenter();
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
	if (ev->isPreventedDefault()) {
		return;
	}
	if (m_pageStack->getPagesCount() > 1) {
		m_pageStack->pop();
		ev->preventDefault();
	}
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
		window->forceCreate();
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

void MobileApp::dispatchResize(sl_ui_len width, sl_ui_len height)
{
	m_pageStack->setFrame(0, 0, width, height);
	if (!m_flagStartedPage) {
		m_flagStartedPage = sl_true;
		onStartPage();
	}
	onResize(width, height);
}

void MobileApp::dispatchResizeToApp(sl_ui_len width, sl_ui_len height)
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchResize(width, height);
	}
}


SLIB_DEFINE_OBJECT(MobileMainWindow, Window)

MobileMainWindow::MobileMainWindow()
{
	SLIB_REFERABLE_CONSTRUCTOR;
}

void MobileMainWindow::onResize(sl_ui_len width, sl_ui_len height)
{
	MobileApp::dispatchResizeToApp(width, height);
}

SLIB_UI_NAMESPACE_END

