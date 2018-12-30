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
		
		static Function<void()> addOnPause(const Function<void()>& callback);
		
		static void removeOnPause(const Function<void()>& callback);

		static void dispatchPauseToApp();
		
		virtual void dispatchResume();
		
		static Function<void()> addOnResume(const Function<void()>& callback);
		
		static void removeOnResume(const Function<void()>& callback);
		
		static void dispatchResumeToApp();
		
		virtual void dispatchBack(UIEvent* ev);
		
		static Function<void()> addOnBack(const Function<void()>& callback);
		
		static void removeOnBack(const Function<void()>& callback);
		
		static sl_bool dispatchBackToApp();
		
		virtual void dispatchCreateActivity();
		
		static void dispatchCreateActivityToApp();
		
		virtual void dispatchDestroyActivity();
		
		static void dispatchDestroyActivityToApp();
		
		virtual void dispatchResize(sl_ui_len width, sl_ui_len height);
		
		static Function<void(sl_ui_len, sl_ui_len)> addOnResize(const Function<void(sl_ui_len, sl_ui_len)>& callback);
		
		static void removeOnResize(const Function<void(sl_ui_len, sl_ui_len)>& callback);

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
