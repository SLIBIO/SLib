#include "MainPage.h"
#include "config.h"

void MainPage::onOpen()
{
	btnLogin->setOnClick([](View*) {
		WeChatSDK::login([](WeChatLoginResult& response) {
			if (response.flagCancel) {
				return;
			}
			if (response.flagSuccess) {
				String code = response.code;
				// Important: You should obtain access token on server-side. This implementation is only for testing purpose
				WeChat::initialize(WECHAT_APP_ID, WECHAT_APP_SECRET, WECHAT_UNIVERSAL_LINK);
			} else {
				UI::showAlert("Error " + response.error);
			}
		});
	});
}
