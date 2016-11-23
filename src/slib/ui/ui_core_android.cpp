#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/mobile_app.h"

#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/log.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidPoint, "android/graphics/Point")
	SLIB_JNI_INT_FIELD(x);
	SLIB_JNI_INT_FIELD(y);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_AndroidUtil, "slib/platform/android/ui/Util")
	SLIB_JNI_STATIC_METHOD(getDefaultDisplay, "getDefaultDisplay", "(Lslib/platform/android/SlibActivity;)Landroid/view/Display;");
	SLIB_JNI_STATIC_METHOD(getDisplaySize, "getDisplaySize", "(Landroid/view/Display;)Landroid/graphics/Point;");
SLIB_JNI_END_CLASS

void _AndroidUiThread_runDispatchCallback(JNIEnv* env, jobject _this);

SLIB_JNI_BEGIN_CLASS(_AndroidUiThread, "slib/platform/android/ui/UiThread")
	SLIB_JNI_STATIC_METHOD(isUiThread, "isUiThread", "()Z");
	SLIB_JNI_STATIC_METHOD(dispatch, "dispatch", "()V");
	SLIB_JNI_STATIC_METHOD(runLoop, "runLoop", "()V");
	SLIB_JNI_STATIC_METHOD(quitLoop, "quitLoop", "()V");

	SLIB_JNI_NATIVE(nativeDispatchCallback, "nativeDispatchCallback", "()V", _AndroidUiThread_runDispatchCallback);
SLIB_JNI_END_CLASS

void _Android_onCreateActivity(JNIEnv* env, jobject _this, jobject activity);
void _Android_onDestroyActivity(JNIEnv* env, jobject _this, jobject activity);
void _Android_onResumeActivity(JNIEnv* env, jobject _this, jobject activity);
void _Android_onPauseActivity(JNIEnv* env, jobject _this, jobject activity);
jboolean _Android_onBack(JNIEnv* env, jobject _this, jobject activity);

SLIB_JNI_BEGIN_CLASS(_Android, "slib/platform/android/Android")
	SLIB_JNI_NATIVE(onCreateActivity, "nativeOnCreateActivity", "(Landroid/app/Activity;)V", _Android_onCreateActivity);
	SLIB_JNI_NATIVE(onDestroyActivity, "nativeOnDestroyActivity", "(Landroid/app/Activity;)V", _Android_onDestroyActivity);
	SLIB_JNI_NATIVE(onResumeActivity, "nativeOnResumeActivity", "(Landroid/app/Activity;)V", _Android_onResumeActivity);
	SLIB_JNI_NATIVE(onPauseActivity, "nativeOnPauseActivity", "(Landroid/app/Activity;)V", _Android_onPauseActivity);
	SLIB_JNI_NATIVE(onBack, "nativeOnBack", "(Landroid/app/Activity;)Z", _Android_onBack);
SLIB_JNI_END_CLASS

class _Android_Screen : public Screen
{
public:
	JniGlobal<jobject> m_display;
	int m_width;
	int m_height;

public:
	static Ref<_Android_Screen> create(jobject display)
	{
		Ref<_Android_Screen> ret;
		if (display) {
			JniLocal<jobject> size = _AndroidUtil::getDisplaySize.callObject(sl_null, display);
			if (size.isNotNull()) {
				ret = new _Android_Screen();
				if (ret.isNotNull()) {
					ret->m_display = display;
					ret->m_width = _JAndroidPoint::x.get(size);
					ret->m_height = _JAndroidPoint::y.get(size);
				}
			}
		}
		return ret;
	}

public:
    // override
	UIRect getRegion()
	{
		UIRect ret;
		ret.left = 0;
		ret.top = 0;
		ret.right = (sl_ui_pos)m_width;
		ret.bottom = (sl_ui_pos)m_height;
		return ret;
	}
};

Ref<Screen> UI::getPrimaryScreen()
{
	SLIB_STATIC_ZERO_INITIALIZED(SafeRef<Screen>, ret)
	if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
		return Ref<Screen>::null();
	}
	if (ret.isNull()) {
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jobject> display = _AndroidUtil::getDefaultDisplay.callObject(sl_null, jactivity);
			ret = _Android_Screen::create(display);
		}
	}
	return ret;
}

Ref<Screen> UI::getFocusedScreen()
{
	return UI::getPrimaryScreen();
}

List< Ref<Screen> > UI::getScreens()
{
	List< Ref<Screen> > ret;
	Ref<Screen> screen = UI::getPrimaryScreen();
	if (screen.isNotNull()) {
		ret.add(screen);
	}
	return ret;
}

sl_bool UI::isUiThread()
{
	return _AndroidUiThread::isUiThread.callBoolean(sl_null) != 0;
}

SLIB_SAFE_STATIC_GETTER(Queue<Callback>, _AndroidUi_getDispatchQueue);

void UI::dispatchToUiThread(const Callback& callback)
{
	if (callback.isNotNull()) {
		Queue<Callback>* queue = _AndroidUi_getDispatchQueue();
		if (queue) {
			queue->push(callback);
			_AndroidUiThread::dispatch.call(sl_null);
		}
	}
}

void _AndroidUiThread_runDispatchCallback(JNIEnv* env, jobject _this)
{
	Queue<Callback>* queue = _AndroidUi_getDispatchQueue();
	if (queue) {
		Callback callback;
		while (queue->pop(&callback)) {
			callback();
		}
	}
}

void UIPlatform::runLoop(sl_uint32 level)
{
	_AndroidUiThread::runLoop.call(sl_null);
}

void UIPlatform::quitLoop()
{
	_AndroidUiThread::quitLoop.call(sl_null);
}

static Ref<UIApp> _g_mobile_app;
void UIPlatform::runApp()
{
	_g_mobile_app = UIApp::getApp();
}

void UIPlatform::quitApp()
{
}

void _Android_onCreateActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Created");
	Android::setCurrentActivity(activity);
	Ref<UIApp> app = UIApp::getApp();
	if (app.isNotNull()) {
		static sl_bool flagStartApp = sl_false;
		if (! flagStartApp) {
			flagStartApp = sl_true;
			UIApp::dispatchStartToApp();
		}
		MobileApp::dispatchCreateActivityToApp();
	}
}

void _Android_onDestroyActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Destroyed");
	MobileApp::dispatchDestroyActivityToApp();
}

void _Android_onResumeActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Resumed");
	Android::setCurrentActivity(activity);
	MobileApp::dispatchResumeToApp();
}

void _Android_onPauseActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Paused");
	MobileApp::dispatchPauseToApp();
}

jboolean _Android_onBack(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "BackPressed");
	return (jboolean)(MobileApp::dispatchBackToApp());
}

SLIB_UI_NAMESPACE_END

#endif
