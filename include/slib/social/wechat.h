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

#ifndef CHECKHEADER_SLIB_SOCIAL_WECHAT
#define CHECKHEADER_SLIB_SOCIAL_WECHAT

#include "definition.h"

#include "../core/string.h"
#include "../core/function.h"

namespace slib
{

	class SLIB_EXPORT WechatAppResponse
	{
	public:
		sl_bool flagSuccess;
		sl_bool flagCancel;
		String error;

	public:
		WechatAppResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WechatAppResponse)

	};

	class SLIB_EXPORT WechatPaymentOrder
	{
	public:
		String partnerId;
		String prepayId;
		String package;
		String nonceStr;
		sl_uint64 timeStamp;
		String sign;
		
	public:
		WechatPaymentOrder();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WechatPaymentOrder)
		
	};

	class SLIB_EXPORT WechatPaymentResponse : public WechatAppResponse
	{
	public:
		WechatPaymentResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WechatPaymentResponse)

	};

	class SLIB_EXPORT WechatPaymentRequest : public WechatPaymentOrder
	{
	public:
		Function<void(WechatPaymentResponse&)> onComplete;
		
	public:
		WechatPaymentRequest();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WechatPaymentRequest)
		
	};

	class SLIB_EXPORT WechatSDK
	{
	public:
		static void initialize(const String& appId, const String& universalURL);
		
		static void initialize(const String& appId);
		
	public:
		static void pay(const WechatPaymentRequest& req);
		
	};
	
}

#endif
