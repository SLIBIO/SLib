#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/network/url_request.h"

#include "../../../inc/slib/core/thread_pool.h"
#include "../../../inc/slib/core/safe_static.h"
#include "../../../inc/slib/core/platform_android.h"

namespace slib
{

	void JNICALL _JUrlRequest_onInit(JNIEnv* env, jobject _this, jlong jinstance);
	void JNICALL _JUrlRequest_onUploadBody(JNIEnv* env, jobject _this, jlong jinstance, jint n);
	void JNICALL _JUrlRequest_onResponse(JNIEnv* env, jobject _this, jlong jinstance, jint status, jstring message, jobjectArray headers);
	void JNICALL _JUrlRequest_onReceiveContent(JNIEnv* env, jobject _this, jlong jinstance, jbyteArray data, jint n);
	void JNICALL _JUrlRequest_onDownloadContent(JNIEnv* env, jobject _this, jlong jinstance, jint n);
	void JNICALL _JUrlRequest_onComplete(JNIEnv* env, jobject _this, jlong jinstance);
	void JNICALL _JUrlRequest_onError(JNIEnv* env, jobject _this, jlong jinstance);

	SLIB_JNI_BEGIN_CLASS(_JUrlRequest, "slib/platform/android/network/UrlRequest")
		SLIB_JNI_STATIC_METHOD(execute, "execute", "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;[BLjava/lang/String;)V")
		SLIB_JNI_METHOD(close, "close", "()V")

		SLIB_JNI_NATIVE(onInit, "nativeOnInit", "(J)V", _JUrlRequest_onInit)
		SLIB_JNI_NATIVE(onUploadBody, "nativeOnUploadBody", "(JI)V", _JUrlRequest_onUploadBody)
		SLIB_JNI_NATIVE(onResponse, "nativeOnResponse", "(JILjava/lang/String;[Ljava/lang/String;)V", _JUrlRequest_onResponse)
		SLIB_JNI_NATIVE(onReceiveContent, "nativeOnReceiveContent", "(J[BI)V", _JUrlRequest_onReceiveContent)
		SLIB_JNI_NATIVE(onDownloadContent, "nativeOnDownloadContent", "(JI)V", _JUrlRequest_onDownloadContent)
		SLIB_JNI_NATIVE(onComplete, "nativeOnComplete", "(J)V", _JUrlRequest_onComplete)
		SLIB_JNI_NATIVE(onError, "nativeOnError", "(J)V", _JUrlRequest_onError)
	SLIB_JNI_END_CLASS


	class _UrlRequestShared
	{
	public:
		Ref<ThreadPool> threadPool;

	public:
		_UrlRequestShared()
		{
			threadPool = ThreadPool::create();
		}

	};

	SLIB_SAFE_STATIC_GETTER(_UrlRequestShared, _getUrlRequestShared)

	class _UrlRequest : public UrlRequest {
	public:
		AtomicJniGlobal<jobject> m_jrequest;

	public:
		_UrlRequest() {
		}

		~_UrlRequest() {
			clear();
		}

		static Ref<_UrlRequest> create(const UrlRequestParam& param, const String& url, const String& downloadFilePath) {
			_UrlRequestShared* shared = _getUrlRequestShared();
			if (shared) {
				Ref<_UrlRequest> ret = new _UrlRequest;
				if (ret.isNotNull()) {
					ret->_init(param, url, downloadFilePath);
					shared->threadPool->addTask(Function<void()>::bind(&(_UrlRequest::execute), WeakRef<_UrlRequest>(ret)));
					return ret;
				}
			}
			return sl_null;
		}

		// override
		void _cancel() {
			clear();
		}

