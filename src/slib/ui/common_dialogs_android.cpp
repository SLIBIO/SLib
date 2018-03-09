/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

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
					JAndroidAlert::type.set(jalert, (int)(type));
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
