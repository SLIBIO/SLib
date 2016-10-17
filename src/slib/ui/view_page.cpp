#include "../../../inc/slib/ui/view_page.h"

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
}

sl_size ViewPager::getPagesCount()
{
	return m_pages.getCount();
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
		if (ViewPage::checkInstance(page)) {
			((ViewPage*)page)->dispatchPageAction(this, action);
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
		if (ViewPage::checkInstance(page)) {
			((ViewPage*)page)->dispatchFinishPageAnimation(this, action);
		}
	}
}

void ViewPager::onResize(sl_ui_len width, sl_ui_len height)
{
	ObjectLocker lock(this);
	ListItems< Ref<View> > pages(m_pages);
	for (sl_size i = 0; i < pages.count; i++) {
		pages[i]->setFrame(0, 0, width, height);
	}
}


SLIB_DEFINE_OBJECT(ViewStack, ViewPager)

ViewStack::ViewStack()
{
	m_pushTransitionType = TransitionType::Push;
	m_pushTransitionDirection = TransitionDirection::FromRightToLeft;
	m_popTransitionType = TransitionType::Push;
	m_popTransitionDirection = TransitionDirection::FromLeftToRight;
	m_transitionCurve = AnimationCurve::EaseInOut;
	m_transitionDuration = 0.5f;
}

void _ViewStack_FinishAnimation(Ref<ViewStack> stack, Ref<View> view, UIPageAction action)
{
	if (stack.isNotNull() && view.isNotNull()) {
		stack->dispatchFinishPageAnimation(view.ptr, action);
		switch (action) {
			case UIPageAction::Pause:
			case UIPageAction::Pop:
				view->setVisibility(Visibility::Hidden);
				stack->removeChild(view);
				break;
			default:
				break;
		}
		view->resetAnimations();
	}
}

void ViewStack::push(const Ref<View>& viewIn, const Transition& _transition, sl_bool flagRemoveAllBackPages)
{
	if (viewIn.isNull()) {
		return;
	}
	
	ObjectLocker lock(this);
	
	sl_size n = m_pages.getCount();
	
	if (n == 0) {
		m_pages.add_NoLock(viewIn);
		viewIn->setFrame(getBounds(), sl_false);
		addChild(viewIn, sl_false);
		dispatchPageAction(viewIn.ptr, UIPageAction::Push);
		dispatchFinishPageAnimation(viewIn.ptr, UIPageAction::Push);
		viewIn->setVisibility(Visibility::Visible, sl_false);
		viewIn->bringToFront();
		return;
	}
	
	Ref<View>* pages = m_pages.getData();
	Ref<View> viewBack = pages[n-1];
	if (viewBack == viewIn) {
		return;
	}
	
	viewIn->setFrame(getBounds(), sl_false);
	
	Transition transition = _transition;
	_applyDefaultPushTransition(transition);
	Ref<Animation> animationPause = Transition::createAnimation(viewBack, transition, UIPageAction::Pause, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewBack, UIPageAction::Pause));
	
	Ref<Animation> animationPush = Transition::createAnimation(viewIn, transition, UIPageAction::Push, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewIn, UIPageAction::Push));
	
	addChild(viewIn, sl_false);
	
	dispatchPageAction(viewBack.ptr, UIPageAction::Pause);
	
	if (flagRemoveAllBackPages) {
		for (sl_size i = 0; i < n; i++) {
			dispatchPageAction(pages[i].ptr, UIPageAction::Pop);
		}
		m_pages.removeAll_NoLock();
	}
	
	m_pages.add_NoLock(viewIn);
	dispatchPageAction(viewIn.ptr, UIPageAction::Push);
	
	Time now = Time::now();
	if (animationPause.isNotNull()) {
		animationPause->start(now);
	}
	if (animationPush.isNotNull()) {
		animationPush->start(now);
	}

	viewIn->bringToFront(sl_false);
	viewIn->setVisibility(Visibility::Visible);
	
	if (animationPause.isNull()) {
		_ViewStack_FinishAnimation(this, viewBack, UIPageAction::Pause);
	}
	if (animationPush.isNull()) {
		_ViewStack_FinishAnimation(this, viewIn, UIPageAction::Push);
	}
	
}

void ViewStack::push(const Ref<View> &page, sl_bool flagRemoveAllBackPages)
{
	Transition transition;
	push(page, transition, flagRemoveAllBackPages);
}

void ViewStack::pop(const Ref<View>& viewOut, const Transition& transition)
{
	ObjectLocker lock(this);
	sl_size n = m_pages.getCount();
	if (n == 0) {
		return;
	}
	if (viewOut == *(m_pages.getItemPtr(n-1))) {
		pop(transition);
	}
}

void ViewStack::pop(const Ref<View>& viewOut)
{
	Transition transition;
	pop(viewOut, transition);
}

