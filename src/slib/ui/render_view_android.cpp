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

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"

#include "view_android.h"

namespace slib
{

	class Android_RenderViewInstance : public Android_ViewInstance
	{
	public:
		Ref<RenderEngine> m_renderEngine;
	};

	void JNICALL Android_GLView_nativeOnCreate(JNIEnv* env, jobject _this, jlong jinstance)
	{
		Ref<Android_RenderViewInstance> instance = Ref<Android_RenderViewInstance>::from(Android_ViewInstance::findInstance(jinstance));
		if (instance.isNotNull()) {
			instance->m_renderEngine.setNull();
		}
	}

	void JNICALL Android_GLView_nativeOnFrame(JNIEnv* env, jobject _this, jlong jinstance, jint width, jint height)
	{
		Ref<Android_RenderViewInstance> instance = Ref<Android_RenderViewInstance>::from(Android_ViewInstance::findInstance(jinstance));
		if (instance.isNotNull()) {
			Ref<View> _view = instance->getView();
			if (RenderView* view = CastInstance<RenderView>(_view.get())) {
				Ref<RenderEngine> engine = instance->m_renderEngine;
				if (engine.isNull()) {
					engine = GLES::createEngine();
					instance->m_renderEngine = engine;
				}
				if (engine.isNotNull()) {
					if (width > 0 && height > 0) {
						engine->setViewport(0, 0, width, height);
						view->dispatchFrame(engine.get());
					}
				}
			}
		}
	}

	SLIB_JNI_BEGIN_CLASS(JAndroidGLView, "slib/platform/android/ui/view/UiGLView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiGLView;")

		SLIB_JNI_STATIC_METHOD(setRenderMode, "_setRenderMode", "(Landroid/view/View;I)Z")
		SLIB_JNI_STATIC_METHOD(requestRender, "_requestRender", "(Landroid/view/View;)V")

		SLIB_JNI_NATIVE(nativeOnCreate, "nativeOnCreate", "(J)V", Android_GLView_nativeOnCreate)
		SLIB_JNI_NATIVE(nativeOnFrame, "nativeOnFrame", "(JII)V", Android_GLView_nativeOnFrame)

	SLIB_JNI_END_CLASS

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidGLView::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_RenderViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				JAndroidGLView::setRenderMode.callBoolean(sl_null, handle, m_redrawMode);
			}
		}
		return ret;
	}

	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidGLView::setRenderMode.callBoolean(sl_null, handle, mode);
		}
	}

	void RenderView::_requestRender_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidGLView::requestRender.call(sl_null, handle);
		}
	}

}

#endif
