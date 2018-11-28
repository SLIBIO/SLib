/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/common_dialogs.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/safe_static.h"

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


	void _priv_AndroidAlert_runShowResult(JNIEnv* env, jobject _this, jlong _alert, int result);

	SLIB_JNI_BEGIN_CLASS(JAndroidAlert, "slib/platform/android/ui/Alert")
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

		SLIB_JNI_NATIVE(nativeShowAlertResult, "nativeShowResult", "(JI)V", _priv_AndroidAlert_runShowResult);

	SLIB_JNI_END_CLASS

	class _priv_UiAlertResult : public Referable
	{
	public:
		Function<void()> onOk;
		Function<void()> onCancel;
		Function<void()> onYes;
		Function<void()> onNo;
	};

	typedef CHashMap<jlong, Ref<_priv_UiAlertResult> > _priv_UiAlertMap;
	SLIB_SAFE_STATIC_GETTER(_priv_UiAlertMap, _priv_AndroidUi_alerts)

	sl_bool AlertDialog::_show()
	{
		_priv_UiAlertMap* alertMap = _priv_AndroidUi_alerts();
		if (!alertMap) {
			return sl_false;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			Ref<_priv_UiAlertResult> result = new _priv_UiAlertResult();
			if (result.isNotNull()) {
				result->onOk = onOk;
				result->onCancel = onCancel;
				result->onYes = onYes;
				result->onNo = onNo;

				JniLocal<jobject> jalert = JAndroidAlert::init.newObject(sl_null);
				if (jalert.isNotNull()) {
					JAndroidAlert::type.set(jalert, (int)(buttons));
					JAndroidAlert::caption.set(jalert, caption);
					JAndroidAlert::text.set(jalert, text);
					jlong lresult = (jlong)(result.get());
					JAndroidAlert::nativeObject.set(jalert, lresult);
					JAndroidAlert::titleOk.set(jalert, titleOk);
					JAndroidAlert::titleCancel.set(jalert, titleCancel);
					JAndroidAlert::titleYes.set(jalert, titleYes);
					JAndroidAlert::titleNo.set(jalert, titleNo);
					alertMap->put(lresult, result);
					if (JAndroidAlert::show.callBoolean(jalert, jactivity)) {
						return sl_true;
					}
					alertMap->remove(lresult);
				}
			}
		}
		return sl_false;
	}

	void _priv_AndroidAlert_runShowResult(JNIEnv* env, jobject _this, jlong _alert, int result)
	{
		_priv_UiAlertMap* alertMap = _priv_AndroidUi_alerts();
		if (!alertMap) {
			return;
		}

		Ref<_priv_UiAlertResult> alert;
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
