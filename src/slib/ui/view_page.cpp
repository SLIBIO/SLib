#include "../../../inc/slib/ui/view_page.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/mobile_app.h"

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
	m_pushTransitionCurve = AnimationCurve::EaseInOut;
	m_pushTransitionDuration = 0.5f;
	m_popTransitionType = TransitionType::Push;
	m_popTransitionDirection = TransitionDirection::FromLeftToRight;
	m_popTransitionCurve = AnimationCurve::EaseInOut;
	m_popTransitionDuration = 0.5f;
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
				view->setEnabled(sl_true);
				break;
		}
		view->resetAnimations();
	}
}

void ViewStack::_push(Ref<View> viewIn, Transition transition, sl_bool flagRemoveAllBackPages)
{
	if (viewIn.isNull()) {
		return;
	}
	
	ObjectLocker lock(this);
	
	sl_size n = m_pages.getCount();
	
	if (n == 0) {
		m_pages.add_NoLock(viewIn);
		viewIn->setFrame(getBounds(), UIUpdateMode::NoRedraw);
		addChild(viewIn, UIUpdateMode::NoRedraw);
		dispatchPageAction(viewIn.ptr, UIPageAction::Push);
		dispatchFinishPageAnimation(viewIn.ptr, UIPageAction::Push);
		viewIn->setVisibility(Visibility::Visible, UIUpdateMode::NoRedraw);
		viewIn->bringToFront();
		return;
	}
	
	Ref<View>* pages = m_pages.getData();
	Ref<View> viewBack = pages[n-1];
	if (viewBack == viewIn) {
		return;
	}
	
	viewIn->setFrame(getBounds(), UIUpdateMode::NoRedraw);
	
	viewBack->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	viewIn->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	_applyDefaultPushTransition(transition);
	Ref<Animation> animationPause = Transition::createAnimation(viewBack, transition, UIPageAction::Pause, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewBack, UIPageAction::Pause));
	
	Ref<Animation> animationPush = Transition::createAnimation(viewIn, transition, UIPageAction::Push, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewIn, UIPageAction::Push));
	
	addChild(viewIn, UIUpdateMode::NoRedraw);
	
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

	viewIn->bringToFront(UIUpdateMode::NoRedraw);
	viewIn->setVisibility(Visibility::Visible);
	
	if (animationPause.isNull()) {
		_ViewStack_FinishAnimation(this, viewBack, UIPageAction::Pause);
	}
	if (animationPush.isNull()) {
		_ViewStack_FinishAnimation(this, viewIn, UIPageAction::Push);
	}
	
}

void ViewStack::push(const Ref<View>& viewIn, const Transition& transition, sl_bool flagRemoveAllBackPages)
{
	if (UI::isUiThread()) {
		_push(viewIn, transition, flagRemoveAllBackPages);
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewStack, _push, this, viewIn, transition, flagRemoveAllBackPages));
	}
}

void ViewStack::push(const Ref<View>& viewIn, sl_bool flagRemoveAllBackPages)
{
	if (UI::isUiThread()) {
		_push(viewIn, Transition(), flagRemoveAllBackPages);
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewStack, _push, this, viewIn, Transition(), flagRemoveAllBackPages));
	}
}

