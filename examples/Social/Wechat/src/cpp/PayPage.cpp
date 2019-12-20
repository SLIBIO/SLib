#include "PayPage.h"

#include "config.h"

void PayPage::onOpen()
{
	txtAmount->setText("1");
	auto thiz = ToWeakRef(this);
	UrlRequest::send("https://api.ipify.org", [this, thiz](UrlRequest* req) {
		auto ref = ToRef(thiz);
		if (ref.isNull()) {
			return;
		}
		String ip = req->getResponseContentAsString();
		txtIp->setText(ip);
	});
	btnOrder->setOnClick([this, thiz](View*) {
		// Note: This section has to be implemented on server-side. Here, added for testing purpose
		WeChatCreateOrderParam param;
		param.orderId = "Test" + String::fromUint64(Time::now().getMillisecondsCount());
		param.amount = (sl_uint64)(txtAmount->getText().parseDouble() * 100);
		param.body = "Test Payment";
		param.apiKey = WECHAT_PAY_API_KEY;
		param.appId = WECHAT_APP_ID;
		param.businessId = WECHAT_PAY_BUSINESS_NUMBER;
		param.notifyUrl = "http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php";
		param.onComplete = [this, thiz](WeChatCreateOrderResult& result) {
			auto ref = ToRef(thiz);
			if (ref.isNull()) {
				return;
			}
			Log("Request", "%s", String::fromUtf8(result.request->getRequestBody()));
			Log("Response", "%s", result.responseText);
			txtOrder->setText(String::format("%s", result.responseText));
			if (result.flagSuccess) {
				m_order = result.order;
			}
		};
		WeChat::createOrder(param);
	});
	btnPay->setOnClick([this](View*) {
		if (m_order.prepayId.isEmpty()) {
			UI::showAlert("Please make order");
			return;
		}
		WeChatPaymentRequest req;
		req.order = m_order;
		req.onComplete = [](WeChatPaymentResult& result) {
			if (result.flagCancel) {
				return;
			}
			if (result.flagSuccess) {
				UI::showAlert("Payment Completed");
			} else {
				UI::showAlert("Error " + result.error);
			}
		};
		WeChatSDK::pay(req);
	});
}