		static void execute(const WeakRef<_UrlRequest>& weakReq) {
			Ref<_UrlRequest> request(weakReq);
			if (request.isNull()) {
				return;
			}
			JniLocal<jstring> url = Jni::getJniString(request->m_url);
			JniLocal<jstring> method = Jni::getJniString(HttpMethods::toString(request->m_method));
			JniLocal<jobjectArray> jheaders;
			{
				CList<String> list;
				Iterator< Pair<String, String> > iterator = request->m_requestHeaders.toIterator();
				Pair<String, String> pair;
				while (iterator.next(&pair)) {
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
						request->onError();
						return;
					}
				}
			}
			JniLocal<jobjectArray> jadditionalHeaders;
			{
				CList<String> list;
				Iterator< Pair<String, String> > iterator = request->m_additionalRequestHeaders.toIterator();
				Pair<String, String> pair;
				while (iterator.next(&pair)) {
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
						request->onError();
						return;
					}
				}
			}
			JniLocal<jbyteArray> jbody;
			if (request->m_requestBody.isNotEmpty()) {
				Memory mem = request->m_requestBody;
				sl_uint32 n = (sl_uint32)(mem.getSize());
				jbody = Jni::newByteArray(n);
				if (jbody.isNotNull()) {
					Jni::setByteArrayRegion(jbody, 0, n, (jbyte*)(mem.getData()));
				} else {
					request->onError();
					return;
				}
			}
			JniLocal<jstring> downloadFilePath = Jni::getJniString(request->m_downloadFilePath);
			request.setNull();
			_JUrlRequest::execute.call(sl_null, (jlong)((sl_reg)((void*)&weakReq)), url.get(), method.get(), jheaders.get(), jadditionalHeaders.get(), jbody.get(), downloadFilePath.get());
		}

		void clear() {
			JniGlobal<jobject> jrequest(m_jrequest);
			if (jrequest.isNotNull()) {
				_JUrlRequest::close.call(jrequest);
			}
			m_jrequest.setNull();
		}

		void dispatchUploadBody(int n)
		{
			onUploadBody(n);
		}

		void dispatchReceiveResponse(int status, jstring message, jobjectArray headers)
		{
			m_responseStatus = (HttpStatus)status;
			m_responseMessage = Jni::getString(message);
			if (headers) {
				sl_uint32 n = Jni::getArrayLength(headers);
				if (n > 0) {
					Map<String, String> map;
					map.initHash(0, HashIgnoreCaseString(), EqualsIgnoreCaseString());
					if (map.isNotNull()) {
						for (sl_uint32 i = 0; i < n - 1; i += 2) {
							String key = Jni::getStringArrayElement(headers, i);
							String value = Jni::getStringArrayElement(headers, i + 1);
							map.ref->put_NoLock(key, value, MapPutMode::AddAlways);
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

		void dispatchError()
		{
			onError();
		}

	};

	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		return Ref<UrlRequest>::from(_UrlRequest::create(param, url, downloadFilePath));
	}

	void JNICALL _JUrlRequest_onInit(JNIEnv* env, jobject _this, jlong jinstance)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->m_jrequest = _this;
		}
	}

	void JNICALL _JUrlRequest_onUploadBody(JNIEnv* env, jobject _this, jlong jinstance, jint n)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->dispatchUploadBody(n);
		}
	}

	void JNICALL _JUrlRequest_onResponse(JNIEnv* env, jobject _this, jlong jinstance, jint status, jstring message, jobjectArray headers)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->dispatchReceiveResponse(status, message, headers);
		}
	}

	void JNICALL _JUrlRequest_onReceiveContent(JNIEnv* env, jobject _this, jlong jinstance, jbyteArray data, jint n)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->dispatchReceiveContent(data, n);
		}
	}

	void JNICALL _JUrlRequest_onDownloadContent(JNIEnv* env, jobject _this, jlong jinstance, jint n)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->dispatchDownloadContent(n);
		}
	}

	void JNICALL _JUrlRequest_onComplete(JNIEnv* env, jobject _this, jlong jinstance)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->dispatchComplete();
		}
	}

	void JNICALL _JUrlRequest_onError(JNIEnv* env, jobject _this, jlong jinstance)
	{
		Ref<_UrlRequest> request(*((WeakRef<_UrlRequest>*)((void*)((sl_reg)jinstance))));
		if (request.isNotNull()) {
			request->dispatchError();
		}
	}

}

#endif
