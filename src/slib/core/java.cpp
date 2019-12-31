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

#ifdef SLIB_PLATFORM_USE_JNI

#include "slib/core/java.h"
#include "slib/core/hash_map.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

#define JNIVERSION JNI_VERSION_1_4

namespace slib
{

	namespace priv
	{
		namespace java
		{

			static void logJniError(const String& text)
			{
				LogError("SLIB_JNI", text);
			}


			JavaVM* g_jvmShared = sl_null;
			SLIB_THREAD JNIEnv* g_envCurrent = sl_null;


			class SharedContext
			{
			public:
				CHashMap<String, JniClass> classes;
				
				CList<JClass*> singleton_classes;
				CList<JMethod*> singleton_methods;
				CList<JStaticMethod*> singleton_static_methods;
				CList<JField*> singleton_fields;
				CList<JStaticField*> singleton_static_fields;
				CList<JNativeMethod*> native_methods;
			};

			SLIB_SAFE_STATIC_GETTER(SharedContext, getSharedContext)


			SLIB_JNI_BEGIN_CLASS(JInputStream, "java/io/InputStream")
				SLIB_JNI_METHOD(read, "read", "([B)I");
				SLIB_JNI_METHOD(close, "close", "()V");
			SLIB_JNI_END_CLASS


			JClass::JClass(const char* name)
			{
				this->name = name;
				SharedContext* shared = getSharedContext();
				if (shared) {
					shared->singleton_classes.add(this);
				}
			}

			JMethod::JMethod(JClass* gcls, const char* name, const char* sig)
			{
				this->gcls = gcls;
				this->name = name;
				this->sig = sig;
				this->cls = sl_null;
				this->id = sl_null;
				SharedContext* shared = getSharedContext();
				if (shared) {
					shared->singleton_methods.add(this);
				}
			}

			jobject JMethod::newObject(jobject _null, ...)
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

			void JMethod::call(jobject _this, ...)
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

			String JMethod::callString(jobject _this, ...)
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

			JStaticMethod::JStaticMethod(JClass* gcls, const char* name, const char* sig)
			{
				this->gcls = gcls;
				this->name = name;
				this->sig = sig;
				this->cls = sl_null;
				this->id = sl_null;
				SharedContext* shared = getSharedContext();
				if (shared) {
					shared->singleton_static_methods.add(this);
				}
			}

			void JStaticMethod::call(jobject _null, ...)
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

			String JStaticMethod::callString(jobject _null, ...)
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

#define DEFINE_JMETHOD_MEMBERS(TYPE, NAME) \
			TYPE JMethod::call##NAME(jobject _this, ...) \
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
				va_end(args); \
				return ret; \
			} \
			TYPE JStaticMethod::call##NAME(jobject _null, ...) \
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
				va_end(args); \
				return ret; \
			}

			DEFINE_JMETHOD_MEMBERS(jobject, Object)
			DEFINE_JMETHOD_MEMBERS(jboolean, Boolean)
			DEFINE_JMETHOD_MEMBERS(jbyte, Byte)
			DEFINE_JMETHOD_MEMBERS(jchar, Char)
			DEFINE_JMETHOD_MEMBERS(jshort, Short)
			DEFINE_JMETHOD_MEMBERS(jint, Int)
			DEFINE_JMETHOD_MEMBERS(jlong, Long)
			DEFINE_JMETHOD_MEMBERS(jfloat, Float)
			DEFINE_JMETHOD_MEMBERS(jdouble, Double)

			JField::JField(JClass* gcls, const char* name, const char* sig)
			{
				this->gcls = gcls;
				this->name = name;
				this->sig = sig;
				this->cls = sl_null;
				this->id = sl_null;
				SharedContext* shared = getSharedContext();
				if (shared) {
					shared->singleton_fields.add(this);
				}
			}

			String JField::getString(jobject _this)
			{
				if (cls && id) {
					JNIEnv* env = Jni::getCurrent();
					if (env) {
						JniLocal<jstring> str((jstring)(env->GetObjectField(_this, id)));
						if (Jni::checkException()) {
							Jni::printException();
							Jni::clearException();
						} else {
							if (str.isNotNull()) {
								return Jni::getString(str);
							}
						}
					}
				}
				return sl_null;
			}

