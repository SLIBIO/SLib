#include <slib/core/platform_android.h>

#include "app.h"

JNIEXPORT jint JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	Android::initialize(jvm);
	SLIB_TEMPLATE_APP_NAMEApp::main();
	return JNI_VERSION_1_4;
}
