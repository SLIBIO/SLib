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

#ifndef CHECKHEADER_SLIB_CORE_JAVA
#define CHECKHEADER_SLIB_CORE_JAVA

/**********************************************************

 This JNI library is implemented for shared JVM (one JVM)

**********************************************************/

#include "definition.h"

#if defined(SLIB_PLATFORM_USE_JNI)

#include "object.h"
#include "string.h"

#include <jni.h>

namespace slib
{

	template <class T>
	class SLIB_EXPORT JniLocal
	{
	public:
		T value;

	public:
		JniLocal();

		JniLocal(T value);

		~JniLocal();

	public:
		operator T&();

		operator T() const;

		T operator=(T value);

		T get() const;

		sl_bool isNotNull() const;

		sl_bool isNull() const;

		void setNull();

		void free();

	};

	class SLIB_EXPORT CJniGlobalBase : public Referable
	{
		SLIB_DECLARE_OBJECT
	};

	template <class T>
	class SLIB_EXPORT CJniGlobal : public CJniGlobalBase
	{
	protected:
		CJniGlobal() = default;

		~CJniGlobal();

	public:
		static Ref< CJniGlobal<T> > from(T obj);

	public:
		T object;

	};

	template <class T>
	class SLIB_EXPORT JniGlobal
	{
	public:
		Ref< CJniGlobal<T> > ref;
		SLIB_REF_WRAPPER(JniGlobal, CJniGlobal<T>)

	public:
		JniGlobal(T obj);

		JniGlobal(const JniLocal<T>& obj);

	public:
		static JniGlobal<T> from(T obj);

	public:
		JniGlobal<T>& operator=(T obj);

		JniGlobal<T>& operator=(const JniLocal<T>& obj);

	public:
		T get() const;

		operator T() const;

	};


	template <class T>
	class SLIB_EXPORT Atomic< JniGlobal<T> >
	{
	public:
		AtomicRef< CJniGlobal<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CJniGlobal<T>)

	public:
		Atomic(T obj);

		Atomic(JniLocal<T>& obj);

	public:
		Atomic& operator=(T obj);

		Atomic& operator=(JniLocal<T>& obj);