void ViewStack::_pop(Ref<View> _viewOut, Transition transition)
{
	ObjectLocker lock(this);
	
	sl_size n = m_pages.getCount();
	
	if (n == 0) {
		return;
	}
	
	Ref<View> viewOut = *(m_pages.getItemPtr(n-1));
	if (_viewOut.isNotNull() && _viewOut != viewOut) {
		return;
	}
	
	if (n == 1) {
		dispatchPageAction(viewOut.ptr, UIPageAction::Pop);
		dispatchFinishPageAnimation(viewOut.ptr, UIPageAction::Pop);
		removeChild(viewOut);
		viewOut->setVisibility(Visibility::Hidden, UIUpdateMode::NoRedraw);
		m_pages.removeAll_NoLock();
		return;
	}
	
	Ref<View> viewBack = *(m_pages.getItemPtr(n-2));
	
	viewBack->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	viewOut->setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	_applyDefaultPopTransition(transition);
	Ref<Animation> animationPop = Transition::createAnimation(viewOut, transition, UIPageAction::Pop, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewOut, UIPageAction::Pop));
	
	Ref<Animation> animationResume = Transition::createAnimation(viewBack, transition, UIPageAction::Resume, SLIB_CALLBACK(_ViewStack_FinishAnimation, Ref<ViewStack>(this), viewBack, UIPageAction::Resume));
	
	addChild(viewBack, UIUpdateMode::NoRedraw);

	dispatchPageAction(viewOut.ptr, UIPageAction::Pop);
	dispatchPageAction(viewBack.ptr, UIPageAction::Resume);
	
	Time now = Time::now();
	if (animationPop.isNotNull()) {
		animationPop->start(now);
	}
	if (animationResume.isNotNull()) {
		animationResume->start(now);
	}
	
	viewBack->bringToFront(UIUpdateMode::NoRedraw);
	viewOut->bringToFront(UIUpdateMode::NoRedraw);
	
	viewBack->setVisibility(Visibility::Visible);
	
	m_pages.popBack_NoLock();
	
	if (animationPop.isNull()) {
		_ViewStack_FinishAnimation(this, viewOut, UIPageAction::Pop);
	}
	if (animationResume.isNull()) {
		_ViewStack_FinishAnimation(this, viewBack, UIPageAction::Resume);
	}

}

void ViewStack::pop(const Ref<View>& viewOut, const Transition& transition)
{
	if (UI::isUiThread()) {
		_pop(viewOut, transition);
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewStack, _pop, this, viewOut, transition));
	}
}

void ViewStack::pop(const Ref<View>& viewOut)
{
	if (UI::isUiThread()) {
		_pop(viewOut, Transition());
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewStack, _pop, this, viewOut, Transition()));
	}
}

void ViewStack::pop(const Transition& transition)
{
	if (UI::isUiThread()) {
		_pop(Ref<View>::null(), transition);
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewStack, _pop, this, Ref<View>::null(), transition));
	}
}

void ViewStack::pop()
{
	if (UI::isUiThread()) {
		_pop(Ref<View>::null(), Transition());
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewStack, _pop, this, Ref<View>::null(), Transition()));
	}
}

Ref<View> ViewStack::getCurrentPage()
{
	ObjectLocker lock(this);
	sl_size n = m_pages.getCount();
	if (n > 0) {
		return *(m_pages.getItemPtr(n-1));
	} else {
		return Ref<View>::null();
	}
}

TransitionType ViewStack::getPushTransitionType()
{
	return m_pushTransitionType;
}

void ViewStack::setPushTransitionType(TransitionType type)
{
	m_pushTransitionType = type;
}

TransitionType ViewStack::getPopTransitionType()
{
	return m_popTransitionType;
}

void ViewStack::setPopTransitionType(TransitionType type)
{
	m_popTransitionType = type;
}

void ViewStack::setTransitionType(TransitionType type)
{
	m_pushTransitionType = type;
	m_popTransitionType = type;
}

TransitionDirection ViewStack::getPushTransitionDirection()
{
	return m_pushTransitionDirection;
}

void ViewStack::setPushTransitionDirection(TransitionDirection direction)
{
	m_pushTransitionDirection = direction;
}

TransitionDirection ViewStack::getPopTransitionDirection()
{
	return m_popTransitionDirection;
}

void ViewStack::setPopTransitionDirection(TransitionDirection direction)
{
	m_popTransitionDirection = direction;
}

void ViewStack::setTransitionDirection(TransitionDirection direction)
{
	m_pushTransitionDirection = direction;
	m_popTransitionDirection = direction;
}

float ViewStack::getPushTransitionDuration()
{
	return m_pushTransitionDuration;
}

void ViewStack::setPushTransitionDuration(float duration)
{
	m_pushTransitionDuration = duration;
}

float ViewStack::getPopTransitionDuration()
{
	return m_popTransitionDuration;
}

void ViewStack::setPopTransitionDuration(float duration)
{
	m_popTransitionDuration = duration;
}

void ViewStack::setTransitionDuration(float duration)
{
	m_pushTransitionDuration = duration;
	m_popTransitionDuration = duration;
}

AnimationCurve ViewStack::getPushTransitionCurve()
{
	return m_pushTransitionCurve;
}

void ViewStack::setPushTransitionCurve(AnimationCurve curve)
{
	m_pushTransitionCurve = curve;
}

