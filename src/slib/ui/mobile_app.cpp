#include "../../../inc/slib/ui/mobile_app.h"
#include "../../../inc/slib/ui/view.h"

#include "../../../inc/slib/core/animation.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(MobileApp, UIApp)

MobileApp::MobileApp()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	Ref<MobileMainWindow> window = new MobileMainWindow;
	setMainWindow(window);
	
	m_pageStack = new ViewStack;
	window->addView(m_pageStack);
	
}

Ref<MobileApp> MobileApp::getApp()
{
	Ref<Application> app(Application::getApp());
	if (MobileApp::checkInstance(app.ptr)) {
		return Ref<MobileApp>::from(app);
	}
	return Ref<MobileApp>::null();
}

Ref<MobileMainWindow> MobileApp::getMainWindow()
{
	Ref<Window> window = UIApp::getMainWindow();
	if (MobileMainWindow::checkInstance(window.ptr)) {
		return Ref<MobileMainWindow>::from(window);
	}
	return Ref<MobileMainWindow>::null();
}

sl_bool MobileApp::m_flagPaused = sl_false;

sl_bool MobileApp::isPaused()
{
	return m_flagPaused;
}

void MobileApp::addView(const Ref<View>& view)
{
	void* _thiz = this;
	if (_thiz) {
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->addView(view);
		}
	}
}

void MobileApp::removeView(const Ref<View>& view)
{
	void* _thiz = this;
	if (_thiz) {
		Ref<MobileMainWindow> window = getMainWindow();
		if (window.isNotNull()) {
			window->removeView(view);
		}
	}
}

Ref<ViewStack> MobileApp::getPageStack()
{
	return m_pageStack;
}

void MobileApp::openPage(const Ref<View>& page, const Transition& transition)
{
	m_pageStack->push(page, transition);
}

void MobileApp::openPage(const Ref<View>& page)
{
	m_pageStack->push(page);
}

void MobileApp::openHomePage(const Ref<View>& page, const Transition& transition)
{
	m_pageStack->push(page, transition, sl_true);
}

void MobileApp::openHomePage(const Ref<View>& page)
{
	m_pageStack->push(page, sl_true);
}

void MobileApp::closePage(const Ref<View>& page, const Transition& transition)
{
	m_pageStack->pop(page, transition);
}

void MobileApp::closePage(const Ref<View>& page)
{
	m_pageStack->pop(page);
}

void MobileApp::closePage(const Transition& transition)
{
	m_pageStack->pop(transition);
}

void MobileApp::closePage()
{
	m_pageStack->pop();
}

void MobileApp::onPause()
{
}

void MobileApp::onResume()
{
}

void MobileApp::onBack(UIEvent* ev)
{
}

void MobileApp::onCreateActivity()
{
}

void MobileApp::onDestroyActivity()
{
}

void MobileApp::onResize(sl_ui_len width, sl_ui_len height)
{
}

void MobileApp::dispatchPause()
{
	onPause();
}

void MobileApp::dispatchPauseToApp()
{
	Animation::pauseAnimationCenter();
	m_flagPaused = sl_true;
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchPause();
	}
}

void MobileApp::dispatchResume()
{
	onResume();
}

void MobileApp::dispatchResumeToApp()
{
	Animation::resumeAnimationCenter();
	m_flagPaused = sl_false;
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchResume();
	}
}

void MobileApp::dispatchBack(UIEvent* ev)
{
	onBack(ev);
}

sl_bool MobileApp::dispatchBackToApp()
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			app->dispatchBack(ev.ptr);
			if (ev->isPreventedDefault()) {
				return sl_false;
			}
		}
	}
	return sl_true;
}

void MobileApp::dispatchCreateActivity()
{
	Ref<MobileMainWindow> window = getMainWindow();
	if (window.isNotNull()) {
		window->create();
	}
	
	onCreateActivity();
}

void MobileApp::dispatchCreateActivityToApp()
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchCreateActivity();
	}
}

