#include "MainPage.h"

void MainPage::onOpen()
{
	switchSetDefault->setOnChange([](SwitchView*, sl_bool value) {
		if (value) {
			Application::setDefaultCallingApp([]() {
				Println("Set Default Calling App Result: %d", Application::isDefaultCallingApp());
			});
		} else {
			Application::changeDefaultCallingApp();
		}
	});

	btnDial->setOnClick([this](View*) {
		Device::openDial(txtPhoneNumber->getText());
	});

	btnCall->setOnClick([this](View*) {
		Application::grantPermissions(AppPermissions::CallPhone, [this]() {
			Device::callPhone(txtPhoneNumber->getText());
		});
	});

	MobileApp::getApp()->setOnOpenUrl([this](MobileApp* app, UIEvent* ev) {
		String phoneNumber = Url::getPhoneNumber(ev->getUrl());
		if (phoneNumber.isNotEmpty()) {
			Toast::show("Open Dial: " + phoneNumber);
			txtPhoneNumber->setText(phoneNumber);
		}
	});
}

void MainPage::onResume()
{
	switchSetDefault->setValue(Application::isDefaultCallingApp());
}
