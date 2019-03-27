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

#include "slib/ui/mobile_app.h"

#include "slib/ui/view.h"
#include "slib/ui/resource.h"
#include "slib/ui/animation.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(MobileApp, UIApp)

	MobileApp::MobileApp()
	{
	}

	MobileApp::~MobileApp()
	{
		Locale::removeOnChangeCurrentLocale(m_callbackOnChangeLocale);
	}

	void MobileApp::init()
	{
		UIApp::init();
		
		Ref<MobileMainWindow> window = new MobileMainWindow;
		setMainWindow(window);
		
		m_contentView = window->getContentView();
		
		m_navigationController = new ViewPageNavigationController;
		m_navigationController->setWidthFilling(1, UIUpdateMode::Init);
		m_navigationController->setHeightFilling(1, UIUpdateMode::Init);
		m_navigationController->setOpaque(sl_true, UIUpdateMode::Init);
		m_navigationController->setBackgroundColor(Color::White);
		m_navigationController->setVisibility(Visibility::Hidden, UIUpdateMode::Init);
		m_contentView->addChild(m_navigationController, UIUpdateMode::Init);

		m_callbackOnChangeLocale = SLIB_FUNCTION_CLASS(MobileApp, dispatchChangeCurrentLocale, this);
		Locale::addOnChangeCurrentLocale(m_callbackOnChangeLocale);
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
	
	Ref<ViewPageNavigationController> MobileApp::getNavigationController()
	{
		return m_navigationController;
	}
	
	Ref<View> MobileApp::getLoadingPage()
	{
		return getStartupPage();
	}

	Ref<View> MobileApp::getStartupPage()
	{
		return sl_null;
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
		m_navigationController->push(page, transition);
	}
	
	void MobileApp::openPage(const Ref<View>& page)
	{
		m_navigationController->push(page);
	}
	
	void MobileApp::openHomePage(const Ref<View>& page, const Transition& transition)
	{
		m_navigationController->pushPageAfterPopAllPages(page, transition);
	}
	
	void MobileApp::openHomePage(const Ref<View>& page)
	{
		m_navigationController->pushPageAfterPopAllPages(page);
	}
	
	void MobileApp::closePage(const Ref<View>& page, const Transition& transition)
	{
		m_navigationController->pop(page, transition);
	}
	
	void MobileApp::closePage(const Ref<View>& page)
	{
		m_navigationController->pop(page);
	}
	
	void MobileApp::closePage(const Transition& transition)
	{
		m_navigationController->pop(transition);
	}
	
	void MobileApp::closePage()
	{
		m_navigationController->pop();
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
		if (page.isNull()) {
			return;
		}
		Ref<View> content = m_contentView;
		if (content.isNotNull()) {
			page->popup(content, flagFillParentBackground);
		}
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
		if (page.isNull()) {
			return;
		}
		ListLocker< Ref<ViewPage> > popups(m_popupPages);
		if (popups.count > 0) {
			if (page == popups[popups.count-1]) {
				page->close();
			}
		}
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
		ListLocker< Ref<ViewPage> > popups(m_popupPages);
		if (popups.count > 0) {
			Ref<ViewPage> page = popups[popups.count-1];
			if (page.isNotNull()) {
				page->close();
			}
		}
	}
	
	void MobileApp::openStartupPage()
	{
		Ref<View> page = getStartupPage();
		if (page.isNotNull()) {
			openHomePage(page, TransitionType::None);
		}
	}
	
	void MobileApp::dispatchStart()
	{
		UIApp::dispatchStart();
#ifdef SLIB_PLATFORM_IS_DESKTOP
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->forceCreate();
		}
#endif
	}
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, Pause)

	void MobileApp::dispatchPause()
	{
		SLIB_INVOKE_EVENT_HANDLER(Pause)
		
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			Ref<View> page = controller->getCurrentPage();
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
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, Resume)

	void MobileApp::dispatchResume()
	{
		SLIB_INVOKE_EVENT_HANDLER(Resume)
		
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			Ref<View> page = controller->getCurrentPage();
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->dispatchResume();
			}
		}
		ListLocker< Ref<ViewPage> > popups(m_popupPages);
		for (sl_size i = 0; i < popups.count; i++) {
			Ref<ViewPage> page = popups[i];
			if (page.isNotNull()) {
				page->dispatchResume();
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
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, BackPressed, UIEvent* ev)

	void MobileApp::dispatchBackPressed(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(BackPressed, ev)
		
		if (ev->isPreventedDefault()) {
			return;
		}
		{
			ListLocker< Ref<ViewPage> > popups(m_popupPages);
			if (popups.count > 0) {
				Ref<ViewPage> page = popups[popups.count-1];
				if (page.isNotNull()) {
					page->dispatchBackPressed(ev);
					if (!(ev->isPreventedDefault())) {
						page->close();
						ev->preventDefault();
					}
				}
				return;
			}
		}
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			Ref<View> _page = controller->getCurrentPage();
			if (ViewPage* page = CastInstance<ViewPage>(_page.get())) {
				page->dispatchBackPressed(ev);
				if (!(ev->isPreventedDefault())) {
					if (controller->getPagesCount() > 1) {
						page->close();
						ev->preventDefault();
					}
				}
			}
		}
	}
	
	sl_bool MobileApp::dispatchBackPressedToApp()
	{
		Ref<MobileApp> app = getApp();
		if (app.isNotNull()) {
			Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
			if (ev.isNotNull()) {
				app->dispatchBackPressed(ev.get());
				if (ev->isPreventedDefault()) {
					return sl_false;
				}
			}
		}
		return sl_true;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, CreateActivity)

	void MobileApp::dispatchCreateActivity()
	{
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->forceCreate();
		}
		
		SLIB_INVOKE_EVENT_HANDLER(CreateActivity)
	}
	
	void MobileApp::dispatchCreateActivityToApp()
	{
		Ref<MobileApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchCreateActivity();
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, DestroyActivity)
	
	void MobileApp::dispatchDestroyActivity()
	{
		SLIB_INVOKE_EVENT_HANDLER(DestroyActivity)
	}
	
	void MobileApp::dispatchDestroyActivityToApp()
	{
		Ref<MobileApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchDestroyActivity();
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, Resize, sl_ui_len width, sl_ui_len height)

	void MobileApp::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		UIResource::updateDefaultScreenSize();
		
		if (m_navigationController->getPagesCount() == 0) {
			Ref<View> page = getLoadingPage();
			if (page.isNotNull()) {
				m_navigationController->setVisibility(Visibility::Visible);
				openHomePage(page, TransitionType::None);
			}
		}
		
		SLIB_INVOKE_EVENT_HANDLER(Resize, width, height);
	}
	
	void MobileApp::dispatchResizeToApp(sl_ui_len width, sl_ui_len height)
	{
		Ref<MobileApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchResize(width, height);
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(MobileApp, ChangeCurrentLocale)

	void MobileApp::dispatchChangeCurrentLocale()
	{
		if (m_navigationController->getPagesCount() > 0) {
			openStartupPage();
		}
		
		SLIB_INVOKE_EVENT_HANDLER(ChangeCurrentLocale)
	}
	
	
	SLIB_DEFINE_OBJECT(MobileMainWindow, Window)
	
	MobileMainWindow::MobileMainWindow()
	{
	}
	
	MobileMainWindow::~MobileMainWindow()
	{
	}

	void MobileMainWindow::onResize(sl_ui_len width, sl_ui_len height)
	{
		MobileApp::dispatchResizeToApp(width, height);
	}

}
