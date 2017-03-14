#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/core/preference.h"

#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/platform_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_AndroidPreference, "slib/platform/android/Preference")
		SLIB_JNI_STATIC_METHOD(setValue, "setValue", "(Lslib/platform/android/SlibActivity;Ljava/lang/String;Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(getValue, "getValue", "(Lslib/platform/android/SlibActivity;Ljava/lang/String;)Ljava/lang/String;");
	SLIB_JNI_END_CLASS

	void Preference::setValue(const String& key, const Json& value)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jkey = Jni::getJniString(key);
			String _value = value.toJsonString();
			JniLocal<jstring> jvalue = Jni::getJniString(_value);
			_AndroidPreference::setValue.call(sl_null, jactivity, jkey.get(), jvalue.get());
		}
	}

	Json Preference::getValue(const String& key)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jkey = Jni::getJniString(key);
			String value = _AndroidPreference::getValue.callString(sl_null, jactivity, jkey.get());
			return Json::parseJson(value);
		}
		return sl_null;
	}

}

#endif
