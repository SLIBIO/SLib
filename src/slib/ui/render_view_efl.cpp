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

#include "slib/ui/core.h"
#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"
#include "slib/ui/mobile_app.h"

#include "view_efl.h"

#include <Elementary_GL_Helpers.h>

ELEMENTARY_GLVIEW_GLOBAL_DECLARE()

#include <Evas.h>

namespace slib
{

	namespace priv
	{
		namespace render_view
		{
			class RenderViewInstance : public EFL_ViewInstance
			{
			public:
				Ref<RenderEngine> m_renderEngine;
				sl_bool m_flagContinuously;
				sl_bool m_flagRequestRender;
				Ecore_Animator* m_timer;

			public:
				RenderViewInstance()
				{
					m_flagContinuously = sl_false;
					m_flagRequestRender = sl_false;
					m_timer = sl_null;
				}

				~RenderViewInstance()
				{
					if (m_timer) {
						::ecore_animator_del(m_timer);
					}
				}

				static void glInit(Evas_Object* handle)
				{
					Ref<RenderViewInstance> instance = Ref<RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
					if (instance.isNotNull()) {
						instance->m_renderEngine = GLES::createEngine();
						if (RenderView* view = CastInstance<RenderView>(_view.get())) {
							view->dispatchCreateEngine(instance->m_renderEngine.get());
						}
					}
				}

				static void glResize(Evas_Object* handle)
				{
					Ref<RenderViewInstance> instance = Ref<RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
					if (instance.isNotNull()) {
						int w = 0;
						int h = 0;
						elm_glview_size_get(handle, &w, &h);
						if (w > 0 && h > 0) {
							instance->m_renderEngine->setViewport(0, 0, w, h);
						}
					}
				}

				static void glRender(Evas_Object* handle)
				{
					Ref<RenderViewInstance> instance = Ref<RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
					if (instance.isNotNull()) {
						Ref<View> _view = instance->getView();
						if (RenderView* view = CastInstance<RenderView>(_view.get())) {
							view->dispatchFrame(instance->m_renderEngine.get());
							glFlush();
						}
					}
				}

				static void glDelete(Evas_Object* handle)
				{
				}

				static Eina_Bool timer(void* data)
				{
					Evas_Object* handle = (Evas_Object*)data;
					Ref<RenderViewInstance> instance = Ref<RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
					if (instance.isNotNull()) {
						if (instance->m_flagContinuously || instance->m_flagRequestRender) {
							instance->m_flagRequestRender = sl_false;
							::elm_glview_changed_set(handle);
						}
					}
					return EINA_TRUE;
				}

			};

		}
	}

	using namespace priv::render_view;


	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
	{
		Evas_Object* handleParent = UIPlatform::getViewHandle(parent);

		if (handleParent) {

			Evas_Object* handle = ::elm_glview_version_add(handleParent, EVAS_GL_GLES_2_X);

			if (handle) {

				ELEMENTARY_GLVIEW_GLOBAL_USE(handle);

				Ref<RenderViewInstance> ret = EFL_ViewInstance::create<RenderViewInstance>(this, parent, EFL_ViewType::OpenGL, handle, sl_true);

				if (ret.isNotNull()) {

					::elm_glview_mode_set(handle, (Elm_GLView_Mode)(ELM_GLVIEW_ALPHA | ELM_GLVIEW_DEPTH));
					::elm_glview_resize_policy_set(handle, ELM_GLVIEW_RESIZE_POLICY_RECREATE);
					::elm_glview_render_policy_set(handle, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);
					::elm_glview_init_func_set(handle, RenderViewInstance::glInit);
					::elm_glview_resize_func_set(handle, RenderViewInstance::glResize);
					::elm_glview_render_func_set(handle, RenderViewInstance::glRender);
					::elm_glview_del_func_set(handle, RenderViewInstance::glDelete);

					ret->m_flagContinuously = (m_redrawMode == RedrawMode::Continuously);
					ret->m_timer = ::ecore_animator_add(RenderViewInstance::timer, handle);

					ret->installTouchEvents();

					return ret;
				}
			}
		}

		return sl_null;

	}
	
	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		Ref<RenderViewInstance> instance = Ref<RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			instance->m_flagContinuously = (mode == RedrawMode::Continuously);
		}
	}
	
	void RenderView::_requestRender_NW()
	{
		Ref<RenderViewInstance> instance = Ref<RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			instance->m_flagRequestRender = sl_true;
		}
	}
	
}

#endif