void ViewStack::pop(const Transition& _transition)
{
	ObjectLocker lock(this);
	
	sl_size n = m_pages.getCount();
	
	if (n == 0) {
		return;
	}
	
	if (n == 1) {
		Ref<View> viewOut = *(m_pages.getItemPtr(0));
		dispatchPageAction(viewOut.ptr, UIPageAction::Pop);
		dispatchFinishPageAnimation(viewOut.ptr, UIPageAction::Pop);
		removeChild(viewOut);
		viewOut->setVisibility(Visibility::Hidden, sl_false);
		m_pages.removeAll_NoLock();
		return;
	}
	
	Ref<View> viewBack = *(m_pages.getItemPtr(n-2));
	Ref<View> viewOut = *(m_pages.getItemPtr(n-1));
	
	Transition transition = _transition;
	_applyDefaultPopTransition(transition);
	Ref<Animation> animationPop = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewOut, UIPageAction::Pop));
	
	Ref<Animation> animationResume = Transition::createAnimation(viewBack, transition, UIPageAction::Resume, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewBack, UIPageAction::Resume));
	
	addChild(viewBack, sl_false);

	dispatchPageAction(viewOut.ptr, UIPageAction::Pop);
	dispatchPageAction(viewBack.ptr, UIPageAction::Resume);
	
	Time now = Time::now();
	if (animationPop.isNotNull()) {
		animationPop->start(now);
	}
	if (animationResume.isNotNull()) {
		animationResume->start(now);
	}
	
	viewBack->bringToFront(sl_false);
	viewOut->bringToFront(sl_false);
	
	viewBack->setVisibility(Visibility::Visible);
	
	m_pages.popBack_NoLock();
	
	if (animationPop.isNull()) {
		_ViewStack_FinishAnimation(this, viewOut, UIPageAction::Pop);
	}
	if (animationResume.isNull()) {
		_ViewStack_FinishAnimation(this, viewBack, UIPageAction::Resume);
	}

}

void ViewStack::pop()
{
	Transition transition;
	pop(transition);
}

TransitionType ViewStack::getPushTransitionType()
{
	return m_pushTransitionType;
}

void ViewStack::setPushTransitionType(TransitionType type)
{
	m_pushTransitionType = type;
}

TransitionDirection ViewStack::getPushTransitionDirection()
{
	return m_pushTransitionDirection;
}

void ViewStack::setPushTransitionDirection(TransitionDirection direction)
{
	m_pushTransitionDirection = direction;
}

TransitionType ViewStack::getPopTransitionType()
{
	return m_popTransitionType;
}

void ViewStack::setPopTransitionType(TransitionType type)
{
	m_popTransitionType = type;
}

TransitionDirection ViewStack::getPopTransitionDirection()
{
	return m_popTransitionDirection;
}

void ViewStack::setPopTransitionDirection(TransitionDirection direction)
{
	m_popTransitionDirection = direction;
}

void ViewStack::setTransitionType(TransitionType type)
{
	m_pushTransitionType = type;
	m_popTransitionType = type;
}

void ViewStack::setTransitionDirection(TransitionDirection direction)
{
	m_pushTransitionDirection = direction;
	m_popTransitionDirection = direction;
}

float ViewStack::getTransitionDuration()
{
	return m_transitionDuration;
}

void ViewStack::setTransitionDuration(float duration)
{
	m_transitionDuration = duration;
}

AnimationCurve ViewStack::getTransitionCurve()
{
	return m_transitionCurve;
}

void ViewStack::setTransitionCurve(AnimationCurve curve)
{
	m_transitionCurve = curve;
}

void ViewStack::_applyDefaultPushTransition(Transition& transition)
{
	if (transition.type == TransitionType::Default) {
		transition.type = m_pushTransitionType;
	}
	if (transition.direction == TransitionDirection::Default) {
		transition.direction = m_pushTransitionDirection;
	}
	if (transition.duration < SLIB_EPSILON) {
		transition.duration = m_transitionDuration;
	}
	if (transition.curve == AnimationCurve::Default) {
		transition.curve = m_transitionCurve;
	}
}

void ViewStack::_applyDefaultPopTransition(Transition& transition)
{
	if (transition.type == TransitionType::Default) {
		transition.type = m_popTransitionType;
	}
	if (transition.direction == TransitionDirection::Default) {
		transition.direction = m_popTransitionDirection;
	}
	if (transition.duration < SLIB_EPSILON) {
		transition.duration = m_transitionDuration;
	}
	if (transition.curve == AnimationCurve::Default) {
		transition.curve = m_transitionCurve;
	}
}


SLIB_DEFINE_OBJECT(ViewPage, ViewGroup)

ViewPage::ViewPage()
{
	SLIB_REFERABLE_CONSTRUCTOR
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

void ViewPage::dispatchPageAction(ViewPager* pager, UIPageAction action)
{
	onPageAction(action);
	switch (action) {
		case UIPageAction::Push:
			onOpen();
			onResume();
			break;
		case UIPageAction::Pop:
			onPause();
			onClose();
			break;
		case UIPageAction::Resume:
			onResume();
			break;
		case UIPageAction::Pause:
			onPause();
			break;
	}
}

void ViewPage::dispatchFinishPageAnimation(ViewPager* pager, UIPageAction action)
{
	onFinishPageAnimation(action);
}

SLIB_UI_NAMESPACE_END
