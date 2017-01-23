#include "../../../inc/slib/ui/view_page.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/mobile_app.h"

#include "../../../inc/slib/core/scoped.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)
#	include "../../../inc/slib/core/platform_android.h"
#endif

SLIB_UI_NAMESPACE_BEGIN

void IViewPagerListener::onPageAction(ViewPager* navigation, View* page, UIPageAction action)
{
}

void IViewPagerListener::onFinishPageAnimation(ViewPager* navigation, View* page, UIPageAction action)
{
}

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
	
	m_countActiveTransitions = 0;
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
	ObjectLocker lock(this);
	if (mode == UIUpdateMode::Init) {
		page->setFrame(getBoundsInnerPadding(), mode);
		m_pages.addIfNotExist_NoLock(page);
	} else {
		if (m_pages.getCount() == 0) {
			m_pages.add_NoLock(page);
			lock.unlock();
			page->setFrame(getBoundsInnerPadding(), mode);
			addChild(page, mode);
			dispatchPageAction(page.get(), UIPageAction::Resume);
			m_indexCurrent = 0;
			return;
		}
		m_pages.addIfNotExist_NoLock(page);
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
		if (index == m_indexCurrent) {
			Ref<View> oldPage = m_pages.getData()[index];
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
			if (mode != UIUpdateMode::Init) {
				if (oldPage.isNotNull()) {
					if (oldPage->getParent().isNotNull()) {
						dispatchPageAction(oldPage.get(), UIPageAction::Pause);
					}
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
	if (mode == UIUpdateMode::Init) {
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
		addChild(viewIn, mode);
		dispatchPageAction(viewOut.get(), UIPageAction::Pause);
		dispatchPageAction(viewIn.get(), UIPageAction::Resume);
		removeChild(viewOut, mode);
	}
}

void _ViewPager_FinishAnimation(const Ref<ViewPager>& pager, const Ref<View>& view, UIPageAction action)
{
	if (pager.isNotNull() && view.isNotNull()) {
		pager->dispatchFinishPageAnimation(view.get(), action);
		switch (action) {
			case UIPageAction::Pause:
			case UIPageAction::Pop:
				if (pager->getCurrentPage() != view) {
					view->setVisibility(Visibility::Hidden);
					pager->removeChild(view);
				}
				break;
			default:
				view->setEnabled(sl_true);
				break;
		}
		pager->setEnabled(sl_true);
	}
}

void ViewPager::_stopCurrentAnimations()
{
	if (m_currentPushAnimation.isNotNull()) {
		m_currentPushAnimation->dispatchEndFrame();
		m_currentPushAnimation->stop();
		m_currentPushAnimation->removeAllTargets();
		m_currentPushAnimation.setNull();
	}
	if (m_currentPopAnimation.isNotNull()) {
		m_currentPopAnimation->dispatchEndFrame();
		m_currentPopAnimation->stop();
		m_currentPopAnimation->removeAllTargets();
		m_currentPopAnimation.setNull();
	}
}

void ViewPager::_resetAnimationStatus(const Ref<View>& view)
{
	view->setTranslation(0, 0, UIUpdateMode::Init);
	view->setScale(1, 1, UIUpdateMode::Init);
	view->setRotation(0, UIUpdateMode::Init);
	view->setAlpha(1, UIUpdateMode::Init);
}

void ViewPager::_goTo(sl_size index, const Transition& _transition)
{
#if defined(SLIB_PLATFORM_IS_ANDROID)
	Android::dismissKeyboard();
#endif
	
	Transition transition = _transition;

	ObjectLocker lock(this);
	
	ScopedCounter counter(&m_countActiveTransitions);
	
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
	
	_stopCurrentAnimations();
	
	setEnabled(sl_false);
	viewOut->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	viewIn->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	Ref<Animation> animationPause, animationResume;
	if (flagNext) {
		_applyDefaultPushTransition(transition);
		animationPause = Transition::createAnimation(viewOut, transition, UIPageAction::Pause, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewOut, UIPageAction::Pause));
		animationResume = Transition::createAnimation(viewIn, transition, UIPageAction::Push, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewIn, UIPageAction::Resume));
	} else {
		_applyDefaultPopTransition(transition);
		animationPause = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewOut, UIPageAction::Pause));
		animationResume = Transition::createAnimation(viewIn, transition, UIPageAction::Resume, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewIn, UIPageAction::Resume));
	}
	
	m_currentPushAnimation = animationResume;
	m_currentPopAnimation = animationPause;
	
	_resetAnimationStatus(viewIn);
	if (animationResume.isNotNull()) {
		animationResume->dispatchStartFrame();
	}
	
	addChild(viewIn, UIUpdateMode::NoRedraw);
	
	dispatchPageAction(viewOut.get(), UIPageAction::Pause);
	
	dispatchPageAction(viewIn.get(), UIPageAction::Resume);
	
	if (animationPause.isNotNull()) {
		animationPause->dispatchStartFrame();
	}
	
	if (flagNext) {
		viewIn->bringToFront(UIUpdateMode::NoRedraw);
	} else {
		viewIn->bringToFront(UIUpdateMode::NoRedraw);
		viewOut->bringToFront(UIUpdateMode::NoRedraw);
	}
	viewIn->setVisibility(Visibility::Visible);
	
	if (animationPause.isNull()) {
		_ViewPager_FinishAnimation(this, viewOut, UIPageAction::Pause);
	}
	if (animationResume.isNull()) {
		_ViewPager_FinishAnimation(this, viewIn, UIPageAction::Resume);
	}
	
	viewIn->runAfterDraw([animationPause, animationResume]() {
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
	if (isDrawingThread() && m_countActiveTransitions == 0) {
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
#if defined(SLIB_PLATFORM_IS_ANDROID)
	Android::dismissKeyboard();
#endif

	if (viewIn.isNull()) {
		return;
	}
	
	Transition transition(_transition);
	
	ObjectLocker lock(this);
	
	ScopedCounter counter(&m_countActiveTransitions);
	
	_stopCurrentAnimations();
	
	sl_size n = m_pages.getCount();
	
	if (n == 0) {
		m_indexCurrent = 0;
		m_pages.add_NoLock(viewIn);
		viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::NoRedraw);
		_resetAnimationStatus(viewIn);
		addChild(viewIn, UIUpdateMode::NoRedraw);
		dispatchPageAction(viewIn.get(), UIPageAction::Push);
		dispatchFinishPageAnimation(viewIn.get(), UIPageAction::Push);
		viewIn->setVisibility(Visibility::Visible, UIUpdateMode::NoRedraw);
		viewIn->bringToFront();
		return;
	}
	
	Ref<View>* pages = m_pages.getData();
	Ref<View> viewBack = pages[m_indexCurrent];
	if (viewBack == viewIn) {
		return;
	}
	
	m_indexCurrent = n;

	viewIn->setFrame(getBoundsInnerPadding(), UIUpdateMode::NoRedraw);
	
	setEnabled(sl_false);
	viewBack->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	viewIn->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	_applyDefaultPushTransition(transition);
	Ref<Animation> animationPause = Transition::createAnimation(viewBack, transition, UIPageAction::Pause, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewBack, UIPageAction::Pause));
	
	Ref<Animation> animationPush = Transition::createAnimation(viewIn, transition, UIPageAction::Push, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewIn, UIPageAction::Push));
	
	m_currentPushAnimation = animationPush;
	m_currentPopAnimation = animationPause;
	
	_resetAnimationStatus(viewIn);
	if (animationPush.isNotNull()) {
		animationPush->dispatchStartFrame();
	}

	addChild(viewIn, UIUpdateMode::NoRedraw);
	
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
	
	if (animationPause.isNotNull()) {
		animationPause->dispatchStartFrame();
	}

	viewIn->bringToFront(UIUpdateMode::NoRedraw);
	viewIn->setVisibility(Visibility::Visible);
	
	if (animationPause.isNull()) {
		_ViewPager_FinishAnimation(this, viewBack, UIPageAction::Pause);
	}
	if (animationPush.isNull()) {
		_ViewPager_FinishAnimation(this, viewIn, UIPageAction::Push);
	}
	
	viewIn->runAfterDraw([animationPause, animationPush]() {
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
	if (isDrawingThread() && m_countActiveTransitions == 0) {
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
#if defined(SLIB_PLATFORM_IS_ANDROID)
	Android::dismissKeyboard();
#endif
	
	Transition transition = _transition;
	
	ObjectLocker lock(this);
	
	ScopedCounter counter(&m_countActiveTransitions);

	sl_size n = m_pages.getCount();
	
	if (n <= m_minimumPagesCount) {
		return;
	}
	
	sl_size indexPop = m_indexCurrent;
	Ref<View> viewOut = *(m_pages.getPointerAt(indexPop));
	if (_viewOut.isNotNull() && _viewOut != viewOut) {
		return;
	}
	
	_stopCurrentAnimations();
	
	if (n == 1) {
		dispatchPageAction(viewOut.get(), UIPageAction::Pop);
		dispatchFinishPageAnimation(viewOut.get(), UIPageAction::Pop);
		removeChild(viewOut);
		viewOut->setVisibility(Visibility::Hidden, UIUpdateMode::NoRedraw);
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
	viewBack->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	viewOut->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	_applyDefaultPopTransition(transition);
	Ref<Animation> animationPop = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewOut, UIPageAction::Pop));
	
	Ref<Animation> animationResume = Transition::createAnimation(viewBack, transition, UIPageAction::Resume, Function<void()>::bind(&_ViewPager_FinishAnimation, Ref<ViewPager>(this), viewBack, UIPageAction::Resume));
	
	m_currentPopAnimation = animationPop;
	m_currentPushAnimation = animationResume;
	
	_resetAnimationStatus(viewBack);
	if (animationResume.isNotNull()) {
		animationResume->dispatchStartFrame();
	}

	addChild(viewBack, UIUpdateMode::NoRedraw);

	dispatchPageAction(viewOut.get(), UIPageAction::Pop);
	dispatchPageAction(viewBack.get(), UIPageAction::Resume);
	
	if (animationPop.isNotNull()) {
		animationPop->start();
	}
	if (animationResume.isNotNull()) {
		animationResume->start();
	}
	
	viewBack->bringToFront(UIUpdateMode::NoRedraw);
	viewOut->bringToFront(UIUpdateMode::NoRedraw);
	
	viewBack->setVisibility(Visibility::Visible);
	
	m_pages.removeAt_NoLock(indexPop);
	
	if (animationPop.isNull()) {
		_ViewPager_FinishAnimation(this, viewOut, UIPageAction::Pop);
	}
	if (animationResume.isNull()) {
		_ViewPager_FinishAnimation(this, viewBack, UIPageAction::Resume);
	}
	
}

void ViewPager::pop(const Ref<View>& viewOut, const Transition& transition)
{
	if (isDrawingThread() && m_countActiveTransitions == 0) {
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

void ViewPager::onPageAction(View* page, UIPageAction action)
{
}

void ViewPager::onFinishPageAnimation(View* page, UIPageAction action)
{
}

void ViewPager::dispatchPageAction(View* page, UIPageAction action)
{
	if (page) {
		onPageAction(page, action);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onPageAction(this, page, action);
		}
		if (ViewPage* _page = CastInstance<ViewPage>(page)) {
			_page->dispatchPageAction(this, action);
		}
	}
}

void ViewPager::dispatchFinishPageAnimation(View* page, UIPageAction action)
{
	if (page) {
		onFinishPageAnimation(page, action);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onFinishPageAnimation(this, page, action);
		}
		if (ViewPage* _page = CastInstance<ViewPage>(page)) {
			_page->dispatchFinishPageAnimation(this, action);
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

void ViewPager::onSwipe(GestureType type)
{
	if (m_flagSwipeNavigation) {
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
	
	m_flagDidPopup = sl_false;
	m_flagClosingPopup = sl_false;
	m_popupBackgroundColor = Color::zero();
	m_flagDidModalOnUIThread = sl_false;
	
	m_countActiveTransitions = 0;
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
		if (m_flagDidPopup) {
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
		if (m_flagDidPopup) {
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
	if (m_flagDidPopup) {
		if (m_flagClosingPopup) {
			return;
		}
		m_flagClosingPopup = sl_true;
		
		Ref<MobileApp> mobile = MobileApp::getApp();
		if (mobile.isNotNull()) {
			mobile->m_popupPages.removeValue(this);
		}
		
		if (isDrawingThread() && m_countActiveTransitions == 0) {
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

void ViewPage::openPage(const Ref<View>& page, const Transition& transition)
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

void ViewPage::openPage(const Ref<View>& page)
{
	openPage(page, Transition());
}

void ViewPage::openHomePage(const Ref<View>& page, const Transition& transition)
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

void ViewPage::openHomePage(const Ref<View>& page)
{
	openHomePage(page, Transition());
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

class _ViewPagePopupBackground : public ViewGroup
{
	SLIB_DECLARE_OBJECT
};

SLIB_DEFINE_OBJECT(_ViewPagePopupBackground, ViewGroup)

void ViewPage::_openPopup(const Ref<View>& parent, Transition transition, sl_bool flagFillParentBackground)
{
	ObjectLocker lock(this);
	
	ScopedCounter counter(&m_countActiveTransitions);
	
	Ref<View> viewAdd;
	if (flagFillParentBackground) {
		Ref<_ViewPagePopupBackground> back = new _ViewPagePopupBackground;
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
	
	_applyDefaultOpeningPopupTransition(transition);
	
	setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	Ref<Animation> animation = Transition::createPopupAnimation(this, transition, UIPageAction::Push, SLIB_BIND_WEAKREF(void(), ViewPage, _finishPopupAnimation, this, UIPageAction::Push));
	
	parent->addChild(viewAdd, UIUpdateMode::NoRedraw);
	
	dispatchOpen();
	
	if (animation.isNotNull()) {
		animation->dispatchStartFrame();
	}
	
	setVisibility(Visibility::Visible);
	
	if (animation.isNull()) {
		_finishPopupAnimation(UIPageAction::Push);
	} else {
		runAfterDraw([animation]() {
			animation->start();
		});
	}
}

void ViewPage::_closePopup(Transition transition)
{
	ObjectLocker lock(this);
	
	ScopedCounter counter(&m_countActiveTransitions);
	
#if defined(SLIB_PLATFORM_IS_ANDROID)
	Android::dismissKeyboard();
#endif
	
	_applyDefaultClosingPopupTransition(transition);
	
	setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	Ref<View> parent = getParent();
	if (parent.isNotNull()) {
		if (IsInstanceOf<_ViewPagePopupBackground>(parent)) {
			parent->setBackgroundColor(Color::zero());
		}
	}
	
	Ref<Animation> animation = Transition::createPopupAnimation(this, transition, UIPageAction::Pop, SLIB_BIND_WEAKREF(void(), ViewPage, _finishPopupAnimation, this, UIPageAction::Pop));
	
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
	
	dispatchFinishPageAnimation(sl_null, action);
	
	if (action == UIPageAction::Pop) {
		
		Ref<View> parent = getParent();
		if (parent.isNotNull()) {
			if (IsInstanceOf<_ViewPagePopupBackground>(parent)) {
				Ref<View> parent2 = parent->getParent();
				if (parent2.isNotNull()) {
					parent2->removeChild(parent);
				}
			} else {
				parent->removeChild(this);
			}
		}
		
		Ref<Event> event = m_eventClosePopup;
		if (event.isNotNull()) {
			event->set();
			m_eventClosePopup.setNull();
		}
		
		if (m_flagDidModalOnUIThread) {
			m_flagDidModalOnUIThread = sl_false;
			UI::quitLoop();
		}
		
		m_flagDidPopup = sl_false;
		
	} else {
		setEnabled(sl_true, UIUpdateMode::NoRedraw);
	}
}

void ViewPage::popup(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground)
{
	if (parent.isNull()) {
		return;
	}
	ObjectLocker lock(this);
	if (m_flagDidPopup) {
		return;
	}
	
	Ref<MobileApp> mobile = MobileApp::getApp();
	if (mobile.isNotNull()) {
		mobile->m_popupPages.add(this);
	}

	if (isDrawingThread() && m_countActiveTransitions == 0) {
		_openPopup(parent, transition, flagFillParentBackground);
	} else {
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ViewPage, _openPopup, this, parent, transition, flagFillParentBackground));
	}
	m_flagDidPopup = sl_true;
	m_flagClosingPopup = sl_false;
}

void ViewPage::popup(const Ref<View>& parent, sl_bool flagFillParentBackground)
{
	popup(parent, Transition(), flagFillParentBackground);
}

void ViewPage::popupPage(const Ref<ViewPage>& pageOther, const Transition& transition, sl_bool flagFillParentBackground)
{
	if (pageOther.isNotNull()) {
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			Ref<View> parent = pager->getParent();
			if (parent.isNotNull()) {
				pageOther->popup(parent, transition, flagFillParentBackground);
			}
		}
	}
}

void ViewPage::popupPage(const Ref<ViewPage>& pageOther, sl_bool flagFillParentBackground)
{
	popupPage(pageOther, Transition(), flagFillParentBackground);
}

sl_bool ViewPage::isPopup()
{
	return m_flagDidPopup;
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

void ViewPage::modal(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground)
{
	
	if (parent.isNull()) {
		return;
	}
	ObjectLocker lock(this);
	if (m_flagDidPopup) {
		return;
	}
	
	popup(parent, transition, flagFillParentBackground);
	
	lock.unlock();
	
	if (UI::isUiThread()) {
		m_flagDidModalOnUIThread = sl_true;
		UI::runLoop();
		m_flagDidModalOnUIThread = sl_false;
	} else {
		m_flagDidModalOnUIThread = sl_false;
		Ref<Event> ev = Event::create(sl_false);
		if (ev.isNotNull()) {
			m_eventClosePopup = ev;
			ev->wait();
			m_eventClosePopup.setNull();
		}
	}
	
}

void ViewPage::modal(const Ref<View>& parent, sl_bool flagFillParentBackground)
{
	modal(parent, Transition(), flagFillParentBackground);
}

void ViewPage::modalPage(const Ref<ViewPage>& pageOther, const Transition& transition, sl_bool flagFillParentBackground)
{
	if (pageOther.isNotNull()) {
		Ref<ViewPager> pager = getPager();
		if (pager.isNotNull()) {
			Ref<View> parent = pager->getParent();
			if (parent.isNotNull()) {
				pageOther->modal(parent, transition, flagFillParentBackground);
			}
		}
	}
}

void ViewPage::modalPage(const Ref<ViewPage>& pageOther, sl_bool flagFillParentBackground)
{
	modalPage(pageOther, Transition(), flagFillParentBackground);
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

void ViewPage::onFinishPageAnimation(UIPageAction action)
{
}

void ViewPage::onBackPressed(UIEvent* ev)
{
}

void ViewPage::dispatchPageAction(ViewPager* pager, UIPageAction action)
{
	m_pager = pager;
	onPageAction(action);
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

void ViewPage::dispatchFinishPageAnimation(ViewPager* pager, UIPageAction action)
{
	m_pager = pager;
	onFinishPageAnimation(action);
}

void ViewPage::dispatchOpen()
{
	dispatchResume();
	onOpen();
}

void ViewPage::dispatchClose()
{
	dispatchPause();
	onClose();
	getOnClose()(this);
}

void ViewPage::dispatchResume()
{
	onResume();
}

void ViewPage::dispatchPause()
{
	onPause();
}

void ViewPage::dispatchBackPressed(UIEvent* ev)
{
	onBackPressed(ev);
}

SLIB_UI_NAMESPACE_END
