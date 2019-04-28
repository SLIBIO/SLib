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

#include "slib/ui/photo.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"

#include "slib/core/file.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	void _priv_TakePhoto_onComplete(JNIEnv* env, jobject _this, jstring filePath, jint fd, jint rotation, jboolean flipHorz, jboolean flipVert, jboolean flagCancel);

	SLIB_JNI_BEGIN_CLASS(JAndroidTakePhoto, "slib/platform/android/camera/TakePhoto")
		SLIB_JNI_STATIC_METHOD(open, "open", "(Landroid/app/Activity;ZLjava/lang/String;)V");
		SLIB_JNI_NATIVE(onComplete, "navtiveOnComplete", "(Ljava/lang/String;IIZZZ)V", _priv_TakePhoto_onComplete);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JUtil, "slib/platform/android/Android")
		SLIB_JNI_STATIC_METHOD(getPicturesDirectory, "getPicturesDirectory", "()Ljava/lang/String;");
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


	void PhotoKit::saveImage(const PhotoKit::SaveImageParam& param)
	{
		Memory content;
		if (param.image.isNotNull()) {
			Ref<Image> image = param.image->toImage();
			if (image.isNotNull()) {
				content = image->saveJPEG();
			}
		} else if (param.content.isNotNull()) {
			content = param.content;
		}
		if (content.isNotNull()) {
			String path = JUtil::getPicturesDirectory.callString(sl_null) + "/" + Time::now().format("%04y-%02m-%02d_%02H%02M%02S.jpg");
			sl_size nWritten = File::writeAllBytes(path, content);
			if (nWritten == content.getSize()) {
				param.onComplete(path);
				return;
			}
		}
		param.onComplete(sl_null);
	}

}

#endif
