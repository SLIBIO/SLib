#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_USE_JNI

#include "../../../inc/slib/core/java.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/safe_static.h"

#define JNIVERSION JNI_VERSION_1_4

SLIB_NAMESPACE_BEGIN

static void _logJniError(const String& text)
{
	SLIB_LOG_ERROR("SLIB_JNI", text);
}

JavaVM* _g_jvm_shared = sl_null;

SLIB_THREAD JNIEnv* _gt_jniCurrent = sl_null;

class _Jni_Shared
{
public:
    HashMap<String, JniClass> classes;
    
    CList<_JniSingletonClass*> singleton_classes;
    CList<_JniSingletonMethod*> singleton_methods;
    CList<_JniSingletonStaticMethod*> singleton_static_methods;
    CList<_JniSingletonField*> singleton_fields;
    CList<_JniSingletonStaticField*> singleton_static_fields;
    CList<_JniNativeMethod*> native_methods;
};

SLIB_SAFE_STATIC_GETTER(_Jni_Shared, _Jni_getShared)

//#define JNI_LOG_INIT_LOAD

void Jni::initialize(JavaVM* jvm)
{
	static sl_bool flagInit = sl_false;
	
	if (! flagInit) {
		
		flagInit = sl_true;
		Jni::setSharedJVM(jvm);

		_Jni_Shared* shared = _Jni_getShared();
		if (!shared) {
			return;
		}

		// singleton classes
		{
			ListLocker< _JniSingletonClass* > list(shared->singleton_classes);
			for (sl_size i = 0; i < list.count; i++) {
				_JniSingletonClass* obj = list[i];
#if defined(JNI_LOG_INIT_LOAD)
				SLIB_LOG("LOADING JAVA CLASS", obj->name);
#endif
				obj->cls = Jni::getClass(obj->name);
				if (obj->cls.isNull()) {
					SLIB_LOG("LOADING JAVA CLASS FAILED", obj->name);
				}
			}
		}


		// singleton fields
		{
			ListLocker< _JniSingletonField* > list(shared->singleton_fields);
			for (sl_size i = 0; i < list.count; i++) {
				_JniSingletonField* obj = list[i];
				JniClass cls = obj->gcls->cls;
				if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
					SLIB_LOG("LOADING JAVA FIELD", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
#endif
					obj->cls = cls;
					obj->id = cls.getFieldID(obj->name, obj->sig);
					if (obj->id == sl_null) {
						SLIB_LOG("LOADING JAVA FIELD FAILED", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
					}
				}
			}
		}
		// singleton static fields
		{
			ListLocker< _JniSingletonStaticField* > list(shared->singleton_static_fields);
			for (sl_size i = 0; i < list.count; i++) {
				_JniSingletonStaticField* obj = list[i];
				JniClass cls = obj->gcls->cls;
				if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
					SLIB_LOG("LOADING JAVA STATIC FIELD", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
#endif
					obj->cls = cls;
					obj->id = cls.getStaticFieldID(obj->name, obj->sig);
					if (obj->id == sl_null) {
						SLIB_LOG("LOADING JAVA STATIC FIELD FAILED", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
					}
				}
			}
		}
		// singleton methods
		{
			ListLocker< _JniSingletonMethod* > list(shared->singleton_methods);
			for (sl_size i = 0; i < list.count; i++) {
				_JniSingletonMethod* obj = list[i];
				JniClass cls = obj->gcls->cls;
				if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
					SLIB_LOG("LOADING JAVA METHOD", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
#endif
					obj->cls = cls;
					obj->id = cls.getMethodID(obj->name, obj->sig);
					if (obj->id == sl_null) {
						SLIB_LOG("LOADING JAVA METHOD FAILED", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
					}
				}
			}
		}
		// singleton static methods
		{
			ListLocker< _JniSingletonStaticMethod* > list(shared->singleton_static_methods);
			for (sl_size i = 0; i < list.count; i++) {
				_JniSingletonStaticMethod* obj = list[i];
				JniClass cls = obj->gcls->cls;
				if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
					SLIB_LOG("LOADING JAVA STATIC METHOD", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
#endif
					obj->cls = cls;
					obj->id = cls.getStaticMethodID(obj->name, obj->sig);
					if (obj->id == sl_null) {
						SLIB_LOG("LOADING JAVA STATIC METHOD FAILED", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
					}
				}
			}
		}
		// native methods
		{
			ListLocker< _JniNativeMethod* > list(shared->native_methods);
			for (sl_size i = 0; i < list.count; i++) {
				_JniNativeMethod* obj = list[i];
				JniClass cls = obj->gcls->cls;
				if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
					SLIB_LOG("REGISTERING JAVA NATIVE", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
#endif
					if (!cls.registerNative(obj->name, obj->sig, obj->fn)) {
						SLIB_LOG("REGISTERING JAVA NATIVE FAILED", String(obj->gcls->name) + "::" + obj->name + " " + obj->sig);
					}
				}
			}
		}
	}
}

void Jni::setSharedJVM(JavaVM *jvm)
{
	if (_g_jvm_shared != jvm) {
		_g_jvm_shared = jvm;
	}
}

JavaVM* Jni::getSharedJVM()
{
	return _g_jvm_shared;
}

JNIEnv* Jni::getCurrent()
{
	JNIEnv *env = _gt_jniCurrent;
	if (! env) {
		env = Jni::attachThread();
	}
	return env;
}

void Jni::setCurrent(JNIEnv* jni)
{
	_gt_jniCurrent = jni;
}

JNIEnv* Jni::attachThread(JavaVM* jvm)
{
	if (! jvm) {
		jvm = Jni::getSharedJVM();
	}
	JNIEnv *env = sl_null;
	if (jvm) {
		if (jvm->GetEnv((void**) &env, JNIVERSION) != JNI_OK) {
#if defined(SLIB_PLATFORM_IS_ANDROID)
			jint res = jvm->AttachCurrentThread(&env, sl_null);
#else
			jint res = jvm->AttachCurrentThread((void**)&env, sl_null);
#endif
			if ((res < 0) || !env) {
				_logJniError("Failed to attach thread");
			}
			Jni::setCurrent(env);
		}
	}
	if (env) {
		Jni::setCurrent(env);
	}
	return env;
}

void Jni::detachThread(JavaVM* jvm)
{
	if (! jvm) {
		jvm = Jni::getSharedJVM();
	}
	if (jvm) {
		jvm->DetachCurrentThread();
	}
}

JniClass Jni::findClass(const char* className)
{
	JNIEnv *env = getCurrent();
	if (env) {
		JniLocal<jclass> cls = env->FindClass(className);
		if (Jni::checkException()) {
			_logJniError(String("Exception occurred while finding class: ") + className);
			Jni::printException();
			Jni::clearException();
		} else {
			if (cls.isNotNull()) {
				return JniClass::from(cls);
			}
		}
	}
	return JniClass::null();
}

JniClass Jni::getClass(const String& className)
{
	_Jni_Shared* shared = _Jni_getShared();
	if (!shared) {
		return JniClass::null();
	}
	JniClass ret;
	if (shared->classes.get(className, &ret)) {
		return ret;
	}
	ret = Jni::findClass(className.getData());
	if (ret.isNotNull()) {
		Jni::registerClass(className, ret);
	}
	return ret;
}

void Jni::registerClass(const String& className, jclass cls)
{
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->classes.put(className, cls);
	}
}

void Jni::unregisterClass(const String& className)
{
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->classes.remove(className);
	}
}

sl_bool Jni::isSameObject(jobject ref1, jobject ref2)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->IsSameObject(ref1, ref2) != 0;
	}
	return sl_false;
}

