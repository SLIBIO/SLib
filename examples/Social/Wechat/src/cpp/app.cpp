#include "app.h"

#include "MainPage.h"

SLIB_DEFINE_APPLICATION(ExampleWechatApp, MobileApp)

ExampleWechatApp::ExampleWechatApp()
{
}

Ref<View> ExampleWechatApp::getStartupPage()
{
	return new MainPage;
}

void ExampleWechatApp::onStart()
{
	UI::setAvailableScreenOrientationsPortrait();
	
	WechatSDK::initialize("YOUR APP ID", "YOUR APP UNIVERSAL URL");
}
