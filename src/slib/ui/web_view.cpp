#include "../../../inc/slib/ui/web_view.h"

SLIB_UI_NAMESPACE_BEGIN
WebView::WebView()
{
	m_flagOfflineContent = sl_false;
}

void WebView::loadURL(const String& url)
{
	m_flagOfflineContent = sl_false;
	m_offlineContentHTML.setNull();
	m_urlOrigin = url;
	_load();
}

void WebView::loadHTML(const String& html, const String& baseURL)
{
	m_flagOfflineContent = sl_true;
	m_offlineContentHTML = html;
	m_urlOrigin = baseURL;
	_load();
}

sl_bool WebView::isOfflineContent()
{
	return m_flagOfflineContent;
}

String WebView::getOriginURL()
{
	return m_urlOrigin;
}

String WebView::getErrorMessage()
{
	return m_lastErrorMessage;
}

void WebView::onStartLoad(const String& url)
{
	PtrLocker<IWebViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onStartLoad(this, url);
	}
}

void WebView::onFinishLoad(const String& url, sl_bool flagFailed)
{
	PtrLocker<IWebViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onFinishLoad(this, url, flagFailed);
	}
}

void WebView::onMessageFromJavaScript(const String& msg, const String& param)
{
	PtrLocker<IWebViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onMessageFromJavaScript(this, msg, param);
	}
}

void IWebViewListener::onStartLoad(WebView* view, const String& url)
{
}

void IWebViewListener::onFinishLoad(WebView* view, const String& url, sl_bool flagFailed)
{
}

void IWebViewListener::onMessageFromJavaScript(WebView* view, const String& msg, const String& param)
{
}
SLIB_UI_NAMESPACE_END
