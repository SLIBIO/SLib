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
}

void MainPage::onResume()
{
	switchSetDefault->setValue(Device::isDefaultCallingApp());
}
