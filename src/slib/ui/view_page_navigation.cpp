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

#include "slib/ui/view_page_navigation.h"

#include "slib/ui/view_page.h"
#include "slib/ui/render_view.h"

#include "slib/core/scoped.h"

#if defined(SLIB_UI_IS_ANDROID)
#	include "slib/core/platform_android.h"
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(ViewPageNavigationController, ViewGroup)

	ViewPageNavigationController::ViewPageNavigationController()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		m_flagSwipeNavigation = sl_false;
		
		m_pushTransition.type = TransitionType::Slide;
		m_pushTransition.direction = TransitionDirection::FromRightToLeft;
		m_pushTransition.curve = AnimationCurve::EaseInOut;
		m_pushTransition.duration = 0.2f;
		m_popTransition.type = TransitionType::Slide;
		m_popTransition.direction = TransitionDirection::FromLeftToRight;
		m_popTransition.curve = AnimationCurve::EaseInOut;
		m_popTransition.duration = 0.2f;
		
		m_countActiveTransitionAnimations = 0;
	}

	ViewPageNavigationController::~ViewPageNavigationController()
	{
		ListElements< Ref<View> > pages(m_pages);
		for (sl_size i = 0; i < pages.count; i++) {
			if (i == 0) {
				dispatchPageAction(pages[pages.count - 1 - i].get(), UIPageAction::Pause);
			}
			dispatchPageAction(pages[pages.count - 1 - i].get(), UIPageAction::Pop);
		}
	}

	sl_size ViewPageNavigationController::getPagesCount()
	{
		return m_pages.getCount();
	}
	
	List< Ref<View> > ViewPageNavigationController::getPages()
	{
		return m_pages.duplicate();
	}

	Ref<View> ViewPageNavigationController::getCurrentPage()
	{
		return m_pages.getValueAt(m_pages.getCount() - 1);
	}

	void ViewPageNavigationController::_onFinishAnimation(const Ref<View>& view, UIPageAction action)
	{
		dispatchEndPageAnimation(view.get(), action);
		switch (action) {
			case UIPageAction::Pause:
			case UIPageAction::Pop:
				if (getCurrentPage() != view) {
					view->setVisibility(Visibility::Hidden);
					removeChild(view);
				}
				break;
			default:
				view->setEnabled(sl_true);
				break;
		}
		setEnabled(sl_true);
		Base::interlockedDecrement(&m_countActiveTransitionAnimations);
	}

	void ViewPageNavigationController::_resetAnimationStatus(const Ref<View>& view)
	{
		view->setTranslation(0, 0, UIUpdateMode::Init);
		view->setScale(1, 1, UIUpdateMode::Init);
		view->setRotation(0, UIUpdateMode::None);
		view->setAlpha(1, UIUpdateMode::Redraw);
	}

	void ViewPageNavigationController::_push(const Ref<View>& viewIn, sl_size countRemoveTop, const Transition* _transition)
	{

		if (viewIn.isNull()) {
			return;
		}

		viewIn->setParent(this);
		
		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPageNavigationController, _push, this, viewIn, countRemoveTop, _transition), 100);
			return;
		}

		if (!_transition) {
			if (ViewPage* _page = CastInstance<ViewPage>(viewIn.get())) {
				_transition = &(_page->m_openingTransition);
			} else {
				static Transition defaultTransition;
				_transition = &defaultTransition;
			}
		}
		Transition transition(*_transition);
		
		sl_size n = m_pages.getCount();
		
		if (n == 0) {

#if defined(SLIB_UI_IS_ANDROID)
			Android::dismissKeyboard();
#endif

			ScopedCounter counter(&m_countActiveTransitionAnimations);
			m_pages.add_NoLock(viewIn);
			viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::None);
			_resetAnimationStatus(viewIn);
			addChild(viewIn);
			dispatchPageAction(viewIn.get(), UIPageAction::Push);
			dispatchPageAction(viewIn.get(), UIPageAction::Resume);
			dispatchEndPageAnimation(viewIn.get(), UIPageAction::Push);
			viewIn->setVisibility(Visibility::Visible, UIUpdateMode::None);
			viewIn->bringToFront();
			return;
		}
		
		Ref<View>* pages = m_pages.getData();
		Ref<View> viewBack = pages[n - 1];
		if (viewBack == viewIn) {
			return;
		}

