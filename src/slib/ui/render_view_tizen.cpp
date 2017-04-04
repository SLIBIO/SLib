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

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/ui/core.h"
#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"
#include "slib/ui/mobile_app.h"

#include "view_tizen.h"

#include <Elementary_GL_Helpers.h>

ELEMENTARY_GLVIEW_GLOBAL_DECLARE()

#include <Evas.h>

namespace slib
{

	class _RenderViewInstance : public Tizen_ViewInstance
	{
	public:
		Ref<RenderEngine> m_renderEngine;
		sl_bool m_flagContinuously;
		sl_bool m_flagRequestRender;
		Ecore_Animator* m_timer;

	public:
		_RenderViewInstance()
		{
			m_flagContinuously = sl_false;
			m_flagRequestRender = sl_false;
			m_timer = sl_null;
		}

		~_RenderViewInstance()
		{
			if (m_timer) {
				::ecore_animator_del(m_timer);
			}
		}

		static void glInit(Evas_Object* handle)
		{
			Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
			if (instance.isNotNull()) {
				instance->m_renderEngine = GLES::createEngine();
			}
		}

		static void glResize(Evas_Object* handle)
		{
			Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
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
			Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
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
			Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(UIPlatform::getViewInstance(handle));
			if (instance.isNotNull()) {
				if (instance->m_flagContinuously || instance->m_flagRequestRender) {
					instance->m_flagRequestRender = sl_false;
					::elm_glview_changed_set(handle);
				}
			}
			return EINA_TRUE;
		}

	};

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
	{
		Evas_Object* handleParent = UIPlatform::getViewHandle(parent);

		if (handleParent) {

			Evas_Object* handle = ::elm_glview_version_add(handleParent, EVAS_GL_GLES_2_X);

			if (handle) {

				ELEMENTARY_GLVIEW_GLOBAL_USE(handle);

				Ref<_RenderViewInstance> ret = Tizen_ViewInstance::create<_RenderViewInstance>(this, parent, TizenViewType::OpenGL, handle, sl_true);

				if (ret.isNotNull()) {

					::elm_glview_mode_set(handle, (Elm_GLView_Mode)(ELM_GLVIEW_ALPHA | ELM_GLVIEW_DEPTH));
					::elm_glview_resize_policy_set(handle, ELM_GLVIEW_RESIZE_POLICY_RECREATE);
					::elm_glview_render_policy_set(handle, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);
					::elm_glview_init_func_set(handle, _RenderViewInstance::glInit);
					::elm_glview_resize_func_set(handle, _RenderViewInstance::glResize);
					::elm_glview_render_func_set(handle, _RenderViewInstance::glRender);
					::elm_glview_del_func_set(handle, _RenderViewInstance::glDelete);

					ret->m_flagContinuously = (m_redrawMode == RedrawMode::Continuously);
					ret->m_timer = ::ecore_animator_add(_RenderViewInstance::timer, handle);

					ret->installTouchEvents();

					return ret;
				}
			}
		}

		return sl_null;

	}
	
	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			instance->m_flagContinuously = (mode == RedrawMode::Continuously);
		}
	}
	
	void RenderView::_requestRender_NW()
	{
		Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			instance->m_flagRequestRender = sl_true;
		}
	}
	
}

#endif
