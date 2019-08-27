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

#include "slib/ui/web_view.h"

#include "view_android.h"

namespace slib
{

	namespace priv
	{
		namespace web_view
		{

			class WebViewHelper : public WebView
			{
			public:
				void load(jobject handle);

				static void JNICALL nativeOnStartLoad(JNIEnv* env, jobject _this, jlong instance, jstring jurl)
				{
					Ref<WebViewHelper> helper = CastRef<WebViewHelper>(Android_ViewInstance::findView(instance));
					if (helper.isNotNull()) {
						String url = Jni::getString(jurl);
						helper->dispatchStartLoad(url);
					}
				}

				static void JNICALL nativeOnFinishLoad(JNIEnv* env, jobject _this, jlong instance, jstring jurl)
				{
					Ref<WebViewHelper> helper = CastRef<WebViewHelper>(Android_ViewInstance::findView(instance));
					if (helper.isNotNull()) {
						String url = Jni::getString(jurl);
						helper->dispatchFinishLoad(url, sl_false);
					}
				}

				static void JNICALL nativeOnErrorLoad(JNIEnv* env, jobject _this, jlong instance, jstring jurl, jstring jerror)
				{
					Ref<WebViewHelper> helper = CastRef<WebViewHelper>(Android_ViewInstance::findView(instance));
					if (helper.isNotNull()) {
						helper->m_lastErrorMessage = Jni::getString(jerror);
						String url = Jni::getString(jurl);
						helper->dispatchFinishLoad(url, sl_true);
					}
				}

				static void JNICALL nativeOnMessage(JNIEnv* env, jobject _this, jlong instance, jstring jmsg, jstring jparam)
				{
					Ref<WebViewHelper> helper = CastRef<WebViewHelper>(Android_ViewInstance::findView(instance));
					if (helper.isNotNull()) {
						String msg = Jni::getString(jmsg);
						if (msg.isNotEmpty()) {
							String param = Jni::getString(jparam);
							helper->dispatchMessageFromJavaScript(msg, param);
						}
					}
				}
			};

			SLIB_JNI_BEGIN_CLASS(JWebView, "slib/platform/android/ui/view/UiWebView")

				SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiWebView;");

				SLIB_JNI_STATIC_METHOD(load, "_load", "(Landroid/view/View;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(loadHTML, "_loadHTML", "(Landroid/view/View;Ljava/lang/String;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(getURL, "_getURL", "(Landroid/view/View;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getTitle, "_getTitle", "(Landroid/view/View;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(goBack, "_goBack", "(Landroid/view/View;)V");
				SLIB_JNI_STATIC_METHOD(goForward, "_goForward", "(Landroid/view/View;)V");
				SLIB_JNI_STATIC_METHOD(reload, "_reload", "(Landroid/view/View;)V");
				SLIB_JNI_STATIC_METHOD(runJavaScript, "_runJavaScript", "(Landroid/view/View;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(setCustomUserAgent, "_setCustomUserAgent", "(Landroid/view/View;Ljava/lang/String;)V");

				SLIB_JNI_STATIC_METHOD(clearCache, "_clearCache", "(Landroid/app/Activity;)V");
				SLIB_JNI_STATIC_METHOD(clearCookie, "_clearCookie", "(Landroid/app/Activity;)V");

				SLIB_JNI_NATIVE(nativeOnStartLoad, "nativeOnStartLoad", "(JLjava/lang/String;)V", WebViewHelper::nativeOnStartLoad);
				SLIB_JNI_NATIVE(nativeOnFinishLoad, "nativeOnFinishLoad", "(JLjava/lang/String;)V", WebViewHelper::nativeOnFinishLoad);
				SLIB_JNI_NATIVE(nativeOnErrorLoad, "nativeOnErrorLoad", "(JLjava/lang/String;Ljava/lang/String;)V", WebViewHelper::nativeOnErrorLoad);
				SLIB_JNI_NATIVE(nativeOnMessage, "nativeOnMessage", "(JLjava/lang/String;Ljava/lang/String;)V", WebViewHelper::nativeOnMessage);

			SLIB_JNI_END_CLASS

			void WebViewHelper::load(jobject handle)
			{
				JniLocal<jstring> jurl = Jni::getJniString(m_urlOrigin);
				if (m_flagOfflineContent) {
					JniLocal<jstring> jcontent = Jni::getJniString(m_offlineContentHTML);
					JWebView::loadHTML.call(sl_null, handle, jcontent.get(), jurl.get());
				} else {
					JWebView::load.call(sl_null, handle, jurl.get());
				}
			}

			class WebViewInstance : public Android_ViewInstance, public IWebViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				void refreshSize(WebView* view) override
				{
				}

				void load(WebView* view) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						(static_cast<WebViewHelper*>(view))->load(handle);
					}
				}

				sl_bool getURL(WebView* view, String& _out) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						_out = JWebView::getURL.callString(sl_null, handle);
						return sl_true;
					}
					return sl_false;
				}

				sl_bool getPageTitle(WebView* view, String& _out) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						_out = JWebView::getTitle.callString(sl_null, handle);
						return sl_true;
					}
					return sl_false;
				}

				void goBack(WebView* view) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JWebView::goBack.call(sl_null, handle);
					}
				}

				void goForward(WebView* view) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JWebView::goForward.call(sl_null, handle);
					}
				}

				void reload(WebView* view) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JWebView::reload.call(sl_null, handle);
					}
				}

				void runJavaScript(WebView* view, const String& script) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JniLocal<jstring> jscript = Jni::getJniString(script);
						JWebView::runJavaScript.call(sl_null, handle, jscript.get());
					}
				}

				void setCustomUserAgent(WebView* view, const String& agent) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JniLocal<jstring> jvalue = Jni::getJniString(agent);
						JWebView::setCustomUserAgent.call(sl_null, handle, jvalue.get());
					}
				}

			};

			SLIB_DEFINE_OBJECT(WebViewInstance, Android_ViewInstance)

		}
	}

	using namespace priv::web_view;

	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* _parent)
	{
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JWebView::create.callObject(sl_null, parent->getContext());
			Ref<WebViewInstance> ret = Android_ViewInstance::create<WebViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				ret->setCustomUserAgent(this, getCustomUserAgent());
				ret->load(this);
				return ret;
			}
		}
		return sl_null;
	}

	Ptr<IWebViewInstance> WebView::getWebViewInstance()
	{
		return CastRef<WebViewInstance>(getViewInstance());
	}


	void DefaultWebViewProvider::clearCache()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JWebView::clearCache.call(sl_null, jactivity);
		}
	}

	void DefaultWebViewProvider::clearCookie()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JWebView::clearCookie.call(sl_null, jactivity);
		}
	}
}

#endif
