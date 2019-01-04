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

#include "slib/ui/view_page.h"

#include "slib/ui/core.h"
#include "slib/ui/mobile_app.h"
#include "slib/ui/render_view.h"

#include "slib/core/scoped.h"

#if defined(SLIB_UI_IS_ANDROID)
#	include "slib/core/platform_android.h"
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(ViewPager, View)

	ViewPager::ViewPager()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setCreatingChildInstances(sl_true);
		
		m_indexCurrent = 0;
		m_minimumPagesCount = 0;
		m_flagSwipeNavigation = sl_false;
		
		m_pushTransitionType = TransitionType::Push;
		m_pushTransitionDirection = TransitionDirection::FromRightToLeft;
		m_pushTransitionCurve = AnimationCurve::EaseInOut;
		m_pushTransitionDuration = 0.5f;
		m_popTransitionType = TransitionType::Push;
		m_popTransitionDirection = TransitionDirection::FromLeftToRight;
		m_popTransitionCurve = AnimationCurve::EaseInOut;
		m_popTransitionDuration = 0.5f;
		
		m_countActiveTransitionAnimations = 0;
	}

	ViewPager::~ViewPager()
	{
		ListElements< Ref<View> > pages(m_pages);
		sl_size indexCurrent = m_indexCurrent;
		if (indexCurrent < pages.count) {
			dispatchPageAction(pages[indexCurrent].get(), UIPageAction::Pause);
		}
		for (sl_size i = 0; i < pages.count; i++) {
			dispatchPageAction(pages[i].get(), UIPageAction::Pop);
		}
	}

	sl_size ViewPager::getPagesCount()
	{
		return m_pages.getCount();
	}

	sl_size ViewPager::getCurrentPageIndex()
	{
		return m_indexCurrent;
	}

	Ref<View> ViewPager::getCurrentPage()
	{
		ObjectLocker lock(this);
		sl_size index = m_indexCurrent;
		if (index < m_pages.getCount()) {
			return m_pages.getData()[index];
		} else {
			return sl_null;
		}
	}

	sl_size ViewPager::getMinimumPagesCount()
	{
		return m_minimumPagesCount;
	}

	void ViewPager::setMinimumPagesCount(sl_size count)
	{
		m_minimumPagesCount = count;
	}

	void ViewPager::addPage(const Ref<View>& page, UIUpdateMode mode)
	{
		if (page.isNull()) {
			return;
		}
		page->setParent(this);
		ObjectLocker lock(this);
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode) || m_pages.getCount() > 0) {
			if (m_pages.addIfNotExist_NoLock(page)) {
				lock.unlock();
				page->setFrame(getBoundsInnerPadding(), mode);
				dispatchPageAction(page.get(), UIPageAction::Push);
			}
		} else {
			m_pages.add_NoLock(page);
			lock.unlock();
			page->setFrame(getBoundsInnerPadding());
			addChild(page, mode);
			dispatchPageAction(page.get(), UIPageAction::Push);
			dispatchPageAction(page.get(), UIPageAction::Resume);
			m_indexCurrent = 0;
		}
	}

	void ViewPager::removePageAt(sl_size index, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		sl_size n = m_pages.getCount();
		if (n <= m_minimumPagesCount) {
			return;
		}
		if (index < n) {
			Ref<View> oldPage = m_pages.getData()[index];
			if (index == m_indexCurrent) {
				Ref<View> newPage;
				m_pages.removeAt_NoLock(index);
				n--;
				if (n == 0) {
					m_indexCurrent = 0;
				} else {
					if (index == n) {
						index--;
						m_indexCurrent = index;
					}
					newPage = m_pages.getData()[index];
				}
				lock.unlock();
				if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
					if (oldPage.isNotNull()) {
						dispatchPageAction(oldPage.get(), UIPageAction::Pop);
					}
				} else {
					if (oldPage.isNotNull()) {
						dispatchPageAction(oldPage.get(), UIPageAction::Pause);
						dispatchPageAction(oldPage.get(), UIPageAction::Pop);
						removeChild(oldPage, mode);
					}
					if (newPage.isNotNull()) {
						newPage->setFrame(getBoundsInnerPadding());
						addChild(newPage, mode);
						dispatchPageAction(newPage.get(), UIPageAction::Resume);
					}
				}
			} else {
				m_pages.removeAt_NoLock(index);
				lock.unlock();
				dispatchPageAction(oldPage.get(), UIPageAction::Pop);
			}
		}
	}

	void ViewPager::selectPage(sl_size index, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (index >= m_pages.getCount()) {
			return;
		}
		Ref<View>* pages = m_pages.getData();
		Ref<View> viewIn = pages[index];
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			m_indexCurrent = index;
			addChild(viewIn, mode);
			dispatchPageAction(viewIn.get(), UIPageAction::Resume);
		} else {
			if (m_indexCurrent == index) {
				return;
			}
			Ref<View> viewOut = pages[m_indexCurrent];
			m_indexCurrent = index;
			if (viewOut == viewIn) {
				return;
			}
			addChild(viewIn, UIUpdateMode::None);
			dispatchPageAction(viewOut.get(), UIPageAction::Pause);
			dispatchPageAction(viewIn.get(), UIPageAction::Resume);
			removeChild(viewOut, mode);
		}
	}

	void ViewPager::_onFinishAnimation(const Ref<View>& view, UIPageAction action)
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

	void ViewPager::_resetAnimationStatus(const Ref<View>& view)
	{
		view->setTranslation(0, 0, UIUpdateMode::Init);
		view->setScale(1, 1, UIUpdateMode::Init);
		view->setRotation(0, UIUpdateMode::None);
		view->setAlpha(1, UIUpdateMode::Redraw);
	}

	void ViewPager::_goTo(sl_size index, const Transition& _transition)
	{

		Transition transition = _transition;

		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPager, _goTo, this, index, transition), 100);
			return;
		}

		if (m_indexCurrent == index) {
			return;
		}
		if (index >= m_pages.getCount()) {
			return;
		}
		sl_bool flagNext;
		if (index > m_indexCurrent) {
			flagNext = sl_true;
		} else {
			flagNext = sl_false;
		}
		
		Ref<View>* pages = m_pages.getData();
		Ref<View> viewOut = pages[m_indexCurrent];
		Ref<View> viewIn = pages[index];
		m_indexCurrent = index;
		if (viewOut == viewIn) {
			return;
		}

