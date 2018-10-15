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

#include "slib/ui/window.h"
#include "slib/ui/screen.h"
#include "slib/ui/core.h"

#include "view_gtk.h"

namespace slib
{
	class _priv_GTK_Window : public WindowInstance
	{
	public:
		GtkWindow* m_window;
		AtomicRef<ViewInstance> m_viewContent;
		sl_bool m_flagClosed;
		
		UIPoint m_location;
		UISize m_size;
		UIPoint m_origin;
		
		sl_bool m_flagFirstMove;
		sl_bool m_flagFirstResize;
		sl_bool m_flagResizeByOrigin;
		
		sl_bool m_flagMinimized;
		sl_bool m_flagMaximized;
		
	public:
		_priv_GTK_Window()
		{
			m_window = sl_null;
			m_flagClosed = sl_true;
			
			m_flagFirstMove = sl_true;
			m_flagFirstResize = sl_true;
			m_flagResizeByOrigin = sl_false;
			
			m_flagMinimized = sl_false;
			m_flagMaximized = sl_false;
		}
		
		~_priv_GTK_Window()
		{
			_release();
		}
		
	public:
		static Ref<_priv_GTK_Window> create(GtkWindow* window)
		{
			if (window) {
				Ref<_priv_GTK_Window> ret = new _priv_GTK_Window();
				if (ret.isNotNull()) {
					ret->_init(window);
					return ret;
				}
			}
			return sl_null;
		}
		
		void _init(GtkWindow* window)
		{
			g_object_ref_sink(window);
			
			m_window = window;
			m_flagClosed = sl_false;
			m_origin.x = 0;
			m_origin.y = 0;
			
			GtkWidget* contentWidget = gtk_fixed_new();
			if (contentWidget) {
				gtk_container_add((GtkContainer*)window, contentWidget);
				gtk_widget_show(contentWidget);
			}
			Ref<GTK_ViewInstance> content = GTK_ViewInstance::create<GTK_ViewInstance>((GtkWidget*)window);
			if (content.isNotNull()) {
				content->setChildrenContainer(contentWidget);
				content->setWindowContent(sl_true);
				content->installEvents();
				m_viewContent = content;
			}
			
			g_signal_connect(window, "destroy", G_CALLBACK(_ui_win_on_destroy_cb), NULL);
			g_signal_connect(window, "delete-event", G_CALLBACK(_ui_win_on_close_cb), NULL);
			g_signal_connect(window, "window-state-event", G_CALLBACK(_ui_win_on_window_state_cb), NULL);
			g_signal_connect(window, "configure-event", G_CALLBACK(_ui_win_on_configure_event_cb), NULL);
			g_signal_connect(window, "notify::is-active", G_CALLBACK(_ui_win_on_notify_is_active_cb), NULL);
						
			gint x, y, width, height;
			gtk_window_get_position(window, &x, &y);
			gtk_window_get_size(window, &width, &height);
			
			m_location.x = x;
			m_location.y = y;
			m_size.x = width;
			m_size.y = height;

			UIPlatform::registerWindowInstance(window, this);
		}
		
		static void _release_handle(GtkWindow* window)
		{
			gtk_widget_destroy((GtkWidget*)window);
			g_object_unref(window);
		}

		void _release()
		{
			GtkWindow* window = m_window;
			if (window) {
				m_window = sl_null;
				if (m_flagClosed) {
					g_object_unref(window);
				} else {
					if (UI::isUiThread()) {
						_release_handle(window);
					} else {
						UI::dispatchToUiThread(Function<void()>::bind(&_release_handle, window));
					}
				}
				UIPlatform::removeWindowInstance(window);
			}
			m_viewContent.setNull();
			m_flagClosed = sl_true;
		}
		
		void _on_destroy()
		{
			m_flagClosed = sl_true;
		}
		
		static void _ui_win_on_destroy_cb(GtkWindow* window, gpointer user_data)
		{
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(window);
			if (instance.isNotNull()) {
				((_priv_GTK_Window*)(instance.get()))->_on_destroy();
			}
			UIPlatform::removeWindowInstance(window);
		}

