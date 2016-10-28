#include "../../../inc/slib/ui/mobile_app.h"

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/resource.h"
#include "../../../inc/slib/core/animation.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(MobileApp, UIApp)

MobileApp::MobileApp()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	Ref<MobileMainWindow> window = new MobileMainWindow;
	setMainWindow(window);
	
	m_contentView = window->getContentView();
	
	m_pageStack = new ViewStack;
	m_contentView->addChild(m_pageStack, UIUpdateMode::Init);
	
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

Ref<View> MobileApp::getRootView()
{
	Ref<Window> window = UIApp::getMainWindow();
	if (window.isNotNull()) {
		return window->getContentView();
	}
	return Ref<View>::null();
}

Ref<View> MobileApp::getContentView()
{
	return m_contentView;
}

Ref<ViewStack> MobileApp::getPageStack()
{
	return m_pageStack;
}

Ref<View> MobileApp::getStartupPage()
{
	return m_startupPage;
}

void MobileApp::setStartupPage(const Ref<View>& page)
{
	m_startupPage = page;
}

void MobileApp::addViewToRoot(const Ref<View>& view)
{
	Ref<MobileMainWindow> window = getMainWindow();
	if (window.isNotNull()) {
		window->addView(view);
	}
}

void MobileApp::addViewToContent(const Ref<View>& view)
{
	Ref<View> content = m_contentView;
	if (content.isNotNull()) {
		content->addChild(view);
	}
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

void MobileApp::popupPage(const Ref<ViewPage>& page, const Transition& transition, sl_bool flagFillParentBackground)
{
	if (page.isNull()) {
		return;
	}
	Ref<View> content = m_contentView;
	if (content.isNotNull()) {
		page->popup(content, transition, flagFillParentBackground);
	}
}

void MobileApp::popupPage(const Ref<ViewPage>& page, sl_bool flagFillParentBackground)
{
	popupPage(page, Transition(), flagFillParentBackground);
}

void MobileApp::closePopup(const Ref<ViewPage>& page, const Transition& transition)
{
	if (page.isNull()) {
		return;
	}
	ListLocker< Ref<ViewPage> > popups(m_popupPages);
	if (popups.count > 0) {
		if (page == popups[popups.count-1]) {
			page->close(transition);
		}
	}
}

void MobileApp::closePopup(const Ref<ViewPage>& page)
{
	closePopup(page, Transition());
}

void MobileApp::closePopup(const Transition& transition)
{
	ListLocker< Ref<ViewPage> > popups(m_popupPages);
	if (popups.count > 0) {
		Ref<ViewPage> page = popups[popups.count-1];
		if (page.isNotNull()) {
			page->close(transition);
		}
	}
}

void MobileApp::closePopup()
{
	closePopup(Transition());
}

void MobileApp::modalPage(const Ref<ViewPage>& page, const Transition& transition, sl_bool flagFillParentBackground)
{
	if (page.isNull()) {
		return;
	}
	Ref<View> content = m_contentView;
	if (content.isNotNull()) {
		page->modal(content, transition, flagFillParentBackground);
	}
}

void MobileApp::modalPage(const Ref<ViewPage>& page, sl_bool flagFillParentBackground)
{
	modalPage(page, Transition(), flagFillParentBackground);
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

void MobileApp::dispatchPause()
{
	Animation::pauseAnimationCenter();
	onPause();
	Ref<ViewStack> stack = m_pageStack;
	if (stack.isNotNull()) {
		Ref<View> page = stack->getCurrentPage();
		if (ViewPage::checkInstance(page.ptr)) {
			((ViewPage*)(page.ptr))->dispatchPause();
		}
	}
	ListLocker< Ref<ViewPage> > popups(m_popupPages);
	for (sl_size i = 0; i < popups.count; i++) {
		Ref<ViewPage> page = popups[i];
		if (page.isNotNull()) {
			page->dispatchPause();
		}
	}
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
	Ref<ViewStack> stack = m_pageStack;
	if (stack.isNotNull()) {
		Ref<View> page = stack->getCurrentPage();
		if (ViewPage::checkInstance(page.ptr)) {
			((ViewPage*)(page.ptr))->dispatchPause();
		}
	}
	ListLocker< Ref<ViewPage> > popups(m_popupPages);
	for (sl_size i = 0; i < popups.count; i++) {
		Ref<ViewPage> page = popups[i];
		if (page.isNotNull()) {
			page->dispatchPause();
		}
	}
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
	{
		ListLocker< Ref<ViewPage> > popups(m_popupPages);
		if (popups.count > 0) {
			Ref<ViewPage> page = popups[popups.count-1];
			if (page.isNotNull()) {
				page->dispatchBackPressed(ev);
				if (ev->isPreventedDefault()) {
					return;
				}
				page->close();
			}
			ev->preventDefault();
			return;
		}
	}
	if (m_pageStack->getPagesCount() > 1) {
		Ref<View> page = m_pageStack->getCurrentPage();
		if (ViewPage::checkInstance(page.ptr)) {
			((ViewPage*)(page.ptr))->dispatchBackPressed(ev);
			if (ev->isPreventedDefault()) {
				return;
			}
		}
		m_pageStack->pop();
		ev->preventDefault();
		return;
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
	UIResource::updateDefaultScreenSize();
	m_pageStack->setFrame(0, 0, width, height);
	if (!m_flagStartedPage) {
		Ref<View> page = m_startupPage;
		if (page.isNotNull()) {
			openHomePage(page);
			m_flagStartedPage = sl_true;
		}
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

