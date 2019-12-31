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

namespace slib
{

	namespace priv
	{
		namespace java
		{

			class SLIB_EXPORT JClass
			{
			public:
				JClass(const char* name);

			public:
				const char* name;
				JniClass cls;
			};

			class SLIB_EXPORT JMethod
			{
			public:
				JMethod(JClass* gcls, const char* name, const char* sig);

			public:
				jobject callObject(jobject _this, ...);
				jboolean callBoolean(jobject _this, ...);
				jbyte callByte(jobject _this, ...);
				jchar callChar(jobject _this, ...);
				jshort callShort(jobject _this, ...);
				jint callInt(jobject _this, ...);
				jlong callLong(jobject _this, ...);
				jfloat callFloat(jobject _this, ...);
				jdouble callDouble(jobject _this, ...);
				void call(jobject _this, ...);
				String callString(jobject _this, ...);
				jobject newObject(jobject _null, ...);

			public:
				JClass*gcls;
				const char* name;
				const char* sig;
				jclass cls;
				jmethodID id;
			};

			class SLIB_EXPORT JStaticMethod
			{
			public:
				JStaticMethod(JClass* gcls, const char* name, const char* sig);

			public:
				jobject callObject(jobject _null, ...);
				jboolean callBoolean(jobject _null, ...);
				jbyte callByte(jobject _null, ...);
				jchar callChar(jobject _null, ...);
				jshort callShort(jobject _null, ...);
				jint callInt(jobject _null, ...);
				jlong callLong(jobject _null, ...);
				jfloat callFloat(jobject _null, ...);
				jdouble callDouble(jobject _null, ...);
				void call(jobject _null, ...);
				String callString(jobject _null, ...);

			public:
				JClass* gcls;
				const char* name;
				const char* sig;
				jclass cls;
				jmethodID id;
			};

			class SLIB_EXPORT JField
			{
			public:
				JField(JClass* gcls, const char* name, const char* sig);

			public:
				jobject getObject(jobject _this);
				void setObject(jobject _this, jobject value);
				jboolean getBoolean(jobject _this);
				void setBoolean(jobject _this, jboolean value);
				jbyte getByte(jobject _this);
				void setByte(jobject _this, jbyte value);
				jchar getChar(jobject _this);
				void setChar(jobject _this, jchar value);
				jshort getShort(jobject _this);
				void setShort(jobject _this, jshort value);
				jint getInt(jobject _this);
				void setInt(jobject _this, jint value);
				jlong getLong(jobject _this);
				void setLong(jobject _this, jlong value);
				jfloat getFloat(jobject _this);
				void setFloat(jobject _this, jfloat value);
				jdouble getDouble(jobject _this);
				void setDouble(jobject _this, jdouble value);
				String getString(jobject _this);
				void setString(jobject _this, const StringParam& value);
			
			public:
				JClass*gcls;
				const char* name;
				const char* sig;
				jclass cls;
				jfieldID id;
			};
			
			
			class SLIB_EXPORT JObjectField : protected JField
			{
			public:
				JObjectField(JClass* gcls, const char* name, const char* sig);
			public:
				jobject get(jobject _this);
				void set(jobject _this, jobject value);
			};

			class SLIB_EXPORT JStaticField
			{
			public:
				JStaticField(JClass* gcls, const char* name, const char* sig);

			public:
				jobject getObject(jobject _null);
				void setObject(jobject _null, jobject value);
				jboolean getBoolean(jobject _null);
				void setBoolean(jobject _null, jboolean value);
				jbyte getByte(jobject _null);
				void setByte(jobject _null, jbyte value);
				jchar getChar(jobject _null);
				void setChar(jobject _null, jchar value);
				jshort getShort(jobject _null);
				void setShort(jobject _null, jshort value);
				jint getInt(jobject _null);
				void setInt(jobject _null, jint value);
				jlong getLong(jobject _null);
				void setLong(jobject _null, jlong value);
				jfloat getFloat(jobject _null);
				void setFloat(jobject _null, jfloat value);
				jdouble getDouble(jobject _null);
				void setDouble(jobject _null, jdouble value);
				String getString(jobject _null);
				void setString(jobject _null, const StringParam& value);

			public:
				JClass* gcls;
				const char* name;
				const char* sig;
				jclass cls;
				jfieldID id;
			};

			class SLIB_EXPORT JStaticObjectField : protected JStaticField
			{
			public:
				JStaticObjectField(JClass* gcls, const char* name, const char* sig);
			public:
				jobject get();
				void set(jobject value);
			};

		}
	}


#define PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(TYPE, NAME) \
	namespace priv { \
		namespace java { \
			class J##NAME##Field : protected JField \
			{ \
			public: \
				J##NAME##Field(JClass* gcls, const char* name); \
				TYPE get(jobject _this); \
				void set(jobject _this, TYPE value); \
			}; \
		} \
	}

	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(jboolean, Boolean)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(sl_int8, Byte)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(sl_uint16, Char)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(sl_int16, Short)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(sl_int32, Int)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(sl_int64, Long)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(float, Float)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(double, Double)
	PRIV_SLIB_JNI_DECLARE_FIELD_TYPE(String, String)

#define PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(TYPE, NAME) \
	namespace priv { \
		namespace java { \
			class JStatic##NAME##Field : protected JStaticField \
			{ \
			public: \
				JStatic##NAME##Field(JClass* gcls, const char* name); \
				TYPE get(); \
				void set(TYPE value); \
			}; \
		} \
	}

	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(jboolean, Boolean)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(sl_int8, Byte)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(sl_uint16, Char)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(sl_int16, Short)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(sl_int32, Int)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(sl_int64, Long)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(float, Float)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(double, Double)
	PRIV_SLIB_JNI_DECLARE_STATIC_FIELD_TYPE(String, String)