jobjectRefType Jni::getRefType(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->GetObjectRefType(obj);
	}
	return JNIInvalidRefType;
}

sl_bool Jni::isInvalidRef(jobject obj)
{
	return Jni::getRefType(obj) == JNIInvalidRefType;
}

sl_bool Jni::isLocalRef(jobject obj)
{
	return Jni::getRefType(obj) == JNILocalRefType;
}

jobject Jni::newLocalRef(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->NewLocalRef(obj);
	}
	return 0;
}

void Jni::deleteLocalRef(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		jobjectRefType type = env->GetObjectRefType(obj);
		if (type == JNILocalRefType) {
			env->DeleteLocalRef(obj);
		}
	}
}

sl_bool Jni::isGlobalRef(jobject obj)
{
	return Jni::getRefType(obj) == JNIGlobalRefType;
}

jobject Jni::newGlobalRef(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->NewGlobalRef(obj);
	}
	return 0;
}

void Jni::deleteGlobalRef(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		jobjectRefType type = env->GetObjectRefType(obj);
		if (type == JNIGlobalRefType) {
			env->DeleteGlobalRef(obj);
		}
	}
}

sl_bool Jni::isWeakRef(jobject obj)
{
	return Jni::getRefType(obj) == JNIWeakGlobalRefType;
}

jobject Jni::newWeakRef(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->NewWeakGlobalRef(obj);
	}
	return 0;
}

void Jni::deleteWeakRef(jobject obj)
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		jobjectRefType type = env->GetObjectRefType(obj);
		if (type == JNIWeakGlobalRefType) {
			env->DeleteWeakGlobalRef(obj);
		}
	}
}

jstring Jni::getJniString(const String& str)
{
	jstring ret = sl_null;
	if (str.isNotNull()) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->NewStringUTF(str.getData());
		}
	}
	return ret;
}

jstring Jni::getJniString16(const String16& str)
{
	jstring ret = sl_null;
	if (str.isNotNull()) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->NewString((jchar*)(str.getData()), str.getLength());
		}
	}
	return ret;
}

String Jni::getString(jstring str)
{
	String ret;
	if (str) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			sl_uint32 len = (sl_uint32)(env->GetStringUTFLength(str));
			const char* sz = env->GetStringUTFChars(str, sl_null);
			if (sz) {
				ret = String::fromUtf8(sz, len);
				env->ReleaseStringUTFChars(str, sz);
			}
		}
	}
	return ret;
}

String16 Jni::getString16(jstring str)
{
	String16 ret;
	if (str) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			sl_uint32 len = (sl_uint32)(env->GetStringLength(str));
			const jchar* sz = env->GetStringChars(str, sl_null);
			if (sz) {
				ret = String16::fromUtf16(sz, len);
				env->ReleaseStringChars(str, sz);
			}
		}
	}
	return ret;
}

sl_uint32 Jni::getArrayLength(jarray array)
{
	sl_uint32 ret = 0;
	if (array) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->GetArrayLength(array);
		}
	}
	return ret;
}

jobjectArray Jni::newObjectArray(jclass clsElement, sl_uint32 length)
{
	jobjectArray ret = sl_null;
	if (clsElement) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->NewObjectArray(length, clsElement, sl_null);
		}
	}
	return ret;
}

jobject Jni::getObjectArrayElement(jobjectArray array, sl_uint32 index)
{
	jobject ret = sl_null;
	if (array) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->GetObjectArrayElement(array, index);
		}
	}
	return ret;
}

