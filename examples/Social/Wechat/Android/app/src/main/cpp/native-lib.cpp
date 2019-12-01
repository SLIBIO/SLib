#include <slib/core/platform_android.h>

#include "../../../../../src/cpp/app.h"

JNIEXPORT jint JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	Android::initialize(jvm);
	ExampleWechatApp::main();
	return JNI_VERSION_1_4;
}