#if defined(SLIB_UI_IS_ANDROID)
		Android::dismissKeyboard();
#endif

		viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::None);
		
		setEnabled(sl_false);
		viewBack->setEnabled(sl_false, UIUpdateMode::None);
		viewIn->setEnabled(sl_false, UIUpdateMode::None);
		
		_applyDefaultPushTransition(transition);
		Ref<Animation> animationPause = Transition::createAnimation(viewBack, transition, UIPageAction::Pause, SLIB_BIND_REF(void(), ViewPageNavigationController, _onFinishAnimation,this, viewBack, UIPageAction::Pause));
		
		Ref<Animation> animationPush = Transition::createAnimation(viewIn, transition, UIPageAction::Push, SLIB_BIND_REF(void(), ViewPageNavigationController, _onFinishAnimation,this, viewIn, UIPageAction::Push));
		
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		
		_resetAnimationStatus(viewIn);
		if (animationPush.isNotNull()) {
			animationPush->dispatchStartFrame();
		}

		addChild(viewIn);
		
		dispatchPageAction(viewBack.get(), UIPageAction::Pause);
		
		if (countRemoveTop > 0) {
			sl_size n = m_pages.getCount();
			if (countRemoveTop > n) {
				countRemoveTop = n;
			}
			for (sl_size i = 0; i < countRemoveTop; i++) {
				dispatchPageAction(pages[n - 1 - i].get(), UIPageAction::Pop);
			}
			m_pages.setCount_NoLock(n - countRemoveTop);
		}
		
		m_pages.add_NoLock(viewIn);
		dispatchPageAction(viewIn.get(), UIPageAction::Push);
		dispatchPageAction(viewIn.get(), UIPageAction::Resume);
		
		if (animationPause.isNotNull()) {
			animationPause->dispatchStartFrame();
		}

		viewIn->bringToFront(UIUpdateMode::None);
		viewIn->setVisibility(Visibility::Visible);
		
		if (animationPause.isNull()) {
			_onFinishAnimation(viewBack, UIPageAction::Pause);
		}
		if (animationPush.isNull()) {
			_onFinishAnimation(viewIn, UIPageAction::Push);
		}
		
		_runAnimationProc(viewIn, [animationPause, animationPush]() {
			if (animationPause.isNotNull()) {
				animationPause->start();
			}
			if (animationPush.isNotNull()) {
				animationPush->start();
			}
		});
		
	}

	void ViewPageNavigationController::push(const Ref<View>& page, const Transition& transition)
	{
		pushPageAfterPopPages(page, 0, transition);
	}
	
	void ViewPageNavigationController::push(const Ref<View>& page)
	{
		pushPageAfterPopPages(page, 0);
	}

	void ViewPageNavigationController::pushPageAfterPopPages(const Ref<View>& page, sl_size countPop, const Transition& transition)
	{
		if (isDrawingThread()) {
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				ObjectLocker lock(_page);
				if (_page->m_popupState != ViewPage::PopupState::None) {
					return;
				}
				_push(page, countPop, &transition);
			} else {
				_push(page, countPop, &transition);
			}
		} else {
			void (ViewPageNavigationController::*func)(const Ref<View>& page, sl_size countPop, const Transition& transition) = &ViewPageNavigationController::pushPageAfterPopPages;
			dispatchToDrawingThread(Function<void()>::bindWeakRef(ToWeakRef(this), func, page, countPop, transition));
		}
	}
	
	void ViewPageNavigationController::pushPageAfterPopPages(const Ref<View>& page, sl_size countPop)
	{
		if (isDrawingThread()) {
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				ObjectLocker lock(_page);
				if (_page->m_popupState != ViewPage::PopupState::None) {
					return;
				}
				_push(page, countPop, sl_null);
			} else {
				_push(page, countPop, sl_null);
			}
		} else {
			void (ViewPageNavigationController::*func)(const Ref<View>& page, sl_size countPop) = &ViewPageNavigationController::pushPageAfterPopPages;
			dispatchToDrawingThread(Function<void()>::bindWeakRef(ToWeakRef(this), func, page, countPop));
		}
	}
	
	void ViewPageNavigationController::pushPageAfterPopAllPages(const Ref<View>& page, const Transition& transition)
	{
		pushPageAfterPopPages(page, SLIB_SIZE_MAX, transition);
	}
	
	void ViewPageNavigationController::pushPageAfterPopAllPages(const Ref<View>& page)
	{
		pushPageAfterPopPages(page, SLIB_SIZE_MAX);
	}
	
	void ViewPageNavigationController::_pop(const Ref<View>& _viewOut, const Transition* _transition)
	{

		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPageNavigationController, _pop, this, _viewOut, _transition), 100);
			return;
		}
		
		sl_size n = m_pages.getCount();
		
		if (n <= 1) {
			return;
		}
		
		Ref<View> viewOut = *(m_pages.getPointerAt(n - 1));
		if (_viewOut.isNotNull() && _viewOut != viewOut) {
			return;
		}

		if (!_transition) {
			if (ViewPage* _page = CastInstance<ViewPage>(viewOut.get())) {
				_transition = &(_page->m_closingTransition);
			} else {
				static Transition defaultTransition;
				_transition = &defaultTransition;
			}
		}
		Transition transition(*_transition);
		