		static gboolean _ui_win_on_close_cb(GtkWindow* window, GdkEvent* event, gpointer user_data)
		{
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(window);
			if (instance.isNotNull()) {
				_priv_GTK_Window* _instance = static_cast<_priv_GTK_Window*>(instance.get());
				if (_instance->onClose()) {
					_instance->close();
				}
			}
			return sl_true; // ignore default behavior of GTK+ core
		}
		
		void _on_window_state(GdkEventWindowState* event)
		{
			if (event->changed_mask & GDK_WINDOW_STATE_ICONIFIED) {
				if (event->new_window_state & GDK_WINDOW_STATE_ICONIFIED) {
					m_flagMinimized = sl_true;
					onMinimize();
				} else {
					m_flagMinimized = sl_false;
					onDeminimize();
				}
			}
			if (event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED) {
				if (event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {
					m_flagMaximized = sl_true;
					onMaximize();
				} else {
					m_flagMaximized = sl_false;
					onDemaximize();
				}
			}
		}
		
		static gboolean _ui_win_on_window_state_cb(GtkWindow* window, GdkEventWindowState* event, gpointer user_data)
		{
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(window);
			if (instance.isNotNull()) {
				((_priv_GTK_Window*)(instance.get()))->_on_window_state(event);
			}
			return sl_false;
		}
		
		void _on_configure_event(GtkWindow* window, GdkEventConfigure* event)
		{
			gint x, y;
			gtk_window_get_position(window, &x, &y);
			
			gint ox = 0, oy = 0;
			GdkWindow* gwindow = gtk_widget_get_window((GtkWidget*)window);
			if (gwindow) {
				gdk_window_get_origin(gwindow, &ox, &oy);
			}

			ox -= x;
			oy -= y;
			
			if (m_flagResizeByOrigin) {
				if (!(Math::isAlmostZero(ox - m_origin.x) && Math::isAlmostZero(oy - m_origin.y))) {
					m_origin.x = ox;
					m_origin.y = oy;
					m_size.x -= ox;
					m_size.y -= oy;
					gtk_window_resize(window, m_size.x, m_size.y);
					if (!(gtk_window_get_resizable(window))) {
						gtk_widget_set_size_request((GtkWidget*)window, m_size.x, m_size.y);
					}
					m_flagResizeByOrigin = sl_false;
					return;
				}
			}
			
			m_origin.x = ox;
			m_origin.y = oy;
			
			gint width, height;
			gtk_window_get_size(window, &width, &height);
			
			sl_bool flagResize = !(Math::isAlmostZero(width - m_size.x) && Math::isAlmostZero(height - m_size.y));
			m_size.x = width;
			m_size.y = height;

			if (m_flagFirstResize) {
				m_flagFirstResize = sl_false;
				onResize(width, height);
			} else {
				sl_bool flagMove = !(Math::isAlmostZero(x - m_location.x) && Math::isAlmostZero(y - m_location.y));
				m_location.x = x;
				m_location.y = y;
				if (flagResize) {
					onResize(width, height);
				}
				if (flagMove) {
					onMove();
				}
			}
		}
		
		static gboolean _ui_win_on_configure_event_cb(GtkWindow* window, GdkEventConfigure* event, gpointer user_data)
		{
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(window);
			if (instance.isNotNull()) {
				((_priv_GTK_Window*)(instance.get()))->_on_configure_event(window, event);
			}
			return sl_false;
		}
		
		void _on_notify_is_active(GtkWindow* window)
		{
			if (gtk_window_is_active(window)) {
				onActivate();
			} else {
				onDeactivate();
			}
		}
		
		static void _ui_win_on_notify_is_active_cb(GtkWindow* window, GParamSpec* pspec, gpointer user_data)
		{
			Ref<WindowInstance> instance = UIPlatform::getWindowInstance(window);
			if (instance.isNotNull()) {
				((_priv_GTK_Window*)(instance.get()))->_on_notify_is_active(window);
			}
		}

		static Ref<WindowInstance> create(const WindowInstanceParam& param)
		{
			GtkWindow* window = (GtkWindow*)(gtk_window_new(GTK_WINDOW_TOPLEVEL));
			if (!window) {
				return sl_null;
			}

			if (param.parent.isNotNull()) {
				_priv_GTK_Window* w = (_priv_GTK_Window*)(param.parent.get());
				gtk_window_set_transient_for(window, w->m_window);
			}
			
			if (param.flagBorderless || param.flagFullScreen || !(param.flagShowTitleBar)) {
				gtk_window_set_decorated(window, sl_false);
			}
			
			if (param.flagDialog) {
				gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DIALOG);
			}
			
			if (param.flagModal) {
				gtk_window_set_modal(window, sl_true);
			}
			
			if (param.flagFullScreen) {
				gtk_window_fullscreen(window);
			}
			
			gtk_window_set_title(window, param.title.getData());

			UIRect frameScreen;
			if (param.screen.isNotNull()) {
				frameScreen = param.screen->getRegion();
				GdkScreen* screen = UIPlatform::getScreenHandle(param.screen.get());
				if (screen) {
					gtk_window_set_screen(window, screen);
				}
			} else {
				frameScreen = UI::getScreenRegion();
			}
			UIRect frameWindow = param.calculateRegion(frameScreen);
			
			Ref<_priv_GTK_Window> ret = create(window);
			if (ret.isNull()) {
				g_object_ref_sink(window);
				g_object_unref(window);
				return sl_null;
			}
			
			ret->m_size = frameWindow.getSize();
			ret->m_location = frameWindow.getLocation();
			
			if (!(param.flagClientSize || param.flagBorderless || param.flagFullScreen || !(param.flagShowTitleBar))) {
				ret->m_flagResizeByOrigin = sl_true;
			}

			gtk_widget_set_size_request((GtkWidget*)window, 0, 0);
			gtk_window_resize(window, ret->m_size.x, ret->m_size.y);
			
			return ret;
		}
		