#if defined(SLIB_UI_IS_ANDROID)
		Android::dismissKeyboard();
#endif

		setEnabled(sl_false);
		viewOut->setEnabled(sl_false, UIUpdateMode::None);
		viewIn->setEnabled(sl_false, UIUpdateMode::None);
		viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::None);
		
		Ref<Animation> animationPause, animationResume;
		if (flagNext) {
			_applyDefaultPushTransition(transition);
			animationPause = Transition::createAnimation(viewOut, transition, UIPageAction::Pause, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation, this, viewOut, UIPageAction::Pause));
			animationResume = Transition::createAnimation(viewIn, transition, UIPageAction::Push, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation, this, viewIn, UIPageAction::Resume));
		} else {
			_applyDefaultPopTransition(transition);
			animationPause = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation, this, viewOut, UIPageAction::Pause));
			animationResume = Transition::createAnimation(viewIn, transition, UIPageAction::Resume, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation, this, viewIn, UIPageAction::Resume));
		}
		
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		
		_resetAnimationStatus(viewIn);
		if (animationResume.isNotNull()) {
			animationResume->dispatchStartFrame();
		}
		
		addChild(viewIn, UIUpdateMode::None);
		
		dispatchPageAction(viewOut.get(), UIPageAction::Pause);
		
		dispatchPageAction(viewIn.get(), UIPageAction::Resume);
		
		if (animationPause.isNotNull()) {
			animationPause->dispatchStartFrame();
		}
		
		if (flagNext) {
			viewIn->bringToFront(UIUpdateMode::None);
		} else {
			viewIn->bringToFront(UIUpdateMode::None);
			viewOut->bringToFront(UIUpdateMode::None);
		}
		viewIn->setVisibility(Visibility::Visible);
		
		if (animationPause.isNull()) {
			_onFinishAnimation(viewOut, UIPageAction::Pause);
		}
		if (animationResume.isNull()) {
			_onFinishAnimation(viewIn, UIPageAction::Resume);
		}
		
		_runAnimationProc(viewIn, [animationPause, animationResume]() {
			if (animationPause.isNotNull()) {
				animationPause->start();
			}
			if (animationResume.isNotNull()) {
				animationResume->start();
			}
		});
		
	}

	void ViewPager::goToPageAt(sl_size index, const Transition& transition)
	{
		if (isDrawingThread()) {
			_goTo(index, transition);
		} else {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPager, _goTo, this, index, transition));
		}
	}

	void ViewPager::goToPageAt(sl_size index)
	{
		goToPageAt(index, Transition());
	}

	void ViewPager::goPrev(const Transition& transition)
	{
		sl_size index = m_indexCurrent;
		if (index > 0) {
			goToPageAt(index - 1, transition);
		}
	}

	void ViewPager::goPrev()
	{
		sl_size index = m_indexCurrent;
		if (index > 0) {
			goToPageAt(index - 1);
		}
	}

	void ViewPager::goNext(const Transition& transition)
	{
		sl_size index = m_indexCurrent;
		if (index + 1 < m_pages.getCount()) {
			goToPageAt(index + 1, transition);
		}
	}

	void ViewPager::goNext()
	{
		sl_size index = m_indexCurrent;
		if (index + 1 < m_pages.getCount()) {
			goToPageAt(index + 1);
		}
	}

	void ViewPager::_push(const Ref<View>& viewIn, const Transition& _transition, sl_bool flagRemoveAllBackPages)
	{

		if (viewIn.isNull()) {
			return;
		}

		viewIn->setParent(this);
		
		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPager, _push, this, viewIn, _transition, flagRemoveAllBackPages), 100);
			return;
		}

		Transition transition(_transition);
		
		sl_size n = m_pages.getCount();
		
		if (n == 0) {

#if defined(SLIB_UI_IS_ANDROID)
			Android::dismissKeyboard();
#endif

			ScopedCounter counter(&m_countActiveTransitionAnimations);
			m_indexCurrent = 0;
			m_pages.add_NoLock(viewIn);
			viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::None);
			_resetAnimationStatus(viewIn);
			addChild(viewIn, UIUpdateMode::None);
			dispatchPageAction(viewIn.get(), UIPageAction::Push);
			dispatchPageAction(viewIn.get(), UIPageAction::Resume);
			dispatchEndPageAnimation(viewIn.get(), UIPageAction::Push);
			viewIn->setVisibility(Visibility::Visible, UIUpdateMode::None);
			viewIn->bringToFront();
			return;
		}
		
		Ref<View>* pages = m_pages.getData();
		Ref<View> viewBack = pages[m_indexCurrent];
		if (viewBack == viewIn) {
			return;
		}

