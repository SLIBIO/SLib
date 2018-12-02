#include "app.h"

#include "../gen/resources.h"

SLIB_DEFINE_APPLICATION(SAppExampleApp, MobileApp)

SAppExampleApp::SAppExampleApp()
{
	setStartupPage(new example::ui::HelloWorld);
}

void SAppExampleApp::onStart()
{
}
