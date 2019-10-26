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

#ifndef CHECKHEADER_SLIB_VIEW_PAGE
#define CHECKHEADER_SLIB_VIEW_PAGE

#include "definition.h"

#include "view_page_navigation.h"

namespace slib
{

	class SLIB_EXPORT ViewPage : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewPage();
		
		~ViewPage();

	public:
		Ref<ViewPageNavigationController> getNavigationController();
		
		void setNavigationController(const Ref<ViewPageNavigationController>& controller);
		
		void setTransition(const Transition& opening, const Transition& closing);
		
		
		void open(const Ref<ViewPageNavigationController>& controller, const Transition& transition);
		
		void open(const Ref<ViewPageNavigationController>& controller);
		
		void openHome(const Ref<ViewPageNavigationController>& controller, const Transition& transition);
		
		void openHome(const Ref<ViewPageNavigationController>& controller);
		
		void close(const Transition& transition);
		
		void close();
		
		void goToPage(const Ref<View>& pageOther, const Transition& transition);
		
		void goToPage(const Ref<View>& pageOther);
		
		void goToHomePage(const Ref<View>& pageOther, const Transition& transition);
		
		void goToHomePage(const Ref<View>& pageOther);
		
		
		void popup(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
		
		void popup(const Ref<View>& parent, sl_bool flagFillParentBackground = sl_true);

		Ref<Window> popupWindow(const Ref<Window>& parent, sl_ui_len width = 0, sl_ui_len height = 0);

		sl_bool isPopup();
		
		Color getPopupBackgroundColor();
		
		void setPopupBackgroundColor(const Color& color);
		
		void setCloseOnClickBackground();
		
		static void setDefaultPopupTransition(const Transition& opening, const Transition& closing);
		
		static Color getDefaultPopupBackgroundColor();
		
		static void setDefaultPopupBackgroundColor(const Color& color);

	public:
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Open)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Close)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Resume)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Pause)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, PageAction, ViewPageNavigationController* controller, UIPageAction action)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, EndPageAnimation, ViewPageNavigationController* controller, UIPageAction action)
		// For mobile platforms
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, BackPressed, UIEvent* ev)
		// Pressed mobile back button or Closed popup window
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Back, UIEvent* ev)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, ClickBackground, UIEvent* ev)

	public:
		using View::dispatchOK;
		void dispatchOK(UIEvent* ev) override;

		using View::dispatchCancel;
		void dispatchCancel(UIEvent* ev) override;
		
	protected:
		void _openPopup(const Ref<View>& parent, Transition transition, sl_bool flagFillParentBackground);
		
		void _closePopup(Transition transition);
		
		void _finishPopupAnimation(UIPageAction action);
				
		void _applyDefaultOpeningPopupTransition(Transition& transition);
		
		void _applyDefaultClosingPopupTransition(Transition& transition);
		
		void _onClosePopupWindow(Window* window, UIEvent* ev);

	protected:
		AtomicWeakRef<ViewPageNavigationController> m_navigationController;
		
		enum class PopupState
		{
			None, Popup, ClosingPopup, ShowWindow
		};
		PopupState m_popupState;
		
		Transition m_openingTransition;
		Transition m_closingTransition;
		
		Color m_popupBackgroundColor;

		sl_reg m_countActiveTransitionAnimations;
		
		friend class ViewPageNavigationController;
		
	};

}

#endif
