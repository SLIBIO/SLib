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

#if defined(SLIB_UI_IS_IOS) || defined(SLIB_UI_IS_MACOS)

#include "slib/ui/web_view.h"

#if defined(SLIB_UI_IS_IOS)
#	include "view_ios.h"
typedef UIView OSView;
typedef slib::iOS_ViewInstance WebViewInstance;
#else
#	include "view_macos.h"
typedef NSView OSView;
typedef slib::macOS_ViewInstance WebViewInstance;
#endif

#import <WebKit/WebKit.h>

typedef WKWebView OSWebView;

@interface _priv_Slib_Apple_WebView : OSWebView<WKNavigationDelegate, WKUIDelegate, WKScriptMessageHandler>
{
	@public slib::WeakRef<WebViewInstance> m_viewInstance;
}
@end

namespace slib
{
	class _priv_WebView : public WebView
	{
	public:
		void _load(OSWebView* handle)
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
		
		void _applyProperties(OSWebView* handle)
		{
#if defined(SLIB_UI_IS_IOS)
			handle.scrollView.bounces = NO;
#endif
			[handle setCustomUserAgent:(Apple::getNSStringFromString(m_customUserAgent, nil))];
			_load(handle);
		}
		
		void _onStartLoad(OSWebView* handle, NSURL* url)
		{
			dispatchStartLoad(Apple::getStringFromNSString(url.absoluteString));
		}
		
		void _onFinishLoad(OSWebView* handle)
		{
			dispatchFinishLoad(getURL(), sl_false);
		}
		
		void _onLoadError(OSWebView* handle, NSError* error)
		{
			m_lastErrorMessage = Apple::getStringFromNSString([error localizedDescription]);
			if (error != nil && error.userInfo != nil) {
				NSString* url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
				if (url != nil) {
					dispatchFinishLoad(Apple::getStringFromNSString(url), sl_true);
					return;
				}
			}
			dispatchFinishLoad(getURL(), sl_true);
		}
		
		void _onInvokeMethod(OSWebView* handle, id body)
		{
			NSString* s = [NSString stringWithFormat:@"%@", body];
			String msg = Apple::getStringFromNSString(s);
			String param;
			sl_reg index = msg.indexOf("::");
			if (index >= 0) {
				param = msg.substring(index+2);
				msg = msg.substring(0, index);
			}
			if (msg.isNotEmpty()) {
				dispatchMessageFromJavaScript(msg, param);
			}
		}
		
	};
	
#if defined(SLIB_UI_IS_IOS)
	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_Apple_WebView* handle = [[_priv_Slib_Apple_WebView alloc] initWithFrame:frame];
		if (handle != nil) {
			((_priv_WebView*)this)->_applyProperties(handle);
		}
		IOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}
#else
	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_Apple_WebView* handle = [[_priv_Slib_Apple_WebView alloc] initWithFrame:frame];
		if (handle != nil) {
			((_priv_WebView*)this)->_applyProperties(handle);
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}
#endif
	
	void WebView::_refreshSize_NW()
	{
	}
	
	void WebView::_load_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(WebView, _load_NW, this));
			return;
		}
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			((_priv_WebView*)this)->_load(wv);
		}
	}
	
	String WebView::_getURL_NW()
	{
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			NSString* s = wv.URL.absoluteString;
			return Apple::getStringFromNSString(s);
		}
		return m_urlOrigin;
	}
	
	String WebView::_getPageTitle_NW()
	{
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			NSString* title = wv.title;
			return Apple::getStringFromNSString(title);
		}
		return sl_null;
	}
	
	void WebView::_goBack_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(WebView, _goBack_NW, this));
			return;
		}
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv goBack];
		}
	}
	
	void WebView::_goForward_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(WebView, _goForward_NW, this));
			return;
		}
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv goForward];
		}
	}
	
	void WebView::_reload_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(WebView, _reload_NW, this));
			return;
		}
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv reload];
		}
	}
	
	void WebView::_runJavaScript_NW(const String& script)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), WebView, _runJavaScript_NW, this, script));
			return;
		}
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			NSString* _script = Apple::getNSStringFromString(script);
			if (_script != nil && _script.length != 0) {
				[wv evaluateJavaScript:_script completionHandler:nil];
			}
		}
	}
	
	void WebView::_clearCache_NW()
	{
		NSSet* websiteDataTypes = [NSSet setWithArray:@[
														WKWebsiteDataTypeDiskCache,
														WKWebsiteDataTypeOfflineWebApplicationCache,
														WKWebsiteDataTypeMemoryCache,
														WKWebsiteDataTypeLocalStorage,
														//WKWebsiteDataTypeCookies,
														WKWebsiteDataTypeSessionStorage,
														WKWebsiteDataTypeIndexedDBDatabases,
														WKWebsiteDataTypeWebSQLDatabases,
														//WKWebsiteDataTypeFetchCache, //(iOS 11.3, *)
														//WKWebsiteDataTypeServiceWorkerRegistrations, //(iOS 11.3, *)
														]];
		
		NSDate *dateFrom = [NSDate dateWithTimeIntervalSince1970:0];

		[[WKWebsiteDataStore defaultDataStore] removeDataOfTypes:websiteDataTypes modifiedSince:dateFrom completionHandler:^{}];
	}
	
	void WebView::_clearCookie_NW()
	{
		NSSet* websiteDataTypes = [NSSet setWithArray:@[WKWebsiteDataTypeCookies]];
		NSDate *dateFrom = [NSDate dateWithTimeIntervalSince1970:0];
		[[WKWebsiteDataStore defaultDataStore] removeDataOfTypes:websiteDataTypes modifiedSince:dateFrom completionHandler:^{}];
	}
	
	void WebView::_setCustomUserAgent_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(WebView, _setCustomUserAgent_NW, this));
			return;
		}
		OSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv setCustomUserAgent:(Apple::getNSStringFromString(m_customUserAgent, nil))];
		}
	}
}

