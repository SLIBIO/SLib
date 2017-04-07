/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#define USE_WK_WEB_VIEW

#if defined(SLIB_PLATFORM_IS_OSX)

#include "slib/ui/web_view.h"

#include "view_osx.h"
#import <WebKit/WebKit.h>

#ifdef USE_WK_WEB_VIEW
typedef WKWebView OSWebView;
@interface _Slib_OSX_WebView : OSWebView<WKNavigationDelegate, WKUIDelegate, WKScriptMessageHandler>
#else
typedef WebView OSWebView;
@interface _Slib_OSX_WebView : OSWebView<WebFrameLoadDelegate, WebUIDelegate>
#endif
{
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
}
@end

namespace slib
{

	class _WebView : public WebView
	{
	public:
		void _load(OSWebView* handle)
		{
			NSString* strURL = Apple::getNSStringFromString(m_urlOrigin);
#ifdef USE_WK_WEB_VIEW
			OSWebView* mainFrame = handle;
#else
			WebFrame* mainFrame = [handle mainFrame];
#endif
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
		
#ifndef USE_WK_WEB_VIEW
		String _callJavaScript(OSWebView* wv, const String& func, const String* params, sl_uint32 nParams)
		{
			WebScriptObject* so = [wv windowScriptObject];
			NSMutableArray* arr = [NSMutableArray new];
			if (params && nParams) {
				for (sl_uint32 i = 0; i < nParams; i++) {
					NSString* param = Apple::getNSStringFromString(params[i]);
					[arr addObject:param];
				}
			}
			NSString* s = Apple::getNSStringFromString(func);
			id ret = [so callWebScriptMethod:s withArguments:arr];
			s = [NSString stringWithFormat:@"%@", ret];
			return Apple::getStringFromNSString(s);
		}

		String _getContentHTML(OSWebView* wv)
		{
			WebFrame* mainFrame = [wv mainFrame];
			WebDataSource *source = [mainFrame dataSource];
			NSData *data = [source data];
			String ret = String::fromUtf8([data bytes], (sl_int32)([data length]));
			return ret;
		}
#endif
	};

	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* _parent)
	{
		OSX_VIEW_CREATE_INSTANCE_BEGIN
		_Slib_OSX_WebView* handle = [[_Slib_OSX_WebView alloc] initWithFrame:frame];
		if (handle != nil) {
			((_WebView*)this)->_applyProperties(handle);
		}
		OSX_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void WebView::_refreshSize_NW()
	{
	}

	void WebView::_load_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			((_WebView*)this)->_load(wv);
		}
	}

	String WebView::_getURL_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
#ifdef USE_WK_WEB_VIEW
			NSString* s = wv.URL.absoluteString;
#else
			NSString* s = [wv mainFrameURL];
#endif
			return Apple::getStringFromNSString(s);
		}
		return m_urlOrigin;
	}

	String WebView::_getPageTitle_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
#ifdef USE_WK_WEB_VIEW
			NSString* title = wv.title;
#else
			WebFrame* mainFrame = [wv mainFrame];
			WebDataSource *source = [mainFrame dataSource];
			NSString* title = [source pageTitle];
#endif
			String ret = Apple::getStringFromNSString(title);
			return ret;
		}
		return sl_null;
	}

	void WebView::_goBack_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv goBack];
		}
	}

	void WebView::_goForward_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv goForward];
		}
	}

	void WebView::_reload_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			[wv reload:nil];
		}
	}

	void WebView::_runJavaScript_NW(const String& script)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[OSWebView class]]) {
			OSWebView* wv = (OSWebView*)handle;
			NSString* _script = Apple::getNSStringFromString(script);
			if (_script != nil && _script.length != 0) {
#ifdef USE_WK_WEB_VIEW
				[wv evaluateJavaScript:_script completionHandler:nil];
#else
				[wv stringByEvaluatingJavaScriptFromString:_script];
#endif
			}
		}
	}

}

