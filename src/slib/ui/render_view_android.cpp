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

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/render_view.h"

#include "slib/render/opengl.h"

#include "view_android.h"

namespace slib
{

	namespace priv
	{
		namespace render_view
		{

			void JNICALL OnCreate(JNIEnv* env, jobject _this, jlong jinstance);
			void JNICALL OnFrame(JNIEnv* env, jobject _this, jlong jinstance, jint width, jint height);

			SLIB_JNI_BEGIN_CLASS(JGLView, "slib/platform/android/ui/view/UiGLView")

				SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiGLView;")

				SLIB_JNI_STATIC_METHOD(setRenderMode, "_setRenderMode", "(Landroid/view/View;I)Z")
				SLIB_JNI_STATIC_METHOD(requestRender, "_requestRender", "(Landroid/view/View;)V")

				SLIB_JNI_NATIVE(nativeOnCreate, "nativeOnCreate", "(J)V", OnCreate)
				SLIB_JNI_NATIVE(nativeOnFrame, "nativeOnFrame", "(JII)V", OnFrame)

			SLIB_JNI_END_CLASS

			class RenderViewHelper : public RenderView
			{
			};

			class RenderViewInstance : public Android_ViewInstance, public IRenderViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				Ref<RenderEngine> m_renderEngine;

			public:
				void setRedrawMode(RenderView* view, RedrawMode mode) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JGLView::setRenderMode.callBoolean(sl_null, handle, mode);
					}
				}

				void requestRender(RenderView* view) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JGLView::requestRender.call(sl_null, handle);
					}
				}

				void onCreate()
				{
					m_renderEngine.setNull();
				}

				void onFrame(jint width, jint height)
				{
					Ref<RenderViewHelper> helper = CastRef<RenderViewHelper>(getView());
					if (helper.isNotNull()) {
						Ref<RenderEngine> engine = m_renderEngine;
						if (engine.isNull()) {
							engine = GLES::createEngine();
							m_renderEngine = engine;
							helper->dispatchCreateEngine(engine.get());
						}
						if (engine.isNotNull()) {
							if (width > 0 && height > 0) {
								engine->setViewport(0, 0, width, height);
								helper->dispatchFrame(engine.get());
							}
						}
					}
				}
			};

			SLIB_DEFINE_OBJECT(RenderViewInstance, Android_ViewInstance)

			void JNICALL OnCreate(JNIEnv* env, jobject _this, jlong jinstance)
			{
				Ref<RenderViewInstance> instance = CastRef<RenderViewInstance>(Android_ViewInstance::findInstance(jinstance));
				if (instance.isNotNull()) {
					instance->onCreate();
				}
			}

			void JNICALL OnFrame(JNIEnv* env, jobject _this, jlong jinstance, jint width, jint height)
			{
				Ref<RenderViewInstance> instance = CastRef<RenderViewInstance>(Android_ViewInstance::findInstance(jinstance));
				if (instance.isNotNull()) {
					instance->onFrame(width, height);
				}
			}

		}
	}

	using namespace priv::render_view;

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JGLView::create.callObject(sl_null, parent->getContext());
			Ref<RenderViewInstance> ret = Android_ViewInstance::create<RenderViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject jhandle = ret->getHandle();
				JGLView::setRenderMode.callBoolean(sl_null, jhandle, m_redrawMode);
				return ret;
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
