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

#include "view_gtk.h"

#include "slib/ui/core.h"

namespace slib
{

	GTK_ViewInstance::GTK_ViewInstance()
	{
		m_handle = sl_null;
		m_handleChildrenContainer = sl_null;
		m_actionDrag = UIAction::MouseMove;
	}
	
	GTK_ViewInstance::~GTK_ViewInstance()
	{
		_release();
	}
	
	void GTK_ViewInstance::_init(GtkWidget* handle)
	{
		g_object_ref_sink(handle);
		m_handle = handle;
		m_handleChildrenContainer = handle;
		UIPlatform::registerViewInstance(handle, this);
	}

	void GTK_ViewInstance::applyProperties(View* view, ViewInstance* _parent)
	{
		GtkWidget* handle = m_handle;
		if (!handle) {
			return;
		}
		
		if (!(view->isEnabled())) {
			gtk_widget_set_sensitive(handle, sl_false);
		}
		
		if (GTK_IS_FIXED(handle) || GTK_IS_DRAWING_AREA(handle)) {
			if (view->isDrawing()) {
				gtk_widget_set_app_paintable(handle, sl_true);
			}
			installEvents();
		}
		
		GtkWidget* parent = sl_null;
		if (_parent) {
			parent = ((GTK_ViewInstance*)_parent)->m_handleChildrenContainer;
		}
		UIRect frame = view->getFrame();
		if (parent && GTK_IS_FIXED(parent)) {
			sl_ui_pos x = frame.left;
			sl_ui_pos y = frame.top;
			Vector2 t;
			if (view->getFinalTranslationRotationScale(&t, NULL, NULL, NULL)) {
				x += (sl_ui_pos)(t.x);
				y += (sl_ui_pos)(t.y);
			}
			gtk_fixed_put((GtkFixed*)parent, handle, x, y);
		}
		gtk_widget_set_size_request(handle, frame.getWidth(), frame.getHeight());

		if (view->isVisible()) {
			gtk_widget_show(handle);
		}
	}

