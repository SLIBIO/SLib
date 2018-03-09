/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

	public:
		static Ref<MobileApp> getApp();
		
		Ref<MobileMainWindow> getMainWindow();
		
		static sl_bool isPaused();
		
		Ref<View> getRootView();
		
		Ref<View> getContentView();
		
		Ref<ViewPager> getPager();
		
		Ref<View> getStartupPage();
		
		void setStartupPage(const Ref<View>& page);
		
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
		
	protected:
		virtual void onPause();
		
		virtual void onResume();
		
		virtual void onBack(UIEvent* ev);
		
		virtual void onCreateActivity();
		
		virtual void onDestroyActivity();
		
		virtual void onResize(sl_ui_len width, sl_ui_len height);
		
	public:
		void dispatchStart() override;
		
		virtual void dispatchPause();
		
		static void dispatchPauseToApp();
		
		virtual void dispatchResume();
		
		static void dispatchResumeToApp();
		
		virtual void dispatchBack(UIEvent* ev);
		
		static sl_bool dispatchBackToApp();
		
		virtual void dispatchCreateActivity();
		
		static void dispatchCreateActivityToApp();
		
		virtual void dispatchDestroyActivity();
		
		static void dispatchDestroyActivityToApp();
		
		virtual void dispatchResize(sl_ui_len width, sl_ui_len height);
		
		static void dispatchResizeToApp(sl_ui_len width, sl_ui_len height);
		
	protected:
		static sl_bool m_flagPaused;
		
		Ref<View> m_contentView;
		Ref<ViewPager> m_pager;
		Ref<View> m_startupPage;
		CList< Ref<ViewPage> > m_popupPages;
		
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
		
		void onClose(UIEvent* ev) override;
		
	};

}

#endif
