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

#include "slib/ui/web_view.h"

#include "slib/ui/core.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(WebView, View)
	
	WebView::WebView()
	{
		setCreatingNativeWidget(sl_true);
		setFocusable(sl_true);
		
		m_flagOfflineContent = sl_false;
	}
	
	WebView::~WebView()
	{
	}

	void WebView::loadURL(const String& url)
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::loadURL, url)
		}
		m_flagOfflineContent = sl_false;
		m_offlineContentHTML.setNull();
		m_urlOrigin = url;
		if (instance.isNotNull()) {
			instance->load(this);
		}
	}
	
	void WebView::loadHTML(const String& html, const String& baseURL)
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::loadHTML, html, baseURL)
		}
		m_flagOfflineContent = sl_true;
		m_offlineContentHTML = html;
		m_urlOrigin = baseURL;
		if (instance.isNotNull()) {
			instance->load(this);
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
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			String url;
			if (instance->getURL(this, url)) {
				return url;
			}
		}
		return m_urlOrigin;
	}
	
	String WebView::getPageTitle()
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			String title;
			if (instance->getPageTitle(this, title)) {
				return title;
			}
		}
		return sl_null;
	}
	
	void WebView::goBack()
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::goBack)
			instance->goBack(this);
		}
	}
	
	void WebView::goForward()
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::goForward)
			instance->goForward(this);
		}
	}
	
	void WebView::reload()
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::reload)
			instance->reload(this);
		}
	}
	
	void WebView::runJavaScript(const String& script)
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::runJavaScript, script)
			instance->runJavaScript(this, script);
		}
	}
	
	String WebView::getErrorMessage()
	{
		return m_lastErrorMessage;
	}
	
	String WebView::getCustomUserAgent()
	{
		return m_customUserAgent;
	}
	
	void WebView::setCustomUserAgent(const String& userAgent)
	{
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&WebView::setCustomUserAgent, userAgent)
			m_customUserAgent = userAgent;
			instance->setCustomUserAgent(this, userAgent);
		} else {
			m_customUserAgent = userAgent;
		}
	}
	
	void WebView::queryUserAgent(const Function<void(WebView*, String)>& callbackQueryCompletion)
	{
		if (!m_flagOfflineContent && m_urlOrigin.isEmpty()) {
			loadHTML("", "http://localhost");
		}
		m_callbackQueryUserAgentCompletion = callbackQueryCompletion;
		runJavaScript("slib.send('result_query_user_agent', navigator.userAgent)");
	}

	SLIB_DEFINE_EVENT_HANDLER(WebView, StartLoad, const String& url)
	
	void WebView::dispatchStartLoad(const String& url)
	{
		SLIB_INVOKE_EVENT_HANDLER(StartLoad, url)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(WebView, FinishLoad, const String& url, sl_bool flagFailed)

	void WebView::dispatchFinishLoad(const String& url, sl_bool flagFailed)
	{
		SLIB_INVOKE_EVENT_HANDLER(FinishLoad, url, flagFailed)
		
		if (!flagFailed && m_callbackQueryUserAgentCompletion.isNotNull()) {
			runJavaScript("slib.send('result_query_user_agent', navigator.userAgent);");
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(WebView, MessageFromJavaScript, const String& msg, const String& param)

	void WebView::dispatchMessageFromJavaScript(const String& msg, const String& param)
	{
		if (msg == "result_query_user_agent") {
			Function<void(WebView*, String)> callback = m_callbackQueryUserAgentCompletion;
			if (callback.isNotNull()) {
				callback(this, param);
				m_callbackQueryUserAgentCompletion.setNull();
			}
			return;
		}
		
		SLIB_INVOKE_EVENT_HANDLER(MessageFromJavaScript, msg, param)
	}
	
	void WebView::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		View::dispatchResize(width, height);
		Ptr<IWebViewInstance> instance = getWebViewInstance();
		if (instance.isNotNull()) {
			instance->refreshSize(this);
		}
	}
	
#if !defined(SLIB_UI)
	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	Ptr<IWebViewInstance> WebView::getWebViewInstance()
	{
		return sl_null;
	}
	
	void DefaultWebViewProvider::clearCache()
	{
	}
	
	void DefaultWebViewProvider::clearCookie()
	{
	}
#endif

}