			void JField::setString(jobject _this, const StringParam& value)
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

			JStaticField::JStaticField(JClass* gcls, const char* name, const char* sig)
			{
				this->gcls = gcls;
				this->name = name;
				this->sig = sig;
				this->cls = sl_null;
				this->id = sl_null;
				SharedContext* shared = getSharedContext();
				if (shared) {
					shared->singleton_static_fields.add(this);
				}
			}

			String JStaticField::getString(jobject _null)
			{
				if (cls && id) {
					JNIEnv* env = Jni::getCurrent();
					if (env) {
						JniLocal<jstring> str((jstring)(env->GetStaticObjectField(cls, id)));
						if (Jni::checkException()) {
							Jni::printException();
							Jni::clearException();
						} else {
							if (str.isNotNull()) {
								return Jni::getString(str);
							}
						}
					}
				}
				return sl_null;
			}

			void JStaticField::setString(jobject _null, const StringParam& value)
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

#define DEFINE_JFIELD_MEMBERS(TYPE, NAME) \
			TYPE JField::get##NAME(jobject _this) \
			{ \
				if (cls && id && _this) { \
					JNIEnv* env = Jni::getCurrent(); \
					if (env) { \
						TYPE ret = env->Get##NAME##Field(_this, id); \
						if (Jni::checkException()) { \
							Jni::printException(); \
							Jni::clearException(); \
						} else { \
							return ret; \
						} \
					} \
				} \
				return 0; \
			} \
			void JField::set##NAME(jobject _this, TYPE value) \
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
			TYPE JStaticField::get##NAME(jobject _null) \
			{ \
				if (cls && id) { \
					JNIEnv* env = Jni::getCurrent(); \
					if (env) { \
						TYPE ret = env->GetStatic##NAME##Field(cls, id); \
						if (Jni::checkException()) { \
							Jni::printException(); \
							Jni::clearException(); \
						} else { \
							return ret; \
						} \
					} \
				} \
				return 0; \
			} \
			void JStaticField::set##NAME(jobject _null, TYPE value) \
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

			DEFINE_JFIELD_MEMBERS(jobject, Object)
			DEFINE_JFIELD_MEMBERS(jboolean, Boolean)
			DEFINE_JFIELD_MEMBERS(jbyte, Byte)
			DEFINE_JFIELD_MEMBERS(jchar, Char)
			DEFINE_JFIELD_MEMBERS(jshort, Short)
			DEFINE_JFIELD_MEMBERS(jint, Int)
			DEFINE_JFIELD_MEMBERS(jlong, Long)
			DEFINE_JFIELD_MEMBERS(jfloat, Float)
			DEFINE_JFIELD_MEMBERS(jdouble, Double)

#define DEFINE_JFIELD_TYPE_MEMBERS(TYPE, NAME, SIG) \
			J##NAME##Field::J##NAME##Field(JClass* gcls, const char* name) \
				: JField(gcls, name, SIG) \
			{ \
			} \
			TYPE J##NAME##Field::get(jobject _this) \
			{ \
				return get##NAME(_this); \
			} \
			void J##NAME##Field::set(jobject _this, TYPE value) \
			{ \
				set##NAME(_this, value); \
			} \

			DEFINE_JFIELD_TYPE_MEMBERS(jboolean, Boolean, "Z")
			DEFINE_JFIELD_TYPE_MEMBERS(sl_int8, Byte, "B")
			DEFINE_JFIELD_TYPE_MEMBERS(sl_uint16, Char, "C")
			DEFINE_JFIELD_TYPE_MEMBERS(sl_int16, Short, "S")
			DEFINE_JFIELD_TYPE_MEMBERS(sl_int32, Int, "I")
			DEFINE_JFIELD_TYPE_MEMBERS(sl_int64, Long, "J")
			DEFINE_JFIELD_TYPE_MEMBERS(float, Float, "F")
			DEFINE_JFIELD_TYPE_MEMBERS(double, Double, "D")
			DEFINE_JFIELD_TYPE_MEMBERS(String, String, "Ljava/lang/String;")

