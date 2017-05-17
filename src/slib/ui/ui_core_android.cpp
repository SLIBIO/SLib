/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/ui/core.h"

#include "slib/ui/screen.h"
#include "slib/ui/platform.h"
#include "slib/ui/mobile_app.h"
#include "slib/core/io.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

#include "ui_core_common.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidPoint, "android/graphics/Point")
		SLIB_JNI_INT_FIELD(x);
		SLIB_JNI_INT_FIELD(y);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(_AndroidUtil, "slib/platform/android/ui/Util")
		SLIB_JNI_STATIC_METHOD(getDefaultDisplay, "getDefaultDisplay", "(Landroid/app/Activity;)Landroid/view/Display;");
		SLIB_JNI_STATIC_METHOD(getDisplaySize, "getDisplaySize", "(Landroid/view/Display;)Landroid/graphics/Point;");
		SLIB_JNI_STATIC_METHOD(openURL, "openURL", "(Landroid/app/Activity;Ljava/lang/String;)V");
	SLIB_JNI_END_CLASS

	void _AndroidUiThread_runDispatchCallback(JNIEnv* env, jobject _this);
	void _AndroidUiThread_runDispatchDelayedCallback(JNIEnv* env, jobject _this, jlong ptr);

	SLIB_JNI_BEGIN_CLASS(_AndroidUiThread, "slib/platform/android/ui/UiThread")
		SLIB_JNI_STATIC_METHOD(isUiThread, "isUiThread", "()Z");
		SLIB_JNI_STATIC_METHOD(dispatch, "dispatch", "()V");
		SLIB_JNI_STATIC_METHOD(dispatchDelayed, "dispatchDelayed", "(JI)V");
		SLIB_JNI_STATIC_METHOD(runLoop, "runLoop", "()V");
		SLIB_JNI_STATIC_METHOD(quitLoop, "quitLoop", "()V");

		SLIB_JNI_NATIVE(nativeDispatchCallback, "nativeDispatchCallback", "()V", _AndroidUiThread_runDispatchCallback);
		SLIB_JNI_NATIVE(nativeDispatchDelayedCallback, "nativeDispatchDelayedCallback", "(J)V", _AndroidUiThread_runDispatchDelayedCallback);
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
			JniLocal<jobject> size = _AndroidUtil::getDisplaySize.callObject(sl_null, display);
			if (size.isNotNull()) {
				Ref<_Android_Screen> ret = new _Android_Screen();
				if (ret.isNotNull()) {
					ret->m_display = display;
					ret->m_width = _JAndroidPoint::x.get(size);
					ret->m_height = _JAndroidPoint::y.get(size);
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
			ret = _Android_Screen::create(sl_null);
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

	void UI::openUrl(const String& _url) {
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jurl = Jni::getJniString(_url);
			_AndroidUtil::openURL.call(sl_null, jactivity, jurl.get());
		}
	}
	
	sl_bool UI::isUiThread()
	{
		return _AndroidUiThread::isUiThread.callBoolean(sl_null) != 0;
	}

	void UI::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (delayMillis == 0) {
			if (_UIDispatcher::addCallback(callback)) {
				_AndroidUiThread::dispatch.call(sl_null);
			}
		} else {
			if (delayMillis > 0x7fffffff) {
				delayMillis = 0x7fffffff;
			}
			sl_reg ptr;
			if (_UIDispatcher::addDelayedCallback(callback, ptr)) {
				_AndroidUiThread::dispatchDelayed.call(sl_null, (jlong)ptr, delayMillis);
			}
		}
	}

	void _AndroidUiThread_runDispatchCallback(JNIEnv* env, jobject _this)
	{
		_UIDispatcher::processCallbacks();
	}

	void _AndroidUiThread_runDispatchDelayedCallback(JNIEnv* env, jobject _this, jlong ptr)
	{
		_UIDispatcher::processDelayedCallback((sl_reg)ptr);
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
		Log("Activity", "Created");
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
		Log("Activity", "Destroyed");
		MobileApp::dispatchDestroyActivityToApp();
	}

	void _Android_onResumeActivity(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "Resumed");
		Android::setCurrentActivity(activity);
		MobileApp::dispatchResumeToApp();
	}

	void _Android_onPauseActivity(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "Paused");
		MobileApp::dispatchPauseToApp();
	}

	jboolean _Android_onBack(JNIEnv* env, jobject _this, jobject activity)
	{
		Log("Activity", "BackPressed");
		return (jboolean)(MobileApp::dispatchBackToApp());
	}

}

#endif
