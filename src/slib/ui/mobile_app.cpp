#include "../../../inc/slib/ui/mobile_app.h"

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/resource.h"
#include "../../../inc/slib/ui/animation.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(MobileApp, UIApp)
	
	MobileApp::MobileApp()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		Ref<MobileMainWindow> window = new MobileMainWindow;
		setMainWindow(window);
		
		m_contentView = window->getContentView();
		
		m_pager = new ViewPager;
		m_pager->setOpaque(sl_true);
		m_pager->setMinimumPagesCount(1);
		m_contentView->addChild(m_pager, UIUpdateMode::Init);
		
	}
	
	Ref<MobileApp> MobileApp::getApp()
	{
		return CastRef<MobileApp>(Application::getApp());
	}
	
	Ref<MobileMainWindow> MobileApp::getMainWindow()
	{
		return CastRef<MobileMainWindow>(UIApp::getMainWindow());
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
		return sl_null;
	}
	
	Ref<View> MobileApp::getContentView()
	{
		return m_contentView;
	}
	
	Ref<ViewPager> MobileApp::getPager()
	{
		return m_pager;
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
		m_pager->push(page, transition);
	}
	
	void MobileApp::openPage(const Ref<View>& page)
	{
		m_pager->push(page);
	}
	
	void MobileApp::openHomePage(const Ref<View>& page, const Transition& transition)
	{
		m_pager->push(page, transition, sl_true);
	}
	
	void MobileApp::openHomePage(const Ref<View>& page)
	{
		m_pager->push(page, sl_true);
	}
	
	void MobileApp::closePage(const Ref<View>& page, const Transition& transition)
	{
		m_pager->pop(page, transition);
	}
	
	void MobileApp::closePage(const Ref<View>& page)
	{
		m_pager->pop(page);
	}
	
	void MobileApp::closePage(const Transition& transition)
	{
		m_pager->pop(transition);
	}
	
	void MobileApp::closePage()
	{
		m_pager->pop();
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
		onPause();
		Ref<ViewPager> pager = m_pager;
		if (pager.isNotNull()) {
			Ref<View> page = pager->getCurrentPage();
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->dispatchPause();
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
		{
			Ref<UIAnimationLoop> al = UIAnimationLoop::getInstance();
			if (al.isNotNull()) {
				al->pause();
			}
		}
		{
			Ref<AnimationLoop> al = AnimationLoop::getDefault();
			if (al.isNotNull()) {
				al->pause();
			}
		}
	}
	
	void MobileApp::dispatchResume()
	{
		onResume();
		Ref<ViewPager> pager = m_pager;
		if (pager.isNotNull()) {
			Ref<View> page = pager->getCurrentPage();
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->dispatchPause();
			}
		}
		ListLocker< Ref<ViewPage> > popups(m_popupPages);
		for (sl_size i = 0; i < popups.count; i++) {
			Ref<ViewPage> page = popups[i];
			if (page.isNotNull()) {
				page->dispatchPause();
			}
		}
		{
			Ref<UIAnimationLoop> al = UIAnimationLoop::getInstance();
			if (al.isNotNull()) {
				al->resume();
			}
		}
		{
			Ref<AnimationLoop> al = AnimationLoop::getDefault();
			if (al.isNotNull()) {
				al->resume();
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
					ev->preventDefault();
				}
				return;
			}
		}
		if (m_pager->getPagesCount() > 1) {
			Ref<View> page = m_pager->getCurrentPage();
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->dispatchBackPressed(ev);
				ev->preventDefault();
			}
			return;
		}
	}
	
	sl_bool MobileApp::dispatchBackToApp()
	{
		Ref<MobileApp> app = getApp();
		if (app.isNotNull()) {
			Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
			if (ev.isNotNull()) {
				app->dispatchBack(ev.get());
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
		m_pager->setFrame(0, 0, width, height);
		if (m_pager->getPagesCount() == 0) {
			Ref<View> page = m_startupPage;
			if (page.isNotNull()) {
				openHomePage(page);
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
}
