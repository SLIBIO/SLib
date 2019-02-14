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

#include "slib/ui/clipboard.h"

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/platform.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidClipboard, "slib/platform/android/ui/Clipboard")
		SLIB_JNI_STATIC_METHOD(hasText, "hasText", "(Landroid/app/Activity;)Z");
		SLIB_JNI_STATIC_METHOD(getText, "getText", "(Landroid/app/Activity;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(setText, "setText", "(Landroid/app/Activity;Ljava/lang/String;)V");
	SLIB_JNI_END_CLASS
	
	sl_bool Clipboard::hasText()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidClipboard::hasText.callBoolean(sl_null, jactivity) != 0;
		}
		return sl_false;
	}
	
	String Clipboard::getText()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidClipboard::getText.callString(sl_null, jactivity);
		}
		return sl_null;
	}
	
	void Clipboard::setText(const String& text)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jtext = Jni::getJniString(text);
			return JAndroidClipboard::setText.call(sl_null, jactivity, jtext.get());
		}
	}

}

#endif