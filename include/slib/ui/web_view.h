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

#ifndef CHECKHEADER_SLIB_UI_WEB_VIEW
#define CHECKHEADER_SLIB_UI_WEB_VIEW

#include "definition.h"

#ifndef SLIB_UI_IS_GTK

#include "view.h"

namespace slib
{
	
	class SLIB_EXPORT WebView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		WebView();
		
		~WebView();

	public:
		virtual void loadURL(const String& url);
		
		virtual void loadHTML(const String& html, const String& baseURL);
		
		sl_bool isOfflineContent();
		
		String getOriginURL();
		
		String getURL();
		
		String getPageTitle();
		
		virtual void goBack();
		
		virtual void goForward();
		
		virtual void reload();
		
		virtual void runJavaScript(const String& script);
		
		virtual void clearCache();
		
		
		String getErrorMessage();
		
		String getCustomUserAgent();
		
		void setCustomUserAgent(const String& userAgent);
		
		void queryUserAgent(const Function<void(WebView*, String)>& callbackQueryCompletion);
		
	public:
		SLIB_PROPERTY(AtomicFunction<void(WebView*, String)>, OnStartLoad)
		
		SLIB_PROPERTY(AtomicFunction<void(WebView*, String, sl_bool)>, OnFinishLoad)
		
		SLIB_PROPERTY(AtomicFunction<void(WebView*, String, String)>, OnMessageFromJavaScript)
		
	protected:
		virtual void onStartLoad(const String& url);
		
		virtual void onFinishLoad(const String& url, sl_bool flagFailed);
		
		virtual void onMessageFromJavaScript(const String& msg, const String& param);
		
		void dispatchAttach() override;
		
		void dispatchResize(sl_ui_len width, sl_ui_len height) override;
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		virtual void dispatchStartLoad(const String& url);
		
		virtual void dispatchFinishLoad(const String& url, sl_bool flagFailed);
		
		virtual void dispatchMessageFromJavaScript(const String& msg, const String& param);
		
	private:
		void _refreshSize_NW();
		
		void _load_NW();
		
		String _getURL_NW();
		
		String _getPageTitle_NW();
		
		void _goBack_NW();
		
		void _goForward_NW();
		
		void _reload_NW();
		
		void _runJavaScript_NW(const String& script);
		
		void _clearCache_NW();
		
		void _setCustomUserAgent_NW();
		
	protected:
		AtomicString m_urlOrigin;
		AtomicString m_offlineContentHTML;
		sl_bool m_flagOfflineContent;
		AtomicString m_lastErrorMessage;
		
		sl_bool m_flagClearCacheOnAttach;
		AtomicString m_customUserAgent;
		AtomicFunction<void(WebView*, String)> m_callbackQueryUserAgentCompletion;

	};

}

#endif

#endif