AnimationCurve ViewStack::getPopTransitionCurve()
{
	return m_popTransitionCurve;
}

void ViewStack::setPopTransitionCurve(AnimationCurve curve)
{
	m_popTransitionCurve = curve;
}

void ViewStack::setTransitionCurve(AnimationCurve curve)
{
	m_pushTransitionCurve = curve;
	m_popTransitionCurve = curve;
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
		transition.duration = m_pushTransitionDuration;
	}
	if (transition.curve == AnimationCurve::Default) {
		transition.curve = m_pushTransitionCurve;
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
		transition.duration = m_popTransitionDuration;
	}
	if (transition.curve == AnimationCurve::Default) {
		transition.curve = m_popTransitionCurve;
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
}

Ref<ViewPager> ViewPage::getPager()
{
	return m_pager;
}

void ViewPage::setPager(const Ref<ViewPager>& pager)
{
	m_pager = pager;
}

Ref<ViewStack> ViewPage::getPageStack()
{
	Ref<ViewPager> pager(m_pager);
	if (ViewStack::checkInstance(pager.ptr)) {
		return Ref<ViewStack>::from(pager);
	}
	return Ref<ViewStack>::null();
}

void ViewPage::open(const Ref<ViewStack>& stack, const Transition& transition)
{
	if (stack.isNotNull()) {
		ObjectLocker lock(this);
		if (m_flagDidPopup) {
			return;
		}
		stack->push(this, transition);
	}
}

void ViewPage::open(const Ref<ViewStack>& stack)
{
	open(stack, Transition());
}

void ViewPage::openHome(const Ref<ViewStack>& stack, const Transition& transition)
{
	if (stack.isNotNull()) {
		ObjectLocker lock(this);
		if (m_flagDidPopup) {
			return;
		}
		stack->push(this, transition, sl_true);
	}
}

void ViewPage::openHome(const Ref<ViewStack>& stack)
{
	openHome(stack, Transition());
}

void ViewPage::close(const Transition& transition)
{
	ObjectLocker lock(this);
	if (m_flagDidPopup) {
		if (m_flagClosingPopup) {
			return;
		}
		m_flagClosingPopup = sl_true;
		if (UI::isUiThread()) {
			_closePopup(transition);
		} else {
			UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewPage, _closePopup, this, transition));
		}
	} else {
		Ref<ViewStack> stack = getPageStack();
		if (stack.isNotNull()) {
			stack->pop(this, transition);
		}
	}
}

void ViewPage::close()
{
	close(Transition());
}

void ViewPage::openPage(const Ref<View>& page, const Transition& transition)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		if (ViewPage::checkInstance(page.ptr)) {
			((ViewPage*)(page.ptr))->open(stack, transition);
		} else {
			stack->push(page, transition);
		}
	}
}

void ViewPage::openPage(const Ref<View>& page)
{
	openPage(page, Transition());
}

void ViewPage::openHomePage(const Ref<View>& page, const Transition& transition)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		if (ViewPage::checkInstance(page.ptr)) {
			((ViewPage*)(page.ptr))->openHome(stack, transition);
		} else {
			stack->push(page, transition, sl_true);
		}
	}
}

void ViewPage::openHomePage(const Ref<View>& page)
{
	openHomePage(page, Transition());
}

TransitionType ViewPage::getGlobalOpeningTransitionType()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPushTransitionType();
	}
	return TransitionType::Default;
}

void ViewPage::setGlobalOpeningTransitionType(TransitionType type)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPushTransitionType(type);
	}
}

TransitionType ViewPage::getGlobalClosingTransitionType()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPopTransitionType();
	}
	return TransitionType::Default;
}

void ViewPage::setGlobalClosingTransitionType(TransitionType type)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPopTransitionType(type);
	}
}

void ViewPage::setGlobalTransitionType(TransitionType type)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setTransitionType(type);
	}
}

TransitionDirection ViewPage::getGlobalOpeningTransitionDirection()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPushTransitionDirection();
	}
	return TransitionDirection::Default;
}

void ViewPage::setGlobalOpeningTransitionDirection(TransitionDirection direction)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPushTransitionDirection(direction);
	}
}

TransitionDirection ViewPage::getGlobalClosingTransitionDirection()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPopTransitionDirection();
	}
	return TransitionDirection::Default;
}