#if defined(SLIB_UI_IS_ANDROID)
		Android::dismissKeyboard();
#endif

		m_indexCurrent = n;

		viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::None);
		
		setEnabled(sl_false);
		viewBack->setEnabled(sl_false, UIUpdateMode::None);
		viewIn->setEnabled(sl_false, UIUpdateMode::None);
		
		_applyDefaultPushTransition(transition);
		Ref<Animation> animationPause = Transition::createAnimation(viewBack, transition, UIPageAction::Pause, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation,this, viewBack, UIPageAction::Pause));
		
		Ref<Animation> animationPush = Transition::createAnimation(viewIn, transition, UIPageAction::Push, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation,this, viewIn, UIPageAction::Push));
		
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		
		_resetAnimationStatus(viewIn);
		if (animationPush.isNotNull()) {
			animationPush->dispatchStartFrame();
		}

		addChild(viewIn, UIUpdateMode::None);
		
		dispatchPageAction(viewBack.get(), UIPageAction::Pause);
		
		if (flagRemoveAllBackPages) {
			for (sl_size i = 0; i < n; i++) {
				dispatchPageAction(pages[i].get(), UIPageAction::Pop);
			}
			m_pages.removeAll_NoLock();
			m_indexCurrent = 0;
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

	void ViewPager::push(const Ref<View>& viewIn, const Transition& transition, sl_bool flagRemoveAllBackPages)
	{
		if (isDrawingThread()) {
			_push(viewIn, transition, flagRemoveAllBackPages);
		} else {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPager, _push, this, viewIn, transition, flagRemoveAllBackPages));
		}
	}

	void ViewPager::push(const Ref<View>& viewIn, sl_bool flagRemoveAllBackPages)
	{
		push(viewIn, Transition(), flagRemoveAllBackPages);
	}

	void ViewPager::_pop(const Ref<View>& _viewOut, const Transition& _transition)
	{

		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPager, _pop, this, _viewOut, _transition), 100);
			return;
		}

		Transition transition = _transition;
		
		sl_size n = m_pages.getCount();
		
		if (n <= m_minimumPagesCount) {
			return;
		}
		
		sl_size indexPop = m_indexCurrent;
		Ref<View> viewOut = *(m_pages.getPointerAt(indexPop));
		if (_viewOut.isNotNull() && _viewOut != viewOut) {
			return;
		}

#if defined(SLIB_UI_IS_ANDROID)
		Android::dismissKeyboard();
