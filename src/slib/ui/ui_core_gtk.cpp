/*
 *  Copyright (c) 2008-2018 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	
	pthread_t _g_main_thread = 0;

	sl_bool UIPlatform::initializeGtk()
	{
		return gtk_init_check(NULL, NULL);
	}
	
	class _priv_GTK_Screen : public Screen
	{
	public:
		GdkScreen* m_screen;
		UIRect m_region;
		
	public:
		_priv_GTK_Screen()
		{
			m_screen = sl_null;
		}

		~_priv_GTK_Screen()
		{
			_release();
		}

	public:
		static Ref<_priv_GTK_Screen> create(GdkScreen* screen)
		{
			if (screen) {
				Ref<_priv_GTK_Screen> ret = new _priv_GTK_Screen();
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
	
	Ref<Screen> UI::getPrimaryScreen()
	{
		SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Screen>, ret)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		if (ret.isNull()) {
			ret = _priv_GTK_Screen::create(gdk_screen_get_default());
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
		return _priv_GTK_Screen::create(handle);
	}
	
	GdkScreen* UIPlatform::getScreenHandle(Screen* _screen)
	{
		_priv_GTK_Screen* screen = (_priv_GTK_Screen*)_screen;
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
		return _g_main_thread == ::pthread_self();
	}
	
	static gboolean _ui_dispatch_callback(gpointer user_data)
	{
		_priv_UIDispatcher::processCallbacks();
		return sl_false;
	}

	static gboolean _ui_dispatch_timer_callback(gpointer user_data)
	{
		Callable<void()>* callable = reinterpret_cast<Callable<void()>*>(user_data);
		callable->invoke();
		callable->decreaseReference();
		return sl_false;
	}

	void UI::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (callback.isNull()) {
			return;
		}
		if (delayMillis == 0) {
			if (_priv_UIDispatcher::addCallback(callback)) {
				g_idle_add(_ui_dispatch_callback, sl_null);
			}
		} else {
			Callable<void()>* callable = callback.ref.get();
			callable->increaseReference();
			g_timeout_add(delayMillis, _ui_dispatch_timer_callback, callable);
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
		_g_main_thread = ::pthread_self();

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
