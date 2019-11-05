#include "MainPage.h"

SLIB_DEFINE_SINGLETON(MainPage)

void MainPage::initCallbacks()
{
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

	sl_uint32 nSIM = Device::getSimSlotsCount();
	if (nSIM > 0) {
		for (sl_uint32 i = 0; i < nSIM; i++) {
			selectSIM->addItem(String::fromUint32(i), String::format("SIM%d(%s)", i+1, Device::getPhoneNumber(i)), UIUpdateMode::Init);
		}
	}
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

	btnCall->setOnClick([this](View*) {
		Application::grantPermissions(AppPermissions::CallPhone, [this]() {
			String value = selectSIM->getSelectedValue();
			if (value.isNotEmpty()) {
				Device::callPhone(txtPhoneNumber->getText(), value.parseUint32());
			} else {
				Device::callPhone(txtPhoneNumber->getText());
			}
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
