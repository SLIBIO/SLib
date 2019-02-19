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

#include "slib/social/oauth.h"

#include "slib/ui/mobile_app.h"
#include "slib/ui/view_page.h"
#include "slib/ui/web_view.h"
#include "slib/ui/button.h"
#include "slib/ui/resource.h"
#include "slib/core/log.h"

#include "../resources.h"

#define TAG "OAuth"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(OAuthLoginResult)
	
	OAuthLoginResult::OAuthLoginResult()
	{
		flagError = sl_true;
		flagCancel = sl_false;
	}
	
	sl_bool OAuthLoginResult::isSuccess() const
	{
		return !(flagError || flagCancel);
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(OAuthLoginParam)
	
	OAuthLoginParam::OAuthLoginParam()
	{
	}
	
	
	class OAuthLoginDialog : public ViewPage
	{
	public:
		Ref<WebView> webView;
		WeakRef<Window> window;
		
	public:
		void init(const String& url, const Function<void(const String& url)>& onRedirectUrl)
		{
			Ref<Button> btnCancel = new Button;
			btnCancel->setCancelOnClick();
			btnCancel->setText(string::cancel::get(), UIUpdateMode::Init);
			btnCancel->setAlignParentLeft(UIUpdateMode::Init);
			btnCancel->setAlignParentTop(UIUpdateMode::Init);
			btnCancel->setWidthWrapping(UIUpdateMode::Init);
			btnCancel->setHeightWrapping(UIUpdateMode::Init);
			btnCancel->setMarginTop(UIResource::getSafeAreaInsetTop(), UIUpdateMode::Init);
#ifdef SLIB_PLATFORM_IS_MOBILE
			sl_real fontSize = (sl_real)(UIResource::getScreenMinimum() / 20);
#else
			sl_real fontSize = 20;
#endif
			btnCancel->setFontSize(fontSize, UIUpdateMode::Init);
			btnCancel->setPadding((sl_ui_pos)(fontSize / 3), UIUpdateMode::Init);
			addChild(btnCancel, UIUpdateMode::Init);
			
			webView = new WebView;
			webView->setAlignParentLeft(UIUpdateMode::Init);
			webView->setBelow(btnCancel, UIUpdateMode::Init);
			webView->setWidthFilling(1, UIUpdateMode::Init);
			webView->setHeightFilling(1, UIUpdateMode::Init);
			webView->loadURL(url);
			webView->setOnStartLoad([onRedirectUrl](WebView*, const String& url) {
				onRedirectUrl(url);
			});
			addChild(webView, UIUpdateMode::Init);
			
			setWidthFilling(1, UIUpdateMode::Init);
			setHeightFilling(1, UIUpdateMode::Init);
			
			setOnBack([onRedirectUrl](View* view, UIEvent* ev) {
				onRedirectUrl(sl_null);
			});
		}
		
	};
	
	void OAuth1::login(const OAuthLoginParam& param)
	{
		auto onComplete = param.onComplete;
		auto weak = ToWeakRef(this);
		getLoginUrl(param.requestTokenParams, [weak, onComplete](const String& url, const String& token, const String& secret) {
			if (url.isEmpty()) {
				OAuthLoginResult result;
				onComplete(result);
				return;
			}
			Ref<OAuthLoginDialog> dlg = new OAuthLoginDialog;
			if (dlg.isNull()) {
				OAuthLoginResult result;
				onComplete(result);
				return;
			}
			auto weakDlg = ToWeakRef(dlg);
			dlg->init(url, [weak, weakDlg, secret, onComplete](const String& url) {
				auto thiz = ToRef(weak);
				if (thiz.isNull()) {
					OAuthLoginResult result;
					onComplete(result);
					return;
				}
				if (url.isEmpty()) {
					OAuthLoginResult result;
					result.flagError = sl_false;
					result.flagCancel = sl_true;
					onComplete(result);
					return;
				}
				if (url.startsWith(thiz->m_callbackUrl)) {
					Log(TAG, "Redirected to Callback URL: %s", url);
					auto params = Url(url).getQueryParameters();
					String token = params["oauth_token"];
					String tokenVerifier = params["oauth_verifier"];
					if (token.isEmpty() || tokenVerifier.isEmpty()) {
						OAuthLoginResult result;
						onComplete(result);
						return;
					}
					UrlRequestParam request;
					request.method = HttpMethod::POST;
					request.url = thiz->m_accessTokenUrl;
					request.setRequestBodyAsString("oauth_verifier=" + Url::encodeUriComponentByUTF8(tokenVerifier));
					thiz->authorizeRequest(request, token, secret, sl_null);
					request.onComplete = [weak, weakDlg, onComplete](UrlRequest* request) {
						auto thiz = ToRef(weak);
						if (thiz.isNull()) {
							OAuthLoginResult result;
							onComplete(result);
							return;
						}
						if (request->isError()) {
							logUrlRequestError(request);
							OAuthLoginResult result;
							onComplete(result);
							return;
						}
						
						String response = request->getResponseContentAsString();
						Log(TAG, "AccessToken Response: %s", response);
						
						auto params = HttpRequest::parseParameters(response);
						String token = params["oauth_token"];
						String tokenSecret = params["oauth_token_secret"];
						if (token.isEmpty() || tokenSecret.isEmpty()) {
							OAuthLoginResult result;
							onComplete(result);
							return;
						}

						thiz->m_token = token;
						thiz->m_tokenSecret = tokenSecret;
						
						auto dlg = ToRef(weakDlg);
						if (dlg.isNotNull()) {
							dlg->close();
							auto window = ToRef(dlg->window);
							if (window.isNotNull()) {
								window->decreaseReference();
							}
						}
						
						OAuthLoginResult result;
						result.flagError = sl_false;
						result.token = token;
						result.tokenSecret = tokenSecret;
						result.parameters = params;
						onComplete(result);
					};
					UrlRequest::send(request);
				}
			});
#ifdef SLIB_PLATFORM_IS_MOBILE
			Ref<MobileApp> app = MobileApp::getApp();
			if (app.isNotNull()) {
				Transition transition;
				transition.type = TransitionType::Cover;
				transition.direction = TransitionDirection::FromBottomToTop;
				transition.duration = 0.2f;
				app->popupPage(dlg, transition);
				return;
			}
			dlg->setSize(UIResource::getScreenWidth(), UIResource::getScreenHeight(), UIUpdateMode::Init);
#else
			dlg->setSize(400, 600, UIUpdateMode::Init);
#endif
			dlg->setLeft((UIResource::getScreenWidth() - dlg->getWidth()) / 2, UIUpdateMode::Init);
			dlg->setTop((UIResource::getScreenHeight() - dlg->getHeight()) / 2, UIUpdateMode::Init);
			Ref<Window> window = dlg->popupWindow(sl_null);
			if (window.isNull()) {
				OAuthLoginResult result;
				onComplete(result);
				return;
			}
			window->increaseReference();
			dlg->window = window;
		});
	}
	
}
