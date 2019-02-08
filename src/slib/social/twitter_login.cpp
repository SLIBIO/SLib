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

#include "slib/social/twitter.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterLoginResult)
	
	TwitterLoginResult::TwitterLoginResult()
	{
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TwitterLoginParam)
	
	TwitterLoginParam::TwitterLoginParam()
	{
	}
	
	
	void Twitter::login(const TwitterLoginParam& _param)
	{
		void* _thiz = this;
		if (!_thiz) {
			TwitterLoginResult result;
			_param.onComplete(result);
			return;
		}
		auto onComplete = _param.onComplete;
		OAuthLoginParam param = _param;
		auto weak = ToWeakRef(this);
		param.onComplete = [weak, onComplete](OAuthLoginResult& _result) {
			auto thiz = ToRef(weak);
			if (thiz.isNull()) {
				TwitterLoginResult result;
				onComplete(result);
				return;
			}
			String userId = _result.parameters["user_id"];
			String userScreenName = _result.parameters["screen_name"];
			if (userId.isEmpty() || userScreenName.isEmpty()) {
				TwitterLoginResult result;
				onComplete(result);
				return;
			}
			TwitterLoginResult result;
			*((OAuthLoginResult*)&result) = _result;
			if (result.flagError || result.flagCancel) {
				onComplete(result);
				return;
			}
			thiz->m_userId = userId;
			thiz->m_userScreenName = userScreenName;
			result.userId = userId;
			result.userScreenName = userScreenName;
			onComplete(result);
		};
		OAuth1::login(param);
	}
	
	void Twitter::login(const Function<void(TwitterLoginResult& result)>& onComplete)
	{
		TwitterLoginParam param;
		param.onComplete = onComplete;
		login(param);
	}
	
}
