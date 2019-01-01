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
		SLIB_REFERABLE_CONSTRUCTOR
		
		Ref<MobileMainWindow> window = new MobileMainWindow;
		setMainWindow(window);
		
		m_contentView = window->getContentView();
		
		m_pager = new ViewPager;
		m_pager->setWidthFilling(1, UIUpdateMode::Init);
		m_pager->setHeightFilling(1, UIUpdateMode::Init);
		m_pager->setOpaque(sl_true, UIUpdateMode::Init);
		m_pager->setBackgroundColor(Color::White);
		m_pager->setMinimumPagesCount(1);
		m_pager->setVisibility(Visibility::Hidden, UIUpdateMode::Init);
		m_contentView->addChild(m_pager, UIUpdateMode::Init);

		m_callbackOnChangeLocale = SLIB_FUNCTION_CLASS(MobileApp, onChangeCurrentLocale, this);
		Locale::addOnChangeCurrentLocale(m_callbackOnChangeLocale);
	}
	
	MobileApp::~MobileApp()
	{
		Locale::removeOnChangeCurrentLocale(m_callbackOnChangeLocale);
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
	
	void MobileApp::openStartupPage()
	{
		Ref<View> page = getStartupPage();
		if (page.isNotNull()) {
			Transition transition;
			transition.type = TransitionType::None;
			openHomePage(page, transition);
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
	
	void MobileApp::onResize(sl_ui_len width, sl_ui_len height)
	{
	}
	
	void MobileApp::onChangeCurrentLocale()
	{
		openStartupPage();
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
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<void()> >, _g_priv_ui_mobile_app_callback_list_onPause)
	
	Function<void()> MobileApp::addOnPause(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onPause))) {
			_g_priv_ui_mobile_app_callback_list_onPause.addIfNotExist(callback);
		}
		return callback;
	}
	
	void MobileApp::removeOnPause(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onPause))) {
			_g_priv_ui_mobile_app_callback_list_onPause.remove(callback);
		}
	}
	
	void MobileApp::dispatchPauseToApp()
	{
		m_flagPaused = sl_true;
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onPause))) {
			ListLocker< Function<void()> > list(_g_priv_ui_mobile_app_callback_list_onPause);
			for (sl_size i = 0; i < list.count; i++) {
				list[i]();
			}
		}
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
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<void()> >, _g_priv_ui_mobile_app_callback_list_onResume)
	
	Function<void()> MobileApp::addOnResume(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onResume))) {
			_g_priv_ui_mobile_app_callback_list_onResume.addIfNotExist(callback);
		}
		return callback;
	}
	
	void MobileApp::removeOnResume(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onResume))) {
			_g_priv_ui_mobile_app_callback_list_onResume.remove(callback);
		}
	}
	
	void MobileApp::dispatchResumeToApp()
	{
		m_flagPaused = sl_false;
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onResume))) {
			ListLocker< Function<void()> > list(_g_priv_ui_mobile_app_callback_list_onResume);
			for (sl_size i = 0; i < list.count; i++) {
				list[i]();
			}
		}
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
		}
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<void()> >, _g_priv_ui_mobile_app_callback_list_onBack)
	
	Function<void()> MobileApp::addOnBack(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onBack))) {
			_g_priv_ui_mobile_app_callback_list_onBack.addIfNotExist(callback);
		}
		return callback;
	}
	
	void MobileApp::removeOnBack(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onBack))) {
			_g_priv_ui_mobile_app_callback_list_onBack.remove(callback);
		}
	}
	
	sl_bool MobileApp::dispatchBackToApp()
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onBack))) {
			ListLocker< Function<void()> > list(_g_priv_ui_mobile_app_callback_list_onBack);
			for (sl_size i = 0; i < list.count; i++) {
				list[i]();
			}
		}
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
		if (m_pager->getPagesCount() == 0) {
			Ref<View> page = getLoadingPage();
			if (page.isNotNull()) {
				m_pager->setVisibility(Visibility::Visible);
				Transition transition;
				transition.type = TransitionType::None;
				openHomePage(page, transition);
			}
		}
		onResize(width, height);
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<void(sl_ui_len, sl_ui_len)> >, _g_priv_ui_mobile_app_callback_list_onResize)
	
	Function<void(sl_ui_len, sl_ui_len)> MobileApp::addOnResize(const Function<void(sl_ui_len, sl_ui_len)>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onResize))) {
			_g_priv_ui_mobile_app_callback_list_onResize.addIfNotExist(callback);
		}
		return callback;
	}
	
	void MobileApp::removeOnResize(const Function<void(sl_ui_len, sl_ui_len)>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onResize))) {
			_g_priv_ui_mobile_app_callback_list_onResize.remove(callback);
		}
	}
	
	void MobileApp::dispatchResizeToApp(sl_ui_len width, sl_ui_len height)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_ui_mobile_app_callback_list_onResize))) {
			ListLocker< Function<void(sl_ui_len,sl_ui_len)> > list(_g_priv_ui_mobile_app_callback_list_onResize);
			for (sl_size i = 0; i < list.count; i++) {
				list[i](width, height);
			}
		}
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
	
	MobileMainWindow::~MobileMainWindow()
	{
	}

	void MobileMainWindow::onResize(sl_ui_len width, sl_ui_len height)
	{
		MobileApp::dispatchResizeToApp(width, height);
	}
	
	void MobileMainWindow::onClose(UIEvent* ev)
	{
#ifdef SLIB_PLATFORM_IS_DESKTOP
		MobileApp::quit();
#endif
	}

}
