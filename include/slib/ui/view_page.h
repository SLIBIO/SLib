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

#include "view.h"
#include "transition.h"
#include "window.h"

#include "../core/event.h"

namespace slib
{

	class ViewPage;

	class SLIB_EXPORT ViewPager : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewPager();
		
		~ViewPager();

	public:
		sl_size getPagesCount();
		
		sl_size getCurrentPageIndex();
		
		Ref<View> getCurrentPage();
		
		sl_size getMinimumPagesCount();
		
		void setMinimumPagesCount(sl_size count);
		
		void addPage(const Ref<View>& page, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removePageAt(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void selectPage(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void goToPageAt(sl_size index, const Transition& transition);
		
		void goToPageAt(sl_size index);
		
		void goPrev(const Transition& transition);
		
		void goPrev();
		
		void goNext(const Transition& transition);
		
		void goNext();
		
		void push(const Ref<View>& page, const Transition& transition, sl_bool flagRemoveAllBackPages = sl_false);

		void push(const Ref<View>& page, sl_bool flagRemoveAllBackPages = sl_false);
		
		void pop(const Ref<View>& page, const Transition& transition);
		
		void pop(const Ref<View>& page);
		
		void pop(const Transition& transition);
		
		void pop();
		
		void setSwipeNavigation(sl_bool flag);
		
		TransitionType getPushTransitionType();
		
		void setPushTransitionType(TransitionType type);
		
		TransitionType getPopTransitionType();
		
		void setPopTransitionType(TransitionType type);
		
		void setTransitionType(TransitionType type);
		
		TransitionDirection getPushTransitionDirection();
		
		void setPushTransitionDirection(TransitionDirection direction);
		
		TransitionDirection getPopTransitionDirection();
		
		void setPopTransitionDirection(TransitionDirection direction);
		
		void setTransitionDirection(TransitionDirection direction);
		
		float getPushTransitionDuration();
		
		void setPushTransitionDuration(float duration);

		float getPopTransitionDuration();
		
		void setPopTransitionDuration(float duration);
		
		void setTransitionDuration(float duration);
		
		AnimationCurve getPushTransitionCurve();
		
		void setPushTransitionCurve(AnimationCurve curve);
		
		AnimationCurve getPopTransitionCurve();
		
		void setPopTransitionCurve(AnimationCurve curve);
		
		void setTransitionCurve(AnimationCurve curve);
		
	protected:
		void _onFinishAnimation(const Ref<View>& view, UIPageAction action);
		
		void _resetAnimationStatus(const Ref<View>& view);
			
		void _goTo(sl_size index, const Transition& transition);
		
		void _push(const Ref<View>& page, const Transition& transition, sl_bool flagRemoveAllBackPages);
		
		void _pop(const Ref<View>& page, const Transition& transition);
		
		void _applyDefaultPushTransition(Transition& transition);
		
		void _applyDefaultPopTransition(Transition& transition);
		
		static void _runAnimationProc(const Ref<View>& view, const Function<void()>& callback);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(ViewPager, PageAction, View* page, UIPageAction action)
		SLIB_DECLARE_EVENT_HANDLER(ViewPager, EndPageAnimation, View* page, UIPageAction action)

	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		void onChangePadding() override;
		
		void onSwipe(GestureEvent* ev) override;
		
	protected:
		CList< Ref<View> > m_pages;
		sl_size m_indexCurrent;
		sl_size m_minimumPagesCount;

		sl_bool m_flagSwipeNavigation;

		TransitionType m_pushTransitionType;
		TransitionType m_popTransitionType;
		TransitionDirection m_pushTransitionDirection;
		TransitionDirection m_popTransitionDirection;
		float m_pushTransitionDuration; // seconds
		float m_popTransitionDuration; // seconds
		AnimationCurve m_pushTransitionCurve;
		AnimationCurve m_popTransitionCurve;
		
		sl_reg m_countActiveTransitionAnimations;
		
		friend class ViewPage;
		
	};

	class SLIB_EXPORT ViewPage : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewPage();
		
		~ViewPage();

	public:
		Ref<ViewPager> getPager();
		
		void setPager(const Ref<ViewPager>& pager);
		
		
		void open(const Ref<ViewPager>& pager, const Transition& transition);
		
		void open(const Ref<ViewPager>& pager);
		
		void openHome(const Ref<ViewPager>& pager, const Transition& transition);
		
		void openHome(const Ref<ViewPager>& pager);
		
		void close(const Transition& transition);
		
		void close();
		
		void goToPage(const Ref<View>& pageOther, const Transition& transition);
		
		void goToPage(const Ref<View>& pageOther);
		
		void goToHomePage(const Ref<View>& pageOther, const Transition& transition);
		
		void goToHomePage(const Ref<View>& pageOther);
		
		
		TransitionType getGlobalOpeningTransitionType();
		
		void setGlobalOpeningTransitionType(TransitionType type);
		
		TransitionType getGlobalClosingTransitionType();
		
		void setGlobalClosingTransitionType(TransitionType type);
		
		void setGlobalTransitionType(TransitionType type);
		
		TransitionDirection getGlobalOpeningTransitionDirection();
		
		void setGlobalOpeningTransitionDirection(TransitionDirection direction);
		
		TransitionDirection getGlobalClosingTransitionDirection();
		
		void setGlobalClosingTransitionDirection(TransitionDirection direction);
		
		void setGlobalTransitionDirection(TransitionDirection direction);
		
		float getGlobalOpeningTransitionDuration();
		
		void setGlobalOpeningTransitionDuration(float duration);
		
		float getGlobalClosingTransitionDuration();
		
		void setGlobalClosingTransitionDuration(float duration);
		
		void setGlobalTransitionDuration(float duration);
		
		AnimationCurve getGlobalOpeningTransitionCurve();
		
		void setGlobalOpeningTransitionCurve(AnimationCurve curve);
		
		AnimationCurve getGlobalClosingTransitionCurve();
		
		void setGlobalClosingTransitionCurve(AnimationCurve curve);
		
		void setGlobalTransitionCurve(AnimationCurve curve);
		
		
		void popup(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
		
		void popup(const Ref<View>& parent, sl_bool flagFillParentBackground = sl_true);

		Ref<Window> popupWindow(const Ref<Window>& parent);

		sl_bool isPopup();
		
		Color getPopupBackgroundColor();
		
		void setPopupBackgroundColor(const Color& color);
		
		TransitionType getGlobalOpeningPopupTransitionType();
		
		void setGlobalOpeningPopupTransitionType(TransitionType type);
		
		TransitionType getGlobalClosingPopupTransitionType();
		
		void setGlobalClosingPopupTransitionType(TransitionType type);
		
		void setGlobalPopupTransitionType(TransitionType type);
		
		TransitionDirection getGlobalOpeningPopupTransitionDirection();
		
		void setGlobalOpeningPopupTransitionDirection(TransitionDirection direction);
		
		TransitionDirection getGlobalClosingPopupTransitionDirection();
		
		void setGlobalClosingPopupTransitionDirection(TransitionDirection direction);
		
		void setGlobalPopupTransitionDirection(TransitionDirection direction);
		
		float getGlobalOpeningPopupTransitionDuration();
		
		void setGlobalOpeningPopupTransitionDuration(float duration);
		
		float getGlobalClosingPopupTransitionDuration();
		
		void setGlobalClosingPopupTransitionDuration(float duration);
		
		void setGlobalPopupTransitionDuration(float duration);
		
		AnimationCurve getGlobalOpeningPopupTransitionCurve();
		
		void setGlobalOpeningPopupTransitionCurve(AnimationCurve curve);
		
		AnimationCurve getGlobalClosingPopupTransitionCurve();
		
		void setGlobalClosingPopupTransitionCurve(AnimationCurve curve);
		
		void setGlobalPopupTransitionCurve(AnimationCurve curve);
		
		Color getGlobalPopupBackgroundColor();
		
		void setGlobalPopupBackgroundColor(const Color& color);

	public:
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Open)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Close)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Resume)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Pause)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, PageAction, ViewPager* pager, UIPageAction action)
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, EndPageAnimation, ViewPager* pager, UIPageAction action)
		// For mobile platforms
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, BackPressed, UIEvent* ev)
		// Pressed mobile back button or Closed popup window
		SLIB_DECLARE_EVENT_HANDLER(ViewPage, Back, UIEvent* ev)
		
	public:
		void dispatchOK(UIEvent* ev) override;

		void dispatchCancel(UIEvent* ev) override;
		
	protected:
		void _openPopup(const Ref<View>& parent, Transition transition, sl_bool flagFillParentBackground);
		
		void _closePopup(Transition transition);
		
		void _finishPopupAnimation(UIPageAction action);
				
		void _applyDefaultOpeningPopupTransition(Transition& transition);
		
		void _applyDefaultClosingPopupTransition(Transition& transition);
		
		void _onClosePopupWindow(Window* window, UIEvent* ev);

	protected:
		AtomicWeakRef<ViewPager> m_pager;
		
		enum class PopupState
		{
			None, Popup, ClosingPopup, ShowWindow
		};
		PopupState m_popupState;
		
		Color m_popupBackgroundColor;

		sl_reg m_countActiveTransitionAnimations;
		
	};

}

#endif
