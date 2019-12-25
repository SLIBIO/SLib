#include "app.h"

#include "config.h"
#include "MainPage.h"

SLIB_DEFINE_APPLICATION(ExampleAlipayApp, MobileApp)

ExampleAlipayApp::ExampleAlipayApp()
{
	AlipaySDK::initialize(ALIPAY_APP_SCHEME);
}

Ref<View> ExampleAlipayApp::getStartupPage()
{
	return new MainPage;
}

void ExampleAlipayApp::onStart()
{
	UI::setAvailableScreenOrientationsPortrait();
}