void Jni::setObjectArrayElement(jobjectArray array, sl_uint32 index, jobject value)
{
	if (array) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			env->SetObjectArrayElement(array, index, value);
		}
	}
}

jobjectArray Jni::newStringArray(sl_uint32 length)
{
	SLIB_STATIC_STRING(cls, "java/lang/String");
	return Jni::newObjectArray(Jni::getClass(cls), length);
}

String Jni::getStringArrayElement(jobjectArray array, sl_uint32 index)
{
	String ret;
	JniLocal<jstring> v((jstring)(Jni::getObjectArrayElement(array, index)));
	if (v.isNotNull()) {
		ret = Jni::getString(v);
	}
	return ret;
}

String16 Jni::getString16ArrayElement(jobjectArray array, sl_uint32 index)
{
	String16 ret;
	JniLocal<jstring> v((jstring)(Jni::getObjectArrayElement(array, index)));
	if (v.isNotNull()) {
		ret = Jni::getString16(v);
	}
	return ret;
}

void Jni::setStringArrayElement(jobjectArray array, sl_uint32 index, const String& value)
{
	JniLocal<jstring> v(Jni::getJniString(value));
	Jni::setObjectArrayElement(array, index, v);
}

void Jni::setString16ArrayElement(jobjectArray array, sl_uint32 index, const String16& value)
{
	JniLocal<jstring> v(Jni::getJniString16(value));
	Jni::setObjectArrayElement(array, index, v);
}

#define DEFINE_JNI_ARRAY(TYPE, NAME) \
TYPE##Array Jni::new##NAME##Array(sl_uint32 length) \
{ \
	TYPE##Array ret = sl_null; \
	JNIEnv* env = Jni::getCurrent(); \
	if (env) { \
		ret = env->New##NAME##Array(length); \
	} \
	return ret; \
} \
TYPE* Jni::get##NAME##ArrayElements(TYPE##Array array, jboolean* isCopy) \
{ \
	TYPE* ret = sl_null; \
	if (array) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->Get##NAME##ArrayElements(array, isCopy); \
		} \
	} \
	return ret; \
} \
void Jni::release##NAME##ArrayElements(TYPE##Array array, TYPE* buf, jint mode) \
{ \
	if (array && buf) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->Release##NAME##ArrayElements(array, buf, mode); \
		} \
	} \
} \
void Jni::get##NAME##ArrayRegion(TYPE##Array array, sl_uint32 index, sl_uint32 len, TYPE* buf) \
{ \
	if (array && buf) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->Get##NAME##ArrayRegion(array, index, len, buf); \
		} \
	} \
} \
void Jni::set##NAME##ArrayRegion(TYPE##Array array, sl_uint32 index, sl_uint32 len, TYPE* buf) \
{ \
	if (array && buf) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->Set##NAME##ArrayRegion(array, index, len, buf); \
		} \
	} \
} \

DEFINE_JNI_ARRAY(jboolean, Boolean)
DEFINE_JNI_ARRAY(jbyte, Byte)
DEFINE_JNI_ARRAY(jchar, Char)
DEFINE_JNI_ARRAY(jshort, Short)
DEFINE_JNI_ARRAY(jint, Int)
DEFINE_JNI_ARRAY(jlong, Long)
DEFINE_JNI_ARRAY(jfloat, Float)
DEFINE_JNI_ARRAY(jdouble, Double)

jobject Jni::newDirectByteBuffer(void* address, sl_size capacity)
{
	JNIEnv* env = Jni::getCurrent();
	if (env && address && capacity) {
		return env->NewDirectByteBuffer(address, capacity);
	}
	return sl_null;
}

void* Jni::getDirectBufferAddress(jobject buf)
{
	JNIEnv* env = Jni::getCurrent();
	if (env && buf) {
		return env->GetDirectBufferAddress(buf);
	}
	return sl_null;
}

sl_size Jni::getDirectBufferCapacity(jobject buf)
{
	JNIEnv* env = Jni::getCurrent();
	if (env && buf) {
		return (sl_size)(env->GetDirectBufferCapacity(buf));
	}
	return 0;
}

sl_bool Jni::checkException()
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->ExceptionCheck() != 0;
	}
	return sl_false;
}

void Jni::clearException()
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		env->ExceptionClear();
	}
}

void Jni::printException()
{
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		env->ExceptionDescribe();
	}
}


SLIB_JNI_BEGIN_CLASS(_JniInputStream, "java/io/InputStream")
	SLIB_JNI_METHOD(read, "read", "([B)I");
	SLIB_JNI_METHOD(close, "close", "()V");
SLIB_JNI_END_CLASS

sl_int32 Jni::readFromInputStream(jobject stream, jbyteArray array)
{
	if (stream && array) {
		sl_int32 n = _JniInputStream::read.callInt(stream, array);
		if (n < 0) {
			n = 0;
		}
		if (Jni::checkException()) {
			n = -1;
			Jni::printException();
			Jni::clearException();
		}
		return n;
	}
	return -1;
}

void Jni::closeInputStream(jobject stream)
{
	if (stream) {
		_JniInputStream::close.call(stream);
		if (Jni::checkException()) {
			Jni::printException();
			Jni::clearException();
		}
	}
}

/************************************************
	JniClass
**************************************************/

SLIB_DEFINE_ROOT_OBJECT(_JniGlobalBase)

template class _JniGlobal<jobject>;
template class _JniGlobal<jclass>;
template class _JniGlobal<jstring>;

SLIB_DEFINE_REF_WRAPPER(JniClass, JniSafeClass, _JniGlobal<jclass>, ref)

SLIB_DEFINE_REF_WRAPPER(JniSafeClass, JniClass, _JniGlobal<jclass>, ref)