	void GTK_ViewInstance::_release()
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			UIPlatform::removeViewInstance(handle);
			g_object_unref(handle);
			m_handle = sl_null;
			m_handleChildrenContainer = sl_null;
		}
	}

	GtkWidget* GTK_ViewInstance::getHandle()
	{
		return m_handle;
	}
	
	void GTK_ViewInstance::setChildrenContainer(GtkWidget* widget)
	{
		m_handleChildrenContainer = widget;
	}

	sl_bool GTK_ViewInstance::isValid()
	{
		return sl_true;
	}
	
	void GTK_ViewInstance::setFocus()
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(GTK_ViewInstance, setFocus, this));
				return;
			}
			gtk_widget_grab_focus(handle);
		}
	}
	
	void GTK_ViewInstance::invalidate()
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				void(GTK_ViewInstance::*func)() = &GTK_ViewInstance::invalidate;
				UI::dispatchToUiThread(CreateFunctionFromWeakRef(WeakRef<GTK_ViewInstance>(this), func));
				return;
			}
			gtk_widget_queue_draw(handle);
		}
	}
	
	void GTK_ViewInstance::invalidate(const UIRect& rect)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				void(GTK_ViewInstance::*func)(const UIRect&) = &GTK_ViewInstance::invalidate;
				UI::dispatchToUiThread(Function<void()>::bindWeakRef(WeakRef<GTK_ViewInstance>(this), func, rect));
				return;
			}
			gtk_widget_queue_draw_area(handle, rect.left, rect.top, rect.getWidth(), rect.getHeight());
		}
	}
	
	UIRect GTK_ViewInstance::getFrame()
	{
		return UIRect::zero();
	}
	
	void GTK_ViewInstance::setFrame(const UIRect& frame)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), GTK_ViewInstance, setFrame, this, frame));
				return;
			}
			GtkWidget* parent = gtk_widget_get_parent(handle);
			if (parent && GTK_IS_FIXED(parent)) {
				gtk_fixed_move((GtkFixed*)parent, handle, frame.left, frame.top);
			}
			gtk_widget_set_size_request(handle, frame.getWidth(), frame.getHeight());
		}
	}
	
	void GTK_ViewInstance::setTransform(const Matrix3& m)
	{
	}
	
	void GTK_ViewInstance::setVisible(sl_bool flag)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), GTK_ViewInstance, setVisible, this, flag));
				return;
			}
			if (flag) {
				gtk_widget_show(handle);
			} else {
				gtk_widget_hide(handle);
			}
		}
	}
	
	void GTK_ViewInstance::setEnabled(sl_bool flag)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), GTK_ViewInstance, setEnabled, this, flag));
				return;
			}
			if (flag) {
				gtk_widget_set_sensitive(handle, sl_true);
			} else {
				gtk_widget_set_sensitive(handle, sl_false);
			}
		}
	}
	
	void GTK_ViewInstance::setOpaque(sl_bool flag)
	{
	}
	
	void GTK_ViewInstance::setAlpha(sl_real alpha)
	{
	}

	void GTK_ViewInstance::setClipping(sl_bool flag)
	{
	}

	void GTK_ViewInstance::setDrawing(sl_bool flag)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), GTK_ViewInstance, setDrawing, this, flag));
				return;
			}
			if (GTK_IS_FIXED(handle)) {
				if (flag) {
					gtk_widget_set_app_paintable(handle, sl_true);
				} else {
					gtk_widget_set_app_paintable(handle, sl_false);
				}
			}
		}
	}
	
	UIPointf GTK_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			sl_ui_len x, y;
			UIPlatform::getScreenLocationOfWidget(handle, &x, &y);
			return UIPointf((sl_ui_posf)(ptScreen.x - x), (sl_ui_posf)(ptScreen.y - y));
		}
		return ptScreen;
	}
	
	UIPointf GTK_ViewInstance::convertCoordinateFromViewToScreen(const UIPointf& ptView)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			sl_ui_len x, y;
			UIPlatform::getScreenLocationOfWidget(handle, &x, &y);
			return UIPointf((sl_ui_posf)(ptView.x + x), (sl_ui_posf)(ptView.y + y));
		}
		return ptView;
	}
	
	void GTK_ViewInstance::addChildInstance(const Ref<ViewInstance>& _child)
	{
		GTK_ViewInstance* child = static_cast<GTK_ViewInstance*>(_child.get());
		if (child) {
			GtkWidget* handle = m_handleChildrenContainer;
			if (handle) {
				GtkWidget* handleChild = child->m_handle;
				if (handleChild) {
					if (GTK_IS_FIXED(handle)) {
						UIPoint location;
						Ref<View> view = child->getView();
						if (view.isNotNull()) {
							location = view->getLocation();
						} else {
							location.x = 0;
							location.y = 0;
						}
						gtk_fixed_put((GtkFixed*)handle, handleChild, location.x, location.y);
					} else if (GTK_IS_CONTAINER(handle)) {
						gtk_container_add((GtkContainer*)handle, handleChild);
					}
				}
			}
		}
	}
	
	void GTK_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
	{
		GTK_ViewInstance* child = static_cast<GTK_ViewInstance*>(_child.get());
		if (child) {
			GtkWidget* handle = m_handleChildrenContainer;
			if (handle) {
				GtkWidget* handleChild = child->m_handle;
				if (handleChild) {
					if (GTK_IS_CONTAINER(handle)) {
						gtk_container_remove((GtkContainer*)handle, handleChild);
					}
				}
			}
		}
	}
	
	void GTK_ViewInstance::bringToFront()
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(GTK_ViewInstance, bringToFront, this));
				return;
			}
			GdkWindow* window = handle->window;
			if (window) {
				gdk_window_raise(window);
			}
		}
	}
	
	void GTK_ViewInstance::installEvents()
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			g_signal_connect(handle, "expose_event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "motion-notify-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "button-press-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "button-release-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "enter-notify-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "leave-notify-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "key-press-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "key-release-event", G_CALLBACK(eventCallback), sl_null);
			g_signal_connect(handle, "scroll-event", G_CALLBACK(eventCallback), sl_null);
			gtk_widget_set_events(handle,
								  GDK_EXPOSURE_MASK |
								  GDK_POINTER_MOTION_MASK | GDK_BUTTON_MOTION_MASK |
								  GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
								  GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
								  GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
								  GDK_SCROLL_MASK);
		}
	}
	
	gboolean GTK_ViewInstance::eventCallback(GtkWidget* widget, GdkEvent* event, gpointer user_data)
	{
		Ref<GTK_ViewInstance> instance = Ref<GTK_ViewInstance>::from(UIPlatform::getViewInstance(widget));
		if (instance.isNotNull()) {
			switch (event->type) {
				case GDK_EXPOSE:
					instance->onExposeEvent((GdkEventExpose*)event);
					break;
				case GDK_MOTION_NOTIFY:
					instance->onMotionNotifyEvent((GdkEventMotion*)event);
					break;
				case GDK_BUTTON_PRESS:
				case GDK_2BUTTON_PRESS:
				case GDK_BUTTON_RELEASE:
					instance->onButtonEvent((GdkEventButton*)event);
					break;
				case GDK_ENTER_NOTIFY:
				case GDK_LEAVE_NOTIFY:
					instance->onCrossingEvent((GdkEventCrossing*)event);
					break;
				case GDK_KEY_PRESS:
				case GDK_KEY_RELEASE:
					instance->onKeyEvent((GdkEventKey*)event);
					break;
				case GDK_SCROLL:
					instance->onScrollEvent((GdkEventScroll*)event);
					break;
			}
		}
		return sl_true;
	}
	
	void GTK_ViewInstance::applyModifiers(UIEvent* event, guint state)
	{
		if (state & GDK_SHIFT_MASK) {
			event->setShiftKey();
		}
		if (state & GDK_CONTROL_MASK) {
			event->setControlKey();
		}
		if (state & GDK_MOD1_MASK) {
			event->setAltKey();
		}
		if (state & GDK_MOD4_MASK) {
			event->setWindowsKey();
		}
	}
	
	gboolean GTK_ViewInstance::onExposeEvent(GdkEventExpose* event)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			GdkWindow* window = handle->window;
			if (window) {
				cairo_t* cairo = gdk_cairo_create(window);
				if (cairo) {
					GtkAllocation region = handle->allocation;
					Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, cairo, region.width, region.height);
					if (canvas.isNotNull()) {
						Rectangle rect;
						rect.left = event->area.x;
						rect.top = event->area.y;
						rect.right = rect.left + event->area.width;
						rect.bottom = rect.top + event->area.height;
						canvas->setInvalidatedRect(rect);
						onDraw(canvas.get());
					}
				}
			}
		}
		return sl_true;
	}
	
	gboolean GTK_ViewInstance::onMotionNotifyEvent(GdkEventMotion* gevent)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			Time time;
			time.setMillisecondsCount(gevent->time);
			Ref<UIEvent> event = UIEvent::createMouseEvent(m_actionDrag, gevent->x, gevent->y, time);
			if (event.isNotNull()) {
				applyModifiers(event.get(), gevent->state);
				onMouseEvent(event.get());
				if (event->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	gboolean GTK_ViewInstance::onButtonEvent(GdkEventButton* gevent)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			UIAction action;
			if (gevent->button == 1) { // Left
				if (gevent->type == GDK_BUTTON_PRESS) {
					action = UIAction::LeftButtonDown;
					m_actionDrag = UIAction::LeftButtonDrag;
				} else if (gevent->type == GDK_BUTTON_RELEASE) {
					action = UIAction::LeftButtonUp;
					m_actionDrag = UIAction::MouseMove;
				} else { // GDK_2BUTTON_PRESS
					action = UIAction::LeftButtonDoubleClick;
				}
			} else if (gevent->button == 2) { // Middle
				if (gevent->type == GDK_BUTTON_PRESS) {
					action = UIAction::MiddleButtonDown;
					m_actionDrag = UIAction::MiddleButtonDrag;
				} else if (gevent->type == GDK_BUTTON_RELEASE) {
					action = UIAction::MiddleButtonUp;
					m_actionDrag = UIAction::MouseMove;
				} else { // GDK_2BUTTON_PRESS
					action = UIAction::MiddleButtonDoubleClick;
				}
			} else if (gevent->button == 3) { // Right
				if (gevent->type == GDK_BUTTON_PRESS) {
					action = UIAction::RightButtonDown;
					m_actionDrag = UIAction::RightButtonDrag;
				} else if (gevent->type == GDK_BUTTON_RELEASE) {
					action = UIAction::RightButtonUp;
					m_actionDrag = UIAction::MouseMove;
				} else { // GDK_2BUTTON_PRESS
					action = UIAction::RightButtonDoubleClick;
				}
			} else {
				return sl_false;
			}
			Time time;
			time.setMillisecondsCount(gevent->time);
			gdouble x = gevent->x;
			gdouble y = gevent->y;
			GdkWindow* window = handle->window;
			if (window && window != gevent->window) {
				gint wx = 0, wy = 0;
				gdk_window_get_origin(window, &wx, &wy);
				x = gevent->x_root - wx;
				y = gevent->y_root - wy;
			}
			Ref<UIEvent> event = UIEvent::createMouseEvent(action, x, y, time);
			if (event.isNotNull()) {
				applyModifiers(event.get(), gevent->state);
				onMouseEvent(event.get());
				if (event->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	gboolean GTK_ViewInstance::onCrossingEvent(GdkEventCrossing* gevent)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			UIAction action;
			if (gevent->type == GDK_ENTER_NOTIFY) {
				action = UIAction::MouseEnter;
			} else { // GDK_LEAVE_NOTIFY
				action = UIAction::MouseLeave;
			}
			Time time;
			time.setMillisecondsCount(gevent->time);
			gdouble x = gevent->x;
			gdouble y = gevent->y;
			GdkWindow* window = handle->window;
			if (window && window != gevent->window) {
				gint wx = 0, wy = 0;
				gdk_window_get_origin(window, &wx, &wy);
				x = gevent->x_root - wx;
				y = gevent->y_root - wy;
			}
			Ref<UIEvent> event = UIEvent::createMouseEvent(action, x, y, time);
			if (event.isNotNull()) {
				applyModifiers(event.get(), gevent->state);
				onMouseEvent(event.get());
				if (event->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	gboolean GTK_ViewInstance::onKeyEvent(GdkEventKey* gevent)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			UIAction action;
			if (gevent->type == GDK_KEY_PRESS) {
				action = UIAction::KeyDown;
			} else { // GDK_KEY_RELEASE
				action = UIAction::KeyUp;
			}
			Keycode key = UIEvent::getKeycodeFromSystemKeycode(gevent->keyval);
			Time time;
			time.setMillisecondsCount(gevent->time);
			Ref<UIEvent> event = UIEvent::createKeyEvent(action, key, gevent->keyval, time);
			if (event.isNotNull()) {
				applyModifiers(event.get(), gevent->state);
				ViewInstance::onKeyEvent(event.get());
				if (event->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	gboolean GTK_ViewInstance::onScrollEvent(GdkEventScroll* gevent)
	{
		GtkWidget* handle = m_handle;
		if (handle) {
			Time time;
			time.setMillisecondsCount(gevent->time);
			gdouble x = gevent->x;
			gdouble y = gevent->y;
			GdkWindow* window = handle->window;
			if (window && window != gevent->window) {
				gint wx = 0, wy = 0;
				gdk_window_get_origin(window, &wx, &wy);
				x = gevent->x_root - wx;
				y = gevent->y_root - wy;
			}
			gint dx = 0;
			gint dy = 0;
			gint delta = 10;
			switch (gevent->direction) {
				case GDK_SCROLL_UP:
					dy = -delta;
					break;
				case GDK_SCROLL_DOWN:
					dy = delta;
					break;
				case GDK_SCROLL_LEFT:
					dx = -delta;
					break;
				case GDK_SCROLL_RIGHT:
					dx = delta;
					break;
				default:
					return sl_false;
			}
			Ref<UIEvent> event = UIEvent::createMouseWheelEvent(x, y, dx, dy, time);
			if (event.isNotNull()) {
				applyModifiers(event.get(), gevent->state);
				onMouseWheelEvent(event.get());
				if (event->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	
	Ref<ViewInstance> View::createGenericInstance(ViewInstance* _parent)
	{
		GTK_ViewInstance* parent = static_cast<GTK_ViewInstance*>(_parent);
		
		GtkWidget* handle;
		if (m_flagCreatingChildInstances) {
			handle = gtk_fixed_new();
		} else {
			handle = gtk_drawing_area_new();
		}
		
		if (handle) {
			GTK_WIDGET_UNSET_FLAGS(handle, GTK_NO_WINDOW);
			GTK_WIDGET_SET_FLAGS(handle, GTK_CAN_FOCUS);
			Ref<GTK_ViewInstance> ret = GTK_ViewInstance::create<GTK_ViewInstance>(this, parent, handle);
			if (ret.isNotNull()) {
				return ret;
			}
			g_object_ref_sink(handle);
			g_object_unref(handle);
		}
		return sl_null;
	}


	Ref<ViewInstance> UIPlatform::createViewInstance(GtkWidget* handle)
	{
		Ref<ViewInstance> ret = UIPlatform::_getViewInstance(handle);
		if (ret.isNotNull()) {
			return ret;
		}
		return GTK_ViewInstance::create<GTK_ViewInstance>(handle);
	}
	
	void UIPlatform::registerViewInstance(GtkWidget* handle, ViewInstance* instance)
	{
		UIPlatform::_registerViewInstance(handle, instance);
	}
	
	Ref<ViewInstance> UIPlatform::getViewInstance(GtkWidget* handle)
	{
		return UIPlatform::_getViewInstance(handle);
	}
	
	Ref<View> UIPlatform::getView(GtkWidget* handle)
	{
		Ref<ViewInstance> instance = UIPlatform::_getViewInstance(handle);
		if (instance.isNotNull()) {
			return instance->getView();
		}
		return sl_null;
	}

	void UIPlatform::removeViewInstance(GtkWidget* handle)
	{
		UIPlatform::_removeViewInstance(handle);
	}
	
	GtkWidget* UIPlatform::getViewHandle(ViewInstance* _instance)
	{
		GTK_ViewInstance* instance = static_cast<GTK_ViewInstance*>(_instance);
		if (instance) {
			return instance->getHandle();
		}
		return sl_null;
	}
	
	GtkWidget* UIPlatform::getViewHandle(View* view)
	{
		if (view) {
			Ref<ViewInstance> _instance = view->getViewInstance();
			if (_instance.isNotNull()) {
				GTK_ViewInstance* instance = static_cast<GTK_ViewInstance*>(_instance.get());
				return instance->getHandle();
			}
		}
		return sl_null;
	}

}

#endif


