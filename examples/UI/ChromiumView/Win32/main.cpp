#include <slib/core/platform_windows.h>

#include "../src/app.h"

#pragma comment(lib, "libcef.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	Windows::setDebugFlags();
	Chromium::startup(hInstance);
	ExampleChromiumViewApp::main();
	Chromium::shutdown();
	return 0;
}