JniClass::JniClass(jclass cls) : ref(_JniGlobal<jclass>::from(cls))
{
}

JniClass& JniClass::operator=(jclass cls)
{
	ref = _JniGlobal<jclass>::from(cls);
	return *this;
}

JniClass JniClass::from(jclass cls)
{
	return JniClass(cls);
}

JniClass JniClass::getClassOfObject(jobject obj)
{
	JniClass ret;
	if (obj) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jclass> cls = env->GetObjectClass(obj);
			ret = JniClass::from(cls);
		}
	}
	return ret;
}

jclass JniClass::get() const
{
	_JniGlobal<jclass>* o = ref.ptr;
	if (ref.isNotNull()) {
		return o->object;
	} else {
		return 0;
	}
}

JniClass::operator jclass() const
{
	return get();
}

sl_bool JniClass::isInstanceOf(jobject obj) const
{
	jclass cls = get();
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		return env->IsInstanceOf(obj, cls) != 0;
	}
	return sl_false;
}

jmethodID JniClass::getMethodID(const char* name, const char* sig) const
{
	jmethodID ret = sl_null;
	jclass cls = get();
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		ret = env->GetMethodID(cls, name, sig);
		if (Jni::checkException()) {
			Jni::printException();
			Jni::clearException();
		}
		return ret;
	}
	return 0;
}

jmethodID JniClass::getStaticMethodID(const char* name, const char* sig) const
{
	jmethodID ret = sl_null;
	jclass cls = get();
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		ret = env->GetStaticMethodID(cls, name, sig);
		if (Jni::checkException()) {
			Jni::printException();
			Jni::clearException();
		}
	}
	return ret;
}

jfieldID JniClass::getFieldID(const char* name, const char* sig) const
{
	jfieldID ret = sl_null;
	jclass cls = get();
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		ret = env->GetFieldID(cls, name, sig);
		if (Jni::checkException()) {
			Jni::printException();
			Jni::clearException();
		}
	}
	return ret;
}

jfieldID JniClass::getStaticFieldID(const char* name, const char* sig) const
{
	jfieldID ret = sl_null;
	jclass cls = get();
	JNIEnv* env = Jni::getCurrent();
	if (env) {
		ret = env->GetStaticFieldID(cls, name, sig);
		if (Jni::checkException()) {
			Jni::printException();
			Jni::clearException();
		}
	}
	return ret;
}

#define DEFINE_JNI_CALL_METHOD(TYPE, NAME) \
TYPE JniClass::call##NAME##Method(jmethodID method, jobject _this, ...) const \
{ \
	va_list args; \
	va_start(args, _this); \
	TYPE ret = 0; \
	if (method && _this) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->Call##NAME##MethodV(_this, method, args); \
		} \
	} \
	va_end(args); \
	return ret; \
} \
TYPE JniClass::call##NAME##Method(const char* name, const char* sig, jobject _this, ...) const \
{ \
	va_list args; \
	va_start(args, _this); \
	TYPE ret = 0; \
	if (_this) { \
		jmethodID method = getMethodID(name, sig); \
		if (method) { \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				ret = env->Call##NAME##MethodV(_this, method, args); \
			} \
		} else { \
			_logJniError(String("Failed to get method id - ") + name + " " + sig); \
		} \
	} \
	va_end(args); \
	return ret; \
} \
TYPE JniClass::callStatic##NAME##Method(jmethodID method, ...) const \
{ \
	va_list args; \
	va_start(args, method); \
	TYPE ret = 0; \
	if (method) { \
		jclass cls = get(); \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->CallStatic##NAME##MethodV(cls, method, args); \
		} \
	} \
	va_end(args); \
	return ret; \
} \
TYPE JniClass::callStatic##NAME##Method(const char* name, const char* sig, ...) const \
{ \
	va_list args; \
	va_start(args, sig); \
	TYPE ret = 0; \
	jclass cls = get(); \
	jmethodID method = getStaticMethodID(name, sig); \
	if (method) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->CallStatic##NAME##MethodV(cls, method, args); \
		} \
	} else { \
		_logJniError(String("Failed to get static method id - ") + name + " " + sig); \
	} \
	va_end(args); \
	return ret; \
} \
TYPE _JniSingletonMethod::call##NAME(jobject _this, ...) \
{ \
	va_list args; \
	va_start(args, _this); \
	TYPE ret = 0; \
	if (cls && id && _this) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->Call##NAME##MethodV(_this, id, args); \
			if (Jni::checkException()) { \
				Jni::printException(); \
				Jni::clearException(); \
			} \
		} \
	} \
	return ret;\
} \
TYPE _JniSingletonStaticMethod::call##NAME(jobject _null, ...) \
{ \
	va_list args; \
	va_start(args, _null); \
	TYPE ret = 0; \
	if (cls && id) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->CallStatic##NAME##MethodV(cls, id, args); \
			if (Jni::checkException()) { \
				Jni::printException(); \
				Jni::clearException(); \
			} \
		} \
	} \
	return ret;\
}

DEFINE_JNI_CALL_METHOD(jobject, Object)
DEFINE_JNI_CALL_METHOD(jboolean, Boolean)
DEFINE_JNI_CALL_METHOD(jbyte, Byte)
DEFINE_JNI_CALL_METHOD(jchar, Char)
DEFINE_JNI_CALL_METHOD(jshort, Short)
DEFINE_JNI_CALL_METHOD(jint, Int)
DEFINE_JNI_CALL_METHOD(jlong, Long)
DEFINE_JNI_CALL_METHOD(jfloat, Float)
DEFINE_JNI_CALL_METHOD(jdouble, Double)