void MobileApp::dispatchDestroyActivity()
{
	onDestroyActivity();
}

void MobileApp::dispatchDestroyActivityToApp()
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchDestroyActivity();
	}
}

void MobileApp::dispatchResize(sl_ui_len width, sl_ui_len height)
{
	m_pageStack->setFrame(0, 0, width, height);
	onResize(width, height);
}

void MobileApp::dispatchResizeToApp(sl_ui_len width, sl_ui_len height)
{
	Ref<MobileApp> app = getApp();
	if (app.isNotNull()) {
		app->dispatchResize(width, height);
	}
}


SLIB_DEFINE_OBJECT(MobileMainWindow, Window)

MobileMainWindow::MobileMainWindow()
{
	SLIB_REFERABLE_CONSTRUCTOR;
}

void MobileMainWindow::onResize(sl_ui_len width, sl_ui_len height)
{
	MobileApp::dispatchResizeToApp(width, height);
}


SLIB_DEFINE_OBJECT(MobilePage, ViewPage)

MobilePage::MobilePage()
{
	SLIB_REFERABLE_CONSTRUCTOR
}

void MobilePage::openPage(const Transition& transition)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		app->openPage(this, transition);
	}
}

void MobilePage::openPage()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		app->openPage(this);
	}
}

void MobilePage::openHomePage(const Transition& transition)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		app->openHomePage(this, transition);
	}
}

void MobilePage::openHomePage()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		app->openHomePage(this);
	}
}

void MobilePage::closePage(const Transition& transition)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		app->closePage(this, transition);
	}
}

void MobilePage::closePage()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		app->closePage(this);
	}
}

TransitionType MobilePage::getGlobalOpeningTransitionType()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			return stack->getPushTransitionType();
		}
	}
	return TransitionType::Default;
}

void MobilePage::setGlobalOpeningTransitionType(TransitionType type)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setPushTransitionType(type);
		}
	}
}

TransitionDirection MobilePage::getGlobalOpeningTransitionDirection()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			return stack->getPushTransitionDirection();
		}
	}
	return TransitionDirection::Default;
}

void MobilePage::setGlobalOpeningTransitionDirection(TransitionDirection direction)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setPushTransitionDirection(direction);
		}
	}
}

TransitionType MobilePage::getGlobalClosingTransitionType()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			return stack->getPopTransitionType();
		}
	}
	return TransitionType::Default;
}

void MobilePage::setGlobalClosingTransitionType(TransitionType type)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setPopTransitionType(type);
		}
	}
}

TransitionDirection MobilePage::getGlobalClosingTransitionDirection()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			return stack->getPopTransitionDirection();
		}
	}
	return TransitionDirection::Default;
}

void MobilePage::setGlobalClosingTransitionDirection(TransitionDirection direction)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setPopTransitionDirection(direction);
		}
	}
}

void MobilePage::setGlobalTransitionType(TransitionType type)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setTransitionType(type);
		}
	}
}

void MobilePage::setGlobalTransitionDirection(TransitionDirection direction)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setTransitionDirection(direction);
		}
	}
}

float MobilePage::getGlobalTransitionDuration()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			return stack->getTransitionDuration();
		}
	}
	return 0;
}

void MobilePage::setGlobalTransitionDuration(float duration)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setTransitionDuration(duration);
		}
	}
}

AnimationCurve MobilePage::getGlobalTransitionCurve()
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			return stack->getTransitionCurve();
		}
	}
	return AnimationCurve::Default;
}

void MobilePage::setGlobalTransitionCurve(AnimationCurve curve)
{
	Ref<MobileApp> app = MobileApp::getApp();
	if (app.isNotNull()) {
		Ref<ViewStack> stack = app->getPageStack();
		if (stack.isNotNull()) {
			stack->setTransitionCurve(curve);
		}
	}
}


SLIB_UI_NAMESPACE_END

