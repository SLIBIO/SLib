#ifndef CHECKHEADER_SLIB_CORE_OBJECT
#define CHECKHEADER_SLIB_CORE_OBJECT

#include "definition.h"

#include "reference.h"
#include "mutex.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Object : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(Object)
private:
	Mutex m_locker;

public:
	SLIB_INLINE Object() {}

public:
	SLIB_INLINE Mutex* getLocker() const
	{
		return (Mutex*)(&m_locker);
	}

	SLIB_INLINE void lock() const
	{
		m_locker.lock();
	}

	SLIB_INLINE void unlock() const
	{
		m_locker.unlock();
	}

	SLIB_INLINE sl_bool tryLock() const
	{
		return m_locker.tryLock();
	}

};

class SLIB_EXPORT ObjectLocker : public MutexLocker
{
public:
	SLIB_INLINE ObjectLocker()
	{
	}
	
	SLIB_INLINE ObjectLocker(const Object* object) : MutexLocker(object ? object->getLocker(): sl_null)
	{
	}

	SLIB_INLINE ObjectLocker(const Object* object1, const Object* object2) : MutexLocker(object1 ? object1->getLocker() : sl_null, object2 ? object2->getLocker() : sl_null)
	{
	}
	
public:
	SLIB_INLINE void lock(const Object* object)
	{
		if (object) {
			MutexLocker::lock(object->getLocker());
		}
	}
	
	void lock(const Object* object1, const Object* object2)
	{
		if (object1) {
			if (object2) {
				MutexLocker::lock(object1->getLocker(), object2->getLocker());
			} else {
				MutexLocker::lock(object1->getLocker());
			}
		} else {
			if (object2) {
				MutexLocker::lock(object2->getLocker());
			}
		}
	}

};

template <class TYPE>
class SafeStaticDestructor
{
public:
	TYPE*o;
	SafeStaticDestructor(TYPE* p)
	{
		o = p;
	}
	~SafeStaticDestructor()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		o->~TYPE();
#endif
	}
};

SLIB_NAMESPACE_END

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
	}\
	static slib::SafeStaticDestructor<TYPE> _static_destructor_instance_##NAME(&NAME);

#define SLIB_SAFE_STATIC_GETTER(TYPE, FUNC, ...) \
	static TYPE& FUNC() { \
		SLIB_SAFE_STATIC(TYPE, ret, ##__VA_ARGS__) \
		return ret; \
	}

#define SLIB_SAFE_STATIC_REF(TYPE, NAME) \
	static void* _g_globalref_##NAME = 0; \
	TYPE& NAME = *((TYPE*)&_g_globalref_##NAME); \
	static slib::SafeStaticDestructor<TYPE> _static_destructor_instance_##NAME(&NAME);



#define SLIB_PROPERTY_INLINE(TYPE, NAME) protected: \
	TYPE _m_property_##NAME; \
public: \
	SLIB_INLINE TYPE const& get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(TYPE const& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_BOOLEAN_PROPERTY_INLINE(NAME) protected: \
	sl_bool _m_property_##NAME; \
public: \
	SLIB_INLINE sl_bool is##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(sl_bool v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_WEAK_PROPERTY_INLINE(TYPE, NAME) protected: \
	slib::SafeWeakRef<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_REF_PROPERTY_INLINE(TYPE, NAME) protected: \
	slib::SafeRef<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_PROPERTY(TYPE, NAME) protected: \
	TYPE _m_property_##NAME; \
public: \
	virtual TYPE const& get##NAME() const { \
		return _m_property_##NAME; \
	} \
	virtual void set##NAME(TYPE const& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_BOOLEAN_PROPERTY(NAME) protected: \
	sl_bool _m_property_##NAME; \
public: \
	virtual sl_bool is##NAME() const { \
		return _m_property_##NAME; \
	} \
	virtual void set##NAME(sl_bool v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_WEAK_PROPERTY(TYPE, NAME) protected: \
	slib::SafeWeakRef<TYPE> _m_property_##NAME; \
public: \
	virtual slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	virtual void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_REF_PROPERTY(TYPE, NAME) protected: \
	slib::SafeRef<TYPE> _m_property_##NAME; \
public: \
	virtual slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	virtual void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}



#endif