jobject JniClass::newObject(jmethodID method, ...) const
{
	va_list args;
	va_start(args, method);
	jobject ret = sl_null;
	if (method) {
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->NewObjectV(cls, method, args);
		}
	}
	va_end(args);
	return ret;
}

jobject JniClass::newObject(const char* sig, ...) const
{
	va_list args;
	va_start(args, sig);
	jobject ret = sl_null;
	jclass cls = get();
	jmethodID method = getMethodID("<init>", sig);
	if (method) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->NewObjectV(cls, method, args);
		}
	} else {
		_logJniError(String("Failed to get constructor id - <init> ") + sig);
	}
	va_end(args);
	return ret;
}

jobject JniClass::newObject() const
{
	return newObject("()V");
}

jobject _JniSingletonMethod::newObject(jobject _null, ...)
{
	va_list args;
	va_start(args, _null);
	jobject ret = sl_null;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			ret = env->NewObjectV(cls, id, args);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
	va_end(args);
	return ret;
}

void JniClass::callVoidMethod(jmethodID method, jobject _this, ...) const
{
	va_list args;
	va_start(args, _this);
	if (method && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			env->CallVoidMethodV(_this, method, args);
		}
	}
	va_end(args);
}

void JniClass::callVoidMethod(const char* name, const char* sig, jobject _this, ...) const
{
	va_list args;
	va_start(args, _this);
	if (_this) {
		jmethodID method = getMethodID(name, sig);
		if (method) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				env->CallVoidMethodV(_this, method, args);
			}
		} else {
			_logJniError(String("Failed to get method id - ") + name + " " + sig);
		}
	}
	va_end(args);
}

void _JniSingletonMethod::call(jobject _this, ...)
{
	va_list args;
	va_start(args, _this);
	if (cls && id && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			env->CallVoidMethodV(_this, id, args);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
	va_end(args);
}

void JniClass::callStaticVoidMethod(jmethodID method, ...) const
{
	va_list args;
	va_start(args, method);
	if (method) {
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			env->CallStaticVoidMethodV(cls, method, args);
		}
	}
	va_end(args);
}

void JniClass::callStaticVoidMethod(const char* name, const char* sig, ...) const
{
	va_list args;
	va_start(args, sig);
	jclass cls = get();
	jmethodID method = getStaticMethodID(name, sig);
	if (method) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			env->CallStaticVoidMethodV(cls, method, args);
		}
	} else {
		_logJniError(String("Failed to get static method id - ") + name + " " + sig);
	}
	va_end(args);
}

void _JniSingletonStaticMethod::call(jobject _null, ...)
{
	va_list args;
	va_start(args, _null);
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			env->CallStaticVoidMethodV(cls, id, args);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
	va_end(args);
}

String JniClass::callStringMethod(jmethodID method, jobject _this, ...) const
{
	va_list args;
	va_start(args, _this);
	String ret;
	if (method && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallObjectMethodV(_this, method, args)));
			if (str.isNotNull()) {
				ret = Jni::getString(str);
			}
		}
	}
	va_end(args);
	return ret;
}

String16 JniClass::callString16Method(jmethodID method, jobject _this, ...) const
{
	va_list args;
	va_start(args, _this);
	String16 ret;
	if (method && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallObjectMethodV(_this, method, args)));
			if (str.isNotNull()) {
				ret = Jni::getString16(str);
			}
		}
	}
	va_end(args);
	return ret;
}

String JniClass::callStringMethod(const char* name, const char* sig, jobject _this, ...) const
{
	va_list args;
	va_start(args, _this);
	String ret;
	if (_this) {
		jmethodID method = getMethodID(name, sig);
		if (method) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				JniLocal<jstring> str((jstring)(env->CallObjectMethodV(_this, method, args)));
				if (str.isNotNull()) {
					ret = Jni::getString(str);
				}
			}
		} else {
			_logJniError(String("Failed to get method id - ") + name + " " + sig);
		}
	}
	va_end(args);
	return ret;
}

String16 JniClass::callString16Method(const char* name, const char* sig, jobject _this, ...) const
{
	va_list args;
	va_start(args, _this);
	String16 ret;
	if (_this) {
		jmethodID method = getMethodID(name, sig);
		if (method) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				JniLocal<jstring> str((jstring)(env->CallObjectMethodV(_this, method, args)));
				if (str.isNotNull()) {
					ret = Jni::getString16(str);
				}
			}
		} else {
			_logJniError(String("Failed to get method id - ") + name + " " + sig);
		}
	}
	va_end(args);
	return ret;
}

String _JniSingletonMethod::callString(jobject _this, ...)
{
	va_list args;
	va_start(args, _this);
	String ret;
	if (cls && id && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallObjectMethodV(_this, id, args)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString(str);
				}
			}
		}
	}
	va_end(args);
	return ret;
}

String16 _JniSingletonMethod::callString16(jobject _this, ...)
{
	va_list args;
	va_start(args, _this);
	String16 ret;
	if (cls && id && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallObjectMethodV(_this, id, args)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString16(str);
				}
			}
		}
	}
	va_end(args);
	return ret;
}

String JniClass::callStaticStringMethod(jmethodID method, ...) const
{
	va_list args;
	va_start(args, method);
	String ret;
	if (method) {
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallStaticObjectMethodV(cls, method, args)));
			if (str.isNotNull()) {
				ret = Jni::getString(str);
			}
		}
	}
	va_end(args);
	return ret;
}

