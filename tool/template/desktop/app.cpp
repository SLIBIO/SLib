#include "app.h"

SLIB_DEFINE_APPLICATION(SLIB_TEMPLATE_APP_NAMEApp, UIApp)

SLIB_TEMPLATE_APP_NAMEApp::SLIB_TEMPLATE_APP_NAMEApp()
{
}

void SLIB_TEMPLATE_APP_NAMEApp::onStart()
{
	Ref<Window> window = new Window;
	window->setTitle("SLIB_TEMPLATE_APP_NAME");
	window->setFrame(100, 100, 400, 300);
	window->setOnClose([](Window* window, UIEvent* ev) {
		UIApp::quit();
	});
	window->create();
	setMainWindow(window);
}
