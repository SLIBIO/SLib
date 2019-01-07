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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/core.h"

#include "slib/ui/screen.h"
#include "slib/ui/platform.h"
#include "slib/ui/mobile_app.h"
#include "slib/core/locale.h"
#include "slib/core/io.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

#include "ui_core_common.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidPoint, "android/graphics/Point")
		SLIB_JNI_INT_FIELD(x);
		SLIB_JNI_INT_FIELD(y);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JAndroidUtil, "slib/platform/android/ui/Util")
		SLIB_JNI_STATIC_METHOD(getDefaultDisplay, "getDefaultDisplay", "(Landroid/app/Activity;)Landroid/view/Display;");
		SLIB_JNI_STATIC_METHOD(getDisplaySize, "getDisplaySize", "(Landroid/view/Display;)Landroid/graphics/Point;");
		SLIB_JNI_STATIC_METHOD(getScreenOrientation, "getScreenOrientation", "(Landroid/app/Activity;)I");
		SLIB_JNI_STATIC_METHOD(setScreenOrientations, "setScreenOrientations", "(Landroid/app/Activity;ZZZZ)V");
		SLIB_JNI_STATIC_METHOD(openURL, "openURL", "(Landroid/app/Activity;Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(getStatusBarHeight, "getStatusBarHeight", "(Landroid/app/Activity;)I");
		SLIB_JNI_STATIC_METHOD(setStatusBarStyle, "setStatusBarStyle", "(Landroid/app/Activity;I)V");		
		SLIB_JNI_STATIC_METHOD(setBadgeNumber, "setBadgeNumber", "(Landroid/app/Activity;I)V");
	SLIB_JNI_END_CLASS

	void _priv_AndroidUiThread_runDispatchCallback(JNIEnv* env, jobject _this);
	void _priv_AndroidUiThread_runDispatchDelayedCallback(JNIEnv* env, jobject _this, jlong ptr);

	SLIB_JNI_BEGIN_CLASS(JAndroidUiThread, "slib/platform/android/ui/UiThread")
		SLIB_JNI_STATIC_METHOD(isUiThread, "isUiThread", "()Z");
		SLIB_JNI_STATIC_METHOD(dispatch, "dispatch", "()V");
		SLIB_JNI_STATIC_METHOD(dispatchDelayed, "dispatchDelayed", "(JI)V");
		SLIB_JNI_STATIC_METHOD(runLoop, "runLoop", "()V");
		SLIB_JNI_STATIC_METHOD(quitLoop, "quitLoop", "()V");

		SLIB_JNI_NATIVE(nativeDispatchCallback, "nativeDispatchCallback", "()V", _priv_AndroidUiThread_runDispatchCallback);
		SLIB_JNI_NATIVE(nativeDispatchDelayedCallback, "nativeDispatchDelayedCallback", "(J)V", _priv_AndroidUiThread_runDispatchDelayedCallback);
	SLIB_JNI_END_CLASS

	void _priv_Android_onCreateActivity(JNIEnv* env, jobject _this, jobject activity);
	void _priv_Android_onDestroyActivity(JNIEnv* env, jobject _this, jobject activity);
	void _priv_Android_onResumeActivity(JNIEnv* env, jobject _this, jobject activity);
	void _priv_Android_onPauseActivity(JNIEnv* env, jobject _this, jobject activity);
	jboolean _priv_Android_onBack(JNIEnv* env, jobject _this, jobject activity);
	void _priv_Android_onConfigurationChanged(JNIEnv* env, jobject _this, jobject activity);

	SLIB_JNI_BEGIN_CLASS(JAndroid, "slib/platform/android/Android")
		SLIB_JNI_NATIVE(onCreateActivity, "nativeOnCreateActivity", "(Landroid/app/Activity;)V", _priv_Android_onCreateActivity);
		SLIB_JNI_NATIVE(onDestroyActivity, "nativeOnDestroyActivity", "(Landroid/app/Activity;)V", _priv_Android_onDestroyActivity);
		SLIB_JNI_NATIVE(onResumeActivity, "nativeOnResumeActivity", "(Landroid/app/Activity;)V", _priv_Android_onResumeActivity);
		SLIB_JNI_NATIVE(onPauseActivity, "nativeOnPauseActivity", "(Landroid/app/Activity;)V", _priv_Android_onPauseActivity);
		SLIB_JNI_NATIVE(onBack, "nativeOnBack", "(Landroid/app/Activity;)Z", _priv_Android_onBack);
		SLIB_JNI_NATIVE(onConfigurationChanged, "nativeOnConfigurationChanged", "(Landroid/app/Activity;)V", _priv_Android_onConfigurationChanged);
	SLIB_JNI_END_CLASS

	class _priv_Android_Screen : public Screen
	{
	public:
		JniGlobal<jobject> m_display;
		int m_width;
		int m_height;

	public:
		static Ref<_priv_Android_Screen> create(jobject display)
		{
			JniLocal<jobject> size = JAndroidUtil::getDisplaySize.callObject(sl_null, display);
			if (size.isNotNull()) {
				Ref<_priv_Android_Screen> ret = new _priv_Android_Screen();
				if (ret.isNotNull()) {
					ret->m_display = display;
					ret->m_width = JAndroidPoint::x.get(size);
					ret->m_height = JAndroidPoint::y.get(size);
					return ret;
				}
			}
			return sl_null;
		}

	public:
		UIRect getRegion() override
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
		SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Screen>, ret)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		if (ret.isNull()) {
			ret = _priv_Android_Screen::create(sl_null);
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
			ret.add_NoLock(screen);
		}
		return ret;
	}

	ScreenOrientation UI::getScreenOrientation()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return (ScreenOrientation)(JAndroidUtil::getScreenOrientation.callInt(sl_null, jactivity));
		}
		return ScreenOrientation::Portrait;
	}
	
	void UI::attemptRotateScreenOrientation()
	{
		List<ScreenOrientation> orientations(getAvailableScreenOrientations());
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			if (orientations.isEmpty()) {
				JAndroidUtil::setScreenOrientations.call(sl_null, jactivity, sl_true, sl_true, sl_true, sl_true);
			} else {
				JAndroidUtil::setScreenOrientations.call(sl_null, jactivity,
					orientations.contains(ScreenOrientation::Portrait),
					orientations.contains(ScreenOrientation::LandscapeRight),
					orientations.contains(ScreenOrientation::PortraitUpsideDown),
					orientations.contains(ScreenOrientation::LandscapeLeft)
					);
			}
		}
	}

	sl_bool UI::isUiThread()
	{
		return JAndroidUiThread::isUiThread.callBoolean(sl_null) != 0;
	}

	void UI::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (delayMillis == 0) {
			if (_priv_UIDispatcher::addCallback(callback)) {
				JAndroidUiThread::dispatch.call(sl_null);
			}
		} else {
			if (delayMillis > 0x7fffffff) {
				delayMillis = 0x7fffffff;
			}
			sl_reg ptr;
			if (_priv_UIDispatcher::addDelayedCallback(callback, ptr)) {
				JAndroidUiThread::dispatchDelayed.call(sl_null, (jlong)ptr, delayMillis);
			}
		}
	}

	void UI::openUrl(const String& _url) {
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jurl = Jni::getJniString(_url);
			JAndroidUtil::openURL.call(sl_null, jactivity, jurl.get());
		}
	}
	
	void UI::dismissKeyboard()
	{
		Android::dismissKeyboard();
	}

	sl_ui_len UI::getScreenStatusBarHeight()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidUtil::getStatusBarHeight.callInt(sl_null, jactivity);
		}
		return 0;
	}

	void UI::setStatusBarStyle(StatusBarStyle style)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JAndroidUtil::setStatusBarStyle.call(sl_null, jactivity, (int)style);
		}
	}

	void UI::setBadgeNumber(sl_uint32 number)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JAndroidUtil::setBadgeNumber.call(sl_null, jactivity, number);
		}
	}
	
	void _priv_AndroidUiThread_runDispatchCallback(JNIEnv* env, jobject _this)
	{
		_priv_UIDispatcher::processCallbacks();
	}

	void _priv_AndroidUiThread_runDispatchDelayedCallback(JNIEnv* env, jobject _this, jlong ptr)
	{
		_priv_UIDispatcher::processDelayedCallback((sl_reg)ptr);
	}

	void UIPlatform::runLoop(sl_uint32 level)
	{
		JAndroidUiThread::runLoop.call(sl_null);
	}

	void UIPlatform::quitLoop()
	{
		JAndroidUiThread::quitLoop.call(sl_null);
	}

	static Ref<UIApp> _g_mobile_app;
	void UIPlatform::runApp()
	{
		_g_mobile_app = UIApp::getApp();
	}

	void UIPlatform::quitApp()
	{
	}

	void _priv_Android_onCreateActivity(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "onCreateActivity");
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
		Locale::dispatchChangeCurrentLocale();
	}

	void _priv_Android_onDestroyActivity(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "onDestroyActivity");
		MobileApp::dispatchDestroyActivityToApp();
	}

	void _priv_Android_onResumeActivity(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "onResumeActivity");
		Android::setCurrentActivity(activity);
		MobileApp::dispatchResumeToApp();
	}

	void _priv_Android_onPauseActivity(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "onPauseActivity");
		MobileApp::dispatchPauseToApp();
	}

	jboolean _priv_Android_onBack(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "onBackPressed");
		return (jboolean)(MobileApp::dispatchBackPressedToApp());
	}

	void _priv_Android_onConfigurationChanged(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "onConfigurationChanged");
		Locale::dispatchChangeCurrentLocale();
	}

}

#endif