@implementation _priv_Slib_Apple_WebView

-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setNavigationDelegate:self];
		[self setUIDelegate:self];
		
		[self.configuration.userContentController addScriptMessageHandler:self name:@"slib_send"];
		WKUserScript* script = [[WKUserScript alloc] initWithSource:@"window.slib = {send: function(msg,param){window.webkit.messageHandlers.slib_send.postMessage(msg+'::'+param);}}" injectionTime:WKUserScriptInjectionTimeAtDocumentStart forMainFrameOnly:NO];
		[self.configuration.userContentController addUserScript:script];
	}
	return self;
}

- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(void))completionHandler
{
#if defined(SLIB_UI_IS_IOS)
	UIAlertController *alertController = [UIAlertController alertControllerWithTitle:message message:@"" preferredStyle:UIAlertControllerStyleAlert];
	[alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
		completionHandler();
	}]];
	UIWindow* window = slib::UIPlatform::getKeyWindow();
	if (window != nil) {
		UIViewController* rootController = [window rootViewController];
		[rootController presentViewController:alertController animated:YES completion:nil];
	}
#else
	NSAlert *alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle:@"OK"];
	[alert setMessageText:message];
	[alert runModal];
	completionHandler();
#endif
}

- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL result))completionHandler
{
#if defined(SLIB_UI_IS_IOS)
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
#else
	NSAlert* alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setMessageText:message];
	BOOL ret = ([alert runModal] == NSAlertFirstButtonReturn);
	completionHandler(ret);
#endif
}

- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(nullable NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString * __nullable result))completionHandler
{
#if defined(SLIB_UI_IS_IOS)
	UIAlertController *alertController = [UIAlertController alertControllerWithTitle:prompt message:@"" preferredStyle:UIAlertControllerStyleAlert];
	__weak UIAlertController* _alertController = alertController;
	[alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
		textField.text = defaultText;
	}];
	[alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
		UIAlertController* c = _alertController;
		if (c != nil) {
			NSString *input = ((UITextField *)c.textFields.firstObject).text;
			completionHandler(input);
		} else {
			completionHandler(@"");
		}
	}]];
	[alertController addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
		completionHandler(nil);
	}]];
	UIWindow* window = slib::UIPlatform::getKeyWindow();
	if (window != nil) {
		UIViewController* rootController = [window rootViewController];
		[rootController presentViewController:alertController animated:YES completion:nil];
	}
#else
	NSAlert* alert = [NSAlert alertWithMessageText:prompt defaultButton:@"OK" alternateButton:@"Cancel" otherButton:nil informativeTextWithFormat:@""];
	NSTextField* input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 24)];
	[input setStringValue:defaultText];
	[alert setAccessoryView:input];
	NSInteger button = [alert runModal];
	NSString* ret = nil;
	if (button == NSAlertDefaultReturn) {
		[input validateEditing];
		ret = [input stringValue];
	}
	completionHandler(ret);
#endif
}

- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
}

- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
}

- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
	if (navigationAction.targetFrame != nil && navigationAction.targetFrame.mainFrame) {
		slib::Ref<WebViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			slib::Ref<slib::View> _view = instance->getView();
			if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
				view->_onStartLoad(self, navigationAction.request.URL);
			}
		}
	}
	decisionHandler(WKNavigationActionPolicyAllow);
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation
{
	slib::Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onFinishLoad(self);
		}
	}
}

- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	slib::Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onLoadError(self, error);
		}
	}
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	slib::Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onLoadError(self, error);
		}
	}
}

- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
	id body = message.body;
	slib::Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onInvokeMethod(self, body);
		}
	}
}

- (BOOL)becomeFirstResponder
{
	slib::Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onSetFocus();
	}
	return [super becomeFirstResponder];
}

@end

#endif
