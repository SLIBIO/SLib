/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

	class SLIB_EXPORT _JniSingletonClass
	{
	public:
		_JniSingletonClass(const char* name);

	public:
		const char* name;
		JniClass cls;
	};

	class SLIB_EXPORT _JniSingletonMethod
	{
	public:
		_JniSingletonMethod(_JniSingletonClass* gcls, const char* name, const char* sig);

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
		String16 callString16(jobject _this, ...);
		jobject newObject(jobject _null, ...);

	public:
		_JniSingletonClass*gcls;
		const char* name;
		const char* sig;
		jclass cls;
		jmethodID id;
	};

	class SLIB_EXPORT _JniSingletonStaticMethod
	{
	public:
		_JniSingletonStaticMethod(_JniSingletonClass* gcls, const char* name, const char* sig);

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
		String16 callString16(jobject _null, ...);

	public:
		_JniSingletonClass* gcls;
		const char* name;
		const char* sig;
		jclass cls;
		jmethodID id;
	};

	class SLIB_EXPORT _JniSingletonField
	{
	public:
		_JniSingletonField(_JniSingletonClass* gcls, const char* name, const char* sig);

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
		String16 getString16(jobject _this);
		void setString(jobject _this, const String& value);
		void setString16(jobject _this, const String16& value);
	
	public:
		_JniSingletonClass*gcls;
		const char* name;
		const char* sig;
		jclass cls;
		jfieldID id;
	};
	
	
	class SLIB_EXPORT _JniSingletonObjectField : protected _JniSingletonField
	{
	public:
		_JniSingletonObjectField(_JniSingletonClass* gcls, const char* name, const char* sig);
	public:
		jobject get(jobject _this);
		void set(jobject _this, jobject value);
	};

#define _SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(TYPE, NAME) \
class _JniSingleton##NAME##Field : protected _JniSingletonField \
{ \
public: \
	_JniSingleton##NAME##Field(_JniSingletonClass* gcls, const char* name); \
	TYPE get(jobject _this); \
	void set(jobject _this, TYPE value); \
};

	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(jboolean, Boolean)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(sl_int8, Byte)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(sl_uint16, Char)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(sl_int16, Short)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(sl_int32, Int)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(sl_int64, Long)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(float, Float)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(double, Double)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(String, String)
	_SLIB_JNI_DECLARE_SINGLETON_FIELD_TYPE(String16, String16)

	class SLIB_EXPORT _JniSingletonStaticField
	{
	public:
		_JniSingletonStaticField(_JniSingletonClass* gcls, const char* name, const char* sig);

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
		String16 getString16(jobject _null);
		void setString(jobject _null, const String& value);
		void setString16(jobject _null, const String16& value);

	public:
		_JniSingletonClass* gcls;
		const char* name;
		const char* sig;
		jclass cls;
		jfieldID id;
	};

	class SLIB_EXPORT _JniSingletonStaticObjectField : protected _JniSingletonStaticField
	{
	public:
		_JniSingletonStaticObjectField(_JniSingletonClass* gcls, const char* name, const char* sig);
	public:
		jobject get();
		void set(jobject value);
	};


#define _SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(TYPE, NAME) \
class _JniSingletonStatic##NAME##Field : protected _JniSingletonStaticField \
{ \
public: \
	_JniSingletonStatic##NAME##Field(_JniSingletonClass* gcls, const char* name); \
	TYPE get(); \
	void set(TYPE value); \
};

	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(jboolean, Boolean)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(sl_int8, Byte)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(sl_uint16, Char)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(sl_int16, Short)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(sl_int32, Int)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(sl_int64, Long)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(float, Float)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(double, Double)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(String, String)
	_SLIB_JNI_DECLARE_SINGLETON_STATIC_FIELD_TYPE(String16, String16)


	class SLIB_EXPORT _JniNativeMethod
	{
	public:
		_JniNativeMethod(_JniSingletonClass* gcls, const char* name, const char* sig, const void* fn);

	public:
		_JniSingletonClass*gcls;
		const char* name;
		const char* sig;
		const void* fn;
	};


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
