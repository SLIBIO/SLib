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

#ifndef CHECKHEADER_SLIB_SOCIAL_ALIPAY
#define CHECKHEADER_SLIB_SOCIAL_ALIPAY

#include "definition.h"

#include "../core/json.h"

namespace slib
{

	class SLIB_EXPORT AlipayResult
	{
	public:
		sl_bool flagSuccess;
		sl_bool flagCancel;
		String error;
		HashMap<String, String> response;

	public:
		AlipayResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AlipayResult)

	};

	class SLIB_EXPORT AlipayBusinessContent
	{
	public:
		String tradeId; // required
		sl_uint64 amount; // required, unit: yuan/100
		String subject; // required
		
		String body;
		String timeout;
		String sellerId;
		String productCode;

	public:
		AlipayBusinessContent();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AlipayBusinessContent)
		
	public:
		String toString() const;

	};

	class SLIB_EXPORT AlipayOrder
	{
	public:
		String appId;
		Time timeStamp;
		AlipayBusinessContent content;
		
		String signature;
		
	public:
		AlipayOrder();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AlipayOrder)
		
	public:
		sl_bool sign(const StringParam& privateKey_PEM);
		
		String toString() const;
		
	private:
		String generateString(sl_bool flagEncode) const;
		
	};

	class SLIB_EXPORT AlipayPaymentResult : public AlipayResult
	{
	public:
		AlipayPaymentResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AlipayPaymentResult)

	};

	class SLIB_EXPORT AlipayPaymentRequest
	{
	public:
		String order;
		
		Function<void(AlipayPaymentResult&)> onComplete;
		
	public:
		AlipayPaymentRequest();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AlipayPaymentRequest)
		
	};

	class SLIB_EXPORT AlipaySDK
	{
	public:
		static void initialize(const String& appScheme);
		
	public:
		static void pay(const AlipayPaymentRequest& req);
		
		static void pay(const String& order, const Function<void(AlipayPaymentResult&)>& onComplete);

	};
	
}

#endif
