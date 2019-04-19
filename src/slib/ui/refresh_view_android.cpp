/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/ui/refresh_view.h"

#include "view_android.h"

namespace slib
{

	void JNICALL _priv_AndroidRefreshView_nativeOnRefresh(JNIEnv* env, jobject _this, jlong instance);

	SLIB_JNI_BEGIN_CLASS(JAndroidRefreshView, "slib/platform/android/ui/view/UiRefreshView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Landroid/view/View;");

		SLIB_JNI_STATIC_METHOD(setRefreshing, "_setRefreshing", "(Landroid/view/View;Z)V");

		SLIB_JNI_NATIVE(nativeOnRefresh, "nativeOnRefresh", "(J)V", _priv_AndroidRefreshView_nativeOnRefresh);

	SLIB_JNI_END_CLASS

	class _priv_RefreshView : public RefreshView
	{
	public:
		void _applyProperties(jobject handle, ViewInstance* viewInstance)
		{
		}

		void _onRefresh()
		{
			_onRefresh_NW();
		}
	};

	void JNICALL _priv_AndroidRefreshView_nativeOnRefresh(JNIEnv* env, jobject _this, jlong instance)
	{
		Ref<View> _view = Android_ViewInstance::findView(instance);
		if (_priv_RefreshView* view = CastInstance<_priv_RefreshView>(_view.get())) {
			view->_onRefresh();
		}
	}

	Ref<ViewInstance> RefreshView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidRefreshView::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((_priv_RefreshView*)this)->_applyProperties(handle, ret.get());
			}
		}
		return ret;
	}

	void RefreshView::_setRefreshing_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidRefreshView::setRefreshing.call(sl_null, handle, flag ? 1 : 0);
		}
	}

}

#endif
