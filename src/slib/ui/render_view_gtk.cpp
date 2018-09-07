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

#if defined(SLIB_UI_IS_GTK)

#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"

#include "view_gtk.h"

#include "gdk/x11/gdkx.h"

namespace slib
{

	class GTK_RenderViewInstance : public GTK_ViewInstance, public IRenderCallback
	{
	public:
		AtomicRef<Renderer> m_renderer;

	public:
		GTK_RenderViewInstance()
		{
		}

		~GTK_RenderViewInstance()
		{
			Ref<Renderer> renderer = m_renderer;
			if (renderer.isNotNull()) {
				renderer->release();
			}
		}

	public:
		void setRenderer(const Ref<Renderer>& renderer, RedrawMode redrawMode)
		{
			m_renderer = renderer;
			if (renderer.isNotNull()) {
				renderer->setRenderingContinuously(redrawMode == RedrawMode::Continuously);
			}
		}

		gboolean onExposeEvent(GdkEventExpose* event) override
		{
			Ref<Renderer> renderer = m_renderer;
			if (renderer.isNotNull()) {
				renderer->requestRender();
				return sl_true;
			}
			return sl_false;
		}

		void onFrame(RenderEngine* engine) override
		{
			Ref<View> _view = getView();
			if (RenderView* view = CastInstance<RenderView>(_view.get())) {
				view->dispatchFrame(engine);
			}
		}
	};

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		GTK_ViewInstance* parent = static_cast<GTK_ViewInstance*>(_parent);		
		GtkWidget* handle = gtk_drawing_area_new();
		
		if (handle) {
			GTK_WIDGET_UNSET_FLAGS(handle, GTK_NO_WINDOW);
			GTK_WIDGET_SET_FLAGS(handle, GTK_CAN_FOCUS);
			Ref<GTK_RenderViewInstance> ret = GTK_ViewInstance::create<GTK_RenderViewInstance>(this, parent, handle);
			if (ret.isNotNull()) {
				GtkWidget* handle = ret->getHandle();
				if (handle) {
					gtk_widget_realize(handle);
					GdkWindow* gwindow = handle->window;
					if (gwindow) {
						void* xdisplay = GDK_WINDOW_XDISPLAY(gwindow);
						XID xwindow = GDK_WINDOW_XWINDOW(gwindow);
						if (xdisplay && xwindow != None) {
							RendererParam rp;
							rp.callback = WeakRef<GTK_RenderViewInstance>(ret);
							Ref<Renderer> renderer = GLX::createRenderer(xdisplay, xwindow, rp);
							if (renderer.isNotNull()) {
								ret->setRenderer(renderer, m_redrawMode);
								return ret;
							}
						}
					}
				}
			} else {
				g_object_ref_sink(handle);
				g_object_unref(handle);
			}
		}
		return sl_null;
	}

	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		Ref<GTK_RenderViewInstance> instance = Ref<GTK_RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			Ref<Renderer> renderer = instance->m_renderer;
			if (renderer.isNotNull()) {
				renderer->setRenderingContinuously(mode == RedrawMode::Continuously);
			}
		}
	}

	void RenderView::_requestRender_NW()
	{
		Ref<GTK_RenderViewInstance> instance = Ref<GTK_RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			Ref<Renderer> renderer = instance->m_renderer;
			if (renderer.isNotNull()) {
				renderer->requestRender();
			}
		}
	}

}

#endif
