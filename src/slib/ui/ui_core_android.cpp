#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/log.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidPoint, "android/graphics/Point")
	SLIB_JNI_INT_FIELD(x);
	SLIB_JNI_INT_FIELD(y);
SLIB_JNI_END_CLASS

void _AndroidUi_runDispatchCallback(JNIEnv* env, jobject _this);
void _AndroidUi_runShowAlertResult(JNIEnv* env, jobject _this, jlong _alert, int result);

SLIB_JNI_BEGIN_CLASS(_AndroidUtil, "slib/platform/android/ui/Util")
	SLIB_JNI_STATIC_METHOD(getDefaultDisplay, "getDefaultDisplay", "(Lslib/platform/android/SlibActivity;)Landroid/view/Display;");
	SLIB_JNI_STATIC_METHOD(getDisplaySize, "getDisplaySize", "(Landroid/view/Display;)Landroid/graphics/Point;");
	SLIB_JNI_STATIC_METHOD(isUiThread, "isUiThread", "()Z");
	SLIB_JNI_STATIC_METHOD(dispatch, "dispatch", "(Lslib/platform/android/SlibActivity;)V");
	SLIB_JNI_STATIC_METHOD(showAlert, "showAlert", "(Lslib/platform/android/SlibActivity;Lslib/platform/android/ui/Alert;)V");

	SLIB_JNI_NATIVE(nativeDispatchCallback, "nativeDispatchCallback", "()V", _AndroidUi_runDispatchCallback);
	SLIB_JNI_NATIVE(nativeShowAlertResult, "nativeShowAlertResult", "(JI)V", _AndroidUi_runShowAlertResult);
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
	sl_int32 m_width;
	sl_int32 m_height;

private:
	_Android_Screen()
	{
	}

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

	Rectangle getRegion()
	{
		Rectangle ret;
		ret.left = 0;
		ret.top = 0;
		ret.right = (sl_real)m_width;
		ret.bottom = (sl_real)m_height;
		return ret;
	}
};

Ref<Screen> UI::getPrimaryScreen()
{
	SLIB_SAFE_STATIC_REF(SafeRef<Screen>, ret);
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

sl_bool UI::isUIThread()
{
	return _AndroidUtil::isUiThread.callBoolean(sl_null) != 0;
}

SLIB_SAFE_STATIC_GETTER(Queue< Ref<Runnable> >, _AndroidUi_getDispatchQueue);

void UI::runOnUIThread(const Ref<Runnable>& callback)
{
	jobject jactivity = Android::getCurrentActivity();
	if (jactivity) {
		if (callback.isNotNull()) {
			Queue< Ref<Runnable> >& queue = _AndroidUi_getDispatchQueue();
			queue.push(callback);
			_AndroidUtil::dispatch.call(sl_null, jactivity);
		}
	}
}
void _AndroidUi_runDispatchCallback(JNIEnv* env, jobject _this)
{
	Queue< Ref<Runnable> >& queue = _AndroidUi_getDispatchQueue();
	Ref<Runnable> callback;
	while (queue.pop(&callback)) {
		if (callback.isNotNull()) {
			callback->run();
		}
	}
}

void UI::runLoop()
{
}

void UI::quitLoop()
{
}


SLIB_JNI_BEGIN_CLASS(_JAndroidAlert, "slib/platform/android/ui/Alert")
	SLIB_JNI_NEW(init, "()V");
	SLIB_JNI_INT_FIELD(type);
	SLIB_JNI_STRING_FIELD(text);
	SLIB_JNI_STRING_FIELD(caption);
	SLIB_JNI_STRING_FIELD(titleOk);
	SLIB_JNI_STRING_FIELD(titleCancel);
	SLIB_JNI_STRING_FIELD(titleYes);
	SLIB_JNI_STRING_FIELD(titleNo);
	SLIB_JNI_LONG_FIELD(nativeObject);
SLIB_JNI_END_CLASS

class _UiAlertResult : public Referable
{
public:
	Ref<Runnable> onOk;
	Ref<Runnable> onCancel;
	Ref<Runnable> onYes;
	Ref<Runnable> onNo;
};

typedef Map<jlong, Ref<_UiAlertResult> > _UiAlertMap;
SLIB_SAFE_STATIC_GETTER(_UiAlertMap, _AndroidUi_alerts);

void UI::showAlert(const AlertParam& param)
{
	jobject jactivity = Android::getCurrentActivity();
	if (jactivity) {
		Ref<_UiAlertResult> result = new _UiAlertResult();
		if (result.isNotNull()) {
			result->onOk = param.onOk;
			result->onCancel = param.onCancel;
			result->onYes = param.onYes;
			result->onNo = param.onNo;

			JniLocal<jobject> jparam = _JAndroidAlert::init.newObject(sl_null);
			if (jparam.isNotNull()) {
				_JAndroidAlert::type.set(jparam, param.type);
				_JAndroidAlert::caption.set(jparam, param.caption);
				_JAndroidAlert::text.set(jparam, param.text);
				jlong lresult = (jlong)(result.get());
				_JAndroidAlert::nativeObject.set(jparam, lresult);
				_JAndroidAlert::titleOk.set(jparam, param.titleOk);
				_JAndroidAlert::titleCancel.set(jparam, param.titleCancel);
				_JAndroidAlert::titleYes.set(jparam, param.titleYes);
				_JAndroidAlert::titleNo.set(jparam, param.titleNo);
				_AndroidUi_alerts().put(lresult, result);
				_AndroidUtil::showAlert.call(sl_null, jactivity, jparam.value);
			}
		}
	}
}

void _AndroidUi_runShowAlertResult(JNIEnv* env, jobject _this, jlong _alert, int result)
{
	Ref<_UiAlertResult> alert;
	_AndroidUi_alerts().get(_alert, &alert);
	if (alert.isNotNull()) {
		_AndroidUi_alerts().remove(_alert);
		switch (result) {
		case 0: // OK
			if (alert->onOk.isNotNull()) {
				alert->onOk->run();
			}
			break;
		case 1: // Cancel
			if (alert->onCancel.isNotNull()) {
				alert->onCancel->run();
			}
			break;
		case 2: // Yes
			if (alert->onYes.isNotNull()) {
				alert->onYes->run();
			}
			break;
		case 3: // No
			if (alert->onNo.isNotNull()) {
				alert->onNo->run();
			}
			break;
		}
	}
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
			UIApp::dispatchStart();
		}
		UIApp::dispatchMobileCreate();
	}
}

void _Android_onDestroyActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Destroyed");
	UIApp::dispatchMobileDestroy();
}

void _Android_onResumeActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Resumed");
	Android::setCurrentActivity(activity);
	UIApp::dispatchMobileResume();
}

void _Android_onPauseActivity(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "Paused");
	UIApp::dispatchMobilePause();
}

jboolean _Android_onBack(JNIEnv* env, jobject _this, jobject activity)
{
	SLIB_LOG("Activity", "BackPressed");
	return (jboolean)(UIApp::dispatchMobileBack());
}

SLIB_UI_NAMESPACE_END

#endif
