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

#include "slib/social/alipay.h"

#include "slib/core/string_buffer.h"
#include "slib/network/url.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AlipayResult)

	AlipayResult::AlipayResult()
	{
		flagSuccess = sl_false;
		flagCancel = sl_false;
	}

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AlipayBusinessContent)

	AlipayBusinessContent::AlipayBusinessContent()
	{
		SLIB_STATIC_STRING(s, "30m")
		timeout = s;
	}

	String AlipayBusinessContent::toString() const
	{
		Json json;
		json.putItem("subject", subject);
		json.putItem("out_trade_no", tradeId);
		json.putItem("total_amount", String::format("%d.%02d", amount / 100, amount % 100));
		json.putItem("seller_id", sellerId.getNotNull());
		json.putItem("product_code", productCode.getNotNull());
		if (body.isNotNull()) {
			json.putItem("body", body);
		}
		if (timeout.isNotNull()) {
			json.putItem("timeout_express", timeout);
		}
		return json.toJsonString();
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AlipayOrder)

	AlipayOrder::AlipayOrder()
	{
	}

	String AlipayOrder::toString() const
	{
		return generateString(sl_true);
	}

	String AlipayOrder::generateString(sl_bool flagEncode) const
	{
		Map<String, String> info;
		info.put_NoLock("method", "alipay.trade.app.pay");
		info.put_NoLock("charset", "utf-8");
		info.put_NoLock("version", "1.0");
		info.put_NoLock("sign_type", "RSA2");
		info.put_NoLock("app_id", appId);
		Time time = timeStamp;
		if (time.isZero()) {
			time = Time::now();
		}
		info.put_NoLock("timestamp", time.toString());
		info.put_NoLock("biz_content", content.toString());
		
		StringBuffer buf;
		sl_bool flagFirst = sl_true;
		for (auto& item : info) {
			if (flagFirst) {
				flagFirst = sl_false;
			} else {
				buf.addStatic("&");
			}
			buf.add(item.key);
			buf.addStatic("=");
			if (flagEncode) {
				buf.add(Url::encodePercent(item.value));
			} else {
				buf.add(item.value);
			}
		}
		if (flagEncode) {
			buf.addStatic("&sign=");
			buf.add(Url::encodePercent(signature));
		}
		return buf.merge();
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AlipayPaymentResult)

	AlipayPaymentResult::AlipayPaymentResult()
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AlipayPaymentRequest)

	AlipayPaymentRequest::AlipayPaymentRequest()
	{
	}

}
