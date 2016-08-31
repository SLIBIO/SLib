#ifndef CHECKHEADER_SLIB_CORE_OBJECT
#define CHECKHEADER_SLIB_CORE_OBJECT

#include "definition.h"

#include "reference.h"
#include "mutex.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Object : public Referable
{
	SLIB_DECLARE_OBJECT
	
private:
	Mutex m_locker;

public:
	Mutex* getLocker() const;
	
	void lock() const;

	void unlock() const;

	sl_bool tryLock() const;

};

class SLIB_EXPORT ObjectLocker : public MutexLocker
{
public:
	ObjectLocker();
	
	ObjectLocker(const Object* object);

	ObjectLocker(const Object* object1, const Object* object2);
	
public:
	void lock(const Object* object);
	
	void lock(const Object* object1, const Object* object2);
	
};

SLIB_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_WIN32)

SLIB_NAMESPACE_BEGIN

template <class T>
class SafeStaticDestructor
{
public:
	T* object;
	sl_bool flagFreed;
	
public:
	SafeStaticDestructor(T* p);
	
	~SafeStaticDestructor();
	
};

template <class T>
SLIB_INLINE SafeStaticDestructor<T>::SafeStaticDestructor(T* p) : flagFreed(sl_false)
{
	object = p;
}

template <class T>
SLIB_INLINE SafeStaticDestructor<T>::~SafeStaticDestructor()
{
	flagFreed = sl_true;
	object->~T();
}

SLIB_NAMESPACE_END

#define SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME) static slib::SafeStaticDestructor<TYPE> _static_destructor_instance_##NAME(&NAME);
#define SLIB_SAFE_STATIC_CHECK_FREED(NAME) _static_destructor_instance_##NAME.flagFreed

#else

#define SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME)
#define SLIB_SAFE_STATIC_CHECK_FREED(NAME) 0

#endif

#define SLIB_SAFE_STATIC(TYPE, NAME, ...) \
	static sl_uint8 _static_safemem_##NAME[sizeof(TYPE)]; \
	static sl_int32 _static_safeflag_##NAME = 0; \
	TYPE& NAME = *((TYPE*)((void*)(_static_safemem_##NAME))); \
	SLIB_STATIC_SPINLOCK(_static_safelock_##NAME); \
	if (_static_safeflag_##NAME == 0) { \
		_static_safelock_##NAME.lock(); \
		if (_static_safeflag_##NAME == 0) { \
			new (&NAME) TYPE(__VA_ARGS__); \
			_static_safeflag_##NAME = 1; \
		} \
		_static_safelock_##NAME.unlock(); \
	} \
	SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME)

#define SLIB_SAFE_STATIC_GETTER(TYPE, FUNC, ...) \
	static TYPE* FUNC() { \
		SLIB_SAFE_STATIC(TYPE, ret, ##__VA_ARGS__) \
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) { \
			return 0; \
		} \
		return &ret; \
	}

#define SLIB_STATIC_ZERO_INITIALIZED(TYPE, NAME) \
	static char _static_safemem_##NAME[sizeof(TYPE)] = {0}; \
	TYPE& NAME = *((TYPE*)((void*)(_static_safemem_##NAME))); \
	SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME)


#define SLIB_PROPERTY(TYPE, NAME) protected: \
	TYPE _m_property_##NAME; \
public: \
	SLIB_INLINE TYPE const& get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(TYPE const& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_BOOLEAN_PROPERTY(NAME) protected: \
	sl_bool _m_property_##NAME; \
public: \
	SLIB_INLINE sl_bool is##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(sl_bool v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_REF_PROPERTY(TYPE, NAME) protected: \
	slib::SafeRef<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_WEAK_PROPERTY(TYPE, NAME) protected: \
	slib::SafeWeakRef<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_PTR_PROPERTY(TYPE, NAME) protected: \
	slib::SafePtr<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ptr<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ptr<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_STRING_PROPERTY(NAME) protected: \
	slib::SafeString _m_property_##NAME; \
public: \
	SLIB_INLINE slib::String get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::String& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_STRING16_PROPERTY(NAME) protected: \
	slib::SafeString16 _m_property_##NAME; \
public: \
	SLIB_INLINE slib::String16 get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::String16& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_VARIANT_PROPERTY(NAME) protected: \
	slib::SafeVariant _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Variant get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Variant& v) { \
		_m_property_##NAME = v; \
	}


#endif

