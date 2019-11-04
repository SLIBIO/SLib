#include "MainPage.h"

void MainPage::init()
{
	ui::MainPage::init();
	Device::addOnIncomingCall([this](String callId, String phoneNumber) {
		Println("Incoming: %s, %s", callId, phoneNumber);
		groupCall->setVisible(sl_true);
		lblCallInfo->setText("In: " + phoneNumber);
		btnAnswer->setVisible(sl_true);
		btnAnswer->setOnClick([this, callId](View*) {
			Device::answerCall(callId);
			btnAnswer->setVisible(sl_false);
		});
		btnEndCall->setOnClick([callId](View*) {
			Device::endCall(callId);
		});
	});
	Device::addOnOutgoingCall([this](String callId, String phoneNumber) {
		Println("Outgoing: %s, %s", callId, phoneNumber);
		groupCall->setVisible(sl_true);
		lblCallInfo->setText("Out: " + phoneNumber);
		btnAnswer->setVisible(sl_false);
		btnEndCall->setOnClick([callId](View*) {
			Device::endCall(callId);
		});
	});
	Device::addOnEndCall([this](String callId, String phoneNumber) {
		Println("End: %s, %s", callId, phoneNumber);
		groupCall->setVisible(sl_false);
	});
}

void MainPage::onOpen()
{
	Application::grantPermissions(AppPermissions::ReadPhoneState);

	switchSetDefault->setOnChange([](SwitchView*, sl_bool value) {
		if (value) {
			Application::setDefaultCallingApp([]() {
				Println("Set Default Calling App Result: %d", Application::isDefaultCallingApp());
			});
		} else {
			Application::openDefaultAppsSetting();
		}
	});
	switchSystemOverlay->setOnChange([](SwitchView*, sl_bool value) {
		Application::openSystemOverlaySetting();
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
	switchSystemOverlay->setValue(Application::isEnabledSystemOverlay());
}