			JObjectField::JObjectField(JClass* gcls, const char* name, const char* sig)
				: JField(gcls, name, sig)
			{
			}

			jobject JObjectField::get(jobject _this)
			{
				return getObject(_this);
			}

			void JObjectField::set(jobject _this, jobject value)
			{
				setObject(_this, value);
			}

			JStaticObjectField::JStaticObjectField(JClass* gcls, const char* name, const char* sig)
				: JStaticField(gcls, name, sig)
			{
			}

			jobject JStaticObjectField::get()
			{
				return getObject(sl_null);
			}

			void JStaticObjectField::set(jobject value)
			{
				setObject(sl_null, value);
			}

#define DEFINE_JSTATICFIELD_TYPE_MEMBERS(TYPE, NAME, SIG) \
			JStatic##NAME##Field::JStatic##NAME##Field(JClass* gcls, const char* name) \
				: JStaticField(gcls, name, SIG) \
			{ \
			} \
			TYPE JStatic##NAME##Field::get() \
			{ \
				return get##NAME(sl_null); \
			} \
			void JStatic##NAME##Field::set(TYPE value) \
			{ \
				set##NAME(sl_null, value); \
			} \

			DEFINE_JSTATICFIELD_TYPE_MEMBERS(jboolean, Boolean, "Z")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(sl_int8, Byte, "B")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(sl_uint16, Char, "C")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(sl_int16, Short, "S")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(sl_int32, Int, "I")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(sl_int64, Long, "J")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(float, Float, "F")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(double, Double, "D")
			DEFINE_JSTATICFIELD_TYPE_MEMBERS(String, String, "Ljava/lang/String;")

			JNativeMethod::JNativeMethod(JClass* gcls, const char* name, const char* sig, const void* fn)
			{
				this->gcls = gcls;
				this->name = name;
				this->sig = sig;
				this->fn = fn;
				SharedContext* shared = getSharedContext();
				if (shared) {
					shared->native_methods.add(this);
				}
			}

		}
	}

	using namespace priv::java;

