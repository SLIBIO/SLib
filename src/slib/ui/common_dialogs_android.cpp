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

#include "slib/core/file.h"
#include "slib/core/safe_static.h"

#include "../resources.h"

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
		SLIB_JNI_BOOLEAN_FIELD(flagHyperText);
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
		Function<void(DialogResult)> onResult;
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


	sl_bool FileDialog::run()
	{
		return sl_false;
	}

	sl_bool FileDialog::_run()
	{
		return sl_false;
	}


	void _priv_TakePhoto_onComplete(JNIEnv* env, jobject _this, jstring filePath, jint fd, jint rotation, jboolean flipHorz, jboolean flipVert, jboolean flagCancel);

	SLIB_JNI_BEGIN_CLASS(JAndroidTakePhoto, "slib/platform/android/camera/TakePhoto")
		SLIB_JNI_STATIC_METHOD(open, "open", "(Landroid/app/Activity;ZLjava/lang/String;)V");
		SLIB_JNI_NATIVE(onComplete, "navtiveOnComplete", "(Ljava/lang/String;IIZZZ)V", _priv_TakePhoto_onComplete);
	SLIB_JNI_END_CLASS

	class _priv_TakePhotoResult : public TakePhotoResult
	{
	public:
		void setResult(String path, int fd, RotationMode rotation, sl_bool flipHorz, sl_bool flipVert)
		{
			flagSuccess = sl_false;
			Memory mem;
			if (path.isNotEmpty()) {
				filePath = path;
				mem = File::readAllBytes(path);
			} else {
				File file(fd);
				mem = file.readAllBytes();
				fileContent = mem;
				file.clearHandle();
			}
			if (mem.isNull()) {
				return;
			}
			FlipMode flip;
			if (flipHorz) {
				if (flipVert) {
					flip = FlipMode::Both;
				} else {
					flip = FlipMode::Horizontal;
				}
			} else {
				if (flipVert) {
					flip = FlipMode::Vertical;
				} else {
					flip = FlipMode::None;
				}
			}
			NormalizeRotateAndFlip(rotation, flip);
			if (rotation == RotationMode::Rotate0 && flip == FlipMode::None) {
				flagSuccess = sl_true;
			} else {
				filePath.setNull();
				fileContent.setNull();
				if (mem.isNotNull()) {
					Ref<Image> image = Image::loadFromMemory(mem);
					if (image.isNotNull()) {
						drawable = image->rotate(rotation, flip);
						if (drawable.isNotNull()) {
							flagSuccess = sl_true;
						}
					}
				}
			}
		}
	};

	class _priv_TakePhoto_Static
	{
	public:
		Mutex lock;
		Function<void(TakePhotoResult&)> callback;

		void run(TakePhoto& takePhoto, sl_bool flagCamera)
		{
			Function<void(TakePhotoResult&)> oldCallback;
			MutexLocker locker(&lock);
			if (callback.isNotNull()) {
				oldCallback = callback;
				callback.setNull();
			}
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				callback = takePhoto.onComplete;
				locker.unlock();
				if (oldCallback.isNotNull()) {
					TakePhotoResult result;
					oldCallback(result);
				}
				JniLocal<jstring> jpath = Jni::getJniString(takePhoto.outputFilePath);
				JAndroidTakePhoto::open.call(sl_null, jactivity, flagCamera, jpath.get());
			} else {
				locker.unlock();
				if (oldCallback.isNotNull()) {
					TakePhotoResult result;
					oldCallback(result);
				}
				if (takePhoto.onComplete.isNotNull()) {
					TakePhotoResult result;
					takePhoto.onComplete(result);
				}
			}
		}

		void onComplete(String filePath, int fd, RotationMode rotation, sl_bool flipHorz, sl_bool flipVert, sl_bool flagCancel)
		{
			Function<void(TakePhotoResult&)> callback;
			{
				MutexLocker locker(&lock);
				callback = this->callback;
				this->callback.setNull();
			}
			if (callback.isNotNull()) {
				_priv_TakePhotoResult result;
				if (!flagCancel && (filePath.isNotEmpty() || fd != (int)SLIB_FILE_INVALID_HANDLE)) {
					result.setResult(filePath, fd, rotation, flipHorz, flipVert);
				}
				result.flagCancel = flagCancel;
				callback(result);
			}
		}
	};
	SLIB_SAFE_STATIC_GETTER(_priv_TakePhoto_Static, _g_priv_TakePhoto_get);

	static void _priv_TakePhoto_run(TakePhoto& takePhoto, sl_bool flagCamera)
	{
		_priv_TakePhoto_Static* p = _g_priv_TakePhoto_get();
		if (p) {
			p->run(takePhoto, flagCamera);
		}
	}

	void TakePhoto::takeFromCamera()
	{
		_priv_TakePhoto_run(*this, sl_true);
	}

	void TakePhoto::chooseFromLibrary()
	{
		_priv_TakePhoto_run(*this, sl_false);
	}

    void _priv_TakePhoto_onComplete(JNIEnv* env, jobject _this, jstring filePath, jint fd, jint rotation, jboolean flipHorz, jboolean flipVert, jboolean flagCancel)
    {
	    _priv_TakePhoto_Static* p = _g_priv_TakePhoto_get();
	    if (p) {
	    	p->onComplete(Jni::getString(filePath), (int)fd, (slib::RotationMode)rotation, flipHorz, flipVert, flagCancel);
	    }
    }

}

#endif
