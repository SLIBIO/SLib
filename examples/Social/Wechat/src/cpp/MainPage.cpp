#include "MainPage.h"

void MainPage::onOpen()
{
	btnLogin->setOnClick([](View*) {
		WechatSDK::login([](WechatLoginResponse& response) {
			if (response.flagCancel) {
				return;
			}
			if (response.flagSuccess) {
				UI::showAlert("Token " + response.code);
			} else {
				UI::showAlert("Error " + response.error);
			}
		});
	});
}
