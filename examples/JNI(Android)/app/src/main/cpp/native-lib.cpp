#include <jni.h>

#include <slib/core.h>
#include <slib/core/platform_android.h>

using namespace slib;

JNIEXPORT jint JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	Android::initialize(jvm);
	return JNI_VERSION_1_4;
}

SLIB_JNI_BEGIN_CLASS(MainActivity, "io/slib/examplejni/MainActivity")
	SLIB_JNI_METHOD(callJava, "callJava", "(Ljava/lang/String;I)Ljava/lang/String;")
SLIB_JNI_END_CLASS

extern "C"
JNIEXPORT jstring JNICALL
Java_io_slib_examplejni_MainActivity_callCpp(
		JNIEnv *env, jobject thiz,
		jstring jMessage, int nSeconds
) {

	/*
	 * We need to keep the global reference to the activity handle,
	 * because we are going to call back to Java after 5 seconds.
	 * At that moment, this function call is already finished and
	 * the local reference `thiz` is not available.
	 */
	JniGlobal<jobject> activity(thiz);

	/*
	 * Convert JNI string handle to a `String` C++ object provided by SLib.
	 * You can also use `std::string` according to your convenience.
	 */
	String strMessage = Jni::getString(jMessage);

	Time time = Time::now();

	Log("JNI", "Called from Java with params: %s, %d", strMessage, nSeconds);

	Dispatch::setTimeout([=] () {

		String strSend = String::format("Welcome to SLib. I've received your message '%s' on %s", strMessage, time);

		/*
		 * JniLocal automatically frees resources owned by the local reference.
		 * Jni::getJniString creates a local reference to the `java/lang/String` object,
		 * we need to free this reference to avoid `local reference limit` problem.
		 * In most cases (using few local refs), you can ignore this method and simply put the
		 * returned local reference handle (returned by Jni::getJniString) into the Java call.
		 */
		JniLocal<jstring> jSend(Jni::getJniString(strSend));

		String strRet = MainActivity::callJava.callString(activity.get(), jSend.get(), nSeconds);

		Log("JNI", "JNI received a return value: %s", strRet);

	}, nSeconds * 1000);

	return Jni::getJniString(String::format("JNI will call your callback `callJava` after %d seconds", nSeconds));

}
