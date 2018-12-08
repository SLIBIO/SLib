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

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/web_view.h"

#include "view_ios.h"
#import <WebKit/WebKit.h>

typedef WKWebView OSWebView;
@interface _priv_Slib_iOS_WebView : OSWebView<WKNavigationDelegate, WKUIDelegate, WKScriptMessageHandler>
{
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
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
			if (![NSThread isMainThread]) {
				dispatch_async(dispatch_get_main_queue(), ^{
					_applyProperties(handle);
				});
				return;
			}
			handle.scrollView.bounces = NO;
			[handle setCustomUserAgent:(Apple::getNSStringFromString(m_customUserAgent, nil))];
			_load(handle);
		}
		
		void _onStartLoad(OSWebView* handle)
		{
			dispatchStartLoad(getURL());
		}
		
		void _onFinishLoad(OSWebView* handle)
		{
			dispatchFinishLoad(getURL(), sl_false);
		}
		
		void _onLoadError(OSWebView* handle, NSError* error)
		{
			m_lastErrorMessage = Apple::getStringFromNSString([error localizedDescription]);
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
		
		void _onInvokeMethod(OSWebView* handle, NSArray* args)
		{
			sl_uint32 n = (sl_uint32)([args count]);
			List<String> params;
			for (sl_uint32 i = 0; i < n; i++) {
				id val = [args objectAtIndex:i];
				NSString* s = [NSString stringWithFormat:@"%@", val];
				params.add(Apple::getStringFromNSString(s));
			}
			String msg = params.getValueAt(0);
			if (msg.isNotEmpty()) {
				String param = params.getValueAt(0);
				dispatchMessageFromJavaScript(msg, param);
			}
		}
	};
	
	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_iOS_WebView* handle = [[_priv_Slib_iOS_WebView alloc] initWithFrame:frame];
		if (handle != nil) {
			((_priv_WebView*)this)->_applyProperties(handle);
		}
		IOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}
	
	void WebView::_refreshSize_NW()
	{
	}
	
	void WebView::_load_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_load_NW();
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			((_priv_WebView*)this)->_load(wv);
		}
	}
	
	String WebView::_getURL_NW()
	{
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			NSString* s = wv.URL.absoluteString;
			return Apple::getStringFromNSString(s);
		}
		return m_urlOrigin;
	}
	
	String WebView::_getPageTitle_NW()
	{
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			NSString* title = wv.title;
			String ret = Apple::getStringFromNSString(title);
			return ret;
		}
		return sl_null;
	}
	
	void WebView::_goBack_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_goBack_NW();
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv goBack];
		}
	}
	
	void WebView::_goForward_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_goForward_NW();
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv goForward];
		}
	}
	
	void WebView::_reload_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_reload_NW();
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv reload];
		}
	}
	
	void WebView::_runJavaScript_NW(const String& script)
	{
		if (![NSThread isMainThread]) {
			String _script = script;
			dispatch_async(dispatch_get_main_queue(), ^{
				_runJavaScript_NW(_script);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
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
														WKWebsiteDataTypeCookies,
														WKWebsiteDataTypeSessionStorage,
														WKWebsiteDataTypeIndexedDBDatabases,
														WKWebsiteDataTypeWebSQLDatabases,
														//WKWebsiteDataTypeFetchCache, //(iOS 11.3, *)
														//WKWebsiteDataTypeServiceWorkerRegistrations, //(iOS 11.3, *)
														]];
		
		NSDate *dateFrom = [NSDate dateWithTimeIntervalSince1970:0];

		[[WKWebsiteDataStore defaultDataStore] removeDataOfTypes:websiteDataTypes modifiedSince:dateFrom completionHandler:^{
		}];
	}
	
	void WebView::_setCustomUserAgent_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_setCustomUserAgent_NW();
			});
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv setCustomUserAgent:(Apple::getNSStringFromString(m_customUserAgent, nil))];
		}
	}
}

@implementation _priv_Slib_iOS_WebView
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
}

- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
}

- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onStartLoad(self);
		}
	}
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onFinishLoad(self);
		}
	}
}

- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onLoadError(self, error);
		}
	}
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
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
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_priv_WebView* view = slib::CastInstance<slib::_priv_WebView>(_view.get())) {
			view->_onInvokeMethod(self, body);
		}
	}
}

@end

#endif
