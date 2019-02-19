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

#include "slib/social/etsy.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EtsyLoginResult)
	
	EtsyLoginResult::EtsyLoginResult()
	{
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(EtsyLoginParam)
	
	EtsyLoginParam::EtsyLoginParam()
	{
	}
	
	
	void Etsy::login(const EtsyLoginParam& _param)
	{
		void* _thiz = this;
		if (!_thiz) {
			EtsyLoginResult result;
			_param.onComplete(result);
			return;
		}
		auto onComplete = _param.onComplete;
		
		OAuthLoginParam param = _param;
		HashMap<String, String> requestTokenParams;
		requestTokenParams.putAll_NoLock(_param.requestTokenParams);
		if (_param.scopes.isNotEmpty()) {
			requestTokenParams.put_NoLock("scope", StringBuffer::join(" ", _param.scopes));
		} else {
			requestTokenParams.put_NoLock("scope", "listings_r");
		}
		param.requestTokenParams = requestTokenParams;
		
		auto weak = ToWeakRef(this);
		param.onComplete = [weak, onComplete](OAuthLoginResult& _result) {
			auto thiz = ToRef(weak);
			if (thiz.isNull()) {
				EtsyLoginResult result;
				*((OAuthLoginResult*)&result) = _result;
				result.flagError = sl_true;
				onComplete(result);
				return;
			}
			if (_result.flagError || _result.flagCancel) {
				EtsyLoginResult result;
				*((OAuthLoginResult*)&result) = _result;
				onComplete(result);
				return;
			}
			UrlRequestParam rp;
			rp.method = HttpMethod::GET;
			rp.url = "https://openapi.etsy.com/v2/users/__SELF__";
			thiz->authorizeRequest(rp);
			rp.onComplete = [weak, _result, onComplete](UrlRequest* request) {
				EtsyLoginResult result;
				*((OAuthLoginResult*)&result) = _result;
				auto thiz = ToRef(weak);
				if (thiz.isNull()) {
					result.flagError = sl_true;
					onComplete(result);
					return;
				}
				if (request->isError()) {
					logUrlRequestError(request);
					result.flagError = sl_true;
					onComplete(result);
					return;
				}
				
				Json content = request->getResponseContentAsJson();
				Json jsonUser = content["results"][0];
				if (jsonUser.isNull()) {
					result.flagError = sl_true;
					onComplete(result);
					return;
				}
				
				Ptr<EtsyUser> user = MakeShared<EtsyUser>();
				if (user.isNull()) {
					result.flagError = sl_true;
					onComplete(result);
					return;
				}
				
				FromJson(jsonUser, *user);

				result.user = *user;
				thiz->m_userLoggined = user;
				onComplete(result);
			};
			UrlRequest::send(rp);
		};
		OAuth1::login(param);
	}
	
	void Etsy::login(const Function<void(EtsyLoginResult& result)>& onComplete)
	{
		EtsyLoginParam param;
		param.onComplete = onComplete;
		login(param);
	}
	
	void Etsy::login(const List<String>& scopes, const Function<void(EtsyLoginResult& result)>& onComplete)
	{
		EtsyLoginParam param;
		param.scopes = scopes;
		param.onComplete = onComplete;
		login(param);
	}
	
}
