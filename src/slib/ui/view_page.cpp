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

#include "slib/ui/mobile_app.h"

#if defined(SLIB_UI_IS_ANDROID)
#	include "slib/core/platform_android.h"
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(ViewPage, ViewGroup)

	ViewPage::ViewPage()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		m_popupState = PopupState::None;
		m_popupBackgroundColor = Color::zero();
		
		m_countActiveTransitionAnimations = 0;
		
		setBackgroundColor(Color::White, UIUpdateMode::Init);
	}

	ViewPage::~ViewPage()
	{
	}

	Ref<ViewPageNavigationController> ViewPage::getNavigationController()
	{
		return m_navigationController;
	}

	void ViewPage::setNavigationController(const Ref<ViewPageNavigationController>& controller)
	{
		m_navigationController = controller;
	}

	void ViewPage::open(const Ref<ViewPageNavigationController>& controller, const Transition& transition)
	{
		if (controller.isNotNull()) {
			ObjectLocker lock(this);
			if (m_popupState != PopupState::None) {
				return;
			}
			controller->push(this, transition);
		}
	}

	void ViewPage::open(const Ref<ViewPageNavigationController>& controller)
	{
		open(controller, Transition());
	}

	void ViewPage::openHome(const Ref<ViewPageNavigationController>& controller, const Transition& transition)
	{
		if (controller.isNotNull()) {
			ObjectLocker lock(this);
			if (m_popupState != PopupState::None) {
				return;
			}
			controller->pushPageAfterPopAllPages(this, transition);
		}
	}

	void ViewPage::openHome(const Ref<ViewPageNavigationController>& controller)
	{
		openHome(controller, Transition());
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
			Ref<ViewPageNavigationController> controller = getNavigationController();
			if (controller.isNotNull()) {
				controller->pop(this, transition);
			}
		}
	}

	void ViewPage::close()
	{
		close(Transition());
	}

	void ViewPage::goToPage(const Ref<View>& page, const Transition& transition)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->open(controller, transition);
			} else {
				controller->push(page, transition);
			}
		}
	}

	void ViewPage::goToPage(const Ref<View>& page)
	{
		goToPage(page, Transition());
	}

	void ViewPage::goToHomePage(const Ref<View>& page, const Transition& transition)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			if (ViewPage* _page = CastInstance<ViewPage>(page.get())) {
				_page->openHome(controller, transition);
			} else {
				controller->pushPageAfterPopAllPages(page, transition);
			}
		}
	}

	void ViewPage::goToHomePage(const Ref<View>& page)
	{
		goToHomePage(page, Transition());
	}

	TransitionType ViewPage::getGlobalOpeningTransitionType()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPushTransitionType();
		}
		return TransitionType::Default;
	}

	void ViewPage::setGlobalOpeningTransitionType(TransitionType type)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPushTransitionType(type);
		}
	}

	TransitionType ViewPage::getGlobalClosingTransitionType()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPopTransitionType();
		}
		return TransitionType::Default;
	}

	void ViewPage::setGlobalClosingTransitionType(TransitionType type)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPopTransitionType(type);
		}
	}

	void ViewPage::setGlobalTransitionType(TransitionType type)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setTransitionType(type);
		}
	}

	TransitionDirection ViewPage::getGlobalOpeningTransitionDirection()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPushTransitionDirection();
		}
		return TransitionDirection::Default;
	}

	void ViewPage::setGlobalOpeningTransitionDirection(TransitionDirection direction)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPushTransitionDirection(direction);
		}
	}

	TransitionDirection ViewPage::getGlobalClosingTransitionDirection()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPopTransitionDirection();
		}
		return TransitionDirection::Default;
	}

	void ViewPage::setGlobalClosingTransitionDirection(TransitionDirection direction)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPopTransitionDirection(direction);
		}
	}

	void ViewPage::setGlobalTransitionDirection(TransitionDirection direction)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setTransitionDirection(direction);
		}
	}

	float ViewPage::getGlobalOpeningTransitionDuration()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPushTransitionDuration();
		}
		return 0;
	}

	void ViewPage::setGlobalOpeningTransitionDuration(float duration)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPushTransitionDuration(duration);
		}
	}

	float ViewPage::getGlobalClosingTransitionDuration()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPopTransitionDuration();
		}
		return 0;
	}

	void ViewPage::setGlobalClosingTransitionDuration(float duration)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPopTransitionDuration(duration);
		}
	}

	void ViewPage::setGlobalTransitionDuration(float duration)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setTransitionDuration(duration);
		}
	}

	AnimationCurve ViewPage::getGlobalOpeningTransitionCurve()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPushTransitionCurve();
		}
		return AnimationCurve::Default;
	}

	void ViewPage::setGlobalOpeningTransitionCurve(AnimationCurve curve)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPushTransitionCurve(curve);
		}
	}

	AnimationCurve ViewPage::getGlobalClosingTransitionCurve()
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			return controller->getPopTransitionCurve();
		}
		return AnimationCurve::Default;
	}

	void ViewPage::setGlobalClosingTransitionCurve(AnimationCurve curve)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setPopTransitionCurve(curve);
		}
	}

	void ViewPage::setGlobalTransitionCurve(AnimationCurve curve)
	{
		Ref<ViewPageNavigationController> controller = m_navigationController;
		if (controller.isNotNull()) {
			controller->setTransitionCurve(curve);
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
			WeakRef<ViewPage> page = this;
			back->setOnClickEvent([page](View* view, UIEvent* ev) {
				if (view->getChildAt(ev->getPoint()).isNotNull()) {
					return;
				}
				Ref<ViewPage> _page = page;
				if (_page.isNotNull()) {
					_page->dispatchClickBackground(ev);
				}
			});
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
		
		parent->addChild(viewAdd);
		
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
			ViewPageNavigationController::_runAnimationProc(this, [animation]() {
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
			dispatchBack(ev);
			if (ev->isPreventedDefault()) {
				return;
			}
			m_popupState = PopupState::None;
			lock.unlock();
			dispatchPause();
			dispatchClose();
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

	void ViewPage::setCloseOnClickBackground()
	{
		setOnClickBackground([](ViewPage* page, UIEvent*) {
			page->close();
		});
	}
	
	TransitionType _g_viewPage_globalOpeningPopupTransitionType = TransitionType::Zoom;
	TransitionDirection _g_viewPage_globalOpeningPopupTransitionDirection = TransitionDirection::FromBottomToTop;
	float _g_viewPage_globalOpeningPopupTransitionDuration = 0.25f;
	AnimationCurve _g_viewPage_globalOpeningPopupTransitionCurve = AnimationCurve::Overshoot;
	TransitionType _g_viewPage_globalClosingPopupTransitionType = TransitionType::Fade;
	TransitionDirection _g_viewPage_globalClosingPopupTransitionDirection = TransitionDirection::FromTopToBottom;
	float _g_viewPage_globalClosingPopupTransitionDuration = 0.2f;
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

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, Open)

	void ViewPage::dispatchOpen()
	{
		SLIB_INVOKE_EVENT_HANDLER(Open)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ViewPage, Close)
	
	void ViewPage::dispatchClose()
	{
		SLIB_INVOKE_EVENT_HANDLER(Close)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ViewPage, Resume)
	
	void ViewPage::dispatchResume()
	{
		SLIB_INVOKE_EVENT_HANDLER(Resume)
	}

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, Pause)
	
	void ViewPage::dispatchPause()
	{
		SLIB_INVOKE_EVENT_HANDLER(Pause)
	}

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, PageAction, ViewPageNavigationController* controller, UIPageAction action)
	
	void ViewPage::dispatchPageAction(ViewPageNavigationController* controller, UIPageAction action)
	{
		m_navigationController = controller;
		
		SLIB_INVOKE_EVENT_HANDLER(PageAction, controller, action)
		
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

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, EndPageAnimation, ViewPageNavigationController* controller, UIPageAction action)

	void ViewPage::dispatchEndPageAnimation(ViewPageNavigationController* controller, UIPageAction action)
	{
		m_navigationController = controller;
		
		SLIB_INVOKE_EVENT_HANDLER(EndPageAnimation, controller, action)
	}

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, BackPressed, UIEvent* ev)

	void ViewPage::dispatchBackPressed(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(BackPressed, ev)
		
		dispatchBack(ev);
	}

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, Back, UIEvent* ev)

	void ViewPage::dispatchBack(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(Back, ev)
	}

	SLIB_DEFINE_EVENT_HANDLER(ViewPage, ClickBackground, UIEvent* ev)
	
	void ViewPage::dispatchClickBackground(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(ClickBackground, ev)
	}

	void ViewPage::dispatchOK(UIEvent* ev)
	{
		ViewGroup::dispatchOK(ev);
	}

	void ViewPage::dispatchCancel(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(Cancel, ev)

		dispatchBack(ev);
		
		if (ev->isPreventedDefault()) {
			return;
		}
		
		close();
	}

}
