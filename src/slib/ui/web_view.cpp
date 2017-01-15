#include "../../../inc/slib/ui/web_view.h"

SLIB_UI_NAMESPACE_BEGIN

void IWebViewListener::onStartLoad(WebView* view, const String& url)
{
}

void IWebViewListener::onFinishLoad(WebView* view, const String& url, sl_bool flagFailed)
{
}

void IWebViewListener::onMessageFromJavaScript(WebView* view, const String& msg, const String& param)
{
}


SLIB_DEFINE_OBJECT(WebView, View)

WebView::WebView()
{
	setCreatingNativeWidget(sl_true);
	m_flagOfflineContent = sl_false;
}

void WebView::loadURL(const String& url)
{
	m_flagOfflineContent = sl_false;
	m_offlineContentHTML.setNull();
	m_urlOrigin = url;
	if (isNativeWidget()) {
		_load_NW();
	}
}

void WebView::loadHTML(const String& html, const String& baseURL)
{
	m_flagOfflineContent = sl_true;
	m_offlineContentHTML = html;
	m_urlOrigin = baseURL;
	if (isNativeWidget()) {
		_load_NW();
	}
}

sl_bool WebView::isOfflineContent()
{
	return m_flagOfflineContent;
}

String WebView::getOriginURL()
{
	return m_urlOrigin;
}

String WebView::getURL()
{
	if (isNativeWidget()) {
		return _getURL_NW();
	}
	return sl_null;
}

String WebView::getPageTitle()
{
	if (isNativeWidget()) {
		return _getPageTitle_NW();
	}
	return sl_null;
}

void WebView::goBack()
{
	if (isNativeWidget()) {
		_goBack_NW();
	}
}

void WebView::goForward()
{
	if (isNativeWidget()) {
		_goForward_NW();
	}
}

void WebView::reload()
{
	if (isNativeWidget()) {
		_reload_NW();
	}
}

void WebView::runJavaScript(const String& script)
{
	if (isNativeWidget()) {
		_runJavaScript_NW(script);
	}
}

String WebView::getErrorMessage()
{
	return m_lastErrorMessage;
}

void WebView::onStartLoad(const String& url)
{
}

void WebView::onFinishLoad(const String& url, sl_bool flagFailed)
{
}

void WebView::onMessageFromJavaScript(const String& msg, const String& param)
{
}

void WebView::onResize(sl_ui_len width, sl_ui_len height)
{
	if (isNativeWidget()) {
		_refreshSize_NW();
	}
}

void WebView::dispatchStartLoad(const String& url)
{
	onStartLoad(url);
	PtrLocker<IWebViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onStartLoad(this, url);
	}
	getOnStartLoad()(url);
}

void WebView::dispatchFinishLoad(const String& url, sl_bool flagFailed)
{
	onFinishLoad(url, flagFailed);
	PtrLocker<IWebViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onFinishLoad(this, url, flagFailed);
	}
	getOnFinishLoad()(url, flagFailed);
}

void WebView::dispatchMessageFromJavaScript(const String& msg, const String& param)
{
	onMessageFromJavaScript(msg, param);
	PtrLocker<IWebViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onMessageFromJavaScript(this, msg, param);
	}
	getOnMessageFromJavaScript()(msg, param);
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
}

void WebView::_refreshSize_NW()
{
}

void WebView::_load_NW()
{
}

String WebView::_getURL_NW()
{
	return sl_null;
}

String WebView::_getPageTitle_NW()
{
	return sl_null;
}

void WebView::_goBack_NW()
{
}

void WebView::_goForward_NW()
{
}

void WebView::_reload_NW()
{
}

void WebView::_runJavaScript_NW(const String& script)
{
}

#endif

SLIB_UI_NAMESPACE_END