	namespace priv
	{
		namespace java
		{		
			class SLIB_EXPORT JNativeMethod
			{
			public:
				JNativeMethod(priv::java::JClass* gcls, const char* name, const char* sig, const void* fn);

			public:
				priv::java::JClass*gcls;
				const char* name;
				const char* sig;
				const void* fn;
			};
		}
	}

	template <class T>
	SLIB_INLINE JniLocal<T>::JniLocal() : value(sl_null)
	{
	}

	template <class T>
	SLIB_INLINE JniLocal<T>::JniLocal(T _value) : value(_value)
	{
	}

	template <class T>
	SLIB_INLINE JniLocal<T>::~JniLocal()
	{
		free();
	}

	template <class T>
	SLIB_INLINE JniLocal<T>::operator T&()
	{
		return value;
	}

	template <class T>
	SLIB_INLINE JniLocal<T>::operator T() const
	{
		return value;
	}

	template <class T>
	SLIB_INLINE T JniLocal<T>::operator=(T value)
	{
		this->value = value;
		return value;
	}

	template <class T>
	SLIB_INLINE T JniLocal<T>::get() const
	{
		return value;
	}

	template <class T>
	SLIB_INLINE sl_bool JniLocal<T>::isNotNull() const
	{
		return value != sl_null;
	}

	template <class T>
	SLIB_INLINE sl_bool JniLocal<T>::isNull() const
	{
		return value == sl_null;
	}

	template <class T>
	SLIB_INLINE void JniLocal<T>::setNull()
	{
		this->value = sl_null;
	}

	template <class T>
	SLIB_INLINE void JniLocal<T>::free()
	{
		if (value) {
			Jni::deleteLocalRef(value);
			value = sl_null;
		}
	}


	template <class T>
	CJniGlobal<T>::~CJniGlobal()
	{
		Jni::deleteGlobalRef(object);
	}

	template <class T>
	Ref< CJniGlobal<T> > CJniGlobal<T>::from(T obj)
	{
		Ref< CJniGlobal<T> > ret;
		if (obj) {
			jobject jglobal = Jni::newGlobalRef(obj);
			if (jglobal) {
				ret = new CJniGlobal<T>();
				if (ret.isNotNull()) {
					ret->object = (T)jglobal;
					return ret;
				}
				Jni::deleteGlobalRef(jglobal);
			}
		}
		return sl_null;
	}


	template <class T>
	SLIB_INLINE JniGlobal<T>::JniGlobal(T obj) : ref(CJniGlobal<T>::from(obj))
	{
	}

	template <class T>
	SLIB_INLINE JniGlobal<T>::JniGlobal(const JniLocal<T>& obj) : ref(CJniGlobal<T>::from(obj.value))
	{
	}

	template <class T>
	SLIB_INLINE JniGlobal<T> JniGlobal<T>::from(T obj)
	{
		return JniGlobal<T>(obj);
	}

	template <class T>
	SLIB_INLINE JniGlobal<T>& JniGlobal<T>::operator=(T obj)
	{
		ref = CJniGlobal<T>::from(obj);
		return *this;
	}

	template <class T>
	SLIB_INLINE JniGlobal<T>& JniGlobal<T>::operator=(const JniLocal<T>& obj)
	{
		ref = CJniGlobal<T>::from(obj.value);
		return *this;
	}

	template <class T>
	SLIB_INLINE T JniGlobal<T>::get() const
	{
		CJniGlobal<T>* o = ref.get();
		if (o) {
			return o->object;
		} else {
			return 0;
		}
	}

	template <class T>
	SLIB_INLINE JniGlobal<T>::operator T() const
	{
		CJniGlobal<T>* o = ref.get();
		if (o) {
			return o->object;
		} else {
			return 0;
		}
	}


	template <class T>
	SLIB_INLINE Atomic< JniGlobal<T> >::Atomic(T obj) : ref(CJniGlobal<T>::from(obj))
	{
	}

	template <class T>
	SLIB_INLINE Atomic< JniGlobal<T> >::Atomic(JniLocal<T>& obj) : ref(CJniGlobal<T>::from(obj.value))
	{
	}

	template <class T>
	AtomicJniGlobal<T>& Atomic< JniGlobal<T> >::operator=(T obj)
	{
		ref = CJniGlobal<T>::from(obj);
		return *this;
	}

	template <class T>
	AtomicJniGlobal<T>& Atomic< JniGlobal<T> >::operator=(JniLocal<T>& obj)
	{
		ref = CJniGlobal<T>::from(obj.value);
		return *this;
	}

	template <class T>
	T Atomic< JniGlobal<T> >::get() const
	{
		Ref< CJniGlobal<T> > o(ref);
		if (o.isNotNull()) {
			return o->object;
		} else {
			return 0;
		}
	}

}
