#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/common_dialogs.h"
#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

DialogResult AlertDialog::run()
{
	return _runByShow();
}

DialogResult AlertDialog::_run()
{
	return DialogResult::Cancel;
}

void AlertDialog::show()
{
	_showOnUiThread();
}


void _AndroidAlert_runShowResult(JNIEnv* env, jobject _this, jlong _alert, int result);

SLIB_JNI_BEGIN_CLASS(_JAndroidAlert, "slib/platform/android/ui/Alert")
	SLIB_JNI_INT_FIELD(type);
	SLIB_JNI_STRING_FIELD(text);
	SLIB_JNI_STRING_FIELD(caption);
	SLIB_JNI_STRING_FIELD(titleOk);
	SLIB_JNI_STRING_FIELD(titleCancel);
	SLIB_JNI_STRING_FIELD(titleYes);
	SLIB_JNI_STRING_FIELD(titleNo);
	SLIB_JNI_LONG_FIELD(nativeObject);

	SLIB_JNI_NEW(init, "()V");
	SLIB_JNI_METHOD(show, "show", "(Lslib/platform/android/SlibActivity;)V");

	SLIB_JNI_NATIVE(nativeShowAlertResult, "nativeShowResult", "(JI)V", _AndroidAlert_runShowResult);

SLIB_JNI_END_CLASS

class _UiAlertResult : public Referable
{
public:
	Ref<Runnable> onOk;
	Ref<Runnable> onCancel;
	Ref<Runnable> onNo;
};

typedef HashMap<jlong, Ref<_UiAlertResult> > _UiAlertMap;
SLIB_SAFE_STATIC_GETTER(_UiAlertMap, _AndroidUi_alerts);

void AlertDialog::_show()
{
	jobject jactivity = Android::getCurrentActivity();
	if (jactivity) {
		Ref<_UiAlertResult> result = new _UiAlertResult();
		if (result.isNotNull()) {
			result->onOk = onOk;
			result->onCancel = onCancel;
			result->onNo = onNo;

			JniLocal<jobject> jalert = _JAndroidAlert::init.newObject(sl_null);
			if (jalert.isNotNull()) {
				_JAndroidAlert::type.set(jalert, (int)(type));
				_JAndroidAlert::caption.set(jalert, caption);
				_JAndroidAlert::text.set(jalert, text);
				jlong lresult = (jlong)(result.ptr);
				_JAndroidAlert::nativeObject.set(jalert, lresult);
				_JAndroidAlert::titleOk.set(jalert, titleOk);
				_JAndroidAlert::titleCancel.set(jalert, titleCancel);
				_JAndroidAlert::titleYes.set(jalert, titleYes);
				_JAndroidAlert::titleNo.set(jalert, titleNo);
				_AndroidUi_alerts().put(lresult, result);
				_JAndroidAlert::show.call(jalert, jactivity);
			}
		}
	}
}

void _AndroidAlert_runShowResult(JNIEnv* env, jobject _this, jlong _alert, int result)
{
	Ref<_UiAlertResult> alert;
	_AndroidUi_alerts().get(_alert, &alert);
	if (alert.isNotNull()) {
		_AndroidUi_alerts().remove(_alert);
		switch (result) {
		case 0: // OK
			if (alert->onOk.isNotNull()) {
				alert->onOk->run();
			}
			break;
		case 2: // Yes
			if (alert->onOk.isNotNull()) {
				alert->onOk->run();
			}
			break;
		case 3: // No
			if (alert->onNo.isNotNull()) {
				alert->onNo->run();
			}
			break;
		default: // Cancel
			if (alert->onCancel.isNotNull()) {
				alert->onCancel->run();
			}
			break;
		}
	}
}


sl_bool FileDialog::run()
{
	return sl_false;
}

sl_bool FileDialog::_run()
{
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif
