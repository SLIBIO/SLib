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

#ifndef CHECKHEADER_SLIB_UI_MOBILE_APP
#define CHECKHEADER_SLIB_UI_MOBILE_APP

#include "definition.h"

#include "app.h"
#include "event.h"
#include "window.h"
#include "view_page.h"

namespace slib
{

	class MobileMainWindow;
	
	class SLIB_EXPORT MobileApp : public UIApp
	{
		SLIB_DECLARE_OBJECT
		
	public:
		MobileApp();
		
		~MobileApp();

	protected:
		void init() override;

	public:
		static Ref<MobileApp> getApp();
		
		Ref<MobileMainWindow> getMainWindow();
		
		static sl_bool isPaused();
		
		Ref<View> getRootView();
		
		Ref<View> getContentView();
		
		Ref<ViewPageNavigationController> getNavigationController();
		
		virtual Ref<View> getLoadingPage();

		virtual Ref<View> getStartupPage();
		
		void addViewToRoot(const Ref<View>& view);
		
		void addViewToContent(const Ref<View>& view);
		
		void openPage(const Ref<View>& page, const Transition& transition);
		
		void openPage(const Ref<View>& page);
		
		void openHomePage(const Ref<View>& page, const Transition& transition);
		
		void openHomePage(const Ref<View>& page);
		
		void closePage(const Ref<View>& page, const Transition& transition);
		
		void closePage(const Ref<View>& page);
		
		void closePage(const Transition& transition);
		
		void closePage();
		
		void popupPage(const Ref<ViewPage>& page, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
		
		void popupPage(const Ref<ViewPage>& page, sl_bool flagFillParentBackground = sl_true);
		
		void closePopup(const Ref<ViewPage>& page, const Transition& transition);
		
		void closePopup(const Ref<ViewPage>& page);
		
		void closePopup(const Transition& transition);
		
		void closePopup();
		
		void openStartupPage();
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, Pause)
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, Resume)
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, BackPressed, UIEvent* ev)
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, CreateActivity)
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, DestroyActivity)
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, Resize, sl_ui_len width, sl_ui_len height)
		SLIB_DECLARE_EVENT_HANDLER(MobileApp, ChangeCurrentLocale)

	public:
		void dispatchStart() override;

		static void dispatchPauseToApp();
		
		static void dispatchResumeToApp();
		
		static sl_bool dispatchBackPressedToApp();
		
		static void dispatchCreateActivityToApp();
		
		static void dispatchDestroyActivityToApp();
		
		static void dispatchResizeToApp(sl_ui_len width, sl_ui_len height);
		
	protected:
		static sl_bool m_flagPaused;
		
		Ref<View> m_contentView;
		Ref<ViewPageNavigationController> m_navigationController;
		CList< Ref<ViewPage> > m_popupPages;
		Function<void()> m_callbackOnChangeLocale;
		
		friend class ViewPage;
		
	};
	
	class SLIB_EXPORT MobileMainWindow : public Window
	{
		SLIB_DECLARE_OBJECT
		
	public:
		MobileMainWindow();
		
		~MobileMainWindow();

	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
	};

}

#endif
