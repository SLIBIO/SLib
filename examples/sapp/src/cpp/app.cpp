#include "app.h"

#include "MainMenu.h"

SLIB_DEFINE_APPLICATION(SAppExampleApp, MobileApp)

SAppExampleApp::SAppExampleApp()
{
	setStartupPage(new MainMenu);
}

void SAppExampleApp::onStart()
{
}