#endif

		if (n == 1) {
			ScopedCounter counter(&m_countActiveTransitionAnimations);
			dispatchPageAction(viewOut.get(), UIPageAction::Pause);
			dispatchPageAction(viewOut.get(), UIPageAction::Pop);
			dispatchEndPageAnimation(viewOut.get(), UIPageAction::Pop);
			removeChild(viewOut);
			viewOut->setVisibility(Visibility::Hidden, UIUpdateMode::None);
			m_pages.removeAll_NoLock();
			m_indexCurrent = 0;
			return;
		}
		
		Ref<View> viewBack;
		if (indexPop > 0) {
			m_indexCurrent = indexPop - 1;
			viewBack = *(m_pages.getPointerAt(indexPop - 1));
		} else {
			m_indexCurrent = 0;
			viewBack = *(m_pages.getPointerAt(1));
		}
		
		setEnabled(sl_false);
		viewBack->setEnabled(sl_false, UIUpdateMode::None);
		viewOut->setEnabled(sl_false, UIUpdateMode::None);
		
		_applyDefaultPopTransition(transition);
		Ref<Animation> animationPop = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation,this, viewOut, UIPageAction::Pop));
		
		Ref<Animation> animationResume = Transition::createAnimation(viewBack, transition, UIPageAction::Resume, SLIB_BIND_REF(void(), ViewPager, _onFinishAnimation,this, viewBack, UIPageAction::Resume));
		
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
		
		m_pages.removeAt_NoLock(indexPop);
		
		if (animationPop.isNull()) {
			_onFinishAnimation(viewOut, UIPageAction::Pop);
		}
		if (animationResume.isNull()) {
			_onFinishAnimation(viewBack, UIPageAction::Resume);
		}
		
	}

	void ViewPager::pop(const Ref<View>& viewOut, const Transition& transition)
	{
		if (isDrawingThread()) {
			_pop(viewOut, transition);
		} else {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPager, _pop, this, viewOut, transition));
		}
	}

	void ViewPager::pop(const Ref<View>& viewOut)
	{
		pop(viewOut, Transition());
	}

	void ViewPager::pop(const Transition& transition)
	{
		pop(Ref<View>::null(), transition);
	}

	void ViewPager::pop()
	{
		pop(Ref<View>::null(), Transition());
	}

	void ViewPager::setSwipeNavigation(sl_bool flag)
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

	TransitionType ViewPager::getPushTransitionType()
	{
		return m_pushTransitionType;
	}

	void ViewPager::setPushTransitionType(TransitionType type)
	{
		m_pushTransitionType = type;
	}

	TransitionType ViewPager::getPopTransitionType()
	{
		return m_popTransitionType;
	}

	void ViewPager::setPopTransitionType(TransitionType type)
	{
		m_popTransitionType = type;
	}

	void ViewPager::setTransitionType(TransitionType type)
	{
		m_pushTransitionType = type;
		m_popTransitionType = type;
	}

	TransitionDirection ViewPager::getPushTransitionDirection()
	{
		return m_pushTransitionDirection;
	}

	void ViewPager::setPushTransitionDirection(TransitionDirection direction)
	{
		m_pushTransitionDirection = direction;
	}

	TransitionDirection ViewPager::getPopTransitionDirection()
	{
		return m_popTransitionDirection;
	}

	void ViewPager::setPopTransitionDirection(TransitionDirection direction)
	{
		m_popTransitionDirection = direction;
	}

	void ViewPager::setTransitionDirection(TransitionDirection direction)
	{
		m_pushTransitionDirection = direction;
		m_popTransitionDirection = direction;
	}

	float ViewPager::getPushTransitionDuration()
	{
		return m_pushTransitionDuration;
	}

	void ViewPager::setPushTransitionDuration(float duration)
	{
		m_pushTransitionDuration = duration;
	}

	float ViewPager::getPopTransitionDuration()
	{
		return m_popTransitionDuration;
	}

	void ViewPager::setPopTransitionDuration(float duration)
	{
		m_popTransitionDuration = duration;
	}

	void ViewPager::setTransitionDuration(float duration)
	{
		m_pushTransitionDuration = duration;
		m_popTransitionDuration = duration;
	}

	AnimationCurve ViewPager::getPushTransitionCurve()
	{
		return m_pushTransitionCurve;
	}

	void ViewPager::setPushTransitionCurve(AnimationCurve curve)
	{
		m_pushTransitionCurve = curve;
	}

	AnimationCurve ViewPager::getPopTransitionCurve()
	{
		return m_popTransitionCurve;
	}

	void ViewPager::setPopTransitionCurve(AnimationCurve curve)
	{
		m_popTransitionCurve = curve;
	}

	void ViewPager::setTransitionCurve(AnimationCurve curve)
	{
		m_pushTransitionCurve = curve;
		m_popTransitionCurve = curve;
	}

	void ViewPager::_applyDefaultPushTransition(Transition& transition)
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

	void ViewPager::_applyDefaultPopTransition(Transition& transition)
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
	
	void ViewPager::_runAnimationProc(const Ref<View>& view, const Function<void()>& callback)
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
	
	void ViewPager::onPageAction(View* page, UIPageAction action)
	{
	}

	void ViewPager::onEndPageAnimation(View* page, UIPageAction action)
	{
	}

	void ViewPager::dispatchPageAction(View* page, UIPageAction action)
	{
		if (page) {
			onPageAction(page, action);
			if (ViewPage* _page = CastInstance<ViewPage>(page)) {
				_page->dispatchPageAction(this, action);
			}
			getOnPageAction()(this, page, action);
		}
	}

	void ViewPager::dispatchEndPageAnimation(View* page, UIPageAction action)
	{
		if (page) {
			onEndPageAnimation(page, action);
			if (ViewPage* _page = CastInstance<ViewPage>(page)) {
				_page->dispatchEndPageAnimation(this, action);
			}
		}
	}

	void ViewPager::onResize(sl_ui_len width, sl_ui_len height)
	{
		UIRect rect(getPaddingLeft(), getPaddingTop(), width - getPaddingRight(), height - getPaddingBottom());
		ObjectLocker lock(this);
		ListElements< Ref<View> > pages(m_pages);
		for (sl_size i = 0; i < pages.count; i++) {
			pages[i]->setFrame(rect);
		}
	}

	void ViewPager::onChangePadding()
	{
		Rectangle rect(getBoundsInnerPadding());
		ObjectLocker lock(this);
		ListElements< Ref<View> > pages(m_pages);
		for (sl_size i = 0; i < pages.count; i++) {
			pages[i]->setFrame(rect);
		}
	}

	void ViewPager::onSwipe(GestureEvent* ev)
	{
		if (m_flagSwipeNavigation) {
			GestureType type = ev->type;
			if (type == GestureType::SwipeLeft) {
				goNext();
			} else if (type == GestureType::SwipeRight) {
				goPrev();
			}
		}
	}


	SLIB_DEFINE_OBJECT(ViewPage, ViewGroup)

	ViewPage::ViewPage()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		m_popupState = PopupState::None;
		m_popupBackgroundColor = Color::zero();
		
		m_countActiveTransitionAnimations = 0;
		
		setClipping(sl_true, UIUpdateMode::Init);
		
	}

	ViewPage::~ViewPage()
	{
	}

	Ref<ViewPager> ViewPage::getPager()
	{
		return m_pager;
	}

	void ViewPage::setPager(const Ref<ViewPager>& pager)
	{
		m_pager = pager;
	}

	void ViewPage::open(const Ref<ViewPager>& pager, const Transition& transition)
	{
		if (pager.isNotNull()) {
			ObjectLocker lock(this);
			if (m_popupState != PopupState::None) {
				return;
			}
			pager->push(this, transition);
		}
	}

	void ViewPage::open(const Ref<ViewPager>& pager)
	{
		open(pager, Transition());
	}

	void ViewPage::openHome(const Ref<ViewPager>& pager, const Transition& transition)
	{
		if (pager.isNotNull()) {
			ObjectLocker lock(this);
			if (m_popupState != PopupState::None) {
				return;
			}
			pager->push(this, transition, sl_true);
		}
	}

	void ViewPage::openHome(const Ref<ViewPager>& pager)
	{
		openHome(pager, Transition());
	}

	void ViewPage::close(const Transition& transition)
	{
		ObjectLocker lock(this);
		if (m_popupState == PopupState::ShowWindow) {
			m_popupState = PopupState::None;
			Ref<Window> window = getWindow();
			lock.unlock();
			dispatchPause();
			dispatchClose();
			if (window.isNotNull()) {
				window->close();
			}
		} else if (m_popupState == PopupState::Popup) {
			m_popupState = PopupState::ClosingPopup;
			Ref<MobileApp> mobile = MobileApp::getApp();
			if (mobile.isNotNull()) {
				mobile->m_popupPages.remove(this);
			}
			if (isDrawingThread()) {
				_closePopup(transition);
			} else {
				dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPage, _closePopup, this, transition));
			}
		} else {
			Ref<ViewPager> pager = getPager();
			if (pager.isNotNull()) {
				pager->pop(this, transition);
			}
		}
	}

	void ViewPage::close()
	{
		close(Transition());
	}

	void ViewPage::goToPage(const Ref<View>& page, const Transition& transition)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->open(pager, transition);
			} else {
				pager->push(page, transition);
			}
		}
	}

	void ViewPage::goToPage(const Ref<View>& page)
	{
		goToPage(page, Transition());
	}

	void ViewPage::goToHomePage(const Ref<View>& page, const Transition& transition)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->openHome(pager, transition);
			} else {
				pager->push(page, transition, sl_true);
			}
		}
	}

	void ViewPage::goToHomePage(const Ref<View>& page)
	{
		goToHomePage(page, Transition());
	}

	TransitionType ViewPage::getGlobalOpeningTransitionType()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPushTransitionType();
		}
		return TransitionType::Default;
	}

	void ViewPage::setGlobalOpeningTransitionType(TransitionType type)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPushTransitionType(type);
		}
	}

	TransitionType ViewPage::getGlobalClosingTransitionType()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPopTransitionType();
		}
		return TransitionType::Default;
	}

	void ViewPage::setGlobalClosingTransitionType(TransitionType type)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPopTransitionType(type);
		}
	}

	void ViewPage::setGlobalTransitionType(TransitionType type)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setTransitionType(type);
		}
	}

	TransitionDirection ViewPage::getGlobalOpeningTransitionDirection()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPushTransitionDirection();
		}
		return TransitionDirection::Default;
	}

	void ViewPage::setGlobalOpeningTransitionDirection(TransitionDirection direction)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPushTransitionDirection(direction);
		}
	}

	TransitionDirection ViewPage::getGlobalClosingTransitionDirection()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPopTransitionDirection();
		}
		return TransitionDirection::Default;
	}

	void ViewPage::setGlobalClosingTransitionDirection(TransitionDirection direction)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPopTransitionDirection(direction);
		}
	}

	void ViewPage::setGlobalTransitionDirection(TransitionDirection direction)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setTransitionDirection(direction);
		}
	}

	float ViewPage::getGlobalOpeningTransitionDuration()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPushTransitionDuration();
		}
		return 0;
	}

	void ViewPage::setGlobalOpeningTransitionDuration(float duration)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPushTransitionDuration(duration);
		}
	}

	float ViewPage::getGlobalClosingTransitionDuration()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPopTransitionDuration();
		}
		return 0;
	}

	void ViewPage::setGlobalClosingTransitionDuration(float duration)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPopTransitionDuration(duration);
		}
	}

	void ViewPage::setGlobalTransitionDuration(float duration)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setTransitionDuration(duration);
		}
	}

	AnimationCurve ViewPage::getGlobalOpeningTransitionCurve()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPushTransitionCurve();
		}
		return AnimationCurve::Default;
	}

	void ViewPage::setGlobalOpeningTransitionCurve(AnimationCurve curve)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPushTransitionCurve(curve);
		}
	}

	AnimationCurve ViewPage::getGlobalClosingTransitionCurve()
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			return pager->getPopTransitionCurve();
		}
		return AnimationCurve::Default;
	}

	void ViewPage::setGlobalClosingTransitionCurve(AnimationCurve curve)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setPopTransitionCurve(curve);
		}
	}

	void ViewPage::setGlobalTransitionCurve(AnimationCurve curve)
	{
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			pager->setTransitionCurve(curve);
		}
	}

	class _priv_ViewPagePopupBackground : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
	};

	SLIB_DEFINE_OBJECT(_priv_ViewPagePopupBackground, ViewGroup)

	void ViewPage::_openPopup(const Ref<View>& parent, Transition transition, sl_bool flagFillParentBackground)
	{
		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPage, _openPopup, this, parent, transition, flagFillParentBackground), 100);
			return;
		}
		
		Ref<View> viewAdd;
		if (flagFillParentBackground) {
			Ref<_priv_ViewPagePopupBackground> back = new _priv_ViewPagePopupBackground;
			Color color = m_popupBackgroundColor;
			if (color.isZero()) {
				color = getGlobalPopupBackgroundColor();
			}
			back->setBackgroundColor(color);
			back->setWidthFilling(1, UIUpdateMode::Init);
			back->setHeightFilling(1, UIUpdateMode::Init);
			back->addChild(this, UIUpdateMode::Init);
			viewAdd = back;
		} else {
			viewAdd = this;
		}
		
		setVisibility(Visibility::Hidden, UIUpdateMode::None);
		setTranslation(0, 0, UIUpdateMode::Init);
		setScale(1, 1, UIUpdateMode::Init);
		setRotation(0, UIUpdateMode::None);
		setAlpha(1, UIUpdateMode::None);
		
		_applyDefaultOpeningPopupTransition(transition);
		
		setEnabled(sl_false, UIUpdateMode::None);
		
		Ref<Animation> animation = Transition::createPopupAnimation(this, transition, UIPageAction::Push, SLIB_BIND_WEAKREF(void(), ViewPage, _finishPopupAnimation, this, UIPageAction::Push));
		
		parent->addChild(viewAdd, UIUpdateMode::None);
		
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		
		dispatchOpen();
		
		dispatchResume();
		
		if (animation.isNotNull()) {
			animation->dispatchStartFrame();
		}
		
		setVisibility(Visibility::Visible);
		
		if (animation.isNull()) {
			_finishPopupAnimation(UIPageAction::Push);
		} else {
			ViewPager::_runAnimationProc(this, [animation]() {
				animation->start();
			});
		}
	}

	void ViewPage::_closePopup(Transition transition)
	{

		ObjectLocker lock(this);
		
		if (m_countActiveTransitionAnimations) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPage, _closePopup, this, transition), 100);
			return;
		}