#if defined(SLIB_UI_IS_ANDROID)
		Android::dismissKeyboard();
#endif

		Ref<View> viewBack = *(m_pages.getPointerAt(n - 2));

		setEnabled(sl_false);
		viewBack->setEnabled(sl_false, UIUpdateMode::None);
		viewOut->setEnabled(sl_false, UIUpdateMode::None);
		
		_applyDefaultPopTransition(transition);
		Ref<Animation> animationPop = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, SLIB_BIND_REF(void(), ViewPageNavigationController, _onFinishAnimation,this, viewOut, UIPageAction::Pop));
		
		Ref<Animation> animationResume = Transition::createAnimation(viewBack, transition, UIPageAction::Resume, SLIB_BIND_REF(void(), ViewPageNavigationController, _onFinishAnimation,this, viewBack, UIPageAction::Resume));
		
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		
		_resetAnimationStatus(viewBack);
		if (animationResume.isNotNull()) {
			animationResume->dispatchStartFrame();
		}

		addChild(viewBack, UIUpdateMode::None);

		dispatchPageAction(viewOut.get(), UIPageAction::Pause);
		dispatchPageAction(viewOut.get(), UIPageAction::Pop);
		dispatchPageAction(viewBack.get(), UIPageAction::Resume);
		
		if (animationPop.isNotNull()) {
			animationPop->start();
		}
		if (animationResume.isNotNull()) {
			animationResume->start();
		}
		
		viewBack->bringToFront(UIUpdateMode::None);
		viewOut->bringToFront(UIUpdateMode::None);
		
		viewBack->setVisibility(Visibility::Visible);
		
		m_pages.popBack_NoLock();
		
		if (animationPop.isNull()) {
			_onFinishAnimation(viewOut, UIPageAction::Pop);
		}
		if (animationResume.isNull()) {
			_onFinishAnimation(viewBack, UIPageAction::Resume);
		}
		
	}

	void ViewPageNavigationController::pop(const Ref<View>& viewOut, const Transition& transition)
	{
		if (isDrawingThread()) {
			_pop(viewOut, &transition);
		} else {
			void (ViewPageNavigationController::*func)(const Ref<View>& viewOut, const Transition& transition) = &ViewPageNavigationController::pop;
			dispatchToDrawingThread(Function<void()>::bindWeakRef(ToWeakRef(this), func, viewOut, transition));
		}
	}

	void ViewPageNavigationController::pop(const Ref<View>& viewOut)
	{
		if (isDrawingThread()) {
			_pop(viewOut, sl_null);
		} else {
			void (ViewPageNavigationController::*func)(const Ref<View>& viewOut) = &ViewPageNavigationController::pop;
			dispatchToDrawingThread(Function<void()>::bindWeakRef(ToWeakRef(this), func, viewOut));
		}
	}

	void ViewPageNavigationController::pop(const Transition& transition)
	{
		pop(Ref<View>::null(), transition);
	}

	void ViewPageNavigationController::pop()
	{
		pop(Ref<View>::null());
	}

	void ViewPageNavigationController::setSwipeNavigation(sl_bool flag)
	{
		m_flagSwipeNavigation = flag;
		if (flag) {
			Ref<GestureDetector> gesture = createGestureDetector();
			if (gesture.isNotNull()) {
				gesture->enable(GestureType::SwipeLeft);
				gesture->enable(GestureType::SwipeRight);
			}
		}
	}

	void ViewPageNavigationController::setTransition(const Transition& opening, const Transition& closing)
	{
		if (opening.type != TransitionType::Default) {
			m_pushTransition.type = opening.type;
		}
		if (opening.direction != TransitionDirection::Default) {
			m_pushTransition.direction = opening.direction;
		}
		if (opening.duration > 0) {
			m_pushTransition.duration = opening.duration;
		}
		if (opening.curve != AnimationCurve::Default) {
			m_pushTransition.curve = opening.curve;
		}
		if (closing.type != TransitionType::Default) {
			m_popTransition.type = closing.type;
		}
		if (closing.direction != TransitionDirection::Default) {
			m_popTransition.direction = closing.direction;
		}
		if (closing.duration > 0) {
			m_popTransition.duration = closing.duration;
		}
		if (closing.curve != AnimationCurve::Default) {
			m_popTransition.curve = closing.curve;
		}
	}
	
	void ViewPageNavigationController::_applyDefaultPushTransition(Transition& transition)
	{
		if (transition.type == TransitionType::Default) {
			transition.type = m_pushTransition.type;
		}
		if (transition.direction == TransitionDirection::Default) {
			transition.direction = m_pushTransition.direction;
		}
		if (transition.duration <= 0) {
			transition.duration = m_pushTransition.duration;
		}
		if (transition.curve == AnimationCurve::Default) {
			transition.curve = m_pushTransition.curve;
		}
	}

	void ViewPageNavigationController::_applyDefaultPopTransition(Transition& transition)
	{
		if (transition.type == TransitionType::Default) {
			transition.type = m_popTransition.type;
		}
		if (transition.direction == TransitionDirection::Default) {
			transition.direction = m_popTransition.direction;
		}
		if (transition.duration <= 0) {
			transition.duration = m_popTransition.duration;
		}
		if (transition.curve == AnimationCurve::Default) {
			transition.curve = m_popTransition.curve;
		}
	}
	
	void ViewPageNavigationController::_runAnimationProc(const Ref<View>& view, const Function<void()>& callback)
	{
		sl_bool flagRender = sl_false;
		Ref<View> t = view;
		while (1) {
			if (IsInstanceOf<RenderView>(t)) {
				flagRender = sl_true;
				break;
			}
			t = t->getParent();
			if (t.isNull()) {
				break;
			}
		}
		if (flagRender) {
			view->runAfterDraw(callback);
		} else {
			view->dispatchToDrawingThread(callback, 10);
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ViewPageNavigationController, PageAction, View* page, UIPageAction action)

	void ViewPageNavigationController::dispatchPageAction(View* page, UIPageAction action)
	{
		if (page) {
			SLIB_INVOKE_EVENT_HANDLER(PageAction, page, action)
			if (ViewPage* _page = CastInstance<ViewPage>(page)) {
				_page->dispatchPageAction(this, action);
			}
		}
	}

	SLIB_DEFINE_EVENT_HANDLER(ViewPageNavigationController, EndPageAnimation, View* page, UIPageAction action)
	
	void ViewPageNavigationController::dispatchEndPageAnimation(View* page, UIPageAction action)
	{
		if (page) {
			SLIB_INVOKE_EVENT_HANDLER(EndPageAnimation, page, action)
			if (ViewPage* _page = CastInstance<ViewPage>(page)) {
				_page->dispatchEndPageAnimation(this, action);
			}
		}
	}

	void ViewPageNavigationController::onResize(sl_ui_len width, sl_ui_len height)
	{
		UIRect rect(getPaddingLeft(), getPaddingTop(), width - getPaddingRight(), height - getPaddingBottom());
		ObjectLocker lock(this);
		ListElements< Ref<View> > pages(m_pages);
		for (sl_size i = 0; i < pages.count; i++) {
			pages[i]->setFrame(rect);
		}
	}

	void ViewPageNavigationController::onChangePadding()
	{
		Rectangle rect(getBoundsInnerPadding());
		ObjectLocker lock(this);
		ListElements< Ref<View> > pages(m_pages);
		for (sl_size i = 0; i < pages.count; i++) {
			pages[i]->setFrame(rect);
		}
	}

	void ViewPageNavigationController::onSwipe(GestureEvent* ev)
	{
		if (m_flagSwipeNavigation) {
			GestureType type = ev->type;
			if (type == GestureType::SwipeRight) {
				pop();
			}
		}
	}

}