		Ref<ViewInstance> getContentView() override
		{
			return m_viewContent;
		}
		
		void close() override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(_priv_GTK_Window, close, this));
					return;
				}
				GtkWindow* window = m_window;
				if (window) {
					UIPlatform::removeWindowInstance(window);
					gtk_widget_destroy((GtkWidget*)window);
				}
				m_flagClosed = sl_true;
			}
			m_viewContent.setNull();
		}
		
		sl_bool isClosed() override
		{
			return m_flagClosed;
		}
		
		sl_bool setParent(const Ref<WindowInstance>& windowParent) override
		{
			GtkWindow* window = m_window;
			if (window && !m_flagClosed) {
				if (windowParent.isNotNull()) {
					gtk_window_set_transient_for(window, UIPlatform::getWindowHandle(windowParent.get()));
				} else {
					gtk_window_set_transient_for(window, sl_null);
				}
				return sl_true;
			}
			return sl_false;
		}
		
		sl_bool setFocus() override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setFocus, this));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					gtk_window_present(window);
					return sl_true;
				}
			}
			return sl_false;
		}
		
		UIRect getFrame() override
		{
			UIRect ret;
			ret.left = m_location.x;
			ret.top = m_location.y;
			ret.right = ret.left + m_size.x + m_origin.x;
			ret.bottom = ret.top + m_size.y + m_origin.y;
			return ret;
		}
		
		sl_bool setFrame(const UIRect& frame) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setFrame, this, frame));
					return sl_true;
				}
				UIPoint location = frame.getLocation();
				UISize size;
				size.x = frame.getWidth() - m_origin.x;
				size.y = frame.getHeight() - m_origin.y;
				if (m_location.isAlmostEqual(location) && m_size.isAlmostEqual(size)) {
					return sl_true;
				}
				m_location = location;
				m_size = size;
				GtkWindow* window = m_window;
				if (window) {
					gtk_window_move(window, m_location.x, m_location.y);
					gtk_window_resize(window, m_size.x, m_size.y);
					if (!(gtk_window_get_resizable(window))) {
						gtk_widget_set_size_request((GtkWidget*)window, m_size.x, m_size.y);
					}
				}
				return sl_true;
			}
			return sl_false;
		}
		
		UIRect getClientFrame() override
		{
			UIRect rect;
			rect.left = m_location.x + m_origin.x;
			rect.top = m_location.y + m_origin.y;
			rect.right = rect.left + m_size.x;
			rect.bottom = rect.top + m_size.y;
			return rect;
		}
		
		UISize getClientSize() override
		{
			return m_size;
		}
		
		sl_bool setClientSize(const UISize& size) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setClientSize, this, size));
					return sl_true;
				}
				if (m_size.isAlmostEqual(size)) {
					return sl_true;
				}
				m_size = size;
				GtkWindow* window = m_window;
				if (window) {
					gtk_window_resize(window, m_size.x, m_size.y);
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setTitle(const String& title) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setTitle, this, title));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					gtk_window_set_title(window, title.getData());
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setBackgroundColor(const Color& color) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setBackgroundColor, this, color));
					return sl_true;
				}
				Ref<ViewInstance> viewContent = m_viewContent;
				if (viewContent.isNotNull()) {
					GtkWidget* content = UIPlatform::getViewHandle(viewContent.get());
					if (content) {
						if (color.a == 0) {
							gtk_widget_modify_bg(content, GTK_STATE_NORMAL, sl_null);
						} else {
							GdkColor gcolor;
							UIPlatform::getGdkColor(color, &gcolor);
							gtk_widget_modify_bg(content, GTK_STATE_NORMAL, &gcolor);
						}
					}
				}
				return sl_true;
			}
			return sl_false;
		}
		
		sl_bool isMinimized() override
		{
			return m_flagMinimized;
		}
		
		sl_bool setMinimized(sl_bool flag) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setMinimized, this, flag));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					if (m_flagMinimized) {
						if (!flag) {
							m_flagMinimized = sl_false;
							gtk_window_deiconify(window);
						}
					} else {
						if (flag) {
							m_flagMinimized = sl_true;
							gtk_window_iconify(window);
						}
					}
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool isMaximized() override
		{
			return m_flagMaximized;
		}
		
		sl_bool setMaximized(sl_bool flag) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setMaximized, this, flag));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					if (m_flagMaximized) {
						if (!flag) {
							m_flagMaximized = sl_false;
							gtk_window_unmaximize(window);
						}
					} else {
						if (flag) {
							m_flagMaximized = sl_true;
							gtk_window_maximize(window);
						}
					}
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setVisible(sl_bool flag) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setVisible, this, flag));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					if (flag) {
						gtk_widget_show((GtkWidget*)window);
						if (m_flagFirstMove) {
							m_flagFirstMove = sl_false;
							gtk_window_move(window, m_location.x, m_location.y);
						}
					} else {
						gtk_widget_hide((GtkWidget*)window);
					}
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setAlwaysOnTop(sl_bool flag) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setAlwaysOnTop, this, flag));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					if (flag) {
						gtk_window_set_keep_above(window, sl_true);
					} else {
						gtk_window_set_keep_above(window, sl_false);
					}
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setCloseButtonEnabled(sl_bool flag) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setCloseButtonEnabled, this, flag));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					if (flag) {
						gtk_window_set_deletable(window, sl_true);
					} else {
						gtk_window_set_deletable(window, sl_false);
					}
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setMinimizeButtonEnabled(sl_bool flag) override
		{
			return sl_false;
		}
		
		sl_bool setMaximizeButtonEnabled(sl_bool flag) override
		{
			return sl_false;
		}
		
		sl_bool setResizable(sl_bool flag) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setResizable, this, flag));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					if (flag) {
						if (!(gtk_window_get_resizable(window))) {
							gtk_widget_set_size_request((GtkWidget*)window, 0, 0);
							gtk_window_set_resizable(window, sl_true);
							gtk_window_resize(window, m_size.x, m_size.y);
						}
					} else {
						if (gtk_window_get_resizable(window)) {
							gtk_widget_set_size_request((GtkWidget*)window, m_size.x, m_size.y);
							gtk_window_set_resizable(window, sl_false);
						}
					}
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setAlpha(sl_real alpha) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setAlpha, this, alpha));
					return sl_true;
				}
				GtkWindow* window = m_window;
				if (window) {
					gtk_window_set_opacity(window, alpha);
					return sl_true;
				}
			}
			return sl_false;
		}
		
		sl_bool setTransparent(sl_bool flag) override
		{
			return sl_false;
		}
		
		UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen) override
		{
			return UIPointf(ptScreen.x - m_location.x, ptScreen.y - m_location.y);
		}
		
		UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow) override
		{
			return UIPointf(ptWindow.x + m_location.x, ptWindow.y + m_location.y);
		}
		
		UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen) override
		{
			return UIPointf(ptScreen.x - m_location.x - m_origin.x, ptScreen.y - m_location.y - m_origin.y);
		}
		
		UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient) override
		{
			return UIPointf(ptClient.x + m_location.x + m_origin.x, ptClient.y + m_location.y + m_origin.y);
		}
		
		UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow) override
		{
			return UIPointf(ptWindow.x - m_origin.x, ptWindow.y - m_origin.y);
		}
		
		UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient) override
		{
			return UIPointf(ptClient.x + m_origin.x, ptClient.y + m_origin.y);
		}
		
		UISize getWindowSizeFromClientSize(const UISize& sizeClient) override
		{
			return UISize(sizeClient.x + m_origin.x, sizeClient.y + m_origin.y);
		}
		
		UISize getClientSizeFromWindowSize(const UISize& sizeWindow) override
		{
			return UISize(sizeWindow.x - m_origin.x, sizeWindow.y - m_origin.y);
		}
		
		void setSizeRange(const UISize& sizeMinimum, const UISize& sizeMaximum, float aspectRatioMinimum, float aspectRatioMaximum) override
		{
			if (!m_flagClosed) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), _priv_GTK_Window, setSizeRange, this, sizeMinimum, sizeMaximum, aspectRatioMinimum, aspectRatioMaximum));
					return;
				}
				GtkWindow* window = m_window;
				if (window) {
					GdkGeometry geometry;
					Base::zeroMemory(&geometry, sizeof(geometry));

					gint hints = GDK_HINT_MIN_SIZE;
					geometry.min_width = SLIB_MAX(0, sizeMinimum.x);
					geometry.min_height = SLIB_MAX(0, sizeMinimum.y);

					if (sizeMaximum.x > 0 || sizeMaximum.y > 0) {
						hints |= GDK_HINT_MAX_SIZE;
						gint w = sizeMaximum.x;
						if (w <= 0) {
							w = 100000;
						}
						geometry.max_width = w;
						gint h = sizeMaximum.y;
						if (h <= 0) {
							h = 100000;
						}
						geometry.max_height = h;
					}
					if (aspectRatioMinimum > 0 || aspectRatioMaximum > 0) {
						hints |= GDK_HINT_ASPECT;
						gdouble r;
						r = aspectRatioMinimum;
						if (r <= 0) {
							r = 0.00001;
						}
						geometry.min_aspect = r;
						r = aspectRatioMaximum;
						if (r <= 0) {
							r = 100000;
						}
						geometry.max_aspect = r;
					}
					gtk_window_set_geometry_hints(window, (GtkWidget*)window, &geometry, (GdkWindowHints)(hints));
				}
			}
		}
		
	};

	
	Ref<WindowInstance> Window::createWindowInstance(const WindowInstanceParam& param)
	{
		return _priv_GTK_Window::create(param);
	}
	
	
	Ref<WindowInstance> UIPlatform::createWindowInstance(GtkWindow* window)
	{
		Ref<WindowInstance> ret = UIPlatform::_getWindowInstance(window);
		if (ret.isNotNull()) {
			return ret;
		}
		return _priv_GTK_Window::create(window);
	}
	
	void UIPlatform::registerWindowInstance(GtkWindow* window, WindowInstance* instance)
	{
		UIPlatform::_registerWindowInstance(window, instance);
	}
	
	Ref<WindowInstance> UIPlatform::getWindowInstance(GtkWindow* window)
	{
		return UIPlatform::_getWindowInstance(window);
	}
	
	void UIPlatform::removeWindowInstance(GtkWindow* window)
	{
		UIPlatform::_removeWindowInstance(window);
	}
	
	GtkWindow* UIPlatform::getWindowHandle(WindowInstance* instance)
	{
		_priv_GTK_Window* window = static_cast<_priv_GTK_Window*>(instance);
		if (window) {
			return window->m_window;
		} else {
			return sl_null;
		}
	}
	
	GtkWindow* UIPlatform::getWindowHandle(Window* window)
	{
		if (window) {
			Ref<WindowInstance> _instance = window->getWindowInstance();
			if (_instance.isNotNull()) {
				_priv_GTK_Window* instance = static_cast<_priv_GTK_Window*>(_instance.get());
				return instance->m_window;
			}
		}
		return sl_null;
	}
	
}

#endif
