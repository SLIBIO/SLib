#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/common_dialogs.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

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
		SLIB_JNI_METHOD(show, "show", "(Lslib/platform/android/SlibActivity;)Z");

		SLIB_JNI_NATIVE(nativeShowAlertResult, "nativeShowResult", "(JI)V", _AndroidAlert_runShowResult);

	SLIB_JNI_END_CLASS

	class _UiAlertResult : public Referable
	{
	public:
		Function<void()> onOk;
		Function<void()> onCancel;
		Function<void()> onYes;
		Function<void()> onNo;
	};

	typedef HashMap<jlong, Ref<_UiAlertResult> > _UiAlertMap;
	SLIB_SAFE_STATIC_GETTER(_UiAlertMap, _AndroidUi_alerts)

	sl_bool AlertDialog::_show()
	{
		_UiAlertMap* alertMap = _AndroidUi_alerts();
		if (!alertMap) {
			return sl_false;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			Ref<_UiAlertResult> result = new _UiAlertResult();
			if (result.isNotNull()) {
				result->onOk = onOk;
				result->onCancel = onCancel;
				result->onYes = onYes;
				result->onNo = onNo;

				JniLocal<jobject> jalert = _JAndroidAlert::init.newObject(sl_null);
				if (jalert.isNotNull()) {
					_JAndroidAlert::type.set(jalert, (int)(type));
					_JAndroidAlert::caption.set(jalert, caption);
					_JAndroidAlert::text.set(jalert, text);
					jlong lresult = (jlong)(result.get());
					_JAndroidAlert::nativeObject.set(jalert, lresult);
					_JAndroidAlert::titleOk.set(jalert, titleOk);
					_JAndroidAlert::titleCancel.set(jalert, titleCancel);
					_JAndroidAlert::titleYes.set(jalert, titleYes);
					_JAndroidAlert::titleNo.set(jalert, titleNo);
					alertMap->put(lresult, result);
					if (_JAndroidAlert::show.callBoolean(jalert, jactivity)) {
						return sl_true;
					}
					alertMap->remove(lresult);
				}
			}
		}
		return sl_false;
	}

	void _AndroidAlert_runShowResult(JNIEnv* env, jobject _this, jlong _alert, int result)
	{
		_UiAlertMap* alertMap = _AndroidUi_alerts();
		if (!alertMap) {
			return;
		}

		Ref<_UiAlertResult> alert;
		alertMap->get(_alert, &alert);
		if (alert.isNotNull()) {
			alertMap->remove(_alert);
			switch (result) {
			case 0: // OK
				alert->onOk();
				break;
			case 2: // Yes
				alert->onYes();
				break;
			case 3: // No
				alert->onNo();
				break;
			default: // Cancel
				alert->onCancel();
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

}

#endif
