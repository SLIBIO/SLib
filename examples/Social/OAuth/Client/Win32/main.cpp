#include <slib/core/platform_windows.h>

#include "../src/cpp/app.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	Windows::setDebugFlags();
	OAuthClientApp::main();
	return 0;
}