String16 JniClass::callStaticString16Method(jmethodID method, ...) const
{
	va_list args;
	va_start(args, method);
	String16 ret;
	if (method) {
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallStaticObjectMethodV(cls, method, args)));
			if (str.isNotNull()) {
				ret = Jni::getString16(str);
			}
		}
	}
	va_end(args);
	return ret;
}

String JniClass::callStaticStringMethod(const char* name, const char* sig, ...) const
{
	va_list args;
	va_start(args, sig);
	String ret;
	jclass cls = get();
	jmethodID method = getStaticMethodID(name, sig);
	if (method) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallStaticObjectMethodV(cls, method, args)));
			if (str.isNotNull()) {
				ret = Jni::getString(str);
			}
		}
	} else {
		_logJniError(String("Failed to get static method id - ") + name + " " + sig);
	}
	va_end(args);
	return ret;
}

String16 JniClass::callStaticString16Method(const char* name, const char* sig, ...) const
{
	va_list args;
	va_start(args, sig);
	String16 ret;
	jclass cls = get();
	jmethodID method = getStaticMethodID(name, sig);
	if (method) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallStaticObjectMethodV(cls, method, args)));
			if (str.isNotNull()) {
				ret = Jni::getString16(str);
			}
		}
	} else {
		_logJniError(String("Failed to get static method id - ") + name + " " + sig);
	}
	va_end(args);
	return ret;
}

String _JniSingletonStaticMethod::callString(jobject _null, ...)
{
	va_list args;
	va_start(args, _null);
	String ret;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallStaticObjectMethodV(cls, id, args)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString(str);
				}
			}
		}
	}
	va_end(args);
	return ret;
}

String16 _JniSingletonStaticMethod::callString16(jobject _null, ...)
{
	va_list args;
	va_start(args, _null);
	String16 ret;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->CallStaticObjectMethodV(cls, id, args)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString16(str);
				}
			}
		}
	}
	va_end(args);
	return ret;
}

#define DEFINE_JNI_FIELD(TYPE, NAME) \
TYPE JniClass::get##NAME##Field(jfieldID field, jobject _this) const \
{ \
	TYPE ret = 0; \
	if (field && _this) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->Get##NAME##Field(_this, field); \
		} \
	} \
	return ret; \
} \
TYPE JniClass::get##NAME##Field(const char* name, const char* sig, jobject _this) const \
{ \
	TYPE ret = 0; \
	if (_this) { \
		jfieldID field = getFieldID(name, sig); \
		if (field) { \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				ret = env->Get##NAME##Field(_this, field); \
			} \
		} else { \
			_logJniError(String("Failed to get field id - ") + name + " " + sig); \
		} \
	} \
	return ret; \
} \
TYPE JniClass::getStatic##NAME##Field(jfieldID field) const \
{ \
	TYPE ret = 0; \
	if (field) { \
		jclass cls = get(); \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->GetStatic##NAME##Field(cls, field); \
		} \
	} \
	return ret; \
} \
TYPE JniClass::getStatic##NAME##Field(const char* name, const char* sig) const \
{ \
	TYPE ret = 0; \
	jclass cls = get(); \
	jfieldID field = getStaticFieldID(name, sig); \
	if (field) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->GetStatic##NAME##Field(cls, field); \
		} \
	} else { \
		_logJniError(String("Failed to get static field id - ") + name + " " + sig); \
	} \
	return ret; \
} \
void JniClass::set##NAME##Field(jfieldID field, jobject _this, TYPE value) const \
{ \
	if (field && _this) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->Set##NAME##Field(_this, field, value); \
		} \
	} \
} \
void JniClass::set##NAME##Field(const char* name, const char* sig, jobject _this, TYPE value) const \
{ \
	if (_this) { \
		jfieldID field = getFieldID(name, sig); \
		if (field) { \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				env->Set##NAME##Field(_this, field, value); \
			} \
		} else { \
			_logJniError(String("Failed to get field id - ") + name + " " + sig); \
		} \
	} \
} \
void JniClass::setStatic##NAME##Field(jfieldID field, TYPE value) const \
{ \
	if (field) { \
		jclass cls = get(); \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->SetStatic##NAME##Field(cls, field, value); \
		} \
	} \
} \
void JniClass::setStatic##NAME##Field(const char* name, const char* sig, TYPE value) const \
{ \
	jclass cls = get(); \
	jfieldID field = getStaticFieldID(name, sig); \
	if (field) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->SetStatic##NAME##Field(cls, field, value); \
		} \
	} else { \
		_logJniError(String("Failed to get static field id - ") + name + " " + sig); \
	} \
} \
TYPE _JniSingletonField::get##NAME(jobject _this) \
{ \
	TYPE ret = 0; \
	if (cls && id && _this) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->Get##NAME##Field(_this, id); \
			if (Jni::checkException()) { \
				Jni::printException(); \
				Jni::clearException(); \
			} \
		} \
	} \
	return ret; \
} \
void _JniSingletonField::set##NAME(jobject _this, TYPE value) \
{ \
	if (cls && id && _this) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->Set##NAME##Field(_this, id, value); \
			if (Jni::checkException()) { \
				Jni::printException(); \
				Jni::clearException(); \
			} \
		} \
	} \
} \
TYPE _JniSingletonStaticField::get##NAME(jobject _null) \
{ \
	TYPE ret = 0; \
	if (cls && id) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			ret = env->GetStatic##NAME##Field(cls, id); \
			if (Jni::checkException()) { \
				Jni::printException(); \
				Jni::clearException(); \
			} \
		} \
	} \
	return ret; \
} \
void _JniSingletonStaticField::set##NAME(jobject _null, TYPE value) \
{ \
	if (cls && id) { \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			env->SetStatic##NAME##Field(cls, id, value); \
			if (Jni::checkException()) { \
				Jni::printException(); \
				Jni::clearException(); \
			} \
		} \
	} \
}