	public:
		T get() const;

	};

	template <class T>
	using AtomicJniGlobal = Atomic< JniGlobal<T> >;


	class JniClass;

	template <>
	class SLIB_EXPORT Atomic<JniClass>
	{
	public:
		AtomicRef< CJniGlobal<jclass> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CJniGlobal<jclass>)

	public:
		Atomic(jclass cls);
	
	public:
		Atomic& operator=(jclass cls);
	
	};
	
	typedef Atomic<JniClass> AtomicJniClass;
	
	class SLIB_EXPORT JniClass
	{
	public:
		Ref< CJniGlobal<jclass> > ref;
		SLIB_REF_WRAPPER(JniClass, CJniGlobal<jclass>)
		
	public:
		JniClass(jclass cls);

	public:
		JniClass& operator=(jclass cls);

	public:
		static JniClass from(jclass cls);

		static JniClass getClassOfObject(jobject object);

	public:
		jclass get() const;

		operator jclass() const;

	public:
		sl_bool isInstanceOf(jobject obj) const;

		/*
		 * Signature
		 * Z - boolean
		 * B - byte
		 * C - char
		 * S - short
		 * I - int
		 * J - long long
		 * F - float
		 * D - double
		 * V - void
		 * L<class-name>; - object
		 * [<type> - type[]
		 * (arg-types)ret-type : method type
		 */
		jmethodID getMethodID(const char* name, const char* sig) const;
		jmethodID getStaticMethodID(const char* name, const char* sig) const;
		jfieldID getFieldID(const char* name, const char* sig) const;
		jfieldID getStaticFieldID(const char* name, const char* sig) const;

		jobject newObject(jmethodID method, ...) const;
		jobject newObject(const char* sigConstructor, ...) const;
		jobject newObject() const;

		jobject callObjectMethod(jmethodID method, jobject _this, ...) const;
		jobject callObjectMethod(const char* name, const char* sig, jobject _this, ...) const;
		jobject callStaticObjectMethod(jmethodID method, ...) const;
		jobject callStaticObjectMethod(const char* name, const char* sig, ...) const;
		jboolean callBooleanMethod(jmethodID method, jobject _this, ...) const;
		jboolean callBooleanMethod(const char* name, const char* sig, jobject _this, ...) const;
		jboolean callStaticBooleanMethod(jmethodID method, ...) const;
		jboolean callStaticBooleanMethod(const char* name, const char* sig, ...) const;
		jbyte callByteMethod(jmethodID method, jobject _this, ...) const;
		jbyte callByteMethod(const char* name, const char* sig, jobject _this, ...) const;
		jbyte callStaticByteMethod(jmethodID method, ...) const;
		jbyte callStaticByteMethod(const char* name, const char* sig, ...) const;
		jchar callCharMethod(jmethodID method, jobject _this, ...) const;
		jchar callCharMethod(const char* name, const char* sig, jobject _this, ...) const;
		jchar callStaticCharMethod(jmethodID method, ...) const;
		jchar callStaticCharMethod(const char* name, const char* sig, ...) const;
		jshort callShortMethod(jmethodID method, jobject _this, ...) const;
		jshort callShortMethod(const char* name, const char* sig, jobject _this, ...) const;
		jshort callStaticShortMethod(jmethodID method, ...) const;
		jshort callStaticShortMethod(const char* name, const char* sig, ...) const;
		jint callIntMethod(jmethodID method, jobject _this, ...) const;
		jint callIntMethod(const char* name, const char* sig, jobject _this, ...) const;
		jint callStaticIntMethod(jmethodID method, ...) const;
		jint callStaticIntMethod(const char* name, const char* sig, ...) const;
		jlong callLongMethod(jmethodID method, jobject _this, ...) const;
		jlong callLongMethod(const char* name, const char* sig, jobject _this, ...) const;
		jlong callStaticLongMethod(jmethodID method, ...) const;
		jlong callStaticLongMethod(const char* name, const char* sig, ...) const;
		jfloat callFloatMethod(jmethodID method, jobject _this, ...) const;
		jfloat callFloatMethod(const char* name, const char* sig, jobject _this, ...) const;
		jfloat callStaticFloatMethod(jmethodID method, ...) const;
		jfloat callStaticFloatMethod(const char* name, const char* sig, ...) const;
		jdouble callDoubleMethod(jmethodID method, jobject _this, ...) const;
		jdouble callDoubleMethod(const char* name, const char* sig, jobject _this, ...) const;
		jdouble callStaticDoubleMethod(jmethodID method, ...) const;
		jdouble callStaticDoubleMethod(const char* name, const char* sig, ...) const;
		void callVoidMethod(jmethodID method, jobject _this, ...) const;
		void callVoidMethod(const char* name, const char* sig, jobject _this, ...) const;
		void callStaticVoidMethod(jmethodID method, ...) const;
		void callStaticVoidMethod(const char* name, const char* sig, ...) const;
		String16 callStringMethod(jmethodID method, jobject _this, ...) const;
		String16 callStringMethod(const char* name, const char* sig, jobject _this, ...) const;
		String16 callStaticStringMethod(jmethodID method, ...) const;
		String16 callStaticStringMethod(const char* name, const char* sig, ...) const;

		jobject getObjectField(jfieldID field, jobject _this) const;
		jobject getObjectField(const char* name, const char* sig, jobject _this) const;
		void setObjectField(jfieldID field, jobject _this, jobject value) const;
		void setObjectField(const char* name, const char* sig, jobject _this, jobject value) const;
		jobject getStaticObjectField(jfieldID field) const;
		jobject getStaticObjectField(const char* name, const char* sig) const;
		void setStaticObjectField(jfieldID field, jobject value) const;
		void setStaticObjectField(const char* name, const char* sig, jobject value) const;
		jboolean getBooleanField(jfieldID field, jobject _this) const;
		jboolean getBooleanField(const char* name, const char* sig, jobject _this) const;
		void setBooleanField(jfieldID field, jobject _this, jboolean value) const;
		void setBooleanField(const char* name, const char* sig, jobject _this, jboolean value) const;
		jboolean getStaticBooleanField(jfieldID field) const;
		jboolean getStaticBooleanField(const char* name, const char* sig) const;
		void setStaticBooleanField(jfieldID field, jboolean value) const;
		void setStaticBooleanField(const char* name, const char* sig, jboolean value) const;
		jbyte getByteField(jfieldID field, jobject _this) const;
		jbyte getByteField(const char* name, const char* sig, jobject _this) const;
		void setByteField(jfieldID field, jobject _this, jbyte value) const;
		void setByteField(const char* name, const char* sig, jobject _this, jbyte value) const;
		jbyte getStaticByteField(jfieldID field) const;
		jbyte getStaticByteField(const char* name, const char* sig) const;
		void setStaticByteField(jfieldID field, jbyte value) const;
		void setStaticByteField(const char* name, const char* sig, jbyte value) const;
		jchar getCharField(jfieldID field, jobject _this) const;
		jchar getCharField(const char* name, const char* sig, jobject _this) const;
		void setCharField(jfieldID field, jobject _this, jchar value) const;
		void setCharField(const char* name, const char* sig, jobject _this, jchar value) const;
		jchar getStaticCharField(jfieldID field) const;
		jchar getStaticCharField(const char* name, const char* sig) const;
		void setStaticCharField(jfieldID field, jchar value) const;
		void setStaticCharField(const char* name, const char* sig, jchar value) const;
		jshort getShortField(jfieldID field, jobject _this) const;
		jshort getShortField(const char* name, const char* sig, jobject _this) const;
		void setShortField(jfieldID field, jobject _this, jshort value) const;
		void setShortField(const char* name, const char* sig, jobject _this, jshort value) const;
		jshort getStaticShortField(jfieldID field) const;
		jshort getStaticShortField(const char* name, const char* sig) const;
		void setStaticShortField(jfieldID field, jshort value) const;
		void setStaticShortField(const char* name, const char* sig, jshort value) const;
		jint getIntField(jfieldID field, jobject _this) const;
		jint getIntField(const char* name, const char* sig, jobject _this) const;
		void setIntField(jfieldID field, jobject _this, jint value) const;
		void setIntField(const char* name, const char* sig, jobject _this, jint value) const;
		jint getStaticIntField(jfieldID field) const;
		jint getStaticIntField(const char* name, const char* sig) const;
		void setStaticIntField(jfieldID field, jint value) const;
		void setStaticIntField(const char* name, const char* sig, jint value) const;
		jlong getLongField(jfieldID field, jobject _this) const;
		jlong getLongField(const char* name, const char* sig, jobject _this) const;
		void setLongField(jfieldID field, jobject _this, jlong value) const;
		void setLongField(const char* name, const char* sig, jobject _this, jlong value) const;
		jlong getStaticLongField(jfieldID field) const;
		jlong getStaticLongField(const char* name, const char* sig) const;
		void setStaticLongField(jfieldID field, jlong value) const;
		void setStaticLongField(const char* name, const char* sig, jlong value) const;
		jfloat getFloatField(jfieldID field, jobject _this) const;
		jfloat getFloatField(const char* name, const char* sig, jobject _this) const;
		void setFloatField(jfieldID field, jobject _this, jfloat value) const;
		void setFloatField(const char* name, const char* sig, jobject _this, jfloat value) const;
		jfloat getStaticFloatField(jfieldID field) const;
		jfloat getStaticFloatField(const char* name, const char* sig) const;
		void setStaticFloatField(jfieldID field, jfloat value) const;
		void setStaticFloatField(const char* name, const char* sig, jfloat value) const;
		jdouble getDoubleField(jfieldID field, jobject _this) const;
		jdouble getDoubleField(const char* name, const char* sig, jobject _this) const;
		void setDoubleField(jfieldID field, jobject _this, jdouble value) const;
		void setDoubleField(const char* name, const char* sig, jobject _this, jdouble value) const;
		jdouble getStaticDoubleField(jfieldID field) const;
		jdouble getStaticDoubleField(const char* name, const char* sig) const;
		void setStaticDoubleField(jfieldID field, jdouble value) const;
		void setStaticDoubleField(const char* name, const char* sig, jdouble value) const;

		String16 getStringField(jfieldID field, jobject _this) const;
		String16 getStringField(const char* name, const char* sig, jobject _this) const;
		String16 getStaticStringField(jfieldID field) const;
		String16 getStaticStringField(const char* name, const char* sig) const;

		void setStringField(jfieldID field, jobject _this, const String16& value) const;
		void setStringField(const char* name, const char* sig, jobject _this, const String16& value) const;
		void setStaticStringField(jfieldID field, const String16& value) const;
		void setStaticStringField(const char* name, const char* sig, const String16& value) const;

		sl_bool registerNative(const char* name, const char* sig, const void* fn) const;

	};


	class SLIB_EXPORT Jni
	{
	public:
		static void initialize(JavaVM* jvm);

		static void setSharedJVM(JavaVM* jvm);
		static JavaVM* getSharedJVM();

		// thread-storage
		static JNIEnv* getCurrent();
		static void setCurrent(JNIEnv* jni);

		// attach and set current
		static JNIEnv* attachThread(JavaVM* jvm = sl_null);
		static void detachThread(JavaVM* jvm = sl_null);

		// class
		static JniClass getClass(const String& className);
		static void registerClass(const String& className, jclass cls);
		static void unregisterClass(const String& className);
		static JniClass findClass(const char* className);

		// object
		static sl_bool isSameObject(jobject ref1, jobject ref2);

		static jobjectRefType getRefType(jobject obj);
		static sl_bool isInvalidRef(jobject obj);

		static sl_bool isLocalRef(jobject obj);
		static jobject newLocalRef(jobject obj);
		static void deleteLocalRef(jobject obj);

		static sl_bool isGlobalRef(jobject obj);
		static jobject newGlobalRef(jobject obj);
		static void deleteGlobalRef(jobject obj);

		static sl_bool isWeakRef(jobject obj);
		static jobject newWeakRef(jobject obj);
		static void deleteWeakRef(jobject obj);

		// string
		static jstring getJniString(const String16& str);
		static String16 getString(jstring str);

		/*
		 * Array release<TYPE>ArrayElements Mode
		 * 0 - commit and free
		 * JNI_COMMIT - commit only
		 * JNI_ABORT - free only
		 */
		static sl_uint32 getArrayLength(jarray array);
		static jobjectArray newObjectArray(jclass clsElement, sl_uint32 length);
		static jobject getObjectArrayElement(jobjectArray array, sl_uint32 index);
		static void setObjectArrayElement(jobjectArray array, sl_uint32 index, jobject value);
		static jobjectArray newStringArray(sl_uint32 length);
		static String16 getStringArrayElement(jobjectArray array, sl_uint32 index);
		static void setStringArrayElement(jobjectArray array, sl_uint32 index, const String16& value);
		static jbooleanArray newBooleanArray(sl_uint32 length);
		static jboolean* getBooleanArrayElements(jbooleanArray array, jboolean* isCopy = sl_null);
		static void releaseBooleanArrayElements(jbooleanArray array, jboolean* buf, jint mode = 0);
		static void getBooleanArrayRegion(jbooleanArray array, sl_uint32 index, sl_uint32 len, jboolean* buf);
		static void setBooleanArrayRegion(jbooleanArray array, sl_uint32 index, sl_uint32 len, jboolean* buf);
		static jbyteArray newByteArray(sl_uint32 length);
		static jbyte* getByteArrayElements(jbyteArray array, jboolean* isCopy);
		static void releaseByteArrayElements(jbyteArray array, jbyte* buf, jint mode = 0);
		static void getByteArrayRegion(jbyteArray array, sl_uint32 index, sl_uint32 len, jbyte* buf);
		static void setByteArrayRegion(jbyteArray array, sl_uint32 index, sl_uint32 len, jbyte* buf);
		static jcharArray newCharArray(sl_uint32 length);
		static jchar* getCharArrayElements(jcharArray array, jboolean* isCopy = sl_null);
		static void releaseCharArrayElements(jcharArray array, jchar* buf, jint mode = 0);
		static void getCharArrayRegion(jcharArray array, sl_uint32 index, sl_uint32 len, jchar* buf);
		static void setCharArrayRegion(jcharArray array, sl_uint32 index, sl_uint32 len, jchar* buf);
		static jshortArray newShortArray(sl_uint32 length);
		static jshort* getShortArrayElements(jshortArray array, jboolean* isCopy = sl_null);
		static void releaseShortArrayElements(jshortArray array, jshort* buf, jint mode = 0);
		static void getShortArrayRegion(jshortArray array, sl_uint32 index, sl_uint32 len, jshort* buf);
		static void setShortArrayRegion(jshortArray array, sl_uint32 index, sl_uint32 len, jshort* buf);
		static jintArray newIntArray(sl_uint32 length);
		static jint* getIntArrayElements(jintArray array, jboolean* isCopy = sl_null);
		static void releaseIntArrayElements(jintArray array, jint* buf, jint mode = 0);
		static void getIntArrayRegion(jintArray array, sl_uint32 index, sl_uint32 len, jint* buf);
		static void setIntArrayRegion(jintArray array, sl_uint32 index, sl_uint32 len, jint* buf);
		static jlongArray newLongArray(sl_uint32 length);
		static jlong* getLongArrayElements(jlongArray array, jboolean* isCopy = sl_null);
		static void releaseLongArrayElements(jlongArray array, jlong* buf, jint mode = 0);
		static void getLongArrayRegion(jlongArray array, sl_uint32 index, sl_uint32 len, jlong* buf);
		static void setLongArrayRegion(jlongArray array, sl_uint32 index, sl_uint32 len, jlong* buf);
		static jfloatArray newFloatArray(sl_uint32 length);
		static jfloat* getFloatArrayElements(jfloatArray array, jboolean* isCopy = sl_null);
		static void releaseFloatArrayElements(jfloatArray array, jfloat* buf, jint mode = 0);
		static void getFloatArrayRegion(jfloatArray array, sl_uint32 index, sl_uint32 len, jfloat* buf);
		static void setFloatArrayRegion(jfloatArray array, sl_uint32 index, sl_uint32 len, jfloat* buf);
		static jdoubleArray newDoubleArray(sl_uint32 length);
		static jdouble* getDoubleArrayElements(jdoubleArray array, jboolean* isCopy = sl_null);
		static void releaseDoubleArrayElements(jdoubleArray array, jdouble* buf, jint mode = 0);
		static void getDoubleArrayRegion(jdoubleArray array, sl_uint32 index, sl_uint32 len, jdouble* buf);
		static void setDoubleArrayRegion(jdoubleArray array, sl_uint32 index, sl_uint32 len, jdouble* buf);

		// direct buffer
		static jobject newDirectByteBuffer(void* address, sl_size capacity);
		static void* getDirectBufferAddress(jobject buf);
		static sl_size getDirectBufferCapacity(jobject buf);

		// exception
		static sl_bool checkException();
		static void clearException();
		static void printException();

		// input stream
		static sl_int32 readFromInputStream(jobject stream, jbyteArray array);
		static void closeInputStream(jobject stream);
	
	};
}

