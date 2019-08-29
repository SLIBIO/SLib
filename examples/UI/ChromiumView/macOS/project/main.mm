#include "app.h"

int main(int argc, const char * argv[])
{
	Chromium::startup(argc, argv);
	ExampleChromiumViewApp::main();
	Chromium::shutdown();
	return 0;
}