//#define JNI_LOG_INIT_LOAD

	void Jni::initialize(JavaVM* jvm)
	{
		static sl_bool flagInit = sl_false;
		
		if (! flagInit) {
			
			flagInit = sl_true;
			Jni::setSharedJVM(jvm);

			SharedContext* shared = getSharedContext();
			if (!shared) {
				return;
			}

			// singleton classes
			{
				ListLocker< JClass* > list(shared->singleton_classes);
				for (sl_size i = 0; i < list.count; i++) {
					JClass* obj = list[i];
#if defined(JNI_LOG_INIT_LOAD)
					Log("LOADING JAVA CLASS", obj->name);
#endif
					obj->cls = Jni::getClass(obj->name);
					if (obj->cls.isNull()) {
						Log("LOADING JAVA CLASS FAILED", obj->name);
					}
				}
			}


			// singleton fields
			{
				ListLocker< JField* > list(shared->singleton_fields);
				for (sl_size i = 0; i < list.count; i++) {
					JField* obj = list[i];
					JniClass cls = obj->gcls->cls;
					if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
						Log("LOADING JAVA FIELD", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
#endif
						obj->cls = cls;
						obj->id = cls.getFieldID(obj->name, obj->sig);
						if (obj->id == sl_null) {
							Log("LOADING JAVA FIELD FAILED", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
						}
					}
				}
			}
			// singleton static fields
			{
				ListLocker< JStaticField* > list(shared->singleton_static_fields);
				for (sl_size i = 0; i < list.count; i++) {
					JStaticField* obj = list[i];
					JniClass cls = obj->gcls->cls;
					if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
						Log("LOADING JAVA STATIC FIELD", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
#endif
						obj->cls = cls;
						obj->id = cls.getStaticFieldID(obj->name, obj->sig);
						if (obj->id == sl_null) {
							Log("LOADING JAVA STATIC FIELD FAILED", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
						}
					}
				}
			}
			// singleton methods
			{
				ListLocker< JMethod* > list(shared->singleton_methods);
				for (sl_size i = 0; i < list.count; i++) {
					JMethod* obj = list[i];
					JniClass cls = obj->gcls->cls;
					if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
						Log("LOADING JAVA METHOD", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
#endif
						obj->cls = cls;
						obj->id = cls.getMethodID(obj->name, obj->sig);
						if (obj->id == sl_null) {
							Log("LOADING JAVA METHOD FAILED", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
						}
					}
				}
			}
			// singleton static methods
			{
				ListLocker< JStaticMethod* > list(shared->singleton_static_methods);
				for (sl_size i = 0; i < list.count; i++) {
					JStaticMethod* obj = list[i];
					JniClass cls = obj->gcls->cls;
					if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
						Log("LOADING JAVA STATIC METHOD", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
#endif
						obj->cls = cls;
						obj->id = cls.getStaticMethodID(obj->name, obj->sig);
						if (obj->id == sl_null) {
							Log("LOADING JAVA STATIC METHOD FAILED", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
						}
					}
				}
			}
			// native methods
			{
				ListLocker< JNativeMethod* > list(shared->native_methods);
				for (sl_size i = 0; i < list.count; i++) {
					JNativeMethod* obj = list[i];
					JniClass cls = obj->gcls->cls;
					if (cls.isNotNull()) {
#if defined(JNI_LOG_INIT_LOAD)
						Log("REGISTERING JAVA NATIVE", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
#endif
						if (!cls.registerNative(obj->name, obj->sig, obj->fn)) {
							Log("REGISTERING JAVA NATIVE FAILED", "%s::%s %s", String(obj->gcls->name), obj->name, obj->sig);
						}
					}
				}
			}
		}
	}

	void Jni::setSharedJVM(JavaVM *jvm)
	{
		if (g_jvmShared != jvm) {
			g_jvmShared = jvm;
		}
	}

	JavaVM* Jni::getSharedJVM()
	{
		return g_jvmShared;
	}

	JNIEnv* Jni::getCurrent()
	{
		JNIEnv *env = g_envCurrent;
		if (! env) {
			env = Jni::attachThread();
		}
		return env;
	}

	void Jni::setCurrent(JNIEnv* jni)
	{
		g_envCurrent = jni;
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
					logJniError("Failed to attach thread");
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
				logJniError(String("Exception occurred while finding class: ") + className);
				Jni::printException();
				Jni::clearException();
			} else {
				if (cls.isNotNull()) {
					return JniClass::from(cls);
				}
			}
		}
		return sl_null;
	}

	JniClass Jni::getClass(const String& className)
	{
		SharedContext* shared = getSharedContext();
		if (!shared) {
			return sl_null;
		}
		JniClass ret;
		if (shared->classes.get(className, &ret)) {
			return ret;
		}
		ret = Jni::findClass(className.getData());
		if (ret.isNotNull()) {
			Jni::registerClass(className, ret);
			return ret;
		}
		return sl_null;
	}

	void Jni::registerClass(const String& className, jclass cls)
	{
		SharedContext* shared = getSharedContext();
		if (shared) {
			shared->classes.put(className, cls);
		}
	}

	void Jni::unregisterClass(const String& className)
	{
		SharedContext* shared = getSharedContext();
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

	jstring Jni::getJniString(const StringParam& _str)
	{
		if (_str.isNotNull()) {
			StringData16 str(_str);
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				return env->NewString((jchar*)(str.getData()), (jsize)(str.getLength()));
			}
		}
		return sl_null;
	}

	jstring Jni::getJniString(const sl_char16* str, sl_size length)
	{
		if (str) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				return env->NewString((jchar*)(str), (jsize)(length));
			}
		}
		return sl_null;
	}

	String Jni::getString(jstring str)
	{
		if (str) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				sl_uint32 len = (sl_uint32)(env->GetStringLength(str));
				const jchar* sz = env->GetStringChars(str, sl_null);
				if (sz) {
					String ret = String::create((const char16_t*)sz, len);
					env->ReleaseStringChars(str, sz);
					return ret;
				}
			}
		}
		return sl_null;
	}

	sl_uint32 Jni::getArrayLength(jarray array)
	{
		if (array) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				return env->GetArrayLength(array);
			}
		}
		return 0;
	}

	jobjectArray Jni::newObjectArray(jclass clsElement, sl_uint32 length)
	{
		if (clsElement) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				return env->NewObjectArray(length, clsElement, sl_null);
			}
		}
		return sl_null;
	}

	jobject Jni::getObjectArrayElement(jobjectArray array, sl_uint32 index)
	{
		if (array) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				return env->GetObjectArrayElement(array, index);
			}
		}
		return sl_null;
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
		JniLocal<jstring> v((jstring)(Jni::getObjectArrayElement(array, index)));
		if (v.isNotNull()) {
			return Jni::getString(v);
		}
		return sl_null;
	}

	void Jni::setStringArrayElement(jobjectArray array, sl_uint32 index, const StringParam& value)
	{
		JniLocal<jstring> v(Jni::getJniString(value));
		Jni::setObjectArrayElement(array, index, v);
	}