#define SLIB_JNI_BEGIN_CLASS(CLASS, NAME) \
namespace CLASS \
{ \
	static slib::priv::java::JClass _gcls(NAME); \
	SLIB_INLINE slib::JniClass get() { \
		return _gcls.cls; \
	}

#define SLIB_JNI_END_CLASS \
}

#define SLIB_JNI_BEGIN_CLASS_SECTION(CLASS) \
namespace CLASS \
{ \

#define SLIB_JNI_END_CLASS_SECTION \
}

#define SLIB_JNI_NEW(VAR, SIG) static slib::priv::java::JMethod VAR(&_gcls, "<init>", SIG);

#define SLIB_JNI_METHOD(VAR, NAME, SIG) static slib::priv::java::JMethod VAR(&_gcls, NAME, SIG);
#define SLIB_JNI_STATIC_METHOD(VAR, NAME, SIG) static slib::priv::java::JStaticMethod VAR(&_gcls, NAME, SIG);

#define SLIB_JNI_FIELD(VAR, NAME, SIG) static slib::priv::java::JField VAR(&_gcls, NAME, SIG);
#define SLIB_JNI_OBJECT_FIELD(VAR, SIG) static slib::priv::java::JObjectField VAR(&_gcls, (#VAR), SIG);
#define SLIB_JNI_BOOLEAN_FIELD(VAR) static slib::priv::java::JBooleanField VAR(&_gcls, (#VAR));
#define SLIB_JNI_BYTE_FIELD(VAR) static slib::priv::java::JByteField VAR(&_gcls, (#VAR));
#define SLIB_JNI_CHAR_FIELD(VAR) static slib::priv::java::JCharField VAR(&_gcls, (#VAR));
#define SLIB_JNI_SHORT_FIELD(VAR) static slib::priv::java::JShortField VAR(&_gcls, (#VAR));
#define SLIB_JNI_INT_FIELD(VAR) static slib::priv::java::JIntField VAR(&_gcls, (#VAR));;
#define SLIB_JNI_LONG_FIELD(VAR) static slib::priv::java::JLongField VAR(&_gcls, (#VAR));
#define SLIB_JNI_FLOAT_FIELD(VAR) static slib::priv::java::JFloatField VAR(&_gcls, (#VAR));
#define SLIB_JNI_DOUBLE_FIELD(VAR) static slib::priv::java::JDoubleField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STRING_FIELD(VAR) static slib::priv::java::JStringField VAR(&_gcls, (#VAR));

