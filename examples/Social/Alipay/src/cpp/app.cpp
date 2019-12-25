#include "app.h"

#include "MainPage.h"

SLIB_DEFINE_APPLICATION(ExampleAlipayApp, MobileApp)

ExampleAlipayApp::ExampleAlipayApp()
{
}

Ref<View> ExampleAlipayApp::getStartupPage()
{
	return new MainPage;
}

void ExampleAlipayApp::onStart()
{
	UI::setAvailableScreenOrientationsPortrait();
}
