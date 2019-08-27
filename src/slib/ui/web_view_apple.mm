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
typedef slib::iOS_ViewInstance BaseInstance;
#else
#	include "view_macos.h"
typedef slib::macOS_ViewInstance BaseInstance;
#endif

#import <WebKit/WebKit.h>

namespace slib
{
	namespace priv
	{
		namespace web_view
		{
			class WebViewInstance;
		}
	}
}

@interface SLIBWebViewHandle : WKWebView<WKNavigationDelegate, WKUIDelegate, WKScriptMessageHandler>
{
	@public slib::WeakRef<slib::priv::web_view::WebViewInstance> m_viewInstance;
}
@end

namespace slib
{

	namespace priv
	{
		namespace web_view
		{

			class WebViewHelper : public WebView
			{
			public:
				void load(WKWebView* handle)
				{
					NSString* strURL = Apple::getNSStringFromString(m_urlOrigin);
					WKWebView* mainFrame = handle;
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
				
				void apply(WKWebView* handle)
				{
#if defined(SLIB_UI_IS_IOS)
					handle.scrollView.bounces = NO;
#endif
					[handle setCustomUserAgent:(Apple::getNSStringFromString(m_customUserAgent, nil))];
					load(handle);
				}
				
			};
			
			class WebViewInstance : public BaseInstance, public IWebViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				WKWebView* getHandle()
				{
					return (WKWebView*)m_handle;
				}
				
				Ref<WebViewHelper> getHelper()
				{
					return CastRef<WebViewHelper>(getView());
				}
				
				void refreshSize(WebView* view) override
				{
				}
				
				void load(WebView* view) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						static_cast<WebViewHelper*>(view)->load(handle);
					}
				}
				
				sl_bool getURL(WebView* view, String& _out) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						NSString* s = handle.URL.absoluteString;
						_out = Apple::getStringFromNSString(s);
						return sl_true;
					}
					return sl_false;
				}
				
				sl_bool getPageTitle(WebView* view, String& _out) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						NSString* title = handle.title;
						_out = Apple::getStringFromNSString(title);
						return sl_true;
					}
					return sl_false;
				}
				
				void goBack(WebView* view) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						[handle goBack];
					}
				}
				
				void goForward(WebView* view) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						[handle goForward];
					}
				}
				
				void reload(WebView* view) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						[handle reload];
					}
				}
				
				void runJavaScript(WebView* view, const String& script) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						NSString* _script = Apple::getNSStringFromString(script);
						if (_script != nil && _script.length != 0) {
							[handle evaluateJavaScript:_script completionHandler:nil];
						}
					}
				}
				
				void setCustomUserAgent(WebView* view, const String& agent) override
				{
					WKWebView* handle = getHandle();
					if (handle != nil) {
						[handle setCustomUserAgent:(Apple::getNSStringFromString(agent, nil))];
					}
				}
				
				void onStartLoad(WKWebView* handle, NSURL* url)
				{
					Ref<WebViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						helper->dispatchStartLoad(Apple::getStringFromNSString(url.absoluteString));
					}
				}
				
				void onFinishLoad(WKWebView* handle)
				{
					Ref<WebViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						String url = Apple::getStringFromNSString(handle.URL.absoluteString);
						helper->dispatchFinishLoad(url, sl_false);
					}
				}
				
				void onLoadError(WKWebView* handle, NSError* error)
				{
					Ref<WebViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						String url = Apple::getStringFromNSString(error.userInfo[NSURLErrorFailingURLStringErrorKey]);
						if (url.isNull()) {
							url = Apple::getStringFromNSString(handle.URL.absoluteString);
						}
						helper->dispatchFinishLoad(url, sl_true);
					}
				}
				
				void onInvokeMethod(WKWebView* handle, id body)
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
						Ref<WebViewHelper> helper = getHelper();
						if (helper.isNotNull()) {
							helper->dispatchMessageFromJavaScript(msg, param);
						}
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(WebViewInstance, BaseInstance)

		}
	}

	using namespace priv::web_view;

	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* parent)
	{
		Ref<WebViewInstance> ret = BaseInstance::create<WebViewInstance, SLIBWebViewHandle>(this, parent);
		if (ret.isNotNull()) {
			WKWebView* handle = ret->getHandle();
			static_cast<WebViewHelper*>(this)->apply(handle);
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IWebViewInstance> WebView::getWebViewInstance()
	{
		return CastRef<WebViewInstance>(getViewInstance());
	}
	
	
	void DefaultWebViewProvider::clearCache()
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
	
	void DefaultWebViewProvider::clearCookie()
	{
		NSSet* websiteDataTypes = [NSSet setWithArray:@[WKWebsiteDataTypeCookies]];
		NSDate *dateFrom = [NSDate dateWithTimeIntervalSince1970:0];
		[[WKWebsiteDataStore defaultDataStore] removeDataOfTypes:websiteDataTypes modifiedSince:dateFrom completionHandler:^{}];
	}
	
}

using namespace slib;
using namespace slib::priv::web_view;

@implementation SLIBWebViewHandle

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
	UIViewController* rootController = UIPlatform::getCurrentViewController();
	if (rootController != nil) {
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
	UIViewController* rootController = UIPlatform::getCurrentViewController();
	if (rootController != nil) {
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
	UIViewController* rootController = UIPlatform::getCurrentViewController();
	if (rootController != nil) {
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
		Ref<WebViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			instance->onStartLoad(self, navigationAction.request.URL);
		}
	}
	decisionHandler(WKNavigationActionPolicyAllow);
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation
{
	Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onFinishLoad(self);
	}
}

- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onLoadError(self, error);
	}
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
	Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onLoadError(self, error);
	}
}

- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
	id body = message.body;
	Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onInvokeMethod(self, body);
	}
}

- (BOOL)becomeFirstResponder
{
	Ref<WebViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onSetFocus();
	}
	return [super becomeFirstResponder];
}

@end

#endif
