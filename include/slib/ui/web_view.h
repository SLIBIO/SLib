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

#ifndef CHECKHEADER_SLIB_UI_WEB_VIEW
#define CHECKHEADER_SLIB_UI_WEB_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{
	
	class IWebViewInstance;
	
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
		
		
		String getErrorMessage();
		
		String getCustomUserAgent();
		
		void setCustomUserAgent(const String& userAgent);
		
		void queryUserAgent(const Function<void(WebView*, String)>& callbackQueryCompletion);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(WebView, StartLoad, const String& url)
		SLIB_DECLARE_EVENT_HANDLER(WebView, FinishLoad, const String& url, sl_bool flagFailed)
		SLIB_DECLARE_EVENT_HANDLER(WebView, MessageFromJavaScript, const String& msg, const String& param)
		
	protected:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;

		virtual Ptr<IWebViewInstance> getWebViewInstance();
		
	public:
		void dispatchResize(sl_ui_len width, sl_ui_len height) override;
		
	protected:
		AtomicString m_urlOrigin;
		AtomicString m_offlineContentHTML;
		sl_bool m_flagOfflineContent;
		AtomicString m_lastErrorMessage;
		
		AtomicString m_customUserAgent;
		AtomicFunction<void(WebView*, String)> m_callbackQueryUserAgentCompletion;

	};
	
	class SLIB_EXPORT IWebViewInstance
	{
	public:
		virtual void refreshSize(WebView* view) = 0;
		
		virtual void load(WebView* view) = 0;
		
		virtual sl_bool getURL(WebView* view, String& _out) = 0;
		
		virtual sl_bool getPageTitle(WebView* view, String& _out) = 0;
		
		virtual void goBack(WebView* view) = 0;
		
		virtual void goForward(WebView* view) = 0;
		
		virtual void reload(WebView* view) = 0;
		
		virtual void runJavaScript(WebView* view, const String& script) = 0;
		
		virtual void setCustomUserAgent(WebView* view, const String& agent) = 0;
		
	};
	
	class SLIB_EXPORT DefaultWebViewProvider
	{
	public:
		static void clearCache();
		
		static void clearCookie();

	};

}

#endif
