#include "MainPage.h"

#include "config.h"
#include "PayPage.h"

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
				auto wechat = WeChat::getInstance();
				wechat->requestAccessTokenFromCode(code, WECHAT_UNIVERSAL_LINK, [wechat](OAuthAccessTokenResult& result) {
					if (result.flagSuccess) {
						wechat->getUser([](WeChatResult& result, WeChatUser& user) {
							if (result.flagSuccess) {
								UI::showAlert(String::format("User=%s", user.toJson()));
							} else {
								UI::showAlert(String::format("Error=%s", result.response));
							}
						});
					} else {
						UI::showAlert(String::format("Error=%s", result.response));
					}
				});
			} else {
				UI::showAlert("Error " + response.error);
			}
		});
	});
	
	btnPay->setOnClick([this](View*) {
		goToPage(new PayPage);
	});
}