#define DEFINE_JNI_ARRAY(TYPE, NAME) \
	TYPE##Array Jni::new##NAME##Array(sl_uint32 length) \
	{ \
		JNIEnv* env = Jni::getCurrent(); \
		if (env) { \
			return env->New##NAME##Array(length); \
		} \
		return sl_null; \
	} \
	TYPE* Jni::get##NAME##ArrayElements(TYPE##Array array, jboolean* isCopy) \
	{ \
		if (array) { \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				return env->Get##NAME##ArrayElements(array, isCopy); \
			} \
		} \
		return sl_null; \
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

	sl_int32 Jni::readFromInputStream(jobject stream, jbyteArray array)
	{
		if (stream && array) {
			sl_int32 n = JInputStream::read.callInt(stream, array);
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
			JInputStream::close.call(stream);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			}
		}
	}

/************************************************
				JniClass
**************************************************/

	SLIB_DEFINE_ROOT_OBJECT(CJniGlobalBase)

	JniClass::JniClass(jclass cls) : ref(CJniGlobal<jclass>::from(cls))
	{
	}

	JniClass& JniClass::operator=(jclass cls)
	{
		ref = CJniGlobal<jclass>::from(cls);
		return *this;
	}

	JniClass JniClass::from(jclass cls)
	{
		return JniClass(cls);
	}

	JniClass JniClass::getClassOfObject(jobject obj)
	{
		if (obj) {
			JNIEnv* env = Jni::getCurrent();
			if (env) {
				JniLocal<jclass> cls = env->GetObjectClass(obj);
				return JniClass::from(cls);
			}
		}
		return sl_null;
	}

	jclass JniClass::get() const
	{
		CJniGlobal<jclass>* o = ref.get();
		if (ref.isNotNull()) {
			return o->object;
		}
		return sl_null;
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
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			jmethodID ret = env->GetMethodID(cls, name, sig);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				return ret;
			}
		}
		return sl_null;
	}

	jmethodID JniClass::getStaticMethodID(const char* name, const char* sig) const
	{
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			jmethodID ret = env->GetStaticMethodID(cls, name, sig);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				return ret;
			}
		}
		return sl_null;
	}

	jfieldID JniClass::getFieldID(const char* name, const char* sig) const
	{
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			jfieldID ret = env->GetFieldID(cls, name, sig);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				return ret;
			}
		}
		return sl_null;
	}

	jfieldID JniClass::getStaticFieldID(const char* name, const char* sig) const
	{
		jclass cls = get();
		JNIEnv* env = Jni::getCurrent();
		if (env) {
			jfieldID ret = env->GetStaticFieldID(cls, name, sig);
			if (Jni::checkException()) {
				Jni::printException();
				Jni::clearException();
			} else {
				return ret;
			}
		}
		return sl_null;
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
				logJniError(String("Failed to get method id - ") + name + " " + sig); \
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
			logJniError(String("Failed to get static method id - ") + name + " " + sig); \
		} \
		va_end(args); \
		return ret; \
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
			logJniError(String("Failed to get constructor id - <init> ") + sig);
		}
		va_end(args);
		return ret;
	}

	jobject JniClass::newObject() const
	{
		return newObject("()V");
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
				logJniError(String("Failed to get method id - ") + name + " " + sig);
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
			logJniError(String("Failed to get static method id - ") + name + " " + sig);
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
				logJniError(String("Failed to get method id - ") + name + " " + sig);
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
			logJniError(String("Failed to get static method id - ") + name + " " + sig);
		}
		va_end(args);
		return ret;
	}