DEFINE_JNI_FIELD(jobject, Object)
DEFINE_JNI_FIELD(jboolean, Boolean)
DEFINE_JNI_FIELD(jbyte, Byte)
DEFINE_JNI_FIELD(jchar, Char)
DEFINE_JNI_FIELD(jshort, Short)
DEFINE_JNI_FIELD(jint, Int)
DEFINE_JNI_FIELD(jlong, Long)
DEFINE_JNI_FIELD(jfloat, Float)
DEFINE_JNI_FIELD(jdouble, Double)

String JniClass::getStringField(jfieldID field, jobject _this) const
{
	String ret;
	JniLocal<jstring> str((jstring)(getObjectField(field, _this)));
	if (str.isNotNull()) {
		ret = Jni::getString(str);
	}
	return ret;
}

String16 JniClass::getString16Field(jfieldID field, jobject _this) const
{
	String16 ret;
	JniLocal<jstring> str((jstring)(getObjectField(field, _this)));
	if (str.isNotNull()) {
		ret = Jni::getString16(str);
	}
	return ret;
}

String JniClass::getStringField(const char* name, const char* sig, jobject _this) const
{
	String ret;
	JniLocal<jstring> str((jstring)(getObjectField(name, sig, _this)));
	if (str.isNotNull()) {
		ret = Jni::getString(str);
	}
	return ret;
}

String16 JniClass::getString16Field(const char* name, const char* sig, jobject _this) const
{
	String16 ret;
	JniLocal<jstring> str((jstring)(getObjectField(name, sig, _this)));
	if (str.isNotNull()) {
		ret = Jni::getString16(str);
	}
	return ret;
}

String JniClass::getStaticStringField(jfieldID field) const
{
	String ret;
	JniLocal<jstring> str((jstring)(getStaticObjectField(field)));
	if (str.isNotNull()) {
		ret = Jni::getString(str);
	}
	return ret;
}

String16 JniClass::getStaticString16Field(jfieldID field) const
{
	String16 ret;
	JniLocal<jstring> str((jstring)(getStaticObjectField(field)));
	if (str.isNotNull()) {
		ret = Jni::getString16(str);
	}
	return ret;
}

String JniClass::getStaticStringField(const char* name, const char* sig) const
{
	String ret;
	JniLocal<jstring> str((jstring)(getStaticObjectField(name, sig)));
	if (str.isNotNull()) {
		ret = Jni::getString(str);
	}
	return ret;
}

String16 JniClass::getStaticString16Field(const char* name, const char* sig) const
{
	String16 ret;
	JniLocal<jstring> str((jstring)(getStaticObjectField(name, sig)));
	if (str.isNotNull()) {
		ret = Jni::getString16(str);
	}
	return ret;
}

void JniClass::setStringField(jfieldID field, jobject _this, const String& value) const
{
	JniLocal<jstring> str(Jni::getJniString(value));
	setObjectField(field, _this, str);
}

void JniClass::setString16Field(jfieldID field, jobject _this, const String16& value) const
{
	JniLocal<jstring> str(Jni::getJniString16(value));
	setObjectField(field, _this, str);
}

void JniClass::setStringField(const char* name, const char* sig, jobject _this, const String& value) const
{
	JniLocal<jstring> str(Jni::getJniString(value));
	setObjectField(name, sig, _this, str);
}

void JniClass::setString16Field(const char* name, const char* sig, jobject _this, const String16& value) const
{
	JniLocal<jstring> str(Jni::getJniString16(value));
	setObjectField(name, sig, _this, str);
}

void JniClass::setStaticStringField(jfieldID field, const String& value) const
{
	JniLocal<jstring> str(Jni::getJniString(value));
	setStaticObjectField(field, str);
}

void JniClass::setStaticString16Field(jfieldID field, const String16& value) const
{
	JniLocal<jstring> str(Jni::getJniString16(value));
	setStaticObjectField(field, str);
}

void JniClass::setStaticStringField(const char* name, const char* sig, const String& value) const
{
	JniLocal<jstring> str(Jni::getJniString(value));
	setStaticObjectField(name, sig, str);
}

void JniClass::setStaticString16Field(const char* name, const char* sig, const String16& value) const
{
	JniLocal<jstring> str(Jni::getJniString16(value));
	setStaticObjectField(name, sig, str);
}

String _JniSingletonField::getString(jobject _this)
{
	String ret;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->GetObjectField(_this, id)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString(str);
				}
			}
		}
	}
	return ret;
}

String16 _JniSingletonField::getString16(jobject _this)
{
	String16 ret;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->GetObjectField(_this, id)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString16(str);
				}
			}
		}
	}
	return ret;
}

String _JniSingletonStaticField::getString(jobject _null)
{
	String ret;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->GetStaticObjectField(cls, id)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString(str);
				}
			}
		}
	}
	return ret;
}

String16 _JniSingletonStaticField::getString16(jobject _null)
{
	String16 ret;
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str((jstring)(env->GetStaticObjectField(cls, id)));
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				if (str.isNotNull()) {
					ret = Jni::getString16(str);
				}
			}
		}
	}
	return ret;
}

