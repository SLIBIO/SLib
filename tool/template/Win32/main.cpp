#include <slib/core/platform_windows.h>

#include "app.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	Windows::setDebugFlags();
	SLIB_TEMPLATE_APP_NAMEApp::main();
	return 0;
}
