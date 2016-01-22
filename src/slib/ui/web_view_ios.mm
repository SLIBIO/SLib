#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/web_view.h"

#include "view_ios.h"
#import <WebKit/WebKit.h>

typedef WKWebView OSWebView;
@interface _Slib_iOS_WebView : OSWebView<WKNavigationDelegate, WKUIDelegate, WKScriptMessageHandler>
{
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
}
@end

SLIB_UI_NAMESPACE_BEGIN

class _WebView : public WebView
{
public:
	void __load(OSWebView* handle)
	{
		NSString* strURL = Apple::getNSStringFromString(m_urlOrigin);
		OSWebView* mainFrame = handle;
		if (m_flagOfflineContent) {
			NSString* content = Apple::getNSStringFromString(m_offlineContentHTML);
			if (strURL == nil || strURL.length == 0) {
				[mainFrame loadHTMLString:content baseURL:nil];
			} else {
				NSURL* url = [NSURL URLWithString:strURL];
				[mainFrame loadHTMLString:content baseURL:url];
			}
		} else {
			if (strURL == nil || strURL.length == 0) {
				return;
			}
			NSURL* url = [NSURL URLWithString:strURL];
			NSURLRequest* request = [NSURLRequest requestWithURL:url];
			[mainFrame loadRequest:request];
		}
	}
	
	void __applyProperties(OSWebView* handle)
	{
		__load(handle);
	}
	
	void __onStartLoad(OSWebView* handle)
	{
		onStartLoad(getURL());
	}
	
	void __onFinishLoad(OSWebView* handle)
	{
		onFinishLoad(getURL(), sl_false);
	}
	
	void __onLoadError(OSWebView* handle, NSError* error)
	{
		m_lastErrorMessage = Apple::getStringFromNSString([error localizedDescription]);
		onFinishLoad(getURL(), sl_true);
	}
	
	void __onInvokeMethod(OSWebView* handle, id body)
	{
		NSString* s = [NSString stringWithFormat:@"%@", body];
		String msg = Apple::getStringFromNSString(s);
		String param;
		sl_int32 index = msg.indexOf("::");
		if (index >= 0) {
			param = msg.substring(index+2);
			msg = msg.substring(0, index);
		}
		if (msg.isNotEmpty()) {
			onMessageFromJavaScript(msg, param);
		}
	}
	
	void __onInvokeMethod(OSWebView* handle, NSArray* args)
	{
		sl_uint32 n = (sl_uint32)([args count]);
		List<String> params;
		for (sl_uint32 i = 0; i < n; i++) {
			id val = [args objectAtIndex:i];
			NSString* s = [NSString stringWithFormat:@"%@", val];
			params.add(Apple::getStringFromNSString(s));
		}
		String msg = params.getItemValue(0, String::null());
		if (msg.isNotEmpty()) {
			String param = params.getItemValue(0, String::null());
			onMessageFromJavaScript(msg, param);
		}
	}
};

Ref<ViewInstance> WebView::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_WebView* handle = [[_Slib_iOS_WebView alloc] initWithFrame:frame];
	if (handle != nil) {
		((_WebView*)this)->__applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

void WebView::onResize()
{
	View::onResize();
}

void WebView::_load()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		((_WebView*)this)->__load(wv);
	}
}

String WebView::getURL()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		NSString* s = wv.URL.absoluteString;
		return Apple::getStringFromNSString(s);
	}
	return m_urlOrigin;
}

String WebView::getPageTitle()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		NSString* title = wv.title;
		String ret = Apple::getStringFromNSString(title);
		return ret;
	}
	return String::null();
}

void WebView::goBack()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		[wv goBack];
	}
}

void WebView::goForward()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		[wv goForward];
	}
}

void WebView::reload()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		[wv reload];
	}
}

void WebView::runJavaScript(const String& script)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
		OSWebView* wv = (OSWebView*)handle;
		NSString* _script = Apple::getNSStringFromString(script);
		if (_script != nil && _script.length != 0) {
			[wv evaluateJavaScript:_script completionHandler:nil];
		}
	}
}
SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_WebView
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setNavigationDelegate:self];
		[self setUIDelegate:self];
		
		[self.configuration.userContentController addScriptMessageHandler:self name:@"slib_send"];
		WKUserScript* script = [[WKUserScript alloc] initWithSource:@"window.slib_send=function(msg,param){window.webkit.messageHandlers.slib_send.postMessage(msg+'::'+param);}" injectionTime:WKUserScriptInjectionTimeAtDocumentStart forMainFrameOnly:NO];
		[self.configuration.userContentController addUserScript:script];
	}
	return self;
}

- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(void))completionHandler
{
	UIAlertController *alertController = [UIAlertController alertControllerWithTitle:message message:@"" preferredStyle:UIAlertControllerStyleAlert];
	[alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
		completionHandler();
	}]];
	UIWindow* window = slib::UIPlatform::getKeyWindow();
	if (window != nil) {
		UIViewController* rootController = [window rootViewController];
		[rootController presentViewController:alertController animated:YES completion:nil];
	}
}

- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL result))completionHandler
{
	UIAlertController *alertController = [UIAlertController alertControllerWithTitle:message message:@"" preferredStyle:UIAlertControllerStyleAlert];
	[alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
		completionHandler(YES);
	}]];
	[alertController addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *) {
		completionHandler(NO);
	 }]];
	UIWindow* window = slib::UIPlatform::getKeyWindow();
	if (window != nil) {
		UIViewController* rootController = [window rootViewController];
		[rootController presentViewController:alertController animated:YES completion:nil];
	}
}

- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(nullable NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString * __nullable result))completionHandler
{
	UIAlertController *alertController = [UIAlertController alertControllerWithTitle:prompt message:@"" preferredStyle:UIAlertControllerStyleAlert];
	[alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
		textField.text = defaultText;
	}];
	[alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
		NSString *input = ((UITextField *)alertController.textFields.firstObject).text;
		completionHandler(input);
	}]];
	[alertController addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
		completionHandler(nil);
	}]];
	UIWindow* window = slib::UIPlatform::getKeyWindow();
	if (window != nil) {
		UIViewController* rootController = [window rootViewController];
		[rootController presentViewController:alertController animated:YES completion:nil];
	}
}

- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
}

- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::WebView::checkInstance(view)) {
			((slib::_WebView*)(view.get()))->__onStartLoad(self);
		}
	}
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::WebView::checkInstance(view)) {
			((slib::_WebView*)(view.get()))->__onFinishLoad(self);
		}
	}
}

- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::WebView::checkInstance(view)) {
			((slib::_WebView*)(view.get()))->__onLoadError(self, error);
		}
	}
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::WebView::checkInstance(view)) {
			((slib::_WebView*)(view.get()))->__onLoadError(self, error);
		}
	}
}

- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
	id body = message.body;
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::WebView::checkInstance(view)) {
			((slib::_WebView*)(view.get()))->__onInvokeMethod(self, body);
		}
	}
}

@end

#endif