@implementation _Slib_OSX_WebView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
#ifdef USE_WK_WEB_VIEW
		[self setNavigationDelegate:self];
		[self setUIDelegate:self];
		
		[self.configuration.userContentController addScriptMessageHandler:self name:@"slib_send"];
		WKUserScript* script = [[WKUserScript alloc] initWithSource:@"window.slib_send=function(msg,param){window.webkit.messageHandlers.slib_send.postMessage(msg+'::'+param);}" injectionTime:WKUserScriptInjectionTimeAtDocumentStart forMainFrameOnly:NO];
		[self.configuration.userContentController addUserScript:script];
#else
		[self setFrameLoadDelegate:self];
		[self setUIDelegate:self];
#endif
	}
	return self;
}

#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(void))completionHandler
#else
- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WebFrame *)frame
#endif
{
	NSAlert *alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle:@"OK"];
	[alert setMessageText:message];
	[alert runModal];
#ifdef USE_WK_WEB_VIEW
	completionHandler();
#endif
}

#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL result))completionHandler
#else
- (BOOL)webView:(WebView *)sender runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WebFrame *)frame
#endif
{
	NSAlert* alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setMessageText:message];
	BOOL ret = ([alert runModal] == NSAlertFirstButtonReturn);
#ifdef USE_WK_WEB_VIEW
	completionHandler(ret);
#else
	return ret;
#endif
}

#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(nullable NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString * __nullable result))completionHandler
#else
- (NSString *)webView:(WebView *)sender runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(NSString *)defaultText initiatedByFrame:(WebFrame *)frame
#endif
{
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
#ifdef USE_WK_WEB_VIEW
	completionHandler(ret);
#else
	return ret;
#endif
}

#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(null_unspecified WKNavigation *)navigation
#else
- (void)webView:(WebView *)sender didReceiveServerRedirectForProvisionalLoadForFrame:(WebFrame *)frame
#endif
{
}

#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(null_unspecified WKNavigation *)navigation
{
#else
- (void)webView:(WebView *)sender didStartProvisionalLoadForFrame:(WebFrame *)frame
{
	if (frame != [self mainFrame]) {
		return;
	}
#endif
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_WebView* view = slib::CastInstance<slib::_WebView>(_view.get())) {
			view->_onStartLoad(self);
		}
	}
}

#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation
{
#else
- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
	if (frame != [self mainFrame]) {
		return;
	}
#endif
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_WebView* view = slib::CastInstance<slib::_WebView>(_view.get())) {
			view->_onFinishLoad(self);
		}
	}
}
	
#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
#else
- (void)webView:(WebView *)sender didFailProvisionalLoadWithError:(NSError *)error forFrame:(WebFrame *)frame
{
	if (frame != [self mainFrame]) {
		return;
	}
#endif
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_WebView* view = slib::CastInstance<slib::_WebView>(_view.get())) {
			view->_onLoadError(self, error);
		}
	}
}


#ifdef USE_WK_WEB_VIEW
- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error
{
#else
- (void)webView:(WebView *)sender didFailLoadWithError:(NSError *)error forFrame:(WebFrame *)frame
{
	if (frame != [self mainFrame]) {
		return;
	}
#endif
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_WebView* view = slib::CastInstance<slib::_WebView>(_view.get())) {
			view->_onLoadError(self, error);
		}
	}
}
	
#ifdef USE_WK_WEB_VIEW
- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
	id body = message.body;
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_WebView* view = slib::CastInstance<slib::_WebView>(_view.get())) {
			view->_onInvokeMethod(self, body);
		}
	}
}
#else
- (void)webView:(WebView *)sender didClearWindowObject:(WebScriptObject *)windowObject forFrame:(WebFrame *)frame
{
	[windowObject setValue:self forKey:@"slib_send"];
}

- (id)invokeDefaultMethodWithArguments:(NSArray *)args
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		slib::Ref<slib::View> _view = instance->getView();
		if (slib::_WebView* view = slib::CastInstance<slib::_WebView>ckInstance(_view.get())) {
			view->_onInvokeMethod(self, args);
		}
	}
	return nil;
}
#endif

@end

#endif
