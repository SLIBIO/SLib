#include "MainPage.h"

void MainPage::onOpen()
{
	switchSetDefault->setOnChange([](SwitchView*, sl_bool value) {
		if (value) {
			Device::setDefaultCallingApp([]() {
				Println("Set Default Calling App Result: %d", Device::isDefaultCallingApp());
			});
		} else {
			Device::changeDefaultCallingApp();
		}
	});

	btnDial->setOnClick([this](View*) {
		Device::openDial(txtPhoneNumber->getText());
	});

	btnCall->setOnClick([this](View*) {
		Device::grantPermissions(DevicePermissions::CallPhone, [this]() {
			Device::callPhone(txtPhoneNumber->getText());
		});
	});
}

void MainPage::onResume()
{
	switchSetDefault->setValue(Device::isDefaultCallingApp());
}
