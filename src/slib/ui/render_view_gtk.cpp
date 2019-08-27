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

#include "slib/ui/render_view.h"

#include "slib/render/opengl.h"

#include "view_gtk.h"

#include "gdk/x11/gdkx.h"

namespace slib
{

	namespace priv
	{
		namespace render_view
		{

			class RenderViewHelper : public RenderView
			{
			};

			class RenderViewInstance : public GTK_ViewInstance, public IRenderViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				AtomicRef<Renderer> m_renderer;
				RenderEngine* m_pLastEngine;

			public:
				RenderViewInstance()
				{
					m_pLastEngine = sl_null;
				}

				~RenderViewInstance()
				{
					Ref<Renderer> renderer = m_renderer;
					if (renderer.isNotNull()) {
						renderer->release();
					}
				}

			public:
				void setRedrawMode(RenderView* view, RedrawMode mode) override
				{
					Ref<Renderer> renderer = m_renderer;
					if (renderer.isNotNull()) {
						renderer->setRenderingContinuously(mode == RedrawMode::Continuously);
					}
				}

				void requestRender(RenderView* view) override
				{
					Ref<Renderer> renderer = m_renderer;
					if (renderer.isNotNull()) {
						renderer->requestRender();
					}
				}

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

				void onFrame(RenderEngine* engine)
				{
					Ref<RenderViewHelper> helper = CastRef<RenderViewHelper>(getView());
					if (helper.isNotNull()) {
						if (m_pLastEngine != engine) {
							helper->dispatchCreateEngine(engine);
						}
						helper->dispatchFrame(engine);
						m_pLastEngine = engine;
					}
				}

			};

			SLIB_DEFINE_OBJECT(RenderViewInstance, GTK_ViewInstance)

		}
	}

	using namespace priv::render_view;

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		GTK_ViewInstance* parent = static_cast<GTK_ViewInstance*>(_parent);		
		GtkWidget* handle = gtk_drawing_area_new();
		
		if (handle) {
			GTK_WIDGET_UNSET_FLAGS(handle, GTK_NO_WINDOW);
			GTK_WIDGET_SET_FLAGS(handle, GTK_CAN_FOCUS);
			Ref<RenderViewInstance> ret = GTK_ViewInstance::create<RenderViewInstance>(this, parent, handle);
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
							rp.onFrame = SLIB_FUNCTION_WEAKREF(RenderViewInstance, onFrame, ret);
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

	Ptr<IRenderViewInstance> RenderView::getRenderViewInstance()
	{
		return CastRef<RenderViewInstance>(getViewInstance());
	}
	
}

#endif
