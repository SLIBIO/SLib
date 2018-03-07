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

#include "slib/network/url_request.h"

#include "slib/core/safe_static.h"
#include "slib/core/platform_android.h"

namespace slib
{

	void JNICALL JUrlRequest_onInit(JNIEnv* env, jobject _this, jlong jinstance);
	void JNICALL JUrlRequest_onUploadBody(JNIEnv* env, jobject _this, jlong jinstance, jint n);
	void JNICALL JUrlRequest_onResponse(JNIEnv* env, jobject _this, jlong jinstance, jint status, jstring message, jobjectArray headers);
	void JNICALL JUrlRequest_onReceiveContent(JNIEnv* env, jobject _this, jlong jinstance, jbyteArray data, jint n);
	void JNICALL JUrlRequest_onDownloadContent(JNIEnv* env, jobject _this, jlong jinstance, jint n);
	void JNICALL JUrlRequest_onComplete(JNIEnv* env, jobject _this, jlong jinstance);
	void JNICALL JUrlRequest_onError(JNIEnv* env, jobject _this, jlong jinstance, jstring errorMessage);

	SLIB_JNI_BEGIN_CLASS(JUrlRequest, "slib/platform/android/network/UrlRequest")
		SLIB_JNI_STATIC_METHOD(execute, "execute", "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;[BLjava/lang/String;)V")
		SLIB_JNI_METHOD(close, "close", "()V")

		SLIB_JNI_NATIVE(onInit, "nativeOnInit", "(J)V", JUrlRequest_onInit)
		SLIB_JNI_NATIVE(onUploadBody, "nativeOnUploadBody", "(JI)V", JUrlRequest_onUploadBody)
		SLIB_JNI_NATIVE(onResponse, "nativeOnResponse", "(JILjava/lang/String;[Ljava/lang/String;)V", JUrlRequest_onResponse)
		SLIB_JNI_NATIVE(onReceiveContent, "nativeOnReceiveContent", "(J[BI)V", JUrlRequest_onReceiveContent)
		SLIB_JNI_NATIVE(onDownloadContent, "nativeOnDownloadContent", "(JI)V", JUrlRequest_onDownloadContent)
		SLIB_JNI_NATIVE(onComplete, "nativeOnComplete", "(J)V", JUrlRequest_onComplete)
		SLIB_JNI_NATIVE(onError, "nativeOnError", "(JLjava/lang/String;)V", JUrlRequest_onError)
	SLIB_JNI_END_CLASS

	class UrlRequest_Impl : public UrlRequest {
	public:
		AtomicJniGlobal<jobject> m_jrequest;

	public:
		UrlRequest_Impl() {
		}

		~UrlRequest_Impl() {
			clear();
		}

		static Ref<UrlRequest_Impl> create(const UrlRequestParam& param, const String& url) {
			Ref<UrlRequest_Impl> ret = new UrlRequest_Impl;
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
			JniLocal<jstring> method = Jni::getJniString(HttpMethods::toString(m_method));
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
			JniLocal<jobjectArray> jadditionalHeaders;
			{
				CList<String> list;
				for (auto& pair : m_additionalRequestHeaders) {
					list.add_NoLock(pair.key);
					list.add_NoLock(pair.value);
				}
				ListElements<String> m(list);
				if (m.count > 0) {
					jadditionalHeaders = Jni::newStringArray(m.count);
					if (jadditionalHeaders.isNotNull()) {
						for (sl_size i = 0; i < m.count; i++) {
							Jni::setStringArrayElement(jadditionalHeaders, i, m[i]);
						}
					} else {
						m_lastErrorMessage = "Memory Failure";
						onError();
						return;
					}
				}
			}
			JniLocal<jbyteArray> jbody;
			if (m_requestBody.isNotEmpty()) {
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
			JUrlRequest::execute.call(sl_null, (jlong)((sl_reg)((void*)this)), url.get(), method.get(), jheaders.get(), jadditionalHeaders.get(), jbody.get(), downloadFilePath.get());
		}

		void clear() {
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
					map.init();
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
				if (mem.isNotEmpty()) {
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

	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url)
	{
		return Ref<UrlRequest>::from(UrlRequest_Impl::create(param, url));
	}

	void JNICALL JUrlRequest_onInit(JNIEnv* env, jobject _this, jlong jinstance)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->m_jrequest = _this;
		}
	}

	void JNICALL JUrlRequest_onUploadBody(JNIEnv* env, jobject _this, jlong jinstance, jint n)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->dispatchUploadBody(n);
		}
	}

	void JNICALL JUrlRequest_onResponse(JNIEnv* env, jobject _this, jlong jinstance, jint status, jstring message, jobjectArray headers)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->dispatchReceiveResponse(status, message, headers);
		}
	}

	void JNICALL JUrlRequest_onReceiveContent(JNIEnv* env, jobject _this, jlong jinstance, jbyteArray data, jint n)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->dispatchReceiveContent(data, n);
		}
	}

	void JNICALL JUrlRequest_onDownloadContent(JNIEnv* env, jobject _this, jlong jinstance, jint n)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->dispatchDownloadContent(n);
		}
	}

	void JNICALL JUrlRequest_onComplete(JNIEnv* env, jobject _this, jlong jinstance)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->dispatchComplete();
		}
	}

	void JNICALL JUrlRequest_onError(JNIEnv* env, jobject _this, jlong jinstance, jstring errorMessage)
	{
		UrlRequest_Impl* request = (UrlRequest_Impl*)((void*)((sl_reg)jinstance));
		if (request) {
			request->dispatchError(errorMessage);
		}
	}

}

#endif
