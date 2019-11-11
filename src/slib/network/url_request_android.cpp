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

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/network/url_request.h"

#include "slib/core/safe_static.h"
#include "slib/core/platform_android.h"

namespace slib
{

	namespace priv
	{
		namespace url_request
		{
			void JNICALL OnInit(JNIEnv* env, jobject _this, jlong jinstance);
			void JNICALL OnUploadBody(JNIEnv* env, jobject _this, jlong jinstance, jint n);
			void JNICALL OnResponse(JNIEnv* env, jobject _this, jlong jinstance, jint status, jstring message, jobjectArray headers);
			void JNICALL OnReceiveContent(JNIEnv* env, jobject _this, jlong jinstance, jbyteArray data, jint n);
			void JNICALL OnDownloadContent(JNIEnv* env, jobject _this, jlong jinstance, jint n);
			void JNICALL OnComplete(JNIEnv* env, jobject _this, jlong jinstance);
			void JNICALL OnError(JNIEnv* env, jobject _this, jlong jinstance, jstring errorMessage);

			SLIB_JNI_BEGIN_CLASS(JUrlRequest, "slib/platform/android/network/UrlRequest")
				SLIB_JNI_STATIC_METHOD(execute, "execute", "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;[BLjava/lang/String;IZ)V")
				SLIB_JNI_METHOD(close, "close", "()V")

				SLIB_JNI_NATIVE(onInit, "nativeOnInit", "(J)V", OnInit)
				SLIB_JNI_NATIVE(onUploadBody, "nativeOnUploadBody", "(JI)V", OnUploadBody)
				SLIB_JNI_NATIVE(onResponse, "nativeOnResponse", "(JILjava/lang/String;[Ljava/lang/String;)V", OnResponse)
				SLIB_JNI_NATIVE(onReceiveContent, "nativeOnReceiveContent", "(J[BI)V", OnReceiveContent)
				SLIB_JNI_NATIVE(onDownloadContent, "nativeOnDownloadContent", "(JI)V", OnDownloadContent)
				SLIB_JNI_NATIVE(onComplete, "nativeOnComplete", "(J)V", OnComplete)
				SLIB_JNI_NATIVE(onError, "nativeOnError", "(JLjava/lang/String;)V", OnError)
			SLIB_JNI_END_CLASS

			class UrlRequestImpl : public UrlRequest {
			public:
				AtomicJniGlobal<jobject> m_jrequest;

			public:
				UrlRequestImpl() {
				}

				~UrlRequestImpl() {
					clear();
				}

				static Ref<UrlRequestImpl> create(const UrlRequestParam& param, const String& url) {
					Ref<UrlRequestImpl> ret = new UrlRequestImpl;
					if (ret.isNotNull()) {
						ret->_init(param, url);
						return ret;
					}
					return sl_null;
				}

				void _cancel() override
				{
					clear();
				}

				void _sendSync() override
				{
					JniLocal<jstring> url = Jni::getJniString(m_url);
					JniLocal<jstring> method = Jni::getJniString(HttpMethodHelper::toString(m_method));
					JniLocal<jobjectArray> jheaders;
					{
						CList<String> list;
						for (auto& pair : m_requestHeaders) {
							list.add_NoLock(pair.key);
							list.add_NoLock(pair.value);
						}
						ListElements<String> m(list);
						if (m.count > 0) {
							jheaders = Jni::newStringArray(m.count);
							if (jheaders.isNotNull()) {
								for (sl_size i = 0; i < m.count; i++) {
									Jni::setStringArrayElement(jheaders, i, m[i]);
								}
							} else {
								m_lastErrorMessage = "Memory Failure";
								onError();
								return;
							}
						}
					}
					JniLocal<jbyteArray> jbody;
					if (m_requestBody.isNotNull()) {
						Memory mem = m_requestBody;
						sl_uint32 n = (sl_uint32)(mem.getSize());
						jbody = Jni::newByteArray(n);
						if (jbody.isNotNull()) {
							Jni::setByteArrayRegion(jbody, 0, n, (jbyte*)(mem.getData()));
						} else {
							m_lastErrorMessage = "Memory Failure";
							onError();
							return;
						}
					}
					JniLocal<jstring> downloadFilePath = Jni::getJniString(m_downloadFilePath);
					JUrlRequest::execute.call(sl_null, (jlong)((sl_reg)((void*)this)), url.get(), method.get(), jheaders.get(), jbody.get(), downloadFilePath.get(), m_timeout, m_flagAllowInsecureConnection);
				}

