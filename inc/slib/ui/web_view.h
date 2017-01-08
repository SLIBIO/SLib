#ifndef CHECKHEADER_SLIB_UI_WEB_VIEW
#define CHECKHEADER_SLIB_UI_WEB_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class WebView;

class SLIB_EXPORT IWebViewListener
{
public:
	// return sl_false to cancel the loading
	virtual void onStartLoad(WebView* view, const String& url);

	virtual void onFinishLoad(WebView* view, const String& url, sl_bool flagFailed);
	
	// [Javascript] window.slib_send(msg, param);
	virtual void onMessageFromJavaScript(WebView* view, const String& msg, const String& param);
};

class SLIB_EXPORT WebView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	WebView();
	
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
	
public:
	SLIB_PROPERTY(AtomicPtr<IWebViewListener>, Listener)
	
protected:
	virtual void onStartLoad(const String& url);
	
	virtual void onFinishLoad(const String& url, sl_bool flagFailed);
	
	virtual void onMessageFromJavaScript(const String& msg, const String& param);

	// override
	void onResize(sl_ui_len width, sl_ui_len height);
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);

	void dispatchStartLoad(const String& url);
	
	void dispatchFinishLoad(const String& url, sl_bool flagFailed);
	
	void dispatchMessageFromJavaScript(const String& msg, const String& param);

private:
	void _refreshSize_NW();

	void _load_NW();
	
	String _getURL_NW();
	
	String _getPageTitle_NW();
	
	void _goBack_NW();
	
	void _goForward_NW();
	
	void _reload_NW();
	
	void _runJavaScript_NW(const String& script);
	
protected:
	AtomicString m_urlOrigin;
	AtomicString m_offlineContentHTML;
	sl_bool m_flagOfflineContent;
	AtomicString m_lastErrorMessage;
	
};

SLIB_UI_NAMESPACE_END

#endif
