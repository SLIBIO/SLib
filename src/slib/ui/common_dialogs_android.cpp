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

#include "../resources.h"

namespace slib
{

	namespace priv
	{
		namespace alert_dialog
		{

			void OnResultShowAlertDialog(JNIEnv* env, jobject _this, jlong _alert, int result);

			SLIB_JNI_BEGIN_CLASS(JAndroidAlert, "slib/platform/android/ui/Alert")
				SLIB_JNI_INT_FIELD(type);
				SLIB_JNI_STRING_FIELD(text);
				SLIB_JNI_BOOLEAN_FIELD(flagHyperText);
				SLIB_JNI_STRING_FIELD(caption);
				SLIB_JNI_STRING_FIELD(titleOk);
				SLIB_JNI_STRING_FIELD(titleCancel);
				SLIB_JNI_STRING_FIELD(titleYes);
				SLIB_JNI_STRING_FIELD(titleNo);
				SLIB_JNI_LONG_FIELD(nativeObject);

				SLIB_JNI_NEW(init, "()V");
				SLIB_JNI_METHOD(show, "show", "(Lslib/platform/android/SlibActivity;)Z");

				SLIB_JNI_NATIVE(nativeShowAlertResult, "nativeShowResult", "(JI)V", OnResultShowAlertDialog);
			SLIB_JNI_END_CLASS

			class AlertDialogResult : public Referable
			{
			public:
				Function<void(DialogResult)> onResult;
			};

			typedef CHashMap<jlong, Ref<AlertDialogResult> > AlertDialogMap;
			SLIB_SAFE_STATIC_GETTER(AlertDialogMap, GetAlertDialogMap)

			void OnResultShowAlertDialog(JNIEnv* env, jobject _this, jlong _alert, int result)
			{
				AlertDialogMap* alertMap = GetAlertDialogMap();
				if (!alertMap) {
					return;
				}

				Ref<AlertDialogResult> alert;
				alertMap->get(_alert, &alert);
				if (alert.isNotNull()) {
					alertMap->remove(_alert);
					switch (result) {
					case 0: // OK
						alert->onResult(DialogResult::Ok);
						break;
					case 2: // Yes
						alert->onResult(DialogResult::Yes);
						break;
					case 3: // No
						alert->onResult(DialogResult::No);
						break;
					default: // Cancel
						alert->onResult(DialogResult::Cancel);
						break;
					}
				}
			}


		}
	}

	using namespace priv::alert_dialog;

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

	sl_bool AlertDialog::_show()
	{
		AlertDialogMap* alertMap = GetAlertDialogMap();
		if (!alertMap) {
			return sl_false;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			Ref<AlertDialogResult> result = new AlertDialogResult();
			if (result.isNotNull()) {
				result->onResult = SLIB_FUNCTION_REF(AlertDialog, _onResult, this);

				JniLocal<jobject> jalert = JAndroidAlert::init.newObject(sl_null);
				if (jalert.isNotNull()) {
					JAndroidAlert::type.set(jalert, (int)(buttons));
					JAndroidAlert::caption.set(jalert, caption);
					JAndroidAlert::text.set(jalert, text);
					JAndroidAlert::flagHyperText.set(jalert, flagHyperText);
					jlong lresult = (jlong)(result.get());
					JAndroidAlert::nativeObject.set(jalert, lresult);
					JAndroidAlert::titleOk.set(jalert, titleOk);
					JAndroidAlert::titleCancel.set(jalert, titleCancel);
					String _titleYes = titleYes;
					if (_titleYes.isEmpty()) {
						_titleYes = string::yes::get();
					}
					JAndroidAlert::titleYes.set(jalert, _titleYes);
					String _titleNo = titleNo;
					if (_titleNo.isEmpty()) {
						_titleNo = string::no::get();
					}
					JAndroidAlert::titleNo.set(jalert, _titleNo);
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