#define DEFINE_JNI_FIELD(TYPE, NAME) \
	TYPE JniClass::get##NAME##Field(jfieldID field, jobject _this) const \
	{ \
		if (field && _this) { \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				return env->Get##NAME##Field(_this, field); \
			} \
		} \
		return 0; \
	} \
	TYPE JniClass::get##NAME##Field(const char* name, const char* sig, jobject _this) const \
	{ \
		if (_this) { \
			jfieldID field = getFieldID(name, sig); \
			if (field) { \
				JNIEnv* env = Jni::getCurrent(); \
				if (env) { \
					return env->Get##NAME##Field(_this, field); \
				} \
			} else { \
				logJniError(String("Failed to get field id - ") + name + " " + sig); \
			} \
		} \
		return 0; \
	} \
	TYPE JniClass::getStatic##NAME##Field(jfieldID field) const \
	{ \
		if (field) { \
			jclass cls = get(); \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				return env->GetStatic##NAME##Field(cls, field); \
			} \
		} \
		return 0; \
	} \
	TYPE JniClass::getStatic##NAME##Field(const char* name, const char* sig) const \
	{ \
		jclass cls = get(); \
		jfieldID field = getStaticFieldID(name, sig); \
		if (field) { \
			JNIEnv* env = Jni::getCurrent(); \
			if (env) { \
				return env->GetStatic##NAME##Field(cls, field); \
			} \
		} else { \
			logJniError(String("Failed to get static field id - ") + name + " " + sig); \
		} \
		return 0; \
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
				logJniError(String("Failed to get field id - ") + name + " " + sig); \
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
			logJniError(String("Failed to get static field id - ") + name + " " + sig); \
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
		JniLocal<jstring> str((jstring)(getObjectField(field, _this)));
		if (str.isNotNull()) {
			return Jni::getString(str);
		}
		return sl_null;
	}

	String JniClass::getStringField(const char* name, const char* sig, jobject _this) const
	{
		JniLocal<jstring> str((jstring)(getObjectField(name, sig, _this)));
		if (str.isNotNull()) {
			return Jni::getString(str);
		}
		return sl_null;
	}

	String JniClass::getStaticStringField(jfieldID field) const
	{
		JniLocal<jstring> str((jstring)(getStaticObjectField(field)));
		if (str.isNotNull()) {
			return Jni::getString(str);
		}
		return sl_null;
	}

	String JniClass::getStaticStringField(const char* name, const char* sig) const
	{
		JniLocal<jstring> str((jstring)(getStaticObjectField(name, sig)));
		if (str.isNotNull()) {
			return Jni::getString(str);
		}
		return sl_null;
	}

	void JniClass::setStringField(jfieldID field, jobject _this, const StringParam& value) const
	{
		JniLocal<jstring> str(Jni::getJniString(value));
		setObjectField(field, _this, str);
	}

	void JniClass::setStringField(const char* name, const char* sig, jobject _this, const StringParam& value) const
	{
		JniLocal<jstring> str(Jni::getJniString(value));
		setObjectField(name, sig, _this, str);
	}

	void JniClass::setStaticStringField(jfieldID field, const StringParam& value) const
	{
		JniLocal<jstring> str(Jni::getJniString(value));
		setStaticObjectField(field, str);
	}

	void JniClass::setStaticStringField(const char* name, const char* sig, const StringParam& value) const
	{
		JniLocal<jstring> str(Jni::getJniString(value));
		setStaticObjectField(name, sig, str);
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

	Atomic<JniClass>::Atomic(jclass cls) : ref(CJniGlobal<jclass>::from(cls))
	{
	}

	AtomicJniClass& Atomic<JniClass>::operator=(jclass cls)
	{
		ref = CJniGlobal<jclass>::from(cls);
		return *this;
	}

}

#endif
