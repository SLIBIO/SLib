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

#include "../../../inc/slib/ui/web_view.h"

#include "view_android.h"

namespace slib
{

	class _WebView : public WebView
	{
	public:
		void __load(jobject handle);

		static void JNICALL nativeOnStartLoad(JNIEnv* env, jobject _this, jlong instance, jstring jurl)
		{
			Ref<View> _view = Android_ViewInstance::findView(instance);
			if (_WebView* view = CastInstance<_WebView>(_view.get())) {
				String url = Jni::getString(jurl);
				view->dispatchStartLoad(url);
			}
		}

		static void JNICALL nativeOnFinishLoad(JNIEnv* env, jobject _this, jlong instance, jstring jurl)
		{
			Ref<View> _view = Android_ViewInstance::findView(instance);
			if (_WebView* view = CastInstance<_WebView>(_view.get())) {
				String url = Jni::getString(jurl);
				view->dispatchFinishLoad(url, sl_false);
			}
		}

		static void JNICALL nativeOnErrorLoad(JNIEnv* env, jobject _this, jlong instance, jstring jurl, jstring jerror)
		{
			Ref<View> _view = Android_ViewInstance::findView(instance);
			if (_WebView* view = CastInstance<_WebView>(_view.get())) {
				view->m_lastErrorMessage = Jni::getString(jerror);
				String url = Jni::getString(jurl);
				view->dispatchFinishLoad(url, sl_true);
			}
		}

		static void JNICALL nativeOnMessage(JNIEnv* env, jobject _this, jlong instance, jstring jmsg, jstring jparam)
		{
			Ref<View> _view = Android_ViewInstance::findView(instance);
			if (_WebView* view = CastInstance<_WebView>(_view.get())) {
				String msg = Jni::getString(jmsg);
				if (msg.isNotEmpty()) {
					String param = Jni::getString(jparam);
					view->dispatchMessageFromJavaScript(msg, param);
				}
			}
		}
	};

	SLIB_JNI_BEGIN_CLASS(_JAndroidWebView, "slib/platform/android/ui/view/UiWebView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiWebView;");

		SLIB_JNI_STATIC_METHOD(load, "_load", "(Landroid/view/View;Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(loadHTML, "_loadHTML", "(Landroid/view/View;Ljava/lang/String;Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(getURL, "_getURL", "(Landroid/view/View;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getTitle, "_getTitle", "(Landroid/view/View;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(goBack, "_goBack", "(Landroid/view/View;)V");
		SLIB_JNI_STATIC_METHOD(goForward, "_goForward", "(Landroid/view/View;)V");
		SLIB_JNI_STATIC_METHOD(reload, "_reload", "(Landroid/view/View;)V");
		SLIB_JNI_STATIC_METHOD(runJavaScript, "_runJavaScript", "(Landroid/view/View;Ljava/lang/String;)V");

		SLIB_JNI_NATIVE(nativeOnStartLoad, "nativeOnStartLoad", "(JLjava/lang/String;)V", _WebView::nativeOnStartLoad);
		SLIB_JNI_NATIVE(nativeOnFinishLoad, "nativeOnFinishLoad", "(JLjava/lang/String;)V", _WebView::nativeOnFinishLoad);
		SLIB_JNI_NATIVE(nativeOnErrorLoad, "nativeOnErrorLoad", "(JLjava/lang/String;Ljava/lang/String;)V", _WebView::nativeOnErrorLoad);
		SLIB_JNI_NATIVE(nativeOnMessage, "nativeOnMessage", "(JLjava/lang/String;Ljava/lang/String;)V", _WebView::nativeOnMessage);

	SLIB_JNI_END_CLASS

	void _WebView::__load(jobject handle)
	{
		JniLocal<jstring> jurl = Jni::getJniString(m_urlOrigin);
		if (m_flagOfflineContent) {
			JniLocal<jstring> jcontent = Jni::getJniString(m_offlineContentHTML);
			_JAndroidWebView::loadHTML.call(sl_null, handle, jcontent.get(), jurl.get());
		} else {
			_JAndroidWebView::load.call(sl_null, handle, jurl.get());
		}
	}

	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = _JAndroidWebView::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((_WebView*)this)->__load(handle);
			}
		}
		return ret;
	}

	void WebView::_refreshSize_NW()
	{
	}

	void WebView::_load_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_WebView*)this)->__load(handle);
		}
	}

	String WebView::_getURL_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			return _JAndroidWebView::getURL.callString(sl_null, handle);
		}
		return sl_null;
	}

	String WebView::_getPageTitle_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			return _JAndroidWebView::getTitle.callString(sl_null, handle);
		}
		return sl_null;
	}

	void WebView::_goBack_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			_JAndroidWebView::goBack.call(sl_null, handle);
		}
	}

	void WebView::_goForward_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			_JAndroidWebView::goForward.call(sl_null, handle);
		}
	}

	void WebView::_reload_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			_JAndroidWebView::reload.call(sl_null, handle);
		}
	}

	void WebView::_runJavaScript_NW(const String& script)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jscript = Jni::getJniString(script);
			_JAndroidWebView::runJavaScript.call(sl_null, handle, jscript.get());
		}
	}

}

#endif
