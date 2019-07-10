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

#if defined(SLIB_UI_IS_GTK)

#include "ui_core_common.h"

#include "slib/ui/platform.h"
#include "slib/ui/app.h"
#include "slib/ui/screen.h"

#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace ui_core
		{

			pthread_t g_threadMain = 0;

			class ScreenImpl : public Screen
			{
			public:
				GdkScreen* m_screen;
				UIRect m_region;
				
			public:
				ScreenImpl()
				{
					m_screen = sl_null;
				}

				~ScreenImpl()
				{
					_release();
				}

			public:
				static Ref<ScreenImpl> create(GdkScreen* screen)
				{
					if (screen) {
						Ref<ScreenImpl> ret = new ScreenImpl();
						if (ret.isNotNull()) {
							g_object_ref_sink(screen);
							ret->m_screen = screen;
							UIRect region;
							region.left = 0;
							region.top = 0;
							region.right = gdk_screen_get_width(screen);
							region.bottom = gdk_screen_get_height(screen);
							ret->m_region = region;
							return ret;
						}
					}
					return sl_null;
				}
				
			public:
				void _release()
				{
					GdkScreen* screen = m_screen;
					if (screen) {
						m_screen = sl_null;
						g_object_unref(screen);
					}
				}

				UIRect getRegion() override
				{
					return m_region;
				}
				
			};
			
			static gboolean DispatchCallback(gpointer user_data)
			{
				UIDispatcher::processCallbacks();
				return sl_false;
			}

			static gboolean DelayedDispatchCallback(gpointer user_data)
			{
				Callable<void()>* callable = reinterpret_cast<Callable<void()>*>(user_data);
				callable->invoke();
				callable->decreaseReference();
				return sl_false;
			}

		}
	}

	using namespace priv::ui_core;
	
	sl_bool UIPlatform::initializeGtk()
	{
		return gtk_init_check(NULL, NULL);
	}
	
	Ref<Screen> UI::getPrimaryScreen()
	{
		SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Screen>, ret)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		if (ret.isNull()) {
			ret = ScreenImpl::create(gdk_screen_get_default());
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

	Ref<Screen> UIPlatform::createScreen(GdkScreen* handle)
	{
		return ScreenImpl::create(handle);
	}
	
	GdkScreen* UIPlatform::getScreenHandle(Screen* _screen)
	{
		ScreenImpl* screen = (ScreenImpl*)_screen;
		if (screen) {
			return screen->m_screen;
		}
		return sl_null;
	}

	void UI::openUrl(const String& url)
	{
		GError* error = NULL;
		gtk_show_uri(NULL, url.getData(), GDK_CURRENT_TIME, &error);
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
				g_idle_add(DispatchCallback, sl_null);
			}
		} else {
			Callable<void()>* callable = callback.ref.get();
			callable->increaseReference();
			g_timeout_add(delayMillis, DelayedDispatchCallback, callable);
		}
	}

	void UIPlatform::runLoop(sl_uint32 level)
	{
		gtk_main();
	}

	void UIPlatform::quitLoop()
	{
		gtk_main_quit();
	}

	void UIPlatform::runApp()
	{
		g_threadMain = ::pthread_self();

		UIPlatform::initializeGtk();
		
		UIApp::dispatchStartToApp();

		gtk_main();
		
		UIApp::dispatchExitToApp();
	}

	void UIPlatform::quitApp()
	{
		gtk_main_quit();
	}

	void UIPlatform::getGdkColor(const Color& color, GdkColor* _out)
	{
		_out->red = (guint16)(color.r) * 257;
		_out->green = (guint16)(color.g) * 257;
		_out->blue = (guint16)(color.b) * 257;
	}

	void UIPlatform::getScreenLocationOfWidget(GtkWidget* widget, sl_ui_len* out_x, sl_ui_len* out_y)
	{
		sl_ui_len x = 0;
		sl_ui_len y = 0;
		GdkWindow* window = widget->window;
		if (window) {
			gint ox = 0;
			gint oy = 0;
			gdk_window_get_origin(window, &ox, &oy);
			GtkAllocation allocation = widget->allocation;
			x = ox + allocation.x;
			y = oy + allocation.y;
		}
		if (out_x) {
			*out_x = x;
		}
		if (out_y) {
			*out_y = y;
		}
	}
	
}

#endif