				void clear()
				{
					JniGlobal<jobject> jrequest(m_jrequest);
					if (jrequest.isNotNull()) {
						JUrlRequest::close.call(jrequest);
					}
					m_jrequest.setNull();
				}

				void dispatchUploadBody(int n)
				{
					m_sizeBodySent += n;
					onUploadBody(n);
				}

				void dispatchReceiveResponse(int status, jstring message, jobjectArray headers)
				{
					m_responseStatus = (HttpStatus)status;
					m_responseMessage = Jni::getString(message);
					if (headers) {
						sl_uint32 n = Jni::getArrayLength(headers);
						if (n > 0) {
							HttpHeaderMap map;
							map.initialize();
							if (map.isNotNull()) {
								for (sl_uint32 i = 0; i < n - 1; i += 2) {
									String key = Jni::getStringArrayElement(headers, i);
									String value = Jni::getStringArrayElement(headers, i + 1);
									map.ref->add_NoLock(key, value);
								}
							}
							m_responseHeaders = map;
							String strLength = map.getValue("Content-Length", String::null());
							if (strLength.isNotEmpty()) {
								m_sizeContentTotal = strLength.parseUint64();
							}
						}
					}
					onResponse();
				}

				void dispatchReceiveContent(jbyteArray data, int n)
				{
					#define STACK_BUF_SIZE 4096
					if (n > STACK_BUF_SIZE) {
						Memory mem = Memory::create(n);
						if (mem.isNotNull()) {
							Jni::getByteArrayRegion(data, 0, n, (jbyte*)(mem.getData()));
							onReceiveContent(mem.getData(), n, mem);
						} else {
							clear();
							m_lastErrorMessage = "Memory Failure";
							onError();
						}
					} else {
						jbyte buf[STACK_BUF_SIZE];
						Jni::getByteArrayRegion(data, 0, n, buf);
						onReceiveContent(buf, n, Memory::null());
					}
				}

				void dispatchDownloadContent(int n)
				{
					onDownloadContent(n);
				}

				void dispatchComplete()
				{
					onComplete();
				}

				void dispatchError(jstring errorMessage)
				{
					m_lastErrorMessage = Jni::getString(errorMessage);
					onError();
				}

			};

			void JNICALL OnInit(JNIEnv* env, jobject _this, jlong jinstance)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->m_jrequest = _this;
				}
			}

			void JNICALL OnUploadBody(JNIEnv* env, jobject _this, jlong jinstance, jint n)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->dispatchUploadBody(n);
				}
			}

			void JNICALL OnResponse(JNIEnv* env, jobject _this, jlong jinstance, jint status, jstring message, jobjectArray headers)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->dispatchReceiveResponse(status, message, headers);
				}
			}

			void JNICALL OnReceiveContent(JNIEnv* env, jobject _this, jlong jinstance, jbyteArray data, jint n)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->dispatchReceiveContent(data, n);
				}
			}

			void JNICALL OnDownloadContent(JNIEnv* env, jobject _this, jlong jinstance, jint n)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->dispatchDownloadContent(n);
				}
			}

			void JNICALL OnComplete(JNIEnv* env, jobject _this, jlong jinstance)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->dispatchComplete();
				}
			}

			void JNICALL OnError(JNIEnv* env, jobject _this, jlong jinstance, jstring errorMessage)
			{
				UrlRequestImpl* request = (UrlRequestImpl*)((void*)((sl_reg)jinstance));
				if (request) {
					request->dispatchError(errorMessage);
				}
			}

		}
	}

	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url)
	{
		return Ref<UrlRequest>::from(priv::url_request::UrlRequestImpl::create(param, url));
	}

}

#endif
