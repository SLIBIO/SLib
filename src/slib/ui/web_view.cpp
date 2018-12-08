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

#ifndef SLIB_UI_IS_GTK

#include "slib/ui/web_view.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(WebView, View)
	
	WebView::WebView()
	{
		setCreatingNativeWidget(sl_true);
		m_flagOfflineContent = sl_false;
		m_flagClearCacheOnAttach = sl_false;
	}
	
	WebView::~WebView()
	{
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
	
	void WebView::clearCache()
	{
#if defined(SLIB_PLATFORM_IS_APPLE) || defined(SLIB_PLATFORM_IS_WIN32)
		_clearCache_NW();
#else
		if (isNativeWidget()) {
			_clearCache_NW();
		} else {
			m_flagClearCacheOnAttach = sl_true;
		}
#endif
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
		m_customUserAgent = userAgent;
		_setCustomUserAgent_NW();
	}
	
	void WebView::queryUserAgent(const Function<void(WebView*, String)>& callbackQueryCompletion)
	{
		if (!m_flagOfflineContent && m_urlOrigin.isEmpty()) {
			loadHTML("", "http://localhost");
		}
		m_callbackQueryUserAgentCompletion = callbackQueryCompletion;
		runJavaScript("slib.send('result_query_user_agent', navigator.userAgent)");
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
	
	void WebView::dispatchAttach()
	{
		View::dispatchAttach();
		if (m_flagClearCacheOnAttach) {
			m_flagClearCacheOnAttach = sl_false;
			clearCache();
		}
	}
	
	void WebView::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		View::dispatchResize(width, height);
		if (isNativeWidget()) {
			_refreshSize_NW();
		}
	}
	
	void WebView::dispatchStartLoad(const String& url)
	{
		onStartLoad(url);
		getOnStartLoad()(this, url);
	}
	
	void WebView::dispatchFinishLoad(const String& url, sl_bool flagFailed)
	{
		onFinishLoad(url, flagFailed);
		getOnFinishLoad()(this, url, flagFailed);
		if (!flagFailed && m_callbackQueryUserAgentCompletion.isNotNull()) {
			runJavaScript("slib.send('result_query_user_agent', navigator.userAgent);");
		}
	}
	
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
		onMessageFromJavaScript(msg, param);
		getOnMessageFromJavaScript()(this, msg, param);
	}
	
	
#if !defined(SLIB_UI)
	
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
	
	void WebView::_clearCache_NW()
	{
	}
	
	void WebView::_setCustomUserAgent_NW()
	{
	}
	
#endif

}

#endif