void ViewPage::setGlobalClosingTransitionDirection(TransitionDirection direction)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPopTransitionDirection(direction);
	}
}

void ViewPage::setGlobalTransitionDirection(TransitionDirection direction)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setTransitionDirection(direction);
	}
}

float ViewPage::getGlobalOpeningTransitionDuration()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPushTransitionDuration();
	}
	return 0;
}

void ViewPage::setGlobalOpeningTransitionDuration(float duration)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPushTransitionDuration(duration);
	}
}

float ViewPage::getGlobalClosingTransitionDuration()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPopTransitionDuration();
	}
	return 0;
}

void ViewPage::setGlobalClosingTransitionDuration(float duration)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPopTransitionDuration(duration);
	}
}

void ViewPage::setGlobalTransitionDuration(float duration)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setTransitionDuration(duration);
	}
}

AnimationCurve ViewPage::getGlobalOpeningTransitionCurve()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPushTransitionCurve();
	}
	return AnimationCurve::Default;
}

void ViewPage::setGlobalOpeningTransitionCurve(AnimationCurve curve)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPushTransitionCurve(curve);
	}
}

AnimationCurve ViewPage::getGlobalClosingTransitionCurve()
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->getPopTransitionCurve();
	}
	return AnimationCurve::Default;
}

void ViewPage::setGlobalClosingTransitionCurve(AnimationCurve curve)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setPopTransitionCurve(curve);
	}
}

void ViewPage::setGlobalTransitionCurve(AnimationCurve curve)
{
	Ref<ViewStack> stack = getPageStack();
	if (stack.isNotNull()) {
		stack->setTransitionCurve(curve);
	}
}

class _ViewPagePopupBackground : public ViewGroup
{
	SLIB_DECLARE_OBJECT
};

SLIB_DEFINE_OBJECT(_ViewPagePopupBackground, ViewGroup)

void ViewPage::_openPopup(Ref<View> parent, Transition transition, sl_bool flagFillParentBackground)
{
	ObjectLocker lock(this);
	
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
	
	Ref<Animation> animation = Transition::createPopupAnimation(this, transition, UIPageAction::Push, SLIB_CALLBACK_WEAKREF(ViewPage, _finishPopupAnimation, this, UIPageAction::Push));
	
	parent->addChild(viewAdd, UIUpdateMode::NoRedraw);
	
	dispatchOpen();
	
	if (animation.isNotNull()) {
		animation->start();
	}
	
	setVisibility(Visibility::Visible);
	
	if (animation.isNull()) {
		_finishPopupAnimation(UIPageAction::Push);
	}

}

void ViewPage::_closePopup(Transition transition)
{
	ObjectLocker lock(this);
	
	_applyDefaultClosingPopupTransition(transition);
	
	setEnabled(sl_false, UIUpdateMode::NoRedraw);
	
	Ref<Animation> animation = Transition::createPopupAnimation(this, transition, UIPageAction::Pop, SLIB_CALLBACK_REF(ViewPage, _finishPopupAnimation, this, UIPageAction::Pop));
	
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
			if (_ViewPagePopupBackground::checkInstance(parent.ptr)) {
				Ref<View> parent2 = parent->getParent();
				if (parent2.isNotNull()) {
					parent2->removeChild(parent);
				}
			} else {
				parent->removeChild(this);
			}
		}
		
		Ref<MobileApp> mobile = MobileApp::getApp();
		if (mobile.isNotNull()) {
			ListLocker< Ref<ViewPage> > popups(mobile->m_popupPages);
			if (popups.count > 0) {
				if (popups[popups.count-1] == this) {
					mobile->m_popupPages.popBack_NoLock();
				}
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
	resetAnimations();
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

	if (UI::isUiThread()) {
		_openPopup(parent, transition, flagFillParentBackground);
	} else {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewPage, _openPopup, this, parent, transition, flagFillParentBackground));
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
		Ref<ViewStack> stack = getPageStack();
		if (stack.isNotNull()) {
			Ref<View> parent = stack->getParent();
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
		Ref<ViewStack> stack = getPageStack();
		if (stack.isNotNull()) {
			Ref<View> parent = stack->getParent();
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
	Ref<Runnable> callback = getOnClose();
	if (callback.isNotNull()) {
		callback->run();
	}
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
