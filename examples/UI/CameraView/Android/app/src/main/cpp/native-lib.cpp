#include "../../../../../src/app.h"

#include <slib/core/platform_android.h>

using namespace slib;

JNIEXPORT jint JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	Android::initialize(jvm);
	ExampleCameraViewApp::main();
	return JNI_VERSION_1_4;
}