void _JniSingletonField::setString(jobject _this, const String& value)
{
	if (cls && id && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str(Jni::getJniString(value));
			env->SetObjectField(_this, id, str);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
}

void _JniSingletonField::setString16(jobject _this, const String16& value)
{
	if (cls && id && _this) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str(Jni::getJniString16(value));
			env->SetObjectField(_this, id, str);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
}

void _JniSingletonStaticField::setString(jobject _null, const String& value)
{
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str(Jni::getJniString(value));
			env->SetObjectField(cls, id, str);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
}

void _JniSingletonStaticField::setString16(jobject _null, const String16& value)
{
	if (cls && id) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JniLocal<jstring> str(Jni::getJniString16(value));
			env->SetObjectField(cls, id, str);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}
}

sl_bool JniClass::registerNative(const char* name, const char* sig, const void* fn) const
{
	jclass cls = get();
	if (cls) {
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			JNINativeMethod method;
			method.name = (char*)name;
			method.signature = (char*)sig;
			method.fnPtr = (void*)fn;

			return 0 == env->RegisterNatives(cls, &method, 1);
		}
	}
	return sl_false;
}

JniSafeClass::JniSafeClass(jclass cls) : ref(_JniGlobal<jclass>::from(cls))
{
}

JniSafeClass& JniSafeClass::operator=(jclass cls)
{
	ref = _JniGlobal<jclass>::from(cls);
	return *this;
}

_JniSingletonClass::_JniSingletonClass(const char* name)
{
	this->name = name;
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->singleton_classes.add(this);
	}
}

_JniSingletonMethod::_JniSingletonMethod(_JniSingletonClass* gcls, const char* name, const char* sig)
{
	this->gcls = gcls;
	this->name = name;
	this->sig = sig;
	this->cls = sl_null;
	this->id = sl_null;
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->singleton_methods.add(this);
	}
}

_JniSingletonStaticMethod::_JniSingletonStaticMethod(_JniSingletonClass* gcls, const char* name, const char* sig)
{
	this->gcls = gcls;
	this->name = name;
	this->sig = sig;
	this->cls = sl_null;
	this->id = sl_null;
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->singleton_static_methods.add(this);
	}
}

_JniSingletonField::_JniSingletonField(_JniSingletonClass* gcls, const char* name, const char* sig)
{
	this->gcls = gcls;
	this->name = name;
	this->sig = sig;
	this->cls = sl_null;
	this->id = sl_null;
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->singleton_fields.add(this);
	}
}

_JniSingletonObjectField::_JniSingletonObjectField(_JniSingletonClass* gcls, const char* name, const char* sig)
	: _JniSingletonField(gcls, name, sig)
{
}

jobject _JniSingletonObjectField::get(jobject _this)
{
	return getObject(_this);
}

void _JniSingletonObjectField::set(jobject _this, jobject value)
{
	setObject(_this, value);
}


#define _SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(TYPE, NAME, SIG) \
	_JniSingleton##NAME##Field::_JniSingleton##NAME##Field(_JniSingletonClass* gcls, const char* name) \
		: _JniSingletonField(gcls, name, SIG) \
	{ \
	} \
	TYPE _JniSingleton##NAME##Field::get(jobject _this) \
	{ \
		return get##NAME(_this); \
	} \
	void _JniSingleton##NAME##Field::set(jobject _this, TYPE value) \
	{ \
		set##NAME(_this, value); \
	} \

_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(jboolean, Boolean, "Z")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(sl_int8, Byte, "B")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(sl_uint16, Char, "C")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(sl_int16, Short, "S")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(sl_int32, Int, "I")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(sl_int64, Long, "J")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(float, Float, "F")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(double, Double, "D")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(String, String, "Ljava/lang/String;")
_SLIB_JNI_DEFINE_SINGLETON_FIELD_TYPE(String16, String16, "Ljava/lang/String;")


_JniSingletonStaticField::_JniSingletonStaticField(_JniSingletonClass* gcls, const char* name, const char* sig)
{
	this->gcls = gcls;
	this->name = name;
	this->sig = sig;
	this->cls = sl_null;
	this->id = sl_null;
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->singleton_static_fields.add(this);
	}
}


_JniSingletonStaticObjectField::_JniSingletonStaticObjectField(_JniSingletonClass* gcls, const char* name, const char* sig)
	: _JniSingletonStaticField(gcls, name, sig)
{
}

jobject _JniSingletonStaticObjectField::get()
{
	return getObject(sl_null);
}

void _JniSingletonStaticObjectField::set(jobject value)
{
	setObject(sl_null, value);
}


#define _SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(TYPE, NAME, SIG) \
	_JniSingletonStatic##NAME##Field::_JniSingletonStatic##NAME##Field(_JniSingletonClass* gcls, const char* name) \
		: _JniSingletonStaticField(gcls, name, SIG) \
	{ \
	} \
	TYPE _JniSingletonStatic##NAME##Field::get() \
	{ \
		return get##NAME(sl_null); \
	} \
	void _JniSingletonStatic##NAME##Field::set(TYPE value) \
	{ \
		set##NAME(sl_null, value); \
	} \

_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(jboolean, Boolean, "Z")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(sl_int8, Byte, "B")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(sl_uint16, Char, "C")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(sl_int16, Short, "S")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(sl_int32, Int, "I")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(sl_int64, Long, "J")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(float, Float, "F")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(double, Double, "D")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(String, String, "Ljava/lang/String;")
_SLIB_JNI_DEFINE_SINGLETON_STATIC_FIELD_TYPE(String16, String16, "Ljava/lang/String;")


_JniNativeMethod::_JniNativeMethod(_JniSingletonClass* gcls, const char* name, const char* sig, const void* fn)
{
	this->gcls = gcls;
	this->name = name;
	this->sig = sig;
	this->fn = fn;
	_Jni_Shared* shared = _Jni_getShared();
	if (shared) {
		shared->native_methods.add(this);
	}
}

SLIB_NAMESPACE_END

#endif
