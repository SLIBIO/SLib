/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"

#include "view_android.h"

namespace slib
{

	class _RenderViewInstance : public Android_ViewInstance
	{
	public:
		Ref<RenderEngine> m_renderEngine;
	};

	void JNICALL _AndroidGLView_nativeOnCreate(JNIEnv* env, jobject _this, jlong jinstance)
	{
		Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(Android_ViewInstance::findInstance(jinstance));
		if (instance.isNotNull()) {
			instance->m_renderEngine.setNull();
		}
	}

	void JNICALL _AndroidGLView_nativeOnFrame(JNIEnv* env, jobject _this, jlong jinstance, jint width, jint height)
	{
		Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(Android_ViewInstance::findInstance(jinstance));
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

	SLIB_JNI_BEGIN_CLASS(_JAndroidGLView, "slib/platform/android/ui/view/UiGLView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiGLView;")

		SLIB_JNI_STATIC_METHOD(setRenderMode, "_setRenderMode", "(Landroid/view/View;I)Z")
		SLIB_JNI_STATIC_METHOD(requestRender, "_requestRender", "(Landroid/view/View;)V")

		SLIB_JNI_NATIVE(nativeOnCreate, "nativeOnCreate", "(J)V", _AndroidGLView_nativeOnCreate)
		SLIB_JNI_NATIVE(nativeOnFrame, "nativeOnFrame", "(JII)V", _AndroidGLView_nativeOnFrame)

	SLIB_JNI_END_CLASS

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = _JAndroidGLView::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<_RenderViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				_JAndroidGLView::setRenderMode.callBoolean(sl_null, handle, m_redrawMode);
			}
		}
		return ret;
	}

	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			_JAndroidGLView::setRenderMode.callBoolean(sl_null, handle, mode);
		}
	}

	void RenderView::_requestRender_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			_JAndroidGLView::requestRender.call(sl_null, handle);
		}
	}

}

#endif
