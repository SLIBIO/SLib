#include "app.h"

#include "MainPage.h"
#include "config.h"

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
	
	WeChatSDK::initialize(WECHAT_APP_ID, WECHAT_UNIVERSAL_LINK);
	
	// This should be used on server side. Here, added for testing purpose
	WeChat::initialize(WECHAT_APP_ID, WECHAT_APP_SECRET, WECHAT_UNIVERSAL_LINK);
}
