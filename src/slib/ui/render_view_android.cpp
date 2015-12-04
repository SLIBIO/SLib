#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"

#include "view_android.h"

SLIB_UI_NAMESPACE_BEGIN

class _RenderViewInstance : public Android_ViewInstance
{
public:
	Ref<RenderEngine> m_renderEngine;
};

void JNICALL _AndroidGLView_nativeOnCreate(JNIEnv* env, jobject _this, jlong jinstance)
{
	Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(Android_ViewInstance::getAndroidInstance(jinstance));
	if (instance.isNotNull()) {
		Ref<RenderEngine> engine = instance->m_renderEngine;
		if (engine.isNotNull()) {
			engine->release();
		}
		instance->m_renderEngine.setNull();
	}
}

void JNICALL _AndroidGLView_nativeOnFrame(JNIEnv* env, jobject _this, jlong jinstance, jint width, jint height)
{
	Ref<_RenderViewInstance> instance = Ref<_RenderViewInstance>::from(Android_ViewInstance::getAndroidInstance(jinstance));
	if (instance.isNotNull()) {
		Ref<View> _view = instance->getView();
		if (RenderView::checkInstance(_view)) {
			RenderView* view = (RenderView*)(_view.get());
			Ref<RenderEngine> engine = instance->m_renderEngine;
			if (engine.isNull()) {
				engine = GLES::createEngine();
				instance->m_renderEngine = engine;
			}
			if (engine.isNotNull()) {
				if (width > 0 && height > 0) {
					engine->setViewport(0, 0, width, height);
					view->dispatchOnFrame(engine.get());
				}
			}
		}
	}
}

SLIB_JNI_BEGIN_CLASS(_JAndroidGLView, "slib/platform/android/ui/view/UiGLView")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiGLView;");

	SLIB_JNI_STATIC_METHOD(setRenderMode, "_setRenderMode", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(requestRender, "_requestRender", "(Landroid/view/View;)V");

	SLIB_JNI_NATIVE(nativeOnCreate, "nativeOnCreate", "(J)V", _AndroidGLView_nativeOnCreate);
	SLIB_JNI_NATIVE(nativeOnFrame, "nativeOnFrame", "(JII)V", _AndroidGLView_nativeOnFrame);
SLIB_JNI_END_CLASS

Ref<ViewInstance> RenderView::createInstance(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidGLView::create.callObject(sl_null, parent->getContext());
		ret = Android_ViewInstance::create<_RenderViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			_JAndroidGLView::setRenderMode.callBoolean(sl_null, handle, m_renderMode);
		}
	}
	return Ref<ViewInstance>::from(ret);
}

void RenderView::setRenderMode(RenderView::RenderMode mode)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		if (_JAndroidGLView::setRenderMode.callBoolean(sl_null, handle, mode) != 0) {
			m_renderMode = mode;
		}
	} else {
		m_renderMode = mode;
	}
}

void RenderView::requestRender()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidGLView::requestRender.call(sl_null, handle);
	}
}

SLIB_UI_NAMESPACE_END

#endif
