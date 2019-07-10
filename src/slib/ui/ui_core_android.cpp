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
#include "slib/ui/resource.h"

#include "slib/core/locale.h"
#include "slib/core/io.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

#include "ui_core_common.h"

namespace slib
{

	namespace priv
	{
		namespace ui_core
		{
			
			Ref<UIApp> g_app;
			
			SLIB_JNI_BEGIN_CLASS(JAndroidPoint, "android/graphics/Point")
				SLIB_JNI_INT_FIELD(x);
				SLIB_JNI_INT_FIELD(y);
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JAndroidRect, "android/graphics/Rect")
				SLIB_JNI_INT_FIELD(left);
				SLIB_JNI_INT_FIELD(top);
				SLIB_JNI_INT_FIELD(right);
				SLIB_JNI_INT_FIELD(bottom);
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JAndroidUtil, "slib/platform/android/ui/Util")
				SLIB_JNI_STATIC_METHOD(getDefaultDisplay, "getDefaultDisplay", "(Landroid/app/Activity;)Landroid/view/Display;");
				SLIB_JNI_STATIC_METHOD(getDisplaySize, "getDisplaySize", "(Landroid/view/Display;)Landroid/graphics/Point;");
				SLIB_JNI_STATIC_METHOD(getScreenOrientation, "getScreenOrientation", "(Landroid/app/Activity;)I");
				SLIB_JNI_STATIC_METHOD(setScreenOrientations, "setScreenOrientations", "(Landroid/app/Activity;ZZZZ)V");
				SLIB_JNI_STATIC_METHOD(openURL, "openURL", "(Landroid/app/Activity;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(getSafeAreaInsets, "getSafeAreaInsets", "(Landroid/app/Activity;)Landroid/graphics/Rect;");
				SLIB_JNI_STATIC_METHOD(getStatusBarHeight, "getStatusBarHeight", "(Landroid/app/Activity;)I");
				SLIB_JNI_STATIC_METHOD(setStatusBarStyle, "setStatusBarStyle", "(Landroid/app/Activity;I)V");		
				SLIB_JNI_STATIC_METHOD(setBadgeNumber, "setBadgeNumber", "(Landroid/app/Activity;I)V");
			SLIB_JNI_END_CLASS

			void DispatchCallback(JNIEnv* env, jobject _this);
			void DispatchDelayedCallback(JNIEnv* env, jobject _this, jlong ptr);

			SLIB_JNI_BEGIN_CLASS(JAndroidUiThread, "slib/platform/android/ui/UiThread")
				SLIB_JNI_STATIC_METHOD(isUiThread, "isUiThread", "()Z");
				SLIB_JNI_STATIC_METHOD(dispatch, "dispatch", "()V");
				SLIB_JNI_STATIC_METHOD(dispatchDelayed, "dispatchDelayed", "(JI)V");
				SLIB_JNI_STATIC_METHOD(runLoop, "runLoop", "()V");
				SLIB_JNI_STATIC_METHOD(quitLoop, "quitLoop", "()V");

				SLIB_JNI_NATIVE(nativeDispatchCallback, "nativeDispatchCallback", "()V", DispatchCallback);
				SLIB_JNI_NATIVE(nativeDispatchDelayedCallback, "nativeDispatchDelayedCallback", "(J)V", DispatchDelayedCallback);
			SLIB_JNI_END_CLASS

			void OnCreateActivity(JNIEnv* env, jobject _this, jobject activity);
			void OnDestroyActivity(JNIEnv* env, jobject _this, jobject activity);
			void OnResumeActivity(JNIEnv* env, jobject _this, jobject activity);
			void OnPauseActivity(JNIEnv* env, jobject _this, jobject activity);
			jboolean OnBack(JNIEnv* env, jobject _this, jobject activity);
			void OnConfigurationChanged(JNIEnv* env, jobject _this, jobject activity);
			void OnChangeWindowInsets(JNIEnv* env, jobject _this, jobject activity);
			void OnOpenUrl(JNIEnv* env, jobject _this, jobject activity, jstring url);

			SLIB_JNI_BEGIN_CLASS(JAndroid, "slib/platform/android/Android")
				SLIB_JNI_STATIC_METHOD(setKeyboardAdjustMode, "setKeyboardAdjustMode", "(Landroid/app/Activity;I)V");

				SLIB_JNI_NATIVE(onCreateActivity, "nativeOnCreateActivity", "(Landroid/app/Activity;)V", OnCreateActivity);
				SLIB_JNI_NATIVE(onDestroyActivity, "nativeOnDestroyActivity", "(Landroid/app/Activity;)V", OnDestroyActivity);
				SLIB_JNI_NATIVE(onResumeActivity, "nativeOnResumeActivity", "(Landroid/app/Activity;)V", OnResumeActivity);
				SLIB_JNI_NATIVE(onPauseActivity, "nativeOnPauseActivity", "(Landroid/app/Activity;)V", OnPauseActivity);
				SLIB_JNI_NATIVE(onBack, "nativeOnBack", "(Landroid/app/Activity;)Z", OnBack);
				SLIB_JNI_NATIVE(onConfigurationChanged, "nativeOnConfigurationChanged", "(Landroid/app/Activity;)V", OnConfigurationChanged);
				SLIB_JNI_NATIVE(onChangeWindowInsets, "nativeOnChangeWindowInsets", "(Landroid/app/Activity;)V", OnChangeWindowInsets);
				SLIB_JNI_NATIVE(onOpenUrl, "nativeOnOpenUrl", "(Landroid/app/Activity;Ljava/lang/String;)V", OnOpenUrl);
			SLIB_JNI_END_CLASS

			class ScreenImpl : public Screen
			{
			public:
				JniGlobal<jobject> m_display;
				int m_width;
				int m_height;

			public:
				static Ref<ScreenImpl> create(jobject display)
				{
					JniLocal<jobject> size = JAndroidUtil::getDisplaySize.callObject(sl_null, display);
					if (size.isNotNull()) {
						Ref<ScreenImpl> ret = new ScreenImpl();
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

			void OnCreateActivity(JNIEnv* env, jobject _this, jobject activity)
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

			void OnDestroyActivity(JNIEnv* env, jobject _this, jobject activity)
			{
				Log("Activity", "onDestroyActivity");
				MobileApp::dispatchDestroyActivityToApp();
			}

			void OnResumeActivity(JNIEnv* env, jobject _this, jobject activity)
			{
				Log("Activity", "onResumeActivity");
				Android::setCurrentActivity(activity);
				MobileApp::dispatchResumeToApp();
			}

			void OnPauseActivity(JNIEnv* env, jobject _this, jobject activity)
			{
				Log("Activity", "onPauseActivity");
				MobileApp::dispatchPauseToApp();
			}

			jboolean OnBack(JNIEnv* env, jobject _this, jobject activity)
			{
				Log("Activity", "onBackPressed");
				return (jboolean)(MobileApp::dispatchBackPressedToApp());
			}

			void OnConfigurationChanged(JNIEnv* env, jobject _this, jobject activity)
			{
				Log("Activity", "onConfigurationChanged");
				Locale::dispatchChangeCurrentLocale();
			}

			void OnChangeWindowInsets(JNIEnv* env, jobject _this, jobject activity)
			{
				Log("Activity", "onChangeWindowInsets");
				UIResource::updateDefaultScreenSize();
			}

			void OnOpenUrl(JNIEnv* env, jobject _this, jobject activity, jstring jurl)
			{
				String url = Jni::getString(jurl);
				Log("Activity", "onOpenUrl: %s", url);
				MobileApp::dispatchOpenUrlToApp(url);
			}

			void DispatchCallback(JNIEnv* env, jobject _this)
			{
				UIDispatcher::processCallbacks();
			}

			void DispatchDelayedCallback(JNIEnv* env, jobject _this, jlong ptr)
			{
				UIDispatcher::processDelayedCallback((sl_reg)ptr);
			}

			void UpdateKeyboardAdjustMode(UIKeyboardAdjustMode mode)
			{
				jobject jactivity = Android::getCurrentActivity();
				if (jactivity) {
					JAndroid::setKeyboardAdjustMode.call(sl_null, jactivity, (jint)mode);
				}
			}

		}
	}

	using namespace priv::ui_core;

	Ref<Screen> UI::getPrimaryScreen()
	{
		SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Screen>, ret)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		if (ret.isNull()) {
			ret = ScreenImpl::create(sl_null);
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
			if (UIDispatcher::addCallback(callback)) {
				JAndroidUiThread::dispatch.call(sl_null);
			}
		} else {
			if (delayMillis > 0x7fffffff) {
				delayMillis = 0x7fffffff;
			}
			sl_reg ptr;
			if (UIDispatcher::addDelayedCallback(callback, ptr)) {
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
	
	UIEdgeInsets UI::getSafeAreaInsets()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jobject> jrect = JAndroidUtil::getSafeAreaInsets.callObject(sl_null, jactivity);
			if (jrect.isNotNull()) {
				UIEdgeInsets ret;
				ret.left = (sl_ui_len)(JAndroidRect::left.get(jrect));
				ret.top = (sl_ui_len)(JAndroidRect::top.get(jrect));
				ret.right = (sl_ui_len)(JAndroidRect::right.get(jrect));
				ret.bottom = (sl_ui_len)(JAndroidRect::bottom.get(jrect));
				return ret;
			}
		}
		UIEdgeInsets ret = {0, 0, 0, 0};
		return ret;
	}

	sl_ui_len UI::getStatusBarHeight()
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
			UIResource::updateDefaultScreenSize();
		}
	}

	void UI::setBadgeNumber(sl_uint32 number)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JAndroidUtil::setBadgeNumber.call(sl_null, jactivity, number);
		}
	}
	
	void UIPlatform::runLoop(sl_uint32 level)
	{
		JAndroidUiThread::runLoop.call(sl_null);
	}

	void UIPlatform::quitLoop()
	{
		JAndroidUiThread::quitLoop.call(sl_null);
	}

	void UIPlatform::runApp()
	{
		g_app = UIApp::getApp();
	}

	void UIPlatform::quitApp()
	{
	}

}

#endif
