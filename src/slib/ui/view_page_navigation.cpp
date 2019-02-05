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
		
		m_pushTransitionType = TransitionType::Slide;
		m_pushTransitionDirection = TransitionDirection::FromRightToLeft;
		m_pushTransitionCurve = AnimationCurve::EaseInOut;
		m_pushTransitionDuration = 0.2f;
		m_popTransitionType = TransitionType::Slide;
		m_popTransitionDirection = TransitionDirection::FromLeftToRight;
		m_popTransitionCurve = AnimationCurve::EaseInOut;
		m_popTransitionDuration = 0.2f;
		
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

	void ViewPageNavigationController::_push(const Ref<View>& viewIn, sl_size countRemoveTop, const Transition& _transition)
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

		Transition transition(_transition);
		
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
		pushPageAfterPopPages(page, 0, Transition());
	}

	void ViewPageNavigationController::pushPageAfterPopPages(const Ref<View>& page, sl_size countPop, const Transition& transition)
	{
		if (isDrawingThread()) {
			_push(page, countPop, transition);
		} else {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPageNavigationController, _push, this, page, countPop, transition));
		}
	}
	
	void ViewPageNavigationController::pushPageAfterPopPages(const Ref<View>& page, sl_size countPop)
	{
		pushPageAfterPopPages(page, countPop, Transition());
	}
	
	void ViewPageNavigationController::pushPageAfterPopAllPages(const Ref<View>& page, const Transition& transition)
	{
		pushPageAfterPopPages(page, SLIB_SIZE_MAX, transition);
	}
	
	void ViewPageNavigationController::pushPageAfterPopAllPages(const Ref<View>& page)
	{
		pushPageAfterPopPages(page, SLIB_SIZE_MAX, Transition());
	}
	
	void ViewPageNavigationController::_pop(const Ref<View>& _viewOut, const Transition& _transition)
	{

		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPageNavigationController, _pop, this, _viewOut, _transition), 100);
			return;
		}

		Transition transition = _transition;
		
		sl_size n = m_pages.getCount();
		
		if (n <= 1) {
			return;
		}
		
		Ref<View> viewOut = *(m_pages.getPointerAt(n - 1));
		if (_viewOut.isNotNull() && _viewOut != viewOut) {
			return;
		}

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
			_pop(viewOut, transition);
		} else {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPageNavigationController, _pop, this, viewOut, transition));
		}
	}

	void ViewPageNavigationController::pop(const Ref<View>& viewOut)
	{
		pop(viewOut, Transition());
	}

	void ViewPageNavigationController::pop(const Transition& transition)
	{
		pop(Ref<View>::null(), transition);
	}

	void ViewPageNavigationController::pop()
	{
		pop(Ref<View>::null(), Transition());
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

	TransitionType ViewPageNavigationController::getPushTransitionType()
	{
		return m_pushTransitionType;
	}

	void ViewPageNavigationController::setPushTransitionType(TransitionType type)
	{
		m_pushTransitionType = type;
	}

	TransitionType ViewPageNavigationController::getPopTransitionType()
	{
		return m_popTransitionType;
	}

	void ViewPageNavigationController::setPopTransitionType(TransitionType type)
	{
		m_popTransitionType = type;
	}

	void ViewPageNavigationController::setTransitionType(TransitionType type)
	{
		m_pushTransitionType = type;
		m_popTransitionType = type;
	}

	TransitionDirection ViewPageNavigationController::getPushTransitionDirection()
	{
		return m_pushTransitionDirection;
	}

	void ViewPageNavigationController::setPushTransitionDirection(TransitionDirection direction)
	{
		m_pushTransitionDirection = direction;
	}

	TransitionDirection ViewPageNavigationController::getPopTransitionDirection()
	{
		return m_popTransitionDirection;
	}

	void ViewPageNavigationController::setPopTransitionDirection(TransitionDirection direction)
	{
		m_popTransitionDirection = direction;
	}

	void ViewPageNavigationController::setTransitionDirection(TransitionDirection direction)
	{
		m_pushTransitionDirection = direction;
		m_popTransitionDirection = direction;
	}

	float ViewPageNavigationController::getPushTransitionDuration()
	{
		return m_pushTransitionDuration;
	}

	void ViewPageNavigationController::setPushTransitionDuration(float duration)
	{
		m_pushTransitionDuration = duration;
	}

	float ViewPageNavigationController::getPopTransitionDuration()
	{
		return m_popTransitionDuration;
	}

	void ViewPageNavigationController::setPopTransitionDuration(float duration)
	{
		m_popTransitionDuration = duration;
	}

	void ViewPageNavigationController::setTransitionDuration(float duration)
	{
		m_pushTransitionDuration = duration;
		m_popTransitionDuration = duration;
	}

	AnimationCurve ViewPageNavigationController::getPushTransitionCurve()
	{
		return m_pushTransitionCurve;
	}

	void ViewPageNavigationController::setPushTransitionCurve(AnimationCurve curve)
	{
		m_pushTransitionCurve = curve;
	}

	AnimationCurve ViewPageNavigationController::getPopTransitionCurve()
	{
		return m_popTransitionCurve;
	}

	void ViewPageNavigationController::setPopTransitionCurve(AnimationCurve curve)
	{
		m_popTransitionCurve = curve;
	}

	void ViewPageNavigationController::setTransitionCurve(AnimationCurve curve)
	{
		m_pushTransitionCurve = curve;
		m_popTransitionCurve = curve;
	}

	void ViewPageNavigationController::_applyDefaultPushTransition(Transition& transition)
	{
		if (transition.type == TransitionType::Default) {
			transition.type = m_pushTransitionType;
		}
		if (transition.direction == TransitionDirection::Default) {
			transition.direction = m_pushTransitionDirection;
		}
		if (transition.duration < SLIB_EPSILON) {
			transition.duration = m_pushTransitionDuration;
		}
		if (transition.curve == AnimationCurve::Default) {
			transition.curve = m_pushTransitionCurve;
		}
	}

	void ViewPageNavigationController::_applyDefaultPopTransition(Transition& transition)
	{
		if (transition.type == TransitionType::Default) {
			transition.type = m_popTransitionType;
		}
		if (transition.direction == TransitionDirection::Default) {
			transition.direction = m_popTransitionDirection;
		}
		if (transition.duration < SLIB_EPSILON) {
			transition.duration = m_popTransitionDuration;
		}
		if (transition.curve == AnimationCurve::Default) {
			transition.curve = m_popTransitionCurve;
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
