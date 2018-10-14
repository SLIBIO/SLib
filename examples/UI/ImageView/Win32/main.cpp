#include <slib/core/platform_windows.h>

#include "../src/app.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	Windows::setDebugFlags();
	ExampleImageViewApp::main();
	return 0;
}
