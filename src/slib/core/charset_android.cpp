/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifdef SLIB_PLATFORM_IS_ANDROID

#include "slib/core/charset.h"

#include "slib/core/platform_android.h"

namespace slib
{
	
	namespace priv
	{
		namespace charset
		{

			SLIB_JNI_BEGIN_CLASS(JCharsets, "slib/util/Charsets")
				SLIB_JNI_STATIC_METHOD(encode, "encode", "(ILjava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)I");
				SLIB_JNI_STATIC_METHOD(decode, "decode", "(ILjava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)I");
			SLIB_JNI_END_CLASS

			#define MAX_LEN 0x1000000

			sl_size Encode16(const sl_char16* utf16, sl_size lenUtf16, sl_uint32 codepage, void* output, sl_reg sizeOutputBuffer)
			{
				JNIEnv* env = Jni::getCurrent();
				if (!env) {
					return 0;
				}
				if (!lenUtf16) {
					return 0;
				}				
				if (!sizeOutputBuffer) {
					return 0;
				}
				if (lenUtf16 > MAX_LEN) {
					lenUtf16 = MAX_LEN;
				}
				JniLocal<jobject> bufInput = env->NewDirectByteBuffer((void*)utf16, (jlong)(lenUtf16 << 1));
				if (bufInput.isNotNull()) {
					JniLocal<jobject> bufOutput;
					if (output && sizeOutputBuffer > 0) {
						bufOutput = env->NewDirectByteBuffer(output, (jlong)sizeOutputBuffer);
					}
					sl_size ret = (sl_size)(JCharsets::encode.callInt(sl_null, (jint)codepage, bufInput.get(), bufOutput.get()));
					if (sizeOutputBuffer > 0) {
						if (ret > (sl_size)sizeOutputBuffer) {
							ret = sizeOutputBuffer;
						}
					}
					return ret;
				}
				return 0;
			}
			
			sl_size Decode16(sl_uint32 codepage, const void* input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer)
			{
				JNIEnv* env = Jni::getCurrent();
				if (!env) {
					return 0;
				}
				if (!sizeInput) {
					return 0;
				}				
				if (!lenUtf16Buffer) {
					return 0;
				}
				if (sizeInput > MAX_LEN) {
					sizeInput = MAX_LEN;
				}
				JniLocal<jobject> bufInput = env->NewDirectByteBuffer((void*)input, (jlong)sizeInput);
				if (bufInput.isNotNull()) {
					JniLocal<jobject> bufOutput;
					if (utf16 && lenUtf16Buffer > 0) {
						bufOutput = env->NewDirectByteBuffer(utf16, (jlong)(lenUtf16Buffer << 1));
					}
					sl_size ret = (sl_size)(JCharsets::decode.callInt(sl_null, (jint)codepage, bufInput.get(), bufOutput.get()));
					if (lenUtf16Buffer > 0) {
						if (ret > (sl_size)lenUtf16Buffer) {
							ret = lenUtf16Buffer;
						}
					}
					return ret;
				}
				return 0;
			}
			
		}
	}
	
}

#endif
