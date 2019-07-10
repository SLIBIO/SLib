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

#if defined(SLIB_UI_IS_EFL)

#include "ui_core_common.h"

#include "slib/ui/mobile_app.h"
#include "slib/ui/screen.h"
#include "slib/ui/window.h"
#include "slib/ui/platform.h"

#include "slib/core/log.h"
#include "slib/core/safe_static.h"
#include "slib/core/scoped.h"

#include "slib/core/platform_tizen.h"

#include <app.h>
#include <Ecore.h>
#include <Elementary.h>
#include <pthread.h>

#if defined(SLIB_PLATFORM_IS_TIZEN)
#	include <system_info.h>
#endif

namespace slib
{

	namespace priv
	{
		namespace ui_core
		{

			pthread_t g_threadMain = 0;
			sl_int32 g_nLevelMainLoop = 0;

			class ScreenImpl : public Screen
			{
			public:
				int m_width;
				int m_height;

			public:
				static Ref<ScreenImpl> create()
				{
					Ref<ScreenImpl> ret = new ScreenImpl();
					if (ret.isNotNull()) {
						ret->m_width = 0;
						ret->m_height = 0;
#if defined(SLIB_PLATFORM_IS_TIZEN)
						::system_info_get_platform_int("http://tizen.org/feature/screen.width", &(ret->m_width));
						::system_info_get_platform_int("http://tizen.org/feature/screen.height", &(ret->m_height));
#endif
						return ret;
					}			
					return sl_null;
				}

			public:
				UIRect getRegion() override
				{
					int rotation = 0;

					Evas_Object* win = UIPlatform::getMainWindow();
					if (win) {
						rotation = ::elm_win_rotation_get(win);
					}

					List<ScreenOrientation> orientations = UI::getAvailableScreenOrientations();
					if(orientations.getCount() > 0) {
						if(orientations.indexOf((ScreenOrientation)rotation) == -1) {
							rotation = (int)(orientations.getValueAt(0));
						}
					}

					UIRect ret;
					ret.left = 0;
					ret.top = 0;
					if (rotation == 90 || rotation == 270) {
						ret.right = (sl_ui_pos)m_height;
						ret.bottom = (sl_ui_pos)m_width;
					} else {
						ret.right = (sl_ui_pos)m_width;
						ret.bottom = (sl_ui_pos)m_height;
					}
					return ret;
				}
			};

			static void DispatchCallback(void* data)
			{
				UIDispatcher::processCallbacks();
			}

			static Eina_Bool DelayedDispatchCallback(void* data)
			{
				Callable<void()>* callable = reinterpret_cast<Callable<void()>*>(data);
				callable->invoke();
				callable->decreaseReference();
				return ECORE_CALLBACK_CANCEL;
			}

			static void QuitCallback(void* data)
			{
			}

			static bool CreateCallback(void* data)
			{
				Log("App", "Create");
				::elm_config_accel_preference_set("opengl");
				UIApp::dispatchStartToApp();
				MobileApp::dispatchCreateActivityToApp();
				return true;
			}

			static void ResumeCallback(void* data)
			{
				Log("App", "Resume");
				MobileApp::dispatchResumeToApp();
			}

			static void PauseCallback(void* data)
			{
				Log("App", "Pause");
				MobileApp::dispatchPauseToApp();
			}

			static void TerminateCallback(void* data)
			{
				Log("App", "Terminate");
				MobileApp::dispatchDestroyActivityToApp();
				UIApp::dispatchExitToApp();
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
			ret = ScreenImpl::create();
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

	void UI::openUrl(const String& url)
	{
		app_control_h app_control;
		if (0 == ::app_control_create(&app_control)) {
			::app_control_set_operation(app_control, APP_CONTROL_OPERATION_DEFAULT);
			::app_control_set_app_id(app_control, "com.samsung.browser");
			::app_control_set_uri(app_control, url.getData());
			::app_control_send_launch_request(app_control, NULL, NULL);
			::app_control_destroy(app_control);
		}
	}

	sl_bool UI::isUiThread()
	{
		return g_threadMain == ::pthread_self();
	}
	
	void UI::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (callback.isNull()) {
			return;
		}
		if (delayMillis == 0) {
			if (UIDispatcher::addCallback(callback)) {
				::ecore_main_loop_thread_safe_call_async(DispatchCallback, sl_null);
			}
		} else {
			Callable<void()>* callable = callback.ref.get();
			callable->increaseReference();
			::ecore_timer_loop_add((double)delayMillis / 1000.0, DelayedDispatchCallback, callable);
		}
	}

	void UIPlatform::runLoop(sl_uint32 level)
	{
		sl_int32 loopLevel = Base::interlockedIncrement32(&g_nLevelMainLoop);
		while (loopLevel == g_nLevelMainLoop) {
			::ecore_main_loop_iterate();
		}
	}

	void UIPlatform::quitLoop()
	{
		Base::interlockedDecrement(&g_nLevelMainLoop);
		::ecore_main_loop_thread_safe_call_async(QuitCallback, sl_null);
	}

	void UIPlatform::runApp()
	{
		g_threadMain = ::pthread_self();

		Ref<Application> app = Application::getApp();
		if (app.isNull()) {
			return;
		}

		ListLocker<String> args(app->getArguments());
		sl_uint32 n = (sl_uint32)(args.count);
		SLIB_SCOPED_BUFFER(char*, 64, p, n)
		for (sl_uint32 i = 0; i < n; i++) {
			p[i] = args[i].getData();
		}

		ui_app_lifecycle_callback_s event_callback = {0,};
		event_callback.create = CreateCallback;
		event_callback.resume = ResumeCallback;
		event_callback.pause = PauseCallback;
		event_callback.terminate = TerminateCallback;

		::ui_app_main(n, p, &event_callback, sl_null);

	}

	void UIPlatform::quitApp()
	{
		::ui_app_exit();
	}

}

#endif