#define SLIB_JNI_STATIC_FIELD(VAR, NAME, SIG) static slib::priv::java::JStaticField VAR(&_gcls, NAME, SIG);
#define SLIB_JNI_STATIC_OBJECT_FIELD(VAR, SIG) static slib::priv::java::JStaticObjectField VAR(&_gcls, (#VAR), SIG);
#define SLIB_JNI_STATIC_BOOLEAN_FIELD(VAR) static slib::priv::java::JStaticBooleanField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_BYTE_FIELD(VAR) static slib::priv::java::JStaticByteField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_CHAR_FIELD(VAR) static slib::priv::java::JStaticCharField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_SHORT_FIELD(VAR) static slib::priv::java::JStaticShortField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_INT_FIELD(VAR) static slib::priv::java::JStaticIntField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_LONG_FIELD(VAR) static slib::priv::java::JStaticLongField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_FLOAT_FIELD(VAR) static slib::priv::java::JStaticFloatField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_DOUBLE_FIELD(VAR) static slib::priv::java::JStaticDoubleField VAR(&_gcls, (#VAR));
#define SLIB_JNI_STATIC_STRING_FIELD(VAR) static slib::priv::java::JStaticStringField VAR(&_gcls, (#VAR));

#define SLIB_JNI_NATIVE(VAR, NAME, SIG, fn) static slib::priv::java::JNativeMethod VAR(&_gcls, NAME, SIG, (const void*)(fn));
#define SLIB_JNI_NATIVE_IMPL(VAR, NAME, SIG, RET, ...) \
	static RET JNICALL JNativeMethodImpl_##VAR(JNIEnv* env, jobject _this, ##__VA_ARGS__); \
	static slib::priv::java::JNativeMethod VAR(&_gcls, NAME, SIG, (const void*)(JNativeMethodImpl_##VAR)); \
	RET JNICALL JNativeMethodImpl_##VAR(JNIEnv* env, jobject _this, ##__VA_ARGS__)

#include "detail/java.inc"

#endif

#endif
