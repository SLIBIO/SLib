/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "app.h"

#include "config.h"

SLIB_DEFINE_APPLICATION(ExampleFCMApp, MobileApp)

ExampleFCMApp::ExampleFCMApp()
{
}

void ExampleFCMApp::onStart()
{
	UI::setAvailableScreenOrientationsLandscape();
	
	initUI();
	
	UI::setBadgeNumber(0);

	auto push = FCM::getInstance();
	push->addOnRefreshToken([this](String deviceToken) {
		Log("FCM", deviceToken);
		txtMyToken->setText(deviceToken);
		deviceId = deviceToken;
		addDevice(deviceToken);
	});
	push->addOnReceiveMessage([this](PushNotificationMessage& message) {
		if (!(isPaused())) {
			// When app is foreground, reset badget after 1 second (after FCM already added badge)
			Dispatch::setTimeout([]() {
				UI::setBadgeNumber(0);
			}, 1000);
		}
		if (message.flagClicked) {
			Log("FCM: Clicked Message", "title=%s, content=%s, data=%s", message.title, message.content, message.data);
		} else {
			Log("FCM: Received Message", "title=%s, content=%s, data=%s", message.title, message.content, message.data);
		}
		txtReceivedMessage->setText(message.content);
	});
	push->start();
	startBroadcast();
}

void ExampleFCMApp::onResume()
{
	UI::setBadgeNumber(0);
}

void ExampleFCMApp::addDevice(const String& device)
{
	ObjectLocker lock(this);
	if (selectReceiver->getValues().contains(device)) {
		return;
	}
	selectReceiver->addItem(device, device);
}

#define BROADCAST_PORT 45231

// broadcast device token over LAN, so that devices can list the other device tokens in the same LAN
void ExampleFCMApp::startBroadcast()
{
	Ref<Socket> socket = Socket::openUdp();
	socket->setOption_Broadcast(sl_true);
	socket->setNonBlockingMode(sl_true);
	if (socket->bind(BROADCAST_PORT)) { // port number
		timerSendBroadcast = Timer::start([this, socket](Timer*) {
			if (deviceId.isNotEmpty()) {
				SocketAddress address(IPv4Address(IPv4Address::Broadcast), BROADCAST_PORT);
				sl_int32 n = socket->sendTo(address, deviceId.getData(), (sl_uint32)(deviceId.getLength()));
				n = n;
			}
		}, 1000);
		threadReceiveBroadcast = Thread::start([this, socket]() {
			auto event = SocketEvent::createRead(socket);
			while (Thread::isNotStoppingCurrent()) {
				SocketAddress address;
				char buf[512];
				sl_reg n = socket->receiveFrom(address, buf, 512);
				if (n > 0) {
					addDevice(String(buf, n));
				} else {
					event->waitEvents();
				}
			}
		});
	}
}

void ExampleFCMApp::onClickSend(View*)
{
	String receiver = selectReceiver->getSelectedValue();
	if (receiver.isEmpty()) {
		return;
	}
	FCM_SendParam param;
	param.legacyServerKey = FCM_SERVICE_LEGACY_KEY;
	param.receiverDeviceToken = receiver;
	param.message.title = "FCM";
	param.message.content = txtSendingMessage->getText();
	param.message.data = Json({
		JsonItem("message", param.message.content),
		JsonItem("time", Time::now()),
		JsonItem("list", Json({
			"string1", "string2", "string3"
		}))
	});
	param.message.badge = 1;
	param.callback = [](FCM_ServiceSendResponse& response) {
		if (response.flagSuccess) {
			UI::showAlert(String::format("Success: %s", response.success));
		} else {
			UI::showAlert(String::format("Error: %s", response.request->getResponseContentAsString()));
		}
	};
	FCM_Service::sendNotification(param);
}

void ExampleFCMApp::initUI()
{
	sl_real fontSizeLabel = UIResource::getScreenMinimum() / 25;
	sl_real fontSizeEdit = UIResource::getScreenMinimum() / 30;
	sl_real heightEdit = UIResource::getScreenMinimum() / 20;
	sl_real padding = UIResource::getScreenMinimum() / 35;
	
	getMainWindow()->setBackgroundColor(Color::White);
	
	Ref<LinearView> linear = new LinearView;
	linear->setWidthFilling();
	linear->setMargin(UI::getSafeAreaInsets());
	linear->setHeightWrapping();
	linear->setCenterVertical();
	linear->setPadding(padding);
	
	Ref<LabelView> lblMyToken = new LabelView;
	lblMyToken->setWidthFilling();
	lblMyToken->setHeightWrapping();
	lblMyToken->setFontSize(fontSizeLabel);
	lblMyToken->setText("My Device Token:");
	linear->addChild(lblMyToken);
	
	txtMyToken = new EditView;
	txtMyToken->setWidthFilling();
	txtMyToken->setHeight(heightEdit);
	txtMyToken->setFontSize(fontSizeEdit);
	linear->addChild(txtMyToken);
	
	Ref<LabelView> lblOtherToken = new LabelView;
	lblOtherToken->setMarginTop(padding);
	lblOtherToken->setWidthFilling();
	lblOtherToken->setHeightWrapping();
	lblOtherToken->setFontSize(fontSizeLabel);
	lblOtherToken->setText("Select Target Device:");
	linear->addChild(lblOtherToken);
	
	selectReceiver = new SelectView;
	selectReceiver->setWidthFilling();
	selectReceiver->setHeight(heightEdit);
	selectReceiver->setFontSize(fontSizeEdit);
	linear->addChild(selectReceiver);
	
	Ref<LabelView> lblSendingMessage = new LabelView;
	lblSendingMessage->setMarginTop(padding);
	lblSendingMessage->setWidthFilling();
	lblSendingMessage->setHeightWrapping();
	lblSendingMessage->setFontSize(fontSizeLabel);
	lblSendingMessage->setText("Sending Message:");
	linear->addChild(lblSendingMessage);
	
	txtSendingMessage = new EditView;
	txtSendingMessage->setWidthFilling();
	txtSendingMessage->setHeight(heightEdit);
	txtSendingMessage->setFontSize(fontSizeEdit);
	txtSendingMessage->setHintText("Input message");
	linear->addChild(txtSendingMessage);
	
	Ref<LabelView> lblReceivedMessage = new LabelView;
	lblReceivedMessage->setMarginTop(padding);
	lblReceivedMessage->setWidthFilling();
	lblReceivedMessage->setHeightWrapping();
	lblReceivedMessage->setFontSize(fontSizeLabel);
	lblReceivedMessage->setText("Received Message:");
	linear->addChild(lblReceivedMessage);
	
	txtReceivedMessage = new EditView;
	txtReceivedMessage->setWidthFilling();
	txtReceivedMessage->setHeight(heightEdit);
	txtReceivedMessage->setFontSize(fontSizeEdit);
	txtReceivedMessage->setHintText("");
	linear->addChild(txtReceivedMessage);
	
	Ref<Button> btnSend = new Button;
	btnSend->setWidthWrapping();
	btnSend->setHeightWrapping();
	btnSend->setCenterHorizontal();
	btnSend->setMarginTop(padding);
	btnSend->setFontSize(fontSizeLabel);
	btnSend->setBackgroundColor(Color::LightGray);
	btnSend->setPadding(padding);
	btnSend->setText("Send Notification");
	btnSend->setOnClick(SLIB_FUNCTION_WEAKREF(ExampleFCMApp, onClickSend, this));
	linear->addChild(btnSend);
	
	addViewToContent(linear);
}