#if defined(SLIB_UI_IS_ANDROID)
		Android::dismissKeyboard();
#endif

		_applyDefaultClosingPopupTransition(transition);
		
		setEnabled(sl_false, UIUpdateMode::None);
		
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			if (IsInstanceOf<_priv_ViewPagePopupBackground>(parent)) {
				parent->setBackgroundColor(Color::zero());
			}
		}
		
		Ref<Animation> animation = Transition::createPopupAnimation(this, transition, UIPageAction::Pop, SLIB_BIND_WEAKREF(void(), ViewPage, _finishPopupAnimation, this, UIPageAction::Pop));
		
		Base::interlockedIncrement(&m_countActiveTransitionAnimations);
		
		dispatchPause();

		dispatchClose();
		
		if (animation.isNotNull()) {
			animation->start();
		}
		
		if (animation.isNull()) {
			_finishPopupAnimation(UIPageAction::Pop);
		}
		
	}

	void ViewPage::_finishPopupAnimation(UIPageAction action)
	{
		ObjectLocker lock(this);
		
		dispatchEndPageAnimation(sl_null, action);
		
		if (action == UIPageAction::Pop) {
			
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				if (IsInstanceOf<_priv_ViewPagePopupBackground>(parent)) {
					Ref<View> parent2 = parent->getParent();
					if (parent2.isNotNull()) {
						parent2->removeChild(parent);
					}
				} else {
					parent->removeChild(this);
				}
			}
			
			m_popupState = PopupState::None;
			
		} else {
			setEnabled(sl_true, UIUpdateMode::None);
		}
		
		Base::interlockedDecrement(&m_countActiveTransitionAnimations);
	}

	sl_bool ViewPage::_dispatchBack()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchBack(ev.get());
			if (ev->isPreventedDefault()) {
				return sl_false;
			}
		}
		return sl_true;
	}

	void ViewPage::popup(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground)
	{
		if (parent.isNull()) {
			return;
		}
		ObjectLocker lock(this);
		if (m_popupState != PopupState::None) {
			return;
		}
		
		Ref<MobileApp> mobile = MobileApp::getApp();
		if (mobile.isNotNull()) {
			mobile->m_popupPages.add(this);
		}

		if (isDrawingThread()) {
			_openPopup(parent, transition, flagFillParentBackground);
		} else {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPage, _openPopup, this, parent, transition, flagFillParentBackground));
		}
		m_popupState = PopupState::Popup;
	}

	void ViewPage::popup(const Ref<View>& parent, sl_bool flagFillParentBackground)
	{
		popup(parent, Transition(), flagFillParentBackground);
	}

	Ref<Window> ViewPage::popupWindow(const Ref<Window>& parent)
	{
		ObjectLocker lock(this);
		
		if (m_popupState != PopupState::None) {
			return sl_null;
		}
		
		Ref<Window> window = new Window;
		
		if (window.isNotNull()) {
			window->setParent(parent);
			window->setContentView(this);
			window->setDialog(sl_true);
			if (isCenterVertical() && isCenterHorizontal()) {
				window->setCenterScreenOnCreate(sl_true);
			} else {
				window->setLeft(getLeft());
				window->setTop(getTop());
			}
			window->setClientSize(getWidth(), getHeight());
			window->setModal(sl_true);
			window->setOnClose(SLIB_FUNCTION_WEAKREF(ViewPage, _onClosePopupWindow, this));
			
			window->create();
			
			m_popupState = PopupState::ShowWindow;
			
			lock.unlock();
			
			dispatchOpen();
			
			dispatchResume();
			
			return window;
			
		}
		
		return sl_null;
	}

	void ViewPage::_onClosePopupWindow(Window* window, UIEvent* ev)
	{
		ObjectLocker lock(this);
		if (m_popupState == PopupState::ShowWindow) {
			if (_dispatchBack()) {
				m_popupState = PopupState::None;
				lock.unlock();
				dispatchPause();
				dispatchClose();
			} else {
				ev->preventDefault();
			}
		}
	}

	sl_bool ViewPage::isPopup()
	{
		return m_popupState == PopupState::Popup;
	}

	Color ViewPage::getPopupBackgroundColor()
	{
		return m_popupBackgroundColor;
	}

	void ViewPage::setPopupBackgroundColor(const Color& color)
	{
		m_popupBackgroundColor = color;
	}

	TransitionType _g_viewPage_globalOpeningPopupTransitionType = TransitionType::Zoom;
	TransitionDirection _g_viewPage_globalOpeningPopupTransitionDirection = TransitionDirection::FromBottomToTop;
	float _g_viewPage_globalOpeningPopupTransitionDuration = 0.3f;
	AnimationCurve _g_viewPage_globalOpeningPopupTransitionCurve = AnimationCurve::Overshoot;
	TransitionType _g_viewPage_globalClosingPopupTransitionType = TransitionType::Fade;
	TransitionDirection _g_viewPage_globalClosingPopupTransitionDirection = TransitionDirection::FromTopToBottom;
	float _g_viewPage_globalClosingPopupTransitionDuration = 0.3f;
	AnimationCurve _g_viewPage_globalClosingPopupTransitionCurve = AnimationCurve::Linear;
	SLIB_STATIC_COLOR(_g_viewPage_globalPopupBackgroundColor, 0, 0, 0, 120)

	TransitionType ViewPage::getGlobalOpeningPopupTransitionType()
	{
		return _g_viewPage_globalOpeningPopupTransitionType;
	}

	void ViewPage::setGlobalOpeningPopupTransitionType(TransitionType type)
	{
		_g_viewPage_globalOpeningPopupTransitionType = type;
	}

	TransitionType ViewPage::getGlobalClosingPopupTransitionType()
	{
		return _g_viewPage_globalClosingPopupTransitionType;
	}

	void ViewPage::setGlobalClosingPopupTransitionType(TransitionType type)
	{
		_g_viewPage_globalClosingPopupTransitionType = type;
	}

	void ViewPage::setGlobalPopupTransitionType(TransitionType type)
	{
		_g_viewPage_globalOpeningPopupTransitionType = type;
		_g_viewPage_globalClosingPopupTransitionType = type;
	}

	TransitionDirection ViewPage::getGlobalOpeningPopupTransitionDirection()
	{
		return _g_viewPage_globalOpeningPopupTransitionDirection;
	}

	void ViewPage::setGlobalOpeningPopupTransitionDirection(TransitionDirection direction)
	{
		_g_viewPage_globalOpeningPopupTransitionDirection = direction;
	}

	TransitionDirection ViewPage::getGlobalClosingPopupTransitionDirection()
	{
		return _g_viewPage_globalClosingPopupTransitionDirection;
	}

	void ViewPage::setGlobalClosingPopupTransitionDirection(TransitionDirection direction)
	{
		_g_viewPage_globalClosingPopupTransitionDirection = direction;
	}

	void ViewPage::setGlobalPopupTransitionDirection(TransitionDirection direction)
	{
		_g_viewPage_globalOpeningPopupTransitionDirection = direction;
		_g_viewPage_globalClosingPopupTransitionDirection = direction;
	}

	float ViewPage::getGlobalOpeningPopupTransitionDuration()
	{
		return _g_viewPage_globalOpeningPopupTransitionDuration;
	}

	void ViewPage::setGlobalOpeningPopupTransitionDuration(float duration)
	{
		_g_viewPage_globalOpeningPopupTransitionDuration = duration;
	}

	float ViewPage::getGlobalClosingPopupTransitionDuration()
	{
		return _g_viewPage_globalClosingPopupTransitionDuration;
	}

	void ViewPage::setGlobalClosingPopupTransitionDuration(float duration)
	{
		_g_viewPage_globalClosingPopupTransitionDuration = duration;
	}

	void ViewPage::setGlobalPopupTransitionDuration(float duration)
	{
		_g_viewPage_globalOpeningPopupTransitionDuration = duration;
		_g_viewPage_globalClosingPopupTransitionDuration = duration;
	}

	AnimationCurve ViewPage::getGlobalOpeningPopupTransitionCurve()
	{
		return _g_viewPage_globalOpeningPopupTransitionCurve;
	}

	void ViewPage::setGlobalOpeningPopupTransitionCurve(AnimationCurve curve)
	{
		_g_viewPage_globalOpeningPopupTransitionCurve = curve;
	}

	AnimationCurve ViewPage::getGlobalClosingPopupTransitionCurve()
	{
		return _g_viewPage_globalClosingPopupTransitionCurve;
	}

	void ViewPage::setGlobalClosingPopupTransitionCurve(AnimationCurve curve)
	{
		_g_viewPage_globalClosingPopupTransitionCurve = curve;
	}

	void ViewPage::setGlobalPopupTransitionCurve(AnimationCurve curve)
	{
		_g_viewPage_globalOpeningPopupTransitionCurve = curve;
		_g_viewPage_globalClosingPopupTransitionCurve = curve;
	}

	Color ViewPage::getGlobalPopupBackgroundColor()
	{
		return _g_viewPage_globalPopupBackgroundColor;
	}

	void ViewPage::setGlobalPopupBackgroundColor(const Color& color)
	{
		_g_viewPage_globalPopupBackgroundColor = color;
	}

	void ViewPage::_applyDefaultOpeningPopupTransition(Transition& transition)
	{
		if (transition.type == TransitionType::Default) {
			transition.type = getGlobalOpeningPopupTransitionType();
		}
		if (transition.direction == TransitionDirection::Default) {
			transition.direction = getGlobalOpeningPopupTransitionDirection();
		}
		if (transition.duration < SLIB_EPSILON) {
			transition.duration = getGlobalOpeningPopupTransitionDuration();
		}
		if (transition.curve == AnimationCurve::Default) {
			transition.curve = getGlobalOpeningPopupTransitionCurve();
		}
	}

	void ViewPage::_applyDefaultClosingPopupTransition(Transition& transition)
	{
		if (transition.type == TransitionType::Default) {
			transition.type = getGlobalClosingPopupTransitionType();
		}
		if (transition.direction == TransitionDirection::Default) {
			transition.direction = getGlobalClosingPopupTransitionDirection();
		}
		if (transition.duration < SLIB_EPSILON) {
			transition.duration = getGlobalClosingPopupTransitionDuration();
		}
		if (transition.curve == AnimationCurve::Default) {
			transition.curve = getGlobalClosingPopupTransitionCurve();
		}
	}

	void ViewPage::onOpen()
	{
	}

	void ViewPage::onClose()
	{
	}

	void ViewPage::onResume()
	{
	}

	void ViewPage::onPause()
	{
	}

	void ViewPage::onPageAction(UIPageAction action)
	{
	}

	void ViewPage::onEndPageAnimation(UIPageAction action)
	{
	}

	void ViewPage::onBackPressed(UIEvent* ev)
	{
	}

	void ViewPage::onBack(UIEvent* ev)
	{
	}

	void ViewPage::dispatchPageAction(ViewPager* pager, UIPageAction action)
	{
		m_pager = pager;
		onPageAction(action);
		getOnPageAction()(this, pager, action);
		switch (action) {
			case UIPageAction::Push:
				dispatchOpen();
				break;
			case UIPageAction::Pop:
				dispatchClose();
				break;
			case UIPageAction::Resume:
				dispatchResume();
				break;
			case UIPageAction::Pause:
				dispatchPause();
				break;
		}
	}

	void ViewPage::dispatchEndPageAnimation(ViewPager* pager, UIPageAction action)
	{
		m_pager = pager;
		onEndPageAnimation(action);
		getOnEndPageAnimation()(this, pager, action);
	}

	void ViewPage::dispatchOpen()
	{
		onOpen();
		getOnOpen()(this);
	}

	void ViewPage::dispatchClose()
	{
		onClose();
		getOnClose()(this);
	}

	void ViewPage::dispatchResume()
	{
		onResume();
		getOnResume()(this);
	}

	void ViewPage::dispatchPause()
	{
		onPause();
		getOnPause()(this);
	}

	void ViewPage::dispatchBackPressed(UIEvent* ev)
	{
		onBackPressed(ev);
		if (ev->isPreventedDefault()) {
			return;
		}
		if (_dispatchBack()) {
			close();
		}
	}

	void ViewPage::dispatchBack(UIEvent* ev)
	{
		onBack(ev);
		getOnBack()(this, ev);
	}

	void ViewPage::dispatchOK(UIEvent* ev)
	{
		ViewGroup::dispatchOK(ev);
	}

	void ViewPage::dispatchCancel(UIEvent* ev)
	{
		onCancel(ev);
		getOnCancel()(this, ev);
		if (ev->isStoppedPropagation()) {
			return;
		}
		if (ev->isPreventedDefault()) {
			return;
		}
		if (_dispatchBack()) {
			close();
		}
	}

}
