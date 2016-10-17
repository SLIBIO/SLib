#ifndef CHECKHEADER_SLIB_UI_MOBILE_APP
#define CHECKHEADER_SLIB_UI_MOBILE_APP

#include "definition.h"

#include "app.h"
#include "event.h"
#include "window.h"
#include "view_page.h"

SLIB_UI_NAMESPACE_BEGIN

class MobileMainWindow;

class SLIB_EXPORT MobileApp : public UIApp
{
	SLIB_DECLARE_OBJECT
	
public:
	MobileApp();
	
public:
	static Ref<MobileApp> getApp();
	
	Ref<MobileMainWindow> getMainWindow();
	
	static sl_bool isPaused();
	
	void addView(const Ref<View>& view);
	
	void removeView(const Ref<View>& view);
	
	Ref<ViewStack> getPageStack();
	
	void openPage(const Ref<View>& page, const Transition& transition);
	
	void openPage(const Ref<View>& page);
	
	void openHomePage(const Ref<View>& page, const Transition& transition);

	void openHomePage(const Ref<View>& page);
	
	void closePage(const Ref<View>& page, const Transition& transition);
	
	void closePage(const Ref<View>& page);
	
	void closePage(const Transition& transition);
	
	void closePage();
	
protected:
	virtual void onPause();
	
	virtual void onResume();
	
	virtual void onBack(UIEvent* ev);
	
	virtual void onCreateActivity();
	
	virtual void onDestroyActivity();
	
	virtual void onResize(sl_ui_len width, sl_ui_len height);
	
public:
	virtual void dispatchPause();
	
	static void dispatchPauseToApp();
	
	virtual void dispatchResume();
	
	static void dispatchResumeToApp();
	
	virtual void dispatchBack(UIEvent* ev);
	
	static sl_bool dispatchBackToApp();
	
	virtual void dispatchCreateActivity();
	
	static void dispatchCreateActivityToApp();
	
	virtual void dispatchDestroyActivity();
	
	static void dispatchDestroyActivityToApp();
	
	virtual void dispatchResize(sl_ui_len width, sl_ui_len height);
	
	static void dispatchResizeToApp(sl_ui_len width, sl_ui_len height);
	
private:
	static sl_bool m_flagPaused;
	
	Ref<ViewStack> m_pageStack;
	
};

class SLIB_EXPORT MobileMainWindow : public Window
{
	SLIB_DECLARE_OBJECT
	
public:
	MobileMainWindow();
	
protected:
	// override
	void onResize(sl_ui_len width, sl_ui_len height);
	
};

class SLIB_EXPORT MobilePage : public ViewPage
{
	SLIB_DECLARE_OBJECT
	
public:
	MobilePage();
	
public:
	void openPage(const Transition& transition);
	
	void openPage();
	
	void openHomePage(const Transition& transition);
	
	void openHomePage();
	
	void closePage(const Transition& transition);
	
	void closePage();

public:
	static TransitionType getGlobalOpeningTransitionType();
	
	static void setGlobalOpeningTransitionType(TransitionType type);
	
	static TransitionDirection getGlobalOpeningTransitionDirection();
	
	static void setGlobalOpeningTransitionDirection(TransitionDirection direction);
	
	static TransitionType getGlobalClosingTransitionType();
	
	static void setGlobalClosingTransitionType(TransitionType type);
	
	static TransitionDirection getGlobalClosingTransitionDirection();
	
	static void setGlobalClosingTransitionDirection(TransitionDirection direction);
	
	static void setGlobalTransitionType(TransitionType type);
	
	static void setGlobalTransitionDirection(TransitionDirection direction);
	
	static float getGlobalTransitionDuration();
	
	static void setGlobalTransitionDuration(float duration);
	
	static AnimationCurve getGlobalTransitionCurve();
	
	static void setGlobalTransitionCurve(AnimationCurve curve);

	
};

SLIB_UI_NAMESPACE_END

#endif
