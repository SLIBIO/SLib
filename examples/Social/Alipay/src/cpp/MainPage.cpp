#include "MainPage.h"

#include "config.h"

void MainPage::onOpen()
{
	btnPay->setOnClick([this](View*) {
		// Note: This section should be implemented on server-side. Here, added for testing purpose
		AlipayOrder order;
		order.appId = ALIPAY_APP_ID;
		order.content.tradeId = "t" + String::fromInt64(Time::now().toInt());
		order.content.amount = txtAmount->getText().parseUint64();
		if (!(order.content.amount)) {
			UI::showAlert("Please input the amount!", [this]() {
				txtAmount->setFocus();
			});
			return;
		}
		order.content.subject = "Test";
		order.content.body = "Test payment";
		if (!(order.sign(ALIPAY_PRIVATE_KEY))) {
			UI::showAlert("Sign Error!");
			return;
		}
		
		AlipaySDK::pay(order.toString(), [](AlipayPaymentResult& result) {
			if (result.flagCancel) {
				return;
			}
			if (result.flagSuccess) {
				UI::showAlert("Success");
			} else {
				UI::showAlert("Error: " + result.error);
			}
		});
	});
}
