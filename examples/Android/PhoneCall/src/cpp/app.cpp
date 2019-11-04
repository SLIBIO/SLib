#include "app.h"

#include "MainPage.h"

SLIB_DEFINE_APPLICATION(PhoneCallApp, MobileApp)

Ref<MainPage> g_mainPage;

PhoneCallApp::PhoneCallApp()
{
	g_mainPage = new MainPage;
}

Ref<View> PhoneCallApp::getStartupPage()
{
	return g_mainPage;
}

void PhoneCallApp::onStart()
{
	UI::setAvailableScreenOrientationsPortrait();
}
