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
	SLIB_DECLARE_OBJECT(WebView, View)
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
	SLIB_PTR_PROPERTY(IWebViewListener, Listener)
	
public:
	virtual void onStartLoad(const String& url);
	
	virtual void onFinishLoad(const String& url, sl_bool flagFailed);
	
	virtual void onMessageFromJavaScript(const String& msg, const String& param);

protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
	// override
	void onResize();
	
	void _load();
	
protected:
	SafeString m_urlOrigin;
	SafeString m_offlineContentHTML;
	sl_bool m_flagOfflineContent;
	SafeString m_lastErrorMessage;
	
};

SLIB_UI_NAMESPACE_END

#endif
