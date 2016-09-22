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
	
protected:
	virtual void onPause();
	
	virtual void onResume();
	
	virtual void onBack(UIEvent* ev);
	
	virtual void onCreateActivity();
	
	virtual void onDestroyActivity();
	
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
	
private:
	static sl_bool m_flagPaused;
	
	
};

class SLIB_EXPORT MobileMainWindow : public Window
{
	SLIB_DECLARE_OBJECT
	
public:
	MobileMainWindow();
	
};

class SLIB_EXPORT MobilePage : public ViewPage
{
	
};

SLIB_UI_NAMESPACE_END

#endif
